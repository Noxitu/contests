// Pamietaj, ze prawdziwa biblioteka nie zalaczy assert.h ani stdio.h
// za Ciebie!

#ifndef PALINDROMY_H
#define PALINDROMY_H

#include <assert.h>
#include <stdio.h>

// SmallSample

int __SmallSampleGetLength() {
  return 6;
}

char __SmallSampleGetLetter(int i) {
  static const char* str = "kajakk";
  assert(i >= 0 && i < 6);
  return str[i];
}

// LargeSample

const int __MAXN = 500 * 1000 * 1000;

int __LargeSampleGetLength() {
  return __MAXN;
}

char __LargeSampleGetLetter(int i) {
  assert(i >= 0 && i < __MAXN);
  return 'a';
}
  
int __lib_initialized = 0;
int (*__GetLength_ptr)();
char (*__GetLetter_ptr)(int);

static void __LibInit() {
  int TID;
  scanf("%d", &TID);
  assert(1 <= TID && TID <= 2);
  if (TID == 1) {
    __GetLength_ptr = __SmallSampleGetLength;
    __GetLetter_ptr = __SmallSampleGetLetter;
  } else if (TID == 2) {
    __GetLength_ptr = __LargeSampleGetLength;
    __GetLetter_ptr = __LargeSampleGetLetter;
  }
  __lib_initialized = 1;
}

long long GetLength() {
  if (!__lib_initialized) {
    __LibInit();
  }
  return (*__GetLength_ptr)();
}

char GetLetter(long long i) {
  if (!__lib_initialized) {
    __LibInit();
  }
  return (*__GetLetter_ptr)((int)i);
}

#endif // PALINDROMY_H
