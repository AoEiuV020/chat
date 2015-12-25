/*****************************************************
	^> File Name: chat-jni.c
	^> Author: AoEiuV020
	^> Mail: 490674483@qq.com
	^> Created Time: 2015/12/24 - 16:56:59
****************************************************/
#include <jni.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_addr
#include <sys/select.h>
#include <android/log.h>
#include <pthread.h>
#define LOG_TAG "JNIChat"
int sfd=-1;
struct sockaddr_in sai;
unsigned short port;
const char *ip;
const char *message;
jstring Java_com_aoeiuv020_chat_Main_errtostr(JNIEnv* env,jobject thiz,jint err)
{
	const char *str;
	/*
	str =err==1?"make socket err"
		:err==2?"connect err"
		:"other err";
		// */
	str=strerror(err);
	return (*env)->NewStringUTF(env,str);
}
jint Java_com_aoeiuv020_chat_Main_recv(JNIEnv* env,jobject thiz)
{
	jclass cmain=(*env)->GetObjectClass(env,thiz);
	jmethodID addmessage=(*env)->GetMethodID(env,cmain,"addmessage","(Ljava/lang/String;)V");
	jstring message;
	message=(*env)->NewStringUTF(env,"hello");
	(*env)->CallVoidMethod(env,thiz,addmessage,message);
	return (jint)0;
}
jint Java_com_aoeiuv020_chat_Main_send(JNIEnv* env,jobject thiz,jstring jmessage)
{
	const char *buf;
	buf=(*env)->GetStringUTFChars(env,jmessage,0);
	errno=0;
	write(sfd,buf,strlen(buf));
	return (jint)errno;
}
jint Java_com_aoeiuv020_chat_Main_connect(JNIEnv* env,jobject thiz,jstring jip,jint jport)
{
	if(port==(short)jport&&!strcmp(ip,(*env)->GetStringUTFChars(env,jip,0)))
	{
		return (jint)0;
	}
	if(sfd!=-1)
	{
		close(sfd);
		sfd=-1;
	}
	port=(short)jport;
	ip=(*env)->GetStringUTFChars(env,jip,0);

	memset(&sai,0,sizeof(sai));
	sai.sin_family=AF_INET;
	sai.sin_port=htons(port);
	sai.sin_addr.s_addr=inet_addr(ip);
	errno=0;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{
		return (jint)errno;
	}
	if(-1==connect(sfd,(const struct sockaddr *)&sai,sizeof(sai)))
	{
		sfd=-1;
		return (jint)errno;
	}
	return (jint)0;
}
