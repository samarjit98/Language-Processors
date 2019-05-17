%{
	#include <bits/stdc++.h>
	using namespace std;
	extern int yylineno;
	extern int yylex();
	extern FILE* yyin;
	FILE* fout;

	void yyerror (const char *s) {
   		fprintf (stderr, "%s\n", s);
 	}	

 	struct symbol_t{
 		int lineno;
 		string type;
 	};
 	struct place_t{
 		char place[10];
 	};

 	map<string, vector<symbol_t> > sym_table;
 	int labelctr = 0, tempctr = 0;

 	string new_label(){
		char tmp[10];
		sprintf(tmp, "%d", labelctr); labelctr++;
		string s(tmp);
		return "_l" + s;  
	}

	string new_temp(){
		char tmp[10];
		sprintf(tmp, "%d", tempctr); tempctr++;
		string s(tmp);
		return "_t" + s;  
	}
%}

%union{
	int assop, i;
	struct place_t* place;
	char* id;
	char* relop;
}

%token MAIN IF ELSE WHILE DEC
%token <id> ID
%token <id> INTEGER
%token <relop> RELOP
%nonassoc '(' ')'
%nonassoc '{' '}'
%nonassoc ';'
%left OR
%left AND
%left NOT
%left '+' '-'
%left '*' '/'
%type <place> EXPR STMT STMTS PROGRAM BOOL BLOCK
%start PROGRAM

%%

PROGRAM: MAIN '{' STMTS '}' { 
		printf("Ended code generation!\n"); 
	}
	;

STMTS: STMT ';'
	| BLOCK
	| STMT ';' STMTS 
	| BLOCK STMTS
	;

STMT: DEC ID {
		
	}
	| ID '=' EXPR {
		
	}
	;

EXPR: EXPR '+' EXPR {
		
	}
	| EXPR '*' EXPR {
		
	}
	| '(' EXPR ')' {
		
	}
	| '-' EXPR {
		
	}
	| ID {
		
	}
	| INTEGER {

	}
	;

BLOCK: IF '(' BOOL ')' '{' STMTS '}' {

	} 
	
	| IF '(' BOOL ')' '{' STMTS '}' ELSE '{' STMTS '}' { 
		
	}
	
	| WHILE '(' BOOL ')' '{' STMTS '}' {
		
	}
	;

BOOL: BOOL AND BOOL {
		
	}
	| BOOL OR BOOL {
		
	}
	| NOT BOOL {
		
	}
	| EXPR RELOP EXPR {
		
	}

%%

int main(int argc,char *argv[]){
	if(argc!=2){
		printf("Number of input files not proper");
		exit(0);
	}
	yyin = fopen(argv[1], "r");
	if(yyin==NULL){
		perror("Input file not opened");
		exit(0);
	}
	fout = fopen("./outputs/output.txt", "w");
	yyparse();
	fclose(fout);
	return 0;
}






