//
// Created by Michal_Marszalek on 15.01.2021.
//
/*
#include "b2_user_settings.h"
#include <cstddef>

#if __cplusplus >= 201703L

#include <memory_resource>
#include <unordered_map>
#include <atomic>

static std::byte stackBuffer[10485760]; //10MB
static std::pmr::monotonic_buffer_resource pmrBuffer(stackBuffer, sizeof stackBuffer);
static std::pmr::unsynchronized_pool_resource unsynchronizedPoolResource(&pmrBuffer);
static std::pmr::unordered_map<void *, std::size_t> sizes(&unsynchronizedPoolResource);
static std::atomic_flag save = ATOMIC_FLAG_INIT;

void *b2Alloc(int32 size) {
    while (save.test_and_set(std::memory_order_acquire));

    void *result;
    try{
        result = unsynchronizedPoolResource.allocate(size);
    }catch (...){
        result = nullptr;
    }

    if(result){sizes[result] = size;}

    save.clear(std::memory_order_release);
    return result;
}

void b2Free(void *mem) {
    while (save.test_and_set(std::memory_order_acquire));

    unsynchronizedPoolResource.deallocate(mem, sizes[mem]);

    save.clear(std::memory_order_release);
}

#else
void* b2Alloc(int32 size)
{
    return b2Alloc_Default(size);
}

/// If you implement b2Alloc, you should also implement this function.
void b2Free(void* mem)
{
    b2Free_Default(mem);
}
#endif
*/
