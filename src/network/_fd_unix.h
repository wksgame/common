#include<unistd.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<netinet/in.h>		//sockaddr_in
#include<arpa/inet.h>		//ip地址转换
#include<errno.h>

bool SetSocketNoBlock(const int sock);