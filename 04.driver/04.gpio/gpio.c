#include "includes.h"

/*
** �̼���������
*/
void GPIO_Init_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE);

	/*
	** ������������
	*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	/*
	** PF11:�̸��ź� PF12:�����ź� PF13:��ͣ�ź�
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/*
	** �����������
	*/
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	

	/*
	** PF10--������˸��
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	

	/*
	** PG3:���ϵ�12V PG4--PG5:����ɢ�ȷ��� PG6:���ϵ� PG7:���Ƹ��� PG8:������·�� PG10:������˸��
	*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_10;
	GPIO_Init(GPIOG, &GPIO_InitStructure);	

	/*
	** �̼���ʼ������
	**			1.���ư�֮��������˸�ر�
	**			2.������·������--��������·�͸���
	**			3.AC���Ƹ���
	**			4.���ϵ�
	**			5.���ȿ���
	**			6.���ϵ�PG3 12V����
	*/
	Upgr_FlickerLedOff;Ac_Ctr_On;AcSlave_Ctr_On;FaultLed_Off;Fan_Off;Fan2_Off;FaultLed_Off_12V;
}

/*
** �̼����� Output
** @param:
**			label-->�̼����
**			flag -->�����߼���־
*/
void firewareCtr(uint8 label, bool flag){
	switch(label){
		case setFirewareUpgre_Label:
			if(flag == true){
				Upgr_FlickerLedOn;
			}else{
				Upgr_FlickerLedOff;
			}
			break;
		case setFirewareFan_Label:
			if(flag == true){
				Fan_On;
				Fan2_On;
			}else{
				Fan_Off;
				Fan2_Off;
			}
			break;
		case setFireACSlaveCtr_label:
			if(flag == true){
				AcSlave_Ctr_On;
			}else{
				AcSlave_Ctr_Off;
			}
			break;
		case setFireACCtr_Label:
			if(flag == true){
				Ac_Ctr_Off;
			}else{
				Ac_Ctr_On;
			}
			break;
		case setFireFalutLed_Label:
			if(flag == true){
				FaultLed_On;
				FaultLed_On_12V;
			}else{
				FaultLed_Off;
				FaultLed_Off_12V;
			}
			break;
		case setFireCtrRunLed_Label:
			if(flag == true){
				Ctr_RunLed_On;
			}else{
				Ctr_RunLed_Off;
			}	
			break;
	}
}

/*
** �̼�״̬��ȡ Input
** @param:
**			label-->�̼����
** @return:
**			1-->����
**			0-->����
*/
uint8 firewareYxIn(uint8 label){
	switch(label){
		case getYxIn_Emer:
			return ((Yx_In_Emer == 0)?1:0);
		case getYxIn_Smoke:
			return ((Yx_In_Smoke == 0)?0:1);
		case getYxIn_Anti:
			return ((Yx_In_Anti == 0)?0:1);
	}
	
	return 0;
}

