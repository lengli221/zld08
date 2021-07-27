#include "includes.h"

/*
** get Flash �����洢��ַ
*/
uint32 getUpgrFlashAddr(UpgrFilePara upgr){
	uint32 flashInitAddr = Upgrade_FlashAddrInit; 
	int16 loc = 0;
	
	
	if(upgr.upgrFileType.board == 0/*�쳣����*/ || upgr.upgrFileType.board == ComBoradType){/*ͨѶ��̼���*/
		 flashInitAddr = Upgrade_FlashAddrInit; 
	}else if(upgr.upgrFileType.board == ChargeBoardType){/*������̼���*/
		loc = get_ChargeLocation(upgr);
		flashInitAddr = Upgr_FlashAddr_Charge(loc);
	}
	else{/*��ع̼���*/
		loc = get_BatLocation(upgr);
		flashInitAddr = Upgr_FlashAddr_Bat(loc);
	}

	return flashInitAddr;
}

