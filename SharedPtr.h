#ifndef __SHAREDPTR_H__
#define __SHAREDPTR_H__

#include <pthread.h>
#include <algorithm>

template <typename T>
class SharedPtr
{
        typedef T ObjectType;

        ObjectType* object;
        unsigned int* counter;

        pthread_mutex_t* _mutex;

        SharedPtr();

    public:

        SharedPtr(ObjectType* obj) : object(obj), _mutex(new pthread_mutex_t)
        {
            std::cout << "Constructor" << std::endl;

            counter = new unsigned int;

            ++(*counter);

            pthread_mutex_init(_mutex, 0);
        }

        SharedPtr(const SharedPtr& cPtr) : object(cPtr.object), counter(cPtr.counter), _mutex(cPtr._mutex)
        {
            std::cout << "Copy Constructor" << std::endl;

            pthread_mutex_lock(_mutex);

            ++(*counter);

            pthread_mutex_unlock(_mutex);
        }

        ~SharedPtr()
        {
            std::cout << "Destructor" << std::endl;

            pthread_mutex_lock(_mutex);

            if ( (--(*counter)) == 0 )
            {
                std::cout << "Final destruction" << std::endl;

                delete counter;
                delete object;

                counter = 0;
                object = 0;
            }

            pthread_mutex_unlock(_mutex);
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
