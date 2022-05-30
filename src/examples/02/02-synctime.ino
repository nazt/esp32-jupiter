#include <Wire.h>
#include <NTP.hpp>
#include <SPI.h>
#include "Rtc_Pcf8563.h"
#include <WiFi.h>

const char *ssid = "Lily_2.4G";
const char *password = "zxc12345";

NTP ntpModule;

const int i2c_scl_pin = 5;
const int i2c_sda_pin = 4;
const int rtc_int_pin = 18;

const uint8_t i2c_pcf8563_addr = 0x51;
Rtc_Pcf8563 rtc(i2c_pcf8563_addr);

void setup()
{
    Serial.begin(115200);
    Serial.println("Start..");

    Wire.begin(i2c_sda_pin, i2c_scl_pin);
    Wire.setClock(400000);
    rtc.initClock();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected.");
    ntpModule.setup();

    // rtc.setDate(14, 6, 3, 1, 10);
    // // hr, min, sec
    // rtc.setTime(1, 15, 0);
}

void loop()
{
    ntpModule.loop();
    if (ntpModule.isSynced)
    {
        Serial.println(&ntpModule.timeinfo, "%A, %B %d %Y %H:%M:%S");
        // RtcDateTime dt = RtcDateTime(ntpModule.timeinfo.tm_year, ntpModule.timeinfo.tm_mon + 1, ntpModule.timeinfo.tm_mday, ntpModule.timeinfo.tm_hour, ntpModule.timeinfo.tm_min, ntpModule.timeinfo.tm_sec);
        rtc.setDate(ntpModule.timeinfo.tm_mday, ntpModule.timeinfo.tm_wday, ntpModule.timeinfo.tm_mon + 1, 1, ntpModule.timeinfo.tm_year);
        rtc.setTime(ntpModule.timeinfo.tm_hour, ntpModule.timeinfo.tm_min, ntpModule.timeinfo.tm_sec);
        // print from rtc
        Serial.print("Hour:");
        Serial.print(rtc.getHour());
        Serial.print(" Minute:");
        Serial.print(rtc.getMinute());
        Serial.print(" Second:");
        Serial.print(rtc.getSecond());
        Serial.print(" Weekday:");
        Serial.print(rtc.getWeekday());
        Serial.print(" Day:");
        Serial.print(rtc.getDay());
        Serial.print(" Month:");
        Serial.print(rtc.getMonth());
        Serial.print(" Year:");
        Serial.println(rtc.getYear() + 1900);

        delay(1000);
    }
}