#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
FILE *file_in, *file_out;
const int lenprog = 10000;//���� �������� ����� �������������� ����� ������� ���������� �������, � �� �������� ��������, ��� ������������ �����
const int Key_Words_num = 16;
const char * KWs[Key_Words_num] =
{
"begin", "end", "var", "real",
"boolean", "false","true", "do",
"repeat", "until", "function","type",
"record", "and", "or", "read"
};// ��� ������ �������� ����, ������� �������������� ������ ����������
//��������� �����
//������� ��������� ����� �������� �����, ��� ����� ������ ������������� � �����
#define keyW_begin 1
#define keyW_end 2
#define keyW_var 3
#define keyW_real 4
#define keyW_boolean 5
#define keyW_false 6
#define keyW_true 7
#define keyW_do 8
#define keyW_else 9
#define keyW_for 10
#define keyW_function 11
#define keyW_type 12
#define keyW_record 13
#define keyW_and 14
#define keyW_or 15
#define keyW_read 16
#define keyW_procedure 17
//����
//���� ��������� ������
#define IntType 1 //�����
#define RealType 2 //�������
#define BoolType 3 //����������
#define ArrayType 4 //������
const int cuservars = 6;
int uvars;
//��� "�����"
#define errore_word 0 //������
#define w_spa 1 //�������
#define string_word 2 //������
#define number 3 //�����
#define w_ch 4 //������ (�����������)
//���������� ��� ���� ������� � �������� ������� ������, � ��� �������
//������ ������������ ����� ������� ����� ������ '\n', ��� � ������������ � ���������� ��� ������������ ��������� ����� ���������
char instr[lenprog],
outstr[lenprog];
int inlen, outlen, inpos;//������� ������� (������ �������, ��������, ������� � ������ �������)
int koper1, koper2, ker;//��� ���������� ������� ���������� ���������� �� �������, �������� ����� � ���-�� ������
int lenght_world; //����� "�����"
int wtype; //��� "�����"
int wnumf; //0 - ����� �����, 1 - �������
int dt; //������ ������� (�� ������ ������)
typedef struct var { //������� ������ ����������
	char s[64]; //�����-� ����������
	int tp; //��� ����������
	var * next;
};
typedef struct types { //������� ������ �����
	char s[64]; //�����-� ����������
	int tid;
	types * next;
};
var *vars, *lvars; //������ ����������
//(���������� � ���������)
types * typeslist;
// ��������� �������
int fle();
// ���������� ������� ��������� ������ � ������������ �������,
// � ������� ��������� ������ � �����������, �� ������ � ���������
// ��������� ���� ������, ����� ���� ���������� ������� ���������� ����������
// � c++
//�������� ���������� � ������
// ��� gl - ��� ����������, ���������� ��� ���������.
int addvar(int gl) {
	var *np, *p;
	if (gl) p = vars; else p = lvars;
	// int memcmp(const void *buf1, const void *buf2, size_t count);
	// ������� memcmp() ���������� ������ count �������� ��������, ���������� ����������� buf1 � buf2.
	// buf1 ������ buf2 = ������ ����
	// buf1 ����� buf2 = 0
	// buf1 ������ buf2 = ������ ����
	while (p) {
		if (strlen(p->s) == lenght_world &&
			!memcmp(p->s, &instr[inpos], lenght_world)) return 0;
		p = p->next;
	}
	np = new var;
	// void *memcpy(void *to, const void *from, size_t count);
	// ������� memcpy() �������� count �������� �� �������,
	// ����������� ���������� from, � ������, ���������� ���������� to.
	// ���� �������� ������� �������������, ��������� ������� memcopy() �� ����������.
	memcpy(np->s, &instr[inpos], lenght_world);
	// ����� ����� ���������� �� ���������� 0 (�����
	// ��� �� ����� ������ ��� ���������� �� ����� �������)
	np->s[lenght_world] = 0;
	// ��� ���������� ������ ��� -1 (�����, ��� �� ����� ��������������)
	np->tp = -1;
	// ���� ���������� ����������, �� ��������� ���� ������� � ������
	// (�.�. ���������� ���� �� ����������) ������ ���������� ����������
	// ����� � ������ ������ ���������
	if (gl) {
		np->next = vars;
		vars = np;
	}
	else {
		np->next = lvars;
		lvars = np;
	}
	return 1;
}
int addnewtype()
{
	types *head = typeslist, *ntype;
	while (head) {
		if (strlen(head->s) == lenght_world &&
			!memcmp(head->s, &instr[inpos], lenght_world)) return 0;
		head = head->next;
	}
	ntype = new types;
	memcpy(ntype->s, &instr[inpos], lenght_world);
	ntype->s[lenght_world] = 0;
	ntype->tid = uvars;
	uvars++;
	ntype->next = typeslist;
	typeslist = ntype;
	return 1;
}
//��������� ���� ����������
// ���� gl - ������������� ������������ = 1, �� ��� ���������� ����������
// ����� ���������
// ���� ������ ���� ���������� (������� ����)
// ����������� while ����� ������������ ���:
// ���� ��������� �� ��������� p ���������� ���������:
// ���� ��� ���������� -1 �� ��������� �� �������� ���
// � ����� ��� ������
void settype(int gl, int type) {
	var *p;
	if (gl) p = vars; else p = lvars;
	while (p) {
		if (p->tp == -1) p->tp = type;
		p = p->next;
	}
}
// ��������� ���� ����������
// ��� ���� ��� ��� ������ ��� ������� �����
int vartype() {
	var * p;
	// ��������� ��������� ����������
	p = lvars;
	while (p) {
		if (strlen(p->s) == lenght_world && // ���� ����� �� ������� ������ ����� �������� ���������� (� ������),
			!memcmp(p->s, &instr[inpos], lenght_world)) return p->tp; // �� ������� ��� ����������
		p = p->next;
	}
	// ��������� ���������� ����������
	p = vars;
	while (p) {
		if (strlen(p->s) == lenght_world &&
			!memcmp(p->s, &instr[inpos], lenght_world)) return p->tp;
		p = p->next;
	}
	return 0;
}
char* usertype()
{
	types * p;
	// ��������� ��������� ����������
	p = typeslist;
	while (p) {
		if (strlen(p->s) == lenght_world && // ���� ����� �� ������� ������ ����� �������� ���������� (� ������),
			!memcmp(p->s, &instr[inpos], lenght_world)) return p->s; // �� ������� ��� ����������
		p = p->next;
	}
	return 0;
}
// ������������ ������ ����������
// ������ �������� ����� �������
// ��� ��� ��������� ����������, ��� � ��� ����������
void freevars(int gl) {
	var *p, *pp;
	p = lvars;
	while (p) {
		pp = p;
		p = p->next;
		delete pp;
	}
	lvars = NULL;
	if (gl) {
		p = vars;
		while (p) {
			pp = p;
			p = p->next;
			delete pp;
		}
		vars = NULL;
	}
}
// ���� �� ������� ������� ��������� �����, ����� ����������� ������������� �����
// ������� �������� �������� ���
// ����� ����������� ����� ����������� � ������ ��� ���������� ��� �������
// (�����, �����, ������ ��.�.),
// ����� ������� ���� ���������� �������� ����� ������� (�����, ���������� � �.�.)
// ��� ������������������ ��������( ���������, �������� ����� � �.�.)
// ���������� ������� ����������� ��� ���������� ��� ����� �����
//������� ���������� ���� �� ��������� �����
////��� "�����"
// errore_word 0 //������
// w_spa 1 //�������
// string_word 2 //������
// number 3 //�����
// w_ch 4 //������ (�����������, ...)
// ������ �����
int ReadWord()
{
	int st = 0;
	char c; // ������ ������� ������������
	lenght_world = 0;// ����� �������������� �����
	while (lenght_world + inpos < inlen) {
		c = instr[inpos + lenght_world];
		switch (st) { // ��������� ������� � ���������� ����� ����������� ���������
		case 0:
			if (c == ' ' || c == '\t' || c == '\n') st = 1;
			else
				if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) st = 2;
				else
					if (c >= '0' && c <= '9') st = 3;
					else
						if (
							c == '.' || c <= ',' || c >= ':' || c <= ';' ||
							c == '+' || c <= '-' || c >= '*' || c <= '/' ||
							c == '\''
							)
						{
							lenght_world = 1; return wtype = w_ch;
						}//����������, ��� ��� ������
						else { lenght_world = 0; return wtype = errore_word; }
			break;
		case 1:
			if (c == ' ' || c == '\t' || c == '\n') lenght_world++;
			else return wtype = w_spa; ///����������, ��� ��� ������ ������
			break;
		case 2:
			if (
				(c >= 'A' && c <= 'Z') ||
				(c >= 'a' && c <= 'z') ||
				(c >= '0' && c <= '9') ||
				c == '_'
				) lenght_world++;
			else return wtype = string_word;
			break;
		case 3:
			if (c >= '0' && c <= '9') lenght_world++; else
				if (c == '.'&& instr[inpos + lenght_world + 1] != '.') { // �������, ��� ���� ����� � �����, �.� ��� �������
					lenght_world++;
					st = 5;
				}
				else {
					wnumf = 0;
					return wtype = number;//����������, ��� ��� �����
				}
			break;
		case 5:
			if (c >= '0' && c <= '9') lenght_world++; else {
				wnumf = 1;
				return wtype = number;//����������, ��� ��� ����� (������� �����)
			}
		}
	}
	lenght_world = 0;
	return 0;
}
//������ ������ � �������� �����
void put_str(char * s) {
	int l = strlen(s);
	for (int i = 0; i < l; i++)
		outstr[outlen + i] = (char)s;
	outlen += l;
}
int scmp(char * m, char * s, int n) {
	int l = strlen(s);
	if (n > l) l = n;
	return _memicmp(m, s, l);
}
//������ ������� � �������� �����
void put_char(char c) {
	outstr[outlen] = c;
	outlen++;
}
void wcopy() {
	for (int i = 0; i < lenght_world; i++)
		outstr[outlen + i] = (char)instr[inpos];
	inpos += lenght_world;
	outlen += lenght_world;
}
int wequ(char * s) {
	return (!scmp(&instr[inpos], s, lenght_world));
}
//���������� �������
void wskip() {
	inpos += lenght_world;
}
void wstr(char * s) {
	// char *strncpy (char *dst, const char *src, size_t len);
	// dst � ��������� �� ����� ����������.
	// src � ��������� �� �������� ������.
	// len � ������������ ���������� ���������� �������� (��. ������ ������������ ����).
	// ������� �������� �� ������ src � ����� dst �� ����� ��� len ��������
	// (������� ������� ������), �� ���������� ���������� ������
	// ������� �������� (���� ����� ������ src ������ ��� ����� len).
	// ���� ����� ������ src ������ len, �� ����� ���������� �� len ������.
	// ������� ���������� �������� dst.
	strncpy(s, &instr[inpos], lenght_world);
}
//��������� ����, � ���� ��� ����� �������, ��������� ���������, ������������ �������
int ReadWordS() {
	ReadWord();
	if (wtype == w_spa) {
		wskip();
		ReadWord();
	}
	return wtype;
}
//���������� �������
void inc_dt()
{
	dt += 2;
}
//���������� �������
void dec_dt()
{
	dt -= 2;
}
//����� �������
void put_dt()
{
	for (int i = 0; i < dt; i++) put_char(' ');
}
//����� �������
void put_dt11()
{
	char s[10];
	for (int i = 0; i < dt; i++) {
		sprintf(s, "%d", i / 2);
		put_str(s);
	}
}
//�������� �����
int ReadText(char * s1)
{
	if ((file_in = fopen(s1, "rt")) == NULL) {
		return 0;
	}
	fseek(file_in, 0, SEEK_END);
	inlen = ftell(file_in);
	printf("%i", inlen);
	fseek(file_in, 0, SEEK_SET);
	if (inlen > lenprog) inlen = lenprog;
	inlen = fread(instr, 1, inlen, file_in);
	instr[inlen] = 0;
	inpos = 0;
	outlen = 0;
	return 1;
}
//����� ������������� ������
int PutText(char * s2)
{
	if ((file_out = fopen(s2, "wt")) == NULL) {
		return 0;
	}
	fwrite(outstr, outlen, 1, file_out);
	return 1;
}
//����� ���������� ���������
void Errore_oper()
{
	put_char('\n');
	put_str((char *)"< ������! > \n");
	int k;
	while (1) {
		ReadWordS();
		if (instr[inpos] == ';' || inpos >= inlen) {
			wcopy();
			break;
		};
		wcopy();
	}
	ker++; // ���������� �������� ������
}
//�������� �� �����������
int find_Comment() {
	return (instr[inpos] == '{' ||
		instr[inpos] == '(' || instr[inpos + 1] == '*');
}
//�������� �� �������� �����������
void fin_Comment() {
	if (instr[inpos] == '{') {
		outstr[outlen] = '/';
		outstr[outlen + 1] = '*';
		inpos++;
		outlen += 2;
		while (instr[inpos] != '}' && inpos < inlen) {
			if (inpos >= inlen) return;
			outstr[outlen] = instr[inpos];
			inpos++;
			outlen++;
		}
		outstr[outlen] = '*';
		outstr[outlen + 1] = '/';
		inpos++;
		outlen += 2;
	}
	else {
		outstr[outlen] = '/';
		outstr[outlen + 1] = '*';
		inpos += 2;
		outlen += 2;
		while (!(instr[inpos] == '*' && instr[inpos + 1] == ')')
			&& inpos < inlen) {
			if (inpos >= inlen) return;
			outstr[outlen] = instr[inpos];
			inpos++;
			outlen++;
		}
		outstr[outlen] = '*';
		outstr[outlen + 1] = '/';
		inpos += 2;
		outlen += 2;
	}
	put_char('\n');
}
// ����� ���� �������� �� �������� �����,
// ������������ ���������� ���� ����� �� ������ �� ������
// � ���� ��, �� ������������ ����� ����� � ������
// �������� �� �������� �����
int funckeyW() {
	for (int i = 0; i < Key_Words_num; i++) {// �������� ����� ������� Key_Words_num
		if (!scmp(&instr[inpos], (char *)KWs[i], lenght_world))
			return i + 1;
	}
	return 0;
}
// ��� ��� ��������� ���������� ����� ����� var,
// ����� ���� ��������� � ��� ������� �������� ���������� �� ������� ���������
// ��������� �������� ����������: x1,..,xn: ���;
int fIntRealBoolAr(int svar, int gl) {
	char s[256];
	int label;
	int sp = 0;
	ReadWordS();
	while (1) {
		if (wtype != string_word || funckeyW() || gl > 0 && vartype()) return 0;
		addvar(gl);
		if (svar) {
			s[sp] = '&';
			s[sp + 1] = ' ';
			sp += 2;
		}
		memcpy(&s[sp], &instr[inpos], lenght_world);
		inpos += lenght_world;
		sp += lenght_world;
		ReadWordS();
		if (instr[inpos] == ',') {
			s[sp] = ',';
			inpos++;
			sp++;
		}
		else break;
		ReadWordS();
	} // while(1)
	if (instr[inpos] == ':') { //��� ����������
		inpos++;
		ReadWordS();
		if (wtype != string_word) return 0;
		if (!scmp(&instr[inpos], (char *)"boolean", lenght_world)) {
			settype(gl, BoolType);
			put_str((char *)"int ");
			wskip();
			memcpy(&outstr[outlen], &s[0], sp);
			outlen += sp;
		}
		else
			if (!scmp(&instr[inpos], (char *)"real", lenght_world)) {
				settype(gl, RealType);
				put_str((char *)"float ");
				wskip();
				memcpy(&outstr[outlen], &s[0], sp);
				outlen += sp;
			}
			else
				if (!scmp(&instr[inpos], (char *)"integer", lenght_world)) {
					settype(gl, IntType);
					put_str((char *)"int ");
					wskip();
					memcpy(&outstr[outlen], &s[0], sp);
					outlen += sp;
				}
				else
					if (!scmp(&instr[inpos], usertype(), lenght_world)) {
						put_str(usertype());
						put_str((char *)" ");
						wskip();
						memcpy(&outstr[outlen], &s[0], sp);
						outlen += sp;
					}
					else
						if (!scmp(&instr[inpos], (char *)"array", lenght_world)) {
							wskip();
							settype(gl, ArrayType);
							ReadWordS();
							if (instr[inpos] != '[') return 0;
							s[sp] = '[';
							inpos++;
							sp++;
							while (1) {
								ReadWordS();
								if (instr[inpos] != '1') return 0;
								wskip();
								ReadWordS();
								if (instr[inpos] != '.' || instr[inpos + 1] != '.')
									return 0;
								inpos += 2;
								ReadWordS();
								if (wtype != number) return 0;
								memcpy(&s[sp], &instr[inpos], lenght_world);
								inpos += lenght_world;
								sp += lenght_world;
								ReadWordS();
								if (instr[inpos] == ']') {
									s[sp] = ']';
									inpos++;
									sp++;
									break;
								}
								if (instr[inpos] == ',') {
									inpos++;
									s[sp] = ']';
									sp++;
									s[sp] = '[';
									sp++;
								}
							}
							ReadWordS();
							if (wtype != string_word) return 0;
							if (scmp(&instr[inpos], (char *)"of", lenght_world)) return 0;
							wskip();
							ReadWordS();
							if (wtype != string_word) return 0;
							if (scmp(&instr[inpos], (char *)"real", lenght_world)) return 0;
							wskip();
							put_str((char *)"float ");
							memcpy(&outstr[outlen], &s[0], sp);
							outlen += sp;
						}
	}
	else return 0;
	return 1;
}
//��������� �������� ����� ����������
int funcvar(int gl) {
	inpos += lenght_world;
	ReadWordS();
	do {
		koper1++;
		if (find_Comment()) { //�����������
			fin_Comment();
			koper2++;
			continue;
		}
		put_dt();
		if (!fIntRealBoolAr(0, gl)) Errore_oper();
		else koper2++;
		ReadWordS();
		if (instr[inpos] != ';')
			return 0;
		wskip();
		put_str((char *)";\n");
		ReadWordS();
		if (wtype != string_word || funckeyW())
			return 1;
	} while (1);
}
//��������� ����� �������� ����������
int funcvardescr() {
	inpos += lenght_world;
	int k, svar;
	ReadWordS();
	do {
		k = funckeyW();
		svar = k == keyW_var;
		if (svar) {
			wskip();
			ReadWordS();
		}
		if (!fIntRealBoolAr(svar, 0)) return 0;
		ReadWordS();
		if (instr[inpos] != ';') return 1;
		wskip();
		put_str((char *)", ");
		ReadWordS();
		k = funckeyW();
		if (wtype != string_word || k && k != keyW_var) return 0;
	} while (1);
}
int funcbegin(int k);
//��������� ���������
int funcprocedure() {
	//������ ��������� ���������
	wskip();
	put_str((char *)"\nvoid ");
	ReadWordS();
	if (wtype != string_word || vartype()) return 0;
	addvar(1);
	settype(1, 10); //����������� �������������
	wcopy();
	ReadWordS();
	if (instr[inpos] != '(') return 0;
	put_char('(');
	//������ ����������
	if (!funcvardescr()) return 0;
	ReadWordS();
	if (instr[inpos] != ')') return 0;
	wcopy();
	ReadWordS();
	if (instr[inpos] != ';') return 0;
	wskip();
	put_str((char *)"\n{\n");
	inc_dt();
	//���� ���������
	int b;
	do {
		b = 1;
		ReadWordS();
		if (!scmp(&instr[inpos], (char *)"var", lenght_world)) {
			koper1++;
			if (!funcvar(0)) return 0;
		}
		else
			if (!scmp(&instr[inpos], (char *)"begin", lenght_world)) {
				if (!funcbegin(2)) return 0;
				b = 0;
			}
			else
				if (find_Comment()) fin_Comment();
				else return 0;
	} while (b == 1);
	freevars(0);
	inpos++;
	return 1;
}
//��������� ��������� record
int funcrecord() {
	wskip();
	put_str((char *)"struct ");
	ReadWordS();
	if (wtype != string_word || funckeyW()) return 0;
	addnewtype();
	wcopy();
	ReadWordS();
	if (instr[inpos] != '=') return 0;
	wskip();
	ReadWordS();
	if (!wequ((char *)"record")) return 0;
	put_str((char *)"\n{\n");
	inc_dt();
	if (!funcvar(-1)) return 0;
	dec_dt();
	ReadWordS();
	if (!wequ((char *)"end")) return 0;
	wskip();
	put_char('}');
	ReadWordS();
	if (instr[inpos] != ';') return 0;
	wcopy();
	put_str((char *)"\n\n");
	inpos += lenght_world;
	return 1;
}
//��������� ��������� read
int funcread(int ln) {
	char s[256];
	int sp;
	int t;
	wskip();
	put_dt();
	put_str((char *)"scanf");
	ReadWordS();
	if (instr[inpos] != '(') return 0;
	inpos++;
	put_str((char *)"(\"");
	sp = 0;
	while (1) {
		ReadWordS();
		if (wtype != string_word) return 0;
		t = vartype();
		if (t == IntType) put_str((char *)"%d");
		else if (t == RealType) put_str((char *)"%f");
		else return 0;
		s[sp] = '&';
		sp += lenght_world;
		memcpy(&s[sp], &instr[inpos], lenght_world);
		inpos += lenght_world;
		sp += lenght_world;
		ReadWordS();
		if (instr[inpos] != ',') break;
		s[sp] = instr[inpos];
		inpos++;
		sp++;
	}
	put_str((char *)"\",");
	memcpy(&outstr[outlen], s, sp);
	outlen += sp;
	ReadWordS();
	if (instr[inpos] != ')') return 0;
	inpos++;
	put_char(')');
	ReadWordS();
	if (instr[inpos] != ';') return 0;
	inpos++;
	if (ln) put_str((char *)"; printf(\"\\n\");\n");
	else put_str((char *)";\n");
	return 1;
}
//��������� ��������������� ���������
int fae() {
	ReadWordS();
	if (instr[inpos] == '+') {
		wcopy();
	}
	else
		if (instr[inpos] == '-') {
			wcopy();
		}
	while (1) {
		ReadWordS();
		if (wtype == number) wcopy(); else
			if (wtype == string_word && vartype() == IntType) wcopy(); else
				if (wtype == string_word && vartype() == RealType) wcopy(); else
					if (wtype == string_word && vartype() == ArrayType) {
						wcopy();
						ReadWordS();
						if (instr[inpos] == '[') {
							wcopy();
							while (1) {
								if (!fae()) return 0;
								put_str((char *)"-1");
								ReadWordS();
								if (instr[inpos] == ']') {
									wcopy();
									break;
								}
								if (instr[inpos] == ',') {
									wskip();
									put_str((char *)"][");
								}
							}
						}
					}
					else
						if (instr[inpos] == '(') {
							wcopy();
							if (!fae()) return 0;
							ReadWordS();
							if (instr[inpos] != ')') return 0;
							inpos++;
							put_char(')');
						}
						else return 0;
		ReadWordS();
		char c = instr[inpos];
		if (c == '+' || c == '-' || c == '*' || c == '/') wcopy();
		else return 1;
	}
}
//��������� ��������������� ���������
int ae() {
	char c, c1;
	if (!fae()) return 0;
	ReadWordS();
	c = instr[inpos];
	c1 = instr[inpos + 1];
	if (c == '<'&&c1 == '>') {
		inpos += 2;
		put_str((char *)"!=");
	}
	else
		if (c == '=') {
			inpos++;
			put_str((char *)"==");
		}
		else
			if (c == '>' || c == '<') {
				if (c1 == '=') {
					inpos += 2;
				}
				else wcopy();
			}
	ReadWordS();
	if (!fae()) return 0;
	return 1;
}
//��������� ����������� ���������
int fle() {
	int k;
	char c, c1;
	int arifm, ip, op;
	while (1) {
		ReadWordS();
		k = funckeyW();
		int ip, op;
		ip = inpos;
		op = outlen;
		arifm = 0;
		if (instr[inpos] == '+' ||
			instr[inpos] == '(' ||
			instr[inpos] == '-' ||
			wtype == string_word && !funckeyW() ||
			wtype == number)
			arifm = ae();
		if (!arifm) {
			inpos = ip;
			outlen = op;
			ReadWordS();
			k = funckeyW();
			//------
			if (wtype == string_word && k == keyW_true) {
				wskip();
				put_char('1');
			}
			else
				if (wtype == string_word && k == keyW_false) {
					wskip();
					put_char('0');
				}
				else
					if (wtype == string_word && vartype() == BoolType) wcopy(); else
						if (instr[inpos] == '(') {
							wcopy();
							if (!fle()) return 0;
							ReadWordS();
							if (instr[inpos] != ')') return 0;
							inpos++;
							put_char(')');
						}
						else return 0;
		}
		ReadWordS();
		k = funckeyW();
		if (k == keyW_or) put_str((char *)"||"); else
			if (k == keyW_and) put_str((char *)"&&");
			else return 1;
		wskip();
	}
}
//�������� �� ������������
int asign_a_var() {
	int type = vartype();
	if (!(type == BoolType || type == RealType || type == IntType)) return 0;
	//put_char('\n');
	put_dt();
	wcopy();
	ReadWordS();
	if (instr[inpos] != ':' || instr[inpos + 1] != '=')
		return 0;
	put_char('=');
	inpos += 2;
	if (type == BoolType) {
		if (!fle()) return 0;
	}
	else
		if (!fae()) return 0;
	ReadWordS();
	if (instr[inpos] != ';') return 0;
	wcopy();
	put_char('\n');
	return 1;
}
//��������� ��������� repeat
int funcelse() {
	wskip();
	put_dt();
	put_str((char *)"do {\n");
	inc_dt();
	return 1;
}
//��������� ��������� repeat
int funcif() {
	wskip();
	dec_dt();
	put_dt();
	put_str((char *)"} if (");
	if (!fle()) return 0;
	put_char(')');
	ReadWordS();
	if (instr[inpos] != ';') return 0;
	inpos++;
	put_str((char *)";\n");
	return 1;
}
//��������� ����� ����������
int funcbegin(int gl) {
	int rep_n = 0;
	if (gl != 3) wskip();
	if (gl == 1) put_str((char *)"\n\nvoid main()\n");
	if ((gl != 2) || (gl != 3)) {
		put_dt();
		put_str((char *)"{\n");
		inc_dt();
	}
	int b;
	do {
		b = 1;
		ReadWordS();
		if (find_Comment()) {
			fin_Comment();
			continue;
		}
		switch (funckeyW()) {
		case keyW_begin:
			koper1++;
			if (!funcbegin(0)) return 0;
			koper2++;
			break;
		case keyW_read:
			koper1++;
			if (!funcread(0)) return 0;
			koper2++;
			break;
		case keyW_procedure:
			if (!funcprocedure()) return 0;
			break;
		case keyW_end:
			koper1++;
			if (gl == 3) return 3;
			wskip();
			dec_dt();
			put_dt();
			put_str((char *)"}\n");
			ReadWordS();
			if (gl == 1 && instr[inpos] == '.' ||
				gl != 1 && instr[inpos] == ';') {
				wskip();
				koper2++;
				return 1;
			}
			else
			{
				wskip();
				return 0;
			}
		case 0:
			if (!asign_a_var()) return 0; //������������
			break;
		default:
			return 0;
		}
	} while (b);
	return 1;
}
//������� ������� �������������� �������
//�������������� ������
int Translate()
{
	int b;
	int k;
	koper1 = koper2 = 0;
	put_str((char *)"#include <stdio.h>\n\n"); //��� scanf � printf
	do {
		b = 1;
		ReadWordS();
		k = funckeyW(); //�������� �� �������� �����
		if (k == keyW_var) { //������ ����� ����������
			koper1++;
			if (!funcvar(1)) {
				Errore_oper();
			}
			else koper2++;
		}
		else
			if (k == keyW_type) { //���� �������� �����
				koper1++;
				if (!funcrecord()) {
					Errore_oper();
				}
				else koper2++;
			}
			else
				if (k == keyW_procedure) { //���������
					if (!funcprocedure()) {
						Errore_oper();
					}
				}
				else
					if (k == keyW_begin) { //������ ����� ����������
						if (!funcbegin(1)) {
							Errore_oper();
						}
						b = 0;
					}
					else
						if (find_Comment()) fin_Comment(); //�����������
						else {
							koper1++;
							Errore_oper();
						};
	} while (b == 1);
	if (instr[inpos] != '.') return 0; //����� � ����� ���������
	inpos++;
	return 1;
}
void main()
{
	setlocale(LC_ALL, "Russian");
	char s[128];
	printf("������� ��� �����(� �����������) � ����� �� ����� Pascal:");
	scanf("%s", s);
	if (!ReadText(s))
	{
		printf("\n������ �������� �����!");
	}
	vars = NULL;
	lvars = NULL;
	uvars = cuservars;
	Translate();
	freevars(1);
	printf("\n������� ��� ��������� ������ ����������� .�:");
	scanf("%s", s);
	if (!PutText(s))
	{
		printf("\n������ �������� �����!");
	}
	printf("\n���������� ����������"
		" � �������� �����: %d", koper1);
	printf("\n���������� ����������"
		" � ���������� �����: %d", koper2);
	printf("\n���������� ��������� ����������"
		" ������� �� ���� ����������: %d", ker);
	printf("\n\n��������� �������� � �����: %s", s);
	fclose(file_in);
	fclose(file_out);
	while (!_kbhit());
}

