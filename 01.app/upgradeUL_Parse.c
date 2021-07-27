#include "includes.h"

static int32 upgradeULParse_TaskStk[1024] = {0};

extern UpperLayerPara upperLayerPara;

/*
** Զ������֮ϵͳ����
*/
UpgradeSysPara upgradeSysPara;
UpgradeReply upgradeReply;

/*
** Զ������֮ϵͳ������ʼ��
*/
static void UpgradePro_ParaInit(void){
	memset((uint8*)&upgradeSysPara.processCtr.step,0x00,sizeof(UpgradeSysPara));
	/*
	** �������̲���
	*/
	upgradeSysPara.processCtr.step = ChkFrameStartFlag;
	upgradeSysPara.processCtr.binFilePara.len = 0;
	upgradeSysPara.processCtr.binFilePara.crc16 = 0;
	/*
	** Զ������
	** 1.�̼���ַ֮������ʼ��
	** 2.��ʼ��Flash��ʼ���ڲ���ַ
	*/
	upgradeSysPara.firmware.addr = Firmware_HandwareAddr;
	upgradeSysPara.firmware.interFlashAddr = getUpgrFlashAddr(getUpgrFilePara());
	/*
	** ���»ظ���������
	*/
	upgradeReply.ack = UpgradeFrame_Ack;
}

/*
** У������֡������Ĵ�С,��ֹ�ڴ����ݸ��Ƶ���Ӳ�ж�
*/
static bool ChkFrameDataItemLenIsOk(uint16 len){
	uint16 baseLen = 0;
	bool ret = false;

	switch(upgradeSysPara.processCtr.step){
		case ChkFrameStartFlag:
			baseLen = sizeof(FileNameItem);	
			break;
		case ChkFrameIngFlag:
			baseLen = FrameDataItem_MaxLen;
			break;
	}

	if(len <= baseLen){
		ret = true;
	}

	return ret;
}

/*
** У���ַ�Ϸ���
*/
static bool ChkFrameAddrIsOk(uint8 addr){
	bool ret = true;

	if(addr != upgradeSysPara.firmware.addr){
		ret = false;
	}

	return ret;
}

/*
** У��֡ID�Ϸ���
*/
static bool ChkFrameIdIsOk(uint16 frameId){
	bool ret = true;

	switch(upgradeSysPara.processCtr.step){
		case ChkFrameStartFlag:
			if(frameId != 0){
				ret = false;
			}
			break;
		case ChkFrameIngFlag:
			if(((uint8)(frameId - upgradeSysPara.processCtr.frameLastId )) != 1){
				ret = false;
			}
			break;
	}
	
	return ret;
}

/*
** ����ͨѶ����������
*/
void update_ComUpgrQueue(UpgrFilePara upgr){
	/*
	** ͨѶ����в���
	*/
	ComBup com = {0};

	com.upgradeWay = upgr.upgrFileType.cmdType;
	com.binFileType.flag = upgr.upgrFileType.board;
	com.detailedInfo = upgr.upgrFileType.detailedInfo;/*�����ϸ��Ϣ*/
	com.softVer = upgr.upgrFileType.softVer;
	com.binFileItemPara.len = upgr.upgrFileType.binFileItemPara.len;
	com.binFileItemPara.crc16 = upgr.upgrFileType.binFileItemPara.crc16;
	com.flashAddr = upgr.upgrFileType.flashAddr;
	UpdataComBoardQueuePara(com);	
}

/*
** Զ������֮�ظ�֡����
*/
static void Upgrade_ReplyFrame(void){
	uint8 tx[sizeof(UpgradeProFrame)+sizeof(UpgradeReply)] = {0};
	uint32 len = 0;
	uint16 crc16=0;
	UpgradeProFrame upgradeProFrame; 

	/*20210129--����һ���澯�����˳�*/
	if(oneWaring_ForbidUpgrDownFile() == true){
		upgradeReply.flag = 0xFE;
	}
	
	upgradeProFrame.flag = Frame_Head;
	upgradeProFrame.addr = upgradeSysPara.firmware.addr;
	upgradeProFrame.frameId = upgradeSysPara.processCtr.frameId;
	upgradeProFrame.dataFrameLen = sizeof(UpgradeReply);
	memcpy((uint8*)&tx[len],(uint8*)&upgradeProFrame.flag,sizeof(UpgradeProFrame));
	len += (sizeof(UpgradeProFrame)-sizeof(uint16));

	memcpy((uint8*)&tx[len],(uint8*)&upgradeReply.ack,sizeof(UpgradeReply));
	len += sizeof(UpgradeReply);

	crc16 = CRC16((uint8*)&tx[0],(uint32)len);

	memcpy((uint8*)&tx[len],(uint8*)&crc16,sizeof(uint16));
	len += sizeof(uint16);
		
	BSP_SmSend(BSP_ComType_USART, BSP_ComUpperLayer, (uint8 *)&tx[0], (uint16 *)&len);
}

/*
** Զ������֮У������֡����֡���Ϸ��Իظ�
*/
static void ChkFrameDataIsIllegal(uint16 frameId){
	upgradeReply.flag = ((uint8)(frameId - upgradeSysPara.processCtr.frameLastId)) == 0?FrameIdRepeat:FrameIdSpan;
	Upgrade_ReplyFrame();
}

/*
** Զ������֮���ƺ���
*/
static void Upgrade_StartEndCancelFun(void*rx,uint16 dataItemLen){
	FileNameItem fileNameItem = {0};
	uint8 flag = 0;
	uint16 sectorNum = 0;
	uint16 i = 0;
	uint8 tempData[FrameDataItem_MaxLen] = {0};
	uint16 readFlashNum = 0;
	uint32 flashInitAddr = Upgrade_FlashAddrInit;
	/*
	** ����������ַ
	*/
	uint32 flashBaseAddr = Upgrade_FlashAddrInit;
	uint16 uchInit = 0xFFFF;
	uint8 indexInit = 0;
	uint16 binFileCrc16 = 0;
	/*
	** ���ư������ļ���������
	*/
	UpgrFilePara upgr = getUpgrFilePara();
	/*
	** ����flash��ʼ/������ַ��ַ
	*/
	flashInitAddr = getUpgrFlashAddr(upgr);
	flashBaseAddr = flashInitAddr;
	
	if(dataItemLen <= sizeof(FileNameItem)){/*���̿���֡����Ҫ��,copy����*/
		memcpy((uint8*)&fileNameItem.flag,(uint8*)rx,dataItemLen);
	}

	switch(fileNameItem.flag){
		case ChkFrameStartFlag:
			flag = ChkFrameStartFlag;
			sectorNum = (uint16)(fileNameItem.word/sFLASH_SPI_SECTORSIZE);
			sectorNum += (uint16)(fileNameItem.word%sFLASH_SPI_SECTORSIZE) !=0?1:0;
			for(i=0;i<sectorNum;i++){
				sFLASH_EraseSector((uint32)(flashBaseAddr + i*sFLASH_SPI_SECTORSIZE));
			}
			/*
			** ��������֡�쳣֮��,���¿�ʼ����֡����
			*/
			UpgradePro_ParaInit();
			/*
			** ����bin�ļ�����֡
			*/
			upgradeSysPara.processCtr.step = ChkFrameIngFlag;
			upgradeSysPara.processCtr.binFilePara.crc16 = fileNameItem.fileCrc16;
			break;
		case ChkFrameStopFlag:
			flag = ChkFrameStopFlag;
			/*
			** ����bin�ļ�У��
			*/
			readFlashNum = (uint16)(upgradeSysPara.processCtr.binFilePara.len/FrameDataItem_MaxLen);
			for(i=0;i<readFlashNum;i++){
				sFLASH_ReadBuffer((uint8 *)&tempData[0], flashInitAddr, FrameDataItem_MaxLen);
				binFileCrc16 = CRC16_Upgrade((uint8 *)&tempData[0], FrameDataItem_MaxLen, uchInit, (uint8 *)&indexInit);
				uchInit = binFileCrc16;
				memset((uint8*)&tempData[0],0x00,FrameDataItem_MaxLen);
				flashInitAddr += FrameDataItem_MaxLen;
			}
			
			if((upgradeSysPara.processCtr.binFilePara.len - readFlashNum*FrameDataItem_MaxLen) != 0){
				sFLASH_ReadBuffer((uint8 *)&tempData[0], flashInitAddr, 
					upgradeSysPara.processCtr.binFilePara.len - readFlashNum*FrameDataItem_MaxLen);
				binFileCrc16 = CRC16_Upgrade((uint8 *)&tempData[0], 
					upgradeSysPara.processCtr.binFilePara.len - readFlashNum*FrameDataItem_MaxLen, uchInit, (uint8 *)&indexInit);
			}

			/*
			** У��Ϸ�
			*/
			if(binFileCrc16 == upgradeSysPara.processCtr.binFilePara.crc16 && binFileCrc16 == fileNameItem.fileCrc16
				&& upgradeSysPara.processCtr.binFilePara.len == fileNameItem.word){
				/*�ļ�����ʽ�쳣(������bin�ļ�)*/
				if(FileNameVerOpt((uint8 *)&fileNameItem.fileName[0], dataItemLen-sizeof(uint8)-sizeof(uint32)-sizeof(uint16))){
					/*
					** set����bin�ļ�����
					** ����:���ݵ�ذ���ϸ��Ϣ(Ӳ���汾/ID)�洢ָ����ַλ��
					** 		��������:
					**				1.�ļ�������
					**				2.�ļ���
					**				3.�ļ�������
					** 				4.�ļ�У������
					**				5.����flash�洢��ַ
					*/
					upgr = getUpgrFilePara();
					upgr.upgrFileType.fileNameLen = dataItemLen-sizeof(uint8)-sizeof(uint32)-sizeof(uint16);
					memcpy((uint8*)&upgr.upgrFileType.fileName[0],(uint8*)&fileNameItem.fileName[0],
						 dataItemLen-sizeof(uint8)-sizeof(uint32)-sizeof(uint16));
					upgr.upgrFileType.binFileItemPara.len = upgradeSysPara.processCtr.binFilePara.len;
					upgr.upgrFileType.binFileItemPara.crc16 = upgradeSysPara.processCtr.binFilePara.crc16;
					upgr.upgrFileType.flashAddr = flashBaseAddr;
					set_UpgrBinFilePara(upgr);
					/*
					** ���¿��ư����/ͨѶ������汾��
					*/
					updateSoftVerPara();

					if(oneWaring_ForbidUpgrDownFile() == false){
						/*
						** ����ͨѶ����������
						*/
						update_ComUpgrQueue(upgr);
					}
					
					/*
					** ���ư�֮�����������֮�����ѱ���
					*/
					memset((uint8*)&upgr.upgrFileType.cmdType,0x00,sizeof(UpgrFilePara));
					setUpgrFilePara(upgr);
					/*
					** set �ָ��ϲ�Э��֮APPӦ�ò�Э���־
					*/
					set_CtrBoardTaskRecoverFlag();
				}else{
					flag = FileNameFormatIilegal;
				}
			}else{/*�ļ�����У�����*/
				flag = FileItemChkErr;
			}
			break;
	}

	/*
	** ���»ظ���־
	*/
	upgradeReply.flag = flag;
}

/*
** Զ������֮���������ݺ���
*/
static void Upgrade_DataItemFun(void*rx,uint16 dataItemLen){
	uint8 flag = ChkFrameIngFlag;

	/*
	** Error:#852:expression must be a pointer to complete object type
	*/
	/*
	** sFLASH_WriteData((uint8*)&rx[0],upgradeSysPara.firmware.interFlashAddr, dataItemLen);
	*/
	sFLASH_WriteData((uint8*)rx,upgradeSysPara.firmware.interFlashAddr, dataItemLen);
	/*
	** ����Flashд��ַ ����binд���ֽ���
	*/
	upgradeSysPara.firmware.interFlashAddr += dataItemLen;
	upgradeSysPara.processCtr.binFilePara.len += dataItemLen;
	/*
	** ����֡ID
	*/
	upgradeSysPara.processCtr.frameLastId = upgradeSysPara.processCtr.frameId;
	/*
	** ���»ظ���־
	*/
	upgradeReply.flag = flag;	
}

/*
** Զ������֮����ִ�к������
*/
static const UpgradePro upgradeProTabel[] = {
	{ChkFrameStartFlag,Upgrade_StartEndCancelFun},
	{ChkFrameIngFlag,Upgrade_DataItemFun}
};
uint8 upgradeProTabelNum = sizeof(upgradeProTabel)/sizeof(UpgradePro);

/*
** Զ������֮����ִ�к���
*/
static void UpgradeProTabelExecuteFun(uint8*rx,uint16 dataItemLen){
	uint8 i=0;
	
	for(i=0;i<upgradeProTabelNum;i++){
		if(upgradeSysPara.processCtr.step == upgradeProTabel[i].step){
			upgradeProTabel[i].handle(rx,dataItemLen);
			Upgrade_ReplyFrame();
			break;
		}
	}
}

/*
** ��������Э��֡
*/
static void ParseUpgradeProFrame(uint8*rxbuf,uint16*rxlen){
	uint8* ptrbuf = rxbuf;
	uint16 updataLen = *rxlen;
	uint16 saveDataLen = *rxlen;
	uint16 dataItemLen = 0;
	uint16 crc16 = 0;
	uint16 frameId = 0;
	
	for(;updataLen>=(uint16)(sizeof(UpgradeProFrame));){
		/*˵��:���������Ż���,�������*/
		memcpy((uint8*)&dataItemLen,(uint8*)&ptrbuf[4],sizeof(uint16));
		if(ptrbuf[0] == Frame_Head && ChkFrameDataItemLenIsOk(dataItemLen)){
			if(updataLen >= dataItemLen+sizeof(UpgradeProFrame)){
				memcpy((uint8*)&crc16,(uint8*)&ptrbuf[sizeof(UpgradeProFrame)-sizeof(uint16)+dataItemLen],sizeof(uint16));
				if(crc16 == CRC16((uint8 *)&ptrbuf[0], sizeof(UpgradeProFrame)-sizeof(uint16)+dataItemLen)){
					/*֡ID*/
					memcpy((uint8*)&frameId,(uint8*)&ptrbuf[2],sizeof(uint8)+sizeof(uint8));
					/*����"�������ݽ���֡"*/
					if(frameId == 0){
						upgradeSysPara.processCtr.step = ChkFrameStartFlag;
					}	
					/*���µ�ǰ֡ID*/
					upgradeSysPara.processCtr.frameId = frameId;
					if(ChkFrameAddrIsOk(ptrbuf[sizeof(uint8)]) && ChkFrameIdIsOk(frameId)){
						/*ִ֡�к���*/
						UpgradeProTabelExecuteFun((uint8*)&ptrbuf[6],dataItemLen);
						/*
						** �峬ʱ��ʱ��
						*/
						TickOut((uint32 *)&upperLayerPara.upgrComBatTaskEn.itickout, 0);
					}else{
						/*��ַ�Ϸ�,֡�쳣*/
						if(ChkFrameAddrIsOk(ptrbuf[sizeof(uint8)])){
							ChkFrameDataIsIllegal(frameId);
						}
					}
					ptrbuf += (dataItemLen+sizeof(UpgradeProFrame));
					updataLen -= (dataItemLen+sizeof(UpgradeProFrame));
					dataItemLen = 0;
					/*
					** ��������ļ���ʱ
					*/
					//UpgradeCom_ClearComTickOut();
				}else{
					ptrbuf++;
					updataLen--;
					dataItemLen = 0;
				}
			}else{
				break;
			}
		}else{
			ptrbuf++;
			updataLen--;
			dataItemLen = 0;
		}
	}
	memcpy((uint8*)&rxbuf[0],(uint8*)&ptrbuf[0],updataLen);
	*rxlen = updataLen;
	memset((uint8*)rxbuf+updataLen,0x00,saveDataLen-updataLen);
}

/*
** Զ������֮����bin�ļ�֮����˸Ƶ��--500ms
** param:flag:true--���� false--�ر�
*/
void UpgradeUL_CtrLedLogic(bool flag){
	static uint32 cnt = 0;
	static bool ledFlickFlag = false;
	
	if(flag == true){/*������˸*/
		if(TickOut((uint32 *)&cnt, 500) == true){
			TickOut((uint32 *)&cnt, 0);
			ledFlickFlag = ledFlickFlag == false?true:false;
			firewareCtr(setFirewareUpgre_Label, ledFlickFlag);
		}			
	}else{/*�ر���˸*/
		firewareCtr(setFirewareUpgre_Label, false);
	}
}

/*
** �ṩϵͳ���֮Զ�������ӿ�����֮�ϲ�Э��
*/
void SM_UpgradeULParse_Task(void* p_arg){
	/*
	*����֡,ճ֡,��֡,�쳣֡�����bufԣ��
	*/
	uint8 ucRxBuf[sizeof(uint16)*(FrameDataItem_MaxLen+sizeof(UpgradeProFrame))];/*��ȷ������stk֮��Ĺ�ϵ:��������512����ϵͳ��λ*/
	uint8 *rxptr;
	uint16 ucLen = 0;
	uint16 ucRecvLen = 0;	

	/*
	** Զ������֮ϵͳ������ʼ��
	*/
	UpgradePro_ParaInit();

	Sleep(6000);
	
	for(;;){
		ucRecvLen = 0;
		BSP_SmRecv(BSP_ComType_USART,BSP_ComUpperLayer,ucRxBuf + ucLen,&ucRecvLen);
		ucLen += ucRecvLen;
		if(ucLen >( sizeof(ucRxBuf))){
			ucLen = 0;
		}
		
		if(ucRecvLen == 0 && ucLen != 0){
			rxptr = ucRxBuf;
			ucRecvLen = ucLen;
			ParseUpgradeProFrame((uint8 *)rxptr, (uint16 *)&ucRecvLen);
			ucLen = ucRecvLen;
		}

		/*
		** ���¿��Ź��Ĵ���--ι��
		*/
		watchdogUpdate();
		
		Sleep(10);
	}
}

/*
** �ṩϵͳ���֮Զ�������ӿں���֮�ϲ�Э��
*/
void UgradeULParse_Init(void){
	Thread_create(SM_UpgradeULParse_Task, 0, 
		(int32 *)&upgradeULParse_TaskStk[1024-1], (int8)UgradeULParseTask_Prio);
}

