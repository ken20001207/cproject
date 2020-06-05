#ifndef _STUD_H
#define _STUD_H
#include "data.h"

int stud_input_information(struct Student *sp, char *thename, int thegender);

int stud_apply(struct Student *sp, struct Application *ap);

char *stud_view_refults(struct Student *sp, struct Application *ap);

int stud_view_scores(struct Student *sp, char *codes);

int stud_change_password(struct Student *sp, char *password1, char *password2);

int stud_load(struct Student *sp, char *password);

char **student_check_score(struct Course_node *upn, struct Student *sp);

#endif
