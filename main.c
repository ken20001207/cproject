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

    printf("本学院目前受理的学生申请事项如下: \n");
    struct Application_node *curr_app = data->applications;
    while (curr_app != NULL) {
        struct Application *app = curr_app->data;
        printf("学生 %s 申请了 %s , 目前状态为 %s\n", app->schoolnumber, app->title, app->status);
        curr_app = curr_app->next;
    }

    // Example usage of checkpassword()
    printf("\n");
    if (checkPassword("3190000001", "password"))
        printf("3190000001 的密码是 password 没错!\n");
    else
        printf("3190000001 的密码不是 password， 或是这个学生不存在于这个学院!\n");

    printf("\n");
    if (checkPassword("3190000001", "wrong_password"))
        printf("3190000001 的密码是 wrong_password 没错!\n");
    else
        printf("3190000001 的密码不是 wrong_password 或是这个学生不存在于这个学院!\n");

    return 0;
}
