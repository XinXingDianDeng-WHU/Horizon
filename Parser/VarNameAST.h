#pragma once
#include "AST.h"
#include <string>
#include "VarNameAST.h"
#include "IdentifierAST.h"
#include "IntAST.h"
#include "ExpAST.h"
#include "Enums_Structs.h"

class VarNameAST:public AST
{
public:
	int type;//0:identifier  1:<var_name> [ <exp> ]   2:<var_name> -> <var_name>  3:<var_name> . <var_name>  4:<var_name> @ * <exp>
	IdentifierAST* identifier = nullptr;
	ExpAST* intExp = nullptr;
	VarNameAST* left = nullptr;// type = 1 ʱ��left
	VarNameAST* right = nullptr;
	ExpAST* exp = nullptr;

	//��̬������ start
	zx::Type varType = zx::Type::ZXVOID;
	zx::Type finalToType = zx::Type::ZXVOID;//�����ָ�룬���ʾ����ָ���type
	int pointerNum = 0;//�����ָ�룬��ʾָ�����
	string structName = "";//���������struct����finalToType��struct�����ʾstruct������
	bool isArray = false;
	void setType(zx::Type varType);
	void setFinalToType(zx::Type finalToType);
	void setPointerNum(int pointerNum);
	void setStructName(string name);

	//��̬������ end

	VarNameAST(IdentifierAST* identifier);// type = 0
	VarNameAST(VarNameAST* left, ExpAST* intExp);//type = 1
	VarNameAST(VarNameAST* left, VarNameAST* right, int type);// type = 2   3
	VarNameAST(ExpAST* exp);// type = 4
	virtual ~VarNameAST();
};

