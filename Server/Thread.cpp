#include "Thread.h"  
#include <iostream>  
#include <stdio.h> 
#include <deque>

void CTask::SetConnFd(int data)  
{  
    connfd = data;  
}  
 
int CTask::GetConnFd()
{
    return connfd;
}
/**
* 初始化数据
*/
deque<CTask*> CThreadPool::m_deqTaskList;         //任务列表  
bool CThreadPool::shutdown = false;  
      
pthread_mutex_t CThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;   
pthread_cond_t CThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;  

/** 
 * 线程池管理类构造函数 
 */  
CThreadPool::CThreadPool(int threadNum)  
{  
    this->m_iThreadNum = threadNum;  
  //  cout << "最多允许客户端数量 " << threadNum  << endl;  
    Create();       //*创建对象时便创建线程。
}
 
 
/** 
 * 线程回调函数 
 */  
void* CThreadPool::ThreadFunc(void* threadData)  
{  
    pthread_t tid = pthread_self();  
    while (1)  
    {  
 
        //* 线程开启时先上锁 */
        pthread_mutex_lock(&m_pthreadMutex);  
        while (m_deqTaskList.size() == 0 && !shutdown)  
        {  
            //* 没有任务时，线程等待状态（条件变量）*/
            pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);  
        }  
          
        if (shutdown)  
        {  
            pthread_mutex_unlock(&m_pthreadMutex);  
          //  printf("线程 %lu 与客户端断开连接\n", pthread_self());  
            pthread_exit(NULL);   
        }  
          
      //  printf("线程 %lu 已连接客户端\n", tid);  
            
        /** 
        * 取任务队列并处理之 
        */ 
 
        //deque<CTask*>::iterator iter = m_deqTaskList.front();
        CTask* task = m_deqTaskList.front();  
        m_deqTaskList.pop_front();
 
        //* 取完任务后释放锁*/
        pthread_mutex_unlock(&m_pthreadMutex);  
          
        task->Run(); /** 执行任务 */  
         
    }  
    return (void*)0;  
}  

/** 
 * 往任务队列里边添加任务并发出线程同步信号 
 */  
int CThreadPool::AddTask(CTask *task)  
{  
    pthread_mutex_lock(&m_pthreadMutex);  
    this->m_deqTaskList.push_back(task);  
    pthread_mutex_unlock(&m_pthreadMutex); 
 
    // * 添加任务 条件变量发信号，非阻塞  */
    pthread_cond_signal(&m_pthreadCond);       
    return 0;  
}  

/** 
 * 创建线程 
 */  
int CThreadPool::Create()  
{  
    pthread_id = (pthread_t*)malloc(sizeof(pthread_t) * m_iThreadNum);  
    for(int i = 0; i < m_iThreadNum; i++)  
    {  
        pthread_create(&pthread_id[i], NULL, ThreadFunc, NULL);  
    }  
    return 0;  
}  
 
/** 
 * 停止所有线程 
 */  
int CThreadPool::StopAll()  
{  
    /** 避免重复调用 */  
    if (shutdown)  
    {  
        return -1;    
    }  
    printf("Now I will end all threads!!\n");  
    /** 唤醒所有等待线程，线程池要销毁了 */  
    shutdown = true;  
    pthread_cond_broadcast(&m_pthreadCond);  
      
    /** 阻塞等待线程退出，否则就成僵尸了 */  
    for (int i = 0; i < m_iThreadNum; i++)  
    {  
        pthread_join(pthread_id[i], NULL);    
    }  
      
    free(pthread_id);  
    pthread_id = NULL;  
      
    /** 销毁条件变量和互斥体 */  
    pthread_mutex_destroy(&m_pthreadMutex);  
    pthread_cond_destroy(&m_pthreadCond);  
      
    return 0;  
}  
 
/** 
 * 获取当前队列中任务数 
 */  
int CThreadPool::getTaskSize()  
{  
    return m_deqTaskList.size();      
}  

