#ifndef _MAIN_H
#define _MAIN_H
#include "data.h"

struct Application *event_find(struct College *op, char *thetitle);

char *view_requ(struct Application *ap);

struct College *total(int college);

#endif
