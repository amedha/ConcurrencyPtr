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

        SharedPtr() : object(NULL), counter(NULL)
        {
            std::cout << "Default Constructor" << std::endl;
        }

        SharedPtr(BaseType* obj) : object(obj)
        {
            counter = new int(1);
            std::cout << "Constructor New: counter_value: " << *counter << " counter_addr: " << counter << " object_addr: " << object << std::endl;
        }

        SharedPtr(const SharedPtr<BaseType>& sPtr) : object(sPtr.object), counter(sPtr.counter)
        {
            std::cout << "Copy Constructor before: counter_value: " << *counter << " counter_addr: " << counter << " object_addr: " << object << std::endl;
            fetch_and_add(counter, 1);
            std::cout << "Copy Constructor after: counter_value: " << *counter << " counter_addr: " << counter << " object_addr: " << object << std::endl;
        }

        template <typename DerivedType>
        SharedPtr(const SharedPtr<DerivedType>& sPtr) : /*object(sPtr.object),*/ counter(sPtr.counter)
        {
            object = dynamic_cast<BaseType*>(sPtr.object);
            std::cout << "Copy Constructor Derived before: counter_value: " << *counter << " counter_addr: " << counter << " object_addr: " << object << std::endl;
            fetch_and_add(counter, 1);
            std::cout << "Copy Constructor Derived after: counter_value: " << *counter << " counter_addr: " << counter << " object_addr: " << object << std::endl;
        }

        ~SharedPtr()
        {
            if (!counter) return;
            
            std::cout << "Descteruction before: counter_value: " << *counter << " counter_addr: " << counter << " object_addr: " << object << std::endl;
            int _counter = fetch_and_add(counter, -1);
            std::cout << "Descteruction after: counter_value: " << *counter << " counter_addr: " << counter << " object_addr: " << object << std::endl;

            if ( _counter == 0 )
            {
                std::cout << "Final Destruction begin: counter_addr: " << counter << std::endl;
                delete counter;
                counter = NULL;
                delete object;
                object = NULL;
                std::cout << "Final Destruction end: counter_addr: " << counter << std::endl;
            }
        }

        SharedPtr<BaseType>& operator=(SharedPtr<BaseType> sp)
        {
            std::cout << "operator= base" << std::endl;

            swap(sp);

            return *this;
        }

        template <typename DerivedType>
        SharedPtr<BaseType>& operator=(SharedPtr<DerivedType> sp)
        {
            std::cout << "operator= derived" << std::endl;

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

        bool isNull()
        {
            if (counter == NULL)
                return true;
            return false;
        }

        bool isNotNull()
        {
            if (counter == NULL)
                return false;
            return true;
        }

        T* get()
        {
            return object;
        }
};

#endif //__SHAREDPTR_H__
