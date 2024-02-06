# ESP32 S2 FTM Anchor

This software is based on this example: [https://github.com/espressif/esp-idf/tree/master/examples/wifi/ftm](https://github.com/espressif/esp-idf/tree/master/examples/wifi/ftm).

The software launches on start a soft AP with FTM responder support. The *ssid* of the AP is *ftm_MAC*, where *MAC* is the MAC address of the module.

This software is intended to be used in conjunction with the ESP32 S2 FTM TAG, which is available here: [https://github.com/GTEC-USC/esp32s2-ftm-tag](https://github.com/GTEC-UDC/esp32s2-ftm-tag)

## Use

Just compile a flash it in a ESP32 S2 module using the ESPRESSIF instructions: [https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#introduction](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#introduction)

## Cite

The code in this repository is related to the following work:

*V. Barral Vales, O. C. Fernández, T. Domínguez-Bolaño, C. J. Escudero and J. A. García-Naya, "Fine Time Measurement for the Internet of Things: A Practical Approach Using ESP32," in IEEE Internet of Things Journal, vol. 9, no. 19, pp. 18305-18318, 1 Oct.1, 2022, doi: 10.1109/JIOT.2022.3158701.* 

If you make use of this code, a citation is appreciated.
