#include "includes.h"

static int32 upperLayerParse_TaskStk[1024] = {0};

/*
** ʹ��Extern���������ļ�����
*/
extern UpperLayerReplyPara ulReplyPara;
extern SysParam sysParam;

/*
** �ϲ�Э��֮�ṹ�����
*/
UpperLayerPara upperLayerPara;


/*
** �ϲ�Э��֮�ṹ�����ָ���ṩ�ӿڵ���
*/
UpperLayerPara* getUpperLayerPara(void){
	return(&upperLayerPara);
} 

/*
** ���¿��ư�����汾��
*/
void update_CtrSfotVer(uint16 ver){
	/*uint16 temp = 0;*/
	uint8 len = 0;
	uint8 charVer[4] = {0};/*����汾��*/
	#if CtrHardware == CtrHardware_Andriod
		uint8 charVerHard[4] = {"0200"};/*Ӳ���汾��*/
	#else
		uint8 charVerHard[4] = {"0100"};/*Ӳ���汾��*/
	#endif
	
	
	/*temp = bcdToUInt((const uint8 *)&ver, sizeof(uint16));*/
	len = _itoa(ver/*temp*/, (char*)&charVer[0]);
	memset((uint8*)&upperLayerPara.sysSet.ctrSoftVer[0],'0',4);
	memcpy((uint8*)&upperLayerPara.sysSet.ctrSoftVer[4-len],(uint8*)&charVer[0],len);
	memcpy((uint8*)&upperLayerPara.sysSet.ctrHareVer[0],(uint8*)&charVerHard[0],4);
}

/*
** ��ʼ���ϲ�Э�����
*/
static void init_UpperLayerPara(void){
	char temp[10] = {"�����"};
	char tempp[6] = {"CDR-U1"};
	uint16 ctrVer = get_CtrSoftVer();
	
	memset((uint8*)&upperLayerPara.sysSet.fireId[0],0x00,sizeof(UpperLayerPara));
	/*
	** ���¹����豸ID����
	*/
	memcpy((uint8*)&upperLayerPara.sysSet.id[0],(uint8*)&sysParam.deviceId.show[0],16);
	/*
	** ���̱�ʶ �����ͺ�
	*/
	memcpy((uint8*)&upperLayerPara.sysSet.fireId[0],(uint8*)&temp[0],10);
	memcpy((uint8*)&upperLayerPara.sysSet.ctrModel[0],(uint8*)&tempp[0],6);
	/*
	** ���¿��ư�����汾��
	*/
	update_CtrSfotVer(ctrVer);
}

/*
** update �ϲ���ر�־
*/
static void update_UpperFieldRef(void){
	DeviceId deviceIdTemp = {0};

	/*
	** ���¹�ѹ����,Ƿѹ���
	*/
	PhaseVCAbnCloseAllCharger();
	
	/*
	** �������ϵͳ����
	** 		1.�豸��ַ
	*/
	deviceIdTemp = get_SysDeviceId();
	memcpy((uint8*)&upperLayerPara.sysSet.id[0],(uint8*)&deviceIdTemp.show[0],sizeof(DeviceId));
}

/*
**У������֡���ݳ��������Ƿ�Ϸ�
*/
bool ChkFrameDataLenIsOk(uint16 frameLen){
	bool ret = false;

	if(frameLen>=FrameLenIsLegal_Min && frameLen<=FrameLenIsLegal_Max){
		ret = true;
		
	}
	return ret;
}

/*
**У���豸��ַ
*/
bool ChkDeviceAddrIsOk(uint8 addr){
	bool ret = true;

	return ret;
}

/*
** ע����Ϣ
*/
void UL_RegisterInfo(uint16 rxlen,uint8*rx){
	Serve s = {0};
	#if CtrHardware == CtrHardware_Andriod
		SmCmd cmd = {true,CMD_ID_Register_Tx,0,0};
	#endif
	
	switch(rx[0]){/*ע���Ƿ�ɹ�*/
		case 0x01:/*ע��ɹ�*/
				s.registerFlag = true;
				set_RegisterState(1);/*��ע��*/
			break;
		case 0x02:/*ע��ʧ��*/
				s.registerFlag = false;
			break;
	}	
	#if CtrHardware == CtrHardware_Andriod
	switch(rx[1]){/*ע����Ϣ֮��ѯ��ظ�*/
		case 0x02:/*��׿������ѯ*/
				setCmd(cmd);		
			break;
	}
	#endif
	/*
	** ����ע����Ϣ��־
	*/
	ulReplyPara.registerLogic.flag = false;
	/*
	** set����˲���
	*/
	setServePara(s);
}

/*
** ע����Ϣ
*/
void UL_LogoutInfo(uint16 rxlen,uint8*rx){
#if CtrHardware == CtrHardware_Dwin
	Serve s = {0};

	switch(rx[0]){/*ע����Ϣ*/
		case 0x01:/*ע���ɹ�*/
			s.registerFlag = false;
			set_RegisterState(0);/*δע��*/
			break;
		case 0x02:/*ע��ʧ��*/
			
			break;
	}
	/*
	** ����ע����Ϣ��־
	*/
	ulReplyPara.logoutLogic.flag = false;
	
	setServePara(s);
#endif
}

/*
** ״̬��Ϣ���
*/
void UL_StateInfoChange(uint16 rxlen,uint8*rx){
	SmCmd cmd = {true,CMD_ID_StateInfoChange_Tx,0,0};
	uint8 len = 0;

	/*
	** �ظ�/��ѯ
	*/
	switch(rx[len]){
		case 0x01:/*�ظ�*/
			
			break;
		case 0x2:/*��ѯ*/
			setCmd(cmd);
			break;
		case 0xFE:/*�ֿظ�λ��ȷ�ϱ�־*/
			upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.comRecoverFlag = false;
			break;
	}
	len += sizeof(uint8);
	/*
	** �����Ƿ�����
	*/
	switch(rx[len]){
		case 0x01:/*����*/
			upperLayerPara.sysFire.state.bits.isOnline = true;
			break;
		case 0x02:/*����*/
			upperLayerPara.sysFire.state.bits.isOnline = false;
			break;
	}
}

/*
** ״̬��Ϣ
*/
void UL_StateInfo(uint16 rxlen,uint8*rx){
	SmCmd cmd = {true,CMD_ID_StateInfo_Tx,0,0};

	setCmd(cmd);
}

/*
** ������Ϣ֮�¼��ӿں���������
*/
static const CabinetCtrInfo cabinetCtrInfoFunTable[] = {
	{CabCtrInfo_EventId_Contactor,cabCtrInfo_EventId_ContactorFun},
	{CabCtrInfo_EventId_UpdateSoc,cabCtrInfo_EventId_UpdateSocPara},
	{CabCtrInfo_EventId_ChgTime,cabCtrInfo_EventId_ChgTime},
	{CabCtrInfo_EventId_ChgOverTemp,cabCtrInfo_EventId_ChgOverTemp},
	{CabCtrInfo_EventId_ChgDoorOT,cabCtrInfo_EventId_ChgDoorOT},
	{CabCtrInfo_EventId_SysTempInfo,cabCtrInfo_EventId_SysTempInfo},
	{CabCtrInfo_EventId_UpgrCmd,cabCtrInfo_EventId_UpgradeFileCmd},
	{CabCtrInfo_EventId_GetFileName,cabCtrInfo_EventId_GetFileName},
	{CabCtrInfo_EventId_GetBatFireInfo,cabCtrInfo_EventId_GetBatFireInfo},
	{CabCtrInfo_EventId_GetchgerTemp,cabCtrInfo_EventId_AllchgTemp},
	{CabCtrInfo_EventId_GetWifiNamePw,cabCtrInfo_EventId_GetWifiNamePw},
	{CabCtrInfo_EventId_BmsPF,cabCtrInfo_EventId_BmsPFState},
	{CabCtrInfo_EventId_ChkSocLimit,cabCtrInfo_EventId_ChkSocLimit},
	{CabCtrInfo_EventId_ChkRunComVer,cabCtrInfo_EventId_ChkRunComVer},
	/*-------------------------------------------�����ѹУ׼ֵ--------------------------------------------------------------------------*/	
	{CabCtrInfo_EventId_ChkLvAd,cabCtrInfo_EventId_ChkPhaseLvVoltAD},
	{CabCtrInfo_EventId_ChkHvAd,cabCtrInfo_EventId_ChkPhaseHvVoltAD},
	{CabCtrInfo_EventId_SetPAdjust,cabCtrInfo_EventId_SetPhaseAjust},
	{CabCtrInfo_EventId_GetRuntimeV,cabCtrInfo_EventId_GetRuntimeVolt},
	/*-----------------------------------------------------------------------------------------------------------------------------------*/
	{CabCtrInfo_EventId_CfgFireUpgrN,cabCtrInfo_EventId_CfgFireUpgrNum},
	{CabCtrInfo_EventId_ClearFire,cabCtrInfo_EventId_ClearFire},
	{CabCtrInfo_EventId_UpgrFailR,cabCtrInfo_EventId_UpgrFailReson},
	{CabCtrInfo_EventId_FireEnterT,cabCtrInfo_EventId_FireEnterTestModel},
	{CabCtrInfo_EventId_ChkRealityN,cabCtrInfo_EventId_ChkRealityUpgrNum},
	{CabCtrInfo_EventId_TestMUpgrNum,cabCtrInfo_EventId_TestModelFireUpgrNum},
	{CabCtrInfo_EventId_ChargerOCL,cabCtrInfo_EventId_ChargerOCLimitCfg},
	/*---------------------------------------------------------����̼�------------------------------------------------------------------*/
	{CabCtrInfo_EventId_OneClearAllFire,cabCtrInfo_EventId_OneKeyClearFire},
	/*-----------------------------------------------------------------------------------------------------------------------------------*/
	/*----------------------------���ų���-��������������--��˽ӿ�(�ⲿ���)-1.05-----------------------------------------------------*/
	{CabCtrInfo_EventId_BatOTempLimit,cabCtrInfo_EventId_SetBatOTempLimit},
	{cabCtrInfo_EventId_NanduLowPLimit,cabCtrInfo_EventId_SetNanduLowPLimit},
	{CabCtrInfo_EventId_BatLowPLimit,cabCtrInfo_EventId_SetBatLowPLimit},
	/*-----------------------------------------------------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------��ؽ���SOC----------------------------------------------------------------*/
	{CabCtrInfo_EventId_ChkAllInsSoc,cabCtrInfo_EventId_ChkAllInsertSoc},
	/*-----------------------------------------------------------------------------------------------------------------------------------*/
	{CabCtrInfo_PhaseVCDebugMode,cabCtrInfo_EventId_PhaseCurOpenFlag}
};
static uint8 cabinetCtrInfoFunTableNum = sizeof(cabinetCtrInfoFunTable)/sizeof(CabinetCtrInfo);


/*
** ������Ϣ
*/
void UL_CtrInfo(uint16 rxlen,uint8*rx){
	SmCmd cmd = {true,CMD_ID_Ctr_Tx,0,0};
	uint16 itemId = 0;
	uint8 i = 0;
	bool itemIdLegality = false;
	uint8 itemDataLen = 0;
	uint8 offsetAddr = 0;
	uint8 txEventDataLen = 0;	

	/*
	** ����¼��ظ��� -- �Ƿ������¼��ظ�������֡����?-->��δ����
	*/
	memset((uint8*)&upperLayerPara.cabinetCtrInfoReply.dataItemlen,0x00,sizeof(CabinetCtrInfoReply));

	for(;rxlen >= sizeof(uint16);){
		/*
		** �¼�ID
		*/
		itemIdLegality = false;
		memcpy((uint8*)&itemId,(uint8*)&rx[offsetAddr],sizeof(uint16));
		for(i=0;i<cabinetCtrInfoFunTableNum;i++){
			if(itemId == cabinetCtrInfoFunTable[i].itemId){
				itemIdLegality = true;
				/*�������¼�*/
				memcpy((uint8*)&upperLayerPara.cabinetCtrInfoReply.tx[upperLayerPara.cabinetCtrInfoReply.dataItemlen],
							(uint8*)&itemId,sizeof(uint16));
				offsetAddr += sizeof(uint16);
				rxlen -= sizeof(uint16);
				upperLayerPara.cabinetCtrInfoReply.dataItemlen += sizeof(uint16);
				itemDataLen = rx[offsetAddr];
				offsetAddr += sizeof(uint8);
				rxlen -= sizeof(uint8);
				cabinetCtrInfoFunTable[i].handle(itemDataLen,(uint8*)&rx[offsetAddr]
					,(uint8*)&txEventDataLen,
					(uint8*)&upperLayerPara.cabinetCtrInfoReply.tx[upperLayerPara.cabinetCtrInfoReply.dataItemlen]);
				offsetAddr += itemDataLen;
				rxlen -= itemDataLen;
				upperLayerPara.cabinetCtrInfoReply.dataItemlen += txEventDataLen;
				break;
			}
		}
		/*
		**�¼�ID���Ϸ�
		*/
		if(itemIdLegality == false){
			break;
		}
		
	}

	setCmd(cmd);
}

/*
** BMS��Ϣ
*/
void UL_BmsInfo(uint16 rxlen,uint8*rx){
	SmCmd cmd = {true,CMD_ID_BmsInfo_Tx,0,0};

	/*
	** ���ŵ�ַ
	*/
	cmd.doorAddr = rx[0];

	setCmd(cmd);
}

/*
** BMS��Ҫ��Ϣ
*/
void UL_BmsMainInfo(uint16 rxlen,uint8*rx){
	SmCmd cmd = {true,CMD_ID_BatMainInfo_Tx,0,0};

	setCmd(cmd);
}

/*
** ����
*/
void UL_Domain(uint16 rxlen,uint8*rx){
	ulReplyPara.domain.flag = true;
}

/*
** ��ѯ�������Ϣ
*/
void UL_ChkChargeInfo(uint16 rxlen,uint8*rx){
	SmCmd cmd = {true,CMD_ID_ChargeInfo_Tx,0,0};

	if(rx[0] < SysCtr_AllDoorNum){/*�жϲ��ŵ�ַ�Ϸ���*/
		/*
		** ���µ�ַ��Ϣ
		*/
		cmd.doorAddr = rx[0];
		setCmd(cmd);
	}
}

/*
** ִ�к������
*/
static const UpperLayerFunTable upperLayerFunTabel[] = {
	{CMD_ID_Register_Rx,UL_RegisterInfo},
	{CMD_ID_Logout_Rx,UL_LogoutInfo},
	{CMD_ID_StateInfoChange_Rx,UL_StateInfoChange},
	{CMD_ID_StateInfo_Rx,UL_StateInfo},
	{CMD_ID_Ctr_Rx,UL_CtrInfo},
	{CMD_ID_BmsInfo_Rx,UL_BmsInfo},
	{CMD_ID_Domain_Rx,UL_Domain},
	{CMD_ID_BatMainInfo_Rx,UL_BmsMainInfo},
	{CMD_ID_ChargeInfo_Rx,UL_ChkChargeInfo}
};
static uint8 upperLayerFunTabelNum = sizeof(upperLayerFunTabel)/sizeof(UpperLayerFunTable);

/*
**�ϲ�Э��ִ֡�к����������
*/
void UpperLayerExecuteFun(MsgHead msgHead,uint8*msgItem){
	uint8 i=0;
	for(i=0;i<upperLayerFunTabelNum;i++){
		if(msgHead.id == upperLayerFunTabel[i].id){
			upperLayerFunTabel[i].handle(msgHead.dataLen,(uint8*)msgItem);
			break;
		}
	}
}

/*
**��������֡��֡,ճ֡,�쳣֡����
*/
void ParseUpperLayerFrame(uint8* rxbuf,uint16* rxlen){
	/*
	**��ʱ����
	*/
	UpperLayerFrame ulFrameTmp = {0};
	/*
	**�쳣֡,ճ֡�������
	*/
	uint8 *ptrbuf = rxbuf;	
	uint16 updataLen = *rxlen;
	uint16 frameLen = 0;
	uint16 msgLen=0;
	uint16 saveDataLen = *rxlen;

	for(;updataLen>=(uint16)(sizeof(ulFrameTmp.head)+sizeof(ulFrameTmp.msgFrameLen));){
		memcpy((uint8*)&frameLen,(uint8*)&ptrbuf[1] ,sizeof(uint16));
		if(ptrbuf[0] == UpperLayerFrame_Head&&ChkFrameDataLenIsOk(frameLen)){
			if(updataLen>=frameLen){
				if(CalcBcc((uint8 *)&ptrbuf[0], (uint16)frameLen-2) == ptrbuf[frameLen-2] 
					&& ptrbuf[frameLen-1] == UpperLayerFrame_End){
					memcpy((uint8*)&ulFrameTmp.head,(uint8*)&ptrbuf[0],sizeof(ulFrameTmp.head)+sizeof(ulFrameTmp.msgFrameLen)+sizeof(MsgHead));
					memcpy((uint8*)&msgLen,(uint8*)&ulFrameTmp.msgHead.dataLen,sizeof(uint16));
					ulFrameTmp.chkCode = ptrbuf[frameLen-2];
					if(ChkDeviceAddrIsOk((uint8)ulFrameTmp.msgHead.addr)){
						UpperLayerExecuteFun(ulFrameTmp.msgHead, 
							(uint8 *)(ptrbuf+sizeof(ulFrameTmp.head)+sizeof(ulFrameTmp.msgFrameLen)+sizeof(ulFrameTmp.msgHead)));
						memset((uint8*)&ulFrameTmp.head,0x00,sizeof(ulFrameTmp));
					}
					ptrbuf += frameLen;
					updataLen -= frameLen;
					frameLen = 0;					
				}
				else{
					ptrbuf++;
					updataLen--;
					frameLen = 0;
				}
			}
			else{
				break;
			}
		}
		else{
			ptrbuf++;
			updataLen--;
			frameLen = 0;
		}
	}
	memcpy((uint8*)&rxbuf[0],(uint8*)&ptrbuf[0],updataLen);
	*rxlen = updataLen;
	memset((uint8*)rxbuf+updataLen,0x00,saveDataLen-updataLen);
}

void SM_UpperLayerParse_Task(void* p_arg){
	/*�޸�:*/
	uint8 ucRxBuf[sizeof(uint16)*(1024+sizeof(UpgradeProFrame))/*1024*/];
	uint8 *rxptr;
	uint16 ucLen = 0;
	uint16 ucRecvLen = 0;

	/*
	** ��ʼ���ϲ�Э�����
	*/
	init_UpperLayerPara();
	
	Sleep(4000);

	for(;;){
		ucRecvLen = 0;
		BSP_SmRecv(BSP_ComType_USART,BSP_ComUpperLayer,ucRxBuf + ucLen,&ucRecvLen);
		ucLen += ucRecvLen;
		if(ucLen >( sizeof(ucRxBuf)))
		{
			ucLen = 0;
		}
		
		if(ucRecvLen == 0 && ucLen != 0)
		{
			rxptr = ucRxBuf;
			ucRecvLen = ucLen;
			ParseUpperLayerFrame((uint8 *)rxptr, (uint16 *)&ucRecvLen);
			ucLen = ucRecvLen;
		}	

		/*
		** CAN1 Tx Task
		*/
		CAN1_TxDataTask();

		/*
		** update �ϲ���ر�־
		*/
		update_UpperFieldRef();

		/*
		** ���ư�ִ��BootLoader����:
		**		1.����Զ������ָ��
		**		2.�ȴ�ִ�лظ�,800ms֮���������ؽ���BootLoader
		*/
		enterCtrBootLoader();

		/*
		** 20210518--����Ƿ��������ԭ��δ��ȡ
		*/
		runtimeChk_UpgrResonFlag();

		/*
		** ���¿��Ź��Ĵ���--ι��
		*/
		watchdogUpdate();

		Sleep(10);
	}
}

/*
** �ṩϵͳ��������ϲ�Э������ӿں���
*/
void UpperLayerParse_Init(void){
	Thread_create(SM_UpperLayerParse_Task, 0,
		(int32 *)&upperLayerParse_TaskStk[1024-1], (int8)UpperLayerParseTask_Prio);
}

