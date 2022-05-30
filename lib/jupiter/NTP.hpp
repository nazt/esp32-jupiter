#include <Arduino.h>
#include <time.h>

class NTP
{
    const char *ntpServer = "pool.ntp.org";
    const long gmtOffset_sec = 7 * 3600;
    const int daylightOffset_sec = 0;

public:
    struct tm timeinfo;
    bool isSynced = false;
    void printLocalTime()
    {
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Failed to obtain time");
            isSynced = false;
        }
        else
        {
            Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
            isSynced = true;
        }
    }

    void setup()
    {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        printLocalTime();
    }

    void loop()
    {
        printLocalTime();
    }
};