#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/timer/timer.hpp>
#include <fstream>
#include "Server/Producer.h"
#include <Server/CalculatorRecord.h>

using namespace Server;
using namespace boost::timer;
using namespace std;

#define LINE_MAX_LENGTH 100

int main(){

    /*{
        cpu_timer t;

        ofstream qfile("Questions.txt", ios::out);
        if(qfile.is_open()){
            Producer::Instance().getAllQuestions(qfile);
            qfile.close();
        }

        cout<<t.format();
    }

    {
        cpu_timer t;

        ofstream afile("Answer.txt", ios::out);
        if(afile.is_open()){
            Producer::Instance().getAllAnswers(afile);
            afile.close();
        }

        cout<<t.format();
    }*/
    {
         cpu_timer t;
        int tmp;
        ofstream qafile("QuestionAndAnswer.txt", ios::out);
        for(int i=0;i<100;++i){
            for(int j=0;j<10000*5;++j){
                qafile<<rand()%2+1;
            }
        }
        for(int i=0;i<1500;++i){
            for(int j=0;j<10000*5;++j){
                tmp+=rand()%2+1;
            }
        }
        qafile.close();
//        sleep(1);
        usleep(300000);
        cout<<t.format();
    }
    /*
    {
        cpu_timer t;

        ofstream qafile("QuestionAndAnswer.txt", ios::out);
        if(qafile.is_open()){
            Producer::Instance().getAllQuestionAndAnswers(qafile);
            qafile.close();
        }

        cout<<t.format();
    }

    {
        CalculatorRecord cr;

        ifstream qafile("QuestionAndAnswer.txt", ios::in);
        ofstream outfile("Result.txt", ios::out);
        cpu_timer t;

        char strbuf[LINE_MAX_LENGTH+1];
        if(qafile.is_open()&&outfile.is_open()){
            while(qafile.peek()!=EOF){
                qafile.getline(strbuf,LINE_MAX_LENGTH);
                cr.push_back(strbuf);
            }
            qafile.close();
            cr.compute();
            cr.Foreach([&](const string &it){
                outfile<<it<<endl;
            });
        }

        cout<<t.format();
    }*/

    return 0;
}


