#include "saveXML.h"
int sta_count = 0;
int hotspot_count = 0;

void save_hotspot(struct raw_hotspot_xml_data* hotspot_pointer) 
{
    FILE* stream;
    char filename [52] = "145-510002-";
    long seconds = time((time_t*)NULL);
    char curtime[11];
    sprintf(curtime,"%010ld",seconds);
    strcat(filename,curtime);
    char* line = "-";
    strcat(filename,line);  
    char str[6];
    sprintf(str,"%05d",hotspot_count);
    hotspot_count++;
    strcat(filename,str); 
    char* back = "-WA_SOURCE_FJ_1002-0.xml";
    strcat(filename,back);
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

    /*print*/
    printf("------------------%d----------------------\n", hotspot_count);
    printf("SSID:%s MAC:%s\n rssi:%d Channel:%d\n recieved time:%s\n encryption type:%s\n", hotspot_pointer->ssid, hotspot_pointer->mac, hotspot_pointer->rssi, 
        hotspot_pointer->channel, hotspot_pointer->recieved_time, hotspot_pointer->encryption_type);
}

void save_sta(struct raw_sta_xml_data* sta_pointer) 
{
    FILE* stream;
    char filename [52] = "145-510002-";
    long seconds = time((time_t*)NULL);
    char curtime[11];
    sprintf(curtime,"%010ld",seconds);
    strcat(filename,curtime);
    char* line = "-";
    strcat(filename,line);  
    char str[6];
    sprintf(str,"%05d",sta_count);
    sta_count++;
    strcat(filename,str); 
    char* back = "-WA_SOURCE_FJ_1001-0.xml";
    strcat(filename,back);
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
}
