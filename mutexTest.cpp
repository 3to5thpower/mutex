#include "mutex.hpp"

#include <iostream>
#include <thread>

#include "safeQueue.hpp"

std::mutex stdout_m;  // std::cout の排他制御
template <typename T>
void threadsafe_print(T v) {
    std::lock_guard<std::mutex> lock(stdout_m);
    std::cout << v << std::endl;
}

int main(void) {
    std::shared_ptr<Mutex<int>> counter(new Mutex<int>(0));
    std::thread t1([&] {
        for (int i = 0; i < 100000; i++) {
            auto num = counter->lock();
            *num += 1;
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < 100000; i++) {
            auto num = counter->lock();
            *num += 1;
        }
    });

    t1.join();
    t2.join();

    std::cout << *(counter->lock()) << std::endl;
    safe_queue<int> sq;
    for (int i = 0; i < 1000; ++i) {
        sq.push(i);
    }

    std::thread t3([&] {
        while (!sq.empty()) {
            auto value = sq.pop();
            threadsafe_print("[thread A] poped: " + std::to_string(*value));
        }
    });
    std::thread t4([&] {
        while (!sq.empty()) {
            auto value = sq.pop();
            threadsafe_print("[thread B] poped: " + std::to_string(*value));
        }
    });

    t3.join();
    t4.join();
    std::cout << sq.empty() << std::endl;
}