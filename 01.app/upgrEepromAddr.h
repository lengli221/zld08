#ifndef __UPGREEPROMADDR_H_
#define __UPGREEPROMADDR_H_

typedef struct{
	uint32 ver;/*�汾��*/
	uint32 fileLen;/*�ļ�������*/
	uint32 fileItem;/*�ļ�����*/
}UpgrEepromAddr;


/*
** ��ȡ���������洢EEPROM��ַ
*/
uint32 get_UpgrEepromAddr(UpgrFilePara upgr);
/*
** ��ȡ�������(Ӳ���汾��,���ID)�洢EEPROM��ַ
** ���Ʋ���:
** 				���ڵ�س�ʼ��������ȡEEPROM��ַ
*/
uint32 get_UpgrBatEepromAddr(uint8 loc,UpgrFilePara upgr);
#endif

