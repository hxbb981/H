#pragma once
#include <iostream>
#include <jsoncpp/json/json.h>
#include "Socket.hpp"
using namespace LogMudel;
using namespace SocketMoudel;
// using func_t = std::function<Response(Request &req)>;,定义在后面

class Request
{
public:
    Request()
    {
    }
    Request(int x, int y, char oper)
        : _x(x), _y(y), _oper(oper) {}

    // 序列化
    std::string serialize()
    {
        // 将结构体中的消息转化成字符串
        Json::Value root;
        root["x"] = _x;
        root["y"] = _y;
        root["oper"] = _oper;
        Json::FastWriter writer;
        std::string s = writer.write(root); // 转化为字符串
        return s;
    }

    // 反序列化
    void deserialize(std::string &str)
    {
        Json::Value root;
        Json::Reader reader;
        bool ok = reader.parse(str, root); // 将str中的数据格式化到root中
        if (ok)
        {
            _x = root["x"].asInt();
            _y = root["y"].asInt(); // 大小写
            _oper = root["oper"].asInt();
        }
    }
    int Getx() { return _x; }
    int Gety() { return _y; }
    char Getchar() { return _oper; }

    ~Request() {}

private:
    int _x;
    int _y;
    char _oper;
};

class Response
{

public:
    Response() {};
    Response(int result, int code) : _result(result), _code(code) {};

    // 序列化
    std::string serialize()
    {
        // 将结构体中的消息转化成字符串
        Json::Value root;
        root["result"] = _result;
        root["code"] = _code;
        Json::FastWriter writer;
        std::string s = writer.write(root); // 转化为字符串
        return s;
    }

    // 反序列化
    void deserialize(std::string &str)
    {
        Json::Value root;
        Json::Reader reader;
        bool ok = reader.parse(str, root); // 将str中的数据格式化到root中
        if (ok)
        {
            _result = root["result"].asInt();
            _code = root["code"].asInt();
        }
    }

    void Showresult()
    {
        std::cout<<_result << std::endl;
    }

    void Setresult(int result)
    {
        _result = result;
    }
    int GetResult()
    {
        return _result;
    }
    void Setcode(int code)
    {
        _code = code;
    }

    ~Response() {}

private:
    int _result; // 计算的结果
    int _code;   // 不同出错的情况
};

const std::string sep = "\r\n";
using fun = std::function<Response(Request &req)>;

class Protocol
{
public:
    Protocol()
    {
    }
    Protocol(fun fnc)
        : func(fnc)
    {
    }
    std::string Encode(std::string &str)
    {
        std::string len = std::to_string(str.size());
        return len + sep + str + sep;
    }

    bool Decode(std::string &buffer, std::string *package)
    {
        // 我们拿到了对应的buffer，对他进行校验
        ssize_t pos = buffer.find(sep);
        if (pos == std::string::npos)
        {
            return false;
        }
        // 能够得到前面的抱头

        std::string package_len_str = buffer.substr(0, pos);
        int package_len = std::stoi(package_len_str);

        int all_len = package_len_str.size() + package_len + sep.size() * 2;

        if (buffer.size() >= all_len)
        {
            // 说明能够得到一个完整的包头
            std::string message = buffer.substr(pos + sep.size(), package_len); // 从pos位置开始，提取package_len个字符
            *package = message;
            buffer.erase(0, all_len);
            return true;
        }
        return false;
    }

    bool Getresponse(std::shared_ptr<Socket> fd, std::string &message, Response *rese)
    {
        // 向fd中读数据
        while (true)
        {
            // 从缓冲区读数据
            int m = fd->Recv(&message);
            if (m > 0)
            {
                // 检查是否得到完整串
                std::string buffer;
                // bool is_complate = Decode(message, &buffer);

                // if (!is_complate)
                // {
                //     continue;
                // }
                // // 一定得到了字串 就是buffer

                // // 对buffer进行反序列化
                while (Decode(message, &buffer))
                {
                    std::cout<<buffer<<std::endl;

                    rese->deserialize(buffer);
                }
                return true;
            }
            else if (m == 0)
            {
                std::cout << "quit" << std::endl;
                return false;
            }
            else
            {
                std::cout << "error" << std::endl;
                return false;
            }
        }
    }

    void GetRequest(std::shared_ptr<Socket> fd, InetAddr &clie)
    {
        // 这里正是我们所要执行的函数
        std::string buffer_queue;
        while (true)
        {
            // 从缓冲区中读字符串
            int m = fd->Recv(&buffer_queue);
            if (m > 0)
            {
                // 说明从缓冲区中读到了字符串
                // 1:解析包头
                std::string _package;
                bool is_complate = Decode(buffer_queue, &_package);
                if (is_complate)
                {
                    // 得到了一个完整的包头，对他进行反序列化
                    std::cout << _package << std::endl;

                    Request requ; // 空构造
                    requ.deserialize(_package);

                    // 计算结果
                    Response res = func(requ);

                    // res.Showresult();

                    // 对计算结果进行序列化
                    std::string rese = res.serialize();

                    // 对字符串添加包头
                    std::string send_str = Encode(rese);

                    // 发送数据

                    fd->Send(send_str);
                }
            }
            else if (m == 0)
            {
                LOG(Loglevel::DEBUG) << "quit";
                exit(QUIT);
            }
            else
            {
                LOG(Loglevel::DEBUG) << "recv error";
                exit(RECV_ERROR);
            }
        }
    }
    std::string sendrequest(int x, int y, char oper)
    {
        Request req(x, y, oper);

        // 将消息序列化
        std::string str = req.serialize();

        // 添加包头

        std::string send_str = Encode(str);

        return send_str;
    }

private:
    fun func;
};