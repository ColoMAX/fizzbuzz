#ifndef FUZZBUZZ_H__1940
#define FUZZBUZZ_H__1940

// void * == struct fizzbuzz_obj *
typedef int (*rdPtrList_t)(const void*);

#define FIZZ_bit (0)
#define BUZZ_bit (FIZZ_bit + 1)

struct fizzbuzz_obj {
  int value;
  enum {
    BASE = 0,
    FIZZ = 1 << FIZZ_bit,
    BUZZ = 1 << BUZZ_bit,
    FIZZBUZZ = ((1 << FIZZ_bit) | (1 << BUZZ_bit)),
    NUMSTATES
  } state;
  int baseGame;
  int* history;
  int historyUsed;
  int historyLen;
  //    array of function pointers to check
  //    first number is the input number to validate
  //    second optional argument
  //    third the pointer of all previous numbers
  rdPtrList_t* checkNumber;
  char* printBuffer;
  FILE* buffFp;
  size_t printBufferSize;
};

int FzBz_init(struct fizzbuzz_obj* obj, rdPtrList_t* checkNumber);

int FzBz_check(struct fizzbuzz_obj* obj, int x);

void FzBz_print(const struct fizzbuzz_obj* x);
void FzBz_flush(struct fizzbuzz_obj* obj);
int FzBz_destroy(struct fizzbuzz_obj* obj);
#endif