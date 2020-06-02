// data.h - by 3190106167 Yuanlin Lin

// ---------- Object Types ---------- //
#ifndef OBJECT_TYPES
#define OBJECT_TYPES

// Score Object
struct Score {
    // School number of the student this score belongs to
    char *schoolnumber;
    // Score title
    char *title;
    // Score
    int score;
};

// List node of score object
struct Score_node {
    // Previous node of score in list
    struct Score_node *prev;
    // Pointer to score object
    struct Score *data;
    // Next node of score in list
    struct Score_node *next;
};

// Course Object
struct Course {
    // Course Code
    char *code;
    // Course Name
    char *name;
    // number of students
    int student_num;
    // students
    char **students;
    // scores
    struct Score_node *scores;
};

// List node of course object
struct Course_node {
    // Previous node of course in list
    struct Course_node *prev;
    // Pointer to course object
    struct Course *data;
    // Next node of course in list
    struct Course_node *next;
};

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
    char *hisclass;
    // md5 password of student
    char *md5;
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

// Application Object
struct Application {
    // Title of this application
    char *title;
    // number of students who submit this application
    int student_num;
    // students who submit this application
    char **applicants;
    // statuses of applications
    char **statuses;
    // requierment of this application
    char *requirement;
};

struct Application_node {
    // Previous node in list
    struct Application_node *prev;
    // Pointer to Application
    struct Application *data;
    // Next node in list
    struct Application_node *next;
};

// College object
struct College {
    // Name of college
    char *name;
    // Linked list of classes in this college
    struct Class_node *classes;
    // Courses of this college
    struct Course_node *courses;
    // Applications of this college
    struct Application_node *applications;
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

// get student data by schoolnumber
struct Student *getStudentBySchoolNumber(char *schoolnumber);

// get class data by class id
struct Class *getClassByID(char *class_id);

// get Course data by course code
struct Course *getCourseByCode(char *course_code);

// check password of student is correct
int checkPassword(char *schoolnumber, char *password);

// Create new college object
void newCollege(char *name);

#endif
