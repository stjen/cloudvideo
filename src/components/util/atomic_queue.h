#ifndef SRC_COMPONENTS_UTIL_ATOMIC_QUEUE
#define SRC_COMPONENTS_UTIL_ATOMIC_QUEUE

#include <mutex>
#include <queue>
#include <thread>

namespace components::util {

template <typename T> class AtomicQueue {
public:
  void push(const T &value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(value);
  }

  void pop() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.pop();
  }

  T front() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.front();
  }

  bool empty() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.empty();
  }

private:
  std::queue<T> m_queue;
  mutable std::mutex m_mutex;
};
} // namespace components::util

#endif /* SRC_COMPONENTS_UTIL_ATOMIC_QUEUE */
