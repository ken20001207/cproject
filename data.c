// data.c - by 3190106167 Yuanlin Lin

#include "data.h"

#include "general.h"

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

    struct College *newCollege = malloc(sizeof(newCollege));
    struct Class *tempClass = NULL;

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
        }

        line_size = getline(&line_buf, &line_buf_size, fp);
    }

    return 1;
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
    fprintf(fp, "College-End");
    return 1;
}