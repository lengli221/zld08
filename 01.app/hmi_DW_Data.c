#include "includes.h"

/*
** ҳ���ջ
*/
static int32 HMI_DW_TaskSTK[512] = {0};

/*
** ʹ��ָ����������ļ�����
*/
UpperLayerReplyPara* hmi_ULRP = null;
UpperLayerPara* hmi_ULP = null;

/*
** ��ǰ����ҳ��
*/
HMI_DW_PAGE thispage;

Hmi_PhaseVC hmi_PhaseVc;

/*
** ����Ƿ���ת�������������
*/
bool checkIsJumpInputPw(void){
	static uint8 state = 0;
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteInputPwAddr,0};
	static uint32 itick = 0;
	static uint8 cnt = 0;
	bool ret = false;

	switch(state){
		case 0:
			clear_InputPwPara();
			iParam[1] = D_LCD_CMD_WriteInputPwAddr;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			state++;
			break;
		case 1:
			iParam[1] = D_LCD_CMD_ReadInputPwAddr;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			state++;			
			break;
		case 2:
			if(get_InputPwPara() == 0x01){
				cnt++;
				if(cnt == 1){/*�״δ���,�嶨ʱ��*/
					TickOut((uint32 *)&itick, 0);
				}
				state = 0;
			}else{
				state = 1;
			}
			break;
	}

	if(cnt != 0){
		if(TickOut((uint32 *)&itick, 2000) == false){
			if(cnt >= 3){
				cnt = 0;
				state = 0;
				TickOut((uint32 *)&itick, 0);
				ret = true;
			}
		}else{/*��ʱ*/
			cnt = 0;
			state = 0;
			TickOut((uint32 *)&itick, 0);
		}
	}

	return ret;
}


/*
** ��ҳ���ַ��ת��ַ
*/
uint8 deviceAddrTemp;/*��ʱ��ַ����*/
void batInfo_JumpAddr(void){
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteStatePara,0};
	static uint8 state = 0;

	switch(state){
		case 0:
			clearLcdBatInfoDoorAddr();
			iParam[1] = D_LCD_CMD_WriteJumpDeviceAddr;
			iParam[2] = 0;	
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			state++;			
			break;
		case 1:
			iParam[1] = D_LCD_CMD_ReadJumpDeviceAddr;
			iParam[2] = 0;	
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			state++;			
			break;
		case 2:
			if(getLcdBatInfoDoorAddr() != 0){
				LCD_SetScreenID(HMI_DW_PAGE_BatInfo);
				iParam[1] = D_LCD_CMD_JumpScreen;
				iParam[2] = 0;
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				state = 0;
				/*��ʱ��ַ����*/
				deviceAddrTemp = getLcdBatInfoDoorAddr();
			}else{
				state = 1;
			}	
			break;		
	}
}

/*
** ��ҳ��������ʾ
*/
#define DoorNumVoiceTime									(uint32)780/*700ms*/
DoorNumDefine hmiUpgrIsFinshFlag;
void mainPageVoice(void){
	uint32 iParam[3] = {0x01,D_LCD_CMD_CtrVoiceInfo,0};
	static uint8 state = 0;
	static uint32 timePara = 0;
	static uint32 itick = 0;
	static uint8 deviceAddr = 0;
	static uint8 deviceAddr_1 = 0;
	static uint8 deviceAddr_2 = 0;
	static uint8 deviceAddr_3 = 0;
	uint8 i = 0;
	static bool flag = false;

// 	/*
// 	** ��������
// 	*/
// 	hmi_ULP->stateInfoChange.sysLogic.batFileDownload = 0xFFFFFFFFFFFF;
	
	switch(state){
		case 0:/*�ֿ�������*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				for(i=deviceAddr;i<SysCtr_AllDoorNum;i++){
					if((hmi_ULP->stateInfoChange.sysLogic.comUpgr&(DoorNumDefine)((DoorNumDefine)0x01<<i)) != 0){
						deviceAddr = i+1;
						iParam[2] = i;
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						flag = true;
						timePara = DoorNumVoiceTime;
						break;
					}
				}
				if(flag == false){
					timePara = 0;
					deviceAddr = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 1:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(flag == true){
					iParam[2] = _ComUpgr;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;
				}else{
					timePara = 0;
				}
				flag = false;
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 2:/*���������*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				for(i=deviceAddr_1;i<SysCtr_AllDoorNum;i++){
					if((hmi_ULP->stateInfoChange.sysLogic.batUpgr&(DoorNumDefine)((DoorNumDefine)0x01<<i)) != 0){
						deviceAddr_1 = i+1;
						iParam[2] = i;
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						flag = true;
						timePara = DoorNumVoiceTime;
						break;
					}
				}
				if(flag == false){
					timePara = 0;
					deviceAddr_1 = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 3:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(flag == true){
					iParam[2] = _BatUpgr;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;
				}else{
					timePara = 0;
				}
				flag = false;
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;	
		case 4:/*�ֿ������ļ���*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				for(i=deviceAddr_2;i<SysCtr_AllDoorNum;i++){
					if((hmi_ULP->stateInfoChange.sysLogic.batFileDownload&(DoorNumDefine)((DoorNumDefine)0x01<<i)) != 0){
						deviceAddr_2 = i+1;
						iParam[2] = i;
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						flag = true;
						timePara = DoorNumVoiceTime;
						break;
					}
				}
				if(flag == false){
					timePara = 0;
					deviceAddr_2 = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 5:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(flag == true){
					iParam[2] = _FileDownload;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;
				}else{
					timePara = 0;
				}
				flag = false;
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;	
		case 6:/*����������*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				for(i=deviceAddr_3;i<SysCtr_AllDoorNum;i++){
					if(((hmi_ULP->stateInfoChange.sysLogic.batUpgrIsOk&(DoorNumDefine)((DoorNumDefine)0x01<<i)) != 0)
						&& ((hmiUpgrIsFinshFlag&(DoorNumDefine)((DoorNumDefine)0x01<<i)) == 0)){
						hmiUpgrIsFinshFlag |=(DoorNumDefine)((DoorNumDefine)1<<i);
						deviceAddr_3 = i+1;
						iParam[2] = i;
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						flag = true;
						timePara = DoorNumVoiceTime;
						break;
					}
				}
				if(flag == false){
					timePara = 0;
					deviceAddr_3 = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 7:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(flag == true){
					iParam[2] = _BatUpgrFinsh;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;
				}else{
					timePara = 0;
				}
				flag = false;
				TickOut((uint32 *)&itick, 0);
				state = 0;
			}
			break;				
	}
}

/*
** get ע��/ע�� key
*/
void get_RegisterLogout(void){
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteRegisterBtn,0};
	static uint8 state = 0;
	static uint8 state1 = 0;
	static uint32 registerStateTick = 0;

	switch(state){
		case 0:
			TickOut((uint32 *)&registerStateTick, 0);
			clear_RegisterBtn();
			iParam[1] = D_LCD_CMD_WriteRegisterBtn;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			state++;	
			break;
		case 1:
			iParam[1] = D_LCD_CMD_ReadRegisterBtn;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			state++;			
			break;
		case 2:
			if(get_RegisterBtn() != 0){
				if(get_ServePara().registerFlag == false){/*δע����ע��,��ע��,�����ظ�ע��*/
					hmi_ULRP->registerLogic.flag = true;
					set_RegisterState(2);/*ע����*/
				}
				state = 0;
			}else{
				state = 1;
			}
			break;
	}

	/*
	** update State 
	*/
	if(TickOut((uint32 *)&registerStateTick, 0) == true){
		TickOut((uint32 *)&registerStateTick, 0);
		iParam[1] = D_LCD_CMD_WriteRegisterBtn;
		iParam[2] = 0;
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
	}

	switch(state1){
		case 0:
			clear_LogoutBtn();
			iParam[1] = D_LCD_CMD_WriteLogoutBtn;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			state1++;			
			break;
		case 1:
			iParam[1] = D_LCD_CMD_ReadLogoutBtn;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			state1++;			
			break;
		case 2:
			if(get_LogoutBtn() != 0){
				hmi_ULRP->logoutLogic.flag = true;
				set_RegisterState(4);/*ע����*/
				state1 = 0;
			}else{
				state1 = 1;
			}
			break;		
	}
}

/*
** ��ҳ���������
*/
static void HMI_DW_MainPage(void){
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteStatePara,0};

	switch(thispage.state){
		case 0:
			iParam[1] = D_LCD_CMD_WriteStatePara;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			iParam[1] = D_LCD_CMD_WriteIconPara;
			iParam[2] = thispage.state - 1;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 7:
			thispage.state = 0;
			break;
		default:
			
			break;
			
	}

	/*
	** ���һ��ҳ�澯��
	*/
	if(checkFirstPageWaring() == true){
		iParam[1] = D_LCD_CMD_JumpScreen;
		iParam[2] = 0;
		LCD_SetScreenID(HMI_DW_PAGE_Waring);
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
	}

	/*
	** ����Ƿ���ת�������������
	*/	
	if(checkIsJumpInputPw() == true){
		iParam[1] = D_LCD_CMD_JumpScreen;
		iParam[2] = 0;
		LCD_SetScreenID(HMI_DW_PAGE_InputPw);
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);		
	}

	/*
	** ��ҳ���ַ��ת��ַ
	*/
	batInfo_JumpAddr();	

	/*
	** ��ҳ��������ʾ
	*/
	mainPageVoice()	;

	/*
	** get ע��/ע�� key
	*/
	get_RegisterLogout();
}

/*
** �����Ϣҳ��(�������)
*/
extern void waringPageVoice(void);
static void HMI_DW_BatInfoPage(void){
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteBatInfoBase,0};
	uint8 doorAddr = (deviceAddrTemp - 0x01)/*��ʱ��ַ����*//*getLcdBatInfoDoorAddr()*/;

	switch(thispage.state){
		case 0:
			iParam[1] = D_LCD_CMD_WriteBatInfoBase;
			iParam[2] = doorAddr;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 1:
			iParam[1] = D_LCD_CMD_WriteBatInfoRun;
			iParam[2] = doorAddr;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;				
			break;
		case 2:
 			iParam[1] = D_LCD_CMD_WriteBatInfoUse;
 			iParam[2] = doorAddr;
 			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;				
			break;	
		case 3:
			iParam[1] = D_LCD_CMD_WriteBatInfoVol;
			iParam[2] = doorAddr;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;				
			break;	
		case 4:
			iParam[1] = D_LCD_CMD_WriteBatInfoRef;
			iParam[2] = doorAddr;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state = 0;				
			break;				
	}	

	/*
	** һ������������Ϣ
	*/
	waringPageVoice();
	
}

/*
** һ������������Ϣ
*/
void waringPageVoice(void){
	uint32 iParam[3] = {0x01,D_LCD_CMD_CtrVoiceInfo,0};	
	static uint8 state = 0;
	static uint32 timePara = 0;
	static uint32 itick = 0;
	static uint8 deviceAddr = 0;
	static uint8 deviceAddr_1 = 0;
	static uint8 deviceAddr_2 = 0;
	static uint8 deviceAddr_3 = 0;
	static uint8 deviceAddr_4 = 0;
	static uint8 deviceAddr_5 = 0;	
	static uint8 deviceAddr_6 = 0;	
	uint8 i = 0;
	static bool flag = false;
	
	switch(state){
		case 0:/*������*/
			if(TickOut((uint32 *)&itick,timePara) == true){
				if(hmi_ULP->stateInfoChange.sysModuleStateInfo.bits.smoke == true){
					iParam[2] = _SmokeWaring;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;/*3S*/
				}else{
					timePara = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 1:/*���ֹ���*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_ULP->stateInfoChange.sysModuleStateInfo.bits.chgDoorOverTemp == true){
					iParam[2] = _ChgDoorTempOT;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;/*3S*/
				}else{
					timePara = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}			
			break;
		case 2:/*����¶ȹ���*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				for(i = deviceAddr;i<SysCtr_AllDoorNum;i++){
					if((hmi_ULP->stateInfoChange.sysLogic.batIsErr&(DoorNumDefine)((DoorNumDefine)0x01<<i)) != 0){
						deviceAddr = i+1;
						iParam[2] = i;	
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						flag = true;
						timePara = DoorNumVoiceTime;
						break;
					}
				}
				if(flag == false){
					timePara = 0;
					deviceAddr = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 3:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(flag == true){
					iParam[2] = _BatTempOT;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;
				}else{
					timePara = 0;
				}
				flag = false;
				TickOut((uint32 *)&itick, 0);
				state++;				
			}
			break;
		case 4:/*���������*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				for(i = deviceAddr_1;i<SysCtr_AllDoorNum;i++){
					if((hmi_ULP->stateInfoChange.sysLogic.chgOverTemp&(DoorNumDefine)((DoorNumDefine)0x01<<i)) != 0){
						deviceAddr_1 = i+1;
						iParam[2] = i;	
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						flag = true;
						timePara = DoorNumVoiceTime;
						break;
					}
				}
				if(flag == false){
					timePara = 0;
					deviceAddr_1 = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 5:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(flag == true){
					iParam[2] = _ChgTempOT;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;
				}else{
					timePara = 0;
				}
				flag = false;
				TickOut((uint32 *)&itick, 0);
				state++;				
			}
			break;	
		case 6:/*���ʱ�����*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				for(i = deviceAddr_2;i<SysCtr_AllDoorNum;i++){
					if((hmi_ULP->stateInfoChange.sysLogic.batChgOTime&(DoorNumDefine)((DoorNumDefine)0x01<<i)) != 0){
						deviceAddr_2 = i+1;
						iParam[2] = i;	
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						flag = true;
						timePara = DoorNumVoiceTime;
						break;
					}
				}
				if(flag == false){
					timePara = 0;
					deviceAddr_2 = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 7:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(flag == true){
					iParam[2] = _ChgTimeTimeOut;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;
				}else{
					timePara = 0;
				}
				flag = false;
				TickOut((uint32 *)&itick, 0);
				state++;				
			}
			break;	
		case 8:/*���������*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				for(i = deviceAddr_3;i<SysCtr_AllDoorNum;i++){
					if((hmi_ULP->stateInfo.batOnline_1minNoCur&(DoorNumDefine)((DoorNumDefine)0x01<<i)) != 0){
						deviceAddr_3 = i+1;
						iParam[2] = i;	
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						flag = true;
						timePara = DoorNumVoiceTime;
						break;
					}
				}
				if(flag == false){
					timePara = 0;
					deviceAddr_3 = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 9:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(flag == true){
					iParam[2] = _ChgErr;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;
				}else{
					timePara = 0;
				}
				flag = false;
				TickOut((uint32 *)&itick, 0);
				state++;				
			}
			break;	
		case 10:/*�ֿع���*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				for(i = deviceAddr_4;i<SysCtr_AllDoorNum;i++){
					if((hmi_ULP->stateInfoChange.sysLogic.comFault&(DoorNumDefine)((DoorNumDefine)0x01<<i)) != 0){
						deviceAddr_4 = i+1;
						iParam[2] = i;	
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						flag = true;
						timePara = DoorNumVoiceTime;
						break;
					}
				}
				if(flag == false){
					timePara = 0;
					deviceAddr_4 = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 11:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(flag == true){
					iParam[2] = _comErr;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;
				}else{
					timePara = 0;
				}
				flag = false;
				TickOut((uint32 *)&itick, 0);
				state++;				
			}
			break;	
		case 12:/*��ع���*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				for(i = deviceAddr_5;i<SysCtr_AllDoorNum;i++){
					if((hmi_ULP->stateInfoChange.sysLogic.batFault&(DoorNumDefine)((DoorNumDefine)0x01<<i)) != 0){
						deviceAddr_5 = i+1;
						iParam[2] = i;	
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						flag = true;
						timePara = DoorNumVoiceTime;
						break;
					}
				}
				if(flag == false){
					timePara = 0;
					deviceAddr_5 = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 13:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(flag == true){
					iParam[2] = _BatErr;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;
				}else{
					timePara = 0;
				}
				flag = false;
				TickOut((uint32 *)&itick, 0);
				state++;				
			}
			break;	
		case 14:/*�ֿ�ʧ��*/
			if(TickOut((uint32 *)&itick, timePara) == true){
				for(i = deviceAddr_6;i<SysCtr_AllDoorNum;i++){
					if(hmi_ULP->sysFire.state.bits.interCom == true){
						deviceAddr_6 = i+1;
						iParam[2] = i;	
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						flag = true;
						timePara = DoorNumVoiceTime;
						break;
					}
				}
				if(flag == false){
					timePara = 0;
					deviceAddr_6 = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;
		case 15:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(flag == true){
					iParam[2] = _ComIsLose;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;
				}else{
					timePara = 0;
				}
				flag = false;
				TickOut((uint32 *)&itick, 0);
				state++;				
			}
			break;	
		case 16:/*��ѹ���*/
			if(TickOut((uint32 *)&itick,timePara) == true){
				if(hmi_PhaseVc.bits.hv == true){
					iParam[2] = _PhaseOverV;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;/*3S*/
				}else{
					timePara = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;	
		case 17:/*Ƿѹ���*/
			if(TickOut((uint32 *)&itick,timePara) == true){
				if(hmi_PhaseVc.bits.lv == true){
					iParam[2] = _PhaseLV;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;/*3S*/
				}else{
					timePara = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state++;
			}
			break;	
		case 18:/*�������*/
			if(TickOut((uint32 *)&itick,timePara) == true){
				if(hmi_PhaseVc.bits.oc == true){
					iParam[2] = _PhaseOC;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					timePara = 3000;/*3S*/
				}else{
					timePara = 0;
				}
				TickOut((uint32 *)&itick, 0);
				state = 0;
			}
			break;				
	}
}

/*
** һ��ҳ�澯��
*/
static void HMI_DW_FirstWaring(void){
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteFirstWaring,0};
	
	switch(thispage.state){
		case 0:/*������Ϣ֮����״̬��Ϣ*/
			iParam[1] = D_LCD_CMD_WriteFirstWaring;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 1:/*��ҳ��ͼ��+��ѹ������ַ*/
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			iParam[1] = D_LCD_CMD_WriteIconPara;
			iParam[2] = thispage.state - 1;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 7:
			thispage.state = 0;
			break;
	}

	/*
	** һ������������Ϣ
	*/
	waringPageVoice();

	/*
	** ��ҳ���ַ��ת��ַ
	*/
	batInfo_JumpAddr();	

	/*
	** ���һ��ҳ�澯��
	*/
	if(checkFirstPageWaring() == false){
		iParam[1] = D_LCD_CMD_JumpScreen;
		iParam[2] = 0;
		LCD_SetScreenID(HMI_DW_PAGE_MAIN);
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
	}	
}

/*
** �������ҳ��
*/
static void HMI_DW_PwPage(void){
	uint32 iParam[3] = {0x01,D_LCD_CMD_WritePwShow,0};
	LcdAdminPw lcdAdminTemp = {0};
	uint8 intputPw[4*sizeof(uint16)] = {0};
	uint16 superPw[4] = {0x3132,0x3334,0x3536,0xFFFF};
	uint8 page = 0;

	/*
	** ���²�����ѡ--�ṩ�Ա�ʹ��
	*/
	memset((uint8*)&intputPw[0], ' ',4*sizeof(uint16));

	switch(thispage.state){
		case 0:
			clear_OkBtn();
			iParam[1] = D_LCD_CMD_WriteOkBtn;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 1:
			clear_PwShow();
			iParam[1] = D_LCD_CMD_WritePwShow;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 2:
			clear_PwInput();
			iParam[1] = D_LCD_CMD_WritePwInput;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;				
			break;
		case 3:
			iParam[1] = D_LCD_CMD_ReadPwInput;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 4:
			lcdAdminTemp = get_PwInput();
			if(memcmp((uint8*)&lcdAdminTemp.ascii[0],(uint8*)&intputPw[0],4*sizeof(uint16)) != 0){
				set_PwAscii();
				iParam[1] = D_LCD_CMD_WritePwShow;
				iParam[2] = 0;
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				thispage.state++;	
			}else{
				thispage.state = 3;
			}
			break;
		case 5:
			iParam[1] = D_LCD_CMD_ReadOkBtn;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 6:
			if(get_OkBtn() != 0){/*��������ֵ*/
				lcdAdminTemp = get_PwInput();
				if(memcmp((uint16*)&lcdAdminTemp.ascii[0],(uint16*)&superPw[0],8) == 0){/*����һ��*/
					page = HMI_DW_PAGE_SysInfo;					
				}else{
					page = HMI_DW_PAGE_PwNoCorrect;
				}
				iParam[1] = D_LCD_CMD_JumpScreen;
				iParam[2] = 0;
				LCD_SetScreenID(page);
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);	
				/*
				** ����������ʾ�������
				*/
				clear_PwShow();
				iParam[1] = D_LCD_CMD_WritePwShow;
				iParam[2] = 0;
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			}else{
				thispage.state = 3;
			}
			break;
	}
}

/*
** get Device ID 
*/
void get_HMI_DeviceId(void){
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteDeviceId,0};
	static uint8 state = 0;

	switch(state){
		case 0x00:
			update_DeviceId(get_SysDeviceId());
			iParam[1] = D_LCD_CMD_WriteDeviceId;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			state++;
			break;
		case 0x01:
			iParam[1] = D_LCD_CMD_ReadDeviceId;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			state++;			
			break;
		case 0x02:
			if(memcmp((uint8*)&get_SysDeviceId().show[0],(uint8*)&get_DeviceId().show[0],sizeof(DeviceId)) != 0){
				if(get_ServePara().registerFlag ==  false){/*��ע��,����������豸ID*/
					set_SysDeviceId(get_DeviceId());
				}
				state = 0;
			}else{
				state = 1;
			}
			break;
	}
}

/*
** ϵͳ��Ϣ
*/
static void HMI_DW_SysInfoPage(void){
	uint32 iParam[3] = {0x01,D_LCD_CMD_WritePwShow,0};
	static SysDomain sysDomainTemp = {0};
	uint8 local[64] = {0};
	uint8 remote[64] = {0};

	switch(thispage.state){
		case 0:
			update_Domain();
			updateTempDomain((uint8 *)&sysDomainTemp.local[0], (uint8 *)&sysDomainTemp.remote[0]);
			iParam[1] = D_LCD_CMD_WriteLocalDomainShow;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 1:
			iParam[1] = D_LCD_CMD_WriteRemoteDomainShow;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;				
			break;
		case 2:
			iParam[1] = D_LCD_CMD_ReadLocalDomainInput;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 3:
			iParam[1] = D_LCD_CMD_ReadRemoteDomainInput;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 4:
			get_LocalDomainInput((uint8 *)&local[0]);
			if(memcmp((uint8*)&local[0],(uint8*)&sysDomainTemp.local[0],64) != 0){
				/*
				** ���±�������--����Զ������
				*/
				sysDomainTemp = get_SysDomain();
				memcpy((uint8*)&sysDomainTemp.local[0],(uint8*)&local[0],64);
				set_SysDomain(sysDomainTemp);
			}
			thispage.state++;
			break;
		case 5:
			get_RemoteDomainInput((uint8 *)&remote[0]);
			if(memcmp((uint8*)&remote[0],(uint8*)&sysDomainTemp.remote[0],64) != 0){
				/*
				** ����Զ������--������������
				*/
				memcpy((uint8*)&sysDomainTemp.remote[0],(uint8*)&remote[0],64);
				set_SysDomain(sysDomainTemp);
			}else{
				thispage.state = 2;
			}
			break;
	}

	/*
	** get ע��/ע�� key
	*/
	get_RegisterLogout();

	/*
	** get Device ID 
	*/
	get_HMI_DeviceId();	
	
}

/*
** ���벻��ȷ
*/
static void HMI_DW_PwNoCorrect(void){

}

/*
** WIFI �û���������
*/
static void HMI_DW_WIFINamePw(void){
	static uint8 stepName = 0;
	static uint8 stepPw = 0;
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteWifiName,0};
	static WifiSet wifiSetTemp = {0};
	uint8 name[32] = {0};
	uint8 pw[32] = {0};
	static uint32 itick = 0;

	switch(stepName){
		case 0:
			init_WifiNameToDwinShow();
			update_WifiName((uint8 *)&wifiSetTemp.name[0]);/*�����ʼWIFI�û���*/
			iParam[1] = D_LCD_CMD_WriteWifiName;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			stepName++;
			break;
		case 1:
			iParam[1] = D_LCD_CMD_ReadWifiName;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			stepName++;			
			break;
		case 2:
			update_WifiName((uint8 *)&name[0]);/*��ȡʵʱ�û����Ƿ���*/
			if(memcmp((uint8*)&wifiSetTemp.name[0],(uint8*)&name[0],32) != 0){
				/*
				** ����WIFI�û���
				*/
				set_WifiNameSysSet((uint8*)&name[0]);
				stepName = 0;
			}else{
				stepName = 1;
			}
			break;
	}

	switch(stepPw){
		case 0:
			init_WifiPwToDwinShow();
			update_WifiPw((uint8 *)&wifiSetTemp.pw[0]);/*�����ʼWIFI����*/
			iParam[1] = D_LCD_CMD_WriteWifiPw;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			stepPw++;
			break;
		case 1:
			iParam[1] = D_LCD_CMD_ReadWifiPw;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			stepPw++;			
			break;
		case 2:
			update_WifiPw((uint8 *)&pw[0]);/*��ȡʵʱ�����Ƿ���*/
			if(memcmp((uint8*)&wifiSetTemp.pw[0],(uint8*)&pw[0],32) != 0){
				/*
				** ����WIFI����
				*/
				set_WifiPwSysSet((uint8*)&pw[0]);
				stepPw = 0;
			}else{
				stepPw = 1;
			}
			break;
	}

	/*
	** ��������Ƿ�����--����֮��Ļ��ʾ
	*/
	if(TickOut((uint32 *)&itick, 200) == true){
		iParam[1] = D_LCD_CMD_WriteStatePara;/*ʹ����ҳ��״̬����--�����Ƿ�����*/
		iParam[2] = 0;
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);		
	}
	
}

/*
** ҳ���������
*/
const HMI_DW_PAGE hmi_DW_PageTable[] = {
	{HMI_DW_PAGE_MAIN,0,HMI_DW_MainPage},
	{HMI_DW_PAGE_BatInfo,0,HMI_DW_BatInfoPage},
	{HMI_DW_PAGE_Waring,0,HMI_DW_FirstWaring},
	{HMI_DW_PAGE_InputPw,0,HMI_DW_PwPage},
	{HMI_DW_PAGE_SysInfo,0,HMI_DW_SysInfoPage},
	{HMI_DW_PAGE_PwNoCorrect,0,HMI_DW_PwNoCorrect},
	{HMI_DW_PAGE_WifiNamePw,0,HMI_DW_WIFINamePw},
};
uint8 hmi_DW_PageTableNum = sizeof(hmi_DW_PageTable)/sizeof(HMI_DW_PAGE);

/*
** ҳ�������ʼ��������ʼ��
*/
static void HMI_DW_DATA_Init(void){
	thispage.id = HMI_DW_PAGE_MAIN;
	thispage.state = 0;
	thispage.handle = hmi_DW_PageTable[0].handle;

	/*
	** ��ʼ��HMI����
	*/
	hmi_ULRP = getULReplyPara();
	hmi_ULP = getUpperLayerPara();

	/*
	** ��ʼ��LCD��������
	*/
	init_LcdPara();
}


/*
** ҳ������ִ�к���
*/
static void HMI_DW_Thread(void* p_arg){
	#if CtrHardware == CtrHardware_Dwin || Screen_Compatibility == 1
	static uint32 btick = 0;
	uint32 iParam[3] = {0x01,D_LCD_CMD_ReadScreenId,0};
	uint8 i = 0;
	#endif
	/*
	** ҳ�������ʼ��������ʼ��
	*/
	HMI_DW_DATA_Init();

	Sleep(1000);
	
	#if CtrHardware == CtrHardware_Dwin || Screen_Compatibility == 1
	for(i=0;i<3;i++){/*ϵͳ��λ������ҳ��*/
		iParam[1] = D_LCD_CMD_JumpScreen;
		LCD_SetScreenID(HMI_DW_PAGE_MAIN);
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
	}
	#endif

	for(;;){
		#if CtrHardware == CtrHardware_Dwin || Screen_Compatibility == 1
		if(hmi_ULP->upgrCtrReset.flag == false){
			/*
			** ҳ���л��ӿں���
			*/
			HMI_DW_PageDataTableFun();		

			/*
			** ��ȡ��ǰҳ�����
			*/
			if(TickOut((uint32 *)&btick, 200) == true){
				iParam[1] = D_LCD_CMD_ReadScreenId;
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				TickOut((uint32 *)&btick, 0x00);
			}
		}else{/*����������--�л�ҳ��*/
			if(TickOut((uint32 *)&btick, 100) == true){
				iParam[1] = D_LCD_CMD_JumpScreen;
				LCD_SetScreenID(HMI_DW_PAGE_CtrUpgr);
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);	
				TickOut((uint32 *)&btick, 0x00);
			}
		}
		#endif
		/*
		** ���¿��Ź��Ĵ���--ι��
		*/
		watchdogUpdate();

		Sleep(20);
	}
}


/*
** ҳ����������ʼ��
*/
void HMI_Init(void){
	Thread_create(HMI_DW_Thread, 0, (int32 *)&HMI_DW_TaskSTK[512-1], HMI_DW_Task_Prio);
}

