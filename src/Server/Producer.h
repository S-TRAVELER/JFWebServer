#ifndef JF_SERVER_PRODUCER_H
#define JF_SERVER_PRODUCER_H

#include <iostream>
#include <memory>
#include <set>
#include <vector>

#include "Server/Fraction.h"
#include "Util/Util.h"

namespace Server {

#define MAX_SIZE 15
#define MAX_STEP 5
#define MAX_INT 2

using namespace std;

typedef std::pair<int, int> IntPair;
typedef vector<IntPair> PairVector;

class Producer
{
public:
    static Producer& Instance();

    ~Producer(){}
    bool generate(size_t round, size_t num, ostream &os = std::cout);
    vector<pair<string,string>> generate(size_t round, size_t num,bool iabort);

    void randIndexs();

    void getAllQuestions(ostream &questions);
    void getAllAnswers(ostream &answers);
    void getAllQuestionAndAnswers(ostream &answers);

private:
    Producer() ;
private:
    vector<int> _fast_indexs;
    vector<tuple<int,string,string>> _formula;
    vector<int> _randIndexs;
};


}


#endif  // JF_SERVER_PRODUCER_H
