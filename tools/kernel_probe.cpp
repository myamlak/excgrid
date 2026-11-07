// The kernel probe: a tiny CLI the verification scripts drive.  Reads lines
// "<name> rhoA rhoB sigmaAa sigmaAb sigmaBb [tauA tauB]" on stdin, prints one
// line "exc vrhoA vrhoB vsigmaAa vsigmaAb vsigmaBb vtauA vtauB" per input
// line.  The tau fields are optional: a GGA point keeps its five-value form and
// the tau derivatives print as zero.  Built alongside the library (never
// installed); consumed by tools/verify_pyscf.py.

#include "excgrid/kernel.hpp"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::string line;

    while (std::getline(std::cin, line))
    {
        std::istringstream fields(line);
        std::string name;

        if (!(fields >> name))
        {
            continue;
        }

        double rhoA = 0.0;
        double rhoB = 0.0;
        double sigmaAa = 0.0;
        double sigmaAb = 0.0;
        double sigmaBb = 0.0;
        double tauA = 0.0;
        double tauB = 0.0;

        if (!(fields >> rhoA >> rhoB >> sigmaAa >> sigmaAb >> sigmaBb))
        {
            std::printf("malformed functional point: %s\n", line.c_str());
            continue;
        }

        // Both or neither: half a tau pair is not a point this probe will guess at.
        const bool withTau = static_cast<bool>(fields >> tauA >> tauB);

        const excgrid::XcFunctional* functional = excgrid::FindFunctional(name);

        if (functional == nullptr)
        {
            std::printf("unknown functional %s\n", name.c_str());
            continue;
        }

        excgrid::PointInputs inputs;
        inputs.Set(excgrid::Component::RhoA, rhoA);
        inputs.Set(excgrid::Component::RhoB, rhoB);
        inputs.Set(excgrid::Component::SigmaAa, sigmaAa);
        inputs.Set(excgrid::Component::SigmaAb, sigmaAb);
        inputs.Set(excgrid::Component::SigmaBb, sigmaBb);

        if (withTau)
        {
            inputs.Set(excgrid::Component::TauA, tauA);
            inputs.Set(excgrid::Component::TauB, tauB);
        }

        excgrid::PointResult result;

        if (functional->EvaluatePoint(inputs, result) != excgrid::KernelStatus::kOk)
        {
            std::printf("refused %s\n", name.c_str());
            continue;
        }

        std::printf("%.16e %.16e %.16e %.16e %.16e %.16e %.16e %.16e\n",
                    result.exc,
                    result.first[excgrid::IndexOf(excgrid::Component::RhoA)],
                    result.first[excgrid::IndexOf(excgrid::Component::RhoB)],
                    result.first[excgrid::IndexOf(excgrid::Component::SigmaAa)],
                    result.first[excgrid::IndexOf(excgrid::Component::SigmaAb)],
                    result.first[excgrid::IndexOf(excgrid::Component::SigmaBb)],
                    result.first[excgrid::IndexOf(excgrid::Component::TauA)],
                    result.first[excgrid::IndexOf(excgrid::Component::TauB)]);
    }

    return 0;
}
