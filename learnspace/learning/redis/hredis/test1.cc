#include <iostream>
#include <hiredis/hiredis.h>
#include<stdio.h>
class readis
{
private:
    redisContext *_connect; // redisContext 管理上
    redisReply *_reply;

public:
    readis(/* args */) {}
    ~readis()
    {
        _connect = nullptr;
        _reply = nullptr;
    }

    bool conecttoredis(std::string host, int port)
    {
        _connect = redisConnect(host.c_str(), port);
        if (_connect == nullptr && _connect->err)
        {
            std::cerr << _connect->errstr << std::endl;
            return false;
        }
        return true;
    }
    std::string get(std::string key)
    {
        _reply = (redisReply *)redisCommand(_connect, "GET %s", key.c_str());
        std::string str = _reply->str;
        freeReplyObject(_reply);
        return str;
    }
    void set(std::string key,std::string value)
    {
        redisCommand(_connect,"SET %s %s",key.c_str(),value.c_str());
        
    }
    //如果redis设置了密码，则需要密码登录
	void auth(std::string password){
		redisCommand(this->_connect, "auth %s", password.c_str());
	}
};

int main()
{
	readis *r = new readis();
	if(!r->conecttoredis("127.0.0.1", 6379))
	{
		printf("connect error!\n");
		return 0;
	}
	r->auth("123456");
	r->set("name", "Andy");
	printf("Get the name is %s\n", r->get("name").c_str());
	delete r;
	return 0;
}
