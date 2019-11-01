#pragma once
#include "AST.h"
#include "ExpAST.h"
#include <vector>
class ExpListAST :
	public AST
{
private:
	void setTypes(ExpAST* exp);
public:
	vector<ExpAST*>* exps = nullptr;

	zx::Type expType = zx::Type::VoiD;//��exp�ı������ͣ�Ĭ��ΪVOID
	int pointerNum = 0;//ָ����
	zx::Type finalToType = zx::Type::VoiD;//����ָ�������
	string structName = "";//����ǽṹ�����FinalToTypeΪ�ṹ�壬��structName�ǽṹ������

	ExpListAST(ExpAST* exp);
	void addExp(ExpAST* exp);
	bool check(ExpAST* exp);//���exp�Ƿ��list�������
	virtual ~ExpListAST();
};

