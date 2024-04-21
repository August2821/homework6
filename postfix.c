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

/* stack ������ �켱����, lparen = 0 ���� ���� */
typedef enum{
	lparen = 0,  /* ( ���� ��ȣ */
	rparen = 9,  /* ) ������ ��ȣ*/
	times = 7,   /* * ���� */
	divide = 6,  /* / ������ */
	plus = 5,    /* + ���� */
	minus = 4,   /* - ���� */
	operand = 1 /* �ǿ����� */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression�� �����ϴ� �迭 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix�� ����� ���ڿ��� �����ϴ� �迭 */
char postfixStack[MAX_STACK_SIZE];	/* postfix�� ��ȯ�� ���� �ʿ��� ���� */
int evalStack[MAX_STACK_SIZE];		/* ����� ���� �ʿ��� ���� */

int postfixStackTop = -1;  /* postfixStack�� top */
int evalStackTop = -1;	   /* evalStack�� top */

int evalResult = 0;	   /* ��� ����� ���� */

void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();//���� ǥ������ �Է¹޴� �Լ�
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();//���� ǥ������ ���� ǥ�������� ��ȯ�ϴ� �Լ�
void debug();//���� ���¸� ������ϴ� �Լ�
void reset();//��� ���� �ʱ�ȭ�ϴ� �Լ�
void evaluation();//���� ǥ������ ����ϰ� ����� �����ϴ� �Լ�

int main()
{
	char command;

	//q or Q�� �ԷµǸ� do while�� ����
	do{
		printf("\n[----- [��ΰ�] [2023041054] -----]\n");
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);//Infix=i,Postfix=p,Eval=e,Debug=d,Reset=r,Quit=q �� �ϳ� �Է� ����

		//command ���� ���� �Լ� ȣ��
		switch(command) {
		case 'i': case 'I':
			getInfix(); //���� ǥ���� �Է¹޴� �Լ� ȣ��
			break;
		case 'p': case 'P':
			toPostfix(); //���� ǥ�������� ��ȯ�ϴ� �Լ� ȣ��
			break;
		case 'e': case 'E':
			evaluation(); //���� ǥ������ ����ϰ� ����� evalResult�� �����ϴ� �Լ� ȣ��
			break;
		case 'd': case 'D':
			debug();//���� ���¸� ������ϴ� �Լ� ȣ��
			break;
		case 'r': case 'R':
			reset();//���� ���¸� �ʱ�ȭ�ϴ� �Լ� ȣ��
			break;
		case 'q': case 'Q':
			break;
		default://���� case�� �ƴ� ���� �ԷµǸ� �Ʒ� printf ���
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

//x���� postfixStackTop�� ���� 1 ���� ��Ų �� postfixStack�� ���� ����
void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
}

char postfixPop()
{
    char x;

	//postfixStackTop�� -1�̶�� postfixStack�� ����ֱ� ������ null ����
    if(postfixStackTop == -1)
        return '\0';
    else {
	//postfixStackTop == -1�� �ƴ϶�� postfixStack[postfixStackTop]�� x�� ���� �� postfixStackTop �� -1
    	x = postfixStack[postfixStackTop--];
    }
	//x�� return, ���ÿ��� pop�� ���ÿ��� ���� ������ ���̱� ������ return�ϴ� ��
    return x;
	//���� ���±� ������ ������ ��Ҹ� ����Ű�� top�� -1�� ����� ��
}

//x���� evalStackTop�� ���� 1 ���� ��Ų �� evalStack�� ���� ����
void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()
{
	//evalStackTop�� -1�̶�� evalStack�� ����ֱ� ������ -1 ����
    if(evalStackTop == -1)
        return -1;
	//evalStackTop�� -1�� �ƴ϶�� evalStack[evalStackTop]�� x�� ���� �� evalStackTop�� �� -1
    else
        return evalStack[evalStackTop--];
	//���� ���±� ������ ������ ��Ҹ� ����Ű�� top�� -1�� ����� ��
}

/**
 * infix expression�� �Է¹޴´�.
 * infixExp���� �Էµ� ���� �����Ѵ�.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); //���� ǥ���� �Է� ����
}

//���� symbol�� � ������ ��ū���� �Ǵ��Ͽ� �������� �켱������ ��Ÿ���� precedence ���� ��ȯ
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

//���� x�� ���� �켱���� ���� getToken�Լ��� ȣ���Ͽ� ��ȯ��
precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * �����ϳ��� ���޹޾�, postfixExp�� �߰�
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
	//���� '\0'�̶�� postfixExp�� �������
		strncpy(postfixExp, c, 1);
		//strncpy �Լ��� ����Ͽ� c�� postfixExp�� ù ��° ���ڷ� ����
	else
		strncat(postfixExp, c, 1);
		//strncat �Լ��� ����Ͽ� c�� postfixExp�� ������ ���� �ڿ� �߰�
}

/**
 * infixExp�� ���ڸ� �ϳ��� �о�鼭 stack�� �̿��Ͽ� postfix�� �����Ѵ�.
 * ����� postfix�� postFixExp�� ����ȴ�.
 */
void toPostfix()
{
	/* infixExp�� ���� �ϳ����� �б����� ������ */
	char *exp = infixExp;
	char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */

	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	while(*exp != '\0')
	{
		/* �ʿ��� ���� �ϼ� */
		switch(*exp) {
            case '(':
                postfixPush(*exp);//postfixPush �Լ��� ����Ͽ� '('�� postfixStack�� ����
                exp++;//exp �����͸� 1 �������� ���� ���ڸ� ���� �� �ְ� ��
                break;
            case ')':
				//postfixStack�� ����ִ� ���, postfixStack�� �� ���� �ִ� �������� �켱������ '('���� ������ Ȯ��
                while (postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) != lparen) {
                    //postfixPop �Լ��� ����Ͽ� postfixStack���� �����ڸ� �ϳ��� ���� postfixExp�� ����
					char temp = postfixPop();
                    charCat(&temp);
                }
				//postfixStack�� ����ִ� ��� error ����ϰ� �Լ� ����
                if (postfixStackTop == -1) {
                    printf("error\n");
                    return;
                }
                postfixPop();
                exp++;//exp �����͸� 1 �������� ���� ���ڸ� ���� �� �ְ� ��
                break;
            case '+':
            case '-':
            case '*':
            case '/':
			//case�� +,-,*,/�� �Ʒ� �ڵ� ����
				//postfixStack�� ������� ���� ���, postfixStack�� �� ���� �ִ� �������� �켱������ ���� ���� �������� �켱�������� ũ�ų� ���� ���
                while (postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) {
                    char temp = postfixPop();//postfixStack���� ������ �����ڸ� ���Ͽ� temp ������ ����
                    charCat(&temp);//charCat �Լ��� ����Ͽ� temp ������ ����� �����ڸ� postfixExp ���ڿ��� �߰�
                }
                postfixPush(*exp);//���� ���� ������ (*exp)�� postfixStack�� ����
                exp++;//exp �����͸� 1 �������� ���� ���ڸ� ���� �� �ְ� ��
                break;
            default:
				//���ڶ��
                while (*exp >= '0' && *exp <= '9') {
                    charCat(&*exp);//charCat �Լ��� ����Ͽ� ���� ���� ���ڸ� postfixExp ���ڿ��� �߰�
                    exp++;//exp �����͸� 1 �������� ���� ���ڸ� ���� �� �ְ� ��
                }
                break;
        }
	}

	/* �ʿ��� ���� �ϼ� */
	// postfix ���ÿ� �����ִ� �����ڵ��� postfixExp�� �߰�
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
	/* postfixExp, evalStack�� �̿��� ��� */
    char *exp = postfixExp;
    int x, y,num=0;

    // postfixExp�� ��ȸ�ϸ鼭 ����� ����
    while (*exp != '\0') {
        switch (*exp) {
            case '+':
                y = evalPop();//���ڸ� ����
                x = evalPop();//���ڸ� ����
                evalPush(x + y);//���� ���� evalStack�� ����
                break;
            case '-':
                y = evalPop();//���ڸ� ����
                x = evalPop();//���ڸ� ����
                evalPush(x - y);//���� ���� evalStack�� ����
                break;
            case '*':
                y = evalPop();//���ڸ� ����
                x = evalPop();//���ڸ� ����
                evalPush(x * y);//���� ���� evalStack�� ����
                break;
            case '/':
                y = evalPop();//���ڸ� ����
                x = evalPop();//���ڸ� ����
                evalPush(x / y);//���� ���� evalStack�� ����
                break;
            default:
                // ������ ���
                num=*exp;
                num = num - '0';//ASCII �ڵ� ������ '0'�� ���� ���� ������ ��ȯ
                evalPush(num);//��ȯ�� ���ڸ� evalPush() �Լ��� ����Ͽ� evalStack�� ����
                break;
        }
        exp++;//exp �����͸� 1 �������� ���� ���ڸ� ���� �� �ְ� ��
    }

    // evalStack�� �����ִ� ������ ���� ��� ���
    evalResult = evalPop();
}
