// {
//     "uid" : "605325871",
//             "name" : "guo tian yu",
//                      "passwd" : "147258369",
//                                 "Email" : "605325871@qq.com"
// }

#include <iostream>
#include "json/json.h"
#include <fstream>
#include "myredis.hpp"

#include <cstdlib>
#include <time.h>

void menu()
{
    std::cout << "---------------------" << std::endl;
    std::cout << "-                   -" << std::endl;
    std::cout << "-     1.登陆         -" << std::endl;
    std::cout << "-     2.注册         -" << std::endl;
    std::cout << "-                   -" << std::endl;
    std::cout << "---------------------" << std::endl;
}
struct user
{
    std::string uid;
    std::string name;
    std::string passwd;
    std::string email;
    std::string peopleXid;
};

struct trueuser
{
    std::string name;
    std::string passwd;
};
class forJson
{
public:
    user us;
    trueuser tus;

public:
    forJson(/* args */)
    {

        srand((unsigned)time(NULL));
    }
    ~forJson() {}

    std::string createJson()
    {
        std::string jsonstr;
        Json::Value root;
        Json::StreamWriterBuilder writerbuilder;
        std::ostringstream os;

        std::cout << "请输入账号" << std::endl;
        std::cin >> us.uid;
        std::cout << "请输入昵称" << std::endl;
        std::cin >> us.name;
        std::cout << "请输入密码" << std::endl;
        std::cin >> us.passwd;
        std::cout << "请输入邮箱" << std::endl;
        std::cin >> us.email;

        us.peopleXid = std::to_string(rand() % 1000);
        root["uid"] = us.uid;
        root["name"] = us.name;
        root["passwd"] = us.passwd;
        root["email"] = us.email;

        std::unique_ptr<Json::StreamWriter> jsonswrite(writerbuilder.newStreamWriter());

        jsonswrite->write(root, &os);
        jsonstr = os.str();
        // std::cout << jsonstr << std::endl;
        return jsonstr;
    }

    bool parseJson(const std::string &info)
    {
        if (info.empty())
            return false;

        bool res;
        JSONCPP_STRING errs;
        Json::Value root;
        Json::CharReaderBuilder readerBuilder;

        std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
        res = jsonReader->parse(info.c_str(), info.c_str() + info.length(), &root, &errs);
        if (!res || !errs.empty())
        {
            std::cerr << "parseJson err. " << errs << std::endl;
        }
        tus.name = root["name"].asString();
        tus.passwd = root["passwd"].asString();
        return true;
    }
};

int main()
{
    std::string choice;
    std::string logname;
    std::string logpasswd;
    std::string fromjson;
    int truechoice;
    forJson user;
    Redis redis;
    redis.connect("127.0.0.1", 6379);
    while (true)
    {
        menu();
        std::cout << "请输入选择" << std::endl;
        std::cin >> choice;
        truechoice = atoi(choice.c_str());
        if (truechoice != 1 || truechoice != 2)
        {
            continue;
        }
        else if (truechoice == 1)
        {
            int count_key = 3;
            do
            {
                std::cout << "请输入账号" << std::endl;
                std::cin >> logname;
                if ((redis.get(logname)) == "nil")
                {
                    std::cout << "未注册" << std::endl;
                    break;
                }
                else
                {
                    user.parseJson(redis.get(logname));
                    std::cin >> logpasswd;
                    if (logpasswd == user.tus.passwd)
                    {
                        count_key = 0;
                    }
                    else
                    {
                        std::cout << "还有" << count_key << "机会" << std::endl;
                        count_key--;
                    }
                }

            } while (count_key);
        }
        else if (truechoice == 2)
        {
            auto str = user.createJson();
            redis.set(user.us.uid, str);
        }
    }
}
