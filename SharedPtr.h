#ifndef __SHAREDPTR_H__
#define __SHAREDPTR_H__

#include <pthread.h>
#include <algorithm>

template <typename T>
class SharedPtr
{
        typedef T ObjectType;

        ObjectType* object;
        int* counter;

        SharedPtr();

        inline int
        fetch_and_add(
            int* counter,
            int value)
        {
            int old_value = value;
            asm volatile("lock; xaddl %%eax, %2;" : "=a" (value) : "a" (value), "m" (*counter) : "memory");
            return value + old_value;
        }

    public:

        SharedPtr(ObjectType* obj) : object(obj)
        {
            counter = new int;
            ++(*counter);
        }

        SharedPtr(const SharedPtr& cPtr) : object(cPtr.object), counter(cPtr.counter)
        {
            fetch_and_add(counter, 1);
        }

        ~SharedPtr()
        {
            int _counter = fetch_and_add(counter, -1);

            if ( _counter == 0 )
            {
                delete counter;
                delete object;
            }
        }

        friend void swap(SharedPtr& spl, SharedPtr& spr)
        {
            using std::swap;

            swap(spl.object, spr.object);
            swap(spl.counter, spr.counter);
            swap(spl._mutex, spr._mutex);
        }

        SharedPtr& operator=(SharedPtr sp)
        {
            swap(*this, sp);

            return *this;
        }

        T& operator*() const
        {
            return *object;
        }

        T* operator->() const
        {
            return object;
        }
};

#endif //__SHAREDPTR_H__
