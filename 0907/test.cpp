#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
//分离一个线程
void * tfn1(void * arg){
    printf("the thread\n");
    return NULL;
}
int main(void){
    int iRet;
    pthread_t tid;
    pthread_attr_t attr;//线程属性对象
    iRet = pthread_attr_init(&attr);//初始化
    if(iRet){
        printf("can't init attr %s/n", strerror(iRet));
	    return iRet;
    }
    //设置线程的分离属性为分离状态
    iRet = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(iRet){
        printf("can't set attr %s\n", strerror(iRet));
	    return iRet;
    }
    //创建一个线程，带上这个属性
    iRet = pthread_create(&tid, &attr, tfn1, NULL);
    if(iRet){
        printf("can't create thread %s\n", strerror(iRet));
	    return iRet;
    }
    //由于状态分离，因此得不到线程结束状态信息，pthread_join函数会出错
    iRet = pthread_join(tid, NULL);
    if(iRet){
        printf("thread has been detached\n");
	    return iRet;
    }
    return 0;
}
