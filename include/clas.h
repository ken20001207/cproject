#ifndef _CLAS_H
#define _CLAS_H
#include "data.h"

int clas_creat_stud(struct College *op, struct Class *cp, char *studentnumber,
                    char *studentname, int studentgender);  //创建学生

int clas_delete_stud(struct Class *cp, char *studentnumber);  //删除学生

int clas_check_stud(struct Class *cp,
                    char *stud_number);  //查看是否属于自己班级的同学

int input_score(struct Score_node *rpn, char *schoolnumber, char *titles,
                int thescore);  //录入同学成绩

int clas_event_exam(struct Application *ap, char *studentnumber,
                    int thestatues);  //批准/拒绝同学

#endif
