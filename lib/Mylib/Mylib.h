
#ifndef Mylib_H
#define Mylib_H


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include <esp_wifi.h>
#include "nvs_flash.h"
#include "string.h"
#include "freertos/event_groups.h"
//FreeRTOS event group to signal when we are connected
static EventGroupHandle_t s_wifi_event_group;
#define AP_MAX_CONN 4
#define EXAMPLE_ESP_MAXIMUM_RETRY  10
#define AP_CHANNEL 0
//=============================>



#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "wifi station";


//reconnect flag



class wificlass{

    public:
    bool urflag=false;
    char R_STA_SSID[10];
    char R_STA_PASSWORD[10] ;
    int s_retry_num = 0;
    bool WifiSTA(char STA_SSID[],char STA_PASSWORD[]);// Setting up the STA connection
    void WifiAP(char AP_SSID[],char AP_Password[]);///Setting up the AP connection
    bool scan();//For scanning
    bool reconnect();//For reconnecting
    bool print_ip();

};

#endif