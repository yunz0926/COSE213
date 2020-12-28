#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 65
#define MALLOC(p,s) \
	if (!(p = malloc(s))){\
		fprintf(stderr, "Insufficient memory");\
		exit(EXIT_FAILURE); \
	}
typedef enum {head, entry} tagfield;
typedef struct matrixNode *matrixPointer;
typedef struct {
	int row;
	int col;
	int value;
} entryNode;
typedef struct matrixNode{
	matrixPointer down;
	matrixPointer right;
	tagfield tag;
	union {
		matrixPointer next;
		entryNode entry;
	} u;
};

matrixPointer nodes[MAX_SIZE]; 
//array of H in matrices (H is a node in headnode lists that is recorded numRows and numCols)

void mread();
void mwrite();
void merase();
void madd();
void mmult();
void mtranspose();
matrixPointer insertNode(int row, int col, int value, matrixPointer *last);
matrixPointer newNode();

int main()
{
	int stop = 0;
	
	while (1) //repeat until input is '7'(Exit)
	{
		int input;
		
		system("cls"); //clear console
		printf("1. Read\n2. Write\n3. Erase\n4. Add\n5. Multiply\n6. Transpose\n7. Exit\n");
		scanf("%d", &input);

		switch (input)
		{
		case 1:
			system("cls");
			mread();
			break;
		case 2:
			system("cls");
			mwrite();
			break;
		case 3:
			system("cls");
			merase();
			break;
		case 4:
			system("cls");
			madd();
			break;
		case 5:
			system("cls");
			mmult();
			break;
		case 6:
			system("cls");
			mtranspose();
			break;
		case 7:
			stop = 1;
			break;
		}

		if (stop)
			break;
	}

	printf("\nExit the program\n");

	return;
}
void mread(void)
{
	int numRows, numCols, numTerms, numHeads, i;
	int row, col, value, currentRow, index;
	matrixPointer temp, last, node;
	printf("Save index: ");
	scanf("%d", &index);
	matrixPointer hdnode[MAX_SIZE];
	printf("Enter the number of rows, columns and number of nonzero terms: ");
	scanf("%d%d%d", &numRows, &numCols, &numTerms);
	numHeads = (numCols > numRows) ? numCols : numRows;
	node = newNode();
	node->tag = entry;
	node->u.entry.row = numRows;
	node->u.entry.col = numCols;

	if (!numHeads) node->right = node;
	else {
		for (i = 0; i < numHeads; i++)
		{
			temp = newNode();
			hdnode[i] = temp; hdnode[i]->tag = head;
			hdnode[i]->right = temp; hdnode[i]->u.next = temp;
		}
		currentRow = 0;
		last = hdnode[0];
		for (i = 0; i < numTerms; i++) {
			printf("Enter row, column and value: ");
			scanf("%d%d%d", &row, &col, &value);
			if (row > currentRow) {
				last->right = hdnode[currentRow];
				currentRow = row; last = hdnode[row];
			}
			temp = insertNode(row, col, value, &last);			
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
		}
		last->right = hdnode[currentRow];
		for (i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		for (i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = node;
		node->right = hdnode[0];
	}

	if (nodes[index] != NULL) //if save index is already occupied, free that
		free(nodes[index]);
	nodes[index] = node;
}


void mwrite()
{
	int i, index;
	printf("Print index: ");
	scanf("%d", &index);
	matrixPointer node = nodes[index], temp, head;
	if (!node)
	{
		printf("Cannot print! The index is empty\n");
	}
	else
	{
		head = node->right;
		printf("\n numRows = %d, numCols = %d\n", node->u.entry.row, node->u.entry.col);
		printf(" The matrix by row, column and value: \n\n");
		for (i = 0; i < node->u.entry.row; i++) {
			for (temp = head->right; temp != head; temp = temp->right)
				printf("%5d%5d%5d \n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
			head = head->u.next;
		}
	}

	/*a step before back to Menu(because time to read result would be needed) */
	char key;
	printf("\nPress Enter to back to Menu: ");
	scanf("%c", &key);
	scanf("%c", &key);
}

void merase()
{
	int i, numHeads, index;
	printf("Erase index: ");
	scanf("%d", &index);
	matrixPointer x, y, node = nodes[index], head = node->right;
	
	for (i = 0; i < node->u.entry.row; i++) {
		y = head->right;
		while (y != head) {
			x = y; y = y->right; free(x);
		}
		x = head; head = head->u.next; free(x);
	}
	y = head;
	while (y != node) {
		x = y; y = y->u.next; free(x);
	}
	free(node);
	nodes[index] = NULL;
}

void madd()
{
	int first, second, save, numHeads, row, col, i;
	matrixPointer a, b, d, hdA, hdB, tempA, tempB, temp, last;
	printf("First, second, save index: ");
	scanf("%d%d%d", &first, &second, &save);
	a = nodes[first], b = nodes[second];

	if (a->u.entry.row != b->u.entry.row || a->u.entry.col != b->u.entry.col) //if it impossible to add two matrices
	{
		printf("Error! Cannot add two matrices\n");
		return;
	}
	
	matrixPointer hdnode[MAX_SIZE];

	row = a->u.entry.row;
	col = a->u.entry.col;
	numHeads = (row > col) ? row : col;
	d = newNode();
	d->tag = entry; d->u.entry.row = row; d->u.entry.col = col;
	
	if (!numHeads)
		d->right = d;
	else {
		for (i = 0; i < numHeads; i++)
		{
			temp = newNode();
			hdnode[i] = temp; hdnode[i]->tag = head;
			hdnode[i]->right = temp; hdnode[i]->u.next = temp;
		}
		hdA = a->right;
		hdB = b->right;
		for (i = 0; i < row; i++) {  //
			tempA = hdA->right;
			tempB = hdB->right;
			last = hdnode[i];
			while (tempA != hdA && tempB != hdB)
			{
				if (tempA->u.entry.col < tempB->u.entry.col)  
				{
					//insert A's element 
					temp = insertNode(i, tempA->u.entry.col, tempA->u.entry.value, &last);
					hdnode[temp->u.entry.col]->u.next->down = temp;
					hdnode[temp->u.entry.col]->u.next = temp;
					tempA = tempA->right;
					//move tempA to right index
				}
				else if (tempA->u.entry.col > tempB->u.entry.col)
				{
					//insert B's element
					temp = insertNode(i, tempB->u.entry.col, tempB->u.entry.value, &last);
					hdnode[temp->u.entry.col]->u.next->down = temp;
					hdnode[temp->u.entry.col]->u.next = temp;
					tempB = tempB->right;
					//move tempB to right index
				}
				else //if the same locations of A and B both are not 0
				{
					//insert a element with value of (A's element + B's element)
					temp = insertNode(i, tempA->u.entry.col, tempA->u.entry.value + tempB->u.entry.value, &last);
					hdnode[temp->u.entry.col]->u.next->down = temp;
					hdnode[temp->u.entry.col]->u.next = temp;
					tempA = tempA->right;
					tempB = tempB->right;
				}
			}
			for (; tempA != hdA; tempA = tempA->right) //if there are some leftovers in A
			{
				//insert all of the left overs
				temp = insertNode(i, tempA->u.entry.col, tempA->u.entry.value, &last);
				hdnode[temp->u.entry.col]->u.next->down = temp;
				hdnode[temp->u.entry.col]->u.next = temp;
			}
			for (; tempB != hdB; tempB = tempB->right) //if there are some leftovers in B
			{
				//insert all of the left overs
				temp = insertNode(i, tempB->u.entry.col, tempB->u.entry.value, &last);
				hdnode[temp->u.entry.col]->u.next->down = temp;
				hdnode[temp->u.entry.col]->u.next = temp;
			}
			last->right = hdnode[i];//make a circular list
			hdA = hdA->u.next; 
			hdB = hdB->u.next;
			//move to next row
		}

		for (i = 0; i < col; i++)
			hdnode[i]->u.next->down = hdnode[i];
		for (i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = d;
		d->right = hdnode[0];
	}
	if (nodes[save] != NULL)
		free(nodes[save]);
	nodes[save] = d;
}

void mtranspose()
{
	int trans, save, row, col, numHeads, i;
	matrixPointer a, b, temp, tempA, headA, last;
	matrixPointer hdnode[MAX_SIZE];
	printf("Transpose index, save index: ");
	scanf("%d%d", &trans, &save);
	a = nodes[trans];
	headA = a->right;
	row = a->u.entry.col;
	col = a->u.entry.row;
	numHeads = (row > col) ? row : col;
	b = newNode();
	b->tag = entry; b->u.entry.row = row; b->u.entry.col = col;

	for (i = 0; i < numHeads; i++)
	{
		temp = newNode();
		hdnode[i] = temp; hdnode[i]->tag = head;
		hdnode[i]->right = temp; hdnode[i]->u.next = temp;
	}

	for (i = 0; i < row; i++)
	{
		last = hdnode[i];
		for (tempA = headA->down; tempA != headA; tempA = tempA->down) //Until visit all the elements of H[i] of A vertically
		{
			//insert elements horizontally into H[i] of new matrix
			temp = insertNode(i, tempA->u.entry.row, tempA->u.entry.value, &last);
			hdnode[tempA->u.entry.row]->u.next->down = temp;
			hdnode[tempA->u.entry.row]->u.next = temp;
		}
		last->right = hdnode[i]; //make a circular list
		headA = headA->u.next; //move to next headnode
	}
	for (i = 0; i < col; i++)
	{
		hdnode[i]->u.next->down = hdnode[i];
	}
	for (i = 0; i < numHeads - 1; i++)
		hdnode[i]->u.next = hdnode[i + 1];
	hdnode[numHeads - 1]->u.next = b;
	b->right = hdnode[0];

	if (nodes[save] != NULL)
		free(nodes[save]);
	nodes[save] = b;
}

void mmult()
{
	int first, second, save, numHeads, row, col, sum, i, j;
	matrixPointer a, b, d, temp, tempA, tempB, headA, headB, last;
	matrixPointer hdnode[MAX_SIZE];
	printf("First, second, save index: ");
	scanf("%d%d%d", &first, &second, &save);
	a = nodes[first], b = nodes[second];

	if (a->u.entry.col != b->u.entry.row) //if it is impossible to multiply two matrices
	{
		printf("Error! Cannot multiply two matrices\n");
	}
	row = a->u.entry.row; //
	col = b->u.entry.col;
	numHeads = (row > col) ? row : col;
	d = newNode();
	d->tag = entry; d->u.entry.row = row; d->u.entry.col = col;
	
	if (!numHeads)
		d->right = d;

	for (i = 0; i < numHeads; i++)
	{
		temp = newNode();
		hdnode[i] = temp; hdnode[i]->tag = head;
		hdnode[i]->right = temp; hdnode[i]->u.next = temp;
	}

	headA = a->right;
	for (i = 0; i < row; i++)
	{
		last = hdnode[i];
		headB = b->right; //start from H[0] of B
		for (j = 0; j < col; j++)
		{
			tempA = headA->right; //tempA moves horizontally
			tempB = headB->down; //tempB moves vertically
			sum = 0;
			while(tempA != headA && tempB != headB)
			{

				if (tempA->u.entry.col < tempB->u.entry.row)
					tempA = tempA->right;
				else if (tempA->u.entry.col > tempB->u.entry.row)
					tempB = tempB->down;
				else //if two elements that can be multiply are found 
				{
					//add A[i][k]*B[k][j] to sum for D[i][j]
					sum += (tempA->u.entry.value) * (tempB->u.entry.value);
					tempA = tempA->right;
					tempB = tempB->down;
				}
			}
			if (sum != 0)
			{
				//insert a node of D[i][j]
				temp = insertNode(i, j, sum, &last);
				hdnode[j]->u.next->down = temp;
				hdnode[j]->u.next = temp;
			}
			
			headB = headB->u.next;
		}
		last->right = hdnode[i]; //make a circular list
		headA = headA->u.next;
	}
	for (i = 0; i < col; i++)
		hdnode[i]->u.next->down = hdnode[i];
	for (i = 0; i < numHeads - 1; i++)
		hdnode[i]->u.next = hdnode[i + 1];
	hdnode[numHeads - 1]->u.next = d;
	d->right = hdnode[0];

	if (nodes[save] != NULL)
		free(nodes[save]);
	nodes[save] = d;
}
matrixPointer insertNode(int row, int col, int value, matrixPointer *last) //function for creating and inserting a node right after last
{
	matrixPointer temp;
	MALLOC(temp, sizeof(*temp));
	temp->tag = entry; temp->u.entry.row = row; temp->u.entry.col = col;
	temp->u.entry.value = value;
	(*last)->right = temp; //link new node to the most recent node
	*last = temp; //new node becomes the last node of the list

	return temp;
}
matrixPointer newNode() //malloc and return a new node
{
	matrixPointer temp;
	MALLOC(temp, sizeof(*temp));

	return temp;
}