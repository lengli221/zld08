#ifndef OS_APP_CFG_H_
#define OS_APP_CFG_H_

/*
** ����ϵͳģ�鹦����#define 
** 1.CAN1 ����ʹ�ܵ���
*/
#define Debug_CAN1_TxFun 								1
#define MaskOCur_ShortFun									0/*��1--����������� ��0--���ι������*/
/*
** ������й̼���
*/
#define ClearAllFireware										0
/*
** ���ι�ѹ�������
*/
#define MaskOVCChk											0

/*
** �ͻ���ѡ��
*/
#define Client_Choice_MeiTuan								0x01/*����*/
#define Client_Choice_PineCone							0x02/*���ɹ�*/
#define Client_Choice_All										0xFE/*���ݿͻ���*/
#define Client_Choice											Client_Choice_MeiTuan

/*
** ��׿�Խ�Ӳ��ѡ��
*/
#define Android_HWhioce_232								0x01/*���ܳ���--��ѡѡ��*/
#define Android_HWhioce_485								0x02/*���ܳ���--ǰ�ڹ��ܲ���*/
#define Android_HWhioce									Android_HWhioce_232

/*
** ���ư���ݵ�����
*/
#define Screen_Compatibility								0x00/*��׿���ݵ�����*/

/*
** ���ư�Ӳ���汾
*/
#define CtrHardware_Dwin									0x01/*������*/
#define CtrHardware_Andriod								0x02/*��׿*/
#define CtrHardware											CtrHardware_Andriod

/*
** VerCtr
*/
#define VerCtr_TestFrock									0x01/*���Թ�װ*/
#define VerCtr_Normal										0x02/*��ʽ�����汾*/
#define VerCtr													VerCtr_Normal	

/*
** ϵͳӲ������
*/												
#define Debug_Flash_BugAnalysis  							0

/*
**�������ȼ�����
*/
#define MainTask_Prio										(int8)10
#define SysCtrlLogic_Prio										(int8)8
#define UpperLayerParseTask_Prio						(int8)6
#define UpperLayerReplyTask_Prio						(int8)7
#define LowerLayerParseTask_Prio						(int8)4
#define LowerLayerReplyTask_Prio						(int8)5

#define UgradeULParseTask_Prio							(int8)11
#define UgradeLLReplyTask_Prio							(int8)13
#define UgradeLLParseTask_Prio							(int8)12

#define SM_DW_LCDTask_Prio								(int8)15
#define HMI_DW_Task_Prio									(int8)16

#endif

