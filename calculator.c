
/*****************************************************************************
 * Copyright (C) Aditya Neralkar neralkarad15.it@coep.ac.in
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "stack.h"
#include <math.h>
  
int lflag = 0, dflag = 0, stackfull = 0, error = 0, ope = 0, pwr = 0;

/*reads line from the terminal and converts it to a string*/
int readline(char *arr, int len) {
	int i = 0;
	int ch;
	while((ch = getchar()) != '\n' && i < len - 1) {
		arr[i] = ch;
		i++;
	}
	arr[i] = '\0';
	return i;
}

#define OPERAND 10 
#define OPERATOR 20
#define VARIABLE 30
#define	END	40
#define ERROR	50


typedef struct token {
	int type;
	num number;
	char op;
}token;

num variable[46];


void initvar() {
	int i = 0;
	for(i = 0; i < 46; i++) {
		variable[i] = initnum(variable[i]);
		variable[i].bd[0] = '0';
		variable[i].bi = 1;
	}
}


enum states { SPC, DIG, OPR, ALPH, STOP, ERR };

/*converts string into tokens*/
token *getnext(char *arr, int *reset) {
	static int currstate = SPC;
	int nextstate;
	int d1flag = 0; 
	static int i = 0;
	if(*reset == 1) {
		i = 0;
		currstate = SPC;
		*reset = 0;
	}
	num a;
	a = initnum(a);
	char operator(char*);
	int x, y;
	char *expre, *opr;
	static char optr = '\0';
	token *t = (token *)malloc(sizeof(token));
	if(currstate == ALPH) {
		expre = (char *)malloc(sizeof(char) * 9);
		x = 0;
		*(expre + x) = arr[i - 1];
		x++;
	}
	if(currstate == OPR) {
		opr = (char *)malloc(sizeof(char) * 4);
		y = 0;
		*(opr + y) = arr[i - 1];
		y++;
	}
	if(arr[i - 1] == '.') {
		dflag = 1;
		d1flag = 1;
	}
	if(currstate == DIG) {
		if(d1flag == 1) {
			a.ad[a.ai] = arr[i - 1];
			a.ai++;
		}
		else {
			a.bd[a.bi] = arr[i - 1];
			a.bi++;
		}
	}
	
	while(1) {
		switch(arr[i]) {
			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9': case '.':
				nextstate = DIG;
				break;
			case 'a': case 'b': case 'c': case 'd':
			case 'e': case 'f': case 'g': case 'h':
			case 'l': case 'k': case 'j': case 'i':
			case 'm': case 'n': case 'o': case 'p':
			case 't': case 's': case 'r': case 'q':
			case 'u': case 'v': case 'w': case 'x':
			case 'y': case 'z': case 'G': case 'H':
			case 'L': case 'K': case 'J': case 'I':
			case 'M': case 'N': case 'O': case 'P':
			case 'T': case 'S': case 'R': case 'Q':
			case 'U': case 'V': case 'W': case 'X':
			case 'Y': case 'Z':
				nextstate = ALPH;
				break;
			case '+': case '-': case '*': case '/':
			case '%': case '(': case ')': case '^':
			case '&': case '|': case '>': case '<': 
			case '=':
				nextstate = OPR;
				break;
			case '\0':
				nextstate = STOP;
				break;
			case ' ':
				nextstate = SPC;
				break;
			default:
				if((optr == 'J' || optr == 'K' || optr == 'L') && arr[i] >= 'A' && arr[i] <= 'F') {
					nextstate = DIG;
					break;
				}
				else {
					nextstate = ERR;
					break;
				}
		}
		switch(currstate) {
			case SPC:
				if(arr[i] == '.') {
					dflag = 1;
					d1flag = 1;
					break;
				}
				if(nextstate == DIG && d1flag != 1) {
					a.bd[a.bi] = arr[i];
					a.bi++;
				}
				else if(nextstate == DIG && d1flag == 1) {
					a.ad[a.ai] = arr[i];
					a.ai++;
				}
				else if(nextstate == ALPH) {
					x = 0;
					expre = (char *)malloc(sizeof(char) * 9);
					*(expre + x) = arr[i];
					x++;
				}		
				else if(nextstate == OPR) {
					y = 0;
					opr = (char *)malloc(sizeof(char) * 4);
					*(opr + y) = arr[i];
					y++;
				}		
				break;
			case DIG:
				if(arr[i] == '.') {
					dflag = 1;
					d1flag = 1;
					break;
				}
				if(nextstate == DIG && d1flag != 1) {
					a.bd[a.bi] = arr[i];
					a.bi++;
					if(a.bi == a.blimit)
						a = breinitnum(a);
				}
				else if(nextstate == DIG && d1flag == 1) {
					a.ad[a.ai] = arr[i];
					a.ai++;
					if(a.ai == a.alimit)
						a = areinitnum(a);
				}	
				else  {
					if(a.bi == 0) {
						a.bd[a.bi] = '0';
						a.bi++;
					}
					a.bd[a.bi] = '\0';
					a.ad[a.ai] = '\0';
					t->type = OPERAND;
					t->number = a;
					i++;
					currstate = nextstate;
					return t;
				}
				break;
			case ALPH:
				if(nextstate == ALPH) {
					*(expre + x) = arr[i];
					x++;
				}
				else {
					*(expre + x) = '\0';
					t->type = OPERATOR;
					t->op = operator(expre);
					optr = t->op;
					if(t->op == '1') {
						if(strlen(expre) != 1)
							t->type = ERROR;
						else {
							t->type = OPERAND;
							a.bd[0] = '0';
							a.bi = 1;
							a.v = expre[0];
							a.var = 1;
							t->number = a;
						}
					}
					free(expre);
					i++;
					currstate = nextstate;
					return t;
				}
				break;
			case OPR:
				if(nextstate == OPR) {
					*(opr + y) = arr[i];
					y++;
				}
				else {
					*(opr + y) = '\0';
					t->type = OPERATOR;
					t->op = operator(opr);
					optr = t->op;
					if(t->op == '1') {
						t->type = ERROR;
					}
					free(opr);
					currstate = nextstate;
					i++;
					return t;
				}
				break;
			case STOP:
				t->type = END;
				currstate = nextstate;
				return t;
				break;
			case ERR:
				t->type = ERROR;
				currstate = nextstate;
				return t;
				break;
		}
		currstate = nextstate;
		i++;
	}
}

/*assigns unique character to each operation
 *these characters are later used as operators 
 */
char operator (char *expr) {
	if(lflag == 1) {
		if(strcmp(expr, "sin") == 0)
			return 'a';
		else if(strcmp(expr, "cos") == 0)
			return 'b';
		else if(strcmp(expr, "tan") == 0)
			return 'c';
		else if(strcmp(expr, "cot") == 0)
			return 'd';		
		else if(strcmp(expr, "sec") == 0)
			return 'e';
		else if(strcmp(expr, "cosec") == 0)
			return 'f';
		else if(strcmp(expr, "log") == 0)
			return 'g';		
		else if(strcmp(expr, "ln") == 0)
			return 'h';		
		else if(strcmp(expr, "exp") == 0)
			return 'i';
	}		
	if(strcmp(expr, "sqrt") == 0)
		return 'j';		
	else if(strcmp(expr, "cbrt") == 0)
		return 'k';		
	else if(strcmp(expr, "&&") == 0)
		return 'l';						
	else if(strcmp(expr, "||") == 0)
		return 'm';
	else if(strcmp(expr, ">") == 0)
		return 'n';
	else if(strcmp(expr, "<") == 0)
		return 'o';				
	else if(strcmp(expr, ">=") == 0)
		return 'p';
	else if(strcmp(expr, "<=") == 0)
		return 'q';
	else if(strcmp(expr, "=") == 0)
		return 'r';
	else if(strcmp(expr, "==") == 0)
		return 's';
	else if(strcmp(expr, "!") == 0)
		return 't';
	else if(strcmp(expr, "floor") == 0)
		return 'u';
	else if(strcmp(expr, "ceiling") == 0)
		return 'v';
	else if(strcmp(expr, "bo") == 0)
		return 'A';		
	else if(strcmp(expr, "bd") == 0)
		return 'B';						
	else if(strcmp(expr, "bh") == 0)
		return 'C';
	else if(strcmp(expr, "ob") == 0)
		return 'D';
	else if(strcmp(expr, "od") == 0)
		return 'E';				
	else if(strcmp(expr, "oh") == 0)
		return 'F';
	else if(strcmp(expr, "db") == 0)
		return 'G';
	else if(strcmp(expr, "do") == 0)
		return 'H';
	else if(strcmp(expr, "dh") == 0)
		return 'I';
	else if(strcmp(expr, "hb") == 0)
		return 'J';
	else if(strcmp(expr, "ho") == 0)
		return 'K';
	else if(strcmp(expr, "hd") == 0)
		return 'L';
	else if(strcmp(expr, "+") == 0)
		return '+';
	else if(strcmp(expr, "-") == 0)
		return '-';
	else if(strcmp(expr, "/") == 0)
		return '/';
	else if(strcmp(expr, "*") == 0)
		return '*';
	else if(strcmp(expr, "^") == 0)
		return '^';
	else if(strcmp(expr, "%") == 0)
		return '%';
	else if(strcmp(expr, "(") == 0)
		return '(';
	else if(strcmp(expr, ")") == 0)
		return ')';
	else if(strcmp(expr, "++") == 0)
		return 'M';
	else if(strcmp(expr, "--") == 0)
		return 'N';
	else if(strcmp(expr, "+=") == 0)
		return 'O';
	else if(strcmp(expr, "-=") == 0)
		return 'P';
	else if(strcmp(expr, "*=") == 0)
		return 'Q';
	else if(strcmp(expr, "/=") == 0)
		return 'R';
	else
		return '1';
}

/*returns precedence of operator*/
int precedence(char b) {
	switch(b) {
		case 'l': case 'm': case 'n': case 'o':
		case 'p': case 'r': case 'q': case 's':
			return -1;
			break;
		case '(':
			return 4;
			break;
		case ')':
			return 0;
			break;
		case '+': case '-':
			return 1;
			break;
		case '*': case '/': case '%': case '^':
			return 2;
			break;
		case 'a': case 'b': case 'c': case 'd':
		case 'e': case 'f': case 'g': case 'h':
		case 'k': case 'j': case 'i': case 't':	
		case 'A': case 'B': case 'C': case 'D':
		case 'E': case 'F': case 'G': case 'H':
		case 'I': case 'J': case 'K': case 'L':
		case 'S': case 'T': case 'U': case 'V':
		case 'u': case 'v':
			return 3;
			break;
	}
	return -2;
}

num add(num, num);
num sub(num, num);
num mul(num, num);
num divi(num, num);
num mod(num, num);

/*returns variable of n * pi i.e. 3.14 * 10^28*/
num getpi(num t) {
	t.bd[0] = '3';
	t.bd[1] = '1';
	t.bd[2] = '4';
	t.bd[3] = '1';
	t.bd[4] = '5';
	t.bd[5] = '9';
	t.bd[6] = '2';
	t.bd[7] = '6';
	t.bd[8] = '5';
	t.bd[9] = '3';
	t.bd[10] = '5';
	t.bd[11] = '8';
	t.bd[12] = '9';
	t.bd[13] = '7';
	t.bd[14] = '9';
	t.bd[15] = '3';
	t.bd[16] = '2';
	t.bd[17] = '3';
	t.bd[18] = '8';
	t.bd[19] = '4';
	t.bd[20] = '6';
	t.bd[21] = '2';
	t.bd[22] = '6';
	t.bd[23] = '4';
	t.bd[24] = '3';
	t.bd[25] = '3';
	t.bd[26] = '8';
	t.bd[27] = '3';
	t.bd[28] = '2';
	t.bd[29] = '\0';
	t.bi = 30;
	return t;
}

/*adds 'i' zeros before given number
 *e.g. adz(15, 2) returns 0015
 */
num adz(num t, int i) {
	if(i == 0)
		return t; 
	int j = t.bi;
	while(i + t.bi >= t.blimit)
		t = breinitnum(t);
	while(j > -1) {
		t.bd[j + i] = t.bd[j];
		j--;
	}
	j = 0;
	t.bi += i; 
	while(j < i)
		t.bd[j++] = '0';
	return t;
}

/*adds 'i' zeros at end of given number
 *e.g. adza(15.23, 2) returns 15.2300
 */
num adza(num t, int i) {
	int j = 0;
	while(i + t.ai >= t.alimit)
		t = areinitnum(t);
	while(j < i)
		t.ad[t.ai + j++] = '0';
	t.ad[t.ai + i] = '\0';
	t.ai += i;
	return t;
}

/*removes zeros before given number
 *e.g. adz(000015) returns 15
 */
num rmz(num t) {
	int i = 0, j = 0;
	while(t.bd[i] == '0')
		i++;
	if(i == 0)
		return t;
	while(t.bd[i + j - 1] != '\0') {
		t.bd[j] = t.bd[i + j];
		j++;
	}
	t.bi = j - 1;
	return t;
}

/*removes zeros after given number
 *e.g. adz(15.9800) returns 15.98
 */
num rmza(num t) {
	int i = t.ai - 1, j = 0;
	while(t.ad[i--] == '0')
		j++;
	if(j == 0)
		return t;
	t.ad[t.ai - j] = '\0';
	t.ai -= j;
	return t;
}

num carryclr(num res, int k) {
	int m;
	while(k > 0) {
		if(res.bd[k] > '9') {
			m = res.bd[k] - 48;
			res.bd[k] = (m % 10) + 48;
			res.bd[k - 1] += m / 10;
		}
	k--;
	}
	return res;
}

num add(num x, num y) {
	if(x.sign != y.sign) {
		if(x.sign == 1) {
			x.sign = 0;
			return sub(x, y);
		}
		else {
			y.sign = 0;
			return sub(y, x);
		}
	}
	int i = 0, flag = 0;
	num res;
	res = initnum(res);
	if(x.ai >= y.ai) {
		res = adza(res, x.ai);
		while(i < y.ai) {
			res.ad[i] = x.ad[i] + y.ad[i] - '0';
			i++;
		}
	}
	else {
		res = adza(res, y.ai);
		while(i < x.ai) {
			res.ad[i] = x.ad[i] + y.ad[i] - '0';
			i++;
		}	
	}
	i--;
	while(i > 0) {
		if(res.ad[i] > '9') {
			res.ad[i - 1]++;
			res.ad[i] -= 10;
		}
		i--;
	}
	if(x.bi > y.bi) {
		res = adz(res, x.bi + 1);
		i = x.bi - 1;
		y = adz(y, x.bi - y.bi);
	}
	else {
		res = adz(res, y.bi + 1);
		i = y.bi - 1;
		x = adz(x, y.bi - x.bi);
	}
	while(i > -1) {
		res.bd[i + 1] = x.bd[i] + y.bd[i] - '0';
		if(flag == 1) {
			res.bd[i + 1]++;
			flag = 0;
		}
		if(res.bd[i + 1] > '9') {
			flag = 1;
			res.bd[i + 1] -= 10;
		}
		i--;
	}
	
	if(flag == 1) {
		res.bd[i + 1] = '1';
		flag = 0;
	}
	if(res.ad[0] > '9') {
		res.bd[res.bi - 1] += 1;
		res.ad[0] -= 10;
	}
	res = rmz(res);
	res = rmza(res);
	if(pwr == 0 && ope == 0) {
		free(x.bd);
		free(x.ad);
		free(y.bd);
		free(y.ad);
	}
	return res;
}
num sub(num x, num y) {
	if(x.sign != y.sign) {
		if(x.sign == 1) {
			x.sign = 0;
			return add(x, y);
		}
		else {
			x.sign = 1;
			return add(x, y);
		}	
	}
	num res;
	res = initnum(res);
	x = rmz(x);
	y = rmz(y);
	int i = 0, bflag = 0, flag = 0;
	i = 0;
	if(x.bi > y.bi)
		flag = 2;
	else if(y.bi > x.bi)
		flag = 1;
	else {
		while(i < x.bi) {
			if(x.bd[i] > y.bd[i]) {
				flag = 2;
					break;
			}
			else if(x.bd[i] < y.bd[i]) {
				flag = 1;
				break;
			}
			i++;
		}
		if(flag == 0) {
			i = 0;
			while(i < x.ai) {
				if(x.ad[i] > y.ad[i]) {
					flag = 2;
					break;
				}
				else if(x.ad[i] < y.ad[i]) {
					flag = 1;
					break;
				}
				i++;
			}	
		}
	}
	i = x.ai - y.ai;
	if(i > 0)
		y = adza(y, i);
	else
		x = adza(x, i * -1);
	res = adza(res, y.ai);
	i = y.ai - 1;
	if(flag == 0) {
		res.bi = 1;
		res.bd[0] = '0';
		res.bd[1] = '\0';
	}
	else if(flag == 1) {				/*y > x*/
		while(i > -1) {
			res.ad[i] = y.ad[i] - x.ad[i] + '0';
			if(bflag == 1)
				res.ad[i]--;
			bflag = 0;
			if(res.ad[i] < '0') {
				bflag = 1;
				res.ad[i] += 10;
			}
			i--;
		}
		res = rmza(res);
		x = adz(x, y.bi - x.bi);
		res = adz(res, y.bi);
		i = y.bi - 1;
		while(i > -1) {
			res.bd[i] = y.bd[i] - x.bd[i] + '0';
			if(bflag == 1)
				res.bd[i]--;
			bflag = 0;
			if(res.bd[i] < '0') {
				bflag = 1;
				res.bd[i] += 10;
			}
			i--;
		}
	}
	else {							/*x > y*/
		while(i > -1) {
			res.ad[i] = x.ad[i] - y.ad[i] + '0';
			if(bflag == 1)
				res.ad[i]--;
			bflag = 0;
			if(res.ad[i] < '0') {
				bflag = 1;
				res.ad[i] += 10;
			}
			i--;
		}
		res = rmza(res);
		i = x.bi - 1;
		res = adz(res, x.bi);
		y = adz(y, x.bi - y.bi);
		while(i > -1) {
			res.bd[i] = x.bd[i] - y.bd[i] + '0';
			if(bflag == 1)
				res.bd[i]--;
			bflag = 0;
			if(res.bd[i] < '0') {
				bflag = 1;
				res.bd[i] += 10;
			}
			i--;
		}
		res.sign = 1;
	}
	res = rmz(res);
	if(pwr == 0 && ope == 0) {
		free(y.bd);
		free(y.ad);
		free(x.bd);
		free(x.ad);
	}
	return res;
}

num mul(num x, num y) {
	num res;
	res = initnum(res);
	int i = 0, j = 0, m;
	strcat(x.bd, x.ad);
	strcat(y.bd, y.ad);
	if(x.bi + x.ai > y.bi + y.ai)
		y = adz(y, x.bi + x.ai - y.ai - y.bi);
	else if(x.bi + x.ai < y.bi + y.ai)
		x = adz(x, y.bi + y.ai - x.bi - x.ai);
	res = adz(res, x.bi + x.ai + y.ai + y.bi);
	while(i < y.bi + y.ai) {
		j = 0;
		while(j < x.bi + x.ai) {
			m = (x.bd[j] - '0') * (y.bd[i] - '0');
			res.bd[i + j + 1] += m % 10;
			res.bd[i + j] += m / 10;
			if(((i + 1) * j) % 15 == 0)
				res = carryclr(res, i + j);
			j++;
		}
		i++;
	}
	i = x.bi + y.bi + x.ai + y.ai - 1;
	res = carryclr(res, i);
	res.ai = x.ai + y.ai;
	res.bi -= res.ai;
	i = res.bi;
	j = 0;
	while(j < res.ai) {
		res.ad[j] = res.bd[res.bi + j];
		j++;
	}
	res.ad[res.ai] = '\0';
	res.bd[res.bi] = '\0';
	if(x.sign == y.sign)
		res.sign = 0;
	else
		res.sign = 1;
	res = rmza(res);
	if(pwr == 0) {	
		free(x.bd);
		free(x.ad);
		free(y.bd);
		free(y.ad);
	}
	return rmz(res);
}
num divi(num x, num y) {
	num q, r, b;
	q = initnum(q);
	r = initnum(r);
	b = initnum(b);
	strcat(x.bd, x.ad);
	strcat(y.bd, y.ad);
	if(x.sign == y.sign)
		q.sign = 0;
	else
		q.sign = 1;
	x.sign = y.sign = 0;
	x.bi += x.ai;
	y.bi += y.ai;
	q.ai = y.ai - x.ai;
	x.ai = y.ai = 0;
	x.ad[0] = y.ad[0] = '\0';
	ope = 1;
	int i = 0, j;
	while(i < y.bi) {
		j = -1;
		r.bd[r.bi] = y.bd[i];
		r.bd[r.bi + 1] = '\0';
		r.bi ++;
		strcpy(b.bd, r.bd);
		strcpy(b.ad, r.ad);
		b.bi = r.bi;
		b.ai = r.ai;b.sign = r.sign;
		while(b.sign == 0) {
			x = rmz(x);
			b = sub(x, b);
			j++;
		}
		r = add(x, b);
		q.bd[i] = j + 48;
		i++;
	}
	q.bd[i] = '\0';
	q.bi = i;
	r = rmz(r);
	if(lflag == 1 && r.bi != 0) {
		i = 0;
		while(i < 7) {
			j = -1;
			r.bd[r.bi] = '0';
			r.bd[r.bi + 1] = '\0';
			r.bi ++;
			strcpy(b.bd, r.bd);
			strcpy(b.ad, r.ad);
			b.bi = r.bi;
			b.ai = r.ai;b.sign = r.sign;
			while(b.sign == 0) {
				b = sub(x, b);
				j++;
			}
			r = add(x, b);;
			q.bd[q.bi + i] = j + 48;
			i++;
		}
		q.bi += i;
		q.bd[q.bi] = '\0';
		q.ai += 7;
	}
	q.bi -= q.ai;
	if(lflag == 0) {
		q.ai = 0;
	}
	else {
		j = 0;
		while(j < q.ai) {
			q.ad[j] = q.bd[q.bi + j];
			j++;
		}
	}
	q.ad[q.ai] = '\0';
	q.bd[q.bi] = '\0';
	ope = 0;
	q = rmza(q);
	free(r.bd);
	free(r.ad);
	free(b.bd);
	free(b.ad);
	if(pwr == 0) {
		free(y.bd);
		free(y.ad);
		free(x.bd);
		free(x.ad);
	}
	return rmz(q);
}
num mod(num x, num y) {
	num r, b;
	r = initnum(r);
	b = initnum(b);
	int i = 0, j, k;
	if(x.ai > y.ai) {
		k = x.ai;
		j = x.ai - y.ai;
		y = adza(y, j);
	}
	else if(x.ai < y.ai) {
		k = y.ai;
		j = y.ai - x.ai;
		x = adza(x, j);
	}
	else {
		k = j = x.ai;
	}
	strcat(x.bd, x.ad);
	strcat(y.bd, y.ad);
	x.bi += x.ai;
	y.bi += y.ai;
	x.ai = y.ai = 0;
	x.ad[0] = y.ad[0] = '\0';
	ope = 1;	
	while(i < y.bi) {
		j = -1;
		r.bd[r.bi] = y.bd[i];
		r.bd[r.bi + 1] = '\0';
		r.bi ++;
		b = r;
		while(b.sign == 0) {
			b = sub(x, b);
			j++;
		}
		r = add(x, b);
		i++;
	}
	r.bi -= k;
	j = 0;
	while(j < k) {
		r.ad[j] = r.bd[r.bi + j];
		j++;
	}
	ope = 0;
	r.ai = k;
	r.ad[r.ai] = '\0';
	r.bd[r.bi] = '\0';
	r = rmza(r);
	free(b.bd);
	free(b.ad);
	free(y.bd);
	free(y.ad);
	free(x.bd);
	free(x.ad);
	return rmz(r);
}

/*does all operations*/
num solve(char op, num x, num y) {
	int flag = 0, i = 0, j = 0;
	char str[30], c;
	str[0] = '.';
	str[1] = '\0';
	num res, t;
	long double p = 0;
	if(op != 'r' && y.var == 1) {
		if(y.v >= 'a' && y.v <= 'z') {
			strcpy(y.bd, variable[y.v - 'a'].bd);
			y.bi = variable[y.v - 'a'].bi;
			y.sign = variable[y.v - 'a'].sign;
			strcpy(y.ad, variable[y.v - 'a'].ad);
			y.ai = variable[y.v - 'a'].ai;
			y.alimit = variable[y.v - 'a'].alimit;
			y.blimit = variable[y.v - 'a'].blimit;
		}
		else if(y.v >= 'G' && y.v <= 'Z') {
			strcpy(y.bd, variable[y.v - 'G' + 26].bd);
			y.bi = variable[y.v - 'G' + 26].bi;
			y.sign = variable[y.v - 'G' + 26].sign;
			strcpy(y.ad, variable[y.v - 'G' + 26].ad);
			y.ai = variable[y.v - 'G' + 26].ai;
			y.alimit = variable[y.v - 'G' + 26].alimit;
			y.blimit = variable[y.v - 'G' + 26].blimit;
		}
		else
			error = 1;
	}
	if(x.var == 1) {
		if(x.v >= 'a' && x.v <= 'z') {
			strcpy(x.bd, variable[x.v - 'a'].bd);
			x.bi = variable[x.v - 'a'].bi;
			x.sign = variable[x.v - 'a'].sign;
			strcpy(x.ad, variable[x.v - 'a'].ad);
			x.ai = variable[x.v - 'a'].ai;
			x.alimit = variable[x.v - 'a'].alimit;
			x.blimit = variable[x.v - 'a'].blimit;
		}
		else if(x.v >= 'G' && x.v <= 'Z') {
			strcpy(x.bd, variable[x.v - 'G' + 26].bd);
			x.bi = variable[x.v - 'G' + 26].bi;
			x.sign = variable[x.v - 'G' + 26].sign;
			strcpy(x.ad, variable[x.v - 'G' + 26].ad);
			x.ai = variable[x.v - 'G' + 26].ai;
			x.alimit = variable[x.v - 'G' + 26].alimit;
			x.blimit = variable[x.v - 'G' + 26].blimit;
		}
		else
			error = 1;
	}
	switch(op) {
		case '+':
			return add(x, y);
			break;
		case '-':
			return sub(x, y);
			break;
		case '*':
			return mul(x, y);
			break;
		case '/':
			x = rmz(x);
			if(x.bi == 0 && x.ai == 0) {
				printf("division by 0 \n");
				error = 1;
				free(x.bd);
				free(x.ad);
				return y;
			}
			else
				return divi(x, y);
			break;
		case '%':
			x = rmz(x);
			if(x.bi == 0 && x.ai == 0) {
				printf("mod by 0 \n");
				error = 1;
				free(y.bd);
				free(y.ad);
				return x;
			}
			else
				return mod(x, y);
			break;
		case '^':
			x = rmz(x);
			x = rmza(x);
			if(x.ai != 0) {
				printf("exponent must be integer");
				error = 1;
				free(y.bd);
				free(y.ad);
				return x;
			}
			else if(x.bi == 0) {
				t = initnum(t);
				t.bi = 1;
				t.bd[0] = '1';
				t.bd[1] = '\0';
				free(y.bd);
				free(y.ad);
				free(x.bd);
				free(x.ad);
				return t;
			}
			else if(x.bi == 1 && x.bd[0] == '1' && x.sign == 0) {
				free(x.bd);
				free(x.ad);
				return y;
			}
			else {
				t = initnum(t);
				t.bi = 1;
				t.bd[0] = '2';
				t.bd[1] = '\0';
				pwr = 1;
				if(x.sign == 0) {
					res = y;
					x.bd[x.bi - 1]++;
					x = sub(t, x);
					while(x.bi > 0) {
						if(x.bd[x.bi - 1] % 2 == 1)
							res = mul(res, y);
						x = divi(t, x);
						y = mul(y, y);
						x = rmz(x);y = rmz(y);
					}
				}
				else {
					res = t;
					while(x.bi > 0) {
						res = divi(y, res);
						x = sub(t, x);
						x = rmz(x);
					}
				}
			}
			pwr = 0;
			free(t.bd);
			free(t.ad);
			free(x.bd);
			free(x.ad);
			free(y.bd);
			free(y.ad);
			return res;
			break;			
		case 'a': case 'b':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			if(x.bi > 38) {
				t = getpi(t);
				while(x.bi > 38) {
					x = sub(t, x);
					x = rmz(x);
				}
				t.bi = 1;
				t.bd[0] = 1;
				t.bd[1] = '\0';
			}
			strcat(str, x.ad);
			p = atof(x.bd) + atof(str);
			switch(op) {
				case 'a':
					p = sin(p);
					break;
				case 'b':
					p = cos(p);
					break;
			}
			i = 0;
			if(p < 0) {
				t.sign = 1;
				p *= -1;
			}
			if(((int)p)) {
				free(x.bd);
				free(x.ad);
				return t;
			}
			while(i < 6) {
				t.ad[i] = 48 + (int)(p * 10);
				p = (p * 10) -(int)(p * 10);
				i++;
			}
			t.ad[i] = '\0';
			t.ai = 6;
			t.bd[0] = '\0';
			t.bi = 0;
			free(x.bd);
			free(x.ad);
			return t;
			break;
		case 'c': case 'd': case 'e': case 'f':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			if(x.bi > 38) {
				t = getpi(t);
				while(x.bi > 38) {
					x = sub(t, x);
					x = rmz(x);
				}
				t.bi = 1;
				t.bd[0] = 1;
				t.bd[1] = '\0';
			}
			strcat(str, x.ad);
			p = atof(x.bd) + atof(str);
			switch(op) {
				case 'c':
					p = tan(p);
					break;
				case 'd':
					p = 1 / tan(p);
					break;
				case 'e':
					p = 1 / cos(p);
					break;
				case 'f':
					p = 1 / sin(p);
					break;
			}
			i = 0;
			if(p < 0) {
				t.sign = 1;
				p *= -1;
			}
			while(p >= 1) {
				p = p / 10;
				i++;
			}
			t.bd[i] = '\0';
			t.bi = i;
			j = 0;
			while(i > j) {
				t.bd[j] = 48 + (int)(p * 10);
				p = (p * 10) -(int)(p * 10);
				j++;
			}
			i = 0;
			while(i < 6) {
				t.ad[i] = 48 + (int)(p * 10);
				p = (p * 10) -(int)(p * 10);
				i++;
			}
			t.ad[i] = '\0';
			t.ai = 6;
			free(x.bd);
			free(x.ad);
			return t;
			break;
		case 'g': case 'h': case 'i':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			strcat(str, x.ad);
			p = atof(x.bd) + atof(str);
			if(op == 'g')
				p = logl(p) / log(10);
			else if(op == 'h')
				p = logl(p);
			else
				p = expl(p);
			i = 0;
			if(p < 0) {
				t.sign = 1;
				p *= -1;
			}
			while(p >= 1) {
				p = p / 10;
				i++;
			}
			t.bd[i] = '\0';
			t.bi = i;
			j = 0;
			while(i > j) {
				t.bd[j] = 48 + (int)(p * 10);
				p = (p * 10) -(int)(p * 10);
				j++;
			}
			i = 0;
			while(i < 6) {
				t.ad[i] = 48 + (int)(p * 10);
				p = (p * 10) -(int)(p * 10);
				i++;
			}
			t.ad[i] = '\0';
			t.ai = 6;
			free(x.ad);
			free(x.bd);
			return t;
			break;
		case 'j': case 'k':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			if(x.sign == 1 && op == 'j') {
				error = 1;
				printf("squareroot of negative number is not real\n");
				return x;
			}
			strcat(str, x.ad);
			p = atof(x.bd) + atof(str);
			if(op == 'j')
				p = sqrt(p);
			else
				p = cbrt(p);
			i = 0;
			if(p < 0) {
				t.sign = 1;
				p *= -1;
			}
			while(p >= 1) {
				p = p / 10;
				i++;
			}
			t.bd[i] = '\0';
			t.bi = i;
			j = 0;
			while(i > j) {
				t.bd[j] = 48 + (int)(p * 10);
				p = (p * 10) -(int)(p * 10);
				j++;
			}
			i = 0;
			while(i < 6) {
				t.ad[i] = 48 + (int)(p * 10);
				p = (p * 10) -(int)(p * 10);
				i++;
			}
			t.ad[i] = '\0';
			t.ai = 6;
			free(x.bd);
			free(x.ad);
			return t;
			break;
		case 'l': case 'm':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			x = rmz(x);
			x = rmza(x);
			y = rmz(y);
			y = rmza(y);
			if(op == 'l') {
				if(!((x.ai + x.bi) && (y.ai + y.bi)))
					t.bd[0] = '0';
			}
			else
				if(!((x.ai + x.bi) || (y.ai + y.bi)))
					t.bd[0] = '0';
			free(x.bd);
			free(x.ad);
			return t;
			break;
		case 'n': case 'p':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			x = rmz(x);
			y = rmz(y);
			if(x.bi > y.bi)
				t.bd[0] = '0';
			else if(x.bi == y.bi) {
				while(i < x.bi) {
					if(x.bd[i] > y.bd[i]) {
						flag = 2;
						t.bd[0] = '0';
						break;
					}
					else if(x.bd[i] < y.bd[i]) {
						flag = 1;
						break;
					}
					i++;
				}
				if(flag == 0) {
					i = 0;
					while((i < x.ai) || (i < y.ai)) {
						if(x.ad[i] > y.ad[i]) {
							flag = 2;
							t.bd[0] = '0';
							break;
						}
						else if(x.ad[i] < y.ad[i]) {
							flag = 1;
							break;
						}
						i++;
					}	
				}
				if(op == 'n' && flag == 0)
					t.bd[0] = '0';
			}
			free(x.bd);
			free(x.ad);
			return t;
			break;
		case 'o': case 'q':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			x = rmz(x);
			y = rmz(y);
			if(x.bi < y.bi)
				t.bd[0] = '0';
			else if(x.bi == y.bi) {
				while(i < x.bi) {
					if(x.bd[i] > y.bd[i]) {
						flag = 2;
						break;
					}
					else if(x.bd[i] < y.bd[i]) {
						flag = 1;
						t.bd[0] = '0';
						break;
					}
					i++;
				}
				if(flag == 0) {
					i = 0;
					while(i < x.ai ) {
						if(x.ad[i] > y.ad[i]) {
							flag = 2;
							break;
						}
						else if(x.ad[i] < y.ad[i]) {
							flag = 1;
							t.bd[0] = '0';
							break;
						}
						i++;
					}	
				}
				if((flag == 0) && (op == 'o'))
					t.bd[0] = '0';
			}
			free(x.bd);
			free(x.ad);
			return t;
			break;
		case 'r':
			if(y.var == 0)
				error = 1;
			else {
				if(y.v >= 'a' && y.v <= 'z') {
					strcpy(variable[y.v - 'a'].bd, x.bd);
					variable[y.v - 'a'].bi = x.bi;
					variable[y.v - 'a'].sign = x.sign;
					strcpy(variable[y.v - 'a'].ad, x.ad);
					variable[y.v - 'a'].ai = x.ai;
					variable[y.v - 'a'].alimit = x.alimit;
					variable[y.v - 'a'].blimit = x.blimit;
				}
				else if(y.v >= 'G' && y.v <= 'Z') {
					strcpy(variable[y.v - 'G' + 26].bd, x.bd);
					variable[y.v - 'G' + 26].bi = x.bi;
					variable[y.v - 'G' + 26].sign = x.sign;
					strcpy(variable[y.v - 'G' + 26].ad, x.ad);
					variable[y.v - 'G' + 26].ai = x.ai;
					variable[y.v - 'G' + 26].alimit = x.alimit;
					variable[y.v - 'G' + 26].blimit = x.blimit;
				}
			}
			return x;// '=' optr
			break;
		case 's': case 't':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			x = rmz(x);
			y = rmz(y);
			x = rmza(x);
			y = rmza(y);
			if(op == 't') {
				if(x.bi != 0 || x.ai != 0)
					t.bd[0] = '0';
				free(x.bd);
				free(x.ad);
				return t;
			}
			if((x.bi != y.bi) || (x.ai != y.ai)) {
				t.bd[0] = '0';
				return t;
			}
			while(i < x.bi) {
				if(x.bd[i] != y.bd[i]) {
					t.bd[0] = '0';
					return t;
				}
				i++;
			}
			while(i < x.ai) {
				if(x.ad[i] != y.ad[i]) {
					t.bd[0] = '0';
					return t;
				}
				i++;
			}
			free(x.bd);
			free(x.ad);
			return t;
			break;
		case 'u':case 'v':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			if(x.sign == 1) {
				if(op == 'u')
					op = 'v';
				else
					op = 'u';
			}
			if(op == 'v' && x.ai != 0)
				x = add(x, t);
			x.ai = 0;
			x.ad[0] = '\0';
			return x;
			break;
		case 'A':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			i = x.bi % 3;
			if(i)
				x = adz(x, 3 - i);
			i = 0;
			while(i < x.bi) {
				t.bd[i / 3] = 48;
				if(x.bd[i] == '1')
					t.bd[i / 3] += 4;
				else if(x.bd[i] != '0')
					error = 1;
				if(x.bd[i + 1] == '1')
					t.bd[i / 3] += 2;
				else if(x.bd[i + 1] != '0')
					error = 1;
				if(x.bd[i + 2] == '1')
					t.bd[i / 3] += 1;
				else if(x.bd[i + 2] != '0')
					error = 1;
				i += 3;
			}
			t.bd[i / 3] = '\0';
			t.bi = i / 3;
			if(lflag == 1) {
				i = x.ai % 3;
				if(i)
					x = adza(x, 3 - i);
				i = 0;
				while(i < x.ai) {
					t.ad[i / 3] = 48;
					if(x.ad[i] == '1')
						t.ad[i / 3] += 4;
					else if(x.ad[i] != '0')
						error = 1;
					if(x.ad[i + 1] == '1')
						t.ad[i / 3] += 2;
					else if(x.ad[i + 1] != '0')
						error = 1;
					if(x.ad[i + 2] == '1')
						t.ad[i / 3] += 1;
					else if(x.ad[i + 2] != '0')
						error = 1;
					i += 3;
				}
				t.ad[i / 3] = '\0';
				t.ai = i / 3;
			}
			free(x.bd);
			free(x.ad);
			return t;/*bo*/
			break;
		case 'C':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			i = x.bi % 4;
			if(i)
				x = adz(x, 4 - i);
			i = 0;
			while(i < x.bi) {
				t.bd[i / 4] = 48;
				if(x.bd[i] == '1')
					t.bd[i / 4] += 8;
				else if(x.bd[i] != '0')
					error = 1;
				if(x.bd[i + 1] == '1')
					t.bd[i / 4] += 4;
				else if(x.bd[i + 1] != '0')
					error = 1;
				if(x.bd[i + 2] == '1')
					t.bd[i / 4] += 2;
				else if(x.bd[i + 2] != '0')
					error = 1;
				if(x.bd[i + 3] == '1')
					t.bd[i / 4] += 1;
				else if(x.bd[i + 3] != '0')
					error = 1;
				if(t.bd[i / 4] > '9')
					t.bd[i / 4] += 7;
				i += 4;
			}
			t.bd[i / 4] = '\0';
			t.bi = i / 4;
			if(lflag == 1) {
				i = x.ai % 4;
				if(i)
					x = adza(x, 4 - i);
				i = 0;
				while(i < x.ai) {
					t.ad[i / 4] = 48;
					if(x.ad[i] == '1')
						t.ad[i / 4] += 8;
					else if(x.ad[i] != '0')
						error = 1;
					if(x.ad[i + 1] == '1')
						t.ad[i / 4] += 4;
					else if(x.ad[i + 1] != '0')
						error = 1;
					if(x.ad[i + 2] == '1')
						t.ad[i / 4] += 2;
					else if(x.ad[i + 2] != '0')
						error = 1;
					if(x.ad[i + 3] == '1')
						t.ad[i / 4] += 1;
					else if(x.ad[i + 3] != '0')
						error = 1;
					if(t.ad[i / 4] > '9')
						t.ad[i / 4] += 7;
					i += 4;
				}
				t.ad[i / 4] = '\0';
				t.ai = i / 4;
			}
			free(x.bd);
			free(x.ad);
			return t;/*bh*/
			break;
		case 'D':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			t.bd[0] = '\0';
			while(i < x.bi) {
				switch(x.bd[i]) {
					case '0':
						strcat(t.bd, "000");
						break;
					case '1':
						strcat(t.bd, "001");
						break;
					case '2':
						strcat(t.bd, "010");
						break;
					case '3':
						strcat(t.bd, "011");
						break;
					case '4':
						strcat(t.bd, "100");
						break;
					case '5':
						strcat(t.bd, "101");
						break;
					case '6':
						strcat(t.bd, "110");
						break;
					case '7':
						strcat(t.bd, "111");
						break;
					default :
						error = 1;
						return x;
						break;
				}
				i++;
			}
			t.bi = 3 * x.bi;
			if(lflag == 1) {
				t.ad[0] = '\0';
				while(i < x.ai) {
					switch(x.ad[i]) {
						case '0':
							strcat(t.ad, "000");
							break;
						case '1':
							strcat(t.ad, "001");
							break;
						case '2':
						strcat(t.ad, "010");
							break;
						case '3':
							strcat(t.ad, "011");
							break;
						case '4':
							strcat(t.ad, "100");
							break;
						case '5':
							strcat(t.ad, "101");
							break;
						case '6':
							strcat(t.ad, "110");
							break;
						case '7':
							strcat(t.ad, "111");
							break;
						default :
							error = 1;
							return x;
							break;
					}
					i++;
				}
				t.ai = 3 * x.ai;
			}
			free(x.bd);
			free(x.ad);
			return t;/*ob*/
			break;
		case 'E':case 'B':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			if(op == 'B')
				j = 2;
			else
				j = 8;
			while(i < x.bi) {
				if(x.bd[i] >= '0' && x.bd[i] < ('0' + j))
					p += ((pow(j, x.bi - i - 1)) * (x.bd[i] - 48));
				else {
					error = 1;
					return x;
				}
				i++;
			}
			if(lflag == 1) {
				i = 0;
				while(i < x.ai) {
					if(x.ad[i] >= '0' && x.ad[i] <= ('0' + j))
						p += ((pow(j, -(1 + i))) * (x.ad[i] - 48));
					else {
						error = 1;
						return x;
					}
					i++;
				}
			}
			i = 0;
			while(p >= 1) {
				p = p / 10;
				i++;
			}
			t.bd[i] = '\0';
			t.bi = i;
			j = 0;
			while(i > j) {
				t.bd[j] = 48 + ((int)(p * 10) % 10);
				p = p * 10;
				j++;
			}
			i = 0;
			while(i < 6) {
				t.ad[i] = 48 + ((int)(p * 10) % 10);
				p = p * 10;
				i++;
			}
			t.ai = 6;
			t.ad[i] = '\0';
			free(x.bd);
			free(x.ad);
			return t;/*od & bd*/
			break;
		case 'F':
			x = solve('D', x, x);
			x = solve('C', x, x);
			return x;/*oh*/
			break;
		case 'G':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			p = atof(x.bd);
			while(p) {
				t.bd[i] = ((int)p % 2) + 48;
				i++;
				p = (int)(p / 2);
			}
			t.bi = i;
			t.bd[i] = '\0';
			i--;
			while(i != j) {
				c = t.bd[i];
				t.bd[i] = t.bd[j];
				t.bd[j] = c;
				i--;
				if(i == j)
					break;
				j++;
			}
			free(x.bd);
			free(x.ad);
			return t;/*db*/
			break;
		case 'H':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			p = atof(x.bd);
			while(p) {
				t.bd[i] = ((int)p % 8) + 48;
				i++;
				p = (int)(p / 8);
			}
			t.bi = i;
			t.bd[i] = '\0';
			i--;
			while(i != j) {
				c = t.bd[i];
				t.bd[i] = t.bd[j];
				t.bd[j] = c;
				i--;
				if(i == j)
					break;
				j++;
			}
			free(x.bd);
			free(x.ad);
			return t;/*do*/
			break;
		case 'I':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			p = atof(x.bd);
			while(p) {
				t.bd[i] = ((int)p % 16) + 48;
				if(t.bd[i] > '9')
					t.bd[i] += 'A' - '9' - 1;
				i++;
				p = (int)(p / 16);
			}
			t.bi = i;
			t.bd[i] = '\0';
			i--;
			while(i != j) {
				c = t.bd[i];
				t.bd[i] = t.bd[j];
				t.bd[j] = c;
				i--;
				if(i == j)
					break;
				j++;
			}
			free(x.bd);
			free(x.ad);
			return t;/*dh*/
			break;
		case 'J':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			t.bd[0] = '\0';
			while(i < x.bi) {
				switch(x.bd[i]) {
					case '0':
						strcat(t.bd, "0000");
						break;
					case '1':
						strcat(t.bd, "0001");
						break;
					case '2':
						strcat(t.bd, "0010");
						break;
					case '3':
						strcat(t.bd, "0011");
						break;
					case '4':
						strcat(t.bd, "0100");
						break;
					case '5':
						strcat(t.bd, "0101");
						break;
					case '6':
						strcat(t.bd, "0110");
						break;
					case '7':
						strcat(t.bd, "0111");
						break;
					case '8':
						strcat(t.bd, "1000");
						break;
					case '9':
						strcat(t.bd, "1001");
						break;
					case 'A':
						strcat(t.bd, "1010");
						break;
					case 'B':
						strcat(t.bd, "1011");
						break;
					case 'C':
						strcat(t.bd, "1100");
						break;
					case 'D':
						strcat(t.bd, "1101");
						break;
					case 'E':
						strcat(t.bd, "1110");
						break;
					case 'F':
						strcat(t.bd, "1111");
						break;
					default :
						error = 1;
						break;
				}
				i++;
			}
			t.bi = 4 * x.bi;
			if(lflag == 1) {
				t.ad[0] = '\0';
				while(i < x.ai) {
					switch(x.bd[i]) {
						case '0':
							strcat(t.ad, "0000");
							break;
						case '1':
							strcat(t.ad, "0001");
							break;
						case '2':
							strcat(t.ad, "0010");
							break;
						case '3':
							strcat(t.ad, "0011");
							break;
						case '4':
							strcat(t.ad, "0100");
							break;
						case '5':
							strcat(t.ad, "0101");
							break;
						case '6':
							strcat(t.ad, "0110");
							break;
						case '7':
							strcat(t.ad, "0111");
							break;
						case '8':
							strcat(t.ad, "1000");
							break;
						case '9':
							strcat(t.ad, "1001");
							break;
						case 'A':
							strcat(t.ad, "1010");
							break;
						case 'B':
							strcat(t.ad, "1011");
							break;
						case 'C':
							strcat(t.ad, "1100");
							break;
						case 'D':
							strcat(t.ad, "1101");
							break;
						case 'E':
							strcat(t.ad, "1110");
							break;
						case 'F':
							strcat(t.ad, "1111");
							break;
						default :
							error = 1;
							break;
					}
				i++;
				}
				t.ai = 4 * x.ai;
			}
			free(x.bd);
			free(x.ad);
			return t;/*hb*/
			break;
		case 'K':
			x = solve('J', x, x);
			x = solve('A', x, x);
			return x;/*ho*/
			break;
		case 'L':
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			while(i < x.bi) {
				if(x.bd[i] >= '0' && x.bd[i] <= '9')
					p += ((pow(16, x.bi - i - 1)) * (x.bd[i] - 48));
				else if(x.bd[i] >= 'A' && x.bd[i] <= 'F')
					p += ((pow(16, x.bi - i - 1)) * (x.bd[i] - 'A' + 10));
				else {
					error = 1;
					return x;
				}
				i++;
			}
			if(lflag == 1) {
				i = 0;
				while(i < x.ai) {
					if(x.ad[i] >= '0' && x.ad[i] <= '9')
						p += ((pow(16, -(1 + i))) * (x.ad[i] - 48));
					else if(x.ad[i] >= 'A' && x.ad[i] <= 'F')
						p += ((pow(16, -(1 + i))) * (x.ad[i] - 'A' + 10));
					else {
						error = 1;
						return x;
					}
					i++;
				}
			}
			i = 0;
			while(p >= 1) {
				p = p / 10;
				i++;
			}
			t.bd[i] = '\0';
			t.bi = i;
			j = 0;
			while(i > j) {
				t.bd[j] = 48 + ((int)(p * 10) % 10);
				p = p * 10;
				j++;
			}
			i = 0;
			while(i < 6) {
				t.ad[i] = 48 + ((int)(p * 10) % 10);
				p = p * 10;
				i++;
			}
			t.ai = 6;
			t.ad[i] = '\0';
			free(x.bd);
			free(x.ad);
			return t;/*hd*/
			break;
		case 'M':
			return x;/*++*/
			break;
		case 'N':
			return x;/*--*/
			break;
		case 'O': case 'P': case 'Q': case 'R':
			if(y.var == 0) {
				error = 1;
				return y;
			}
			c = y.v;
			switch(op) {
				case 'O':
					y = add(x, y);
					break;
				case 'P':
					y = sub(x, y);
					break;
				case 'Q':
					y = mul(x, y);
					break;
				case 'R':
					x = rmz(x);
					if(x.bi == 0 && x.ai == 0) {
						printf("division by 0 \n");
						error = 1;
						free(x.bd);
						free(x.ad);
						return y;
					}
					else
						y = divi(x, y);
					break;
			}
			if(c >= 'a' && c <= 'z') {
				strcpy(variable[c - 'a'].bd, y.bd);
				variable[c - 'a'].bi = y.bi;
				variable[c - 'a'].sign = y.sign;
				strcpy(variable[c - 'a'].ad, y.ad);
				variable[c - 'a'].ai = y.ai;
				variable[c - 'a'].alimit = y.alimit;
				variable[c - 'a'].blimit = y.blimit;
			}
			else if(c >= 'G' && c <= 'Z') {
				strcpy(variable[c - 'G' + 26].bd, y.bd);
				variable[c - 'G' + 26].bi = y.bi;
				variable[c - 'G' + 26].sign = y.sign;
				strcpy(variable[c - 'G' + 26].ad, y.ad);
				variable[c - 'G' + 26].ai = y.ai;
				variable[c - 'G' + 26].alimit = y.alimit;
				variable[c - 'G' + 26].blimit = y.blimit;
			}
			return y;
			break;
		case 'S': case 'U':
			if(y.var != 1) {
				error = 1;
				return y;
			}
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			c = y.v;
			ope = 1;
			y = add(y, t);
			if(c >= 'a' && c <= 'z') {
				strcpy(variable[c - 'a'].bd, y.bd);
				variable[c - 'a'].bi = y.bi;
				variable[c - 'a'].sign = y.sign;
				strcpy(variable[c - 'a'].ad, y.ad);
				variable[c - 'a'].ai = y.ai;
				variable[c - 'a'].alimit = y.alimit;
				variable[c - 'a'].blimit = y.blimit;
			}
			else if(c >= 'G' && c <= 'Z') {
				strcpy(variable[c - 'G' + 26].bd, y.bd);
				variable[c - 'G' + 26].bi = y.bi;
				variable[c - 'G' + 26].sign = y.sign;
				strcpy(variable[c - 'G' + 26].ad, y.ad);
				variable[c - 'G' + 26].ai = y.ai;
				variable[c - 'G' + 26].alimit = y.alimit;
				variable[c - 'G' + 26].blimit = y.blimit;
			}
			ope = 0;
			if(op == 'U')
				y = sub(t, y);
			return y;
			break;
		case 'T': case'V':
			if(y.var != 1) {
				error = 1;
				return y;
			}
			t = initnum(t);
			t.bi = 1;
			t.bd[0] = '1';
			t.bd[1] = '\0';
			c = y.v;
			ope = 1;
			y = sub(t, y);
			if(c >= 'a' && c <= 'z') {
				strcpy(variable[c - 'a'].bd, y.bd);
				variable[c - 'a'].bi = y.bi;
				variable[c - 'a'].sign = y.sign;
				strcpy(variable[c - 'a'].ad, y.ad);
				variable[c - 'a'].ai = y.ai;
				variable[c - 'a'].alimit = y.alimit;
				variable[c - 'a'].blimit = y.blimit;
			}
			else if(c >= 'G' && c <= 'Z') {
				strcpy(variable[c - 'G' + 26].bd, y.bd);
				variable[c - 'G' + 26].bi = y.bi;
				variable[c - 'G' + 26].sign = y.sign;
				strcpy(variable[c - 'G' + 26].ad, y.ad);
				variable[c - 'G' + 26].ai = y.ai;
				variable[c - 'G' + 26].alimit = y.alimit;
				variable[c - 'G' + 26].blimit = y.blimit;
			}
			ope = 0;
			if(op == 'V')
				y = add(t, y);
			return y;
			break;
		default:
			error = 1;
			return x;
			break;				
	}

}


char ctop(stackc *b) {
	char optr = popc(b);
	pushc(b, optr);
	return optr;
}


num infixeval(char *str) {
	token *t;
	num result, x, y;
	y = initnum(y);
	x = initnum(x);
	int  pretok = 0, curtok = 0;
	stacki a;
	initi(&a);
	stackc b;
	initc(&b);
	char optr;
	int reset = 1, h = 0, p, q;
	while(1) {
		t = getnext(str, &reset);
		if(error == 1)
			return x;
		if(t->type == ERR)
			return x;
		curtok = t->type;
		if(curtok == pretok && pretok == OPERAND && t->op != '(' && t->op != ')') {
			error = 1;
			return x;
		}
		if((curtok == pretok || emptyi(&a)) && t->op == '-') {		/*if int_stack is empty then '-' is 1st char of string*/
			if(!fulli(&a)) {
				x.bd[0] = '1';
				x.bi = 1;
				x.ad[0] = '\0';
				x.ai = 0;
				x.bd[1] = '\0';
				x.sign = 1;
				pushi(&a, x);
			}
			else {
				error = 1;
				stackfull = 1;
				return x;
			}
			if(!fullc(&b))
				pushc(&b, '*');
			else {
				error = 1;
				stackfull = 1;
				return x;
			}
			continue;
		}
		if(t->type == END && h != 0) {
			error = 1;
			return x;
		}
		if(t->type == OPERAND) {
			if(!fulli(&a))
				pushi(&a, t->number);
			else {
				error = 1;
				stackfull = 1;
				return x;
			}
		}
		else if (t->type == OPERATOR) {
			switch(t->op) {
				case '(':
					h++;
					break;
				case ')':
					h--;
					break;
				case 'M':case 'N':
					t->op += 'S' - 'M'; 
					if(pretok == OPERAND)
						(t->op) += 2;
					break;
			}
			if(!emptyc(&b)) {
				optr = ctop(&b);
				if(t->op == 'r') {
					error = 1;
					return x;
				}
				p = precedence(optr);
				q = precedence(t->op);
				if(p == -2 || q == -2) {
					error = 1;
					return x;
				}
				if(q > p) {
					if(!fullc(&b))	
						pushc(&b, t->op);
					else {
						error = 1;
						stackfull = 1;
						return x;
					}
				} 	
				else {	
					while(q <= p) {
						optr = popc(&b);
						if(optr == '(') {
							pushc(&b, optr);
							break;
						}
						else if(optr == 'a' || optr == 'b' || optr == 'c' || optr == 'd' || 
						        optr == 'h' || optr == 'g' || optr == 'f' || optr == 'e' || 
						        optr == 'i' || optr == 'j' || optr == 'k' || optr == 'A' ||
						        optr == 'B' || optr == 'C' || optr == 'D' || optr == 'E' || 
						        optr == 'F' || optr == 'G' || optr == 'H' || optr == 'I' || 
						        optr == 'J' || optr == 'K' || optr == 'L' || optr == 'T' ||
							optr == 'S' || optr == 'u' || optr == 'v' || optr == 'U' ||
							optr == 'V') {
							if(!emptyi(&a))
								x = popi(&a);
							else {
								error = 1;
								return x;						
							}
							result = solve(optr, x, x);
							pushi(&a, result);
							if(!emptyc(&b)) {
								optr = ctop(&b);
								p = precedence(optr);
							}
							else
								break;
						}
						else{
							if(!emptyi(&a))
								x = popi(&a);
							else {
								error = 1;
								return x;
							}
							if(!emptyi(&a))
								y = popi(&a);
							else {
								error = 1;
								return x;
							}
							result = solve(optr, x, y);
							pushi(&a, result);
							if(!emptyc(&b)) {
								optr = ctop(&b);
								p = precedence(optr);
							}
							else
								break;
						}
					}
					if(t->op != ')')
						pushc(&b, t->op);
					else
						optr = popc(&b);
				}
			}
			else {
				pushc(&b, t->op);
			}
		}
		else if (t->type == END) {
			if(!emptyc(&b)) {
				while(!emptyc(&b)){
					optr = popc(&b);
					if(optr == 'a' || optr == 'b' || optr == 'c' || optr == 'd' || 
					   optr == 'h' || optr == 'g' || optr == 'f' || optr == 'e' || 
					   optr == 'i' || optr == 'j' || optr == 'k' || optr == 'A' ||
					   optr == 'B' || optr == 'C' || optr == 'D' || optr == 'E' || 
					   optr == 'F' || optr == 'G' || optr == 'H' || optr == 'I' || 
					   optr == 'J' || optr == 'K' || optr == 'L' || optr == 'S' ||
					   optr == 'T' || optr == 'u' || optr == 'v' || optr == 'U' ||
					   optr == 'V') {
						if(!emptyi(&a))
							x = popi(&a);
						else {
							error = 1;
							return x;
						}
						result = solve(optr, x, x);
						pushi(&a, result);
					}
					else {
						if(!emptyi(&a))
							x = popi(&a);
						else {
							error = 1;
							return x;
						}
						if(!emptyi(&a))
							y = popi(&a);
						else {
							error = 1;
							return x;
						}
						result = solve(optr, x, y);
						pushi(&a, result);
					}
				}
				return result;
			}
			else {
				x = popi(&a);		/*no need to check if int_stack is empty because there has to be a number*/	
				if(!emptyi(&a)) {
					error = 1;
					return x;
				}
				else 
					return x;
			}
		} 
		else if (t->type == ERROR) {
			error = 1;
			return x; 
		}
		pretok = curtok;
	}
	free(t);
}

void printans(num ans) {
	if(ans.var == 1) {
		ans.var = 0;
		if(ans.v >= 'a' && ans.v <= 'z') {
			strcpy(ans.bd, variable[ans.v - 'a'].bd);
			ans.bi = variable[ans.v - 'a'].bi;
			ans.sign = variable[ans.v - 'a'].sign;
			strcpy(ans.ad, variable[ans.v - 'a'].ad);
			ans.ai = variable[ans.v - 'a'].ai;
			ans.alimit = variable[ans.v - 'a'].alimit;
			ans.blimit = variable[ans.v - 'a'].blimit;
		}
		else if(ans.v >= 'G' && ans.v <= 'Z') {
			strcpy(ans.bd, variable[ans.v - 'G' + 26].bd);
			ans.bi = variable[ans.v - 'G' + 26].bi;
			ans.sign = variable[ans.v - 'G' + 26].sign;
			strcpy(ans.ad, variable[ans.v - 'G' + 26].ad);
			ans.ai = variable[ans.v - 'G' + 26].ai;
			ans.alimit = variable[ans.v - 'G' + 26].alimit;
			ans.blimit = variable[ans.v - 'G' + 26].blimit;
		}
		else
			error = 1;
	}
	if(error == 1) {
		if(stackfull == 1) {
			fprintf(stderr, "exprression is too long ... stack is full\n");
			stackfull = 0;
		}
		else
			fprintf(stderr, "Error in expression\n");
		error = 0;
	}
	else {
		if(ans.sign == 1) 
			printf("-");
		if(ans.bi == 0)
			printf("0");
		if((dflag == 1 || lflag == 1) && (ans.ai != 0)) {
			strcat(ans.bd, ".");
			strcat(ans.bd, ans.ad);
			puts(ans.bd);
		}	
		else {
			puts(ans.bd);				
		}
	}
	dflag = 0;
	free(ans.bd);
	free(ans.ad);
}
void printusage() {
	printf("usage:  ./project  [option] [filename]\n\noption\tdescription\n-l    \ttrigonometric, log, ln and e^(expression) options\n-h    \thelp and exit\n--help\thelp and exit\n");
}

int main(int argc,char *argv[]) {
	char str[128], *o;
	o = &str[0];
	int q = 1;
	initvar();
	num ans;
	int x;
	size_t b = 128;
	ssize_t z;
	FILE *fp;
	num infixeval(char *infix);
	while(q < argc) {
		if((strcmp(argv[q], "-h") == 0) || (strcmp(argv[q], "--help") == 0)) {
			printusage();
			return 0;
		}
		else if(strcmp(argv[q], "-l") == 0)
			lflag = 1;
		else {
			fp = fopen(argv[q], "r");
			if(fp == NULL) {
				if(argv[q][0] == '-') {
					printf("invalid option ... '%s'\n", argv[q]);
					printusage();
					return 0;
				}
				else {
					printf("Error: %s :No such file exists\n", argv[q]);
					return 0;
				}
			}
			while((z = getline(&o, &b, fp)) != -1) {
				str[strlen(str) - 1] = '\0';
				ans = infixeval(str);
				printans(ans);
			}
			fclose(fp);
		}
		q++;
	}
	while(printf("> ") && (x = readline(str, 128))) {
		ans = infixeval(str);
		printans(ans);
	}
	for(x = 0;x < 46; x++) {
		free((variable)[x].bd);
		free((variable)[x].ad);
	}
	return 0;
}
