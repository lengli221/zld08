#ifndef __LCDDATA_VOICE_H_
#define __LCDDATA_VOICE_H_

/*
** voice loc 
*/
#define _DoorBase_VoiceLoc				(uint8)140/*���Ż������*/
#define _SmokeWaring						(uint8)188/*������*/
#define _ChgDoorTempOT					(uint8)190/*��������¶ȹ���*/
#define _ChgTempOT							(uint8)192/*������¶ȹ���*/
#define _BatTempOT							(uint8)194/*����¶ȹ���*/
#define _ChgTimeTimeOut					(uint8)196/*���ʱ�䳬��10Сʱ*/
#define _BatUpgr								(uint8)198/*�������*/
#define _ComUpgr								(uint8)200/*�ֿ�����*/
#define _BatErr									(uint8)202/*��ع���*/
#define _comErr								(uint8)204/*�ֿع���*/
#define _ChgErr								(uint8)206/*���������*/
#define _ComIsLose							(uint8)208/*�ֿ�ʧ��*/
#define _AllBatUpgr							(uint8)210/*���е��������*/
#define _AllComUpgr							(uint8)212/*���зֿ�������*/
#define _FileDownload						(uint8)214/*�ļ�������*/
#define _PhaseOverV							(uint8)216/*��ѹ����*/
#define _PhaseLV								(uint8)218/*Ƿѹ����*/
#define _PhaseOC								(uint8)220/*��������*/
#define _BatUpgrFinsh								(uint8)222/*����������*/

/*
** ��Ƶ����
*/
bool lcd_WriteVoiceCtr(void *pData,uint8 dev);
#endif

