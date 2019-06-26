#pragma once

namespace my {

// This shared_mutex is like shared_spinlock. I might use
// http://howardhinnant.github.io/shared_mutex
// http://howardhinnant.github.io/shared_mutex.cpp
// but idk if cheating acceptable by the challenge.
// Original shared_mutex is more complicated and more cache-friendly, so...
class shared_mutex {
public:
    std::atomic<int> flag_writer{};
    std::atomic<int> flag_reader{};

    ///
    /// only one thread can write
    ///
    void lock() {
        while (flag_reader && flag_writer)
            ;
        flag_writer++;
    }

    void unlock() {
        flag_writer--;
    }

    ///
    /// many threads can read
    ///
    void lock_shared() {
        while(flag_writer)
            ;
        flag_reader++;
    }

    // read unlock
    void unlock_shared() {
        flag_reader--;
    }
};

class shared_recursive_mutex : public shared_mutex {
public:
    void lock() {
        std::thread::id this_id = std::this_thread::get_id();
        if (owner == this_id) {
            // recursive locking
            count++;
        } else {
            // normal locking
            shared_mutex::lock();
            owner = this_id;
            count = 1;
        }
    }

    void unlock() {
        if (count > 1) {
            // recursive unlocking
            count--;
        } else {
            // normal unlocking
            owner = std::thread::id();
            count = 0;
            shared_mutex::unlock();
        }
    }

private:
    std::atomic<std::thread::id> owner;
    int count;
};

} // namespace my