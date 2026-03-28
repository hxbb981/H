#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define PATH "."
#define Name "myfifo"
class fifoname
{
public:
    fifoname( const char *path, const char *name) : _path(path),
                                       _name(name)
    {
        _fifoname = _path + "/" + _name;
        umask(0);
        int n = mkfifo(_fifoname.c_str(), 0666);
        if (n == 0)
        {
            std::cout << "管道创建成功" << std::endl;
        }
        else
        {
            perror("mkfifo");
        }
    }
    ~fifoname()
    {
        int n = unlink(_fifoname.c_str());
        if (n == 0)
        {
            std::cout << "管道已经被删除" << std::endl;
        }
        else
        {
            perror("unlink");
        }
    }

private:
    std::string _path;
    std::string _name;
    std::string _fifoname;
};

class fifoopener
{
public:
    fifoopener(const char *path, const char *name) : _path(path),
                                         _name(name),
                                         _fd(-1)
    {
        //_path(path);不可以这样写，这样编译器认为这是函数调用，因为_path在进入这个函数体前已经被初始化过了，所以被当成是函数调用
        _fifoname = _path + "/" + _name;
    }
    ~fifoopener()
    {
    }
    int openforwrite()
    {
        int fd = open(_fifoname.c_str(), O_WRONLY);
        if (fd == -1)
        {
            perror("open");
        }
        else
        {
            _fd = fd;
            return fd;
        }
    }
    int openforread()
    {
        int fd = open(_fifoname.c_str(), O_RDONLY);
        if (fd == -1)
        {
            perror("open");
        }
        else
        {
            _fd = fd;
            return fd;
        }
    }
    void Read()
    {
        while (true)
        {
            char buffer[1024];
            int n = read(_fd, buffer, sizeof(buffer) - 1);
            if (n < 0)
            {
                perror("read");
            }
            else if (n == 0)
            {
                std::cout << "serve退出，我也退出" << std::endl;
            }
            else
            {
                buffer[n] = 0; // 我们给他设置终止符，否则会读到二进制的乱码
                std::cout << "我们读到得数据为：" << buffer << std::endl;
            }
        }
    }
    void Write()
    {
        while (true)
        {
            char buffer[1024];
            std::cout << "请输入你向文件中所写得数据" << std::endl;
            std::cin >> buffer;
            int n = write(_fd, buffer, sizeof(buffer) - 1);
            if (n == -1)
            {
                perror("write");
            }
            else
            {
                std::cout << "你向文件中所写得数据为：" << buffer << std::endl;
            }
        }
    }
    void Close(){
        close(_fd);
    }

private:
    std::string _path;
    std::string _name;
    std::string _fifoname;
    int _fd;
};
