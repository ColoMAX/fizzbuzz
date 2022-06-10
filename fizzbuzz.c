
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//
#include "fizzbuzz.h"

static int check_mod(int x, int y);
static int check_mod3(const void *s_t);
static int check_mod5(const void *s_t);
static void dbgprintf(const char *, ...);

const char fzbzStr[] = "fizzbuzz\n";
static rdPtrList_t checkNumberStandard[] = {&check_mod3, &check_mod5, NULL};

static int check_mod(int x, int y) { return x % y == 0; }

static int check_mod3(const void *s_t) {
  return check_mod(((struct fizzbuzz_obj *)s_t)->value, 3) << FIZZ_bit;
}
static int check_mod5(const void *s_t) {
  return check_mod(((struct fizzbuzz_obj *)s_t)->value, 5) << BUZZ_bit;
}

int FzBz_init(struct fizzbuzz_obj *obj, rdPtrList_t *checkNumber) {
  // if (!((uintptr_t)obj | (uintptr_t)checkNumber)) {
  //   return -1;
  // }
  int err = 0;
  // if (!obj) {
  //   err = -1;
  // }
  obj->value = 0;
  obj->state = NUMSTATES;
  obj->baseGame = checkNumber ? 0 : 1;
  obj->historyLen = 16;
  obj->historyUsed = 0;
  obj->history = reallocarray(NULL, obj->historyLen, sizeof(int));
  obj->checkNumber = checkNumber ? checkNumber : checkNumberStandard;
  if (!obj->history) {
    err = -2;
  } else {
    obj->printBufferSize = 32;
    obj->buffFp = open_memstream(&(obj->printBuffer), &(obj->printBufferSize));
    if (!obj->buffFp) {
      err = -3;
      free(obj->history);
    }
  }
  return err;
}

int FzBz_destroy(struct fizzbuzz_obj *obj) {
  int ret = fclose(obj->buffFp);
  free(obj->history);
  free(obj->printBuffer);

  return ret;
}
int FzBz_check(struct fizzbuzz_obj *obj, int x) {
  // multiple of 3 FIZZ
  // mutliple of 5 BUZZ
  // multiple of both FIZZBUZZ
  // otherwise just the number
  if (!obj) {
    return -1;
  }
  obj->value = x;
  obj->state = BASE;
  if (obj->historyUsed > obj->historyLen) {
    const size_t newLen = obj->historyLen * 2;
    int *newHistory = reallocarray(obj->history, newLen, sizeof(int));
    if (!newHistory) {
      free(obj->history);
      exit(-ENOMEM);
    }
    obj->history = newHistory;
    obj->historyLen = newLen;
  }
  obj->history[obj->historyUsed++] = x;

  dbgprintf("%d: ", x);
  obj->value = x;
  if (obj->baseGame) {
#if 1
    size_t checkFnc = 0;
    while (obj->checkNumber[checkFnc]) {
      int res = (*obj->checkNumber[checkFnc])(obj);
      dbgprintf("%d ", res);
      obj->state |= res;
      checkFnc++;
    }
    dbgprintf("\n");
#else
    if (x % 3 == 0) obj->state |= FIZZ_bit;
    if (x % 5 == 0) obj->state |= BUZZ_bit;
#endif
  } else {
    // similair as basegame, but define more outputs
    free(obj->history);
    exit(-ENOSYS);
  }
  return 0;
}

void FzBz_flush(struct fizzbuzz_obj *obj) {
  fflush(obj->buffFp);  // or flush
  if (islower(obj->printBuffer[0])) {
    obj->printBuffer[0] += ('A' - 'a');
  }
  fwrite(obj->printBuffer, sizeof(obj->printBuffer[0]), obj->printBufferSize,
         stdout);
  putchar('\n');
}

void FzBz_print(const struct fizzbuzz_obj *obj) {
  switch (obj->state) {
    case BASE:
      fprintf(obj->buffFp, "%d\n", obj->value);
      break;
    case FIZZ:
      fprintf(obj->buffFp, "%.4s\n", fzbzStr);
      break;
    case BUZZ:
      fputs(&fzbzStr[4], obj->buffFp);
      break;
    case FIZZBUZZ:
      fputs(fzbzStr, obj->buffFp);
      break;
    default:
      break;
  }
}

static void dbgprintf(const char *fmt, ...) {
#if 0
  int ret;
  va_list ap;
  va_start(ap, fmt);
  ret = vfprintf(stderr, fmt, ap);
  va_end(ap);
  // return ret;
#else

#endif
}