#ifndef __UPGRFAILRESON_H_
#define __UPGRFAILRESON_H_

#pragma pack(1)
typedef struct{
	uint8 reson;/*ʧ��ԭ��*/
	uint16 hardVer;/*Ӳ���汾*/
	uint8 fireId[16];/*�̼�ID*/
	uint16 softVer;/*����ǰ��ذ汾��*/
	uint8 id[16];/*���ID*/
	uint16 fireSoftVer;/*�̼�����汾��*//*����V1.8*/
	uint8 upgrModel;/*����ģʽ*//*�ֿ�Э��:V1.7*/
}Bat;/*ͬһ�ṹ�����ڵ������ʧ��ԭ��ͳ����ʧ��ԭ��ṹ��*/

typedef struct{
	Bat bat;
}UpgrFail;/*����ʧ��ԭ��--����Ե�ش���,Ϊ��Լ�ռ�*/

/*
** Door Exit Upgr Reson No Read Flag
*/
typedef struct{
	DoorNumDefine bat;/*���*/
	DoorNumDefine charger;/*�����*/
}DoorEUResonNoRead;
#pragma pack()

/*
** ����ʧ��ԭ�������ʼ��--���ʧ��ԭ�����3(FIFOQueue_DepthLimit)������
*/
void init_UpgrFailPara(void);
/*
** set--����ʧ��ԭ��
*/
void set_UpgrFailPara(uint8 type,UpgrFail u,uint8 loc);
/*
** get--����ʧ��ԭ��
*/
void* get_UpgrFailPara(uint8 type,uint8 loc);
/*
** 20210518--����Ƿ��������ԭ��δ��ȡ
*/
void runtimeChk_UpgrResonFlag(void);
/*
** 20210518--��ȡԭ���ϱ�
*/
DoorEUResonNoRead get_DoorEUResonNoRead(void);
#endif

