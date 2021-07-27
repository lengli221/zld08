#ifndef __CTRGETUPGRFILEPARA_H_
#define __CTRGETUPGRFILEPARA_H_

#include "upgradeUL.h"

/*
** �ļ�����
*/
#define ComBoradType										0x01/*ͨѶ��*/
#define BatBoardType											0x02/*��ذ�*/
#define ChargeBoardType									0x03/*�����*/

/*
** ���ư������ļ�֮�����ṹ��
*/
#pragma pack(1)
typedef struct{
	uint16 hardVer;/*Ӳ���汾*/
	uint8 id[16];/*���ID*/
}DetailedInfo;

typedef struct{
	uint8 cmdType;/*��������/ǿ������*/
	uint8 board;/*��������:ͨѶ��/��ذ�*/
	DetailedInfo detailedInfo;/*������ϸ��Ϣ*/
	uint16 softVer;/*����汾*/
	uint32 flashAddr;/*flash�洢��ַ*/
	uint8 fileNameLen;/*�ļ�������*/
	uint8 fileName[FileName_MaxLen];/*�̼�����(���ų���24Byte):�������ݶ�30Byte,Ԥ��6Byte*/
	BinFileItemPara binFileItemPara;/*bin�ļ�����*/
}UpgrFileType;/*65 byte*/

typedef struct{
	UpgrFileType upgrFileType;
}UpgrFilePara;
#pragma pack()

/*
** ���ư������ļ�֮����������ʼ��
*/
void init_UpgrFilePara(void);
/*
** set���ư������ļ�֮�������� 
*/
void setUpgrFilePara(UpgrFilePara u);
/*
** get���ư������ļ�֮�������� 
*/
UpgrFilePara getUpgrFilePara(void);

#endif

