#include "includes.h"

/*
** �²�Э�������������
*/
extern LowerLayerParam llParam;

/*
** �²�Э��֮��ز�״̬����
*/
extern void LLParse_BatDoorState(uint8 rxlen,uint8* item,uint8 addr);
/*
** �������״̬
*/
extern void LLParse_UpgrState(uint8 rxlen,uint8* item,uint8 addr);
/*
** ����ϵͳ��Ϣ
*/
extern void LLPrse_ChgSysInfo(uint8 rxlen,uint8* item,uint8 addr);
/*
** �²�Э��֮BMS��Ϣ
*/
extern void LLParse_BatInfo(uint8 rxlen,uint8* item,uint8 addr);
/*
** ��������汾 
*/
extern void LLParse_UpdateSoftVer(uint8 rxlen,uint8* item,uint8 addr);
/*
** ����֮��ѯͨѶС����������
*/
extern void UpgradeLLParse_ChkComRunArea(uint8 len,uint8* item,uint8 addr);
/*
** ����֮�����ļ��汾��
*/
extern void UpgradeLLParse_FileVer(uint8 len,uint8* item,uint8 addr);
/*
** ����֮�ļ��ֽ���+�ļ�����У��
*/
extern void UpgradeLLParse_FileByteItemCrc(uint8 len,uint8* item,uint8 addr);
/*
** ����֮�ļ�����
*/
extern void UpgradeLLParse_FileItem(uint8 len,uint8* item,uint8 addr);
/*
** update charge State Info
*/
extern void LLParse_ChargeStateInfo(uint8 rxlen,uint8* item,uint8 addr);
/*
** Charge Upgrade State
*/
extern void LLParse_ChargeUpgradeState(uint8 rxlen,uint8* item,uint8 addr);
/*
** charge Realtime Info
*/
extern void LLParse_ChargeRealtimeInfo(uint8 rxlen,uint8* item,uint8 addr);
/*
** Charge Base Info
*/
extern void LLParse_ChargeBaseInfo(uint8 rxlen,uint8* item,uint8 addr);
/*
** upgr Fail Reason
*/
extern void LLParse_UpgrFailReason(uint8 rxlen,uint8* item,uint8 addr);
/*
** �²�Э��֮���ݽ���
*/
static const LowerUpgrFunTable lowerUpgrFunTable[] = {
	{LL_FunId_BatDoorState,LLParse_BatDoorState},
	{LL_FunId_BatUpgrState,LLParse_UpgrState},
	{LL_FunId_ChgSysInfo,LLPrse_ChgSysInfo},
	{LL_FunId_BmsInfo,LLParse_BatInfo},
	{UpgradeLL_FunId_ChkAppRunVer,LLParse_UpdateSoftVer},
	{UpgradeLL_FunId_RunArea,UpgradeLLParse_ChkComRunArea},
	{UpgradeLL_FunId_Ver,UpgradeLLParse_FileVer},
	{UpgradeLL_FunId_FileByteCrc,UpgradeLLParse_FileByteItemCrc},
	{UpgradeLL_FunId_FileItem,UpgradeLLParse_FileItem},
	{LL_FunId_chargeStateInfo,LLParse_ChargeStateInfo},
	{LL_FunId_chargeUpgrState,LLParse_ChargeUpgradeState},
	{LL_FunId_chargeRealtimeInfo,LLParse_ChargeRealtimeInfo},
	{LL_FunId_chargeBaseInfo,LLParse_ChargeBaseInfo},
	{LL_FunId_upgrFailReason,LLParse_UpgrFailReason}
};
static uint8 lowerUpgrFunTableNum = sizeof(lowerUpgrFunTable)/sizeof(LowerUpgrFunTable);

/*
** �²�Э��֮���ݽ���֮����������
*/
void lowerUpgrFunAnalyze(void){
	CanRxMsg rxMsg = {0};
	uint8 funId = 0;
	uint8 i=0;
	uint8 deviceAddr = 0;	

	for(;CAN_RecAnalysis(CAN_Port_1, (CanRxMsg *)&rxMsg) == true;){
		funId = (uint8)((rxMsg.ExtId>>16)&0x000000FF);
		for(i=0;i<lowerUpgrFunTableNum;i++){
			if(funId == lowerUpgrFunTable[i].funId){
				deviceAddr = (uint8)(rxMsg.ExtId&0x000000FF);		
				/*
				** ���ӵ�ַ�ݴ���
				*/
				if(deviceAddr<=(uint8)23){
					lowerUpgrFunTable[i].handle(rxMsg.DLC,(uint8*)&rxMsg.Data[0],deviceAddr);
					/*
					** �ж�ͨѶ�Ƿ�ʧ��
					*/
					TickOut((uint32 *)&llParam.comBoardChk[deviceAddr].itick, 0);
					llParam.comBoardChk[deviceAddr].cnt = 0;
					llParam.comBoardChk[deviceAddr].comAbn = false;
				}
				break;
			}
		}
		/*
		** ���ſ�ι��
		*/
		watchdogUpdate();
	}

	for(;CAN_RecAnalysis(CAN_Port_2, (CanRxMsg *)&rxMsg) == true;){
		funId = (uint8)((rxMsg.ExtId>>16)&0x000000FF);
		for(i=0;i<lowerUpgrFunTableNum;i++){
			if(funId == lowerUpgrFunTable[i].funId){
				deviceAddr = (uint8)(rxMsg.ExtId&0x000000FF);	
				/*
				** ���ӵ�ַ�ݴ���
				*/
				if(deviceAddr >= (uint8)24 && deviceAddr <= (uint8)47){
					lowerUpgrFunTable[i].handle(rxMsg.DLC,(uint8*)&rxMsg.Data[0],deviceAddr);
					/*
					** �ж�ͨѶ�Ƿ�ʧ��
					*/
					TickOut((uint32 *)&llParam.comBoardChk[deviceAddr].itick, 0);
					llParam.comBoardChk[deviceAddr].cnt = 0;
					llParam.comBoardChk[deviceAddr].comAbn = false;
				}
				break;
			}
		}
		/*
		** ���ſ�ι��
		*/
		watchdogUpdate();
	}		
}

