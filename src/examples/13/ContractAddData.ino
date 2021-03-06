#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#endif
#ifdef __SAMD21G18A__
#include <WiFiNINA.h>
#endif

#include <map>
#include "IoTeX-blockchain-client.h"
#include "secrets.h"
#include "addDataAbi.h"
#include <Jupiter.hpp>

constexpr const char ip[] = "gateway.iotexlab.io";
constexpr const char baseUrl[] = "iotexapi.APIService";
constexpr const int port = 10000;
constexpr const char wifiSsid[] = SECRET_WIFI_SSID;
constexpr const char wifiPass[] = SECRET_WIFI_PASS;

// Create the IoTeX client connection
Connection<Api> connection(ip, port, baseUrl);

const int led_wifi_pin = 19;
const int led_cloud_pin = 2;

void initWiFi()
{
#if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_STA);
#endif
  WiFi.begin(wifiSsid, wifiPass);
  Serial.print(F("Connecting to WiFi .."));
  // counter
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (counter++ > 2000)
    {
      Serial.println(F("Failed to connect to WiFi"));
      ESP.deepSleep(1);
    }
    digitalWrite(led_wifi_pin, !digitalRead(led_wifi_pin));
    Serial.print('.');
    delay(100);
  }
  Serial.println(F("Connected. IP: "));
  Serial.println(WiFi.localIP());
  digitalWrite(led_wifi_pin, HIGH);
}

const char *rootCACertificate = R"(-----BEGIN CERTIFICATE-----
MIIDzTCCArWgAwIBAgIQCjeHZF5ftIwiTv0b7RQMPDANBgkqhkiG9w0BAQsFADBa
MQswCQYDVQQGEwJJRTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJl
clRydXN0MSIwIAYDVQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTIw
MDEyNzEyNDgwOFoXDTI0MTIzMTIzNTk1OVowSjELMAkGA1UEBhMCVVMxGTAXBgNV
BAoTEENsb3VkZmxhcmUsIEluYy4xIDAeBgNVBAMTF0Nsb3VkZmxhcmUgSW5jIEVD
QyBDQS0zMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEua1NZpkUC0bsH4HRKlAe
nQMVLzQSfS2WuIg4m4Vfj7+7Te9hRsTJc9QkT+DuHM5ss1FxL2ruTAUJd9NyYqSb
16OCAWgwggFkMB0GA1UdDgQWBBSlzjfq67B1DpRniLRF+tkkEIeWHzAfBgNVHSME
GDAWgBTlnVkwgkdYzKz6CFQ2hns6tQRN8DAOBgNVHQ8BAf8EBAMCAYYwHQYDVR0l
BBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMBIGA1UdEwEB/wQIMAYBAf8CAQAwNAYI
KwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2VydC5j
b20wOgYDVR0fBDMwMTAvoC2gK4YpaHR0cDovL2NybDMuZGlnaWNlcnQuY29tL09t
bmlyb290MjAyNS5jcmwwbQYDVR0gBGYwZDA3BglghkgBhv1sAQEwKjAoBggrBgEF
BQcCARYcaHR0cHM6Ly93d3cuZGlnaWNlcnQuY29tL0NQUzALBglghkgBhv1sAQIw
CAYGZ4EMAQIBMAgGBmeBDAECAjAIBgZngQwBAgMwDQYJKoZIhvcNAQELBQADggEB
AAUkHd0bsCrrmNaF4zlNXmtXnYJX/OvoMaJXkGUFvhZEOFp3ArnPEELG4ZKk40Un
+ABHLGioVplTVI+tnkDB0A+21w0LOEhsUCxJkAZbZB2LzEgwLt4I4ptJIsCSDBFe
lpKU1fwg3FZs5ZKTv3ocwDfjhUkV+ivhdDkYD7fa86JXWGBPzI6UAPxGezQxPk1H
goE6y/SJXQ7vTQ1unBuCJN0yJV0ReFEQPaA1IwQvZW+cwdFD19Ae8zFnWSfda9J1
CZMRJCQUzym+5iPDuI9yP+kHyCREU3qzuWFloUwOxkgAyXVjBYdwRVKD05WdRerw
6DEdfgkfCv4+3ao8XnTSrLE=
-----END CERTIFICATE-----)";
// Not sure if WiFiClientSecure checks the validity date of the certificate.
// Setting clock just to be sure...
void setClock()
{
  configTime(7 * 3600, 0, "pool.ntp.org");

  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);
  // counter
  int i = 0;
  while (nowSecs < 8 * 3600 * 2)
  {
    digitalWrite(led_cloud_pin, !digitalRead(led_cloud_pin));
    if (i++ > 100)
    {
      ESP.deepSleep(1);
    }
    delay(100);
    Serial.print(F("."));
    yield();
    nowSecs = time(nullptr);
  }

  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
  digitalWrite(led_cloud_pin, LOW);
  digitalWrite(led_wifi_pin, LOW);
}

static Jupiter jupiter;
const int i2c_scl_pin = 5;
const int i2c_sda_pin = 4;
const uint8_t i2c_lis2d_addr = 0x31;
const uint8_t i2c_lm75_addr = 0x48;
Generic_LM75_9_to_12Bit_OneShot_Compatible lm75(&Wire, i2c_lm75_addr, &Generic_LM75_11Bit_Attributes);
LIS2DW12Sensor Accelero(&Wire, i2c_lis2d_addr);
void setup()
{
  pinMode(led_cloud_pin, OUTPUT);
  pinMode(led_wifi_pin, OUTPUT);
  Serial.begin(115200);
  Wire.begin(i2c_sda_pin, i2c_scl_pin);
  Wire.setClock(400000);
  delay(100);

   Serial.println("Starting temperature conversion...");
    // lm75.startOneShotConversion();
    lm75.disableShutdownMode();
    delay(100);
    Serial.print("temperature is: ");
    Serial.print(lm75.readTemperatureC());
    Serial.print(" C  ");
    lm75.enableShutdownMode();

  // lm75.disableShutdownMode();
  // lm75.enableOneShotMode();


  IotexHelpers.setModuleLogLevel("USER", IotexLogLevel::INFO);
  IotexHelpers.setModuleLogLevel("HTTP", IotexLogLevel::INFO);
  IotexHelpers.setModuleLogLevel("GENERAL", IotexLogLevel::INFO);
  IotexHelpers.setModuleLogLevel("CONTRACT", IotexLogLevel::INFO);

  IotexHelpers.setGlobalLogLevel(IotexLogLevel::INFO);

  // Connect to the wifi network
  initWiFi();
  setClock();
}
#include <HTTPClient.h>

#include <WiFiClientSecure.h>

void post(String data)
{
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client)
  {
    client->setCACert(rootCACertificate);
    {
      HTTPClient https;
      https.addHeader("Content-Type", "application/json");
      //   https.addHeader("Content-Type", "application/x-www-form-urlencoded");

      Serial.print("[HTTPS] begin...\n");
      if (https.begin(*client, "https://hono-boilerplate.laris.workers.dev/proxy/4690"))
      { // HTTPS
        Serial.print("[HTTPS] POST...\n");
        // start connection and send HTTP header
        // Serial.printf("post data is %s\n", data.c_str());
        int httpCode = https.POST(data);
        // int httpCode = https.GET();

        // httpCode will be negative on error
        if (httpCode > 0)
        {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
          {
            String payload = https.getString();
            Serial.print("RESP: ");
            Serial.println(payload);
          }
        }
        else
        {
          Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }

        https.end();
      }
      else
      {
        Serial.printf("[HTTPS] Unable to connect\n");
      }

      // End extra scoping block
    }

    delete client;
  }
  else
  {
    Serial.println("Unable to create client");
  }
}

void loop()
{
  // Private key of the origin address
  const char PK_CHARS[] = SECRET_PRIVATE_KEY;
  // Contract address
  const char contractAddress[] = "io1d3wc0r7mqcu2vxwt3ax20xefw567jh6my8ryz7";
  uint8_t pk[IOTEX_PRIVATE_KEY_SIZE];
  signer.str2hex(PK_CHARS, pk, IOTEX_PRIVATE_KEY_SIZE);

  // Create the account
  Account originAccount(pk);
  AccountMeta accMeta;
  int nonce = 0;

  char publicKeyBuf[IOTEX_PUBLIC_KEY_C_STRING_SIZE] = {0};
  char privateKeyBuf[IOTEX_PRIVATE_KEY_C_STRING_SIZE] = {0};
  char ethAddressBuf[ETH_ADDRESS_C_STRING_SIZE] = {0};
  char ioAddressBuf[IOTEX_ADDRESS_C_STRING_SIZE] = {0};

  char pubWtf[IOTEX_PUBLIC_KEY_SIZE] = {0};

  // Get the account details
  originAccount.getPublicKey((uint8_t *)pubWtf);
  originAccount.getPublicKeyString(publicKeyBuf);
  originAccount.getPrivateKeyString(privateKeyBuf);
  originAccount.getEthereumAddress(ethAddressBuf);
  originAccount.getIotexAddress(ioAddressBuf);
  Serial.printf("Public Key: %s\n", publicKeyBuf);
  Serial.printf("Ethereum Address: %s\n", ethAddressBuf);
  Serial.printf("IoTeX Address: %s\n", ioAddressBuf);
  Serial.printf("contract Address: %s\n", contractAddress);

  // Wire.begin(4, 5);
  // Wire.setClock(400000);

  // jupiter.begin();
  // Serial.print("Temp:");
  // Serial.println(jupiter.getTemp());

  // lm75.startOneShotConversion();
  // Serial.println("Waiting for conversion to be ready...");
  // int waited_ms = 0;
  // // while(!lm75.checkConversionReady()) {
  // //   waited_ms++;
  // //   delay(1);
  // // }

  // Serial.print("Conversion ready in ");
  // Serial.print(waited_ms);
  // Serial.print(" ms; temperature is: ");
  // Serial.print(lm75.readTemperatureC());
  // Serial.println(" C!");
  // Serial.println();

  lm75.enableShutdownMode();
  ParameterValue tokenId = MakeParamUint(1);
  ParameterValue temp = MakeParamUint((uint32_t)(lm75.readTemperatureC()*100));
  ParameterValue humid = MakeParamUint(0);
  ParameterValuesDictionary params;
  params.AddParameter("_tokenId", tokenId);
  params.AddParameter("_temp", temp);
  params.AddParameter("_humid", humid);

  // Create the contract
  String abi = addDataAbiJson;
  Contract contract(abi);

  // Generate call data
  String callData = "";
  contract.generateCallData("setTempAndHumid", params, callData);
  Serial.print("Calling contract with data: ");
  Serial.println(callData);

  uint8_t hash[IOTEX_HASH_SIZE] = {0};

  responsetypes::ActionCore_Execution core;
  core.version = 1;
  core.gasLimit = 1000000;
  core.nonce = nonce;
  strcpy(core.gasPrice, "1000000000000");
  strcpy(core.execution.amount, "0");
  strcpy(core.execution.contract, contractAddress);
  core.execution.data = callData;

  // rpc::Wallets::sendExecution(this->host_, execution, senderPubKey, signature);

  uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
  // originAccount.signMessage((const uint8_t*)"hello", 5, signature);

  originAccount.signMessage((const uint8_t *)callData.c_str(), callData.length(), signature);
  // originAccount.signExecutionAction(core, signature);
  Serial.println("===========");

  rpc::RpcCallData callDataX = rpc::Wallets::sendExecution(connection.host, core, (const uint8_t *)pubWtf, signature);
  Serial.println("=== SIG SELF SIGN ===");
  for (int i = 0; i < IOTEX_SIGNATURE_SIZE; i++)
  {
    Serial.printf("%02x", signature[i]);
  }
  Serial.println();
  uint8_t h[IOTEX_HASH_SIZE] = {0};
  // rpc::RpcCallData callDataX =
  // connection.api.wallets.sendExecution((const uint8_t *)pubWtf,signature, core, h);

  Serial.println("=== SIG iotex sendExecution ===");
  for (int i = 0; i < IOTEX_SIGNATURE_SIZE; i++)
  {
    Serial.printf("%02x", signature[i]);
  }
  Serial.println();
  Serial.println("===========================");
  // debug hash
  for (int i = 0; i < IOTEX_HASH_SIZE; i++)
  {
    Serial.printf("%02x", h[i]);
  }
  Serial.println();
  Serial.println("===========================");
  Serial.println("===========================");
  // Serial.println(callDataX.body);
  // Serial.print("Result: ");
  Serial.println(callDataX.url);
  digitalWrite(led_wifi_pin, HIGH);
  digitalWrite(led_cloud_pin, HIGH);
  post(callDataX.body);

  Serial.println("Program finished");

  // deepSleep for 30 seconds
  digitalWrite(led_wifi_pin, LOW);
  digitalWrite(led_cloud_pin, LOW);
  ESP.deepSleep(10000000);
  while (true)
  {
    delay(10000);
  }
}