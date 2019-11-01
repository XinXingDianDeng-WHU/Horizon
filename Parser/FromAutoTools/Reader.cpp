

#include "Reader.h"
#include <fstream>
#include <iostream>

char* Reader::buffer = nullptr;
int Reader::length = 0;


void Reader::readFile(const char* fileName)
{
	fstream in;
	in.open(fileName, ios::in);
	in.seekg(0, ios::end);//�ƶ���ĩβ
	Reader::length = in.tellg();//��ó���
	in.seekg(0, ios::beg);//�ƶ�����ͷ
	Reader::buffer = new char[length];
	in.read(Reader::buffer, length);
	in.close();
}


vector<Production*>* Reader::readProductions(const char* fileName)//��ȡProduction �� vector
{
	// $����   @��ʾ�Ƶ���
	int row = 1;
	Reader::readFile(fileName);
	vector<Production*>* vectorPointer = new vector<Production*>();
	string s = "";
	/*if (buffer[length - 1] != '#')
	{
		cout << buffer[length - 1];
		Error("�ķ���ʽ����Ӧ����#��β",row);
	}*/
	Production* pro = nullptr;
	vector<ProductionSym*>* right = new vector<ProductionSym*>();
	int i = 0;
	//for (int i = 0; i < Reader::length; i++)
	while(buffer[i] != '$')
	{
		pro = new Production();
		ProductionSym* left = nullptr;
		while (buffer[i] != ' ')
		{
			/*if ((buffer[i] < '0' || buffer[i] > '9') && (buffer[i] < 'a' || buffer[i] > 'z') && (buffer[i] < 'A' || buffer[i] > 'Z') && buffer[i] != '_' && buffer[i] != '<' && buffer[i] != '>')
			{
				Error("�Ƿ��ַ�", row);
			}*/
			s += buffer[i];
			i++;
		}
		if (s[0] != '<' || s[s.length() - 1] != '>')
		{
			Error("���ս��������<>������",row);
		}
		//left = new ProductionSym(s.substr(1, s.length() - 2), true);
		left = new ProductionSym(s, true);
		pro->setLeft(left);
		left = nullptr;
		s = "";
		i++;
		if (buffer[i] != '@')
		{
			Error("������@��ʾ�Ƶ�����",row);
		}
		i++;
		if (buffer[i] != ' ')
		{
			Error("������֮��ȱ�ٿո�",row);
		}
		i++;
		if (buffer[i] == '$') // ����
		{
			Error("�Ƿ�����", row);
			/*delete right;
			right = nullptr;
			return vectorPointer;*/
		}
		while (buffer[i] != '\n')
		{
			while (buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '$' && buffer[i] != '@')
			{
				/*if ((buffer[i] < '0' || buffer[i] > '9') && (buffer[i] < 'a' || buffer[i] > 'z') && (buffer[i] < 'A' || buffer[i] > 'Z') && buffer[i] != '_' && buffer[i] != '<' && buffer[i] != '>')
				{
					Error("�Ƿ��ַ�", row);
				}*/
				s += buffer[i];
				i++;
			}
			if (buffer[i] == ' ')
			{
				i++;
			}
			else if(buffer[i]!='\n')
			{
				Error("$��@�ڴ˴��Ƿ���", row);
			}
			if (s[0] == '<' && s[s.length() - 1] == '>')
			{
				//ProductionSym* tsym = new ProductionSym(s.substr(1, s.length() - 2), true);
				if (s.length() == 2)
				{
					ProductionSym* tsym = new ProductionSym(s, false);
					right->push_back(tsym);
					s = "";
				}
				else
				{
					ProductionSym* tsym = new ProductionSym(s, true);
					right->push_back(tsym);
					s = "";
				}
			}
			else
			{
				ProductionSym* tsym = new ProductionSym(s, false);
				right->push_back(tsym);
				s = "";
			}
		}
		i++;
		pro->setRight(right);
		vectorPointer->push_back(pro);
		
		right = new vector<ProductionSym*>();
		row++;
	}

	// �����ڴ�
	delete Reader::buffer;
	Reader::buffer = nullptr;
	Reader::length = 0;
	return vectorPointer;
}

void Reader::Error(const char* detail,int row)
{
	cout << "�ļ�" << "��" << row << "�г���";
	cout << detail<< endl;
	exit(0);
}
