#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char table[1000][1000][5];
char operators[100]; int num_op = 0;

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

void parse(){
	char word[100];
	printf("Enter word to be parsed:"); scanf("%s", word); strcat(word, "$");

	char stack[100]; int top = 0;
	stack[top] = '$'; top++;
	int ip = 0;

	printf("\n\nParsing actions:\n\n");
	while(1){
		printf("\tSTACK STATE: ");
		for(int i=0; i<top; i++)printf("%c ", stack[i]);
		printf("\n");

		printf("\tRELATIONSHIP: %c and %c is ", word[ip], stack[top-1]);
		printf("%s", table[(int)word[ip]][(int)stack[top-1]]);
		printf("\n");

		if(word[ip]=='$' && stack[top-1]=='$'){
			printf("\tParsing complete!\n"); break;
		}
		else if(strcmp(table[(int)word[ip]][(int)stack[top-1]], ".>")==0 || strcmp(table[(int)word[ip]][(int)stack[top-1]], ".=")==0){
			stack[top] = word[ip]; top++;
			printf("\t%c added to stack.\n", word[ip]); ip++;
		}
		else if(strcmp(table[(int)word[ip]][(int)stack[top-1]], "<.")==0){
			char c;
			printf("\tPopping state..\n");
			do{
				c = stack[top-1]; top--;

				printf("\tRELATIONSHIP: %c and %c is ", c, stack[top-1]);
				printf("%s", table[(int)c][(int)stack[top-1]]);
				printf("\n");
				printf("\tSTACK STATE: ");
				for(int i=0; i<top; i++)printf("%c ", stack[i]);
				printf("\n");

			}while(strcmp(table[(int)c][(int)stack[top-1]], ".>")!=0);
		}
		else{
			printf("\tError!\n"); break;
		}
		printf("\n");
	}
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

*/