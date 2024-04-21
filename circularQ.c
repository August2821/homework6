/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 //자료의 최대 크기 설정

typedef char element; //자료형 char를 element란 이름으로 사용

//구조체 선언
typedef struct {
	element queue[MAX_QUEUE_SIZE];//자료형 element(char) 타입의 queue MAX_QUEUE_SIZE의 배열
	int front, rear; //front: queue의 첫 번째 요소의 위치, rear: queue의 마지막 요소의 위치
}QueueType; //QueueType이란 이름의 구조체 선언


QueueType *createQueue(); //queue 생성 함수
int freeQueue(QueueType *cQ); //QueueType *cQ 동적할당 해제 함수
int isEmpty(QueueType *cQ); //queue가 비어있는지 체크하는 함수
int isFull(QueueType *cQ); //queue가 full인지 체크하는 함수
void enQueue(QueueType *cQ, element item); //queue에 입력한 data를 삽입하는 함수
void deQueue(QueueType *cQ);//queue에 data를 삭제하는 함수
void printQ(QueueType *cQ);//현재 queue에 들어있는 data를 출력해주는 함수
void debugQ(QueueType *cQ); //queue의 상태를 디버깅 하는 함수
element getElement();//queue에 삽입할 data를 받는 함수

int main(void)
{


	QueueType *cQ = createQueue();//createQueue 함수의 리턴 값을 QueueType 타입의 변수 *cQ에 저장
	element data;//자료형 element(char) 타입의 변수
	char command;

	do{
		printf("\n[----- [김민경] [2023041054] -----]\n");
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);//Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q 이것들 중 하나 입력 받음

		switch(command) {
		case 'i': case 'I': //i, I(insert)를 입력하면
			data = getElement(); //data를 입력 받고 data 변수에 저장
			enQueue(cQ, data); //queue에 data 변수 값을 삽입
			break; //멈추고 do while문의 조건으로 이동

		case 'd': case 'D': //d, D(delete)를 입력하면
			deQueue(cQ);//함수 호출
			break; //멈추고 do while문의 조건으로 이동

		case 'p': case 'P': //p, P(printQ)를 입력하면
			printQ(cQ);//함수 호출
			break; //멈추고 do while문의 조건으로 이동

		case 'b': case 'B': //b, B(debug)를 입력하면
			debugQ(cQ);//함수 호출
			break; //멈추고 do while문의 조건으로 이동

		case 'q': case 'Q': //q, Q(quit)를 입력하면
   	        freeQueue(cQ); //함수 호출
			break; //멈추고 do while문의 조건으로 이동

		default:
			//위에 case들이 모두 해당되지 않는다면
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); //출력
			break; //멈추고 do while문의 조건으로 이동
		}

	}while(command != 'q' && command != 'Q'); 
	//q or Q가 입력되면 do while 종료


	return 0;
}

QueueType *createQueue()//QueueType 구조체를 가리키는 포인터를 반환
{
	QueueType *cQ;//QueueType 타입의 변수 *cQ 생성

	//동적 메모리 할당, QueueType 크기 = queue(4byte)+ front(4byte)+rear(4byte)
	cQ = (QueueType *)malloc(sizeof(QueueType));

	//초기화를 안 시키면 쓰레기값이 나와서 queue 초기화 시키기
	for(int i=0;i<MAX_QUEUE_SIZE;i++){
		cQ->queue[i]=0;
	}

	cQ->front = 0;//front의 시작 주소 0
	cQ->rear = 0;//rear 시작 주소 0
	return cQ;//cQ 포인터 리턴
}

int freeQueue(QueueType *cQ)
{
	//NULL 포인터는 이미 해제된 메모리 블록을 가리키므로, 해제하려고 시도하면 오류가 발생할 수 있기 때문에 검사
    if(cQ == NULL) return 1; //cQ가 null인지 검사
    free(cQ); //cQ 동적할당 해제
    return 1;
}

element getElement()
{
	element item; //element(char) 자료형의 변수 item 선언
	printf("Input element = ");
	scanf(" %c", &item); //input 값 입력
	return item; //저장 된 item(input 값)을 리턴
}


/* complete the function */
int isEmpty(QueueType *cQ)
{
	if(cQ->front==cQ->rear){ //fron와 rear가 같다면 queue가 비어있음
		printf("empty\n");
		return 1; //큐가 empty이기 때문에 1 리턴
	}
    else return 0; //큐가 empty가 아니기 때문에 0 리턴
}

/* complete the function */
int isFull(QueueType *cQ)
{
	//(cQ->rear + 1) % MAX_QUEUE_SIZE 연산은 큐의 순환 특성을 고려하여 다음 삽입할 위치를 계산
	//계산된 값이 front와 같다면 queue가 full임
	if(((cQ->rear+1)%MAX_QUEUE_SIZE)==cQ->front){
		printf("full\n");
		return 1; //큐가 full이므로 1 return
	}
   else return 0; //큐가 full이 아니므로 0 return
}


/* complete the function */


void enQueue(QueueType *cQ, element item)
{
	if(isFull(cQ)) return;	//큐가 full이면 함수 종료
	else{
		//queue에서 현재 rear 요소에서 다음 요소를 가리키도록 front 인덱스를 1 증가 시킴
		//%MAX_QUEUE_SIZE는 circular Queue 특성을 고려해 rear 인덱스가 첫 번째 요소로 되돌아 갈 수 있도록 함
		cQ->rear=(cQ->rear+1)%MAX_QUEUE_SIZE;
		
		//추가할 요소 item을 queue의 rear 인덱스에 해당하는 위치에 저장
		cQ->queue[cQ->rear]=item;
	}
}

/* complete the function */
//element item는 사용하지 않아서 삭제
void deQueue(QueueType *cQ)
{
	if(isEmpty(cQ)) return; //큐가 비어 있으면 함수 종료
	else{
		//cQ->front 멤버 변수를 갱신
		//queue에서 현재 front 요소에서 다음 요소를 가리키도록 front 인덱스를 1 증가 시킴
		//%MAX_QUEUE_SIZE는 circular Queue 특성을 고려해 front 인덱스가 첫 번째 요소로 되돌아 갈 수 있도록 함
		cQ->front=(cQ->front+1)%MAX_QUEUE_SIZE;

		//front 값 삭제
		cQ->queue[cQ->front]=0;
	}
}


void printQ(QueueType *cQ)
{
	int i, first, last;

	//queue의 첫 번째 요소의 위치를 나머지 연산으로 구함
	first = (cQ->front + 1)%MAX_QUEUE_SIZE;

	//queue의 마지막 요소의 위치를 나머지 연산으로 구함
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;

	printf("Circular Queue : [");

	i = first;
	while(i != last){
		//queue의 i번째 요소 출력 => 첫 번째 요소부터 마지막 요소까지 모두 출력
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE; //i도 나머지 연산을 해야지 값이 나옴 i++ 하면 안됨

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{
	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) {
			printf("  [%d] = front\n", i); //i와 front가 같으면 출력
			continue;//if문이 ture이면 아래 코드를 실행하지 않고 넘어감
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); //인덱스 i에 해당하는 queue의 요소 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);//queue의 front와 rear의 값 출력
}


