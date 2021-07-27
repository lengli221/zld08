#ifndef __UPGRADEUL_H_
#define __UPGRADEUL_H_

#include "ctrGetUpgrFilePara.h"

/*
** �ļ���������ݳ���
*/
#define FileName_MaxLen										30/*�������ŵ�ذ������ļ�����ļ�����󳤶�*//*128*/
/*
** Զ������֮����֡��󳤶�
*/
#define FrameDataItem_MaxLen								1024	
/*
** Զ������֮����֡֡ͷ
*/
#define Frame_Head												(uint8)0x68
/*
** Զ������֮�̼���ַ
*/
#define Firmware_HandwareAddr								(uint8)0xCC
/*
** Ӧ���־Ack
*/
#define UpgradeFrame_Ack										(uint8)0x06				

/*
** Զ������֡Ӧ���־FLAG
*/
enum{
	ChkFrameStartFlag = 0x02,//����"�������ݴ���"���̿���--0x03
	ChkFrameIngFlag = 0x01,
	ChkFrameStopFlag = 0x03,

	FileNameFormatIilegal = 0xF5,//�ļ�����ʽ���Ϸ�(������bin�ļ�)
	WriteAppErr = 0xF6,//дAPP���쳣
	EraseAppErr = 0xF7,//����APP���쳣
	EraseBackupErr = 0xF8,//�����������쳣
	FrameIdSpan = 0xF9,//����֡��Խ
	FrameIdRepeat = 0xFA,//����֡�ظ�
	WriteFlashErr = 0xFB,//д�������쳣
	FileItemChkErr = 0xFC,//�ļ�����У�����
	FrameTimout = 0xFE,//֡��ʱ,ȡ������ģʽ
};

/*
*1.�ֽڶ��뷽������
*#pragma�Ƿ���������
*2.�ṹ��"�ֶ�"����ʼ��ַ��ռ�ڴ�ռ��С
*/
#pragma pack(1)
/*
** ����֡�ṹ
*/
typedef struct{
	uint8 flag;
	uint8 addr;
	uint16 frameId;
	uint16 dataFrameLen;
	uint16 crc16;
}UpgradeProFrame;

/*
** �ļ�����������
*/
typedef struct{
	uint8 flag;
	uint32 word;
	uint16 fileCrc16;
	uint8 fileName[FileName_MaxLen];
}FileNameItem;

/*
** ֡�ظ�����������
*/
typedef struct{
	uint8 ack;
	uint8 flag;
}UpgradeReply;
#pragma pack()

/*
** Զ������֮ϵͳ����
** 1.���̿���
** 2.�̼���ַ
** 3.ͨѶС������Flag
*/
typedef struct{
	uint32 itick;/*��ʱ��*/
	bool comBinFileRecOk;/*ͨѶС��bin�ļ��������*/
}ComBinParam;

typedef struct{
	uint8 step;/*���̿���*/
	uint16 frameId;/*��ǰ֡ID*/
	uint16 frameLastId;/*��ִ����һ֡֡ID*/
	BinFileItemPara binFilePara;/*bin�ļ�����*/
}ProcessCtr;

typedef struct{
	uint8 addr;
	uint32 interFlashAddr;/*��¼Flash��ַ*/
}Firmware;;

typedef struct{
	ProcessCtr processCtr;
	Firmware firmware;
}UpgradeSysPara;

/*
** Զ������֮����ִ�к������ṹ��
*/
typedef struct{
	uint8 step;
	void (*handle)(void*,uint16);
}UpgradePro;

/*
** �ṩϵͳ���֮Զ�������ӿں���֮�ϲ�Э��
*/
void UgradeULParse_Init(void);
/*
** Զ������֮����bin�ļ�֮����˸Ƶ��--500ms
** param:flag:true--���� false--�ر�
*/
void UpgradeUL_CtrLedLogic(bool flag);
#endif

