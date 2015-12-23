# chat
自己在不同设备间聊天传文件的程序，
最简单的聊天室服务器，
只有一个房间，
还有几个bug，
1,3到最高的所有file descriptor都发送，哪怕已经退出了，
2,file descriptor最高的人退出时没有减fdmax,
