# ESP32 S2 FTM Anchor

This software is based on this example: [https://github.com/espressif/esp-idf/tree/master/examples/wifi/ftm](https://github.com/espressif/esp-idf/tree/master/examples/wifi/ftm).

The software launches on start a soft AP with FTM responder support. The *ssid* of the AP is *ftm_MAC*, where *MAC* is the MAC address of the module.

This software is intended to be used in conjunction with the ESP32 S2 FTM TAG, which is available here:

## USE

Just compile a flash it in a ESP32 S2 module using the ESPRESSIF instructions: [https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#introduction](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#introduction)