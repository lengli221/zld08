#include "includes.h"

static int32 upgradeLLParse_TaskStk[512] = {0};

/*
** ʹ��Extern�����ļ�UpgradeLL_Reply.c����
*/
extern UpgradeLLParam upgradeLLParam;

/*
** ����֮��ѯͨѶС����������
*/
void UpgradeLLParse_ChkComRunArea(uint8 len,uint8* item,uint8 addr){
	uint8 rxlen = 0;
	
	/*
	** ��������ظ�
	*/
	switch(item[rxlen]){
		case 0x01:/*BootLoader*/
			upgradeLLParam.proCtr.areaFlag |= (DoorNumDefine)((DoorNumDefine)1<<addr);
			/*
			** ������������������ -- 20210221 -- ����ʧ��֮����ܴ������ж���������--��Ϊ��ѯ��������Ͳ�ѯ�汾����ͬһ�������·�
			*/
			//upgradeLLParam.proCtr.upgradePermit |= (DoorNumDefine)((DoorNumDefine)1<<addr);
			break;
// 		case 0x02:/*App*/
// 			upgradeLLParam.proCtr.areaFlag &= ~(DoorNumDefine)((DoorNumDefine)1<<addr);
// 			/*
// 			** ������������������
// 			*/
// 			upgradeLLParam.proCtr.upgradePermit &= ~(DoorNumDefine)((DoorNumDefine)1<<addr);	
// 			break;
	}
}

/*
** ����֮�����ļ��汾�� -- 0xF1
*/
void UpgradeLLParse_FileVer(uint8 len,uint8* item,uint8 addr){
	uint8 rxlen = 0;

	/*
	** �����ļ��汾�Żظ� �Ƿ����� �����ļ����� ϵͳ�汾��
	*/
	switch(item[rxlen]){/*�Ƿ�����*/
		case 0x01:/*��������*/
			upgradeLLParam.proCtr.upgradePermit |= (DoorNumDefine)((DoorNumDefine)1<<addr);
			/*
			** ��������״̬���
			*/
			
			break;
		case 0x2:/*�ܾ�����*/
			upgradeLLParam.proCtr.upgradePermit &= ~(DoorNumDefine)((DoorNumDefine)1<<addr);	
			break;
	}
}

/*
** ����֮�ļ��ֽ���+�ļ�����У�� -- 0xF2
*/
void UpgradeLLParse_FileByteItemCrc(uint8 len,uint8* item,uint8 addr){
	uint8 rxlen = 0;
	/*20210221--��ֿܷ�,���������ʧ��֮��,������ӦF2--�ֶ�:0x02*/
	static DoorNumDefine sub_ChkFailFlag = 0;
	
	/*
	** ����֡���ձ�־
	*/
	upgradeLLParam.proCtr.fileByteCrcRecFlag |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	
	/*
	** ״̬�ظ�����
	*/
	switch(item[rxlen]){
		case 0x01:/*�ȴ��������*/
			/*
			** �������ݴ����־
			*/
			upgradeLLParam.proCtr.binDataItemTransmit |= (DoorNumDefine)((DoorNumDefine)1<<addr);
			/*����Ż���־*/
			sub_ChkFailFlag = 0;
			break;
		case 0x02:/*������ݽ���,��������У��ȴ�����App��*/
			/*
			** ���½������ݴ���
			*/
			upgradeLLParam.proCtr.binDataItemEndFlag |= (DoorNumDefine)((DoorNumDefine)1<<addr);
			/*���:�ֿ���У��ʧ��֮���ظ�F2--0x02�ֶ�*/
			upgradeLLParam.proCtr.binDataItemEndFlag &= (DoorNumDefine)(~sub_ChkFailFlag);			
			break;
		case 0xF6:/*����App����汾��ʧ��*/
		case 0xF7:/*����App������ʧ��*/
		case 0xF8:/*����App����ʧ��*/
		case 0xF9:/*����������ʧ��*/
		case 0xFA:/*�ļ������ֽ������ռ����*/
		case 0xFB:/*�ļ�����У��Ϸ�*/
		case 0xFC:/*�����ļ�����У�鲻�Ϸ�*/
		case 0xFD:/*�ָ�����APP��*/
			sub_ChkFailFlag |= (DoorNumDefine)((DoorNumDefine)1<<addr);
			UpgradeLL_AssignCanelUpgrade(addr);
			break;
	}

	/*
	** �ԱȽ���bootloader��־λ����ձ�־λ��ͬʱ������һ����--�޸�ʱ��:20210226--��Ҫ������ѯ3Sȷ��һ����λ"��ʼ������־"-->�����������ֱ����ת����
	*/
	//if(upgradeLLParam.proCtr.areaFlag == upgradeLLParam.proCtr.fileByteCrcRecFlag
	//	&& upgradeLLParam.fileByteItemCrc.cmd == 0x01){/*�ڿ�ʼ��������ʱ�Ž������ݴ�������*/
	//	upgradeLLParam.proCtr.step = UpgradeLL_Proc_DataItemTra;
	//	/*
	//	** ����֮���������ļ����ݲ���
	//	*/
	//	Upgrade_UpdateFileItemPara(1);			
	//}

	/*
	** �ԱȽ������ݸ������ַ�ͽ����������ַһ����--20210226��;�˳������ڱ�־λ:binDataItemTransmi��δ���,�ȴ�3S֮���ѯ���е����ݺ�Ӳ�ѯ�ڽ����������
	** Ϊ��;�˳���:�Ӵ˴������������
	*/
	if(upgradeLLParam.proCtr.binDataItemTransmit == upgradeLLParam.proCtr.binDataItemEndFlag
		&& upgradeLLParam.fileByteItemCrc.cmd == 0x02){/*��������֡����*/
		upgradeLLParam.proCtr.step  = UpgradeLL_Proc_Finsh;/*��������*/	
	}
}

/*
** ����֮�ļ�����--F3
*/
void UpgradeLLParse_FileItem(uint8 len,uint8* item,uint8 addr){
	uint8 rxlen = 0;
	uint16 frameLabel = 0;
	volatile uint8 byteNum = 0;
	uint8 replyAnalysis = 0;
	static bool recFlag = true;
	/*------------------20210226Ŀǰ��ȡ�ķ�����:���������ļ���������(F3)����,�ֿز�����Ӧ-------------*/
	/*
	** ֡��ʶ ֡���������ֽ��� �ظ�����
	*/
	memcpy((uint8*)&frameLabel,(uint8*)&item[rxlen],sizeof(uint16));
	rxlen += sizeof(uint16);
	byteNum = item[rxlen];
	rxlen += sizeof(uint8);
	replyAnalysis = item[rxlen];
	rxlen += sizeof(uint8);

	switch(replyAnalysis){/*�ظ�����*/
		case 0x01:/*��Ӧ֡��ɽ���*/
		case 0x05:/*֡�ظ�*/
			/*
			** �����Ż�����
			*/
			if(upgradeLLParam.proCtr.fileItemRecFlag == 0){
				recFlag = false;
			}
			upgradeLLParam.proCtr.fileItemRecFlag |= (DoorNumDefine)((DoorNumDefine)1<<addr);
			
			/*
			** �ԱȽ����ļ��ֽ���+�ļ�����У���־���ļ����ݽ��ձ�־
			*/
			if(upgradeLLParam.proCtr.binDataItemTransmit == upgradeLLParam.proCtr.fileItemRecFlag
				&& recFlag == false){
				recFlag = true;
				upgradeLLParam.fileItemCtr.cnt = 0;
				upgradeLLParam.fileItem.frameLabel++;
				
				/*
				** ����֮�ļ�����֮����֡��ɷ��ͱ�־λ
				*/			
				if(Upgrade_FileItemTraFinshFlag() == true){
					upgradeLLParam.proCtr.step  = UpgradeLL_Proc_FileByteCrc;
					upgradeLLParam.fileByteItemCrc.circCnt = 0;
					/*
					** ������������֮�������ݴ���
					*/
					upgradeLLParam.fileByteItemCrc.cmd = 0x02;/*�������ݴ���*/
					/*��������䶨ʱ��--�ֿؿ������һ��δ���ճɹ�--�޸�ʱ��:20210222--����11:01*/
					TickOut((uint32 *)&upgradeLLParam.fileByteItemCrc.itick, 0);
				}
			}
			break;
		case 0x02:/*�����쳣(δ�ɹ����չ���0xF2���)*/			
		case 0x03:/*֡��ʶ�쳣(��ע:֡��Խ)*/
		case 0x04:/*дFlash�쳣*/
			upgradeLLParam.proCtr.fileItemRecFlag &= ~(DoorNumDefine)((DoorNumDefine)1<<addr);	
			UpgradeLL_AssignCanelUpgrade(addr);
			break;			
	}
}

/*
** �ṩϵͳ���֮Զ�������ӿ�����֮�²�Э��֮��������
*/
void SM_UpgradeLLParse_Task(void* p_arg){
	/*
	** ϵͳ��λ��ʱ6S,��ֹ����������,����ͨѶ��������Bootloader��
	*/
	Sleep(6000);
	
	for(;;){
		/*
		** �²�Э��֮���ݽ���֮����������
		*/
		lowerUpgrFunAnalyze();

		/*
		** ���¿��Ź��Ĵ���--ι��
		*/
		watchdogUpdate();
		
		Sleep(4);
	}
}

/*
** �ṩϵͳ���֮Զ�������ӿں���֮�²�Э��֮��������
*/
void UpgradeLLParse_Init(void){
	Thread_create(SM_UpgradeLLParse_Task, 0,
		(int32 *)&upgradeLLParse_TaskStk[512-1], (int8)UgradeLLParseTask_Prio);
}

