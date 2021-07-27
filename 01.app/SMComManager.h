#ifndef __SMCOMMANAGER_H_
#define __SMCOMMANAGER_H_

#define D_SM_TASKID_LCD       								0/*��ʾ��*/
#define D_SM_TaskNum            								1/*ͨ�������������*/  

#define D_SM_NewCmd             								0x0001/*����ָ����Ϣ*/       
#define D_SM_NoCmd              									0x0000/*û����ָ����Ϣ*/    

#define D_SM_CycleCmd           								0x0000/*ѭ��ָ����Ϣ*/      
#define D_SM_InitiativeCmd      								0x0001/*����ָ����Ϣ*/   

#define D_ComType_COM            				 				0x0001/*�������ͺ�*/ 

#define D_RCV_CNT               									100


/*
** ����ʾ��ͨѶ����
*/
#define D_LCD_CMD_JumpScreen							0x00/*ҳ����ת*/
#define D_LCD_CMD_ReadScreenId						0x01/*��ȡ��ǰҳ��*/
#define D_LCD_CMD_WriteStatePara						0x02/*д��ҳ��״̬����*/
#define D_LCD_CMD_WriteIconPara						0x03/*д��ҳ��icon��ز���*/
#define D_LCD_CMD_WriteJumpDeviceAddr				0x04/*д��ҳ����ת�������Ϣ��ַ*/
#define D_LCD_CMD_ReadJumpDeviceAddr				0x05/*����ҳ����ת�������Ϣ��ַ*/
#define D_LCD_CMD_WriteInputPwAddr					0x06/*д��ҳ������ҳ���ַ*/
#define D_LCD_CMD_ReadInputPwAddr					0x07/*����ҳ������ҳ���ַ*/
#define D_LCD_CMD_WriteBatInfoBase					0x08/*д�����Ϣ*/
#define D_LCD_CMD_WriteBatInfoRun					0x09
#define D_LCD_CMD_WriteBatInfoUse					0x0A
#define D_LCD_CMD_WriteBatInfoVol						0x0B
#define D_LCD_CMD_WriteBatInfoRef						0x0C
#define D_LCD_CMD_WriteFirstWaring					0x0D/*дһ��ҳ��澯*/
#define D_LCD_CMD_WritePwShow						0x0E/*д������ʾ�ؼ�*/
#define D_LCD_CMD_WritePwInput						0x0F/*д��������ؼ�*/
#define D_LCD_CMD_ReadPwInput						0x10/*����������ؼ�*/
#define D_LCD_CMD_ReadOkBtn							0x11/*������Ok����*/
#define D_LCD_CMD_WriteOkBtn							0x12/*д����Ok����*/

#define D_LCD_CMD_WriteLocalDomainShow			0x13/*дlocal������ʾ*/
#define D_LCD_CMD_ReadLocalDomainInput			0x14/*��local��������*/		
#define D_LCD_CMD_WriteRemoteDomainShow		0x15/*дremote������ʾ*/
#define D_LCD_CMD_ReadRemoteDomainInput			0x16/*��remote��������*/	

#define D_LCD_CMD_WriteRegisterBtn					0x17/*дע����Ϣ*/
#define D_LCD_CMD_ReadRegisterBtn					0x18/*��ע����Ϣ*/
#define D_LCD_CMD_WriteLogoutBtn						0x19/*дע����Ϣ*/
#define D_LCD_CMD_ReadLogoutBtn						0x1A/*��ע����Ϣ*/

#define D_LCD_CMD_CtrVoiceInfo							0x1B/*����������Ϣ*/

#define D_LCD_CMD_WriteDeviceId						0x1C/*д�豸ID*/
#define D_LCD_CMD_ReadDeviceId						0x1D/*���豸ID*/

#define D_LCD_CMD_WriteWifiName						0x1E/*дWIFI����*/
#define D_LCD_CMD_ReadWifiName						0x1F/*��WIFI����*/

#define D_LCD_CMD_WriteWifiPw							0x20/*дWIFI����*/
#define D_LCD_CMD_ReadWifiPw							0x21/*��WIFI����*/

/*
** ͨ���߼�����Ԫ
*/
typedef struct __SM_LogicUnit__{
	uint8 ucCmdType;/*�������� �Ƿ�ѭ��*/ 
	uint32 ucCmdCode;/*������*/
	void *pFrame;/*����֡*/  
	bool (*PackageFrame)(void *,void *,uint16 *);/*����ص�*/
	bool (*ParseFrame)(void *,void *,uint16);/*����ص�*/
	bool (*SetDataCmd)(void *,void *);/*�������ݻص�(֡����)������(*ParseFrame)��������*/
	bool (*GetDataCmd)(void *,uint8);/*��ȡ���ݻص���֡���� �豸��)������(*PackageFrame)�������*/
	void *pDevAddrs;/*�ӻ���ַ*/
	uint8 *ucDevNum;
	uint32 *ucComState;/*ͨ��״̬(֧��32�����豸)--ͨ���쳣��1*/
	uint16 *ComFailCnt;/*ͨѶʧ����*/
	uint32 *ucComFlag;/*ͨѶ����״̬	ͨ���쳣��1*/
}SM_LogicUnit;

/*
** ͨ���������
*/
typedef struct __SM_TaskPara__{
	uint8 ucComType;/*ͨ�Ŷ˿�����*/
	uint8 ucComID;/*ͨ�Ŷ˿ں�*/
	SM_LogicUnit *pst_LogicTable;/*ͨ���߼���*/
	uint16 ucTableLen;/*ͨ���߼���Ԫ��*/
	uint32 iParam[3];/*���� 1Ϊ�Ƿ���ָ�� 2Ϊָ������ 3Ϊ���豸��*/
	int16 cmdNum;
} SM_TaskPara;


/*
** ͨѶ����������
*/
bool SM_SetCmd(uint8 ucSmNo,uint32* piParam);
/*
** ��ȡͨѶ������
*/
bool SM_GetCmd(uint8 ucSmNo,uint32* piParam);
/*
** ͨѶ��������
*/
void SM_DisposeSmCom(void *pPara);
/*
** ͨѶ������ӿں���
*/
void SM_InitSmComManagement(void);
#endif

