#pragma once
#include "Enums_Structs.h"
#include <string>

class Variable
{
private:
	zx::Type type;
	std::string name;
	std::string value = "";
	
	//�ṹ�嶨��ָ��

	//
	bool isArray;
	int arrayLength = 0;
	std::string structName = "";//���type��STRUCT������struct������
	zx::Type toType = zx::Type::VoiD;//���type��ָ�룬���ָ���Ŀ�������
	zx::Type finalToType = zx::Type::VoiD;//
	std::string finalStructName = "";//���finalToType��STRUCT������struct������
	int pointerNum = 0;
public:
	Variable(zx::Type type, std::string name,bool isArray);
	Variable(zx::Type type, std::string name, std::string structName, bool isArray);
	Variable(zx::Type type, std::string name, zx::Type toType, bool isArray);

	void setValue(std::string val);
	void setArratLength(int length);
	zx::Type getType();
	std::string getValue();
	std::string getName();
	std::string getStructName();
	zx::Type getFinalToType();
	int getPointerNum();
	zx::Type getToType();
	std::string getFinalStructName();
	int getArrayLength();
	void setPointerNum(int num);
	void setFinalToType(zx::Type finalToType);
	void setFinalStructName(std::string name);
	void setArray();
	bool IsArray();
};

