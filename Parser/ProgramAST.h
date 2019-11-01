#pragma once
#include "AST.h"
#include "DecExpAST.h"
#include "FuncDefineAST.h"

class ProgramAST :
	public AST
{
public:
	vector<DecExpAST*>* dec_exp_ASTs;//����AST   �������������ͽṹ������
	vector<FuncDefineAST*>* func_define_ASTs;//��������AST


	ProgramAST();
	virtual ~ProgramAST();
	void addDecExpAST(DecExpAST* ast);
	void addFuncDefineAST(FuncDefineAST* ast);

	
};

