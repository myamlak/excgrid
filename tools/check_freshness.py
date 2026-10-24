#!/usr/bin/env python3
"""The codegen freshness gate: the .ey and .ys sources are unchanged since the blessing.

Cheap enough for a commit hook.  It does not prove the committed generated code
matches its sources - `python tools/regenerate.py --check` does that.

Usage:
    python tools/check_freshness.py            # verify; exit 1 on any drift
    python tools/check_freshness.py --bless    # rewrite the manifest
"""

import argparse
import hashlib
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEFS_DIR = ROOT / "xc_defs"
MANIFEST = DEFS_DIR / "freshness.json"

# Codegen input suffixes; a new input missing from the manifest fails the gate.
SOURCE_SUFFIXES = (".ey", ".ys")

NOTE = (
    "Codegen input hashes for the freshness gate. Written by tools/regenerate.py after a "
    "successful regeneration, checked by tools/check_freshness.py at commit time. This proves "
    "the sources are unchanged since the blessing; it does NOT prove the committed generated "
    "code matches them - that is what `python tools/regenerate.py --check` proves, which costs "
    "a full Yacas expansion and is therefore a deliberate run, not a hook."
)


def source_files():
    """Every codegen input, in a stable order."""
    return sorted(path for path in DEFS_DIR.iterdir() if path.suffix in SOURCE_SUFFIXES)


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def current_sources():
    return {path.name: digest(path) for path in source_files()}


def write_manifest():
    """Writes the manifest from the current sources; returns the entry count."""
    payload = {"note": NOTE, "sources": current_sources()}
    MANIFEST.write_text(json.dumps(payload, indent=2, sort_keys=True) + "\n",
                        encoding="utf-8", newline="\n")
    return len(payload["sources"])


def check():
    if not MANIFEST.exists():
        print(f"freshness: no manifest at {MANIFEST.relative_to(ROOT)}")
        print("freshness: run `python tools/check_freshness.py --bless` once, after a "
              "`regenerate.py --check` run has confirmed the tree")
        return 1

    try:
        expected = json.loads(MANIFEST.read_text(encoding="utf-8"))["sources"]
    except (json.JSONDecodeError, KeyError) as error:
        print(f"freshness: {MANIFEST.relative_to(ROOT)} is unreadable: {error}")
        return 1

    actual = current_sources()
    changed = sorted(name for name in expected if name in actual and actual[name] != expected[name])
    missing = sorted(name for name in expected if name not in actual)
    added = sorted(name for name in actual if name not in expected)

    if not (changed or missing or added):
        print(f"freshness OK: {len(actual)} codegen input(s) unchanged since the blessing")
        return 0

    for name in changed:
        print(f"freshness: CHANGED  {name}")
    for name in missing:
        print(f"freshness: MISSING  {name}")
    for name in added:
        print(f"freshness: NEW      {name}")

    print(f"freshness FAILED: {len(changed)} changed, {len(missing)} missing, {len(added)} new")
    print("freshness: run `python tools/regenerate.py` (from excgrid/) and commit the "
          "regenerated files; it rewrites the manifest on success")
    return 1


def main():
    parser = argparse.ArgumentParser(description="excgrid codegen freshness gate")
    parser.add_argument("--bless", action="store_true",
                        help="rewrite the manifest from the current sources")
    args = parser.parse_args()

    if args.bless:
        count = write_manifest()
        print(f"freshness: blessed {count} codegen input(s) into "
              f"{MANIFEST.relative_to(ROOT)}")
        print("freshness: this asserts the sources are unchanged FROM NOW ON; it does not "
              "verify they produced the committed generated code - run "
              "`python tools/regenerate.py --check` for that")
        return 0

    return check()


if __name__ == "__main__":
    sys.exit(main())
