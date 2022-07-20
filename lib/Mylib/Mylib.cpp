    #include "Mylib.h"
    
    
    extern "C"
    {  
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include <esp_wifi.h>
#include "nvs_flash.h"
#include "string.h"
#include "freertos/event_groups.h"
//FreeRTOS event group to signal when we are connected
    
bool wificlass::scan(){
    //printf("12123123");
// configure and run the scan process in blocking mode
//Currently, the esp_wifi_scan_start() API is supported only in Station or Station+AP mode
    wifi_scan_config_t scan_config = {
    .ssid = 0,
    .bssid = 0,
    .channel = 0, 
        .show_hidden = true};

    printf("WiFi Init....\n");
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_start());
    printf("Start scanning...\n");
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
    printf(" completed!\n");

  // get the list of APs found in the last scan
    uint16_t ap_num=20;
    wifi_ap_record_t ap_records[20];
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_num, ap_records));
    
  // print the list 
    printf("Found %d access points:\n", ap_num);

    printf("               SSID              | Channel | RSSI |   MAC \n\n");
  //printf("----------------------------------------------------------------\n");
    for(int i = 0; i < ap_num; i++)
    printf("%32s | %7d | %4d   %2x:%2x:%2x:%2x:%2x:%2x   \n", ap_records[i].ssid, ap_records[i].primary, ap_records[i].rssi , *ap_records[i].bssid, *ap_records[i].bssid+1, *(ap_records[i].bssid)+2, *(ap_records[i].bssid)+3, *(ap_records[i].bssid)+4, *(ap_records[i].bssid)+5);
//  printf("----------------------------------------------------------------\n");   
    return true;
}   

bool wificlass::WifiSTA(char STA_SSID[],char STA_PASSWORD[])
{ 
    
    strcpy(R_STA_SSID,STA_SSID);
    strcpy(R_STA_PASSWORD, STA_PASSWORD);
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(nvs_flash_init());

    tcpip_adapter_init();
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    //Event Handler
    //=========
    //printf("12123123\n");

    wifi_config_t sta_config;
    strcpy((char *)sta_config.sta.ssid,STA_SSID);//This is the SSID of the target AP, to which the station wants to connect to.
    strcpy((char *)sta_config.sta.password, STA_PASSWORD);//Password of the target AP.
    sta_config.sta.channel=0;//If the channel is 0, the station scans the channel 1 ~ N to search for the target AP
    sta_config.sta.bssid_set=0;//If bssid_set is 0, the station connects to the AP whose SSID is the same as the field “ssid”, while the field “bssid” is ignored


    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA,&sta_config));
    printf("starting\n");
    ESP_ERROR_CHECK(esp_wifi_start());// starts wifi usage
    printf("Started\n");
    ESP_ERROR_CHECK(esp_wifi_connect());
    
    


    int g=strlen((char *)sta_config.sta.ssid);
    //printf("%d",g);
    //printf("\n");
    //printf("%s",sta_config.sta.ssid);
    //printf("\n");
    //printf("%s",sta_config.sta.password);
    //printf("\n");
    //printf("%d",strlen((char *)sta_config.sta.password));
    //printf("\n");

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
    number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler()  */
    //print_ip();
    if (esp_wifi_connect()==ESP_OK &&esp_wifi_start()==ESP_OK)
        return true;
    else
        return false;
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);
    //print_ip();
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                STA_SSID, STA_PASSWORD);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                STA_SSID, STA_PASSWORD);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    return true;
  //IPP

// IP address.
  //printf("%s",status());

}

bool wificlass::print_ip()
{
  
    printf("Entering print_ip()...\n");
    esp_netif_ip_info_t ip_info;
    esp_netif_t* netif=NULL;
    netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif == NULL)
        printf("Pointer is NULL.\n");
    else
    {
        // If we got a pointer, show the address.
        printf("Pointer is: 0x%" PRIXPTR "\n", (uintptr_t)netif);

        // Copy the netif IP info into our variable.
        esp_netif_get_ip_info(netif, &ip_info);

        printf("My IP: " IPSTR "\n", IP2STR(&ip_info.ip));
        printf("My GW: " IPSTR "\n", IP2STR(&ip_info.gw));
        printf("My NETMASK: " IPSTR "\n", IP2STR(&ip_info.netmask));
    }
    return true;
        //vTaskDelay(1000 / portTICK_PERIOD_MS);
    
}

void wificlass::WifiAP(char AP_SSID[],char AP_PASSWORD[])
{


    ESP_ERROR_CHECK(nvs_flash_init());//
    
    tcpip_adapter_init();
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));//The main task calls esp_wifi_init() to create the Wi-Fi driver task and initialize the Wi-Fi driver.
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));// esp_wifi_set_mode() (WIFI_MODE_STA) to configure the Wi-Fi mode 


    wifi_config_t ap_config;
    printf("%s",ap_config.ap.ssid);
    strcpy((char *)ap_config.ap.ssid,AP_SSID);//SSID of AP;
    strcpy((char *)ap_config.ap.password, AP_PASSWORD);//Password of AP; if the auth mode is WIFI_AUTH_OPEN, this field will be ignored.
    ap_config.ap.ssid_len = strlen(AP_SSID);           //Length of SSID
    ap_config.ap.max_connection=4;                    //Currently, ESP Wi-Fi supports up to 10 Wi-Fi connections. If max_connection > 10, AP defaults the value to 10.
    ap_config.ap.authmode=WIFI_AUTH_WPA2_PSK;         //Auth mode of ESP AP; currently, ESP Wi-Fi does not support AUTH_WEP. If the authmode is an invalid value, AP defaults the value to WIFI_AUTH_OPEN.
    ap_config.ap.channel=0;                           //Channel of AP; if the channel is out of range, the Wi-Fi driver defaults the channel to channel 1
    printf((char *)ap_config.ap.ssid);
    printf("starting\n");
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP,&ap_config));
    ESP_ERROR_CHECK(esp_wifi_start());//  esp_wifi_start() to start the Wi-Fi driver.
    printf("started\n");
    //ESP_ERROR_CHECK(esp_wifi_connect());
    
    
    while(1) {  vTaskDelay(3000 / portTICK_RATE_MS);
    // scann();
    }


}

/**
 * will force a disconnect and then start reconnecting to AP
 * return true when successful
 */
bool wificlass::reconnect()
{
    //if(esp_wifi_get_mode() & WIFI_MODE_STA) {
        if(esp_wifi_disconnect() == ESP_OK) {
            WifiSTA(R_STA_SSID,R_STA_PASSWORD);
        }
   // }
    return false;
};

}