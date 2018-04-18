#include<bits/stdc++.h>
#include "assembler_pass_1.cpp"

using namespace std;

ofstream obj,lst;                     // To write object file and list file
ifstream intm;                        // To read intermediate file          

vector<string> mod;                    // To store the lines which needs modifications

void split(string s, vector<string> results,int lline,string a[])      // To get the different words from intermediate file
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
            a[1]="";                                    // a[0] = line_number
        else                                            // a[1] = label
            a[1]=results[2];                            // a[2] = opcode
                                                        // a[3] = operand
        a[2]=results[3];                                // a[4] = loc
        if(results[4]=="-")                             // a[5] = contents_of_program_counter_for_this_line
            a[3]="";
        else
            a[3]=results[4];

        a[4]=toHex(stoint(results[1]));

        if(results.size()==5)
            a[5]="";
        else
            a[5]=toHex(stoint(results[5]));
    }
}        

/* To check immediateediate or indirectirect mode of addressing */

bool immediate,indirect;



/* Variables to be used while writing text files */

int text_length=0,base=INT_MAX;
string text_s="",text_e="";  


/* Function to get the object code of the current instruction */

string get_object_code(string a[])
{
    int format;
    string code_1,code_2,code_3,pc_content,operand_addr;


    if(a[2]=="BYTE")                                                    // If the opcode is BYTE then object code is simply the written string in instruction 
    {
        int i;
        code_1="";  
        if(a[3][0]=='X')                                                // If the string is in hexadecimal X' '
            for(i=2;i<a[3].length()-1;++i) code_1+=a[3][i];
        else                                                            // If the string stores characters C' '
            for(i=2;i<a[3].length()-1;++i)
                code_1+=toHex((int)a[3][i]);
        return code_1;
    }
    if(a[2]=="WORD")                                                    // If the opcode is WORD then the operand is object code
    {
        code_1=toHex(atoi(a[3].c_str()));
        return code_1;
    }
    if(a[2]=="RSUB")                                                    // Since RSUB has no operands it is separately handled
    {
        code_1="4F0000";
        return code_1;
    }
    if(a[2]=="+RSUB")
    {
        code_1="4F000000";
        return code_1;
    }
    if(a[2][0]=='+')                                                    // Checking for format 4 instruction
    {
        format=4;
        a[2]=a[2].substr(1);
    }
    else
        format=OPTAB[a[2]].format;
    if(format==1)                                                       // Format 1 ==> Object Code = opcode
    {
        code_1=OPTAB[a[2]].opcode;
        return code_1;
    }
    if(format==2)                                                       // Format 2 ==> Object code = opcode + reg_number + (reg_number||integer)
    {
        code_1=OPTAB[a[2]].opcode;
        if(a[3].length()==3)
        {
            code_2=toHex(reg_num(a[3][0]));
            if(isdigit(a[3][2]))  code_2=code_2+toHex(a[3][2]-1);
            else
            {
                code_2=code_2+toHex(reg_num(a[3][2]));
            }
        }
        else  //a[3].length==1
        {
            if(isdigit(a[3][0]))
            {
                code_2=toHex(atoi(a[3].c_str()))+"0";
            }
            else
            {
                code_2=toHex(reg_num(a[3][0]))+"0";
            }
        }
        return (code_1+extendTo(2,code_2));
    }
    if(format==3)                                                       // There are different addressing modes here which needs to be taken care of
    {
        code_1=OPTAB[a[2]].opcode;
        if(a[3][0]=='#')                                                // Immediate Mode
        {
            immediate=true;
            code_1=toHex(toDec(code_1)+1);
            a[3]=a[3].substr(1);
            if(isdigit(a[3][0]))
            {
                code_2="0";
                code_3=toHex(atoi(a[3].c_str()));
                return extendTo(2,code_1)+code_2+extendTo(3,code_3);
            }
        }
        else if(a[3][0]=='@')                                           // Indirect Mode
        {
            indirect=true;
            code_1=toHex(toDec(code_1)+2);
            a[3]=a[3].substr(1);
        }
        else                                                            // Direct Mode
            code_1=toHex(toDec(code_1)+3);
        code_2="0";
        bool x=false;
        if(a[3][a[3].length()-2]==',')
        {
            x=true;
            code_2=toHex(toDec(code_2)+8);
            a[3]=a[3].substr(0,a[3].length()-2);
        }
        pc_content=toHex(toDec(a[5]));
        operand_addr=toHex(toDec(SYMTAB[a[3]].address));
        if(x) a[3]+=",X";
        int disp=toDec(operand_addr)-toDec(pc_content);
        if(disp>=-2048 && disp<2048)                                    // If PC relative addressing can be done or not
        {
            code_2=toHex(toDec(code_2)+2);
            if(disp<0) disp+=4096;
            code_3=toHex(disp);
            return extendTo(2,code_1)+extendTo(1,code_2)+extendTo(3,code_3);
        }
        disp=toDec(operand_addr)-base;
        if(disp>=-2048 && disp<2048)                                   
        {
            code_2=toHex(toDec(code_2)+4);
            if(disp<0) disp+=4096;
            code_3=toHex(disp);
            return extendTo(2,code_1)+extendTo(1,code_2)+extendTo(3,code_3);
        }
        //If still here, means overflow
        error_flag=1;
        error<<"Line "<<a[0]<<": Overflow detected"<<endl;
    }
    if(format==4)                                                       // Here we also need to store modification records
    {
        code_1=OPTAB[a[2]].opcode;
        if(a[3][0]=='#')
        {
            immediate=true;
            code_1=toHex(toDec(code_1)+1);
            a[3]=a[3].substr(1);
            if(isdigit(a[3][0]))
            {
                code_2="0";
                code_3=toHex(atoi(a[3].c_str()));
                a[2]="+"+a[2];
                return code_1+code_2+extendTo(5,code_3);
            }
        }
        else if(a[3][0]=='@')
        {
            indirect=true;
            code_1=toHex(toDec(code_1)+2);
            a[3]=a[3].substr(1);
        }
        else
            code_1=toHex(toDec(code_1)+3);
        bool x=false;
        code_2="1";
        if(a[3][a[3].length()-2]==',')
        {
            x=true;
            code_2=toHex(toDec(code_2)+8);
            a[3]=a[3].substr(0,a[3].length()-2);
        }
        operand_addr=toHex(toDec(SYMTAB[a[3]].address));
        if(x) a[3]+=",X";
        code_3=operand_addr;
        a[2]="+"+a[2];
        hexa loc_ctr=toHex(toDec(a[4])+1);
        string alpha="M^"+extendTo(6,loc_ctr)+"^05";            // Writing modification record
        mod.push_back(alpha);                                   // Storing it in a vector for object file
        return extendTo(2,code_1)+extendTo(1,code_2)+extendTo(5,code_3);
    }
}


void process_instruction(string a[])                            // assemble the instruction
{
    string object_code,loc_ctr;                                 // to store object code generated and location counter values
    int format;

    if(a[2]=="RESB"||a[2]=="RESW")                              // Directly write to list file as no object_code required
    {
        lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
        if(text_length>0) obj<<text_s<<"^"<<extendTo(2,toHex(text_length/2))<<text_e<<endl;
        text_s="";
        text_e="";
        text_length=0;
        return;
    }
    
    immediate=indirect=false;

    object_code=get_object_code(a);

    if(a[2]=="BYTE"||a[2]=="WORD")
        lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<"\t\t"<<object_code<<endl;
    else
    {
        if(immediate)     
            lst<<a[0]<<"\t\t"<<extendTo(4,a[4])<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t#"<<a[3]<<"\t\t"<<object_code<<endl;
        else if(indirect) 
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

void style_begin()
{
    lst<<"Line \t \t Loc \t \t \tSource Statement \t \t \t Object Code "<<endl;
    lst<<"_____________________________________________________________________________________________"<<endl;
    lst<<endl<<endl;
}

void style_end()
{
    lst<<"_____________________________________________________________________________________________"<<endl;
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
    string start;

    // To check if error flags were encountered in the previous pass
    if(error_flag)
    {
        cout<<"Errors encountered! See error file! "<<endl;
        //exit(1);
    }
    
    style_begin();

    string s;
    getline(intm,s);

    split(s,results,lline,a);
    
    while(a[1][0]=='.')                          // To read initial comments
    {
        lst<<a[0]<<"\t \t"<<s<<endl;
        lline+=5;
        getline(intm,s);
        split(s,results,lline,a);
    }

    if(a[2]=="START")                           // To write Header record
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

    while(true)                                   // To iterate over whole intermediate file
    {
        getline(intm,s);
        lline+=5;
        split(s,results,lline,a);
        //cout<<a[0]<<" "<<a[1]<<" "<<a[2]<<" "<<a[3]<<" "<<a[4]<<" "<<a[5]<<endl;

        if(s[0]=='.')                              // Comment Line
        {
            lst<<a[0]<<"\t\t "<<s<<endl;
            continue;
        }
        if(a[2]=="END")                             // Last line
        {
            lst<<a[0]<<"\t\t\t\t\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
            if(text_length>0)
            {
                obj<<text_s<<"^"<<extendTo(2,toHex(text_length/2))<<text_e<<endl;
            }
            text_length=0;
            text_s="";
            text_e="";
            for(int i=0;i<mod.size();i++)
                obj<<mod[i]<<endl;
            obj<<"E^"<<extendTo(6,start)<<endl;
            break;
        }
       // cout<<a[2]<<endl;
        process_instruction(a);                      // Any other line      
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

    style_end();

    intm.close();
    obj.close();
    lst.close();

    return 0;
}
