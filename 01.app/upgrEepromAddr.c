#include "includes.h"

/*
** 获取升级(版本号,文件名长度,文件名)存储EEPROM地址
*/
extern UpgrFilePara upgrComBinFilePara;
extern UpgrFilePara upgrBatBinFilePara[UpgrBatMemoryNum];
extern UpgrFilePara upgrChargeBinFilePara[UpgrChargeMemoryNum];
uint32 get_UpgrEepromAddr(UpgrFilePara upgr){
	uint32 addr = 0;
	int16 loc = 0;

	if(upgr.upgrFileType.board == 0/*容错处理*/||upgr.upgrFileType.board == ComBoradType){/*通讯板*/
		addr = UpgrBinFilePara_CommAddr;
		upgrComBinFilePara = upgr;
	}else if(upgr.upgrFileType.board == ChargeBoardType){/*充电器固件包*/
		loc = get_ChargeLocation(upgr);
		addr = UpgrBinFilePara_ChargeAddr(loc);
		upgrChargeBinFilePara[loc] = upgr;
	}else{/*电池包*/
		loc = get_BatLocation(upgr);
		addr = UpgrBinFilePara_BatAddr(loc);
		upgrBatBinFilePara[loc] = upgr;
	}

	return addr;
}

/*
** 获取电池升级(硬件版本号,电池ID) + 充电器固件包 存储EEPROM地址
** 控制策略:
** 				用于电池初始化参数获取EEPROM地址
*/
uint32 get_UpgrBatEepromAddr(uint8 loc,UpgrFilePara upgr){
	if(upgr.upgrFileType.board == ChargeBoardType){/*充电器固件包*/
		return UpgrBinFilePara_ChargeAddr(loc);
	}else{/*电池固件包*/
		return UpgrBinFilePara_BatAddr(loc);
	}
}

