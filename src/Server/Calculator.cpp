#include "Calculator.h"
#include <assert.h>
#include <iostream>

namespace Server {

void Calculator::reset()
{
    _operators.clear();
    _nums.clear();
    _keys.clear();
}

Fraction Calculator::compute(const string &str)
{
    reset();
    parse(str);
    bool tmp;
    return onCompute(tmp);
}

Fraction Calculator::onCompute(bool &onErr){
    bool exit=false;
    onErr=false;

    for(auto &it:_keys)
    {
        // cout<<str<<endl;
        // cout<<str.substr(_keypos[i-1],_keypos[i]-_keypos[i-1]-1)<<endl;

        const string &tmp = it;

        char op;
        // cout<<"size: "<<tmp.size()<<" "<<"str: "<<tmp<<endl;

        if (tmp[0] >= '0' && tmp[0] <= '9')
        {
            _nums.push_back(Fraction::parse(tmp));
        }
        else if (tmp[0] != ' ')
        {
            switch (tmp[0])
            {

            case SUB:
            case ADD:
                if (_operators.size()>0&&_operators.back()==MUL||_operators.back()==DIV||_operators.back()==SUB){
                    while (op != LBR && _operators.size() > 0)
                    {
                        op = _operators.back();
                        _operators.pop_back();
                        if (op != LBR)
                        {
                            calculate(op);
                        }
                    }

                    if (op == LBR)
                    {
                        _operators.push_back(op);
                    }
                }
                _operators.push_back(tmp[0]);

                break;
            case MUL:
            case LBR:
                _operators.push_back(tmp[0]);
                break;
            case RBR:
                while (op != LBR && _operators.size() > 0)
                {
                    op = _operators.back();
                    _operators.pop_back();
                    if (op != LBR)
                    {
                        calculate(op);
                    }
                }
                break;
            case EQU:
                while (_operators.size() > 0)
                {
                    op = _operators.back();
                    _operators.pop_back();
                    if (op != LBR)
                    {
                        calculate(op);
                    }
                }
                exit=true;
                break;
            default:
                if (tmp.compare(divide) == 0)
                {
                    _flag = true;
                    _operators.push_back(DIV);
                }
                else
                {
                    onErr=true;
                    return Fraction();
                }
                break;
            }
        }
        if(exit){
            break;
        }
    }
    if(_nums.size() != 1){
        onErr=true;
        return Fraction();
    }
    return _nums.back();
}

bool Calculator::calculate(char op)
{
    auto tmp2 = _nums.back();
    _nums.pop_back();
    auto tmp1 = _nums.back();
    _nums.pop_back();
    switch (op)
    {
    case ADD:
        _nums.push_back(tmp1 + tmp2);
        break;
    case SUB:
        _nums.push_back(tmp1 - tmp2);
        break;
    case MUL:
        _nums.push_back(tmp1 * tmp2);
        break;
    case DIV:
        _nums.push_back(tmp1 / tmp2);
        break;
    default:
        return false;
    }
    return true;
}

void Calculator::parse(const string &str)
{

    regex match("\\s*([\\(\\)x=+\\-]|÷|[0-9'/]+)\\s*");
    smatch result;

    auto iterStart=str.cbegin();
    auto pos=str.find('.');
    if(pos!=str.npos){
        iterStart+=pos+1;
    }

    while(regex_search(iterStart,str.cend(),result,match)){
        _keys.push_back(result[1]);
        iterStart=result[0].second;
    }
}

bool Calculator::check(const string &str){
    string tmp;
    return check(str,tmp);
}

bool Calculator::check(const string &str, string &answer){
    reset();
    parse(str);
    answer="请检查，式子错误或未填写答案";

    if(_keys.size()>2&&_keys[_keys.size()-2]=="="&&_keys.back()[0]>='0'&&_keys.back()[0]<='9'){
        bool err;
        auto tmp=onCompute(err);
        answer=string(tmp);

        return err? ! err:tmp==Fraction::parse(_keys.back());
    }

    return false;
}

}
