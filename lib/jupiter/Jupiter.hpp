#ifndef JUPITER_H
#define JUPITER_H

#include <Arduino.h>
#include <Wire.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "Temperature_LM75_Derived.h"
#include "LIS2DW12Sensor.h"
class Jupiter {
public:
    Jupiter() {};
    void begin() {
        // LIS2DW12Sensor Accelero(&Wire, i2c_lis2d_addr);

        if (accelero == nullptr) {
            accelero = new LIS2DW12Sensor(&Wire, i2c_lis2d_addr);
            accelero->begin();
            accelero->Set_X_ODR_With_Mode(200, LIS2DW12_HIGH_PERFORMANCE_MODE, LIS2DW12_LOW_NOISE_ENABLE);

            accelero->Set_X_FS(2);
            accelero->Set_bw_Mode(lis2dw12_bw_filt_t::LIS2DW12_ODR_DIV_10);
            accelero->Set_FIFO_Mode(lis2dw12_fmode_t::LIS2DW12_FIFO_MODE);
        }

        if (lm75 == nullptr) {
            lm75 = new Generic_LM75_9_to_12Bit_OneShot_Compatible(&Wire, i2c_lm75_addr, &Generic_LM75_11Bit_Attributes);
        }
    };

    float getTemp() {
        if (lm75) {
            lm75->startOneShotConversion();
            lm75->disableShutdownMode();
            Serial.print(lm75->readTemperatureC());
            lm75->enableShutdownMode();
        }
    }
    ~Jupiter() {
        // delete accelero;
        // delete lm75;
    };
private:
    const int led_wifi_pin = 19;
    const int led_cloud_pin = 2;

    const int _scl = 5;
    const int _sda = 4;
    const uint32_t _speed = 400000;
    const int rtc_int_pin = 18;
    const int bt_boot_pin = 0;

    const uint8_t i2c_lis2d_addr = 0x31;
    const uint8_t i2c_lm75_addr = 0x48;
    Generic_LM75_9_to_12Bit_OneShot_Compatible *lm75 = nullptr;
    LIS2DW12Sensor *accelero = nullptr;
};



#endif /* JUPITER_H */