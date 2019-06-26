/*
 * Unit 2.
 * Написать собственную реализацию shared_mutex поддерживающий рекурсию. Не используя C14.
 * Использовать платформонезависимый код – средства stl и boost (но не использовать shared_mutex).
 */

#include <utility>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <atomic>
#include <shared_mutex>
#include <boost/circular_buffer.hpp>
#include "../common.hh"
#include "shared_mutex.hh"
#include "nanocube.hh"




template <class NANOCUBE>
void reader_shared(NANOCUBE& n) {
    n.mutex.lock_shared();
    for(size_t s = 0; s<200000000; s++) {
        if (n.read(s) == -1)
            std::cout<<"a"<<std::endl;
    }
    n.mutex.unlock_shared();
}

template <class NANOCUBE>
void reader(NANOCUBE& n) {
    n.mutex.lock();
    for(size_t s = 0; s<200000000; s++) {
        if (n.read(s) == -1)
            std::cout<<"a"<<std::endl;
    }
    n.mutex.unlock();
}

template <class NANOCUBE>
void writer(NANOCUBE& n) {
    for(size_t s = 1; s<100; s++) {
        n.mutex.lock();
        n.write(s);
        n.mutex.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(10000));
    }
}

void bench() {
    constexpr size_t reader_threads = 100;
    constexpr size_t writer_threads = 10;

    {
        std_shared_mutex::nanocube n;
        {
            // Timer t1{"writer"};
            writer(n);
        }
        // {
        //     Timer t1{"reader"};
        //     reader(n,repeats);
        // }

        Timer t2{"std_shared_mutex"};
        std::vector<std::thread> threads;
        for (int i=0; i<reader_threads; i++) {
            threads.emplace_back(reader_shared<decltype(n)>, std::ref(n));
        }
        for (int i=0; i<writer_threads; i++) {
            threads.emplace_back(writer<decltype(n)>, std::ref(n));
        }

        for (auto &th : threads) {
            if (th.joinable())
                th.join();
        }
    }
    {
        std_shared_mutex::nanocube n;
        writer(n);

        Timer t2{"std_shared_mutex_invalid_usage"};
        std::vector<std::thread> threads;
        for (int i=0; i<reader_threads; i++) {
            threads.emplace_back(reader<decltype(n)>, std::ref(n));
        }
        for (int i=0; i<writer_threads; i++) {
            threads.emplace_back(writer<decltype(n)>, std::ref(n));
        }

        for (auto &th : threads) {
            if (th.joinable())
                th.join();
        }
    }
    {
        std_mutex::nanocube n;
        writer(n);

        Timer t{"std_mutex"};
        std::vector<std::thread> threads;
        for (int i=0; i<reader_threads; i++) {
            threads.emplace_back(reader<decltype(n)>, std::ref(n));
        }
        for (int i=0; i<writer_threads; i++) {
            threads.emplace_back(writer<decltype(n)>, std::ref(n));
        }

        for (auto &th : threads) {
            if (th.joinable())
                th.join();
        }
    }
    {
        my_shared_mutex::nanocube n;
        writer(n);

        Timer t2{"my_shared_mutex"};
        std::vector<std::thread> threads;
        for (int i=0; i<reader_threads; i++) {
            threads.emplace_back(reader_shared<decltype(n)>, std::ref(n));
        }
        for (int i=0; i<writer_threads; i++) {
            threads.emplace_back(writer<decltype(n)>, std::ref(n));
        }

        for (auto &th : threads) {
            if (th.joinable())
                th.join();
        }
    }
}

int main() {
    using namespace std;
    bench();
    return 0;
}