#include "face.h"

void Face::push(Loop* lp) {

	if (this->Floop == NULL) this->Floop = lp;
	else {

		Loop* tmp = this->Floop;
		while (tmp->nextL != NULL) tmp = tmp->nextL;
		tmp->nextL = lp;
		lp->preL = tmp;

	}

}