#include "includes.h"

/*---------------------�������ʧ��ԭ���ϱ�--------------------------------*/
FIFOQueue fifoQueue[SysCtr_AllDoorNum] = {0};
UpgrFail upgrFail[SysCtr_AllDoorNum][FIFOQueue_DepthLimit] = {0};
/*-------------------------------------------------------------------------*/
/*---------------------���������ʧ��ԭ���ϱ�------------------------------*/
FIFOQueue chargerFIFOQueue[SysCtr_AllDoorNum] = {0};
UpgrFail chargerUpgrFail[SysCtr_AllDoorNum][FIFOQueue_DepthLimit] = {0};
/*-------------------------------------------------------------------------*/

/*ԭ���Ƿ�δ��ȡ��־*/
DoorEUResonNoRead doorEUResonNoRead = {0};

/*
** ����ʧ��ԭ�������ʼ��--���ʧ��ԭ�����3(FIFOQueue_DepthLimit)������
*/
void init_UpgrFailPara(void){
	uint16 i = 0;

	/*FIFO���г�ʼ��*/
	for(i=0;i<SysCtr_AllDoorNum;i++){
		init_FIFOQueue((FIFOQueue *)&fifoQueue[i]);
		init_FIFOQueue((FIFOQueue *)&chargerFIFOQueue[i]);
	}
}

/*
** set--����ʧ��ԭ�� -- �����ɹ�,ʧ�ܾ��ϱ�--Э��V1.8
*/
void set_UpgrFailPara(uint8 type,UpgrFail u,uint8 loc){
	uint16 depth = 0;
	switch(type){
		case 0x03:/*��ع̼���*/
			depth = size_FIFOQueue((FIFOQueue *)&fifoQueue[loc]);
			if(depth == FIFOQueue_DepthLimit){
				pop_FIFOQueue((FIFOQueue *)&fifoQueue[loc]);/*������֮�󶪵����ʧ��ԭ��*/
				depth = size_FIFOQueue((FIFOQueue *)&fifoQueue[loc]);
			}
			memset((uint8*)&upgrFail[loc][depth],0,sizeof(UpgrFail));
			upgrFail[loc][depth] = u;
			push_FIFOQueue((FIFOQueue *)&fifoQueue[loc], (void *)&upgrFail[loc][depth]);			
			break;
		case 0x04:/*������̼���*/
			depth = size_FIFOQueue((FIFOQueue *)&chargerFIFOQueue[loc]);
			if(depth == FIFOQueue_DepthLimit){
				pop_FIFOQueue((FIFOQueue *)&chargerFIFOQueue[loc]);/*������֮�󶪵����ʧ��ԭ��*/
				depth = size_FIFOQueue((FIFOQueue *)&chargerFIFOQueue[loc]);
			}
			memset((uint8*)&chargerUpgrFail[loc][depth],0,sizeof(UpgrFail));
			chargerUpgrFail[loc][depth] = u;
			push_FIFOQueue((FIFOQueue *)&chargerFIFOQueue[loc], (void *)&chargerUpgrFail[loc][depth]);		
			break;
	}

}

/*
** get--����ʧ��ԭ��
*/
void* get_UpgrFailPara(uint8 type,uint8 loc){
	switch(type){
		case 0x03:/*��ع̼���*/		
			return pop_FIFOQueue((FIFOQueue *)&fifoQueue[loc]);
		case 0x04:/*������̼���*/
			return pop_FIFOQueue((FIFOQueue *)&chargerFIFOQueue[loc]);
	}

	return NULL;
}

/*
** 20210518--����Ƿ��������ԭ��δ��ȡ
*/
void runtimeChk_UpgrResonFlag(void){
	uint8 i = 0;
	
	for(i=0;i<SysCtr_AllDoorNum;i++){
		/*���*/
		if(size_FIFOQueue((FIFOQueue *)&fifoQueue[i]) != 0){/*����ԭ��*/
			doorEUResonNoRead.bat |= (DoorNumDefine)((DoorNumDefine)0x01<<i);
		}else{
			doorEUResonNoRead.bat &= (DoorNumDefine)~((DoorNumDefine)1<<i);	
		}
		/*�����*/
		if(size_FIFOQueue((FIFOQueue *)&chargerFIFOQueue[i])){
			doorEUResonNoRead.charger |= (DoorNumDefine)((DoorNumDefine)0x01<<i);
		}else{
			doorEUResonNoRead.charger &= (DoorNumDefine)~((DoorNumDefine)1<<i);	
		}
	}
}

/*
** 20210518--��ȡԭ���ϱ�
*/
DoorEUResonNoRead get_DoorEUResonNoRead(void){
	return doorEUResonNoRead;
}

