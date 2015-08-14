#ifndef _SAVEXML_H_
#define _SAVEXML_H_
#include "listener.h"
#include "delete.h"

void writeIndex();
void save_hotspot(struct raw_hotspot_xml_data* hotspot_pointer);
void save_sta(struct raw_sta_xml_data* sta_pointer);

// record
int addNewStation(RAW_STA_XML_DATA* raw_pointer);
int addNewHotspot(RAW_HOTSPOT_XML_DATA* raw_pointer);

// refresh known data of station and hotspot
void refreshAndZip();
char *code;

// count the number of records
int hotspot_records_count;
// count the number of records
int sta_records_count;

#endif

