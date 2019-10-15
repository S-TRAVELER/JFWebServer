#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/StreamCopier.h"
#include "Poco/JSON/Object.h"
#include "Poco/Net/DNS.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <stdlib.h>
#include <Poco/ThreadPool.h>
#include <Poco/TaskManager.h>
#include <vector>
#include <functional>
#include <unistd.h>

#include <Server/CalculatorRecord.h>
#include "Server/Producer.h"

using namespace std;
using namespace Server;

using Poco::Net::ServerSocket;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using namespace Poco::JSON;
using Poco::Net::DNS;
using Poco::Net::HostEntry;
 
/*
poco提供了HTTPServer类创建一个http服务器
官网介绍:TCPServer的子类，实现了全功能的多线程HTTP服务器。
		 必须提供HTTPRequestHandlerFactory。 ServerSocket必须绑定并处于侦听状态。
		 要配置服务器的各个方面，可以将HTTPServerParams对象传递给构造函数
HTTPServer有3个构造函数，用其中一个测试就行
HTTPServer(HTTPRequestHandlerFactory::Ptr pFactory, const ServerSocket& socket, HTTPServerParams::Ptr pParams)
*/
 
 
/*
HTTPRequestHandler官网介绍：
Derived classes must override the handleRequest() method. Furthermore, a HTTPRequestHandlerFactory must be provided
翻译：派生类必须覆盖handleRequest（）方法。此外，必须提供HTTPRequestHandlerFactory
The handleRequest() method must perform the complete handling of the HTTP request connection.As soon as the handleRequest() method returns, the request handler object is destroyed.
A new HTTPRequestHandler object will be created for each new HTTP request that is received by the HTTPServer.
翻译：handleRequest（）方法必须执行HTTP请求连接的完整处理。 一旦handleRequest（）方法返回，请求处理程序对象就被销毁。
	  将为HTTPServer接收的每个新HTTP请求创建一个新的HTTPRequestHandler对象。
handleRequest()函数功能：
	  virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) = 0
	  Must be overridden by subclasses：必须被子类覆盖
	  Handles the given request：处理给定的请求
*/

map<string,shared_ptr<CalculatorRecord>> idRecords;
Poco::ThreadPool threadpool(4, 16, 60, POCO_THREAD_STACK_SIZE);
Poco::TaskManager tmanger(threadpool);

class MTask:public Poco::Task{
public:
    typedef function<void()> Func;
    MTask(const std::string& strName,Func fun=nullptr) :
        Poco::Task(strName),
        _fun(fun){}
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


class RequestHandLer :public HTTPRequestHandler
{
public:
 
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {
    /*    std::istream &i = request.stream();
        int len = request.getContentLength();
        char* buffer = new char[len+1];
        i.read(buffer, len);
        cout<<"buffer: "<<buffer<<endl;*/

        response.setChunkedTransferEncoding(true);//设置分组传输编码
        response.setContentType("text/html; charset=UTF-8");//设置内容类型
        response.set("Vary", "Accept-Encoding");

		std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流

        std::fstream test_html("./test.html", ios::in);

        if(test_html.is_open()){
            ostringstream tmp;
            tmp<<test_html.rdbuf();
            ostr<<tmp.str();
        }else{
            response.setStatus(to_string(404));
            ostr<<"<h1>404</h1>";
        }
        test_html.close();

	}
 
};

class GetHandLer :public HTTPRequestHandler
{
public:

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {
        std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流

        const string &copy_URI=request.getURI();
        size_t pos=copy_URI.rfind("/");

        if(pos!=copy_URI.npos){
            string fileName=copy_URI.substr(pos+1);
            cout<<fileName<<endl;
            std::fstream file(fileName, ifstream::binary|ios::in);

            if(fileName=="ip"){
                const HostEntry& entry = DNS::thisHost();

                const HostEntry::AddressList& addrs = entry.addresses();
                HostEntry::AddressList::const_iterator addr_it = addrs.begin();
                for (; addr_it != addrs.end(); ++addr_it){
                    response.setContentType("text/html");//设置内容类型
                    ostr<<"Address: " << addr_it->toString() << std::endl;
                    std::cout << "Address: " << addr_it->toString() << std::endl;
                }
            }else if(file.is_open()){
                response.setContentType("image/png");//设置内容类型
                ostringstream tmp;
                tmp<<file.rdbuf();
                ostr<<tmp.str();
                file.close();
            }else{
                response.setContentType("text/html");//设置内容类型
                response.setStatus(to_string(404));
                ostr<<"<h1>404</h1>";
            }
        }else{
            response.setContentType("text/html");//设置内容类型
            response.setStatus(to_string(404));
            ostr<<"<h1>404</h1>";
        }

    }

};

class FaviconHandLer :public HTTPRequestHandler
{
public:

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {
        response.setContentType("image/png");//设置内容类型

        std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流
        char buffer[4*1024]{'\0'};

        std::fstream pic("./jf_icon.png", ifstream::binary|ios::in);

        if(pic.is_open()){
            ostringstream tmp;
            tmp<<pic.rdbuf();
            ostr<<tmp.str();
        }else{
            response.setStatus(to_string(404));
            ostr<<"<h1>404</h1>";
        }
        pic.close();
    }

};

class CheckHandLer :public HTTPRequestHandler
{
public:

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {

        string method=request.getURI().substr(request.getURI().rfind('/')+1);

        if(method=="id"){
            response.setContentType("text/html; charset=utf-8");//设置内容类型
            response.set("Vary", "Accept-Encoding");
            std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流
            string id=request.getHost();
            string tmp=id+to_string(rand()%100);
            idRecords[tmp]=nullptr;
//            ostr<<"\"id\":"<<"\""<<tmp<<"\""<<endl;
            ostr<<tmp;
        }else{
            string tmp=request.get("id");
            auto it=idRecords.find(tmp);
            if(it==idRecords.end()){
                response.setChunkedTransferEncoding(true);//设置分组传输编码
                response.setContentType("text/plain");//设置内容类型
                response.set("Vary", "Accept-Encoding");
                std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流
                ostr<<"ERROR";
                return;
            }

            if(method=="question"){
                 it->second.reset(new CalculatorRecord);
                 std::istream& rs = request.stream();
                 std::string requestText;
                 Poco::StreamCopier copier;
                 copier.copyToString(rs, requestText);
                 size_t iStart=0,pos;
                 cout<<requestText<<endl;
                 for(int i=0;i<3;++i){
                     if((pos=requestText.find('\n',iStart))!=requestText.npos){
                        iStart=pos+1;
                     }
                 }

                 while((pos=requestText.find('\n',iStart))!=requestText.npos){
                    it->second->push_back(requestText.substr(iStart,pos-iStart));
                    iStart=pos+1;
                 }

                 response.setChunkedTransferEncoding(true);//设置分组传输编码
                 response.setContentType("text/plain");//设置内容类型
                 response.set("Vary", "Accept-Encoding");
                 std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流
                 ostr<<"OK";
                 auto tmp_record=it->second;
                 MTask* task=new MTask(it->first,[=](){
                     tmp_record->compute();
                     cout<<"finish"<<endl;
                 });
                 tmanger.start(task);

            }else if(method=="answer"){
                response.set("Vary", "Accept-Encoding");
                response.setChunkedTransferEncoding(true);//设置分组传输编码

                response.setContentType("text/plain");//设置内容类型

                std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流
                it->second->Foreach([&](const string & it){
                    ostr<<it<<endl;
                });

                idRecords.erase(tmp);
                cout<<idRecords.size()<<endl;
            }else if(method=="rate"){
                response.set("Vary", "Accept-Encoding");
                response.setChunkedTransferEncoding(true);//设置分组传输编码
                int rate=it->second->getProgressRate();
                response.setContentType("text/html");//设置内容类型
                std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流
                ostr<<to_string(rate);
            }else{
                response.setContentType("text/html; charset=utf-8");//设置内容类型
                response.set("Vary", "Accept-Encoding");
                std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流
                response.setStatus("404");
                ostr<<"404";

            }

        }
    }

};


class DownloadHandLer :public HTTPRequestHandler
{
public:

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {


        response.setChunkedTransferEncoding(true);//设置分组传输编码
        response.setContentType("text/plain");//设置内容类型
        response.set("Vary", "Accept-Encoding");

        std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流

        const string &copy_URI=request.getURI();
        size_t pos=copy_URI.rfind("/");

        if(pos!=copy_URI.npos){
            string fileName=copy_URI.substr(pos+1);
            if(fileName=="question"){
                Producer::Instance().getAllQuestions(ostr);
            }else if(fileName=="answer"){
                Producer::Instance().getAllAnswers(ostr);
            }else{
                response.setStatus(to_string(404));
                ostr<<"<h1>404</h1>";
            }
        }else{
            response.setStatus(to_string(404));
            ostr<<"<h1>404</h1>";
        }

    }

};

class NotFoundHanler :public HTTPRequestHandler
{
public:

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {

        response.setChunkedTransferEncoding(true);//设置分组传输编码
        response.setContentType("text/html; charset=UTF-8");//设置内容类型
        response.set("Vary", "Accept-Encoding");

        std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流
        response.setStatus(to_string(404));
        ostr<<"<h1>404</h1>";
    }
};

class QuestionsHandLer :public HTTPRequestHandler
{
public:

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {
        string URI=request.getURI();
        size_t range=0;
        {
            smatch result;
            regex rangeMatch("range=([0-9]+)");
            if(regex_search(URI.cbegin(),URI.cend(),result,rangeMatch)){
                range=atoi(string(result[1]).c_str());
            }
        }

        response.setChunkedTransferEncoding(true);//设置分组传输编码
        response.setContentType("application/json");//设置内容类型
        response.set("Vary", "Accept-Encoding");

        std::ostream& ostr = response.send();//这个函数返回一个响应请求的输出流
        ostringstream ss;

        Object total;
        Poco::JSON::Array datas;

        vector<pair<string,string>> formulas=Producer::Instance().generate(range,10,false);
        for(auto &it: formulas){
            Object tmp;
            tmp.set("question",it.first);
            tmp.set("answer",it.second);
            datas.add(tmp);
        }
        total.set("data",datas);
        total.set("message","success");
        total.set("statusCode",200);


        total.stringify(ss);
        ostr<<ss.str();
        cout<<ss.str()<<endl;
    }

};
 
class RequestHandlerFactory :public HTTPRequestHandlerFactory
{
public:
	/*
	为给定的HTTP请求创建一个新的请求处理程序，参数是HTTPServerRequest，上面要定义一个HTTPServerRequest对象传过来
	该方法应该检查给定的HTTPServerRequest对象（例如方法）
	和URI），并创建一个适当的HTTPRequestHandler对象来处理
	请求
	*/
	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
        string URI=request.getURI();
        string firstPram;
        smatch result;
        regex URL("/[a-z]*");
        if(regex_search(URI.cbegin(),URI.cend(),result,URL)){
            firstPram=result[0];
        }
        cout<<"hh"<<URI<<endl;
        if(firstPram=="/favicon"){
            return new FaviconHandLer();
        }else if(firstPram=="/question"){
            return new QuestionsHandLer();
        }else if(firstPram=="/"){
            return new RequestHandLer();
        }else if(firstPram=="/download"){
            return new DownloadHandLer();
        }else if(firstPram=="/get"){
            return new GetHandLer();
        }else if(firstPram=="/check"){
            return new CheckHandLer();
        }else{
            return new NotFoundHanler();
        }
	}
};
 
 
/*
创建一个应用程序ServerApplication
HTTPServer(HTTPRequestHandlerFactory::Ptr pFactory, const ServerSocket& socket, HTTPServerParams::Ptr pParams)
*/
 
class Myapp :public ServerApplication
{
protected:
	int main(const std::vector<std::string>& args)
	{
 
		HTTPServer HSer(new RequestHandlerFactory, ServerSocket(8080), new HTTPServerParams);// ServerSocket是匿名对象
		HSer.start();//启动http服务器
		waitForTerminationRequest();//
		std::cout << "关闭" << std::endl;
		HSer.stop();
		return Application::EXIT_OK;
 
	}
};

int main(int argc, char** argv)
{
    Myapp app;
	return app.run(argc, argv);
}
