#ifndef __LOWER_UPGR_ANALYZE_H_
#define __LOWER_UPGR_ANALYZE_H_

typedef struct{
	uint8 funId;/*���ܺ���ID*/
	void (*handle)(uint8,uint8*,uint8)/*1.���ݳ��� 2.������ 3.�豸��ַ*/;
}LowerUpgrFunTable;

/*
** �²�Э��֮���ݽ���֮����������
*/
void lowerUpgrFunAnalyze(void);

#endif

