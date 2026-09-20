# excgrid

The grid + XC-kernel + D3 library: molecular block-grid construction
(Murray-Handy-Laming radial, Lebedev-Laikov angular, Becke/SSF
partition), the per-point LDA/GGA/hybrid kernel family (energy density +
first derivatives, Yacas-code-generated from the committed `xc_defs/*.ey`
sources), and the Grimme D3 zero-damping dispersion with analytic
gradients.  The frozen boundary contract is `docs/kernel-api.md`.

## References

The formulas and data the kernels implement:

- Slater exchange: J. C. Slater, *A simplification of the Hartree-Fock
  method*, Phys. Rev. 81 (1951) 385.
- VWN correlation (VWN3 = RPA parameters, VWN5 = interpolation V):
  S. H. Vosko, L. Wilk, M. Nusair, *Accurate spin-dependent electron
  liquid correlation energies for local spin density calculations*,
  Can. J. Phys. 58 (1980) 1200.
- PW92 correlation: J. P. Perdew, Y. Wang, *Accurate and simple analytic
  representation of the electron-gas correlation energy*, Phys. Rev. B
  45 (1992) 13244.
- Becke88 exchange: A. D. Becke, *Density-functional exchange-energy
  approximation with correct asymptotic behavior*, Phys. Rev. A 38
  (1988) 3098.
- PW91 exchange/correlation: J. P. Perdew, Y. Wang, *Accurate and simple
  density functional for the electronic exchange energy: Generalized
  gradient approximation*, Phys. Rev. B 33 (1986) 8800; J. P. Perdew, in
  *Electronic Structure of Solids '91*.
- PBE exchange/correlation: J. P. Perdew, K. Burke, M. Ernzerhof,
  *Generalized gradient approximation made simple*, Phys. Rev. Lett. 77
  (1996) 3865.
- revPBE: Y. Zhang, W. Yang, *Comment on "Generalized gradient
  approximation made simple"*, Phys. Rev. Lett. 80 (1998) 890.
- RPBE: B. Hammer, L. B. Hansen, J. K. Norskov, *Improved adsorption
  energetics within density functional theory using revised PBE
  functionals*, Phys. Rev. B 59 (1999) 7413.
- PBEsol: J. P. Perdew et al., *Restoring the density-gradient expansion
  for exchange in solids and surfaces*, Phys. Rev. Lett. 100 (2008)
  136406.
- mPW91: C. Adamo, V. Barone, *Exchange functionals with improved
  long-range behavior and adiabatic connection methods without
  adjustable parameters*, J. Chem. Phys. 108 (1998) 664.
- LYP: C. Lee, W. Yang, R. G. Parr, *Development of the Colle-Salvetti
  correlation-energy formula into a functional of the electron density*,
  Phys. Rev. B 37 (1988) 785.
- P86: J. P. Perdew, *Density-functional approximation for the
  correlation energy of the inhomogeneous electron gas*, Phys. Rev. B 33
  (1986) 8822 (R), with the Perdew-Zunger 1981 LDA piece (Phys. Rev. B
  23, 5048).
- B3LYP: A. D. Becke, J. Chem. Phys. 98 (1993) 5648; P. J. Stephens et
  al., J. Phys. Chem. 98 (1994) 11623.
- PBE0: C. Adamo, V. Barone, J. Chem. Phys. 110 (1999) 6158.
- Radial quadrature: C. W. Murray, N. C. Handy, G. J. Laming, Mol. Phys.
  78 (1993) 997.
- Lebedev-Laikov quadrature: V. I. Lebedev, V. I. Laikov, Dokl. Math. 59
  (1999) 477.
- Becke partition: A. D. Becke, J. Chem. Phys. 88 (1988) 2547, with the
  SSF step (Stratmann, Scuseria, Frisch, Chem. Phys. Lett. 257 (1996)
  213) and Bragg-Slater radii (J. Chem. Phys. 41 (1964) 3199).
- D3 dispersion: S. Grimme, J. Antony, S. Ehrlich, H. Krieg, *A consistent
  and accurate ab initio parametrization of density functional dispersion
  correction (DFT-D) for the 94 elements H-Pu*, J. Chem. Phys. 132 (2010)
  154104.
