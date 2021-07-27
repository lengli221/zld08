#include "includes.h"

/*
** ��ȡ����(�汾��,�ļ�������,�ļ���)�洢EEPROM��ַ
*/
extern UpgrFilePara upgrComBinFilePara;
extern UpgrFilePara upgrBatBinFilePara[UpgrBatMemoryNum];
extern UpgrFilePara upgrChargeBinFilePara[UpgrChargeMemoryNum];
uint32 get_UpgrEepromAddr(UpgrFilePara upgr){
	uint32 addr = 0;
	int16 loc = 0;

	if(upgr.upgrFileType.board == 0/*�ݴ���*/||upgr.upgrFileType.board == ComBoradType){/*ͨѶ��*/
		addr = UpgrBinFilePara_CommAddr;
		upgrComBinFilePara = upgr;
	}else if(upgr.upgrFileType.board == ChargeBoardType){/*������̼���*/
		loc = get_ChargeLocation(upgr);
		addr = UpgrBinFilePara_ChargeAddr(loc);
		upgrChargeBinFilePara[loc] = upgr;
	}else{/*��ذ�*/
		loc = get_BatLocation(upgr);
		addr = UpgrBinFilePara_BatAddr(loc);
		upgrBatBinFilePara[loc] = upgr;
	}

	return addr;
}

/*
** ��ȡ�������(Ӳ���汾��,���ID) + ������̼��� �洢EEPROM��ַ
** ���Ʋ���:
** 				���ڵ�س�ʼ��������ȡEEPROM��ַ
*/
uint32 get_UpgrBatEepromAddr(uint8 loc,UpgrFilePara upgr){
	if(upgr.upgrFileType.board == ChargeBoardType){/*������̼���*/
		return UpgrBinFilePara_ChargeAddr(loc);
	}else{/*��ع̼���*/
		return UpgrBinFilePara_BatAddr(loc);
	}
}

