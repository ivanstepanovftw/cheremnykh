
#undef MUTEX_TYPE
#define MUTEX_TYPE std::mutex
#include <mutex>
namespace std_mutex {
#include "bits_nanocube.hh"
}

#undef MUTEX_TYPE
#define MUTEX_TYPE std::shared_mutex
#include <shared_mutex>
namespace std_shared_mutex {
#include "bits_nanocube.hh"
}

#undef MUTEX_TYPE
#define MUTEX_TYPE my::shared_mutex
#include "shared_mutex.hh"
namespace my_shared_mutex {
#include "bits_nanocube.hh"
}

#undef MUTEX_INCLUDE
#undef MUTEX_TYPE
