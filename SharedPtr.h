#ifndef __SHAREDPTR_H__
#define __SHAREDPTR_H__

#include <pthread.h>

template <typename T, typename U = unsigned int>
class SharedPtr
{
        typedef T ObjectType;
        typedef U CountType;

        ObjectType* object;
        CountType* counter;

        pthread_mutex_t* _mutex;

    public:
    
        SharedPtr() : object(0), counter(0) {}

        SharedPtr(ObjectType* obj, CountType* cnt = 0) : object(obj), _mutex(new pthread_mutex_t)
        {
            if (!cnt)
            {
                cnt = new CountType;
            }

            pthread_mutex_init(_mutex, 0);
        }

        SharedPtr(const SharedPtr& cPtr) : object(cPtr->object), counter(cPtr->counter), _mutex(cPtr->_mutex)
        {
            (*counter)++;
        }

        ~SharedPtr()
        {
            bool destroy_mutex = false;

            pthread_mutex_lock(_mutex);

            if ( (--(*counter)) == 0 )
            {
                delete counter;
                delete object;
                destroy_mutex = true;
            }

            pthread_mutex_unlock(_mutex);

            if (destroy_mutex)
            {
                pthread_mutex_destroy(_mutex);
                delete _mutex;
            }
        }
};

#endif //__SHAREDPTR_H__
