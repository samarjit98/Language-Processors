#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char table[1000][1000][5];
char operators[100]; int num_op = 0;
char derivation_sequence[100][100]; int curr_der = 0;
char word_now[100]; int wip;

void init(){
	printf("Enter number of operators:");
	int n; scanf("%d", &n);
	while(n--){
		scanf("%s", &operators[num_op]); num_op++;
	}

	for(int i=0; i<num_op; i++){
		for(int j=0; j<num_op; j++){
			printf("Relationship between %c and %c:", operators[i], operators[j]); 
			scanf("%s", table[(int)operators[i]][(int)operators[j]]);
		}
	} 
}

void replace(char c){
	int i = wip-1;
	while(i>=0){
		if(word_now[i] == c)break; 
		i--;
	}

	if(word_now[i]=='d')word_now[i] = 'E';
	else{
		word_now[i-1] = 'E';
		for(int j=i+2; j<strlen(word_now); j++)word_now[j-2] = word_now[j];

		word_now[strlen(word_now)-2] = '\0';
		wip = wip - 2;
	}
	strcpy(derivation_sequence[curr_der], word_now); curr_der++;
}

void parse(){
	char word[100];
	printf("Enter word to be parsed:"); scanf("%s", word); strcat(word, "$");
	strcpy(word_now, word);
	strcpy(derivation_sequence[curr_der], word_now); curr_der++;

	char stack[100]; int top = 0;
	stack[top] = '$'; top++;
	int ip = 0; wip = 0;

	FILE* fout = fopen("output.txt", "w");

	fprintf(fout, "\n\nParsing actions for %s:\n\n", word);
	while(1){
		fprintf(fout, "\tSTACK STATE: ");
		for(int i=0; i<top; i++)fprintf(fout, "%c ", stack[i]);
		fprintf(fout, "\n");

		fprintf(fout, "\tRELATIONSHIP: %c and %c is ", word[ip], stack[top-1]);
		fprintf(fout, "%s", table[(int)word[ip]][(int)stack[top-1]]);
		fprintf(fout, "\n");

		if(word[ip]=='$' && stack[top-1]=='$'){
			fprintf(fout, "\tParsing complete!\n"); break;
		}
		else if(strcmp(table[(int)word[ip]][(int)stack[top-1]], ".>")==0 || strcmp(table[(int)word[ip]][(int)stack[top-1]], ".=")==0){
			stack[top] = word[ip]; top++;
			fprintf(fout, "\t%c added to stack.\n", word[ip]); ip++; wip++;
		}
		else if(strcmp(table[(int)word[ip]][(int)stack[top-1]], "<.")==0){
			char c;
			fprintf(fout, "\tPopping state..\n");
			do{
				c = stack[top-1]; top--;
				replace(c);

				fprintf(fout, "\tRELATIONSHIP: %c and %c is ", c, stack[top-1]);
				fprintf(fout, "%s", table[(int)c][(int)stack[top-1]]);
				fprintf(fout, "\n");
				fprintf(fout, "\tSTACK STATE: ");
				for(int i=0; i<top; i++)fprintf(fout, "%c ", stack[i]);
				fprintf(fout, "\n");

			}while(strcmp(table[(int)c][(int)stack[top-1]], ".>")!=0);
		}
		else{
			fprintf(fout, "\tError!\n"); break;
		}
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n\nDerivation sequence:\n\n");
	for(int i=0; i<curr_der; i++)fprintf(fout, "\t%s\n", derivation_sequence[i]);
}

int main(int argc, char* argv[]){
	init();
	parse();
	return 0;
}

/*

➜  Operator_prec_parsing git:(master) ✗ ./a.out            
Enter number of operators:4
d
+
*
$
Relationship between d and d:0
Relationship between d and +:.>
Relationship between d and *:.>
Relationship between d and $:.>
Relationship between + and d:<.
Relationship between + and +:.>
Relationship between + and *:<.
Relationship between + and $:.>
Relationship between * and d:<.
Relationship between * and +:.>
Relationship between * and *:.>
Relationship between * and $:.>
Relationship between $ and d:<.
Relationship between $ and +:<.
Relationship between $ and *:<.
Relationship between $ and $:.>
Enter word to be parsed:d+d*d


Parsing actions:

	STACK STATE: $ 
	RELATIONSHIP: d and $ is .>
	d added to stack.

	STACK STATE: $ d 
	RELATIONSHIP: + and d is <.
	Popping state..
	RELATIONSHIP: d and $ is .>
	STACK STATE: $ 

	STACK STATE: $ 
	RELATIONSHIP: + and $ is .>
	+ added to stack.

	STACK STATE: $ + 
	RELATIONSHIP: d and + is .>
	d added to stack.

	STACK STATE: $ + d 
	RELATIONSHIP: * and d is <.
	Popping state..
	RELATIONSHIP: d and + is .>
	STACK STATE: $ + 

	STACK STATE: $ + 
	RELATIONSHIP: * and + is .>
	* added to stack.

	STACK STATE: $ + * 
	RELATIONSHIP: d and * is .>
	d added to stack.

	STACK STATE: $ + * d 
	RELATIONSHIP: $ and d is <.
	Popping state..
	RELATIONSHIP: d and * is .>
	STACK STATE: $ + * 

	STACK STATE: $ + * 
	RELATIONSHIP: $ and * is <.
	Popping state..
	RELATIONSHIP: * and + is .>
	STACK STATE: $ + 

	STACK STATE: $ + 
	RELATIONSHIP: $ and + is <.
	Popping state..
	RELATIONSHIP: + and $ is .>
	STACK STATE: $ 

	STACK STATE: $ 
	RELATIONSHIP: $ and $ is .>
	Parsing complete!


Derivation sequence:

	E+d*d$
	E+E*d$
	E+E*E$
	E+E$
	E$

*/