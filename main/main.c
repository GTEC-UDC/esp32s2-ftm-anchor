#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "cmd_system.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_console.h"


static bool s_reconnect = true;
static const char *TAG_ANCHOR = "gtec-ftm-anchor";

static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;
const int DISCONNECTED_BIT = BIT1;


static void wifi_connected_handler(void *arg, esp_event_base_t event_base,
                                   int32_t event_id, void *event_data)
{
    wifi_event_sta_connected_t *event = (wifi_event_sta_connected_t *)event_data;

    ESP_LOGI(TAG_ANCHOR, "Connected to %s (BSSID: "MACSTR", Channel: %d)", event->ssid,
             MAC2STR(event->bssid), event->channel);

    xEventGroupClearBits(wifi_event_group, DISCONNECTED_BIT);
    xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
}

static void disconnect_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (s_reconnect) {
        ESP_LOGI(TAG_ANCHOR, "sta disconnect, s_reconnect...");
        esp_wifi_connect();
    } else {
        ESP_LOGI(TAG_ANCHOR, "sta disconnect");
    }
    xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
    xEventGroupSetBits(wifi_event_group, DISCONNECTED_BIT);
}


void initialise_wifi(void)
{
    static bool initialized = false;

    if (initialized) {
        return;
    }

    ESP_ERROR_CHECK(esp_netif_init());
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_create_default() );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                    WIFI_EVENT_STA_CONNECTED,
                    &wifi_connected_handler,
                    NULL,
                    NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                    WIFI_EVENT_STA_DISCONNECTED,
                    &disconnect_handler,
                    NULL,
                    NULL));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL) );
    ESP_ERROR_CHECK(esp_wifi_start() );
    initialized = true;
}


static bool start_wifi_ap(const char* ssid, const char* pass)
{
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "",
            .ssid_len = 0,
            .max_connection = 4,
            .password = "",
            .authmode = WIFI_AUTH_WPA2_PSK
        },
    };

    s_reconnect = false;
    strlcpy((char*) wifi_config.ap.ssid, ssid, sizeof(wifi_config.ap.ssid));
    strlcpy((char*) wifi_config.ap.password, pass, sizeof(wifi_config.ap.password));


    if (strlen(pass) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    return true;
}


void app_main(void)
{
    uint8_t mac[6];
    char mac_add[17];

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    initialise_wifi();

    printf("\n ==========================================================\n");
    printf(" |                      ESP32 S2 FTM ANCHOR                 |\n");
    printf(" ==========================================================\n\n");

    //GET MAC ADDRESS
    ESP_ERROR_CHECK(esp_base_mac_addr_get(&mac[0]));
    sprintf(&mac_add[0],"ftm_%02X%02X%02X%02X%02X%02X",(unsigned char)mac[0], (unsigned char)mac[1], (unsigned char)mac[2], (unsigned char)mac[3], (unsigned char)mac[4],(unsigned char)mac[5]);

    //Start AP
    start_wifi_ap(mac_add, "ftmftmftmftm");
    printf("Starting SoftAP with FTM Responder support, SSID - %s\n", mac_add);
}
