#ifndef __FIFOQUEUE_H_
#define __FIFOQUEUE_H_

/*
** ����FIFO����������
*/
#define FIFOQueue_DepthLimit						(uint16)0x03

/*
** FIFO���нṹ
*/
typedef struct{
	void* data[FIFOQueue_DepthLimit];
	uint16 size;
}FIFOQueue;


/*
** ���г�ʼ�� 
*/
void init_FIFOQueue(FIFOQueue* q);
/*
** ���
*/
void push_FIFOQueue(FIFOQueue* q,void* data);
/*
** ����
*/
void* pop_FIFOQueue(FIFOQueue* q);
/*
** ���ض��д�С
*/
uint16 size_FIFOQueue(FIFOQueue* q);

#endif

