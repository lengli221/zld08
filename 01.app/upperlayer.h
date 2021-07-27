#ifndef __UPPERLAYER_H_
#define __UPPERLAYER_H_

#include "sysCtrLogic.h"

/*
** ֡ͷ֡β
*/
#define UpperLayerFrame_Head						(uint8)0x68
#define UpperLayerFrame_End							(uint8)0x16

/*
** ֡���ȺϷ�������--���������Ϣ���������¼�Ϊ��·�֡--�μ�Э���_v0.03
*/
#define FrameLenIsLegal_Min							(uint16)0x0009
#define FrameLenIsLegal_Max							(uint16)0x0039

/*
** ֡ID
** 1.�����ϲ�Э��֡
** 2.�ظ��ϲ�Э��֡
*/
#define CMD_ID_Register_Rx							(uint8)0x01/*ע����Ϣ*/
#define CMD_ID_Logout_Rx							(uint8)0x02/*ע����Ϣ*/
#define CMD_ID_StateInfoChange_Rx					(uint8)0x03/*״̬��Ϣ���*/
#define CMD_ID_StateInfo_Rx							(uint8)0x04/*״̬��Ϣ*/
#define CMD_ID_Ctr_Rx								(uint8)0x05/*������Ϣ*/
#define CMD_ID_BmsInfo_Rx							(uint8)0x06/*BMS��Ϣ*/
#define CMD_ID_Domain_Rx							(uint8)0x07/*������Ϣ*/
#define CMD_ID_BatMainInfo_Rx						(uint8)0x08/*��عؼ���Ϣ*/
#define CMD_ID_ChargeInfo_Rx						(uint8)0x0A/*��ѯ�����Ϣ*/

#define CMD_ID_Register_Tx							(uint8)0x81
#define CMD_ID_Logout_Tx							(uint8)0x82
#define CMD_ID_StateInfoChange_Tx					(uint8)0x83
#define CMD_ID_StateInfo_Tx							(uint8)0x84
#define CMD_ID_Ctr_Tx								(uint8)0x85
#define CMD_ID_BmsInfo_Tx							(uint8)0x86
#define CMD_ID_Domain_Tx							(uint8)0x87
#define CMD_ID_BatMainInfo_Tx						(uint8)0x88
#define CMD_ID_ChargeInfo_Tx						(uint8)0x8A


/*
** ������Ϣ֮�¼�����
*/
enum{
	CabCtrInfo_EventId_Contactor 				= 0x0010,/*�Ӵ�����������*/
	CabCtrInfo_EventId_UpdateSoc 				= 0x0020,/*����SOC����ֵ*/
	CabCtrInfo_EventId_ChgTime					= 0x0030,/*���ʱ����ֵ*/
	CabCtrInfo_EventId_ChgOverTemp			= 0x0040,/*�����������ֵ*/
	CabCtrInfo_EventId_ChgDoorOT				=0x0050,/*������ֹ�����ֵ*/
	CabCtrInfo_EventId_SysTempInfo				= 0x0060,/*ϵͳ�¶���Ϣ*/
	CabCtrInfo_EventId_UpgrCmd					= 0x0070,/*Զ����������*/
	CabCtrInfo_EventId_GetFileName 				= 0x0080,/*�ļ���*/
	CabCtrInfo_EventId_GetBatFireInfo			= 0x0090,/*��ȡָ����ذ��̼���Ϣ*/	
	CabCtrInfo_EventId_GetchgerTemp 			= 0x00B0,/*������¶�*/
	CabCtrInfo_EventId_GetWifiNamePw			= 0x00C0,/*��ȡwifi���ƺ�����*/
	CabCtrInfo_EventId_BmsPF						= 0x00D0,/*BMS����״̬/����״̬*/
	CabCtrInfo_EventId_ChkSocLimit				= 0x00E0,/*��ѯSOC��ֵ*/
	CabCtrInfo_EventId_ChkRunComVer 			= 0x00F0,/*��ѯ���а汾(ͨѶ��/��ع̼�)*/
	/*-------------------------------------------�����ѹУ׼ֵ------------------------------------------------------------------*/
	CabCtrInfo_EventId_ChkLvAd					= 0x2120,/*��ѯ��ѹ��ADֵ*/
	CabCtrInfo_EventId_ChkHvAd					= 0x2220,/*��ѯ��ѹ��ADֵ*/
	CabCtrInfo_EventId_SetPAdjust				= 0x2420,/*����У��ϵ��*/	
	CabCtrInfo_EventId_GetRuntimeV				= 0x2320,/*��ȡʵʱ��ѹ*/
	/*-----------------------------------------------------------------------------------------------------------------------------------*/
	CabCtrInfo_EventId_CfgFireUpgrN			= 0x0100,/*������Ӧ�̼�����������*/
	CabCtrInfo_EventId_ClearFire					= 0x0110,/*�����Ӧ�̼���*/
	CabCtrInfo_EventId_UpgrFailR					= 0x0120,/*��Ӧ�ֹ̼�����ʧ�ܵ�ԭ��*/
	CabCtrInfo_EventId_FireEnterT				= 0x0130,/*�̼��������ģʽ*/
	CabCtrInfo_EventId_ChkRealityN				= 0x0140,/*��ѯʵ�ʹ̼���������*/
	CabCtrInfo_EventId_TestMUpgrNum			= 0x0150,/*����ģʽ�²�ѯ�̼���������*/
	CabCtrInfo_EventId_ChargerOCL				= 0x0160,/*�����������ֵ*/
	/*---------------------------------------------------------����̼�-------------------------------------------------------------*/
	CabCtrInfo_EventId_OneClearAllFire 			= 0x3030,/*����ֿ�/���/������̼�--������й̼�*/
	/*-----------------------------------------------------------------------------------------------------------------------------------*/
	/*----------------------------���ų���-��������������--��˽ӿ�(�ⲿ���)-1.05------------------------------*/
	CabCtrInfo_EventId_BatOTempLimit			= 0x0170,/*���ù�����ֵ*/
	cabCtrInfo_EventId_NanduLowPLimit			= 0x0180,/*�����϶�������ֵ*/
	CabCtrInfo_EventId_BatLowPLimit				= 0x0190,/*���õ�ص�����ֵ*/
	/*-----------------------------------------------------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------��ؽ���SOC-----------------------------------------------------------*/
	CabCtrInfo_EventId_ChkAllInsSoc				= 0x01A0,
	/*-----------------------------------------------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------------------------------------------*/
	CabCtrInfo_PhaseVCDebugMode				= 0x3131
	/*-----------------------------------------------------------------------------------------------------------------------------------*/
};
	
/*
** �ϲ�Э��֮�ṹ������
*/
#pragma pack(1)
typedef struct{
	uint8 fireId[10];/*���ұ�ʶ(�����ͺ�)-->String*/
	uint8 id[16];/*����ID-->String*/
	uint8 ctrHareVer[4];/*���ư�Ӳ���汾��->String*/
	uint8 ctrSoftVer[4];/*���ư�����汾��->String*/
	uint8 ctrModel[6];/*�����ͺ�*/
}SysSet;/*ϵͳ���ò���*/

typedef union{
	uint16 flag;
	struct{
		uint16 isOnline:1;/*����*/
		uint16 interCom:1;/*�ڲ�ͨѶ*/
		uint16 smoke:1;/*����*/
		uint16 fan:1;/*����*/
		uint16 res:12;
	}bits;
}State;

typedef struct{
	State state;/*ϵͳ״̬*/
	int16 chgDoorTemp;/*������ֻ���*/
}SysFire;/*ϵͳ�̼�����*/

typedef struct{
	DoorNumDefine batOnline;/*����Ƿ����� ��1:����*/
	DoorNumDefine chgOverTemp;/*��������� ��1:���������*/
	DoorNumDefine batComErr;/*���ͨѶ����*/
	DoorNumDefine chargingErr;/*����쳣*/
	DoorNumDefine batLowTemp;/*��ص���*/
	DoorNumDefine batIsErr;/*����Ƿ���� ��1:���� -- ��ʱ����Ե�ع���*/
	DoorNumDefine batFault;/*���:��1����*/
	DoorNumDefine comFault;/*�ֿ�:��1����*/
	DoorNumDefine batChgOTime;/*��س��ʱ�����*/
	DoorNumDefine comIsOnline;/*�ֿذ��Ƿ����� ��1:����*/
	DoorNumDefine comUpgr;/*�ֿذ����� ��1:������*/
	DoorNumDefine comUpgrIsOk;/*�ֿذ������Ƿ�ɹ�  ��1:�����ɹ�*/
	DoorNumDefine batFileDownload;/*����ļ�������*/
	DoorNumDefine batFileDownloadIsOk;/*����ļ�������� ��1:�������*/
	DoorNumDefine batUpgr;/*������� ��1:������*/
	DoorNumDefine batUpgrIsOk;/*��������Ƿ�ɹ� ��1:�����ɹ�*/
}SysLogic;

typedef union{
	uint16 flag;
	struct{
		uint16 emer:1;/*��ͣ��ť ��1:����*/
		uint16 smoke:1;/*�̸� ��1:����*/
		uint16 phaseOverVFault:1;/*���ѹ��ѹ>270V,���µ���ָܻ�*/
		uint16 phaseOverVWaring:1;/*���ѹ��ѹ�ɻָ�*/
		uint16 ledOpen:1;/*���ⱨ�� ��1:����*/
		uint16 fanOpen:1;/*���� ��1:����*/
		uint16 majorLoopClose:1;/*����· ��1:�Ͽ�--�޸ĳ�:����·*/
		uint16 phaseLV:1;/*���ѹǷѹ,�ɻָ�*/
		uint16 phaseOC:1;/*���������*/
		uint16 phaseCShort:1;/*�������·*/
		uint16 comIdRep:1;/*�ֿذ�ID�ظ� ��1:�쳣*/
		uint16 comErr:1;/*�ֿذ�ͨѶ ��1:�쳣*/
		uint16 chgDoorOverTemp:1;/*���ֹ��� ��1:����*/
		uint16 comRecoverFlag:1;/*�ֿ�һ������֮��λ��־*/
		uint16 anti:1;/*����*/
		uint16 ctrUpgr:1;/*����������*/
	}bits;
}SysModuleStateInfo;

typedef union{
	uint16 flag;
	struct{
		uint16 hardSwitchClose:1;/*Ӳ�����ضϿ�*/
		uint16 abcPhaseCurO:3;/*ABC�������*/
		uint16 abcPhaseCurShort:3;/*���������·*/
		uint16 testModelFlag:1;/*����ģʽ��־--������ڲ���ģʽ*/
		uint16 res_1:8;/*����λ*/
	}bits;
}SysModuleStateInfo_2;

typedef struct{
	uint8 idle;/*�ղ���*/
	uint8 ing;/*������*/
	uint8 full;/*������*/
	uint8 abn;/*�쳣������*/
	uint8 total;/*�ܲ���*/
}DoorTypeNum;

/*
** Sys Logic 2
*/
typedef struct{
	/*------------------------V1.6------------------------------------*/
	DoorNumDefine chargeIsOnline;/*������Ƿ�����*/
	DoorNumDefine chargeOT;/*���������*/
	DoorNumDefine chargeP;/*���������*/
	DoorNumDefine chargeComErr;/*�����ͨѶ����*/
	DoorNumDefine chargeCfgLose;/*���������ʧЧ*/
	DoorNumDefine chargeOCODiff;/*����������������*/
	DoorNumDefine chargeFileDownloading;/*�����������*/
	DoorNumDefine chargeFileDownloadFinsh;/*������������*/
	DoorNumDefine chargeUpgr;/*�����������*/
	DoorNumDefine chargeUpgrIsOk;/*����������Ƿ�ɹ�*/	
	/*---------------------------------------------------------------*/
	DoorNumDefine chargerOC;/*V1.5--���������*/
}SysLogic_2;

typedef struct{
	SysLogic sysLogic;/*�ֽ���:128*/
	uint8 sameFireRepCnt;/*ͬһ�̼���������--�ֽ���:1*/
	SysModuleStateInfo sysModuleStateInfo;/*�ֽ���:2*/
	SysModuleStateInfo_2 sysModuleStateInfo_2;/*�ֽ���:2*/
	DoorTypeNum doorTypeNum;/*�ֽ���:5*/
	SysLogic_2 sysLogic_2;/*�ֽ���:88*/
}StateInfoChange;/*V1.5--ͳ�������ֽ�:226*/

typedef struct{
	DoorNumDefine batOnlineCtrAC;/*��1:���߿���AC*/
	DoorNumDefine remoteCtrAC;/*��1:Զ�̿���AC*/
	DoorNumDefine sysErrCtrAC;/*��1:ϵͳ���϶�AC*/
	DoorNumDefine comIdRep;/*��1:ID�ظ�*/
	DoorNumDefine batOnline_1minNoCur;/*��1:�������,����1min�޵�������-->���������*/
}StateInfo;

typedef struct{
	uint16 dataItemlen;/*�������*/
	uint8 tx[128];/*����������*/
}CabinetCtrInfoReply;

typedef struct{
	bool flag;
	uint32 itick;
}UpgrCtrReset;/*���ư�ȷ�������������ư�֮��ȴ��ظ�(��ʱ)֮�����bootLoader��*/

typedef struct{
	bool flag;
	uint32 itick;
	uint32 itickout;/*�����ļ���ʱ��ʱ��*/
}UpgrComBatTaskEn;/*���ư�ȷ����������ͨѶ��/��ذ��ļ�֮��ȴ��ظ�(��ʱ)֮�����������л�*/

typedef struct{
	SysSet sysSet;
	SysFire sysFire;
	StateInfoChange stateInfoChange;
	StateInfo stateInfo;
	CabinetCtrInfoReply cabinetCtrInfoReply;
	UpgrCtrReset upgrCtrReset;
	UpgrComBatTaskEn upgrComBatTaskEn;
	UpgrComBatTaskEn upgrComBatTaskDis;
	/*��������ͨѶ������а汾���ϱ�*/
	uint16 runComVer[45][SysCtr_AllDoorNum];/*note:runComVer[0]:ͨѶ���������а汾,1-35��Ϊ:��ع̼����汾,ʣ���Ϊ������̼����汾*/
}UpperLayerPara;

typedef struct{
	bool flag;/*������Ϣ*/
}Domain;

typedef struct{
	bool flag;/*�Ƿ�ע��*/
}RegisterLogic;

typedef struct{
	bool flag;/*�Ƿ�ע��*/
}LogoutLogic;

typedef struct{
	Domain domain;/*������Ϣ*/
	RegisterLogic registerLogic;/*ע������*/
	LogoutLogic logoutLogic;/*ע������*/
}UpperLayerReplyPara;

/*
** ��������ϵͳ�ṹ����ؽ���--�ϱ��ϲ�Э�鶨��ṹ��
*/
typedef struct{
	bool refresh;/*���±�־*/
	uint8 id;/*��ϢID*/
	uint8 deviceAddr;/*�豸��ַ--�μ�Э�����ߵ�ַ˵������(��ʱ����)*/
	uint8 doorAddr;/*���ŵ�ַ*/
}SmCmd;

/*
** ֡��ʽ
*/
typedef struct{
	uint8 id;/*��Ϣid*/
	uint16 dataLen;/*���ݳ���*/
	uint8 addr;/*���豸��ַ*/
}MsgHead;

typedef struct{
	uint8 head;/*֡��ʼ��*/
	uint16 msgFrameLen;/*֡����*/
	MsgHead msgHead;/*��Ϣͷ*/
	uint8 chkCode;/*У����*/
	uint8 end;/*֡������*/
}UpperLayerFrame;

/*
** ������Ϣ֮�ӿں����ṹ��
*/
typedef struct{
	uint16 itemId;//�¼�ID
	void (*handle)(uint8,uint8*,uint8*,uint8*);/*�¼������ݳ���,�¼������� �ظ�������� �ظ�����������*/
}CabinetCtrInfo;

/*
** ���ܺ���֮�����ϲ�Э��
*/
typedef struct{
	uint8 id;/*��ϢID*/
	void (*handle)(uint16,uint8*);/*�������,������*/
}UpperLayerFunTable;

/*
** ���ܺ���֮�ظ��ϲ�Э��
*/
typedef struct{
	uint8 id;/*��ϢID*/
	void (*handle)(UpperLayerFrame*,uint8*,uint8);/*��Ϣ��ʽ,��������,���ŵ�ַ*/
}UpperLayerReplyFunTable;
#pragma pack()

/*
** �ϲ�Э��֮�ṹ�����ָ���ṩ�ṹ����
*/
UpperLayerPara* getUpperLayerPara(void);
/*
** �ϲ�Э��֮�ظ����������ӿں���֮�ⲿ��������
*/
UpperLayerReplyPara* getULReplyPara(void);
/*
** �ṩ�����ļ����ýӿں���
** 1.�����������
** 2.��ѯ�������
*/
void setCmd(SmCmd cmd);
uint8 getCmd(void);
/*
** �ṩϵͳ��������ϲ�Э������ӿں���
*/
void UpperLayerParse_Init(void);
/*
** �ṩϵͳ��������ϲ�Э��ظ��ӿں���
*/
void UpperLayerReply_Init(void);
#endif

