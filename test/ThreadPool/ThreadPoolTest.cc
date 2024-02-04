#include "../../src/threadpool/ThreadPool.h"
#include <iostream>
int main() {
    ThreadPool pool(4);

    auto result1 = pool.enqueue([](int x, int y) { return x + y; }, 2, 3);
    auto result2 = pool.enqueue([](int x, int y) { return x * y; }, 4, 5);

    std::cout << "Result1: " << result1.get() << std::endl;
    std::cout << "Result2: " << result2.get() << std::endl;

    return 0;
}