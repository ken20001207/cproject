// data.h - by 3190106167 Yuanlin Lin

// ---------- Object Types ---------- //
#ifndef OBJECT_TYPES
#define OBJECT_TYPES

// Student object
struct Student {
    // School number of student
    char *schoolnumber;
    // Name of student
    char *name;
    // Gender of student, 0: Female, 1: Male
    int gender;
    // Name of the college this student belongs to
    char *college;
    // Id of the class this student belongs to
    char *class;
};

// List node of student object
struct Student_node {
    // Previous node of student in list
    struct Student_node *prev;
    // Pointer to student object
    struct Student *data;
    // Next node of student in list
    struct Student_node *next;
};

// Class Object
struct Class {
    // ID of this class
    char *ClassID;
    // Linked list of students in this class
    struct Student_node *students;
};

// List node of class object
struct Class_node {
    // Previous node in list
    struct Class_node *prev;
    // Pointer to Class Object
    struct Class *data;
    // Next Node in List
    struct Class_node *next;
};

// College object
struct College {
    // Name of college
    char *name;
    // Linked list of classes in this college
    struct Class_node *classes;
};

#endif

// ---------- Functions of Data Process ---------- //
#ifndef DATA_FUNCTIONS
#define DATA_FUNCTIONS

#include "general.h"

// Read data from file and store into program
//
// return 0: Read Failed
// return 1: Read Successed
int readData(FILE *fp);

// Return stored college data
struct College *getCollegeData();

// Save college data stored in program into file
//
// return 0: Export Failed
// return 1: Export Successed
int exportCollegeData(FILE *fp);

#endif