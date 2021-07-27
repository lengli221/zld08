#include "includes.h"

extern SM_TaskPara st_SMTaskPara[D_SM_TaskNum];

/*
** �����������ͨѶ��
*/
void clear_SmCmd(uint8 ucSmNo){
	uint32 iParam[3] = {0};
	uint8 i = 0;

	for(i=0;i<3;i++){
		st_SMTaskPara[ucSmNo].iParam[i] = iParam[0];
	}
}

/*
** ����ͨѶ������
*/
bool SM_SetCmd(uint8 ucSmNo,uint32* piParam){
	uint8 j = 0;
	uint16 cnt = 0;	
	
	if (piParam == null || ucSmNo >= D_SM_TaskNum) 
		return false;
	
	st_SMTaskPara[ucSmNo].cmdNum++;

	do{
		
		cnt++;
		Sleep(10);
	}while((st_SMTaskPara[ucSmNo].iParam[0] != 0) && (cnt < 100));/*�ȴ�֮ǰ��ָ��ִ�����*/

	for(j =0; j < 3;j++){
		st_SMTaskPara[ucSmNo].iParam[2-j] = piParam[2-j];
	}
		
	st_SMTaskPara[ucSmNo].cmdNum--;
	
	if(st_SMTaskPara[ucSmNo].cmdNum < 0){
		st_SMTaskPara[ucSmNo].cmdNum = 0;
	}
	return true;
}

/*
** ��ȡͨѶ������
*/
bool SM_GetCmd(uint8 ucSmNo,uint32* piParam){
	if(piParam != null){
		memcpy(piParam,st_SMTaskPara[ucSmNo].iParam,sizeof(st_SMTaskPara[ucSmNo].iParam));
	}
	
	if(st_SMTaskPara[ucSmNo].iParam[0]){
		return true;/*��ǰ��������Ҫ����*/
	}else{
		return false;/*��ǰͨѶ����*/
	}
}

/*
** ͨѶ�����ͺ���
*/
uint8 ucComBuf[256];
bool SM_DisposeComUnit(SM_TaskPara *pst_TaskPara, int i,uint8 ucAddr)
{
	//uint8 ucComBuf[256];
	uint16 ucLen = 0;
	uint16 ucRecvLen = 0;
	uint16 ucRet = 0;
	uint16 rcv = D_RCV_CNT;
	int j = 0;
	
	memset(ucComBuf,0,sizeof(ucComBuf));

    	/*�������֡*/
    	pst_TaskPara->pst_LogicTable[i].PackageFrame(pst_TaskPara->pst_LogicTable[i].pFrame, ucComBuf, &ucLen);

	/*��������֡*/
    	BSP_SmSend(pst_TaskPara->ucComType, pst_TaskPara->ucComID,ucComBuf, &ucLen );
	
    	/*�жϸ�ͨ�ŵ�Ԫ�Ƿ���Ҫ������������Ҫ�Ļ�ֱ�ӷ���*/
   	 if(pst_TaskPara->pst_LogicTable[i].ParseFrame == null) 
        	return true;
	 
    	ucLen = 0;
    	/*�ȴ���������10ms*/	
	rcv = 100;
	Sleep(10);
	
	/*������Ӧ֡*/
	for(j = 0;j < rcv;j++){    
		BSP_SmRecv(pst_TaskPara->ucComType, pst_TaskPara->ucComID,ucComBuf + ucLen,&ucRecvLen);
		ucLen += ucRecvLen;
		
		if(ucLen >( sizeof(ucComBuf))){
			break;
		}
		
		/*������Ӧ֡*/
		ucRet = pst_TaskPara->pst_LogicTable[i].ParseFrame(pst_TaskPara->pst_LogicTable[i].pFrame,ucComBuf,ucLen);
		if(ucRet)/*�����ɹ�*/{
			if(pst_TaskPara->ucComID == BSP_ComDW_Usart){
				*pst_TaskPara->pst_LogicTable[i].ucComState = 0;
			}else{
				*pst_TaskPara->pst_LogicTable[i].ucComState = (*(pst_TaskPara->pst_LogicTable[i].ucComState)) &(~(1<<ucAddr));
			}
			break;
		}
		/*�ȴ�150ms ������������ */
		Sleep(10);
	}
	
	/*����ʧ��*/
	if((ucRet == false) || (j >= rcv)){
		if(pst_TaskPara->ucComID == BSP_ComDW_Usart) {
			*pst_TaskPara->pst_LogicTable[i].ucComState = 1;
		}else{
			*pst_TaskPara->pst_LogicTable[i].ucComState = (*(pst_TaskPara->pst_LogicTable[i].ucComState))|(1<<ucAddr);
		}
	}
	
    /*�������������������豸����*/
    pst_TaskPara->pst_LogicTable[i].SetDataCmd(pst_TaskPara->pst_LogicTable[i].pFrame,&ucAddr);
    return true;
}

/*
** ͨѶ��������
*/
void SM_DisposeSmCom(void *pPara)
{    
	uint8 i=0,j=0,k=0;
	SM_TaskPara *pst_TaskPara = (SM_TaskPara*)pPara;

	while(1){        	
		if(pst_TaskPara->iParam[0] == D_SM_NewCmd)/*�Ƿ���ѭ��ָ��*/{/*�����л��Ѿ�����������Ĵ���*/
			for (j = 0; j < pst_TaskPara->ucTableLen; j++){                                                
				if(pst_TaskPara->pst_LogicTable[j].ucCmdCode == pst_TaskPara->iParam[1])/*�µ�ָ��ָ����ʲô*/{                 
					if(pst_TaskPara->pst_LogicTable[j].GetDataCmd != null)/*�ж���Ҫ��ȡ�����������Ƿ�Ϊ��*/{
						pst_TaskPara->pst_LogicTable[j].GetDataCmd(pst_TaskPara->pst_LogicTable[j].pFrame,
							pst_TaskPara->iParam[2]);
						if(pst_TaskPara->ucComID == BSP_ComDW_Usart){
							if((((DP_FramePara *)(pst_TaskPara->pst_LogicTable[j].pFrame))->iAddress == 0)||
							(((DP_FramePara *)(pst_TaskPara->pst_LogicTable[j].pFrame))->ucRegLen == 0)){
								break;
							}
						}
					}

					if(pst_TaskPara->ucComType == D_ComType_COM) {
						SM_DisposeComUnit(pst_TaskPara,j,pst_TaskPara->iParam[2]);
						if(pst_TaskPara->ucComID == BSP_ComDW_Usart) {
							if((*(pst_TaskPara->pst_LogicTable[j].ucComState)) == 1){
								pst_TaskPara->pst_LogicTable[j].ComFailCnt[0]++;
								if( pst_TaskPara->pst_LogicTable[j].ComFailCnt[0]>=10)
								{
									pst_TaskPara->pst_LogicTable[j].ComFailCnt[0]=11;  
									*pst_TaskPara->pst_LogicTable[j].ucComFlag = 1;
								}
							}else{
								pst_TaskPara->pst_LogicTable[j].ComFailCnt[0] = 0;
								*pst_TaskPara->pst_LogicTable[j].ucComFlag = 0;
							}
						}else{
							if((*(pst_TaskPara->pst_LogicTable[j].ucComState))&(1<<pst_TaskPara->iParam[2])){
								pst_TaskPara->pst_LogicTable[j].ComFailCnt[pst_TaskPara->iParam[2]]++;
								if( pst_TaskPara->pst_LogicTable[j].ComFailCnt[pst_TaskPara->iParam[2]]>=10){
									pst_TaskPara->pst_LogicTable[j].ComFailCnt[pst_TaskPara->iParam[2]]=11;  
									*pst_TaskPara->pst_LogicTable[j].ucComFlag|=(1<<pst_TaskPara->iParam[2]);
								}
							}else{  
								pst_TaskPara->pst_LogicTable[j].ComFailCnt[pst_TaskPara->iParam[2]]=0;  
								*pst_TaskPara->pst_LogicTable[j].ucComFlag&=(~(1<<pst_TaskPara->iParam[2]));	
							}
						}
					}
					break;
				}    
			}
			pst_TaskPara->iParam[0] = D_SM_NoCmd;/*�Ƿ�����ָ��Ĳ�������*/
		}else{
			if(i >= pst_TaskPara->ucTableLen)
				i=0;
			
			for(; i < pst_TaskPara->ucTableLen; i++){
				if(pst_TaskPara->pst_LogicTable[i].ucCmdType == D_SM_CycleCmd){    
					if(k >= (*pst_TaskPara->pst_LogicTable[i].ucDevNum)){
						k = 0;
					}
					
					for(; k < (*pst_TaskPara->pst_LogicTable[i].ucDevNum); k++){
						if(pst_TaskPara->iParam[0] == D_SM_NewCmd)
							break;
						
						if(pst_TaskPara->pst_LogicTable[i].GetDataCmd != null)/*�ж���Ҫ��ȡ�����������Ƿ�Ϊ��*/{
							pst_TaskPara->pst_LogicTable[i].GetDataCmd(pst_TaskPara->pst_LogicTable[i].pFrame,k);
						}
						
						if(pst_TaskPara->ucComType == D_ComType_COM) {
							SM_DisposeComUnit(pst_TaskPara,i,k);
							if((*(pst_TaskPara->pst_LogicTable[i].ucComState))&(1<<k)){
								pst_TaskPara->pst_LogicTable[i].ComFailCnt[k]++;
								
								if( pst_TaskPara->pst_LogicTable[i].ComFailCnt[k]>=10){
									pst_TaskPara->pst_LogicTable[i].ComFailCnt[k]=11;  
									*pst_TaskPara->pst_LogicTable[i].ucComFlag|=(1<<k);	
								}
							}else{  
								pst_TaskPara->pst_LogicTable[i].ComFailCnt[k]=0;  
								*pst_TaskPara->pst_LogicTable[i].ucComFlag&=(~(1<<k));	
							}
						}
					}
				} 
			}
		}

		/*
		** ���¿��Ź��Ĵ���--ι��
		*/
		watchdogUpdate();
		
		Sleep(20);	
    	}
}

