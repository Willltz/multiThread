#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define THREADNUM 5
pthread_rwlock_t rwlock;//定义一个读写锁
void *readers(void *arg){
    pthread_rwlock_rdlock(&rwlock);//获取一个读出锁
    printf("reader %ld got the lock\n", (long)arg);
    pthread_rwlock_unlock(&rwlock);//释放锁
    pthread_exit((void*)0);
}
void *writers(void *arg){
    pthread_rwlock_wrlock(&rwlock);//获取一个写入锁
    printf("writer %ld got the lock\n", (long)arg);
    pthread_rwlock_unlock(&rwlock);//释放锁
    pthread_exit((void*)0);
}
int main(int argc, char **argv){
    int iRet, i;
    pthread_t writer_id, reader_id;
    pthread_attr_t attr;
    int nreadercount = 1, nwritercount = 1;
    iRet = pthread_rwlock_init(&rwlock, NULL);//初始化读写锁（动态）
    if (iRet) {
        fprintf(stderr, "init lock failed\n");
        return iRet;
    }
    pthread_attr_init(&attr);//初始化一个线程对象的属性,要用pthread_attr_destroy对其去除初始化
    /*pthread_attr_setdetachstate用来设置线程的分离状态
    也就是说一个线程怎么样终止自己，状态设置为PTHREAD_CREATE_DETACHED
    表示以分离状态启动线程*/
    /*线程的分离状态决定一个线程以什么样的方式来终止自己。在默认情况下线程是非分离状态的，这种
     * 情况下，原有的线程等待创建的线程结束。只有当pthread_join（）函数返回时，创建的线程才算
     * 终止，才能释放自己占用的系统资源。而分离线程不是这样子的，它没有被其他的线程所等待，
     * 自己运行结束了，线程也就终止了，马上释放系统资源。*/
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    for (i = 0; i < THREADNUM; i++){
        if (i % 3) {//3个线程调用readers函数
            pthread_create(&reader_id, &attr, readers, (void *)nreadercount);
            printf("create reader %d\n", nreadercount++);
        } else {//2个线程调用writers函数
            pthread_create(&writer_id, &attr, writers, (void *)nwritercount);
            printf("create writer %d\n", nwritercount++);
        }
    }
    /*一定要有sleep，否则其他线程不会执行 ！！！*/
    sleep(5);/*sleep是为了等待另外的线程的执行*/
    pthread_attr_destroy(&attr);//补入
    pthread_rwlock_destroy(&rwlock);//补入
    return 0;
}
