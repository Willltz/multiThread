#include <stdio.h>
#include <pthread.h>
#include <string.h>
//线程调用函数时传入一个结构体:
//若要向pthread_create()中传入多个参数，可以把他们放在一个结构体里
struct arg_type{
	int a;
	char b[100];
};
void* say_hello(void* args){
	arg_type arg_temp=*(arg_type*)args;
	printf("hello from thread,arg_temp.a=%d, arg_temp.b=%s\n",arg_temp.a,arg_temp.b);
	pthread_exit((void*)1);
}
int main(){
	pthread_t tid;
	arg_type arg_temp;
	arg_temp.a=10;
	char temp[100]="I am number one.";
	strncpy(arg_temp.b,temp,sizeof(temp));//把tmp数组的内容放到结构体对应数组中
    int iRet = pthread_create(&tid, NULL, say_hello, &arg_temp);
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
