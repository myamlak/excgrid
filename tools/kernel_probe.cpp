// The kernel probe: a tiny CLI the verification scripts drive.  Reads
// lines "<name> rhoA rhoB sigmaAa sigmaAb sigmaBb" on stdin, prints one
// line "exc vrhoA vrhoB vsigmaAa vsigmaAb vsigmaBb" per input line.
// Built alongside the library (never installed); consumed by
// tools/verify_pyscf.py.

#include "excgrid/kernel.hpp"

#include <cstdio>
#include <iostream>
#include <string>

int main() {
    std::string name;
    double rhoA = 0.0;
    double rhoB = 0.0;
    double sigmaAa = 0.0;
    double sigmaAb = 0.0;
    double sigmaBb = 0.0;

    while (std::cin >> name >> rhoA >> rhoB >> sigmaAa >> sigmaAb >> sigmaBb)
    {
        const excgrid::XcFunctional* functional = excgrid::FindFunctional(name);

        if (functional == nullptr)
        {
            std::printf("unknown functional %s\n", name.c_str());
            continue;
        }

        const excgrid::XcKernelValue value =
            functional->Evaluate(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb);

        std::printf("%.16e %.16e %.16e %.16e %.16e %.16e\n",
                    value.exc,
                    value.vrhoA,
                    value.vrhoB,
                    value.vsigmaAa,
                    value.vsigmaAb,
                    value.vsigmaBb);
    }

    return 0;
}
