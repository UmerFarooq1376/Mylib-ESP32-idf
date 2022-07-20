#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include <esp_wifi.h>
#include "nvs_flash.h"
#include "string.h"
#include "freertos/event_groups.h"
//FreeRTOS event group to signal when we are connected
#include "Mylib.h"
extern "C"
{


void app_main()
{ 
  
  wificlass w;
  
  w.scan();
  vTaskDelay(5000 / portTICK_RATE_MS);

  //Ap Connection
  char AP_S[]= "ESP_Test";
  char AP_P[]= "12345678";
  w.WifiAP(AP_S,AP_P);
  

  //STA Connection
  char STA_S[] ="f10";
  char STA_P[] ="qwerty123";
  
  //w.WifiSTA(STA_S,STA_P);



  vTaskDelay(10000 / portTICK_RATE_MS);
  

  //Disconnectiing and again reconnecting
  printf("%s","Reconnectiing");
  printf("\n");
  vTaskDelay(2000 / portTICK_RATE_MS);
  
  //w.reconnect();
  
  printf("%s","Reconncted");
  printf("\n");
  //

  }
  }
