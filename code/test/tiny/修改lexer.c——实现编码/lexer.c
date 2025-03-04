#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
//���йؼ���
const char* keywordSet[8] = {"else","end","if","read","repeat","then","until","write" };
//��Ӧop��operators_name
static struct
{
	const char* str;
	const char* value;
} reservedWords[16] = {{"+","PLUS"},{"(","LPAN"},{")","RPAN"},{"*","MULTIPLY"},{"%","MOD"},{"-","MINUS"},{"/","DIVIDE"},{":=","ASSIGN"},{";","SEMI"},{"<","LT"},{"<=","LTEQ"},{"<>","NE"},{"=","EQ"},{">","RT"},{">=","RTEQ"},{"^","CARET"} };
//lex�����ӳ��
static struct{
	const char* op;
	const int code;
} keywordLexCode[8]={{"else",3},{"end",4},{"if",1},{"read",7},{"repeat",5},{"then",2},{"until",6},{"write",8} };
static struct{
	const char* op;
	const int code;
} opLexCode[16]={{"%",205},{"(",214},{")",215},{"*",202},{"+",200},{"-",201},{"/",203},{":=",213},{";",212},{"<",206},{"<=",208},{"<>",207},{"=",211},{">",210},{">=",209},{"^",204} };
//�Ƿ��ǹؼ���keywords
bool isKeyWords(const char* str) {
	int i = 0;
	for (i = 0; i < 8; i++) {
		if (!strcmp(str,keywordSet[i])) {
			return true;
		}
	}
	return false;
}
//׷���ַ�
void append(char* str, char tmp) {
	int len = strlen(str);
	str[len] = tmp;
	str[len + 1] = '\0';
}

//�Ƿ��Ǳ����ַ�
const char* getReversedWord(const char* str) {
	int i;	for (i = 0; i < 16; ++i) {
		if (strcmp(reservedWords[i].str, str) == 0) {
			return reservedWords[i].value;
		}
	}
	return NULL;
}

//�ж��ǲ�������
bool isDigit(char ch){
	if(ch >= '0' && ch <= '9') return true;
	else return false;
}
//�ж��Ƿ�����ĸ
bool isAlpha(char ch){
	if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))return true;
	else return false;
}
void lexicalAnalysis(FILE* input_file,FILE* output_file) {
	char current_char = fgetc(input_file);
	if (current_char == ' ' || current_char == '\n' || current_char == '\t'){
		return;
	}
	ungetc(current_char, input_file);
	int current_state = 0;
	bool flag = false;
	bool inId = false;
	bool inNum = false;
	bool isInComment = false;
	char cur_line[1024];
	cur_line[0] = '\0';
	while (!flag) {
		current_char = fgetc(input_file);//��ȡ��һ���ַ�
		switch (current_state) {
		case 0: {
			if(isDigit(current_char)) {
				inNum = true; current_state = 1; break;
			}
			else if(isAlpha(current_char)){
				inId = true; current_state = 4; break;
			}
			switch (current_char) {
			case '+':current_state = 3; break;
			case '(':current_state = 3; break;
			case ')':current_state = 3; break;
			case '*':current_state = 3; break;
			case '%':current_state = 3; break;
			case '-':current_state = 3; break;
			case '/':current_state = 3; break;
			case ':':current_state = 2; break;
			case ';':current_state = 3; break;
			case '<':current_state = 6; break;
			case '=':current_state = 3; break;
			case '>':current_state = 7; break;
			case '^':current_state = 3; break;
			case '{':current_state = 5; break;
			}
			break;
		}
		case 1: {
			if(isDigit(current_char)) {
				inNum = true; current_state = 1; break;
			}
			switch (current_char) {
			}
			break;
		}
		case 2: {
			switch (current_char) {
			case '=':current_state = 3; break;
			}
			break;
		}
		case 3: {
			switch (current_char) {
			}
			break;
		}
		case 4: {
			if(isDigit(current_char)) {
				inNum = true; current_state = 4; break;
			}
			else if(isAlpha(current_char)){
				inId = true; current_state = 4; break;
			}
			switch (current_char) {
			}
			break;
		}
		case 5: {
			switch (current_char) {
			case '}':current_state = 3; break;
			default:current_state = 5; isInComment = true; break;
			}
			break;
		}
		case 6: {
			switch (current_char) {
			case '=':current_state = 3; break;
			case '>':current_state = 3; break;
			}
			break;
		}
		case 7: {
			switch (current_char) {
			case '=':current_state = 3; break;
			}
			break;
		}
		}
		append(cur_line, current_char);
		if (current_state ==1) {
			current_char = fgetc(input_file);
			if(isDigit(current_char)){} 
			else switch (current_char) {
			default: {
				flag=true;
			}
			}
			ungetc(current_char, input_file);
		}
		else if (current_state ==3) {
			current_char = fgetc(input_file);
			switch (current_char) {
			default: {
				flag=true;
			}
			}
			ungetc(current_char, input_file);
		}
		else if (current_state ==4) {
			current_char = fgetc(input_file);
			if(isDigit(current_char)){} 
			else if(isAlpha(current_char)){}
			else switch (current_char) {
			default: {
				flag=true;
			}
			}
			ungetc(current_char, input_file);
		}
		else if (current_state ==6) {
			current_char = fgetc(input_file);
			switch (current_char) {
			case '=':break;
			case '>':break;
			default: {
				flag=true;
			}
			}
			ungetc(current_char, input_file);
		}
		else if (current_state ==7) {
			current_char = fgetc(input_file);
			switch (current_char) {
			case '=':break;
			default: {
				flag=true;
			}
			}
			ungetc(current_char, input_file);
		}
	}
	if (isKeyWords(cur_line)) {
        int i ;for (i = 0; i < 8; i++) {
            if (strcmp(cur_line, keywordLexCode[i].op) == 0) {
                fprintf(output_file, "%d \n", keywordLexCode[i].code);
                printf("%d \n" , keywordLexCode[i].code);
                return;
            }
        }
    }
	if (inId) {
		fprintf(output_file, "101:%s \n", cur_line);
		printf("101:%s \n", cur_line);
	return;
	}
	if (inNum) {
		fprintf(output_file, "100:%s \n", cur_line);
		printf("100:%s \n", cur_line);
		return;
	}
	if (!isInComment) {
		int i ;for (i = 0; i < 16; i++) {
            if (strcmp(cur_line, opLexCode[i].op) == 0) {
                fprintf(output_file, "%d \n", opLexCode[i].code);
                printf("%d \n", opLexCode[i].code);
                return;
            }
        }
	}
};
int main(int argc, char* argv[]) {
	FILE* input_file = fopen("G:/04code/10Qt/byyl-combined5/test/tiny/��sample����/sample.tny", "r");
	if (input_file == NULL) {
		printf("Failed to open input file");
		return 1;
	}
	FILE* output_file = fopen("G:/04code/10Qt/byyl-combined5/test/tiny/��sample����/sample.lex", "w");
	if (output_file == NULL) {
		printf("Failed to open output file");
		fclose(input_file);
		return 1;
	}
	char c;
	while ((c=fgetc(input_file)) != EOF) {
		ungetc(c, input_file);
		lexicalAnalysis(input_file, output_file);
	}
	fprintf(output_file, "EOF:EOF");
	printf("EOF:EOF");
	fclose(input_file);
	fclose(output_file);
	return 0;
}
