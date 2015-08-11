#include <pthread.h>
#include "parser.h"
char* zip_name[20];

pthread_t thd1, thd2;

void name_ini(){
    int i;
    for (i = 0; i < 20; i++) {
        char name[50] = "/tmp/group2/zip/data";

        char str[2];
        sprintf(str,"%02d",i);

        printf("%s\n", str);
        strcat(name,str);
        printf("%s\n", name);
        strcat(name,".zip");
        zip_name[i] = name;
        printf("%s\n", zip_name[i]);
    }
}

void *thread1(void *arg) {
    while(1){
        writeIndex();
        myPcapCatchAndAnaly();
    }
    return NULL;
}

void *thread2(void *arg) {
    int estab_count = 0;
    int upload_count = 0;
    printf("thread2!\n");
    char tmp[50];
    name_ini();
    while(1) {
        if (zip_num > 0) {

            rename("/tmp/group2/zip/data.zip",zip_name[estab_count]);
            zip_num--;
            estab_count++;
            if (estab_count == 20) {
                estab_count = 0;
                if(estab_count == upload_count) {
                    break;
                }
            }
            if(estab_count - upload_count == -1) {
                break;
            }
        }
        if (upload(zip_name[upload_count]) == 0) {
            strcpy(tmp,"");
            strcat(tmp,"rm -rf ");
            strcat(tmp,zip_name[upload_count]);
            system(tmp);
            upload_count++;
            if(upload_count == 20) {
                upload_count = 0;
            }
        } else {
            continue;
        }
    }
    return NULL;
}

void thread_create(){
    int temp;
    if((temp = pthread_create(&thd1, NULL, thread1, NULL)) == 0) {
        printf("线程1被创建\n");
    } else {
        printf("线程1创建失败!\n");
    }

    if((temp = pthread_create(&thd2, NULL, thread2, NULL)) == 0) {
        printf("线程2被创建\n");
    } else {
        printf("线程2创建失败\n");
    }
}



int main() {
    zip_num = 0;
    GAP = 5;
    int comd;
    const char *origin = "http://jxuao.me/upload?user=group2&filename=data.zip";
    strcpy(urls,origin);

    //创建文件夹
    folder_create("/tmp/group2");
    folder_create("/tmp/group2/data");
    folder_create("/tmp/group2/data/hotspot");
    folder_create("/tmp/group2/data/station");
    folder_create("/tmp/group2/zip");
    remove_dir("/tmp/group2/data/hotspot");
    remove_dir("/tmp/group2/data/station");
    remove_dir("/tmp/group2/zip");

    //write the index file for zip file

    printf("Please input a number:\n");
    printf("1.run!\n");
    printf("2.set upload information\n");
    while(scanf("%d",&comd) == 1){
        switch(comd){
            case 1:
                    thread_create();
                    break;
            case 2:
                    seturls();
                    break;
            default:
                    break;
        }
    }
    return 0;
}
