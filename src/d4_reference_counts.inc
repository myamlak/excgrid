// The Gaussian counts N^s of the model's reference weighting, equation 8:
// one entry per reference state, in the order of kElementReference (element
// order, and within an element the source's own reference order).  A count is
// the number of Gaussian functions the weighting sum of that reference state
// runs over.
//
// THESE NUMBERS ARE NOT PUBLISHED BY THE AUTHORS OF THE MODEL.  The paper
// states the rule for choosing them and prints no value for any of them (see
// THIRD_PARTY_NOTICES.md), so the entries below are this project's own and no
// reader should cite them as the authors' values.
//
// They are not fitted values either, and the distinction matters more here
// than anywhere else in this table: the counts are the one term of the chain
// that could absorb a residual.  Over every uniform count from one to a
// million the two-body energy of the caffeine fixture stays inside 0.997913 to
// 0.999281 of its published value, and a mixture the same family admits
// reaches the published value itself.  So the reach is there, and the entries
// below are nevertheless the model's neutral baseline - one Gaussian function
// per reference state, the count the paper's Figure 4 gives the well-separated
// case - because a vector chosen from that family is a fit to the fixture and
// not a value of the model.  The baseline leaves 0.2 per cent, and the family
// could cover that 0.2 per cent by construction; what it cannot do is explain
// it, which is why the baseline is stated rather than resolved.
// tests/d4_test.cpp pins the baseline and measures what the family reaches.

#include <array>
#include <cstddef>

namespace excgrid::d4tables {

inline constexpr std::array<int, 261> kReferenceGaussianCount = {
    // Z = 1
    1, 1,
    // Z = 2
    1,
    // Z = 3
    1, 1, 1,
    // Z = 4
    1, 1, 1, 1,
    // Z = 5
    1, 1, 1, 1, 1,
    // Z = 6
    1, 1, 1, 1, 1, 1, 1,
    // Z = 7
    1, 1, 1, 1, 1,
    // Z = 8
    1, 1, 1, 1,
    // Z = 9
    1, 1,
    // Z = 10
    1,
    // Z = 11
    1, 1, 1,
    // Z = 12
    1, 1, 1, 1,
    // Z = 13
    1, 1, 1, 1,
    // Z = 14
    1, 1, 1, 1, 1,
    // Z = 15
    1, 1, 1, 1,
    // Z = 16
    1, 1, 1,
    // Z = 17
    1, 1,
    // Z = 18
    1,
    // Z = 19
    1, 1, 1,
    // Z = 20
    1, 1, 1, 1,
    // Z = 21
    1, 1, 1, 1,
    // Z = 22
    1, 1, 1, 1,
    // Z = 23
    1, 1, 1, 1,
    // Z = 24
    1, 1, 1, 1,
    // Z = 25
    1, 1, 1,
    // Z = 26
    1, 1, 1,
    // Z = 27
    1, 1, 1, 1,
    // Z = 28
    1, 1, 1, 1,
    // Z = 29
    1, 1,
    // Z = 30
    1, 1,
    // Z = 31
    1, 1, 1,
    // Z = 32
    1, 1, 1, 1, 1,
    // Z = 33
    1, 1, 1, 1,
    // Z = 34
    1, 1, 1,
    // Z = 35
    1, 1,
    // Z = 36
    1,
    // Z = 37
    1, 1, 1,
    // Z = 38
    1, 1, 1, 1,
    // Z = 39
    1, 1, 1,
    // Z = 40
    1, 1, 1, 1,
    // Z = 41
    1, 1, 1, 1,
    // Z = 42
    1, 1, 1, 1,
    // Z = 43
    1, 1, 1,
    // Z = 44
    1, 1, 1,
    // Z = 45
    1, 1, 1, 1,
    // Z = 46
    1, 1, 1,
    // Z = 47
    1, 1,
    // Z = 48
    1, 1,
    // Z = 49
    1, 1, 1, 1,
    // Z = 50
    1, 1, 1, 1, 1,
    // Z = 51
    1, 1, 1, 1,
    // Z = 52
    1, 1, 1,
    // Z = 53
    1, 1,
    // Z = 54
    1,
    // Z = 55
    1, 1, 1,
    // Z = 56
    1, 1, 1, 1,
    // Z = 57
    1, 1, 1,
    // Z = 58
    1,
    // Z = 59
    1, 1,
    // Z = 60
    1, 1,
    // Z = 61
    1, 1,
    // Z = 62
    1, 1,
    // Z = 63
    1, 1,
    // Z = 64
    1, 1,
    // Z = 65
    1, 1,
    // Z = 66
    1, 1,
    // Z = 67
    1, 1,
    // Z = 68
    1, 1,
    // Z = 69
    1, 1,
    // Z = 70
    1, 1,
    // Z = 71
    1, 1,
    // Z = 72
    1, 1, 1, 1,
    // Z = 73
    1, 1, 1, 1,
    // Z = 74
    1, 1, 1,
    // Z = 75
    1, 1, 1,
    // Z = 76
    1, 1, 1,
    // Z = 77
    1, 1, 1, 1, 1,
    // Z = 78
    1, 1, 1,
    // Z = 79
    1, 1,
    // Z = 80
    1, 1,
    // Z = 81
    1, 1, 1, 1,
    // Z = 82
    1, 1, 1, 1, 1,
    // Z = 83
    1, 1, 1, 1,
    // Z = 84
    1, 1, 1,
    // Z = 85
    1, 1,
};

} // namespace excgrid::d4tables
