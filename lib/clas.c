#include "clas.h"

#include "general.h"
#include "total.h"

int clas_creat_stud(struct College *op, struct Class *cp, char *studentnumber,
                    char *studentname, int studentgender) {  //创建学生
    static struct Student_node *spn;
    static struct Student *sp;
    spn = (struct Student_node *)malloc(sizeof(struct Student_node));
    sp = (struct Student *)malloc(sizeof(struct Student_node));
    spn->data = sp;
    static char thecollege[20];
    strcpy(thecollege, op->name);
    sp->college = thecollege;
    sp->gender = studentgender;
    static char theclass[20];
    strcpy(theclass, cp->ClassID);
    sp->hisclass = theclass;
    static char thepassword[20];
    strcpy(thepassword, studentnumber);
    sp->md5 = thepassword;
    static char thename[20];
    strcpy(thename, studentname);
    sp->name = thename;
    static char thestudentnumber[20];
    strcpy(thestudentnumber, studentnumber);
    sp->schoolnumber = thestudentnumber;
    struct Student_node *thestudents = cp->students;
    if (thestudents == NULL) {
        thestudents = spn;
        thestudents->prev = NULL;
        thestudents->next = NULL;
    } else {
        while (thestudents->next != NULL) {
            thestudents = thestudents->next;
        }
        thestudents->next = spn;
        spn->prev = thestudents;
        spn->next = NULL;
    }
    return 0;  //添加成功
}
/*
//先用struct Class *getClassByID(char *class_id)得到cp，也就是班级的指针
char studentnumber[20],studentname[20];
if(textbox(GenUIID(0), x+20+5+w, y, 30, h, studentnumber,
sizeof(studentnumber)));//从键盘输入学号（这段仅是示意 if(textbox(GenUIID(0),
x+20+5+w, y, 30, h, studentname,
sizeof(studentname)));//从键盘输入学号（这段仅是示意
clas_creat_stud(op,cp,studentnumber,studentname);//返回0代表成功
*/
int clas_delete_stud(struct Class *cp, char *studentnumber) {  //删除学生
    struct Student_node *spn = cp->students;
    if (spn == NULL)
        return 1;
    else if (!strcmp(spn->data->schoolnumber, studentnumber)) {
        if (spn->next != NULL) spn->next->prev = NULL;
        cp->students = spn->next;
        return 0;
    } else {
        while (spn->next != NULL) {
            if (!strcmp(spn->next->data->schoolnumber, studentnumber)) {
                spn->next = spn->next->next;
                if (spn->next != NULL) spn->next->prev = spn;
                return 0;  // 0表示成功，1表示失败
            }
            spn = spn->next;
        }
        if (spn->next == NULL) return 1;
    }
}

int clas_check_stud(struct Class *cp,
                    char *stud_number) {  //查看是否属于自己班级的同学
    struct Student_node *spn = cp->students;
    while (spn != NULL) {
        if (!strcmp(spn->data->schoolnumber, stud_number)) return 0;  // 0表示是
        spn = spn->next;
    }
    return 1;
}

int input_score(struct Score_node *rpn, char *schoolnumber, char *titles,
                int thescore) {  //录入同学成绩
    char *thetitle;
    while (rpn != NULL) {
        if (!strcmp(rpn->data->schoolnumber, schoolnumber)) {
            rpn->data->score = thescore;
            thetitle = (char *)malloc(50);
            strcpy(thetitle, titles);
            rpn->data->title = thetitle;
            return 0;  //表示成功输入
        }
        rpn = rpn->next;
    }
    return 1;
}

//在之前用struct Course *getCourseByCode(char *course_code) 得到up，然后输入up->
// scores作为rpn

int clas_event_exam(struct Application *ap, char *studentnumber,
                    int thestatues) {  //批准/拒绝同学
    int i;
    for (i = 0; i < ap->student_num; i++) {
        if (!strcmp(ap->applicants[i], studentnumber)) {
            if (thestatues == 1) {
                strcpy(ap->statuses[i], "class_passed");
            } else if (thestatues == 0) {
                strcpy(ap->statuses[i], "class_rejected");
            }
            return 0;  //表示成功输入
        }
    }
    return 1;
}
