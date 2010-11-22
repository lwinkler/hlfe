/* exception.c */
/* fonctions de manipulation de la pile */

#include "exception.h"

except *_exceptions = NULL;

/* empile */

void push_exc() {
	except* layer;
	layer = (except*) malloc(sizeof(except));
	if (layer == NULL) {
		fprintf(stderr,"[push_exc()]Erreur d'allocation memoire pour la pile");
	}
	layer->super = _exceptions;
	_exceptions = layer;
}

/* depile */

void pop_exc(int ex) {
	except* super;
	super = _exceptions->super;
	free(_exceptions);
	_exceptions = super;
	if (ex) {
		if (_exceptions) {
			longjmp(_exceptions->ctx, ex);
		}
		else {
			fprintf(stderr, "Exception non geree : %d\n", ex);
			exit(-1);
		}
	}
}
