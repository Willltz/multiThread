#include <stdio.h>
#include <pthread.h>
//获得线程id两种方式:
//(1)在线程调用函数中使用pthread_self函数来获得线程id
//(2)创建函数时生成线程id
void* func(void* arg){
	printf("thread id in pthread=%lu\n", pthread_self());//(1)
	pthread_exit((void*)1);
}
int main(){
	int i=10;
    pthread_t tid;
    int iRet = pthread_create(&tid, NULL, func, &i);//(2)tid
    if (iRet){
	    printf("pthread_create error: iRet=%d\n",iRet);
	    return iRet;
    }
	printf("thread id in process=%lu\n", tid);
	void *retval;
	iRet=pthread_join(tid,&retval);
	if (iRet){
	    printf("pthread_join error: iRet=%d\n",iRet);
	    return iRet;
    }
	printf("retval=%ld\n",(long)retval);
	return 0;
}
