#ifndef __HMI_DW_DATA_H_
#define __HMI_DW_DATA_H_

#define HMI_DW_PAGE_MAIN									(uint8)0x00/*��ҳ��*/
#define HMI_DW_PAGE_BatInfo								(uint8)0x01/*�����Ϣҳ��*/
#define HMI_DW_PAGE_Waring									(uint8)0x02/*һ������ҳ��*/
#define HMI_DW_PAGE_SysInfo								(uint8)0x03/*ϵͳ��Ϣ*/
#define HMI_DW_PAGE_CtrUpgr								(uint8)0x04/*���ư�����ҳ��*/
#define HMI_DW_PAGE_InputPw								(uint8)0x06/*�������Ա����*/
#define HMI_DW_PAGE_PwNoCorrect							(uint8)0x08/*���벻��ȷ*/
#define HMI_DW_PAGE_WifiNamePw										(uint8)0x11/*WIFI �û���������*/


typedef union{
	uint8 flag;
	struct{
		uint8 hv:1;/*��ѹ��־*/
		uint8 lv:1;/*Ƿѹ��־*/
		uint8 oc:1;/*������־*/
		uint8 res:5;/*����*/
	}bits;
}Hmi_PhaseVC;


typedef struct
{
	uint8 id;
	uint8 state;
	void (*handle)(void);
}HMI_DW_PAGE;

/*
** ҳ���л��ӿں���
*/
void HMI_DW_PageDataTableFun(void);
/*
** ҳ����������ʼ��
*/
void HMI_Init(void);

#endif

