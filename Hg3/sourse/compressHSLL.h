#pragma once
#include<windows.h>
#include<vector>
#include<queue>
#include"zlib.h"
#pragma comment(lib,"zlib.lib")
#define TRANS_BYTE 1
#define TRANS_BIT 0
//BYTE* RLEcompress(BYTE* src, DWORD& size);//relѹ��
//BYTE* RLEuncompress(BYTE* scr, DWORD& size);//rle��ѹ
//BYTE* ZlibCompress(BYTE* scr, DWORD& size);//zlibѹ��
//BYTE* ZlibUncompress(BYTE* scr, DWORD& size);//zlib��ѹ
//BYTE* Data_to_Bit(BYTE* data, DWORD& size);//����ת����
//DWORD* cEliasGammaCode(BYTE* data, DWORD& size);//EliasGammaCode��ѹ
//BYTE* uEliasGammaCode(DWORD* data, DWORD& size);//EliasGammaCodeѹ��
//void  CompleteReversee(BYTE* data, DWORD size);//��������
//std::vector<mpair> GetHuffmanCode(int* precode, DWORD* frequens, DWORD arraysize);//����������
// //BYTE* data_reform(BYTE*data,DWORD size)://������ϴ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////RLEѹ��
BYTE* RLEcompress(BYTE* src, DWORD& size)//������src��RLE��ʽѹ����size������Ϊ�µ�ֵ���ݴ�Сֵ
{
	if (!src)
		return nullptr;
	BYTE* compressed = new BYTE[size * 2];
	DWORD count = 0;
	DWORD ptr = 0; DWORD pre = src[0];
	for (int i = 0; i < size; i++)
	{
		if (src[i] != pre)
		{
			compressed[ptr] = count;
			compressed[ptr + 1] = pre;
			ptr = ptr + 2;
			pre = src[i]; count = 1;
			continue;
		}
		else if ((i == size - 1) && src[i] == pre)
		{
			compressed[ptr] = count + 1;
			compressed[ptr + 1] = pre;
			size = ptr + 2;
			break;
		}
		else if ((i == size - 1) && src[i] != pre)
		{
			compressed[ptr] = count;
			compressed[ptr + 1] = pre;
			ptr = ptr + 2;
			compressed[ptr] = 1;
			compressed[ptr + 1] = src[i];
			size = ptr + 2;
			break;
		}
		count++;
	}
	BYTE* result = new BYTE[size];
	memcpy(result, compressed, size);
	delete[]compressed;
	return result;
}
//RLE��ѹ����ԭ�棬δ�ı䣩
BYTE* RLEuncompress(BYTE* scr, DWORD& size)//������src��RLE��ʽ��ѹ����size������Ϊ�µ�ֵ���ݴ�Сֵ
{
	BYTE* uncompressed = new BYTE[size * 5];//Ԥ����С
	if (size % 2)
		return nullptr;
	DWORD ptr = 0;
	for (int i = 0; i < size / 2; i++)
	{
		for (int k = 0; k < scr[i * 2]; k++)
		{
			uncompressed[ptr] = scr[i * 2 + 1];
			ptr = ptr + 1;
		}
	}
	size = ptr;
	BYTE* result = new BYTE[size];
	memcpy(result, uncompressed, size);
	delete[]uncompressed;
	return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////zlibѹ��
BYTE* ZlibCompress(BYTE* scr, DWORD& size)//�����zlib.h�����뾲̬�⣬������64λ�¿����С�
{
	DWORD pre = size;
	size = 9999999;
	BYTE* result = new BYTE[size];
	int a = compress(result, &size, scr, pre);
	if (a != Z_OK)
	{
		printf_s("��ѹ������%d\n",a);
		delete[]result;
		return nullptr;
	}
	return result;
}
//zlib��ѹ
BYTE* ZlibUncompress(BYTE* scr, DWORD& size)//�����zlib.h�����뾲̬�⣬������64λ�¿����С�
{
	DWORD pre = size;
	size =9999999;
	BYTE* result = new BYTE[size];
	int a = uncompress(result, &size, scr, pre);
	if (a!=Z_OK)
	{
		printf_s("��ѹ������%d\n", a);
		delete[]result;
		return nullptr;
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�ֽ�����ת����λ����
BYTE* Data_to_Bit(BYTE* data, DWORD& size)
{
	BYTE* bData = new BYTE[size * 8];
	for (int i = 0; i < size; i++)
	{
		for (int k = 0; k < 8; k++)
		{
			bData[i * 8 + k] = ((data[i] >> (7 - k))) % 2;
		}
	}
	size = size * 8;
	return bData;
}

BYTE* Bit_to_Data(BYTE* bit, DWORD& size)//��������ת�ֽ�����(û�е�λԪ��1���ص����飬�������ֽ��������)
{
	DWORD presize = size;
	size % 8 == 0 ? size = size / 8 : size=size/8+1;
	BYTE* result = new BYTE[size]();
	for (int i = 0; i < size; i++)
	{
		for (int k = 7; k >= 0; k--)
		{
			if(i * 8 + 7 - k<presize)
			result[i] += (bit[i * 8 +7-k] << k);
		}
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////EliasGammaCode����������
DWORD* uEliasGammaCode(BYTE* data, DWORD& size)//EliasGammaCode���룬size����Ϊ��������Ĵ�С���롣�޷�����������
{
	DWORD count = 0;
	DWORD* table = new DWORD[99999999]{};
	DWORD ptr = 0;
	for (int i = 0; i < size; i++)
	{
		if (data[i] == 1 && i + count < size)
		{
			table[ptr] = 1;
			for (int k = 1; k <= count; k++)
			{
				table[ptr] = table[ptr] * 2 + data[i + k];
			}
			i = i + count;
			ptr = ptr + 1;
			count = 0;
			continue;
		}
		count++;
	}
	size = ptr;
	DWORD* result = new DWORD[size];
	memcpy(result, table, size * 4);
	delete[]table; table = nullptr;
	return result;
}
/////////////////////////////////////////////////////////////////////��EliasGammaCode�����������飬���޸�size��С
BYTE* cEliasGammaCode(DWORD* data, DWORD& size,DWORD TRANS_KIND)//TRANS_KIND�Ĳ�ͬӰ���Ƿ���󽫱���ı���λ����ת��Ϊ�ֽ������ʡ�ռ䡣
{
	BYTE* table = new BYTE[size *32]();
	DWORD ptr = 0;
	for (int k = 0; k < size; k++)
	{
		if (data[k] == 0)
			return nullptr;
		for (int i = 31; i >= 0; i--)
		{
			if ((data[k] >> i) == 1)
			{
				ptr = ptr + i;
				for (int w = i; w >= 0; w--, ptr++)
				{
					table[ptr] = (data[k] >> w) % 2;
				}
			}
		}
	}
	if (TRANS_KIND)
	{
		if (ptr % 8 != 0)
			ptr = ptr + 8 - (ptr % 8);
		size = ptr / 8;
		BYTE* result = new BYTE[ptr / 8]();
		for (int i = 0; i < ptr / 8; i++)
		{
			for (int k = 0; k < 8; k++)
			{
				result[i] = result[i] + (table[i * 8 + k] << (7 - k));
			}
		}
		delete[]table;
		return result;
	}
	BYTE* rebit = new BYTE[ptr](); size = ptr;
	memcpy(rebit, table, ptr);
	delete[]table;
	return rebit;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Reverse
void  CompleteReversee(BYTE* data, DWORD size)//���ֽ�����ı���λ��ȫ��ת����10110100->00101101
{
	for (int i = 0; i < size; i++)
	{
		BYTE sample = 0;
		for (int k = 0; k < 8; k++)
		sample = sample + (((data[i] >> k) % 2) << (7 - k));
		data[i] = sample;
	}
	return;
}
void BitReverse(BYTE* data, DWORD size)//��Ԫ�ذ�ÿ������ת��ÿ�ֽ��е�Ԫ��˳�򲻱�
{
	for (int i = 0; i < size; i++)
		data[i] = ((data[i] % 4) << 6) + (((data[i] >> 2) % 4) << 4) + (((data[i] >> 4) % 4) << 2) + ((data[i] >> 6) % 4);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////����������
struct huffmannode//������code�ṹ��
{
	DWORD frequens = 0;
	int value = 0;
	DWORD CODE = 0;
	huffmannode* nextl = nullptr;
	huffmannode* nextr = nullptr;
	huffmannode() {}
	huffmannode(int frequens2, int value2)
	{
		frequens = frequens2;
		value = value2;
	}
};
//��������ֵ��
struct mpair
{
	int VALUE;
	DWORD CODE;
};
//����������תΪ�������������result����
void recode(std::vector<mpair>& result, huffmannode* n)
{
	std::queue<huffmannode*> queue;
	queue.push(n);
	while (queue.size() > 0)
	{
		if (queue.front()->nextl != nullptr)
		{
			queue.front()->nextl->CODE = queue.front()->CODE * 2;
			queue.front()->nextr->CODE = queue.front()->CODE * 2 + 1;
			queue.push(queue.front()->nextl);
			queue.push(queue.front()->nextr);
			queue.pop();
		}
		else
		{
			DWORD count = 0;
			result.push_back({ queue.front()->value, queue.front()->CODE });
			queue.pop();
		}
	}
}
//���ȶ��бȽϷ���
struct compare {
	bool operator()(const huffmannode* huffmannode1, const huffmannode* huffmannode2)
	{
		return huffmannode1->frequens > huffmannode2->frequens;
	}
};
//������������Ѿ������˹�������ֵ�ĳ��Ƚ���������
std::vector<mpair> GetHuffmanCode(int* precode, DWORD* frequens, DWORD arraysize)
{
	std::priority_queue<huffmannode*, std::vector<huffmannode*>, compare> queue;
	huffmannode** arr = new huffmannode * [arraysize]();
	for (int i = 0; i < arraysize; i++)
	{
		arr[i] = new huffmannode(frequens[i], precode[i]);
		queue.push(arr[i]);
	}
	while (queue.size() != 1)
	{
		huffmannode* father = new huffmannode();
		father->frequens += queue.top()->frequens;
		father->nextr = (huffmannode*)(queue.top());
		queue.pop();
		father->frequens += queue.top()->frequens;
		father->nextl = (huffmannode*)(queue.top());
		queue.pop();
		queue.push(father);
	}
	std::vector<mpair>s1;
	recode(s1, queue.top());
	return s1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////������ϴ
BYTE* data_reform(BYTE* data, DWORD size)//���ݴ�СΪ4�ı����ſ���ϴ
{
	if (size % 4)
	{
		printf("���ݴ�СΪ4�ı����ſ���ϴ");
		return nullptr;
	}
	BYTE* BIT1 = data;
	BYTE* BIT2 = BIT1 + size / 4;
	BYTE* BIT3 = BIT2 + size / 4;
	BYTE* BIT4 = BIT3 + size / 4;
	BYTE* result = new BYTE[size]();
	DWORD ptr = 0;
	for (int i = 0; i < size; i++)
	for (int k = 0; k < 4; k++)
	result[i] = ((BIT1[i] >> (6 - k * 2)) << 6) + ((BIT2[i] >> (6 - k * 2)) << 4) + ((BIT3[i] >> (6 - k * 2)) << 2) + (BIT4[i] >> (6 - k * 2));
	return result;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��ϴ��ԭ
BYTE* reform_back(char* data, DWORD size)//���ݴ�СΪ4�ı����ſɻ�ԭ
{
	if (size % 4)
	{
		printf("���ݴ�СΪ4�ı����ſɻ�ԭ");
		return nullptr;
	}
	BYTE* BIT1 = new BYTE[size * 4]();
	BYTE* BIT2 = BIT1 +size/4;
	BYTE* BIT3 = BIT2 + size / 4;
	BYTE* BIT4 = BIT3 + size / 4;
	DWORD ptr = 0;
	for (int i = 0; i < size / 4; i++, ptr++)
	{
		for (int k = 0; k < 4; k++)
		{
			BIT1[i] += ((data[i * 4 + k] >> 6) % 4) << (6 - k * 2);
			BIT2[i] += ((data[i * 4 + k] >> 4) % 4) << (6 - k * 2);
			BIT3[i] += ((data[i * 4 + k] >> 2) % 4) << (6 - k * 2);
			BIT4[i] += (data[i * 4 + k] % 4) << (6 - k * 2);
		}
	}
	return BIT1;
}