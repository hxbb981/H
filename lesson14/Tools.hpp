


class Tools{


    public:
    static bool Readoneline(std::string&str,std::string*reline,const std::string divider="\r\n"){
        auto pos=str.find(divider);
        if(pos==std::string::npos){
            return false;
        }

        *reline=str.substr(0,pos);

        str.erase(0,pos+=divider.size());

        return true;

    }

    static bool Readtargetfile(std::string filename,std::string*buffer){
        //读
        std::ifstream in(filename);
        if(!in.is_open()){
            return false;
        }
        std::string line;

        while(std::getline(in,line)){
            *buffer+=line;
        }

        in.close();
        return true;
    }

    static int FileSize(std::string&text){
        std::ifstream in(text,std::ios::binary);//以二进制的形式打开文件

        if(!in.is_open()){
            return -1;
        }

        in.seekg(0,in.end);//将指针移动到文件末尾

        int filesize=in.tellg();//获取文件大小

        in.seekg(in.beg);//将指针移动到文件开始位置

        in.close();


        return filesize;

    }

};