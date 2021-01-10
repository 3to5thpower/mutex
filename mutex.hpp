#include <memory>
#include <mutex>

#pragma once

template <typename T>
class MutexGuard;

template <typename T>
struct Mutex {
    Mutex() = delete;
    Mutex(T&& value)
        : data(std::make_shared<T>(value)),
          mutex(std::unique_ptr<std::mutex>(new std::mutex)) {}

    MutexGuard<T> lock() {
        this->mutex->lock();
        return MutexGuard<T>(*this);
    }
    void unlock() { this->mutex->unlock(); }

    std::unique_ptr<std::mutex> mutex;
    std::shared_ptr<T> data;
};

template <typename T>
class MutexGuard {
    Mutex<T>& mutex;

   public:
    MutexGuard();
    MutexGuard(Mutex<T>& mut) : mutex(mut) {}
    T& operator*() { return *this->mutex.data.get(); }
    T* operator->() { return this->mutex.data.get(); }
    ~MutexGuard() { this->mutex.unlock(); }
};
