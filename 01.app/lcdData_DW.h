#ifndef __LCDDATA_DW_H_
#define __LCDDATA_DW_H_

/*
** II������ҳ�����
*/
#define LCD_ADDR_WriteId								(uint16)(0x0084)
#define LCD_ADDR_Len_WriteId						(uint8)(2)
#define LCD_ADDR_ReadId								(uint16)(0x0014)
#define LCD_ADDR_Len_ReadId						(uint8)(1)
#define LCD_ADDR_WriteVoice							(uint16)(0x00A0)
#define LCD_ADDR_Len_WriteVoice					(uint8)(2)


/*
** ע������:
**			1.�����������ַ��ͻ �ײ��Ѵ���,��ַȷ��ֵ��ͬ���������������ַ����
** ���ĵ�ַ:
**			1.II���������ײ��Ѵ����,ƫ�Ƶ�ַ:0x1000
**			2.��ַ��д��������
*/
#define LCD_Addr_WriteMainState								(uint16)(0x1000)/*��ҳ��״̬��ַ*/
#define LCD_Addr_WriteMainGroup0Icon						(uint16)(0x2100)/*��ҳ��ͼ��+��ѹ������ַ*/
#define LCD_Addr_WriteMainGroup1Icon						(uint16)(0x2200)
#define LCD_Addr_WriteMainGroup2Icon						(uint16)(0x2300)
#define LCD_Addr_WriteMainGroup3Icon						(uint16)(0x2400)
#define LCD_Addr_WriteMainGroup4Icon						(uint16)(0x2500)
#define LCD_Addr_WriteMainGroup5Icon						(uint16)(0x2600)
#define LCD_Addr_JumpBatInfo								(uint16)(0x1500)/*��/д�����Ϣ��ת����*/
#define LCD_Addr_JumpInputPW								(uint16)(0x1510)/*��/д��ת����������ҳ��*/
#define LCD_Addr_WriteBatInfoBaseAddr					(uint16)(0x1700)/*д���ҳ����Ϣ*/
#define LCD_Addr_WriteBatWorkStateAddr					(uint16)(0x1800)
#define LCD_Addr_WriteBatRunStateAddr					(uint16)(0x1900)
#define LCD_Addr_WriteBatVolAddr							(uint16)(0x1A00)
#define LCD_Addr_WriteBatRefAddr							(uint16)(0x1B00)
#define LCD_Addr_WriteFirstWaringAddr						(uint16)(0x1C00)
#define LCD_Addr_WriteShowPwAddr							(uint16)(0x1D00)/*д������ʾ�ؼ�*/
#define LCD_ADDR_UpdatePwInputAddr						(uint16)(0x1D10)/*������������ؼ�*/
#define LCD_ADDR_UpdateOkBtnAddr						(uint16)(0x1D40)/*�������밴���ؼ�*/

#define LCD_Addr_RegisterInfoAddr							(uint16)(0x2A00)/*ע���ַ��Ϣ*/
#define LCD_Addr_LogoutInfoAddr								(uint16)(0x2A10)/*ע����ַ��Ϣ*/

#define LCD_Addr_WifiIsOkBtn									(uint16)(0x2A20)/*WIFIȷ�ϰ�ť*/

#define LCD_ADDR_DomainNameLocal						(uint16)(0x3000)/*��������*/
#define LCD_ADDR_DomainNameRemote						(uint16)(0x3100)/*Զ������*/

#define LCD_ADDR_DeviceID									(uint16)(0x3400)/*�豸ID*/

#define LCD_Addr_WIFIName									(uint16)(0x3500)/*WIFI����*/
#define LCD_Addr_WIFIPw										(uint16)(0x3600)/*WIFI����*/

/*
** ҳ��ID����
*/
typedef struct
{
	uint16 id;
}LCD_SCREENID;

/*
** ���ݲ�������
*/
typedef struct
{
	uint16 addr;
	uint16 *pData;
}LCD_DATAFRAME;

/*
** ҳ���ַ����
*/
typedef struct
{
	uint8 addr;
	uint8 *pData;
}LCD_CTRLFRAME;

/*
** DWINҳ������ṹ����
*/
typedef struct{
	uint16 idle;/*����*/
	uint16 ing;/*���*/
	uint16 full;/*����*/
}LcdMainSysPara;

/*
** ��ҳ����ת����
*/
typedef struct{
	uint16 doorAddr;/*���ŵ�ַ*/
	uint16 inputPw;/*��������*/
}LcdMainJumpPara;

/*
** ����ҳ�����
*/
typedef struct{
	uint16 show[4];/*ʵ������*/
	uint16 ascii[4];/*ASCII����*/
	uint16 okBtn;/*ȷ�ϼ�*/
}LcdAdminPw;

/*
** ϵͳ����֮��������
*/
typedef struct{
	uint8 localShow[64];
	uint8 remoteShow[64];
}GetDomain;

/*
** ע��/ע��
*/
typedef struct{
	uint16 _register;/*ע��*/
	uint16 logout;/*ע��*/
	uint16 registerState;/*ע��״̬*/
}SysCtr;

/*
** �豸ID
*/
typedef struct{
	uint16 show[8];
}DeviceId;

/*
** WIFI ���ƺ�����
*/
typedef struct{
	uint8 name[32];
	uint8 pw[32];
	uint16 btnIsOk;/*ȷ�ϰ�ť*/
}WIFIPara;

/*
** ϵͳ����
*/
typedef struct{
	GetDomain getDomain;/*������ȡ*/
	SysCtr sysCtr;/*ע��/ע��*/
	DeviceId deviceId;/*�豸ID*/
	WIFIPara wifiPara;/*WIFI���ƺ�����*/
}LcdNewSysPara;


/*
** ϵͳ����֮�ص������ṹ������
*/
typedef struct{
	uint16 addr;/*��ַ��Ϣ*/
	void (*handle)(void*,void*);
}LcdGetData;

/*
** ��ʼ��LCD��������
*/
void init_LcdPara(void);

/*
** ����������֮�ϴ�����
*/
bool LCD_SetData(void *pData,void *temp);
/*
** ����������֮�´�����
*/
bool LCD_GetData(void *pData);
/*
** ��ȡ��ǰҳ��ID����
*/
uint16 LCD_GetScreenID(void);
/*
** ������תҳ��ID����
*/
bool LCD_SetScreenID(uint16 screenId);
/*
** ��ȡ����ҳ��ID����
*/
void LCD_GetScreenIdSet(uint8*pdata);
/*
** ����ҳ����ת����
*/
bool LCD_JumpScreen(void *pData, uint8 dev);
/*
** ��ȡ��ǰҳ��ID
*/
bool LCD_ReadScreenID(void *pData, uint8 dev);

/*
** д��ҳ��ϵͳ����
*/
bool lcd_WriteMainStatePara(void *pData,uint8 dev);
/*
** д��ҳ��ICON��ز���
** icon��ز���֡���ݳ���--��Ϊ4�鴦��
*/
bool lcd_WriteMainIconPara(void *pData,uint8 dev);
/*
** ��ȡ�����Ϣҳ���ַ
*/
uint16 getLcdBatInfoDoorAddr(void);
/*
** ��������Ϣҳ���ַ
*/
void clearLcdBatInfoDoorAddr(void);
/*
** д��ҳ����ת--�豸��ַ����
*/
bool lcd_WriteMainJumpBatInfo(void *pData,uint8 dev);
/*
** ����ҳ����ת--�豸��ַ����
*/
bool lcd_ReadMainJumpBatInfo(void *pData,uint8 dev);
/*
** get ���������ַ����
*/
uint16 get_InputPwPara(void);
/*
** clear ���������ַ����
*/
void clear_InputPwPara(void);
/*
** дҳ����ת--���������ַ
*/
bool lcd_WriteMainInputPw(void *pData,uint8 dev);
/*
** ��ҳ����ת--���������ַ
*/
bool lcd_ReadMainInputPw(void *pData,uint8 dev);
/*
** д�����Ϣ֮������Ϣ
*/
bool lcd_WriteBatInfoBase(void *pData,uint8 dev);
/*
** д�����Ϣ֮��ع���״̬
*/
bool lcd_WriteBatInfoRunState(void *pData,uint8 dev);
/*
** д�����Ϣ֮���ʹ��״̬
*/
bool lcd_WriteBatInfoUseState(void *pData,uint8 dev);
/*
** д�����Ϣ֮��о��Ϣ
*/
bool lcd_WriteBatInfoVolState(void *pData,uint8 dev);
/*
** д�����Ϣ֮�����Ϣ֮MOS״̬
*/
bool lcd_WriteBatInfoRefState(void *pData,uint8 dev);
/*
** дһ���澯ҳ��֮������Ϣ
*/
bool lcd_WriteFirstWaringInfo(void *pData,uint8 dev);
/*
** clear ���������ʾ�ı��ؼ�
*/
void clear_PwShow(void);
/*
** set ������ʾ�ı��ؼ�
*/
void set_PwAscii(void);
/*
** д������ʾ�ı��ؼ�
*/
bool lcd_WritePwShow(void *pData,uint8 dev);
/*
** clear ��������ؼ�
*/
void clear_PwInput(void);
/*
** get ��������ؼ�
*/
LcdAdminPw get_PwInput(void);
/*
** д��������ؼ�
*/
bool lcd_WritePwInput(void *pData,uint8 dev);
/*
** ��ȡ��������ؼ�
*/
bool lcd_ReadPwInput(void *pData,uint8 dev);
/*
** clear okBtn����ֵ
*/
void clear_OkBtn(void);
/*
** get OkBtn����ֵ
*/
uint16 get_OkBtn(void);
/*
** д����ȷ�Ͽؼ�
*/
bool lcd_WriteOkBtn(void *pData,uint8 dev);
/*
** ��ȡ����ȷ�Ͽؼ�
*/
bool lcd_ReadPwOkBtn(void *pData,uint8 dev);
/*
** update Temp Domain -->����������ʱ����
*/
void updateTempDomain(uint8* local,uint8* remote);
/*
** get Domain Legal Len
*/
uint8 getDomainIsLegalLen(uint8* space);
/*
** update Domain
*/
void update_Domain(void);
/*
** дLocal������ʾ�ؼ�
*/
bool lcd_WriteLocalDomainShow(void *pData,uint8 dev);
/*
** get Local Domain Input
*/
void getLocalDomainInput(void* pData,void* temp);
/*
** get_Local Domain Input
*/
void get_LocalDomainInput(uint8* rx);
/*
** ��ȡlocal����input
*/
bool lcd_ReadLocalDomainInput(void *pData,uint8 dev);
/*
** update Remote Demain Show
*/
void update_RemoteDomainShow(uint8* space);
/*
** дRemote������ʾ�ؼ�
*/
bool lcd_WriteRemoteDomainShow(void *pData,uint8 dev);
/*
** get Remote Domain Input
*/
void getRemoteDomainInput(void* pData,void* temp);
/*
** get_Remote Domain Input
*/
void get_RemoteDomainInput(uint8* rx);
/*
** ��ȡRemote����
*/
bool lcd_ReadRemoteDomainInput(void *pData,uint8 dev);
/*
** clear ע����Ϣ����ֵ
*/
void clear_RegisterBtn(void);
/*
** set ����ע��״̬
*/
void set_RegisterState(uint16 state);
/*
** get ע����Ϣ����ֵ
*/
uint16 get_RegisterBtn(void);
/*
** дע����Ϣȷ�Ͽؼ�
*/
bool lcd_WriteRegisterBtn(void *pData,uint8 dev);
/*
** ��ȡע����Ϣȷ�Ͽؼ�
*/
bool lcd_ReadRegisterBtn(void *pData,uint8 dev);
/*
** clear ע����Ϣ����ֵ
*/
void clear_LogoutBtn(void);
/*
** get ע����Ϣ����ֵ
*/
uint16 get_LogoutBtn(void);
/*
** дע����Ϣȷ�Ͽؼ�
*/
bool lcd_WriteLogoutBtn(void *pData,uint8 dev);
/*
** ��ȡע����Ϣȷ�Ͽؼ�
*/
bool lcd_ReadLogoutBtn(void *pData,uint8 dev);
/*
** update �豸ID
*/
void update_DeviceId(DeviceId id);
/*
** get �豸ID
*/
DeviceId get_DeviceId(void);
/*
** д�豸��ַ �����ͺ� �����ͺ�
*/
bool lcd_WriteDeviceInfo(void *pData,uint8 dev);
/*
** get From Dwin DeviceId
*/
void get_FromDeinDeviceId(void* pData,void* temp);
/*
** ���豸��ַ �����ͺ� �����ͺ�
*/
bool lcd_ReadDeviceInfo(void *pData,uint8 dev);
/*
** get WifiNamePW Legal Len
*/
uint8 get_WifiNamePwIsLegalLen(uint8* space);
/*
** init WifiName To DWIN Show
*/
void init_WifiNameToDwinShow(void);
/*
** update wifi nane
*/
void update_WifiName(uint8* space);
/*
** дWIFI������ʾ
*/
bool lcd_WriteWifiNameShow(void *pData,uint8 dev);
/*
** get From DWIN WIFI Name
*/
void get_FromDwinWifiName(void* pData,void* temp);
/*
** ��WIFI��������
*/
bool lcd_ReadWifiNameInput(void *pData,uint8 dev);
/*
** init WifiPw To DWIN Show
*/
void init_WifiPwToDwinShow(void);
/*
** update wifi Pw
*/
void update_WifiPw(uint8* space);
/*
** дWIFI������ʾ
*/
bool lcd_WriteWifiPwShow(void *pData,uint8 dev);
/*
** get From DWIN WIFI Pw
*/
void get_FromDwinWifiPw(void* pData,void* temp);
/*
** ��WIFI��������
*/
bool lcd_ReadWifiPwInput(void *pData,uint8 dev);
/*
** дWIFIȷ�ϰ�ť
*/
bool lcd_WriteWifiOkBtn(void *pData,uint8 dev);
/*
** ��WIFIȷ�ϰ�ť
*/
bool lcd_ReadWifiOkBtn(void *pData,uint8 dev);

#endif

