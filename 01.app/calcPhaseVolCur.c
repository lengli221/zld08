#include "includes.h"

/*--------------------------------------------�����ѹУ׼------------------------------------------------------------*/
PhaseVoltAdjustLogic phaseVoltAdjustLogic = {0};
/*--------------------------------------------------------------------------------------------------------------------*/

/*
** get AD Convert Value -- �����ѹ
*/
double get_ADConvertValue(uint8 ch){
	double tempp = ADC_GetADResultAD(ch);
	
	tempp *= 33;
	tempp /=10;
	tempp /=4096;
	tempp += 0.1;/*�����ܵ���*/
	/*tempp = (double)(ADC_GetADResultAD(ch)*33/10/4096);*/
	if(tempp<1.5){/*����ֵ�Ż�����*/
		tempp = 1.5;
	}

	return tempp;
}

/*
** get AD Convert C Value -- �������
*/
double get_ADConvertCValue(uint8 ch){
	double tempp = ADC3_GetADResultAD(ch);
	
	tempp *= 33;
	tempp /=10;
	tempp /=4096;
	tempp += 0.085/*0*/;/*0.035*//*�����ܵ���*/
	if(tempp<1.5){/*����ֵ�Ż�����*/
		tempp = 1.5;
	}

	return tempp;
}

/*
** get ���ѹ����ת��ABC
*/
float get_PhaseV_AD(uint8 ch){
	double temp = 0;
	float tempp = 0;
	
	temp = get_ADConvertValue(ch);
	tempp = (float)(((double)((temp-1.5)*2*2006))/15);

	return(tempp);
}

/*
** get ���������ת��ABC
*/
float get_PhaseCur_AD(uint8 ch){
	double temp = 0;
	float tempp = 0;

	temp = get_ADConvertCValue(ch);
	tempp = (float)(((double)((temp - 1.5)*2*2000)));
	tempp /=(float)(1.18649635*50);

	return(tempp);
}

/*-------------------------------------------ʹ����λ�����������ѹ----------------------------------------------------------------*/
/*
** ��ȡ��ѹ��ADֵ
*/
uint16 get_LvAdValue(uint8 ch){
	uint16 result = 0;
	if(get_PhaseV_AD(ch)>=160*1.414 && get_PhaseV_AD(ch) <= 180*1.414){
		result = ADC_GetADResultAD(ch);
	}else{
		result = 0xFFFF;
	}
	return result;
}

/*
** ��ȡ��ѹ��ADֵ
*/
uint16 get_HvAdValue(uint8 ch){
	uint16 result = 0;
	if(get_PhaseV_AD(ch)>=260*1.414 && get_PhaseV_AD(ch) <= 280*1.414){
		result = ADC_GetADResultAD(ch);
	}else{
		result = 0xFFFF;
	}
	return result;
}

/*
** ����У׼ϵ��
*/
void set_PhaseVoltAdjust(uint8 ch,bool flag,float k,float b){
	uint8 buf[AdjusrPhaseVoltLen] = {0};
	uint8 len = 0;
	uint16 crc = 0;

	/*����ȫ�ֱ�������*/
	phaseVoltAdjustLogic.flag[ch]  = flag;
	phaseVoltAdjustLogic.k[ch] = k;
	phaseVoltAdjustLogic.b[ch]= b;

	/*copy����*/
	memcpy((uint8*)&buf[len],(uint8*)&phaseVoltAdjustLogic.flag[0],3*(sizeof(bool)+sizeof(float)+sizeof(float)));
	len += 3*(sizeof(bool)+sizeof(float)+sizeof(float));

	/*����CRCУ��*/
	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	/*��������*/
 	eeprom_write(AdjusrPhaseVoltAddr, (uint8 *)&buf[0], len);
 	BKP_WriteBackupDat(AdjusrPhaseVoltAddr, (uint8 *)&buf[0], len);	
}

/*
** ��λ��ʼ��У׼ϵ��
*/
void reset_PhaseVoltAdjust(void){
	uint8 buf[AdjusrPhaseVoltLen] = {0};
	uint8 buf_bkp[AdjusrPhaseVoltLen] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	
	uint8 i = 0; 

	/*��ȡ�洢����*/
	eeprom_read(AdjusrPhaseVoltAddr, (uint8 *)&buf[len], AdjusrPhaseVoltLen);
	BKP_ReadBackupRegDat(AdjusrPhaseVoltAddr, (uint8*)&buf_bkp[len], AdjusrPhaseVoltLen);

	/*����CRCУ��*/
	crc = CRC16((uint8 *)&buf[len], (AdjusrPhaseVoltLen - sizeof(uint16)));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], (AdjusrPhaseVoltLen - sizeof(uint16)));
	/*����ƫ����*/
	len += (AdjusrPhaseVoltLen - sizeof(uint16));
	/*��ȡƫ����*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&phaseVoltAdjustLogic.flag[0],(uint8*)&buf[0],(AdjusrPhaseVoltLen - sizeof(uint16)));
		BKP_WriteBackupDat(AdjusrPhaseVoltAddr, (uint8*)&buf[0], AdjusrPhaseVoltLen);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&phaseVoltAdjustLogic.flag[0],(uint8*)&buf_bkp[0],(AdjusrPhaseVoltLen - sizeof(uint16)));
		eeprom_write(AdjusrPhaseVoltAddr, (uint8*)&buf_bkp[0], AdjusrPhaseVoltLen);
	}else{	
		/*��ʼ������*/
		for(i=0;i<3;i++){
			set_PhaseVoltAdjust(i,false, 0, 0);
		}
	}
}

/*
** ��������У׼��ѹ
*/
float calc_PhaseAdjustVolt(uint8 ch){
	double temp = ADC_GetADResultAD(ch)*phaseVoltAdjustLogic.k[ch];
	temp +=  phaseVoltAdjustLogic.b[ch];
	phaseVoltAdjustLogic.volt[ch] = (uint16)(temp);

	return((float)(phaseVoltAdjustLogic.volt[ch]/100));
}
/*---------------------------------------------------------------------------------------------------------------------------------*/


/*
** get_���������ѹ����
** 		����:�������
*/
CalcPhaseVC get_CalcPhaseVC(void){
	CalcPhaseVC cpvc = {0}; 
	uint8 i = 0;

	/*
	** ���������ѹ
	*/
	for(i=0;i<3;i++){
		cpvc.abcPV[i]= get_PhaseV_AD(i);
		if(phaseVoltAdjustLogic.flag[i] == true){
			cpvc.abcPV[i] = calc_PhaseAdjustVolt(i);
		}
	}

	/*
	** �����������
	*/
	for(i=0;i<3;i++){
		cpvc.abcPC[i] = get_PhaseCur_AD(i);
	}

	return cpvc;
}

