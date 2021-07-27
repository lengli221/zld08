#ifndef __CABINETCTRINFO_H_
#define __CABINETCTRINFO_H_

/*
** ������Ϣ֮�Ӵ�����������
*/
void cabCtrInfo_EventId_ContactorFun(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ������Ϣ֮����SOC����
*/
void cabCtrInfo_EventId_UpdateSocPara(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ���ʱ����ֵ
*/
void cabCtrInfo_EventId_ChgTime(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** �����������ֵ
*/
void cabCtrInfo_EventId_ChgOverTemp(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ������ֹ�����ֵ
*/
void cabCtrInfo_EventId_ChgDoorOT(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ���������Ϣ֮ϵͳ�¶������Ϣ
*/
void cabCtrInfo_EventId_SysTempInfo(uint8 itemDatalen,uint8* rx,
	uint8*replyItemDataLen,uint8* tx);
/*
** ���ư�ִ��BootLoader����:
**		1.����Զ������ָ��
**		2.�ȴ�ִ�лظ�,800ms֮���������ؽ���BootLoader
*/
void enterCtrBootLoader(void);
/*
** ����ͨѶ��/��ذ��ļ�����:
**		1.��������ͨѶС������
**		2.�ȴ�ִ�лظ�����,���800ms֮������ϲ������л�
*/
void ctrBoard_TaskSwitch(void);
/*
** set �ָ��ϲ�Э��֮APPӦ�ò�Э���־
*/
void set_CtrBoardTaskRecoverFlag(void);
/*
** �ָ��ϲ�Э��֮APPӦ�ò�Э�� 
**		���Ʋ���:
**				1.�ļ��������֮��,���ñ�־
**				2.�ȴ��ظ�(��ʱ)800ms֮�����������л�
*/
void ctrBoard_TaskRecover(void);
/*
** ���������Ϣ֮�����ļ�
*/
void cabCtrInfo_EventId_UpgradeFileCmd(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ���������Ϣ֮�����ļ�
*/
void cabCtrInfo_EventId_GetFileName(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ���������Ϣ֮��ȡָ����ذ��̼���Ϣ
*/
void cabCtrInfo_EventId_GetBatFireInfo(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ��ѯ���г�������¶�
*/
void cabCtrInfo_EventId_AllchgTemp(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ��ѯBMS����״̬/����״̬
*/
void cabCtrInfo_EventId_BmsPFState(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ��ȡWIFI���ƺ�����
*/
void cabCtrInfo_EventId_GetWifiNamePw(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ��ѯSOC��ֵ
*/
void cabCtrInfo_EventId_ChkSocLimit(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ��ѯͨѶ�����а汾��/��ع̼��汾��
*/
void cabCtrInfo_EventId_ChkRunComVer(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ������Ӧ�̼�����������
*/
void cabCtrInfo_EventId_CfgFireUpgrNum(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** �ظ������Ӧ�̼���
*/
void cabCtrInfo_EventId_ClearFire(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ��Ӧ�ֹ̼�����ʧ��ԭ���ϱ�
*/
void cabCtrInfo_EventId_UpgrFailReson(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** �̼�����ģʽ
*/
void cabCtrInfo_EventId_FireEnterTestModel(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** �����ƹ̼����������������,��ѯʵ�������Ĵ���
*/
void cabCtrInfo_EventId_ChkRealityUpgrNum(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ����ģʽ��,��ػ�������������
*/
void cabCtrInfo_EventId_TestModelFireUpgrNum(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** Charger OC Limit Cfg
*/
void cabCtrInfo_EventId_ChargerOCLimitCfg(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*-------------------------------------------�����ѹУ׼ֵ--------------------------------------------------------------------------*/
/*
** ��ѯ��ѹ��AD����ֵ
*/
void cabCtrInfo_EventId_ChkPhaseLvVoltAD(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ��ѯ��ѹ�����ֵ
*/
void cabCtrInfo_EventId_ChkPhaseHvVoltAD(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ���������ѹУ׼ϵ��
*/
void cabCtrInfo_EventId_SetPhaseAjust(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ��ȡʵʱ��ѹ
*/
void cabCtrInfo_EventId_GetRuntimeVolt(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*-----------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------����̼�------------------------------------------------------------------*/
/*
** һ�����--�ֿ�/���/�����
*/
void cabCtrInfo_EventId_OneKeyClearFire(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*-----------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------���ų���-��������������--��˽ӿ�(�ⲿ���)-1.05-----------------------------------------------------*/
/*
** ���ù��±�����ֵ
*/
void cabCtrInfo_EventId_SetBatOTempLimit(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** �����϶����±�����ֵ
*/
void cabCtrInfo_EventId_SetNanduLowPLimit(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*
** ���õ��±�����ֵ
*/
void cabCtrInfo_EventId_SetBatLowPLimit(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------��ؽ���SOC-----------------------------------------------------------*/
void cabCtrInfo_EventId_ChkAllInsertSoc(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------�����������־---------------------------------------------------*/
void cabCtrInfo_EventId_PhaseCurOpenFlag(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx);
/*-----------------------------------------------------------------------------------------------------------------------------------*/

#endif

