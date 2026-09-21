#!/usr/bin/env python3
"""Generate src/d4_reference.inc from the published D4 reference-state data.

The C6 step of the DFT-D4 model - Caldeweyher, Ehlert, Hansen, Neugebauer,
Spicher, Bannwarth, Grimme, J. Chem. Phys. 150 (2019) 154122 - interpolates
over the element-specific reference systems of each atom (its equations 7
and 8).  Those systems are the paper's own but are not tabulated there; the
numeric values are carried by the reference implementation's generated data,
as the model's other D4 data already is.

What each reference system carries is the polarizability of a whole MOLECULE
A_m X_n of m equivalent atoms A and n atoms X, not of the atom A.  The
paper's equation 5 turns the one into the other, dividing by m and
subtracting the n atoms X, so a usable table needs m, n, the charge of X,
and which reference system of element X to take that atom's polarizability
from.  The data carries all four; this script transcribes them beside the
polarizabilities, and checks m and n against the formula the data names the
reference system by.

This script transcribes NUMBERS ONLY: per element and per reference state,
the reference system the state is taken from, its reference coordination
number, its reference charge, the atom counts and charges equation 5 needs,
and the 23 isotropically averaged dynamic polarizabilities alpha(iw) of the
molecule.  No statement, comment, structure or expression of the source is
reproduced; the output is this project's own table in this project's own
format.

Usage:  python tools/gen_d4_reference.py            # writes src/d4_reference.inc
        python tools/gen_d4_reference.py --check    # verifies the committed file
"""

import argparse
import pathlib
import re
import sys
import urllib.request

SOURCE = "https://raw.githubusercontent.com/dftd4/dftd4/main/src/dftd4/reference.inc"
OUTPUT = pathlib.Path(__file__).resolve().parent.parent / "src" / "d4_reference.inc"

MAX_ELEMENT = 86
N_FREQUENCIES = 23

SYMBOLS = ("H He Li Be B C N O F Ne Na Mg Al Si P S Cl Ar K Ca Sc Ti V Cr Mn Fe Co Ni Cu Zn "
           "Ga Ge As Se Br Kr Rb Sr Y Zr Nb Mo Tc Ru Rh Pd Ag Cd In Sn Sb Te I Xe Cs Ba La Ce "
           "Pr Nd Pm Sm Eu Gd Tb Dy Ho Er Tm Yb Lu Hf Ta W Re Os Ir Pt Au Hg Tl Pb Bi Po At Rn "
           "Fr Ra Ac Th Pa U Np Pu Am Cm Bk Cf Es Fm Md No Lr Rf Db Sg Bh Hs Mt Ds Rg Cn Nh Fl "
           "Mc Lv Ts Og").split()
Z_OF_SYMBOL = {symbol: index + 1 for index, symbol in enumerate(SYMBOLS)}


def fetch(url):
    with urllib.request.urlopen(url, timeout=120) as response:
        return response.read().decode("utf-8", errors="replace")


def scalars(text, name):
    # The source leaves the kind suffix off two of its entries, so the
    # number is read with or without it.
    pattern = r"data\s+%s\s*\(\s*(\d+)\s*,\s*(\d+)\s*\)\s*/\s*([-\d.eE+]+)(?:_wp)?" % name
    return {(int(m.group(1)), int(m.group(2))): float(m.group(3))
            for m in re.finditer(pattern, text)}


def systems(text):
    pattern = r"data\s+refsys\s*\(\s*(\d+)\s*,\s*(\d+)\s*\)\s*/\s*(\d+)"
    return {(int(m.group(1)), int(m.group(2))): int(m.group(3))
            for m in re.finditer(pattern, text)}


def alphas(text):
    pattern = r"data\s+alphaiw\s*\(:,(\d+),\s*(\d+)\)\s*/(.*?)/"
    out = {}

    for m in re.finditer(pattern, text, re.S):
        values = [float(v) for v in re.findall(r"([-\d.]+)_wp", m.group(3))]
        if len(values) != N_FREQUENCIES:
            raise SystemExit("state (%s,%s): %d frequencies, expected %d"
                             % (m.group(1), m.group(2), len(values), N_FREQUENCIES))
        out[(int(m.group(1)), int(m.group(2)))] = values

    return out


def names(text):
    """Each state's reference system, by the formula the data names it by."""
    out = {}
    current = None

    for line in text.split("\n"):
        header = re.match(r"\s*!\s*REF\s+(\S+)", line)
        if header:
            current = header.group(1)

        state = re.match(r"\s*data\s+alphaiw\s*\(:,\s*(\d+),\s*(\d+)\)", line)
        if state and current is not None:
            out[(int(state.group(1)), int(state.group(2)))] = current

    return out


def formula_atoms(name):
    """The atom counts a reference system's formula name states."""
    return {symbol: int(count or 1)
            for symbol, count in re.findall(r"([A-Z][a-z]?)(\d*)", name) if symbol}


def only_other(atoms, symbol):
    others = [other for other in atoms if other != symbol]
    if len(others) != 1:
        return None
    return others[0]


def secondary(text):
    """The secondary reference systems, by their own index.

    Each is where the subtraction of equation 5 takes one atom's
    polarizability from: the molecule's own charge, that atom's share of the
    molecule's polarizability, and its polarizabilities over the grid.
    """
    charge = {int(m.group(1)): float(m.group(2))
              for m in re.finditer(r"data\s+secq\s*\(\s*(\d+)\s*\)\s*/\s*([-\d.eE+]+)(?:_wp)?", text)}
    scale = {int(m.group(1)): float(m.group(2))
             for m in re.finditer(r"data\s+sscale\s*\(\s*(\d+)\s*\)\s*/\s*([-\d.eE+]+)(?:_wp)?", text)}
    alpha = {}

    for m in re.finditer(r"data\s+secaiw\s*\(:,\s*(\d+)\)\s*/(.*?)/", text, re.S):
        values = [float(v) for v in re.findall(r"([-\d.]+)_wp", m.group(2))]
        if len(values) != N_FREQUENCIES:
            raise SystemExit("secondary system %s: %d frequencies, expected %d"
                             % (m.group(1), len(values), N_FREQUENCIES))
        alpha[int(m.group(1))] = values

    out = []

    for index in sorted(alpha):
        if index not in charge or index not in scale:
            raise SystemExit("secondary system %d carries no charge or scale" % index)
        out.append({"index": index, "charge": charge[index], "scale": scale[index],
                    "alpha": alpha[index]})

    return out


def parse(text):
    # The source carries six charge sets per state.  The model applies the
    # classical one: its own reference-charge and reference-alpha routines of
    # the EEQ scheme both read the `cls` columns, and the paper names the
    # descriptor it uses "classical EEQ type partial charges".  The other
    # sets - the GFN-FF, DFT, PBC and EEQ-BC charges, and the `ref` pair -
    # are carried by the source for schemes this library does not ship, and
    # the published per-atom polarizabilities of the model's own test set
    # select the classical pair against every other one: 0.1 per cent root
    # mean square against 2.8 for the `ref` pair.
    charge = scalars(text, "clsq")
    cn = scalars(text, "refcovcn")
    system = systems(text)
    alpha = alphas(text)
    name = names(text)
    share = scalars(text, "ascale")
    hcount = scalars(text, "hcount")
    xcharge = scalars(text, "clsh")

    for table, label in ((share, "ascale"), (hcount, "hcount"), (xcharge, "clsh")):
        missing = sorted(state for state in alpha if state not in table)
        if missing:
            raise SystemExit("%s is missing for %s" % (label, missing[:4]))

    elements = []

    for z in range(1, MAX_ELEMENT + 1):
        states = sorted(i for (i, zz) in cn if zz == z)
        if not states:
            raise SystemExit("element %d carries no reference states" % z)
        if states != list(range(1, len(states) + 1)):
            raise SystemExit("element %d reference indices are not 1..n" % z)

        # The published data is not complete over its own element range: its
        # last element carries reference coordination numbers but no reference
        # polarizabilities, so it has nothing for the C6 step to interpolate.
        # The table stops where the data does.
        if any((i, z) not in alpha for i in states):
            if z != MAX_ELEMENT:
                raise SystemExit("element %d is incomplete but is not the last" % z)
            break

        rows = []
        for i in states:
            atoms = formula_atoms(name[(i, z)])
            symbol = SYMBOLS[z - 1]
            if symbol not in atoms:
                raise SystemExit("state (%d,%d) is named %s, which holds no %s"
                                 % (i, z, name[(i, z)], symbol))

            own = atoms[symbol]
            others = sum(count for other, count in atoms.items() if other != symbol)

            # The share the data gives is the reciprocal of the atom count of
            # the element in the named molecule, and the count it gives is the
            # number of the molecule's other atoms; anything else would mean
            # the name and the numbers disagree.
            if abs(share[(i, z)] * own - 1.0) > 1e-6:
                raise SystemExit("state (%d,%d) is named %s with %d %s but its share is %.8f"
                                 % (i, z, name[(i, z)], own, symbol, share[(i, z)]))

            other = only_other(atoms, symbol)

            # The count of atoms to subtract is the molecule's other atom
            # count wherever one element makes up the rest of it.  It is
            # carried as published rather than enforced, and a row where the
            # name and the count disagree says so in the generated file.
            note = ""

            if others == 0:
                if abs(hcount[(i, z)]) > 1e-9:
                    note = " (published count %.4f of no other atom)" % hcount[(i, z)]
            elif other is not None and abs(hcount[(i, z)] - others) > 1e-9:
                note = " (published count %.4f of %d)" % (hcount[(i, z)], others)

            rows.append({
                "system": system[(i, z)],
                "cn": cn[(i, z)],
                "charge": charge[(i, z)],
                "alpha": alpha[(i, z)],
                "name": name[(i, z)],
                "scale": share[(i, z)],
                "xCount": hcount[(i, z)],
                "xCharge": xcharge[(i, z)],
                "xElement": Z_OF_SYMBOL[other] if other is not None else 0,
                "note": note,
            })
        elements.append(rows)

    return elements


def emit(elements, refs):
    out = []
    add = out.append
    add("// Generated by tools/gen_d4_reference.py from the published DFT-D4")
    add("// reference-state data; do not edit by hand.  Numeric values only, from")
    add("// Caldeweyher, Ehlert, Hansen, Neugebauer, Spicher, Bannwarth, Grimme,")
    add("// J. Chem. Phys. 150 (2019) 154122 (the model), as carried by the")
    add("// reference implementation's generated data (see THIRD_PARTY_NOTICES.md).")
    add("//")
    add("// Self-sufficient: clang-format's include sort may place this file ahead")
    add("// of the consumer's headers, so it carries its own includes.")
    add("")
    add("#include <array>")
    add("#include <cstddef>")
    add("")
    add("namespace excgrid::d4tables {")
    add("")
    add("// The number of frequencies each reference polarizability is given at,")
    add("// which is the model's Casimir-Polder grid size.")
    add("constexpr std::size_t kReferenceFrequencies = %d;" % N_FREQUENCIES)
    add("")
    add("// The largest atomic number the published reference data covers: its")
    add("// last element has no reference polarizabilities, so the table stops")
    add("// one short of the parameter tables' own range.")
    add("constexpr int kReferenceMaxElement = %d;" % len(elements))
    add("")
    add("// One reference system of an element, as the published data carries it:")
    add("// the molecule a state of the element is drawn from, and the terms the")
    add("// model's equation 5 needs to reduce that molecule's polarizability to")
    add("// the element's own atom.  The polarizabilities are the MOLECULE's, so")
    add("// they are m times too large until the reduction is applied.")
    add("struct ElementReference {")
    add("    int system; ///< The reference system the state is taken from.")
    add("    double coordinationNumber; ///< Its reference coordination number.")
    add("    double charge; ///< Its reference partial charge.")
    add("    int xElement; ///< The other element of the reference molecule, 0 for none.")
    add("    double xScale; ///< That element's share of the molecule, its own 1/m.")
    add("    double xCount; ///< How many of its atoms the molecule carries.")
    add("    double xCharge; ///< The charge each of those atoms carries in it.")
    add("    double xReferenceCharge; ///< Its charge in its own reference system.")
    add("    std::array<double, kReferenceFrequencies> xAlpha; ///< Its polarizabilities there.")
    add("    double scale; ///< This element's own share of the molecule, 1/m.")
    add("    std::array<double, kReferenceFrequencies> alpha; ///< The molecule's alpha(iw).")
    add("};")
    add("")
    add("// The reference states of each element, flat: element z's row of")
    add("// kReferenceCount[z] states starts at kReferenceOffset[z].")
    counts = ", ".join(str(len(rows)) for rows in elements)
    add("inline constexpr int kReferenceCount[] = {%s};" % counts)
    add("")

    offsets = []
    running = 0
    for rows in elements:
        offsets.append(running)
        running += len(rows)
    add("inline constexpr int kReferenceOffset[] = {%s};" % ", ".join(str(o) for o in offsets))
    add("")
    add("// One entry per element and reference state, in element order and, within")
    add("// an element, in the source's own reference order.")
    add("inline constexpr ElementReference kElementReference[] = {")
    for z, rows in enumerate(elements, start=1):
        for i, r in enumerate(rows, start=1):
            values = ", ".join("%.7f" % v for v in r["alpha"])
            partner = next(entry for entry in refs if entry["index"] == r["system"])
            add("    // Z = %d, reference %d: %s%s" % (z, i, r["name"], r["note"]))
            add("    {%d, %.7f, %.7f, %d, %.8f, %.7f, %.8f, %.8f," % (
                r["system"], r["cn"], r["charge"], r["xElement"], partner["scale"],
                r["xCount"], r["xCharge"], partner["charge"]))
            xvalues = ", ".join("%.7f" % v for v in partner["alpha"])
            add("     {")
            add("         %s," % xvalues)
            add("     },")
            add("     %.8f, {" % r["scale"])
            add("         %s," % values)
            add("     }},")
    add("};")
    add("")
    add("} // namespace excgrid::d4tables")
    add("")
    return "\n".join(out)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true",
                        help="verify the committed file instead of writing it")
    parser.add_argument("--source", default=SOURCE,
                        help="override the source URL (for offline re-runs)")
    args = parser.parse_args()

    text = fetch(args.source)
    rendered = emit(parse(text), secondary(text))

    if args.check:
        committed = OUTPUT.read_text(encoding="utf-8")
        if committed != rendered:
            sys.exit("d4_reference.inc is stale; re-run without --check")
        print("d4_reference.inc is current")
        return

    with open(OUTPUT, "w", encoding="utf-8", newline="\n") as handle:
        handle.write(rendered)
    print("wrote %s" % OUTPUT)


if __name__ == "__main__":
    main()
