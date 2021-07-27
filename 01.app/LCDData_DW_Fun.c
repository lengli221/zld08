#include "includes.h"

extern LCD_CTRLFRAME LCD_CtrlAddrSheets[];
extern uint8 const  LCD_CtrlAddrSheetsNum;
extern LCD_DATAFRAME LCD_DataAddrSheets[];
extern uint16 const LCD_DataAddrSheetsNum;


/*
** ������֮����Ļ��ȡ����
*/
static LcdGetData lcdGetDataTable[] = {
	{LCD_ADDR_DomainNameLocal,getLocalDomainInput},
	{LCD_ADDR_DomainNameRemote,getRemoteDomainInput},
	{LCD_ADDR_DeviceID,get_FromDeinDeviceId},
	{LCD_Addr_WIFIName,get_FromDwinWifiName},
	{LCD_Addr_WIFIPw,get_FromDwinWifiPw},
};
static uint8 lcdGetDataTableNum = sizeof(lcdGetDataTable)/sizeof(LcdGetData);

/*
** ����������֮�´�����
*/
bool LCD_SetData(void *pData,void *temp){
	uint16 i;
	uint8 j;
	uint16 Vual=0 ;
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if (st_FramePara == null) 
		return false;
	
	if((st_FramePara->ucFunCode == D_DP_CtrlRead)||(st_FramePara->ucFunCode == D_DP_CtrlWrite)){
		for (i = 0; i < LCD_CtrlAddrSheetsNum; i++){
			if(st_FramePara->iAddress == LCD_CtrlAddrSheets[i].addr){/*���Ҷ�Ӧ�Ĵ�����ַ*/
				if((LCD_CtrlAddrSheetsNum - i) < (st_FramePara->ucRegLen))/*��ָֹ��Խ��*/
					return false;
				
				for (j = 0; j < (st_FramePara->ucRegLen); j++){
					*(LCD_CtrlAddrSheets[i + j].pData) = st_FramePara->ucDataSpace[j];
				}
				break;
			}
		}
	}else{
		if(st_FramePara->iAddress < 0x3000){
			for (i = 0; i < LCD_DataAddrSheetsNum; i++){
				if(st_FramePara->iAddress == LCD_DataAddrSheets[i].addr){/*���Ҷ�Ӧ�Ĵ�����ַ*/
					if((LCD_DataAddrSheetsNum - i) < (st_FramePara->ucRegLen))/*��ָֹ��Խ��*/ 
						return false;
					
					for (j = 0; j < (st_FramePara->ucRegLen); j++){
						Vual=st_FramePara->ucDataSpace[j * 2];
						Vual=Vual<<8;
						Vual=Vual+st_FramePara->ucDataSpace[j *2+1];
						*(LCD_DataAddrSheets[i + j].pData)=Vual;
					}		
					break;
				}
			}
		}else{
			for(i=0;i<lcdGetDataTableNum;i++){
				if(st_FramePara->iAddress == lcdGetDataTable[i].addr){
					lcdGetDataTable[i].handle(pData,temp);
					break;
				}
			}
		}
	}
	return true;
}

/*
** ����������֮�ϴ�����
*/
bool LCD_GetData(void *pData)
{
	uint16 i; 
	uint8 j;
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	
	if (pData == null) 
		return false;
	
	/*(D_DP_CtrlRead)0x81ָ����ַ��RD_LEN�ֽڼĴ������� (D_DP_CtrlWrite)0x80ָ����ַд�Ĵ�������*/
	if((st_FramePara->ucFunCode == D_DP_CtrlRead)||(st_FramePara->ucFunCode == D_DP_CtrlWrite))/*ָ����ַ�������ֽ�Ϊ��λ*/{
		for(i = 0; i < LCD_CtrlAddrSheetsNum; i++){
			if(st_FramePara->iAddress == LCD_CtrlAddrSheets[i].addr){
				if((LCD_CtrlAddrSheetsNum - i) < (st_FramePara->ucRegLen)) 
					return false;
				for(j = 0; j < (st_FramePara->ucRegLen); j++){
					st_FramePara->ucDataSpace[j] = *(LCD_CtrlAddrSheets[i + j].pData) & 0xff;
				}
				break;
			}
		}
	}else{/*(D_DP_DataWirte)0x82ָ����ַ��ʼд�����ݴ�(������)�������洢��*/
		for(i = 0; i < LCD_DataAddrSheetsNum; i++){/*˵��:���,��ѯ����ַ�Ƿ�ƥ��,�ҵ�ƥ���ַ*/
			if(st_FramePara->iAddress == LCD_DataAddrSheets[i].addr){/*�жϲ�ѯ��ƥ���ַ֮��������Ƿ������Ҫд��������,����ֱ�ӷ���*/
				if((LCD_DataAddrSheetsNum - i) < (st_FramePara->ucRegLen)) 
					return false;
				for(j = 0; j < (st_FramePara->ucRegLen); j++){
					st_FramePara->ucDataSpace[j * 2] = *(LCD_DataAddrSheets[i + j].pData) >> 8;
					st_FramePara->ucDataSpace[j * 2 + 1] = *(LCD_DataAddrSheets[i + j].pData) & 0xff;
				}
				break;
			}
		}
	}
	return true;
}

