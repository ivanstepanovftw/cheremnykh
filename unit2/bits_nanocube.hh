//
// Created by root on 25.06.19.
//

#if !defined(MUTEX_TYPE)
# error "MUTEX_TYPE not defined"
#endif

#include <boost/circular_buffer.hpp>


class nanocube {
public:
    int read(int index) {
        return data[index%data.size()];
    }

    void write(int value) {
        data.push_back(value);
    }

    mutable MUTEX_TYPE mutex;
    boost::circular_buffer<int> data{100};
};

