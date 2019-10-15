#include <Poco/ThreadPool.h>
#include <Poco/TaskManager.h>
#include <vector>
#include <functional>
#include <unistd.h>

using namespace std;


class MTask:public Poco::Task{
public:
    typedef function<void()> Func;
    MTask(const std::string& strName) :
        Poco::Task(strName){}
    void setFunc(Func fun){
        _fun=fun;
    }
    void runTask()override{
        if(_fun){
             _fun();
        }
    }
private:
    Func _fun;
};


int main(){
    bool _exit=false;
    int maxCapacity(8);
    Poco::ThreadPool threadpool(2, maxCapacity, 60, POCO_THREAD_STACK_SIZE);
    Poco::TaskManager tm(threadpool);
    for(size_t i = 0; i < maxCapacity-1; i++){
        MTask* myTask = new MTask("Task" + std::to_string(i));
        myTask->setFunc([&,i](){
            cout<<"hh:"<<i<<endl;
            if(_exit)return;
            threadpool.start(*myTask);
        });
        threadpool.start(*myTask);
    }
    sleep(10);
    _exit=true;
    threadpool.joinAll();

    return 0;
}

