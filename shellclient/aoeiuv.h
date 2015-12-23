/*****************************************************
	^> File Name: aoeiuv.h
	^> Author: AoEiuV020
	^> Mail: 490674483@qq.com
	^> Created Time: 2015/08/19 - 01:16:50
****************************************************/
#ifndef AOEIUV_H
#define AOEIUV_H

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <signal.h>
#include <dirent.h>

#ifndef __STDC_VERSION__
#define __STDC_VERSION__ 0L
#endif
#define STDC99 199901L

#if __STDC_VERSION__ >= STDC99
int avscret;//aoeiuv syscall return
int _avscret; //should not be use, just for -Wunused-value
/*
#define avsyscall(syscall,...) \
	if((avscret=(syscall(__VA_ARGS__)))==-1) \
	{\
		averr(#syscall);\
        exit(errno);\
	}
// */
#define avsyscall(syscall,...) \
	(\
	 _avscret=\
	 ((((avscret=(syscall(__VA_ARGS__)))==-1)?\
	   (\
		averr(#syscall),\
		exit(errno),\
		0)\
	   :0),avscret)\
	)
// */
#endif

int averr(const char *,...);
int avout(const char *,...);
int favout(FILE *,const char *,...);
int avls(const char *,...);
int lsfile(const char *);
int lspath(const char *);
int avtmp(char *);
int avsystem(char *,...);

int averr(const char *format,...)
{
	va_list arg;
	int err;
	va_start(arg,format);
	err=vfprintf(stderr,format,arg);
	fprintf(stderr,": %s\n",strerror(errno));
	va_end(arg);
	return err;
}
int avout(const char *format,...)
{
	va_list arg;
	int err;
	va_start(arg,format);
	fflush(stdout);
	fsync(STDOUT_FILENO);
	err=vfprintf(stdout,format,arg);
	fprintf(stdout,"\n");
	fflush(stdout);
	fsync(STDOUT_FILENO);
	va_end(arg);
	return err;
}
int favout(FILE *fp,const char *format,...)
{
	va_list arg;
	int err;
	va_start(arg,format);
	fflush(fp);
	fsync(STDOUT_FILENO);
	err=vfprintf(fp,format,arg);
	fprintf(fp,"\n");
	fflush(fp);
	fsync(STDOUT_FILENO);
	va_end(arg);
	return err;
}
int avls(const char *format,...)
{
	char path[256];
	va_list arg;
	struct stat st;

	va_start(arg,format);
	vsnprintf(path,sizeof(path)-1,format,arg);
	va_end(arg);

	if(stat(path,&st)==-1)
	{
		averr("stat");
		exit(errno);
	}
	if(S_ISDIR(st.st_mode))
	{
		return lspath(path);
	}
	else
	{
		return lsfile(path);
	}
}
int lsfile(const char *file)
{
	char link[256]={0};
	struct stat lst;
	errno=0;
	lstat(file,&lst);
	fflush(stdout);
	fsync(STDOUT_FILENO);
	printf("%06o %d:%d %s",lst.st_mode,lst.st_uid,lst.st_gid,file);
	if(S_ISLNK(lst.st_mode))
	{
		memset(link,0,sizeof(link));
		readlink(file,link,sizeof(link)-1);
		printf(" -> %s",link);
	}
	printf("\n");
	fflush(stdout);
	fsync(STDOUT_FILENO);
	return errno;
}
int lspath(const char *path)
{
	char file[256]={0};
	char link[256]={0};
	DIR *dp;
	struct dirent *dirp;
	struct stat lst;
	errno=0;
	if((dp=opendir(path))==NULL)
	{
		averr("opendir");
		return errno;
	}
	fflush(stdout);
	fsync(STDOUT_FILENO);
	while((dirp=readdir(dp))!=NULL)
	{
		sprintf(file,"%s/%s",path,dirp->d_name);
		lstat(file,&lst);
		printf("%02d %06o %d:%d %s",dirp->d_type,lst.st_mode,lst.st_uid,lst.st_gid,dirp->d_name);
		if(dirp->d_type==10) /*DT_LNK*/
		/*if(S_ISLNK(lst.st_mode)) */
		{
			memset(link,0,sizeof(link));
			readlink(file,link,sizeof(link)-1);
			printf(" -> %s",link);
		}
		printf("\n");
	}
	fflush(stdout);
	fsync(STDOUT_FILENO);
	closedir(dp);
	return errno;
}
/*
   int fd;
   fd=avtmp();
   |or
   char *s="/tmp/t-XXXXXX";
   fd=avtmp(s);
// */
int avtmp(char *patharray)
{
	int fd;
	char tmpname[]="/tmp/XXXXXX.avtmp";
	char *p;
	p=patharray?patharray:tmpname;
	fd=patharray?mkstemp(patharray):mkstemps(tmpname,6);
	if(fd==-1)
	{
		averr("make temp %s",p);
		exit(errno);
	}
	if(-1==(unlink(p)))
	{
		averr("unlink %s",p);
		exit(errno);
	}
	return fd;
}
/*
   avsystem("ls -al /proc/%d/fd",getpid());
// */
int avsystem(char *format,...)
{
	va_list arg;
	int err;
	char cmd[1024];
	va_start(arg,format);
	vsnprintf(cmd,sizeof(cmd)-1,format,arg);
	err=system(cmd);
	va_end(arg);
	return err;
}

#endif

