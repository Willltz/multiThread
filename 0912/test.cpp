#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
using namespace std;
//使用条件变量:常和互斥锁配合使用
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;    /*初始构造条件变量*/
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;    /*初始构造锁*/
int x = 10;
int y = 20;
void *func1(void *arg){
	cout<<"func1 start"<<endl;
    pthread_mutex_lock(&qlock);//上锁
    while(x<y)//如果x<y,会阻塞等待,否则继续
    {
        pthread_cond_wait(&qready,&qlock);//条件等待
    }
    pthread_mutex_unlock(&qlock);//解锁
    sleep(3);
	cout<<"func1 end"<<endl;
}
void *func2(void *arg){
	cout<<"func2 start"<<endl;
    pthread_mutex_lock(&qlock);//上锁
    x = 20;
    y = 10;
    cout<<"has change x and y"<<endl;
    pthread_mutex_unlock(&qlock);//解锁
    if(x > y){//发送信号给一个正在处于阻塞状态的线程，使其继续执行
        pthread_cond_signal(&qready);//激活一个等待该条件的线程
    }
    cout<<"func2 end"<<endl;
}
int main(int argc,char **argv){
	pthread_t tid1,tid2;
    int iRet;
    iRet = pthread_create(&tid1,NULL,func1,NULL);//创建线程1，执行函数func1
    if(iRet){
        cout<<"pthread 1 create error"<<endl;
		return iRet;
    }
	sleep(2);//为了确保线程1先执行，创建线程2前先sleep2秒
    iRet = pthread_create(&tid2,NULL,func2,NULL);//创建线程2，执行函数func2
    if(iRet){
        cout<<"pthread 2 create error"<<endl;
		return iRet;
    }
    sleep(5);
    return 0;
}
