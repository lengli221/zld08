#ifndef __CALCPHASEVOLCUR_H_
#define __CALCPHASEVOLCUR_H_

/*
** �����ѹ��������ṹ������
*/
typedef struct{
	float abcPV[3];/*A,B,C���ѹ*/
	float abcPC[3];/*A,B,C�����*/
}CalcPhaseVC;

/*-----------------------------------�����ѹУ׼-------------------------------------------------*/
#pragma pack(1)
typedef struct{
	bool flag[3];/*�Ƿ������У׼��־*/	
	float k[3];/*����ϵ��У׼ֵ*/
	float b[3];
	uint16 volt[3];/*�����ѹ*/
}PhaseVoltAdjustLogic;
#pragma pack()
/*------------------------------------------------------------------------------------------------*/

/*
** get_���������ѹ����
** 		����:�������
*/
CalcPhaseVC get_CalcPhaseVC(void);

/*
** ��ȡ��ѹ��ADֵ
*/
uint16 get_LvAdValue(uint8 ch);

/*
** ��ȡ��ѹ��ADֵ
*/
uint16 get_HvAdValue(uint8 ch);

/*
** ����У׼ϵ��
*/
void set_PhaseVoltAdjust(uint8 ch,bool flag,float k,float b);
/*
** ��λ��ʼ��У׼ϵ��
*/
void reset_PhaseVoltAdjust(void);
#endif

