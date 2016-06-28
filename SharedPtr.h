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

        SharedPtr(const SharedPtr<T>& sPtr) : object(sPtr.object), counter(sPtr.counter)
        {
            fetch_and_add(counter, 1);
        }

        ~SharedPtr()
        {
            int _counter = fetch_and_add(counter, -1);

            if ( _counter == 0 )
            {
                delete counter;
                counter = 0;
                delete object;
                object = 0;
            }
        }

        void swap(SharedPtr<T>& sPtr)
        {
            std::swap(object, sPtr.object);
            std::swap(counter, sPtr.counter);
        }

        SharedPtr<T>& operator=(SharedPtr<T> sp)
        {
            swap(sp);

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

        bool operator!() const
        {
            return !object;
        }

        operator bool() const
        {
            return (bool)object;
        }

        friend inline bool operator==(const SharedPtr& spl, const SharedPtr& spr)
        {
            return spl.object == spr.object;
        }

        friend inline bool operator!=(const SharedPtr& spl, const SharedPtr& spr)
        {
            return spl.object != spr.object;
        }
};

class SharedNullPtr
{
    public:

        template<typename U>
        operator SharedPtr<U>()
        {
            return SharedPtr<U>();
        }
};

extern SharedNullPtr nullPtr;


#endif //__SHAREDPTR_H__
