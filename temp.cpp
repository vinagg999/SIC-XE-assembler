#include<bits/stdc++.h>
#include "Pass_1.cpp"
#include "HEXA_DEC.cpp"

using namespace std;


// Functions to be used 
 
void split(string s, vector<string> results,int lline,string a[])
{
    for(int i=0;i<6;i++)
        a[i]=" ";
    results.clear();

    string x = to_string(lline);
    results.push_back(x);
    stringstream input(s);
    while(!input.eof())                     
    {
        string tmp;
        input >> tmp;
        results.push_back(tmp);
    }

    if(results[1][0]=='.')
    {
        a[0] = to_string(lline);
        a[1] = "$";
        for(int i=1;i<results.size();i++)
        {
            a[2]+=results[i];
            a[2]+=" ";
        }    
    }
    else
    {
        a[0]=results[0];
        if(results[2]=="-")
            a[1]="";
        else
            a[1]=results[2];

        a[2]=results[3];
        if(results[4]=="-")
            a[3]="";
        else
            a[3]=results[4];

        a[4]=toHex(stoint(results[1]));

        if(results.size()==5)
            a[5]="";
        else
            a[5]=toHex(stoint(results[5]));
    }
}        // for splitting the string 

// Global variables
string text_s="",text_e="";
int text_length=0,base=INT_MAX;

// Object, list and intermediate files
ofstream obj,lst;
ifstream intm;

// to find the block we are currently in 
int block_in_use;

bool imm,ind;

string gen_code(string a[])
{
    string ob1,ob2,ob3;
    hexa operand_addr,prgm_ctr;
    int format;
    if(a[2]=="BYTE")
    {
        int i;
        ob1="";
        if(a[3][0]=='X')
            for(i=2;i<a[3].length()-1;++i) ob1+=a[3][i];
        else //a[3][0]=='C'
            for(i=2;i<a[3].length()-1;++i)
                ob1+=toHex((int)a[3][i]);
        return ob1;
    }
    if(a[2]=="WORD")
    {
        ob1=toHex(atoi(a[3].c_str()));
        return ob1;
    }
    if(a[2]=="RSUB")
    {
        ob1="4F0000";
        return ob1;
    }
    if(a[2]=="+RSUB")
    {
        ob1="4F000000";
        return ob1;
    }
    if(a[2][0]=='+')
    {
        format=4;
        a[2]=a[2].substr(1);
    }
    else
        format=OPTAB[a[2]].format;
    if(format==1)
    {
        ob1=OPTAB[a[2]].opcode;
        return ob1;
    }
    if(format==2)
    {
        ob1=OPTAB[a[2]].opcode;
        if(a[3].length()==3)
        {
            ob2=toHex(reg_num(a[3][0]));
            if(isdigit(a[3][2]))  ob2=ob2+toHex(a[3][2]-1);
            else
            {
                ob2=ob2+toHex(reg_num(a[3][2]));
            }
        }
        else  //a[3].length==1
        {
            if(isdigit(a[3][0]))
            {
                ob2=toHex(atoi(a[3].c_str()))+"0";
            }
            else
            {
                ob2=toHex(reg_num(a[3][0]))+"0";
            }
        }
        return (ob1+extendTo(2,ob2));
    }
    if(format==3)
    {
        ob1=OPTAB[a[2]].opcode;
        if(a[3][0]=='#')
        {
            imm=true;
            ob1=toHex(toDec(ob1)+1);
            a[3]=a[3].substr(1);
            if(isdigit(a[3][0]))
            {
                ob2="0";
                ob3=toHex(atoi(a[3].c_str()));
                return extendTo(2,ob1)+ob2+extendTo(3,ob3);
            }
        }
        else if(a[3][0]=='@')
        {
            ind=true;
            ob1=toHex(toDec(ob1)+2);
            a[3]=a[3].substr(1);
        }
        else
            ob1=toHex(toDec(ob1)+3);
        ob2="0";
        bool x=false;
        if(a[3][a[3].length()-2]==',')
        {
            x=true;
            ob2=toHex(toDec(ob2)+8);
            a[3]=a[3].substr(0,a[3].length()-2);
        }
        prgm_ctr=toHex(toDec(a[5]));
        operand_addr=toHex(toDec(SYMTAB[a[3]].address));
        if(x) a[3]+=",X";
        int disp=toDec(operand_addr)-toDec(prgm_ctr);
        if(disp>=-2048 && disp<2048)
        {
            ob2=toHex(toDec(ob2)+2);
            if(disp<0) disp+=4096;
            ob3=toHex(disp);
            return extendTo(2,ob1)+extendTo(1,ob2)+extendTo(3,ob3);
        }
        disp=toDec(operand_addr)-base;
        if(disp>=-2048 && disp<2048)
        {
            ob2=toHex(toDec(ob2)+4);
            if(disp<0) disp+=4096;
            ob3=toHex(disp);
            return extendTo(2,ob1)+extendTo(1,ob2)+extendTo(3,ob3);
        }
        //If still here, means overflow
        error_flag=1;
        error<<"Line "<<a[0]<<": Overflow detected"<<endl;
    }
    if(format==4)
    {
        ob1=OPTAB[a[2]].opcode;
        if(a[3][0]=='#')
        {
            imm=true;
            ob1=toHex(toDec(ob1)+1);
            a[3]=a[3].substr(1);
            if(isdigit(a[3][0]))
            {
                ob2="0";
                ob3=toHex(atoi(a[3].c_str()));
                a[2]="+"+a[2];
                return ob1+ob2+extendTo(5,ob3);
            }
        }
        else if(a[3][0]=='@')
        {
            ind=true;
            ob1=toHex(toDec(ob1)+2);
            a[3]=a[3].substr(1);
        }
        else
            ob1=toHex(toDec(ob1)+3);
        bool x=false;
        ob2="1";
        if(a[3][a[3].length()-2]==',')
        {
            x=true;
            ob2=toHex(toDec(ob2)+8);
            a[3]=a[3].substr(0,a[3].length()-2);
        }
        operand_addr=toHex(toDec(SYMTAB[a[3]].address));
        if(x) a[3]+=",X";
        ob3=operand_addr;
        a[2]="+"+a[2];
        hexa loc_ctr=toHex(toDec(a[4])+1);
        return extendTo(2,ob1)+extendTo(1,ob2)+extendTo(5,ob3);
    }
}


void assemble(string a[])
{
    string object_code;
    hexa loc_ctr;
    int format;
    if(a[2]=="RESB"||a[2]=="RESW")
    {
        lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
        if(text_length>0) obj<<text_s<<"^"<<extendTo(2,toHex(text_length/2))<<text_e<<endl;
        text_s="";
        text_e="";
        text_length=0;
        return;
    }
    
    imm=ind=false;
    object_code=gen_code(a);
    if(a[2]=="BYTE"||a[2]=="WORD")
        lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
    else
    {
        if(imm)     
            lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t#"<<a[3]<<"\t\t\t"<<object_code<<endl;
        else if(ind) 
            lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t@"<<a[3]<<"\t\t"<<object_code<<endl;
        else  
            lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<"\t\t"<<object_code<<endl;
    }
    if(text_s=="")
    {
        loc_ctr=toHex(toDec(a[4]));
        text_s="T^"+extendTo(6,loc_ctr);
        text_e="^"+object_code;
        text_length=object_code.length();
    }
    else if(text_length+object_code.length()>60)
    {
        obj<<text_s<<"^"<<extendTo(2,toHex(text_length/2))<<text_e<<endl;
        loc_ctr=toHex(toDec(a[4]));
        text_s="T^"+extendTo(6,loc_ctr);
        text_e="^"+object_code;
        text_length=object_code.length();
    }
    else
    {
        text_e+="^"+object_code;
        text_length+=object_code.length();
    }
    if(a[2]=="LDB")
    {
        base=toDec(SYMTAB[a[3]].address);
    }
}

void style()
{
    lst<<"Line \t Loc \t \t Source Statement \t \t Object Code "<<endl;
    lst<<"_____________________________________________________________"<<endl;
    lst<<endl<<endl;
}


int main()
{
    // calling the run function of Pass1.cpp to make tables and intermediate file
    exe();

    intm.open("intermediate.txt");
    obj.open("object.txt");
    lst.open("list.txt");

    //string a[6];
    vector<string> results;
    string a[6];

    int lline=5;
    char ch;
    hexa start;

    // To check if error flags were encountered in the previous pass
    if(error_flag)
    {
        cout<<"Errors encountered! See error file! "<<endl;
        exit(1);
    }
    
    style();

    string s;
    getline(intm,s);

    split(s,results,lline,a);
    
    while(a[1][0]=='.')
    {
        lst<<a[0]<<"\t \t"<<s<<endl;
        lline+=5;
        getline(intm,s);
        split(s,results,lline,a);
    }

    if(a[2]=="START")
    {
        lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
        obj<<"H^";
        int i;
        for(i=0;i<a[1].length();++i)
            obj<<a[1][i];
        for(;i<6;++i)
            obj<<" ";
        hexa len;
        len=toHex(prg_len);
        obj<<"^"<<extendTo(6,a[3])<<"^"<<extendTo(6,len)<<endl;
        start=a[3];
    }
    else
    {
        error_flag=1;
        error<<"Intermediate File has no start!"<<endl;
    }

    while(true)
    {
        getline(intm,s);
        lline+=5;
        split(s,results,lline,a);
        cout<<a[0]<<" "<<a[1]<<" "<<a[2]<<" "<<a[3]<<" "<<a[4]<<" "<<a[5]<<endl;

        if(s[0]=='.')
        {
            lst<<a[0]<<"\t\t "<<s<<endl;
            continue;
        }
        if(a[2]=="END")
        {
            lst<<a[0]<<"\t\t\t\t\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
            if(text_length>0)
            {
                obj<<text_s<<"^"<<extendTo(2,toHex(text_length/2))<<text_e<<endl;
            }
            text_length=0;
            text_s="";
            text_e="";
            obj<<"E^"<<extendTo(6,start)<<endl;
            break;
        }
       // cout<<a[2]<<endl;
        assemble(a);
       // cin>>ch;
    }
    if(error_flag)
    {
        cout<<"Errors encountered! See error file! "<<endl;
    }
    else
    {
        cout<<"FILE ASSEMBLED SUCCESSFULY!!"<<endl;
    }
}








