#include "includes.h"

static int32 upperLayerReply_TaskStk[512] = {0};

/*
** �ϲ�Э��֮ʹ��extern���������ļ�����
*/
extern UpperLayerPara upperLayerPara;
/*
** �ϲ�Э��֮ʹ��ָ����������ļ�����
*/
LowerLayerParam* ul_LLParse = null;
SysParam* ulr_SysPara = null;

/*
** �ϲ�Э��֮�ظ���������
*/
UpperLayerReplyPara ulReplyPara;

/*
** �ϲ�Э��֮��������
*/
SmCmd smCmd;

/*
** �ϲ�Э��֮�ظ����������ӿں���֮�ⲿ��������
*/
UpperLayerReplyPara* getULReplyPara(void){
	return(&ulReplyPara);
}

/*
** �ϲ�Э��֮�ظ�������ʼ��
*/
static void init_ULReplyPara(void){
	memset((uint8*)&ulReplyPara.domain.flag,0x00,sizeof(UpperLayerReplyPara));
	ul_LLParse = getLowerLayerParaPtr();
	ulr_SysPara = get_SysParaPtr();
}

/*
** �ṩ�����ļ����ýӿں���
** 1.�����������
** 2.��ѯ�������
*/
void setCmd(SmCmd cmd){
	uint8 cnt=0;
	
	while(smCmd.id != 0 && cnt < 100){
		cnt++;
		Sleep(10);
	}
	memcpy((uint8*)&smCmd.refresh,(uint8*)&cmd.refresh,sizeof(SmCmd));
}

uint8 getCmd(void){
	return(smCmd.id);
} 

/*
** �ϲ�Э��֮�ظ�ע����Ϣ
*/
void ULReply_Register(UpperLayerFrame *ulFreame,uint8* item,uint8 addr){
	uint16 len = 0;
	uint32 timeStamp = 0;/*��λ:ms*/
	
	/*
	** �豸ID,Ӳ���汾��,����汾��,�����ͺ�,�����ͺ�,ע��ʱ��
	*/
	/*
	** �����豸ID����
	*/
	memcpy((uint8*)&upperLayerPara.sysSet.id[0],(uint8*)&ulr_SysPara->deviceId.show[0],16);
	memcpy((uint8*)&item[len],(uint8*)&upperLayerPara.sysSet.id[0],16);
	len += 16;
	/*���ư�Ӳ���汾��*/
	memcpy((uint8*)&item[len],(uint8*)&upperLayerPara.sysSet.ctrHareVer[0],4);
	len += 4;
	/*�ֿذ�Ӳ���汾��*/
	memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[0].batDoorSysPara.comHareVer[0],4);
	len += 4;
	/*���ư�����汾��*/
	memcpy((uint8*)&item[len],(uint8*)&upperLayerPara.sysSet.ctrSoftVer[0],4);
	len += 4;
	/*�ֿذ�����汾��*/
	memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[0].batDoorSysPara.comSoftVer[0],4);
	len += 4;

	memcpy((uint8*)&item[len],(uint8*)&upperLayerPara.sysSet.ctrModel[0],6);
	len += 6;
	memcpy((uint8*)&item[len],(uint8*)&upperLayerPara.sysSet.fireId[0],10);
	len += 10;

	/*
	** ʱ���
	*/
	timeStamp = GetCurSecond()*1000;/*��λ:ms*/
	memcpy((uint8*)&item[len],(uint8*)&timeStamp,sizeof(uint32));
	len += sizeof(uint32);

	/*
	** �������
	*/
	ulFreame->msgHead.dataLen = len;
}

/*
** �ϲ�Э��֮�ظ�ע����Ϣ
*/
void ULReply_Logout(UpperLayerFrame *ulFreame,uint8* item,uint8 addr){
	uint16 len = 0;
	/*
	** �豸ID
	*/
	memcpy((uint8*)&item[len],(uint8*)&upperLayerPara.sysSet.id[0],16);
	len += 16;

	/*
	** �������
	*/
	ulFreame->msgHead.dataLen = len;
}

/*
** �ϲ�Э��֮״̬��Ϣ���
*/
void ULReply_StateInfoChange(UpperLayerFrame *ulFreame,uint8* item,uint8 addr){
	uint16 len = 0;
	
	/*
	** ������
	*/
	memcpy((uint8*)&item[len],(uint8*)&upperLayerPara.stateInfoChange.sysLogic.batOnline,sizeof(StateInfoChange));
	len += sizeof(StateInfoChange);

	/*
	** �������
	*/
	ulFreame->msgHead.dataLen = len;
}

/*
** �ϲ�Э��֮�ظ�״̬��Ϣ
*/
void ULReply_StateInfo(UpperLayerFrame *ulFreame,uint8* item,uint8 addr){
	uint16 len = 0;
	DoorEUResonNoRead resonTemp = {0};

	/*
	** ������ 
	*/
	memcpy((uint8*)&item[len],(uint8*)&upperLayerPara.stateInfo.batOnlineCtrAC,sizeof(StateInfo));
	len += sizeof(StateInfo);
	/*
	** ���ֻ���
	*/
	memcpy((uint8*)&item[len],(uint8*)&upperLayerPara.sysFire.chgDoorTemp,sizeof(uint16));
	len += sizeof(uint16);
	/*
	** ��ذ��̼���
	*/
	memcpy((uint8*)&item[len],(uint8*)&ulr_SysPara->batFireSize,sizeof(uint16));
	len += sizeof(uint16);
	/*
	** ���ԭ���־λ
	*/
	resonTemp = get_DoorEUResonNoRead();
	memcpy((uint8*)&item[len],(uint8*)&resonTemp.bat,sizeof(DoorEUResonNoRead));
	len += sizeof(DoorEUResonNoRead);
	/*
	** �������
	*/
	ulFreame->msgHead.dataLen = len;
}

/*
** �ϲ�Э��֮�ظ�������Ϣ
*/
void ULReply_CtrInfo(UpperLayerFrame * ulFreame,uint8* item,uint8 addr){
	/*
	** �������
	*/
	ulFreame->msgHead.dataLen = upperLayerPara.cabinetCtrInfoReply.dataItemlen;
	/*
	** ������
	*/
	memcpy((uint8*)&item[0],(uint8*)&upperLayerPara.cabinetCtrInfoReply.tx[0],ulFreame->msgHead.dataLen);
}

/*
** BMS��Ϣ
*/
void ULReply_BmsInfo(UpperLayerFrame *ulFreame,uint8* item,uint8 addr){
	uint16 len = 0;

	/*
	** ���ŵ�ַ
	*/
	item[len] = addr;
	len += sizeof(uint8);
	if(addr >= SysCtr_AllDoorNum){/*���ŵ�ַ���Ϸ�*/
		item[len] = 0x02;
		len += sizeof(uint8);
	}else{/*���ŵ�ַ�Ϸ�*/
		item[len] = 0x01;
		len += sizeof(uint8);
		/*
		** ����Ƿ�����
		*/
		if(ul_LLParse->batDoor[addr].batDoorStateInfo.bits.batOnline == false){/*�������*/
			item[len] = 0x02;
			len += sizeof(uint8);
		}else{/*�������*/
			item[len] = 0x01;
			len += sizeof(uint8);
			/*
			** ��س��ʱ��
 			*/
 			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].batDoorSysPara.chgTime,sizeof(uint16));
			len += sizeof(uint16);
			/*
			** �����Ϣ
			*/
			/*������:ʵʱ��ѹ,ʵʱ����,SOC,���̴���*/
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.realVol,sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.realCur,sizeof(uint16));
			len += sizeof(uint16);
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.soc;
			len += sizeof(uint8);
			 item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.firmCode;
			len += sizeof(uint8);	
			/*������:����״̬ ����״̬ SOH SOP*/
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.pState.flag,sizeof(uint16));
			len += sizeof(uint16);
			 item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.faultState.flag;
			len += sizeof(uint8);		
			 item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.soh;
			len += sizeof(uint8);	
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.sop,sizeof(uint16));
			len += sizeof(uint16);	
			/*������:���ڵ�ص�ѹ 16*2*/
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.singleBatVol[0],16*sizeof(uint16));
			len += 16*sizeof(uint16);
			/*������:���ID 16*/
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.batID[0],16);
			len += 16;
			/*������:������,���Ŵ���,��·����*/
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.chgNum,sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.disChgNum,sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.shortNum,sizeof(uint16));
			len += sizeof(uint16);
			/*ѭ������ ʣ������ �������*/
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.circNum,sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.surplusCap,sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.designCap,sizeof(uint16));
			len += sizeof(uint16);	
			/*������:�������¶�,�������¶�,MOS����¶�,PCB�¶�,��ʷ�������¶� ����״̬*/
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.batMaxTemp;
			len += sizeof(uint8);		
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.batMinTemp;
			len += sizeof(uint8);	
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.mosMaxTemp;
			len += sizeof(uint8);		
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.pcbTemp;
			len += sizeof(uint8);		
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.hisMaxBatTemp;
			len += sizeof(uint8);	
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.workState;
			len += sizeof(uint8);
			/*������:Ӳ���汾,����汾,BOOT�汾��,����ͺ�*/	
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.hardVer,sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.softVer,sizeof(uint16));
			len += sizeof(uint16);
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.bootVer;
			len += sizeof(uint8);	
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.batType;
			len += sizeof(uint8);	
			/*������:��ع���ģʽ,��س��ģʽ,MOS״̬*/
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.batWorkMode;
			len += sizeof(uint8);		
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.batChgMode;
			len += sizeof(uint8);	
			item[len] = ul_LLParse->batDoor[addr].bmsInfoMeiTuan.mosState.flag;
			len += sizeof(uint8);	
			/*-------------------����:����������-����_���ͨ��Э��1.15.pdf----------------------*/
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[addr].bmsInfoMeiTuan.batMTV115.materialChangeR,sizeof(BatMeiTuanV115));
			len += sizeof(BatMeiTuanV115);
			/*----------------------------------------------------------------------------------*/
		}
	}
	/*
	** �������
	*/
	ulFreame->msgHead.dataLen = len;
}

/*
** ����
*/
void ULReply_Domain(UpperLayerFrame *ulFreame,uint8* item,uint8 addr){
	uint16 len = 0;
	uint8 legalLen = 0;
	SysDomain sysDomain = get_SysDomain();

	/*
	** ������: ZLD�������� ZLD���� ������������ �������� -- �����豸ID
	*/
	legalLen = getDomainIsLegalLen((uint8 *)&sysDomain.local[0]);
	item[len] = legalLen;
	len += sizeof(uint8);
	memcpy((uint8*)&item[len],(uint8*)&sysDomain.local[0],legalLen);
	len += legalLen;

	legalLen = getDomainIsLegalLen((uint8 *)&sysDomain.remote[0]);
	item[len] = legalLen;
	len += sizeof(uint8);
	memcpy((uint8*)&item[len],(uint8*)&sysDomain.remote[0],legalLen);
	len += legalLen;	

	/*
	** �����豸ID
	*/
	memcpy((uint8*)&item[len],(uint8*)&ulr_SysPara->deviceId.show[0],16);
	len += 16;

	/*
	** �������
	*/
	ulFreame->msgHead.dataLen = len;
}

/*
** Bms��Ҫ��Ϣ
*/
void ULReply_BmsMainInfo(UpperLayerFrame *ulFreame,uint8* item,uint8 addr){
	uint8 i = 0;
	uint16 len = 0;
	
	for(i=0;i<SysCtr_AllDoorNum;i++){
		if(ul_LLParse->batDoor[i].batDoorStateInfo.bits.batOnline == true){/*�������*/
			item[len] = ul_LLParse->batDoor[i].bmsInfoMeiTuan.soc;/*SOCֵ*/
			len += sizeof(uint8);
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[i].batDoorSysPara.insertTime,sizeof(uint16));/*����ʱ��*/
			len += sizeof(uint16);
			/*���ʱ��--20210127*/
			memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->batDoor[i].batDoorSysPara.chgTime,sizeof(uint16));
			len += sizeof(uint16);
		}else{
			memset((uint8*)&item[len],0x00,sizeof(uint8) + sizeof(uint16) + sizeof(uint16));
			len += (sizeof(uint8) + sizeof(uint16) + sizeof(uint16));
		}
	}

	/*
	** �������
	*/
	ulFreame->msgHead.dataLen = len;
}

/*
** ��ѯ�������Ϣ
*/
void ULReply_ChkChargeInfo(UpperLayerFrame *ulFreame,uint8* item,uint8 addr){
	uint16 len = 0;

	/*
	** ���ŵ�ַ
	*/
	item[len] = addr; 
	len += sizeof(uint8);
	/*
	** ���������� �����������Ϣ + �����ʵʱ��Ϣ
	*/
	memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->chargeInfo[addr].base.boot,sizeof(Base));
	len += sizeof(Base);
	memcpy((uint8*)&item[len],(uint8*)&ul_LLParse->chargeInfo[addr].realtimeData.model,sizeof(RealtimeData));
	len += sizeof(RealtimeData);

	/*
	** �������
	*/
	ulFreame->msgHead.dataLen = len;
}

/*
**�ϲ�Э��֮�ظ��ӿں������
*/
static const UpperLayerReplyFunTable upperLayerReplyFunTable[] = {
	{CMD_ID_Register_Tx,ULReply_Register},
	{CMD_ID_Logout_Tx,ULReply_Logout},
	{CMD_ID_StateInfoChange_Tx,ULReply_StateInfoChange},
	{CMD_ID_StateInfo_Tx,ULReply_StateInfo},
	{CMD_ID_Ctr_Tx,ULReply_CtrInfo},
	{CMD_ID_BmsInfo_Tx,ULReply_BmsInfo},
	{CMD_ID_Domain_Tx,ULReply_Domain},
	{CMD_ID_BatMainInfo_Tx,ULReply_BmsMainInfo},
	{CMD_ID_ChargeInfo_Tx,ULReply_ChkChargeInfo}
};
static uint8 upperLayerReplyFunTableNum = sizeof(upperLayerReplyFunTable)/sizeof(UpperLayerReplyFunTable);

/*
** ע����Ϣ�����߼�
*/
void ULReply_RegisterCtrLogic(void){
	SmCmd cmd = {true,CMD_ID_Register_Tx,0,0};
	static uint32 itick = 0;

	if(ulReplyPara.registerLogic.flag == true){
		if(TickOut((uint32 *)&itick, 5000) == true){
			TickOut((uint32 *)&itick, 0);
			setCmd(cmd);
		}
	}
}

/*
** ע����Ϣ�����߼�
*/
void ULReply_LogoutCtrLogic(void){
	SmCmd cmd = {true,CMD_ID_Logout_Tx,0,0};
	static uint32 itick = 0;

	if(ulReplyPara.logoutLogic.flag == true){
		if(TickOut((uint32 *)&itick, 5000) == true){
			TickOut((uint32 *)&itick, 0);
			setCmd(cmd);
		}
	}
}

/*
** ��������״̬��Ϣ
*/
static uint32 stateTick= 0; 
static uint32 dealHardErrTick = 0;/*����Ӳ������,�����ϱ����ݲ��ϴ���*/
void ULReply_UpdateStateInfoLogic(void){
	/*
	** static uint32 itick = OSTimeGet();
	** static uint32 dealHardErr = OSTimeGet();
	** ����: function call is not allowed in a constant expression 
	** static uint32 itick = OSTime;
	** ����: expression must have a constant value
	*/
	SmCmd cmd = {true,CMD_ID_StateInfoChange_Tx,0,0};
	static bool resetFlag = true;
	static StateInfoChange stateInfoChange = {0};

	if(resetFlag == true){
		resetFlag = false;
		stateTick = OSTimeGet();
		dealHardErrTick = OSTimeGet();
	}

	/*�ϵ�ȴ�20S֮��,�ȴ�״̬�ȶ�֮���ڱ��״̬��Ϣ*/
	if(TickOut((uint32 *)&stateTick, 20000) == true){
		if(memcmp((uint8*)&stateInfoChange.sysLogic.batOnline,
			(uint8*)&upperLayerPara.stateInfoChange.sysLogic.batOnline,sizeof(StateInfoChange)) != 0
			&& TickOut((uint32 *)&dealHardErrTick, 1000) == true){
			TickOut((uint32 *)&dealHardErrTick, 0);
			memcpy((uint8*)&stateInfoChange.sysLogic.batOnline,
				(uint8*)&upperLayerPara.stateInfoChange.sysLogic.batOnline,sizeof(StateInfoChange));
			setCmd(cmd);
		}
	}
}

/*
** ���豸�����ϱ�������Ϣ
*/
void powerOnUpDomainInfo(void){
	SmCmd cmd = {true,CMD_ID_Domain_Tx,0,0};
	static uint32 itick = 0;
	static bool powerOnFlag = false;

	if(ulReplyPara.domain.flag == false){/*�����ϱ�����,ֱ����������ȡ����*/
		if(TickOut((uint32 *)&itick, 6000) == true
			|| powerOnFlag == false){
			TickOut((uint32 *)&itick, 0);
			setCmd(cmd);
			powerOnFlag = true;
		}
	}
}

/*
** �ϲ�Э��֮��װ�ظ�֡�ӿں���
*/
void UL_PackectReplyFun(UpperLayerFrame *ulFreame,uint8* item,uint8*tx){
	uint8 len = 0;

	/*֡ͷ*/
	tx[len] = UpperLayerFrame_Head;
	len += sizeof(uint8);

	/*֡���ݳ��ȼ����߼�*/
	ulFreame->msgFrameLen = sizeof(UpperLayerFrame) + ulFreame->msgHead.dataLen;

	/*֡����*/
	memcpy((uint8*)&tx[len],(uint8*)&ulFreame->msgFrameLen,sizeof(uint16));
	len += sizeof(uint16);

	/*��Ϣͷ*/
	memcpy((uint8*)&tx[len],(uint8*)&ulFreame->msgHead.id,sizeof(MsgHead));
	len += sizeof(MsgHead);

	/*��Ϣ������*/
	memcpy((uint8*)&tx[len],(uint8*)&item[0],ulFreame->msgHead.dataLen);
	len += ulFreame->msgHead.dataLen;

	/*У��*/
	tx[len] = CalcBcc((uint8 *)&tx[0], len);
	len += sizeof(uint8);

	/*֡β*/
	tx[len] = UpperLayerFrame_End;
}

void SM_UpperLayerReply_Task(void* p_arg){
	uint8 txItem[256] = {0};
	uint8 txbuf[256] = {0};
	UpperLayerFrame ulFrameTmp = {0};
	uint8 i = 0;

	/*
	** �ϲ�Э��֮�ظ�������ʼ��
	*/
	init_ULReplyPara();

	Sleep(4000);
	
	for(;;){		
		if(smCmd.refresh == true){
			for(i=0;i<upperLayerReplyFunTableNum;i++){
				if(smCmd.id == upperLayerReplyFunTable[i].id){
					ulFrameTmp.msgHead.id = upperLayerReplyFunTable[i].id;
					ulFrameTmp.msgHead.addr = smCmd.deviceAddr;/*�豸��ַ����Э�����߲���˵��*/
					upperLayerReplyFunTable[i].handle((UpperLayerFrame*)&ulFrameTmp,
						(uint8*)&txItem[0],smCmd.doorAddr);
					UL_PackectReplyFun((UpperLayerFrame *)&ulFrameTmp, 
						(uint8 *)&txItem[0], (uint8 *)&txbuf[0]);
					BSP_SmSend(BSP_ComType_USART, BSP_ComUpperLayer, 
						(void *)&txbuf[0], (uint16 *)&ulFrameTmp.msgFrameLen);
					memset((uint8*)&smCmd.refresh,0x00,sizeof(SmCmd));
					break;
				}
			}			
		}
		
		/*
		** ���豸�����ϱ�������Ϣ
		*/
		#if CtrHardware == CtrHardware_Dwin
		powerOnUpDomainInfo();
		#endif

		#if CtrHardware == CtrHardware_Andriod
			ulReplyPara.domain.flag = true;/*��׿Ĭ���ѻ�ȡ����,���ð�׿��,�����Ӱ�׿��ȡ*/
		#endif

		if(ulReplyPara.domain.flag == true){/*���豸��ȡ�����ɹ�,���ϴ�������Ϣ*/
			#if CtrHardware == CtrHardware_Dwin
			/*
			** ע����Ϣ�����߼�
			*/
			ULReply_RegisterCtrLogic();
			/*
			** ע����Ϣ�����߼�
			*/
			ULReply_LogoutCtrLogic();	
			#endif
			/*
			** ��������״̬��Ϣ
			*/
			ULReply_UpdateStateInfoLogic();
		}

		/*
		** CAN1 Tx Task
		*/
		CAN1_TxDataTask();

		/*
		** ���¿��Ź��Ĵ���--ι��
		*/
		watchdogUpdate();
			
		Sleep(10);
	}
}


/*
**�ṩϵͳ��������ϲ�Э��ظ��ӿں���
*/
void UpperLayerReply_Init(void){
	Thread_create(SM_UpperLayerReply_Task, 0, 
		(int32 *)&upperLayerReply_TaskStk[512-1], UpperLayerReplyTask_Prio);
}

