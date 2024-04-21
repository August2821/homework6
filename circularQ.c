/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 //�ڷ��� �ִ� ũ�� ����

typedef char element; //�ڷ��� char�� element�� �̸����� ���

//����ü ����
typedef struct {
	element queue[MAX_QUEUE_SIZE];//�ڷ��� element(char) Ÿ���� queue MAX_QUEUE_SIZE�� �迭
	int front, rear; //front: queue�� ù ��° ����� ��ġ, rear: queue�� ������ ����� ��ġ
}QueueType; //QueueType�̶� �̸��� ����ü ����


QueueType *createQueue(); //queue ���� �Լ�
int freeQueue(QueueType *cQ); //QueueType *cQ �����Ҵ� ���� �Լ�
int isEmpty(QueueType *cQ); //queue�� ����ִ��� üũ�ϴ� �Լ�
int isFull(QueueType *cQ); //queue�� full���� üũ�ϴ� �Լ�
void enQueue(QueueType *cQ, element item); //queue�� �Է��� data�� �����ϴ� �Լ�
void deQueue(QueueType *cQ);//queue�� data�� �����ϴ� �Լ�
void printQ(QueueType *cQ);//���� queue�� ����ִ� data�� ������ִ� �Լ�
void debugQ(QueueType *cQ); //queue�� ���¸� ����� �ϴ� �Լ�
element getElement();//queue�� ������ data�� �޴� �Լ�

int main(void)
{


	QueueType *cQ = createQueue();//createQueue �Լ��� ���� ���� QueueType Ÿ���� ���� *cQ�� ����
	element data;//�ڷ��� element(char) Ÿ���� ����
	char command;

	do{
		printf("\n[----- [��ΰ�] [2023041054] -----]\n");
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);//Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q �̰͵� �� �ϳ� �Է� ����

		switch(command) {
		case 'i': case 'I': //i, I(insert)�� �Է��ϸ�
			data = getElement(); //data�� �Է� �ް� data ������ ����
			enQueue(cQ, data); //queue�� data ���� ���� ����
			break; //���߰� do while���� �������� �̵�

		case 'd': case 'D': //d, D(delete)�� �Է��ϸ�
			deQueue(cQ);//�Լ� ȣ��
			break; //���߰� do while���� �������� �̵�

		case 'p': case 'P': //p, P(printQ)�� �Է��ϸ�
			printQ(cQ);//�Լ� ȣ��
			break; //���߰� do while���� �������� �̵�

		case 'b': case 'B': //b, B(debug)�� �Է��ϸ�
			debugQ(cQ);//�Լ� ȣ��
			break; //���߰� do while���� �������� �̵�

		case 'q': case 'Q': //q, Q(quit)�� �Է��ϸ�
   	        freeQueue(cQ); //�Լ� ȣ��
			break; //���߰� do while���� �������� �̵�

		default:
			//���� case���� ��� �ش���� �ʴ´ٸ�
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); //���
			break; //���߰� do while���� �������� �̵�
		}

	}while(command != 'q' && command != 'Q'); 
	//q or Q�� �ԷµǸ� do while ����


	return 0;
}

QueueType *createQueue()//QueueType ����ü�� ����Ű�� �����͸� ��ȯ
{
	QueueType *cQ;//QueueType Ÿ���� ���� *cQ ����

	//���� �޸� �Ҵ�, QueueType ũ�� = queue(4byte)+ front(4byte)+rear(4byte)
	cQ = (QueueType *)malloc(sizeof(QueueType));

	//�ʱ�ȭ�� �� ��Ű�� �����Ⱚ�� ���ͼ� queue �ʱ�ȭ ��Ű��
	for(int i=0;i<MAX_QUEUE_SIZE;i++){
		cQ->queue[i]=0;
	}

	cQ->front = 0;//front�� ���� �ּ� 0
	cQ->rear = 0;//rear ���� �ּ� 0
	return cQ;//cQ ������ ����
}

int freeQueue(QueueType *cQ)
{
	//NULL �����ʹ� �̹� ������ �޸� ����� ����Ű�Ƿ�, �����Ϸ��� �õ��ϸ� ������ �߻��� �� �ֱ� ������ �˻�
    if(cQ == NULL) return 1; //cQ�� null���� �˻�
    free(cQ); //cQ �����Ҵ� ����
    return 1;
}

element getElement()
{
	element item; //element(char) �ڷ����� ���� item ����
	printf("Input element = ");
	scanf(" %c", &item); //input �� �Է�
	return item; //���� �� item(input ��)�� ����
}


/* complete the function */
int isEmpty(QueueType *cQ)
{
	if(cQ->front==cQ->rear){ //fron�� rear�� ���ٸ� queue�� �������
		printf("empty\n");
		return 1; //ť�� empty�̱� ������ 1 ����
	}
    else return 0; //ť�� empty�� �ƴϱ� ������ 0 ����
}

/* complete the function */
int isFull(QueueType *cQ)
{
	//(cQ->rear + 1) % MAX_QUEUE_SIZE ������ ť�� ��ȯ Ư���� ����Ͽ� ���� ������ ��ġ�� ���
	//���� ���� front�� ���ٸ� queue�� full��
	if(((cQ->rear+1)%MAX_QUEUE_SIZE)==cQ->front){
		printf("full\n");
		return 1; //ť�� full�̹Ƿ� 1 return
	}
   else return 0; //ť�� full�� �ƴϹǷ� 0 return
}


/* complete the function */


void enQueue(QueueType *cQ, element item)
{
	if(isFull(cQ)) return;	//ť�� full�̸� �Լ� ����
	else{
		//queue���� ���� rear ��ҿ��� ���� ��Ҹ� ����Ű���� front �ε����� 1 ���� ��Ŵ
		//%MAX_QUEUE_SIZE�� circular Queue Ư���� ����� rear �ε����� ù ��° ��ҷ� �ǵ��� �� �� �ֵ��� ��
		cQ->rear=(cQ->rear+1)%MAX_QUEUE_SIZE;
		
		//�߰��� ��� item�� queue�� rear �ε����� �ش��ϴ� ��ġ�� ����
		cQ->queue[cQ->rear]=item;
	}
}

/* complete the function */
//element item�� ������� �ʾƼ� ����
void deQueue(QueueType *cQ)
{
	if(isEmpty(cQ)) return; //ť�� ��� ������ �Լ� ����
	else{
		//cQ->front ��� ������ ����
		//queue���� ���� front ��ҿ��� ���� ��Ҹ� ����Ű���� front �ε����� 1 ���� ��Ŵ
		//%MAX_QUEUE_SIZE�� circular Queue Ư���� ����� front �ε����� ù ��° ��ҷ� �ǵ��� �� �� �ֵ��� ��
		cQ->front=(cQ->front+1)%MAX_QUEUE_SIZE;

		//front �� ����
		cQ->queue[cQ->front]=0;
	}
}


void printQ(QueueType *cQ)
{
	int i, first, last;

	//queue�� ù ��° ����� ��ġ�� ������ �������� ����
	first = (cQ->front + 1)%MAX_QUEUE_SIZE;

	//queue�� ������ ����� ��ġ�� ������ �������� ����
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;

	printf("Circular Queue : [");

	i = first;
	while(i != last){
		//queue�� i��° ��� ��� => ù ��° ��Һ��� ������ ��ұ��� ��� ���
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE; //i�� ������ ������ �ؾ��� ���� ���� i++ �ϸ� �ȵ�

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{
	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) {
			printf("  [%d] = front\n", i); //i�� front�� ������ ���
			continue;//if���� ture�̸� �Ʒ� �ڵ带 �������� �ʰ� �Ѿ
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); //�ε��� i�� �ش��ϴ� queue�� ��� ���

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);//queue�� front�� rear�� �� ���
}


