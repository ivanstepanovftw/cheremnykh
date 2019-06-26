/*
 * Usage: ./sort unsorted_file sorted_file
 *
 * Example output for 64 MiB chunk_mem_limit:
 * chunk0: done in 1.69231 seconds
 * chunk1: done in 1.68235 seconds
 * chunk2: done in 1.76075 seconds
 * chunk3: done in 1.80372 seconds
 * chunk4: done in 1.75653 seconds
 * chunk5: done in 1.80459 seconds
 * chunk6: done in 1.78352 seconds
 * chunk7: done in 1.72759 seconds
 * chunk8: done in 1.66968 seconds
 * chunk9: done in 1.54443 seconds
 * chunks: done in 17.2256 seconds
 * merge_sort: done in 3.84935 seconds
 * bin to string: done in 51.4071 seconds
 * main: done in 72.4942 seconds
 */
#include <iostream>
#include <chrono>
#include <fstream>
#include <random>
#include <filesystem>
#include <boost/iostreams/device/mapped_file.hpp>
#include "../common.hh"


template <class T>
bool merge_sort(const std::filesystem::path& in1, const std::filesystem::path& in2, const std::string& s) {
    boost::iostreams::mapped_file mf1(in1);
    boost::iostreams::mapped_file mf2(in2);
    if (std::ofstream fo{s}) {
        auto *v1_beg = reinterpret_cast<T *>(mf1.data());
        auto *v1_end = reinterpret_cast<T *>(mf1.data() + mf1.size());
        auto *v2_beg = reinterpret_cast<T *>(mf2.data());
        auto *v2_end = reinterpret_cast<T *>(mf2.data() + mf2.size());
        auto *v1_cur = v1_beg;
        auto *v2_cur = v2_beg;
        for (; v1_cur < v1_end && v2_cur < v2_end;) {
            if (*v1_cur <= *v2_cur) {
                fo.write((char *)(&*v1_cur), sizeof(T));
                v1_cur++;
            }
            else {
                fo.write((char *)(&*v2_cur), sizeof(T));
                v2_cur++;
            }
         }
        // if one of file ended, write remain
        for (; v1_cur < v1_end; v1_cur++) {
            fo.write((char *)(&*v1_cur), sizeof(T));
         }
        for (; v2_cur < v2_end; v2_cur++) {
            fo.write((char *)(&*v2_cur), sizeof(T));
        }
    } else
        return false;

    return true;
}


int main(int argc, char **argv) {
    Timer t_main{"main"};

    using target_type = double;

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " unsorted_file sorted_file" << std::endl;
        return 1;
    }
    std::string unsorted_file{argv[1]};
    std::string sorted_file{argv[2]};

    if (!std::filesystem::exists(unsorted_file)) {
        std::cerr << "sort: error: " << unsorted_file << ": No such file or directory" << std::endl;
        return 1;
    }

    constexpr size_t chunk_mem_limit = 64u << 20u;

    std::filesystem::path bin_dir = std::filesystem::temp_directory_path() / "unit1_sort";
    std::filesystem::remove_all(bin_dir);
    std::filesystem::create_directories(bin_dir);
    std::vector<std::filesystem::path> bins;

    if (std::ifstream unsorted{unsorted_file}) {
        Timer t{"chunks"};

        for (size_t chunk = 0; !unsorted.eof(); chunk++) {
            Timer t2{"chunk" + std::to_string(chunk)};
            std::vector<target_type> arr;
            arr.reserve(chunk_mem_limit / sizeof(target_type));
            std::string line;
            for (size_t c = 0; c < chunk_mem_limit / sizeof(target_type) && std::getline(unsorted, line); c++) {
                arr.emplace_back(std::stod(line));
            }
            std::sort(arr.begin(), arr.end());

            if (std::ofstream f{bins.emplace_back(bin_dir / (std::to_string(chunk) + ".tmp"))}) {
                f.write(reinterpret_cast<char *>(arr.data()), arr.size() * sizeof(target_type));
            } else {
                std::cerr << "sort: error: " << bins.back() << ": Cannot open" << std::endl;
                return 1;
            }
        }
    } else {
        std::cerr << "sort: error: " << unsorted_file << ": Cannot open" << std::endl;
        return 1;
    }

    {
        Timer t{"merge_sort"};
        for (size_t i = 0; i + 1 < bins.size(); i += 2) {
            if (!merge_sort<target_type>(bins[i], bins[i + 1], bins.emplace_back(bin_dir / (std::to_string(bins.size()) + ".tmp"))))
                break;
            std::filesystem::remove(bins[i]);
            std::filesystem::remove(bins[i + 1]);
        }
    }

    {
        Timer t{"bin to string"};
        boost::iostreams::mapped_file mf(bins.back());  // throwable
        if (std::ofstream fo{sorted_file}) {
            auto *v_beg = reinterpret_cast<target_type *>(mf.data());
            auto *v_end = reinterpret_cast<target_type *>(mf.data() + mf.size());
            for (auto *v_cur = v_beg; v_cur < v_end; v_cur++) {
                fo << *v_cur << '\n';
            }
        } else {
            std::cerr << "sort: error: " << sorted_file << ": Cannot open" << std::endl;
            return 1;
        }
    }
}
