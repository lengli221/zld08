#ifndef __UPGRADELL_H_
#define __UPGRADELL_H_

/*
** Ŀ���ַ֮�㲥����
*/
#define DstAddr_BoardCast_Define							(uint8)0xBB

/*
** �㲥����������
*/
#define BroadCast_UgpradeMaxCnt							(uint8)0x04/*Ӧ��������:����3�ζ���Ϊ������Ҫ����4��*/

/*
** ֡�������
*/
#define UpgradeLL_FrameMax									(uint8)0x05

/*
** ����֮����#define����
** 1.��ѯͨѶС��boot loader--Fun ID:0xF0
** 2.�����ļ��汾��--Fun ID:0xF1--Ӧ��APP�㴦��
** 3.�ļ��ֽ���+�ļ�����У��--Fun ID:0xF2
** 4.�ļ�����----Fun ID:0xF3
** 5.��ѯApp����汾�� -- Fun ID:0xF4(ע��:���ư�,ͨѶ��App����)
*/
#define UpgradeLL_FunId_RunArea							(uint8)0xF0
#define UpgradeLL_FunId_Ver									(uint8)0xF1
#define UpgradeLL_FunId_FileByteCrc							(uint8)0xF2
#define UpgradeLL_FunId_FileItem							(uint8)0xF3
#define UpgradeLL_FunId_ChkAppRunVer					(uint8)0xF4

/*
** ���̿��Ʋ�������
*/
enum{
	UpgradeLL_ProcChkRunArea = 0x00,/*��ѯ��������*/
	UpgradeLL_Proc_FileByteCrc = 0x01,/*�ļ��ֽ���+�ļ�����У��*/
	UpgradeLL_Proc_DataItemTra = 0x02,/*��ʼ���ݴ����־*/ 
	UpgradeLL_Proc_Finsh = 0x03,/*�������(��ע:bin�ļ���������)*/
	UpgradeLL_Proc_Over = 0xFF,/*ͨѶС���������*/
};

/*
** ����֮�����ṹ������
** 1.���̿���
** 2.��ѯͨѶС����������
** 3.
*/
typedef struct{
	uint8 step;/*���̲���*/
	DoorNumDefine onLine;/*���߱�־*/
	DoorNumDefine areaFlag;/*��1������BootLoader ��0������App����*/
	DoorNumDefine upgradePermit;/*��1�������� ��0�ܾ�����*///ע��:��ʱ��־areaFlag/upgradePermit����һ���� ������ʱ���200MS
	DoorNumDefine fileByteCrcRecFlag;/*��1��ʾ�ļ��ֽ���+�ļ�����У����ջظ�֡*/
	DoorNumDefine binDataItemTransmit;/*��1��ʾ��ʼ���ݴ���*/
	DoorNumDefine binDataItemEndFlag;/*��1��ʾ������������--20210226--�������ð�׿ͳ�Ƴɹ���ʧ�ܴ���*/
	DoorNumDefine fileItemRecFlag;/*��1--����֡�������*/
}ProcCtr;

typedef struct{
	uint8 cnt;
}ProcCtrRep;

typedef struct{
	uint8 circCnt;/*ѭ������*/
	uint32 itick;/*��ʱ��*/
}RunArea;

typedef struct{
	uint8 circCnt;/*ѭ������*/
	uint32 itick;/*��ʱ��*/
	uint8 cmd;/*������������:0x01--��ʼ�������� 0x02--�������ݴ��� 0xFD:ȡ������*/
}FileByteItemCrc;

typedef struct{
	uint16 frameLabel;/*֡��ʶ*/
	uint8 dataItemLen;/*֡���������ֽ���*/
	uint8 data[sizeof(uint32)];/*�ļ�����*/
}FileItem;

typedef struct{
	uint8 cnt;/*֡�ظ�����*/
	uint32 itick;/*��ʱ��*/
}FileItemCtr;

typedef struct{
	uint8 cnt;/*֡�ظ�����*/
	uint32 itick;/*��ʱ��*/
}AppRunVer;

typedef struct{
	ProcCtr proCtr;
	RunArea runArea;
	FileByteItemCrc fileByteItemCrc;
	FileItem fileItem;
	FileItemCtr fileItemCtr;
	AppRunVer appRunVer;
}UpgradeLLParam;

typedef struct{
	ProcCtrRep proCtrRep;	
}UpgrLLPara;

/*
** �²�Э��֮ϵͳģ�鴦���б�ṹ������
*/
typedef struct{
	uint8 funId;/*���ܺ���ID*/
	void (*handle)(uint8,uint8*,uint8)/*1.���ݳ��� 2.������ 3.�豸��ַ*/;
}UpgradeParseFunTable;

/*
** ����֮�����ṹ�������ʼ��
*/
void UpgradeLLParam_Init(void);
/*
** ���������������
*/
void clear_UpgrLLParamCnt(void);
/*
** ���������Լ�
*/
void add_UpgrCnt(void);
/*
** ����֮�ļ�����֮����֡��ɷ��ͱ�־λ
*/
bool Upgrade_FileItemTraFinshFlag(void);
/*
** ����֮���������ļ����ݲ���
*/
void Upgrade_UpdateFileItemPara(uint16 frameLabel);
/*
** ָ��ȡ��������ӦͨѶ��������
*/
void UpgradeLL_AssignCanelUpgrade(uint8 addr);
/*
** �ṩϵͳ���֮Զ�������ӿں���֮�²�Э��֮�ظ�����
*/
void UpgradeLLReply_Init(void);
/*
** �ṩϵͳ���֮Զ�������ӿں���֮�²�Э��֮��������
*/
void UpgradeLLParse_Init(void);

#endif

