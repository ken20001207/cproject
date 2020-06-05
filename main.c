#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "imgui.h"
#include "data.h"

static double winwidth, winheight;
static int pageID = 0;
static int timer = 0;

static struct Student *loginStudentData;

static struct Student *curr_page_student;
static struct Class *curr_page_class;
static struct Course *curr_page_course;
static struct Application *curr_page_application;

// if 1, show wrong password messange
static int wrongPassword = 0;
static char message[255];

void DisplayClear(void);
void startTimer(int id, int timeinterval);
void display(void);

void CharEventProcess(char ch) {
    uiGetChar(ch);
    display();
}

void KeyboardEventProcess(int key, int event) {
    uiGetKeyboard(key, event);
    display();
}

void MouseEventProcess(int x, int y, int button, int event) {
    uiGetMouse(x, y, button, event);
    display();
}

void TimerEventProcess(int timerID) { display(); }

void Main() {
    SetWindowTitle("College Data Manage System");
    SetWindowSize(40, 30);
    InitGraphics();

    winwidth = GetWindowWidth();
    winheight = GetWindowHeight();

    registerCharEvent(CharEventProcess);
    registerKeyboardEvent(KeyboardEventProcess);
    registerMouseEvent(MouseEventProcess);
    registerTimerEvent(TimerEventProcess);

    startTimer(timer, 50);

    // InitConsole();

    /** Load Data */
    FILE *fp;
    fp = fopen("data.txt", "r");
    readData(fp);
    fclose(fp);
}

void drawMenu() {
    static char *menuListFile[] = {"File", "Save Changes | Ctrl-S",
                                   "Exit   | Ctrl-E"};
    static char *menuListUser[] = {"Page", "Back to Index"};
    static char *menuListHelp[] = {"Help", "Show More  | Ctrl-M", "About"};
    static char *selectedLabel = NULL;

    double fH = GetFontHeight();
    double x = 0;
    double y = winheight;
    double h = fH * 1.5;
    double w = TextStringWidth(menuListHelp[0]) * 2;
    double wlist = TextStringWidth(menuListFile[1]) * 1.2;
    double xindent = winheight / 20;
    int selection;

    drawMenuBar(0, y - h, winwidth, h);

    selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile,
                         sizeof(menuListFile) / sizeof(menuListFile[0]));
    if (selection > 0) selectedLabel = menuListFile[selection];
    if (selection == 1) {
        FILE *fp;
        fp = fopen("data.txt", "w");
        exportCollegeData(fp);
        fclose(fp);
    } else if (selection == 3) {
        exit(-1);
    }

    selection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListUser,
                         sizeof(menuListUser) / sizeof(menuListUser[0]));
    if (selection > 0) selectedLabel = menuListUser[selection];
    if (selection == 1) {
        strcpy(message, "");
        pageID = 0;
    }

    menuListHelp[1] = "Show Less | Ctrl-M";
    selection =
        menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menuListHelp,
                 sizeof(menuListHelp) / sizeof(menuListHelp[0]));
    if (selection > 0) selectedLabel = menuListHelp[selection];
}

void drawIndex() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x = winwidth / 2.5;
    double y = winheight / 2 - h;
    double w = winwidth / 5;

    SetPenColor("Blue");

    if (button(GenUIID(0), x, y + 1.5 * h, w, h, "College Manager Login")) {
        pageID = 1;
    }

    if (button(GenUIID(0), x, y - 1.5 * h, w, h, "Student Login")) {
        pageID = 2;
    }

    x = winwidth / 2;
    SetPenColor("Blue");
    drawLabel(x - TextStringWidth("Current College:") / 2, y - 4 * h,
              "Current College:");
    drawLabel(x - TextStringWidth(getCollegeData()->name) / 2, y - 5 * h,
              getCollegeData()->name);
}

void drawManagerLoginPage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x = winwidth / 2.5;
    double y = winheight / 2 + 5 * h;
    double w = winwidth / 5;

    static char username[80] = "";
    static char password[80] = "";

    SetPenColor("Blue");
    drawLabel(x, y -= 2 * h, "Manager User Name");
    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, username, sizeof(username));

    SetPenColor("Blue");
    drawLabel(x, y -= 2 * h, "Manager Password");
    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, password, sizeof(password));

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Manager Login")) {
        if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
            strcpy(message, "");
            pageID = 3;
        } else {
            strcpy(message, "Username or Password is wrong.");
        }
    }

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Back to Index")) {
        strcpy(message, "");
        pageID = 0;
    }
}

void drawStudentLoginPage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x = winwidth / 2.5;
    double y = winheight / 2 + 5 * h;
    double w = winwidth / 5;

    static char schoolnumber[80] = "";
    static char password[80] = "";

    SetPenColor("Blue");
    drawLabel(x, y -= 2 * h, "School Number");
    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, schoolnumber,
            sizeof(schoolnumber));

    SetPenColor("Blue");
    drawLabel(x, y -= 2 * h, "Password");
    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, password, sizeof(password));

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Student Login")) {
        if (checkPassword(schoolnumber, password)) {
            loginStudentData = getStudentBySchoolNumber(schoolnumber);
            strcpy(message, "");
            pageID = 4;
        } else {
            strcpy(message, "School Number or Password is wrong.");
        }
    }

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Back to Index")) {
        strcpy(message, "");
        pageID = 0;
    }
}

void drawCollegeManagePage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x1 = winwidth / 12;
    double x2 = 4 * winwidth / 12;
    double x3 = 7 * winwidth / 12;
    double y = winheight / 2 + 8 * h;
    double w = 2 * winwidth / 12;

    static char classID[80] = "Class ID";
    static char courseCode[80] = "Course Code";
    static char applicationName[80] = "Application Name";

    /** Render Class list*/
    SetPenColor("Violet");
    drawLabel(x1, y -= 2 * h, "Class List");

    struct Class_node *curr_cla_node = getCollegeData()->classes;
    while (curr_cla_node != NULL) {
        SetPenColor("Blue");
        drawLabel(x1, y -= 1.5 * h, curr_cla_node->data->ClassID);

        curr_cla_node = curr_cla_node->next;
    }

    if (button(GenUIID(0), x1, y -= 2 * h, w, h, "+ Add Class")) {
        pageID = 7;
    }

    SetPenColor("Violet");
    drawLabel(x1, y -= 2 * h, "Manage Classes");

    SetPenColor("Blue");
    textbox(GenUIID(0), x1, y -= 2 * h, w, h, classID, sizeof(classID));

    if (button(GenUIID(0), x1, y -= 2 * h, w, h, "Manage")) {
        if (getClassByID(classID) != NULL) {
            curr_page_class = getClassByID(classID);
            strcpy(message, "");
            pageID = 5;
        } else {
            strcpy(message, "Please enter correct class ID!");
        }
    }

    /** Render Course List */
    y = winheight / 2 + 8 * h;

    SetPenColor("Violet");
    drawLabel(x2, y -= 2 * h, "Course List");

    struct Course_node *curr_cou_node = getCollegeData()->courses;
    while (curr_cou_node != NULL) {
        SetPenColor("Blue");
        drawLabel(x2, y -= 1.5 * h, curr_cou_node->data->code);

        SetPenColor("Blue");
        drawLabel(x2 + fH, y -= h, curr_cou_node->data->name);

        curr_cou_node = curr_cou_node->next;
    }

    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "+ Add Course")) {
        pageID = 10;
    }

    SetPenColor("Violet");
    drawLabel(x2, y -= 2 * h, "Manage Courses");

    SetPenColor("Blue");
    textbox(GenUIID(0), x2, y -= 2 * h, w, h, courseCode, sizeof(courseCode));

    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "Manage")) {
        if (getCourseByCode(courseCode) != NULL) {
            curr_page_course = getCourseByCode(courseCode);
            strcpy(message, "");
            pageID = 6;
        } else {
            strcpy(message, "Please enter correct course code!");
        }
    }

    /** Render Application list*/
    y = winheight / 2 + 8 * h;

    SetPenColor("Violet");
    drawLabel(x3, y -= 2 * h, "Application List");

    struct Application_node *curr_app_node = getCollegeData()->applications;
    while (curr_app_node != NULL) {
        SetPenColor("Blue");
        drawLabel(x3, y -= 1.5 * h, curr_app_node->data->title);

        curr_app_node = curr_app_node->next;
    }

    if (button(GenUIID(0), x3, y -= 2 * h, w, h, "+ Add Application")) {
        pageID = 8;
    }

    SetPenColor("Violet");
    drawLabel(x3, y -= 2 * h, "Manage Applications");

    SetPenColor("Blue");
    textbox(GenUIID(0), x3, y -= 2 * h, w, h, applicationName,
            sizeof(applicationName));

    if (button(GenUIID(0), x3, y -= 2 * h, w, h, "Manage")) {
        if (getApplicationByName(applicationName) != NULL) {
            curr_page_application = getApplicationByName(applicationName);
            strcpy(message, "");
            pageID = 9;
        } else {
            strcpy(message, "Please enter correct application title!");
        }
    }
}

void drawStudentInfoPage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x1 = 2 * winwidth / 12;
    double x2 = 6 * winwidth / 12;
    double y = winheight / 2 + 6 * h;
    double w = 2 * winwidth / 12;

    /** Left Infos */

    SetPenColor("Violet");
    drawLabel(x1, y -= 1.5 * h, "Name: ");
    SetPenColor("Blue");
    drawLabel(x1 + 2 * fH, y -= 1.5 * h, loginStudentData->name);

    SetPenColor("Violet");
    drawLabel(x1, y -= 1.5 * h, "College: ");
    SetPenColor("Blue");
    drawLabel(x1 + 2 * fH, y -= 1.5 * h, loginStudentData->college);

    SetPenColor("Violet");
    drawLabel(x1, y -= 1.5 * h, "Class: ");
    SetPenColor("Blue");
    drawLabel(x1 + 2 * fH, y -= 1.5 * h, loginStudentData->hisclass);

    SetPenColor("Violet");
    drawLabel(x1, y -= 1.5 * h, "Gender: ");
    SetPenColor("Blue");
    drawLabel(x1 + 2 * fH, y -= 1.5 * h,
              loginStudentData->gender ? "Male" : "Female");

    /** Right Buttons */

    y = winheight / 2 + 6 * h;

    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "Update Info")) {
        pageID = 11;
    } else if (button(GenUIID(0), x2, y -= 2 * h, w, h,
                      "Application Management")) {
        pageID = 12;
    } else if (button(GenUIID(0), x2, y -= 2 * h, w, h, "Check Score")) {
        pageID = 13;
    } else if (button(GenUIID(0), x2, y -= 2 * h, w, h, "Change Password")) {
        pageID = 14;
    }
}

void drawManageClassPage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x1 = winwidth / 12;
    double x2 = 3 * winwidth / 12;
    double x3 = 6 * winwidth / 12;
    double x4 = 9 * winwidth / 12;
    double y = winheight / 2 + 8 * h;
    double w = 2 * winwidth / 12;

    static char StudentNumber1[80] = "Student Number";
    static char StudentNumber2[80] = "Student Number";
    static char StudentNumber3[80] = "Student Number";
    static char studentname[80] = "Student Name";
    static char studentgender[80] = "Student's Gender";

    static char app_title_input[80] = "Application Title";
    static char app_schoolnumber_input[80] = "Student Number";

    /** Col 1 */
    SetPenColor("Violet");
    drawLabel(x1, y -= 2 * h, "Student List");

    struct Student_node *spn = curr_page_class->students;
    while (spn != NULL) {
        SetPenColor("Blue");
        drawLabel(x1, y -= 1.5 * h, spn->data->schoolnumber);
        drawLabel(x1, y -= h, spn->data->name);
        spn = spn->next;
    }

    /** Col 2 */

    y = winheight / 2 + 8 * h;

    SetPenColor("Violet");
    drawLabel(x2, y -= 2 * h, "Add new student into class");

    SetPenColor("Blue");
    textbox(GenUIID(0), x2, y -= 2 * h, w, h, StudentNumber2,
            sizeof(StudentNumber2));

    SetPenColor("Blue");
    textbox(GenUIID(0), x2, y -= 2 * h, w, h, studentname, sizeof(studentname));

    SetPenColor("Blue");
    textbox(GenUIID(0), x2, y -= 2 * h, w, h, studentgender,
            sizeof(studentgender));
    int thegender;
    if (!strcmp(studentgender, "1"))
        thegender = 1;
    else
        thegender = 0;

    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "Add Student")) {
        int gender = -1;
        if (strcmp(studentgender, "Male") == 0) {
            strcpy(message, "");
            gender = 0;
        } else if (strcmp(studentgender, "Female") == 0) {
            strcpy(message, "");
            gender = 1;
        } else {
            strcpy(message, "Gender must be \"Male\" or \"Female\"");
        }

        if (gender != -1)
            clas_creat_stud(getCollegeData(), curr_page_class, StudentNumber2,
                            studentname, 0);
    }

    SetPenColor("Violet");
    drawLabel(x2, y -= 2 * h, "Delete a student in class");

    SetPenColor("Blue");
    textbox(GenUIID(0), x2, y -= 2 * h, w, h, StudentNumber3,
            sizeof(StudentNumber3));

    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "Delete Student")) {
        if (getStudentBySchoolNumber(StudentNumber3) == NULL) {
            strcpy(message, "Please enter correct student number");
        } else {
            clas_delete_stud(curr_page_class, StudentNumber3);
        }
    }

    /** Col 3 : Class-Level Application Mangement */

    y = winheight / 2 + 8 * h;

    SetPenColor("Violet");
    drawLabel(x3, y -= 2 * h, "Application List");

    struct Application_node *apn = getCollegeData()->applications;
    while (apn != NULL) {
        SetPenColor("Blue");
        drawLabel(x3, y -= 1.5 * h, apn->data->title);
        SetPenColor("Gray");
        drawLabel(x3 + 2 * fH, y -= h, apn->data->requirement);
        int i = 0;
        for (i = 0; i < apn->data->student_num; i++) {
            SetPenColor("Brown");
            char applicantsAndStatus[255] = "";
            strcat(applicantsAndStatus, apn->data->applicants[i]);
            strcat(applicantsAndStatus, "  ");
            strcat(applicantsAndStatus, apn->data->statuses[i]);
            drawLabel(x3 + 2 * fH, y -= h, applicantsAndStatus);
        }
        apn = apn->next;
    }

    /** Col 4 */
    y = winheight / 2 + 8 * h;

    SetPenColor("Violet");
    drawLabel(x4, y -= 2 * h, "Approval items");

    SetPenColor("Blue");
    textbox(GenUIID(0), x4, y -= 2 * h, w, h, app_title_input,
            sizeof(app_title_input));

    SetPenColor("Blue");
    textbox(GenUIID(0), x4, y -= 2 * h, w, h, app_schoolnumber_input,
            sizeof(app_schoolnumber_input));

    if (button(GenUIID(0), x4, y -= 2 * h, w, h, "Approval")) {
        struct Application *appl = getApplicationByName(app_title_input);
        if (appl == NULL) {
            strcpy(message, "Pleas input correct application title.");
        } else
            clas_event_exam(appl, app_schoolnumber_input, 1);
    }

    if (button(GenUIID(0), x4, y -= 2 * h, w, h, "Reject")) {
        struct Application *appl = getApplicationByName(app_title_input);
        if (appl == NULL) {
            strcpy(message, "Pleas input correct application title.");
        } else
            clas_event_exam(appl, app_schoolnumber_input, 0);
    }

    SetPenColor("Violet");
    drawLabel(x4, y -= 2 * h, "Delete Class");

    if (button(GenUIID(0), x4, y -= 2 * h, w, h, "Delete the class")) {
        coll_delete_clas(getCollegeData(), curr_page_class->ClassID);
        pageID = 3;
    }

    SetPenColor("Violet");
    drawLabel(x4, y -= 2 * h, "Return to College Manage");

    if (button(GenUIID(0), x4, y -= 2 * h, w, h, "Return")) {
        pageID = 3;
    }
}

void drawManageCoursePage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x1 = winwidth / 12;
    double x2 = 6 * winwidth / 12;
    double x3 = 9 * winwidth / 12;
    double x4 = 3 * winwidth / 12;
    double y = winheight / 2 + 8 * h;
    double w = 2 * winwidth / 12;

    static char StudentNumber1[80] = "Student Number";
    static char StudentNumber2[80] = "Student Number";
    static char StudentsScore[80] = "Score'";
    static char StudentsTitle[80] = "Title of score";

    struct Score_node *rpn = curr_page_course->scores;

    SetPenColor("Violet");
    drawLabel(x1, y -= 2 * h, "Student List");

    int i = 0;
    for (i = 0; i < curr_page_course->student_num; i++) {
        SetPenColor("Blue");
        drawLabel(x1, y -= 1.5 * h, curr_page_course->students[i]);
    }

    y = winheight / 2 + 8 * h;

    SetPenColor("Violet");
    drawLabel(x4, y -= 2 * h, "Score List");

    rpn = curr_page_course->scores;

    int hisscore;
    char sco[10];
    while (rpn != NULL) {
        SetPenColor("Blue");
        hisscore = rpn->data->score;
        if (hisscore == -1) {
            strcpy(sco, "Not entry");
        } else if (hisscore < 10) {
            sco[0] = hisscore + '0';
            sco[1] = '\0';
        } else if (hisscore < 100) {
            sco[2] = '\0';
            sco[1] = hisscore % 10 + '0';
            sco[0] = hisscore / 10 + '0';
        } else {
            strcpy(sco, "100");
        }
        char t[80] = "";
        strcat(t, rpn->data->schoolnumber);
        strcat(t, " got ");
        strcat(t, sco);
        strcat(t, " point in ");
        strcat(t, rpn->data->title);
        drawLabel(x4, y -= 1.5 * h, t);
        rpn = rpn->next;
    }

    y = winheight / 2 + 8 * h;

    SetPenColor("Violet");
    drawLabel(x2, y -= 2 * h, "Add new student into course");

    SetPenColor("Blue");
    textbox(GenUIID(0), x2, y -= 2 * h, w, h, StudentNumber1,
            sizeof(StudentNumber1));

    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "+ Add Student")) {
        struct Student *stu = getStudentBySchoolNumber(StudentNumber1);
        if (stu != NULL) {
            coll_course_creat_stud(curr_page_course, StudentNumber1);
            strcpy(message, "");
        } else {
            strcpy(message, "Student not in this college!");
        }
    }

    SetPenColor("Violet");
    drawLabel(x2, y -= 2 * h, "Add new record of score");

    SetPenColor("Blue");
    textbox(GenUIID(0), x2, y -= 2 * h, w, h, StudentNumber2,
            sizeof(StudentNumber2));

    SetPenColor("Blue");
    textbox(GenUIID(0), x2, y -= 2 * h, w, h, StudentsTitle,
            sizeof(StudentsTitle));

    SetPenColor("Blue");
    textbox(GenUIID(0), x2, y -= 2 * h, w, h, StudentsScore,
            sizeof(StudentsScore));

    int thescore = 0;
    i = 0;
    int flag = 1;
    while (StudentsScore[i] != '\0') {
        if (StudentsScore[i] - '0' >= 0 && StudentsScore[i] - '0' <= 9)
            thescore = thescore * 10 + StudentsScore[i] - '0';
        else {
            flag = 0;
            break;
        }
        if (thescore < 0 || thescore > 100) flag = 0;
        i++;
    }

    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "+ Input Scores")) {
        struct Student *stud = getStudentBySchoolNumber(StudentNumber2);
        if (stud != NULL && flag) {
            strcpy(message, "");
            input_score(curr_page_course->scores, StudentNumber2, StudentsTitle,
                        atoi(StudentsScore));
        } else {
            strcpy(message, "Student not in this course.");
        }
    }

    y = winheight / 2 + 8 * h;

    if (button(GenUIID(0), x3, y -= 2 * h, w, h, "Delete the Course")) {
        coll_course_delete(getCollegeData(), curr_page_course->code);
        pageID = 3;
    }

    if (button(GenUIID(0), x3, y -= 2 * h, w, h, "Return")) {
        pageID = 3;
    }
}

void drawManageApplicationPage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x1 = winwidth / 6;
    double x2 = 3 * winwidth / 6;
    double y = winheight / 2 + 6 * h;
    double w = winwidth / 3;

    static char schoolnumber[80] = "School Number";

    SetPenColor("Violet");
    drawLabel(x1, y -= 2 * h, "Applicants List");

    int i = 0;
    for (i = 0; i < curr_page_application->student_num; i++) {
        SetPenColor("Blue");
        drawLabel(x1, y -= h, curr_page_application->applicants[i]);
        SetPenColor("Gray");
        drawLabel(x1 + 2 * fH, y -= h, curr_page_application->statuses[i]);
    }

    y = winheight / 2 + 6 * h;

    SetPenColor("Violet");
    drawLabel(x2, y -= 2 * h, "Change status of application");

    SetPenColor("Blue");
    textbox(GenUIID(0), x2, y -= 2 * h, w, h, schoolnumber,
            sizeof(schoolnumber));

    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "Accept")) {
        struct Student *stu = getStudentBySchoolNumber(schoolnumber);
        if (stu == NULL) {
            strcpy(message, "Please enter correct student number.");
        } else {
            if (!strcmp(
                    stud_view_refults(getStudentBySchoolNumber(schoolnumber),
                                      curr_page_application),
                    "class_passed")) {
                strcpy(message, "");
                coll_event_exam(curr_page_application, schoolnumber, 1);
            } else
                strcpy(message, "College can not accept this application now.");
        }
    }

    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "Reject")) {
        struct Student *stu = getStudentBySchoolNumber(schoolnumber);
        if (stu == NULL) {
            strcpy(message, "Please enter correct student number.");
        } else {
            if (!strcmp(
                    stud_view_refults(getStudentBySchoolNumber(schoolnumber),
                                      curr_page_application),
                    "class_passed")) {
                strcpy(message, "");
                coll_event_exam(curr_page_application, schoolnumber, 0);
            } else
                strcpy(message, "College can not reject this application now.");
        }
    }

    SetPenColor("Violet");
    drawLabel(x2, y -= 2 * h, "Return to college manage");
    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "Return")) {
        pageID = 3;
    }
}

void drawCreateClassPage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x = winwidth / 2.5;
    double y = winheight / 2 + 6 * h;
    double w = winwidth / 5;

    static char classid[80] = "";

    SetPenColor("Blue");
    drawLabel(x, y -= 2 * h, "Class ID");
    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, classid, sizeof(classid));

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Create")) {
        coll_creat_clas(getCollegeData(), classid);
        pageID = 3;
    }

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Cancel")) {
        pageID = 3;
    }
}

void drawCreateApplicationPage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x = winwidth / 2.5;
    double y = winheight / 2 + 6 * h;
    double w = winwidth / 5;

    static char title[80] = "";
    static char requirement[80] = "";

    SetPenColor("Blue");
    drawLabel(x, y -= 2 * h, "Application Title");
    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, title, sizeof(title));

    SetPenColor("Blue");
    drawLabel(x, y -= 2 * h, "Application Requirements");
    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, requirement, sizeof(requirement));

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Create")) {
        coll_event_creat(getCollegeData(), title, requirement);
        pageID = 3;
    }

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Cancel")) {
        pageID = 3;
    }
}

void drawCreateCoursePage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x = winwidth / 2.5;
    double y = winheight / 2 + 6 * h;
    double w = winwidth / 5;

    static char name[80] = "";
    static char code[80] = "";

    SetPenColor("Blue");
    drawLabel(x, y -= 2 * h, "Course Code");
    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, code, sizeof(code));

    SetPenColor("Blue");
    drawLabel(x, y -= 2 * h, "Course Name");
    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, name, sizeof(name));

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Create")) {
        coll_course_creat(getCollegeData(), code, name);
        pageID = 3;
    }

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Cancel")) {
        pageID = 3;
    }
}

void drawUpdateInfoPage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x = winwidth / 2.5;
    double y = winheight / 2 + 6 * h;
    double w = winwidth / 5;

    static char sname[80] = "Your Name";
    static char sgender[80] = "Male or Female";

    SetPenColor("Blue");
    drawLabel(x, y -= 1.5 * h, "New Name: ");
    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, sname, sizeof(sname));

    SetPenColor("Blue");
    drawLabel(x, y -= 1.5 * h, "New Gender: ");
    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, sgender, sizeof(sgender));

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Update")) {
        if (strcmp(sgender, "Male") == 0) {
            strcpy(message, "");
            stud_input_information(loginStudentData, sname, 1);
            pageID = 4;
        } else if (strcmp(sgender, "Female") == 0) {
            strcpy(message, "");
            stud_input_information(loginStudentData, sname, 0);
            pageID = 4;
        } else {
            strcpy(message, "Gender must be \"Male\" or \"Female\"");
        }
    }

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Cancel")) {
        strcpy(message, "");
        pageID = 4;
    }
}

void drawApplicationManagePage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x1 = winwidth / 12;
    double x2 = 5 * winwidth / 12;
    double y = winheight / 2 + 8 * h;
    double w = 2 * winwidth / 12;

    static char classID[80] = "Class ID";
    static char courseCode[80] = "Course Code";
    static char applicationName[80] = "Application Name";

    SetPenColor("Violet");
    drawLabel(x1, y -= 2 * h, "Applications of College");

    struct Application_node *curr_app_node = getCollegeData()->applications;
    while (curr_app_node != NULL) {
        SetPenColor("Blue");
        drawLabel(x1, y -= 1.5 * h, curr_app_node->data->title);

        SetPenColor("Gray");
        drawLabel(x1 + fH, y -= 1.5 * h, curr_app_node->data->requirement);
        int a = isSubmitApplication(curr_app_node->data->title,
                                    loginStudentData->schoolnumber);
        if (a == -1) {
            SetPenColor("Gray");
            drawLabel(x1 + fH, y -= 1.5 * h,
                      "You has not submit this application.");
        } else {
            SetPenColor("Red");
            drawLabel(x1 + fH, y -= 1.5 * h, curr_app_node->data->statuses[a]);
        }

        curr_app_node = curr_app_node->next;
    }

    y = winheight / 2 + 8 * h;

    SetPenColor("Violet");
    drawLabel(x2, y -= 2 * h, "Submit / Cancel Applications");

    SetPenColor("Blue");
    textbox(GenUIID(0), x2, y -= 2 * h, w, h, applicationName,
            sizeof(applicationName));

    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "Submit / Cancel")) {
        struct Application *app = getApplicationByName(applicationName);
        if (app != NULL) {
            if (isSubmitApplication(applicationName,
                                    loginStudentData->schoolnumber) == -1) {
                stud_apply(loginStudentData, app);
            } else {
                strcpy(message, "You have already submited this application!");
            }
        } else {
            strcpy(message, "Please enter correct application title!");
        }
    }

    if (button(GenUIID(0), x2, y -= 2 * h, w, h, "Back to Student Info")) {
        strcpy(message, "");
        pageID = 4;
    }
}

void drawCheckScorePage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x = winwidth / 2.5;
    double y = winheight / 1.5;
    double w = winwidth / 5;

    char **scores =
        student_check_score(getCollegeData()->courses, loginStudentData);

    int length = sizeof(scores) / sizeof(scores[0]);
    int i = 0;
    for (i = 0; i < length; i++) {
        drawLabel(x, y -= 2 * h, scores[i]);
    }
}

void drawChangePasswordPage() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x = winwidth / 2.5;
    double y = winheight / 1.5;
    double w = winwidth / 5;

    static char currpass[80] = "";
    static char newpass[80] = "";

    SetPenColor("Blue");
    drawLabel(x, y -= 2 * h, "Old Password");

    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, currpass, sizeof(currpass));

    SetPenColor("Blue");
    drawLabel(x, y -= 2 * h, "New Password");

    SetPenColor("Blue");
    textbox(GenUIID(0), x, y -= 2 * h, w, h, newpass, sizeof(newpass));

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Change")) {
        if (!stud_change_password(loginStudentData, currpass, newpass))
            pageID = 4;
    }

    if (button(GenUIID(0), x, y -= 2 * h, w, h, "Cancel")) {
        pageID = 4;
    }
}

void drawMessage() {
    double fH = GetFontHeight();
    SetPenColor("Red");
    drawLabel(0 + fH, 0 + fH, message);
}

void display() {
    double fH = GetFontHeight();
    double h = fH * 2;
    double x = winwidth / 2;
    double y = winheight;
    double w = winwidth / 5;

    DisplayClear();

    drawMenu();
    drawMessage();

    switch (pageID) {
        case 0:  // Index
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("College Data Manage System") / 2,
                      y - 2 * h, "College Data Manage System");
            drawIndex();
            break;
        case 1:  // Manager Login
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Manager Login") / 2, y - 2 * h,
                      "Manager Login");
            drawManagerLoginPage();
            break;
        case 2:  // Student Login
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Student Login") / 2, y - 2 * h,
                      "Student Login");
            drawStudentLoginPage();
            break;
        case 3:  // College Manage Index
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("College Manage") / 2, y - 2 * h,
                      "College Manage");
            drawCollegeManagePage();
            break;
        case 4:  // Student Info
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Student Info") / 2, y - 2 * h,
                      "Student Info");
            drawLabel(x - TextStringWidth(loginStudentData->schoolnumber) / 2,
                      y - 3 * h, loginStudentData->schoolnumber);
            drawStudentInfoPage();
            break;
        case 5:  // Manage Specific Class
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Manage Class") / 2, y - 2 * h,
                      "Manage Class");
            drawLabel(x - TextStringWidth(curr_page_class->ClassID) / 2,
                      y - 3 * h, curr_page_class->ClassID);
            drawManageClassPage();
            break;
        case 6:  // Manage Specific Course
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Manage Course") / 2, y - 2 * h,
                      "Manage Course");
            drawLabel(x - TextStringWidth(curr_page_course->name) / 2,
                      y - 3 * h, curr_page_course->name);
            drawManageCoursePage();
            break;
        case 7:  // Add new Class
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Add new Class") / 2, y - 2 * h,
                      "Add new Class");
            drawCreateClassPage();
            break;
        case 8:  // Add New Application
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Add New Application") / 2, y - 2 * h,
                      "Add New Application");
            drawCreateApplicationPage();
            break;
        case 9:  // Manage Specific Application
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Manage Application") / 2, y - 2 * h,
                      "Manage Application");
            drawLabel(x - TextStringWidth(curr_page_application->title) / 2,
                      y - 3 * h, curr_page_application->title);
            drawManageApplicationPage();
            break;
        case 10:  // Add New Course
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Add New Course") / 2, y - 2 * h,
                      "Add New Course");
            drawCreateCoursePage();
            break;
        case 11:  // Update Student Info
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Update Info") / 2, y - 2 * h,
                      "Update Info");
            drawUpdateInfoPage();
            break;
        case 12:  // Student Application Management
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Application Management") / 2,
                      y - 2 * h, "Application Management");
            drawApplicationManagePage();
            break;
        case 13:  // Student Check Score
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Check Score") / 2, y - 2 * h,
                      "Check Score");
            drawCheckScorePage();
            break;
        case 14:  // Student Change Password
            SetPenColor("Blue");
            drawLabel(x - TextStringWidth("Change Password") / 2, y - 2 * h,
                      "Change Password");
            drawChangePasswordPage();
            break;
    }
}
