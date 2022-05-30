#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "Temperature_LM75_Derived.h"
#include "LIS2DW12Sensor.h"
// #include <math.h>

#define M_PI 3.14159265358979323846

const int led_wifi_pin = 19;
const int led_cloud_pin = 2;

const int i2c_scl_pin = 5;
const int i2c_sda_pin = 4;
const int rtc_int_pin = 18;

const int bt_boot_pin = 0;

const uint8_t i2c_lis2d_addr = 0x31;
const uint8_t i2c_lm75_addr = 0x48;

Generic_LM75_9_to_12Bit_OneShot_Compatible lm75(&Wire, i2c_lm75_addr, &Generic_LM75_11Bit_Attributes);
LIS2DW12Sensor Accelero(&Wire, i2c_lis2d_addr);

void setup()
{
    Serial.begin(115200);
    Wire.begin(i2c_sda_pin, i2c_scl_pin);
    Wire.setClock(400000);

    Accelero.begin();
    Accelero.Set_X_ODR_With_Mode(200, LIS2DW12_HIGH_PERFORMANCE_MODE, LIS2DW12_LOW_NOISE_ENABLE);

    Accelero.Set_X_FS(2);
    Accelero.Set_bw_Mode(lis2dw12_bw_filt_t::LIS2DW12_ODR_DIV_10);
    Accelero.Set_FIFO_Mode(lis2dw12_fmode_t::LIS2DW12_FIFO_MODE);
}

float ax0 = 0, ay0 = 0, az0 = 0;
float ax = 0, ay = 0, az = 0;
uint32_t bt_ct = 0;

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(led_wifi_pin, HIGH);
    digitalWrite(led_cloud_pin, HIGH);
    delay(100);
    // digitalWrite(led_wifi_pin, LOW);
    digitalWrite(led_cloud_pin, LOW);
    delay(100);

    Serial.println("Starting temperature conversion...");
    // lm75.startOneShotConversion();
    lm75.disableShutdownMode();

    delay(100);
    Serial.print("temperature is: ");
    Serial.print(lm75.readTemperatureC());
    Serial.print(" C  ");
    lm75.enableShutdownMode();

    delay(250);

    if ((bt_boot_pin) == 0)
    {
        bt_ct = millis() + 5000;
    }

    int32_t accelerometer[3] = {0};
    int16_t accelerometer_tmp[3];
    Accelero.Enable_X();

    for (int x = 0; x < 3; x++)
    {
        Accelero.Set_FIFO_Mode(lis2dw12_fmode_t::LIS2DW12_BYPASS_MODE);
        delay(10);
        Accelero.Set_FIFO_Mode(lis2dw12_fmode_t::LIS2DW12_FIFO_MODE);
        uint16_t num_samples = 0;
        do
        {
            Accelero.Get_FIFO_Num_Samples(&num_samples);
            delay(5);
        } while (num_samples < 32);

        for (size_t i = 0; i < 32; i++)
        {
            Accelero.Get_X_AxesRaw(accelerometer_tmp);
            for (int i = 0; i < 3; i++)
            {
                accelerometer[i] += accelerometer_tmp[i];
            }
        }
    }

    ax = (float)accelerometer[0] / 3200.0f / 3.0f;
    ay = (float)accelerometer[1] / 3200.0f / 3.0f;
    az = (float)accelerometer[2] / 3200.0f / 3.0f;

    if (bt_ct > millis())
    {

        ax0 = ax;
        ay0 = ay;
        az0 = az;
        digitalWrite(led_wifi_pin, HIGH);
    }
    else
    {
        digitalWrite(led_wifi_pin, LOW);
    }

    for (int i = 0; i < 3; i++)
    {
        accelerometer[i] /= (32 * 3);
    }

    float inclination = 180.0 * acos((ax0 * ax + ay0 * ay + az0 * az) / sqrt((ax * ax + ay * ay + az * az) * (ax0 * ax0 + ay0 * ay0 + az0 * az0))) / M_PI;

    Serial.print(" x:");
    Serial.print(accelerometer[0]);
    Serial.print("  y:");
    Serial.print(accelerometer[1]);
    Serial.print("  z:");
    Serial.print(accelerometer[2]);

    Serial.print("  inclination:");
    Serial.print(inclination);
    Serial.println();

    Accelero.Disable_X();
}
