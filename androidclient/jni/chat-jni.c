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
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)) 
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)) 
int sfd=-1;
struct sockaddr_in sai;
unsigned short port;
const char *ip;
const char *message;
pthread_t precv;
JavaVM* gjvm;
jobject gthiz;
jmethodID gaddmessage;
void addmessage(JNIEnv *env,jobject thiz,const char *str)
{
	jstring message;
	message=(*env)->NewStringUTF(env,str);
	(*env)->CallVoidMethod(env,gthiz,gaddmessage,message);
}
void* JNI_addmessage(void *arg)
{
	JNIEnv* env;
	(*gjvm)->AttachCurrentThread(gjvm,&env,NULL);
	int i=0;
	while(1<10)
	{
		addmessage(env,gthiz,"hello");
		sleep(1);
	}
	// */
	(*gjvm)->DetachCurrentThread(gjvm);
	return arg;
}
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
JNIEXPORT jint Java_com_aoeiuv020_chat_Main_init(JNIEnv* env,jobject thiz)
{
	(*env)->GetJavaVM(env,&gjvm);
	gthiz=(*env)->NewGlobalRef(env,thiz);
	jclass cmain=(*env)->GetObjectClass(env,gthiz);
	LOGE("cmain=%p,",cmain);
	gaddmessage=(*env)->GetMethodID(env,cmain,"addmessage","(Ljava/lang/String;)V");
	LOGE("gaddmessage=%p,",gaddmessage);
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
	if(port==(unsigned short)jport&&!strcmp(ip,(*env)->GetStringUTFChars(env,jip,0)))
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
	addmessage(env,gthiz,ip);
	pthread_create(&precv,NULL,JNI_addmessage,NULL);
	sleep(3);
//	pthread_join(precv,NULL);
	return (jint)0;
}
