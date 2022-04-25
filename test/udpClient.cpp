#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<thread>
int udp_socket_fd;

void input(){
    printf("进入线程\n");
    char buf[1024] = {0};
	//2 循环发送数据
    while (true)
    {
		printf("Please input msg:\n");
		scanf("%s",buf);//从键盘输入获取要发送的消息
        write(udp_socket_fd, buf, strlen(buf)); 
        if(strcmp(buf, "exit") == 0)
		{
			break;//退出循环
		}
		memset(buf,0,sizeof(buf));//清空存留消息
    }
}
int main(int argc, char * argv[])
{
	//检查命令行参数是否匹配
	if(argc != 3)
	{
		printf("请传递对方的ip和端口号");
		return -1;
	}
	
	int port = atoi(argv[2]);//从命令行获取端口号
	if( port<1025 || port>65535 )//0~1024一般给系统使用，一共可以分配到65535
	{
		printf("端口号范围应为1025~65535");
		return -1;
	}
     int reuse = 1;
	struct sockaddr_in my_addr = {0};
    my_addr.sin_family = PF_INET;//使用IPv4协议
    my_addr.sin_port = htons(8002);//设置接收方端口号
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //设置接收方IP 
    unsigned int addr_length=sizeof(my_addr);
	//1 创建udp通信socket
	udp_socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
    int ret = setsockopt(udp_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse,sizeof(reuse));
    if (ret) {
        exit(1);
    }
    
    ret = setsockopt(udp_socket_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));
    if (ret) {
        exit(1);
    }
        
    bind(udp_socket_fd, (struct sockaddr *) &my_addr, sizeof(my_addr));
	if(udp_socket_fd == -1)
	{
		perror("socket failed!\n");
		return -1;
	}

    struct sockaddr_in dest_addr = {0};
    dest_addr.sin_family = PF_INET;//使用IPv4协议
    dest_addr.sin_port = htons(port);//设置接收方端口号
    dest_addr.sin_addr.s_addr = inet_addr(argv[1]); //设置接收方IP 
    connect(udp_socket_fd, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr));
    if (connect(udp_socket_fd, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr)) == -1) {
        perror("chid connect");
        exit(1);
    } else {

    }
	//设置目的IP地址
    //启动线程
    printf("启动线程\n");
    std::thread t(input);
    
	char buf[1024] = {0};
    
	while(true){
        int re=read(udp_socket_fd, buf, 1024); 
       
        buf[re]='\0';
        printf("%d %s\n",re,buf);
    }
    
	//3 关闭通信socket
    close(udp_socket_fd);
    return 0;
}