#ifndef __COMBOARDUPGRADEPARAM_H_
#define __COMBOARDUPGRADEPARAM_H_

#pragma pack(1)
typedef union{
	uint8 flag;
	struct{
		uint8 comBoard:1;/*ͨѶ��*/
		uint8 batBms:1;/*���BMS*/
		uint8 res:6;
	}bits;
}BinFileType;

typedef struct{
	uint8 upgradeWay;/*������ʽ*/
	BinFileType binFileType;/*bin�ļ�����*/
	uint16 softVer;/*����汾��*/
	DetailedInfo detailedInfo;/*�����ϸ��Ϣ*/
	uint32 flashAddr;/*flash��ַ*/
	BinFileItemPara binFileItemPara;/*bin�ļ����ݲ���,�ֽڳ���,����У��*/
}ComBup;
#pragma pack()

/*
** ͨѶ������������ʼ��
*/
void ComBoardUpgrPara_Init(void);
/*
** �����ǰͨѶ�����������
*/
void Clear_CurComBupPara(void);
/*
** get ��ǰ����������ز���
*/
ComBup getCurComBup(void);
/*
** ����ͨѶС����в���
*/
void UpdataComBoardQueuePara(ComBup com);
/*
** ͨѶ���������+������ز�������
*/
void ComBoard_TaskSwitch(void);
#endif

