#ifndef __SHAREDPTR_H__
#define __SHAREDPTR_H__

#include <iostream>
#include <algorithm>

template <typename T>
class SharedPtr
{
        typedef T BaseType;

    public:

        BaseType* object;
        int* counter;

        inline int fetch_and_add(int* counter, int value)
        {
            int old_value = value;

            asm volatile("lock; xaddl %%ebx, %2;" : "=b" (value) : "b" (value), "m" (*counter) : "memory");

            return value + old_value;
        }

    public:

        SharedPtr() : object(0), counter(0)
        {
            std::cout << "Default Constructor" << std::endl;
        }

        SharedPtr(BaseType* obj) : object(obj)
        {
            counter = new int(1);
        }

        SharedPtr(const SharedPtr<BaseType>& sPtr) : object(sPtr.object), counter(sPtr.counter)
        {
            fetch_and_add(counter, 1);
            std::cout << "Copy Constructor Base" << std::endl;
        }

        template <typename DerivedType>
        SharedPtr(const SharedPtr<DerivedType>& sPtr) : object(sPtr.object), counter(sPtr.counter)
        {
            fetch_and_add(counter, 1);
            std::cout << "Copy Constructor Derived" << std::endl;
        }

        ~SharedPtr()
        {
            if (!counter) return;

            int _counter = fetch_and_add(counter, -1);

            if ( _counter == 0 )
            {
                delete counter;
                counter = 0;
                delete object;
                object = 0;
            }
        }

        SharedPtr<BaseType>& operator=(SharedPtr<BaseType> sp)
        {
            swap(sp);

            return *this;
        }

        template <typename DerivedType>
        SharedPtr<BaseType>& operator=(SharedPtr<DerivedType> sp)
        {
            swap(sp);

            return *this;
        }

        void swap(SharedPtr<BaseType>& sPtr)
        {
            std::swap(object, sPtr.object);
            std::swap(counter, sPtr.counter);
        }

        template <typename DerivedType>
        void swap(SharedPtr<DerivedType>& sPtr)
        {
            std::swap(object, sPtr.object);
            std::swap(counter, sPtr.counter);
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

#endif //__SHAREDPTR_H__
