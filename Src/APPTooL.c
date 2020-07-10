#include "APPTooL.h"
#include <string.h>
#include "BspConfig.h"
uint8_t lasttrivalue[TRLENGHT] = { 0 };


uint8_t TimData[15] = { S_SPORTTIM, S_HOUR, S_MINUTE, S_SECOND };//�ʱ��
uint8_t KcalData[25] = { S_THISKCAL, S_KCAL, S_WELCOME };//��������+��ӭ�ٴ�ʹ��
/*
����:    SingleTrig
����ȫ��:  SingleTrig
��������:    public 
����ֵ:   void
Qualifier:���δ�������
����: fun p   �ص�����,������Ĵ�����
����: int val �����ֵ
����:inval    �ص������ò���
����: index ������������
����: com   �Ƚ�ֵ
//���������:����ȫ�ֱ�����
*/
void SingleTrig(TrlCallback p, uint8_t val, uint8_t inval, uint8_t index,uint8_t com)
{
	if (index >= 0 && index < TRLENGHT)
	{
		index = index;
	}
	else if (index >= 10)
	{
		index = 9;
	}
	else
	{
		index = 0;
	}
	if (val != lasttrivalue[index])
	{
		if (val == com)  //�͵�ƽ����
		{
			(*p)(inval); //
		}
		osDelay(10);
	}
	lasttrivalue[index] = val;
}
uint8_t DataSeparation(double dat, uint8_t *res_dat)//���ݷ���-------------��С����
{
	uint8_t intlen = 0, flen = 0, reslen;
	uint16_t intData = (uint16_t)dat;//ȡ��������
	uint8_t pointarray[4] = { 0 };
	double fData = dat - intData;//ȡС������o
	//printf("fData:%f\r\n", fData);
	pointarray[0] = S_POINT;
	//��ȡʮ��λ����
	uint8_t sf = (int)(fData * 10) % 10;
	//��ȡ�ٷ�λ����
	uint8_t bf = (int)(fData * 100) % 10;
	//��ȡǧ��λ����
	uint8_t qf = (int)(dat * 1000) % 10;
	//printf("qf:%d\r\n", (int)(dat * 1000));
	//�ж�ǧ��λ�Ƿ���ڵ���5

	intlen = PrepartData(intData, res_dat);
	if (fData == 0)
	{
		reslen = intlen;
	}
	else
	{
		if (bf == 0 && qf == 0)
		{
			//insertOneByte(playarray,intlen+2,PLAY_POINT,intlen)

			pointarray[1] = sf;
			reslen = insertArray(res_dat, intlen, pointarray, 2, intlen);
		}
		else if (bf != 0 && qf == 0)
		{
			pointarray[1] = sf;
			pointarray[2] = bf;
			reslen = insertArray(res_dat, intlen, pointarray, 3, intlen);
		}
		else
		{
			pointarray[1] = sf;
			pointarray[2] = bf;
			pointarray[3] = qf;
			reslen = insertArray(res_dat, intlen, pointarray, 4, intlen);
		}
	}
	return reslen;
}
/*
uint8_t PrepartData(uint16_t dat, uint8_t *res_dat)// ������ַ�ֽ����
{
	uint8_t len, q, b, s, g;
	if (dat > 9999)
		dat = 9999;
	q = dat / 1000; //ǧλ
	b = dat % 1000 / 100;//��λ
	s = dat % 100 / 10; //ʮλ
	g = dat % 10;  //��λ

	if ((dat <= 9) && (dat >= 0)) //0-9
	{
		res_dat[0] = dat;//������ַ5-14
		len = 1;
	}
	if ((dat > 9) && (dat <= 99))//10-99
	{
		if ((dat % 10) == 0)
		{
			res_dat[0] = s;
			res_dat[1] = 10;//������ַ15 ��ʮ
			len = 2;
		}
		else
		{
			res_dat[0] = s;
			res_dat[1] = 10;
			res_dat[2] = g;
			len = 3;
		}
	}
	if ((dat > 99) && (dat <= 999))//100-999
	{
		if ((dat % 100) == 0)
		{
			res_dat[0] = b;
			res_dat[1] = 11;//������ַ16 �ǰ�
			len = 2;
		}
		else if ((dat % 10) == 0)
		{
			res_dat[0] = b;
			res_dat[1] = 11;//������ַ16 �ǰ�
			res_dat[2] = s;
			res_dat[3] = 10;//������ַ15 ��ʮ
			len = 4;
		}
		else if (dat % 100 > 0 && dat % 100 < 10)
		{
			res_dat[0] = b;
			res_dat[1] = 11;//������ַ16 �ǰ�
			res_dat[2] = 0;
			res_dat[3] = g;
			len = 4;

		}
		else
		{
			res_dat[0] = b;
			res_dat[1] = 11;//������ַ16 �ǰ�
			res_dat[2] = s;
			res_dat[3] = 10;//������ַ15 ��ʮ
			res_dat[4] = g;
			len = 5;
		}

	}
	if ((dat > 999) && (dat <= 9999))//1000-9999
	{
		if ((dat % 1000) == 0)
		{
			res_dat[0] = q;
			res_dat[1] = 12;//������ַ17 ��ǧλ
			len = 2;
		}
		else if (b == 0 && s == 0 && g != 0)//001
		{
			res_dat[0] = q;
			res_dat[1] = 12;//������ַ16 ��ǧλ
			res_dat[2] = 0;
			res_dat[3] = g;
			len = 4;
		}
		else if (b == 0 && s != 0 && g == 0)//010
		{
			res_dat[0] = q;
			res_dat[1] = 12;//������ַ16 ��ǧλ
			res_dat[2] = 0;
			res_dat[3] = s;//������ַ15 ��ʮ
			res_dat[4] = 10;
			len = 5;
		}
		else if (b == 0 && s != 0 && g != 0)//011--------��λ=0 ʮλ�͸�λ������0----------------��Ϊ�˴����ж�һ��״̬�������ظ�����
		{
			res_dat[0] = q;
			res_dat[1] = 12;
			res_dat[2] = 0;
			res_dat[3] = s;//������ַ15 ��ʮ
			res_dat[4] = 10;
			res_dat[5] = g;
			len = 6;

		}
		else if (b != 0 && s == 0 && g == 0)//100
		{
			res_dat[0] = q;
			res_dat[1] = 12;//������ַ16 ��ǧλ
			res_dat[2] = b;//������ַ15 ��ʮ
			res_dat[3] = 11;
			len = 4;
		}
		else if (b != 0 && s != 0 && g == 0)//110*********************************
		{
			res_dat[0] = q;
			res_dat[1] = 12;//������ַ16 ��ǧλ
			res_dat[2] = b;//������ַ15 ��ʮ
			res_dat[3] = 11;
			res_dat[4] = s;
			res_dat[5] = 10;
			len = 6;
		}
		else if (b != 0 && s == 0 && g != 0)//101
		{
			res_dat[0] = q;
			res_dat[1] = 12;//������ַ16 ��ǧλ
			res_dat[2] = b;//������ַ15 ��ʮ
			res_dat[3] = 11;
			res_dat[4] = 0;
			res_dat[5] = g;
			len = 6;
		}
		//else if(b!=0&&s!=0&&g!=0)//111
		else//111
		{
			res_dat[0] = q;
			res_dat[1] = 12;//������ַ16 ��ǧλ
			res_dat[2] = b;
			res_dat[3] = 11;
			res_dat[4] = s;
			res_dat[5] = 10;
			res_dat[6] = g;
			len = 7;
		}


	}

	return len;//�������鳤��
}*/
uint8_t PrepartData(uint16_t dat, uint8_t *res_dat)// ������ַ�ֽ����
{
	uint8_t len, q, b, s, g;
	if (dat > 9999)
		dat = 9999;
	q = dat / 1000; //ǧλ
	b = dat % 1000 / 100;//��λ
	s = dat % 100 / 10; //ʮλ
	g = dat % 10;  //��λ

	if ((dat <= 9) && (dat >= 0)) //0-9
	{
		res_dat[0] = dat + 5;//������ַ5-14
		len = 1;
	}
	if ((dat > 9) && (dat <= 99))//10-99
	{
		if ((dat % 10) == 0)
		{
			res_dat[0] = s + 5;
			res_dat[1] = 15;//������ַ15 ��ʮ
			len = 2;
		}
		else
		{
			res_dat[0] = s + 5;
			res_dat[1] = 15;
			res_dat[2] = g + 5;
			len = 3;
		}
	}
	if ((dat > 99) && (dat <= 999))//100-999
	{
		if ((dat % 100) == 0)
		{
			res_dat[0] = b + 5;
			res_dat[1] = 16;//������ַ16 �ǰ�
			len = 2;
		}
		else if ((dat % 10) == 0)
		{
			res_dat[0] = b + 5;
			res_dat[1] = 16;//������ַ16 �ǰ�
			res_dat[2] = s + 5;
			res_dat[3] = 15;//������ַ15 ��ʮ
			len = 4;
		}
		else if (dat % 100 > 0 && dat % 100 < 10)
		{
			res_dat[0] = b + 5;
			res_dat[1] = 16;//������ַ16 �ǰ�
			res_dat[2] = 5;
			res_dat[3] = g + 5;
			len = 4;

		}
		else
		{
			res_dat[0] = b + 5;
			res_dat[1] = 16;//������ַ16 �ǰ�
			res_dat[2] = s + 5;
			res_dat[3] = 15;//������ַ15 ��ʮ
			res_dat[4] = g + 5;
			len = 5;
		}

	}
	if ((dat > 999) && (dat <= 9999))//1000-9999
	{
		if ((dat % 1000) == 0)
		{
			res_dat[0] = q + 5;
			res_dat[1] = 17;//������ַ17 ��ǧλ
			len = 2;
		}
		else if (b == 0 && s == 0 && g != 0)//001
		{
			res_dat[0] = q + 5;
			res_dat[1] = 17;//������ַ16 ��ǧλ
			res_dat[2] = 5;
			res_dat[3] = g + 5;
			len = 4;
		}
		else if (b == 0 && s != 0 && g == 0)//010
		{
			res_dat[0] = q + 5;
			res_dat[1] = 17;//������ַ16 ��ǧλ
			res_dat[2] = 5;
			res_dat[3] = s + 5;//������ַ15 ��ʮ
			res_dat[4] = 15;
			len = 5;
		}
		else if (b == 0 && s != 0 && g != 0)//011--------��λ=0 ʮλ�͸�λ������0----------------��Ϊ�˴����ж�һ��״̬�������ظ�����
		{
			res_dat[0] = q + 5;
			res_dat[1] = 17;
			res_dat[2] = 5;
			res_dat[3] = s + 5;//������ַ15 ��ʮ
			res_dat[4] = 15;
			res_dat[5] = g + 5;
			len = 6;

		}
		else if (b != 0 && s == 0 && g == 0)//100
		{
			res_dat[0] = q + 5;
			res_dat[1] = 17;//������ַ16 ��ǧλ
			res_dat[2] = b + 5;//������ַ15 ��ʮ
			res_dat[3] = 16;
			len = 4;
		}
		else if (b != 0 && s != 0 && g == 0)//110*********************************
		{
			res_dat[0] = q + 5;
			res_dat[1] = 17;//������ַ16 ��ǧλ
			res_dat[2] = b + 5;//������ַ15 ��ʮ
			res_dat[3] = 16;
			res_dat[4] = s + 5;
			res_dat[5] = 15;
			len = 6;
		}
		else if (b != 0 && s == 0 && g != 0)//101
		{
			res_dat[0] = q + 5;
			res_dat[1] = 17;//������ַ16 ��ǧλ
			res_dat[2] = b + 5;//������ַ15 ��ʮ
			res_dat[3] = 16;
			res_dat[4] = 5;
			res_dat[5] = g + 5;
			len = 6;
		}
		//else if(b!=0&&s!=0&&g!=0)//111
		else//111
		{
			res_dat[0] = q + 5;
			res_dat[1] = 17;//������ַ16 ��ǧλ
			res_dat[2] = b + 5;
			res_dat[3] = 16;
			res_dat[4] = s + 5;
			res_dat[5] = 15;
			res_dat[6] = g + 5;
			len = 7;
		}


	}

	return len;//�������鳤��
}
//************************************
// ����:    MergedData
// ����ȫ��:  MergedData
// ��������:    public 
// ����ֵ:   uint8_t
// Qualifier: ���������������鳤�Ⱥ�����
// ����: double numdata //1,�����2,��������
// ����: uint8_t * dat  //����õ�����
//************************************
uint8_t MergedData(double numdata, uint8_t *dat, uint8_t com)//dat��С����9
{
	uint8_t reslen = 0, templen = 0;

	switch (com)
	{
	case 1:
		dat[0] = S_THISSPORT; //���������
		dat[1] = S_NUMBER;
		uint8_t tempdat[10] = { 0 };
		templen = DataSeparation(numdata, tempdat);
		reslen = insertArray(dat, 2, tempdat, templen, 1);
		break;
	case 2:
		dat[0] = S_THISKCAL; //������������
		dat[1] = S_KCAL;
		dat[2] = S_WELCOME;
		uint8_t tdat[16] = { 0 };
		templen = DataSeparation(numdata, tdat);
		reslen = insertArray(dat, 3, tdat, templen, 1);
	default:
		break;
	}
	return reslen;
}
uint8_t GetTimData(uint32_t tdata, uint8_t *dat)//dat��С����16
{
	uint8_t reslen = 0;
	dat[0] = S_SPORTTIM, dat[1] = S_HOUR, dat[2] = S_MINUTE; dat[3] = S_SECOND;
	uint8_t th = tdata / 3600, tme = tdata % 3600 / 60, ts = tdata % 3600 % 60;//ʱ���֣���
	uint8_t thlen = 0, tmelen = 0, tslen = 0;
	uint8_t thdata[8] = { 0 }, tmedata[4] = { 0 }, tsdata[4] = { 0 }; //ʱ���֣��������
	thlen = PrepartData(th, thdata);//ʱ
	tmelen = PrepartData(tme, tmedata);//��
	tslen = PrepartData(ts, tsdata);//��

	reslen = insertArray(dat, 4, thdata, thlen, 1);
	reslen = insertArray(dat, reslen, tmedata, tmelen, reslen - 2);
	reslen = insertArray(dat, reslen, tsdata, tslen, reslen - 1);
	return reslen;

}
uint8_t GetPlayData(uint16_t num, uint16_t tim, double mcal,uint8_t *dat)//�˴�dat����������С38
{
	uint8_t reslen = 0;
	uint8_t nlen = 0, tlen = 0, clen = 0;
	uint8_t tdat[20] = {0}, cdat[10] = {0};
	nlen = MergedData(num, dat, 1);
	tlen = GetTimData(tim, tdat);
	clen = MergedData(mcal,cdat,2);
	reslen = insertArray(dat, nlen, tdat, tlen, nlen);  //����ʱ��
	reslen = insertArray(dat, reslen, cdat, clen, reslen);//��������
	return reslen;
}
//�������
//************************************
// ����:    insertArray
// ����ȫ��:  insertArray
// ��������:    public 
// ����ֵ:   uint8_t         ���鳤��   -1 ʧ��
// Qualifier: ��һ�������в�����һ������
// ����: uint8_t * arr    ԭʼ����
// ����: uint16_t arrlen  ԭʼ���鳤�ȣ��ڲ���Ч���ݳ��ȣ�
// ����: uint8_t * inarr  ��������
// ����: uint16_t inarrlen �������鳤��
// ����: uint16_t pos      ����λ�� ��0��ʼ�����65535
//************************************
int insertArray(uint8_t *arr, uint16_t arrlen, uint8_t *inarr, uint16_t inarrlen,uint16_t pos)
{
	int res = -2;
	uint16_t i = 0;
	if ((arrlen + inarrlen) < 65536)
	{
		//memcpy(&buf_date[bufdateIndex], resdate, resdatelen);
		memcpy(&arr[pos + inarrlen], &arr[pos], arrlen - pos + 1);//�����pos�󣬺���inarrlenλ
		memcpy(&arr[pos], inarr, inarrlen);

		res = arrlen + inarrlen;
	}
	else
		res = -1;
	return res;
}
//����һ���ַ�
int insertOneByte(uint8_t *arr, uint16_t arrlen, uint8_t indata, uint16_t pos)
{
	int res = -2;
	int i = 0;
	if ((arrlen + 1) < 65536)
	{
		for (i = arrlen; i >= pos; i--)
		{
			arr[i + 1] = arr[i];
		}
		arr[pos] = indata;
		res = arrlen + 1;
	}
	else
		res = -1;
	return res;
}
//�ַ���ƴ�Ӻ���(��ά����Ϣƴ��)
//************************************
// ����:    StrJoin
// ����ȫ��:  StrJoin
// ��������:    public 
// ����ֵ:   uint8_t
// Qualifier:
// ����: char * mac       ������ַ
// ����: char * id		  ����ID��
// ����: char * lastdata  ��������
//************************************
uint8_t StrJoin(char *mac, char*id,char *lastdata)
{

	//strcpy(lastdata, QR_HEAD);//ͷ�����ƽ�����
	//strcat(lastdata, mac);
	//strcat(lastdata, QR_EQNUM);
	//strcat(lastdata, id);
	//strcat(lastdata, QR_END);
	//return (strlen(QR_HEAD)+strlen(mac)+strlen(QR_EQNUM)+strlen(id)+strlen(QR_END)+1);
	return 0;
}

//��·�����
uint16_t ConsumeHeat(float weight, float tim, float v)
{
	return (uint16_t)((3 * weight * tim * v * 10) / 40);
}