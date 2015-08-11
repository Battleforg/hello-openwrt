#include "listener.h"
void writeIndex();
void save_hotspot(struct raw_hotspot_xml_data* hotspot_pointer);
void save_sta(struct raw_sta_xml_data* sta_pointer); 

// record
int addNewStation(RAW_STA_XML_DATA* raw_pointer);
int addNewHotspot(RAW_HOTSPOT_XML_DATA* raw_pointer);
int zip_num;
// refresh known data of station and hotspot
void refreshAndUpload();