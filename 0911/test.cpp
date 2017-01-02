#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
//使用pthread_mutex_trylock尝试加锁
pthread_mutex_t mutex_x= PTHREAD_MUTEX_INITIALIZER;//全局的互斥锁:静态方式创建锁
int total_ticket_num=20;
void *sell_ticket1(void *arg){
	for(int i=0;i<20;i++){
		pthread_mutex_lock(&mutex_x);//上锁
		if(total_ticket_num>0){
			printf("thread1 sell the %dth ticket\n",20-total_ticket_num+1);
			total_ticket_num--;
		}
        sleep(1);
		pthread_mutex_unlock(&mutex_x);//解锁
		//sleep(1);
	}
	return 0;
}
void *sell_ticket2(void *arg){
    int iRet=0;
	for(int i=0;i<10;i++){
	    iRet=pthread_mutex_trylock(&mutex_x);//使用pthread_mutex_trylock尝试加锁
	    if(iRet==EBUSY){
	        printf ("sell_ticket2:the variable is locked by sell_ticket1.\n");
	    }else if(iRet==0){
		if(total_ticket_num>0){
		    printf("thread2 sell the %dth ticket\n",20-total_ticket_num+1);
		    total_ticket_num--;
		}
		pthread_mutex_unlock(&mutex_x);//解锁
	    }
	    sleep(1);
	}
	return 0;
}

int main(){
    pthread_t tids[2];
	int iRet = pthread_create(&tids[0], NULL, &sell_ticket1, NULL);//创建线程1
	if(iRet){
	    printf("pthread_create error, iRet=%d\n",iRet);
	    return iRet;
	}
	iRet = pthread_create(&tids[1], NULL, &sell_ticket2, NULL);//创建线程2
	if(iRet){
	    printf("pthread_create error, iRet=%d\n",iRet);
	    return iRet;
	}
	sleep(5);
	void *retval;
	iRet=pthread_join(tids[0], &retval);//
	if(iRet){
	    printf("tid=%ld join error, iRet=%d\n",tids[0],iRet);
	}else{
	    printf("retval=%ld\n",(long)retval);
	}
	iRet=pthread_join(tids[1], &retval);//
	if(iRet){
	    printf("tid=%ld join error, iRet=%d\n",tids[1],iRet);
	}else{
	    printf("retval=%ld\n",(long)retval);
	}
    return 0;
}
