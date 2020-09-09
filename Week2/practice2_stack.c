//Template
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
	int nData;
	struct Node *next;
} Node;

typedef struct Stack
{
	Node *top;
} Stack;

void InitializeStack(Stack *stack);
void Push(Stack *stack, int nData);
int Pop(Stack *stack);

int main(void)
{
	//source code

	Stack stack;
	InitializeStack(&stack);

	int nDatas[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	for (int i = 0; i < 10; i++)
	{
		Push(&stack, nDatas[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		printf("%d\n", Pop(&stack));
	}

	return 0;
}

void InitializeStack(Stack *stack)
{
	//source code
	stack->top = NULL;
}

void Push(Stack *stack, int nData)
{
	//source code
	Node *temp = (Node *)malloc(sizeof(Node));
	temp->nData = nData;
	temp->next = stack->top;
	stack->top = temp;
}

int Pop(Stack *stack)
{
	//source code
	Node *deleteNode;
	int ret;

	deleteNode = stack->top;
	ret = deleteNode->nData;

	stack->top = deleteNode->next;

	free(deleteNode);
	return ret;
}
