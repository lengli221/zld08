#include "includes.h"

static int32 lowerLayerReply_TaskStk[512] = {0};

/*
** ���������ļ���������--ʹ�ú���ָ��
*/
UpperLayerPara* llR_ULP = null;
SysParam* llR_SysPara = null;

/*
** �²�Э���������֮��������ļ�ʹ��extern 
*/
extern LowerLayerParam llParam;

/*
** �²�Э��֮������ʼ��
*/
static void LowerLayerReplyParam_Init(void){
	llR_ULP = getUpperLayerPara();
	llR_SysPara = get_SysParaPtr();
	/*
	** clear ͨѶ��/��ذ�����汾��
	*/	
	clear_AppVer();
}

/*
** state Chk Not download File
*/
bool stateChkNotDownloadFile(uint8 deviceAddr){
	bool ret = false;

	if((llR_ULP->stateInfoChange.sysLogic.comUpgr&((DoorNumDefine)(((DoorNumDefine)0x01)<<deviceAddr))) == (DoorNumDefine)0/*�ֿ�����δ������*/
		&& (llR_ULP->stateInfoChange.sysLogic.batFileDownload&((DoorNumDefine)(((DoorNumDefine)0x01)<<deviceAddr))) == (DoorNumDefine)0/*����ļ�����δ������*/
		&& (llR_ULP->stateInfoChange.sysLogic_2.chargeFileDownloading&((DoorNumDefine)(((DoorNumDefine)0x01)<<deviceAddr))) == (DoorNumDefine)0/*������ļ�����δ������*/
		){
		ret = true;
	}

	return ret;
}

/*
** �²�Э��֮�ظ�֮��ѯ�����Ϣ
*/
void LLReply_ChkBatStateInfo(uint8 cmd,uint8* len,uint8* item){
	uint8 txlen = 0;
	uint8 socLinit = get_SocLimit();
	uint16 chgOverTemp = get_ChgOverTempLimit();
	uint16 chgTime = get_ChgTimeLimit();
	uint16 sysModulePara = 0;

	/*
	** ������
	** 1.ϵͳ����--�漰ͨѶ��ֹͣ��紦���߼�
	** 2.soc��ֵ
	** 3.�����������ֵ
	** 4.���ʱ����ֵ 0xD38F -- 1101 0011 1000 1111
	*/
	sysModulePara = llR_ULP->stateInfoChange.sysModuleStateInfo.flag & SysModuleStateInfo_MaskBit_Upgr;
	memcpy((uint8*)&item[txlen],(uint8*)&sysModulePara,sizeof(uint16));
	txlen += sizeof(uint16);
	
	item[txlen] = socLinit;
	txlen += sizeof(uint8);
	memcpy((uint8*)&item[txlen],(uint8*)&chgOverTemp,sizeof(uint16));
	txlen += sizeof(uint16);
	memcpy((uint8*)&item[txlen],(uint8*)&chgTime,sizeof(uint16));
	txlen += sizeof(uint16);
	/*
	** �����Ӧ��־λ
	*/
	item[txlen] = cmd;
	txlen += sizeof(uint8);

	/*
	** �������
	*/
	*len = txlen;
}


/*
** �²�Э��֮��ѯ��ѯ�����Ϣ
*/
void LLReplay_PollingChkBatStateInfo(void){
	static uint8 deviceAddr = 0;
	uint8 len = 0;
	uint8 tx[8] = {0};

	if(stateChkNotDownloadFile(deviceAddr) == true){		
		LLReply_ChkBatStateInfo(0x00,(uint8 *)&len, (uint8 *)&tx[0]);
		CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, LL_FunId_BatDoorState);
	}
	
	llParam.comBoardChk[deviceAddr].cnt++;
	if((TickOut((uint32 *)&llParam.comBoardChk[deviceAddr].itick, 60000) == true) &&llParam.comBoardChk[deviceAddr].cnt >= LLParse_FrameCntMax){
		llParam.comBoardChk[deviceAddr].cnt = LLParse_FrameCntMax;
		if((llR_ULP->stateInfoChange.sysLogic.comUpgr&((DoorNumDefine)(((DoorNumDefine)0x01)<<deviceAddr))) == (DoorNumDefine)0/*�ֿ�����δ������*/
			/*stateChkNotDownloadFile(deviceAddr) == true*/
			){/*20210226--����ԭ��:������Ϣ-->0x03--�����ѯ,�����������ع̼���ͬʱҲ���ѯ״̬--˵��:���ڷֿ���boot��δ�ظ�FunId = 0x03�ֶ�*/
				llParam.comBoardChk[deviceAddr].comAbn = true;/*ͨѶ�嶪ʧ*/
				/*20210226--�ֿ�ʧ��ǿ�ƽ�������״̬�������ļ�״̬*/
				/*�Ż����������:20210227*/
				llR_ULP->stateInfoChange.sysLogic.comUpgr &= ~(DoorNumDefine)((DoorNumDefine)1<<deviceAddr);
				llR_ULP->stateInfoChange.sysLogic.comUpgrIsOk &= ~(DoorNumDefine)((DoorNumDefine)1<<deviceAddr);
				llR_ULP->stateInfoChange.sysLogic.batFileDownload &= ~(DoorNumDefine)((DoorNumDefine)1<<deviceAddr);	
				llR_ULP->stateInfoChange.sysLogic.batFileDownloadIsOk &= ~(DoorNumDefine)((DoorNumDefine)1<<deviceAddr);	
				llR_ULP->stateInfoChange.sysLogic_2.chargeFileDownloading &= ~(DoorNumDefine)((DoorNumDefine)1<<deviceAddr);
				llR_ULP->stateInfoChange.sysLogic_2.chargeFileDownloadFinsh &= ~(DoorNumDefine)((DoorNumDefine)1<<deviceAddr);
				/*20210227--����������͵�����������־*/
				llR_ULP->stateInfoChange.sysLogic.batUpgr &= ~(DoorNumDefine)((DoorNumDefine)1<<deviceAddr);	
				llR_ULP->stateInfoChange.sysLogic.batUpgrIsOk &= ~(DoorNumDefine)((DoorNumDefine)1<<deviceAddr);	
				llR_ULP->stateInfoChange.sysLogic_2.chargeUpgr &= ~(DoorNumDefine)((DoorNumDefine)1<<deviceAddr);
				llR_ULP->stateInfoChange.sysLogic_2.chargeUpgrIsOk &= ~(DoorNumDefine)((DoorNumDefine)1<<deviceAddr);				
		}else{/*�ֿ������в��ж�ʧ��*/
			TickOut((uint32 *)&llParam.comBoardChk[deviceAddr].itick, 0);
			llParam.comBoardChk[deviceAddr].cnt = 0;
			llParam.comBoardChk[deviceAddr].comAbn = false;
		}
	}
	
	deviceAddr++;
	if(deviceAddr == SysCtr_AllDoorNum){
		deviceAddr = 0;
	}
}

/*
** fill Chg Sys Info
*/
void fill_ChgSysInfo(uint8* item,uint8* len){
	uint8 txlen = 0;
	
	/*����ֿش���һ������*/
	if(chk_IsExitBatChargerOT() == true){
		item[txlen] |= 0x01; 
	}
	txlen += sizeof(uint8);

	/*����������*/
	item[txlen] = get_ChargerOCLimit();
	txlen += sizeof(uint8);

	/*��ع�����ֵ*/
	item[txlen] = get_BatOTempLimit();
	txlen += sizeof(uint8);

	/*�϶����±���*/
	item[txlen] = get_NanduLowPLimit();
	txlen += sizeof(uint8);

	/*��ص��±���*/
	item[txlen] = get_BatLowPLimit();
	txlen += sizeof(uint8);

	/*������*/
	*len = txlen;
}

/*
** �²�Э��֮��ѯ����ϵͳ��Ϣ
*/
void LLReply_ChkChgSysInfo(void){
	static uint8 deviceAddr = 0;
	uint8 len = 0;
	uint8 tx[8] = {0};

	//if(stateChkNotDownloadFile(deviceAddr) == true){
		/*fill Chg Sys Info*/		
		fill_ChgSysInfo((uint8 *)&tx[0], (uint8 *)&len);
		CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, LL_FunId_ChgSysInfo);
	//}
	deviceAddr++;
	if(deviceAddr == SysCtr_AllDoorNum){
		deviceAddr = 0;
	}
}

/*
** ��ѯ�����Ϣ
*/
void LLReply_ChkBmsInfo(void){
	static uint8 deviceAddr = 0;
	uint8 len = 1;
	uint8 tx[8] = {0};
	uint8 i = 0;

	for(i=deviceAddr;i<SysCtr_AllDoorNum;i++){
		if(llParam.batDoor[i].batDoorStateInfo.bits.batOnline == true){
			deviceAddr = i;
			if(stateChkNotDownloadFile(deviceAddr) == true){
				CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, LL_FunId_BmsInfo);
			}
			break;
		}
	}

	deviceAddr++;
	if(deviceAddr == SysCtr_AllDoorNum){
		deviceAddr = 0;
	}
}

/*
** chk Charge State info
*/
void LLReply_ChkChargeStateInfo(void){
	static uint8 deviceAddr = 0;
	uint8 len = 1;
	uint8 tx[8] = {0};

	if(stateChkNotDownloadFile(deviceAddr) == true){
		CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, LL_FunId_chargeStateInfo);
	}

	deviceAddr++;
	if(deviceAddr == SysCtr_AllDoorNum){
		deviceAddr = 0;
	}	
}

/*
** chk Charge realtime info
*/
void LLReply_ChkChargeRealtimeInfo(void){
	static uint8 deviceAddr = 0;
	uint8 len = 1;
	uint8 tx[8] = {0};

	if(stateChkNotDownloadFile(deviceAddr) == true){
		if(llParam.chargeInfo[deviceAddr].state.bits.online == true/*���������*/){
			CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, LL_FunId_chargeRealtimeInfo);
		}
	}

	deviceAddr++;
	if(deviceAddr == SysCtr_AllDoorNum){
		deviceAddr = 0;
	}
}

/*
** reset Chk Charger Base Info
*/
DoorNumDefine reset_IsGetBaseInfoFlag = 0;/*��⸴λ�Ƿ��ȡ��������Ϣ*/
void reset_ChkChargerBaseInfo(void){
	static uint8 deviceAddr = 0;
	uint8 len = 1;
	uint8 tx[8] = {0x01};/*��ѯ������Ϣ*/
	
	if((stateChkNotDownloadFile(deviceAddr) == true)
		&&((reset_IsGetBaseInfoFlag&((DoorNumDefine)(((DoorNumDefine)0x01)<<deviceAddr))) == (DoorNumDefine)0
		&& (llParam.chargeInfo[deviceAddr].state.bits.online == true))/*���ļ����غͳ����������Ϣδ��ȡ�� + ����������߼�*/){
		CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, LL_FunId_chargeBaseInfo);
	}

	deviceAddr++;
	if(deviceAddr == SysCtr_AllDoorNum){
		deviceAddr = 0;
	}	
}

/*
** power On ����ļ��汾��Ϣ�Ƿ�һ����
** @param:
**			label->���
** @return:
**			true->��һ��-->������������
**			false->һ��
*/
extern bool oneWaringResetSetFlag;
bool powerOn_ChkVer(uint8 label){
	uint8 i = 0;
	uint16 binFileVer = 0;
	UpgrFilePara upgrTemp = {0};
	bool ret = false;

	if(label == 0){
		binFileVer = llR_SysPara->newSoftVer.comFile;/*ͨѶ���ⲿFlash�洢����汾��*/
	}else if(label >= 0x80){
		upgrTemp = get_UpgrChargeFilePara(label - 0x80);
		binFileVer = upgrTemp.upgrFileType.softVer;/*�ⲿ������ļ��汾��*/
	}else{
		upgrTemp = get_UpgrBatFilePara(label - 1);
		binFileVer = upgrTemp.upgrFileType.softVer;/*�ⲿ��ع̼��ļ��汾��*/
	}
	
	for(i=0;i<SysCtr_AllDoorNum;i++){
		/*����:��֧��δ����һ�����������,�����ֿ�,�������¿��ļ�*/
		if(oneWaring_ForbidUpgrDownFile() == false && llR_ULP->stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose == false/*��һ�������ҿ��غ���*/
			&& oneWaringResetSetFlag == false/*20210302һ���澯�ѷ���,�µ�ָ�֮����������¿�ʼ�ж��Ƿ�����*/){
			if(llParam.comBoardChk[i].comAbn == false){/*ͨѶ������*/
				if(binFileVer != llR_SysPara->newSoftVer.comApp[i]/*binFileVer > llR_SysPara->newSoftVer.comApp[i]*/&& binFileVer != 0
					&& llR_SysPara->newSoftVer.upgrFailNumAcc[i] != 0xFFFF/*��Ӧ�ֿذ��Ӧ�̼�����ʧ�ܴ���δ�ﵽ3������*/){
					ret = true;
					break;
				}	
			}
		}
	}
	return ret;
}

/*
** update Com Bat Soft Version
*/
void update_ComBatSoftVer(uint8* len, uint8 *item,uint8 label){
	uint8 txlen = 0;
	/*
	** ��ѯͨѶ��App�汾��,��ѯָ����ذ��汾��Ϣ
	*/
	item[txlen] = label;
	txlen += sizeof(uint8);
	
	/*�����Ӧ�̼�*/
	item[txlen] = 0;
	if(label >= 1 && label < (get_batFireSize() + 1)){/*���ָ����ع̼���*/
		if(get_ClearFire(0x03, label - 1) == true){
			item[txlen] = 0xFE;
		}
	}
	if(label >= 0x80 && label < (0x80+ get_ChargeFireSize())){/*���ָ��������̼���*/
		if(get_ClearFire(0x04, label - 0x80) == true){
			item[txlen] = 0xFE;
		}		
	}
	txlen += sizeof(uint8);

	/*��Ӧ�̼��Ƿ�������ģʽ*/
	item[txlen] = 0;
	if(label >= 1 && label < (get_batFireSize() + 1)){/*��ع̼�*/
		if(get_FireEnterTestModel(0x03, label - 1) == true){
			item[txlen] = 0xEE;
		}
	}
	if(label >= 0x80 && label < (0x80+ get_ChargeFireSize())){
		if(get_FireEnterTestModel(0x04,  label - 0x80) == true){
			item[txlen] = 0xEE;
		}
	}
	txlen += sizeof(uint8);

	/*
	** �������
	*/
	*len = txlen;
}

/*
** ��ѯApp����汾�� -- FunID:0xF4 -- Ӧ��App����ݴ��� -- �㲥��ʽ����
** ���Ʋ���:
**			������ϵ��ж�ͨѶ��/��ع̼���app����汾�Ƿ�ƫ��
*/
extern void update_ComUpgrQueue(UpgrFilePara upgr);
void chkComAppVer_BC(void){
	static uint8 deviceAddr = DstAddr_BoardCast_Define;/*�㲥������ַ*/
	uint8 len = 0;
	uint8 tx[8] = {0};
	static bool ret = false;/*�ϵ��־*/
	static uint32 itick = 0;
	static uint8 cnt = 0;
	static uint8 label = 0;
	UpgrFilePara upgr = {0};
	
	if(ret == false){
		if(TickOut((uint32 *)&itick, 400) == true){
			TickOut((uint32 *)&itick, 0);
			if(cnt++ >= /*sizeof(uint16)**/UpgradeLL_FrameMax){/*�޸Ĳ���:���5��,ÿ��400ms����--һ���̼�ʱ��2S*/
				cnt = 0;
				if(powerOn_ChkVer(label) == true){/*��������*/
					if(label == 0x00){/*ͨѶ��*/
						upgr = get_UpgrComFilePara();
					}else if(label >= 0x80){/*��ع̼���*/
						upgr = get_UpgrChargeFilePara(label - 0x80);
					}else{
						upgr = get_UpgrBatFilePara(label - 1);
					}
					/*
					** �ݴ���
					** 		�������:--����Ϸֿذ�����bootloader�����汾�Ѵ��ڰ汾����,���ڴ��������ǿ������
					*/
					upgr.upgrFileType.cmdType = 0x02/*0x01*/;/*ԭʼ���:�ϵ�����֮���޸ĳ�"����ʲô������ʽ�޸ĳ�-->��������"*/
					update_ComUpgrQueue(upgr);
				}
				label++;
				if(label == get_batFireSize() + 1){/*��ع̼���*/
					label = 0x80;/*�л�����ѯ:������̼���*/
				}

				if(label >= (0x80 + get_ChargeFireSize())){/*������̼���*/
					label = 0;
				}
			}
			
			if(label < get_batFireSize()+ 1/*ͨѶ��,��ع̼���*/
				|| (label>= 0x80 && label<  (0x80 + get_ChargeFireSize()))/*������̼���*/
			){
				update_ComBatSoftVer((uint8 *)&len, (uint8 *)&tx[0], label);
				CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], deviceAddr, UpgradeLL_FunId_ChkAppRunVer);
			}
		}
	}	
}

/*
** �²�Э��֮�ظ��������ӿ�
*/
void SM_LowerLayerReply_Task(void* p_arg){
	static uint32 itick = 0;
	static uint32 itick1 = 0;
	static uint32 itick2 = 0;
	static uint32 itickCharge = 0;
	static uint32 itickChargeRealtime = 0;
	static uint32 itickChkChargerBaseInfo = 0;
	ComBup comCurBupTemp = {0}; 
	/*
	** �²�Э��֮������ʼ��
	*/
	LowerLayerReplyParam_Init();
	
	Sleep(2000);
	
	for(;;){
		if(TickOut((uint32 *)&itick, 10/*25*/) == true){/*���䷢��ʱ��2ms*/
			TickOut((uint32 *)&itick, 0);
			/*
			** ��ѯApp����汾�� -- FunID:0xF4 -- Ӧ��App����ݴ��� -- �㲥��ʽ����
			** ���Ʋ���:
			**			������ϵ��ж�ͨѶ��app����汾�Ƿ�ƫ��
			*/			
			comCurBupTemp = getCurComBup();
			if(comCurBupTemp.binFileType.flag == 0){
				chkComAppVer_BC();
			}
			/*
			** �²�Э��֮��ѯ��ѯ�����Ϣ
			*/
			LLReplay_PollingChkBatStateInfo();
		}

		if(TickOut((uint32 *)&itick2, 50) == true){
			TickOut((uint32 *)&itick2, 0);
			/*��ѯ--������Ϣ:ID-->0x03*/
			LLReply_ChkChgSysInfo();
		}

		if(TickOut((uint32 *)&itickCharge, 20) == true){/*���䷢��ʱ��2ms*/
			TickOut((uint32 *)&itickCharge, 0);
			/*
			** chk Charge State info
			*/
			LLReply_ChkChargeStateInfo();			
		}

		if(TickOut((uint32 *)&itick1, 100/*200*/) == true){/*���䷢��ʱ��2ms*/
			TickOut((uint32 *)&itick1, 0);
			/*
			** ��ѯ�����Ϣ
			*/
			LLReply_ChkBmsInfo();			
		}

		if(TickOut((uint32 *)&itickChargeRealtime, 120) == true){/*���䷢��ʱ��2ms*/
			TickOut((uint32 *)&itickChargeRealtime, 0);
			/*
			** chk Charge realtime info
			*/
			LLReply_ChkChargeRealtimeInfo();	
		}

		if(TickOut((uint32 *)&itickChkChargerBaseInfo, 50) == true){/*���䷢��ʱ��*/
			TickOut((uint32 *)&itickChkChargerBaseInfo, 0);
			/*reset Chk Charger Base Info*/
			reset_ChkChargerBaseInfo();			
		}
		
		/*
		** CAN1 Tx Task
		*/
		CAN1_TxDataTask();
		/*
		** ���¿��Ź��Ĵ���--ι��
		*/
		watchdogUpdate();	
		
		Sleep(5);
	}
}

/*
**�ṩϵͳ��������²�Э���ѯ�ӿں���
*/
void LowerLayerReply_Init(void){
	Thread_create(SM_LowerLayerReply_Task, 0, 
		(int32 *)&lowerLayerReply_TaskStk[512-1], LowerLayerReplyTask_Prio);
}

