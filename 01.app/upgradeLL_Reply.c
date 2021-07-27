#include "includes.h"

static int32 upgradeLLReply_TaskStk[512] = {0};

/*
** �ϲ�Э��֮ʹ��extern���������ļ�����
*/
extern UpperLayerPara upperLayerPara;

/*
** ʹ��ָ����������ļ�����
*/
UpperLayerPara* ulp_ULLR = null; 

/*
** ����֮�����ṹ�����
*/
UpgradeLLParam upgradeLLParam;
/*
** ����ʧ�ܲ������ƽṹ�����
*/
UpgrLLPara upgrLLPara;

/*
** ����֮�����ṹ�������ʼ��
*/
bool upgrFlag;
void UpgradeLLParam_Init(void){
	memset((uint8*)&upgradeLLParam.proCtr.step,0x00,sizeof(UpgradeLLParam));
	ulp_ULLR = getUpperLayerPara();
	upgrFlag = false;
}

/*
** ���������������
*/
void clear_UpgrLLParamCnt(void){
	upgrLLPara.proCtrRep.cnt = 0;
}

/*
** ���������Լ�
*/
void add_UpgrCnt(void){
	upgrLLPara.proCtrRep.cnt++;
}

/*
** �����ļ��汾��֮���
*/
void upgrFileMultiPackect_1(uint8* len, uint8 *item){
	uint8 txLen = 0;	
	ComBup comBupTemp = getCurComBup();

	/*
	** ֡��ʶ ֡�ֽڳ���
	*/
	item[txLen] = 1;
	txLen += sizeof(uint8);
	item[txLen] = 6;
	txLen += sizeof(uint8);
	/*
	** ������ʽ �ļ����� ����汾�� Ӳ���汾��
	*/
	item[txLen] = comBupTemp.upgradeWay;
	txLen += sizeof(uint8);
	item[txLen] = comBupTemp.binFileType.flag;
	txLen += sizeof(uint8);
	memcpy((uint8*)&item[txLen],(uint8*)&comBupTemp.softVer,sizeof(uint16));
	txLen += sizeof(uint16);
	memcpy((uint8*)&item[txLen],(uint8*)&comBupTemp.detailedInfo.hardVer,sizeof(uint16));
	txLen += sizeof(uint16);

	/*
	** �������ݳ���
	*/
	*len = txLen;
}

void upgrFileMultiPackect_2(uint8* len, uint8 *item){
	uint8 txLen = 0;	
	ComBup comBupTemp = getCurComBup();

	/*
	** ֡��ʶ ֡�ֽڳ���
	*/
	item[txLen] = 2;
	txLen += sizeof(uint8);
	item[txLen] = 6;
	txLen += sizeof(uint8);
	/*
	** �豸ID
	*/
	memcpy((uint8*)&item[txLen],(uint8*)&comBupTemp.detailedInfo.id[0],6);
	txLen += 6;

	/*
	** �������ݳ���
	*/
	*len = txLen;	
}

void upgrFileMultiPackect_3(uint8* len, uint8 *item){
	uint8 txLen = 0;	
	ComBup comBupTemp = getCurComBup();

	/*
	** ֡��ʶ ֡�ֽڳ���
	*/
	item[txLen] = 3;
	txLen += sizeof(uint8);
	item[txLen] = 6;
	txLen += sizeof(uint8);
	/*
	** �豸ID
	*/
	memcpy((uint8*)&item[txLen],(uint8*)&comBupTemp.detailedInfo.id[6],6);
	txLen += 6;

	/*
	** �������ݳ���
	*/
	*len = txLen;	
}

void upgrFileMultiPackect_4(uint8* len, uint8 *item){
	uint8 txLen = 0;	
	ComBup comBupTemp = getCurComBup();

	/*
	** ֡��ʶ ֡�ֽڳ���
	*/
	item[txLen] = 4;
	txLen += sizeof(uint8);
	item[txLen] = 4;
	txLen += sizeof(uint8);
	/*
	** �豸ID
	*/
	memcpy((uint8*)&item[txLen],(uint8*)&comBupTemp.detailedInfo.id[12],6);
	txLen += 6;

	/*
	** �������ݳ���
	*/
	*len = txLen;	
}

/*
** ����֮�²�Э��֮��ѯ����ͨѶС����������--�㲥��ʽ����
*/
static void UgradeLL_BCComRunAreaLogic(void){
	static uint8 deviceAddr = DstAddr_BoardCast_Define;/*�㲥������ַ*/
	uint8 len = 1;
	uint8 tx[8] = {0x01,0};

	if(upgradeLLParam.proCtr.step == UpgradeLL_ProcChkRunArea 
		&& upgradeLLParam.runArea.circCnt <= sizeof(uint16)*UpgradeLL_FrameMax){
		/*���200Ms��ѯһ��,����ͨѶС�帴λ�ȴ�ʱ������*/
		if(TickOut((uint32 *)&upgradeLLParam.runArea.itick, 195) == true){
			TickOut((uint32 *)&upgradeLLParam.runArea.itick, 0);
			CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, UpgradeLL_FunId_RunArea);
			/*
			** ����֮���ͨѶС��������ʽ
			*/
			upgrFileMultiPackect_1((uint8 *)&len, (uint8 *)&tx[0]);
			CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, UpgradeLL_FunId_Ver);	
			upgrFileMultiPackect_2((uint8 *)&len, (uint8 *)&tx[0]);
			CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, UpgradeLL_FunId_Ver);	
			upgrFileMultiPackect_3((uint8 *)&len, (uint8 *)&tx[0]);
			CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, UpgradeLL_FunId_Ver);	
			upgrFileMultiPackect_4((uint8 *)&len, (uint8 *)&tx[0]);
			CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, UpgradeLL_FunId_Ver);	
			
			upgradeLLParam.runArea.circCnt++;
		}
	}else{
		if(upgradeLLParam.proCtr.step == UpgradeLL_ProcChkRunArea){/*������ͨѶС��״̬�Ѹ������,��ʼ��������*/
			upgradeLLParam.proCtr.step = UpgradeLL_Proc_FileByteCrc;
			/*
			** ������������֮��ʼ���ݴ���
			*/
			upgradeLLParam.fileByteItemCrc.cmd = 0x01;/*��ʼ���ݴ���*/
			/*�Ż��߼�:��ֹ�·�F1֮��,��δ��ȫ���ղ����±�־λ--֮ǰ�汾��F0����λ����������־�����쳣--�޸�ʱ��:20210222*/
			TickOut((uint32 *)&upgradeLLParam.fileByteItemCrc.itick, 0);
			/*20210226--�Ż��߼�:��ѯ���������־����3S֮������λ*/
			upgradeLLParam.fileByteItemCrc.circCnt = 0;
		}
	}
}

/*
** ����֮����ļ��ֽ�����+�ļ�����У��
*/
void UpgradeLL_FillFileByteItemCrc(uint8* len, uint8 *item){
	uint8 txlen = 0;
	BinFileItemPara binFileItemPara = getCurComBup().binFileItemPara;

	/*
	** ������ ������������ �ļ��ֽ��� �ļ�����У��
	*/
	item[txlen] = upgradeLLParam.fileByteItemCrc.cmd;
	txlen += sizeof(uint8);
	memcpy((uint8*)&item[txlen],(uint8*)&binFileItemPara.len,sizeof(uint32));
	txlen += sizeof(uint32);
	memcpy((uint8*)&item[txlen],(uint8*)&binFileItemPara.crc16,sizeof(uint32));
	txlen += sizeof(uint16);
	/*
	** �������
	*/
	*len = txlen;	
}

/*
** �����ϲ�Э���������ֶδ���
** @param: 
***		filed-->�����ֶ� 
**		lab  --> 0x01:"������"�ֶ� 0x02:"�������"�ֶ�
*/
void update_Upperfiled(DoorNumDefine filed,uint8 lab){
	ComBup comBupTemp = getCurComBup();
	
	if(comBupTemp.binFileType.flag == ComBoradType){/*ͨѶ��*/
		switch(lab){
			case 0x01:/*������*/
				ulp_ULLR->stateInfoChange.sysLogic.comUpgr = filed;
				ulp_ULLR->stateInfoChange.sysLogic.comUpgrIsOk = 0;
				break;
			case 0x02:/*�������*/
				ulp_ULLR->stateInfoChange.sysLogic.comUpgr = 0;
				ulp_ULLR->stateInfoChange.sysLogic.comUpgrIsOk = filed;				
				break;
		}
		/*�Ż����������:20210227*/		
		ulp_ULLR->stateInfoChange.sysLogic.batFileDownload = 0;
		ulp_ULLR->stateInfoChange.sysLogic.batFileDownloadIsOk = 0;				
		ulp_ULLR->stateInfoChange.sysLogic_2.chargeFileDownloading = 0; 
		ulp_ULLR->stateInfoChange.sysLogic_2.chargeFileDownloadFinsh = 0;
	}else if(comBupTemp.binFileType.flag == ChargeBoardType){/*������̼�*/			
		switch(lab){
			case 0x01:/*������*/
				ulp_ULLR->stateInfoChange.sysLogic_2.chargeFileDownloading = filed;
				ulp_ULLR->stateInfoChange.sysLogic_2.chargeFileDownloadFinsh = 0;
				break;
			case 0x02:/*�������*/
				ulp_ULLR->stateInfoChange.sysLogic_2.chargeFileDownloading = 0;
				ulp_ULLR->stateInfoChange.sysLogic_2.chargeFileDownloadFinsh = filed;				
				break;	
		}	
		/*�Ż����������:20210227*/	
		ulp_ULLR->stateInfoChange.sysLogic.comUpgr = 0;
		ulp_ULLR->stateInfoChange.sysLogic.comUpgrIsOk = 0;		
		ulp_ULLR->stateInfoChange.sysLogic.batFileDownload = 0;
		ulp_ULLR->stateInfoChange.sysLogic.batFileDownloadIsOk = 0;			
	}else{/*��ع̼�*/
		/*�Ż�����*/
		if(comBupTemp.binFileType.flag == BatBoardType){
			switch(lab){
				case 0x01:/*������*/
					ulp_ULLR->stateInfoChange.sysLogic.batFileDownload = filed;
					ulp_ULLR->stateInfoChange.sysLogic.batFileDownloadIsOk = 0;				
					break;
				case 0x02:/*�������*/
					ulp_ULLR->stateInfoChange.sysLogic.batFileDownload = 0;
					ulp_ULLR->stateInfoChange.sysLogic.batFileDownloadIsOk = filed;					
					break;
			}
			/*�Ż����������:20210227*/	
			ulp_ULLR->stateInfoChange.sysLogic.comUpgr = 0;
			ulp_ULLR->stateInfoChange.sysLogic.comUpgrIsOk = 0;					
			ulp_ULLR->stateInfoChange.sysLogic_2.chargeFileDownloading = 0; 
			ulp_ULLR->stateInfoChange.sysLogic_2.chargeFileDownloadFinsh = 0;
		}
	}	

	/*
	** ����ͬһ�̼�����������������
	*/
	if(lab == 0x01){/*������*/
		ulp_ULLR->stateInfoChange.sameFireRepCnt = upgrLLPara.proCtrRep.cnt;
	}
}

/*
** ����֮�²�Э��֮�㲥����֮�ļ��ֽ���+�ļ�����У��
*/
static void UpgradeLL_BCFileByteItemCrc(void){
	static uint8 deviceAddr = DstAddr_BoardCast_Define;/*�㲥������ַ*/
	uint8 len = 0;
	uint8 tx[8] = {0};
	static uint32 tickBase = 200;

	/*
	** ���¶�ʱ����׼ʱ��
	*/
	if(upgradeLLParam.fileByteItemCrc.cmd == 0x01){/*��ʼ���ݴ���*/
		tickBase = 1000;
	}

	if(upgradeLLParam.fileByteItemCrc.cmd == 0x02){/*�������ݴ���*/
		tickBase = 1000;/*���»�׼ʱ����ڽ������ݴ�������ͨѶС����Ҫ����,дFlash��Ҫʱ��,Ŀǰ����700MS*/
	}	

	if(upgradeLLParam.proCtr.step == UpgradeLL_Proc_FileByteCrc
		&& upgradeLLParam.fileByteItemCrc.circCnt < 3/*sizeof(uint16)*UpgradeLL_FrameMax*/
		&& upgradeLLParam.proCtr.upgradePermit != 0){/*��������ͨѶ�����>=1*/
		if(TickOut((uint32 *)&upgradeLLParam.fileByteItemCrc.itick, tickBase) == true){
			TickOut((uint32 *)&upgradeLLParam.fileByteItemCrc.itick, 0);
			UpgradeLL_FillFileByteItemCrc((uint8 *)&len, (uint8 *)&tx[0]);
			CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, UpgradeLL_FunId_FileByteCrc);
			upgradeLLParam.fileByteItemCrc.circCnt++;
			/*
			** �����ϲ�Э���������ֶδ���--�޸�:��������upgradeLLParam.fileByteItemCrc.cmd == 0x01Ϊ��ʼ��������
			*/
			if(upgradeLLParam.fileByteItemCrc.cmd == 0x01 && upgradeLLParam.fileByteItemCrc.circCnt == 3/*����3Sͳ��������־*/){
				update_Upperfiled(upgradeLLParam.proCtr.upgradePermit, 0x01);/*���¹̼�/�����ļ���*/
			}
		}
	}else{
		if(upgradeLLParam.proCtr.step == UpgradeLL_Proc_FileByteCrc ){
			if(upgradeLLParam.fileByteItemCrc.cmd == 0x01/*��ʼ��������*/
				&& upgradeLLParam.proCtr.binDataItemTransmit != 0){/*���������ݵ�ַ����*/
				upgradeLLParam.proCtr.step = UpgradeLL_Proc_DataItemTra;
				/*
				** ����֮���������ļ����ݲ���
				*/
				Upgrade_UpdateFileItemPara(1);		
			}else{/*20210226��;�˳������ڱ�־λ:binDataItemTransmi��δ���,�ȴ�3S֮���ѯ���е����ݺ�Ӳ�ѯ�ڽ����������*/
				upgradeLLParam.proCtr.step  = UpgradeLL_Proc_Finsh;/*��������*/
			}
		}
	}
}

/*
** ����֮�ļ�����֮����֡��ɷ��ͱ�־λ
*/
bool Upgrade_FileItemTraFinshFlag(void){
	bool ret = false;
	BinFileItemPara binFileItemPara = getCurComBup().binFileItemPara;
	uint32 binLen = binFileItemPara.len;
	uint8 frameNum = 0;

	/*
	** ���ݲ�ͬ�ļ���С�ĵ�Ƭ�� 
	*/
	if((binLen%sizeof(uint32)) != 0){
		frameNum = 1;
	}
	binLen = ((binLen/sizeof(uint32)) + frameNum)*sizeof(uint32);
	
	if(/*binFileItemPara.len*/binLen == (upgradeLLParam.fileItem.frameLabel- 0x01)*sizeof(uint32)){
		ret = true;
	}

	return ret;
}

/*
** ����֮���������ļ����ݲ���
*/
void Upgrade_UpdateFileItemPara(uint16 frameLabel){
	ComBup comBupTemp = getCurComBup();
	
	/*
	** ֡���
	*/
	upgradeLLParam.fileItem.frameLabel = frameLabel;
	/*
	** ֡���������ֽ�
	*/
	upgradeLLParam.fileItem.dataItemLen = sizeof(uint32);
	/*
	** �ļ����� 
	*/
	sFLASH_ReadBuffer((uint8 *)&upgradeLLParam.fileItem.data[0], comBupTemp.flashAddr + (frameLabel - 0x01)*sizeof(uint32), sizeof(uint32));
}

/*
** ����֮����ļ�����
*/
void UpgradeLL_FillFileItem(uint8* len, uint8 *item){
	uint8 txlen = 0;

	/*
	** ������ ֡��ʶ ֡���������ֽ��� �ļ�����
	*/
	memcpy((uint8*)&item[txlen],(uint8*)&upgradeLLParam.fileItem.frameLabel,sizeof(uint16));
	txlen += sizeof(uint16);
	item[txlen] = upgradeLLParam.fileItem.dataItemLen;
	txlen += sizeof(uint8);
	memcpy((uint8*)&item[txlen],(uint8*)&upgradeLLParam.fileItem.data[0],sizeof(uint32));
	txlen += sizeof(uint32);
	/*
	** �������
	*/
	*len = txlen;		
}

/*
** ����֮�²�Э��֮�㲥����֮�ļ�����
*/
static void UpgradeLL_BCFileItem(void){
	static uint8 deviceAddr = DstAddr_BoardCast_Define;/*�㲥������ַ*/
	static uint8 len = 0;
	static uint8 tx[8] = {0};

	/*
	** �ļ������·�����:
	** 					1.��ʱ17msÿһ֡�·�
	**					2.�����ֿ��Ƿ��յ�
	*/	
	if(upgradeLLParam.proCtr.step == UpgradeLL_Proc_DataItemTra
		&& TickOut((uint32 *)&upgradeLLParam.fileItemCtr.itick,10) == true){
		/*20210128����Ƿ��������������־*/
		if(upperLayerPara.stateInfoChange.sysLogic.comUpgr == 0
				&& upperLayerPara.stateInfoChange.sysLogic.batFileDownload == 0
				&& upperLayerPara.stateInfoChange.sysLogic_2.chargeFileDownloading == 0){
			upgradeLLParam.proCtr.step  = UpgradeLL_Proc_FileByteCrc;
			upgradeLLParam.fileByteItemCrc.circCnt = 0;
			/*
			** ������������֮�������ݴ���
			*/
			upgradeLLParam.fileByteItemCrc.cmd = 0x02;/*�������ݴ���*/
			/*���㰲׿������*/
			Sleep(3000);
		}
		TickOut((uint32 *)&upgradeLLParam.fileItemCtr.itick, 0);
		upgradeLLParam.fileItemCtr.cnt  = 0;
		if(upgradeLLParam.fileItemCtr.cnt == 0){/*��������*/
			/*20210226--���ݴ����쳣����(����Ӱ��ʵ��ʹ��):����16λ��Ƭ��,���һ�������ɷ��ĸ��ֽ�,����������ֽ�Ϊ��Ч�ֽ�,�����ƹ�У�����F2���ֽ�����У��,����û������*/
			Upgrade_UpdateFileItemPara(upgradeLLParam.fileItem.frameLabel);
			UpgradeLL_FillFileItem((uint8 *)&len, (uint8 *)&tx[0]);
			/*
			** ����֡��־֮��,�����ձ�־λ
			*/
			upgradeLLParam.proCtr.fileItemRecFlag = 0;
		}
		
		if(upgradeLLParam.fileItemCtr.cnt++ >= UpgradeLL_FrameMax){/*�޸ĳɲ���У���ط�����֮��,���ж�����֮����߼�����Ч*/
			upgradeLLParam.fileItemCtr.cnt = 0;
			/*
			** ͨѶС��δ�ظ��ʸ��¿�����ͨѶС���־λ
			*/
			upgradeLLParam.proCtr.binDataItemTransmit = upgradeLLParam.proCtr.fileItemRecFlag;
			/*
			** ����֮�ļ�����֮����֡��ɷ��ͱ�־λ
			*/			
			if(Upgrade_FileItemTraFinshFlag() == true ){
				upgradeLLParam.proCtr.step  = UpgradeLL_Proc_FileByteCrc;
				upgradeLLParam.fileByteItemCrc.circCnt = 0;
				/*
				** ������������֮�������ݴ���
				*/
				upgradeLLParam.fileByteItemCrc.cmd = 0x02;/*�������ݴ���*/
			}
		}
		CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, UpgradeLL_FunId_FileItem);
		upgradeLLParam.fileItem.frameLabel++;
		/*20210226Ŀǰ��ȡ�ķ�����:���������ļ���������(F3)����,�ֿز�����Ӧ--�ļ��������֮����Ӵ����������˳��ļ�����,����֪ͨ�ֿؽ�������*/
		if(Upgrade_FileItemTraFinshFlag() == true){
			upgradeLLParam.proCtr.step  = UpgradeLL_Proc_FileByteCrc;
			upgradeLLParam.fileByteItemCrc.circCnt = 0;/*20210226--ע��:����ͳ��3S������־*/
			/*
			** ������������֮�������ݴ���
			*/
			upgradeLLParam.fileByteItemCrc.cmd = 0x02;/*�������ݴ���*/
			/*��������䶨ʱ��--�ֿؿ������һ��δ���ճɹ�--�޸�ʱ��:20210222--����11:01*/
			TickOut((uint32 *)&upgradeLLParam.fileByteItemCrc.itick, 0);			
		}
	}
}

/*
** ��ѯApp����汾��
*/
extern SysParam sysParam;
extern void update_ComBatSoftVer(uint8* len, uint8 *item,uint8 label);
uint8 assignLabel;
uint16 assignSoftVer[SysCtr_AllDoorNum];
bool assign_ChkVer(uint8 label){
	uint8 i = 0;
	uint16 binFileVer = 0;
	UpgrFilePara upgrTemp = {0};
	bool ret = false;

	if(label == 0){
		binFileVer = sysParam.newSoftVer.comFile;/*ͨѶ���ⲿFlash�洢����汾��*/	
	}else if (label >= 0x80){
		upgrTemp = get_UpgrChargeFilePara(label - 0x80);
		binFileVer = upgrTemp.upgrFileType.softVer;/*�ⲿ������ļ��汾��*/
	}else{
		upgrTemp = get_UpgrBatFilePara(label - 1);
		binFileVer = upgrTemp.upgrFileType.softVer;/*�ⲿ��ع̼��ļ��汾��*/
	}
	
	for(i=0;i<SysCtr_AllDoorNum;i++){
		if(binFileVer != assignSoftVer[i]/*binFileVer > assignSoftVer[i]*/&& binFileVer != 0){
			ret = true;
			break;
		}	
	}
	return ret;
}

static void UpgradeLL_BCChkAppVer(void){
 	static uint8 deviceAddr = DstAddr_BoardCast_Define;/*�㲥������ַ*/
 	uint8 len = 1;
 	uint8 tx[8] = {0};/*0x00--���ͨѶ�����*/
	ComBup  comBupTemp = getCurComBup();
	UpgrFilePara upgr = {0};
	static DoorNumDefine upgrFlagSetFlag = 0;
	DoorNumDefine upgrHeadFlag = 0xE000000000000000;
	
	if(upgradeLLParam.proCtr.step  == UpgradeLL_Proc_Finsh){/*��������*/
		if(TickOut((uint32 *)&upgradeLLParam.appRunVer.itick, 400/*100*//*40*//*60*/) == true){
			TickOut((uint32 *)&upgradeLLParam.appRunVer.itick, 0);
			if(upgradeLLParam.appRunVer.cnt++ >= sizeof(uint16)*UpgradeLL_FrameMax){
				if(comBupTemp.binFileType.flag == BatBoardType
					|| comBupTemp.binFileType.flag == ChargeBoardType){/*20210226��ع̼�/������̼�����*/
					/*����������ɱ�־*/
					upgradeLLParam.proCtr.binDataItemEndFlag |= (DoorNumDefine)upgrHeadFlag;/*һ���������֮����ȷ�ϳɹ�,ʧ�ܸ���,��;�˳�,�ݲ���������*/
					update_Upperfiled(upgradeLLParam.proCtr.binDataItemEndFlag, 0x02);	
				}
				
				/*20210202==������������--���������߼�����*/
				if(comBupTemp.binFileType.flag == ComBoradType){/*����Էֿ�����������������Ϊһ���ֻ�����*/
					upgrFlagSetFlag |= (DoorNumDefine)upgradeLLParam.proCtr.binDataItemEndFlag;
					if(upgrLLPara.proCtrRep.cnt == BroadCast_UgpradeMaxCnt
						|| assign_ChkVer(assignLabel) == false/*�汾һ��*/){/*����:20210201--����ʧ��3��֮����ϱ����ԭ��*/
						upgrFlagSetFlag |= (DoorNumDefine)upgrHeadFlag;	
						/*�����ϲ�Э���������ֶδ���*/
						update_Upperfiled(upgrFlagSetFlag, 0x02);/*���¹̼�/�����ļ����*/
						/*20210227--�����ɹ���λ֮�����־*/
						upgrFlagSetFlag = 0;
					}
				}
				
				if(/*(upgradeLLParam.proCtr.upgradePermit == upgradeLLParam.proCtr.binDataItemEndFlag
					&& upgradeLLParam.proCtr.upgradePermit != 0)
					||*/ upgrLLPara.proCtrRep.cnt >=  BroadCast_UgpradeMaxCnt
					|| assign_ChkVer(assignLabel) == false/*�汾һ��*/){
					upgradeLLParam.proCtr.step  = UpgradeLL_Proc_Over;	
					/*
					** �����ǰͨѶ�����������
					** �ֿش�bootloader�˳���Ҫʱ��
					*/
					if(comBupTemp.binFileType.flag == ComBoradType){/*��ʱ5S*/
						Sleep(5000);
					}
					Clear_CurComBupPara();	
				}else{
					/*���۷ֿ������ɹ�����ʧ�ܶ���ʱ*/
					if(comBupTemp.binFileType.flag == ComBoradType){/*��ʱ5S*/
						Sleep(5000);
					}					
					UpgradeLLParam_Init();
					add_UpgrCnt();
				}	

				if(comBupTemp.binFileType.flag == BatBoardType 
					|| comBupTemp.binFileType.flag == ChargeBoardType){/*20210226��ع̼�/������̼�����*/
					/*������1.5�汾ÿ���������һ��֮����ʱ6S*/
					Sleep(6000);
				}
			}
			
			if(comBupTemp.binFileType.flag == ComBoradType){/*ͨѶ��*/
				assignLabel = 0;
			}else if(comBupTemp.binFileType.flag == ChargeBoardType){/*������̼���*/
				memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&comBupTemp.detailedInfo.hardVer,sizeof(uint16));
				memcpy((uint8*)&upgr.upgrFileType.detailedInfo.id[0],(uint8*)&comBupTemp.detailedInfo.id[0],16);
				assignLabel = get_ChargeLocation(upgr);
				assignLabel += 0x80;
			}else{/*��ذ��̼�*/
				memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&comBupTemp.detailedInfo.hardVer,sizeof(uint16));
				memcpy((uint8*)&upgr.upgrFileType.detailedInfo.id[0],(uint8*)&comBupTemp.detailedInfo.id[0],16);
				assignLabel = get_BatLocation(upgr);
				assignLabel += 1;
			}
			update_ComBatSoftVer((uint8 *)&len, (uint8 *)&tx[0], assignLabel);
 			CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, UpgradeLL_FunId_ChkAppRunVer);
		}
	}
}

/*
** ָ��ȡ��������ӦͨѶ��������
*/
void UpgradeLL_AssignCanelUpgrade(uint8 addr){
	uint8 txlen = 0;
	uint8 item[8] = {0};
	BinFileItemPara binFileItemPara = getCurComBup().binFileItemPara;

	/*
	** ������ ������������ �ļ��ֽ��� �ļ�����У��
	*/
	item[txlen] = 0xFD;/*ȡ������*/
	txlen += sizeof(uint8);
	memcpy((uint8*)&item[txlen],(uint8*)&binFileItemPara.len,sizeof(uint32));
	txlen += sizeof(uint32);
	memcpy((uint8*)&item[txlen],(uint8*)&binFileItemPara.crc16,sizeof(uint32));
	txlen += sizeof(uint16);

	CAN_TransmitAnalysis(CAN_Port_1, txlen, (uint8 *)&item[0], addr, UpgradeLL_FunId_FileByteCrc);
}

/*
** �ṩϵͳ���֮Զ�������ӿ�����֮�²�Э��֮�ظ�����
*/
void SM_UpgradeLLReply_Task(void* p_arg){
 	ComBup comCurBupTemp = {0}; 
	/*
	** ����֮�����ṹ�������ʼ��
	*/
	UpgradeLLParam_Init();
	/*
	** ���������������
	*/
	clear_UpgrLLParamCnt();

	/*
	** ϵͳ��λ��ʱ6S,��ֹ����������,����ͨѶ��������Bootloader��
	*/
	Sleep(6000);
	
	for(;;){
		comCurBupTemp = getCurComBup();
		if(comCurBupTemp.binFileType.flag != 0){
			/*
			** ����֮�²�Э��֮��ѯ����ͨѶС����������
			*/
			UgradeLL_BCComRunAreaLogic();	
			/*
			** ����֮�²�Э��֮�㲥����֮�ļ��ֽ���+�ļ�����У��
			*/
			UpgradeLL_BCFileByteItemCrc();		
			/*
			** ����֮�²�Э��֮�㲥����֮�ļ�����
			*/
			UpgradeLL_BCFileItem();	
			/*
			** ��ѯApp����汾��
			*/
			UpgradeLL_BCChkAppVer();
		}
		/*
		** ���¿��Ź��Ĵ���--ι��
		*/
		watchdogUpdate();
		
		Sleep(5);
	}
}

/*
** �ṩϵͳ���֮Զ�������ӿں���֮�²�Э��֮�ظ�����
*/
void UpgradeLLReply_Init(void){
	Thread_create(SM_UpgradeLLReply_Task, 0,
		(int32 *)&upgradeLLReply_TaskStk[512-1], (int8)UgradeLLReplyTask_Prio);
}

