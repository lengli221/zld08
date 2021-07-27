#ifndef __GPIO_H_
#define __GPIO_H_

/*
** Output
*/
#define Upgr_FlickerLedOn								(GPIOG->BSRRL = GPIO_Pin_10)/*������˸��*/
#define Upgr_FlickerLedOff								(GPIOG->BSRRH = GPIO_Pin_10)
#define Fan_On												(GPIOG->BSRRL = GPIO_Pin_4)/*��������*/
#define Fan_Off												(GPIOG->BSRRH = GPIO_Pin_4)
#define Fan2_On											(GPIOG->BSRRL = GPIO_Pin_5)
#define Fan2_Off											(GPIOG->BSRRH = GPIO_Pin_5)

#define FaultLed_On										(GPIOG->BSRRL = GPIO_Pin_6)
#define FaultLed_Off										(GPIOG->BSRRH = GPIO_Pin_6)
#define FaultLed_On_12V								(GPIOG->BSRRL = GPIO_Pin_3)
#define FaultLed_Off_12V								(GPIOG->BSRRH = GPIO_Pin_3)

#define AcSlave_Ctr_On									(GPIOG->BSRRL = GPIO_Pin_7)
#define AcSlave_Ctr_Off									(GPIOG->BSRRH = GPIO_Pin_7)

#define Ac_Ctr_On											(GPIOG->BSRRH = GPIO_Pin_8)
#define Ac_Ctr_Off											(GPIOG->BSRRL = GPIO_Pin_8)

/*
** �������е�
*/
#define Ctr_RunLed_On									(GPIOF->BSRRL = GPIO_Pin_10)
#define Ctr_RunLed_Off									(GPIOF->BSRRH = GPIO_Pin_10)

/*
** Input 
*/
#define Yx_In_Emer										(GPIOF->IDR & GPIO_Pin_13)
#define Yx_In_Smoke										(GPIOF->IDR & GPIO_Pin_11)/*�̸�:��--���� ��--���� ���϶Ͽ��Ӵ���*/
#define Yx_In_Anti											(GPIOF->IDR & GPIO_Pin_12)/*����:��--���� ��--����*/

/*
** �̼���� Output
*/
enum{
	setFirewareUpgre_Label = 0x00,/*������˸�Ʊ��*/
	setFirewareFan_Label = 0x01,/*���ȱ��*/
	setFireACSlaveCtr_label = 0x02,/*���Ƹ���*/
	setFireACCtr_Label = 0x03,/*��������*/
	setFireFalutLed_Label = 0x04,/*���ϵ�*/
	setFireCtrRunLed_Label = 0x05,/*�������е�*/
};

/*
** �̼���� Input
*/
enum{
	getYxIn_Emer = 0x00,/*��ͣ*/
	getYxIn_Smoke = 0x01,/*�̸�*/
	getYxIn_Anti = 0x02,/*����*/
};

/*
** �̼���������
*/
void GPIO_Init_Config(void);
/*
** �̼����� Output
** @param:
**			label-->�̼����
**			flag -->�����߼���־
*/
void firewareCtr(uint8 label, bool flag);
/*
** �̼�״̬��ȡ Input
** @param:
**			label-->�̼����
** @return:
**			1-->����
**			0-->����
*/
uint8 firewareYxIn(uint8 label);
#endif

