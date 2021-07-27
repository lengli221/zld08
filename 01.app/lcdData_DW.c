#include "includes.h"

/*
** ʹ��ָ�뷽ʽ��������ģ��(�ļ�����)
*/
UpperLayerPara* lcdULPara = null;
LowerLayerParam* lcdLLPara = null;

/*
** ҳ��ID��ز���
** 1.������תҳ��ID
** 2.��ȡ��ǰҳ��ID
*/
static LCD_SCREENID lcdScreenIdSet;
static LCD_SCREENID lcdScreenGetCurId;

/*
** ҳ���������
** 1.��ҳ��֮��ת�����Ϣ��ַ
** 2.ϵͳҳ�����
*/
static LcdMainJumpPara lcdMainJumpPara;
static LcdAdminPw lcdAdminPw;
static LcdNewSysPara lcdNewSysPara;

/*
** ��ʼ��LCD��������
*/
void init_LcdPara(void){
	lcdULPara = getUpperLayerPara();
	lcdLLPara = getLowerLayerParaPtr();
}

/*
** ҳ���ַ����
*/
uint8 res;
LCD_CTRLFRAME LCD_CtrlAddrSheets[] = {
	{0x03,&res},
	{0x04,&res},
};
uint8 const  LCD_CtrlAddrSheetsNum = sizeof(LCD_CtrlAddrSheets) / sizeof(LCD_CTRLFRAME);

/*
** ҳ���ַ������
*/
LCD_DATAFRAME LCD_DataAddrSheets[] = {
	{LCD_ADDR_ReadId,&lcdScreenGetCurId.id},
	{LCD_Addr_JumpBatInfo,&lcdMainJumpPara.doorAddr},
	{LCD_Addr_JumpInputPW,&lcdMainJumpPara.inputPw},
	{LCD_ADDR_UpdatePwInputAddr,&lcdAdminPw.ascii[0]},
	{LCD_ADDR_UpdatePwInputAddr + 1,&lcdAdminPw.ascii[1]},
	{LCD_ADDR_UpdatePwInputAddr + 2,&lcdAdminPw.ascii[2]},
	{LCD_ADDR_UpdatePwInputAddr + 3,&lcdAdminPw.ascii[3]},
	{LCD_ADDR_UpdateOkBtnAddr,&lcdAdminPw.okBtn},
	{LCD_Addr_RegisterInfoAddr,&lcdNewSysPara.sysCtr._register},
	{LCD_Addr_RegisterInfoAddr+1,&lcdNewSysPara.sysCtr.registerState},
	{LCD_Addr_LogoutInfoAddr,&lcdNewSysPara.sysCtr.logout},
	{LCD_Addr_WifiIsOkBtn,&lcdNewSysPara.wifiPara.btnIsOk},
};
uint16 const LCD_DataAddrSheetsNum = sizeof(LCD_DataAddrSheets) / sizeof(LCD_DATAFRAME);

/*
** ��ȡ��ǰҳ��ID����
*/
uint16 LCD_GetScreenID(void){
	return lcdScreenGetCurId.id;
}

/*
** ������תҳ��ID����
*/
bool LCD_SetScreenID(uint16 screenId){
	lcdScreenIdSet.id = screenId;
	return true;
}

/*
** ��ȡ����ҳ��ID����
*/
void LCD_GetScreenIdSet(uint8*pdata){
	*pdata++ = lcdScreenIdSet.id>>8;
	*pdata++ = lcdScreenIdSet.id;
}

/*
** ҳ����ת���ò���֮����ֶ���
*/
void LCD_GetScreenID_II(uint8 *pPara){
	pPara[0] = 0x5A;
	pPara[1] = 0x01;
	LCD_GetScreenIdSet((uint8 *)(pPara+2));
}

/*
** ����ҳ����ת����
*/
bool LCD_JumpScreen(void *pData, uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	
	if (pData == null)
		return false;
	
	st_FramePara->iAddress = LCD_ADDR_WriteId;										
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_ADDR_Len_WriteId;
	LCD_GetScreenID_II(st_FramePara->ucDataSpace);

	return true;
}

/*
** ��ȡ��ǰҳ��ID
*/
bool LCD_ReadScreenID(void *pData, uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	
	if(pData == null)
		return false;
	
	st_FramePara->iAddress = LCD_ADDR_ReadId;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_ADDR_Len_ReadId;

	return true;
}

/*
** ר������������ʾ����
*/
void clear_ErrShow(uint8* space,uint8 len,uint8 dataLen){
	if(dataLen > len){
		memset((uint8*)&space[len],' ',dataLen-len);
	}
}

/*
** LCD��ʾ��������
*/
//#define LCD_ShowTestData						

/*
** ��ҳ��״̬����
*/
static uint8 writeMainStatePara(uint8* space){
	uint8 index = 0;
	uint8 len = 0;
	char online[][4] = {{"����"},{"����"}};
	char state[][4] = {{"�쳣"},{"����"}};
	char fan[][4] = {{"����"},{"�ر�"}};
	__IO uint32 txtPtr = 0; 
		
	/*
	** �ղ� ��� ���� ���� �ڲ�ͨѶ״̬ ������״̬ ���ȿ���״̬ ����ID ������ֻ���
	*/
	len = _itoa(lcdULPara->stateInfoChange.doorTypeNum.idle, (char *)&space[index]);
	index += 2/*len*/;
	len = _itoa(lcdULPara->stateInfoChange.doorTypeNum.ing, (char *)&space[index]);
	index += 2/*len*/;
	len = _itoa(lcdULPara->stateInfoChange.doorTypeNum.full, (char *)&space[index]);
	index += 2/*len*/;	

	/*����1Word*/
	index += 2/*len*/;
	
	txtPtr = lcdULPara->sysFire.state.bits.isOnline == true?(uint32)&online[0][0]:(uint32)&online[1][0];
	memcpy((uint8*)&space[index],(uint8*)txtPtr,4);
	index += 4;
	txtPtr = lcdULPara->sysFire.state.bits.interCom == true?(uint32)&state[0][0]:(uint32)&state[1][0];
	memcpy((uint8*)&space[index],(uint8*)txtPtr,4);
	index += 4;
	txtPtr = lcdULPara->sysFire.state.bits.smoke == true?(uint32)&state[0][0]:(uint32)&state[1][0];
	memcpy((uint8*)&space[index],(uint8*)txtPtr,4);
	index += 4;	
	txtPtr = lcdULPara->sysFire.state.bits.fan == true?(uint32)&fan[0][0]:(uint32)&fan[1][0];
	memcpy((uint8*)&space[index],(uint8*)txtPtr,4);
	index += 4;	
	memcpy((uint8*)&space[index],(uint8*)&lcdULPara->sysSet.id[0],16);
	index += 16;	
	len = strAddRate(lcdULPara->sysFire.chgDoorTemp, (char *)&space[index], 10);
	clear_ErrShow((uint8 *)&space[index], len, 4);
	index += 4;

	return index;
}

/*
** д��ҳ��״̬����
*/
bool lcd_WriteMainStatePara(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}

	st_FramePara->iAddress = LCD_Addr_WriteMainState;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = writeMainStatePara((uint8 *)&st_FramePara->ucDataSpace[0])/sizeof(uint16);

	return true;
}

#ifdef LCD_ShowTestData
/* 
** ���Դ�������
*/
void testCodeData(uint8 doorAddr){
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.soc = 98;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.realVol = 56000;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.realCur = 4500;
	lcdLLPara->batDoor[doorAddr].batDoorSysPara.insertTime = 400;
}
#endif

/*
** �������ݴ���
*/
static uint8 singleIconPara(uint8* space,uint8 doorAddr){
	DoorNumDefine temp = (DoorNumDefine)0x01;
	uint8 icon = 0;
	uint8 index = 24;/*�������ݳ���*/
	uint8 len = 0;
	uint8 hour = 0;
	uint8 min = 0;
	uint16 realCurTemp = 0;

	/*
	** ��ʼ����������
	*/
	memset((uint8*)&space[0],0x00,index);

#ifndef LCD_ShowTestData	
	/*
	** ICONͼ�괦���߼�
	*/
	if((lcdULPara->stateInfoChange.sysLogic.comIsOnline >> doorAddr)&temp){
		icon = 0;/*�ֿذ�������*/
	}else if((lcdULPara->stateInfoChange.sysLogic.comUpgr >> doorAddr)&temp){
		icon = 4;/*�ֿذ�������*/
	}else if((lcdULPara->stateInfoChange.sysLogic.batFileDownload >> doorAddr)&temp){
		icon = 8;/*�ֿ�������*/
	}else if((lcdULPara->stateInfoChange.sysLogic.batUpgr >> doorAddr)&temp){
		icon = 5;/*���������*/
	}else if((lcdULPara->stateInfo.batOnline_1minNoCur >> doorAddr)&temp){
		icon = 6;/*���������*/
	}else if((lcdULPara->stateInfoChange.sysLogic.batIsErr >> doorAddr)&temp){
		icon = 10;/*��ع��¹���*/
	}else if((lcdULPara->stateInfoChange.sysLogic.batFault >> doorAddr)&temp){ 
		icon = 11;/*��ع���*/
	}else if((lcdULPara->stateInfoChange.sysLogic.comFault >> doorAddr)&temp){
		icon = 7;/*�ֿع���*/
	}else if((lcdULPara->stateInfoChange.sysLogic.chgOverTemp >> doorAddr)&temp){
		icon = 12;/*��������¹���*/
	}else if((lcdULPara->stateInfoChange.sysLogic.batChgOTime >> doorAddr)&temp){
		icon = 13;/*������ʱ�����*/
	}else{
		if(lcdLLPara->batDoor[doorAddr].batDoorStateInfo.bits.batOnline == true){/*�������*/
#endif
			#ifdef LCD_ShowTestData
				/*
				** ��������
				*/
				testCodeData(doorAddr);
			#endif
			
			/*
			** ��ʱ���������ſͻ� -- �����Դ����Ժ���
			*/
			if(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.soc <= 20){
				icon = 1;
			}else if(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.soc < get_SocLimit()){
				icon = 2;
			}else{
				icon = 3;
			}
			/*
			** ͼ�� ��ѹmv ����mA SOC ����ʱ��
			*/
			index = 0;/*��ʼ������ռ�ƫ��ֵ*/
			space[index++] = 0;
			space[index++] = icon;
			len = strAddRate(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.realVol,(char*)&space[index], 1000);
			clear_ErrShow((uint8 *)&space[index], len, 6);
			index += 6;/*3 word*/
			realCurTemp = (uint16)lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.realCur;/*������ת����:������ʾ*/
			if(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.realCur < 0){
				realCurTemp = ~realCurTemp + 1;
			}
			len = strAddRate(realCurTemp,(char*)&space[index], 1000);
			clear_ErrShow((uint8 *)&space[index], len, 6);
			index += 6;/*3 word*/
			len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.soc, (char*)&space[index]);
			clear_ErrShow((uint8 *)&space[index], len, 4);
			index += 4;/*2 word*/
			/*
			** ��ʾʱ�� ����:5:30-->5Сʱ30�� �����ʾʱ��:99:59
			*/
			if(lcdLLPara->batDoor[doorAddr].batDoorSysPara.insertTime > 5999){/*���ʱ��:99ʱ59��*/
				lcdLLPara->batDoor[doorAddr].batDoorSysPara.insertTime = 5999;
			}
			hour = (uint8)(lcdLLPara->batDoor[doorAddr].batDoorSysPara.insertTime/60);
			min = (uint8)(lcdLLPara->batDoor[doorAddr].batDoorSysPara.insertTime%60);
			len = _itoa(hour, (char*)&space[index]);/*�����ʾʱ��:6 Byte --> 3Word*/
			index += len;
			space[index] = ':';
			index += sizeof(uint8);
			len = _itoa(min, (char*)&space[index]);
			index += len;
			clear_ErrShow((uint8 *)&space[index], len, 6);
			
			/*��������ƫ��*/
			index = 24;
#ifndef LCD_ShowTestData			
		}else{/*��ز�����*/
			icon = 9;/*ͨѶ���������޵��*/		
		}
	}
#endif
	/*
	** ���Ӹ���״̬Icon״̬���л�
	*/
	space[0] = 0;space[1] = icon;
	
	return index;
}

/*
** �������ݴ���
*/
static uint8 groupIconPara(uint8* space,uint8 dev){
	uint8 startIndex = dev*8;
	uint8 endIndex = (dev+1)*8;
	uint8 len = 0;
	uint8 i = 0;
	uint8 *ptr = space;
	

	for(i = startIndex;i < endIndex;i++){
		ptr = space;
		ptr += len;
		len += singleIconPara(ptr, i);
	}
	return len;
}

/*
** д��ҳ��ICON��ز���
** icon��ز���֡���ݳ���--��Ϊ6�鴦��
*/
bool lcd_WriteMainIconPara(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
	
	st_FramePara->iAddress = LCD_Addr_WriteMainGroup0Icon + dev*0x0100;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = groupIconPara((uint8 *)&st_FramePara->ucDataSpace[0], dev)/sizeof(uint16);
	
	return true;
}

/*
** ��ȡ�����Ϣҳ���ַ
*/
uint16 getLcdBatInfoDoorAddr(void){
	return lcdMainJumpPara.doorAddr;
}

/*
** ��������Ϣҳ���ַ
*/
void clearLcdBatInfoDoorAddr(void){
	lcdMainJumpPara.doorAddr = 0;
}

/*
** д��ҳ����ת--�豸��ַ����
*/
bool lcd_WriteMainJumpBatInfo(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}	

	st_FramePara->iAddress = LCD_Addr_JumpBatInfo;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 1;	

	LCD_GetData(st_FramePara);

	return true;
}

/*
** ����ҳ����ת--�豸��ַ����
*/
bool lcd_ReadMainJumpBatInfo(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}	

	st_FramePara->iAddress = LCD_Addr_JumpBatInfo;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 1;	

	return true;
}

/*
** get ���������ַ����
*/
uint16 get_InputPwPara(void){
	return(lcdMainJumpPara.inputPw);
}

/*
** clear ���������ַ����
*/
void clear_InputPwPara(void){
	lcdMainJumpPara.inputPw = 0;
}

/*
** дҳ����ת--���������ַ
*/
bool lcd_WriteMainInputPw(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}	

	st_FramePara->iAddress = LCD_Addr_JumpInputPW;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 1;	

	LCD_GetData(st_FramePara);

	return true;
}

/*
** ��ҳ����ת--���������ַ
*/
bool lcd_ReadMainInputPw(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}	

	st_FramePara->iAddress = LCD_Addr_JumpInputPW;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 1;	

	return true;
}

#ifdef LCD_ShowTestData
/*
** ��������
*/
void batBaseInfo(uint8 doorAddr){
	lcdLLPara->batDoor[doorAddr].batDoorStateInfo.bits.batOnline = true;
	memset((uint8*)&lcdULPara->sysSet.fireId[0],'A',10);
	memset((uint8*)&lcdULPara->sysSet.id[0],'B',16);
	memset((uint8*)&lcdULPara->sysSet.ctrHareVer[0],'C',4);
	memset((uint8*)&lcdLLPara->batDoor[doorAddr].batDoorSysPara.comHareVer[0],'D',4);
	memset((uint8*)&lcdULPara->sysSet.ctrSoftVer[0],'E',4);
	memset((uint8*)&lcdLLPara->batDoor[doorAddr].batDoorSysPara.comSoftVer[0],'F',4);
	lcdLLPara->batDoor[doorAddr].batDoorSysPara.chgTime = 400;
	lcdLLPara->batDoor[doorAddr].batDoorSysPara.insertTime = 500;	
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.soc = 99;	
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.bootVer = 0x01;	
	memset((uint8*)&lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batID[0],'G',16);
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.firmCode = 0x02;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.hardVer = 200;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.softVer = 300;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.designCap = 4000;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batType = 0x02;
}
#endif

/*
** �ߵ��ֽڽ��洦��
*/
void temp_Cross(uint8* space){
	uint8 temp = 0;
	temp = space[1];
	space[1] = space[0];
	space[0] = temp;
}

/*
** ���ֵ�ػ�����Ϣ(��������Ϣ+��ػ�����Ϣ)
*/
uint8 writeBatInfoBase(uint8* space,uint8 doorAddr){
// 	char bootVer[12] = {"����Boot"};
	char batFireId[][8] = {{"������"},{"������"},{"��ë��"},{"�϶�"},{"���ն���"},{"�Ǻ�"},{"ATL"},{"CATL"}};
	char batCore[][20] = {{"LG M26"},{"LGMH1"},{"����INR18650-29E"},{"����ZM2"},{"��γ"},{"�ȿ�"},{"����*��Ԫ18650"},{"�������"},{"�����"}};
	__IO uint32 txtPtr = 0;
	uint8 tempp[20] = {0};
	uint8 index = 0;
	uint8 len = 0;
	uint32 temp = 0;
	uint16 dataCross = 0;

	/*
	** ��������
	*/
	memset((uint8*)&space[0],' ',134/*130*//*118*/);
	memset((uint8*)&tempp[0],' ',20);
#ifdef LCD_ShowTestData
	/*
	** ��������
	*/
	batBaseInfo(doorAddr);
#endif
	/*
	** ��������Ϣ
	*/
	/*�ֺ�*/
	len = _itoa((doorAddr+1), (char *)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 2);
	index += 2;/*1 word*/
	/*���ұ�ʶ -->string*/
	memcpy((uint8*)&space[index],(uint8*)&lcdULPara->sysSet.fireId[0],10);
	index += 10;/*5 word*/
	/*ID -->string*/
	memcpy((uint8*)&space[index],(uint8*)&lcdULPara->sysSet.id[0],16);
	index += 16;/*8 word*/
	/*Ӳ���汾 ����+�ֿ�*/
	memcpy((uint8*)&space[index],(uint8*)&lcdULPara->sysSet.ctrHareVer[0],4);
	index += 4;/*2 word*/
	memcpy((uint8*)&space[index],(uint8*)&lcdLLPara->batDoor[doorAddr].batDoorSysPara.comHareVer[0],4);
	index += 4;/*2 word*/
	/*����汾 ����+�ֿ�*/
 	memcpy((uint8*)&space[index],(uint8*)&lcdULPara->sysSet.ctrSoftVer[0],4);
 	index += 4;/*2 word*/
	memcpy((uint8*)&space[index],(uint8*)&lcdLLPara->batDoor[doorAddr].batDoorSysPara.comSoftVer[0],4);
	index += 4;/*2 word*/	
	/*������¶�*/
	len = strAddRate(lcdLLPara->batDoor[doorAddr].batDoorSysPara.chargerTemp, (char*)&space[index], 10);
	clear_ErrShow((uint8 *)&space[index], len, 6);
	index += 6;/*3 word*/	

	/*������ʾ--�����Ż���ز����߲�ѯ����汾��*/
	if(lcdLLPara->batDoor[doorAddr].batDoorStateInfo.bits.batOnline == false){/*��ز�����*/
		index = 130;
		memset((uint8*)&space[50],' ',index-50);
		goto endBatInfoBase;
	}	
	
	/*��س��ʱ��*/
	len = _itoa(lcdLLPara->batDoor[doorAddr].batDoorSysPara.chgTime, (char*)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 4);
	index += 4;/*2 word*/
	/*��ؽ���ʱ��*/
	len = _itoa(lcdLLPara->batDoor[doorAddr].batDoorSysPara.insertTime, (char*)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 4);
	index += 4;/*2 word*/	
	/*��ؽ���ʱSOC*/
	len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.soc, (char*)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 4);
	index += 4;/*2 word*/	
	/*
	** ��ػ�����Ϣ
	*/
	/*BOOT�汾��*/
// 	switch(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.bootVer){
// 		case 0x01:
// 			txtPtr = (uint32)&bootVer[0];
// 			break;
// 		default:/*��ֹ�յ�ַ�쳣*/
// 			txtPtr = (uint32)&bootVer[0];
// 			break;
// 	}
// 	memcpy((uint8*)&space[index],(uint8*)txtPtr,12);
	len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.bootVer, (char*)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 12);
	index += 12;/*6 word*/	
	/*���ID*/
	memcpy((uint8*)&space[index],(uint8*)&lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batID[0],16);
	index += 16;/*8 word*/
	/*����*/
	switch(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.firmCode){
		case 0x01:
			txtPtr = (uint32)&batFireId[0][0];
			break;
		case 0x02:
			txtPtr = (uint32)&batFireId[1][0];
			break;
		case 0x03:
			txtPtr = (uint32)&batFireId[2][0];
			break;
		case 0x04:
			txtPtr = (uint32)&batFireId[3][0];
			break;
		case 0x05:
			txtPtr = (uint32)&batFireId[4][0];
			break;
		case 0x06:
			txtPtr = (uint32)&batFireId[5][0];
			break;
		case 0x07:
			txtPtr = (uint32)&batFireId[6][0];
			break;
		case 0x08:
			txtPtr = (uint32)&batFireId[7][0];
			break;			
		default:/*��ֹ�յ�ַ�쳣*/
			txtPtr = (uint32)&tempp[0];
			break;
	}
	memcpy((uint8*)&space[index],(uint8*)txtPtr,8);
	index += 8;/*4 word*/
	/*Ӳ���汾*/
	dataCross = lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.hardVer;
	temp_Cross((uint8 *)&dataCross);
	temp = (((dataCross>>0x08)&0x00FF)*100) + (dataCross&0x00FF);
	/*temp = bcdToUInt((const uint8 *)&dataCross, sizeof(uint16));*/
	len = _itoa(temp, (char*)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 4);
	index += 4;/*2 word*/	
	/*����汾*/
	dataCross = lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.softVer;
	temp_Cross((uint8 *)&dataCross);
	temp = (((dataCross>>0x08)&0x00FF)*100) + (dataCross&0x00FF);
	/*temp = bcdToUInt((const uint8 *)&dataCross, sizeof(uint16));*/
	len = _itoa(temp, (char*)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 4);		
	index += 4;/*2 word*/	
	/*�������*/
	len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.designCap, (char*)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 8/*4*/);
	index += 8/*4*/;/*4 word*//*2 word*/		
	/*��о�ͺ�*/
	switch(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batType){
		case 0x01:
			txtPtr = (uint32)&batCore[0][0];
			break;
		case 0x02:
			txtPtr = (uint32)&batCore[1][0];
			break;
		case 0x03:
			txtPtr = (uint32)&batCore[2][0];
			break;
		case 0x04:
			txtPtr = (uint32)&batCore[3][0];
			break;
		case 0x05:
			txtPtr = (uint32)&batCore[4][0];
			break;
		case 0x06:
			txtPtr = (uint32)&batCore[5][0];
			break;
		case 0x07:
			txtPtr = (uint32)&batCore[6][0];
			break;
		case 0x10:
			txtPtr = (uint32)&batCore[7][0];
			break;
		case 0x11:
			txtPtr = (uint32)&batCore[8][0];
			break;
		default:/*��ֹ�յ�ַ�쳣*/
			txtPtr = (uint32)&tempp[0];
			break;	
	}
	memcpy((uint8*)&space[index],(uint8*)txtPtr,20);
	index += 20/*8*/;/*10 word*//*4 word*/	
endBatInfoBase:
	return index;
}

#ifdef LCD_ShowTestData
/*
** ��������
*/
void testBatRunInfo(uint8 doorAddr){
	lcdLLPara->batDoor[doorAddr].batDoorStateInfo.bits.batOnline = true;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.workState = 0x01;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.realVol = 56899;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.realCur = 5645;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.pState.flag = 0x01;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.faultState.flag = 0x01;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batMaxTemp = 150;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batMinTemp = 20;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.mosMaxTemp = 120;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.pcbTemp = 10;
}
#endif

/*
** ��ع�����Ϣ
*/
uint8 writeBatRunInfo(uint8* space,uint8 doorAddr){
	char state[][14] = {{"�ŵ�ģʽ      "},
						    {"���ģʽ      "},
						    {"����ģʽ      "},
						    {"���������ģʽ"},
						    {"����Ԥ�ŵ�ģʽ"},
						    {"�����践��"}};
	char pState[][8] = {{"�ŵ����"},{"�ŵ����"},{"������"},{"������"},{"�ŵ����"},{"�ŵ�Ƿѹ"},{"������"},{"����ѹ"},{"��·    "}};
	char faultState[][14] = {{"�¶ȴ�������"},
								 {"IC��        "},
								 {"���MOS��   "},
								 {"�ŵ�MOS��   "},
								 {"ʧ��          "},
								 {"ʧЧ          "}};
	char charShow[6] = {"δ����"};
	uint8 i = 0;
	uint16 temp = (uint16)0x01;
	__IO uint32 txtPtr = 0;
	uint8 index = 0;
	uint8 len = 0;
	uint8 tempp[20] = {0};
	int8 tempConvert = 0;
	uint16 realCurTemp = 0;

#ifdef LCD_ShowTestData
	/*
	** ��������
	*/
	testBatRunInfo(doorAddr);
#endif

	memset((uint8*)&tempp[0],' ',20);

	if(lcdLLPara->batDoor[doorAddr].batDoorStateInfo.bits.batOnline == false){/*��ز�����*/
		index = 80/*72*/;
		memset((uint8*)&space[0],0x00,index);
		goto endBatInfoRun;
	}

	/*����״̬*/
	switch(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.workState){
		case 0x01:
			txtPtr = (uint32)&state[0][0];
			break;
		case 0x10:
			txtPtr = (uint32)&state[1][0];
			break;
		case 0x21:
			txtPtr = (uint32)&state[2][0];
			break;
		case 0x30:
			txtPtr = (uint32)&state[3][0];
			break;
		case 0x31:
			txtPtr = (uint32)&state[4][0];
			break;
		case 0xFF:
			txtPtr = (uint32)&state[5][0];
			break;
		default:/*��ֹ�յ�ַ�쳣*/
			txtPtr = (uint32)&tempp[0];
			break;
	}
	memcpy((uint8*)&space[index],(uint8*)txtPtr,14);
	index += 14;/*7 word*/
	/*ʵʱ��ѹ*/
	len = strAddRate(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.realVol,(char*)&space[index], 1000);
	clear_ErrShow((uint8 *)&space[index], len, 6);
	index += 6;/*3 word*/
	/*ʵʱ����*/
	realCurTemp = (uint16)lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.realCur;/*������ת����:������ʾ*/
	if(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.realCur < 0){
		realCurTemp = ~realCurTemp + 1;
	}
	/*len = strAddRate(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.realCur,(char*)&space[index], 1000);*/
	len = strAddRate(realCurTemp,(char*)&space[index], 1000);
	clear_ErrShow((uint8 *)&space[index], len, 6);
	index += 6;/*3 word*/
	/*SOC*/
	len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.soc, (char*)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 4);
	index += 4;/*2 word*/
	/*����״̬*/
	txtPtr = 0;
	for(i=0;i<9;i++){
		if((lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.pState.flag>>i)&temp){
			txtPtr = (uint32)&pState[i][0];			
		}
	}
	if(txtPtr != 0){
		memcpy((uint8*)&space[index],(uint8*)txtPtr,8);
	}else{
		memset((uint8*)&space[index],' ',8);
	}
	index += 8;/*4 word*/
	/*����״̬*/
	txtPtr = 0;
	for(i=2;i<8;i++){
		if((lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.faultState.flag>>i)&temp){
			txtPtr = (uint32)&faultState[i-2][0];	
		}
	}
	
	if(txtPtr != 0){
		memcpy((uint8*)&space[index],(uint8*)txtPtr,14);
	}else{
		memset((uint8*)&space[index],' ',14);
	}
	index += 14;/*7 word*/	
	/*��о����¶�*/
	tempConvert =lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batMaxTemp;
	if(tempConvert == 0 || tempConvert == (int8)0xFF){
		memcpy((uint8*)&space[index],(uint8*)&charShow[0],6);
		clear_ErrShow((uint8 *)&space[index], 6, 8);
	}else{
		len = _itoa( lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batMaxTemp,(char*)&space[index]);
		clear_ErrShow((uint8 *)&space[index], len, 8);
	}
	index += 8;/*4 word*/
	/*��о����¶�*/
	tempConvert = lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batMinTemp;
	if(tempConvert == 0 || tempConvert == (int8)0xFF){
		memcpy((uint8*)&space[index],(uint8*)&charShow[0],6);
		clear_ErrShow((uint8 *)&space[index], 6, 8);
	}else{
		len = _itoa( lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batMinTemp,(char*)&space[index]);
		clear_ErrShow((uint8 *)&space[index], len, 8);
	}
	index += 8;/*4 word*/	
	/*MOS����¶�*/
	tempConvert = lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.mosMaxTemp;
	if(tempConvert == 0 || tempConvert == (int8)0xFF){
		memcpy((uint8*)&space[index],(uint8*)&charShow[0],6);
		clear_ErrShow((uint8 *)&space[index], 6, 8);
	}else{
		len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.mosMaxTemp,(char*)&space[index]);
		clear_ErrShow((uint8 *)&space[index], len, 8);
	}
	index += 8/*4*/;/*4 word*//*2 word*/	
	/*PCB����¶�*/
	tempConvert = lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.pcbTemp;
	if(tempConvert == 0 || tempConvert == (int8)0xFF){
		memcpy((uint8*)&space[index],(uint8*)&charShow[0],6);
		clear_ErrShow((uint8 *)&space[index], 6, 8);
	}else{
		len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.pcbTemp,(char*)&space[index]);
		clear_ErrShow((uint8 *)&space[index], len, 8);
	}
	index += 8/*4*/;/*4 word*//*2 word*/	
endBatInfoRun:
	return index;
}

#ifdef LCD_ShowTestData
/*
** ��������
*/
void testBatUse(uint8 doorAddr){
	lcdLLPara->batDoor[doorAddr].batDoorStateInfo.bits.batOnline = true;
	/*SOH*/
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.soh = 100;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.circNum = 123;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.chgNum = 2345;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.disChgNum = 678;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.shortNum = 567;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.hisMaxBatTemp = 54;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.surplusCap = 3434;	
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batWorkMode = 0x01;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batChgMode = 0x01;
	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.sop = 9;
}
#endif

/*
** ���ʹ��״̬
*/
uint8 writeBatUseState(uint8* space,uint8 doorAddr){
	uint8 index = 0;
	uint8 len = 0;
	char batWorkMode[][22] = {{"�����ģʽ            "},
									   {"�ⲿ�������ģʽ      "},
									   {"�ⲿ����+����ȷ��ģʽ "},
									   {"�ϻ�ģʽ              "}};
	char batChgMode[][10] = {{"��ͨѶ���"},{"ͨѶ���"}};
	__IO uint32 txtPtr = 0;
	uint8 tempp[20] = {0};
	int8 tempConvert = 0;
	char charShow[6] = {"δ����"};

#ifdef LCD_ShowTestData
	/*
	** ��������
	*/
	testBatUse(doorAddr);
#endif	

	memset((uint8*)&tempp[0],' ',20);
	if(lcdLLPara->batDoor[doorAddr].batDoorStateInfo.bits.batOnline == false){/*��ز�����*/
		index = 80;
		memset((uint8*)&space[0],0x00,index);
		goto end_2;
	}
	
	/*SOH*/
	len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.soh, (char*)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 4);
	index += 4;/*2 word*/	
	/*ѭ������*/
	if(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.circNum == 0xFFFF){
		memcpy((uint8*)&space[index],(uint8*)&charShow[0],6);
	}else{
		len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.circNum, (char*)&space[index]);
		clear_ErrShow((uint8 *)&space[index], len, 6);
	}
	index += 6;/*3 word*/	
	/*����������*/
	if(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.chgNum == 0xFFFF){
		memcpy((uint8*)&space[index],(uint8*)&charShow[0],6);
	}else{
		len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.chgNum, (char*)&space[index]);
		clear_ErrShow((uint8 *)&space[index], len, 6);
	}
	index += 6;/*3 word*/
	/*�������Ŵ���*/
	if(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.disChgNum == 0xFFFF){
		memcpy((uint8*)&space[index],(uint8*)&charShow[0],6);
	}else{
		len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.disChgNum, (char*)&space[index]);
		clear_ErrShow((uint8 *)&space[index], len, 6);
	}
	index += 6;/*3 word*/	
	/*��·����*/
	if(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.shortNum == 0xFFFF){
		memcpy((uint8*)&space[index],(uint8*)&charShow[0],6);
	}else{
		len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.shortNum, (char*)&space[index]);
		clear_ErrShow((uint8 *)&space[index], len, 6);
	}
	index += 6;/*3 word*/	
	/*��о��ʷ����¶�*/
	if(tempConvert != 0x8F){
		memcpy((uint8*)&space[index],(uint8*)&charShow[0],6);
	}else{
		len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.hisMaxBatTemp, (char*)&space[index]);
		clear_ErrShow((uint8 *)&space[index], len, 6);
	}
	index += 6;/*3 word*/		
	/*ʣ��������� mAh*/
	len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.surplusCap, (char*)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 8);
	index += 8;/*4 word*/	
	/*��ع���ģʽ*/
	switch(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batWorkMode){
		case 0x01:
			txtPtr = (uint32)&batWorkMode[0][0];
			break;
		case 0x02:
			txtPtr = (uint32)&batWorkMode[1][0];
			break;
		case 0x03:
			txtPtr = (uint32)&batWorkMode[2][0];
			break;
		case 0x04:
			txtPtr = (uint32)&batWorkMode[3][0];
			break;
		default:/*��ֹ�յ�ַ�쳣*/
			txtPtr = (uint32)&tempp[0];
			break;
	}
	memcpy((uint8*)&space[index],(uint8*)txtPtr,22);
	index += 22;/*11 word*/	
	/*��س��ģʽ*/
	switch(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.batChgMode){
		case 0x01:
			txtPtr = (uint32)&batChgMode[0][0];
			break;
		case 0x02:
			txtPtr = (uint32)&batChgMode[1][0];
			break;
		default:/*��ֹ�յ�ַ�쳣*/
			txtPtr = (uint32)&tempp[0];
			break;
	}
	memcpy((uint8*)&space[index],(uint8*)txtPtr,10);
	index += 10;/*5 word*/	
	/*SOP mA*/
	len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.sop, (char*)&space[index]);
	clear_ErrShow((uint8 *)&space[index], len, 6);
	index += 6;/*3 word*/	
end_2:
	return index;
}

/*
** ��о��Ϣ
*/
uint8 writeBatSingleVol(uint8* space,uint8 doorAddr){
	uint8 index = 0;
	uint8 i = 0;
	uint8 len = 0;
	char charShow[6] = {"δ����"};

	if(lcdLLPara->batDoor[doorAddr].batDoorStateInfo.bits.batOnline == false){/*��ز�����*/
		index = 128;
		memset((uint8*)&space[0],0x00,index);
		goto endBatUse;
	}
	
	/*�����ѹ mv-->v��ʾ*/
	for(i=0;i<16;i++){
		if(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.singleBatVol[i] != 0
			&& lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.singleBatVol[i] != 0xFFFF){
			len = _itoa(lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.singleBatVol[i], (char*)&space[index]);
			clear_ErrShow((uint8 *)&space[index], len, 8);
		}else{
			memcpy((uint8*)&space[index],(uint8*)&charShow[0],6);
			clear_ErrShow((uint8 *)&space[index], 6, 8);
		}
  		index += 8;/*4 word*/
	}

endBatUse:
	return index;
}

#ifdef LCD_ShowTestData
/*
** ��������
*/
void testBatRel(uint8 doorAddr){
	lcdLLPara->batDoor[doorAddr].batDoorStateInfo.bits.batOnline = true;

	lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.mosState.flag = 0x01;
}
#endif

/*
** ��������Ϣ
*/
uint8 writeBatRelInfo(uint8* space,uint8 doorAddr){
	uint8 index = 0;
	char ctrState[][4] = {{"�ر�"},{"����"}};
	char mosState[][14] = {{"�ŵ�MOS״̬   "},
								{"���MOS״̬   "},
								{"Ԥ�ŵ�MOS״̬ "}};
	uint8  i = 0;
	uint8 temp = (uint8)0x01;
	__IO uint32 txtPtr = 0;

#ifdef LCD_ShowTestData
	/*
	** ��������
	*/
	testBatRel(doorAddr);
#endif

	if(lcdLLPara->batDoor[doorAddr].batDoorStateInfo.bits.batOnline == false){/*��ز�����*/
		index = 54;
		memset((uint8*)&space[0],0x00,index);
		goto endBatRelInfo;
	}

	/*lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.mosState.flag = 0x0E;*/
	for(i = 1;i<4;i++){
		if((lcdLLPara->batDoor[doorAddr].bmsInfoMeiTuan.mosState.flag>>i)&temp){
			txtPtr = (uint32)&ctrState[1][0];
		}else{
			txtPtr = (uint32)&ctrState[0][0];
		}
		memcpy((uint8*)&space[index],(uint8*)txtPtr,4);
		index += 4;/*2 word*/
		memcpy((uint8*)&space[index],(uint8*)&mosState[i-1][0],14);
		index += 14;/*7 word*/
	}

endBatRelInfo:
	return index;
}

/*
** д�����Ϣ֮������Ϣ
*/
bool lcd_WriteBatInfoBase(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}	

	st_FramePara->iAddress = LCD_Addr_WriteBatInfoBaseAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = writeBatInfoBase((uint8 *)&st_FramePara->ucDataSpace[0], dev)/sizeof(uint16);// 2;//
	
	return true;
}

/*
** д�����Ϣ֮��ع���״̬
*/
bool lcd_WriteBatInfoRunState(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}

	st_FramePara->iAddress = LCD_Addr_WriteBatWorkStateAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = writeBatRunInfo((uint8 *)&st_FramePara->ucDataSpace[0], dev)/sizeof(uint16);	
	
	return true;
}

/*
** д�����Ϣ֮���ʹ��״̬
*/
bool lcd_WriteBatInfoUseState(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}

	st_FramePara->iAddress = LCD_Addr_WriteBatRunStateAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = writeBatUseState((uint8 *)&st_FramePara->ucDataSpace[0], dev)/sizeof(uint16);//5	
	
	return true;
}

/*
** д�����Ϣ֮��о��Ϣ
*/
bool lcd_WriteBatInfoVolState(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}

	st_FramePara->iAddress = LCD_Addr_WriteBatVolAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = writeBatSingleVol((uint8 *)&st_FramePara->ucDataSpace[0], dev)/sizeof(uint16);	
	
	return true;
}

/*
** д�����Ϣ֮�����Ϣ֮MOS״̬
*/
bool lcd_WriteBatInfoRefState(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}

	st_FramePara->iAddress = LCD_Addr_WriteBatRefAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = writeBatRelInfo((uint8 *)&st_FramePara->ucDataSpace[0], dev)/sizeof(uint16);	
	
	return true;
}

/*
** дһ��������Ϣ
*/
extern Hmi_PhaseVC hmi_PhaseVc;
uint8 writeFirstWaring(uint8* space){
	char waring[][12] = {{"��ع���    "},
												{"���������  "},
												{"������ֹ���"},
												{"���ʱ�����"},
												{"������    "},
												{"���������  "},
												{"�ֿع���    "},
												{"��ع���    "},
												{"�ֿ�ʧ��    "},
												{"�����ѹ����"},
												{"����Ƿѹ����"},
												{"�����������"},
												{"���׹���    "},
												{"��ͣ����		"},
	};
	__IO uint32 txtPtr = (uint32)waring[0][0];
	uint8 txlen = 0;
	uint8 len = 0;

	/*
	** ��ʱ���������
	*/
	memset((uint8*)&space[0],0x00,(12+sizeof(uint16))*13);

	if(lcdULPara->stateInfoChange.sysLogic.batIsErr != 0){/*��ع���*/
		txtPtr = (uint32)&waring[0][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;
	}

	if(lcdULPara->stateInfoChange.sysLogic.chgOverTemp != 0){/*���������*/
		txtPtr = (uint32)&waring[1][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;		
	}

	if(lcdULPara->stateInfoChange.sysModuleStateInfo.bits.chgDoorOverTemp == true){/*������ֹ���*/
		txtPtr = (uint32)&waring[2][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}

	if(lcdULPara->stateInfoChange.sysLogic.batChgOTime != 0){/*���ʱ�����*/
		txtPtr = (uint32)&waring[3][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}	

	if(lcdULPara->stateInfoChange.sysModuleStateInfo.bits.smoke == true){/*�̸й���*/
		txtPtr = (uint32)&waring[4][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}	

	if(lcdULPara->stateInfo.batOnline_1minNoCur != 0){/*���������*/
		txtPtr = (uint32)&waring[5][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}		

	if(lcdULPara->stateInfoChange.sysLogic.comFault != 0){/*�ֿع���*/
		txtPtr = (uint32)&waring[6][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}	

	if(lcdULPara->stateInfoChange.sysLogic.batFault != 0){/*��ع���*/
		txtPtr = (uint32)&waring[7][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}

	if(lcdULPara->sysFire.state.bits.interCom != 0){/*�ֿ�ʧ��*/
		txtPtr = (uint32)&waring[8][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}	

	if(hmi_PhaseVc.bits.hv == true){/*��ѹ����*/
		txtPtr = (uint32)&waring[9][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}	

	if(hmi_PhaseVc.bits.lv == true){/*Ƿѹ����*/
		txtPtr = (uint32)&waring[10][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}	

	if(hmi_PhaseVc.bits.oc == true){/*��������*/
		txtPtr = (uint32)&waring[11][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}

	if(lcdULPara->stateInfoChange.sysModuleStateInfo.bits.anti == true){/*���׹���*/
		txtPtr = (uint32)&waring[12][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}	

	if(lcdULPara->stateInfoChange.sysModuleStateInfo.bits.emer == true){/*��ͣ����*/
		txtPtr = (uint32)&waring[13][0];
		memcpy((uint8*)&space[txlen],(uint8*)txtPtr,12);
		txlen += 12;
		len = lineBreak((uint8 *)&space[txlen]);
		txlen += len;	
	}	
	
	return ((12+sizeof(uint16))*13);
}

/*
** дһ���澯ҳ��֮������Ϣ
*/
bool lcd_WriteFirstWaringInfo(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_WriteFirstWaringAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = writeFirstWaring((uint8 *)&st_FramePara->ucDataSpace[0])/sizeof(uint16);

	return true;
}

/*
** clear ���������ʾ�ı��ؼ�
*/
void clear_PwShow(void){
	memset((uint8*)&lcdAdminPw.show[0],' ',4*sizeof(uint16));
}

/*
** set ������ʾ�ı��ؼ�
*/
void set_PwAscii(void){
	memset((uint8*)&lcdAdminPw.show[0],'*',4*sizeof(uint16));
}

/*
** update ������ʾ�ؼ�
*/
void update_PwShow(uint8* space){
	memcpy((uint8*)&space[0],(uint8*)&lcdAdminPw.show[0],4*sizeof(uint16));
}

/*
** д������ʾ�ı��ؼ�
*/
bool lcd_WritePwShow(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_WriteShowPwAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 4;

	/*
	** update ������ʾ�ؼ�
	*/
	update_PwShow((uint8 *)&st_FramePara->ucDataSpace[0]);

	return true;	
}

/*
** clear ��������ؼ�
*/
void clear_PwInput(void){
	memset((uint8*)&lcdAdminPw.ascii[0] ,' ', 4*sizeof(uint16));
}

/*
** get ��������ؼ�
*/
LcdAdminPw get_PwInput(void){
	return lcdAdminPw;
}

/*
** д��������ؼ�
*/
bool lcd_WritePwInput(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_ADDR_UpdatePwInputAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 4;

	LCD_GetData(st_FramePara);

	return true;	
}

/*
** ��ȡ��������ؼ�
*/
bool lcd_ReadPwInput(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_ADDR_UpdatePwInputAddr;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 4;

	return true;		
}


/*
** clear okBtn����ֵ
*/
void clear_OkBtn(void){
	lcdAdminPw.okBtn = 0;
}

/*
** get OkBtn����ֵ
*/
uint16 get_OkBtn(void){
	return lcdAdminPw.okBtn;
}

/*
** д����ȷ�Ͽؼ�
*/
bool lcd_WriteOkBtn(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_ADDR_UpdateOkBtnAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 1;

	LCD_GetData(st_FramePara);

	return true;	
}

/*
** ��ȡ����ȷ�Ͽؼ�
*/
bool lcd_ReadPwOkBtn(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_ADDR_UpdateOkBtnAddr;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 1;

	return true;		
}

/*
** update Temp Domain -->����������ʱ����
*/
void updateTempDomain(uint8* local,uint8* remote){
	memcpy((uint8*)&local[0],(uint8*)&lcdNewSysPara.getDomain.localShow[0],64);
	memcpy((uint8*)&remote[0],(uint8*)&lcdNewSysPara.getDomain.remoteShow[0],64);
}

/*
** get Domain Legal Len
*/
uint8 getDomainIsLegalLen(uint8* space){
	uint8 i = 0;
	uint8 len = 0;

	for(i=0;i<64;i++){
		if(space[i] == 0xFF || space[i] == 0 || space[i] == ' '){
			len = i;
			break;
		}
	}

	if(len == 0){
		len = 64;
	}

	return len;
}

/*
** update Domain
*/
void update_Domain(void){
	SysDomain sysDomain = get_SysDomain();
	uint8 legalLen = 0;

	/*
	** ������������--�ѱ���ʾʹ��
 	*/
 	memset((uint8*)&lcdNewSysPara.getDomain.localShow[0],' ',64);
 	memset((uint8*)&lcdNewSysPara.getDomain.remoteShow[0],' ',64);

	/*
	** ��������
	*/
	legalLen = getDomainIsLegalLen((uint8*)&sysDomain.local[0]);
	memcpy((uint8*)&lcdNewSysPara.getDomain.localShow[0],(uint8*)&sysDomain.local[0],legalLen);

	/*
	** Զ������
	*/
	legalLen = getDomainIsLegalLen((uint8 *)&sysDomain.remote[0]);
	memcpy((uint8*)&lcdNewSysPara.getDomain.remoteShow[0],(uint8*)&sysDomain.remote[0],legalLen);
}

/*
** update local Demain Show
*/
void update_LocalDomainShow(uint8* space){
	memcpy((uint8*)&space[0],(uint8*)&lcdNewSysPara.getDomain.localShow[0],64);
}

/*
** дLocal������ʾ�ؼ�
*/
bool lcd_WriteLocalDomainShow(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_ADDR_DomainNameLocal;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 32;		

	update_LocalDomainShow((uint8*)&st_FramePara->ucDataSpace[0]);

	return true;
}

/*
** get Local Domain Input
*/
void getLocalDomainInput(void* pData,void* temp){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	
	memcpy((uint8*)&lcdNewSysPara.getDomain.localShow[0],(uint8*)&st_FramePara->ucDataSpace[0],64);	
}

/*
** get_Local Domain Input
*/
void get_LocalDomainInput(uint8* rx){
	memcpy((uint8*)&rx[0],(uint8*)&lcdNewSysPara.getDomain.localShow[0],64);
}

/*
** ��ȡlocal����input
*/
bool lcd_ReadLocalDomainInput(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_ADDR_DomainNameLocal;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 32;	

	return true;	
}

/*
** update Remote Demain Show
*/
void update_RemoteDomainShow(uint8* space){
	memcpy((uint8*)&space[0],(uint8*)&lcdNewSysPara.getDomain.remoteShow[0],64);
}

/*
** дRemote������ʾ�ؼ�
*/
bool lcd_WriteRemoteDomainShow(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_ADDR_DomainNameRemote;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 32;	

	update_RemoteDomainShow((uint8*)&st_FramePara->ucDataSpace[0]);

	return true;
}

/*
** get Remote Domain Input
*/
void getRemoteDomainInput(void* pData,void* temp){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	
	memcpy((uint8*)&lcdNewSysPara.getDomain.remoteShow[0],(uint8*)&st_FramePara->ucDataSpace[0],64);	
}

/*
** get_Remote Domain Input
*/
void get_RemoteDomainInput(uint8* rx){
	memcpy((uint8*)&rx[0],(uint8*)&lcdNewSysPara.getDomain.remoteShow[0],64);
}

/*
** ��ȡRemote����
*/
bool lcd_ReadRemoteDomainInput(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_ADDR_DomainNameRemote;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 32;	

	return true;
}


/*
** clear ע����Ϣ����ֵ
*/
void clear_RegisterBtn(void){
	lcdNewSysPara.sysCtr._register = 0;
}

/*
** set ����ע��״̬
*/
void set_RegisterState(uint16 state){
	lcdNewSysPara.sysCtr.registerState = state;
}


/*
** get ע����Ϣ����ֵ
*/
uint16 get_RegisterBtn(void){
	return lcdNewSysPara.sysCtr._register;
}

/*
** дע����Ϣȷ�Ͽؼ�
*/
bool lcd_WriteRegisterBtn(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_RegisterInfoAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 2/*1*/;

	LCD_GetData(st_FramePara);

	return true;	
}

/*
** ��ȡע����Ϣȷ�Ͽؼ�
*/
bool lcd_ReadRegisterBtn(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_RegisterInfoAddr;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 1;

	return true;		
}

/*
** clear ע����Ϣ����ֵ
*/
void clear_LogoutBtn(void){
	lcdNewSysPara.sysCtr.logout = 0;
}

/*
** get ע����Ϣ����ֵ
*/
uint16 get_LogoutBtn(void){
	return lcdNewSysPara.sysCtr.logout;
}

/*
** дע����Ϣȷ�Ͽؼ�
*/
bool lcd_WriteLogoutBtn(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_LogoutInfoAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 1;

	LCD_GetData(st_FramePara);

	return true;	
}

/*
** ��ȡע����Ϣȷ�Ͽؼ�
*/
bool lcd_ReadLogoutBtn(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_LogoutInfoAddr;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 1;

	return true;		
}

/*
** update �豸ID
*/
void update_DeviceId(DeviceId id){
	lcdNewSysPara.deviceId = id;
}

/*
** get �豸ID
*/
DeviceId get_DeviceId(void){
	return lcdNewSysPara.deviceId;
}

/*
** update To Dwin DeviceId
*/
void update_ToDwinDeviceId(uint8* space){
	memcpy((uint8*)&space[0],(uint8*)&lcdNewSysPara.deviceId.show[0],16);
}

/*
** д�豸��ַ �����ͺ� �����ͺ�
*/
bool lcd_WriteDeviceInfo(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_ADDR_DeviceID;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 8;

	/*
	** update To Dwin DeviceId
	*/
	update_ToDwinDeviceId((uint8*)&st_FramePara->ucDataSpace[0]);

	return true;
}

/*
** get From Dwin DeviceId
*/
void get_FromDeinDeviceId(void* pData,void* temp){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	
	memcpy((uint8*)&lcdNewSysPara.deviceId.show[0],(uint8*)&st_FramePara->ucDataSpace[0],16);
}

/*
** ���豸��ַ �����ͺ� �����ͺ�
*/
bool lcd_ReadDeviceInfo(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_ADDR_DeviceID;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 8;

	return true;		
}

/*
** get WifiNamePW Legal Len
*/
uint8 get_WifiNamePwIsLegalLen(uint8* space){
	uint8 i = 0;
	uint8 len = 0;

	for(i=0;i<32;i++){
		if(space[i] == 0xFF || space[i] == 0 || space[i] == ' '){
			len = i;
			break;
		}
	}

	if(len == 0){
		len = 32;
	}

	return len;
}

/*
** init WifiName To DWIN Show
*/
void init_WifiNameToDwinShow(void){
	uint8 legalLen = 0;
	WifiSet wifiSetTemp = get_WifiNamePwSysSet();

	memset((uint8*)&lcdNewSysPara.wifiPara.name[0],' ',32);
	legalLen = get_WifiNamePwIsLegalLen((uint8 *)&wifiSetTemp.name[0]);
	memcpy((uint8*)&lcdNewSysPara.wifiPara.name[0],(uint8 *)&wifiSetTemp.name[0],legalLen);
}


/*
** update wifi nane
*/
void update_WifiName(uint8* space){
	memcpy((uint8*)&space[0],(uint8*)&lcdNewSysPara.wifiPara.name[0],32);
}

/*
** дWIFI������ʾ
*/
bool lcd_WriteWifiNameShow(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_WIFIName;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 16;

	/*
	** update wifi nane
	*/
	update_WifiName((uint8*)&st_FramePara->ucDataSpace[0]);

	return true;
}

/*
** get From DWIN WIFI Name
*/
void get_FromDwinWifiName(void* pData,void* temp){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	memcpy((uint8*)&lcdNewSysPara.wifiPara.name[0],(uint8*)&st_FramePara->ucDataSpace[0],32);
}

/*
** ��WIFI��������
*/
bool lcd_ReadWifiNameInput(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_WIFIName;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 16;

	return true;		
}

/*
** init WifiPw To DWIN Show
*/
void init_WifiPwToDwinShow(void){
	uint8 legalLen = 0;
	WifiSet wifiSetTemp = get_WifiNamePwSysSet();

	memset((uint8*)&lcdNewSysPara.wifiPara.pw[0],' ',32);
	legalLen = get_WifiNamePwIsLegalLen((uint8 *)&wifiSetTemp.pw[0]);
	memcpy((uint8*)&lcdNewSysPara.wifiPara.pw[0],(uint8 *)&wifiSetTemp.pw[0],legalLen);
}

/*
** update wifi Pw
*/
void update_WifiPw(uint8* space){

	
	memcpy((uint8*)&space[0],(uint8*)&lcdNewSysPara.wifiPara.pw[0],32);
}

/*
** дWIFI������ʾ
*/
bool lcd_WriteWifiPwShow(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	/*
	** ����wifi��������
	*/
	uint8 pwLegalLen = 32;
	uint8 i = 0;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_WIFIPw + 0x40;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 16;

	/*
	** update wifi nane
	*/
	update_WifiPw((uint8*)&st_FramePara->ucDataSpace[0]);

	for(i=0;i<32;i++){
		if(lcdNewSysPara.wifiPara.pw[i] == 0 
			|| lcdNewSysPara.wifiPara.pw[i] == 0xFF
			|| lcdNewSysPara.wifiPara.pw[i] == ' '){
			pwLegalLen = i+1;
			break;
		}
	}

	memset((uint8*)&st_FramePara->ucDataSpace[0],'*',pwLegalLen);

	return true;
}

/*
** get From DWIN WIFI Pw
*/
void get_FromDwinWifiPw(void* pData,void* temp){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	memcpy((uint8*)&lcdNewSysPara.wifiPara.pw[0],(uint8*)&st_FramePara->ucDataSpace[0],32);
}

/*
** ��WIFI��������
*/
bool lcd_ReadWifiPwInput(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_WIFIPw;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 16;

	return true;		
}

/*
** дWIFIȷ�ϰ�ť
*/
bool lcd_WriteWifiOkBtn(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_WifiIsOkBtn;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = 1;

	LCD_GetData(st_FramePara);

	return true;		
}

/*
** ��WIFIȷ�ϰ�ť
*/
bool lcd_ReadWifiOkBtn(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}
		
	st_FramePara->iAddress = LCD_Addr_WifiIsOkBtn;
	st_FramePara->ucFunCode = D_DP_DataRead;
	st_FramePara->ucRegLen = 1;

	return true;		
}

