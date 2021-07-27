#ifndef __PHASEVCLIMITANALSZE_H
#define __PHASEVCLIMITANALSZE_H

/*
** ��ʱ��ʱ���׼ʱ��
*/
#define tickTimeBase_V									(uint32)10000

/*
** ���ѹ��ֵ������־�ṹ��
*/
typedef union{
	uint8 flag;
	struct{
		uint8 a:1;/*A��*/		
		uint8 b:1;/*B��*/
		uint8 c:1;/*C��*/
		uint8 res:5;
	}bits;
}PVol;

typedef struct{
	uint8 flag;
	struct{
		uint8 a:1;/*A��*/		
		uint8 b:1;/*B��*/
		uint8 c:1;/*C��*/
		uint8 res:5;
	}bits;
}PCur;

typedef struct{
	PVol pVol;
	PCur pCur;
}PhaseVCAnalyze;

/*
** Phase VC Debug Mode
*/
typedef struct{
	bool flag;/*���ñ�־*/
}PhaseVCDebugMode;

/*
** init Phase VC Debug Mode
*/
void init_Phase_VC_Debug_Mode(void);

/*
** set Phase VC Debug Mode
*/ 
void set_Phase_VC_Debug_Mode(void);

/*
** ���ѹ/������쳣�������
** 		����һ:�ر����г����
*/
bool PhaseVCAbnCloseAllCharger(void);
#endif

