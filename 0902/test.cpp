#include <stdio.h>
#include <pthread.h>
//创建线程时传入类的成员函数:
//如果线程调用的函数是在一个类中，应该把该函数写成静态成员函数
class Hello{
public:
    static void* say_hello(void* args){
	/*线程的运行函数，必须void*，没说的表示返回通用指针、输入通用指针*/
	    printf("hello from thread\n");
	    pthread_exit((void*)1);
    }
};
int main(){
	pthread_t tid;
    int iRet = pthread_create(&tid, NULL, Hello::say_hello, NULL);
	/*参数依次是：创建的线程id，线程参数，调用函数名，传入的函数参数*/
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
	//printf("retval=%d\n",(int)retval);//这样会丢失精度
	return 0;
}
