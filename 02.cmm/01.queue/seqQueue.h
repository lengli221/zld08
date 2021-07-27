#ifndef __SEQQUEUE_H_
#define __SEQQUEUE_H_

/*
** ���������Ȳ�������
*/
#define SeqQueue_DepthLimit						(uint16)30

/*
** ˳����нṹ
*/
typedef struct{
	void* data[SeqQueue_DepthLimit];
	uint16 size;
}SeqQueue;

/*
** ���г�ʼ��
*/
void Init_SeQueue(SeqQueue* q);
/*
** ���
*/
void Push_SeqQueue(SeqQueue* q,void* data);
/*
** ����
*/
void* Pop_SeqQueue(SeqQueue* q);
/*
** ���ض��д�С
*/
uint16 Size_SeqQueue(SeqQueue* q);
#endif

