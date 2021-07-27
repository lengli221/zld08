#ifndef __MODIFY_EEPROMLOGIC_H_
#define __MODIFY_EEPROMLOGIC_H_

/*
** �޸�EEPROM�洢��ȡ�߼�
*/
#pragma pack(1)
typedef struct{
	uint8 board;/*�̼�������Ϣ:0x01--�ֿع̼�,0x02--��ع̼�,0x03--������̼�*/
	uint8 label;/*���--λ����Ϣ*/
	uint16 softver;/*����汾��*/
	uint32 flashAddr;/*flash�洢��ַ*/
	BinFileItemPara binFileItemPara;
}ModifyEepromLogic;/*bin�ļ�����*/
#pragma pack()

/*
** 20210202-->ת���ṹ��洢
*/
ModifyEepromLogic set_TypedefModify(UpgrFilePara* upgr);
/*
** 20210202-->ת���ṹ��洢 -- ָ����ʼ������
*/
ModifyEepromLogic set_TypedefModify_Assign(UpgrFilePara* upgr,uint8 lo);
/*
** 20210220 --> �ṹ��ת��
*/
UpgrFilePara conv_TypedefModify(ModifyEepromLogic modifyEL);

#endif

