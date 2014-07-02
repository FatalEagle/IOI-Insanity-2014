#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
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

void gen14(int COLS = 2000, int LEN = 200) {
/* Multiple connected components, joined by <essay>

<essay> ::= <1>  <2>  <3>  <4>
<1> ::= <10001> a <10002> a ... <10000 + COLS> times
<10001> ::= a
<10002> ::= a
...
<2> ::= <20001> a <20002> a ... <20000 + COLS> times
<10001> ::= a
<20002> ::= a
...

answer: 4*(2*COLS*LEN)
      = 4*2*2000*200 = 3200000
*/
  freopen("testdata/14.in", "w", stdout);
  cout << "<essay> ::= <1> <2> <3> <4>\n";
  for (int i = 1; i <= 4; i++) {
    cout << "<" << i << "> ::=";
    for (int j = 1; j <= COLS; j++) {
      cout << " <" << b36(i*10000 + j) << "> " << string(LEN, 'a');
      cout << "\n";
    }
    cout << "\n";
    for (int j = i*10000; j <= i*10000 + COLS; j++) {
      cout << "<" << b36(j) << "> ::= " << string(LEN, 'a') << "\n";
    }
  }
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

void gen16(int SIZE = 1000) {
/*
<essay> ::= <1>
<1> ::= <2> <2> <2> <2> ... (1000 times)
<2> ::= <3> <3> <3> <3> ... (1000 times)
...
<999> ::= <1000> <1000> ... (1000 times)
<1000> ::= "a"

answer = 1000^999 mod 1000000007
       = 247524702 (according to WolframAlpha)
*/
  freopen("testdata/16.in", "w", stdout);
  cout << "<essay> ::= <1>\n";
  for (int i = 1; i < SIZE; i++) {
    cout << "<" << b36(i) << "> ::=";
    for (int j = 1; j <= SIZE; j++) {
      cout << " <" << b36(i + 1) << ">";
      if (j % 101 == 0) cout << "\n";
    }
    cout << "\n";
  }
  cout << "<" << b36(1000) << "> ::= a\n";
}

void gen_rand(string num, int SIZE = 9999) {
  srand(time(0));
  //each line has random symbols and |'s'
  freopen(("testdata/" + num + ".in").c_str(), "w", stdout);
  for (int i = 1; i <= SIZE; i++) {
    string line;
    while (line.size() < 495) {
      if (rand() % 2) { //literal
        line += " aaaaa";
      } else {
        if (rand() % 2) { //AND
          line += " <" + b36(rand() % SIZE + 1) + ">";
        } else if (!line.empty()) { //OR
          line += " | <" + b36(rand() % SIZE + 1) + ">";
        }
      }
    }
    cout << "<" << b36(i) << "> ::=" << line << "\n";
  }
  cout << "<essay> ::= <1>\n";
}

void gen17() { gen_rand("17"); }
void gen18() { gen_rand("18"); }

void gen19(int SIZE = 9000, int COLS = 50) {
/*
<essay> ::= <1>
<1> ::= <2> x | <3> x | <4> x | <5> x ... (up to <1 + COLS> times)
<2> ::= <3> x | <4> x | <5> x | <6> x ... (up to <2 + COLS> times)
...
<SIZE-1> ::= <SIZE> x
<SIZE> ::= x

answer = (SIZE / COLS + 1) % MOD = 1000000007

for SIZE = 9000, COLS = 50:
  answer = 9000 / 50 + 1 = 181
*/
  freopen("testdata/19.in", "w", stdout);
  cout << "<essay> ::= <1>\n";
  for (int i = 1; i < SIZE; i++) {
    cout << "<" << b36(i) << "> ::=";
    for (int j = i + 1; j <= min(i + COLS, SIZE); j++) {
      if (j > i + 1) cout << " |";
      cout << " <" << b36(j) << "> x";
    }
    cout << "\n";
  }
  cout << "<" << b36(SIZE) << "> ::= x\n";
}

void gen20(int SIZE = 850) {
/*
<essay> ::= <1>
<1> ::= <1> | <2> | <3> | <4> | ... | <SIZE>
<2> ::= <1> | <2> | <3> | <4> | ... | <SIZE>
...
<SIZE> ::= <1> | <2> | <3> | <4> | ... | <SIZE> | x
*/
  freopen("testdata/20.in", "w", stdout);
  cout << "<essay> ::= <1>\n";
  for (int i = 1; i <= SIZE; i++) {
    cout << "<" << b36(i) << "> ::=";
    for (int j = 1; j <= SIZE; j++) {
      if (j > 1) cout << " |";
      cout << " <" << b36(j) << ">";
      if (j % 101 == 0) cout << "\n";
    }
    cout << "\n";
  }
  cout << " | x\n";
}

int main() {
  gen20();
  return 0;
}
