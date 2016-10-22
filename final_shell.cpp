#include <iostream>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>

using namespace std;

extern  int alphasort();
#define FALSE 0
#define TRUE !FALSE

//int minimum_edit_distance(string alpha, string beeta);
void get_partial_match_table(string pattern_string, int pattern_len, int pmt[]);
//string suggest_command(string input);
void curtime();
vector<string> tokenizer(string sentence);
vector<int> get_white_spaces(string sentence);
void get_current_directory();
string strtrim(string sentence);
vector<int> search_substring(string text_string, string pattern_string);
vector<int> KMP_implementation(string text_string, string pattern_string);
void change_directory(string directory);


string arr[9] = {"curtime","whereami","ls","cd","refile","delfile","deldir","grep","exit"};
char pathname[MAXPATHLEN];

int main()
{
	
		cout<<"sanjeev@sanjeev-HP-ProBook-445-G1";
		cout<<">>";
		string command_line;	
		while(1)
		{

		getline(cin, command_line);
		vector<string> tokens = tokenizer(command_line);		

		int command=0;
		for(int i=0;i<8;i++)
		{				
				if(tokens[0] == arr[i])
				{			
					command=i;
					break;
				}					
		}

		switch(command)	
		{
		case 0: curtime();
				break;
		case 1: get_current_directory();		
				break;
		case 2:								 
				break;
		case 3: change_directory(tokens[1]);	
				break;
		case 4://remove_file();
		break;
		case 5://remove_dir();
		break;
		case 6://grep function
		break;
		case 7: //exit(0);		
		break;

		}

	}
}

void get_current_directory()
{
	 char buff [1024];
	 if (getcwd(buff,sizeof(buff))!=NULL) cout<<"Current directory is: "<<buff<<endl;
	 else cout<<"ERROR, getting directory"<<endl;	
}

void ls()
{
	int count,i;
	struct direct **files;
	int file_select();
	char pathname [1024];
	if (getcwd(pathname, sizeof(pathname)) == NULL )
	{				
		cout<<"Error getting path"<<endl;		
	}

	count = scandir(pathname, &files, file_select, alphasort);
	if (count <= 0)
	{		 
		cout<<"No files in this directory"<<endl;		
	}
	for (i=1;i<count+1;++i)
	{

		cout<<files[i-1]->d_name<<"    ";
		
	}


}

int file_select(struct direct *entry)
{
	if ((entry->d_name == ".") ||(entry->d_name == ".."))
		return (FALSE);
	else
		return (TRUE);
}

void curtime()
{
 	time_t rawtime;
 	struct tm * timeinfo;
 	time ( &rawtime );
 	timeinfo = localtime ( &rawtime );
 	cout<<"Current time and date: "<< asctime (timeinfo)<<endl ; // asctime() converts tm structure to string format
}

void change_directory(string path)
{	
	int a;
	string my_path = path;	
	const char * directory = my_path.c_str();	
    a = chdir(directory);    
    cout<<"value of a: "<<a<<endl;
    if (a==0) cout<<"Directory changed successful"<<endl;
    else cout<<"ERROR: No such directory found"<<endl;
}


vector<string> tokenizer(string sentence)
{
	int p;
	sentence = strtrim(sentence);	
	vector<string> tokens (10);		
	vector<int> index_vector = get_white_spaces(sentence);	
	int pre,post,length,i;
	i= 0;
	length = 0;
	string temp;
	while(i < index_vector.size())
	{
		if((i ==0)||(index_vector.at(i) !=0))
		{
			temp.clear();
			pre = index_vector.at(i);
			post = index_vector.at(i+1);			
			for(int j = pre;j<=post;j++)
			{
				temp = temp + sentence[j];
			}			
			tokens.at(length) = temp;
			length++;		
		}

		i = i+2;
	}	
	return tokens;
}
vector<int> get_white_spaces(string sentence)
{	
	vector<int> space_index(20);
	char x;
	int i = 0;
	int j,k;
	k = 0;
	int diff = 0;	
	space_index.at(k) = 0;
	k++;
	while(i<sentence.size())
	{		
		if(sentence[i] == ' ')
		{
			space_index.at(k) = i-1;
			k++;
			j = i;
			while(j<sentence.size()-1)
			{				
				if((sentence[j]==' ')&&(sentence[j+1]!=' ')) 
				{			
					space_index.at(k) = j+1;
					k++;
					break;
				}
				else j++;
			}						
			diff = j-i;
			i = j+1;			
		}	
		else i++;			
		 
	}
	space_index.at(k) = sentence.size()-1;	
	return space_index;
}

string strtrim(string sentence)
{	
	int i = 0;
	sentence = sentence.c_str();
	int j = sentence.size()-1;	
	char x;		
	while(i < j)
	{
		if(sentence[0] != ' ') 
		{
				i = 0;
				break;
		}
		else if((sentence[i] == ' ') && (sentence[i+1] != ' ')) 
		{
				i = i+1;
				break;
		}
		else
		{
			i++;			
		}
	}	
	while(j>0)
	{
		if(sentence[sentence.size()-1] != ' ') break;		
		else if((sentence[j] == ' ') && (sentence[j-1]!=' ')) 
		{	
			j = j-1;
			break;
		}
		else j--;
	}
	string trm_str;
	trm_str.clear();	

	for(int k = i; k<=j; k++)
	{
		x = sentence[k];
		trm_str = trm_str + x;		
	}	
	return trm_str;

}


vector<int> KMP_implementation(string text_string, string pattern_string)   //This is implementation of KMP algorithm
{	vector<int> v;
	v = search_substring(text_string, pattern_string);
	return v;
}

vector<int> search_substring(string text_string, string pattern_string)
{
	vector<int> index_vector;
	index_vector.push_back(0);
	int pattern_len, text_len, j, i,count;
	count  = 0;
	pattern_len = pattern_string.length();
	text_len = text_string.length();
	int pmt[pattern_len];
	j = 0;
	get_partial_match_table(pattern_string, pattern_len, pmt);
	i = 0;
	while (i < text_len)
	{
		if (pattern_string[j] == text_string[i])
		{
			j = j+1;
			i = i+1;
		}
		if (pattern_len == j)
		{			
			index_vector.push_back(i-j-1);
			index_vector.push_back(i-j+1);
			count = count +1;
			j = pmt[j-1];
		}
		else if ((i < text_len)&&(pattern_string[j] != text_string[i]))
		{
			if ( 0 != j)
			{
				j = pmt[j-1];
			}
			else
			{
				i = i + 1;
			}
		}
	}
	return index_vector;   //returns no of times a pattern is contained in it.
}

void get_partial_match_table(string pattern_string, int pattern_len, int pmt[])
{
	int j = 0;
	int i = 1;
	pmt[0] = 0;
	while (i < pattern_len)
	{
		if (pattern_string[i] == pattern_string[j])
		{
			pmt[i] = j+1;
			j++;
			i++;
		}
		else
		{
			if (0 != j)
			{
				j = pmt[j - 1];
			}
			else
			{
				pmt[i] = 0;
				i++;
			}
		
		}
	}
}


int min (int a, int b, int c)
{
	int min;
	if(a<b) min = a;
	else min = b;
	if(min<c)	 return min;		 
	else return c;
}
