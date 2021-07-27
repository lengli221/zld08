#include "includes.h"

SeqQueue comQueue = {0};
ComBup comBup[SeqQueue_DepthLimit] = {0};
ComBup curComBup = {0};

/*
** ͨѶ������������ʼ��
*/
void ComBoardUpgrPara_Init(void){
	/*
	** ���г�ʼ��
	*/
	Init_SeQueue((SeqQueue *)&comQueue);
	/*
	** �����ǰͨѶ�����������
	*/
	Clear_CurComBupPara();
}

/*
** �����ǰͨѶ�����������
*/
void Clear_CurComBupPara(void){
	memset((uint8*)&curComBup.upgradeWay,0x00,sizeof(ComBup));
	/*
	** ���������������
	*/	
	clear_UpgrLLParamCnt();
}

/*
** get ��ǰ����������ز���
*/
ComBup getCurComBup(void){
	return curComBup;
}

/*
** ����ͨѶС����в���
*/
void UpdataComBoardQueuePara(ComBup com){
	uint16 depth = Size_SeqQueue((SeqQueue *)&comQueue);

	memset((uint8*)&comBup[depth].binFileType.flag,0x00,sizeof(ComBup));
	comBup[depth] = com;
	Push_SeqQueue((SeqQueue *)&comQueue, (void *)&comBup[depth]);
}

/*
** ͨѶ���������+������ز�������
*/
void ComBoard_TaskSwitch(void){
// 	static bool resumeFlag = true;
// 	static bool resFlag = true;
	
	/*
	**	״̬���
	*/
	if(curComBup.binFileType.flag == 0){/*ͨѶС�����κ������ļ�����*/
		if(Size_SeqQueue((SeqQueue *)&comQueue) > 0){
			curComBup = *(ComBup*)Pop_SeqQueue((SeqQueue *)&comQueue);
			/*
			** ����֮�����ṹ�������ʼ��
			*/
			UpgradeLLParam_Init();
			/*
			** ���������������
			*/	
			clear_UpgrLLParamCnt();			
			/*
			** ���������Լ�
			*/
			add_UpgrCnt();
		}		
	}

// 	/*
// 	** �����л�
// 	*/
// 	if(curComBup.binFileType.flag != 0){/*ͨѶС�����ļ���Ҫ����--ִ����������*/
// 		if(resumeFlag == true){/*����App�ӿ�����*/
// 			resumeFlag = false;
// 			OSTaskSuspend(LowerLayerParseTask_Prio);
// 			OSTaskSuspend(LowerLayerReplyTask_Prio);
// 		}
// 		
// 		if(resFlag == false){/*�ָ������ӿ�����*/
// 			resFlag = true;
// 			OSTaskResume(UgradeLLReplyTask_Prio);
// 			OSTaskResume(UgradeLLParseTask_Prio);
// 		}		
// 	}else{/*ִ��App�ӿں�������*/
// 		if(resumeFlag == false){/*�ָ�App�ӿ�����*/
// 			resumeFlag = true;
// 			OSTaskResume(LowerLayerParseTask_Prio);
// 			OSTaskResume(LowerLayerReplyTask_Prio);			
// 		}

// 		if(resFlag == true){/*���������ӿ�*/
// 			resFlag = false;
// 			OSTaskSuspend(UgradeLLReplyTask_Prio);
// 			OSTaskSuspend(UgradeLLParseTask_Prio);
// 		}
// 	}
}

