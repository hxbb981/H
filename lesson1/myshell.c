#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<cstring>
#define MAX 100
#define FORMAT "[%s:%s]"
#define MAXARGC 128
#define NONE_REDIR 0
#define INPUT_REDIR 1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3

//命令行参数表
char*arg_c[MAXARGC];
int argc=0;

//重定向方式以及文件名
int way=NONE_REDIR
std::string filename

void Initenv(){
		

}
bool forwardcommandline(char forline[]){
	int start=0;
	int end=strlen(forline)-1;
	while(start<end){
		if(forline[end]=='<'){
			forline[end++]=0;
			trinspace(forline,end);
			way=INPUT_REDIR;
			filename=forline+end;
		}
		else if(forline[end]=='>'){
			if(forline[end-1]=='>'){
				forline[end-1]=0;
				way=APPEND_REDIR;
			}
			else{
				way=OUTPUT_REDIR;
			}
			forline[end++]=0;
			trinspace(forlline,end);
			filename=forline+end;
		}
		else{
			end--;
		}
	}
}
void trimspace(char forline[],int &end){

	while(forline[end]){
		end++;
	}
}
void Execute(){
	pid_t id=fork();
	int fd=-1;
	if(id==0){
			if(way==INPUT_REDIR){
				fd=open(filenamme.c_str,o_RDONLY);
				if(fd<0) exit(1);
				dup2(fd,1);
			}
			else if(way==OUTPUT_REDIR){
				fd=open(filename.c_str,O_CREAT|O_WRONLY|O_TRUNC,0666);
				if(fd<0)exit(1);
				dup2(fd,0);
			}
			else if(way==APPEND_REDIR){
				fd=open(filename.c_str,O_CREAT|O_WRONLY|O_APPEND,0666);
				if(fd<0) exit(1);
				dup2(fd,0);
			}
			else{}
		execvp(arg_c[0],arg_c);
		exit(1);
	}
	int status=0;
	pid_t rid=waitpid(id,&status,0);
	if(rid>0){
		int lastcode=WEXITSTATUS(status);
	
	}
	return;

}
bool commandanalyse(char*anline){
#define ESP " "
	arg_c[argc++]=strtok(anline,ESP);
	while((bool)(arg_c[argc++]=strtok(nullptr,ESP)));
	argc--;
	return argc>0?true:false;
}



bool getcommandline(char* line,int size){
	char*str=fgets(line,size,stdin);
	//去掉\n,enter即为\n
	if(str==NULL)return false;
	str[strlen(str)-1]=0;
	if(strlen(str)-1==0)return false;
	return true;
}
const char*getname(){
	const char*name=getenv("NAME");
	return name==NULL?"NULL":name; 
}
const char*getpwd(){
	const char*pwd=getenv("PWD");
	return pwd==NULL?"NULL":pwd;
}
void printcommandline(){
	char str[MAX];
	snprintf(str,sizeof(str),FORMAT,getname(),getpwd());
	printf("%s",str);
	fflush(stdout);
}

int main(){
	//获取环境变量表
	
	Initenv();

	while(true){
	//打印命令行参数
	
	printcommandline();

	//获取命令并对命令进行处理
	
	char commandline[MAX];
	if(!getcommandline(commandline,sizeof(commandline)))
			continue;
	//对命令进行分析“ls -a -l” “ls”“-a”“-l”
	if(!commandanalyse(commandline))
		continue;
	//对重定向获取打开方式和文件名
	if(!forwardcommandline(commandline));
	//执行命令
	Execute();

}

	return 0;

}
