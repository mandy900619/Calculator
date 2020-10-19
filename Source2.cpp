#include <iostream>

using namespace std;
struct stack {
	bool operatororNot = true;
	char Operator = NULL;
	int data = NULL;
	struct stack* downptr = NULL;
};
typedef struct stack snode;
snode* s1top = NULL;
snode* s2top = NULL;
snode* pushint(int data, snode* sTop) {
	snode* newstack = new snode;
	newstack->data = data;
	newstack->operatororNot = false;
	if (sTop == NULL)
		sTop = newstack;
	else {
		newstack->downptr = sTop;
		sTop = newstack;
	}
	return sTop;
}
snode* pushoperator(char data, snode* sTop) {
	snode* newstack = new snode;
	newstack->Operator = data;
	if (sTop == NULL)
		sTop = newstack;
	else {
		newstack->downptr = sTop;
		sTop = newstack;
	}
	return sTop;
}
snode* pop(snode* sTop) {
	if (sTop->downptr == NULL)
		sTop = NULL;
	else
		sTop = sTop->downptr;
	return sTop;
}
snode* reverseStack(snode* sTop) {
	snode* reversetop = NULL;
	snode* newreverse = new snode;
	snode* list = sTop;
	newreverse->data = list->data;
	newreverse->Operator = list->Operator;
	newreverse->operatororNot = list->operatororNot;
	reversetop = newreverse;
	newreverse->downptr = NULL;
	list = list->downptr;
	while (list != NULL) {
		snode* newreverse = new snode;
		newreverse->data = list->data;
		newreverse->Operator = list->Operator;
		newreverse->operatororNot = list->operatororNot;
		newreverse->downptr = reversetop;
		reversetop = newreverse;
		if (list->downptr == NULL)
			list = NULL;
		else
			list = list->downptr;
	}
	return reversetop;
}
void printpostfix(snode* sTop) {
	if (sTop->operatororNot)
		cout << sTop->Operator;
	else
		cout << sTop->data;
	sTop = sTop->downptr;
	while (sTop != NULL) {
		if (sTop->operatororNot)
			cout << " " << sTop->Operator;
		else
			cout << " " << sTop->data;
		if (sTop->downptr == NULL)
			sTop = NULL;
		else
			sTop = sTop->downptr;
	}
}
bool transformpostfix(char* in, int len) {
	for (int i = 0; i < len; i++) {
		switch (in[i])
		{
		case '(':
			s2top = pushoperator(in[i], s2top);
			break;
		case ')':
			while (s2top != NULL) {
				if (s2top->Operator == '(')
					break;
				char outoperator = s2top->Operator;
				s2top = pop(s2top);
				s1top = pushoperator(outoperator, s1top);
			}
			if (s2top == NULL)
				return false;
			else
				s2top = pop(s2top);
			break;
		case '+':
			if (s2top == NULL) {
				s2top = pushoperator(in[i], s2top);
				break;
			}
			while (s2top->Operator == '*' || s2top->Operator == '+' || s2top->Operator == '-') {
				char outoperator = s2top->Operator;
				s2top = pop(s2top);
				s1top = pushoperator(outoperator, s1top);
				if (s2top == NULL)
					break;
			}
			s2top = pushoperator(in[i], s2top);
			break;
		case '-':
			if (s2top == NULL) {
				s2top = pushoperator(in[i], s2top);
				break;
			}
			while (s2top->Operator == '*' || s2top->Operator == '+' || s2top->Operator == '-') {
				char outoperator = s2top->Operator;
				s2top = pop(s2top);
				s1top = pushoperator(outoperator, s1top);
				if (s2top == NULL)
					break;
			}
			s2top = pushoperator(in[i], s2top);
			break;
		case '*':
			s2top = pushoperator(in[i], s2top);
			break;
		default://¼Æ¦r
			int thenum = (int)in[i] - 48;
			if (i + 1 < len) {
				while (in[i + 1] <= '9' && in[i + 1] >= '0') {
					i++;
					thenum *= 10;
					thenum += ((int)in[i] - 48);
					if (i + 1 >= len)
						break;
				}
			}
			s1top = pushint(thenum, s1top);
			break;
		}
	}
	while (s2top != NULL) {
		char outoperator = s2top->Operator;
		s2top = pop(s2top);
		s1top = pushoperator(outoperator, s1top);
	}
}
int calculate(snode* sTop) {
	int count[70] = {};
	int pos = -1;
	int nowpos = 0;
	int returnvalue = 0;
	while (sTop != NULL) {
		if (sTop->operatororNot) {
			if (pos < 1) {
				return NULL;
			}
			switch (sTop->Operator)
			{
			case'+':
				returnvalue = count[pos - 1] + count[pos];
				break;
			case'-':
				returnvalue = count[pos - 1] - count[pos];
				break;
			case'*':
				returnvalue = count[pos - 1] * count[pos];
				break;
			default:
				return NULL;
			}
			count[pos - 1] = returnvalue;
			count[pos] = NULL;
			pos--;
		}
		else {
			pos++;
			count[pos] = sTop->data;
		}
		if (sTop->downptr == NULL)
			sTop = NULL;
		else
			sTop = sTop->downptr;
	}
	return returnvalue;
}
int lengthof(char* str)
{
	int count = 0;
	while (str[count] != '\0')
		count++;
	return count;
}
int main() {
	char* input = new char[70];
	cin >> input;
	int num = 0;
	num = lengthof(input);
	snode* rever = NULL;
	if (transformpostfix(input, num)) {
		rever = reverseStack(s1top);
		int value = calculate(rever);
		if (value == NULL) {
			cout << "invalid";
		}
		else {
			printpostfix(rever);
			cout << endl << value;
		}
	}
	else
		cout << "invalid";
}