#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
//所有关键字
const char* keywordSet[9] = {"do","double","else","float","if","int","return","void","while" };
//对应op与operators_name
static struct
{
	const char* str;
	const char* value;
} reservedWords[28] = {{"+","PLUS"},{"(","LPAN"},{")","RPAN"},{"*","MULTIPLY"},{"[","LMM"},{"]","RMM"},{"!=","NE"},{"%","MOD"},{"(",""},{")",""},{"*",""},{",","DOU"},{"-","MINUS"},{"/","DIVIDE"},{":=","ASSIGN"},{";","SEMI"},{"<","LT"},{"<=","LTEQ"},{"<>","NE"},{"=","ASSIGN"},{"==","EQ"},{">","RT"},{">=","RTEQ"},{"[",""},{"]",""},{"^","CARET"},{"{","LBM"},{"}","RBM"} };
//lex编码的映射
static struct{
	const char* op;
	const int code;
} keywordLexCode[9]={{"do",8},{"double",5},{"else",1},{"float",4},{"if",2},{"int",3},{"return",6},{"void",7},{"while",9} };
static struct{
	const char* op;
	const int code;
} opLexCode[21]={{"!=",211},{"%",205},{"(",215},{")",216},{"*",202},{"+",200},{",",214},{"-",201},{"/",203},{";",213},{"<",206},{"<=",207},{"=",212},{"==",210},{">",209},{">=",208},{"[",217},{"]",218},{"^",204},{"{",219},{"}",220} };
//是否是关键字keywords
bool isKeyWords(const char* str) {
	int i = 0;
	for (i = 0; i < 9; i++) {
		if (!strcmp(str,keywordSet[i])) {
			return true;
		}
	}
	return false;
}
//追加字符
void append(char* str, char tmp) {
	int len = strlen(str);
	str[len] = tmp;
	str[len + 1] = '\0';
}

//是否是保留字符
const char* getReversedWord(const char* str) {
	int i;	for (i = 0; i < 28; ++i) {
		if (strcmp(reservedWords[i].str, str) == 0) {
			return reservedWords[i].value;
		}
	}
	return NULL;
}
//判断是不是数字
bool isDigit(char ch){
	if(ch >= '0' && ch <= '9') return true;
	else return false;
}
//判断是否是字母
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
		current_char = fgetc(input_file);//获取下一个字符
		switch (current_state) {
		case 0: {
			if(isDigit(current_char)) {
				inNum = true; current_state = 1; break;
			}
			else if(isAlpha(current_char)){
				inId = true; current_state = 5; break;
			}
			switch (current_char) {
			case '+':current_state = 4; break;
			case '(':current_state = 4; break;
			case ')':current_state = 4; break;
			case '*':current_state = 4; break;
			case '[':current_state = 4; break;
			case ']':current_state = 4; break;
			case '!':current_state = 2; break;
			case '%':current_state = 4; break;
			case ',':current_state = 4; break;
			case '-':current_state = 4; break;
			case '/':current_state = 8; break;
			case ';':current_state = 4; break;
			case '<':current_state = 9; break;
			case '=':current_state = 9; break;
			case '>':current_state = 9; break;
			case '^':current_state = 4; break;
			case '_':current_state = 5; break;
			case '{':current_state = 4; break;
			case '}':current_state = 4; break;
			}
			break;
		}
		case 1: {
			if(isDigit(current_char)) {
				inNum = true; current_state = 1; break;
			}
			switch (current_char) {
			case '.':current_state = 3; break;
			}
			break;
		}
		case 2: {
			switch (current_char) {
			case '=':current_state = 4; break;
			}
			break;
		}
		case 3: {
			if(isDigit(current_char)) {
				inNum = true; current_state = 6; break;
			}
			switch (current_char) {
			}
			break;
		}
		case 4: {
			switch (current_char) {
			}
			break;
		}
		case 5: {
			if(isDigit(current_char)) {
				inNum = true; current_state = 5; break;
			}
			else if(isAlpha(current_char)){
				inId = true; current_state = 5; break;
			}
			switch (current_char) {
			case '_':current_state = 5; break;
			}
			break;
		}
		case 6: {
			if(isDigit(current_char)) {
				inNum = true; current_state = 6; break;
			}
			switch (current_char) {
			}
			break;
		}
		case 7: {
			switch (current_char) {
			case '\n':current_state = 4; break;
			default:current_state = 7; isInComment = true; break;
			}
			break;
		}
		case 8: {
			switch (current_char) {
			case '/':current_state = 7; break;
			}
			break;
		}
		case 9: {
			switch (current_char) {
			case '=':current_state = 4; break;
			}
			break;
		}
		}
		append(cur_line, current_char);
		if (current_state ==1) {
			current_char = fgetc(input_file);
			if(isDigit(current_char)){} 
			else switch (current_char) {
			case '.':break;
			default: {
				flag=true;
			}
			}
			ungetc(current_char, input_file);
		}
		else if (current_state ==4) {
			current_char = fgetc(input_file);
			switch (current_char) {
			default: {
				flag=true;
			}
			}
			ungetc(current_char, input_file);
		}
		else if (current_state ==5) {
			current_char = fgetc(input_file);
			if(isDigit(current_char)){} 
			else if(isAlpha(current_char)){}
			else switch (current_char) {
			case '_':break;
			default: {
				flag=true;
			}
			}
			ungetc(current_char, input_file);
		}
		else if (current_state ==6) {
			current_char = fgetc(input_file);
			if(isDigit(current_char)){} 
			else switch (current_char) {
			default: {
				flag=true;
			}
			}
			ungetc(current_char, input_file);
		}
		else if (current_state ==8) {
			current_char = fgetc(input_file);
			switch (current_char) {
			case '/':break;
			default: {
				flag=true;
			}
			}
			ungetc(current_char, input_file);
		}
		else if (current_state ==9) {
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
		int i ;for (i = 0; i < 9; i++) {
			if (strcmp(cur_line, keywordLexCode[i].op) == 0) {
				fprintf(output_file, "%d \n", keywordLexCode[i].code);
				printf("%d \n" , keywordLexCode[i].code);
				return;}
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
		int i; for(i = 0; i < 21; i++){
			if (strcmp(cur_line, opLexCode[i].op) == 0) {
				fprintf(output_file, "%d \n", opLexCode[i].code);
				printf("%d \n", opLexCode[i].code);
				return;
			}
		}
	}
};
int main(int argc, char* argv[]) {
	FILE* input_file = fopen("G:/04code/10Qt/byyl-combined6/test/minic/测试2-多个if嵌套和比较运算/sample.minic", "r");
	if (input_file == NULL) {
		printf("Failed to open input file");
		return 1;
	}
	FILE* output_file = fopen("G:/04code/10Qt/byyl-combined6/test/minic/测试2-多个if嵌套和比较运算/sample.lex", "w");
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