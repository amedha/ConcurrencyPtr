#ifndef __SHAREDPTR_H__
#define __SHAREDPTR_H__

#include <pthread.h>
#include <algorithm>

template <typename T>
class SharedPtr
{
        typedef T ObjectType;

    public:

        ObjectType* object;
        int* counter;

        inline int fetch_and_add(int* counter, int value)
        {
            int old_value = value;
            asm volatile("lock; xaddl %%eax, %2;" : "=a" (value) : "a" (value), "m" (*counter) : "memory");
            return value + old_value;
        }

    public:

        SharedPtr() : object(0), counter(0) {}

        SharedPtr(ObjectType* obj) : object(obj)
        {
            counter = new int(1);
        }

        SharedPtr(const SharedPtr& cPtr) : object(cPtr.object), counter(cPtr.counter)
        {
            fetch_and_add(counter, 1);
        }

        template <typename U>
        SharedPtr(const SharedPtr<U>& cPtr) : object(cPtr.object), counter(cPtr.counter)
        {
            fetch_and_add(counter, 1);
        }

        ~SharedPtr()
        {
            int _counter = fetch_and_add(counter, -1);

            if ( _counter == 0 )
            {
                std::cout << "Final destruction" << std::endl;
                delete counter;
                delete object;
            }
        }

        friend void swap(SharedPtr& spl, SharedPtr& spr)
        {
            using std::swap;

            swap(spl.object, spr.object);
            swap(spl.counter, spr.counter);
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

        T* operator->()
        {
            return object;
        }

        const T* operator->() const
        {
            return object;
        }

        friend inline bool operator==(const SharedPtr& spl, const SharedPtr& spr)
        {
            return spl.object == spr.object;
        }
};

#endif //__SHAREDPTR_H__
