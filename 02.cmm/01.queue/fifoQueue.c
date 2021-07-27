#include "includes.h"

/*
** ���г�ʼ�� 
*/
void init_FIFOQueue(FIFOQueue* q){
	uint16 i = 0;

	for(i=0;i < FIFOQueue_DepthLimit;i++){
		q->data[i] = NULL;
	}

	q->size = 0;
}

/*
** ���
*/
void push_FIFOQueue(FIFOQueue* q,void* data){
	
	if(q == NULL || data == NULL){
		return;
	}

	q->data[q->size] = data;
	q->size++;
}

/*
** ����
** 20210227--������ʱ����
*/
UpgrFail upgrFailTempPara = {0};
void* pop_FIFOQueue(FIFOQueue* q){
	uint16 i = 0;
	void* data = NULL;

	if(q == NULL || q->size == 0){
		return NULL;
	}

	/*data = q->data[0];*/
	/*�����ϱ�����*/
	memcpy((UpgrFail*)&upgrFailTempPara,(UpgrFail*)q->data[0],sizeof(UpgrFail));
	data = (UpgrFail*)&upgrFailTempPara;

	for(i=0;i < q->size - 1;i++){
		/*q->data[i] = q->data[i+1];*//*������ַ����--Ӧ�޸ĳɿ�������*/
		memcpy(q->data[i],q->data[i+1],sizeof(UpgrFail));/*��������--�Ƿ�����Ż���ʹ��sizeof(UpgrFail)--���Ӵ��븴����*/
		/*���ѿ�������--2021027*/
		memset(q->data[i+1],0,sizeof(UpgrFail));
	}
	/*��ָ��ָ��ռ�λ��*/
	q->data[q->size-1] = NULL;
	q->size--;

	return data;
}

/*
** ���ض��д�С
*/
uint16 size_FIFOQueue(FIFOQueue* q){
	return(q->size);
}

