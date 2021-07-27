#ifndef __UPGRADEFLASHADDRALLOT_H
#define __UPGRADEFLASHADDRALLOT_H

/*
** Զ������֮�ļ���С���Ʋ���
*/
#define UpgrFileSizeLimit											(uint32)0x00032000/*0x00019000*//*100K -- �޸ĳ�:200K*/

/*
** Զ������֮�ⲿFlash��ַ����
** 1.ͨѶС���ַ����
** 2.��ذ���ַ����
** 3.����:������̼�����ַ����
*/
#define Upgrade_FlashAddrInit									(uint32)0x001F4000/*2048000/1024 = 2000K*/
#define Upgr_FlashAddr_Bat(loc)										(uint32)(Upgrade_FlashAddrInit + UpgrFileSizeLimit + loc*UpgrFileSizeLimit)
#define Upgr_FlashAddr_Charge(loc)									(uint32)(Upgrade_FlashAddrInit + UpgrFileSizeLimit + UpgrBatMemoryNum*UpgrFileSizeLimit + loc *UpgrFileSizeLimit)

/*
** get Flash �����洢��ַ
*/
uint32 getUpgrFlashAddr(UpgrFilePara upgr);

#endif

