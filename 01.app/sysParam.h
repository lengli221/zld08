#ifndef __SYSPARAM_H_
#define __SYSPARAM_H_

#include "lcdData_DW.h"

/*
** ���ٿռ�����
**		1.��ع̼��������ռ俪�ٺͳ�����̼��������ռ俪��
*/
#define UpgrBatMemoryNum							(uint8)35
#define UpgrChargeMemoryNum						(uint8)9

/****************************ϵͳ������ַ����Backup��EEPROM**********************************/
#define UpgradeKey_Head								(uint8)0xA5

/*backup���������ַ����*/
#define ByteReserveOffset								(0x04)/*Ԥ��֮������Կ����*/

#define UpgradeKey_Addr								0x0000
#define UpgradeKey_Len_Addr							sizeof(UpgradeKey)+0x03/*head+crc16*/

#define UpgradeVer_Addr								ByteReserveOffset + UpgradeKey_Addr + UpgradeKey_Len_Addr
#define UpgradeVer_Len_Addr							sizeof(uint32) + 0x03

#define UpgradeFileNameLen_Addr					ByteReserveOffset+UpgradeVer_Addr+UpgradeVer_Len_Addr
#define UpgradeFileNameLen_Len_Addr				sizeof(uint16)+0x03

#define UpgradeFileName_Addr							ByteReserveOffset + UpgradeFileNameLen_Addr + UpgradeFileNameLen_Len_Addr
#define UpgradeFileName_Len_Addr					128 + 0x03

#define UpgradeOffset_Addr							0x00B4/*ƫ�Ƶ�ַ:180*//*0x100*/

/****************************************************************************************************************************************/
/*
** ���ñ���ע����Ϣ������ַ
*/
 #define Register_LogoutInfo_Addr					UpgradeOffset_Addr
 #define Register_LogoutInfo_Len						(sizeof(uint8) + sizeof(uint16))

 #define LocalRemoteDomain_Addr						Register_LogoutInfo_Addr + Register_LogoutInfo_Len
#define LocalRemoteDomain_Len						(128 + sizeof(uint16))
#define SysDeviceID_Addr								LocalRemoteDomain_Addr + LocalRemoteDomain_Len
#define SysDeviceId_Len									sizeof(DeviceId) + sizeof(uint16)

#define WifiNamePara_Addr								(SysDeviceID_Addr + SysDeviceId_Len)
#define WifiNamePara_Len								(32+sizeof(uint16))

#define WifiPwPara_Addr								(WifiNamePara_Addr + WifiNamePara_Len)
#define WifiPwPara_Len									(32+sizeof(uint16))

/*
** 20120324--���ڵ�һ����ع̼��������ȡ���������ڷ����汾��ع̼���������ַ�ʹ洢�ֽڳ����ص�,���µ�ذ��������ж�Ϊ��һ����ع̼����ƴ���
** �������:����2��ƫ�Ƶ�ַ������wifi�����ַ������
** �����޸�EEPROM��ַ�����޸������洢�߼�--����ӳ���������߼�--�ֳ�����:Ŀǰ���ŵ�һ����ع̼���δʹ��,�������κ�Ӱ��
*/
#define Android_Effective_Addr 						(WifiPwPara_Addr + WifiPwPara_Len + sizeof(uint16))

/*
** �����Ӧ�̼�������������
*/
#define BatFireLimitNum_Addr(loc)							(Android_Effective_Addr + loc*BatFireLimitNum_Len)
#define BatFireLimitNum_Len							(sizeof(int16) + sizeof(uint16))
/*
** ��ع̼�ʵ����������
*/
#define BatRealityUpgrNum_Addr(loc)							((Android_Effective_Addr + UpgrBatMemoryNum*BatFireLimitNum_Len) + loc * BatRealityUpgrNum_Len)
#define BatRealityUpgrNum_Len						(sizeof(uint16) + sizeof(uint16))

/*
** ������̼���������������
*/
#define ChargeLimitNum_Addr(loc)							(((Android_Effective_Addr + UpgrBatMemoryNum*BatFireLimitNum_Len) + UpgrBatMemoryNum * BatRealityUpgrNum_Len) + loc*ChargeFireLimitNum_Len)
#define ChargeFireLimitNum_Len						(sizeof(uint16) + sizeof(uint16))

/*
** ������̼�ʵ����������
*/
#define ChargeRealityNum_Addr(loc)							((((Android_Effective_Addr + UpgrBatMemoryNum*BatFireLimitNum_Len) + UpgrBatMemoryNum * BatRealityUpgrNum_Len) + UpgrChargeMemoryNum*ChargeFireLimitNum_Len)+loc*ChargeRealityNum_Len)						
#define ChargeRealityNum_Len							(sizeof(uint16) + sizeof(uint16))

/*
** ��ذ�����(Ӳ���汾��,���ID)����
*/
#define UpgrBatFileNumAddr							((((Android_Effective_Addr + UpgrBatMemoryNum*BatFireLimitNum_Len) + UpgrBatMemoryNum * BatRealityUpgrNum_Len) + UpgrChargeMemoryNum*ChargeFireLimitNum_Len)+UpgrChargeMemoryNum*ChargeRealityNum_Len)
#define UpgrBatFileNumLen								sizeof(uint16) + sizeof(uint16)/*CRCУ��*/

/*
** ������̼�������
*/
#define UpgrChargeNumAddr							(UpgrBatFileNumAddr+UpgrBatFileNumLen)
#define UpgrChargeNumLen								sizeof(uint16) + sizeof(uint16)/*CRCУ��*/

/*-------------------------------------------ʹ����λ�����������ѹ----------------------------------------------------------------*/
#define AdjusrPhaseVoltAddr							(UpgrChargeNumAddr+UpgrChargeNumLen)
#define AdjusrPhaseVoltLen								(((sizeof(bool)+sizeof(float)+sizeof(float))*3)+sizeof(uint16))
/*---------------------------------------------------------------------------------------------------------------------------------*/

/*
** �ֿذ�(ͨѶ������bin�ļ�����)
*/
#define UpgrBinFilePara_CommAddr					(AdjusrPhaseVoltAddr + AdjusrPhaseVoltLen)
/*
** ��ذ�
*/
#define UpgrBinFilePara_BatAddr(loc)							((UpgrBinFilePara_CommAddr + UpgrBinFilePara_Len) + (loc*UpgrBinFilePara_Len))
/*
** ������̼���--ע��:����35��Դ�������洢��ع̼������͸���
*/
#define UpgrBinFilePara_ChargeAddr(loc)						(((UpgrBinFilePara_CommAddr + UpgrBinFilePara_Len) + (UpgrBatMemoryNum*UpgrBinFilePara_Len)) + (loc*UpgrBinFilePara_Len))
/*
** bin�ļ���ز�������--20210220:�޸�bin�ļ��洢�ֽ���-->�ɽṹ��: (sizeof(UpgrFilePara) + sizeof(uint16)) -- 65Byte ת��  (sizeof(ModifyEepromLogic) + sizeof(uint16)) -- 16 Byte
*/
#define UpgrBinFilePara_Len							(sizeof(ModifyEepromLogic) + sizeof(uint16))/*CRC16У��*/

/*
** Ӧ�ò��ַ����˵��:
** 		1.��ʼ��ַ:Ԥ����������44�ֵ�� 
**		�޸Ĳ���:	UpgrBinFilePara_Len * (UpgrBatMemoryNum(��ع̼���) + UpgrChargeMemoryNum(������̼���) + 1) (ͨѶ��) + ���ư�(0xB4) + 
** 											regLog(3) + batSize(4) + 216(����,�豸ID,Wifi����) 
**											+ UpgrBatMemoryNum * 4 * 2 + chargeSize(4)= 3332 + 280 = 0x0D04 + 280 = 3612 = 0x0E1C
**											+ UpgrChargeMemoryNum * 4 * 2 -->�޸�ʱ��:20210220 --> 0x0E1C + 72 = 0x0E64
---------------------------------------------------------------------------------------------------------------------------------------------
** ����ӳ�䷽��:
**		�޸Ĳ���:	UpgrBinFilePara_Len * (UpgrBatMemoryNum(��ع̼���) + UpgrChargeMemoryNum(������̼���) + 1) (ͨѶ��) + ���ư�(0xB4) + 
** 											regLog(3) + batSize(4) + 216(����,�豸ID,Wifi����) 
**											+ UpgrBatMemoryNum * 4 * 2 + chargeSize(4)
**											+ UpgrChargeMemoryNum * 4 * 2 -->�޸�ʱ��:20210220 = 1470 = 0x05BE
											+ AdjusrPhaseVoltLen--(29Byte) = 1508 = 0x05E4
					-- 20210324--������:ƫ�Ƶ�ַ--��+ sizeof(uint16) = 150A = 0x05E6		 								
*/
#define App_InitBaseAddr										0x05E6

#define TSave_Addr												App_InitBaseAddr
#define TSave_Len													(sizeof(Tsave)+ sizeof(uint16))/*CRC16У��*/
#define SocLimit_Addr												TSave_Addr + TSave_Len
#define SocLimit_Len												(sizeof(uint8) + sizeof(uint16))
#define ChgTimeLimit_Addr										SocLimit_Addr + SocLimit_Len
#define ChgTimeLimit_Len										(sizeof(uint16) + sizeof(uint16))
#define ChgOverTempLimit_Addr								ChgTimeLimit_Addr + ChgTimeLimit_Len
#define ChgOverTempLimit_Len									(sizeof(uint16) + sizeof(uint16))
#define ChgDoorOTLimit_Addr									ChgOverTempLimit_Addr + ChgOverTempLimit_Len
#define ChgDoorOTLimit_Len									(sizeof(uint16) + sizeof(uint16))
/*
** �����������ֵ
*/
#define ChargerOCLimit_Addr									(ChgDoorOTLimit_Addr + ChgDoorOTLimit_Len)	
#define ChargerOCLimit_Len										(sizeof(uint8)+sizeof(16))
/*----------------���񽻻�Э��V1.05--------------*/
#define BatOTempLimit_Addr									(ChargerOCLimit_Addr + ChargerOCLimit_Len)
#define BatOTempLimit_Len										(sizeof(int8) + sizeof(uint16))
#define NanduLowPLimit_Addr									(BatOTempLimit_Addr + BatOTempLimit_Len)
#define NanduLowPLimit_Len									(sizeof(int8) + sizeof(uint16))
#define BatLowPLimit_Addr										(NanduLowPLimit_Addr + NanduLowPLimit_Len)
#define BatLowPLimit_Len										(sizeof(int8) + sizeof(uint16))
/*-------------------------------------------------*/


/****����:Ŀǰ����������ַ���ݽ��洢��Backup*********************/
typedef struct{
	uint8 key[6];
}UpgradeKey;

/*
** ϵͳ����֮���ر����������֮�ṹ������
*/
#pragma pack(1)
typedef struct{
	uint16 ctr;/*���ư����а汾��*/
	uint16 comFile;/*�ⲿFlash�б����ͨѶ���ļ�����汾��*/
	uint16 comApp[SysCtr_AllDoorNum];/*ͨѶ������App�汾��--�ֿذ汾��,��ذ汾*/
	uint16 upgrFailNumAcc[SysCtr_AllDoorNum];/*ͳ������ʧ���ۼƴ���--���ڼ�¼�ֿ���Թ̼�����ʧ�ܵĴ���ͳ��*/
}NewSoftVer;

typedef struct{
	uint16 base;/*ϵͳ�¶Ȼ�׼ֵ*/
	uint16 diff;/*ϵͳ�ز��¶�*/
}Tsave;

/*
** ����
*/
typedef struct{
	uint8 local[64];
	uint8 remote[64];
}SysDomain;

typedef struct{
	bool registerFlag;/*�����ע���Ƿ�ɹ�*/
}Serve;

typedef struct{
	uint8 name[32];
	uint8 pw[32];
}WifiSet;

typedef struct{
	uint16 fireSzie;
}ChargeSaveInfo;/*������洢��Ϣ*/

typedef struct{
	int16 bat[UpgrBatMemoryNum];/*ƽ̨���ƴ���--���*/
	uint16 exceBat[UpgrBatMemoryNum];/*ʵ����������--���*/
	int16 charger[UpgrChargeMemoryNum];/*ƽ̨���ƴ���--�����*/
	uint16 exceCharger[UpgrChargeMemoryNum];/*ʵ����������--�����*/
}FireUpgrNumLimit;/*�̼���������*/

typedef struct{
	bool bat[UpgrBatMemoryNum];/*true:�����Ӧ�̼�--֮���*/
	bool charger[UpgrChargeMemoryNum];/*true:�����Ӧ�̼�--֮�����*/
}ClearFire;/*�����Ӧ�̼���־*/

typedef struct{
	bool bat[UpgrBatMemoryNum];/*true:���Ʒֿؽ������ģʽ--֮���*/
	uint16 batUpgrNum[UpgrBatMemoryNum];/*����ģʽ�µ�ع̼���������--20210224�޸�Ϊ:�ɹ�����*/
	uint16 batUpgrNumFail[UpgrBatMemoryNum];/*����ģʽ�µ�ع̼���������--20210224�޸�Ϊ:ʧ�ܴ���*/
	bool charger[UpgrChargeMemoryNum];/*true:���Ʒֿؽ������ģʽ--֮�����*/
	uint16 chargerUpgrNum[UpgrChargeMemoryNum];/*����ģʽ�³������������--20210224�޸�Ϊ:�ɹ�����*/
	uint16 chargerUpgrNumFail[UpgrChargeMemoryNum];/*����ģʽ�³������������--20210224�޸�Ϊ:ʧ�ܴ���*/
}FireTestModel;/*�̼�����ģʽ*/

typedef struct{
	NewSoftVer newSoftVer;/*����汾�ſ���*/
	uint8 socLimit;/*soc��ֵ*/
	uint16 chgTimeOut;/*���ʱ�䳬ʱ*/
	uint16 chgOverTemp;/*���������*/
	uint16 chgDoorOTemp;/*���ֹ���*/
	Tsave tsave;/*ϵͳ�¶Ȳ���--��׼�¶�/�ز�*/
	uint16 batFireSize;/*��ذ��̼�����*/
	SysDomain sysDomain;/*ϵͳ����:����/Զ��*/
	DeviceId  deviceId;/*�豸ID*/
	Serve serve;/*����˲���*/
	WifiSet wifiSet;/*Wifi ��������*/
	ChargeSaveInfo chargeSaveInfo;/*������洢��Ϣ*/
	FireUpgrNumLimit fireUpgrNumLimit;/*�̼���������*/
	ClearFire clearFire;/*����̼�*/
	FireTestModel fireTestModel;/*�̼��������ģʽ*/
	uint8 chargerOCLimit;/*�����������ֵ*/
	/*----------------���񽻻�Э��V1.05--------------*/
	int8 batOTempLimit;/*��ع�����ֵ*/
	int8 nanduLowPLimit;/*�϶����±�����ֵ*/
	int8 batLowPLimit;/*��ص��±���*/
	/*-------------------------------------------------*/
}SysParam;
#pragma pack()


/*
** ʹ��ָ���ṩ�ⲿ�ļ����ʱ���
*/
SysParam* get_SysParaPtr(void);
/*
** get Ctr�ļ���
*/
void get_CtrFileName(uint8* rx,uint8* rxlen);
/*
** clear ͨѶ��/��ذ�����汾��
*/
void clear_AppVer(void);
/*
** get ͨѶ��/��ذ�����App��汾��
*/
uint16 getComRunAppVer(uint8 addr);
/*
** set ͨѶ��/��ذ�����App��汾��
*/
void setComRunAppVer(uint16 ver,uint8 addr);
/*
** ͳ�����������Ѵ��ۼƲ������ٴν�������ģʽ
*/
void set_statisAccUpgrNum(uint16 numFlag,uint8 addr);
/*
** get Ctr SoftVer
*/
uint16 get_CtrSoftVer(void);
/*
** ���¿��ư����/ͨѶ������汾��
*/
void updateSoftVerPara(void);
/*
** set ������Կ����
*/
void set_UpgrCtrKey(void);
/*
** get Ctr����汾��
*/
uint32 get_CtrVer(void);
/*
** default ��ذ���Ϣ
*/
DetailedInfo default_DetailedInfo(uint16 loc);
/*
** get��ذ��洢���еĴ�С(����)
*/
uint16 get_SizeBatFile(void);
/*
** get ��ع̼�������
** 		����:�����ڲ�ѯ��ذ�����
*/
uint16 get_batFireSize(void);
/*
** set��ذ��洢���еĴ�С(����)
*/
void set_SizeBatFile(uint16 szie);
/*
** ��ʼ����ذ�(Ӳ���汾��,ID)����
*/
void init_BatPara(void);
/*
** set����bin�ļ�����
** ����:���ݵ�ذ���ϸ��Ϣ(Ӳ���汾/ID)�洢ָ����ַλ��
*/
void set_UpgrBinFilePara(UpgrFilePara upgr);
/*
** set Assign��ذ�bin�ļ�����
*/
void set_AssignUpgrBinFilePara(UpgrFilePara upgr,int16 loc);
/*
** get����bin�ļ�����--��ذ�
** @para:
**		loc:ָ��λ��--(0,max(get_SizeBatFile()))
*/
UpgrFilePara get_UpgrBatFilePara(uint16 loc);
/*
** get����bin�ļ�����--ͨѶ��
*/
UpgrFilePara get_UpgrComFilePara(void);
/*
** get��ذ�λ����Ϣ
** @return:
** 			-1:�޵�ذ���Ϣ--�Ƿ����
**			-2:������ʧ
** ����:
**		1.���������ж��Ƿ���ڵ�ذ���Ӧ�ͺ�����
**		2.��λ����Ϣ��ȡEeprom��ַ��Ϣ
*/
int16 get_BatLocation(UpgrFilePara upgr);
/*****************************************������̼���*********************************************/
/*
** get ������̼�������
*/
uint16 get_ChargeFireSize(void);
/*
** set ������̼�������
*/
void set_ChargeFireSize(uint16 size);
/*
** default ������̼�����Ϣ
*/
DetailedInfo default_ChargeDetailedInfo(uint16 loc);
/*
** get ����bin�ļ�����--������̼���
** @param:
** 			loc:ָ��λ��--[0,max(get_ChargeFireSize())
*/
UpgrFilePara get_UpgrChargeFilePara(uint16 loc);
/*
** ��ʼ��������̼���
*/
void init_chargePara(void);
/*
** set Assign ������̼���bin�ļ�����
*/
void set_AssignUpgrChargeBinFilePara(UpgrFilePara upgr,uint16 loc);
/*
** get������̼���λ����Ϣ
*/
int16 get_ChargeLocation(UpgrFilePara upgr);
/**************************************************************************************************/
/*
** set ϵͳ�¶�ֵ(��׼+�ز�)
*/
void set_Tsave(Tsave ts);
/*
** get ϵͳ�¶�ֵ(��׼+�ز�)
*/
Tsave get_Tsave(void);
/*
** set SOC��ֵ
*/
void set_SocLimit(uint8 soc);
/*
** get SOC��ֵ
*/
uint8 get_SocLimit(void);
/*
** set ���ʱ����ֵ
*/
void set_ChgTimeLimit(uint16 time);
/*
** get ���ʱ����ֵ
*/
uint16 get_ChgTimeLimit(void);
/*
** set �����������ֵ
*/
void set_ChgOverTempLimit(uint16 temp);
/*
** get �����������ֵ
*/
uint16 get_ChgOverTempLimit(void);
/*
** set ������ֹ�����ֵ
*/
void set_ChgDoorOTLimit(uint16 temp);
/*
** get ������ֹ�����ֵ
*/
uint16 get_ChgDoorOTLimit(void);
/*-----------------------------------�����������ֵV1.5-----------------------------------------------------*/
/*
** set Charger OC Limit
*/
void set_ChargerOCLimit(uint8 limit);
/*
** get Charger OC Limit
*/
uint8 get_ChargerOCLimit(void);
/*-------------------------------------------------------------------------------------------------------*/
/*
** set ����
*/
void set_SysDomain(SysDomain s);
/*
** get ����
*/
SysDomain get_SysDomain(void);
/*
** set Sys �豸ID
*/
void set_SysDeviceId(DeviceId id);
/*
** get Sys �豸ID
*/
DeviceId get_SysDeviceId(void);
/*
** get ����˲���
*/
Serve get_ServePara(void);
/*
** set����˲���
*/
void setServePara(Serve s);
/*
** get WIFI Name PW
*/
WifiSet get_WifiNamePwSysSet(void);
/*
** set Wifi Name  
*/
void set_WifiNameSysSet(uint8* space);
/*
** set Wifi Pw 
*/
void set_WifiPwSysSet(uint8* space);
/**********************��������������ʼ�2020/12/22--�ܿع̼��Ȳ���***********************************************************/
/*
** set Fire Upgr Num
*/
void set_FireUpgrNum(uint8 type,int16 num,uint16 loc);
/*
** get Fire Upgr Num
*/
int16 get_FireUpgrNum(uint8 type,uint16 loc);
/*
** set Fire Reality Run Num
*/
void set_FireRealityRunNum(uint8 type,uint16 num,uint16 loc);
/*
** get Fire Reality Run Num
*/
uint16 get_FireRealityRunNum(uint8 type,uint16 loc);
/*
** set Clear Fire
*/
void set_ClearFire(uint8 type,bool flag,uint16 loc);
/*
** get Clear FIre
*/
bool get_ClearFire(uint8 type,uint16 loc);
/*
** set Fire Enter Test model
*/
void set_FireEnterTestModel(uint8 type,bool flag,uint16 loc);
/*
** get Fire Enter Test model
*/
bool get_FireEnterTestModel(uint8 type,uint16 loc);
/*
** set Test Model Fire Upgr Num
*/
void set_TestModelFireUpgrNum(uint8 type,uint16 num,uint16 loc);
/*
** get Test Model Fire Upgr Num
*/
uint16 get_TestModelFireUpgrNum(uint8 type,uint16 loc);
/*
** set Test Model Fire Upgr Num -- ʧ�ܴ���
*/
void set_TestModelFireUpgrNumFail(uint8 type,uint16 num,uint16 loc);
/*
** get Test Model Fire Upgr Num -- ʧ�ܴ���
*/
uint16 get_TestModelFireUpgrNumFail(uint8 type,uint16 loc);
/*****************************************************************************************************************************/
/*----------------------------���ų���-��������������--��˽ӿ�(�ⲿ���)-1.05---------------------------------------------*/
/*
** ���õ�ع�����ֵ
*/
void set_BatOTempLimit(int8 temp);
/*
** ��ȡ��ع�����ֵ
*/
int8 get_BatOTempLimit(void);
/*
** �����϶���ص��±���
*/
void set_NanduLowPLimit(int8 temp);
/*
** ��ȡ�϶���ص��±���
*/
int8 get_NanduLowPLimit(void);
/*
** ���õ�ص��±���
*/
void set_BatLowPLimit(int8 temp);
/*
** ��ȡ��ص��±���
*/
int8 get_BatLowPLimit(void);
/*---------------------------------------------------------------------------------------------------------------------------*/
/*
** ϵͳ������ʼ��
*/
void init_SysPara(void);
#endif

