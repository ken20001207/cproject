#include "general.h"
#include "data.h"
#include "coll.h"
#include "total.h"

int coll_creat_clas(struct College *op, char *classid) {  //�����༶
    static struct Class_node *cpn;
    cpn = (struct Class_node *)malloc(sizeof(struct Class_node));
    struct Class_node *theclasses;
    theclasses = op->classes;
    static struct Class *cp;
    cp = (struct Class *)malloc(sizeof(struct Class));
    static char theclassid[20];
    strcpy(theclassid, classid);
    cp->ClassID = theclassid;
    cp->students = NULL;
    cpn->data = cp;
    if (theclasses == NULL) {
        theclasses = cpn;
        cpn->prev = NULL;
        cpn->next = NULL;
    } else {
        while (theclasses->next != NULL) theclasses = theclasses->next;
        theclasses->next = cpn;
        cpn->prev = theclasses;
        cpn->next = NULL;
    }
    return 0;  //��ʾ�ɹ�
}

int coll_delete_clas(struct College *op, char *classid) {  //ɾ���༶
    struct Class_node *cpn;
    cpn = op->classes;
    if (cpn == NULL)
        return 1;
    else if (!strcmp(cpn->data->ClassID, classid)) {
        cpn->next->prev = NULL;
        op->classes = cpn->next;
        return 0;  //��ʾ�ɹ�
    } else {
        while (cpn->next != NULL) {
            if (!strcmp(cpn->next->data->ClassID, classid)) {
                cpn->next = cpn->next->next;
                cpn->next->prev = cpn;
                return 0;
            }
            cpn = cpn->next;
        }
        if (cpn->next == NULL) return 1;
    }
}

int coll_event_creat(struct College *op, char *thetitle,
                     char *therequirement) {  //��������
    static struct Application_node *apn;
    struct Application_node *theapplication;
    apn = (struct Application_node *)malloc(sizeof(struct Application_node));
    static struct Application *ap;
    ap = (struct Application *)malloc(sizeof(struct Application));
    apn->data = ap;
    static char thetitles[40];
    strcpy(thetitles, thetitle);
    ap->title = thetitles;
    static char therequirements[40];
    strcpy(therequirements, therequirement);
    ap->requirement = therequirements;
    ap->student_num = 0;
    static char *theapplicants[100], *thestatuses[100];
    ap->applicants = theapplicants;
    ap->applicants[0] = NULL;
    ap->statuses = thestatuses;
    ap->statuses[0] = NULL;
    theapplication = op->applications;
    if (theapplication == NULL) {
        theapplication = apn;
        apn->prev = NULL;
        apn->next = NULL;
    } else {
        while (theapplication->next != NULL)
            theapplication = theapplication->next;
        theapplication->next = apn;
        apn->prev = theapplication;
        apn->next = NULL;
    }
    return 0;  //��ʾ�ɹ�
}

int coll_event_delete(struct College *op, char *thetitle) {  //ɾ������
    struct Application_node *apn;
    apn = op->applications;
    if (apn == NULL)
        return 1;
    else if (!strcmp(apn->data->title, thetitle)) {
        apn->next->prev = NULL;
        op->applications = apn->next;
        return 0;
    } else {
        while (apn->next != NULL) {
            if (!strcmp(apn->data->title, thetitle)) {
                apn->next = apn->next->next;
                apn->next->prev = apn;
                return 0;  //��ʾ�ɹ�
            }
            apn = apn->next;
        }
        if (apn->next == NULL) return 1;  //��ʾʧ��
    }
}

int coll_event_exam(struct Application *ap, char *studentnumber,
                    int thestatues) {  //ѧԺ��������ͬѧ
    int i;
    for (i = 0; i < ap->student_num; i++) {
        if (!strcmp(ap->applicants[i], studentnumber) &&
            !strcmp(ap->statuses[i], "class_passed")) {
            if (thestatues == 1) {  // 1����ͨ��
                strcpy(ap->statuses[i], "college_passed");
            } else if (thestatues == 0) {
                strcpy(ap->statuses[i], "college_rejected");
            }
            return 0;  //����ʧ��
        }
    }
    return 1;  //����ʧ��
}

int coll_course_creat(struct College *op, char *codes, char *names) {  //�����γ�
    static struct Course_node *upn;
    struct Course_node *thecourses;
    thecourses = op->courses;
    upn = (struct Course_node *)malloc(sizeof(struct Course_node));
    static struct Course *up;
    up = (struct Course *)malloc(sizeof(struct Course));
    static char thecodes[20];
    strcpy(thecodes, codes);
    up->code = thecodes;
    static char thenames[20];
    strcpy(thenames, names);
    up->name = thenames;
    up->student_num = 0;
    up->scores = NULL;
    static char *thestudents[200];
    up->students = thestudents;
    upn->data = up;
    if (thecourses == NULL) {
        thecourses = upn;
        thecourses->prev = NULL;
        thecourses->next = NULL;
    } else {
        thecourses->next = upn;
        upn->prev = thecourses;
        thecourses = thecourses->next;
        thecourses->next = NULL;
    }
    return 0;  //�����ɹ�
}

int coll_course_creat_stud(struct Course *up,
                           char *studentnumber) {  //录入课程名单
    char *thestudentnumber;
    thestudentnumber = (char *)malloc(20);
    strcpy(thestudentnumber, studentnumber);
    up->students[up->student_num] = thestudentnumber;
    up->student_num++;
    return 0;
}

int coll_course_delete(struct College *op, char *codes) {  //ɾ���γ�
    struct Course_node *upn;
    upn = op->courses;
    if (upn == NULL)
        return 1;
    else if (!strcmp(upn->data->code, codes)) {
        upn->next->prev = NULL;
        op->courses = upn->next;
        return 0;
    } else {
        while (upn->next != NULL) {
            if (!strcmp(upn->next->data->code, codes)) {
                upn->next = upn->next->next;
                upn->next->prev = upn;
                return 0;
            }
            upn = upn->next;
        }
        if (upn->next == NULL) return 1;
    }
}
