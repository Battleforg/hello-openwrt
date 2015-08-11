#include "listener.h"
#include "saveXML.h"
#include "delete.h"
#include "upload.h"

int GAP;
struct raw_hotspot_xml_data raw;
struct raw_sta_xml_data raw_sta;

long globalSecond;

void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet);
int myPcapCatchAndAnaly();
void folder_create( const char * foldername);