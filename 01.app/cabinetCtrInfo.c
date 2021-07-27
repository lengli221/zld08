
#include "includes.h"

extern UpperLayerPara upperLayerPara;

/*
** ������Ϣ֮�Ӵ�����������
*/
void cabCtrInfo_EventId_ContactorFun(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/	
	uint16 tmp = 0;

	/*
	** ����
	*/
	switch(rx[0]){
		case 0x01:/*����*/
			/*
			** ������Ϣ -- ��ͣ,�̸�,�¿�,��ѹ>=270V���ɻָ�
			*/	
			if((upperLayerPara.stateInfoChange.sysModuleStateInfo.flag & SysModuleStateInfo_MaskBit_Upgr) != 0){
				tx[txLen] = 0x02;/*����ʧ��*/
				txLen+= sizeof(uint8);
				tmp = upperLayerPara.stateInfoChange.sysModuleStateInfo.flag & SysModuleStateInfo_MaskBit_Upgr;
				memcpy((uint8*)&tx[txLen] ,(uint8*)&tmp,sizeof(uint16));
				txLen += sizeof(uint16);
				upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.majorLoopClose = true;
			}else{
				tx[txLen] = 0x01;/*�����ɹ�*/
				txLen+= sizeof(uint8);
				/*
				** �Ӵ���Ӳ���ӿ� 
				*/
				firewareCtr(setFireACSlaveCtr_label, true);
				upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.majorLoopClose = false;
			}			
			break;
		case 0x02:/*�Ͽ�*/
			tx[txLen] = 0x01;/*�����ɹ�*/
			txLen += sizeof(uint8);
			/*
			** �Ӵ���Ӳ���ӿ� 
			*/
			firewareCtr(setFireACSlaveCtr_label, true);			
			upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.majorLoopClose = true;
			break;
	}

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] = txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** ������Ϣ֮����SOC����
*/
void cabCtrInfo_EventId_UpdateSocPara(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;

	/*
	** ������ �·�����ֵ �ظ������Ƿ�ɹ�
	*/
	tx[txLen] = rx[len];
	txLen += sizeof(uint8);
	
	if(rx[len] >=50 && rx[len] <= 100){/*SOC:70-100*/
		tx[txLen] = 0x01;/*�������³ɹ�*/
		/*
		** set SOC��ֵ
		*/
		set_SocLimit(rx[len]);
	}else{
		tx[txLen] = 0x02;/*��������ʧ��*/
	}
	txLen += sizeof(uint8);

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}

/*
** ���ʱ����ֵ
*/
void cabCtrInfo_EventId_ChgTime(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint16 min = 0;

	/*
	** ������--���ʱ����ֵ,�ظ������Ƿ�ɹ�
	*/
	memcpy((uint8*)&tx[txLen],(uint8*)&rx[len],sizeof(uint16));
	memcpy((uint8*)&min,(uint8*)&rx[len],sizeof(uint16));
	txLen += sizeof(uint16);

	if(min >= 10){/*��ֵ�ݶ����ٴ���10min*/
		tx[txLen] = 0x01;/*�������³ɹ�*/
		/*
		** set ���ʱ����ֵ
		*/
		set_ChgTimeLimit(min);
	}else{
		tx[txLen] = 0x02;/*��������ʧ��*/
	}
	txLen += sizeof(uint8);

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** �����������ֵ
*/
void cabCtrInfo_EventId_ChgOverTemp(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint16 temp = 0;

	/*
	** ������--���ʱ����ֵ,�ظ������Ƿ�ɹ�
	*/
	memcpy((uint8*)&tx[txLen],(uint8*)&rx[len],sizeof(uint16));
	memcpy((uint8*)&temp,(uint8*)&rx[len],sizeof(uint16));
	txLen += sizeof(uint16);

	if(temp >= 400 && temp <= 1000){/*��ֵ�ݶ�40���϶���100���϶�*/
		tx[txLen] = 0x01;/*�������³ɹ�*/
		/*
		** set �����������ֵ
		*/
		set_ChgOverTempLimit(temp);
	}else{
		tx[txLen] = 0x02;/*��������ʧ��*/
	}
	txLen += sizeof(uint8);

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** ������ֹ�����ֵ
*/
void cabCtrInfo_EventId_ChgDoorOT(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint16 temp = 0;

	/*
	** ������--���ʱ����ֵ,�ظ������Ƿ�ɹ�
	*/
	memcpy((uint8*)&tx[txLen],(uint8*)&rx[len],sizeof(uint16));
	memcpy((uint8*)&temp,(uint8*)&rx[len],sizeof(uint16));
	txLen += sizeof(uint16);

	if(temp >= 400 && temp <= 1000){/*��ֵ�ݶ�40���϶���100���϶�*/
		tx[txLen] = 0x01;/*�������³ɹ�*/
		/*
		** set ������ֹ�����ֵ
		*/
		set_ChgDoorOTLimit(temp);
	}else{
		tx[txLen] = 0x02;/*��������ʧ��*/
	}
	txLen += sizeof(uint8);

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** ���������Ϣ֮ϵͳ�¶������Ϣ
*/
void cabCtrInfo_EventId_SysTempInfo(uint8 itemDatalen,uint8* rx,
	uint8*replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len=0;
	Tsave ts = get_Tsave();

	/*
	** ����
	*/
	tx[txLen] = rx[len];
	txLen += sizeof(uint8);
	switch(rx[len]){
		case 0x01:/*��ѯ���ֻ���*/
			memcpy((uint8*)&tx[txLen],(uint8*)&upperLayerPara.sysFire.chgDoorTemp,sizeof(uint16));
			txLen += sizeof(uint16);
			break;
		case 0x02:/*��ѯ��׼�¶Ⱥ��¶Ȼز�ֵ*/
			memcpy((uint8*)&tx[txLen],(uint8*)&ts.base,sizeof(uint16));
			txLen += sizeof(uint16);
			memcpy((uint8*)&tx[txLen],(uint8*)&ts.diff,sizeof(uint16));
			txLen += sizeof(uint16);
			break;
		case 0x03:
			len += sizeof(uint8);
			memcpy((uint8*)&ts.base,(uint8*)&rx[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&ts.diff,(uint8*)&rx[len],sizeof(uint16));
			len += sizeof(uint16);
			if((ts.base >= 250 && ts.base <= 500)
				&&(ts.diff >= 50 && ts.diff <= 100)){
				tx[txLen] = 0x01;/*���óɹ�*/
				set_Tsave(ts);
			}else{
				tx[txLen] = 0x02;/*����ʧ��*/
				ts = get_Tsave();
			}
			txLen += sizeof(uint8);
			memcpy((uint8*)&tx[txLen],(uint8*)&ts.base,sizeof(Tsave));
			txLen += sizeof(Tsave);			
			break;
	}

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** ���ư�֮����Bootloader�����־
*/
void enterCtrBootFlag(void){
	upperLayerPara.upgrCtrReset.flag = true;
	TickOut((uint32 *)&upperLayerPara.upgrCtrReset.itick, 0);
}

/*
** ���ư�֮����ͨѶ��/��ذ��ļ�
*/
void enterComBatFileDownload(void){
	upperLayerPara.upgrComBatTaskEn.flag = true;
	TickOut((uint32 *)&upperLayerPara.upgrComBatTaskEn.itick, 0);
	TickOut((uint32 *)&upperLayerPara.upgrComBatTaskEn.itickout, 0);
}

/*
** ���ư�֮��������ͨѶ/��ذ��ļ�
*/
void downloadComBatFileUpdatePara(uint8 cmdType,uint8 upgrType,
				DetailedInfo detailedInfo){
	UpgrFilePara upgrFileParaTmep = {0};

	/*
	** �������� -- ����ƫ��ֵ0x01:����ϲ�Э����²�Э�鲻һ�µ���
	** 		�������:--����Ϸֿذ�����bootloader�����汾�Ѵ��ڰ汾����,���ڴ��������ǿ������
	*/
	upgrFileParaTmep.upgrFileType.cmdType = 0x02/*cmdType + 0x01*/;/*��Էֿش����ǿ������*/

	/*
	** ��������
	*/
	switch(upgrType){
		case 0x02:/*ͨѶ��*/
			upgrFileParaTmep.upgrFileType.board = ComBoradType;
			break;
		case 0x03:/*��ذ�*/
			upgrFileParaTmep.upgrFileType.board = BatBoardType;
			break;
		case 0x04:/*������̼���*/
			upgrFileParaTmep.upgrFileType.board = ChargeBoardType;
			break;
	}
	/*
	** ������ذ���ϸ��Ϣ
	*/
	upgrFileParaTmep.upgrFileType.detailedInfo = detailedInfo;

	/*
	** ���²���
	** 		Note:set���ư������ļ�֮�������� 
	*/
	setUpgrFilePara(upgrFileParaTmep);	
}

/*
** ���ư�ִ��BootLoader����:
**		1.����Զ������ָ��
**		2.�ȴ�ִ�лظ�,800ms֮���������ؽ���BootLoader
*/
void enterCtrBootLoader(void){
	if(upperLayerPara.upgrCtrReset.flag == true){
		if(TickOut((uint32 *)&upperLayerPara.upgrCtrReset.itick, 800) == true){
			__disable_irq();/*��λ֮ǰ�ر������ж�*/
			NVIC_SystemReset();
		}
	}
}

/*
** ����ͨѶ��/��ذ��ļ�����:
**		1.��������ͨѶС������
**		2.�ȴ�ִ�лظ�����,���800ms֮������ϲ������л�
*/
void ctrBoard_TaskSwitch(void){
	static bool resumeFlag = true;
	static bool resFlag = true;

	/*
	** �����ļ���ʱ����
	*/
	if(upperLayerPara.upgrComBatTaskEn.flag == true){
		if(TickOut((uint32 *)&upperLayerPara.upgrComBatTaskEn.itickout, 5000) == true){
			TickOut((uint32 *)&upperLayerPara.upgrComBatTaskEn.itickout, 0);
			upperLayerPara.upgrComBatTaskEn.flag = false;
		}
	}

	if(upperLayerPara.upgrComBatTaskEn.flag == true){/*ִ��Զ����������*/
		if(TickOut((uint32 *)&upperLayerPara.upgrComBatTaskEn.itick, 800) == true){
			TickOut((uint32 *)&upperLayerPara.upgrComBatTaskEn.itick, 0);
			if(resumeFlag == true){/*����App�ӿں�������*/
				resumeFlag = false;
				OSTaskSuspend(UpperLayerParseTask_Prio);
				OSTaskSuspend(UpperLayerReplyTask_Prio);
			}
			
			if(resFlag == false){/*�ָ�Զ�������ӿں���*/
				resFlag = true;
				OSTaskResume(UgradeULParseTask_Prio);
				/*
				** �ָ�����֮ǰ�峬ʱ��ʱ��
				*/
				TickOut((uint32 *)&upperLayerPara.upgrComBatTaskEn.itickout, 0);
			}
			/*
			** ִ����������˸�߼�--���� -- ע��:����������˸��Ӳ��δ��
			*/
			UpgradeUL_CtrLedLogic(true);
		}
	}else{/*ִ��App�ӿں�������*/
		if(resumeFlag == false){/*�ָ�App�ӿ�����*/
			resumeFlag = true;
			OSTaskResume(UpperLayerParseTask_Prio);
			OSTaskResume(UpperLayerReplyTask_Prio);
		}

		if(resFlag == true){/*����Զ�������ӿں���*/
			resFlag = false;
			OSTaskSuspend(UgradeULParseTask_Prio);
		}
		/*
		** ִ����������˸�߼�--�ر�
		*/
		UpgradeUL_CtrLedLogic(false);		
	}	
}

/*
** set �ָ��ϲ�Э��֮APPӦ�ò�Э���־
*/
void set_CtrBoardTaskRecoverFlag(void){
	upperLayerPara.upgrComBatTaskDis.flag = true;
	TickOut((uint32 *)&upperLayerPara.upgrComBatTaskDis.itick, 0);
}

/*
** �ָ��ϲ�Э��֮APPӦ�ò�Э�� 
**		���Ʋ���:
**				1.�ļ��������֮��,���ñ�־
**				2.�ȴ��ظ�(��ʱ)800ms֮�����������л�
*/
void ctrBoard_TaskRecover(void){
	if(upperLayerPara.upgrComBatTaskDis.flag == true){
		if(TickOut((uint32 *)&upperLayerPara.upgrComBatTaskDis.itick, 800) == true){
			TickOut((uint32 *)&upperLayerPara.upgrComBatTaskDis.itick, 0);
			upperLayerPara.upgrComBatTaskEn.flag = false;
			/*
			** ��ָ������־
			*/
			upperLayerPara.upgrComBatTaskDis.flag = false;
		}
	}
}

/*
** ���������Ϣ֮�����ļ�
*/
extern bool oneWaringResetSetFlag;
extern SeqQueue comQueue;/*����������в���,����Ŀ�����*/
void cabCtrInfo_EventId_UpgradeFileCmd(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	UpgrFilePara upgr = {0};
	DetailedInfo detailedInfoTemp = {0};
	uint16 ver = 0;
	uint16 getVer = 0;
	int16 loc = 0;
	uint8 upgrType = 0;
	uint8 cmdWay = 0;
	ComBup curComBupTemp = getCurComBup();
	bool newIncrease = false;
	DoorNumDefine batUpgringFlag = 0;/*��ƽ���������*/
	DoorNumDefine chargeringFlag = 0;

	/*
	** ��������
	*/
	upgrType = rx[len];
	tx[txLen] = rx[len];
	len += sizeof(uint8);
	txLen += sizeof(uint8);
	/*
	** ��ȡӲ���汾��,ID
	*/
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&rx[len],sizeof(uint16));
	memcpy((uint8*)&tx[txLen],(uint8*)&rx[len],sizeof(uint16));
	len += sizeof(uint16);
	txLen += sizeof(uint16);
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.id[0],(uint8*)&rx[len],16);
	memcpy((uint8*)&tx[txLen],(uint8*)&rx[len],16);
	len += 16;
	txLen += 16;
	/*
	** �Ƿ���Ҫ��������--���ư�,ͨѶ������,��ذ��̼����ݷ���
	*/
	tx[txLen] = 0x01;/*����*/
	switch(upgrType){
		case 0x01:/*���ư�*/
			/*ver = (uint16)get_CtrVer();*/
			ver = get_CtrSoftVer();
			/*
			** ��������Ӳ���汾��Ϣ
			*/
			detailedInfoTemp = upgr.upgrFileType.detailedInfo;
			break;
		case 0x02:/*ͨѶ��*/
			/*
			** ���·ֿ�Ӳ���汾
			*/
			detailedInfoTemp = upgr.upgrFileType.detailedInfo;
			upgr = get_UpgrComFilePara();
			ver = upgr.upgrFileType.softVer;/*����汾��*/
			break;
		case 0x03:/*��ذ�*/
			loc = get_BatLocation(upgr);
			if(loc == -1){
// 				loc = get_batFireSize()/*get_SizeBatFile()*/ + 1;
// 				set_SizeBatFile(loc);
// 				set_AssignUpgrBinFilePara(upgr, loc);
				newIncrease = true;
				ver = 0;
				tx[txLen] = 0x02;/*������*/
			}else{
				ver = get_UpgrBatFilePara(loc).upgrFileType.softVer;
			}
			/*
			** ���µ�ذ���ϸ��Ϣ
			*/
			detailedInfoTemp = upgr.upgrFileType.detailedInfo; 
			break;
		case 0x04:/*������̼���*/
			loc = get_ChargeLocation(upgr);
			if(loc == -1){
// 				loc = get_ChargeFireSize() + 1;
// 				set_ChargeFireSize(loc);
// 				set_AssignUpgrChargeBinFilePara(upgr, loc);
				newIncrease = true;
				ver = 0;
				tx[txLen] = 0x02;/*������*/				
			}else{
				ver = get_UpgrChargeFilePara(loc).upgrFileType.softVer;
			}
			/*
			** ���³��������ϸ��Ϣ
			*/
			detailedInfoTemp = upgr.upgrFileType.detailedInfo;
			break;
	}
	txLen += sizeof(uint8); 
	
	/*
	** ����ID--������ʽ
	*/
	cmdWay = rx[len];
	tx[txLen] = rx[len];
	len += sizeof(uint8);
	txLen += sizeof(uint8);
	/*
	** get ����汾��
	*/
	memcpy((uint8*)&getVer,(uint8*)&rx[len],sizeof(uint16));
	len += sizeof(uint16);
	/*
	** ����ִ��״̬
	** 1.Ԥ��1������,��ָֹ�����
	** 2.�ж���ذ���׿������,ͬ���ص��ֿ�֮��δ��ͻ -- ��������:���س�����̼���������׿�ͷֿس�ͻ,���ͬһ�����ļ�
	** 3.����:20210303--��������ǰ�����ж��Ƿ�������ص��ֿ����ع̼� -- (�����������������ȴ��ֿ���������(20210303ȡ����ע))
	**		3.1--20210525--����:���ڵ�������Լ���������������ؾܾ�����--��ƽ�����
	** 4.�ֿش�ʱ�˿�δ����
	*/
	batUpgringFlag = (upperLayerPara.stateInfoChange.sysLogic.batUpgr & ((DoorNumDefine)0x0000FFFFFFFFFFFF));
	chargeringFlag = (upperLayerPara.stateInfoChange.sysLogic_2.chargeUpgr & ((DoorNumDefine)0x0000FFFFFFFFFFFF));
	if(Size_SeqQueue((SeqQueue *)&comQueue) <= (SeqQueue_DepthLimit - 1)){
		if(((memcmp((uint8*)&curComBupTemp.detailedInfo.hardVer,(uint8*)&upgr.upgrFileType.detailedInfo.hardVer,sizeof(DetailedInfo)) != 0)
			&& ((upgrType == 0x03)||(upgrType == 0x04)))
			|| ((upgrType == 0x01) && (curComBupTemp.binFileType.flag == 0) && (batUpgringFlag == 0) && (chargeringFlag == 0))
			|| ((upgrType == 0x02) && (curComBupTemp.binFileType.flag != ComBoradType))
			){
			tx[txLen] = 0x01;/*��������*/
			switch(cmdWay){
				case 0x00:/*��������*/
				case 0x01:/*ǿ������*/
					if((getVer == ver || newIncrease == true)
						/*
						** ע��:���ذ�,�ֿذ�����Ӳ���汾�ж�
						*/
						#if CtrHardware == CtrHardware_Dwin
						|| ((upgrType == 0x01 || upgrType == 0x02)&&(detailedInfoTemp.hardVer != (uint16)100))
						#elif  CtrHardware == CtrHardware_Andriod
						/*ע��:�����޸����ܳ����Ӳ���汾Ϊ300*/
						|| ((upgrType == 0x01 || upgrType == 0x02)&&(detailedInfoTemp.hardVer != (uint16)300/*200*/))
						#endif
						/*����:һ��������ֹ����*/
						|| oneWaring_ForbidUpgrDownFile() == true	
						|| oneWaringResetSetFlag == true/*20210302һ���澯�ѷ���,�µ�ָ�֮����������¿�ʼ����Ƿ�����ļ�*/
						){
						tx[txLen] = 0x02;/*�ܾ�����*/
						newIncrease = false;
					}else{
						if(upgrType == 0x01){/*���ư�*/
							upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.ctrUpgr = true;
							enterCtrBootFlag();
						}else{/*ͨѶ��/��ذ�--����:������̼���*/
							enterComBatFileDownload();
						}
						downloadComBatFileUpdatePara(cmdWay, upgrType, detailedInfoTemp);
					}
					break;
// 				case 0x01:/*ǿ������*/
// 					#if CtrHardware == CtrHardware_Dwin
// 					if(((upgrType == 0x01 || upgrType == 0x02)&&(detailedInfoTemp.hardVer != (uint16)100))
// 						|| (newIncrease == true)){
// 						tx[txLen] = 0x02;/*�ܾ�����*/
// 						newIncrease = false;
// 					}else
// 					#elif CtrHardware == CtrHardware_Andriod
// 					if(((upgrType == 0x01 || upgrType == 0x02)&&(detailedInfoTemp.hardVer != (uint16)200))
// 						|| (newIncrease == true)){
// 						tx[txLen] = 0x02;/*�ܾ�����*/
// 						newIncrease = false;
// 					}else
// 					#endif
// 					{
// 						if(upgrType == 0x01){/*���ư�*/
// 							enterCtrBootFlag();
// 						}else{/*ͨѶ��/��ذ�*/
// 							enterComBatFileDownload();
// 						}
// 						downloadComBatFileUpdatePara(cmdWay, upgrType, detailedInfoTemp);
// 					}
// 					break;
			}
		}else{
			tx[txLen] = 0x04;/*������æ��*/
		}
	}else{
		tx[txLen] = 0x04;/*������æ��*/
	}
	txLen += sizeof(uint8);

	/*
	** �汾��
	*/
	memcpy((uint8*)&tx[txLen],(uint8*)&ver,sizeof(uint16));
	txLen += sizeof(uint16);

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;

	/*
	** ��������--���������Կ
	*/
	if(upperLayerPara.upgrCtrReset.flag == true){
		set_UpgrCtrKey();
	}
}

/*
** ���������Ϣ֮��ȡ�ļ�����
*/
void cabCtrInfo_EventId_GetFileName(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint8 fileType = 0;
	uint16 ver = 0;
	UpgrFilePara upgr = {0};
	UpgrFilePara upgrTemp = {0};
	int8 loc = -1;
	uint8 fileNameLen = 0;

	/*
	** �ļ�����
	*/
	fileType = rx[len];
	tx[txLen] = rx[len];
	txLen += sizeof(uint8);
	len += sizeof(uint8);

	switch(fileType){/*�ļ�����*/
		case 0x01:/*���ư�*/
			/*ver = (uint16)get_CtrVer();*/
			ver = get_CtrSoftVer();
			break;
		case 0x02:/*ͨѶ��*/
			upgr = get_UpgrComFilePara();
			ver = upgr.upgrFileType.softVer;
			break;
		case 0x03:/*��ذ�*/
			/*Ӳ���汾��,ID*/
			memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&rx[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&upgr.upgrFileType.detailedInfo.id[0],(uint8*)&rx[len],16);
			len += 16;
			loc = get_BatLocation(upgr);
			if(loc != -1){
				upgrTemp = get_UpgrBatFilePara(loc);
				ver = upgrTemp.upgrFileType.softVer;
			}
			break;
		case 0x04:/*������̼���*/
			/*Ӳ���汾��,ID*/
			memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&rx[len],sizeof(uint16));
			len += sizeof(uint16);
			memcpy((uint8*)&upgr.upgrFileType.detailedInfo.id[0],(uint8*)&rx[len],16);
			len += 16;
			loc = get_ChargeLocation(upgr);
			if(loc != -1){
				upgrTemp = get_UpgrChargeFilePara(loc);
				ver = upgrTemp.upgrFileType.softVer;
			}
			break;
	}

	/*
	** ��������Ƿ�Ϊ��ʼ�汾,��ʼ�汾���ļ���
	*/
	if(ver != 0){
		tx[txLen] = 0x01;/*�����ɹ�*/
		txLen += sizeof(uint8);
		switch(fileType){
			case 0x01:/*���ư�*/
				get_CtrFileName((uint8 *)&tx[txLen], (uint8 *)&fileNameLen);
				break;
			case 0x02:/*ͨѶ��*/
				upgr = get_UpgrComFilePara();
				memcpy((uint8*)&tx[txLen],(uint8*)&upgr.upgrFileType.fileName[0],upgr.upgrFileType.fileNameLen);
				fileNameLen = upgr.upgrFileType.fileNameLen;
				break;
			case 0x03:/*��ذ�*/
				upgrTemp = get_UpgrBatFilePara(loc);
				memcpy((uint8*)&tx[txLen],(uint8*)&upgrTemp.upgrFileType.fileName[0],upgrTemp.upgrFileType.fileNameLen);
				fileNameLen = upgrTemp.upgrFileType.fileNameLen;
				break;
			case 0x04:/*������̼���*/
				upgrTemp = get_UpgrChargeFilePara(loc);
				memcpy((uint8*)&tx[txLen],(uint8*)&upgrTemp.upgrFileType.fileName[0],upgrTemp.upgrFileType.fileNameLen);
				fileNameLen = upgrTemp.upgrFileType.fileNameLen;				
				break;
		}
		
	}else{
		tx[txLen] = 0x02;/*����ʧ��*/
		txLen += sizeof(uint8);
	}
	txLen += fileNameLen;

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** ���������Ϣ֮��ȡָ����ذ��̼���Ϣ -- 0x0090 -->����ʹ����ʼ��ŵķ�ʽ��ѯ��ع̼����Ĺ̼���Ϣ
*/
void cabCtrInfo_EventId_GetBatFireInfo(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint16 loc = 0;
	UpgrFilePara upgr = {0};

	/*
	** λ����Ϣ
	*/ 
	memcpy((uint8*)&loc,(uint8*)&rx[len],sizeof(uint8));
	memcpy((uint8*)&tx[txLen],(uint8*)&rx[len],sizeof(uint8));
	txLen += sizeof(uint16);
	len += sizeof(uint16);

	/*
	** �ж�λ����Ϣ�Ƿ�Ϸ�
	*/
	if(loc <= get_batFireSize()){/*�Ϸ�*/
		tx[txLen] = 0x01;
		txLen += sizeof(uint8);
		/*
		** Ӳ���汾,ID,����汾
		*/
		upgr = get_UpgrBatFilePara(loc);
		memcpy((uint8*)&tx[txLen],(uint8*)&upgr.upgrFileType.detailedInfo.hardVer,sizeof(uint16));
		txLen += sizeof(uint16);
		memcpy((uint8*)&tx[txLen],(uint8*)&upgr.upgrFileType.detailedInfo.id[0],16);
		txLen += 16;
		memcpy((uint8*)&tx[txLen],(uint8*)&upgr.upgrFileType.softVer,sizeof(uint16));
		txLen += sizeof(uint16);
	}else{/*���Ϸ�*/
		tx[txLen] = 0x02;
		txLen += sizeof(uint8);
	}

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** ��ѯ���г�������¶�
*/
extern LowerLayerParam llParam;
void cabCtrInfo_EventId_AllchgTemp(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 i = 0;

	for(i=0;i<SysCtr_AllDoorNum;i++){
		memcpy((uint8*)&tx[txLen],(uint8*)&llParam.batDoor[i].batDoorSysPara.chargerTemp,sizeof(uint16));/*������¶�*/	
		txLen += sizeof(uint16);
	}

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}

/*
** ��ѯBMS����״̬/����״̬
*/
void cabCtrInfo_EventId_BmsPFState(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 i = 0;

	for(i=0;i<SysCtr_AllDoorNum;i++){
		/*
		** ��ر���״̬
		*/
		memcpy((uint8*)&tx[txLen],(uint8*)&llParam.batDoor[i].bmsInfoMeiTuan.pState.flag,sizeof(uint16));
		txLen += sizeof(uint16);
		/*
		** ��ع���״̬
		*/
		tx[txLen] = llParam.batDoor[i].bmsInfoMeiTuan.faultState.flag;
		txLen += sizeof(uint8);
	}

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}

/*
** ��ȡWIFI���ƺ�����
*/
void cabCtrInfo_EventId_GetWifiNamePw(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 legalLen = 0;
	WifiSet wifiSetTemp = get_WifiNamePwSysSet();

	legalLen = get_WifiNamePwIsLegalLen((uint8 *)&wifiSetTemp.name[0]);
	tx[txLen] = legalLen;
	txLen += sizeof(uint8);
	memcpy((uint8*)&tx[txLen],(uint8*)&wifiSetTemp.name[0],legalLen);
	txLen += legalLen;

	legalLen = get_WifiNamePwIsLegalLen((uint8 *)&wifiSetTemp.pw[0]);
	tx[txLen] = legalLen;
	txLen += sizeof(uint8);
	memcpy((uint8*)&tx[txLen],(uint8*)&wifiSetTemp.pw[0],legalLen);
	txLen += legalLen;	

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** ��ѯSOC��ֵ
*/
void cabCtrInfo_EventId_ChkSocLimit(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/

	/*
	** SOC��ֵ
	*/
	tx[txLen] = get_SocLimit();
	txLen += sizeof(uint8);

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** ��ѯͨѶ�����а汾��/��ع̼��汾�� -- �¼�:0x00F0 -- �ֿ�ʵ�����зֿ�����汾��,��ع̼�����汾��,���������汾�� -- 20210301 -- �ֽ׶�:��׿�Ŀǰ���������зֿ�ʵ�����еİ汾��
*/
void cabCtrInfo_EventId_ChkRunComVer(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint16 loc = 0;

	/*
	** ��ѯλ����Ϣ
	*/
	memcpy((uint8*)&loc,(uint8*)&rx[len],sizeof(uint16));
	memcpy((uint8*)&tx[txLen],(uint8*)&rx[len],sizeof(uint16));
	txLen += sizeof(uint16);
	len += sizeof(uint16);	

	/*
	** ��ѯλ����Ϣ�Ƿ�Ϸ�
	*/
	if(loc < (get_batFireSize() + 1)/*ͨѶ��͵�ع̼���*/
		|| (loc >= 0x80 && loc < (0x80+ get_ChargeFireSize()))/*������̼���*/
	){/*�Ϸ�*/
		/*
		** ת��������̼���ƫ�Ƶ�ַ
		*/
		if(loc >= 0x80 && loc < (0x80+ get_ChargeFireSize())){
			loc = loc - 0x80 + 36;
		}
	
		tx[txLen] = 0x01;
		txLen += sizeof(uint8);			
		memcpy((uint8*)&tx[txLen],(uint8*)&upperLayerPara.runComVer[loc][0],(uint8)(SysCtr_AllDoorNum*sizeof(uint16)));
		txLen += (SysCtr_AllDoorNum*sizeof(uint16));
	}else{/*���Ϸ�*/
		tx[txLen] = 0x02;
		txLen += sizeof(uint8);		
	}

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}

/*
** ������Ӧ�̼����������� -- �¼�ID:0x0100
*/
void cabCtrInfo_EventId_CfgFireUpgrNum(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint8 fireType = 0;
	UpgrFilePara upgr = {0};
	uint8 cmdExceState = 0x02;/*����ִ��ʧ��*/
	int16 loc = -1;
	int16 upgrNum = -1;

	/*�̼�������*/
	fireType = rx[len];
	tx[txLen] = rx[len];
	len += sizeof(uint8);
	txLen += sizeof(uint8);
	/*Ӳ���汾,ID*/
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&rx[len],sizeof(uint16));
	len += sizeof(uint16);
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.id[0],(uint8*)&rx[len],16);
	len += 16;
	memcpy((uint8*)&tx[txLen],(uint8*)&upgr.upgrFileType.detailedInfo.hardVer,sizeof(DetailedInfo));
	txLen += sizeof(DetailedInfo);
	
	/*��������*/
	memcpy((uint8*)&upgrNum,(uint8*)&rx[len],sizeof(uint16));

	switch(fireType){
		case 0x03:/*��ع̼���*/
			loc = get_BatLocation(upgr);
			break;
		case 0x04:/*������̼���*/
			loc = get_ChargeLocation(upgr);
			break;
	}
	
	if(loc != -1){/*�̼����ҳɹ�*/
		cmdExceState = 0x01;
		set_FireUpgrNum(fireType, upgrNum, loc);
		/*��ʵ����������*/
		set_FireRealityRunNum(fireType, 0, loc);
	}

	/*����ִ��״̬*/
	tx[txLen] = cmdExceState;
	txLen += sizeof(uint8);

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** �ظ������Ӧ�̼��� -- �¼�ID:0x0110
*/
void cabCtrInfo_EventId_ClearFire(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint8 fireType = 0;
	UpgrFilePara upgr = {0};
	UpgrFilePara upgrTemp = {0};
	int16 loc = -1;
	uint8 cmdExceState = 1;/*ִ�гɹ�*/
	
	/*�̼�������*/
	fireType = rx[len];
	tx[txLen] = rx[len];
	len += sizeof(uint8);
	txLen += sizeof(uint8);
	/*Ӳ���汾,ID*/
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&rx[len],sizeof(uint16));
	len += sizeof(uint16);
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.id[0],(uint8*)&rx[len],16);
	len += 16;
	memcpy((uint8*)&tx[txLen],(uint8*)&upgr.upgrFileType.detailedInfo.hardVer,sizeof(DetailedInfo));
	txLen += sizeof(DetailedInfo);	

	switch(fireType){
		case 0x03:/*��ع̼���*/
			loc = get_BatLocation(upgr);
			if(loc != -1){
				upgrTemp = get_UpgrBatFilePara(loc);
				if(upgrTemp.upgrFileType.softVer != 0){
					/*�����ع̼�������*/
					memset((uint8*)&upgrTemp.upgrFileType.softVer,0,sizeof(UpgrFilePara)-sizeof(uint8)-sizeof(uint8)-sizeof(DetailedInfo));
					/*���¹̼��洢����*/
					set_AssignUpgrBinFilePara(upgrTemp, loc);
					/*���÷ֿ�����̼�����־*/
					set_ClearFire(fireType,true,loc);
				}else{
					cmdExceState = 2;/*��Ӧ�̼������*/
				}
			}else{
				cmdExceState = 4;/*�̼���Ϣ������*/
			}
			break;
		case 0x04:/*������̼���*/
			loc = get_ChargeLocation(upgr);
			if(loc != -1){
				upgrTemp = get_UpgrChargeFilePara(loc);
				if(upgrTemp.upgrFileType.softVer != 0){
					/*�����ع̼�������*/
					memset((uint8*)&upgrTemp.upgrFileType.softVer,0,sizeof(UpgrFilePara)-sizeof(uint8)-sizeof(uint8)-sizeof(DetailedInfo));
					/*���¹̼��洢����*/
					set_AssignUpgrChargeBinFilePara(upgrTemp, loc);
					/*���÷ֿ�����̼�����־*/
					set_ClearFire(fireType,true,loc);
				}else{
					cmdExceState = 2;/*��Ӧ�̼������*/
				}
			}else{
				cmdExceState = 4;/*�̼���Ϣ������*/
			}
			break;
	}
	/*����ִ��״̬*/
	tx[txLen] = cmdExceState;
	txLen += sizeof(uint8);

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}

/*
** ��Ӧ�ֹ̼�����ʧ��ԭ���ϱ� -- �¼�ID:0x0120
*/
void cabCtrInfo_EventId_UpgrFailReson(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint8 fireType = 0;
	uint8 addr = 0;
	int8 cmd = -1;
	static UpgrFail upgrFailTemp[SysCtr_AllDoorNum] = {0};
	static UpgrFail upgrChargerFailTemp[SysCtr_AllDoorNum] = {0};
	UpgrFail* upgrFailTempp = NULL;
	uint8 cmdExceState = 1;/*δ���յ�����ԭ���ϱ�����*/
	uint8 i = 0;

	/*�̼�����*/
	fireType = rx[len];
	tx[txLen] = rx[len];
	len += sizeof(uint8);
	txLen += sizeof(uint8);
	/*�豸��ַ*/
	addr = rx[len];
	tx[txLen] = rx[len];
	len += sizeof(uint8);
	txLen += sizeof(uint8);
	/*����*/
	cmd = rx[len];
	switch(cmd){
		case 0x01:/*��ѯ����ʧ��ԭ��*/
			switch(fireType){
				case 0x03:/*��ع̼���*/
					if(upgrFailTemp[addr].bat.reson == 0){
						upgrFailTempp = (UpgrFail*)get_UpgrFailPara(0x03,addr);
						if(upgrFailTempp == NULL){/*��������:��׿��ѯʧ��ԭ���ڷֿ��ϱ�ʧ��ԭ��֮ǰ,����������ʱ(sleep(200ms)),���¶�ȡ*/
							for(i=0;i<40;i++){/*�˴�������ʱ���ʱ��Ϊ:200ms=5*40*/
								Sleep(5);
								upgrFailTempp = (UpgrFail*)get_UpgrFailPara(0x03,addr);
								if(upgrFailTempp != NULL){
									break;
								}
							}
						}
						if(upgrFailTempp != NULL){
							upgrFailTemp[addr] = *(UpgrFail*)upgrFailTempp;
							cmdExceState = 2;/*ִ������ԭ���ϱ�*/
						}
					}else{/*�ϱ�������ʧ��,��δȷ���������ʧ��ԭ�����*/
						cmdExceState = 2;/*ִ������ԭ���ϱ�*/
					}
					break;
				case 0x04:/*������̼���*/
					if(upgrChargerFailTemp[addr].bat.reson == 0){
						upgrFailTempp = (UpgrFail*)get_UpgrFailPara(0x04,addr);
						if(upgrFailTempp != NULL){
							upgrChargerFailTemp[addr] = *(UpgrFail*)upgrFailTempp;
							cmdExceState = 2;/*ִ������ԭ���ϱ�*/
						}
					}else{/*�ϱ�������ʧ��,��δȷ���������ʧ��ԭ�����*/
						cmdExceState = 2;/*ִ������ԭ���ϱ�*/
					}					
					break;
			}
			break;
		case 0x02:/*�����Ӧʧ��ԭ��*/
			switch(fireType){
				case 0x03:/*��ع̼���*/
					memset((uint8*)&upgrFailTemp[addr],0,sizeof(UpgrFail));
					cmdExceState = 4;/*����ɹ�*/
					break;
				case 0x04:/*������̼���*/
					memset((uint8*)&upgrChargerFailTemp[addr],0,sizeof(UpgrFail));
					cmdExceState = 4;/*����ɹ�*/					
					break;
			}			
			break;
	}
	tx[txLen] = cmdExceState;
	txLen += sizeof(uint8);

	/*�������ʧ��ԭ���ֶβ���*/
	if(cmdExceState == 0x02){
		switch(fireType){
			case 0x03:/*��ع̼�*/
				memcpy((uint8*)&tx[txLen],(uint8*)&upgrFailTemp[addr].bat.reson,sizeof(UpgrFail));
				break;
			case 0x04:/*������̼�*/
				memcpy((uint8*)&tx[txLen],(uint8*)&upgrChargerFailTemp[addr].bat.reson,sizeof(UpgrFail));
				break;
		}
		txLen += sizeof(UpgrFail);
	}

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** �̼�����ģʽ -- �¼�ID:0x0130
*/
void cabCtrInfo_EventId_FireEnterTestModel(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint8 fireType = 0;	
	UpgrFilePara upgr = {0};
	uint8 cmdExceState = 4;/*�̼�������*/
	int16 loc = -1;

	/*�̼�����*/
	fireType = rx[len];
	tx[txLen] = fireType;
	len += sizeof(uint8);
	txLen += sizeof(uint8);
	/*Ӳ���汾,ID*/
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&rx[len],sizeof(uint16));
	len += sizeof(uint16);
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.id[0],(uint8*)&rx[len],16);
	len += 16;
	memcpy((uint8*)&tx[txLen],(uint8*)&upgr.upgrFileType.detailedInfo.hardVer,sizeof(DetailedInfo));
	txLen += sizeof(DetailedInfo);	

	switch(fireType){
		case 0x03:/*��ع̼���*/
			loc = get_BatLocation(upgr);
			break;
		case 0x04:/*������̼���*/
			loc = get_ChargeLocation(upgr);
			break;
	}
	if(loc != -1){
		cmdExceState = 1;/*ִ�гɹ�*/
		/*���ù̼��������ģʽ*/
		set_FireEnterTestModel(fireType, true,loc);
		/*���½������ģʽ,����ϴβ���ģʽ,��������*/
		set_TestModelFireUpgrNum(fireType, 0, loc);/*��ɹ�����*/
		set_TestModelFireUpgrNumFail(fireType,  0, loc);/*��ʧ�ܴ���*/
		/*����ģʽ--�������ҪֻҪ�����κ�һ�ֹ̼��������ģʽ,����λ*/
		upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.testModelFlag = true;
	}
	
	/*ִ��״̬*/
	tx[txLen] = cmdExceState;
	txLen += sizeof(uint8);

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}

/*
** �����ƹ̼����������������,��ѯʵ�������Ĵ��� -- �¼�ID:0x0140 -- Pre OTA ͳ����������
*/
void cabCtrInfo_EventId_ChkRealityUpgrNum(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint8 fireType = 0;
	UpgrFilePara upgr = {0};
	int16 loc = -1;
	uint8 cmdExceState = 4;/*�̼���Ϣ������*/
	uint16 realityUpgrNum = 0;/*ʵ����������*/

	/*�̼�������*/
	fireType = rx[len];
	tx[txLen] = rx[len];
	len += sizeof(uint8);
	txLen += sizeof(uint8);
	/*Ӳ���汾,ID*/
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&rx[len],sizeof(uint16));
	len += sizeof(uint16);
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.id[0],(uint8*)&rx[len],16);
	len += 16;
	memcpy((uint8*)&tx[txLen],(uint8*)&upgr.upgrFileType.detailedInfo.hardVer,sizeof(DetailedInfo));
	txLen += sizeof(DetailedInfo);	

	switch(fireType){
		case 0x03:/*��ذ��̼�*/
			loc = get_BatLocation(upgr);
			if(loc != -1){/*�̼����ҵ�*/
				if(get_FireUpgrNum(0x03, loc) == (int16)-1){/*�̼����������޴�������*/
					cmdExceState = 1;/*�̼�������������*/
				}else{
					cmdExceState = 2;/*�̼�����������������*/
				}
			}
			break;
		case 0x04:/*������̼�*/
			loc = get_ChargeLocation(upgr);
			if(loc != -1){/*�̼����ҵ�*/
				if(get_FireUpgrNum(0x04, loc) == (int16)-1){/*�̼����������޴�������*/
					cmdExceState = 1;/*�̼�������������*/
				}else{
					cmdExceState = 2;/*�̼�����������������*/
				}
			}			
			break;
	}
	/*�Ƿ���ڹ̼����ƴ���*/
	tx[txLen] = cmdExceState;
	txLen += sizeof(uint8);

	if(cmdExceState == 2){/*�̼�����������������*/
		realityUpgrNum = get_FireRealityRunNum(fireType, loc);
		memcpy((uint8*)&tx[txLen],(uint8*)&realityUpgrNum,sizeof(uint16));
		txLen += sizeof(uint16);
	}

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** ����ģʽ��,��ػ������������� -- �¼�ID:0x0150 -- ͳ��ѹ��-->�����ɹ�������ʧ�ܴ���
*/
void cabCtrInfo_EventId_TestModelFireUpgrNum(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint8 fireType = 0;
	UpgrFilePara upgr = {0};
	int16 loc = -1;
	uint8 cmdExceState = 2;/*Ӳ���汾/�̼�ID���Ϸ�*/
	uint16 testModelUpgrNum = 0;/*�ɹ�����*/
	uint16 testModelUpgrNumFail = 0;/*ʧ�ܴ���*/

	/*�̼�������*/
	fireType = rx[len];
	tx[txLen] = rx[len];
	len += sizeof(uint8);
	txLen += sizeof(uint8);
	/*Ӳ���汾,ID*/
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&rx[len],sizeof(uint16));
	len += sizeof(uint16);
	memcpy((uint8*)&upgr.upgrFileType.detailedInfo.id[0],(uint8*)&rx[len],16);
	len += 16;
	memcpy((uint8*)&tx[txLen],(uint8*)&upgr.upgrFileType.detailedInfo.hardVer,sizeof(DetailedInfo));
	txLen += sizeof(DetailedInfo);	

	switch(fireType){
		case 0x03:/*��ذ��̼�*/
			loc = get_BatLocation(upgr);
			if(loc != -1){
				cmdExceState = 1;
				testModelUpgrNum = get_TestModelFireUpgrNum(0x03, loc);
				testModelUpgrNumFail = get_TestModelFireUpgrNumFail(0x03, loc);
			}
			break;
		case 0x04:/*chongdian*/
			loc = get_ChargeLocation(upgr);
			if(loc != -1){
				cmdExceState = 1;
				testModelUpgrNum = get_TestModelFireUpgrNum(0x04, loc);
				testModelUpgrNumFail = get_TestModelFireUpgrNumFail(0x04, loc);
			}
			break;
	}
	/*����ִ��״̬*/
	tx[txLen] = cmdExceState;
	txLen += sizeof(uint8);

	if(cmdExceState == 1){
		memcpy((uint8*)&tx[txLen],(uint8*)&testModelUpgrNum,sizeof(uint16));
		txLen += sizeof(uint16);
		/*ʧ�ܴ���*/
		memcpy((uint8*)&tx[txLen],(uint8*)&testModelUpgrNumFail,sizeof(uint16));
		txLen += sizeof(uint16);		
	}

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}

/*
** Charger OC Limit Cfg -- �¼�:0x0160
*/
void cabCtrInfo_EventId_ChargerOCLimitCfg(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/

	tx[txLen] = 0x01;/*���óɹ�*/
	txLen += sizeof(uint8);
	/*�����������ֵ*/
	tx[txLen] = rx[0];
	txLen += sizeof(uint8);
	/*set Charger OC Limit*/
	set_ChargerOCLimit(rx[0]);

	/*
	** ��������ֶ����+������ȷ���
	*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}

/*-------------------------------------------�����ѹУ׼ֵ--------------------------------------------------------------------------*/
/*
** ��ѯ��ѹ��AD����ֵ
*/
void cabCtrInfo_EventId_ChkPhaseLvVoltAD(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint8 ph = 0;
	uint16 lvAdValue = 0;

	/*����*/
	ph = rx[len];
	/*��ȡ��ѹ��ADֵ*/
	lvAdValue = get_LvAdValue(ph);
	/*��������*/
	memcpy((uint8*)&tx[txLen],(uint8*)&lvAdValue,sizeof(uint16));
	txLen += sizeof(uint16);

	/*��������ֶ����+������ȷ���*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}

/*
** ��ѯ��ѹ�����ֵ
*/
void cabCtrInfo_EventId_ChkPhaseHvVoltAD(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint8 ph = 0;
	uint16 hvAdValue = 0;

	/*����*/
	ph = rx[len];
	/*��ȡ��ѹ��ADֵ*/
	hvAdValue = get_HvAdValue(ph);
	/*��������*/
	memcpy((uint8*)&tx[txLen],(uint8*)&hvAdValue,sizeof(uint16));
	txLen += sizeof(uint16);

	/*��������ֶ����+������ȷ���*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}

/*
** ���������ѹУ׼ϵ��
*/
void cabCtrInfo_EventId_SetPhaseAjust(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint8 ph = 0;
	float k = 0;
	float b = 0;

	/*����*/
	ph = rx[len];
	len += sizeof(uint8);
	/*����ϵ��*/
	memcpy((uint8*)&k,(uint8*)&rx[len],sizeof(float));
	len += sizeof(float);
	memcpy((uint8*)&b,(uint8*)&rx[len],sizeof(float));
	len += sizeof(float);	
	set_PhaseVoltAdjust(ph, true, k,  b);

	/*��������ֶ����+������ȷ���*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}

/*
** ��ȡʵʱ��ѹ
*/
void cabCtrInfo_EventId_GetRuntimeVolt(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint8 ph = 0;
	uint16 temp = 0;
	CalcPhaseVC pvc = get_CalcPhaseVC();

	/*����*/
	ph = rx[len];
	len += sizeof(uint8);
	/*��ȡʵʱ��ѹ*/
	temp = (uint16)(pvc.abcPV[ph]);
	memcpy((uint8*)&tx[txLen],(uint8*)&temp,sizeof(uint16));
	txLen += sizeof(uint16);

	/*��������ֶ����+������ȷ���*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}
/*-----------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------����̼�------------------------------------------------------------------*/
/*
** һ�����--�ֿ�/���/�����
*/
void cabCtrInfo_EventId_OneKeyClearFire(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	uint32 clearFireField = 0;/*����̼�*/
	UpgrFilePara upgrTemp = {0};
	uint8 i = 0;

	/*�̼��ֶ�*/
	memcpy((uint8*)&clearFireField,(uint8*)&rx[len],sizeof(uint32));
	len += sizeof(uint32);

	/*����̼�*/
	if(clearFireField == 0xFFFFFFFF){
		/*����ֿع̼�*/
		upgrTemp.upgrFileType.board = ComBoradType;
		set_UpgrBinFilePara(upgrTemp);
		/*�����ع̼�*/
		init_BatPara();/*��ʼ����ع̼�*/
		/*���������̼�*/
		init_chargePara();/*��ʼ��������̼�*/

		/*20210330--����ƽ̨����ProOTA�رչ��ܴ��ڲ�������,��������λ�����*/
		for(i=0;i<get_batFireSize();i++){
			set_FireUpgrNum(0x03, -1, i);
			/*��ʵ����������*/
			set_FireRealityRunNum(0x03, 0, i);				
		}
		
		for(i=0;i<get_ChargeFireSize();i++){
			set_FireUpgrNum(0x04, -1, i);
			/*��ʵ����������*/
			set_FireRealityRunNum(0x04, 0, i);			
		}
	}

	/*��������ֶ����+������ȷ���*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;
}
/*-----------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------���ų���-��������������--��˽ӿ�(�ⲿ���)-1.05-----------------------------------------------------*/
/*
** ���ù��±�����ֵ
*/
void cabCtrInfo_EventId_SetBatOTempLimit(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	int8 temp = 60;/*��ʼ��ֵ*/
	uint8 cmdIsLegal = 0x02;/*��ֵ���Ϸ�*/
	
	/*������ֵ*/
	temp = rx[len];
	tx[txLen] = rx[len];
	len += sizeof(int8);
	txLen += sizeof(int8);

	/*��������ֵ�Ƿ�Ϸ�*/
	if(temp >= 50 && temp <= 70){
		cmdIsLegal = 0x01;/*�Ϸ�*/
		set_BatOTempLimit(temp);
	}

	/*����Ϸ��Իظ�*/
	tx[txLen] = cmdIsLegal;
	txLen += sizeof(uint8);

	/*��������ֶ����+������ȷ���*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}

/*
** �����϶����±�����ֵ
*/
void cabCtrInfo_EventId_SetNanduLowPLimit(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	int8 temp = 15;/*��ʼ��ֵ*/
	uint8 cmdIsLegal = 0x02;/*��ֵ���Ϸ�*/

	/*�϶�������ֵ*/
	temp = rx[len];
	tx[txLen] = rx[len];
	len += sizeof(int8);
	txLen += sizeof(int8);

	/*��������ֵ�Ƿ�Ϸ�*/
	if(temp >= -20 && temp <= 20){
		cmdIsLegal = 0x01;/*�Ϸ�*/
		set_NanduLowPLimit(temp);
	}

	/*����Ϸ��Իظ�*/
	tx[txLen] = cmdIsLegal;
	txLen += sizeof(uint8);

	/*��������ֶ����+������ȷ���*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;
}

/*
** ���õ��±�����ֵ
*/
void cabCtrInfo_EventId_SetBatLowPLimit(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 len = 0;
	int8 temp = 5;/*��ʼ��ֵ*/
	uint8 cmdIsLegal = 0x02;/*��ֵ���Ϸ�*/

	/*��ص��±�����ֵ*/
	temp = rx[len];
	tx[txLen] = rx[len];
	len += sizeof(int8);
	txLen += sizeof(int8);

	/*��������ֵ�Ƿ�Ϸ�*/
	if(temp >= -20 && temp <= 20){
		cmdIsLegal = 0x01;/*�Ϸ�*/
		set_BatLowPLimit(temp);
	}

	/*����Ϸ��Իظ�*/
	tx[txLen] = cmdIsLegal;
	txLen += sizeof(uint8);

	/*��������ֶ����+������ȷ���*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;	
}
/*-----------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------��ؽ���SOC-----------------------------------------------------------*/
void cabCtrInfo_EventId_ChkAllInsertSoc(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint8 i = 0;

	for(i=0;i<SysCtr_AllDoorNum;i++){
		memcpy((uint8*)&tx[txLen],(uint8*)&llParam.batDoor[i].batDoorSysPara.chgBefSoc,sizeof(uint8));
		txLen += sizeof(uint8);
	}

	/*��������ֶ����+������ȷ���*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}
/*-----------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------�����������־---------------------------------------------------*/
void cabCtrInfo_EventId_PhaseCurOpenFlag(uint8 itemDatalen,uint8* rx,
	uint8* replyItemDataLen,uint8* tx){
	uint8 txLen = 1;/*˵��:tx[0]Ԥ���洢�������(�����ֶ�+����������)*/
	uint32 setCurOpenChkFlag = 0;
	uint8 len = 0;
	uint8 phase = 0;

	/*������������־*/
	memcpy((uint8*)&setCurOpenChkFlag,(uint8*)&rx[len],sizeof(uint32));
	len += sizeof(uint32);

	if(setCurOpenChkFlag == (uint32)0xEEEEEEEE){/*���õ���Debugģʽ*/
		set_Phase_VC_Debug_Mode();
		phase = rx[len];/*��*/
		switch(phase){
			case 0:/*A��*/
			case 1:/*B��*/
			case 2:/*C��*/
				upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOC = true;/*������ܱ�־*/
				upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurO |= (uint16)((uint16)1<<phase); 		
				break;	
			case 3:/*ABC����*/
				upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOC = true;/*������ܱ�־*/
				upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurO = 0x07;
				break;
			case 4:/*�ָ�*/
				upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOC = false;
				upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurO = 0;
				break;
			case 5:/*A���·*/
			case 6:/*B���·*/
			case 7:/*C���·*/	
				upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseCShort = true;
				upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurShort |= (uint16)((uint16)1<<(phase - 5));
				break;
			case 8:/*ABC�����·*/
				upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseCShort = true;
				upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurShort = 7;					
				break;
			case 9:/*��·�ָ�*/
				upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseCShort = false;
				upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurShort = 0;				
				break;
		}
	}

	if(setCurOpenChkFlag == (uint32)0xBBBBBBBB){/*�ָ�Ĭ��ģʽ*/
		init_Phase_VC_Debug_Mode();
		upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOC = false;
		upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurO = 0;
		upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseCShort = false;
		upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurShort = 0;			
	}

	/*copy data*/
	memcpy((uint8*)&tx[txLen],(uint8*)&setCurOpenChkFlag,sizeof(uint32));
	txLen += sizeof(uint32);

	/*��������ֶ����+������ȷ���*/
	tx[0] =  txLen-sizeof(uint8);
	replyItemDataLen[0] = txLen;		
}
/*-----------------------------------------------------------------------------------------------------------------------------------*/

