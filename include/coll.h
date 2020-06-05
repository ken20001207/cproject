#ifndef _COLL_H
#define _COLL_H

int coll_creat_clas(struct College *op, char *classid);  //创建班级

int coll_delete_clas(struct College *op, char *classid);  //删除班级

int coll_event_creat(struct College *op, char *thetitle, char *therequirement);  //创建事项

int coll_event_delete(struct College *op, char *thetitle);  //删除事项

int coll_event_exam(struct Application *ap, char *studentnumber, int thestatues);  //批准/拒绝同学（学院层面

int coll_course_creat(struct College *op, char *codes, char *names, int studentnumber);  //创建课程

int coll_course_creat_stud(struct Course *up, int i, char *studentnumber);  //录入课程名单

int coll_course_delete(struct College *op, char *codes);  //删除课程

#endif
