#include "solid.h"

void Solid::push(Face* f) {

	if (this->Sfaces == NULL) this->Sfaces = f;
	else {

		Face* tmp = this->Sfaces;
		while (tmp->nextF != NULL) tmp = tmp->nextF;
		tmp->nextF = f;
		f->preF = tmp;

	}

}