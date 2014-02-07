#ifndef _NANDSEMPHORE_H_
#define _NANDSEMPHORE_H_

/************* NandSemaphore **********/
typedef int NandSemaphore;
void __InitSemaphore(NandSemaphore*sem,int val); //val 1 is unLock val 0 is Lock
void __DeinitSemaphore(NandSemaphore* sem);
void __Semaphore_wait(NandSemaphore* sem);
int __Semaphore_waittimeout(NandSemaphore* sem,long jiffies); //timeout return < 0
void __Semaphore_signal(NandSemaphore* sem);

/************** andMutex **************/
typedef int NandMutex;

void __InitNandMutex(NandMutex *mutex);
void __DeinitNandMutex(NandMutex *mutex);
void __NandMutex_Lock(NandMutex *mutex);
void __NandMutex_Unlock(NandMutex* mutex);
int __NandMutex_TryLock(NandMutex *mutex);

#endif /* _NANDSEMPHORE_H_ */
