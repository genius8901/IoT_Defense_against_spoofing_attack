//mycircularBuffer.h

#ifndef MYCIRCULARBUFFER_H
#define MYCIRCULARBUFFER_H
#include "CircularBuffer.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_assert.h"
class CircularBuffer;
namespace mbed {
template<typename T, uint32_t BufferSize, typename CounterType = uint32_t>
class MyCircularBuffer: public CircularBuffer<T, BufferSize, uint32_t>
{ 
    public:
    T& operator[] (long i){
        return CircularBuffer<T, BufferSize, uint32_t>::_pool[CircularBuffer<T, BufferSize, uint32_t>::_head + i];
    }
    ~MyCircularBuffer(){}
};

}

#endif 