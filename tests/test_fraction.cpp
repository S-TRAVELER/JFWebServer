#include <iostream>
#include <assert.h>
#include "Server/Producer.h"

using namespace Server;
using namespace std;

void test_producer(){
    Producer::Instance().generate(10,10);
}
int main()
{
    test_producer();
    {
        Fraction f1(2, 1, 2);
        Fraction f2(2, 1, 3);
        assert(f1 > f2);
        cout << string(f1) << endl;
        cout << string(f2) << endl;

        cout << string(f1 + f2) << endl;
        cout << string(f1 - f2) << endl;
        cout << string(f1 * f2) << endl;
        cout << string(f1 / f2) << endl;
    }
    {
        Fraction f1(1, 1, 2);
        Fraction f2(2, 1, 3);
        Fraction f3(2, 1, 3);
        assert(f3 == f2);
        assert(f2 - f1 == Fraction(0, 5, 6));
    }

    {
        Fraction f1(0, 1, 2);
        Fraction f2(2);
        assert(f1 + f2 == Fraction(2, 1, 2));
        assert(f2 - f1 == Fraction(1, 1, 2));
        assert(f1 * f2 == Fraction(1));
        assert(f1 / f2 == Fraction(0, 1, 4));
    }

    {
        Fraction f1 = Fraction::parse("4'1/2");
        Fraction f2 = Fraction::parse("4");
        Fraction f3 = Fraction::parse("1/2");

        assert(f1 == Fraction(4, 1, 2));
        assert(f2 == Fraction(4));
        assert(f3 == Fraction(0, 1, 2));
    }


    return 0;
}
