#include "includes.h"

/*
** ��ʼ��������̼���
*/
void init_chargePara(void);

/*
** ϵͳ����֮���ر����������֮�ṹ������
*/
SysParam sysParam;

/*
** ʹ��ָ���ṩ�ⲿ�ļ����ʱ���
*/
SysParam* get_SysParaPtr(void){
	return &sysParam;
} 

/*
** ���ư��ļ�������
*/
uint8 sysCtrFileNameLen;
uint8 sysCtrFileName[FileName_MaxLen];

/*
** ͨѶ��/��ذ��̼�����
*/
UpgrFilePara upgrComBinFilePara = {0};
UpgrFilePara upgrBatBinFilePara[UpgrBatMemoryNum] = {0};
UpgrFilePara upgrChargeBinFilePara[UpgrChargeMemoryNum] = {0};

/*
** clear ͨѶ��/��ذ�����汾��
*/
void clear_AppVer(void){
	memset((uint8*)&sysParam.newSoftVer.comApp[0],0x00,sizeof(uint16)*SysCtr_AllDoorNum);
}

/*
** get ͨѶ��/��ذ�����App��汾��
*/
uint16 getComRunAppVer(uint8 addr){
	return sysParam.newSoftVer.comApp[addr];
}

/*
** set ͨѶ��/��ذ�����App��汾��
*/
void setComRunAppVer(uint16 ver,uint8 addr){
	sysParam.newSoftVer.comApp[addr] = ver;
}

/*
** ͳ�����������Ѵ��ۼƲ������ٴν�������ģʽ
*/
void set_statisAccUpgrNum(uint16 numFlag,uint8 addr){
	sysParam.newSoftVer.upgrFailNumAcc[addr] = numFlag;
}

/*
** get Ctr SoftVer
*/
uint16 get_CtrSoftVer(void){
	return sysParam.newSoftVer.ctr;
}

/*
** ���¿��ư����/ͨѶ������汾��
*/
void updateSoftVerPara(void){
	UpgrFilePara upgrTemp = {0};
	/*
	** ���ư�����汾��
	*/
	sysParam.newSoftVer.ctr = (uint16)get_CtrVer();
	/*
	** ͨѶ�� -->�ⲿFlash�б����ͨѶ���ļ�����汾��
	*/
	upgrTemp = get_UpgrComFilePara();
	sysParam.newSoftVer.comFile = upgrTemp.upgrFileType.softVer;
}

/*----------------------���:ʵ�����а汾��ͬ�ļ����汾���Ƿ�һ��-----------------------------*/
void reset_ChkCtrSoftVer(void){
	/*----------------------ÿ�η����°������޸�-------------------------------------*/
	uint16 ctr_SoftVer = 389;
	uint8 realityFileName[23] ={ "CDG_MD_B_B1_300_389.bin"};
	/*---------------------------------------------------------------------------------*/
	uint32 ver = (uint32)ctr_SoftVer;
	uint16 wFileNameLen = 23;
	uint8 tmpp[sizeof(uint32)+0x03] = {0};
	uint8 fileNameLen[sizeof(uint16)+0x03] = {0};
	uint8 fileName[FileName_MaxLen+0x03] = {0};/*fileName+head+crc16*/
	uint16 crc16 = 0;

	
	if(sysParam.newSoftVer.ctr != ctr_SoftVer || memcmp((uint8*)&realityFileName[0],(uint8*)&sysCtrFileName[0],23) != 0){
		sysParam.newSoftVer.ctr = ctr_SoftVer;
		memcpy((uint8*)&sysCtrFileName[0],(uint8*)&realityFileName[0],23);	

		/*1.д����汾��*/
		tmpp[0] = UpgradeKey_Head;
		memcpy((uint8*)&tmpp[1],(uint8*)&ver,sizeof(uint32));
		crc16 = CRC16((uint8 *)&tmpp[0], sizeof(uint32)+sizeof(uint8));
		memcpy((uint8*)&tmpp[sizeof(uint32)+sizeof(uint8)],(uint8*)&crc16,sizeof(uint16));
		BKP_WriteBackupDat(UpgradeVer_Addr, (uint8*)&tmpp[0], UpgradeVer_Len_Addr);
		eeprom_write(UpgradeVer_Addr, (uint8 *)&tmpp[0], UpgradeVer_Len_Addr);		

		/*2.д�ļ�������,�ļ���*/
		fileNameLen[0] = UpgradeKey_Head;
		memcpy((uint8*)&fileNameLen[1],(uint8*)&wFileNameLen,sizeof(uint16));
		crc16 = CRC16((uint8 *)&fileNameLen[0], sizeof(uint8)+sizeof(uint16));
		memcpy((uint8*)&fileNameLen[sizeof(uint8)+sizeof(uint16)],(uint8*)&crc16,sizeof(uint16));
		BKP_WriteBackupDat(UpgradeFileNameLen_Addr, (uint8 *)&fileNameLen[0], UpgradeFileNameLen_Len_Addr);
		eeprom_write(UpgradeFileNameLen_Addr, (uint8 *)&fileNameLen[0], UpgradeFileNameLen_Len_Addr);	

		fileName[0] = UpgradeKey_Head;
		memcpy((uint8*)&fileName[1],(uint8*)&sysCtrFileName[0],wFileNameLen);
		crc16 = CRC16((uint8 *)&fileName[0], wFileNameLen+sizeof(uint8));
		memcpy((uint8*)&fileName[wFileNameLen+sizeof(uint8)],(uint8*)&crc16,sizeof(uint16));
		BKP_WriteBackupDat(UpgradeFileName_Addr, (uint8 *)&fileName[0], wFileNameLen+sizeof(uint8)+sizeof(uint16));
		eeprom_write(UpgradeFileName_Addr, (uint8 *)&fileName[0], wFileNameLen+sizeof(uint8)+sizeof(uint16));		
	}
}
/*--------------------------------------------------------------------------------------------*/

/*
** set ������Կ����
*/
void set_UpgrCtrKey(void){
	uint8 buf_bkp[UpgradeKey_Len_Addr] = {UpgradeKey_Head,
		0x7A,0x6C,0x64,0x12,0x04,0x01,0x00,0x00};
	uint16 crc16 = 0;

	crc16 = CRC16((uint8 *)&buf_bkp[0], UpgradeKey_Len_Addr - sizeof(uint16));
	memcpy((uint8*)&buf_bkp[sizeof(uint8)+sizeof(UpgradeKey)],(uint8*)&crc16,sizeof(uint16));

	BKP_WriteBackupDat(UpgradeKey_Addr, (uint8*)&buf_bkp[0], UpgradeKey_Len_Addr);
}

/*
** get Ctr����汾��
*/
uint32 get_CtrVer(void){
	uint32 ver = 0;
	uint8 buf[UpgradeVer_Len_Addr] = {0};
	uint8 buf_bkp[UpgradeVer_Len_Addr] = {0};
	uint8 len = 0;
	uint8 head = 0;
	uint8 head_bkp = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;
	
	eeprom_read(UpgradeVer_Addr, (uint8 *)&buf[len], UpgradeVer_Len_Addr);
	BKP_ReadBackupRegDat(UpgradeVer_Addr, (uint8*)&buf_bkp[len], UpgradeVer_Len_Addr);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], sizeof(uint8) + sizeof(uint32));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(uint8) + sizeof(uint32));

	/*
	** ֡ͷ
	*/
	head = buf[len];
	head_bkp = buf_bkp[len];
	len += sizeof(uint8);
	len += sizeof(uint32);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(head == UpgradeKey_Head && crc == getCrc){
		memcpy((uint8*)&ver,(uint8*)&buf[sizeof(uint8)],sizeof(uint32));
		BKP_WriteBackupDat(UpgradeVer_Addr, (uint8*)&buf[0], UpgradeVer_Len_Addr);
	}else if(head_bkp == UpgradeKey_Head && crc_bkp == getCrc_bkp){
		memcpy((uint8*)&ver,(uint8*)&buf_bkp[sizeof(uint8)],sizeof(uint32));
		eeprom_write(UpgradeVer_Addr, (uint8*)&buf_bkp[0], UpgradeVer_Len_Addr);
	}else{
		ver = 1;
	}
	return ver;		
}

/*
** get Ctr�ļ�������
*/
static uint16 get_CtrFileNameLen(void){
	return sysCtrFileNameLen;
}

/*
** ��ʼ������ Ctr�ļ�������
*/
void init_UpgrCtrFileNameLen(void){
	uint8 buf[UpgradeFileNameLen_Len_Addr] = {0};
	uint8 buf_bkp[UpgradeFileNameLen_Len_Addr] = {0};
	uint8 len = 0;
	uint8 head = 0;
	uint8 head_bkp = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;
	
	eeprom_read(UpgradeFileNameLen_Addr, (uint8 *)&buf[len], UpgradeFileNameLen_Len_Addr);
	BKP_ReadBackupRegDat(UpgradeFileNameLen_Addr, (uint8*)&buf_bkp[len], UpgradeFileNameLen_Len_Addr);

	/*
	** ����CRCУ��
	*/
	crc = CRC16((uint8*)&buf[len],sizeof(uint8)+sizeof(uint16));
	crc_bkp = CRC16((uint8*)&buf_bkp[len],sizeof(uint8)+sizeof(uint16));

	/*
	** ֡ͷ
	*/
	head = buf[len];
	head_bkp = buf_bkp[len];
	len += sizeof(uint8);
	len += sizeof(uint16);

	/*
	** get CRCУ��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(head == UpgradeKey_Head && crc == getCrc){
		memcpy((uint8*)&sysCtrFileNameLen,(uint8*)&buf[sizeof(uint8)],sizeof(uint16));
		BKP_WriteBackupDat(UpgradeFileNameLen_Addr, (uint8*)&buf[0], UpgradeFileNameLen_Len_Addr);
	}else if(head_bkp == UpgradeKey_Head && crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysCtrFileNameLen,(uint8*)&buf_bkp[sizeof(uint8)],sizeof(uint16));
		eeprom_write(UpgradeFileNameLen_Addr, (uint8*)&buf_bkp[0], UpgradeFileNameLen_Len_Addr);
	}else{
		sysCtrFileNameLen = 0;
	}
}


/*
** get Ctr�ļ���
*/
void get_CtrFileName(uint8* rx,uint8* rxlen){
	uint16 fileNameLen = 0;

	fileNameLen = get_CtrFileNameLen();

	if(fileNameLen > 0 && fileNameLen <= FileName_MaxLen){/*�ļ������ȺϷ�*/
		memcpy((uint8*)&rx[0],(uint8*)&sysCtrFileName[0],fileNameLen);
	}

	*rxlen = (uint8)fileNameLen;
}

/*
** ��ʼ�����ư��ļ���
*/
void init_sysCtrFileName(void){
	uint8 buf[UpgradeFileName_Len_Addr] = {0};
	uint8 buf_bkp[UpgradeFileName_Len_Addr] = {0};
	uint8 len = 0;
	uint8 head = 0;
	uint8 head_bkp = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;
	uint16 fileNameLen = 0;
	uint16 frameLen = 0;
	fileNameLen = get_CtrFileNameLen();

	if(fileNameLen > 0 && fileNameLen <= FileName_MaxLen){/*�ļ������ȺϷ�*/
		frameLen = fileNameLen + sizeof(uint8) + sizeof(uint16);
		eeprom_read(UpgradeFileName_Addr, (uint8 *)&buf[len], frameLen);
		BKP_ReadBackupRegDat(UpgradeFileName_Addr, (uint8*)&buf_bkp[len], frameLen);

		/*
		** ����CRCУ��
		*/
		crc = CRC16((uint8*)&buf[len],sizeof(uint8)+fileNameLen);
		crc_bkp = CRC16((uint8*)&buf_bkp[len],sizeof(uint8)+fileNameLen);

		/*
		** ֡ͷ
		*/
		head = buf[len];
		head_bkp = buf_bkp[len];
		len += sizeof(uint8);
		len += fileNameLen;

		/*
		** get CRCУ��
		*/
		memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
		memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

		if(head == UpgradeKey_Head && crc == getCrc){
			memcpy((uint8*)&sysCtrFileName[0],(uint8*)&buf[sizeof(uint8)],fileNameLen);
			BKP_WriteBackupDat(UpgradeFileName_Addr, (uint8*)&buf[0], frameLen);
		}else if(head_bkp == UpgradeKey_Head && crc_bkp == getCrc_bkp){
			memcpy((uint8*)&sysCtrFileName[0],(uint8*)&buf_bkp[sizeof(uint8)],fileNameLen);
			eeprom_write(UpgradeFileName_Addr, (uint8*)&buf_bkp[0], frameLen);
		}else{
			memset((uint8*)&sysCtrFileName[0],0x00,fileNameLen);
		}
	}
}

/*
** ���ŵ�ذ�������ʼ��-- 20210520--����������ع̼�
** �����������µ�ع̼��б�--�����ƹ��ṩ--29219524
*/
#define Init_BatFileSzie			29 /*20210413--���ŵ�ع̼�����1��������̼�*/

/*
** default ��ذ���Ϣ
*/
DetailedInfo default_DetailedInfo(uint16 loc){
	DetailedInfo detailInfo[Init_BatFileSzie] = {
		/*������*/{100,"A20301"},{200,"A20608"},{210,"A20708"},{220,"A20508"},{230,"A20308"},{300,"A20614"},
		/*��ë��*/{100,"A30301"},{200,"A30301"},{300,"A30608"},{310,"A30708"},{320,"A30508"},{330,"A30208"},{400,"A31013"},
		/*�϶�*/{100,"A41002"},{200,"A41002"},{300,"A41002"},
		/*���ն���*/{100,"A50606"},{200,"A50607"},
		/*�Ǻ�*/{100,"A61103"},{200,"A61104"},{300,"A61109"},
		/*ATL*/{100,"A71112"},
		/*CATL*/{100,"A81015"},
		/*������*/{211,"A20708"},
		/*������ع̼�:--20210520--�޸�ʱ��:20210524--�����ƹ��ṩ�ı��*/
		{500,"A31016"},
		{400,"A21016"},
		{200,"A71016"},
		{201,"A71016"},
		/*����:20210524--�����ƹ��ṩ�ı��*/
		{200,"A81016"}};

	DetailedInfo detailInfoTemp = {0};
	
	if(loc < Init_BatFileSzie){
		detailInfoTemp = detailInfo[loc];
	}
	return detailInfoTemp;
}

/*
** set����bin�ļ�����
** ����:���ݵ�ذ���ϸ��Ϣ(Ӳ���汾/ID)�洢ָ����ַλ��
*/
void set_UpgrBinFilePara(UpgrFilePara upgr){
	uint8 buf[UpgrBinFilePara_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint32 addr = 0;
	ModifyEepromLogic modifyEepromLogicTemp = {0};

	/*�ṹ��ת���洢*/
	modifyEepromLogicTemp = set_TypedefModify(&upgr);
	memcpy((uint8*)&buf[len],(uint8*)&modifyEepromLogicTemp.board,sizeof(ModifyEepromLogic));
	len += sizeof(ModifyEepromLogic);
	
	/*����CRC16*/
	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	addr = get_UpgrEepromAddr(upgr);
	eeprom_write(addr, (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(addr, (uint8*)&buf[0], len);
}

/*
** set ����bin�ļ�����(��س�ʼ��)+������̼���ʼ��
** 		���Ʋ���:
**				��ʼ��������̼��͵�ع̼�����
*/
void set_InitUpgrBatBinFile(UpgrFilePara upgr,uint8 loc){
	uint8 buf[UpgrBinFilePara_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint32 addr = 0;
	ModifyEepromLogic modifyEepromLogicTemp = {0};

	/*��ʼ�����������:��ع̼���,������̼���*/
	if(upgr.upgrFileType.board != ComBoradType && upgr.upgrFileType.board != 0){/*ͨѶ��*/
		if(upgr.upgrFileType.board == ChargeBoardType){/*������̼���*/
			upgrChargeBinFilePara[loc] = upgr;	 
		}else{/*��ع̼���*/
			upgrBatBinFilePara[loc] = upgr;
		}

		/*�ṹ��ת���洢*/
		modifyEepromLogicTemp = set_TypedefModify_Assign(&upgr,loc);
		memcpy((uint8*)&buf[len],(uint8*)&modifyEepromLogicTemp.board,sizeof(ModifyEepromLogic));
		len += sizeof(ModifyEepromLogic);
		
		/*����CRC16*/
		crc = CRC16((uint8 *)&buf[0], len);
		memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
		len += sizeof(uint16);

		addr = get_UpgrBatEepromAddr(loc,upgr);

		eeprom_write(addr, (uint8 *)&buf[0], len);
		BKP_WriteBackupDat(addr, (uint8*)&buf[0], len);

		/*��ʼ�����µ��/���������*/
		if(upgr.upgrFileType.board == ChargeBoardType){/*������̼���*/
			upgrChargeBinFilePara[loc] = upgr;	 
		}else{/*��ع̼���*/
			upgrBatBinFilePara[loc] = upgr;
		}
	}
}

/*
** set Assign��ذ�bin�ļ�����
*/
void set_AssignUpgrBinFilePara(UpgrFilePara upgr,int16 loc){
	uint8 buf[UpgrBinFilePara_Len] = {0};
	uint16 len = 0;
	uint16 crc = 0;
	ModifyEepromLogic modifyEepromLogicTemp = {0};

	/*ת���ṹ��洢*/
	modifyEepromLogicTemp = set_TypedefModify_Assign(&upgr,loc);
	memcpy((uint8*)&buf[len],(uint8*)&modifyEepromLogicTemp.board,sizeof(ModifyEepromLogic));
	len += sizeof(ModifyEepromLogic);	

	/*����CRC*/	
	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);
	
	eeprom_write(UpgrBinFilePara_BatAddr(loc), (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(UpgrBinFilePara_BatAddr(loc), (uint8*)&buf[0], len);	

	/*���µ�ذ�����*/
	upgrBatBinFilePara[loc] = upgr;
}

/*
** get����bin�ļ�����--��ذ�
** @para:
**		loc:ָ��λ��--(0,max(get_SizeBatFile()))
*/
UpgrFilePara get_UpgrBatFilePara(uint16 loc){

	return upgrBatBinFilePara[loc];
}

/*
** ��ʼ�����µ�ذ��̼�����
*/
void init_UpdateBatFileParam(void){
	uint8 buf[UpgrBinFilePara_Len] = {0};
	uint8 buf_bkp[UpgrBinFilePara_Len] = {0};
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;
	uint8 len = 0;
	DetailedInfo detailedInfoTemp = {0};
	uint8 loc = 0;
	ModifyEepromLogic modifyELTemp = {0};

	for(loc=0;loc<sysParam.batFireSize;loc++){
		len = 0;
		eeprom_read(UpgrBinFilePara_BatAddr(loc), (uint8 *)&buf[len], UpgrBinFilePara_Len);
		BKP_ReadBackupRegDat(UpgrBinFilePara_BatAddr(loc), (uint8*)&buf_bkp[len], UpgrBinFilePara_Len);

		/*���� CRCУ��*/
		crc = CRC16((uint8 *)&buf[len], sizeof(ModifyEepromLogic));
		crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(ModifyEepromLogic));

		len += sizeof(ModifyEepromLogic);
		
		/*get CRC16У��*/
		memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
		memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

		#if ClearAllFireware == 1
			getCrc = 0;
			getCrc_bkp = 0;
		#endif

		if(crc == getCrc){
			memcpy((uint8*)&modifyELTemp.board,(uint8*)&buf[0],sizeof(ModifyEepromLogic));
			upgrBatBinFilePara[loc] = conv_TypedefModify(modifyELTemp);
			BKP_WriteBackupDat(UpgrBinFilePara_BatAddr(loc),(uint8*)&buf[0], UpgrBinFilePara_Len);
		}else if(crc_bkp == getCrc_bkp){
			memcpy((uint8*)&modifyELTemp.board,(uint8*)&buf_bkp[0],sizeof(ModifyEepromLogic));
			upgrBatBinFilePara[loc] = conv_TypedefModify(modifyELTemp);
			eeprom_write(UpgrBinFilePara_BatAddr(loc), (uint8 *)&buf_bkp[0], UpgrBinFilePara_Len);
		}else{
			detailedInfoTemp = default_DetailedInfo(loc);
			upgrBatBinFilePara[loc].upgrFileType.board = BatBoardType;
			memcpy((uint8*)&upgrBatBinFilePara[loc].upgrFileType.detailedInfo.hardVer,(uint8*)&detailedInfoTemp.hardVer,sizeof(DetailedInfo));
			if(detailedInfoTemp.hardVer != 0){/*defalut��ذ�ϵ��*/
				set_AssignUpgrBinFilePara(upgrBatBinFilePara[loc], loc);
			}
		}
		/*���¿��Ź��Ĵ���--ι��*/
		watchdogUpdate();		
	}
}

/*
** get����bin�ļ�����--ͨѶ��
*/
UpgrFilePara get_UpgrComFilePara(void){
	return upgrComBinFilePara;
}

/*
** ��ʼ�ϵ����ͨѶ�����--init_UpdateUpgrCom
*/
void init_UpdateUpgrComFilePara(void){
	UpgrFilePara upgrTemp = {0};
	uint8 buf[UpgrBinFilePara_Len] = {0};
	uint8 buf_bkp[UpgrBinFilePara_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;
	ModifyEepromLogic modifyELTemp = {0};
		
	eeprom_read(UpgrBinFilePara_CommAddr, (uint8 *)&buf[len], UpgrBinFilePara_Len);
	BKP_ReadBackupRegDat(UpgrBinFilePara_CommAddr, (uint8*)&buf_bkp[len], UpgrBinFilePara_Len);

	/*����CRC16У��*/
	crc = CRC16((uint8 *)&buf[len], sizeof(ModifyEepromLogic));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(ModifyEepromLogic));

	len += sizeof(ModifyEepromLogic);

	/*get CRC16У��*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	#if ClearAllFireware == 1
		getCrc = 0;
		getCrc_bkp = 0;
	#endif

	if(crc == getCrc){
		memcpy((uint8*)&modifyELTemp.board,(uint8*)&buf[0],sizeof(ModifyEepromLogic));
		upgrComBinFilePara = conv_TypedefModify(modifyELTemp);
		BKP_WriteBackupDat(UpgrBinFilePara_CommAddr, (uint8*)&buf[0], UpgrBinFilePara_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&modifyELTemp.board,(uint8*)&buf_bkp[0],sizeof(ModifyEepromLogic));
		upgrComBinFilePara = conv_TypedefModify(modifyELTemp);
		eeprom_write(UpgrBinFilePara_CommAddr, (uint8*)&buf_bkp[0], UpgrBinFilePara_Len);
	}else{
		upgrTemp.upgrFileType.board = ComBoradType;
		/*20210324--�Ż�:�״��ϵ��ʼ���ֿ���ز���:Ӳ���汾��--����Ӱ��ʵ���߼�*/
		upgrTemp.upgrFileType.detailedInfo.hardVer = 300;
		set_UpgrBinFilePara(upgrTemp);
	}
}


/*
** get��ذ��洢���еĴ�С(����)
*/
uint16 get_SizeBatFile(void){
	uint16 num = 0;
	uint8 buf[UpgrBatFileNumLen] = {0};
	uint8 buf_bkp[UpgrBatFileNumLen] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;
	
	eeprom_read(UpgrBatFileNumAddr, (uint8 *)&buf[len], UpgrBatFileNumLen);
	BKP_ReadBackupRegDat(UpgrBatFileNumAddr, (uint8*)&buf_bkp[len], UpgrBatFileNumLen);

	/*����CRC16У��*/
	crc = CRC16((uint8 *)&buf[len], sizeof(uint16));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(uint16));

	len += sizeof(uint16);

	/*get CRC16У��*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));
	if(crc == getCrc){
		memcpy((uint8*)&num,(uint8*)&buf[0],sizeof(uint16));
		BKP_WriteBackupDat(UpgrBatFileNumAddr, (uint8*)&buf[0], UpgrBatFileNumLen);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&num,(uint8*)&buf_bkp[0],sizeof(uint16));
		eeprom_write(UpgrBatFileNumAddr, (uint8*)&buf_bkp[0], UpgrBatFileNumLen);
	}else{
		num = Init_BatFileSzie;
		init_BatPara();
		set_SizeBatFile(Init_BatFileSzie);
	}

	/*����Ƿ�������ع̼�*/
	if(num != Init_BatFileSzie){
		num = Init_BatFileSzie;
		set_SizeBatFile(Init_BatFileSzie);
	}
	
	/*get��ذ��洢���еĴ�С(����)--��ʼ����*/
	sysParam.batFireSize = num;
	
	return num;
}

/*
** get ��ع̼�������
** 		����:�����ڲ�ѯ��ذ�����
*/
uint16 get_batFireSize(void){
	return sysParam.batFireSize;	
}

/*
** set��ذ��洢���еĴ�С(����)
*/
void set_SizeBatFile(uint16 szie){
	uint8 buf[UpgrBatFileNumLen] = {0};
	uint16 crc = 0;
	uint8 len = 0;

	sysParam.batFireSize = szie;
	
	memcpy((uint8*)&buf[len],(uint8*)&szie,sizeof(uint16));
	len += sizeof(uint16);
	crc = CRC16((uint8 *)&buf[0], sizeof(uint16));
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(UpgrBatFileNumAddr, (uint8 *)&buf[0], UpgrBatFileNumLen);
	BKP_WriteBackupDat(UpgrBatFileNumAddr, (uint8*)&buf[0], UpgrBatFileNumLen);
}

/*
** ��ʼ����ذ�(Ӳ���汾��,ID)����
*/
void init_BatPara(void){

	UpgrFilePara upgr = {0};
	uint16 i  = 0;
	DetailedInfo detailedInfoTemp = {0};

	upgr.upgrFileType.board = BatBoardType;

	for(i=0;i<Init_BatFileSzie;i++){
		detailedInfoTemp = default_DetailedInfo(i);
		memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&detailedInfoTemp.hardVer,sizeof(DetailedInfo));
		set_InitUpgrBatBinFile(upgr, i);
		/*���¿��Ź��Ĵ���--ι��*/
		watchdogUpdate();
	}
}

/*
** get��ذ�λ����Ϣ
** @return:
** 			-1:�޵�ذ��洢������Ϣ--�Ƿ����
**			-2:������ʧ
** ����:
**		1.���������ж��Ƿ���ڵ�ذ���Ӧ�ͺ�����
**		2.��λ����Ϣ��ȡEeprom��ַ��Ϣ
*/
int16 get_BatLocation(UpgrFilePara upgr){
	int16 loc = -2;
	uint16 i = 0;
	uint16 size = get_batFireSize()/*get_SizeBatFile()*/;
	UpgrFilePara upgrTemp = {0};
	if(size >= Init_BatFileSzie){/*���ŵ������Э��Ĭ�ϵ�ذ�����*/
		loc = -1;/*δ����֮ǰ,�����޵�ذ���Ϣ*/
		for(i = 0;i < size;i++){
			upgrTemp = get_UpgrBatFilePara(i);
			if(memcmp((uint8*)&upgrTemp.upgrFileType.detailedInfo.hardVer,
				(uint8*)&upgr.upgrFileType.detailedInfo.hardVer,sizeof(DetailedInfo)) == 0){
				loc = i;
				break;
			}
		}
	}else{/*�洢��ذ���С������ʧ*/
		init_BatPara();
		set_SizeBatFile(Init_BatFileSzie);
		loc = -2;
	}

	if(loc == -2){/*������ʧ:�ݹ�,�ص�*/
		loc = get_BatLocation(upgr);
	}

	return loc;
} 

/*******************************������̼�����***************************************************/
/*
** ��ʼ��������̼�������
*/
#define Init_ChargeFileSize								2

/*
** get ������̼�������
*/
uint16 get_ChargeFireSize(void){
	return sysParam.chargeSaveInfo.fireSzie;
}

/*
** set ������̼�������
*/
void set_ChargeFireSize(uint16 size){
	uint8 buf[UpgrChargeNumLen] = {0};
	uint16 crc = 0;
	uint8 len = 0;

	sysParam.chargeSaveInfo.fireSzie = size;
	
	memcpy((uint8*)&buf[len],(uint8*)&size,sizeof(uint16));
	len += sizeof(uint16);
	crc = CRC16((uint8 *)&buf[0], sizeof(uint16));
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(UpgrChargeNumAddr, (uint8 *)&buf[0], UpgrChargeNumLen);
	BKP_WriteBackupDat(UpgrChargeNumAddr, (uint8*)&buf[0], UpgrChargeNumLen);	
}

/*
** ��λ���³�����̼�����С
*/
void reset_UpdateChargeFireSize(void){
	uint16 num = 0;
	uint8 buf[UpgrChargeNumLen] = {0};
	uint8 buf_bkp[UpgrChargeNumLen] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;

	eeprom_read(UpgrChargeNumAddr, (uint8 *)&buf[len], UpgrChargeNumLen);
	BKP_ReadBackupRegDat(UpgrChargeNumAddr, (uint8*)&buf_bkp[len], UpgrChargeNumLen);

	/*����CRC16У��*/
	crc = CRC16((uint8 *)&buf[len], sizeof(uint16));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(uint16));

	len += sizeof(uint16);

	/*get CRC16У��*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));
	if(crc == getCrc){
		memcpy((uint8*)&num,(uint8*)&buf[0],sizeof(uint16));
		BKP_WriteBackupDat(UpgrChargeNumAddr, (uint8*)&buf[0], UpgrChargeNumLen);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&num,(uint8*)&buf_bkp[0],sizeof(uint16));
		eeprom_write(UpgrChargeNumAddr, (uint8*)&buf_bkp[0], UpgrChargeNumLen);
	}else{
		num = Init_ChargeFileSize;
		init_chargePara();
		set_ChargeFireSize(Init_ChargeFileSize);
	}

	/*
	** ���µ�ع̼�����������
	*/
	sysParam.chargeSaveInfo.fireSzie = num;
}

/*
** get ����bin�ļ�����--������̼���
** @param:
** 			loc:ָ��λ��--[0,max(get_ChargeFireSize())
*/
UpgrFilePara get_UpgrChargeFilePara(uint16 loc){
	return upgrChargeBinFilePara[loc];
}

/*
** default ������̼�����Ϣ
*/
DetailedInfo default_ChargeDetailedInfo(uint16 loc){
	DetailedInfo detailedInfo[Init_ChargeFileSize] = {
		/*����ӯԴ*/{102,"C11110"},/*20210227--ӯԴӲ���汾�޸�Ϊ:101 -- 20210325--ӯԴӲ���汾�޸�:102*/
		/*������ʯ*/{100,"C21110"}};

	DetailedInfo detailInfoTemp = {0};

	if(loc < Init_ChargeFileSize){
		detailInfoTemp = detailedInfo[loc];
	}
	return detailInfoTemp;
}

/*
** ��ʼ��������̼���
*/
void init_chargePara(void){
	UpgrFilePara upgr = {0};
	uint16 i  = 0;
	DetailedInfo detailedInfoTemp = {0};

	upgr.upgrFileType.board = ChargeBoardType;

	for(i=0;i<Init_ChargeFileSize;i++){
		detailedInfoTemp = default_ChargeDetailedInfo(i);	
		memcpy((uint8*)&upgr.upgrFileType.detailedInfo.hardVer,(uint8*)&detailedInfoTemp.hardVer,sizeof(DetailedInfo));
		set_InitUpgrBatBinFile(upgr,i)	;	
		/*���¿��Ź��Ĵ���--ι��*/
		watchdogUpdate();		
	}
}

/*
** set Assign ������̼���bin�ļ�����
*/
void set_AssignUpgrChargeBinFilePara(UpgrFilePara upgr,uint16 loc){
	uint8 buf[UpgrBinFilePara_Len] = {0};
	uint16 len = 0;
	uint16 crc = 0;
	ModifyEepromLogic modifyEepromLogicTemp = {0};

	/*ת���ṹ��洢*/
	modifyEepromLogicTemp = set_TypedefModify_Assign(&upgr,loc);
	memcpy((uint8*)&buf[len],(uint8*)&modifyEepromLogicTemp.board,sizeof(ModifyEepromLogic));
	len += sizeof(ModifyEepromLogic);	

	/*����CRC*/
	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);
	
	eeprom_write(UpgrBinFilePara_ChargeAddr(loc), (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(UpgrBinFilePara_ChargeAddr(loc), (uint8*)&buf[0], len);		

	/*���³�����̼�������*/
	upgrChargeBinFilePara[loc] = upgr;
}

/*
** ��λ���³�����̼�������
*/
void reset_UpdateChargeFirePara(void){
	uint8 buf[UpgrBinFilePara_Len] = {0};
	uint8 buf_bkp[UpgrBinFilePara_Len] = {0};
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;
	uint8 len = 0;
	DetailedInfo detailedInfoTemp = {0};
	uint8 loc = 0;
	ModifyEepromLogic modifyELTemp = {0};

	for(loc=0;loc<sysParam.chargeSaveInfo.fireSzie;loc++){
		len = 0;
		eeprom_read(UpgrBinFilePara_ChargeAddr(loc), (uint8 *)&buf[len], UpgrBinFilePara_Len);
		BKP_ReadBackupRegDat(UpgrBinFilePara_ChargeAddr(loc), (uint8*)&buf_bkp[len], UpgrBinFilePara_Len);

		/*����CRCУ��*/
		crc = CRC16((uint8 *)&buf[len], sizeof(ModifyEepromLogic));
		crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(ModifyEepromLogic));

		len += sizeof(ModifyEepromLogic);

		/*get CRC16У��*/
		memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
		memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

		#if ClearAllFireware == 1
			getCrc = 0;
			getCrc_bkp = 0;
		#endif

		if(crc == getCrc){
			memcpy((uint8*)&modifyELTemp.board,(uint8*)&buf[0],sizeof(ModifyEepromLogic));
			upgrChargeBinFilePara[loc] = conv_TypedefModify(modifyELTemp);
			BKP_WriteBackupDat(UpgrBinFilePara_ChargeAddr(loc),(uint8*)&buf[0], UpgrBinFilePara_Len);
		}else if(crc_bkp == getCrc_bkp){
			memcpy((uint8*)&modifyELTemp.board,(uint8*)&buf_bkp[0],sizeof(ModifyEepromLogic));
			upgrChargeBinFilePara[loc] = conv_TypedefModify(modifyELTemp);
			eeprom_write(UpgrBinFilePara_ChargeAddr(loc), (uint8 *)&buf_bkp[0], UpgrBinFilePara_Len);
		}else{
			detailedInfoTemp = default_ChargeDetailedInfo(loc);
			upgrChargeBinFilePara[loc].upgrFileType.board = ChargeBoardType;
			memcpy((uint8*)&upgrChargeBinFilePara[loc].upgrFileType.detailedInfo.hardVer,(uint8*)&detailedInfoTemp.hardVer,sizeof(DetailedInfo));
			if(detailedInfoTemp.hardVer != 0){/*defalut������̼���ϵ��*/
				set_AssignUpgrChargeBinFilePara(upgrChargeBinFilePara[loc], loc);
			}
		}
		/*
		** ���¿��Ź��Ĵ���--ι��
		*/
		watchdogUpdate();		
	}
}

/*
** get������̼���λ����Ϣ
*/
int16 get_ChargeLocation(UpgrFilePara upgr){
	int16 loc = -2;
	uint16 i = 0;
	uint16 size = get_ChargeFireSize();
	UpgrFilePara upgrTemp = {0};

	if(size>=Init_ChargeFileSize/*����Ĭ�ϳ�����̼�������*/){
		loc = -1;/*δ����֮ǰ,�����޳�����̼�����Ϣ*/
		for(i=0;i < size;i++){
			upgrTemp = get_UpgrChargeFilePara(i);
			if(memcmp((uint8*)&upgrTemp.upgrFileType.detailedInfo.hardVer,
				(uint8*)&upgr.upgrFileType.detailedInfo.hardVer,sizeof(DetailedInfo)) == 0){
				loc = i;
				break;
			}
		}
	}else{/*�洢��ع̼���������ʧ*/
		init_chargePara();
		set_ChargeFireSize(Init_ChargeFileSize);
		loc = -2;
	}

	if(loc == -2){/*������ʧ:�ݹ�,�ص�*/
		loc = get_ChargeLocation(upgr);
	}

	return loc;
}
/***********************************************************************************************/

/*
** set ϵͳ�¶�ֵ(��׼+�ز�)
*/
void set_Tsave(Tsave ts){
	uint8 buf[TSave_Len] = {0};
	uint16 crc = 0;
	uint8 len = 0;
	
	/*
	** ����ϵͳ����
	*/	
	sysParam.tsave = ts;

	memcpy((uint8*)&buf[len],(uint8*)&sysParam.tsave,sizeof(Tsave));
	len += sizeof(Tsave);

	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(TSave_Addr, (uint8 *)&buf[0], TSave_Len);
	BKP_WriteBackupDat(TSave_Addr, (uint8_t *)&buf[0], TSave_Len);
}

/*
** update ϵͳ�¶�ֵ
*/
void update_Tsave(void){
	Tsave ts = {400,50};
	uint8 buf[TSave_Len] = {0};
	uint8 buf_bkp[TSave_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(TSave_Addr, (uint8 *)&buf[0], TSave_Len);
	BKP_ReadBackupRegDat(TSave_Addr, (uint8*)&buf_bkp[0], TSave_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[0], sizeof(Tsave));
	crc_bkp = CRC16((uint8 *)&buf_bkp[0], sizeof(Tsave));

	len += sizeof(Tsave);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.tsave.base,(uint8*)&buf[0],sizeof(Tsave));
		BKP_WriteBackupDat(TSave_Addr, (uint8*)&buf[0], TSave_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.tsave.base,(uint8*)&buf_bkp[0],sizeof(Tsave));
		eeprom_write(TSave_Addr, (uint8*)&buf_bkp[0], TSave_Len);
	}else{
		set_Tsave(ts);
	}	
}

/*
** get ϵͳ�¶�ֵ(��׼+�ز�)
*/
Tsave get_Tsave(void){
	return sysParam.tsave;
}

/*
** set SOC��ֵ
*/
void set_SocLimit(uint8 soc){
	uint8 buf[SocLimit_Len] = {0};
	uint16 crc = 0;
	uint8 len = 0;
	
	/*
	** ����ϵͳ����
	*/	
	sysParam.socLimit = soc;

	memcpy((uint8*)&buf[len],(uint8*)&sysParam.socLimit,sizeof(uint8));
	len += sizeof(uint8);

	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(SocLimit_Addr, (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(SocLimit_Addr, (uint8_t *)&buf[0], len);
}

/*
** update SOC��ֵ
*/
void update_SocLimit(void){
	uint8 socLimit = 95;
	uint8 buf[SocLimit_Len] = {0};
	uint8 buf_bkp[SocLimit_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(SocLimit_Addr, (uint8 *)&buf[0], SocLimit_Len);
	BKP_ReadBackupRegDat(SocLimit_Addr, (uint8*)&buf_bkp[0], SocLimit_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[0], sizeof(uint8));
	crc_bkp = CRC16((uint8 *)&buf_bkp[0], sizeof(uint8));

	len += sizeof(uint8);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.socLimit,(uint8*)&buf[0],sizeof(uint8));
		BKP_WriteBackupDat(SocLimit_Addr, (uint8*)&buf[0], SocLimit_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.socLimit,(uint8*)&buf_bkp[0],sizeof(uint8));
		eeprom_write(SocLimit_Addr, (uint8*)&buf_bkp[0], SocLimit_Len);
	}else{
		set_SocLimit(socLimit);
	}	
}

/*
** get SOC��ֵ
*/
uint8 get_SocLimit(void){
	return sysParam.socLimit;
}

/*
** set ���ʱ����ֵ
*/
void set_ChgTimeLimit(uint16 time){
	uint8 buf[ChgTimeLimit_Len] = {0};
	uint16 crc = 0;
	uint8 len = 0;
	
	/*
	** ����ϵͳ����
	*/	
	sysParam.chgTimeOut = time;

	memcpy((uint8*)&buf[len],(uint8*)&sysParam.chgTimeOut,sizeof(uint16));
	len += sizeof(uint16);

	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(ChgTimeLimit_Addr, (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(ChgTimeLimit_Addr, (uint8_t *)&buf[0], len);
}

/*
** update ���ʱ����ֵ
*/
void update_ChgTimeLimit(void){
	uint16 chgTimeLimit = 360;/*6Сʱ--360min*/
	uint8 buf[ChgTimeLimit_Len] = {0};
	uint8 buf_bkp[ChgTimeLimit_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(ChgTimeLimit_Addr, (uint8 *)&buf[0], ChgTimeLimit_Len);
	BKP_ReadBackupRegDat(ChgTimeLimit_Addr, (uint8*)&buf_bkp[0], ChgTimeLimit_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[0], sizeof(uint16));
	crc_bkp = CRC16((uint8 *)&buf_bkp[0], sizeof(uint16));

	len += sizeof(uint16);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.chgTimeOut,(uint8*)&buf[0],sizeof(uint16));
		BKP_WriteBackupDat(ChgTimeLimit_Addr, (uint8*)&buf[0], ChgTimeLimit_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.chgTimeOut,(uint8*)&buf_bkp[0],sizeof(uint16));
		eeprom_write(ChgTimeLimit_Addr, (uint8*)&buf_bkp[0], ChgTimeLimit_Len);
	}else{
		set_ChgTimeLimit(chgTimeLimit);
	}	
}

/*
** get ���ʱ����ֵ
*/
uint16 get_ChgTimeLimit(void){
	return sysParam.chgTimeOut;
}

/*
** set �����������ֵ
*/
void set_ChgOverTempLimit(uint16 temp){
	uint8 buf[ChgOverTempLimit_Len] = {0};
	uint16 crc = 0;
	uint8 len = 0;
	
	/*
	** ����ϵͳ����
	*/	
	sysParam.chgOverTemp = temp;

	memcpy((uint8*)&buf[len],(uint8*)&sysParam.chgOverTemp,sizeof(uint16));
	len += sizeof(uint16);

	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(ChgOverTempLimit_Addr, (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(ChgOverTempLimit_Addr, (uint8_t *)&buf[0], len);
}

/*
** update �����������ֵ
*/
void update_ChgOverTempLimit(void){
	uint16 temp = 800;/*80���϶�*/
	uint8 buf[ChgOverTempLimit_Len] = {0};
	uint8 buf_bkp[ChgOverTempLimit_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(ChgOverTempLimit_Addr, (uint8 *)&buf[len], ChgOverTempLimit_Len);
	BKP_ReadBackupRegDat(ChgOverTempLimit_Addr, (uint8*)&buf_bkp[len], ChgOverTempLimit_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], sizeof(uint16));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(uint16));

	len += sizeof(uint16);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.chgOverTemp,(uint8*)&buf[0],sizeof(uint16));
		BKP_WriteBackupDat(ChgOverTempLimit_Addr, (uint8*)&buf[0], ChgOverTempLimit_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.chgOverTemp,(uint8*)&buf_bkp[0],sizeof(uint16));
		eeprom_write(ChgOverTempLimit_Addr, (uint8*)&buf_bkp[0], ChgOverTempLimit_Len);
	}else{
		set_ChgOverTempLimit(temp);
	}
}

/*
** get �����������ֵ
*/
uint16 get_ChgOverTempLimit(void){
	return sysParam.chgOverTemp;
}

/*
** set ������ֹ�����ֵ
*/
void set_ChgDoorOTLimit(uint16 temp){
	uint8 buf[ChgDoorOTLimit_Len] = {0};
	uint16 crc = 0;
	uint8 len = 0;
	
	/*
	** ����ϵͳ����
	*/	
	sysParam.chgDoorOTemp = temp;

	memcpy((uint8*)&buf[len],(uint8*)&sysParam.chgDoorOTemp,sizeof(uint16));
	len += sizeof(uint16);

	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(ChgDoorOTLimit_Addr, (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(ChgDoorOTLimit_Addr, (uint8_t *)&buf[0], len);
}

/*
** update ������ֹ�����ֵ
*/
void update_ChgDoorOTLimit(void){
	uint16 temp = 700;/*70���϶�*/
	uint8 buf[ChgDoorOTLimit_Len] = {0};
	uint8 buf_bkp[ChgDoorOTLimit_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(ChgDoorOTLimit_Addr, (uint8 *)&buf[len], ChgDoorOTLimit_Len);
	BKP_ReadBackupRegDat(ChgDoorOTLimit_Addr, (uint8*)&buf_bkp[len], ChgDoorOTLimit_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], sizeof(uint16));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(uint16));

	len += sizeof(uint16);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.chgDoorOTemp,(uint8*)&buf[0],sizeof(uint16));
		BKP_WriteBackupDat(ChgDoorOTLimit_Addr, (uint8*)&buf[0], ChgDoorOTLimit_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.chgDoorOTemp,(uint8*)&buf_bkp[0],sizeof(uint16));
		eeprom_write(ChgDoorOTLimit_Addr, (uint8*)&buf_bkp[0], ChgDoorOTLimit_Len);
	}else{
		set_ChgDoorOTLimit(temp);
	}
}

/*
** get ������ֹ�����ֵ
*/
uint16 get_ChgDoorOTLimit(void){
	return sysParam.chgDoorOTemp;
}

/*-----------------------------------�����������ֵV1.5-----------------------------------------------------*/
/*
** set Charger OC Limit
*/
void set_ChargerOCLimit(uint8 limit){
	uint8 buf[ChargerOCLimit_Len] = {0};
	uint16 crc = 0;
	uint8 len = 0;

	sysParam.chargerOCLimit = limit;
	
	memcpy((uint8*)&buf[len],(uint8*)&sysParam.chargerOCLimit,sizeof(uint8));
	len += sizeof(uint8);

	/*����CRC*/
	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(ChargerOCLimit_Addr, (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(ChargerOCLimit_Addr, (uint8_t *)&buf[0], len);
}

/*
** get Charger OC Limit
*/
uint8 get_ChargerOCLimit(void){
	return sysParam.chargerOCLimit;
}

/*
** reset Charger OC Limit
*/
void reset_ChargerOCLimit(void){
	uint8 limit = 8;/*8A*/
	uint8 buf[ChargerOCLimit_Len] = {0};
	uint8 buf_bkp[ChargerOCLimit_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(ChargerOCLimit_Addr, (uint8 *)&buf[len], ChargerOCLimit_Len);
	BKP_ReadBackupRegDat(ChargerOCLimit_Addr, (uint8*)&buf_bkp[len], ChargerOCLimit_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], sizeof(uint8));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(uint8));

	len += sizeof(uint8);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.chargerOCLimit,(uint8*)&buf[0],sizeof(uint8));
		BKP_WriteBackupDat(ChargerOCLimit_Addr, (uint8*)&buf[0], ChargerOCLimit_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.chargerOCLimit,(uint8*)&buf_bkp[0],sizeof(uint8));
		eeprom_write(ChargerOCLimit_Addr, (uint8*)&buf_bkp[0], ChargerOCLimit_Len);
	}else{
		set_ChargerOCLimit(limit);
	}
}
/*-------------------------------------------------------------------------------------------------------*/

/*
** set ����
*/
void set_SysDomain(SysDomain s){
	uint8 buf[LocalRemoteDomain_Len] = {0};
	uint16 crc = 0;
	uint8 len = 0;
	
	/*
	** ����ϵͳ����
	*/	
	sysParam.sysDomain = s;

	memcpy((uint8*)&buf[len],(uint8*)&sysParam.sysDomain.local[0],sizeof(SysDomain));
	len += sizeof(SysDomain);

	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(LocalRemoteDomain_Addr, (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(LocalRemoteDomain_Addr, (uint8_t *)&buf[0], len);	
}

/*
** update ����
*/
void update_SysDomain(void){
	SysDomain sysDomainTemp = {{"https://mt.smart2charge.com:30003"},{"https://ebike.meituan.com"}};
	uint8 buf[LocalRemoteDomain_Len] = {0};
	uint8 buf_bkp[LocalRemoteDomain_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(LocalRemoteDomain_Addr, (uint8 *)&buf[len], LocalRemoteDomain_Len);
	BKP_ReadBackupRegDat(LocalRemoteDomain_Addr, (uint8*)&buf_bkp[len], LocalRemoteDomain_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], 128);
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], 128);

	len += sizeof(SysDomain);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.sysDomain.local[0],(uint8*)&buf[0],sizeof(SysDomain));
		BKP_WriteBackupDat(LocalRemoteDomain_Addr, (uint8*)&buf[0], LocalRemoteDomain_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.sysDomain.local[0],(uint8*)&buf_bkp[0],sizeof(SysDomain));
		eeprom_write(LocalRemoteDomain_Addr, (uint8*)&buf_bkp[0], LocalRemoteDomain_Len);
	}else{
		set_SysDomain(sysDomainTemp);
	}
}

/*
** get ����
*/
SysDomain get_SysDomain(void){
	return sysParam.sysDomain;
}

/*
** set Sys �豸ID
*/
void set_SysDeviceId(DeviceId id){
	uint8 buf[SysDeviceId_Len] = {0};
	uint16 crc = 0;
	uint8 len = 0;
	
	/*
	** ����ϵͳ����
	*/	
	sysParam.deviceId = id;

	memcpy((uint8*)&buf[len],(uint8*)&sysParam.deviceId.show[0],sizeof(DeviceId));
	len += sizeof(DeviceId);

	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(SysDeviceID_Addr, (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(SysDeviceID_Addr, (uint8_t *)&buf[0], len);		
}

/*
** update Sys �豸ID
*/
void update_SysDeviceId(void){
	uint8 temp[16] = {'G',0x31,0x31,0x35,0x32,0x31,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31};
	DeviceId deviceIdTemp = {0};
	uint8 buf[SysDeviceId_Len] = {0};
	uint8 buf_bkp[SysDeviceId_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(SysDeviceID_Addr, (uint8 *)&buf[len], SysDeviceId_Len);
	BKP_ReadBackupRegDat(SysDeviceID_Addr, (uint8*)&buf_bkp[len], SysDeviceId_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], sizeof(DeviceId));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(DeviceId));

	len += sizeof(DeviceId);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.deviceId.show[0],(uint8*)&buf[0],sizeof(DeviceId));
		BKP_WriteBackupDat(SysDeviceID_Addr, (uint8*)&buf[0], SysDeviceId_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.deviceId.show[0],(uint8*)&buf_bkp[0],sizeof(DeviceId));
		eeprom_write(SysDeviceID_Addr, (uint8*)&buf_bkp[0], SysDeviceId_Len);
	}else{
		memcpy((uint8*)&deviceIdTemp.show[0],(uint8*)&temp[0],16);
		set_SysDeviceId(deviceIdTemp);
	}
}

/*
** get Sys �豸ID
*/
DeviceId get_SysDeviceId(void){
	return sysParam.deviceId;
}

/*
** get ����˲���
*/
Serve get_ServePara(void){
	return sysParam.serve;
}

/*
** set ����˲���
*/
void setServePara(Serve s){
 	uint8 buf[Register_LogoutInfo_Len] = {0};
 	uint16 crc = 0;
 	uint8 len = 0;
 	
 	/*
 	** ����ϵͳ����
 	*/	
 	sysParam.serve = s;

 	memcpy((uint8*)&buf[len],(uint8*)&sysParam.serve.registerFlag,sizeof(Serve));
 	len += sizeof(Serve);

 	crc = CRC16((uint8 *)&buf[0], len);
 	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
 	len += sizeof(uint16);

 	eeprom_write(Register_LogoutInfo_Addr, (uint8 *)&buf[0], len);
 	BKP_WriteBackupDat(Register_LogoutInfo_Addr, (uint8_t *)&buf[0], len);	
}

/*
** update ����˲���
*/
void update_ServerPara(void){
	Serve ss = {0};
	uint8 buf[Register_LogoutInfo_Len] = {0};
	uint8 buf_bkp[Register_LogoutInfo_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(Register_LogoutInfo_Addr, (uint8 *)&buf[len], Register_LogoutInfo_Len);
	BKP_ReadBackupRegDat(Register_LogoutInfo_Addr, (uint8*)&buf_bkp[len], Register_LogoutInfo_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], sizeof(Serve));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(Serve));

	len += sizeof(Serve);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.serve.registerFlag,(uint8*)&buf[0],sizeof(Serve));
		BKP_WriteBackupDat(Register_LogoutInfo_Addr, (uint8*)&buf[0], Register_LogoutInfo_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.serve.registerFlag,(uint8*)&buf_bkp[0],sizeof(Serve));
		eeprom_write(Register_LogoutInfo_Addr, (uint8*)&buf_bkp[0], Register_LogoutInfo_Len);
	}else{
		ss.registerFlag = false;
		setServePara(ss);
	}

	/*
	** ����ע��ע��״̬
	*/
	if(sysParam.serve.registerFlag == true){
		set_RegisterState(1);/*��ע��*/
	}
}

/*
** get WIFI Name PW
*/
WifiSet get_WifiNamePwSysSet(void){
	return sysParam.wifiSet;
}

/*
** set Wifi Name  
*/
void set_WifiNameSysSet(uint8* space){
	uint8 buf[WifiNamePara_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	WifiSet wifiSetTemp = sysParam.wifiSet;

	memcpy((uint8*)&wifiSetTemp.name[0],(uint8*)&space[0],32);

	 memcpy((uint8*)&buf[len],(uint8*)&wifiSetTemp.name[0],32);
 	len += 32;

 	crc = CRC16((uint8 *)&buf[0], len);
 	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
 	len += sizeof(uint16);

 	eeprom_write(WifiNamePara_Addr, (uint8 *)&buf[0], len);
 	BKP_WriteBackupDat(WifiNamePara_Addr, (uint8_t *)&buf[0], len);	

	/*
	** ����ϵͳ����
	*/
	sysParam.wifiSet = wifiSetTemp;
}

/*
** update Wifi Name
*/
void update_WifiNameSysPara(void){
	uint8 buf[WifiNamePara_Len] = {0};
	uint8 buf_bkp[WifiNamePara_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	
	uint8 initName[32] = {"ZLD"};

	eeprom_read(WifiNamePara_Addr, (uint8 *)&buf[len], WifiNamePara_Len);
	BKP_ReadBackupRegDat(WifiNamePara_Addr, (uint8*)&buf_bkp[len], WifiNamePara_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], 32);
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], 32);

	len += 32;

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.wifiSet.name[0],(uint8*)&buf[0],32);
		BKP_WriteBackupDat(WifiNamePara_Addr, (uint8*)&buf[0], WifiNamePara_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.wifiSet.name[0],(uint8*)&buf_bkp[0],32);
		eeprom_write(WifiNamePara_Addr, (uint8*)&buf_bkp[0], WifiNamePara_Len);
	}else{	
		set_WifiNameSysSet((uint8 *)&initName[0]);
	}
}

/*
** set Wifi Pw 
*/
void set_WifiPwSysSet(uint8* space){
	uint8 buf[WifiPwPara_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	WifiSet wifiSetTemp = sysParam.wifiSet;

	memcpy((uint8*)&wifiSetTemp.pw[0],(uint8*)&space[0],32);

	 memcpy((uint8*)&buf[len],(uint8*)&wifiSetTemp.pw[0],32);
 	len += 32;

 	crc = CRC16((uint8 *)&buf[0], len);
 	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
 	len += sizeof(uint16);

 	eeprom_write(WifiPwPara_Addr, (uint8 *)&buf[0], len);
 	BKP_WriteBackupDat(WifiPwPara_Addr, (uint8_t *)&buf[0], len);	
	
	/*
	** ����ϵͳ����
	*/
	sysParam.wifiSet = wifiSetTemp;
}

/*
** update Wifi Pw
*/
void update_WifiPwSysPara(void){
	uint8 buf[WifiPwPara_Len] = {0};
	uint8 buf_bkp[WifiPwPara_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	
	uint8 initPw[32] = {"zld16888888"};

	eeprom_read(WifiPwPara_Addr, (uint8 *)&buf[len], WifiPwPara_Len);
	BKP_ReadBackupRegDat(WifiPwPara_Addr, (uint8*)&buf_bkp[len], WifiPwPara_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], 32);
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], 32);

	len += 32;

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.wifiSet.pw[0],(uint8*)&buf[0],32);
		BKP_WriteBackupDat(WifiPwPara_Addr, (uint8*)&buf[0], WifiPwPara_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.wifiSet.pw[0],(uint8*)&buf_bkp[0],32);
		eeprom_write(WifiPwPara_Addr, (uint8*)&buf_bkp[0], WifiPwPara_Len);
	}else{	
		set_WifiPwSysSet((uint8 *)&initPw[0]);
	}
}

/**********************��������������ʼ�2020/12/22--�ܿع̼��Ȳ���***********************************************************/
/*
** set Fire Upgr Num
*/
void set_FireUpgrNum(uint8 type,int16 num,uint16 loc){
	uint8 buf[BatFireLimitNum_Len] = {0};
	uint8 len = 0;
	uint16 crc16 = 0;
	
	switch(type){
		case 0x03:/*��ع̼���*/
			sysParam.fireUpgrNumLimit.bat[loc] = num;
			break;
		case 0x04:/*������̼���*/
			sysParam.fireUpgrNumLimit.charger[loc] = num;
			break;
	}

	if((int16)num == (int16)-1){/*���޴�������*/
		/*��ʵ����������*/
		set_FireRealityRunNum(type, 0, loc);
	}

	memcpy((uint8*)&buf[0],(uint8*)&num,sizeof(int16));
	len += sizeof(int16);

	/*����CRC16*/
	crc16 = CRC16((uint8 *)&buf[0], len);
	/*����CRC16*/
	memcpy((uint8*)&buf[len],(uint8*)&crc16,sizeof(uint16));
	len += sizeof(uint16);

	/*дEEPROM��Backup����*/
	switch(type){
		case 0x03:/*��ع̼���*/
			eeprom_write(BatFireLimitNum_Addr(loc), (uint8 *)&buf[0], len);
			BKP_WriteBackupDat(BatFireLimitNum_Addr(loc), (uint8_t *)&buf[0], len);			
			break;
		case 0x04:/*������̼���*/
			eeprom_write(ChargeLimitNum_Addr(loc), (uint8 *)&buf[0], len);
			BKP_WriteBackupDat(ChargeLimitNum_Addr(loc), (uint8_t *)&buf[0], len);			
			break;
	}

}

/*
** get Fire Upgr Num
*/
int16 get_FireUpgrNum(uint8 type,uint16 loc){
	int16 num = -1;
	
	switch(type){
		case 0x03:/*��ع̼���*/
			num = sysParam.fireUpgrNumLimit.bat[loc];
			break;
		case 0x04:/*������̼���*/
			num = sysParam.fireUpgrNumLimit.charger[loc];
			break;
	}
	return num;
}

/*
** reset update Fire Para -- Limit Num -- ��ع̼���λ���� + ������̼�����������
*/
void reset_UpdateFirePara(void){
	uint8 buf[BatFireLimitNum_Len] = {0};
	uint8 buf_bkp[BatFireLimitNum_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc = 0;
	uint16 getCrc_bkp = 0;		
	uint16 loc = 0;

	/*��ع̼������ƴ���--ƽ̨�·�*/
	for(loc = 0;loc < get_batFireSize();loc++){
		len = 0;
		eeprom_read(BatFireLimitNum_Addr(loc), (uint8 *)&buf[0], BatFireLimitNum_Len);
		BKP_ReadBackupRegDat(BatFireLimitNum_Addr(loc), (uint8_t *)&buf_bkp[0], BatFireLimitNum_Len);

		/*����CRC16*/
		crc = CRC16((uint8 *)&buf[0], sizeof(uint16));
		crc_bkp = CRC16((uint8 *)&buf_bkp[0], sizeof(uint16));

		/*����������ƫ����*/
		len += sizeof(uint16);

		/*get �洢CRC16*/
		memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
		memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

		/*�ȶ�У��*/
		if(crc == getCrc){
			memcpy((uint8*)&sysParam.fireUpgrNumLimit.bat[loc],(uint8*)&buf[0],sizeof(uint16));
			BKP_WriteBackupDat(BatFireLimitNum_Addr(loc), (uint8_t *)&buf[0],BatFireLimitNum_Len );
		}else if(crc_bkp == getCrc_bkp){
			memcpy((uint8*)&sysParam.fireUpgrNumLimit.bat[loc],(uint8*)&buf_bkp[0],sizeof(uint16));
			eeprom_write(BatFireLimitNum_Addr(loc), (uint8 *)&buf_bkp[0], BatFireLimitNum_Len);
		}else{
			set_FireUpgrNum(0x03, -1, loc);
		}

		/*ι��*/
		watchdogUpdate();
	}

	/*������̼������ƴ���--ƽ̨�·�*/
	for(loc = 0;loc <get_ChargeFireSize();loc++){
		len = 0;
		eeprom_read(ChargeLimitNum_Addr(loc), (uint8 *)&buf[0], ChargeFireLimitNum_Len);
		BKP_ReadBackupRegDat(ChargeLimitNum_Addr(loc), (uint8_t *)&buf_bkp[0], ChargeFireLimitNum_Len);

		/*����CRC16*/
		crc = CRC16((uint8 *)&buf[0], sizeof(uint16));
		crc_bkp = CRC16((uint8 *)&buf_bkp[0], sizeof(uint16));

		/*����������ƫ����*/
		len += sizeof(uint16);

		/*get �洢CRC16*/
		memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
		memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

		/*�ȶ�У��*/
		if(crc == getCrc){
			memcpy((uint8*)&sysParam.fireUpgrNumLimit.charger[loc],(uint8*)&buf[0],sizeof(uint16));
			BKP_WriteBackupDat(ChargeLimitNum_Addr(loc), (uint8_t *)&buf[0],ChargeFireLimitNum_Len );
		}else if(crc_bkp == getCrc_bkp){
			memcpy((uint8*)&sysParam.fireUpgrNumLimit.charger[loc],(uint8*)&buf_bkp[0],sizeof(uint16));
			eeprom_write(ChargeLimitNum_Addr(loc), (uint8 *)&buf_bkp[0], ChargeFireLimitNum_Len);
		}else{
			set_FireUpgrNum(0x04, -1, loc);
		}

		/*ι��*/
		watchdogUpdate();
	}	
}

/*
** set Fire Reality Run Num
*/
void set_FireRealityRunNum(uint8 type,uint16 num,uint16 loc){
	uint8 buf[BatFireLimitNum_Len] = {0};
	uint8 len = 0;
	uint16 crc16 = 0;
	
	switch(type){
		case 0x03:/*��ع̼���*/
			sysParam.fireUpgrNumLimit.exceBat[loc] = num;
			break;
		case 0x04:/*������̼���*/
			sysParam.fireUpgrNumLimit.exceCharger[loc] = num;
			break;
	}

	memcpy((uint8*)&buf[0],(uint8*)&num,sizeof(uint16));
	len += sizeof(uint16);

	/*����CRC16*/
	crc16 = CRC16((uint8 *)&buf[0], len);
	/*����CRC16*/
	memcpy((uint8*)&buf[len],(uint8*)&crc16,sizeof(uint16));
	len += sizeof(uint16);

	/*дEEPROM��Backup����*/
	switch(type){
		case 0x03:/*��ع̼���*/
			eeprom_write(BatRealityUpgrNum_Addr(loc), (uint8 *)&buf[0], len);
			BKP_WriteBackupDat(BatRealityUpgrNum_Addr(loc), (uint8_t *)&buf[0], len);			
			break;
		case 0x04:/*������̼���*/
			eeprom_write(ChargeRealityNum_Addr(loc), (uint8 *)&buf[0], len);
			BKP_WriteBackupDat(ChargeRealityNum_Addr(loc), (uint8_t *)&buf[0], len);							
			break;
	}
}

/*
** get Fire Reality Run Num
*/
uint16 get_FireRealityRunNum(uint8 type,uint16 loc){
	switch(type){
		case 0x03:/*��ع̼���*/
			return sysParam.fireUpgrNumLimit.exceBat[loc];
		case 0x04:/*������̼���*/
			return sysParam.fireUpgrNumLimit.exceCharger[loc];
	}

	return 0;
}

/*
** reset_Update Fire Reality Run Num -- ��ع̼���������--ʵ���������� + ������̼���ʵ����������
*/
void reset_UpdateFireRealityRunNum(void){
	uint8 buf[BatRealityUpgrNum_Len] = {0};
	uint8 buf_bkp[BatRealityUpgrNum_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc = 0;
	uint16 getCrc_bkp = 0;		
	uint16 loc = 0;

	/*��ع̼���ʵ����������*/
	for(loc = 0;loc < get_batFireSize();loc++){
		len = 0;
		eeprom_read(BatRealityUpgrNum_Addr(loc), (uint8 *)&buf[0], BatRealityUpgrNum_Len);
		BKP_ReadBackupRegDat(BatRealityUpgrNum_Addr(loc), (uint8_t *)&buf_bkp[0], BatRealityUpgrNum_Len);

		/*����CRC16*/
		crc = CRC16((uint8 *)&buf[0], sizeof(uint16));
		crc_bkp = CRC16((uint8 *)&buf_bkp[0], sizeof(uint16));

		/*����������ƫ����*/
		len += sizeof(uint16);

		/*get �洢CRC16*/
		memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
		memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

		/*�ȶ�У��*/
		if(crc == getCrc){
			memcpy((uint8*)&sysParam.fireUpgrNumLimit.exceBat[loc],(uint8*)&buf[0],sizeof(uint16));
			BKP_WriteBackupDat(BatRealityUpgrNum_Addr(loc), (uint8_t *)&buf[0],BatRealityUpgrNum_Len );
		}else if(crc_bkp == getCrc_bkp){
			memcpy((uint8*)&sysParam.fireUpgrNumLimit.exceBat[loc],(uint8*)&buf_bkp[0],sizeof(uint16));
			eeprom_write(BatRealityUpgrNum_Addr(loc), (uint8 *)&buf_bkp[0], BatRealityUpgrNum_Len);
		}else{
			set_FireRealityRunNum(0x03, 0, loc);
		}

		/*ι��*/
		watchdogUpdate();
	}	

	/*�����ʵ����������*/
	for(loc = 0;loc < get_ChargeFireSize();loc++){
		len = 0;
		eeprom_read(ChargeRealityNum_Addr(loc), (uint8 *)&buf[0], ChargeRealityNum_Len);
		BKP_ReadBackupRegDat(ChargeRealityNum_Addr(loc), (uint8_t *)&buf_bkp[0], ChargeRealityNum_Len);

		/*����CRC16*/
		crc = CRC16((uint8 *)&buf[0], sizeof(uint16));
		crc_bkp = CRC16((uint8 *)&buf_bkp[0], sizeof(uint16));

		/*����������ƫ����*/
		len += sizeof(uint16);

		/*get �洢CRC16*/
		memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
		memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

		/*�ȶ�У��*/
		if(crc == getCrc){
			memcpy((uint8*)&sysParam.fireUpgrNumLimit.exceCharger[loc],(uint8*)&buf[0],sizeof(uint16));
			BKP_WriteBackupDat(ChargeRealityNum_Addr(loc), (uint8_t *)&buf[0],ChargeRealityNum_Len );
		}else if(crc_bkp == getCrc_bkp){
			memcpy((uint8*)&sysParam.fireUpgrNumLimit.exceCharger[loc],(uint8*)&buf_bkp[0],sizeof(uint16));
			eeprom_write(ChargeRealityNum_Addr(loc), (uint8 *)&buf_bkp[0], ChargeRealityNum_Len);
		}else{
			set_FireRealityRunNum(0x04, 0, loc);
		}

		/*ι��*/
		watchdogUpdate();
	}		
}

/*
** set Clear Fire
*/
void set_ClearFire(uint8 type,bool flag,uint16 loc){
	switch(type){
		case 0x03:/*��ع̼���*/
			sysParam.clearFire.bat[loc] = flag;
			break;
		case 0x04:/*������̼���*/
			sysParam.clearFire.charger[loc] = flag;
			break;
	}
}

/*
** get Clear FIre
*/
bool get_ClearFire(uint8 type,uint16 loc){
	switch(type){
		case 0x03:/*��ع̼���*/
			return sysParam.clearFire.bat[loc];
		case 0x04:/*������̼���*/
			return sysParam.clearFire.bat[loc];
	}
	return false;
}

/*
** set Fire Enter Test model
*/
void set_FireEnterTestModel(uint8 type,bool flag,uint16 loc){
	switch(type){
		case 0x03:/*��ع̼���*/
			sysParam.fireTestModel.bat[loc] = flag;
			break;
		case 0x04:/*������̼���*/
			sysParam.fireTestModel.charger[loc] = flag;
			break;
	}
}

/*
** get Fire Enter Test model
*/
bool get_FireEnterTestModel(uint8 type,uint16 loc){
	switch(type){
		case 0x03:/*��ع̼���*/
			return sysParam.fireTestModel.bat[loc];
		case 0x04:/*������̼���*/
			return sysParam.fireTestModel.charger[loc];
	}
	return false;
}

/*
** set Test Model Fire Upgr Num -- �ɹ�����
*/
void set_TestModelFireUpgrNum(uint8 type,uint16 num,uint16 loc){
	switch(type){
		case 0x03:/*��ع̼�*/
			sysParam.fireTestModel.batUpgrNum[loc] = num;
			break;
		case 0x04:/*������̼�*/
			sysParam.fireTestModel.chargerUpgrNum[loc] = num;
			break;
	}
}

/*
** get Test Model Fire Upgr Num -- �ɹ�����
*/
uint16 get_TestModelFireUpgrNum(uint8 type,uint16 loc){
	switch(type){
		case 0x03:/*��ع̼�*/
			return sysParam.fireTestModel.batUpgrNum[loc];
		case 0x04:/*������̼�*/
			return sysParam.fireTestModel.chargerUpgrNum[loc];	
	}
	return 0;
}

/*
** set Test Model Fire Upgr Num -- ʧ�ܴ���
*/
void set_TestModelFireUpgrNumFail(uint8 type,uint16 num,uint16 loc){
	switch(type){
		case 0x03:/*��ع̼�*/
			sysParam.fireTestModel.batUpgrNumFail[loc] = num;
			break;
		case 0x04:/*������̼�*/
			sysParam.fireTestModel.chargerUpgrNumFail[loc] = num;
			break;
	}
}

/*
** get Test Model Fire Upgr Num -- ʧ�ܴ���
*/
uint16 get_TestModelFireUpgrNumFail(uint8 type,uint16 loc){
	switch(type){
		case 0x03:/*��ع̼�*/
			return sysParam.fireTestModel.batUpgrNumFail[loc];
		case 0x04:/*������̼�*/
			return sysParam.fireTestModel.chargerUpgrNumFail[loc];	
	}
	return 0;
}
/*****************************************************************************************************************************/

/*----------------------------���ų���-��������������--��˽ӿ�(�ⲿ���)-1.05---------------------------------------------*/
/*
** ���õ�ع�����ֵ
*/
void set_BatOTempLimit(int8 temp){
	uint8 buf[BatOTempLimit_Len] = {0};
	uint16 crc = 0;
	uint8 len = 0;

	sysParam.batOTempLimit = temp;
	
	memcpy((uint8*)&buf[len],(int8*)&sysParam.batOTempLimit,sizeof(int8));
	len += sizeof(int8);

	/*����CRC*/
	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(BatOTempLimit_Addr, (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(BatOTempLimit_Addr, (uint8_t *)&buf[0], len);	
}

/*
** ��ȡ��ع�����ֵ
*/
int8 get_BatOTempLimit(void){
	return(sysParam.batOTempLimit);
}

/*
** ��λ��ȡ������ֵ
*/
void reset_BatOTempLimit(void){
	int8 limit = 60;
	uint8 buf[BatOTempLimit_Len] = {0};
	uint8 buf_bkp[BatOTempLimit_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(BatOTempLimit_Addr, (uint8 *)&buf[len], BatOTempLimit_Len);
	BKP_ReadBackupRegDat(BatOTempLimit_Addr, (uint8*)&buf_bkp[len], BatOTempLimit_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], sizeof(int8));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(int8));

	len += sizeof(uint8);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.batOTempLimit,(int8*)&buf[0],sizeof(int8));
		BKP_WriteBackupDat(BatOTempLimit_Addr, (uint8*)&buf[0], BatOTempLimit_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.batOTempLimit,(int8*)&buf_bkp[0],sizeof(int8));
		eeprom_write(BatOTempLimit_Addr, (uint8*)&buf_bkp[0], BatOTempLimit_Len);
	}else{
		set_BatOTempLimit(limit);
	}
}

/*
** �����϶���ص��±���
*/
void set_NanduLowPLimit(int8 temp){
	uint8 buf[NanduLowPLimit_Len] = {0};
	uint16 crc = 0;
	uint8 len = 0;

	sysParam.nanduLowPLimit = temp;
	
	memcpy((uint8*)&buf[len],(int8*)&sysParam.nanduLowPLimit,sizeof(int8));
	len += sizeof(int8);

	/*����CRC*/
	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(NanduLowPLimit_Addr, (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(NanduLowPLimit_Addr, (uint8_t *)&buf[0], len);	
}

/*
** ��ȡ�϶���ص��±���
*/
int8 get_NanduLowPLimit(void){
	return(sysParam.nanduLowPLimit);
}

/*
** ��λ��ȡ�϶���ص��±���
*/
void reset_NanduLowPLimit(void){
	int8 limit = 15;
	uint8 buf[NanduLowPLimit_Len] = {0};
	uint8 buf_bkp[NanduLowPLimit_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(NanduLowPLimit_Addr, (uint8 *)&buf[len], NanduLowPLimit_Len);
	BKP_ReadBackupRegDat(NanduLowPLimit_Addr, (uint8*)&buf_bkp[len], NanduLowPLimit_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], sizeof(int8));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(int8));

	len += sizeof(uint8);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.nanduLowPLimit,(int8*)&buf[0],sizeof(int8));
		BKP_WriteBackupDat(NanduLowPLimit_Addr, (uint8*)&buf[0], NanduLowPLimit_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.nanduLowPLimit,(int8*)&buf_bkp[0],sizeof(int8));
		eeprom_write(NanduLowPLimit_Addr, (uint8*)&buf_bkp[0], NanduLowPLimit_Len);
	}else{
		set_NanduLowPLimit(limit);
	}
}

/*
** ���õ�ص��±���
*/
void set_BatLowPLimit(int8 temp){
	uint8 buf[BatLowPLimit_Len] = {0};
	uint16 crc = 0;
	uint8 len = 0;

	sysParam.batLowPLimit = temp;
	
	memcpy((uint8*)&buf[len],(int8*)&sysParam.batLowPLimit,sizeof(int8));
	len += sizeof(int8);

	/*����CRC*/
	crc = CRC16((uint8 *)&buf[0], len);
	memcpy((uint8*)&buf[len],(uint8*)&crc,sizeof(uint16));
	len += sizeof(uint16);

	eeprom_write(BatLowPLimit_Addr, (uint8 *)&buf[0], len);
	BKP_WriteBackupDat(BatLowPLimit_Addr, (uint8_t *)&buf[0], len);	
}

/*
** ��ȡ��ص��±���
*/
int8 get_BatLowPLimit(void){
	return(sysParam.batLowPLimit);
}

/*
** ��λ��ȡ���±���
*/
void reset_BatLowPLimit(void){
	int8 limit = 5;
	uint8 buf[BatLowPLimit_Len] = {0};
	uint8 buf_bkp[BatLowPLimit_Len] = {0};
	uint8 len = 0;
	uint16 crc = 0;
	uint16 getCrc = 0;
	uint16 crc_bkp = 0;
	uint16 getCrc_bkp = 0;	

	eeprom_read(BatLowPLimit_Addr, (uint8 *)&buf[len], BatLowPLimit_Len);
	BKP_ReadBackupRegDat(BatLowPLimit_Addr, (uint8*)&buf_bkp[len], BatLowPLimit_Len);

	/*
	** ����CRC16У��
	*/
	crc = CRC16((uint8 *)&buf[len], sizeof(int8));
	crc_bkp = CRC16((uint8 *)&buf_bkp[len], sizeof(int8));

	len += sizeof(uint8);

	/*
	** get CRC16У��
	*/
	memcpy((uint8*)&getCrc,(uint8*)&buf[len],sizeof(uint16));
	memcpy((uint8*)&getCrc_bkp,(uint8*)&buf_bkp[len],sizeof(uint16));

	if(crc == getCrc){
		memcpy((uint8*)&sysParam.batLowPLimit,(int8*)&buf[0],sizeof(int8));
		BKP_WriteBackupDat(BatLowPLimit_Addr, (uint8*)&buf[0], BatLowPLimit_Len);
	}else if(crc_bkp == getCrc_bkp){
		memcpy((uint8*)&sysParam.batLowPLimit,(int8*)&buf_bkp[0],sizeof(int8));
		eeprom_write(BatLowPLimit_Addr, (uint8*)&buf_bkp[0], BatLowPLimit_Len);
	}else{
		set_BatLowPLimit(limit);
	}
}
/*---------------------------------------------------------------------------------------------------------------------------*/

/*
** �ȴ�MCU �ȶ�ʱ��
*/
void wait_McuVoltStableTime(void){
	uint16 i=0;
	uint16 j = 0;
	for(i=0;i<4000;i++){
		for(j=0;j<1000;j++){
			/*
			** ���¿��Ź��Ĵ���--ι��
			*/
			watchdogUpdate();
		}
	}
}

/*
** ϵͳ������ʼ��
*/
void init_SysPara(void){
	/*
	** �ȴ�MCU �ȶ�ʱ��
	*/
	wait_McuVoltStableTime();
	/*
	** update ����˲���
	*/
	update_ServerPara();	
	/*
	** get��ذ��洢���еĴ�С(����)--��ʼ����
	*/
	get_SizeBatFile();
	/*
	** ��ʼ������ Ctr�ļ�������
	*/
	init_UpgrCtrFileNameLen();
	/*
	** ��ʼ�����ư��ļ���
	*/
	init_sysCtrFileName();
	/*
	** ��ʼ�ϵ����ͨѶ�����--init_UpdateUpgrCom
	*/
	init_UpdateUpgrComFilePara();
	/*
	** ��ʼ�����µ�ذ��̼�����
	*/
	init_UpdateBatFileParam()	;
	/*
	** ���¿��ư����/ͨѶ������汾��
	*/
	updateSoftVerPara();	
	
	/*----------------------���:ʵ�����а汾��ͬ�ļ����汾���Ƿ�һ��-----------------------------*/
	/*
	** ÿ�η����°���������������ڹ����ļ���������汾�ŵĶ���
	*/
	 reset_ChkCtrSoftVer();
	/*--------------------------------------------------------------------------------------------*/
	
	/***********************������̼���������*************************/
	/*
	** ��λ���³�����̼�����С
	*/
	reset_UpdateChargeFireSize();
	/*
	** ��λ���³�����̼�������
	*/
	reset_UpdateChargeFirePara();
	/******************************************************************/
	
	/**********************��������������ʼ�2020/12/22--�ܿع̼��Ȳ���***********************************************************/
	/*
	** reset update Fire Para -- Limit Num -- ��ع̼���λ���� + ������̼�����������
	*/
	reset_UpdateFirePara();	
	/*
	** reset_Update Fire Reality Run Num -- ��ع̼���������--ʵ���������� + ������̼���ʵ����������
	*/
	reset_UpdateFireRealityRunNum();	
	/*****************************************************************************************************************************/

	/*-----------------------------------�����ѹУ׼---------------------------------------------*/
	reset_PhaseVoltAdjust();
	/*--------------------------------------------------------------------------------------------*/
	
	/*
	** update ϵͳ�¶�ֵ
	*/
	update_Tsave();
	/*
	** update SOC��ֵ
	*/
	update_SocLimit();	
	/*
	** update ���ʱ����ֵ
	*/
	update_ChgTimeLimit();
	/*
	** update �����������ֵ
	*/
	update_ChgOverTempLimit();	
	/*
	** update �����������ֵ
	*/
	update_ChgDoorOTLimit();	
	/*-----------------------------------�����������ֵV1.5---------------------------------------------------------------------*/
	/*
	** reset Charger OC Limit
	*/
	reset_ChargerOCLimit();
	/*---------------------------------------------------------------------------------------------------------------------------*/
	
	/*----------------------------���ų���-��������������--��˽ӿ�(�ⲿ���)-1.05---------------------------------------------*/
	/*
	** ��λ��ȡ������ֵ
	*/
	reset_BatOTempLimit();
	/*
	** ��λ��ȡ�϶���ص��±���
	*/
	reset_NanduLowPLimit();
	/*
	** ��λ��ȡ���±���
	*/
	 reset_BatLowPLimit();	
	/*---------------------------------------------------------------------------------------------------------------------------*/
	/*
	** update ����
	*/
	update_SysDomain();	
	/*
	** update Sys �豸ID
	*/
	update_SysDeviceId();	
	/*
	** update Wifi Name
	*/
	update_WifiNameSysPara();	
	/*
	** update Wifi Pw
	*/
	update_WifiPwSysPara();	
}

