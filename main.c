#include "data.h"
#include "general.h"

int main() {
    // Step1. Read data in file "data.txt"

    FILE *fp;
    fp = fopen("data.txt", "r");
    readData(fp);
    struct College *data = getCollegeData();
    fclose(fp);

    // Step2. Find student 3190000003, change his name to "Kenny Lin"

    struct Class_node *curr_cla = data->classes;
    while (curr_cla != NULL) {
        struct Student_node *curr_stu = curr_cla->data->students;
        while (curr_stu != NULL) {
            if (strcmp(curr_stu->data->schoolnumber, "3190000003") == 0)
                curr_stu->data->name = "Kenny Lin";
            curr_stu = curr_stu->next;
        }
        curr_cla = curr_cla->next;
    }

    // Step3. Export data to "newdata.txt"

    fp = fopen("newdata.txt", "w");
    exportCollegeData(fp);
    fclose(fp);

    return 0;
}