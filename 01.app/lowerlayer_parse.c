#include "includes.h"

static int32 lowerLayerParse_TaskStk[512] = {0};

/*
** ʹ��ָ�뷽ʽ���������ļ�����
*/
UpperLayerPara* llp_ULP = null;

/*
** �²�Э�������������
*/
LowerLayerParam llParam;

/*
** �²�Э��֮�����ӿ��ṩ����
*/
LowerLayerParam* getLowerLayerParaPtr(void){
	return(&llParam);
}

/*
** �²�Э��֮������ʼ��
*/
static void LowerLayerParam_Init(void){
	memset((uint8*)&llParam.compatibility[0].fireCode,0x00,sizeof(LowerLayerParam));
	llp_ULP = getUpperLayerPara();
}

/*
** �²�Э��֮��ز�״̬��Ϣ�ֶ�֮ʵʱ��ѹ���ݸ���
*/
#if Client_Choice == Client_Choice_All ||  Client_Choice == Client_Choice_MeiTuan
void FillBatDoorStateInfo_MeiTuan(uint8 fireCode,uint8* item,uint8 addr){
	uint8 len = 0;
	
	switch(fireCode){	
		case 0x01:/*������*/
		case 0x02:/*������*/
		case 0x03:/*��ë��*/
		case 0x04:/*�϶�*/
		case 0x05:/*���ն���*/
		case 0x06:/*�Ǻ�*/
		case 0x07:/*ATL*/
		case 0x08:/*CATL*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.realVol,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);

			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.realCur,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);

			llParam.batDoor[addr].bmsInfoMeiTuan.soc = item[len];
			len += sizeof(uint8);
			break;
		default:
			
			break;
	}
}
#endif

#if Client_Choice == Client_Choice_All ||  Client_Choice == Client_Choice_PineCone
void FillBatDoorStateInfo_PineCone(uint8 fireCode,uint8* item,uint8 addr){
	uint8 len = 0;

	switch(fireCode){
		case 0x81:/*�Ǻ�*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoPineCone.batTotalVol,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);

			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoPineCone.realCur,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);

			llParam.batDoor[addr].bmsInfoPineCone.soh = item[len];
			len += sizeof(uint8);
			break;
		default:

			break;
	}
}
#endif

/*
** �ж��쳣������(0b0000 0011 1100 1110)
**		bit6:��1--ϵͳ���϶�AC
**		�쳣����:
**			����һ�������ѻָ�,app����--�ֿ�ȫ��������Ҫ�����ϵ��־,���ǰ�׿û��ʾ,�ֿ��ϱ�ϵͳ���϶�AC,�����ؼ���޿ղ�,����,����
------------------------------------------------------------------------------------------------------------------------------------
**		��ز�״̬ -- Fun ID:0x01				�ֿ��ж�����-->�����˸
**		---------------------------------------------------------
**		bit0:�������
**		bit1:��زճ��������					1
**		bit2:��ع���								1
**		bit3:���������							1
**		bit4:������߿���AC							-- �ֿ�ͳ���޹�����Ϣ:���״̬��,�����,������λ
**		bit5:Զ�̶�AC
**		bit6:ϵͳ���϶ϳ����AC					1
**		bit7:BMSͨѶID�ظ�
**		bit8:��ع���								1
**		bit9:���ͨѶ����							1
**		bit10:����Ƿ��������
**		bit11:��������Ƿ�ɹ�
**		bit12:�ֿع���							1
**		bit13:����쳣							1
**		bit14:��ص���							1
**		bit15:һ���澯�ֿذ帴λ֮����λ		�ֿ��ж�����-->�����˸
**																				---->0x734E
**		---------------------------------------------------------
**		�����״̬��Ϣ -- Fun ID:0x04
**		bit0:���������
**		bit1:���������							1
**		bit2:���������							1
**		bit3:��������ù���						1
**		bit4:���������ʧЧ						1
**		bit5:�����������							1
**		bit6:���������							1
**		
**		bit14:������������
**		bit15:����������ɹ�
**																				---->0x007E
*/
#define AbnDoorNumBits						(uint16)0x734E/*�޸�ʱ��:20210309*//*0x03CE*/
#define ChargerAbnDoorNumBit			(uint16)0x007E

/*
** ����״̬��Ϣ���²�����(�ղ���,������,������,�쳣������,�ܲ���)
*/
uint8 doorStateFlag[SysCtr_AllDoorNum] = {0};
static void updateDoorNum(void){
	DoorTypeNum doorTypeTemp = {0};
	uint8 i = 0;

// 	for(i=0;i<SysCtr_AllDoorNum;i++){
// 		/*
// 		** ״̬λ�Ĺ���λ���ȼ���� -- �������ϲ��ж����� 20210309
// 		**	ע��:����һ����������δֱ����λ���й���λ��ԭ��:�ֿؽ���ϵͳ����λ֮��,���ֵƺ͹���һ����,��ʱʧ����ʧ�������ж�
// 		*/
// 		if((llParam.batDoor[i].batDoorStateInfo.flag & AbnDoorNumBits) != 0 
// 			|| llParam.comBoardChk[i].comAbn == true
// 			|| (llParam.chargeInfo[i].state.flag & ChargerAbnDoorNumBit) != 0){
// 			doorTypeTemp.abn++;
// 		}else{/*�޹���*/
// 			if(llParam.batDoor[i].batDoorStateInfo.bits.batOnline == false){/*��ز�����*/
// 				doorTypeTemp.idle++;
// 			}else{/*�������*/
// 				if(llParam.batDoor[i].bmsInfoMeiTuan.soc >= get_SocLimit()){
// 					doorTypeTemp.full++;
// 				}				
// 			}
// 		}
// 	}

// 	doorTypeTemp.ing = SysCtr_AllDoorNum - (doorTypeTemp.abn + doorTypeTemp.idle + doorTypeTemp.full);
// 	doorTypeTemp.total = SysCtr_AllDoorNum;

// 	/*�޸�:20210309--ͳ��:��������,��������,�ղ�����*/
// 	for(i=0;i<SysCtr_AllDoorNum;i++){
// 		if(llParam.comBoardChk[i].comAbn == false){/*�ֿ�����--�ֿز����߼���Ϊ�ִ��ڹ���*/
// 			if(llParam.batDoor[i].batDoorStateInfo.bits.batOnline == true){/*�������*/
// 				if(llParam.batDoor[i].bmsInfoMeiTuan.soc >= get_SocLimit()){/*����*/
// 					doorTypeTemp.full++;
// 				}else{/*�����*/
// 					if(llParam.batDoor[i].batDoorStateInfo.bits.batOnlineOpenAC == true){/*�ж������*/
// 						doorTypeTemp.ing++;	
// 					}
// 				}
// 			}else{/*ֻҪ��ز����߼��ж�Ϊ�ղ�--��ʹ�����ڹ���,ͬ��Ҳ�ж�Ϊ�ղ�*/
// 				doorTypeTemp.idle++;
// 			}
// 		}
// 	}
// 	doorTypeTemp.abn = SysCtr_AllDoorNum - (doorTypeTemp.idle+doorTypeTemp.ing+doorTypeTemp.full);
// 	doorTypeTemp.total = SysCtr_AllDoorNum;

// 	/*
// 	** ��������,���¼�����ͳ����Ϣ����
// 	*/
// 	if(llp_ULP->stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose == true){
// 		memset((uint8*)&doorTypeTemp.idle,0,sizeof(DoorTypeNum));
// 		doorTypeTemp.abn = SysCtr_AllDoorNum;
// 		doorTypeTemp.total = SysCtr_AllDoorNum;
// 	}

	/*�޸�:20210512
	* ����:
	*	1.�������һ���澯-->�����в�λ��Ϊ�ղ�
	*	2.AC�µ�-->�����в�λ��Ϊ�ղ�
	*	3.�ֿز�����-->��Ϊ�ղ�
	*	4.�ֿؼ�������͵���Լ����������״̬-->��Ϊ�ղ�
	*	5.�����/���ָ��ݷֿ��ж�
	*/
	if(cabinet_OneWaringFlagJudge() == true/*�������һ���澯-->�ղ�*/
		||llp_ULP->stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose == true/*AC�Ͽ�*/
		){
		memset((uint8*)&doorTypeTemp.idle,0,sizeof(DoorTypeNum));
		doorTypeTemp.idle = SysCtr_AllDoorNum;
		doorTypeTemp.total = SysCtr_AllDoorNum;
	}else{
		for(i=0;i<SysCtr_AllDoorNum;i++){
			/*�ֿز������ж�Ϊ����*/
			if((llp_ULP->stateInfoChange.sysLogic.comIsOnline&((DoorNumDefine)(((DoorNumDefine)0x01)<<i))) != (DoorNumDefine)0){
				doorTypeTemp.idle++;
			}else{
				switch(doorStateFlag[i]){
					case 0:/*�ղ�*/
						doorTypeTemp.idle++;
						break;
					case 1:/*�����*/
						doorTypeTemp.ing++;
						break;
					case 2:/*����*/
						doorTypeTemp.full++;
						break;
				}
			}
		}
	}

	/*�����ϲ�Э�����������*/
	llp_ULP->stateInfoChange.doorTypeNum = doorTypeTemp;
}

/*
** �ж�ͨѶ���Ƿ�ʧ��,ID�Ƿ��ظ�,����ϲ��ֶ���Ϣ
*/
void updateComIsAbn(void){
	uint8 i = 0;
	bool ret = false;
	static uint32 closeACtTick = 0;/*��AC֮�����¼���ֿ�ʧ��ʱ���־--�ϱ���־*/

	if(llp_ULP->stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose == true){/*AC�Ͽ�*/
		TickOut((uint32 *)&closeACtTick, 0);
	}	

	for(i = 0;i < SysCtr_AllDoorNum;i++){
		if(llParam.comBoardChk[i].comAbn == true){/*ͨѶ��ʧ��*/
			/*Ӳ��������������ͨѶ��ʧ�����*/
			if(llp_ULP->stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose == false){
				if(TickOut((uint32 *)&closeACtTick, 60000) == true){
					llp_ULP->stateInfoChange.sysLogic.comIsOnline |= (DoorNumDefine)((DoorNumDefine)1<<i);
				}
			}else{
				llp_ULP->stateInfoChange.sysLogic.comIsOnline &= (DoorNumDefine)~((DoorNumDefine)1<<i);
			}
		}else{
			llp_ULP->stateInfoChange.sysLogic.comIsOnline &= (DoorNumDefine)~((DoorNumDefine)1<<i);
		}

		if(llParam.batDoor[i].batDoorStateInfo.bits.comIdRep == true){
			ret = true;
		}
	}

	/*
	** ����ϵͳ״̬λ
	*/
	if(llp_ULP->stateInfoChange.sysLogic.comIsOnline != 0){
		llp_ULP->stateInfoChange.sysModuleStateInfo.bits.comErr = true;
	}else{
		llp_ULP->stateInfoChange.sysModuleStateInfo.bits.comErr = false;
	}	

	llp_ULP->stateInfoChange.sysModuleStateInfo.bits.comIdRep = ret;
}

/*
** �����²�->��ز�״̬��Ϣ->����ϲ�->����״̬��Ϣ���
*/
extern void LLReply_ChkBatStateInfo(uint8 cmd,uint8* len,uint8* item);
extern bool oneWaringResetSetFlag;
void stateInfoSwitch(uint8 addr){
	volatile uint8 temp = 0;
	uint8 tx[8] = {0};
	uint8 len = 0;
	
	/*
	** ���״̬��Ϣ:
	**			Bit0:��1--������� Bit1:��1--��زճ�������� Bit2:��1--��ع��� Bit3:��1--���������(�������,����1min�޵�������)
	**			Bit4:��1--������߿���AC Bit5:��1--Զ�̶�AC Bit6:��1--ϵͳ���϶ϳ����AC Bit7:BMSͨѶ��ID�ظ� 
	**			Bit8:��1--��ع��� Bit9:��1--BMS���ջ�,��ع��� Bit10:��1--��������Ƿ���� Bit11:��1--��������Ƿ�ɹ�
	*/
	if(llParam.batDoor[addr].batDoorStateInfo.bits.batOnline == true){
		llp_ULP->stateInfoChange.sysLogic.batOnline |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic.batOnline &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
		/*��ز�����--������е�������Ϣ*/
		memset((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.realVol,0,sizeof(BmsInfoMeiTuan));		
	}
	
	if(llParam.batDoor[addr].batDoorStateInfo.bits.batDoorChargerOT == true){
		llp_ULP->stateInfoChange.sysLogic.chgOverTemp |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic.chgOverTemp &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	if(llParam.batDoor[addr].batDoorStateInfo.bits.batOT == true){
		llp_ULP->stateInfoChange.sysLogic.batIsErr |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic.batIsErr &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	if(llParam.batDoor[addr].batDoorStateInfo.bits.chargerErr == true){
		llp_ULP->stateInfo.batOnline_1minNoCur |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfo.batOnline_1minNoCur &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	if(llParam.batDoor[addr].batDoorStateInfo.bits.batOnlineOpenAC == true){
		llp_ULP->stateInfo.batOnlineCtrAC |=(DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfo.batOnlineCtrAC &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	if(llParam.batDoor[addr].batDoorStateInfo.bits.sysErrCloseAC == true){
		llp_ULP->stateInfo.sysErrCtrAC |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfo.sysErrCtrAC &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	if(llParam.batDoor[addr].batDoorStateInfo.bits.comIdRep == true){
		llp_ULP->stateInfo.comIdRep |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfo.comIdRep &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	/*��ع���*/
	if(llParam.batDoor[addr].batDoorStateInfo.bits.batTrans == true){
		llp_ULP->stateInfoChange.sysLogic.batFault |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic.batFault &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	/*���ͨѶ����*/
	if( llParam.batDoor[addr].batDoorStateInfo.bits.bmsErr == true){
		llp_ULP->stateInfoChange.sysLogic.batComErr |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic.batComErr &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	/*�ֿع���*/
	if(llParam.batDoor[addr].batDoorStateInfo.bits.comIsFault == true){
		llp_ULP->stateInfoChange.sysLogic.comFault |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic.comFault &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}	

	/*����쳣*/
	if(llParam.batDoor[addr].batDoorStateInfo.bits.chargingErr == true){
		llp_ULP->stateInfoChange.sysLogic.chargingErr |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic.chargingErr &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	/*��ص���*/
	if(llParam.batDoor[addr].batDoorStateInfo.bits.batLowTemp == true){
		llp_ULP->stateInfoChange.sysLogic.batLowTemp |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic.batLowTemp &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	
	if(llParam.batDoor[addr].batDoorStateInfo.bits.oneErrRecover == true){
		LLReply_ChkBatStateInfo(0xFE,(uint8 *)&len, (uint8 *)&tx[0]);
		CAN_TransmitAnalysis(CAN_Port_1, len, (uint8 *)&tx[0], addr, LL_FunId_BatDoorState);		
		llp_ULP->stateInfoChange.sysModuleStateInfo.bits.comRecoverFlag = true;
		/*20210302--һ���澯�ָ���־*/
		oneWaringResetSetFlag = false;
	}
}

/*
** �²�Э��֮��ز�״̬����
*/
void LLParse_BatDoorState(uint8 rxlen,uint8* item,uint8 addr){
	uint8 len = 0;
	
	/*
	** ��������--ͨѶС��һ��ID�ظ����ٸ�����Ӧ����--�ж�ͨѶ�쳣
	*/
	if(llParam.batDoor[addr].batDoorStateInfo.bits.comIdRep == false){/*ͨѶС��δ�ظ�*/
		/*
		** ������ ��ز�״̬��Ϣ ��س��̴��� ʵʱ��ѹ(����)/����ܵ�ѹ(���ɹ�) ʵʱ���� SOC(����)/SOH(���ɹ�)
		*/
		/*
		** ���״̬��Ϣ:
		**			Bit0:��1--������� Bit1:��1--��زճ�������� Bit2:��1--��ع��� Bit3:��1--���������(�������,����1min�޵�������)
		**			Bit4:��1--������߿���AC Bit5:��1--Զ�̶�AC Bit6:��1--ϵͳ���϶ϳ����AC Bit7:BMSͨѶ��ID�ظ� 
		**			Bit8:��1--��ط��� Bit9:��1--BMS���ջ�,��ع��� Bit10:��1--��������Ƿ���� Bit11:��1--��������Ƿ�ɹ�
		*/
		
		memcpy((uint8*)&llParam.batDoor[addr].batDoorStateInfo.flag,(uint8*)&item[len],sizeof(uint16));
		len += sizeof(uint16);
		/*
		** ��س��̴��� �ֶκ�(����)�������ݽ��ڵ�����߸���
		*/
		if(llParam.batDoor[addr].batDoorStateInfo.bits.batOnline == true){
			llParam.compatibility[addr].fireCode = item[len];
			len += sizeof(uint8);

			#if Client_Choice == Client_Choice_All ||  Client_Choice == Client_Choice_MeiTuan
				FillBatDoorStateInfo_MeiTuan(llParam.compatibility[addr].fireCode,(uint8 *)&item[len], addr);
			#endif

			#if Client_Choice == Client_Choice_All ||  Client_Choice == Client_Choice_PineCone
				FillBatDoorStateInfo_PineCone(llParam.compatibility[addr].fireCode,(uint8 *)&item[len], addr);
			#endif
		}

		/*
		** ����������ر�־����
		*/
		if(llp_ULP->stateInfoChange.sysLogic.batUpgr & (DoorNumDefine)((DoorNumDefine)0x01<<addr)){
			if(llParam.batDoor[addr].batDoorStateInfo.bits.batUpgrIsFinsh == true
				|| llParam.batDoor[addr].batDoorStateInfo.bits.batOnline == false/*��ز�����*/){
				llp_ULP->stateInfoChange.sysLogic.batUpgr &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);
				if(llParam.batDoor[addr].batDoorStateInfo.bits.batUpgrIsOk == true){
					llp_ULP->stateInfoChange.sysLogic.batUpgrIsOk |= (DoorNumDefine)((DoorNumDefine)0x01<<addr);
				}
			}
		}
		/*
		** �����²�->��ز�״̬��Ϣ->����ϲ�->����״̬��Ϣ���
		*/
		stateInfoSwitch(addr);
	}
}

/*
** 20210422--����:���һ���澯==������/������
*/
bool chk_BatIsExitOneWaring(void){
	uint8 i = 0;
	bool ret = false;
	
	for(i=0;i<SysCtr_AllDoorNum;i++){
		if(llParam.batDoor[i].bmsInfoMeiTuan.pState.bits.batChgOT == true
			|| llParam.batDoor[i].bmsInfoMeiTuan.pState.bits.batChgOC == true){
			ret = true;/*������ڵ��һ���澯*/
			break;
		}		
	}
	return ret;
}


/*
** ����:20210115--�Ź��������� �ֿ��������� -- �ڳ��������,��ع���(����������ִ���--�ܾ�����)
*/
bool chk_IsExitBatChargerOT(void){
	bool flag = false;

	if(llp_ULP->stateInfoChange.sysLogic.chgOverTemp != 0/*���������--�������*/
		|| llp_ULP->stateInfoChange.sysLogic.batIsErr != 0 /*��ع���*/
		/*-----------------------------��������:���ܳ����(12A)�汾V1.03---------------------------------*/
		|| llp_ULP->stateInfoChange.sysLogic.batChgOTime != 0 /*��س��ʱ�����*/
		|| llp_ULP->stateInfoChange.sysLogic_2.chargerOC != 0 /*���������*/
		|| llp_ULP->stateInfoChange.sysLogic_2.chargeOT != 0/*���������*/
		/*-----------------------------------------------------------------------------------------------*/
		|| chk_BatIsExitOneWaring() == true
	){
		flag = true;
	}

	return flag;
}

/*
** �������״̬
*/
extern DoorNumDefine hmiUpgrIsFinshFlag;
/*����ʶ��:����ģʽ��ProOTAģʽʱ������20210426*/
DoorNumDefine batNomal_UpgrageIsTrue = 0;
void LLParse_UpgrState(uint8 rxlen,uint8* item,uint8 addr){
	static uint8 frame[SysCtr_AllDoorNum] = {0};
	uint8 len = 0;
	uint8 frameLabel = 0;
	uint8 txlen = 1;
	uint8 tx[8] = {0};
	static UpgrFail upgrFailTemp[SysCtr_AllDoorNum] = {0};
	UpgrFilePara upgrTemp = {0};
	int16 loc = -1;
	uint16 upgrExceNum = 0;
	static uint8 upgrModel = 0;
	DoorNumDefine upgrTempField = 0xE000000000000000;
	static uint16 upgrNumSyn[SysCtr_AllDoorNum] = {0};
	static uint16 upgrSynTemp[SysCtr_AllDoorNum] = {0};/*ʹ��static ��ʶԭ����-->��˳��һ��*/
	static bool isChkSynFlag[SysCtr_AllDoorNum] = {0};/*ʹ��static ��ʶԭ����-->��˳��һ��*/
		
	/*
	** ֡��ʶ ֡���ݳ�����ʱδʹ��
	*/
	frameLabel = item[len];
	len += sizeof(uint8);
	len += sizeof(uint8);

	switch(frameLabel){/*֡��ʶ*/
		case 0x01:/*Ӳ���汾+�̼�ID(4�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.hardVer,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&upgrFailTemp[addr].bat.fireId[0],(uint8*)&item[len],4);
			len +=  4;
			break;
		case 0x02:/*�̼�ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.fireId[4],(uint8*)&item[len],6);
			len += 6;
			break;
		case 0x03:/*�̼�ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.fireId[10],(uint8*)&item[len],6);
			len += 6;
			break;
		case 0x04:/*ԭ����汾��+���ID(4�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.softVer,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&upgrFailTemp[addr].bat.id[0],(uint8*)&item[len],4);
			len += 4;
			break;
		case 0x05:/*���ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.id[4],(uint8*)&item[len],6);
			len += 6;				
			break;
		case 0x06:/*���ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.id[10],(uint8*)&item[len],6);
			len += 6;					
			break;
		case 0x07:/*����ģʽ*/
			upgrModel = item[len];
			len += 1;
			if(rxlen == 0x05){/*Э��V1.9����:������ʶ*/
				memcpy((uint8*)&upgrSynTemp[addr],(uint8*)&item[len],sizeof(uint16));
				len += sizeof(uint16);
				isChkSynFlag[addr] = true;/*���ͬ����־*/
			}
			break;
	}


	/*
	** ���֡�Ƿ��������
	*/
	frame[addr] |= 0x01<<(frameLabel - 1);
	if(frame[addr] == 0x7F){
		frame[addr] = 0;
		/*��ȡ�̼�λ��*/
		memcpy((uint8*)&upgrTemp.upgrFileType.detailedInfo.hardVer,(uint8*)&upgrFailTemp[addr].bat.hardVer,sizeof(DetailedInfo));
		/*�������upgrFailTemp[addr]����--upgrFailTemp[addr]�����ڴ����,ԭ��:�ñ���������һ�����������ʶ*/
		memset((uint8*)&upgrFailTemp[addr].bat.hardVer,0,sizeof(UpgrFail));
		loc = get_BatLocation(upgrTemp);
		/*����Ƿ�����ֿ�����*/
		if(loc == -1 ){/*�̼�ID���Ϸ�*/
			tx[0] = 0x04;/*�ܾ�����--��"��ع���","���������"*/
		}else{
			/*����:��ع���,��������½�ֹ����*/
			if(chk_IsExitBatChargerOT() == true){
				tx[0] = 0x02;/*�ܾ�����*/
			}else{
				switch(upgrModel){
					case 0x01:/*����ģʽ*/
						if(get_FireUpgrNum(0x03, loc) == (int16)-1){/*��ѯ��ع̼���Ϊ���޴������߼�-->����:����ģʽ�������޴���������ͳ����������*/
							tx[0] = 0x01;/*��������*/
							batNomal_UpgrageIsTrue |= (DoorNumDefine)((DoorNumDefine)0x01<<addr);
						}else{
							upgrExceNum = get_FireRealityRunNum(0x03, loc);
							if(get_FireUpgrNum(0x03, loc) <= upgrExceNum){
								tx[0] = 0x02;/*�ܾ�����*/
							}else{
								tx[0] = 0x01;/*��������*/
								if(upgrNumSyn[addr] != upgrSynTemp[addr] /*ͬ����־��-->����Ƿ�Ϊ��ͬ��������*/
									|| isChkSynFlag[addr] == false /*V1.8Э��֮����ͬ���߼�*/
									){
									upgrNumSyn[addr] = upgrSynTemp[addr];
									upgrExceNum++;
									if(upgrExceNum == 0xFFFF){
										upgrExceNum = 1;
									}	
									set_FireRealityRunNum(0x03, upgrExceNum, loc);
								}
							}
						}						
						break;
					case 0x02:/*����ģʽ*/
						tx[0] = 0x01;/*��������*/
						break;
				}			

				/*��������ģʽ����λ����������������*/
				if(tx[0] == 0x01){/*��������*/
					/*���µ������ģʽ*/
					switch(upgrModel){
						case 0x01:/*����ģʽ*/
							llp_ULP->stateInfoChange.sysLogic.batUpgr &= (DoorNumDefine)~upgrTempField;
							break;
						case 0x02:/*����ģʽ*/
							llp_ULP->stateInfoChange.sysLogic.batUpgr |= (DoorNumDefine)upgrTempField;
							break;
					}
					
					llp_ULP->stateInfoChange.sysLogic.batUpgr |= (DoorNumDefine)((DoorNumDefine)0x01<<addr);
					llp_ULP->stateInfoChange.sysLogic.batUpgrIsOk &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);
					hmiUpgrIsFinshFlag &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);
					/*20210301--�����ƹ�����,�ֿؽ�֧�ֳ����/��������е�һ��,���Ż����������*/
					llp_ULP->stateInfoChange.sysLogic_2.chargeUpgr &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);
					llp_ULP->stateInfoChange.sysLogic_2.chargeUpgrIsOk &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);					
				}
			}
		}
		/*��ģʽ����*/
		upgrModel = 0;
		/*��ͬ����ʶ����*/
		upgrSynTemp[addr] = 0;
		isChkSynFlag[addr] = false;
		/*�ظ��ֿذ�*/
		CAN_TransmitAnalysis(CAN_Port_1, txlen, (uint8 *)&tx[0], addr, LL_FunId_BatUpgrState);
	}
}

/*
** ����ϵͳ��Ϣ
*/
void LLPrse_ChgSysInfo(uint8 rxlen,uint8* item,uint8 addr){
	uint8 len = 0;
	LPare03_01 lpare03_01 = {0};
	
	/*
	** ���״̬��Ϣ
	*/
	lpare03_01.flag = item[len];
	if(lpare03_01.bits.batOnline == true){/*�������*/
		llParam.batDoor[addr].batDoorStateInfo.bits.batOnline = true;
	}

	/*��ѯ�ֿ��˳�������̼�������*/
	if(lpare03_01.bits.subExitUpgr == true){
		/*20210128--�޸�:��������ʼ��־*/
		llp_ULP->stateInfoChange.sysLogic.comUpgr &= ~(DoorNumDefine)((DoorNumDefine)1<<addr);
		llp_ULP->stateInfoChange.sysLogic.batFileDownload &= ~(DoorNumDefine)((DoorNumDefine)1<<addr);	
		llp_ULP->stateInfoChange.sysLogic_2.chargeFileDownloading &= ~(DoorNumDefine)((DoorNumDefine)1<<addr);
	}

	/*�ղ�,�����,����*/
	if(lpare03_01.bits.idle == true){
		doorStateFlag[addr] = 0;/*�ղ�*/
	}else if(lpare03_01.bits.ing == true){
		doorStateFlag[addr] = 1;/*�����*/
	}else if(lpare03_01.bits.full == true){/*����*/
		doorStateFlag[addr] = 2;/*����*/
	}else{/*��δ��λ-->�ղ�*/
		doorStateFlag[addr] = 0;/*�ղ�*/
	}
	len += sizeof(uint8);

	/*
	** ��ؽ���ʱSOC ��س��ʱ�� ��ؽ���ʱ��
	*/
	if(llParam.batDoor[addr].batDoorStateInfo.bits.batOnline == true){
		llParam.batDoor[addr].batDoorSysPara.chgBefSoc = item[len];	
		len += sizeof(uint8);
		memcpy((uint8*)&llParam.batDoor[addr].batDoorSysPara.chgTime,(uint8*)&item[len],sizeof(uint16));	
		len += sizeof(uint16);
		memcpy((uint8*)&llParam.batDoor[addr].batDoorSysPara.insertTime,(uint8*)&item[len],sizeof(uint16));
		len += sizeof(uint16);
	}else{
		memset((uint8*)&llParam.batDoor[addr].batDoorSysPara.chgBefSoc,0x00,sizeof(uint8)+sizeof(uint16)+sizeof(uint16));
		len += sizeof(uint8)+sizeof(uint16)+sizeof(uint16);
	}

	/*
	** ���µ�ز���ʱ�������־
	*/
	if(llParam.batDoor[addr].batDoorSysPara.insertTime >= get_ChgTimeLimit()){
		llp_ULP->stateInfoChange.sysLogic.batChgOTime |= (DoorNumDefine)((DoorNumDefine)1<<addr); 
	}else{
		llp_ULP->stateInfoChange.sysLogic.batChgOTime &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}
	
	/*
	** ������¶�
	*/
	memcpy((uint8*)&llParam.batDoor[addr].batDoorSysPara.chargerTemp,(uint8*)&item[len],sizeof(uint16));
	len += sizeof(uint16);

	
}

/*
** �²�Э��֮BMS��Ϣ֮���BMS�ֶ���Ϣ
*/
#if Client_Choice == Client_Choice_All ||  Client_Choice == Client_Choice_MeiTuan
void FillBatInfo_Data_MetTuan(uint8* item,uint8 frameLabel,uint8 addr){
	uint8 len = 0;
	uint8 dataLen  = 0;
	
	/*
	** �Ż���������߼�--���յ���ϱ���Ϣ���Ͽɵ������
	** ��ȷ:�Ƿ���ڲ�ѯʱ��һ�µ��������л��쳣,ͨѶ�Ƿ����
	*/
	llParam.batDoor[addr].batDoorStateInfo.bits.batOnline = true;
	
	switch(frameLabel){
		case 0x01:/*������:ʵʱ��ѹ,ʵʱ����,SOC,���̴���*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.realVol,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.realCur,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			llParam.batDoor[addr].bmsInfoMeiTuan.soc = item[len];
			len += sizeof(uint8);
			llParam.batDoor[addr].bmsInfoMeiTuan.firmCode = item[len];
			len += sizeof(uint8);		
			/*����:���̴���*/
			llParam.compatibility[addr].fireCode = llParam.batDoor[addr].bmsInfoMeiTuan.firmCode;
			break;
		case 0x02:/*������:����״̬ ����״̬ SOH SOP*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.pState.flag,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			llParam.batDoor[addr].bmsInfoMeiTuan.faultState.flag = item[len];
			len += sizeof(uint8);
			llParam.batDoor[addr].bmsInfoMeiTuan.soh = item[len];
			len += sizeof(uint8);	
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.sop,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			break;
		case 0x03:/*������:���ڵ�ص�ѹ 16*2*/
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
			dataLen = frameLabel != (uint8)0x08?0x06:0x02;
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.singleBatVol[(frameLabel-0x03)*(0x06/sizeof(uint16))],
				(uint8*)&item[len],dataLen);
			len += dataLen;
			break;
		case 0x09:/*������:���ID 16*/
		case 0x0A:	
		case 0x0B:
			dataLen = frameLabel != (uint8)0x0B?0x06:0x04;
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.batID[(frameLabel-0x09)*0x06],
				(uint8*)&item[len],dataLen);
			len += dataLen;
			break;
		case 0x0C:/*������:������,���Ŵ���,��·����*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.chgNum,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.disChgNum,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.shortNum,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);			
			break;
		case 0x0D:/*ѭ������ ʣ������ �������*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.circNum,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.surplusCap,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.designCap,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);			
			break;
		case 0x0E:/*������:�������¶�,�������¶�,MOS����¶�,PCB�¶�,��ʷ�������¶� ����״̬*/
			llParam.batDoor[addr].bmsInfoMeiTuan.batMaxTemp = item[len];
			len += sizeof(uint8);		
			llParam.batDoor[addr].bmsInfoMeiTuan.batMinTemp = item[len];
			len += sizeof(uint8);	
			llParam.batDoor[addr].bmsInfoMeiTuan.mosMaxTemp = item[len];
			len += sizeof(uint8);		
			llParam.batDoor[addr].bmsInfoMeiTuan.pcbTemp = item[len];
			len += sizeof(uint8);		
			llParam.batDoor[addr].bmsInfoMeiTuan.hisMaxBatTemp = item[len];
			len += sizeof(uint8);	
			llParam.batDoor[addr].bmsInfoMeiTuan.workState = item[len];
			len += sizeof(uint8);
			break;
		case 0x0F:/*������:Ӳ���汾,����汾,BOOT�汾��,����ͺ�*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.hardVer,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.softVer,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);	
			llParam.batDoor[addr].bmsInfoMeiTuan.bootVer = item[len];
			len += sizeof(uint8);		
			llParam.batDoor[addr].bmsInfoMeiTuan.batType = item[len];
			len += sizeof(uint8);				
			break;
		case 0x10:/*������:��ع���ģʽ,��س��ģʽ,MOS״̬*/
			llParam.batDoor[addr].bmsInfoMeiTuan.batWorkMode = item[len];
			len += sizeof(uint8);
			llParam.batDoor[addr].bmsInfoMeiTuan.batChgMode = item[len];
			len += sizeof(uint8);	
			llParam.batDoor[addr].bmsInfoMeiTuan.mosState.flag = item[len];
			len += sizeof(uint8);				
			break;
		/*-------------------����:����������-����_���ͨ��Э��1.15.pdf----------------------*/
		case 0x11:/*���ϱ����¼,��о��,FCC*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.batMTV115.materialChangeR,(uint8*)&item[len],6);
			len += 6;
			break;
		case 0x12:/*��ʵSOC,��س��״̬,�������*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.batMTV115.realitySoc,(uint8*)&item[len],6);
			len += 6;
			break;
		case 0x13:/*�����״̬,��λ����*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.batMTV115.banlanceState[0],(uint8*)&item[len],6);
			len += 6;			
			break;
		case 0x14:/*���ֹͣ����ԭ��*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.batMTV115.chgStopReason[0],(uint8*)&item[len],4);
			len += 6;			
			break;
		/*----------------------------------------------------------------------------------*/
		default:
			break;
	}
}


void FillBatInfo_MetTuan(uint8 fireCode,uint8* item,uint8 frameLabel,uint8 addr){
	switch(fireCode){
		case 0x01:/*������*/
		case 0x02:/*������*/
		case 0x03:/*��ë��*/
		case 0x04:/*�϶�*/
		case 0x05:/*���ն���*/
		case 0x06:/*�Ǻ�*/
		case 0x07:/*ATL*/
		case 0x08:/*CATL*/
			FillBatInfo_Data_MetTuan((uint8 *)&item[0], frameLabel, addr);
			break;
		default:
			/*
			** �Ż�����,���̱��Ϊ��,�������BMS�����Ϣ
			*/
			memset((uint8*)&llParam.batDoor[addr].bmsInfoMeiTuan.realVol,0x00,sizeof(BmsInfoMeiTuan));
			break;
	}
}
#endif

#if Client_Choice == Client_Choice_All ||  Client_Choice == Client_Choice_PineCone
void FillBatInfo_Data_PineCone(uint8* item,uint8 frameLabel,uint8 addr){
	uint8 len = 0;
	uint8 dataLen = 0;

	switch(frameLabel){
		case 0x01:/*������:����ܵ�ѹ,ʵʱ����,��������ٷֱ�,SOH*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoPineCone.batTotalVol,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoPineCone.realCur,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);	
			llParam.batDoor[addr].bmsInfoPineCone.relaCapPer = item[len];
			len += sizeof(uint8);
			llParam.batDoor[addr].bmsInfoPineCone.soh = item[len];
			len += sizeof(uint8);			
			break;
		case 0x02:/*������:ʣ������,��������,ѭ������*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoPineCone.surplusCap,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoPineCone.fullCap,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoPineCone.circNum,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);				
			break;
		case 0x03:/*������:���ڵ�ص�ѹ 13*2*/
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
			dataLen = frameLabel == 0x07?0x06:0x02;
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoPineCone.singleBatVol[(frameLabel-0x03)*(0x06/sizeof(uint16))],
				(uint8*)&item[len],dataLen);
			len += dataLen;
			break;
		case 0x08:/*������:ID���� 20*/
		case 0x09:
		case 0x0A:
		case 0x0B:	
			dataLen = frameLabel == 0x0B?0x06:0x02;
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoPineCone.batID[(frameLabel-0x03)*0x06],
				(uint8*)&item[len],dataLen);
			len += dataLen;			
			break;
		case 0x0C:/*������:����¶�,�汾��*/
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoPineCone.batTemp,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&llParam.batDoor[addr].bmsInfoPineCone.ver[0],(uint8*)&item[len],3);
			len += 3;
			break;
		default:
			
			break;
	}
}

void FillBatInfo_PineCone(uint8 fireCode,uint8* item,uint8 frameLabel,uint8 addr){
	switch(fireCode){
		case 0x81:/*�Ǻ�*/
			FillBatInfo_Data_PineCone((uint8 *)&item[0], frameLabel, addr);
			break;
		default:
			
			break;
	}
}
#endif

/*
** �²�Э��֮BMS��Ϣ
*/
void LLParse_BatInfo(uint8 rxlen,uint8* item,uint8 addr){
	uint8 frameLabel = 0;
	uint8 len = 0;
	
	/*
	** ������ ֡��ʶ ���������� ����������
	*/
	frameLabel = item[len];
	len += sizeof(uint8);
	len += sizeof(uint8);/*ע��:������������ʱδʹ��*/

	#if Client_Choice == Client_Choice_All ||  Client_Choice == Client_Choice_MeiTuan
		FillBatInfo_MetTuan(llParam.compatibility[addr].fireCode, (uint8 *)&item[len], frameLabel, addr);
	#endif

	#if Client_Choice == Client_Choice_All ||  Client_Choice == Client_Choice_PineCone
		FillBatInfo_PineCone(llParam.compatibility[addr].fireCode, (uint8 *)&item[len], frameLabel, addr);
	#endif
}

/*
** ����ͨѶ������汾��
*/
void update_ComSfotVer(uint16 ver,uint8 addr){
	uint8 len = 0;
	uint8 charVer[4] = {0};/*����汾��*/
	#if CtrHardware == CtrHardware_Andriod
		uint8 charVerHard[4] = {"0200"};/*Ӳ���汾��*/
	#else
		uint8 charVerHard[4] = {"0100"};/*Ӳ���汾��*/
	#endif
	
	len = _itoa(ver/*temp*/, (char*)&charVer[0]);
	memset((uint8*)&llParam.batDoor[addr].batDoorSysPara.comSoftVer[0],'0',4);
	memcpy((uint8*)&llParam.batDoor[addr].batDoorSysPara.comSoftVer[4-len],(uint8*)&charVer[0],len);
	memcpy((uint8*)&llParam.batDoor[addr].batDoorSysPara.comHareVer[0],(uint8*)&charVerHard[0],4);
}

/*
** ����Ƿ����ʧ���ֿذ�--ǿ����λ--����ģʽ��
*/
DoorNumDefine force_SetFlag(DoorNumDefine setFlag){
	uint8 i =0;

	for(i=0;i<SysCtr_AllDoorNum;i++){
		if(llParam.comBoardChk[i].comAbn == true/*ͨѶ��ͨѶ��ʧ*/){
			setFlag |= (DoorNumDefine)((((DoorNumDefine)1)<<i));
		}
	}

	return setFlag;
}

/*
** ��������汾 
*/
extern uint8 assignLabel;
extern uint16 assignSoftVer[SysCtr_AllDoorNum];
void LLParse_UpdateSoftVer(uint8 rxlen,uint8* item,uint8 addr){
	/*uint32 comAppRunVer = {0};*/
	uint16 comAppRunVer = 0;
	uint16 verTemp = 0;
	uint8 len = 0;
	uint8 label = 0;
	static uint8 lastLabel = 0;
	static DoorNumDefine clearBatFire = 0; 
	static DoorNumDefine enterTestMode = 0;
	uint16 upgrFailNum = 0;
	uint8 i = 0;

	/*
	** ���
	*/
	label = item[len];
	len += sizeof(uint8);

	/*����Ƿ������--���־λ*/
	if(label != lastLabel){
		lastLabel = label;
		clearBatFire = 0;
		enterTestMode = 0;
		for(i=0;i<SysCtr_AllDoorNum;i++){
			/*�����̼����,�����ͳ������ʧ�ܴ���*/
			set_statisAccUpgrNum(0, i);
		}
	}

	/*
	** ����汾��
	*/
	memcpy((uint8*)&verTemp,(uint8*)&item[len],sizeof(uint16));
	len += sizeof(uint16);

	/*
	** ���ݰ汾��������--�ж����ݳ���
	*/
	if((rxlen - len) > 0){
		/*�����Ӧ�̼���*/
		switch(item[len]){
			case 0x01:/*����ɹ�*/
			case 0x04:/*�̼������ִ��*/
				/*�����ع̼�����Ӧ*/
				clearBatFire |= (DoorNumDefine)((((DoorNumDefine)1)<<addr));
				/*����:20210525--ָ�������ع̼�--�ֿ�ʧ��Ĭ�������*/
				clearBatFire = force_SetFlag(clearBatFire);
				if(label >= 1 && label < (get_batFireSize() + 1)){
					if(get_ClearFire(0x03, label - 1) == true){
						if(clearBatFire == (DoorNumDefine)0xFFFFFFFFFFFF){
							clearBatFire = 0;
							set_ClearFire(0x03, false, label - 1);
						}
					}else{
						clearBatFire = 0;
					}
				}
				/*������̼�����Ӧ*/
				if(label >= 0x80 && label < (0x80 + get_ChargeFireSize())){
					if(get_ClearFire(0x04, label - 0x80) == true){
						if(clearBatFire == (DoorNumDefine)0xFFFFFFFFFFFF){
							clearBatFire = 0;
							set_ClearFire(0x04, false, label - 0x80);
						}
					}else{
						clearBatFire = 0;
					}
				}
				break;
			case 0x02:/*���ʧ��*/
				
				break;
		}
		len += sizeof(uint8);
		
		/*����Ӧ�̼����Ƿ��ѽ��ܲ���ģʽ*/
		if((rxlen - len) > 0){
			switch(item[len]){
				case 0x01:/**/
					/*��ع̼��������ģʽ��Ӧ*/
					enterTestMode |= (DoorNumDefine)((((DoorNumDefine)1)<<addr));
					/*
					** ����Ƿ����ʧ���ֿذ�--ǿ����λ--����ģʽ�� -- �޸�:ʧ��֮��,�������������ò���ģʽ,��ɲ���ģʽ������:AC�Ͽ�(�ֿظ�λ),���зֿؾ��Իظ��������ģʽ
					** 20210525 -- ���Ϊ:�ֿ�ʧ��Ĭ��Ϊ������̼�-- �ѷ���:CDG_MD_B_B1_300_380.bin/CDG_MD_B_B1_300_381.bin
					** �޸�:�߼����ε�20210525
					*/
					//enterTestMode = force_SetFlag(enterTestMode);
					if(label >= 1 && label < (get_batFireSize() + 1)){
						if(get_FireEnterTestModel(0x03, label - 1) == true){
							if(enterTestMode == (DoorNumDefine)0xFFFFFFFFFFFF){
								enterTestMode = 0;
								set_FireEnterTestModel(0x03, false, label - 1);
							}
						}else{
							enterTestMode = 0;
						}
					}
					/*���̼����������ģʽ��Ӧ*/
					if(label >= 0x80 && label < (0x80 + get_ChargeFireSize())){
						if(get_FireEnterTestModel(0x04, label - 0x80) == true){
							if(enterTestMode == (DoorNumDefine)0xFFFFFFFFFFFF){
								enterTestMode = 0;
								set_FireEnterTestModel(0x04, false, label - 0x80);
							}
						}else{
							enterTestMode = 0;
						}
					}
					break;
				case 0x02:/*���ʧ��*/
					
					break;
			}
		}
		len += sizeof(uint8);

		if((rxlen - len) > 0){
			/*��Ӧ�̼��Ƿ�������3����ʧ�� -- 20210301 -- �ֽ׶�(���ͬһ�̼�ͳһ���):�ƹ�������ֿ�����ʧ������֮��,��������,-- ��ع̼�����ʧ�����β�δ����*/
			memcpy((uint8*)&upgrFailNum,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			set_statisAccUpgrNum(upgrFailNum, addr);
		}
	}

	/*
	** ����ͨѶ������汾�� -- ������ע����Ϣ֡--�ֿ�����汾�ŵĸ���--20210301--�ɲ���ά��
	*/
	if(label == 0x00){/*ͨѶ��*/
		update_ComSfotVer(verTemp,addr);
	}

	/*
	** ָ�����������ѯָ�� -- �����߳������߼�����汾�űȶ��߼� -- 20210301
	*/
	if(assignLabel == label){
		assignSoftVer[addr] = verTemp;
	}
	

	/*
	** get ��ǰ���ư屣�����Ӧ��ַͨѶ������ -- �����Ƿ�����߳�,����汾�űȶ� -- 20210301
	*/
	comAppRunVer = getComRunAppVer(addr);
	
	if(comAppRunVer != verTemp){
		/*
		** ϵͳ����֮����ͨѶС��App���а汾
		*/
		setComRunAppVer(verTemp, addr);
	}

	/*
	**	�����ϱ��ӿ�ͨѶ����������汾�͵�ع̼�������汾--����:������̼���ƫ�Ƶ�ַ
	*/
	if(label >= 0x80){
		label = label - 0x80 + 36;/*ת���ڴ��ַƫ��ֵ*/
	}
	llp_ULP->runComVer[label][addr] = verTemp;
}

/*
** update upper Satet Info
*/
void update_UpperStateInfo(uint8 addr){
	/*bit0:���������*/
	if(llParam.chargeInfo[addr].state.bits.online == true){
		llp_ULP->stateInfoChange.sysLogic_2.chargeIsOnline |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic_2.chargeIsOnline &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
		/*
		** �����������clearʵʱ���� -- 20210301 -- Ӧ�ƹ����� -- ����������� -- ����������ʵʱ��Ϣ
		*/
		//memset((uint8*)&llParam.chargeInfo[addr].realtimeData.model,0,sizeof(RealtimeData));
	}

	/*bit1:���������*/
	if(llParam.chargeInfo[addr].state.bits.chargeOT == true){
		llp_ULP->stateInfoChange.sysLogic_2.chargeOT |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic_2.chargeOT &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}	

	/*bit2:���������*/
	if(llParam.chargeInfo[addr].state.bits.chargeP == true){
		llp_ULP->stateInfoChange.sysLogic_2.chargeP |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic_2.chargeP &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	/*bit3:�����ͨѶ����*/
	if(llParam.chargeInfo[addr].state.bits.chargeComErr == true){
		llp_ULP->stateInfoChange.sysLogic_2.chargeComErr |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic_2.chargeComErr &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	/*bit4:����ʧЧ*/
	if(llParam.chargeInfo[addr].state.bits.chargeCfgLose == true){
		llp_ULP->stateInfoChange.sysLogic_2.chargeCfgLose |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic_2.chargeCfgLose &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}	

	/*bit5:�����������*/
	if(llParam.chargeInfo[addr].state.bits.chargeOCODiff == true){
		llp_ULP->stateInfoChange.sysLogic_2.chargeOCODiff |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic_2.chargeOCODiff &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}

	/*bit6:���������*/
	if(llParam.chargeInfo[addr].state.bits.chargerOC == true){
		llp_ULP->stateInfoChange.sysLogic_2.chargerOC |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}else{
		llp_ULP->stateInfoChange.sysLogic_2.chargerOC &= (DoorNumDefine)~((DoorNumDefine)1<<addr);
	}	

	/*
	** ��������������־
	*/
	if(llp_ULP->stateInfoChange.sysLogic_2.chargeUpgr&(DoorNumDefine)((DoorNumDefine)0x01<<addr)){
		if(llParam.chargeInfo[addr].state.bits.chargeUpgrFinsh == true/*�������*/
			|| llParam.chargeInfo[addr].state.bits.online == false/*�����������*/){
			llp_ULP->stateInfoChange.sysLogic_2.chargeUpgr &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);
			if(llParam.chargeInfo[addr].state.bits.chargeUpgrOk == true/*����������ɹ�*/){
				llp_ULP->stateInfoChange.sysLogic_2.chargeUpgrIsOk |= (DoorNumDefine)((DoorNumDefine)0x01<<addr);
			}
		}
	}
}

/*
** update charge State Info
*/
void LLParse_ChargeStateInfo(uint8 rxlen,uint8* item,uint8 addr){
	uint8 len = 0;

	memcpy((uint8*)&llParam.chargeInfo[addr].state.flag,(uint8*)&item[len],sizeof(uint16));
	len += sizeof(uint16);

	/*
	** update upper Satet Info
	*/
	update_UpperStateInfo(addr);	
}

/*
** Charge Upgrade State
*/
DoorNumDefine chargerNomal_UpgrageIsTrue = 0;
void LLParse_ChargeUpgradeState(uint8 rxlen,uint8* item,uint8 addr){
	static uint8 frame[SysCtr_AllDoorNum] = {0};
	uint8 len = 0;
	uint8 frameLabel = 0;
	uint8 txlen = 1;
	uint8 tx[8] = {0};
	static UpgrFail upgrFailTemp[SysCtr_AllDoorNum] = {0};
	UpgrFilePara upgrTemp = {0};
	int16 loc = -1;
	uint16 upgrExceNum = 0;
	static uint8 upgrModel = 0;
	DoorNumDefine upgrTempField = 0xE000000000000000;
	static uint16 upgrNumSyn[SysCtr_AllDoorNum] = {0};
	static uint16 upgrSynTemp[SysCtr_AllDoorNum] = {0};/*ʹ��static ��ʶԭ����-->��˳��һ��*/
	static bool isChkSynFlag[SysCtr_AllDoorNum] = {0};/*ʹ��static ��ʶԭ����-->��˳��һ��*/	
		
	/*
	** ֡��ʶ ֡���ݳ�����ʱδʹ��
	*/
	frameLabel = item[len];
	len += sizeof(uint8);
	len += sizeof(uint8);

	switch(frameLabel){/*֡��ʶ*/
		case 0x01:/*Ӳ���汾+�̼�ID(4�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.hardVer,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&upgrFailTemp[addr].bat.fireId[0],(uint8*)&item[len],4);
			len +=  4;
			break;
		case 0x02:/*�̼�ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.fireId[4],(uint8*)&item[len],6);
			len += 6;
			break;
		case 0x03:/*�̼�ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.fireId[10],(uint8*)&item[len],6);
			len += 6;
			break;
		case 0x04:/*ԭ����汾��+������̼�ID(4�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.softVer,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&upgrFailTemp[addr].bat.id[0],(uint8*)&item[len],4);
			len += 4;
			break;
		case 0x05:/*������̼�ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.id[4],(uint8*)&item[len],6);
			len += 6;				
			break;
		case 0x06:/*������̼�ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.id[10],(uint8*)&item[len],6);
			len += 6;					
			break;
		case 0x07:/*����ģʽ*/
			upgrModel = item[len];
			len += 1;
			if(rxlen == 0x05){/*Э��V1.9����:������ʶ*/
				memcpy((uint8*)&upgrSynTemp[addr],(uint8*)&item[len],sizeof(uint16));
				len += sizeof(uint16);
				isChkSynFlag[addr] = true;/*���ͬ����־*/
			}			
			break;
	}

	/*
	** ���֡�Ƿ��������
	*/
	frame[addr] |= 0x01<<(frameLabel - 1);
	if(frame[addr] == 0x7F){
		frame[addr] = 0;
		/*��ȡ�̼�λ��*/
		memcpy((uint8*)&upgrTemp.upgrFileType.detailedInfo.hardVer,(uint8*)&upgrFailTemp[addr].bat.hardVer,sizeof(DetailedInfo));
		/*�������upgrFailTemp[addr]����*/
		memset((uint8*)&upgrFailTemp[addr].bat.hardVer,0,sizeof(UpgrFail));
		loc = get_ChargeLocation(upgrTemp);
		/*����Ƿ�����ֿ�����*/
		if(loc == -1 ){/*�̼�ID���Ϸ�*/
			tx[0] = 0x02;/*�ܾ�����*/
		}else{
			/*����:��ع���,��������½�ֹ����*/
			if(chk_IsExitBatChargerOT() == true){
				tx[0] = 0x04;/*�ܾ�����--��"��ع���","���������"*/
			}else{
				switch(upgrModel){
					case 0x01:/*����ģʽ*/
						if(get_FireUpgrNum(0x04, loc) == (int16)-1){/*��ѯ��ع̼���Ϊ���޴������߼�-->����:����ģʽ�������޴���������ͳ����������*/
							tx[0] = 0x01;/*��������*/
							chargerNomal_UpgrageIsTrue |= (DoorNumDefine)((DoorNumDefine)0x01<<addr);
						}else{
							upgrExceNum = get_FireRealityRunNum(0x04, loc);
							if(get_FireUpgrNum(0x04, loc) <= upgrExceNum){
								tx[0] = 0x02;/*�ܾ�����*/
							}else{
								tx[0] = 0x01;/*��������*/
								if(upgrNumSyn[addr] != upgrSynTemp[addr] /*ͬ����־��-->����Ƿ�Ϊ��ͬ��������*/
									|| isChkSynFlag[addr] == false /*V1.8Э��֮����ͬ���߼�*/
								){
									upgrExceNum++;
									if(upgrExceNum == 0xFFFF){
										upgrExceNum = 1;
									}
									set_FireRealityRunNum(0x04, upgrExceNum, loc);	
								}
							}
						}						
						break;
					case 0x02:/*����ģʽ*/
						tx[0] = 0x01;/*��������*/
						break;
				}			

				/*��������ģʽ����λ����������������*/
				if(tx[0] == 0x01){/*��������*/
					/*���³��������ģʽ*/
					switch(upgrModel){
						case 0x01:/*����ģʽ*/
							llp_ULP->stateInfoChange.sysLogic_2.chargeUpgr &= (DoorNumDefine)~upgrTempField;
							break;
						case 0x02:/*����ģʽ*/
							llp_ULP->stateInfoChange.sysLogic_2.chargeUpgr |= (DoorNumDefine)upgrTempField;
							break;
					}
					
					llp_ULP->stateInfoChange.sysLogic_2.chargeUpgr |= (DoorNumDefine)((DoorNumDefine)0x01<<addr);
					llp_ULP->stateInfoChange.sysLogic_2.chargeUpgrIsOk &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);
					hmiUpgrIsFinshFlag &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);
					/*20210301--�����ƹ�����,�ֿؽ�֧�ֳ����/��������е�һ��,���Ż����������*/
					llp_ULP->stateInfoChange.sysLogic.batUpgr &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);
					llp_ULP->stateInfoChange.sysLogic.batUpgrIsOk &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);
				}
			}
		}
		/*��ģʽ����*/
		upgrModel = 0;
		/*��ͬ����ʶ*/
		upgrSynTemp[addr] = 0;
		isChkSynFlag[addr] = false;
		/*�ظ��ֿذ�*/
		CAN_TransmitAnalysis(CAN_Port_1, txlen, (uint8 *)&tx[0], addr, LL_FunId_chargeUpgrState);
	}
}

/*
** charge Realtime Info
*/
void LLParse_ChargeRealtimeInfo(uint8 rxlen,uint8* item,uint8 addr){
	uint8 len = 0;
	uint8 frameLabel = 0;
	
	/*
	** ֡��ʶ ֡���ݳ����ݲ�����
	*/
	frameLabel = item[len];
	len += sizeof(uint8);
	len += sizeof(uint8);

	switch(frameLabel){/*֡��ʶ*/
		case 0x01:/*���ģʽ,���ģʽ,��ѹ���,�������*/
			memcpy((uint8*)&llParam.chargeInfo[addr].realtimeData.model,(uint8*)&item[len],6);
			len += 6;
			break;
		case 0x02:/*�����ʵʱ�¶�,�����״̬,����״̬,����״̬*/
			memcpy((uint8*)&llParam.chargeInfo[addr].realtimeData.temp,(uint8*)&item[len],5);
			len += 5;
			break;
	}
}

/*
** Charge Base Info
*/
extern DoorNumDefine reset_IsGetBaseInfoFlag;/*��⸴λ�Ƿ��ȡ��������Ϣ*/
void LLParse_ChargeBaseInfo(uint8 rxlen,uint8* item,uint8 addr){
	static uint8 frame[SysCtr_AllDoorNum] = {0};
	uint8 len = 0;
	uint8 frameLabel = 0;
	uint8 dataLen = 0;
	uint8 txlen = 1;
	uint8 tx[8] = {0x02};/*ȷ���յ�������Ϣ*/
	
	/*
	** ֡��ʶ ֡���ݳ����ݲ�����
	*/
	frameLabel = item[len];
	len += sizeof(uint8);
	len += sizeof(uint8);	

	switch(frameLabel){/*֡��ʶ*/
		case 0x01:/*boot�汾��.���̴���,Ӳ���汾��,App����汾��*/
			memcpy((uint8*)&llParam.chargeInfo[addr].base.boot,(uint8*)&item[len],6);
			len += 6;
			break;
		case 0x02:/*��������ѹ,����������*/
			memcpy((uint8*)&llParam.chargeInfo[addr].base.maxVolt,(uint8*)&item[len],2);
			len += 2;
			break;
		case 0x03:/*�����ID*/
		case 0x04:
		case 0x05:
			dataLen = frameLabel==0x05?0x04:0x06;
			memcpy((uint8*)&llParam.chargeInfo[addr].base.chargeId[(frameLabel-0x03)*6],(uint8*)&item[len],dataLen);
			len += dataLen;
			break;
	}

	/*
	** ���֡�Ƿ��������
	*/
	frame[addr] |= 0x01<<(frameLabel-1);
	if(frame[addr] == 0x1F){
		frame[addr] = 0;
		/*
		** �ظ��ֿذ�
		*/
		CAN_TransmitAnalysis(CAN_Port_1, txlen, (uint8 *)&tx[0], addr, LL_FunId_chargeBaseInfo);		
		reset_IsGetBaseInfoFlag |= (DoorNumDefine)((DoorNumDefine)1<<addr);
	}	
}

/*
** upgr Fail Reason
*/
volatile uint8 testii = 0;
void LLParse_UpgrFailReason(uint8 rxlen,uint8* item,uint8 addr){
	static uint8 frame[SysCtr_AllDoorNum] = {0};
	uint8 len = 0;
	uint8 frameLabel = 0;
	uint8 txlen = 1;
	uint8 tx[8] = {0};
	static UpgrFail upgrFailTemp[SysCtr_AllDoorNum] = {0};
	uint8 fireType = 0;
	UpgrFilePara upgrTemp = {0};
	int16 loc = -1;
	uint16 num = 0;
	static uint16 upgrNumSyn[SysCtr_AllDoorNum] = {0};
	static uint16 upgrSynTemp[SysCtr_AllDoorNum] = {0};/*ʹ��static ��ʶԭ����-->��˳��һ��*/
	static bool isChkSynFlag[SysCtr_AllDoorNum] = {0};/*ʹ��static ��ʶԭ����-->��˳��һ��*/
	volatile bool repReasonFlag = false;/*���ڵ��Զ�������*/
	
	/*
	** ֡��ʶ ֡���ݳ�����ʱδʹ��
	*/
	frameLabel = item[len];
	len += sizeof(uint8);
	len += sizeof(uint8);

	switch(frameLabel){/*֡��ʶ*/
		case 0x01:/*Ӳ���汾+�̼�ID(4�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.hardVer,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&upgrFailTemp[addr].bat.fireId[0],(uint8*)&item[len],4);
			len +=  4;
			break;
		case 0x02:/*�̼�ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.fireId[4],(uint8*)&item[len],6);
			len += 6;
			break;
		case 0x03:/*�̼�ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.fireId[10],(uint8*)&item[len],6);
			len += 6;
			break;
		case 0x04:/*ԭ����汾��+���/�����ID(4�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.softVer,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&upgrFailTemp[addr].bat.id[0],(uint8*)&item[len],4);
			len += 4;
			break;
		case 0x05:/*���/�����ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.id[4],(uint8*)&item[len],6);
			len += 6;				
			break;
		case 0x06:/*���/�����ID(6�ֽ�)*/
			memcpy((uint8*)&upgrFailTemp[addr].bat.id[10],(uint8*)&item[len],6);
			len += 6;					
			break;
		case 0x07:/*����ʧ��ԭ��,�̼�����汾�� + ����ģʽ*/
			upgrFailTemp[addr].bat.reson = item[len];
			len += sizeof(uint8);
			memcpy((uint8*)&upgrFailTemp[addr].bat.fireSoftVer,(uint8*)&item[len],sizeof(uint16));
			len += sizeof(uint16);
			upgrFailTemp[addr].bat.upgrModel = item[len];/*����ģʽ*/
			len += sizeof(uint8);
			if(rxlen == 0x08){/*Э��V1.9����:������ʶ*/
				memcpy((uint8*)&upgrSynTemp[addr],(uint8*)&item[len],sizeof(uint16));
				len += sizeof(uint16);
				isChkSynFlag[addr] = true;/*���ͬ����־*/
			}			
			break;
	}


	/*
	** ���֡�Ƿ��������
	*/
	frame[addr] |= 0x01<<(frameLabel - 1);
	if(frame[addr] == 0x7F){
		frame[addr] = 0;
		/*��ȡ�̼�λ��*/
		memcpy((uint8*)&upgrTemp.upgrFileType.detailedInfo.hardVer,(uint8*)&upgrFailTemp[addr].bat.hardVer,sizeof(DetailedInfo));
		/*�̼�����*/
		if(upgrFailTemp[addr].bat.fireId[0] == 'A'){/*'A'--��ع̼�*/
			fireType = 0x03;
			loc = get_BatLocation(upgrTemp);
		}
		if(upgrFailTemp[addr].bat.fireId[0] == 'C'){/*'C'--�����*/
			fireType = 0x04;
			loc = get_ChargeLocation(upgrTemp);
		}

		/*�ֿؼ���Ƿ�Ϊ����ģʽ-->���ظ�������ģʽ�´��������жϽ�����:��������ģʽ/Pre OTA*/
		if(get_FireUpgrNum(fireType, loc) == -1 /*������������:����ģʽ��,��Pre OTAģʽ*/
			|| (((batNomal_UpgrageIsTrue&(1<<addr)) != 0)&& (fireType == 0x03))
			|| (((chargerNomal_UpgrageIsTrue&(1<<addr)) != 0)&& (fireType == 0x04))){
			
			if(loc != -1){
				if(upgrFailTemp[addr].bat.upgrModel == 0x01){/*�ֿ��ж�Ϊ"����ģʽ"*/
					upgrFailTemp[addr].bat.upgrModel = 0x03;/*����ģʽ*/
					switch(fireType){
						case 0x03:
							batNomal_UpgrageIsTrue &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);
							break;
						case 0x04:
							chargerNomal_UpgrageIsTrue &= (DoorNumDefine)~((DoorNumDefine)0x01<<addr);
							break;
					}
					
				}
			}
		}

		if(upgrNumSyn[addr] != upgrSynTemp[addr] /*ͬ����־��-->����Ƿ�Ϊ��ͬ��������*/
			|| isChkSynFlag[addr] == false /*V1.8Э��֮����ͬ���߼�*/
		){
			/*������ʧ��ԭ����������*/
			set_UpgrFailPara(fireType,upgrFailTemp[addr], addr);
		}else{/*�����߼�--���������һ��*/
			testii = 1;
		}
		/*ͳ�����������߼�*/
		switch(upgrFailTemp[addr].bat.upgrModel){/*20210425��PROOAT��������ͳ�Ʒ���--����������*/
			case 0x01:/*Pre OTAģʽ*/
// 				if(loc != -1){
// 					num = get_FireRealityRunNum(fireType, loc);
// 					num++;
// 					if(num == 0xFFFF){
// 						num = 1;
// 					}
// 					set_FireRealityRunNum(fireType, num, loc);
// 				}
				break;
			case 0x02:/*����ģʽ*/
				if(loc != -1){
					if(upgrNumSyn[addr] != upgrSynTemp[addr] /*ͬ����־��-->����Ƿ�Ϊ��ͬ��������*/
						|| isChkSynFlag[addr] == false /*V1.8Э��֮����ͬ���߼�*/
					){
						if(upgrFailTemp[addr].bat.reson == 0xFF/*�����ɹ�*/){
							num = get_TestModelFireUpgrNum(fireType, loc);
							num++;
							if(num == 0xFFFF){
								num = 1;
							}
							set_TestModelFireUpgrNum(fireType, num, loc);
						}else{/*����ʧ��*/
							num = get_TestModelFireUpgrNumFail(fireType, loc);
							num++;
							if(num == 0xFFFF){
								num = 1;
							}
							set_TestModelFireUpgrNumFail(fireType, num, loc);
						}
					}
				}
				break;
			case 0x03:/*����ģʽ*/
				
				break;
		}
		/*ͬ������*/
		upgrNumSyn[addr] = upgrSynTemp[addr];
		/*���������*/
		memset((uint8*)&upgrFailTemp[addr].bat.reson,0,sizeof(UpgrFail));	
		/*��ͬ����ʶ*/
		upgrSynTemp[addr] = 0;
		isChkSynFlag[addr] = false;
		/*�ظ��ֿذ�*/
		if(repReasonFlag == false){
			CAN_TransmitAnalysis(CAN_Port_1, txlen, (uint8 *)&tx[0], addr, LL_FunId_upgrFailReason);
		}
	}
}

/*
** �²�Э��֮�����������ӿ�
*/
void SM_LowerLayerParse_Task(void* p_arg){
	/*
	** �²�Э��֮������ʼ��
	*/
	LowerLayerParam_Init();

	Sleep(2000);
	
	for(;;){
		/*
		** �²�Э��֮���ݽ���֮����������
		*/
		lowerUpgrFunAnalyze();

		/*
		** �ж�ͨѶ���Ƿ�ʧ��,����ϲ��ֶ���Ϣ
		*/
		updateComIsAbn();

		/*
		** ����״̬��Ϣ���²�����(�ղ���,������,������,�쳣������,�ܲ���)
		*/
		updateDoorNum();		

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
**�ṩϵͳ��������²�Э������ӿں���
*/
void LowerLayerParse_Init(void){
	Thread_create(SM_LowerLayerParse_Task, 0,
		(int32 *)&lowerLayerParse_TaskStk[512-1], (int8)LowerLayerParseTask_Prio);
}

