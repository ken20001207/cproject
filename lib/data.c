// data.c - by 3190106167 Yuanlin Lin

#include "data.h"

#include "general.h"
#include "md5.h"

// Read File By line and remove '\n'
char *getStringFromNewLine(FILE *fp) {
    char *bufline = malloc(1024 * sizeof(char));
    bufline = fgets(bufline, 1024, fp);
    bufline[strlen(bufline) - 1] = '\0';
    return bufline;
}

struct College *college = NULL;

// Read data from file and store into program
//
// return 0: Read Failed
// return 1: Read Successed
int readData(FILE *fp) {
    if (fp == NULL) return 0;

    struct College *newCollege = malloc(sizeof(struct College));
    newCollege->applications = NULL;
    newCollege->classes = NULL;
    newCollege->courses = NULL;

    struct Class *tempClass = NULL;
    struct Course *tempCourse = NULL;
    char *bufline = malloc(sizeof(char));

    while (1) {
        bufline = getStringFromNewLine(fp);

        if (strcmp("College-Start", bufline) == 0) {
            // load college name
            newCollege->name = getStringFromNewLine(fp);

        } else if (strcmp("Class-Start", bufline) == 0) {
            tempClass = malloc(sizeof(struct Class));
            tempClass->students = NULL;

            // load class id
            tempClass->ClassID = getStringFromNewLine(fp);

        } else if (strcmp("Student-Start", bufline) == 0) {
            struct Student *tempStudent = malloc(sizeof(struct Student));

            // load student school number
            tempStudent->schoolnumber = getStringFromNewLine(fp);

            // load student name
            tempStudent->name = getStringFromNewLine(fp);

            // load student gender
            tempStudent->gender = atoi(getStringFromNewLine(fp));

            // load student collge
            tempStudent->college = getStringFromNewLine(fp);

            // load student class id
            tempStudent->hisclass = getStringFromNewLine(fp);

            // load md5 password of student
            tempStudent->md5 = getStringFromNewLine(fp);

            // create a new student node
            struct Student_node *stu_node = malloc(sizeof(struct Student_node));
            stu_node->data = tempStudent;

            // add into list
            if (tempClass->students == NULL) {
                tempClass->students = stu_node;
            } else {
                tempClass->students->prev = stu_node;
                stu_node->next = tempClass->students;
                tempClass->students = stu_node;
            }
        } else if (strcmp("Class-End", bufline) == 0) {
            struct Class_node *cla_node = malloc(sizeof(struct Class_node));
            cla_node->data = tempClass;

            if (newCollege->classes == NULL) {
                newCollege->classes = cla_node;
            } else {
                newCollege->classes->prev = cla_node;
                cla_node->next = newCollege->classes;
                newCollege->classes = cla_node;
            }
        } else if (strcmp("Course-Start", bufline) == 0) {
            tempCourse = malloc(sizeof(struct Course));
            tempCourse->scores = NULL;

            // load course code
            tempCourse->code = getStringFromNewLine(fp);

            // load course name
            tempCourse->name = getStringFromNewLine(fp);

        } else if (strcmp("Students-Start", bufline) == 0) {
            // get number of students in this course
            tempCourse->student_num = atoi(getStringFromNewLine(fp));

            // loop to get all student school number
            char **students = malloc(tempCourse->student_num * (sizeof(char *)));
            int i = 0;
            for (i = 0; i < tempCourse->student_num; i++) {
                students[i] = getStringFromNewLine(fp);
            }

            // save to tempcourse
            tempCourse->students = students;
        } else if (strcmp("Score-Start", bufline) == 0) {
            struct Score *newscore = malloc(sizeof(struct Score));

            // load student number of score
            newscore->schoolnumber = getStringFromNewLine(fp);

            // load score title
            newscore->title = getStringFromNewLine(fp);

            // load score
            newscore->score = atoi(getStringFromNewLine(fp));

            // create new score node
            struct Score_node *score_node = malloc(sizeof(struct Score_node));
            score_node->data = newscore;

            // link into list
            if (tempCourse->scores == NULL) {
                tempCourse->scores = score_node;
            } else {
                tempCourse->scores->prev = score_node;
                score_node->next = tempCourse->scores;
                tempCourse->scores = score_node;
            }
        } else if (strcmp("Course-End", bufline) == 0) {
            struct Course_node *cou_node = malloc(sizeof(struct Course_node));
            cou_node->data = tempCourse;
            if (newCollege->courses == NULL) {
                newCollege->courses = cou_node;
            } else {
                newCollege->courses->prev = cou_node;
                cou_node->next = newCollege->courses;
                newCollege->courses = cou_node;
            }
        } else if (strcmp("Application-Start", bufline) == 0) {
            struct Application *newApplication = malloc(sizeof(struct Application));

            // load title of this application
            newApplication->title = getStringFromNewLine(fp);

            // load requirement of this application
            newApplication->requirement = getStringFromNewLine(fp);

            // get number of applicants of this application
            newApplication->student_num = atoi(getStringFromNewLine(fp));

            // loop to get all applicants school number and status
            char **applicants = malloc(newApplication->student_num * (sizeof(char *)));
            char **statuses = malloc(newApplication->student_num * (sizeof(char *)));
            int i = 0;
            for (i = 0; i < newApplication->student_num; i++) {
                applicants[i] = getStringFromNewLine(fp);
                statuses[i] = getStringFromNewLine(fp);
            }

            // save to newApplication
            newApplication->applicants = applicants;
            newApplication->statuses = statuses;

            // insert into application list
            struct Application_node *app_node = malloc(sizeof(struct Application_node));
            app_node->data = newApplication;

            if (newCollege->applications == NULL) {
                newCollege->applications = app_node;
            } else {
                newCollege->applications->prev = app_node;
                app_node->next = newCollege->applications;
                newCollege->applications = app_node;
            }
        } else if (strcmp("College-End", bufline) == 0) {
            college = newCollege;
            return 1;
        }
    }
    return 0;
}

// Create an new collge object
void newCollege(char *name) {
    struct College *newCollege = malloc(sizeof(struct College));
    newCollege->name = name;
    college = newCollege;
}

// Check if password of stuedent is correct
int checkPassword(char *schoolnumber, char *password) {
    struct Student *stu = getStudentBySchoolNumber(schoolnumber);
    if (stu == NULL) return 0;
    if (md5check(password, stu->md5))
        return 1;
    else
        return 0;
}

// Return college data stored in program
struct College *getCollegeData() {
    return college;
}

// Save college data stored in program into file
//
// return 0: Export Failed
// return 1: Export Successed
int exportCollegeData(FILE *fp) {
    if (fp == NULL) return 0;
    fprintf(fp, "College-Start\n");
    fprintf(fp, "%s\n", college->name);

    struct Class_node *curr_cla = college->classes;
    while (curr_cla != NULL) {
        struct Student_node *curr_stu = curr_cla->data->students;
        fprintf(fp, "Class-Start\n");
        fprintf(fp, "%s\n", curr_cla->data->ClassID);
        while (curr_stu != NULL) {
            fprintf(fp, "Student-Start\n");
            fprintf(fp, "%s\n", curr_stu->data->schoolnumber);
            fprintf(fp, "%s\n", curr_stu->data->name);
            fprintf(fp, "%d\n", curr_stu->data->gender);
            fprintf(fp, "%s\n", curr_stu->data->college);
            fprintf(fp, "%s\n", curr_stu->data->hisclass);
            fprintf(fp, "%s\n", curr_stu->data->md5);
            fprintf(fp, "Student-End\n");
            curr_stu = curr_stu->next;
        }
        fprintf(fp, "Class-End\n");
        curr_cla = curr_cla->next;
    }

    struct Course_node *curr_cou = college->courses;
    while (curr_cou != NULL) {
        fprintf(fp, "Course-Start\n");

        fprintf(fp, "%s\n", curr_cou->data->code);
        fprintf(fp, "%s\n", curr_cou->data->name);

        fprintf(fp, "Students-Start\n");
        fprintf(fp, "%d\n", curr_cou->data->student_num);
        int i = 0;
        for (i = 0; i < curr_cou->data->student_num; i++) {
            fprintf(fp, "%s\n", curr_cou->data->students[i]);
        }
        fprintf(fp, "Students-End\n");

        struct Score_node *curr_sco = curr_cou->data->scores;
        while (curr_sco != NULL) {
            fprintf(fp, "Score-Start\n");
            fprintf(fp, "%s\n", curr_sco->data->schoolnumber);
            fprintf(fp, "%s\n", curr_sco->data->title);
            fprintf(fp, "%d\n", curr_sco->data->score);
            fprintf(fp, "Score-End\n");
            curr_sco = curr_sco->next;
        }

        fprintf(fp, "Course-End\n");
        curr_cou = curr_cou->next;
    }

    struct Application_node *curr_app = college->applications;
    while (curr_app != NULL) {
        fprintf(fp, "Application-Start\n");
        fprintf(fp, "%s\n", curr_app->data->title);
        fprintf(fp, "%s\n", curr_app->data->requirement);
        fprintf(fp, "%d\n", curr_app->data->student_num);
        int i = 0;
        for (i = 0; i < curr_app->data->student_num; i++) {
            fprintf(fp, "%s\n", curr_app->data->applicants[i]);
            fprintf(fp, "%s\n", curr_app->data->statuses[i]);
        }
        fprintf(fp, "Application-End\n");
        curr_app = curr_app->next;
    }

    fprintf(fp, "College-End\n");
    return 1;
}

struct Student *getStudentBySchoolNumber(char *schoolnumber) {
    struct Class_node *curr_cla = college->classes;
    while (curr_cla != NULL) {
        struct Student_node *curr_stu = curr_cla->data->students;
        while (curr_stu != NULL) {
            if (strcmp(schoolnumber, curr_stu->data->schoolnumber) == 0) {
                return curr_stu->data;
            }
            curr_stu = curr_stu->next;
        }
        curr_cla = curr_cla->next;
    }
    return NULL;
}

struct Class *getClassByID(char *class_id) {
    struct Class_node *curr_cla = college->classes;
    while (curr_cla != NULL) {
        if (strcmp(class_id, curr_cla->data->ClassID) == 0) {
            return curr_cla->data;
        }
        curr_cla = curr_cla->next;
    }
    return NULL;
}

struct Course *getCourseByCode(char *course_code) {
    struct Course_node *curr_cou = college->courses;
    while (curr_cou != NULL) {
        if (strcmp(course_code, curr_cou->data->code)) {
            return curr_cou->data;
        }
        curr_cou = curr_cou->next;
    }
    return NULL;
}
