#include <stdio.h>
#include <pthread.h>
//向线程传递参数:
//例:在线程调用函数时传递int类型的参数
void* say_hello(void* args){
	int i=*(int*)args;
	printf("hello from thread,i=%d\n",i);
	pthread_exit((void*)1);
}
int main(){
	pthread_t tid;
	int para=10;//传递的参数,
    int iRet = pthread_create(&tid, NULL, say_hello, &para);
    if (iRet){
	    printf("pthread_create error: iRet=%d\n",iRet);
	    return iRet;
    }
	void *retval;
	iRet=pthread_join(tid,&retval);
	if (iRet){
	    printf("pthread_join error: iRet=%d\n",iRet);
	    return iRet;
    }
	printf("retval=%ld\n",(long)retval);
	return 0;
}
