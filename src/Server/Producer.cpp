#include "Producer.h"
#include "Server/Fraction.h"

namespace Server {

INSTANCE_IMP(Producer);

Producer::Producer() :
    _fast_indexs(2*MAX_SIZE+1),
    _randIndexs(100){

    vector<shared_ptr<PairVector>> FracVector(MAX_STEP);
    for (auto &it : FracVector)
    {
        it = make_shared<PairVector>(MAX_SIZE + MAX_STEP);
    }
    randIndexs();
    /*------------生成公约数为1真分数--------------------*/
    {
        vector<int> randint{1, 2, 3, 5, 7, 11, 13, 17, 19};
        for (int i = 0; i < MAX_SIZE; ++i)
        {
            (*FracVector[0])[i] = {i, i + 1};
        }
        int tmp = 0;

        for (int s = 1; s < MAX_STEP; ++s)
        {
            tmp = 0;
            int step = randint[s];
            for (int i = 0; i < MAX_SIZE;)
            {
                for (int t = 0; t < step - 1; ++t)
                {
                    ++tmp;
                    (*FracVector[s])[i] = {tmp, tmp + step};
                    ++i;
                }
                ++tmp;
            }
        }
    }

    set<tuple<int,string,string> > fractionSet;
    /*------------生成分数二元四则运算式子--------------------*/
    {
        for(int i=3;i<10;++i){
            for(int j=2;j<i;++j){
                int &&key=j+1;

                int tmp1=rand()%key;
                int tmp2=rand()%key;
                fractionSet.insert({key, to_string(tmp1) +" + "+ to_string(tmp2), to_string(tmp1+tmp2)});

                tmp1=rand()%key;
                tmp2=rand()%key;
                fractionSet.insert({key, to_string(tmp1) +" x "+ to_string(tmp2), to_string(tmp1*tmp2)});

                tmp1=rand()%key;
                tmp2=rand()%key+1;
                fractionSet.insert({key, to_string(tmp1) +" ÷ "+ to_string(tmp2), string(Fraction(tmp1)/Fraction(tmp2))});

                tmp1=rand()%key+1;
                tmp2=rand()%tmp1;
                // cout<<"f0: "<<string(f[0])<<" f1: "<<string(f[1])<<" op-: "<<string(f[0]-f[1])<<endl;
                fractionSet.insert({key, to_string(tmp1) +" - "+ to_string(tmp2), to_string(tmp1-tmp2)});
            }

        }


        for(int i=0;i<MAX_STEP;++i){
            for(int j=0;j<MAX_SIZE;++j){
                for(int k=0;k<j;++k){
                    try{
                        int key=(*FracVector[i])[j].second;
                        //控制除运算不要运算太复杂
                        int key_div=key>2?2:key;
                        Fraction f[2];

                        f[0]=Fraction((*FracVector[i])[j],rand()%key);
                        f[1]=Fraction((*FracVector[i])[k],rand()%key);
                        // cout<<"f0: "<<string(f[0])<<" f1: "<<string(f[1])<<" op+: "<<string(f[0]+f[1])<<endl;
                        fractionSet.insert({key,string(f[0])+" + "+ string(f[1]),string(f[0]+f[1])});

                        f[0].setInteger(rand()%key);
                        f[1].setInteger(rand()%key);
                        // cout<<"f0: "<<string(f[0])<<" f1: "<<string(f[1])<<" op*: "<<string(f[0]*f[1])<<endl;
                        fractionSet.insert({key,string(f[0])+" x "+ string(f[1]),string(f[0]*f[1])});

                        f[0].setInteger(rand()%key_div);
                        f[1].setInteger(rand()%key_div+1);
                        // cout<<"f0: "<<string(f[0])<<" f1: "<<string(f[1])<<" op/: "<<string(f[0]/f[1])<<endl;
                        fractionSet.insert({key,string(f[0])+" ÷ "+ string(f[1]),string(f[0]/f[1])});

                        int tmp=rand()%key+1;
                        f[0].setInteger(tmp);
                        f[1].setInteger(rand()%tmp);
                        // cout<<"f0: "<<string(f[0])<<" f1: "<<string(f[1])<<" op-: "<<string(f[0]-f[1])<<endl;
                        fractionSet.insert({key,string(f[0])+" - "+ string(f[1]),string(f[0]-f[1])});

                    }catch(exception &e){
                        cout<<e.what()<<endl;
                    }
                }
            }
        }
        for(int i=1;i<MAX_STEP;++i){
            for(int t=0;t<i;++t){
                for(int j=0;j<MAX_SIZE;++j){
                    for(int k=0;k<j;++k){
                        int key=(*FracVector[i])[j].second>(*FracVector[t])[k].second?(*FracVector[i])[j].second:(*FracVector[t])[k].second;
                        Fraction f[2];

                        f[0]=Fraction((*FracVector[i])[j],rand()%key);
                        f[1]=Fraction((*FracVector[t])[k],rand()%key);
                        fractionSet.insert({key,string(f[0])+" + "+ string(f[1]),string(f[0]+f[1])});

                        f[0].setInteger(rand()%key);
                        f[1].setInteger(rand()%key);
                        fractionSet.insert({key,string(f[0])+" x "+ string(f[1]),string(f[0]*f[1])});

                        f[0].setInteger(rand()%key+2);
                        f[1].setInteger(rand()%key+1);
                        fractionSet.insert({key,string(f[0])+" ÷ "+ string(f[1]),string(f[0]/f[1])});

                        int tmp=rand()%key+1;
                        f[0].setInteger(tmp);
                        f[1].setInteger(rand()%tmp);
                        fractionSet.insert({key,string(f[0])+" - "+ string(f[1]),string(f[0]-f[1])});
                    }
                }
            }
        }
    }

    /*------------生成三元四则运算式子--------------------*/
    {
        std::copy(fractionSet.begin(), fractionSet.end(), back_inserter(_formula));
        for(auto it=_formula.begin();it!=_formula.end();++it){
            string formula=std::get<1>(*it);
            string answer=std::get<2>(*it);
            int max=std::get<0>(*it);
            if(max>16){
                break;
            }
            //为缩小难度故选用整数
            for(int i=1;i<MAX_INT;++i){
                int numtmp=rand()%5+1;
                auto tmpf1 = Fraction(numtmp);
                auto tmpf2 =Fraction::parse(answer);
                if(tmpf2>tmpf1){
                    fractionSet.insert({(max>i?max:i), formula+" - "+to_string(numtmp), string(tmpf2-tmpf1)});
                }

                fractionSet.insert({(max>i?max:i), formula+" + " + to_string(numtmp), string(tmpf2+tmpf1)});
                fractionSet.insert({(max>i?max:i), "( "+formula+" ) "+" x " + to_string(numtmp), string(tmpf2*tmpf1)});
            }
        }
        _formula.clear();
    }


    int index=0,max=0;
    for(auto it=fractionSet.begin();it!=fractionSet.end();++it){
        _formula.push_back(*it);
        auto tmp=std::get<0>(*it);

        if(tmp>max){
            _fast_indexs[tmp]=index;
            max=tmp;
        }

        ++index;
    }
    _fast_indexs[0]=index-1;

}

void Producer::randIndexs(){
    int i=0;
    for(auto &it: _randIndexs){
        it=i++;
    }
    for(int i=0;i<_randIndexs.size();++i){
        swap(_randIndexs[i],_randIndexs[rand()%_randIndexs.size()]);
    }
}

bool Producer::generate(size_t round, size_t num, ostream &os )
{
    int up=_fast_indexs[round];

    if(up<num){
        return false;
    }
    int index=rand()%(up-num);
    int counts=0;

    for(int i=0;i<num/100;++i){
        for(auto it:_randIndexs){
            auto tmp=_formula[it+index];
            os<<get<1>(tmp)+" = "<<get<2>(tmp)<<endl;
            ++counts;
        }
    }

    for(auto it:_randIndexs){
        if(counts==num){
            break;
        }
        auto tmp=_formula[it+index];
        os<<get<1>(tmp)+" = "<<get<2>(tmp)<<endl;
        ++counts;
    }
    return true;
}

vector<pair<string,string>> Producer::generate(size_t round, size_t num, bool iabort){
    vector<pair<string,string>> outputs;

    int up=_fast_indexs[round];

    if(up<num){
        if(iabort){
            return outputs;
        }
        num=up;
    }
    int index=rand()%(up-num);
    int counts=0;

    for(int i=0;i<num/100;++i){
        for(auto it:_randIndexs){
            auto tmp=_formula[it+index];
            outputs.push_back({get<1>(tmp),get<2>(tmp)});
            ++counts;
        }
    }

    for(auto it:_randIndexs){
        if(counts==num){
            break;
        }
        auto tmp=_formula[it+index];
        outputs.push_back({get<1>(tmp)+" = ",get<2>(tmp)});
        ++counts;
    }
    return outputs;
}

void Producer::getAllQuestions(ostream &questions)
{
    questions<<"题库（共10000道）"<<endl<<endl;
    int index=0;
    for(int i=0;i<100;++i){
        for(auto it:_randIndexs){
            auto tmp=_formula[it+i*100];
            questions<<++index<<". "<<get<1>(tmp)+" = "<<endl;
        }
    }
    questions<<endl<<endl<<"请下载答案核对";
}

void Producer::getAllAnswers(ostream &answers)
{
    answers<<"题库答案（共10000道）"<<endl<<endl;
    int index=0;
    for(int i=0;i<100;++i){
        for(auto it:_randIndexs){
            auto tmp=_formula[it+i*100];
            answers <<++index<<". "<< get<2>(tmp)<<endl;
        }
    }
}

void Producer::getAllQuestionAndAnswers(ostream &answers)
{
    answers<<"题库答案（共10000道）"<<endl<<endl;
    int index=0;
    for(int i=0;i<100;++i){
        for(auto it:_randIndexs){
            auto tmp=_formula[it+i*100];
            answers <<++index<<". "<<get<1>(tmp)+" = "<< get<2>(tmp)<<endl;
        }
    }
}




}   //namespace Server
