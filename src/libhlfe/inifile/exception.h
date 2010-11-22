/* exception.h */
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#ifndef __EXCEPTION__
#define __EXCEPTION__  1

/* exceptions de base */

#define ERR_MALLOC     10
#define NULL_POINTEUR  11
#define ERR_FOPEN      12

/*prototypes*/

/* definition des structures de donnees de gestion de la pile des contextes
   et des macros TRY, CATCH, et ENDTRY */


/* liste chainee des contextes */
typedef struct except_ctx
{
	jmp_buf ctx;
	struct except_ctx* super;
} except;
extern except* _exceptions;

/* les macros */
#define TRY { \
  int exc; \
  push_exc(); \
  exc = setjmp(_exceptions->ctx); \
  switch(exc){ \
  case 0:

#define CATCH(X) \
  break; \
  case(X): \
  exc = 0;

#define ENDTRY \
  break; \
  default:; \
  } \
  pop_exc(exc); \
  }

#define THROW(X) longjmp(_exceptions->ctx, X)


void push_exc();
void pop_exc(int);

#endif
