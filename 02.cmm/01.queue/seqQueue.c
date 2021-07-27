#include "includes.h"

/*
** ���г�ʼ��
*/
void Init_SeQueue(SeqQueue* q){
	uint16 i = 0;

	for(i = 0;i < SeqQueue_DepthLimit;i++){
		q->data[i] = NULL;
	}

	q->size = 0;
}

/*
** ���
*/
void Push_SeqQueue(SeqQueue* q,void* data){
	if(q == NULL || data == NULL || q->size == SeqQueue_DepthLimit){
		return;
	}

	q->data[q->size] = data;
	q->size++;
}

/*
** ����
*/
void* Pop_SeqQueue(SeqQueue* q){
	uint16 i = 0; 
	void* data = NULL;

	if(q == NULL || q->size == 0){
		return NULL;
	}

	data = q->data[0];

	for(i = 0;i < q->size - 1;i++){
		q->data[i] = q->data[i+1];
	}
	q->size--;

	return data;
}

/*
** ���ض��д�С
*/
uint16 Size_SeqQueue(SeqQueue* q){
	return(q->size);	
}

