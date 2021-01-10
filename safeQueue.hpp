#include <exception>
#include <queue>

#include "mutex.hpp"

#pragma once

class empty_queue : std::exception {
   public:
    const char* what() const throw() { return "Empty queue"; }
};

template <typename T>
class safeQueue {
   public:
    safeQueue() : que(Mutex<std::queue<T>>(std::queue<T>())) {}
    safeQueue(safeQueue const& other) {
        other.que.lock();
        que = other.que;
    }
    safeQueue& operator=(safeQueue const&) = delete;
    void push(T value) { this->que.lock()->push(value); }
    void pop(T& res) {
        auto q = this->que.lock();
        if (q->empty()) throw empty_queue();
        res = q->front();
        q->pop();
    }
    std::shared_ptr<T> pop() {
        auto q = this->que.lock();
        if (q->empty()) throw empty_queue();
        auto const res = std::make_shared<T>(q->front());
        q->pop();
        return res;
    }
    bool empty() { return this->que.lock()->empty(); }
    size_t size() { this->que.lock()->size(); }

   private:
    Mutex<std::queue<T>> que;
};