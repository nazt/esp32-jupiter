#include <Wire.h>
#include <NTP.hpp>
#include <SPI.h>
#include "Rtc_Pcf8563.h"
#include <WiFi.h>

const char *ssid = "Lily_2.4G";
const char *password = "zxc12345";

const int i2c_scl_pin = 5;
const int i2c_sda_pin = 4;
const int rtc_int_pin = 18;

const uint8_t i2c_pcf8563_addr = 0x51;
// Rtc_Pcf8563 rtc(i2c_pcf8563_addr);

#include "RTClib.h"

RTC_PCF8563 rtc;

char daysOfTheWeek[7][12] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

void setup()
{
    Serial.begin(115200);
    Serial.println("Start..");

    Wire.begin(i2c_sda_pin, i2c_scl_pin);
    Wire.setClock(400000);

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1)
            delay(10);
    }

    if (rtc.lostPower())
    {
        Serial.println("RTC is NOT initialized, let's set the time!");
        rtc.start();
    }

    Serial.println("WiFi connected.");

    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
}

void loop()
{

    // Serial.println(&ntpModule.timeinfo, "%A, %B %d %Y %H:%M:%S");
    DateTime now = rtc.now();

    Serial.print("dayOfWeek:");
    Serial.println(now.dayOfTheWeek());
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(" ");

    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.println(now.year(), DEC);

    delay(1000);
}