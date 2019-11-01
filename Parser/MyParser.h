#pragma once
#include <map>
#include <string>
#include "AssignStmtAST.h"
#include "BreakStmtAST.h"
#include "ContinueStmtAST.h"
#include "DecExpAST.h"
#include "DecVarNameAST.h"
#include "DirectTypeSpecifyAST.h"
#include "EndAST.h"
#include "ExpAST.h"
#include "ValListAST.h"
#include "FormalParaItemAST.h"
#include "FormalParaListAST.h"
#include "FuncCallStmtAST.h"
#include "FuncDefineAST.h"
#include "IdItemAST.h"
#include "IdListAST.h"
#include "IfStmtAST.h"
#include "NumberAST.h"
#include "PointerAST.h"
#include "PointerTypeSpecifyAST.h"
#include "ProgramAST.h"
#include "RealParaItemAST.h"
#include "RealParaListAST.h"
#include "ReturnStmtAST.h"
#include "SingleVarDecAST.h"
#include "StmtAST.h"
#include "StmtsAST.h"
#include "StructDecAST.h"
#include "TypeSpecifyAST.h"
#include "ValAST.h"
#include "ExpListAST.h"
#include "VarDecAST.h"
#include "VarListAST.h"
#include "VarNameAST.h"
#include "WhileStmtAST.h"
#include "OtherSymAST.h"
#include "IntAST.h"
#include "CharAST.h"
#include "StringAST.h"
#include "RealAST.h"
#include "OperatorAST.h"
#include "TypeAST.h"
#include "ReturnStmtAST.h"
#include "AST.h"
#include "Variable.h"
/*#include "..//AutoTools/DFATools.h"
#include "..//AutoTools/DFATools.cpp"
#include "..//AutoTools/Reader.h"
#include "..//AutoTools/Reader.cpp"

#include "..//Lexer/MyLexer.h"
#include "..//Lexer/MyLexer.cpp"
#include "..//AutoTools/Production.h"
#include "..//AutoTools/Production.cpp"*/

#include "..//Parser/FromAutoTools/Production.h"
#include "..//Parser/FromAutoTools/Reader.h"
#include "..//Parser/FromAutoTools/ProductionSym.h"
//#include "..//Parser/FromLexer/header.h"
#include "..//Parser/FromLexer/MyLexer.h"
//#include "..//Parser/FromLexer/unitTest.h"

enum Behavior{ S, r, GOTO, acc };
struct Action
{
	Behavior behavior;
	int aim = -1;//��behavior��S����goto,���ʾĿ��״̬����behavior��r�����ʾ����ʽ���
};
enum Reduce{

};
struct VarIndex
{
	string name;
	int level;
	string funcName;
	FormalParaListAST* formalParaList;
	friend bool operator < (const struct VarIndex& v1, const struct VarIndex& v2);
};
//from AutoTools start
/*enum Token {
	// �ս��
	Plus,// +
	Minus,// -
	Star,// *
	Divide,// /
	Mode,// %
	PlusPlus,// ++
	MinusMinus,// --
	//Star,// *
	More,// >
	Less,// <
	MoreOrEq,// >=
	LessOrEq,// <=
	Eq,// ==
	Neq,// <>
	Or,//  ||
	And,// &&
	Void,// void
	Int,// int
	Real,// real
	Char,// char
	Lb,// {
	Rb,// }
	Ls,// [
	Rs,// ]
	Lp,// (
	Rp,// )
	Comma,// ,
	Semi,// ;
	Dot,// .
	Assign,// =
	Struct,// struct
	If,// if
	Else,// else
	While,// while
	Return,// return
	Continue,//continue
	Break,// break
	Tag,// ��ʶ��
	Const_int,// ��������1  1.1  a
	Const_real,
	Const_char,
	Const_string,
	Pointer_Sign,//  ->
	End,//$
	Not// !
};*/
struct DicIndex//����mapʹ�� : (state,sym) -> string
{
	int state;
	std::string sym;
	friend bool operator < (const struct DicIndex& d1, const struct DicIndex& d2);
};
inline bool operator <(const struct DicIndex& d1, const struct DicIndex& d2) {
	if (d1.state != d2.state) {
		return d1.state < d2.state;
	}
	else {
		if (d1.sym.length() != d2.sym.length()) {
			return d1.sym.length() < d2.sym.length();
		}
		else {
			for (int i = 0; i < d1.sym.length(); i++) {
				if (d1.sym[i] != d2.sym[i]) {
					return d1.sym[i] < d2.sym[i];
				}
			}
			return d1.sym[0] < d2.sym[0];
		}
	}
}
//from AutoTools end

class MyParser
{
private:
	map<DicIndex, string>* SLR1_table;
	vector<AST*>* symStack;//��������ջ
	vector<int>* stateStack;//״̬����ջ
	int stackTop = -1;
	vector<Symbol*>* sourceSymList;//�ʷ�������õķ��Ŵ�
	
	map<string, int>* pro2index = nullptr;

	string empty_str;
	vector<Production*>* productions;
	map<Token, string>* token2strP;
	//��̬������ start
	map<string, StructDecAST*>* definedStructs;//�Ѿ������˵Ľṹ��
	map<string, vector<FuncDefineAST*>>* definedFuncs;//�Ѿ������˵ĺ���
	map<VarIndex, Variable*>* var_table;//������    ��ͬ�����ı���  ��ͨ�ı���      //�ṹ��ı���
	
	map<string,int>* usedStructName;//�ں�����ʹ�ù��Ľṹ�������ٺ������Ľṹ�������жԱ�   int ��ʹ��ʱ���ڵ��к�
	//��̬������ end
	
	//�﷨�������
	void push(AST* symbol, int state);
	AST* pop();
	Action getAction(Symbol* symbol);
	Action getAction(Symbol* symbol, string value);
	void GoTo(AST*& ast, string value);
	void initToken2StrP();
	void readpro2index(const char* proFileName);
	//��̬���������    �ض��屨�����⼸��������ʵ��
	void addVarFromVarDec(VarDecAST* varDec);
	void addStructFromStructDec(StructDecAST* structDec);
	void addFuncFromFuncDefine(FuncDefineAST* funcDefine);
	void addDecExp(DecExpAST* decExp);

	//DFAtools �����
	static map<DicIndex, string>* readSLR1Table(const char* sourceFileName);
public:

	MyParser(const char* sourceCodeFile, const char* SLR1File, const char* proFileName,string empty_str,bool isFile);
	virtual ~MyParser();

	void Parse();
};

