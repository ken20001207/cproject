#ifndef _COLL_H
#define _COLL_H

int coll_creat_clas(struct College *op,char *classid);//�����༶

int coll_delete_clas(struct College *op,char *classid);//ɾ���༶

int coll_event_creat(struct College *op,char *thetitle,char *therequirement);//��������

int coll_event_delete(struct College *op,char *thetitle);//ɾ������

int coll_event_exam(struct Application *ap,char *studentnumber,int thestatues);//ѧԺ��������ͬѧ

int coll_course_creat(struct College *op,char *codes,char *names);//�����γ� 

int coll_course_creat_stud(struct Course *up,char *studentnumber);//¼��γ����� 

int coll_course_delete(struct College *op,char *codes);//ɾ���γ�

#endif
