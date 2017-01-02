#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define THREADNUM 5
pthread_rwlock_t rwlock;//����һ����д��
void *readers(void *arg){
    pthread_rwlock_rdlock(&rwlock);//��ȡһ��������
    printf("reader %ld got the lock\n", (long)arg);
    pthread_rwlock_unlock(&rwlock);//�ͷ���
    pthread_exit((void*)0);
}
void *writers(void *arg){
    pthread_rwlock_wrlock(&rwlock);//��ȡһ��д����
    printf("writer %ld got the lock\n", (long)arg);
    pthread_rwlock_unlock(&rwlock);//�ͷ���
    pthread_exit((void*)0);
}
int main(int argc, char **argv){
    int iRet, i;
    pthread_t writer_id, reader_id;
    pthread_attr_t attr;
    int nreadercount = 1, nwritercount = 1;
    iRet = pthread_rwlock_init(&rwlock, NULL);//��ʼ����д������̬��
    if (iRet) {
        fprintf(stderr, "init lock failed\n");
        return iRet;
    }
    pthread_attr_init(&attr);//��ʼ��һ���̶߳��������,Ҫ��pthread_attr_destroy����ȥ����ʼ��
    /*pthread_attr_setdetachstate���������̵߳ķ���״̬
    Ҳ����˵һ���߳���ô����ֹ�Լ���״̬����ΪPTHREAD_CREATE_DETACHED
    ��ʾ�Է���״̬�����߳�*/
    /*�̵߳ķ���״̬����һ���߳���ʲô���ķ�ʽ����ֹ�Լ�����Ĭ��������߳��ǷǷ���״̬�ģ�����
     * ����£�ԭ�е��̵߳ȴ��������߳̽�����ֻ�е�pthread_join������������ʱ���������̲߳���
     * ��ֹ�������ͷ��Լ�ռ�õ�ϵͳ��Դ���������̲߳��������ӵģ���û�б��������߳����ȴ���
     * �Լ����н����ˣ��߳�Ҳ����ֹ�ˣ������ͷ�ϵͳ��Դ��*/
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    for (i = 0; i < THREADNUM; i++){
        if (i % 3) {//3���̵߳���readers����
            pthread_create(&reader_id, &attr, readers, (void *)nreadercount);
            printf("create reader %d\n", nreadercount++);
        } else {//2���̵߳���writers����
            pthread_create(&writer_id, &attr, writers, (void *)nwritercount);
            printf("create writer %d\n", nwritercount++);
        }
    }
    /*һ��Ҫ��sleep�����������̲߳���ִ�� ������*/
    sleep(5);/*sleep��Ϊ�˵ȴ�������̵߳�ִ��*/
    pthread_attr_destroy(&attr);//����
    pthread_rwlock_destroy(&rwlock);//����
    return 0;
}
