#include "data.h"
#include "general.h"
#include "md5.h"

int main() {
    // Step1. Read data in file "data.txt"

    FILE *fp;
    fp = fopen("data.txt", "r");
    readData(fp);
    struct College *data = getCollegeData();
    fclose(fp);

    printf("本学院目前的申请事项如下: \n");
    struct Application_node *curr_app = data->applications;

    int i = 0;
    while (curr_app != NULL) {
        struct Application *app = curr_app->data;
        printf("\n事项 : %s\n", app->title);
        printf("申请条件 : %s\n", app->requirement);
        for (i = 0; i < app->student_num; i++) {
            printf("学生 %s 申请了本事项，当前状态： %s\n", app->applicants[i], app->statuses[i]);
        }
        curr_app = curr_app->next;
    }

    return 0;
}
