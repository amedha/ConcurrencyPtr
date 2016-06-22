#include <iostream>
#include <string>
#include "SharedPtr.h"

using std::cout;
using std::endl;
using std::string;

/*
void*
thread1_func(void* _args)
{
    SharedPtr<string> *args = (SharedPtr<string>*)_args;
    //SharedPtr<string> sp(*args);

    timespec ts;
    ts.tv_sec = 1;
    ts.tv_nsec = 0;
    nanosleep(&ts, NULL);

    cout << "Thread: " << pthread_self() << endl;
}

void*
thread2_func(void* _args)
{
    SharedPtr<string> *args = (SharedPtr<string>*)_args;
    //SharedPtr<string> sp(*args);

    timespec ts;
    ts.tv_sec = 3;
    ts.tv_nsec = 0;
    nanosleep(&ts, NULL);

    cout << "Thread: " << pthread_self() << endl;
}
*/

int
main()
{
    pthread_t thread1, thread2;

    SharedPtr<string> sp(new string("Barad-dur"));

    //pthread_create(&thread1, 0, thread1_func, (void*)&sp);
    //pthread_create(&thread2, 0, thread2_func, (void*)&sp);

    //cout << "Thread: " << pthread_self() << endl;

    //pthread_exit(0);
}
