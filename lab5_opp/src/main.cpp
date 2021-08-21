#include <iostream>
#include "source.h"

int main(int argc, char **argv) {
    ThreadSafeQueue<int> queue;

    std::vector<int> testVector;
    try {
        testVector.erase(testVector.begin());
    } catch (std::exception &e) {
        std::cerr << "Exception" << std::endl;
        exit(1);
    }


    std::cout << "Size of vector is " << queue.size() << std::endl;

    // Добавление в очередь
    {
        std::thread th1(&ThreadSafeQueue<int>::push, &queue, 1);
        std::thread th2(&ThreadSafeQueue<int>::push, &queue, 2);
        std::thread th3(&ThreadSafeQueue<int>::push, &queue, 3);
        th1.join();
        th2.join();
        th3.join();
    }

    std::cout << "Size of vector is " << queue.size() << std::endl;

    // Удаление из очереди
    int firstElem;
    {
        std::thread th1([&](){
            firstElem = queue.pop();
        });
        th1.join();
        std::cout << firstElem << std::endl;

        std::thread th2([&](){
            firstElem = queue.pop();
        });
        th2.join();
        std::cout << firstElem << std::endl;
    }

    std::cout << "Size of vector is " << queue.size() << std::endl;
    return 0;
}
