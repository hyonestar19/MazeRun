#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <windows.h> // Print Message Box
#define MAX_LENGTH 128
#define MAX_STACK_SIZE 100
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:6011)
#pragma warning(disable:6001)
#pragma warning(disable:28182)

// �� �ະ �� ���� Ȯ�� �Ķ���� �� Maze �߰� �� �� ���� ���� ����
int check_column = 0;

// Maze ���� �Լ�
char Maze[MAX_LENGTH][MAX_LENGTH];


void NoFileErrorCheck(FILE* Check) { // No File Error Check
	if (Check == NULL) {
		MessageBox(NULL, TEXT("Error : No File Detected"), TEXT("Warning!!"), MB_ICONWARNING | MB_OK);
		return 0;
	}
}

int Buff_zero_Check(char buff1) { // No Data in txt check
	if (buff1 == ' ') {
		MessageBox(NULL, TEXT("Error : File Data Error\nData start with blank!!"), TEXT("Warning!!"), MB_ICONWARNING | MB_OK);
		return 0;
	}
}

int Buff_Col_Size_Check(int iter, int col, int dnum) { // Check Each Rows' number of column equals -> If not, break loop and shut down the program
	if (iter != 0 && (col != dnum)) {
		MessageBox(NULL, TEXT("Error : File DAta Error\nEach Rows' Number of Column doesn't equal!!"), TEXT("Warning!!"), MB_ICONWARNING | MB_OK);
		return 0;
	}
}

int Check_Data_Num(char* buff2) { // Find Number of Column
	int s_datanum = 0; // Parameter for saving number of column
	int i;
	for (i = 0;; i++) {
		if (buff2[i] == '1' || buff2[i] == '0' || buff2[i] == 'E' || buff2[i] == 'X') { // IF buff2[i] isn't a space or \0 -> increasing s_datanum
			s_datanum++; // increasing number of column
		}
		else if (buff2[i] == '\0') break; // if buff2 doesn't have data more -> break loop
	}
	return s_datanum; // return Number of Column
}

typedef struct { // ��ǥ�� ��Ÿ�� element r�� row, c�� column
	short r;
	short c;
}element;

typedef struct { // ��ǥ���� ������ ����
	element stack[MAX_STACK_SIZE]; // ��ǥ�� �׾Ƴ��� stack ����
	int top; // stack�� ���� ���� �ε����� -1�� ������ �ִ�.
}StackType;


void init(StackType* s) { // ���� �ʱ�ȭ
	s->top = -1;
}

int is_empty(StackType* s) { // ���� ���� ����
	return (s->top == -1);
}

int is_full(StackType* s) { // ���� �� �� �������� ����
	return (s->top == (MAX_STACK_SIZE - 1));
}

void push(StackType* s, element item) { // ���ÿ� ��� �߰�
	if (is_full(s)) { // ������ �� ���ִٸ� ���� �޽��� ��� �� �ƹ����� ���ϱ�
		MessageBox(NULL, TEXT("Error : Stack OverFlow"), TEXT("Warning!!"), MB_ICONWARNING | MB_OK);
		return;
	}
	else s->stack[++(s->top)] = item; // top�� �ϳ� �÷��ְ� �� �ڸ��� ������ �߰�
}

element pop(StackType* s) { // ������ �������� �� �� ��ȯ �� ���ÿ��� ����
	if (is_empty(s)) { // ������ ��� ������ ���� ��� �� ����
		MessageBox(NULL, TEXT("Error : Stack UnderFlow"), TEXT("Warning!!"), MB_ICONWARNING | MB_OK);
		exit(1);
	}
	else return s->stack[(s->top)--]; // ���� �� ���� ���� ��ȯ�ϰ� top�� �ϳ� ����
}

element peek(StackType* s) { // ������ �������� �� ���� ��ȯ �� ���ÿ��� ����
	if (is_empty(s)) { // ������ ��� ������ ���� ��� �� ����
		MessageBox(NULL, TEXT("Error : No Item in Stack"), TEXT("Warning!!"), MB_ICONWARNING | MB_OK);
		exit(1);
	}
	else return s->stack[s->top]; // ���� �� ���� ���� ��ȯ
}

void pushLoc(StackType* s, int r, int c) { // �� ���������� �����¿� ������ �Ǵ��ϱ�

	if (r < 0 || c < 0) return; // �����̱� �����̴�.
	
	if (Maze[r][c] == '0' || Maze[r][c] == 'X') { // �� �� ������ �ڸ�
		element temp;
		temp.r = r;
		temp.c = c;
		push(s, temp);
	}
}

element FindEntry(int numLine) {
	int fr, fc;
	for (fr = 0; fr < numLine + 1; fr++) {
		for (fc = 0; fc < check_column; fc++) {
			if (Maze[fr][fc] == 'E') {
				element Ent = { fr, fc };
				return Ent;
			}
		}
	}
}


int main() {
	// �ؽ�Ʈ �� �� ���� �б�
	// �ӽ� �޸�
	char buffer[MAX_LENGTH];
	// �� �� ����
	int line_count = 0;


	//input.txt���� �о����
	FILE* input = fopen("input.txt", "r");
	NoFileErrorCheck(input); // input.txt ���� üũ
	

	while (fgets(buffer, MAX_LENGTH, input) != NULL) { // �� �о�� NULL ��ȯ
		int a = Buff_zero_Check(buffer[0]); // Check Data follows standard format
		if (a == 0) break;
		
		int datanum = Check_Data_Num(buffer); // Check Number of Column
		/*printf("datanum = %d\n", datanum);*/ // �ش� ���� �� ���� �ľ�

		if (line_count == 0) check_column = datanum; // When First loop, set Maze's Number of Column to Number of Data

		int b = Buff_Col_Size_Check(line_count, check_column, datanum); // Check Number of Data equals to each rows'
		if (b == 0) break; // IF There's NOT_EQUAL Situation -> break the loop

		int t = 0, i = 0;
		for (i; i < sizeof(buffer); i++) {
			if (buffer[i] == '1' || buffer[i] == '0' || buffer[i] == 'E' || buffer[i] == 'X') { // IF DATA isn't space or NULL -> Saving
				Maze[line_count][t] = buffer[i];
				//printf("%c\n", Maze[line_count][t]); // ����� �ԷµǾ����� Ȯ��
				t++;
			}
			else if (buffer[i] == '\0') {
				Maze[line_count][t] = '\0';
				break;
			}
		}

		line_count++;
	}
	Maze[line_count][0] = '\0';
	fclose(input);
	line_count--;
	// Maze Ȯ�� �ڵ�
	/*printf("line_count : %d\n", line_count);
	int j;
	int k;
	for (j = 0; j <= line_count; j++) {
		for (k = 0; k < check_column; k++) {
			printf("[%d][%d] : %c\n", j, k, Maze[j][k]);
		}
	}*/

	/* Saving Maze at output.txt*/
	FILE* output = fopen("output.txt", "w");
	// Original Set of Maze
	int fpr, fpc;
	for (fpr = 0; fpr < line_count + 1; fpr++) {
		for (fpc = 0; fpc < check_column; fpc++) {
			if (Maze[fpr][fpc] == '1') { // �� means Wall
				fprintf(output, "��");
			}
			else if (Maze[fpr][fpc] == '0') {
				fprintf(output, " ");
			}
			else {
				fprintf(output, "%c", Maze[fpr][fpc]);
			}
		}
		fprintf(output, "\n");
	}
	fprintf(output, "\n");
	fclose(output);



	// Ž�� �ڵ� ����
	int r, c; // Each parameter means row and column
	StackType st; // Generated Stack

	init(&st); // Initialized Stack

	// Find Entry point
	element entry = FindEntry(line_count);
	// Print out Entry Point in CMD
	printf("���� (%d,%d) -> ", entry.r, entry.c);
	element here = entry; // Set Start Point in Maze
	while (Maze[here.r][here.c] != 'X') { // While we arrive Exit Point
		r = here.r;
		c = here.c;
		Maze[r][c] = '.';
		pushLoc(&st, r - 1, c); // stacking Location Upper one if condition satisfied
		pushLoc(&st, r + 1, c); // stacking Location Downward one if condition satisfied 
		pushLoc(&st, r, c - 1); // stacking Location left one if condition satisfied
		pushLoc(&st, r, c + 1); // stacking Location right one if condition satisfied

		if (is_empty(&st)) { // Stack is empty before we arrived,
			printf("����"); // print out "Fail"
			FILE* output2 = fopen("output.txt", "a");
			int rpr, rpc;
			fprintf(output2, "Ž�����");
			for (rpr = 0; rpr < line_count + 1; rpr++) {
				fputs("\n", output2);
				for (rpc = 0; rpc < check_column; rpc++) {
					if (Maze[rpr][rpc] == '1') { // �� means Wall
						fprintf(output2, "��");
					}
					else if (Maze[rpr][rpc] == '0') {
						fprintf(output2, " ");
					}
					else if (Maze[rpr][rpc] == '.') {
						fprintf(output2, "��");
					}
					else {
						fprintf(output2, "%c", Maze[rpr][rpc]);
					}
				}
			}
			fclose(output2);
			return 0; // Exit Main Func. -> Shutting down the program.
		}
		else { // In procedure,
			here = pop(&st); // Go to next step (Primary Order : Right -> Left -> Downward -> Upward)
			printf("(%d,%d) -> ", here.r, here.c); // print out next step's location
		}

	}
	printf("����\nŽ������"); // If we arrived, print out "Arrived" and "Search Successful"
	FILE* output2 = fopen("output.txt", "a");
	int rpr, rpc;
	fprintf(output2, "Ž�����");
	for (rpr = 0; rpr < line_count + 1; rpr++) {
		fputs("\n", output2);
		for (rpc = 0; rpc < check_column; rpc++) {
			if (Maze[rpr][rpc] == '1') { // �� means Wall
				fprintf(output2, "��");
			}
			else if (Maze[rpr][rpc] == '0') {
				fprintf(output2, " ");
			}
			else if (Maze[rpr][rpc] == '.') {
				fprintf(output2, "��");
			}
			else {
				fprintf(output2, "%c", Maze[rpr][rpc]);
			}
		}
	}
	fclose(output2);
	return 0; // Exit Main Func. -> Shutting Down the program.
}