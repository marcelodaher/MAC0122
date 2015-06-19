#include <iostream>
#include "slot.cpp"

#define TRUE (1==1)
#define FALSE (!TRUE)

using namespace std;

int main()
{
    Slot *s1, *s2;
    s1 = new Slot(200,100,1);
    s2 = new Slot(150,100,1);
    cout << s1->interfere(s2);
    cout << s1->antes(s2);
    return 0;
}
