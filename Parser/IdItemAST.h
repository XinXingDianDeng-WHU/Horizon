#pragma once
#include "AST.h"
#include "DecVarNameAST.h"
#include "ExpListAST.h"

class IdItemAST:public AST
{
public:
	DecVarNameAST* decVarNameAST;
	//������������������һΪ��
	ExpAST* exp = nullptr;//����ֵ
	ExpListAST* exps = nullptr;//���ֵ

	IdItemAST(DecVarNameAST* decVarNameAST);
	IdItemAST(DecVarNameAST* decVarNameAST, ExpAST* exp);
	IdItemAST(DecVarNameAST* decVarNameAST, ExpListAST* exps);
	virtual ~IdItemAST();

};

