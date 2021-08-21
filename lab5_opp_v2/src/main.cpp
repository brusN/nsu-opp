#include <iostream>
#include <future>
#include "source.h"

int main(int argc, char **argv) {
    ThreadSafeQueue<int> queue;

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
        /*
        std::thread th1([&]() {
            firstElem = queue.pop();
        });
        th1.join();
        std::cout << firstElem << std::endl;
        */
        std::packaged_task<int()> task([&queue] { // wrap the function
            return queue.pop();
        });
        std::future<int> f1 = task.get_future();  // get a future
        std::thread t1(std::move(task)); // launch on a thread
        f1.wait();
        std::cout << "Deleted element: " << f1.get() << std::endl;
        t1.join();
    }

    std::cout << "Size of vector is " << queue.size() << std::endl;
    return 0;
}
