#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;

int main(){
    // LoginResponse rsp;
    // ResultCode* rc = rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("登录处理失败");

    GetFriendListsResponse rsp;
    ResultCode* rc = rsp.mutable_result();
    rc->set_errcode(0);

    User* usr1 = rsp.add_friend_list();
    usr1->set_name("张三");
    usr1->set_age(18);
    usr1->set_sex(User::MAN);

    User* usr2 = rsp.add_friend_list();
    usr2->set_name("李四");
    usr2->set_age(18);
    usr2->set_sex(User::MAN);

    std::cout<<rsp.friend_list_size()<<std::endl;


    return 0;
}

int main2()
{
    // 封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    // 对象数据序列化 =》 char*
    std::string send_str;
    if (req.SerializeToString(&send_str))
    {
        std::cout << send_str.c_str() << std::endl;
    }

    // 从send_str反序列化一个login请求对象
    LoginRequest reqB;
    if (reqB.ParseFromString(send_str))
    {
        std::cout << reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    }

    return 0;
}