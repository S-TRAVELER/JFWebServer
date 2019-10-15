#ifndef JF_SERVER_FRACTION_H
#define JF_SERVER_FRACTION_H

#include <vector>
#include <string>
#include <stdexcept>

namespace Server {

#define MAX_FRACTION_LEN 20

using namespace std;

class Fraction
{
public:
    Fraction(int integer = 0, int numerator = 0, int denominator = 1) ;
    Fraction(const Fraction &that);
    Fraction(const std::pair<int, int> &that,int integer=0);


    static Fraction parse(const string &str);
    void setInteger(int integer = 0, bool nozore=false);

    void set(int integer = 0, int numerator = 0, int denominator = 1);
    int gcd(int a, int b);
    operator std::string();

    Fraction operator+(const Fraction &that);
    Fraction operator-(const Fraction &that);
    Fraction operator/(const Fraction &that);
    Fraction operator*(const Fraction &that);

#define FRACTION_BINARY_PREDICATE(cmp, auxcmp)               \
    bool operator cmp(const Fraction &that)                  \
    {                                                        \
        int r = _integer - that._integer;                    \
        int d = compare(that._numerator, that._denominator); \
        return ((r auxcmp 0) || ((r == 0) && (d cmp 0)));    \
    }
    FRACTION_BINARY_PREDICATE(<, <)
    FRACTION_BINARY_PREDICATE(<=, <)
    FRACTION_BINARY_PREDICATE(>=, >)
    FRACTION_BINARY_PREDICATE(>, >)

    bool operator==(const Fraction &that);
private:
    inline int compare(int numerator, int denominator)
    {
        return this->_numerator * denominator - numerator * this->_denominator;
    }

private:
    int _integer;
    int _numerator;
    int _denominator;
};

}   //namespace Server


#endif // JF_SERVER_FRACTION_H
