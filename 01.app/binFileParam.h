#ifndef __BINFILEPARAM_H_
#define __BINFILEPARAM_H_

/*
** Զ������֮�ļ���ز���
*/
#pragma pack(1)
typedef struct{
	uint32 len;
	uint16 crc16;
}BinFileItemPara;
#pragma pack()

#endif

