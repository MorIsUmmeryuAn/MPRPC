#include<iostream>
#include<string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

using namespace fixbug;

/*
userservice 原来是一个本地服务 提供了两个进程内的本地方法login和getfriendlists
*/
class UserService : public fixbug::UserServiceRpc{ //使用在rpc服务发布端(rpc服务提供者)
public:
    bool Login(std::string name,std::string pwd){
        std::cout<<"doing local service : Login"<<std::endl;
        std::cout<<"name:"<< name <<"pwd:"<< pwd << std::endl;
        return false;
    }

    bool Register(uint32_t id,std::string name,std::string pwd){
        std::cout<<"doing local service : Register"<<std::endl;
        std::cout<<"id:"<< id <<"name:"<< name <<"pwd:"<< pwd << std::endl;
        return true;
    }

    /*
    重写基类usrpc的虚函数 下面这些方法都是框架直接调用
    1.caller ===> login(loginrequest) ==> muduo ===> callee
    2.callee ===> login(loginrequest) ==> 交给重写的这个login方法上
    */
    void Login(::google::protobuf::RpcController* controller,const ::fixbug::LoginRequest* request,::fixbug::LoginResponse* response,::google::protobuf::Closure* done)
    {
        //框架给业务上报了请求参数 loginrequest 业务获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool loginresult = Login(name,pwd);//做本地业务

        //把响应写入 错误码 错误消息 返回值
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(loginresult);

        //执行回调操作 执行响应对象数据的序列化和网络发送 都由框架完成
        done->Run();
    }

    void Register(::google::protobuf::RpcController* controller,const ::fixbug::RegisterRequest* request,::fixbug::RegisterResponse* response,::google::protobuf::Closure* done)
    {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id,name,pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_success(ret);

        done->Run();
    }
};

int main(int argc,char **argv){
    //调用框架的初始化操作
    MprpcApplication::Init(argc,argv);
    
    //provider是一个rpc网络服务对象
    //把userservice对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());
    
    //启动一个rpc服务发布节点 run之后进程进入阻塞状态 等待远程的rpc调用请求
    provider.Run();

    return 0;
}