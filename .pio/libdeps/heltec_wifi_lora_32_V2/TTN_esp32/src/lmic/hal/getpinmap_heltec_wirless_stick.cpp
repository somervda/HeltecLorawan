/*

Module:  getconfig_ttgo_lora32_v1.cpp

Function:
                Arduino-LMIC C++ HAL pinmap for TTGO ESP32 OLED V1

Copyright & License:
                See accompanying LICENSE file.

Author:
                German Martin, gmag11@gmail.com   June 2019

*/

#include <Arduino.h>

#include "../lmic/oslmic.h"
#include "lmic/arduino_lmic_hal_boards.h"

#define SS 18
#define RST_LoRa 14
#define DIO0 26
#define DIO1 35
#define DIO2 34

namespace TTN_esp32_LMIC
{

    class HalConfiguration_heltec_wireless_stick : public HalConfiguration_t
    {
    public:
        enum DIGITAL_PINS : uint8_t
        {
            PIN_SX1276_NSS = SS,
            PIN_SX1276_NRESET = RST_LoRa,
            PIN_SX1276_DIO0 = DIO0,
            PIN_SX1276_DIO1 = DIO1,
            PIN_SX1276_DIO2 = DIO2,
            PIN_SX1276_ANT_SWITCH_RX = HalPinmap_t::UNUSED_PIN,
            PIN_SX1276_ANT_SWITCH_TX_BOOST = HalPinmap_t::UNUSED_PIN,
            PIN_SX1276_ANT_SWITCH_TX_RFO = HalPinmap_t::UNUSED_PIN,
            PIN_VDD_BOOST_ENABLE = HalPinmap_t::UNUSED_PIN,
        };

        virtual void begin(void) override
        {
            digitalWrite(PIN_SX1276_NSS, 1);
            pinMode(PIN_SX1276_NSS, OUTPUT);
        }

        // virtual void end(void) override

        // virtual ostime_t setModuleActive(bool state) override
    };

    static HalConfiguration_heltec_wireless_stick myConfig;

    static const HalPinmap_t myPinmap = {
        .nss = SS,
        .rxtx = HalPinmap_t::UNUSED_PIN,
        .rst = RST_LoRa,
        .dio = {DIO0, DIO1, DIO2},
    };

    const HalPinmap_t* GetPinmap_heltec_wireless_stick(void) { return &myPinmap; }

}; // namespace TTN_esp32_LMIC
