#include "stud.h"

#include "data.h"
#include "general.h"
#include "md5.h"
#include "total.h"

int stud_input_information(struct Student *sp, char *thename, int thegender) {  //输入信息
    sp->gender = thegender;
    static char names[20];
    strcpy(names, thename);
    sp->name = names;
    return 0;
}

/*
//在此之前先用struct Student *getStudentBySchoolNumber(char *schoolnumber)函数得到sp ,也就是指向该同学的指针 
char thename[20];
if(textbox(GenUIID(0), x+20+5+w, y, 30, h, thename, sizeof(thename)));//从键盘输入名字（这段仅是示意 
//thegender用选项？
if(!stud_input_information(sp,thename,thegender));//表示输入成功 
*/

int stud_apply(struct Student *sp, struct Application *ap) {  //申请事项
    static char thenumber[20];
    strcpy(thenumber, sp->schoolnumber);
    ap->applicants[ap->student_num] = thenumber;
    static char thestatuses[30];
    strcpy(thestatuses, "Not Examined");
    ap->statuses[ap->student_num] = thestatuses;
    ap->student_num++;
    return 0;
}

/*
//在此之前先用struct Student *getStudentBySchoolNumber(char *schoolnumber)函数得到sp ,也就是指向该同学的指针
//先用struct Application * event_find(struct College *op,char *thetitle)找到事项位置ap
if(stud_apply(sp,apn));//添加成功 
*/

char *stud_view_refults(struct Student *sp, struct Application *ap) {  //查看申请结果
    int i;
    for (i = 0; i < ap->student_num; i++) {
        if (!strcmp(ap->applicants[i], sp->schoolnumber)) {
            return ap->statuses[i];
        }
    }
    return NULL;
}

/*
//在此之前先用struct Student *getStudentBySchoolNumber(char *schoolnumber)函数得到sp ,也就是指向该同学的指针
//先用struct Application * event_find(struct College *op,char *thetitle)找到事项位置ap
char *resu;
resu = stud_view_refults(sp,ap);//找到存储事件要求的字符串指针 
if(resu != NULL) DrawTextString(resu);//输出结果 
else  DrawTextString("错误发生");
*/

int stud_view_scores(struct Student *sp, char *codes) {  //查看成绩
    struct Course *up = getCourseByCode(codes);
    struct Score_node *rpn = up->scores;
    while (rpn != NULL) {
        if (!strcmp(rpn->data->schoolnumber, sp->schoolnumber)) {
            return rpn->data->score;  //-1表示成绩未录入
        }
        rpn = rpn->next;
    }
    if (rpn == NULL) return -2;  //表示未修读该课程
}

/*
char codes[20];
int thescore;
if(textbox(GenUIID(0), x+20+5+w, y, 30, h, codes, sizeof(codes)));//从键盘输入编号（这段仅是示意  
thescore = stud_view_scores(sp,codes);//返回的就是成绩 
if(thescore == -1) DrawTextString("未录入成绩");
else if (thescore == -2)DrawTextString("未修读该课程")
else{
	if(thescore == 100){
		char thescores[4] = "100";
	}
	else if(thescore > 9){
		char thescores[3];
		thescores[2] = '\0';
		thescores[1] = thescore % 10 + '0';
		thescore = thescore / 10;
		thescores[0] = thescore + '0';
	}
	else{
		char thescores[2];
		thescores[1] = '\0';
		thescores[0] = thescore + '0';
	}
	DrawTextString(thescores);
} 
*/

int stud_change_password(struct Student *sp, char *password1, char *password2) {  //修改密码
    if (!checkPassword(sp->md5, password1)) {
        return 1;  //表示密码错误
    } else {
        static char newpassword[20];
        strcpy(newpassword, password2);
        sp->md5 = newpassword;
        return 0;
    }
}

/*
char password1[20],password2[20];
if(textbox(GenUIID(0), x+20+5+w, y, 30, h, password1, sizeof(password1)));//从键盘输入原密码（这段仅是示意 
if(textbox(GenUIID(0), x+20+5+w, y, 30, h, password2, sizeof(password2)));//从键盘输入现密码（这段仅是示意 
if(stud_change_password(sp,password1,password2));//0表示已修改 
*/

int stud_load(struct Student *sp, char *password) {  //学生登录
    if (checkPassword(sp->md5, password)) {
        return 0;
    } else {
        return 1;
    }
}

/* 
char password[20];
if(textbox(GenUIID(0), x+20+5+w, y, 30, h, password, sizeof(password)));//从键盘输入密码（这段仅是示意 
if(stud_load(sp,password));//0表示登陆成功 
*/
