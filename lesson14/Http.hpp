#include "Tcpserver.hpp"
#include "Tools.hpp"
#include <unordered_map>
#include <sstream>

const std::string rep = "\r\n";

const std::string path = "./myhtml";

const std::string slash = "/";

const std::string page_404 = "error.html";

const std::string point = ".";

const std::string gspace=" ";

const std::string gline=": ";

class HttpResponse
{
public:
    HttpResponse()  : _version("HTTP/1.0"),_blankline(" "){}

    void SetTargetfile(std::string uri)
    {
        _targetfile = uri;
    }

    void Setcode(int code)
    {
        switch (code)
        {
        case 200:
            _stades = "OK";
            break;
        case 302:
            _stades = "Found";
             break;
        case 404:
            _stades = "Not Found";
             break;
        }
    }
    void SetHeader(const std::string &key, const std::string &value)
    {

        auto pos = _kv.find(key);
        if (pos == _kv.end())
        {
            return;
        }
        _kv.insert(std::make_pair(key, value));
    }

    std::string Uri2Suffix(std::string targetfile)
    {
        // 从我们的kv中查找字段
        //./a/b/c.html;
        auto pos = targetfile.rfind(point);

        if (pos == std::string::npos)
        {
            // 没有就html类型
            return "html.text";
        }
        std::string filekind = targetfile.substr(pos + 1);

        if (filekind == "html")
            return "html.text";
        else if (filekind == "jpg")
            return "image/jpeg";
        else
        {
            return std::string();
            //......
        }
    }

    void MakeResponse()
    {

        if(_targetfile=="./myhtml/favicon.ico"){
            return ;
        }
        LOG(Loglevel::DEBUG)<<_targetfile;

        bool r = Tools::Readtargetfile(_targetfile, &_text);

        
        if (!r)
        {
            // 读取失败了,返回404页面
            _text = "";
            Setcode(404);
            LOG(Loglevel::DEBUG) << "you want file not found";

            _targetfile = path + slash + page_404;

            Tools::Readtargetfile(_targetfile, &_text);
            //设置kv，一个长度字段，一个类型字段
            //1长度
            //int filesize=Tools::FileSize(_text);

            std::string filesize=std::to_string(Tools::FileSize(_text));

            //2:类型
            std::string filekind=Uri2Suffix(_targetfile);

            //设置kv

            SetHeader("Content-Type",filekind);

            SetHeader("Content-Length",filesize);
        }
        else
        {
            // 读取成功
            LOG(Loglevel::DEBUG) << "require message:" << _text;

            Setcode(200);

            //设置kv

            std::string filesize=std::to_string(Tools::FileSize(_text));

            std::string filekind=Uri2Suffix(_targetfile);

            SetHeader("Content-Type",filekind);
            SetHeader("Content-Length",filesize);

        }
    }

    std::string Serialize(){
        //分开来拼写
        //1：请求包头
        std::string requeststr=_version+gspace+std::to_string(_code)+gspace+_stades+rep;//最后的\r\n呢

        //2:kv
        std::string resp_header;

        for(auto &header:_kv){
            std::string resp=header.first+gline+header.second+rep;
            resp_header+=resp;
        }

        return requeststr+resp_header+rep+_text;
    }

private:
    // 响应报文

    // 1：version
    std::string _version;
    // 2:状态码
    int _code;
    // 3:状态描述符
    std::string _stades;
    // 4:kv
    std::unordered_map<std::string, std::string> _kv;
    // 5:空行
    std::string _blankline;
    // 6:正文
    std::string _text;

    // 其他
    std::string _targetfile;
};

class HttpRequest
{
public:
    HttpRequest()
    {
    }

    void Parsereqline(std::string &str)
    {
        std::stringstream ss(str);
        ss >> _method >> _uri >> _version;
    }

    std::string Geturi()
    {
        return _uri;
    }

    // 获得了一个完整的http请求
    bool Deserialize(std::string &requstr)
    {
        // 提取请求行
        std::string requline;

        if (Tools::Readoneline(requstr, &requline, rep))
        {
            // 将请求行分割并初始化
            Parsereqline(requline);
            if(_uri=="/favicon.ico"){
                return false;
            }
            LOG(Loglevel::DEBUG) << "method : " << _method;
            LOG(Loglevel::DEBUG) << "uri : " << _uri;
            LOG(Loglevel::DEBUG) << "_version : " << _version;


            if(_uri=="/"){
                _uri=path+_uri+"text.html";
            }
            else{
                _uri=path+_uri;
            }
            return true;
        }
        return true;
    }

private:
    // 请求报文

    // 1：方法 uri 版本
    std::string _method;
    std::string _uri;
    std::string _version;
    // 2: key value
    std::unordered_map<std::string, std::string> _kv;
    // 3:空行
    std::string _blankline;
    // 4:正文
    std::string _text;
};

class Http
{
public:
    Http(uint16_t port) : tcp(std::make_unique<TcpServer>(port))
    {
    }
    void Start()
    {
        tcp->Start([this](std::shared_ptr<Socket> &s, InetAddr &addr)
                   { this->GetRequest(s, addr); });
    }

    void GetRequest(std::shared_ptr<Socket> &s, InetAddr &addr)
    {

        std::string requeststr;
        // 我们认为它得到了一个完整的请求
        int n = s->Recv(&requeststr);
        if (n > 0)
        {
            HttpRequest req;
            if(!req.Deserialize(requeststr)){
                return;
            };
            HttpResponse res;
            res.SetTargetfile(req.Geturi());

            res.MakeResponse();

            std::string responsestr=res.Serialize();

            std::cout << "-----------" << responsestr << std::endl;           
            s->Send(responsestr);
        }
    }

private:
    std::unique_ptr<TcpServer> tcp;
};