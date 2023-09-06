#pragma once

#include <string>
#include <vector>

namespace edit_distance
{
    // Returns the optimal edits to go from 'left' to 'right'.
    // All edits cost the same, with replace having lower priority than
    // add/remove.
    // Simple implementation of the Wagner-Fischer algorithm.
    // See http://en.wikipedia.org/wiki/Wagner-Fischer_algorithm
    enum EditType { kMatch, kAdd, kRemove, kReplace };

    std::vector<EditType> CalculateOptimalEdits(const std::vector<size_t>& left, const std::vector<size_t>& right);

    // Same as above, but the input is represented as strings.
    std::vector<EditType> CalculateOptimalEdits(const std::vector<std::string>& left,
                                                const std::vector<std::string>& right);

    // Create a diff of the input strings in Unified diff format.
    std::string CreateUnifiedDiff(const std::vector<std::string>& left, const std::vector<std::string>& right,
                                  size_t context = 2);

}  // namespace edit_distance
