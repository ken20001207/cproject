#include "total.h"

#include "data.h"
#include "general.h"

struct Application *event_find(struct College *op, char *thetitle) {  //找到事项位置
    struct Application_node *apn;
    apn = op->applications;
    while (apn != NULL) {
        if (!strcmp(apn->data->title, thetitle)) {
            return apn->data;
        }
        apn = apn->next;
    }
    if (apn == NULL) return NULL;
}

char *view_requ(struct Application *ap) {  //查看事件要求
    if (ap != NULL) return ap->requirement;
    return NULL;
}

struct College *total(int college) {
    FILE *fp;
    struct College *data;
    switch (college) {
        case 1:
            fp = fopen("danqing.txt", "r");
            break; /*打开丹青的文档*/
        case 2:
            fp = fopen("yunfeng.txt", "r");
            break; /*打开云峰的文档*/
        case 3:
            fp = fopen("lantian.txt", "r");
            break; /*打开蓝田的文档*/
        default:
            break;
    }
    readData(fp);
    data = getCollegeData();
    fclose(fp);
    return data;
}

/*选择学院是在哪一部分？
或者说这里不要函数，直接用选项打开？*/
