#include "coll.h"

#include "data.h"
#include "general.h"
#include "total.h"

int coll_creat_clas(struct College *op, char *classid) {  //创建班级
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
    return 0;  //表示成功
}
/*

*/
int coll_delete_clas(struct College *op, char *classid) {  //删除班级
    struct Class_node *cpn;
    cpn = op->classes;
    if (cpn == NULL)
        return 1;
    else if (!strcmp(cpn->data->ClassID, classid)) {
        cpn->next->prev = NULL;
        op->classes = cpn->next;
        return 0;  //表示成功
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

int coll_event_creat(struct College *op, char *thetitle, char *therequirement) {  //创建事项
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
        while (theapplication->next != NULL) theapplication = theapplication->next;
        theapplication->next = apn;
        apn->prev = theapplication;
        apn->next = NULL;
    }
    return 0;  //表示成功
}

int coll_event_delete(struct College *op, char *thetitle) {  //删除事项
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
                return 0;  //表示成功
            }
            apn = apn->next;
        }
        if (apn->next == NULL) return 1;  //表示失败
    }
}

int coll_event_exam(struct Application *ap, char *studentnumber, int thestatues) {  //批准/拒绝同学 thestatues如果是1代表通过（用按钮决定吧
    int i;
    for (i = 0; i < ap->student_num; i++) {
        if (!strcmp(ap->applicants[i], studentnumber) && !strcmp(ap->statuses[i], "class_passed")) {
            if (thestatues == 1) {
                strcpy(ap->statuses[i], "college_passed");
            } else if (thestatues == 0) {
                strcpy(ap->statuses[i], "college_rejected");
            }
            return 0;  //代表失败
        }
    }
    return 1;  //代表失败
}

int coll_course_creat(struct College *op, char *codes, char *names, int studentnumber) {  //创建课程
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
    up->student_num = studentnumber;
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
    return 0;  //代表成功
}

int coll_course_creat_stud(struct Course *up, int i, char *studentnumber) {  //录入课程名单
    static char thestudentnumber[20], rpschoolnumber[20];
    strcpy(thestudentnumber, studentnumber);
    strcpy(rpschoolnumber, studentnumber);
    up->students[i] = thestudentnumber;
    static struct Score_node *rpn;
    struct Score_node *thescore;
    thescore = up->scores;
    rpn = (struct Score_node *)malloc(sizeof(struct Score_node));
    static struct Score *rp;
    rp = (struct Score *)malloc(sizeof(struct Score));
    rpn->data = rp;
    rp->schoolnumber = rpschoolnumber;
    static char rptitle[20];
    strcpy(rptitle, up->name);
    rp->title = rptitle;
    rp->score = -1;
    if (thescore == NULL) {
        thescore = rpn;
        thescore->prev = NULL;
        thescore->next = NULL;
    } else {
        rpn->prev = thescore;
        thescore->next = rpn;
        thescore = thescore->next;
        thescore->next = NULL;
    }
    return 0;
}
int coll_course_delete(struct College *op, char *codes) {  //删除课程
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
