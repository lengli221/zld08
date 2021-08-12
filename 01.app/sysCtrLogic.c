#include "includes.h"

static int32 sysCtrlLogic_TaskStk[256] = {0};

/*
** ʹ��ָ����������ļ�����
*/
UpperLayerPara* sys_ULP = null;

/*
** �̼�����֮��ʼ������
*/
void init_FirewarePara(void){
	sys_ULP = getUpperLayerPara();
}

/*
** �̼�--Output--����
*/
void firewareCtr_Fan(void){
	bool ret = false;
	Tsave ts = get_Tsave();
	int16 curTemp = 0;
	
	ret = CC_Control_Fan((uint16)(ts.base/10), (uint16)(ts.diff/10), (int16*)&curTemp);

	/*
	** ���ó��ֻ���
	*/
	sys_ULP->sysFire.chgDoorTemp = curTemp;
	/*
	** ���ȿ��� ��־λ����
	*/
	firewareCtr(setFirewareFan_Label, ret);
	sys_ULP->sysFire.state.bits.fan = ret;
	sys_ULP->stateInfoChange.sysModuleStateInfo.bits.fanOpen = ret;
}

/*
** �̼�--Output--������
*/
void firewareCtrLed(void){
	if(sys_ULP->stateInfoChange.sysModuleStateInfo.bits.smoke == true/*�̸�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.emer == true/*��ͣ*/
		||  sys_ULP->stateInfoChange.sysModuleStateInfo.bits.chgDoorOverTemp == true /*������ֹ���*/
		|| sys_ULP->stateInfoChange.sysLogic.chgOverTemp != 0/*���������*/
		||  sys_ULP->stateInfoChange.sysLogic.batIsErr != 0 /*��ع���*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseOverVFault == true/*���ѹ��ѹ>270V,���µ���ָܻ�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseOverVWaring == true/*���ѹ��ѹ�ɻָ�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseLV == true/*���ѹǷѹ,�ɻָ�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseOC == true/*���������*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseCShort == true/*�������·*/
		/*-----------------------------��������:���ܳ����(12A)�汾V1.03---------------------------------*/
		|| sys_ULP->stateInfoChange.sysLogic_2.chargerOC != 0 /*���������*/
		|| sys_ULP->stateInfoChange.sysLogic_2.chargeOT != 0/*���������*/
		/*-----------------------------------------------------------------------------------------------*/	
		|| chk_BatIsExitOneWaring() == true
		){
		firewareCtr(setFireFalutLed_Label, true);
		/*
		** �������ⱨ��--�ϱ�
		*/
		sys_ULP->stateInfoChange.sysModuleStateInfo.bits.ledOpen = true;
	}else{
		firewareCtr(setFireFalutLed_Label, false);
		sys_ULP->stateInfoChange.sysModuleStateInfo.bits.ledOpen = false;
	}
}

/*
** ctr run led 
*/
void ctr_RunLed(void){
	static uint32 itick = 0;
	static bool flag = false;

	if(TickOut((uint32 *)&itick, 500) == true){
		TickOut((uint32 *)&itick, 0);
		flag = flag == false?true:false;
		firewareCtr(setFireCtrRunLed_Label, flag);
	}
}

/*
** �̼�--Output
*/
void firewareCtrAnalyze(void){
	/*
	** �̼�--Output--����
	*/
	firewareCtr_Fan();
	/*
	** �̼�--Output--������
	*/
	firewareCtrLed();
	/*
	** ctr run led 
	*/
	ctr_RunLed();	
}



/*
** �̼�--Input--�̸�
*/
void firewareSmoke(void){
	/*static int8 cnt = 0;*/
	static int16 cnt = -150;

	if(firewareYxIn(getYxIn_Smoke)){
		if(cnt++ >= 150/*5*/){
			cnt = 150/*5*/;
			#if VerCtr == VerCtr_Normal
				sys_ULP->sysFire.state.bits.smoke = true;
				sys_ULP->stateInfoChange.sysModuleStateInfo.bits.smoke = true;
			#endif
		}
	}else{
		if(cnt-- <= -150/*-5*/){
			cnt = -150/*-5*/;
			sys_ULP->sysFire.state.bits.smoke = false;
			sys_ULP->stateInfoChange.sysModuleStateInfo.bits.smoke = false;
		}
	}
}

/*
** �̼�--input--��ͣ
*/
void firewareEmer(void){
	static int8 cnt = 0;

	if(firewareYxIn(getYxIn_Emer)){
		if(cnt++ >= 5){
			cnt = 5;
			#if VerCtr == VerCtr_Normal
				sys_ULP->stateInfoChange.sysModuleStateInfo.bits.emer = true;
			#endif
		}
	}else{
		if(cnt-- <= -5){
			cnt = -5;
			sys_ULP->stateInfoChange.sysModuleStateInfo.bits.emer = false;
		}
	}
}

/*
** �̼�--input--����
*/
void firewareAnti(void){
	static int8 cnt = 0;

	if(firewareYxIn(getYxIn_Anti)){
		if(cnt++ >= 5){
			cnt = 5;
			#if VerCtr == VerCtr_Normal
				sys_ULP->stateInfoChange.sysModuleStateInfo.bits.anti = true;
			#endif
		}
	}else{
		if(cnt-- <= -5){
			cnt = -5;
			sys_ULP->stateInfoChange.sysModuleStateInfo.bits.anti = false;
		}
	}
}

/*
** �̼�--Input
*/
void firewareYxInAnalyze(void){
	/*
	** �̼�--Input--�̸�
	*/
	firewareSmoke();
	/*
	** �̼�--input--��ͣ
	*/
	firewareEmer();	
	/*
	** �̼�--input--����
	*/
	firewareAnti();	
	/*
	** ʵʱ���һ��������ֹ����
 	*/
 	oneWaring_ForbidUpgrDownFile();
	/*
	** 20210302 -- һ�����������ֶ��ָ����߼�--���ֶ����µ�(�µ�AC)�߼�
	*/
	oneWaring_NeedHandRecoverLogic();
}

/*
** �ڲ�ͨѶ�쳣(����60S��ȡ�������зֿذ���Ϣ)
*/
void sysInnerComErr(void){
	/*----------------------------------------------------------------------------------------*/
	/*
	** ���ζ�ʱԭ��:
	**				�ֽ�����ֿذ��ʧ��ʱ���޸�Ϊ:60S
	*/
	/*----------------------------------------------------------------------------------------*/
	/*static uint32 itick = 0;*/
	if(sys_ULP->stateInfoChange.sysLogic.comIsOnline == (DoorNumDefine)0xFFFFFFFFFFFF){
		/*Ӳ�����غ��ϲż��ֿ�ʧ��ָ��*/
		if(sys_ULP->stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose == false){
			/*if(TickOut((uint32 *)&itick, 60000) == true){*/
				sys_ULP->sysFire.state.bits.interCom = true;
			/*}*/
		}else{
			sys_ULP->sysFire.state.bits.interCom = false;
		}
	}else{
		/*TickOut((uint32 *)&itick, 0);*/
		sys_ULP->sysFire.state.bits.interCom = false;
	}	
}

/*
** ���³�����ֹ��±�־
*/
void updateChgDoorOverTempFlag(void){
	if(sys_ULP->sysFire.chgDoorTemp >= get_ChgDoorOTLimit()){
		#if VerCtr == VerCtr_Normal
			sys_ULP->stateInfoChange.sysModuleStateInfo.bits.chgDoorOverTemp = true;
		#endif
	}else{
		if(sys_ULP->sysFire.chgDoorTemp /*>= get_ChgDoorOTLimit()*/ < (get_ChgDoorOTLimit() - 50)){/*�ز�5���϶�*/
			sys_ULP->stateInfoChange.sysModuleStateInfo.bits.chgDoorOverTemp = false;
		}
	}
}

/*
** ���һ��ҳ�澯��
*/
extern Hmi_PhaseVC hmi_PhaseVc;
bool checkFirstPageWaring(void){
	bool ret = false;
	
	if(sys_ULP->stateInfoChange.sysLogic.batIsErr != 0 /*��ع���*/
		|| sys_ULP->stateInfoChange.sysLogic.chgOverTemp != 0 /*���������*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.chgDoorOverTemp == true /*������ֹ���*/
		|| sys_ULP->stateInfoChange.sysLogic.batChgOTime != 0 /*���ʱ�����*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.smoke == true/*������*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.emer == true/*��ͣ����*/
		|| sys_ULP->stateInfo.batOnline_1minNoCur != 0 /*���������*/
		|| sys_ULP->stateInfoChange.sysLogic.batFault != 0/*��ع���*/
		|| sys_ULP->stateInfoChange.sysLogic.comFault != 0/*�ֿع���*/
		|| sys_ULP->sysFire.state.bits.interCom == true/*�ڲ�ʧ��*/
		|| hmi_PhaseVc.flag != 0/*��ѹ,Ƿѹ,����*/ 
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.anti == true/*���׹���--20210226--����ά��,����������--�������ڵ�����ʾ�汾����*/){
		
		ret = true;
	}

	return ret;
}

/*
** 20210129--����:���ع̼������з���һ������,�����������ع̼�
**	��ע:����12A V1.5 20210812
**		������ֿ��ѷ�������,���ص�ع̼�,���س�����̼�--������ֹ���ع̼���Ϣ
**		��׿--�������ط������طֿ�,��ع̼�,������̼�,������ֹ,����һ���澯��Ϣ�޷���Ӧ
**		��׿��������,����ֹ
*/
void oneWaringHappen_ImmediatelyEndFire(void){
	/*���ư������ļ���������*/
	UpgrFilePara upgr = getUpgrFilePara();	
	int16 loc = -1;
//	ComBup comCurBupTemp = getCurComBup();

	/*��׿--�������ط������طֿ�,��ع̼�,������̼�,������ֹ,����һ���澯��Ϣ�޷���Ӧ*/
	if(upgr.upgrFileType.board != 0){/*���ڵ��,�ֿع̼�����*/
		memset((uint8*)&upgr.upgrFileType.softVer,0,sizeof(UpgrFilePara)-sizeof(uint8)-sizeof(uint8)-sizeof(DetailedInfo));
		switch(upgr.upgrFileType.board){
			case ComBoradType:/*ͨѶ��*/
				upgr.upgrFileType.board = ComBoradType;
				set_UpgrBinFilePara(upgr);								
				break;
			case BatBoardType:/*��ع̼�*/
				upgr.upgrFileType.board = BatBoardType;
				loc = get_BatLocation(upgr);
				if(loc != -1){
					set_UpgrBinFilePara(upgr);					
				}
				break;
			case ChargeBoardType:
				upgr.upgrFileType.board = BatBoardType;
				loc = get_ChargeFireSize();
				if(loc != -1){
					set_UpgrBinFilePara(upgr);
				}
				break;
		}
		/*�����������*/
		/*���ư�֮�����������֮�����ѱ���*/
		memset((uint8*)&upgr.upgrFileType.cmdType,0x00,sizeof(UpgrFilePara));
		setUpgrFilePara(upgr);
		/*set �ָ��ϲ�Э��֮APPӦ�ò�Э���־*/
		set_CtrBoardTaskRecoverFlag();
	}

//	/*������ֿ��ѷ�������,���ص�ع̼�,���س�����̼�--������ֹ���ع̼���Ϣ*/
//	if(comCurBupTemp.binFileType.flag != 0){
//		Clear_CurComBupPara();	
//		UpgradeLLParam_Init();
//		/*�Ż����������:20210227*/
//		sys_ULP->stateInfoChange.sysLogic.comUpgr = 0;
//		sys_ULP->stateInfoChange.sysLogic.comUpgrIsOk = 0;		
//		sys_ULP->stateInfoChange.sysLogic.batFileDownload = 0;
//		sys_ULP->stateInfoChange.sysLogic.batFileDownloadIsOk = 0;	
//		sys_ULP->stateInfoChange.sysLogic_2.chargeFileDownloading = 0; 
//		sys_ULP->stateInfoChange.sysLogic_2.chargeFileDownloadFinsh = 0;		
//	}
}

/*��⵽�µ��־�����ֿ����������ع̼�����-20210130*/
void chkCloseAc_EndSubUpgrDownProc(void){
	ComBup comCurBupTemp = getCurComBup();

	if(sys_ULP->stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose == true){
		if(comCurBupTemp.binFileType.flag != 0){
			Clear_CurComBupPara();	
			UpgradeLLParam_Init();
			/*�Ż����������:20210227*/
			sys_ULP->stateInfoChange.sysLogic.comUpgr = 0;
			sys_ULP->stateInfoChange.sysLogic.comUpgrIsOk = 0;		
			sys_ULP->stateInfoChange.sysLogic.batFileDownload = 0;
			sys_ULP->stateInfoChange.sysLogic.batFileDownloadIsOk = 0;	
			sys_ULP->stateInfoChange.sysLogic_2.chargeFileDownloading = 0; 
			sys_ULP->stateInfoChange.sysLogic_2.chargeFileDownloadFinsh = 0;	
		}
		/*20210227--����������͵�����������־*/
		sys_ULP->stateInfoChange.sysLogic.batUpgr = 0;
		sys_ULP->stateInfoChange.sysLogic.batUpgrIsOk = 0;
		sys_ULP->stateInfoChange.sysLogic_2.chargeUpgr = 0;
		sys_ULP->stateInfoChange.sysLogic_2.chargeUpgrIsOk = 0;
	}
}

/*
** ����һ��������ֹ�����Լ������ļ�
*/
bool oneWaring_ForbidUpgrDownFile(void){
	bool ret = false;
	
	if(sys_ULP->stateInfoChange.sysModuleStateInfo.bits.smoke == true/*�̸�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.emer == true/*��ͣ*/
		||  sys_ULP->stateInfoChange.sysModuleStateInfo.bits.chgDoorOverTemp == true /*������ֹ���*/
		|| sys_ULP->stateInfoChange.sysLogic.chgOverTemp != 0/*���������--�������*/
		||  sys_ULP->stateInfoChange.sysLogic.batIsErr != 0 /*��ع���*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseOverVFault == true/*���ѹ��ѹ>270V,���µ���ָܻ�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseOverVWaring == true/*���ѹ��ѹ�ɻָ�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseLV == true/*���ѹǷѹ,�ɻָ�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseOC == true/*���������*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseCShort == true/*�������·*/
		/*-----------------------------��������:���ܳ����(12A)�汾V1.03---------------------------------*/
		|| sys_ULP->stateInfoChange.sysLogic.batChgOTime != 0 /*��س��ʱ�����*/
		|| sys_ULP->stateInfoChange.sysLogic_2.chargerOC != 0 /*���������*/
		|| sys_ULP->stateInfoChange.sysLogic_2.chargeOT != 0/*���������*/
		/*-----------------------------------------------------------------------------------------------*/
		|| chk_BatIsExitOneWaring() == true
	){
		ret = true;
		
		/*
		** 20210129--����:���ع̼������з���һ������,�����������ع̼�
		*/
		oneWaringHappen_ImmediatelyEndFire();
	}
	return ret;
}

/*
** ����:����һ���澯����ж�--20210512
*/
bool cabinet_OneWaringFlagJudge(void){
	bool ret = false;

	if(sys_ULP->stateInfoChange.sysModuleStateInfo.bits.smoke == true/*�̸�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.emer == true/*��ͣ*/
		||  sys_ULP->stateInfoChange.sysModuleStateInfo.bits.chgDoorOverTemp == true /*������ֹ���*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseOverVFault == true/*���ѹ��ѹ>270V,���µ���ָܻ�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseOverVWaring == true/*���ѹ��ѹ�ɻָ�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseLV == true/*���ѹǷѹ,�ɻָ�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseOC == true/*���������*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseCShort == true/*�������·*/
	){
		ret = false;
	}
	
	return ret;
}

/*
** 20210302 -- һ�����������ֶ��ָ����߼�--���ֶ����µ�(�µ�AC)�߼�
*/
bool oneWaringResetSetFlag = false;
void oneWaring_NeedHandRecoverLogic(void){
	if(sys_ULP->stateInfoChange.sysModuleStateInfo.bits.smoke == true/*�̸�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.emer == true/*��ͣ*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.chgDoorOverTemp == true /*������ֹ���*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseOverVFault == true/*���ѹ��ѹ>270V,���µ���ָܻ�*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseOC == true/*���������*/
		|| sys_ULP->stateInfoChange.sysModuleStateInfo.bits.phaseCShort == true/*�������·*/
	){
		oneWaringResetSetFlag = true;
	}
}

/*
** ���AC�Ͽ�--����ز���
*/
void chkAcClose_ClearParam(void){
	uint8 i = 0;
	
	if(sys_ULP->stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose == true){/*AC�Ͽ�*/
		for(i=0;i<get_batFireSize();i++){/*���ع̼�����ģʽ*/
			set_FireEnterTestModel(0x03, false, i);
		}

		for(i=0;i<get_ChargeFireSize();i++){/*�������̼�����ģʽ*/
			set_FireEnterTestModel(0x04, false, i);
		}
		/*20210425-Ӧ�Ҫ��--���µ�ģʽ�������������ԭ��--�������/������̼�*/
		/*20210518--����ԭ����ڱ�־λ֮��,Ӧ�Ҫ�����ԭ��*/
		//init_UpgrFailPara();
		/*���е�,�������ģʽ--����ڴ��ڼ������������޷�����״̬*/
		sys_ULP->stateInfoChange.sysModuleStateInfo_2.bits.testModelFlag = false;			
	}
}

/*
** ϵͳ Check
*/
void sysCheck(void){
	/*
	** �ڲ�ͨѶ�쳣(����60S��ȡ�������зֿذ���Ϣ)
	*/
	sysInnerComErr();
	/*
	** ���³�����ֹ��±�־
	*/
	updateChgDoorOverTempFlag();	
	/*��⵽�µ��־�����ֿ����������ع̼�����-20210130*/
	chkCloseAc_EndSubUpgrDownProc();
	/*���AC�Ͽ�--����ز���*/
	chkAcClose_ClearParam();
}

void SM_SysCtrlLogic_Task(void* p_arg){
	/*
	** �̼�����֮��ʼ������
	*/
	init_FirewarePara();

	Sleep(500);

	#if Debug_Flash_BugAnalysis == 1
	/*
	** Flash Bug����˵��
	*/	
	sFlash_BugAnalysis();
	#endif
	
	for(;;){
		/*
		** �̼�--Output
		*/
		firewareCtrAnalyze();		
		/*
		** ϵͳ Check
		*/
		sysCheck();
		/*
		** �̼�--Input
		*/
		firewareYxInAnalyze();

		/*
		** ͨѶ���������+������ز�������
		*/
		ComBoard_TaskSwitch();
		/*
		** ����ͨѶ��/��ذ��ļ�����:
		**		1.��������ͨѶС������
		**		2.�ȴ�ִ�лظ�����,���800ms֮������ϲ������л�
		*/
		ctrBoard_TaskSwitch();
		/*
		** �ָ��ϲ�Э��֮APPӦ�ò�Э�� 
		**		���Ʋ���:
		**				1.�ļ��������֮��,���ñ�־
		**				2.�ȴ��ظ�(��ʱ)800ms֮�����������л�
		*/
		ctrBoard_TaskRecover();	

		/*
		** CAN1 Tx Task
		*/
		CAN1_TxDataTask();

		/*
		** ���¿��Ź��Ĵ���--ι��
		*/
		watchdogUpdate();
		
		Sleep(10);
	}
}

/*
**�ṩϵͳ�������ϵͳ���ƽӿں���
*/
void SysCtrlLogic_Init(void){
	Thread_create(SM_SysCtrlLogic_Task, 0, (int32 *)&sysCtrlLogic_TaskStk[256-1], SysCtrlLogic_Prio);
}

