#pragma once
#include "AST.h"
#include "Enums_Structs.h"
class TypeSpecifyAST:public AST
{
public:
	int son = -1;//0 : <direct_type_specify>      1: <pointer_type_specify>
	/*
	Type type;
	//bool isArray;      ���Ǿ���VarAST����
	//int arrayLength = 0;  ���Ǿ���VarAST����
	std::string structName = "";//���type��STRUCT������struct������
	Type toType = ZXVOID;//���type��ָ�룬���ָ���Ŀ�������
	

	VarDecAST(Type type, IdListAST* idList);
	VarDecAST(Type type, Type toType, IdListAST* idList);
	*/
};

