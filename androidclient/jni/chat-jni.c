/*****************************************************
	^> File Name: stat.c
	^> Author: AoEiuV020
	^> Mail: 490674483@qq.com
	^> Created Time: 2015/08/12 - 21:26:37
****************************************************/
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
void statshow(struct stat *pstat);
void showtime(time_t *time);
int main(int argc, char **argv)
{
	struct stat fs,s,ls;
	char *F;
	int fd;
	if(argc<1)
	{
		fprintf(stderr,"argc<1");
		return 1;
	}
	F=argv[1];
	stat(F,&s);
	statshow(&s);
	return 0;
}
void statshow(struct stat *pstat)
{
	/*
	printf("%o,%lX,%lX,%lX;",pstat->st_mode,pstat->st_ino,pstat->st_dev,pstat->st_rdev);
	printf("%d:%d-%ld;",pstat->st_uid,pstat->st_gid,pstat->st_size);
	// */
	showtime(&pstat->st_atime);//access,last read?
	showtime(&pstat->st_mtime);//modify,last write?
	showtime(&pstat->st_ctime);//change,last change?
	/*
	printf("%ld*%ld=%ld;",pstat->st_blksize,pstat->st_blocks,pstat->st_blksize*pstat->st_blocks);
	printf("%lu;",pstat->st_nlink);
	// */
	printf("\n");
}
void showtime(time_t *time)
{
	char buf[30]={0};
	struct tm *timetm;
	timetm=localtime(time);
	//printf("%s;",asctime(timetm));
	strftime(buf,sizeof(buf)-1,"%G.%m.%d %H:%M:%S %a",timetm);
	printf("%s;",buf);
}
