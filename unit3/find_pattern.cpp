#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <tuple>


/*!
 * find pattern in string based on mask. '?' symbol in 'mask' matches any character.
 */
size_t find_pattern(const std::string& in, const std::string& pattern, const std::string& mask) {
    const size_t len = pattern.size();
    if (len != mask.size())
        throw std::runtime_error("find_pattern: pattern.size() != mask.size()");

    for (size_t b = 0; b < in.size(); b++) {
        size_t matches = 0;

        while (in[b + matches] == pattern[matches] || mask[matches] == '?') {
            matches++;

            if (matches == len) {
                return b;
            }
        }
    }
    return std::string::npos;
}


int main() {
    // [(0: haystack, 1: pattern, 2: mask, 3: expected output), ...]
    using test_t = std::vector<std::tuple<std::string, std::string, std::string, size_t>>;
    test_t test_cases;
    test_cases.emplace_back("", "", "", std::string::npos);
    test_cases.emplace_back("my_apple",  "apple", "*****", 3);
    test_cases.emplace_back("my_apple",  "  ple", "??***", 3);
    test_cases.emplace_back("my_banana", "apple", "*****", std::string::npos);
    test_cases.emplace_back("one two three four", " ????? ", "*?????*", 7);
    test_cases.emplace_back("match after two  spaces", "  ", "**", 15);
    test_cases.emplace_back("??**??**", "??**??**", "********", 0);


    size_t passed = 0;
    for (const auto& i : test_cases) {
        const std::string& haystack = std::get<0>(i);
        const std::string& pattern = std::get<1>(i);
        const std::string& mask = std::get<2>(i);
        const size_t& should_return = std::get<3>(i);
        const size_t match = find_pattern(haystack, pattern, mask);
        if (should_return != match) {
            std::cout<<"haystack: "<<haystack<<", pattern: "<<pattern<<", mask: "<<mask<<", should_return: "<<should_return
                     <<", match: "<<match<<std::endl;
        } else
            passed++;
    }

    std::cout<<"Passed "<<passed<<" tests out of "<<test_cases.size()<<std::endl;
}
