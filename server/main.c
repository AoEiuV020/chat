/*****************************************************
	^> File Name: main.c
	^> Author: AoEiuV020
	^> Mail: 490674483@qq.com
	^> Created Time: 2015/12/22 - 13:09:57
****************************************************/
#include "aoeiuv.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_addr
#include <sys/select.h>
#include <event.h>
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
	int sfd;
	sfd=avsyscall(socket,AF_INET,SOCK_STREAM,0);
	favout(stderr,"sfd=%d,",sfd);
	int flag=1;
	avsyscall(setsockopt,sfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
	avsyscall(bind,sfd,(const struct sockaddr *)&sai,sizeof(sai));
	avsyscall(listen,sfd,3);

	fd_set set,readset;//socketfd,connectfd,
	int fdmax;
	FD_ZERO(&set);
	FD_SET(STDIN_FILENO,&set);
	FD_SET(sfd,&set);
	fdmax=sfd;
	char buf[512];
	int len;
	int newfd;
	int fdi;//index,
	struct sockaddr_in csai;
	socklen_t clen;
	while(1)
	{
		readset=set;
		avsyscall(select,fdmax+1,&readset,NULL,NULL,NULL);
		for(fdi=0;fdi<=fdmax;++fdi)
		{
			if(FD_ISSET(fdi,&readset))
			{
				if(fdi==STDIN_FILENO)
				{
					if(0<(len=avsyscall(read,STDIN_FILENO,buf,sizeof(buf)-1)))
					{
						buf[len]=0;
						if(!strcmp(buf,"exit\n"))
						{
							favout(stderr,"logout...");
							exit(0);
						}
					}
					else
					{
						favout(stderr,"end of stdin...");
						FD_CLR(STDIN_FILENO,&set);
						avsyscall(close,STDIN_FILENO);
					}
				}
				else if(fdi==sfd)
				{
					newfd=avsyscall(accept,sfd,(struct sockaddr *)&csai,&clen);
					favout(stderr,"new client %d,%s:%hu",newfd,inet_ntoa(csai.sin_addr),ntohs(csai.sin_port));
					FD_SET(newfd,&set);
					if(newfd>fdmax)
					{
						fdmax=newfd;
					}
				}
				else
				{
					if(0<(len=read(fdi,buf,sizeof(buf)-1)))
					{

						buf[len]=0;
						int fdj;
						for(fdj=3;fdj<=fdmax;++fdj)
						{
						favout(stderr,"%d,",
     (((( &readset)->__fds_bits)[(( fdj) / (8 * (int) sizeof (__fd_mask)))] & ((__fd_mask) (1UL << (( fdj) % (8 * (int) sizeof (__fd_mask)))))) )
	 );
							if(fdj==sfd||fdj==fdi)
							{
								continue;
							}
							avsyscall(write,fdj,buf,len);
						}
					}
					else
					{
						favout(stderr,"end of %d...",fdi);
						FD_CLR(fdi,&set);
						avsyscall(close,fdi);
					}
				}
			}
		}
	}
	return 0;
}
