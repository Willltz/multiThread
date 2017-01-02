#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <iostream>
#include <pthread.h>
using namespace std;
/*
 * 完美的出租车程序!!!
*/

/*提示出租车到达的条件变量*/
pthread_cond_t taxiCond = PTHREAD_COND_INITIALIZER;
/*同步锁*/
pthread_mutex_t taxiMutex = PTHREAD_MUTEX_INITIALIZER;

//增加一个计数器记录等待线程的个数，在决定触发条件变量前检查该变量!!!!
int travelerCound=0;

void * traveler_arrive(void * name){
	cout<<"Traveler: "<<(char *)name<<" needs a taxi now!"<<endl;
	pthread_mutex_lock(&taxiMutex);//乘客来了后，进入等待，等待taxi
	travelerCound++;//等待线程的个数+1
    pthread_cond_wait(&taxiCond,&taxiMutex);
    pthread_mutex_unlock(&taxiMutex);
	cout<<"Traveler: "<<(char *)name<<" now got a taxi!"<<endl;
	pthread_exit((void*)0);
}

void * taxi_arrive(void * name){
	cout<<"Taxi: "<<(char *)name<<" arrives."<<endl;
	while(1){//加入while永真循环，保证先来的出租车也能检测到是否有乘客到达！！！
		pthread_mutex_lock(&taxiMutex);
		if(travelerCound>0){//taxi到达后，如果有乘客在等待，就会招呼乘客上车
			pthread_cond_signal(&taxiCond);
			pthread_mutex_unlock(&taxiMutex);
            break;//如果有乘客上车，那么该taxi也就退出等待
		}
		pthread_mutex_unlock(&taxiMutex);
	}
	pthread_exit((void*)0);
}

int main(){
	pthread_t tids[3];
	int iRet = pthread_create(&tids[0],NULL,taxi_arrive,(void*)(" Jack "));
	if(iRet){
		printf("pthread_create error: iRet=%d\n",iRet);
		return iRet;
	}
	printf("Time passing by.\n");
	sleep(1);
	iRet = pthread_create(&tids[1],NULL,traveler_arrive,(void*)(" Susan "));
	if(iRet){
		printf("pthread_create error: iRet=%d\n",iRet);
		return iRet;
	}
	printf("Time passing by.\n");
	sleep(1);
	iRet = pthread_create(&tids[2],NULL,taxi_arrive,(void*)(" Mike "));
	if(iRet){
		printf("pthread_create error: iRet=%d\n",iRet);
		return iRet;
	}
	printf("Time passing by.\n");
	sleep(1);
	void *retval;
	for(int i=0;i<3;i++){
		iRet=pthread_join(tids[i],&retval);
		if (iRet){
			printf("pthread_join error: iRet=%d\n",iRet);
			return iRet;
		}
		printf("retval=%ld\n",(long)retval);
	}
    return 0;
}
