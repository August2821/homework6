/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();//중위 표현식을 입력받는 함수
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();//중위 표현식을 후위 표현식으로 변환하는 함수
void debug();//현재 상태를 디버깅하는 함수
void reset();//모든 변수 초기화하는 함수
void evaluation();//후위 표현식을 계산하고 결과를 저장하는 함수

int main()
{
	char command;

	//q or Q가 입력되면 do while문 종료
	do{
		printf("\n[----- [김민경] [2023041054] -----]\n");
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);//Infix=i,Postfix=p,Eval=e,Debug=d,Reset=r,Quit=q 중 하나 입력 받음

		//command 값에 따라 함수 호출
		switch(command) {
		case 'i': case 'I':
			getInfix(); //중위 표현식 입력받는 함수 호출
			break;
		case 'p': case 'P':
			toPostfix(); //후위 표현식으로 변환하는 함수 호출
			break;
		case 'e': case 'E':
			evaluation(); //후위 표현식을 계산하고 결과를 evalResult에 저장하는 함수 호출
			break;
		case 'd': case 'D':
			debug();//현재 상태를 디버깅하는 함수 호출
			break;
		case 'r': case 'R':
			reset();//현재 상태를 초기화하는 함수 호출
			break;
		case 'q': case 'Q':
			break;
		default://위에 case가 아닌 것이 입력되면 아래 printf 출력
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

//x값을 postfixStackTop의 값을 1 증가 시킨 후 postfixStack에 값을 넣음
void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
}

char postfixPop()
{
    char x;

	//postfixStackTop이 -1이라면 postfixStack이 비어있기 때문에 null 리턴
    if(postfixStackTop == -1)
        return '\0';
    else {
	//postfixStackTop == -1이 아니라면 postfixStack[postfixStackTop]을 x에 넣은 후 postfixStackTop 값 -1
    	x = postfixStack[postfixStackTop--];
    }
	//x값 return, 스택에서 pop은 스택에서 값을 꺼내는 것이기 때문에 return하는 것
    return x;
	//값을 꺼냈기 때문에 마지막 요소를 가리키는 top은 -1을 해줘야 함
}

//x값을 evalStackTop의 값을 1 증가 시킨 후 evalStack에 값을 넣음
void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()
{
	//evalStackTop이 -1이라면 evalStack이 비어있기 때문에 -1 리턴
    if(evalStackTop == -1)
        return -1;
	//evalStackTop이 -1이 아니라면 evalStack[evalStackTop]을 x에 넣은 후 evalStackTop의 값 -1
    else
        return evalStack[evalStackTop--];
	//값을 꺼냈기 때문에 마지막 요소를 가리키는 top은 -1을 해줘야 함
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); //중위 표현식 입력 받음
}

//문자 symbol이 어떤 종류의 토큰인지 판단하여 연산자의 우선순위를 나타내는 precedence 값을 반환
precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

//문자 x에 대한 우선순위 값을 getToken함수를 호출하여 반환함
precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
	//만약 '\0'이라면 postfixExp이 비어있음
		strncpy(postfixExp, c, 1);
		//strncpy 함수를 사용하여 c를 postfixExp의 첫 번째 문자로 저장
	else
		strncat(postfixExp, c, 1);
		//strncat 함수를 사용하여 c를 postfixExp의 마지막 문자 뒤에 추가
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		/* 필요한 로직 완성 */
		switch(*exp) {
            case '(':
                postfixPush(*exp);//postfixPush 함수를 사용하여 '('를 postfixStack에 저장
                exp++;//exp 포인터를 1 증가시켜 다음 문자를 읽을 수 있게 함
                break;
            case ')':
				//postfixStack이 비어있는 경우, postfixStack의 맨 위에 있는 연산자의 우선순위가 '('보다 낮은지 확인
                while (postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) != lparen) {
                    //postfixPop 함수를 사용하여 postfixStack에서 연산자를 하나씩 꺼내 postfixExp에 저장
					char temp = postfixPop();
                    charCat(&temp);
                }
				//postfixStack이 비어있는 경우 error 출력하고 함수 종료
                if (postfixStackTop == -1) {
                    printf("error\n");
                    return;
                }
                postfixPop();
                exp++;//exp 포인터를 1 증가시켜 다음 문자를 읽을 수 있게 함
                break;
            case '+':
            case '-':
            case '*':
            case '/':
			//case가 +,-,*,/면 아래 코드 실행
				//postfixStack이 비어있지 않은 경우, postfixStack의 맨 위에 있는 연산자의 우선순위가 현재 읽은 연산자의 우선순위보다 크거나 같은 경우
                while (postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) {
                    char temp = postfixPop();//postfixStack에서 마지막 연산자를 팝하여 temp 변수에 저장
                    charCat(&temp);//charCat 함수를 사용하여 temp 변수에 저장된 연산자를 postfixExp 문자열에 추가
                }
                postfixPush(*exp);//현재 읽은 연산자 (*exp)를 postfixStack에 저장
                exp++;//exp 포인터를 1 증가시켜 다음 문자를 읽을 수 있게 함
                break;
            default:
				//숫자라면
                while (*exp >= '0' && *exp <= '9') {
                    charCat(&*exp);//charCat 함수를 사용하여 현재 읽은 숫자를 postfixExp 문자열에 추가
                    exp++;//exp 포인터를 1 증가시켜 다음 문자를 읽을 수 있게 함
                }
                break;
        }
	}

	/* 필요한 로직 완성 */
	// postfix 스택에 남아있는 연산자들을 postfixExp에 추가
    while (postfixStackTop != -1) {
        char temp = postfixPop();
        charCat(&temp);
    }
}

void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
	/* postfixExp, evalStack을 이용한 계산 */
    char *exp = postfixExp;
    int x, y,num=0;

    // postfixExp를 순회하면서 계산을 수행
    while (*exp != '\0') {
        switch (*exp) {
            case '+':
                y = evalPop();//숫자를 꺼냄
                x = evalPop();//숫자를 꺼냄
                evalPush(x + y);//계산된 값을 evalStack에 저장
                break;
            case '-':
                y = evalPop();//숫자를 꺼냄
                x = evalPop();//숫자를 꺼냄
                evalPush(x - y);//계산된 값을 evalStack에 저장
                break;
            case '*':
                y = evalPop();//숫자를 꺼냄
                x = evalPop();//숫자를 꺼냄
                evalPush(x * y);//계산된 값을 evalStack에 저장
                break;
            case '/':
                y = evalPop();//숫자를 꺼냄
                x = evalPop();//숫자를 꺼냄
                evalPush(x / y);//계산된 값을 evalStack에 저장
                break;
            default:
                // 숫자인 경우
                num=*exp;
                num = num - '0';//ASCII 코드 값에서 '0'을 빼서 숫자 값으로 변환
                evalPush(num);//변환된 숫자를 evalPush() 함수를 사용하여 evalStack에 저장
                break;
        }
        exp++;//exp 포인터를 1 증가시켜 다음 문자를 읽을 수 있게 함
    }

    // evalStack에 남아있는 마지막 값이 계산 결과
    evalResult = evalPop();
}
