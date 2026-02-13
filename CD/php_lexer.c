#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int parameter = 0;
int isVar = 0;
int functionBegin = 0;
typedef struct SymbolTable
{
	char lexeme[100];
	char type[100];
}SymbolTable;

int isIdent(int c)
{
	if((char)(c) == '$')
		return 1;
	return 0;
}

int isAlph(int c)
{
	if((65<=c && c<=90) || (97<=c && c<=122))
		return 1;
	return 0;
}

int isKeyword(char* buffer,int size)
{
	char keywords[6][100] = {"function","if","for","return","int","break"};
	for(int i=0;i<6;i++)
	{
		if(strcmp(keywords[i],buffer) == 0)
			return 1;
	}
	return 0;
}

int isOperator(char *buffer,int size)
{
	if(size == 0)
		return 1;
	char operators[4][5] = {">=","<=","->","++"};
	for(int i=0;i<4;i++)
	{
		if(strcmp(operators[i],buffer) == 0)
			return 1;
	}
	return 0;
}

int isSingleOperator(int c)
{
	char a = (char)(c);
	char operators[7] = {'+','-','*','/','<','>','='};
	for(int i=0;i<7;i++)
	{
		if(operators[i] == a)
			return 1;
	}
	return 0;
}

int isSymbol(int c)
{
	char a = (char)(c);
	char symbols[7] = {'(',')','{','}',';',':',','};
	for(int i=0;i<7;i++)
	{
		if(symbols[i] == a)
			return 1;
	}
	return 0;
}

int isNumber(int c)
{
	if(48<=c && c<=57)
		return 1;
	return 0;
}

int lookup(SymbolTable *table, char *buffer, int tableindex)
{
	for(int i=0;i<tableindex;i++)
	{
		if(strcmp(table[i].lexeme,buffer) == 0)
			return 1;
	}
	return 0;
}

void insert(SymbolTable *table, char *buffer, int parameter,int isVar,int tableindex)
{
	strcpy(table[tableindex].lexeme,buffer);
	if(parameter > 0) // handles function parameters
		strcpy(table[tableindex].type,"PARAMETER");
	else if(isVar > 0) // handles variables within function
		strcpy(table[tableindex].type,"LOCAL VARIABLE");
	else if(functionBegin == 1) // handles function name
		strcpy(table[tableindex].type,"FUNCTION");
	else // handles all other identifiers
		strcpy(table[tableindex].type,"GLOBAL VARIABLE");
	return;
}

void printTable(SymbolTable *table,int tableindex)
{
	printf("\n\n");
	printf("-------------------------SYMBOL TABLE--------------------------\n\n");
	printf("|%-30s|%-30s|\n","LEXEME","TYPE");
	printf("---------------------------------------------------------------\n");
	for(int i=0;i<tableindex;i++)
	{
		printf("|%-30s|%-30s|\n",table[i].lexeme,table[i].type);
	}
}

int main()
{
	FILE *input;
	input = fopen("sample.txt","r");
	if(input == NULL)
	{
		printf("Error in opening the file\n");
		return 0;
	}

	SymbolTable table[1000];
	int symboltableindex = 0;

	int c;
	while((c = fgetc(input)) != EOF)
	{
		// process single line comments
		if((char)(c) == '/')
		{
			c = fgetc(input);
			// handles single line comments
			if((char)(c) == '/')
			{
				while(c!=EOF && (char)(c)!='\n') // consume all chars in comment
					c = fgetc(input);
			}
			//handles multi line comments
			else if((char)(c) == '*')
			{
				c = fgetc(input); // consume the first character of the comment
				char buf[3];
				buf[0] = ' ';
				buf[1] = ' ';
				buf[2] = '\0';
				while(c!=EOF && buf[0]!='*' && buf[1]!='/')
				{
					int n = fgetc(input);
					if((char)(c) == '*' && (char)(n) == '/')
					{
						buf[0] = '*';
						buf[1] = '/';
						break;
					}
					else
					{
						buf[0] = ' ';
						buf[1] = ' ';
					}
					ungetc(n,input);
					c = fgetc(input);
				}
				c = fgetc(input); // consume newline at the end of after */
			}
			else
			{
				ungetc(c,input); // behaves like an unwind
				// now we handle this as a division operator
				printf("[Operator, /]\n");
			}
		}
		else if(isIdent(c) == 1) // checks for $ sign, start of identifier
		{
			char buffer[100];
			int bufferindex = 0;
			while(c!=EOF && (isAlph(c) == 1 || isIdent(c) == 1))
			{
				buffer[bufferindex++] = (char)(c);
				c = fgetc(input);
			}
			ungetc(c,input); // this is important because the while loop consumes extra char
							 // but if n: is there : is skipped, usually they are space seperated
							 // so it's okay, but here it is not
			buffer[bufferindex++] = '\0';
			// insert into the symbol table
			if(lookup(table,buffer,symboltableindex) == 0) // insert only if absent
			{
				insert(table,buffer,parameter,isVar,symboltableindex);
				symboltableindex++;
			}
			printf("[Identifier, %s]\n",buffer);
		}
		else if(isAlph(c) == 1) // if any other character, could be a keyword
		{
			char buffer[100];
			int bufferindex = 0;
			while(c!=EOF && (isAlph(c) == 1 || isNumber(c) == 1))
			{
				buffer[bufferindex++] = (char)(c);
				c = fgetc(input);
			}
			ungetc(c,input); // this is important because the while loop consumes extra char
							 // but if n: is there : is skipped, usually they are space seperated
							 // so it's okay, but here it is not
			buffer[bufferindex++] = '\0';
			if(isKeyword(buffer,bufferindex) == 1)
			{
				if(strcmp(buffer,"function")==0)
					functionBegin = 1;
				printf("[Keyword, %s]\n",buffer);
			}
			else
			{
				if(lookup(table,buffer,symboltableindex) == 0) // insert only if absent
				{
					insert(table,buffer,parameter,isVar,symboltableindex);
					symboltableindex++;
				}
				printf("[Identifier, %s]\n",buffer);
			}
		}
		else if(isSingleOperator(c) == 1) // Maybe we are looking at arithmetic / relational / logical operators
		{
			//handle <? seperately (remove all lines thaat start with )
			int n = fgetc(input);
			if((char)c == '<' && (char)(n) == '?')
			{
				while(c!=EOF && c!='\n')
					c = fgetc(input);
				c = fgetc(input); // consume the new line char
				continue;
			}
			if((char)(c) == '?' && (char)(n) == '>')
			{
				while(c!=EOF && c!='\n')
					c = fgetc(input);
				c = fgetc(input); // consume the new line char
				continue;
			}
			// we read 2 chars as soon as we see an operator, we later revert if it isn't a valid 
			// relational / logical / arithmetic operator
			char buffer[3];
			buffer[0] = (char)(c);
			buffer[1] = (char)(n);
			buffer[2] = '\0';

			if(isOperator(buffer,3) == 1)
				printf("[Operator, %s]\n",buffer);
			else
			{
				buffer[1] = '\0';
				printf("[Operator, %s]\n",buffer);
				ungetc(n,input); // behaves like an unwind
			}
		}
		else if(isSymbol(c) == 1)
		{
			char buffer[100];
			int bufferindex = 0;
			while(c!=EOF && isSymbol(c) == 1)
			{
				buffer[bufferindex++] = (char)(c);
				c = fgetc(input);
			}
			ungetc(c,input); // this is important because the while loop consumes extra char
							 // but if (n is there n is skipped, usually they are space seperated
							 // so it's okay, but here it is not
			buffer[bufferindex++] = '\0';
			printf("[Symbol, %s]\n", buffer);
			// in symbol table this allows to
			// differentiate parameter, and function name and variables within 
			if(buffer[0] == '(' && functionBegin == 1)  
				parameter++;	 		
			// here the code keeps track if the "function" keyword arrives, and
			// then if '(' arrives , then parameters,
			// then if ')' parameter list ends, 
			// then if '{' all variables are arriving 	
			// then if '}' function definition has completed
			// functionBegin = 0, function has ended	
			else if(buffer[0] == ')' && functionBegin == 1)
				parameter--;
			else if(buffer[0] == '{' && functionBegin == 1)
				isVar++;
			else if(buffer[0] == '}' && functionBegin == 1)
			{
				isVar--;	
				if(isVar == 0)
					functionBegin = 0;		
			}
				
		}
		else if(isNumber(c) == 1)
		{
			char buffer[100];
			int bufferindex = 0;
			while(c!=EOF && isNumber(c) == 1)
			{
				buffer[bufferindex++] = (char)(c);
				c = fgetc(input);
			}
			ungetc(c,input); // this is important because the while loop consumes extra char
							 // but if 1; is there ; is skipped, usually they are space seperated
							 // so it's okay, but here it is not
			buffer[bufferindex++] = '\0';
			printf("[Number, %s]\n",buffer);
		}
		else if(c == 34) // ASCII Value of double quotes 
		{
			char buffer[100];
			int bufferindex = 0;
			c = fgetc(input); // read first char in string
			while(c!=EOF && c!=34)
			{
				buffer[bufferindex++] = (char)(c);
				c = fgetc(input);
			}
			buffer[bufferindex++] = '\0';
			printf("[Stirng, \"%s\"]\n",buffer);
		}
		else if(c == '\n' || c == '\t' || c == ' ')
			continue;
	}
	printTable(table,symboltableindex);
	return 0;
}