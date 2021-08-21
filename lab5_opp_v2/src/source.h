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
    unsigned long size() const;

};

template<typename T>
void ThreadSafeQueue<T>::push(const T &value) {
    std::lock_guard<std::mutex> lockGuard(mutex);
    vector.push_back(value);
}

template<typename T>
T ThreadSafeQueue<T>::pop() {
    std::lock_guard<std::mutex> lockGuard(mutex);
    if (vector.empty()) {
        throw std::logic_error("Pop method has called from empty queue!");
    }
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
unsigned long ThreadSafeQueue<T>::size() const {
    return vector.size();
}

#endif //LAB5_OPP_SOURCE_H
