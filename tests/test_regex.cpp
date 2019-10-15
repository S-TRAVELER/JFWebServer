#include <iostream>
#include <regex>
#include "Server/Calculator.h"
#include <assert.h>

using namespace std;
using namespace Server;

int main(){
    {
        Calculator c;
        cout << string(c.compute("12 x 14 + 15 ÷ 3 - 1/23 = ")) << endl;
        cout << string(c.compute("1/2")) << endl;
        cout<< string(c.compute("( 1 + 2 ) x 3 = "))<<endl;
        cout<< string(c.compute("3 x ( 1 + 1'1/2 ) ="))<<endl;
        cout<< "3 x ( 1 + 1'1/2 ) = 7'1/2 "<<(c.check("3 x ( 1 + 1'1/2 ) = 7'1/2")?"true":"false")<<endl;
        string question("1+ 2-3+(1+2)x 5= 1");
        string answer;
        bool result=c.check(question,answer);
        cout<< question<<" "<<(result?"true":"false")<<" answer: "<<answer<<endl;

    }
    {
        string str(" (1'2/3+2 )÷3 x1 -  1/3 = 48");
        regex match("\\s*([\\(\\)x=+\\-]|÷|[0-9'/]+)\\s*");
        smatch result;

        auto iterStart=str.cbegin();

        while(regex_search(iterStart,str.cend(),result,match)){
            cout<<"size: "<<result[1].str().size()<<" "<<result[1]<<endl;
            iterStart=result[0].second;
        }
    }



    return 0;
}
