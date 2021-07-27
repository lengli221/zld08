#include "includes.h"

/*
** ����汾�� ����ASCII��
*/
#define SoftVerNum					3

/*
** �����Ƿ���.bin�ļ��Լ���ȡ�±����
** @return:���ذ汾��Ϣ�±�
*/
int16 fileNameVerSuffix(uint8* data,uint16 len){
	int16 indexSuffix = -1;
	uint16 i = 0;

	for(i = 0;i < len - 3;i++){
		if(data[i] == '.' && data[i+1] == 'b'&& data[i+2] == 'i'&& data[i+3] == 'n'){
			/*
			** ���ذ汾��Ϣ�±�
			*/
			if(i >= SoftVerNum){/*Ŀǰ��������汾��:3 ASCII*/
				indexSuffix = (i - SoftVerNum);
			}
			break;
		}
	}
	return indexSuffix;
}

/*
** �ļ����汾�Ų���
*/
bool FileNameVerOpt(uint8* data,uint16 len){
	bool ret = false;
	int16 indexSuffix = fileNameVerSuffix((uint8 *)&data[0], len);
	uint16 ver = 0;
	uint16 i = 0;
	uint8 temp[SoftVerNum] = {0};
	UpgrFilePara upgr = getUpgrFilePara();

	if(indexSuffix != -1){/*.bin�ļ�,�Ϸ�*/
		memcpy((uint8*)&temp[0],(uint8*)&data[indexSuffix],SoftVerNum);
		for(i = 0;i < SoftVerNum;i++){/*���ļ�����ȡ����汾��*/
			ver *= 10;
			ver += temp[i] - '0';
		}
		/*
		** �Ƿ������ļ�����Ӳ���汾�ŵ�ƥ�����
		*/
		upgr.upgrFileType.fileNameLen = len;
		upgr.upgrFileType.softVer = ver;
		setUpgrFilePara(upgr);
		
		return true;
	}

	return ret;
}

