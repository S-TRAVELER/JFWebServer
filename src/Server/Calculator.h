#ifndef JF_SERVER_CALCULATOR_H
#define JF_SERVER_CALCULATOR_H


#include <string>
#include <vector>
#include <regex>
#include "Server/Fraction.h"

namespace Server {

#define ADD '+'
#define SUB '-'
#define MUL 'x'
#define DIV '$'
#define EQU '='
#define LBR '('
#define RBR ')'
const string divide = "÷";

using namespace std;

class Calculator
{
public:
    void reset();
    Fraction compute(const string &str);
    bool check(const string &str);
    bool check(const string &str, string &answer);
private:
    bool calculate(char op);
    void parse(const string &str);
    Fraction onCompute(bool &onErr);

private:
    bool _flag = false;
    vector<char> _operators;
    vector<Fraction> _nums;
    vector<string> _keys;
};



}   //namespace Server

#endif // JF_SERVER_CALCULATOR_H
