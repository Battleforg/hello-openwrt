#include "listener.h"

void save_hotspot(struct raw_hotspot_xml_data* hotspot_pointer);
void save_sta(struct raw_sta_xml_data* sta_pointer); 

// refresh known data of station and hotspot
void refreshAndUpload();