// data.c - by 3190106167 Yuanlin Lin

#include "data.h"

#include "general.h"
#include "md5.h"

struct College *college = NULL;

// Read data from file and store into program
//
// return 0: Read Failed
// return 1: Read Successed
int readData(FILE *fp) {
    if (fp == NULL) return 0;

    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;

    struct College *newCollege = malloc(sizeof(struct College));
    struct Class *tempClass = NULL;
    struct Course *tempCourse = NULL;

    line_size = getline(&line_buf, &line_buf_size, fp);

    while (line_size >= 0) {
        line_count++;
        if (strcmp("College-Start\n", line_buf) == 0) {
            // load college name
            line_size = getline(&(newCollege->name), &line_buf_size, fp);
            newCollege->name[strlen(newCollege->name) - 1] = '\0';

        } else if (strcmp("Class-Start\n", line_buf) == 0) {
            tempClass = malloc(sizeof(struct Class));

            // load class id
            line_size = getline(&(tempClass->ClassID), &line_buf_size, fp);
            tempClass->ClassID[strlen(tempClass->ClassID) - 1] = '\0';

        } else if (strcmp("Student-Start\n", line_buf) == 0) {
            struct Student *tempStudent = malloc(sizeof(struct Student));

            // load student school number
            line_size = getline(&(tempStudent->schoolnumber), &line_buf_size, fp);
            tempStudent->schoolnumber[strlen(tempStudent->schoolnumber) - 1] = '\0';

            // load student name
            line_size = getline(&(tempStudent->name), &line_buf_size, fp);
            tempStudent->name[strlen(tempStudent->name) - 1] = '\0';

            // load student gender
            char *gender_char = NULL;
            line_size = getline(&gender_char, &line_buf_size, fp);
            tempStudent->gender = gender_char[0] - '0';

            // load student collge
            line_size = getline(&(tempStudent->college), &line_buf_size, fp);
            tempStudent->college[strlen(tempStudent->college) - 1] = '\0';

            // load student class id
            line_size = getline(&(tempStudent->class), &line_buf_size, fp);
            tempStudent->class[strlen(tempStudent->class) - 1] = '\0';

            // load md5 password of student
            line_size = getline(&(tempStudent->md5), &line_buf_size, fp);
            tempStudent->md5[strlen(tempStudent->md5) - 1] = '\0';

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
        } else if (strcmp("Class-End\n", line_buf) == 0) {
            struct Class_node *cla_node = malloc(sizeof(struct Class_node));
            cla_node->data = tempClass;

            if (newCollege->classes == NULL) {
                newCollege->classes = cla_node;
            } else {
                newCollege->classes->prev = cla_node;
                cla_node->next = newCollege->classes;
                newCollege->classes = cla_node;
            }
        } else if (strcmp("College-End", line_buf) == 0) {
            college = newCollege;
        } else if (strcmp("Course-Start\n", line_buf) == 0) {
            tempCourse = malloc(sizeof(struct Course));

            // load course code
            line_size = getline(&(tempCourse->code), &line_buf_size, fp);
            tempCourse->code[strlen(tempCourse->code) - 1] = '\0';

            // load course name
            line_size = getline(&(tempCourse->name), &line_buf_size, fp);
            tempCourse->name[strlen(tempCourse->name) - 1] = '\0';
        } else if (strcmp("Students-Start\n", line_buf) == 0) {
            // get number of students in this course
            char *students_num_string = NULL;
            line_size = getline(&students_num_string, &line_buf_size, fp);
            int students_num = atoi(students_num_string);
            tempCourse->student_num = students_num;

            // loop to get all student school number
            char **students = malloc(students_num * (sizeof(char *)));
            int i = 0;
            for (i = 0; i < students_num; i++) {
                line_size = getline(&(students[i]), &line_buf_size, fp);
                students[i][strlen(students[i]) - 1] = '\0';
            }

            // save to tempcourse
            tempCourse->students = students;
        } else if (strcmp("Score-Start\n", line_buf) == 0) {
            struct Score *newscore = malloc(sizeof(struct Score));

            // load student number of score
            line_size = getline(&(newscore->schoolnumber), &line_buf_size, fp);
            newscore->schoolnumber[strlen(newscore->schoolnumber) - 1] = '\0';

            // load score title
            line_size = getline(&(newscore->title), &line_buf_size, fp);
            newscore->title[strlen(newscore->title) - 1] = '\0';

            // load score
            char *score_string = NULL;
            line_size = getline(&(score_string), &line_buf_size, fp);
            newscore->score = atoi(score_string);

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
        } else if (strcmp("Course-End\n", line_buf) == 0) {
            struct Course_node *cou_node = malloc(sizeof(struct Course_node));
            cou_node->data = tempCourse;
            if (newCollege->courses == NULL) {
                newCollege->courses = cou_node;
            } else {
                newCollege->courses->prev = cou_node;
                cou_node->next = newCollege->courses;
                newCollege->courses = cou_node;
            }
        } else if (strcmp("Application-Start\n", line_buf) == 0) {
            struct Application *newApplication = malloc(sizeof(struct Application));

            // load student number who send this application
            line_size = getline(&(newApplication->schoolnumber), &line_buf_size, fp);
            newApplication->schoolnumber[strlen(newApplication->schoolnumber) - 1] = '\0';

            // load title of this application
            line_size = getline(&(newApplication->title), &line_buf_size, fp);
            newApplication->title[strlen(newApplication->title) - 1] = '\0';

            // load status of this application
            line_size = getline(&(newApplication->status), &line_buf_size, fp);
            newApplication->status[strlen(newApplication->status) - 1] = '\0';

            struct Application_node *app_node = malloc(sizeof(struct Application_node));
            app_node->data = newApplication;

            if (newCollege->applications == NULL) {
                newCollege->applications = app_node;
            } else {
                newCollege->applications->prev = app_node;
                app_node->next = newCollege->applications;
                newCollege->applications = app_node;
            }
        }

        line_size = getline(&line_buf, &line_buf_size, fp);
    }
    return 1;
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
            fprintf(fp, "%s\n", curr_stu->data->class);
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

    fprintf(fp, "College-End");
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