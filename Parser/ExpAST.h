#pragma once
#include "AST.h"
//#include "VarNameAST.h"
#include "ValAST.h"
#include "OperatorAST.h"
#include "FuncCallStmtAST.h"
#include "Enums_Structs.h"
class VarNameAST;
class ExpAST :
	public AST
{
public:
	int type;//0 ��Ŀ�����   1˫Ŀ�����  2  <val>   3 <var_name>  4 <func_call_stmt>     (<exp>)����
	ExpAST* left = nullptr;//��Ŀ�����ֻ��left
	ExpAST* right = nullptr;

	zx::Type expType = zx::Type::VoiD;//��exp�ı������ͣ�Ĭ��ΪVOID
	int pointerNum = 0;//ָ����
	zx::Type finalToType = zx::Type::VoiD;//����ָ�������
	string structName = "";//����ǽṹ�����FinalToTypeΪ�ṹ�壬��structName��ʾ�ṹ�������
	////////////�ṹ��Ŀ��ǣ�

	ValAST* valAST = nullptr;
	VarNameAST* varNameAST = nullptr;
	OperatorAST* operatorAST = nullptr;
	FuncCallStmtAST* funcCallStmtAST = nullptr;

	ExpAST(ExpAST* left, OperatorAST* operatorAST);//��Ŀ
	ExpAST(ExpAST* left, ExpAST* right, OperatorAST* operatorAST);//˫Ŀ
	ExpAST(ValAST* valAST);//<val>
	ExpAST(VarNameAST* varNameAST);//<var_name>
	ExpAST(FuncCallStmtAST* funcCallStmtAST);//<func_call_stmt>
	void setType(zx::Type expType);
	void setFinalToType(zx::Type type);
	void setPointerNum(int num);
	void setStructName(string name);
	virtual ~ExpAST();
};

