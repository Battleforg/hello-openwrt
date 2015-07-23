

#include "listener.h"

typedef struct 
{
	/* data */
	u_char version;
	u_char pad;
	u_char len[2];
	u_char* present; // be careful! present may be more than 4*1

}RADIOTAP_C_HEADER;

typedef struct 
{
  /* data */
  u_char frame_control[2];
  u_char duration[2];
  u_char address1[6]; // DA
  u_char address2[6]; // SA
  u_char address3[6]; // BSS ID
  u_char sequence[2];
  // we not have addr4
  // FIXED 12 BYTES parameters
  u_char timestamp[8];
  u_char becon_interval[2];
  u_char capa_info[2]; // capability info

  // SSID
  u_char ssid_tag_number;
  u_char ssid_tag_length;
  u_char ssid[];

}IEEE80211BEACON_FRAME;

typedef struct encry
{
  /* data */
  uint8_t wpa_version;
  uint8_t group_ciphers;
  uint8_t pair_ciphers;
  uint8_t auth_algs;
} ENCRYPTION;

// get SIGNAL in dBm
void getSignal(const RADIOTAP_C_HEADER *rHeader, const u_char * packet)
{
	// skip constant header
	const u_char * currentPos = (u_char*)(packet + 4);
	
	int signal_count = 0;
	int present_count = 0;
	while(currentPos[3] & EXT)
	{
		if (currentPos[0] & SIGNAL)
		{
			/* code */
			signal_count++;
		}
		currentPos += 4;
		present_count++;
	}

	// last present should be added
	if (currentPos[0] & SIGNAL)
	{
		/* code */
		signal_count++;
	}
	currentPos += 4;
	present_count++;

	// after the steps above, we get total amount of present

	
	if ((&(rHeader->present) & SIGNAL) && (signal_count > 0))
	{
		// how many bit we should shift due to TSFT, FLAGS, RATE, CHANNEL and FHSS
		int shift = 0;
		int i;
		char s;
		for (i = 0; i < present_count; ++i)
		{
			/* code */
			if (rHeader->present[4 * i] & TSFT)
			{
				/*  has Time Synchronization Function timer  */
				shift += 8;
			}
			if (rHeader->present[4 * i] & FLAGS)
			{
				/*  has flags */
				shift += 1;
			}

			if (rHeader->present[4 * i] & RATE)
			{
				/*  has data rate */
				shift += 1;
			}

			if (rHeader->present[4 * i] & CHANNEL)
			{
				/*  has channel information */
				shift += 4;
			}

			 if (rHeader->present[4 * i] & FHSS)
			{
				/*  has FHSS */
				shift += 2;
			}

			if (rHeader->present[4 * i] & SIGNAL)
			{
				/* code */
				s = currentPos[shift];
				s += (~s + 1) * -1;
				shift += 1;
			}

			if (rHeader->present[4 * i] & NOISE)
			{
				/* code */
				shift += 1;
			}
			if (rHeader->present[4 * i] & LOCK)
			{
				/* code */
				shift += 2;
			}
			if (rHeader->present[4 * i + 1] & ANTENNA)
			{
				/* code */
				shift += 1;
			}
			if (rHeader->present[4 * i + 1] & RX_FLAGS)
			{
				/* code */
				if ((16 - shift) == 1)
				{
					// if  RX alignment does not meet requirement(2)
					shift += 1;
				}
				shift += 2;
			}

			currentPos += shift ;
			shift = 0;
		}
		
		s /= signal_count;
		
		printf("SSI signal: %d dBm\n", s);
	}
}

// get channel
void getChannel(const RADIOTAP_C_HEADER *rHeader,const u_char * packet)
{
	// skip constant header
	const u_char * currentPos  = (u_char*)(packet + 4);
	
	int present_count = 0;
	while(currentPos[3] & EXT)
	{
		currentPos += 4;
		present_count++;
	}

	// last present should be added
	currentPos += 4;
	present_count++;

	int shift = 0;
	/* code */
	if (rHeader->present[0] & TSFT)
	{
		/*  has Time Synchronization Function timer  */
		shift += 8;
	}
	if (rHeader->present[0] & FLAGS)
	{
		/*  has flags */
		shift += 1;
	}

	if (rHeader->present[0] & RATE)
	{
		/*  has data rate */
		shift += 1;
	}

	if (rHeader->present[0] & CHANNEL)
	{
		/* calculate channel type */
		int t1 = currentPos[shift+1];
		int t2 = currentPos[shift];
		printf("channel:  ");

		switch((t1<<8) + t2)
		{
			case CHANNEL_1:
			printf("1");
			break;
			case CHANNEL_2:
			printf("2");
			break;
		   case CHANNEL_3:
		   printf("3");
		   break;
		   case CHANNEL_4:
		   printf("4");
		   break;
		   case CHANNEL_5:
		   printf("5");
		   break;
		   case CHANNEL_6:
		   printf("6");
		   break;
		   case CHANNEL_7:
		   printf("7");
		   break;
		   case CHANNEL_8:
		   printf("8");
		   break;
		   case CHANNEL_9:
		   printf("9");
		   break;
		   case CHANNEL_10:
		   printf("10");
		   break;
		   case CHANNEL_11:
		   printf("11");
		   break;
		   case CHANNEL_12:
		   printf("12");
		   break;
		   case CHANNEL_13:
		   printf("13");
		   break;
		   case CHANNEL_14:
		   printf("14");
		   break;
		   default:
		   break;
		}
		printf("\n");
	}
}


// print encryption informaiton
void print_encry(ENCRYPTION * e)
{
  if (!e)
  {
	/* code */
	printf("unknown\n");
  }
  else 
  {
	if (!e->wpa_version)
	{
	  printf("WEP\n");
	}
	else
	{
	  switch(e->wpa_version)
	  {
		case 3:
		printf("mixed WPA/WPA2 "); break;
		case 2:
		printf("WPA2 "); break;
		case 1: 
		printf("WPA "); break;
		default: break;
	  }

	 switch(e->group_ciphers)
	 {
		case MY_CIPHER_NONE:
		printf("NONE\n"); break;
		case MY_CIPHER_WEP40:
		printf(" WEP40\n"); break;
		case MY_CIPHER_TKIP:
		printf("TKIP\n"); break;
		case MY_CIPHER_WRAP:
		printf("WRAP\n"); break;
		case MY_CIPHER_CCMP:
		printf("CCMP\n"); break;
		case MY_CIPHER_WEP104:
		printf("WEP104\n"); break;
		case MY_CIPHER_AESOCB:
		printf("AESOCB\n"); break;
		 case MY_CIPHER_CKIP:
		printf("CKIP\n"); break;
	 }

	}
  }
}


void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet)
{
	int * id  = (int *)arg;
	RADIOTAP_C_HEADER * rHeader = (RADIOTAP_C_HEADER*)packet;
	// calculate radiotap length
	 int l1= rHeader->len[1];
	 int l2 = rHeader->len[0];
	 int radiotap_len = (l1 << 8) + l2;
	
	// packet len which have not been visited yet
	int len_count = pkthdr->len;
	// printf("Packet length: %d\n", pkthdr->len);
	// printf("Number of bytes: %d\n", pkthdr->caplen);
	
	  // skip to 802.11frame header
	 const IEEE80211BEACON_FRAME * bHeader = (IEEE80211BEACON_FRAME*)(packet + radiotap_len);

	 int i;
	 
	 if ((bHeader->frame_control[0]) == BEACON_FRAME &&
		bHeader->frame_control[1] == 0x00)
	 {
		// printf("\nversion: %d\n", rHeader->version);
		// printf("pad: %d\n", rHeader->pad);

	 

		// printf("len: %d\n", radiotap_len);  
		// printf("present: %02X-%02X-%02X-%02X\n", rHeader->present[0], rHeader->present[1], rHeader->present[2], rHeader->present[3]);

		printf("id: %d\n", ++(*id));
		printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec)); 

		// get signal and channel information
		getSignal(rHeader, packet);
		getChannel(rHeader, packet);

	

		// update len_count;
		len_count -= radiotap_len;

		printf("source MAC : %02X-%02X-%02X-%02X-%02X-%02X\n", bHeader->address2[0], bHeader->address2[1], 
		   bHeader->address2[2], bHeader->address2[3], bHeader->address2[4], bHeader->address2[5]);

		printf("SSID : ");
		if (bHeader -> ssid_tag_length)
		{
			/* code */
			// get SSID 
			for (i = 0; i < bHeader->ssid_tag_length; ++i)
			{
				/* code */
				printf("%c", bHeader->ssid[i]);
			}
		}
		else
		{
			printf("Broadcast\n");
		}
		

			printf("\n");
	  
		ENCRYPTION e;
		e.wpa_version = 0;
		e.group_ciphers = 0;
		e.pair_ciphers = 0;
		e.auth_algs = 0;

		static unsigned char wpa_oui[3] = {0x00, 0x50, 0xf2};
		static unsigned char wpa2_oui[3] = {0x00, 0x0f, 0xac};
		// begin scan data to find rsn and wpa type 
		const u_char * p_encryption = &(bHeader->ssid[bHeader->ssid_tag_length]);
		len_count -= (38 + bHeader->ssid_tag_length);

		while (len_count > 0)
		{
			// get tag number;
			int tag_num = *p_encryption;
			// move to tag length and get it
			p_encryption +=1;
			len_count -= 1;
			int tag_len = *p_encryption;
			// if tag number is RSN
			if (tag_num == RSN)
			{
				if (!memcmp(p_encryption + 3, wpa_oui, 3))
			 	{
					/* code */
					e.wpa_version +=1;
				 }
			 	else if (!memcmp(p_encryption + 3, wpa2_oui, 3))
		   		{
			  		 /* code */
			 		 e.wpa_version +=2;
		   		}

				  if (!memcmp(p_encryption + 3, wpa_oui, 3) || !memcmp(p_encryption + 3, wpa2_oui, 3))
				 {
			 	 	/* code */
			  		switch(p_encryption[6])
					{
						case 1: e.group_ciphers |= MY_CIPHER_WEP40; break;
						case 2: e.group_ciphers |= MY_CIPHER_TKIP; break;
						case 4: e.group_ciphers |= MY_CIPHER_CCMP; break;
						case 5: e.group_ciphers |= MY_CIPHER_WEP104; break;
						case 6: /* AES-128-MAC */ break;
						default: break;
			  		}
				 }

			}

			// if tag is VERDOR_SPECIFIC
			else if (tag_num == VERDOR_SPECIFIC)
			{
		 		 if (!memcmp(p_encryption + 1, wpa_oui, 3) && p_encryption[4] == 1)
		  		{
					e.wpa_version +=1;
		  		}
			}

			p_encryption += (tag_len + 1);
			len_count -= (tag_len + 1);
	 	 }
	  	print_encry(&e);
	 	printf("\n\n");
	 }

  
}


int main()
{
	/* code */
	char errBuf[PCAP_ERRBUF_SIZE], * devStr;
	devStr = "wlan0";

	/* open a device, wait until a packet arrives */
	pcap_t * device = pcap_open_live(devStr, 65535, 1, 0, errBuf);
  
	 if(!device)
	 {
		 printf("error: pcap_open_live(): %s\n", errBuf);
		 exit(1);
	  }
  
	

	 /* wait loop forever */
	 int id = 0;
	pcap_loop(device, -1, getPacket, (u_char*)&id);
  
	pcap_close(device);
	return 0;
}
