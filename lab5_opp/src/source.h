#ifndef LAB5_OPP_SOURCE_H
#define LAB5_OPP_SOURCE_H

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>


template<typename T>
class ThreadSafeQueue {
private:
    std::vector<T> vector;
    std::mutex mutex;
public:
    void push(const T &value);
    T pop();
    T peek();
    unsigned long size();

};

template<typename T>
void ThreadSafeQueue<T>::push(const T &value) {
    mutex.lock();
    vector.push_back(value);
    mutex.unlock();
}

template<typename T>
T ThreadSafeQueue<T>::pop() {
    std::lock_guard<std::mutex> lockGuard(mutex);
    int firstElem = vector.front();
    vector.erase(vector.begin());
    return firstElem;
}

template<typename T>
T ThreadSafeQueue<T>::peek() {
    mutex.lock();
    auto firstElem = vector.front();
    mutex.unlock();
    return firstElem;
}

template<typename T>
unsigned long ThreadSafeQueue<T>::size() {
    mutex.lock();
    int size = vector.size();
    mutex.unlock();
    return size;
}


#endif //LAB5_OPP_SOURCE_H
