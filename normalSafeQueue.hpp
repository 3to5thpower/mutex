#include <exception>
#include <memory>
#include <mutex>

class empty_queue : std::exception {
   public:
    const char* what() const throw() { return "Empty queue"; }
};

template <typename T>
class safe_queue {
   public:
    safe_queue() {}
    safe_queue(safe_queue const& other) {
        std::lock_guard<std::mutex> lock(other.m);
        que = other.que;
    }
    safe_queue& operator=(safe_queue const&) = delete;
    void push(T value) {
        std::lock_guard<std::mutex> lock(m);
        que.push(value);
    }
    void pop(T& res) {
        std::lock_guard<std::mutex> lock(m);
        if (que.empty()) throw empty_queue();
        res = que.front();
        que.pop();
    }
    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(m);
        if (que.empty()) throw empty_queue();
        auto const res = std::make_shared<T>(que.front());
        que.pop();
        return res;
    }
    bool empty() const {
        std::lock_guard<std::mutex> lock(m);
        return que.empty();
    }
    size_t size() const {
        std::lock_guard<std::mutex> lock(m);
        return que.size();
    }

   private:
    std::queue<T> que;
    mutable std::mutex m;
};
