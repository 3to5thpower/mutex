#include <iostream>
#include <memory>
#include <queue>
#include <thread>

#include "mutex.hpp"
#include "opencv2/core.hpp"

using namespace std;

using qint = shared_ptr<Mutex<queue<cv::Mat>>>;

void foo(qint src, qint dst) {
    static int cnt = 0;
    if (src->lock()->empty()) {
        this_thread::sleep_for(chrono::milliseconds(100));
        return;
    }
    auto e = src->lock()->front();
    src->lock()->pop();
    dst->lock()->push(e);
}
void bar(qint src, qint dst) {
    static int cnt = 0;
    if (src->lock()->empty()) {
        this_thread::sleep_for(chrono::milliseconds(100));
        return;
    }
    auto e = src->lock()->front();
    src->lock()->pop();
    dst->lock()->push(e);
}

void bal(qint src, qint dst) {
    static int cnt = 0;
    if (src->lock()->empty()) {
        this_thread::sleep_for(chrono::milliseconds(100));
        return;
    }
    auto e = src->lock()->front();
    src->lock()->pop();
    dst->lock()->push(e);
}

int main(void) {
    qint a(new Mutex<queue<cv::Mat>>(queue<cv::Mat>())),
        b(new Mutex<queue<cv::Mat>>(queue<cv::Mat>())),
        c(new Mutex<queue<cv::Mat>>(queue<cv::Mat>())),
        d(new Mutex<queue<cv::Mat>>(queue<cv::Mat>()));

    for (int i = 0; i < 50; i++) {
        cv::Mat e = (cv::Mat_<int>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
        a->lock()->push(e);
    }
    shared_ptr<Mutex<bool>> p(new Mutex<bool>(true));

    auto t1 = thread([=]() {
        while (*p->lock()) foo(a, b);
    });

    auto t2 = thread([=]() {
        while (*p->lock()) bar(b, c);
    });
    auto t3 = thread([=]() {
        while (*p->lock()) bal(c, d);
    });

    this_thread::sleep_for(chrono::milliseconds(1000));
    { *p->lock() = false; }
    t1.join();
    t2.join();
    t3.join();
    for (int i = 0; i < 50; ++i) {
        auto e = d->lock()->front();
        d->lock()->pop();
        cout << e << endl;
    }
}
