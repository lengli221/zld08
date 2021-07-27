#ifndef __COMMON_H_
#define __COMMON_H_

uint16 CRC16(uint8 *puchMsg, uint16 usDataLen);
/*
** ���������ݵ�CRC16У��
*/
uint16 CRC16_Upgrade(uint8 *puchMsg, uint16 usDataLen,uint16 uchInit,uint8* indexInit);
bool TickOut(uint32 *tick,uint32 timeout);
void Sleep(uint32 milliSec);
void uint16Tou16Ascii(uint16 tmp,uint16 *u16Ascii);
bool Compare_Data(uint8*Src,uint8*Dst,uint16 len);
void  Uint16Reverse(uint16 *buf, uint8 len);
uint8 CalcBcc(uint8 *buf,uint16 len);
uint8 calcSum(uint8 *buf,uint16 len);
/*
** ���з� 0x0D 0x0A
*/
uint8 lineBreak(uint8* space);
/*
** ����<-->�ַ���
** @param: num->����ֵ str->ת�����ַ��� radix->����
** @return: �ַ����ַ�����
*/
uint8 itoa(int num,char* str,int radix);
/*
** ָ��(����:10)��������-->�ַ���
** @param: num->����ֵ str->ת�����ַ���
** @return: �ַ����ַ�����
*/
uint8 _itoa(int num,char* str);
/*
** �����ַ�-->�ַ��� ��ӱ��ʲ���
** @param: num->����ֵ str->ת�����ַ��� rate-->����
*/
uint8 strAddRate(int num,char* str,uint32 rate);
/*
** BCDתuint32
*/
uint32 bcdToUInt(const uint8 *byBCDCode, uint16 iBCDCodeLen);
#endif

