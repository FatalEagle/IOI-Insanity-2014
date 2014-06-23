#include <fstream>
#include <cstdio>
#include <iostream>
using namespace std;

char* itoa(int value, char *str, int base = 10) {
  if (base < 2 || base > 36) { *str = '\0'; return str; }
  char *ptr = str, *ptr1 = str, tmp_c;
  int tmp_v;
  do {
    tmp_v = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789"
    "abcdefghijklmnopqrstuvwxyz"[35 + (tmp_v - value*base)];
  } while (value);
  if (tmp_v < 0) *ptr++ = '-';
  for (*ptr-- = '\0'; ptr1 < ptr; *ptr1++ = tmp_c) {
    tmp_c = *ptr;
    *ptr-- = *ptr1;
  }
  return str;
}

//make files a tad smaller by printing symbols in base-36
inline string b36(int x) {
  char s[6];
  return string(itoa(x, s, 36));
}

const int MAXN = 10000;

void gen11() {
/* Generates one big cycle

<essay> ::= <1>
<1> ::= <2>
<2> ::= <3>
...
<9999> ::= <essay>
*/
  freopen("testdata/11.in", "w", stdout);
  cout << "<essay> ::= <1>\n";
  for (int i = 1; i < MAXN - 1; i++)
    cout << '<' << b36(i) << "> ::= <" << b36(i + 1) << ">\n";
  cout << '<' << b36(MAXN - 1) << "> ::= <essay>\n";
}

void gen12() {
/*
Stress test: Generates one big linked list where each node has 450 a's

<essay> ::= <1>
<1> ::= <2> aaaaaaaaaa...(450 times)
<2> ::= <3> aaaaaaaaaa...(450 times)
...
<9999> ::= aaaaaaaaaa...(450 times)

Answer: 450 * 9999 = 4499550
*/
  freopen("testdata/12.in", "w", stdout);
  cout << "<essay> ::= <1>\n";
  for (int i = 1; i < MAXN - 1; i++)
    cout << '<' << b36(i) << "> ::= <" << b36(i + 1) << "> " << string(450, 'a') << "\n";
  cout << '<' << b36(MAXN - 1) << "> ::= " << string(450, 'a') << "\n";
}

void gen13() {
/*
Like the previous, except <essay> is defined as ALL of the symbols.

Answer: 450 * 9999 + 450 * 9998 + ... + 450 * 1
      = 450 * (9999 + 9998 + 9997 + ... + 1)
      = 450 * 4499550  
      = 497749846 (mod 10^9 + 7)
*/
  freopen("testdata/13.in", "w", stdout);
  cout << "<essay> ::=";
  for (int i = 1; i < MAXN; i++) {
    cout << " <" << b36(i) << ">";
    if (i % 130 == 0) cout << "\n"; //1000 chars per line to be nice
  }
  cout << "\n";
  for (int i = 1; i < MAXN - 1; i++)
    cout << '<' << b36(i) << "> ::= <" << b36(i + 1) << "> " << string(450, 'a') << "\n";
  cout << '<' << b36(MAXN - 1) << "> ::= " << string(450, 'a') << "\n";
}


void gen15() {
/*
<essay> ::= <1>
<1> ::= <2> <3> <4> <5> ... <1000> "x"
<2> ::= <3> <4> <5> ... <1000> "x"
...
<999> ::= <1000> "x"
<1000> ::= "x"
*/
  freopen("testdata/15.in", "w", stdout); 
  cout << "<essay> ::= <1>\n";
  for (int i = 1; i <= 1000; i++) {
    cout << "<" << b36(i) << "> ::=";
    for (int j = i + 1; j <= 1000; j++) {
      cout << " <" << b36(j) << ">";
      if (j % 130 == 0) cout << "\n";
    }
    cout << " x\n";
  }
}

void gen_worst(string num, int SIZE) {
/*
<essay> ::= <1>
<1> ::= <1> | <2> | <3> | <4> | ... | <SIZE>
<2> ::= <1> | <2> | <3> | <4> | ... | <SIZE>
...
<SIZE> ::= <1> | <2> | <3> | <4> | ... | <SIZE> | x
*/
  freopen(("testdata/" + num + ".in").c_str(), "w", stdout);
  cout << "<essay> ::= <1>\n";
  for (int i = 1; i <= SIZE; i++) {
    cout << "<" << b36(i) << "> ::=";
    for (int j = 1; j <= SIZE; j++) {
      if (j > 1) cout << " | ";
      cout << " <" << b36(j) << ">";
      if (j % 100 == 0) cout << "\n";
    }
    cout << "\n";
  }
  cout << " | x\n";
}

void gen19() { gen_worst("19", 500); }
void gen20() { gen_worst("20", 1000); }

int main() {
  gen19();
  gen20();
  return 0;
}