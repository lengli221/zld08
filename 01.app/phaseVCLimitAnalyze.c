#include "includes.h"

extern Hmi_PhaseVC hmi_PhaseVc;
extern UpperLayerPara upperLayerPara;

/*
** �������ѹ/������Ƿ�����쳣		
*/
extern bool oneWaringResetSetFlag;

PhaseVCDebugMode phaseVCDebugMode = {0};

/*
** init Phase VC Debug Mode
*/
void init_Phase_VC_Debug_Mode(void){
	phaseVCDebugMode.flag = false;
}

/*
** set Phase VC Debug Mode
*/ 
void set_Phase_VC_Debug_Mode(void){
	phaseVCDebugMode.flag = true;	
}

/*
** ���ڵ��Ե�������
*/
CalcPhaseVC cpvsRunTime = {0};
PhaseVCAnalyze get_PhaseVCAnalyze(void){
	CalcPhaseVC cpvs = get_CalcPhaseVC();
	static PhaseVCAnalyze  pvca = {0};
	uint8 i = 0;
	static uint32 itickPV[3] = {0};
	static uint32 itickPC[3] = {0};
	static bool updateFlag = false;
	static bool powerOnFlag = false;/*�ϵ��־*/
	static uint32 itick = 0;/*�ϵ��ⶨʱ��*/
	static bool l_Flag = false;
	static bool h_Flag = false;
	static uint32 severityItick[3] = {0};/*���ع�ѹ��ʱ��--V1.5*/
	static uint8 phaseOverVFaultRecoverFlag = 0;/*�����ѹ���ع�ѹ�ָ���־*/
	static uint8 phaseLVRecoverFlag = 0;/*�����ѹǷѹ�ָ���־*/
	static uint8 phaseOverVWaringRecoverFlag = 0;/*�����ѹ��ѹ�ָ���־*/
	#if MaskOCur_ShortFun == 1
	static uint8 phaseOCRecoverFlag = 0;/*�����ѹ�����ָ���־*/
	#endif
	
	/*
	** ���ģ����տ��Ƿ�Ͽ�
	*/
	static uint32 chkTimCnt[3] = {0};
	union __VoltChk{
		uint8 flag;
		struct {
			uint8 a;
			uint8 b;
			uint8 c;
		}bits;
	}voltChk;
	/*
	** Ƿѹ�������2S����
	*/
	static uint32 itickLVContinuous[3] = {0};

/*���ι�ѹ�������*/
#if MaskOVCChk == 0
	if(powerOnFlag == false){
		TickOut((uint32 *)&itick, 0);/*��ѹ���������ʱ6S���*/
		powerOnFlag = true;
		TickOut((uint32 *)&severityItick[i], 0);/*���ع�ѹ��ʱ��--V1.5*/
	}
	
	if(TickOut((uint32 *)&itick, 6000) == true){
		/* ���ڵ��Ե�������*/		
		cpvsRunTime = get_CalcPhaseVC();
		
		/*
		** ��ѹ״̬����
		*/	
		if(updateFlag == false){
			for(i=0;i<3;i++){
				if(cpvs.abcPV[i] >= 270*1.414){
					/*��Ƿѹ��ⶨʱ��*/
					TickOut((uint32 *)&itickLVContinuous[i], 0);
					
					#if VerCtr == VerCtr_Normal
						pvca.pVol.flag |= (1<<i);
						/*TickOut((uint32 *)&itickPV[i], 0);*/
						
						/*updateFlag = true;*//*�������ѹ��־,����ֵ���ٻָ�*/
						
						/*����������������,���ѹ>270V���Զ��ָ�*/
						TickOut((uint32 *)&itickPV[i], 0);
						l_Flag = false;/*��ѹ��־*/
						h_Flag = true;/*��ѹ��־*/

						TickOut((uint32 *)&severityItick[i], 0);/*���ع�ѹ��ʱ��--����V1.5�汾*/
						phaseOverVFaultRecoverFlag = 0;
						
						hmi_PhaseVc.bits.hv = true;
						hmi_PhaseVc.bits.lv = false;
						/*�����ϴ����������ѹ��ѹ֮��,���µ�������ָܻ�*/
						upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOverVFault = true;
						/*20210128--���ع�ѹ֮��,�����ѹ��־*/
						upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOverVWaring = false;
						upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseLV = false;
					#endif
				}else{
					/*������뿪���ع�ѹ--�����Ӧ���ع�ѹ��־*/
					if(cpvs.abcPV[i]  < 264*1.414 &&TickOut((uint32 *)&severityItick[i], tickTimeBase_V) == true){
						phaseOverVFaultRecoverFlag |= ((uint8)(1<<i));
						TickOut((uint32 *)&severityItick[i], 0);
						if(phaseOverVFaultRecoverFlag == 0x07){/*�����ѹ�ѻָ�*/
							phaseOverVFaultRecoverFlag = 0;
							upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOverVFault = false;
						}
					}else{
						if(cpvs.abcPV[i]  > 266*1.414){/*�嶨ʱ��Ԥ����ֵ2V*/
							TickOut((uint32 *)&severityItick[i], 0);/*����:�ָ�֮���������1min�������ϱ������־λ*/
						}
					}
				
					if(cpvs.abcPV[i]  <= 166*1.414){
						/*Ƿѹ����������2S���ϲ��ж�Ƿѹ*/
						if(TickOut((uint32 *)&itickLVContinuous[i], 2000) == true){
							#if VerCtr == VerCtr_Normal
								if(upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose == false){/*�ж������Ѽ������,�ʲ����Ƿѹ*/
									pvca.pVol.flag |= (1<<i);
									TickOut((uint32 *)&itickPV[i], 0);
									l_Flag = true;/*��ѹ��־*/
									h_Flag = false;/*��ѹ��־*/
									hmi_PhaseVc.bits.hv = false;
									hmi_PhaseVc.bits.lv = true;
									/*�����ϴ����������ѹǷѹ,�ɻָ�*/
									upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOverVWaring = false;
									upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseLV = true;
									/*Clear--�����ѹǷѹ�ָ���־*/
									phaseLVRecoverFlag = 0;
								}
							#endif
						}
					}
					else if(cpvs.abcPV[i]  >= 264*1.414){
						/*��Ƿѹ��ⶨʱ��*/
						TickOut((uint32 *)&itickLVContinuous[i], 0);
						
						#if VerCtr == VerCtr_Normal
							pvca.pVol.flag |= (1<<i);
							TickOut((uint32 *)&itickPV[i], 0);
							l_Flag = false;/*��ѹ��־*/
							h_Flag = true;/*��ѹ��־*/
							hmi_PhaseVc.bits.hv = true;
							hmi_PhaseVc.bits.lv = false;
							/*��ѹ�ɻָ�--�޸�:20210128--���ع�ѹ��־����,������ѹ*/
							if(upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOverVFault == true/*���ع�ѹ��־*/){
								upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOverVWaring = false;
							}else{
								upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOverVWaring = true;
							}
							upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseLV = false;
							/*clear--�����ѹ��ѹ�ָ���־*/
							phaseOverVWaringRecoverFlag = 0;
						#endif
					}else{
						/*��Ƿѹ��ⶨʱ��*/
						TickOut((uint32 *)&itickLVContinuous[i], 0);
					
						if(cpvs.abcPV[i] >= 180*1.414 && TickOut((uint32 *)&itickPV[i], tickTimeBase_V) == true && l_Flag == true){
							pvca.pVol.flag &= ~(1<<i);
							phaseLVRecoverFlag |= ((uint8)(1<<i));
							if(phaseLVRecoverFlag == 0x07){
								phaseLVRecoverFlag = 0;
								l_Flag = false;
								hmi_PhaseVc.bits.lv = false;
								/*Ƿѹ�ָ�*/
								upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseLV = false;
							}
						}else{
							if(l_Flag == true&&cpvs.abcPV[i] < 178*1.414){/*�嶨ʱ��Ԥ����ֵ2V*/
								TickOut((uint32 *)&itickPV[i], 0);/*����:20210201--�ظ�ʱ���Իָ���ִ�б�׼֮��ʼ��ʱ*/
							}
						}
						
						if(cpvs.abcPV[i] <= 255*1.414 && TickOut((uint32 *)&itickPV[i], tickTimeBase_V) == true && h_Flag == true){/*�ز��ʱ������,��ֹ����*/
							pvca.pVol.flag &= ~(1<<i);
							phaseOverVWaringRecoverFlag |= ((uint8)(1<<i));
							if(phaseOverVWaringRecoverFlag == 0x07){
								phaseOverVWaringRecoverFlag = 0;
								h_Flag = false;
								hmi_PhaseVc.bits.hv = false;
								/*��ѹ�ָ�*/
								upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOverVWaring = false;
								/*������ع�ѹ��־*/
								upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOverVFault = false;
							}
						}else{
							if(h_Flag == true && cpvs.abcPV[i] > 257*1.414){/*�嶨ʱ��Ԥ����ֵ2V*/
								TickOut((uint32 *)&itickPV[i], 0);/*����:20210201--�ظ�ʱ���Իָ���ִ�б�׼֮��ʼ��ʱ*/
							}
						}
					}	
				}

				if(cpvs.abcPV[i]  <= (float)(150*1.414)){/*�ж������Ѱ���*/
					if(TickOut((uint32 *)&chkTimCnt[i], 100) == true){
						voltChk.flag |= (1<<i);
					}
				}else{
					TickOut((uint32 *)&chkTimCnt[i], 0);
					voltChk.flag &= ~(1<<i);
				}
			}
		}

		/*���ذ����ϱ�������*/
		if((voltChk.flag & 0x07) == 0x07){/*���ذ���һ���������ѹ������50V,�������ȱ��bug*/
			upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose = true;

			for(i=0;i<3;i++){
				/*��Ƿѹ��ⶨʱ��*/
				TickOut((uint32 *)&itickLVContinuous[i], 0);
			}

			/*���������ѹ,Ƿѹ��־*/
			pvca.pVol.flag &= ~(1<<i);
			l_Flag = false;/*��ѹ��־*/
			h_Flag = false;/*��ѹ��־*/
			hmi_PhaseVc.bits.hv = false;
			hmi_PhaseVc.bits.lv = false;			
			upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOverVFault = false;
			upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOverVWaring = false;
			upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseLV = false;
			/*��·֮��(�տ��Զ���բ),�������־*/
			upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOC = false;
			upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurO &= ~(1<<i); 			
		}else{
			/*���º��Ͽտ�֮����,���·��־*/
			if(upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose == true){
				/*���·��־*/
				upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseCShort = false;
				/*�����·��־*/
				upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurShort &= ~((uint16)((uint16)1<<i));		
				/*��Ӳ�����ر�־*/
				upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.hardSwitchClose = false;
				/*�Ż��߼�����--����δ�õ��ֿظ�λ״̬��Ϣǿ����λ*/
				if(oneWaringResetSetFlag == true){
					oneWaringResetSetFlag = false;
					upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.comRecoverFlag = true;
				}
			}
		}

		/*����״̬����*/	
		for(i=0;i<3;i++){
			if(phaseVCDebugMode.flag == false){
				#if MaskOCur_ShortFun == 1/*����--���ι������*/
				if(cpvs.abcPC[i] >= 80*1.414){
					upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseCShort = true;
					/*��λ--���·��־*/
					upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurShort |= (uint16)((uint16)1<<i);
					/*����ָ���ʱ��*/
					TickOut((uint32 *)&itickPC[i], 0);
				}else
				#endif
				if(cpvs.abcPC[i] >= 60*1.414){
					#if VerCtr == VerCtr_Normal
						#if CtrHardware == CtrHardware_Andriod/*ע��:����������Գ���,���ܲ��ٸ��¹�������ֵ*/
							#if MaskOCur_ShortFun == 1/*���ι���,���ܳ�������ι������--201014--��ҫ���ֳ�����*/
							pvca.pCur.flag |= (1<<i);
							hmi_PhaseVc.bits.oc = true;
							
							if(upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseCShort == false){/*�ڶ�·�����²�������*/
								/*����*/
								upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOC = true;
								/*�����*/
								upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurO |= (uint16)((uint16)1<<i); 
								/*���·��־*/
								upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseCShort = false;
								/*�����·��־*/
								upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurShort &= ~((uint16)((uint16)1<<i));
							}
							/*clear--�����ѹ�����ָ���־*/
							phaseOCRecoverFlag = 0;
							#endif
						#endif
					#endif
					TickOut((uint32 *)&itickPC[i], 0);
				}else{
					if(cpvs.abcPC[i] <= 30*1.414 && TickOut((uint32 *)&itickPC[i], tickTimeBase_V) == true){/*�ز��ʱ������,��ֹ����*/
						#if MaskOCur_ShortFun == 1
						if(upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseCShort == false){
							pvca.pCur.flag &= ~(1<<i);
							phaseOCRecoverFlag |= ((uint8)(1<<i));
							if(phaseOCRecoverFlag == 0x07){
								phaseOCRecoverFlag = 0;
								hmi_PhaseVc.bits.oc = false;
								/*�����ָ�*/
								upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseOC = false;
							}
							/*�����*/
							upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurO &= ~(1<<i); 
							/*���·��־*/
							upperLayerPara.stateInfoChange.sysModuleStateInfo.bits.phaseCShort = false;	
							/*�����·��־*/
							upperLayerPara.stateInfoChange.sysModuleStateInfo_2.bits.abcPhaseCurShort &= ~((uint16)((uint16)1<<i));	
						}
						#endif
					}else{
						if(cpvs.abcPC[i] > 32*1.414){/*�嶨ʱ��Ԥ����ֵ2A*/
							TickOut((uint32 *)&itickPC[i], 0);/*����:20210201--�ظ�ʱ���Իָ���ִ�б�׼֮��ʼ��ʱ*/
						}
					}
				}
			}
		}
	}
#endif
	return pvca;
}   

/*
** ���ѹ/������쳣�������
** 		����һ:�ر����г����
*/
bool PhaseVCAbnCloseAllCharger(void){
	bool ret = false;
	PhaseVCAnalyze pvsa = get_PhaseVCAnalyze();

	if(pvsa.pVol.flag != 0 || pvsa.pCur.flag != 0){
		ret = true;
	}else{
		ret = false;
	}

	return ret;
}

