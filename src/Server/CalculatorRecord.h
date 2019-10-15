#ifndef CALCULATORRECORD_H
#define CALCULATORRECORD_H
#include <memory>
#include <vector>
#include <atomic>
#include <iostream>
#include "Server/Calculator.h"

namespace Server {
using namespace std ;
class CalculatorRecord
{
public:
    CalculatorRecord():
        _strs(make_shared<vector<string>>()),
        _total(0),
        _finished(0){}
    CalculatorRecord(shared_ptr<vector<string>> strs):
        _strs(strs),
        _total(_strs->size()),
        _finished(0){}

    void push_back(const string &str){
         if(str.rfind("=")!=str.npos){
            _strs->push_back(str);
            ++_total;
         }
    }
    void reset(){
        _strs.reset();
        _strs=make_shared<vector<string>>();
        _total=0;
        _finished=0;
    }
    int getProgressRate(){
        cout<<_finished<<_total<<endl;
        return 100*_finished/_total;
    }
    void compute(){
        for(auto &it:*_strs){
            string answer;
            if(_calculator.check(it,answer)){
                it+=" \t[√]";
            }else{
                it+=" \t[X] answer: "+answer;
            }
            ++_finished;
        }
    }
    void Foreach (function<void(const string &)> fun){
        for(auto &it:*_strs){
            fun(it);
        }
    }
private:
Calculator _calculator;
shared_ptr<vector<string>>  _strs;
int _total;
atomic<int> _finished;
};

}   //namespace Server


#endif // CALCULATORRECORD_H
