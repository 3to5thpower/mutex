#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

int main(void) {
    std::mutex mutex;
    int count = 0;

    std::thread t1([&] {
        for (int i = 0; i < 100; ++i) {
            std::lock_guard<std::mutex> lock(mutex);
            count++;
        }
    });

    std::thread t2([&] {
        for (int i = 0; i < 100; ++i) {
            std::lock_guard<std::mutex> lock(mutex);
            count++;
        }
    });

    t1.join();
    t2.join();
    std::cout << count << std::endl;
}