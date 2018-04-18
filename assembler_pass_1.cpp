#include<bits/stdc++.h>
#include<fstream>
#include "Tables.cpp"

using namespace std;

int error_flag = 0;                                           // To indicate any error encountered
int prg_len=0;                                                // To store length of program                  
ifstream f1;                                                  // For reading input file
ofstream f2,error;                                            // For writing to intermediate and error file

//vector<int> pc;

string to_string( int x )                                     // To convert integer to string in case C++11 compiler is not used
{
  int length = snprintf( NULL, 0, "%d", x );
  assert( length >= 0 );
  char* buf = new char[length + 1];
  snprintf( buf, length + 1, "%d", x );
  std::string str( buf );
  delete[] buf;
  return str;
}


vector<string> split(string s, vector<string> results)         // To split input line and read all the words of the file
{
    results.clear();
    istringstream iss(s);
	do
    {
        string subs;
        iss >> subs;
        results.push_back(subs);
    } while (iss);

    results.pop_back();                                        // Now ensure that we have label opcode and operand 

    if(results[0][0]=='.')
        return results;

    if(results.size()==3)
        return results;

    if(results.size()==1)
    {
        results.insert(results.begin(),"-");
        results.push_back("-");
        return results;
    }

    if(results.size()==2)                                     // A "-" indicates absence of a particular field
    {
        string a="";

        if(results[0][0]=='+')
        {
            a=results[0].substr(1);
            if(OPTAB[a].isthere==true||a=="WORD"||a=="RESW"||a=="BYTE"||a=="RESB"||a=="BASE"||a=="NOBASE"||a=="END")
                results.insert(results.begin(),"-");
            else
            {
                results.push_back("-");
            }
        }
        else
        {
            a=results[0];
            if(OPTAB[a].isthere==true||a=="WORD"||a=="RESW"||a=="BYTE"||a=="RESB"||a=="BASE"||a=="NOBASE"||a=="END")
                results.insert(results.begin(),"-");
            else
            {
                results.push_back("-");
            }
        }
        return results;
    }

    return results;
}

int stoint(string s)                                        // To convert string to integer
{
	stringstream geek(s);
 	int x = 0;
    geek >> x;
 	return x;
}

void exe()
{
	create();
	string s;
	f1.open("input.txt");  										// Read input
	f2.open("intermediate.txt");  								// write to intermediate file
	error.open("error.txt"); 	                                // to write errors encountered
	getline(f1,s); 												// to get first input line
	vector<string> results;

    int loc=0; 													// Location Counter
    results = split(s,results);

    int s_a;
    
    while (results[0]==".")                                     // To read all the comments at the beginning of input file
    {
    	f2<<s<<endl;
    	getline(f1,s);
    	results = split(s,results);
    }

    if(results[1]=="START")                                     // Initializing loc in presence of START directive otherwise loc=0
    {
        if(results[2]!="-")
	    {
            s_a = stoint(results[2]);
	       loc+=s_a;
        }
        else loc = 0;

        //cout<<s_a<<" "<<loc<<endl;
       	f2<<loc<<"\t";
       	for(int i=0;i<results.size();i++)
       		f2<<results[i]<<"\t";
       	f2<<endl;
    	getline(f1,s);
    	results = split(s,results);
    }

    

    while(!f1.eof()&&results[1]!="END")
    {
        //cout<<results[0]<<" "<<results[1]<<" "<<results[2]<<endl;
        int p=0;
    	string opcode =results[1];
    	string label = results[0];
    	string operand = results[2];
    	int mode=0,format=0;

        if(results[1][0]=='+')                                              // Updating format and opcode in case of format 4
        {
            format=4;
            opcode=results[1].substr(1);
        }

    	if(results[0][0]!='.')
    	{
    		if(label!="-")
    		{
    			if(SYMTAB[results[0]].isthere==true)                         // Checking if Symbol is already present
    			{
    			    error<<"Line "<<s<<": Duplicate Symbol"<<endl;
    			   	error_flag=1;
    			}
    			else                                                         // Adding a new symbol to SYMTAB
    			{
    				SYMTAB[results[0]].value = results[0];
    				SYMTAB[results[0]].address = loc;
    			}
    		}
            /* To get the next increement of loc */
	    	if(OPTAB[opcode].isthere==true)
	    	{
	    		if(format==4)
	    			p=format;
	    		else
	    			p=OPTAB[opcode].format;
	    	}
	    	else if(opcode=="WORD")
	    	{
	    		p=3;
	    	}
	    	else if(opcode=="RESW")
	    	{
	    		int x = stoint(operand);
	    		p=(3*x);
	    	}
	    	else if(opcode=="RESB")
	    	{
	    		int x = stoint(operand);
	    		p=x;
	    	}
	    	else if(opcode=="BYTE")
	    	{
	    		string x = operand.substr(1);
	    		if(operand[0]=='C')
	    		p=(x.length()-2);
	    		else
	    		p=((x.length()-2)/2);
	    	}
            else if(opcode=="BASE"||opcode=="NOBASE")
            {
                mode=1;
            }
	    	else
	    	{
	    		error<<"Line "<<s<<": Invalid Opcode"<<endl;
	       		error_flag = 1;
	    	}
    	}

        if(mode==0)                                // Writing intermediate file     
        {
            if(results[0][0]=='.')
                f2<<s<<endl;
            else
            {
                f2<<loc<<"\t";
                for(int i=0;i<results.size();i++)
                f2<<results[i]<<"\t";
                f2<<loc+p<<endl;
            }    
        }
	    else                                       // Case of Base or Nobase
        {
            f2<<"-"<<"\t";
            for(int i=0;i<results.size();i++)
            f2<<results[i]<<"\t";
            f2<<loc+p<<endl;
            
        }
        loc+=p;
		getline(f1,s);
		results=split(s,results);

	}	

	f2<<loc<<"\t";
    for(int i=0;i<results.size();i++)
       		f2<<results[i]<<"\t";
       	f2<<endl;
    prg_len = loc - s_a;

    f1.close();
    error.close();
    f2.close();

}


  
