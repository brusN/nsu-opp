#include "source.h"

void *safeMalloc(size_t sizeofType, size_t count) {
    void *allocatedMemory = malloc(sizeofType * count);
    if (allocatedMemory == nullptr) {
        std::cerr << "Error while allocating memory by using malloc!" << std::endl;
        std::cerr << "Count elements: " << count << " | size of (in bytes)" << sizeofType << std::endl;
        exit(1);
    }
    return allocatedMemory;
}

void *safeCalloc(size_t count, size_t sizeofType) {
    void *allocatedMemory = calloc(count, sizeofType);
    if (allocatedMemory == nullptr) {
        std::cerr << "Error while allocating memory by using calloc!" << std::endl;
        std::cerr << "Count elements: " << count << " | size of (in bytes)" << sizeofType << std::endl;
        exit(1);
    }
    return allocatedMemory;
}

