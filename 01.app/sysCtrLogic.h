#ifndef __SYSCTRLOGIC_H_
#define __SYSCTRLOGIC_H_

/*������*/
#define SysCtr_AllDoorNum						(uint8)0x30//ϵͳ������--����:��ز���--48

/*
** ϵͳ���Ʋ����ṹ�嶨��
*/
#pragma pack(1)

#pragma pack()


/*
** ���һ��ҳ�澯��
*/
bool checkFirstPageWaring(void);
/*
** ����һ��������ֹ�����Լ������ļ�
*/
bool oneWaring_ForbidUpgrDownFile(void);
/*
** ����:����һ���澯����ж�--20210512
*/
bool cabinet_OneWaringFlagJudge(void);
/*
** 20210302 -- һ�����������ֶ��ָ����߼�--���ֶ����µ�(�µ�AC)�߼�
*/
void oneWaring_NeedHandRecoverLogic(void);
/*
**�ṩϵͳ�������ϵͳ���ƽӿں���
*/
void SysCtrlLogic_Init(void);

#endif

