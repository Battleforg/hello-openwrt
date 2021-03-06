#include "saveXML.h"

//write GAB_ZIP_INDEX.xml
void writeIndex() {
    FILE* stream;
    char filename[] = "/tmp/group2/data/GAB_ZIP_INDEX.xml";
    //if the index file exists
    if (fopen(filename, "r")) {
        return ;
    }

    if (fopen(filename, "w")) {
        stream = fopen(filename, "w");
        fprintf(stream, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(stream, "<MESSAGE>\n");
        fprintf(stream, "\t<DATASET name=\"WA_COMMON_010017\" ver=\"1.0\" rmk=\"数据文件索引信息\">\n");
        fprintf(stream, "\t\t<DATA>\n");
        fprintf(stream, "\t\t\t<DATASET name=\"WA_COMMON_010013\" rmk=\"BCP文件描述信息\">\n");
        fprintf(stream, "\t\t\t\t<DATA>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"I010032\" val=\"\" rmk=\"列分隔符（缺少值时默认为换行符\\t）\"/>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"I010033\" val=\"\" rmk=\"行分隔符（缺少值时默认为换行符\\n）\"/>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"A010004\" val=\"WA_SOURCE_FJ_1001\" rmk=\"数据集代码\"/>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"B050016\" val=\"145\" rmk=\"数据来源\"/>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"G020013\" val=\"xxxxxx\" rmk=\"网安专用产品厂家组织机构代码\"/>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"E010008\" val=\"510002\" rmk=\"数据采集地\"/>\n");
        fprintf(stream, "\t\t\t\t\t<!-- 描述BCP文件中的数据列顺序 -->\n");
        fprintf(stream, "\t\t\t\t\t<DATASET name=\"WA_COMMON_010015\" rmk=\"BCP文件数据结构\">\n");
        fprintf(stream, "\t\t\t\t\t\t<DATA>\n");
        fprintf(stream, "\t\t\t\t\t\t\t<ITEM key=\"C040002\" eng=\"MAC\" chn=\"终端MAC地址\"/>\n");
        fprintf(stream, "\t\t\t\t\t\t\t<ITEM key=\"H010014\" eng=\"CAPTURE_TIME\" chn=\"采集时间\"/>\n");
        fprintf(stream, "\t\t\t\t\t\t\t<ITEM key=\"I070003\" eng=\"TERMINAL_FIELD STRENGTH\" chn=\"被采终端场强\"/>\n");
        fprintf(stream, "\t\t\t\t\t\t</DATA>\n");
        fprintf(stream, "\t\t\t\t\t</DATASET>\n");
        fprintf(stream, "\t\t\t\t</DATA>\n");
        fprintf(stream, "\t\t\t\t<DATA>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"I010032\" val=\"\" rmk=\"列分隔符（缺少值时默认为换行符\\t）\"/>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"I010033\" val=\"\" rmk=\"行分隔符（缺少值时默认为换行符\\n）\"/>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"A010004\" val=\"WA_SOURCE_FJ_1002\" rmk=\"数据集代码\"/>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"B050016\" val=\"145\" rmk=\"数据来源\"/>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"G020013\" val=\"xxxxxx\" rmk=\"网安专用产品厂家组织机构代码\"/>\n");
        fprintf(stream, "\t\t\t\t\t<ITEM key=\"E010008\" val=\"510002\" rmk=\"数据采集地\"/>\n");
        fprintf(stream, "\t\t\t\t\t<!-- 描述BCP文件中的数据列顺序 -->\n");
        fprintf(stream, "\t\t\t\t\t<DATASET name=\"WA_COMMON_010015\" rmk=\"BCP文件数据结构\">\n");
        fprintf(stream, "\t\t\t\t\t\t<DATA>\n");
        fprintf(stream, "\t\t\t\t\t\t\t<ITEM key=\"F030011\" eng=\"AP_MAC\" chn=\"热点MAC地址\"/>\n");
        fprintf(stream, "\t\t\t\t\t\t\t<ITEM key=\"F030001\" eng=\"AP_SSID\" chn=\"热点SSID\"/>\n");
        fprintf(stream, "\t\t\t\t\t\t\t<ITEM key=\"F030022\" eng=\"AP_CHANNEL\" chn=\"热点频道\"/>\n");
        fprintf(stream, "\t\t\t\t\t\t\t<ITEM key=\"B040025\" eng=\"mixed WPA/WPA2CCMP\" chn=\"热点加密类型\"/>\n");
        fprintf(stream, "\t\t\t\t\t\t\t<ITEM key=\"H010014\" eng=\"CAPTURE_TIME\" chn=\"采集时间\"/>\n");
        fprintf(stream, "\t\t\t\t\t\t\t<ITEM key=\"F030023\" eng=\"AP_FIELD STRENGTH\" chn=\"热点场强\"/>\n");
        fprintf(stream, "\t\t\t\t\t\t</DATA>\n");
        fprintf(stream, "\t\t\t\t\t</DATASET>\n");
        fprintf(stream, "\t\t\t\t</DATA>\n");
        fprintf(stream, "\t\t\t\t<!-- 多种不同结构的文件可以使用多条记录描述 -->\n");
        fprintf(stream, "\t\t\t</DATASET>\n");
        fprintf(stream, "\t\t</DATA>\n");
        fprintf(stream, "\t</DATASET>\n");
        fprintf(stream, "</MESSAGE>\n");
        fclose(stream);
    }
}

//record  MAC addresses of  known hotspot
char knownHotspotMAC[PACKET_NUMBER][20];

// add new hotspot record to knownHotspotMAC
int addNewHotspot(RAW_HOTSPOT_XML_DATA* raw_pointer) {
    // first find out if the hotspot has already in record.
    int i;
    for (i = 0; i < hotspot_records_count && i < PACKET_NUMBER; ++i) {
        // if the hotspot is in record
        if (!strcmp(raw_pointer ->mac, knownHotspotMAC[i])) {
            // do nothing but return 0 means old record is detected
            return 0;
        }
    }
    // add new hotspot record and record is not full
    if (hotspot_records_count < PACKET_NUMBER) {
        strcpy(knownHotspotMAC[hotspot_records_count], raw_pointer->mac);
    }
    // update record count
    hotspot_records_count++;
    // new record has been added and return 1
    return 1;
}

void save_hotspot(struct raw_hotspot_xml_data* hotspot_pointer) {
    FILE* stream;
    char filename[80] = "/tmp/group2/data/hotspot/145-";
    strcat(filename,code);
    char* line = "-";
    strcat(filename,line);

    long seconds = time((time_t*)NULL);
    char curtime[11];
    sprintf(curtime,"%010ld",seconds);
    strcat(filename,curtime);
    strcat(filename,line);
    char str[6];
    sprintf(str,"%05d",hotspot_records_count);
    strcat(filename,str);
    char* back = "-WA_SOURCE_FJ_1002-0.xml";
    strcat(filename,back);

    if (fopen(filename, "w")) {
        stream = fopen(filename, "w");
        fprintf(stream, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(stream, "<MESSAGE>\n");
        fprintf(stream, "\t<DATASET name=\"WA_SOURCE_FJ_1002\" ver=\"1.0\" rmk=\"被采集热点信息\">\n");
        fprintf(stream, "\t\t<DATA>\n");
        fprintf(stream, "\t\t\t<ITEM key=\"F030011\" val=\"%s\" chn=\"热点MAC地址\" eng=\"AP_MAC\" fmt=\"c\" datatype=\"VARCHAR\" length=\"17\" nullable=\"NO\"/>\n", hotspot_pointer->mac);
        fprintf(stream, "\t\t\t<ITEM key=\"F030001\" val=\"%s\" chn=\"热点SSID\" eng=\"AP_SSID\" fmt=\"c\" datatype=\"VARCHAR\" length=\"64\" nullable=\"YES\"/>\n", hotspot_pointer->ssid);
        fprintf(stream, "\t\t\t<ITEM key=\"F030022\" val=\"%d\" chn=\"热点频道\" eng=\"AP_CHANNEL\" fmt=\"c\" datatype=\"VARCHAR\" length=\"2\" nullable=\"NO\"/>\n", hotspot_pointer->channel);
        fprintf(stream, "\t\t\t<ITEM key=\"B040025\" val=\"%s\" chn=\"热点加密类型\" eng=\"ENCRYPT_ALGORITHM_TYPE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"2\" nullable=\"NO\"/>\n", hotspot_pointer->encryption_type);
        fprintf(stream, "\t\t\t<ITEM key=\"H010014\" val=\"%s\" chn=\"采集时间\" eng=\"CAPTURE_TIME\" fmt=\"n\" datatype=\"INT\" length=\"20\" nullable=\"NO\"/>\n", hotspot_pointer->recieved_time);
        fprintf(stream, "\t\t\t<ITEM key=\"F030023\" val=\"%d\" chn=\"热点场强\" eng=\"AP_FIELD STRENGTH\" fmt=\"c\" datatype=\"VARCHAR\" length=\"8\" nullable=\"NO\"/>\n", hotspot_pointer->rssi);
        //fprintf(stream, "\t\t\t<ITEM key=\"I070001\" val=\"%s\" chn=\"X坐标\" eng=\"X_COORDINATE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"8\" nullable=\"YES\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"I070002\" val=\"%s\" chn=\"Y坐标\" eng=\"Y_COORDINATE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"8\" nullable=\"YES\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"G020004\" val=\"%s\" chn=\"场所编号\" eng=\"NETBAR_WACODE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"14\" nullable=\"NO\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"I070011\" val=\"%s\" chn=\"采集设备编号\" eng=\"COLLECTION_EQUIPMENT ID\" fmt=\"c\" datatype=\"VARCHAR\" length=\"21\" nullable=\"NO\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"F010018\" val=\"%s\" chn=\"采集设备经度\" eng=\"COLLECTION_EQUIPMENT_LONGITUDE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"10\" nullable=\"NO\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"F010019\" val=\"%s\" chn=\"采集设备纬度\" eng=\"COLLECTION_EQUIPMENT_LATITUDE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"10\" nullable=\"NO\"/>\n",s);
        fprintf(stream, "\t\t</DATA>\n");
        fprintf(stream, "\t</DATASET>\n");
        fprintf(stream, "</MESSAGE>\n");
        fclose(stream);
    } else {
        printf("fail to save xml!\n");
    }

    // printf("------hotspot------------%d----------------------\n", hotspot_records_count);
    // printf(" SSID:%s MAC:%s\n rssi:%d Channel:%d\n recieved time:%s\n encryption type:%s\n", hotspot_pointer->ssid, hotspot_pointer->mac, hotspot_pointer->rssi,
    //     hotspot_pointer->channel, hotspot_pointer->recieved_time, hotspot_pointer->encryption_type);

}

// record MAC addresses of know station
char knownStaMAC[PACKET_NUMBER][20];

// add new station record to knownStaMAC
int addNewStation(RAW_STA_XML_DATA* raw_pointer) {
    // first find out if the staion has already in record.
    int i;
    for (i = 0; i < sta_records_count && i < PACKET_NUMBER; ++i) {
        // if the hotspot is in record
        if (!strcmp(raw_pointer ->mac, knownStaMAC[i])) {
            // do nothing but return 0 means old record is detected
            return 0;
        }
    }
    // add new hotspot record and record is not full
    if (sta_records_count < PACKET_NUMBER) {
        strcpy(knownStaMAC[sta_records_count], raw_pointer->mac);
    }
    // update record count
    sta_records_count++;
    // new record has been added and return 1
    return 1;
}

void save_sta(struct raw_sta_xml_data* sta_pointer) {
    FILE* stream;

    char filename [80] = "/tmp/group2/data/station/145-";
    strcat(filename,code);
    char* line = "-";
    strcat(filename,line);

    long seconds = time((time_t*)NULL);
    char curtime[11];
    sprintf(curtime,"%010ld",seconds);
    strcat(filename,curtime);
    strcat(filename,line);
    char str[6];
    sprintf(str,"%05d",sta_records_count);
    strcat(filename,str);
    char* back = "-WA_SOURCE_FJ_1001-0.xml";
    strcat(filename,back);
    if (fopen(filename, "w")) {
        stream = fopen(filename, "w");
        fprintf(stream, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(stream, "<MESSAGE>\n");
        fprintf(stream, "\t<DATASET name=\"WA_BASIC_FJ_1001\" ver=\"1.0\" rmk=\"终端特征信息\">\n");
        fprintf(stream, "\t\t<DATA>\n");
        fprintf(stream, "\t\t\t<ITEM key=\"C040002\" val=\"%s\" chn=\"终端MAC地址\" eng=\"MAC\" fmt=\"c\" datatype=\"VARCHAR\" length=\"17\" nullable=\"NO\"/>\n",sta_pointer->mac);
        //fprintf(stream, "\t\t\t<ITEM key=\"C010002\" val=\"%s\" chn=\"终端品牌\" eng=\"BRAND\" fmt=\"c\" datatype=\"VARCHAR\" length=\"128\" nullable=\"YES\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"F030021\" val=\"%s\" chn=\"终端历史SSID列表\" eng=\"CACHE_SSID\" fmt=\"c\" datatype=\"VARCHAR\" length=\"64\" nullable=\"YES\"/>\n",s);
        fprintf(stream, "\t\t\t<ITEM key=\"H010014\" val=\"%s\" chn=\"采集时间\" eng=\"CAPTURE_TIME\" fmt=\"n\" datatype=\"INT\" length=\"20\" nullable=\"NO\"/>\n",sta_pointer->recieved_time);
        fprintf(stream, "\t\t\t<ITEM key=\"I070003\" val=\"%d\" chn=\"被采终端场强\" eng=\"TERMINAL_FIELD STRENGTH\" fmt=\"c\" datatype=\"VARCHAR\" length=\"8\" nullable=\"NO\"/>\n",sta_pointer->rssi);
        //fprintf(stream, "\t\t\t<ITEM key=\"B040001\" val=\"%s\" chn=\"身份类型\" eng=\"IDENTIFICATION_TYPE\" fmt=\"n\" datatype=\"INT\" length=\"1\" nullable=\"YES\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"B040002\" val=\"%s\" chn=\"身份内容\" eng=\"CERTIFICATE_CODE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"64\" nullable=\"YES\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"I070005\" val=\"%s\" chn=\"接入热点SSID\" eng=\"SSID_POSITION\" fmt=\"c\" datatype=\"VARCHAR\" length=\"256\" nullable=\"YES\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"I070006\" val=\"%s\" chn=\"接入热点MAC\" eng=\"ACCESS_AP_MAC\" fmt=\"c\" datatype=\"VARCHAR\" length=\"17\" nullable=\"YES\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"I070007\" val=\"%s\" chn=\"接入热点频道\" eng=\"ACCESS_AP_CHANNEL\" fmt=\"c\" datatype=\"VARCHAR\" length=\"2\" nullable=\"YES\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"I070008\" val=\"%s\" chn=\"接入热点加密类型\" eng=\"ACCESS_AP_ENCRYPTION_TYPE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"2\" nullable=\"YES\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"I070001\" val=\"%s\" chn=\"X坐标\" eng=\"X_COORDINATE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"8\" nullable=\"YES\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"I070002\" val=\"%s\" chn=\"Y坐标\" eng=\"Y_COORDINATE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"8\" nullable=\"YES\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"G020004\" val=\"%s\" chn=\"场所编号\" eng=\"NETBAR_WACODE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"14\" nullable=\"NO\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"I070011\" val=\"%s\" chn=\"采集设备编号\" eng=\"COLLECTION_EQUIPMENT ID\" fmt=\"c\" datatype=\"VARCHAR\" length=\"21\" nullable=\"NO\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"F010018\" val=\"%s\" chn=\"采集设备经度\" eng=\"COLLECTION_EQUIPMENT LONGITUDE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"10\" nullable=\"NO\"/>\n",s);
        //fprintf(stream, "\t\t\t<ITEM key=\"F010019\" val=\"%s\" chn=\"采集设备纬度\" eng=\"COLLECTION_EQUIPMENT LATITUDE\" fmt=\"c\" datatype=\"VARCHAR\" length=\"10\" nullable=\"NO\"/>\n",s);
        fprintf(stream, "\t\t</DATA>\n");
        fprintf(stream, "\t</DATASET>\n");
        fprintf(stream, "</MESSAGE>\n");
        fclose(stream);
    } else {
        printf("fail to save xml!\n");
    }

    // printf("-----------sta-------%d----------------------\n", sta_records_count);
    // printf(" MAC:%s\n rssi:%d recieved time:%s\n", sta_pointer->mac, sta_pointer->rssi,
    //     sta_pointer->recieved_time);
}

// refresh known data of station and hotspot
void refreshAndZip() {
    // clear known station and hotspot
    int i;
    for (i = 0; i < hotspot_records_count; ++i) {
        memset(knownHotspotMAC[i], 0, sizeof(knownHotspotMAC[i]));
    }

    for (i = 0; i < sta_records_count; ++i) {
        memset(knownStaMAC[i], 0, sizeof(knownStaMAC[i]));
    }

    /* zip
     * -r 递归处理，将指定目录下的所有文件和子目录一并处理。
     * -q 不显示指令执行过程。
    **/
    system("zip -r -q /tmp/group2/zip/data.zip /tmp/group2/data");
    remove_dir("/tmp/group2/data/hotspot");
    hotspot_records_count = 0;
    remove_dir("/tmp/group2/data/station");
    sta_records_count = 0;

}
