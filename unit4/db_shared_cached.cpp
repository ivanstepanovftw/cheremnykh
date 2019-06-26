#include <string>
#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>
#include <iostream>


class i_db {
public:
    virtual bool begin_transaction() = 0;
    virtual bool commit_transaction() = 0;
    virtual bool abort_transaction() = 0;
    virtual std::string get(const std::string& key) = 0;
    virtual std::string set(const std::string& key, const std::string& data) = 0;
    virtual std::string del(const std::string& key) = 0;

    bool is_use{false};
};


/// 'shared' means it used as one instance to many threads.
class db_shared_cached {
public:
    explicit db_shared_cached(i_db& parent)
            : m_parent(parent) {}

    bool begin_transaction() {
        std::unique_lock l(m_sm);
        return m_parent.begin_transaction();
    }

    /// Commit simply means to permanently save all the changes which you have made in the current transaction
    bool commit_transaction() {
        std::unique_lock l(m_sm);
        for (const auto& kv : cache) {
            m_parent.set(kv.first, kv.second);
        }
        return m_parent.commit_transaction();
    }

    /// Abort means to disrupt the action by killing the current ongoing transaction.
    bool abort_transaction() {
        std::unique_lock l(m_sm);
        return m_parent.abort_transaction();
    }

    std::string get(const std::string& key) {
        {
            std::shared_lock l(m_sm);
            if (auto a = cache.find(key); a != cache.end()) {
                return a->second;
            }
        }
        {
            std::unique_lock l(m_sm);
            auto data = m_parent.get(key);
            cache[key] = data;
            return data;
        }
    }

    std::string set(const std::string& key, const std::string& data) {
        std::unique_lock l(m_sm);
        if (m_parent.is_use) {
            return m_parent.set(key, data);
        } else {
            return cache[key] = data;
        }
    }

    std::string del(const std::string& key) {
        std::unique_lock l(m_sm);
        if (m_parent.is_use) {
            return m_parent.del(key);
        } else {
            return cache[key] = "";
        }
    }

private:
    i_db& m_parent;
    std::shared_mutex m_sm;
    std::unordered_map<std::string, std::string> cache;
};


// int main() {
//     std::unordered_map<std::string, std::string> cache;
//     std::cout<<typeid(cache["a"] = "b").name()<<std::endl; //NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
// }
