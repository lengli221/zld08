#include "includes.h"

/*
** ��ϸ˵��:����λ����Ϣ����ļ�����Ϣ
*/
void fill_FileName(uint8 type,uint8 loc,uint8* fileName){
	uint8 subFileNmae[24] = 		{
									"CDG_MD_A_B1_300_000.bin"};
	uint8 batFileName[][24] = {
					/*������*/		{"SBT_SW_A_A1_A100_000.bin"},{"SBT_SW_A_B1_B200_000.bin"},{"SBT_SW_A_B1_B210_000.bin"},{"SBT_SW_A_B1_B220_000.bin"},
									{"SBT_SW_A_B1_B230_000.bin"},{"SBT_SW_A_B1_B300_000.bin"},
					/*��ë��*/ 	{"SBT_SC_A_A1_A100_000.bin"},{"SBT_SC_A_A1_A200_000.bin"},{"SBT_SC_A_B1_B300_000.bin"},{"SBT_SC_A_B1_B310_000.bin"},
									{"SBT_SC_A_B1_B320_000.bin"},{"SBT_SC_A_B1_B330_000.bin"},{"SBT_SC_A_B1_B400_000.bin"},
					/*�϶�*/		{"SBT_ND_A_B1_B100_000.bin"},{"SBT_ND_A_B1_B200_000.bin"},{"SBT_ND_A_B1_B300_000.bin"},
					/*���ն���*/	{"SBT_XR_A_B1_B100_000.bin"},{"SBT_XR_A_B1_B200_000.bin"},
					/*�Ǻ�*/		{"SBT_XH_A_B1_B100_000.bin"},{"SBT_XH_A_B1_B200_000.bin"},{"SBT_XH_A_B1_B300_000.bin"},
					/*ATL*/		{"SBT_AT_A_B1_B100_000.bin"},
					/*CATL*/		{"SBT_CA_A_B1_B100_000.bin"},
					/*������*/		{"SBT_SW_A_B1_B211_000.bin"},
					/*20210520--����������ع̼�--20210524���������ƹ��ṩ���б�*/
					{"SBT_SC_A_B1_B500_000.bin"},
					{"SBT_SW_A_B1_B400_000.bin"},
					{"SBT_AT_A_B1_B200_000.bin"},
					{"SBT_AT_A_B1_B201_000.bin"},
					/*����:20210524--�����ƹ��ṩ�б�*/
					{"SBT_CA_A_B1_B200_000.bin"}};
	uint8 chargerFileName[][24] = {
					/*����ӯԴ Ӳ���汾102*/{"CHG_AP_A_A1_A102_000.bin"},/*20210227--ӯԴӲ���汾�޸�Ϊ:101 20210325--ӯԴӲ���汾�޸�:102*/
					/*������ʯ Ӳ���汾100*/{"CHG_SS_A_A1_A100_000.bin"}};
	uint8 fileNameTemp[24] = {0};
	

	switch(type){
		case ComBoradType:/*�ֿع̼�*/
			memcpy((uint8*)&fileNameTemp[0],(uint8*)&subFileNmae[0],24);
			break;
		case BatBoardType:/*��ع̼�*/
			memcpy((uint8*)&fileNameTemp[0],(uint8*)&batFileName[loc][0],24);
			break;
		case ChargeBoardType:/*������̼�*/
			memcpy((uint8*)&fileNameTemp[0],(uint8*)&chargerFileName[loc][0],24);
			break;
	}

	/*�����ļ���*/
	memcpy((uint8*)&fileName[0],(uint8*)&fileNameTemp[0],24);
}

/*
** ��ϸ˵��:�޸��ļ�������汾��
*/
void modify_FileNameSoftVer(uint8* softVer,uint16 sv){
	uint8 fileNameSoftVer[3] = {0};
	uint8 i = 0;

	for(i=0;i<3;i++){
		fileNameSoftVer[2-i] = (sv%10) + '0';
		sv /= 10;
	}

	memcpy((uint8*)&softVer[0],(uint8*)&fileNameSoftVer[0],3);
}

/*
** 20210202-->ת���ṹ��洢
*/
ModifyEepromLogic set_TypedefModify(UpgrFilePara* upgr){
	ModifyEepromLogic modifyEL = {0};
	int16 loc = 0;
	int16 indexSuffix = -1;

	/*�忨�̼����Ϳ���*/
	modifyEL.board = upgr->upgrFileType.board;
	

	/*�洢�����ؼ�����*/
	switch(upgr->upgrFileType.board){
		case ComBoradType:/*�ֿع̼�*/
			modifyEL.label = 0;
			break;
		case BatBoardType:/*��ع̼�*/
			loc = get_BatLocation(*upgr);
			if(loc != -1){
				modifyEL.label = loc;
			}
			break;
		case ChargeBoardType:/*������̼�*/
			loc = get_ChargeLocation(*upgr);
			if(loc != -1){
				modifyEL.label = loc;
			}
			break;
	}
	modifyEL.softver = upgr->upgrFileType.softVer;
	modifyEL.flashAddr = upgr->upgrFileType.flashAddr;
	modifyEL.binFileItemPara = upgr->upgrFileType.binFileItemPara;

	/*20210221--�״γ�ʼ���߼�,����ļ���*/
	if(upgr->upgrFileType.fileNameLen == 0){
		switch(upgr->upgrFileType.board){
			case ComBoradType:/*�ֿع̼�*/
				upgr->upgrFileType.fileNameLen = 23;								
				break;
			case BatBoardType:/*��ع̼�*/
				upgr->upgrFileType.fileNameLen = 24;		
				break;
			case ChargeBoardType:/*������̼�*/
				upgr->upgrFileType.fileNameLen = 24;		
				break;
		}
		/*�����ļ���*/
		fill_FileName(modifyEL.board, modifyEL.label, (uint8 *)&upgr->upgrFileType.fileName[0]);
		/*�޸��ļ�������汾��*/
		indexSuffix = fileNameVerSuffix((uint8 *)&upgr->upgrFileType.fileName[0], upgr->upgrFileType.fileNameLen);
		if(indexSuffix != -1){
			modify_FileNameSoftVer((uint8 *)&upgr->upgrFileType.fileName[indexSuffix], upgr->upgrFileType.softVer);
		}
	}
	return modifyEL;
}

/*
** 20210202-->ת���ṹ��洢 -- ָ����ʼ������
*/
ModifyEepromLogic set_TypedefModify_Assign(UpgrFilePara* upgr,uint8 lo){
	ModifyEepromLogic modifyEL = {0};
	int16 loc = 0;
	int16 indexSuffix = -1;

	/*�忨�̼����Ϳ���*/
	modifyEL.board = upgr->upgrFileType.board;
	

	/*�洢�����ؼ�����*/
	switch(upgr->upgrFileType.board){
		case ComBoradType:/*�ֿع̼�*/
			modifyEL.label = 0;
			break;
		case BatBoardType:/*��ع̼�*/
			loc = lo;
			if(loc != -1){
				modifyEL.label = loc;
			}
			break;
		case ChargeBoardType:/*������̼�*/
			loc = lo;
			if(loc != -1){
				modifyEL.label = loc;
			}
			break;
	}
	modifyEL.softver = upgr->upgrFileType.softVer;
	modifyEL.flashAddr = upgr->upgrFileType.flashAddr;
	modifyEL.binFileItemPara = upgr->upgrFileType.binFileItemPara;

	/*20210221--�״γ�ʼ���߼�,����ļ���*/
	if(upgr->upgrFileType.fileNameLen == 0){
		switch(upgr->upgrFileType.board){
			case ComBoradType:/*�ֿع̼�*/
				upgr->upgrFileType.fileNameLen = 23;								
				break;
			case BatBoardType:/*��ع̼�*/
				upgr->upgrFileType.fileNameLen = 24;		
				break;
			case ChargeBoardType:/*������̼�*/
				upgr->upgrFileType.fileNameLen = 24;		
				break;
		}
		/*�����ļ���*/
		fill_FileName(modifyEL.board, modifyEL.label, (uint8 *)&upgr->upgrFileType.fileName[0]);
		/*�޸��ļ�������汾��*/
		indexSuffix = fileNameVerSuffix((uint8 *)&upgr->upgrFileType.fileName[0], upgr->upgrFileType.fileNameLen);
		if(indexSuffix != -1){
			modify_FileNameSoftVer((uint8 *)&upgr->upgrFileType.fileName[indexSuffix], upgr->upgrFileType.softVer);
		}
	}
	return modifyEL;
}

/*
** 20210220 --> �ṹ��ת��
*/
UpgrFilePara conv_TypedefModify(ModifyEepromLogic modifyEL){
	UpgrFilePara upgrTemp = {0};
	DetailedInfo detailedInfoTemp = {0};
	int16 indexSuffix = -1;
	
	/*�忨����copy*/
	upgrTemp.upgrFileType.board = modifyEL.board;

	/*���ؼ�����ת����ȫ������*/
	switch(modifyEL.board){
		case ComBoradType:/*�ֿع̼�*/
			upgrTemp.upgrFileType.detailedInfo.hardVer = 300;/*12AӲ���汾��Ϊ:300*/
			upgrTemp.upgrFileType.fileNameLen = 23;
			break;
		case BatBoardType:/*��ع̼�*/
			detailedInfoTemp = default_DetailedInfo(modifyEL.label);
			upgrTemp.upgrFileType.detailedInfo = detailedInfoTemp;
			upgrTemp.upgrFileType.fileNameLen = 24;
			break;
		case ChargeBoardType:/*������̼�*/
			detailedInfoTemp = default_ChargeDetailedInfo(modifyEL.label);
			upgrTemp.upgrFileType.detailedInfo = detailedInfoTemp;
			upgrTemp.upgrFileType.fileNameLen = 24;
			break;
	}
	upgrTemp.upgrFileType.softVer = modifyEL.softver;
	upgrTemp.upgrFileType.flashAddr = modifyEL.flashAddr;
	upgrTemp.upgrFileType.binFileItemPara = modifyEL.binFileItemPara;
	/*�����ļ���*/
	fill_FileName(modifyEL.board, modifyEL.label, (uint8 *)&upgrTemp.upgrFileType.fileName[0]);
	/*�޸��ļ�������汾��*/
	indexSuffix = fileNameVerSuffix((uint8 *)&upgrTemp.upgrFileType.fileName[0], upgrTemp.upgrFileType.fileNameLen);
	if(indexSuffix != -1){
		modify_FileNameSoftVer((uint8 *)&upgrTemp.upgrFileType.fileName[indexSuffix], upgrTemp.upgrFileType.softVer);
	}
	return upgrTemp;
}

