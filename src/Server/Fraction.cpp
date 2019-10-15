#include "Fraction.h"

namespace Server {

Fraction::Fraction(int integer , int numerator , int denominator ) : _integer(integer),
                                                                    _numerator(numerator),
                                                                    _denominator(denominator)
{
    if (!(_denominator > 0))
    {
        throw runtime_error("denominator must bigger than 0");
    }
}
Fraction::Fraction(const Fraction &that)
    : Fraction(that._integer, that._numerator, that._denominator)
{}
Fraction::Fraction(const std::pair<int, int> &that,int integer):
    Fraction(integer,that.first,that.second){}


Fraction Fraction::parse(const string &str)
{
    size_t pos = str.find('\'');
    size_t oldPos;
    int tmp_integer = 0, tmp_numerator = 0, tmp_denominator = 1;
    if (pos != str.npos)
    {
        tmp_integer = atoi(str.substr(0, pos).c_str());
    }
    if (pos == str.npos)
    {
        oldPos = 0;
        pos = 0;
    }
    else
    {
        oldPos = pos + 1;
    }

    pos = str.find('/', pos);
    if (pos != str.npos)
    {
        // cout << "str:" << str << endl;
        tmp_numerator = atoi(str.substr(oldPos, pos - oldPos).c_str());
        tmp_denominator = atoi(str.substr(pos + 1).c_str());
    }
    else
    {
        tmp_integer = atoi(str.c_str());
    }

    return Fraction(tmp_integer, tmp_numerator, tmp_denominator);
}
void Fraction::setInteger(int integer , bool nozore)
{
    _integer = integer;
    if(_integer==0&&nozore&&_numerator==0){
        if(_denominator==1){
            _integer=1;
        }else{
            _numerator=1;
        }
    }
}

void Fraction::set(int integer, int numerator, int denominator)
{
    _integer = integer;
    _numerator = numerator;
    _denominator = denominator;
}

int Fraction::gcd(int a, int b)
{
    int c = a;
    if (b < a)
    {
        a = b;
        b = c;
    }
    while (b)
    {
        c = a % b;
        a = b;
        b = c;
    }
    return a;
}
Fraction::operator std::string()
{
    char buffer[MAX_FRACTION_LEN]{'\0'};
    if (_denominator == 1)
    {
        sprintf(buffer, "%d", _integer + _numerator);
    }
    else if (_numerator == 0)
    {
        sprintf(buffer, "%d", _integer);
    }
    else if (_integer == 0)
    {
        sprintf(buffer, "%d/%d", _numerator, _denominator);
    }
    else
    {
        sprintf(buffer, "%d'%d/%d", _integer, _numerator, _denominator);
    }
    return string(buffer);
}

Fraction Fraction::operator+(const Fraction &that)
{
    int tmp_denominator, tmp_numerator;
    if (that._numerator == 0)
    {
        tmp_denominator = this->_denominator;
        tmp_numerator = this->_numerator;
    }
    else
    {
        int tmp_gcd = gcd(that._denominator, this->_denominator);
        tmp_denominator = that._denominator * this->_denominator / tmp_gcd;
        tmp_numerator = that._numerator * this->_denominator / tmp_gcd + this->_numerator * that._denominator / tmp_gcd;
    }

    if (tmp_numerator > tmp_denominator)
    {
        return Fraction(that._integer + this->_integer + 1, tmp_numerator - tmp_denominator, tmp_denominator);
    }
    else
    {
        return Fraction(that._integer + this->_integer, tmp_numerator, tmp_denominator);
    }
}

Fraction Fraction::operator-(const Fraction &that)
{
    int tmp_denominator, tmp_numerator;
    if (that._denominator == 0)
    {
        tmp_denominator = this->_denominator;
        tmp_numerator = this->_numerator;
    }
    else
    {
        int tmp_gcd = gcd(that._denominator, this->_denominator);
        tmp_denominator = that._denominator * this->_denominator / tmp_gcd;
        tmp_numerator = this->_numerator * that._denominator / tmp_gcd - that._numerator * this->_denominator / tmp_gcd;
    }

    int this_integer = this->_integer;
    if (tmp_numerator < 0)
    {
        tmp_numerator += tmp_denominator;
        this_integer -= 1;
    }

    return Fraction(this_integer - that._integer, tmp_numerator, tmp_denominator);
}

Fraction Fraction::operator/(const Fraction &that)
{
    int this_numerator = this->_numerator + this->_integer * this->_denominator;
    int that_numerator = that._numerator + that._integer * that._denominator;

    if(that_numerator==0){
        throw runtime_error("除数不能为0");
    }

    int tmp_denominator, tmp_numerator;
    if (this_numerator == 0)
    {
        tmp_denominator = 1;
        tmp_numerator = 0;
    }
    else
    {
        tmp_denominator = that_numerator * this->_denominator;
        tmp_numerator = that._denominator * this_numerator;
        int tmp_gcd = gcd(tmp_denominator, tmp_numerator);
        tmp_denominator /= tmp_gcd;
        tmp_numerator /= tmp_gcd;
    }
    int &&tmp_integer = tmp_numerator / tmp_denominator;

    return Fraction(tmp_integer, tmp_numerator - tmp_integer * tmp_denominator, tmp_denominator);
}

Fraction Fraction::operator*(const Fraction &that)
{
    int this_numerator = this->_numerator + this->_integer * this->_denominator;
    int that_numerator = that._numerator + that._integer * that._denominator;

    int tmp_denominator, tmp_numerator;
    if (that_numerator == 0)
    {
        tmp_denominator = 1;
        tmp_numerator = 0;
    }
    else
    {
        tmp_numerator = that_numerator * this_numerator;
        tmp_denominator = that._denominator * this->_denominator;
        int tmp_gcd = gcd(tmp_denominator, tmp_numerator);
        tmp_denominator /= tmp_gcd;
        tmp_numerator /= tmp_gcd;
    }

    int &&tmp_integer = tmp_numerator / tmp_denominator;

    return Fraction(tmp_integer, tmp_numerator - tmp_integer * tmp_denominator, tmp_denominator);
}

bool Fraction::operator==(const Fraction &that)
{
    int r = this->_integer - that._integer;
    int d = compare(that._numerator, that._denominator);
    return (((r == 0) && (d == 0)));
}

}   //namespace Server
