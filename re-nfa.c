#include <stdio.h>
#include <string.h>
#include <ctype.h>

int contains(char string[], char character) {
	int i = 0;
	for(; i < strlen(string); i++)
		if(string[i] == character)
			return 1;
	return 0;
}

void printStatesNfa(char states[], char nfa[40][40]) {
	int i, j;
	int statesSize = strlen(states);
	
	printf("- ");
	for(i = 0; i < statesSize; i++)
		printf("%c ", states[i]);
	printf("\n");
	for(i = 0; i < statesSize + 1; i++)
		printf("--");
	printf("\n");
	
	for(i = 0; i < statesSize; i++) {
		printf("%c|", states[i]);
		for(j = 0; j < statesSize; j++) {
			printf("%c ", nfa[i][j]);
		}
		printf("\n");
	}
}

void printNFA(char nfa[40][40]) {
	int i, j;
	int statesSize = strlen(nfa[0]);
	
	printf("- ");
	for(i = 0; i < statesSize; i++)
		printf("%d ", i);
	printf("\n");
	for(i = 0; i < statesSize + 1; i++)
		printf("--");
	printf("\n");
	
	for(i = 0; i < statesSize; i++) {
		printf("%d|", i);
		for(j = 0; j < statesSize; j++) {
			printf("%c ", nfa[i][j]);
		}
		printf("\n");
	}
}

void reToNfa(const char* re) {
	char states[40], alphabet[20], meta[20];
	int i, j, temp;
	int statesSize = 0, alphabetSize = 0, metaSize = 0;
	char nfa[40][40];
	
	//Creating a state for every symbol in re + accepted state & Finding alphabet
	for(i = 0; i < strlen(re); i++) {
		states[statesSize++] = re[i];
		if(isalpha(re[i]))
			alphabet[alphabetSize++] = re[i];
		else
			meta[metaSize++] = re[i];
	}
	//Accepted state
	states[statesSize++] = '@';
	states[statesSize] = 0;
	
	alphabet[alphabetSize] = 0;
	meta[metaSize] = 0;
	
	//Filling with hyphen-s
	for(i = 0; i < statesSize; i++) {
		for(j = 0; j < statesSize; j++) {
			nfa[i][j] = '-';
		}
		nfa[i][statesSize] = 0;
	}
			
	//Adding transition from alphabet to next state
	for(i = 0; i < strlen(re); i++) {
		if(contains(alphabet, re[i]))
			nfa[i][i + 1] = re[i];
	}
	
	//Adding E-transition from paranthesis to next state
	for(i = 0; i < strlen(re); i++) {
		if(contains("()", re[i]))
			nfa[i][i + 1] = 'E';
	}	
	
	//Adding 3 E-transition for each * operator
	for(i = 0; i < strlen(re); i++) {
		if(re[i] == '*') {
			nfa[i][i + 1] = 'E';
			if(re[i - 1] != ')') {
				nfa[i][i - 1] = 'E';
			} else {
				temp = i;
				while(re[--temp] != '(');
				nfa[temp][i] = 'E';
				nfa[i][temp] = 'E';
			}
		}
	}

	//Adding 2 E-transition for each | operator
	for(i = 0; i < strlen(re); i++) {
		if(re[i] == '|') {
			temp = i;
			while(re[--temp] != '(');
			nfa[temp][i + 1] = 'E';
			temp = i;
			while(re[++temp] != ')');
			nfa[i][temp] = 'E';
		}
	}
	
	//printing
	//printStatesNfa(states, nfa);
	//printf("\n\n\n");
	printNFA(nfa);
}

int main() {
	char re[40];
	printf("Enter regex: ");
	scanf("%s", re);
	reToNfa(re);
	return 0;
}
