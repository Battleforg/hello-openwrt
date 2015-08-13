#include <pthread.h>
#include <signal.h>
#include "parser.h"
#include "upload.h"

int interval = 10;
pthread_t thd1, thd2, thd3;
void *thread1(void *arg){
    //写入索引文件
    writeIndex();
    //初始化
    hotspot_records_count = 0;
    sta_records_count = 0;

    //检测是否能成功进行抓包
    if (myPcapCatchAndAnaly() == 0) {
        printf("device error!!\n");
        //产生错误，结束该线程
        pthread_exit(NULL);
    }
    return NULL;
}

void *thread2(void *arg) {
    int counter;
    int limit = 10;
    int wait_time = 2;
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
                    counter = 1;
                    printf("\n------------------------------\n");
                    printf("upload!\n");
                    globalSecond = seconds;
                    while (upload(dataname)!=0) {
                        counter++;
                        if (counter > limit) {
                            pthread_exit(NULL);
                        }
                        //如果上传失败等待(wait_time)秒后重新上传
                        sleep(wait_time);
                        //更新上传时间
                        globalSecond = seconds;
                        printf("\n------------------------------\n");
                        printf("upload %d times\n", counter);
                    }

                }
            } else {
                printf("no data\n");
            }
        }
        //if thread 1 is closed, then thread 2 will also be end
        if(pthread_kill(thd1,0) != 0){
            break;
        }
    }
    return NULL;
}


void *thread3(void *arg){
    int n = 0;
    //检测当前获取的xml文件个数是否超过阈值
    while(1) {
        n = 0;
        while (hotspot_records_count + sta_records_count > 25) {
            sleep(2);
            n++;
            printf("%d\n",(hotspot_records_count+sta_records_count));
            if (n > 4) {
                printf("!!!!!!\n");
                terminate();
                pthread_exit(NULL);
            }
        }
    }
    return NULL;
}

int thread_create(){
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

    if((temp = pthread_create(&thd3, NULL, thread3, NULL)) == 0) {
        printf("线程3被创建\n");
    } else {
        printf("线程3创建失败!\n");
    }

    if (pthread_kill(thd1,0)!=0 && pthread_kill(thd2,0)!=0 && pthread_kill(thd3,0)!=0) {
        return 0;
    }
    return 1;

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
    code = "510002";
    int flag = 0;

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
    printf("2.set upload interval!\n");
    printf("3.set upload information!\n");
    while(scanf("%d",&comd) == 1){
        switch(comd){
            case 1:
                    if (!thread_create()) {
                        printf("error\n");
                    } else {
                        flag = 1;
                    }
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
        while(flag) {
            if (pthread_kill(thd1,0)!=0 && pthread_kill(thd2,0)!=0 && pthread_kill(thd3,0)!=0) {
                return 0;
            }
        }
    }
    return 0;
}
