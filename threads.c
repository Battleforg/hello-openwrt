#include <pthread.h>
#include "parser.h"
#include "upload.h"
char* zip_name[20];
int interval = 10;
pthread_t thd1, thd2;

void *thread1(void *arg) {
    while(1) {
        writeIndex();
        myPcapCatchAndAnaly();
    }
    return NULL;
}

void *thread2(void *arg) {
    int counter;
    int limit = 10;
    char* oldname = "/tmp/group2/zip/data.zip";
    char* dataname = "/tmp/group2/zip/upload.zip";
    long globalSecond = time((time_t*)NULL);
    sleep(interval);
    long seconds = time((time_t*)NULL);
    while(1) {
        seconds = time((time_t*)NULL);
        int d = seconds - globalSecond;
        //printf("%d\n", d);
        if(d >= interval) {
            refreshAndZip();
            if (!access(oldname,F_OK)) {
                if (rename(oldname, dataname) != 0) {
                    perror("rename");
                } else {
                    counter = 0;
                    globalSecond = seconds;
                    do {
                        counter++;
                        if (counter > limit) {
                            pthread_exit(NULL);
                        }
                        printf("\n------------------------------\n");
                        printf("upload %d time(s)\n", counter);
                    } while (upload(dataname)!=0);

                }
            } else {
                printf("no data\n");
            }
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


void folder_create( const char * foldername){
    if(access(foldername,F_OK) != 0){
        mkdir(foldername,0777);
    }
}

int main() {
    int comd;
    const char *origin = "http://jxuao.me/upload?user=group2&filename=data.zip";
    strcpy(urls,origin);

    //创建文件夹
    folder_create("/tmp/group2");
    folder_create("/tmp/group2/data");
    folder_create("/tmp/group2/data/hotspot");
    folder_create("/tmp/group2/data/station");
    folder_create("/tmp/group2/zip");

    //write the index file for zip file

    printf("Please input a number:\n");
    printf("1.run!\n");
    printf("2.set upload interval!\n");
    printf("3.set upload information!\n");
    while(scanf("%d",&comd) == 1){
        switch(comd){
            case 1:
                    thread_create();
                    break;
            case 2:
                    scanf("%d",&interval);
                    printf("upload interval:%d\n",interval);
                    break;
            case 3:
                    seturls();
                    break;
            default:
                    break;
        }
    }

    remove_dir("/tmp/group2/data/hotspot");
    remove_dir("/tmp/group2/data/station");
    remove_dir("/tmp/group2/zip");
    return 0;
}
