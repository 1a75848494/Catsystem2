#pragma once
#include<Windows.h>
#include<string>

WCHAR* Utf8_To_Wide(const char* str)//utf8����ת���ֽ�
{
	WCHAR* wide;
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);//��ȡstr����Ҫ���ֽڿռ�Ĵ�С
	wide = new WCHAR[i + 1];//����ռ�
	MultiByteToWideChar(CP_UTF8, 0, str, -1, wide, i);//��str�е��ַ�װ����ֽ�����wide
	return wide;//����
}

WCHAR* Char_To_Wide(const char* str)//charת���ֽ�
{
	WCHAR* wide;
	int i = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);//��ȡstr����Ҫ���ֽڿռ�Ĵ�С
	wide = new WCHAR[i + 1];//����ռ�
	MultiByteToWideChar(CP_ACP, 0, str, -1, wide, i);//��str�е��ַ�װ����ֽ�����wide
	return wide;//����
}

LPSTR Wide_To_Char(WCHAR* str)//���ֽ�תchar
{
	char* wide;
	int i = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);//��ȡstr����Ҫ�ֽڿռ�Ĵ�С
	wide = new char[i + 1];//����ռ�
	WideCharToMultiByte(CP_ACP, 0, str, -1, wide, i, NULL, NULL);//��str�е��ַ�װ���ֽ�����wide
	return wide;//����
}

LPSTR Utf8_To_Char(const char* str)//utf8תchar
{
	WCHAR* wide;
	LPSTR ascii;
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wide = new WCHAR[i + 1];//����ռ�
	MultiByteToWideChar(CP_UTF8, 0, str, -1, wide, i);
	i = WideCharToMultiByte(CP_ACP, 0, wide, -1, NULL, 0, NULL, NULL);
	ascii= new char[i + 1];//����ռ�
	WideCharToMultiByte(CP_ACP, 0, wide, -1, ascii, i, NULL, NULL);
	delete[] wide;
	return ascii;//����
}

std::string Utf8_To_Str(const char* str)//utf8תstring
{
	WCHAR* wide;
	LPSTR ascii;
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wide = new WCHAR[i + 1];//����ռ�
	MultiByteToWideChar(CP_UTF8, 0, str, -1, wide, i);
	i = WideCharToMultiByte(CP_ACP, 0, wide, -1, NULL, 0, NULL, NULL);
	ascii = new char[i + 1];//����ռ�
	WideCharToMultiByte(CP_ACP, 0, wide, -1, ascii, i, NULL, NULL);
	std::string s =ascii;
	delete[] ascii;
	delete[] wide;
	return s;//����
}

BYTE* X16_To_Code(char* head, DWORD& size)//���ַ�����ʽ��16���ƴ���ת��Ϊ��������,���������С(�ڶ�������)
{
	byte* data = new byte[50];
	int i = 0; int k = 0;
	if (strlen(head) % 2)
		return nullptr;
	while (head[i])
	{
		byte x = 0;
		if (head[i] >= 'a' && head[i] <= 'f')
		{
			x += (head[i] - 87) * 16;
		}
		else if (head[i] >= 'A' && head[i] <= 'F')
		{
			x += (head[i] - 55) * 16;
		}
		else if (head[i] >= '0' && head[i] <= '9')
		{
			x += (head[i] - 48) * 16;
		}
		else
		{
			return nullptr;
		};
		if (head[i + 1] >= 'a' && head[i + 1] <= 'f')
		{
			x += (head[i + 1] - 87);
		}
		else if (head[i + 1] >= 'A' && head[i + 1] <= 'F')
		{
			x += (head[i + 1] - 55);
		}
		else if (head[i + 1] >= '0' && head[i + 1] <= '9')
		{
			x += (head[i + 1] - 48);
		}
		else
		{
			return nullptr;
		}
		data[k] = x;
		i += 2; k++;
	}
	data[k] = '\0'; size = k;
	return data;
}

long long  X16_To_Int(char* head)//��16�����ַ���ת��Ϊת��Ϊ16��������
{
	int i = 0; long long k = 0;
	while (head[i])
	{
		k = k * 16;
		if (head[i] >= 'a' && head[i] <= 'f')
		{
			k += (head[i] - 87);
		}
		else if (head[i] >= 'A' && head[i] <= 'F')
		{
			k += (head[i] - 55);
		}
		else if (head[i] >= '0' && head[i] <= '9')
		{
			k += (head[i] - 48);
		}
		else
		{
			return -1;
		}
		i++;
	}
	return k;
}

