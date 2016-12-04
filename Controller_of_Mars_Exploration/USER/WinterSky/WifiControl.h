#ifndef __WIFICONTROL_H
#define __WIFICONTROL_H

#include <stm32f10x.h>

#define _HOST "192.168.0.108"
//#define _HOST "10.16.0.234"
//#define _HOST "10.16.0.13"
//#define _HOST "115.25.162.122"
//#define _HOST "115.25.162.120"
//#define _HOST "10.18.196.202"
//#define _HOST "172.16.9.33"
//#define _HOST "10.18.75.217"
//#define _HOST "172.16.9.18"

#define _PORT 8080
//#define _PORT 5432


extern u8 bConnected;

void WIFI_Config(void);
void WIFI_ClientConfig(void);
void WIFI_ConnectToServer(void);
void WIFI_WaitForConnect(void);
void WIFI_Send(char _str[]);
void WIFI_Check(void);
void onExternData(u8 _s, u8 _e);

#endif
