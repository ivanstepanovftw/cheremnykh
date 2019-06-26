#include <iostream>
#include <chrono>
#include <fstream>
#include <random>
#include <filesystem>
#include "../common.hh"

template <class T>
size_t generate(const std::filesystem::path& p, size_t size) {
    if (std::ofstream o{p}) {
        std::mt19937_64 r(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<T> dis(T(0), std::numeric_limits<T>::max());

        while (o.tellp() < size) {
            for(size_t i=0; i<10; i++) {
                o<<dis(r)<<'\n';
            }
        }
        if (ssize_t ret = o.tellp(); ret != -1)
            return ret;
        else
            return 0;
    }
    return 0;
}

int main(int argc, char**argv) {
    using namespace std;

    using target_type = double;
    std::filesystem::path target_file = "a.txt";
    // constexpr size_t target_size = 1u<<30u;
    constexpr size_t target_size = 1u<<20u;

    if (!std::filesystem::exists(target_file) || true) {
        Timer t("generate");
        size_t s = generate<target_type>(target_file, target_size);
        std::clog<<"generate: file: "<<target_file<<", size: "<<s<<std::endl;
    }
    /*
     * example output:
     * generate: file: "a.txt", size: 1073742804
     * generate: done in 54.9419 seconds
     */

    return 0;
}