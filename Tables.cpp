#include<bits/stdc++.h>
using namespace std;

struct op
{
	string opcode;
	int format;
	bool isthere;
	op()
	{
		isthere=false;
	}	
};

struct reg
{
	int num;
	bool isthere;
	reg()
	{
		isthere=false;
	}
};

struct info_label
{
	string address;
	string value;
	//string block;
	bool isthere;
	info_label()
	{
		isthere=false;
	}
};

typedef string mnemonic,label;

typedef std::map<mnemonic,op>   MapType1;
typedef std::map<mnemonic,reg>  MapType2;
typedef std::map<label,info_label>   MapType3;

MapType1 OPTAB;
MapType2 REGISTER;
MapType3 SYMTAB;


void create()
{

	/* Register Information */

	REGISTER["A"].num=0;
	REGISTER["A"].isthere=true;

	REGISTER["X"].num=1;
	REGISTER["X"].isthere=true;

	REGISTER["L"].num=2;
	REGISTER["L"].isthere=true;

	REGISTER["B"].num=3;
	REGISTER["B"].isthere=true;

	REGISTER["S"].num=4;
	REGISTER["S"].isthere=true;

	REGISTER["T"].num=5;
	REGISTER["T"].isthere=true;

	REGISTER["F"].num=6;
	REGISTER["F"].isthere=true;

	REGISTER["PC"].num=8;
	REGISTER["PC"].isthere=true;

	REGISTER["SW"].num=9;
	REGISTER["SW"].isthere=true;

	/* Optable Information */

	OPTAB["ADD"].opcode="18";
	OPTAB["ADD"].format=3;
	OPTAB["ADD"].isthere=true;

	OPTAB["ADDF"].opcode="58";
	OPTAB["ADDF"].format=3;
	OPTAB["ADDF"].isthere=true;

	OPTAB["ADDR"].opcode="90";
	OPTAB["ADDR"].format=2;
	OPTAB["ADDR"].isthere=true;

	OPTAB["AND"].opcode="40";
	OPTAB["AND"].format=3;
	OPTAB["AND"].isthere=true;

	OPTAB["CLEAR"].opcode="B4";
	OPTAB["CLEAR"].format=2;
	OPTAB["CLEAR"].isthere=true;

	OPTAB["COMP"].opcode="28";
	OPTAB["COMP"].format=3;
	OPTAB["COMP"].isthere=true;

	OPTAB["COMPF"].opcode="88";
	OPTAB["COMPF"].format=3;
	OPTAB["COMPF"].isthere=true;

	OPTAB["COMPR"].opcode="A0";
	OPTAB["COMPR"].format=2;
	OPTAB["COMPR"].isthere=true;

	OPTAB["DIV"].opcode="24";
	OPTAB["DIV"].format=3;
	OPTAB["DIV"].isthere=true;

	OPTAB["DIVF"].opcode="64";
	OPTAB["DIVF"].format=3;
	OPTAB["DIVF"].isthere=true;

	OPTAB["DIVR"].opcode="9C";
	OPTAB["DIVR"].format=2;
	OPTAB["DIVR"].isthere=true;

	OPTAB["FIX"].opcode="C4";
	OPTAB["FIX"].format=1;
	OPTAB["FIX"].isthere=true;

	OPTAB["FLOAT"].opcode="C0";
	OPTAB["FLOAT"].format=1;
	OPTAB["FLOAT"].isthere=true;

	OPTAB["HIO"].opcode="F4";
	OPTAB["HIO"].format=1;
	OPTAB["HIO"].isthere=true;

	OPTAB["J"].opcode="3C";
	OPTAB["J"].format=3;
	OPTAB["J"].isthere=true;

	OPTAB["JEQ"].opcode="30";
	OPTAB["JEQ"].format=3;
	OPTAB["JEQ"].isthere=true;

	OPTAB["JGT"].opcode="34";
	OPTAB["JGT"].format=3;
	OPTAB["JGT"].isthere=true;

	OPTAB["JLT"].opcode="38";
	OPTAB["JLT"].format=3;
	OPTAB["JLT"].isthere=true;

	OPTAB["JSUB"].opcode="48";
	OPTAB["JSUB"].format=3;
	OPTAB["JSUB"].isthere=true;

	OPTAB["LDA"].opcode="00";
	OPTAB["LDA"].format=3;
	OPTAB["LDA"].isthere=true;

	OPTAB["LDB"].opcode="68";
	OPTAB["LDB"].format=3;
	OPTAB["LDB"].isthere=true;

	OPTAB["LDCH"].opcode="50";
	OPTAB["LDCH"].format=3;
	OPTAB["LDCH"].isthere=true;

	OPTAB["LDF"].opcode="70";
	OPTAB["LDF"].format=3;
	OPTAB["LDF"].isthere=true;

	OPTAB["LDL"].opcode="08";
	OPTAB["LDL"].format=3;
	OPTAB["LDL"].isthere=true;

	OPTAB["LDS"].opcode="6C";
	OPTAB["LDS"].format=3;
	OPTAB["LDS"].isthere=true;

	OPTAB["LDT"].opcode="74";
	OPTAB["LDT"].format=3;
	OPTAB["LDT"].isthere=true;

	OPTAB["LDX"].opcode="04";
	OPTAB["LDX"].format=3;
	OPTAB["LDX"].isthere=true;

	OPTAB["LPS"].opcode="D0";
	OPTAB["LPS"].format=3;
	OPTAB["LPS"].isthere=true;

	OPTAB["MUL"].opcode="20";
	OPTAB["MUL"].format=3;
	OPTAB["MUL"].isthere=true;

	OPTAB["MULF"].opcode="60";
	OPTAB["MULF"].format=3;
	OPTAB["MULF"].isthere=true;

	OPTAB["MULR"].opcode="98";
	OPTAB["MULR"].format=2;
	OPTAB["MULR"].isthere=true;

	OPTAB["NORM"].opcode="C8";
	OPTAB["NORM"].format=1;
	OPTAB["NORM"].isthere=true;

	OPTAB["OR"].opcode="44";
	OPTAB["OR"].format=3;
	OPTAB["OR"].isthere=true;

	OPTAB["RD"].opcode="D8";
	OPTAB["RD"].format=3;
	OPTAB["RD"].isthere=true;

	OPTAB["RMO"].opcode="AC";
	OPTAB["RMO"].format=2;
	OPTAB["RMO"].isthere=true;

	OPTAB["RSUB"].opcode="4C";
	OPTAB["RSUB"].format=3;
	OPTAB["RSUB"].isthere=true;

	OPTAB["SHIFTL"].opcode="A4";
	OPTAB["SHIFTL"].format=2;
	OPTAB["SHIFTL"].isthere=true;

	OPTAB["SHIFTR"].opcode="A8";
	OPTAB["SHIFTR"].format=2;
	OPTAB["SHIFTR"].isthere=true;

	OPTAB["SIO"].opcode="F0";
	OPTAB["SIO"].format=1;
	OPTAB["SIO"].isthere=true;

	OPTAB["SSK"].opcode="EC";
	OPTAB["SSK"].format=3;
	OPTAB["SSK"].isthere=true;

	OPTAB["STA"].opcode="0C";
	OPTAB["STA"].format=3;
	OPTAB["STA"].isthere=true;

	OPTAB["STB"].opcode="78";
	OPTAB["STB"].format=3;
	OPTAB["STB"].isthere=true;

	OPTAB["STCH"].opcode="54";
	OPTAB["STCH"].format=3;
	OPTAB["STCH"].isthere=true;

	OPTAB["STF"].opcode="80";
	OPTAB["STF"].format=3;
	OPTAB["STF"].isthere=true;

	OPTAB["STI"].opcode="D4";
	OPTAB["STI"].format=3;
	OPTAB["STI"].isthere=true;

	OPTAB["STL"].opcode="14";
	OPTAB["STL"].format=3;
	OPTAB["STL"].isthere=true;

	OPTAB["STS"].opcode="7C";
	OPTAB["STS"].format=3;
	OPTAB["STS"].isthere=true;

	OPTAB["STSW"].opcode="E8";
	OPTAB["STSW"].format=3;
	OPTAB["STSW"].isthere=true;

	OPTAB["STT"].opcode="84";
	OPTAB["STT"].format=3;
	OPTAB["STT"].isthere=true;

	OPTAB["STX"].opcode="10";
	OPTAB["STX"].format=3;
	OPTAB["STX"].isthere=true;

	OPTAB["SUB"].opcode="1C";
	OPTAB["SUB"].format=3;
	OPTAB["SUB"].isthere=true;

	OPTAB["SUBF"].opcode="5C";
	OPTAB["SUBF"].format=3;
	OPTAB["SUBF"].isthere=true;

	OPTAB["SUBR"].opcode="94";
	OPTAB["SUBR"].format=2;
	OPTAB["SUBR"].isthere=true;

	OPTAB["SVC"].opcode="B0";
	OPTAB["SVC"].format=2;
	OPTAB["SVC"].isthere=true;

	OPTAB["TD"].opcode="E0";
	OPTAB["TD"].format=3;
	OPTAB["TD"].isthere=true;

	OPTAB["TIO"].opcode="F8";
	OPTAB["TIO"].format=1;
	OPTAB["TIO"].isthere=true;

	OPTAB["TIX"].opcode="2C";
	OPTAB["TIX"].format=3;
	OPTAB["TIX"].isthere=true;

	OPTAB["TIXR"].opcode="B8";
	OPTAB["TIXR"].format=2;
	OPTAB["TIXR"].isthere=true;

	OPTAB["WD"].opcode="DC";
	OPTAB["WD"].format=3;
	OPTAB["WD"].isthere=true;

}

int reg_num(char ch)
{
	switch(ch)
	{
		case 'A': return 0;
		case 'X': return 1;
		case 'L': return 2;
		case 'B': return 3;
		case 'S': return 4;
		case 'T': return 5;
		case 'F': return 6;
		return -1;
	};
}

typedef string hexa;

hexa extendTo(int dig,hexa a)
{
    hexa temp="";
    for(int i=0;i<dig-a.length();++i)
        temp+='0';
    return temp+a;
}


string toHex(int x)
{
    stringstream ss;
    ss<< hex << x;                          // int decimal_value
    string res (ss.str());                  // string hex value
    return res;
}

int toDec(string x)
{
    int decimal_value;
    stringstream ss;
    ss  << x ;                              // string hex_value
    ss >> hex >> decimal_value ;            //int decimal_value
    return decimal_value;
}
