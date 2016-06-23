#include <iostream>
using namespace std;

void
fetch_and_add( int * variable, int value ) {
        asm volatile("lock; xaddl %%eax, %1;" : /*"=a" (value)*/ : "a" (value), "m" (*variable) : "memory");
}

int
main()
{
    int* a = new int;
    *a = 1;
    int *c = a;

    int b = -5;

    cout << *a << " " << b << endl;

    fetch_and_add(a, b);

    cout << *a << " " << b << endl;
    cout << *c << endl;
}
