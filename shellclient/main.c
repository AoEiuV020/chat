/*****************************************************
	^> File Name: in_client.c
	^> Author: AoEiuV020
	^> Mail: 490674483@qq.com
	^> Created Time: 2015/12/01 - 20:46:33
****************************************************/
#include "aoeiuv.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_addr
#include <sys/select.h>
int main(int argc, char **argv)
{
	char IP[30]="127.0.0.1";
	unsigned short PORT=45678;
	if(argc>1)
	{
		sprintf(IP,"%s",argv[1]);
	}
	if(argc>2)
	{
		sscanf(argv[2],"%hu",&PORT);
	}
	struct sockaddr_in sai;
	sai.sin_family=AF_INET;
	sai.sin_port=htons(PORT);
	sai.sin_addr.s_addr=inet_addr(IP);
	favout(stderr,"family=%x,port=%hu,addr=%s;",sai.sin_family,ntohs(sai.sin_port),inet_ntoa(sai.sin_addr));
	int cfd;
	cfd=avsyscall(socket,AF_INET,SOCK_STREAM,0);
	favout(stderr,"cfd=%d,",cfd);
	avsyscall(connect,cfd,(const struct sockaddr *)&sai,sizeof(sai));
	
	fd_set set,readset;//socketfd,stdinfd,
	int fdmax;
	FD_ZERO(&set);
	FD_SET(STDIN_FILENO,&set);
	FD_SET(cfd,&set);
	fdmax=cfd;
	char buf[512];
	int len;
	while(1)
	{
		readset=set;
		avsyscall(select,fdmax+1,&readset,NULL,NULL,NULL);
		if(FD_ISSET(STDIN_FILENO,&readset))
		{
			if(0<(len=avsyscall(read,STDIN_FILENO,buf,sizeof(buf)-1)))
			{
				buf[len]=0;
				avsyscall(write,cfd,buf,len);
			}
			else
			{
				favout(stderr,"end of stdin...");
				FD_CLR(STDIN_FILENO,&set);
				avsyscall(close,STDIN_FILENO);
				break;
			}
		}
		if(FD_ISSET(cfd,&readset))
		{
			if(0<(len=avsyscall(read,cfd,buf,sizeof(buf)-1)))
			{
				buf[len]=0;
				avsyscall(write,STDOUT_FILENO,buf,len);
			}
			else
			{
				favout(stderr,"end of %d...",cfd);
				FD_CLR(cfd,&set);
				avsyscall(close,cfd);
				break;
			}
		}

	}

	return 0;
}
