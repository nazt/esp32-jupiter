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

constexpr const char ip[] = "gateway.iotexlab.io";
constexpr const char baseUrl[] = "iotexapi.APIService";
constexpr const int port = 10000;
constexpr const char wifiSsid[] = SECRET_WIFI_SSID;
constexpr const char wifiPass[] = SECRET_WIFI_PASS;

// Create the IoTeX client connection
Connection<Api> connection(ip, port, baseUrl);
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>

#include <WiFiClientSecure.h>

const char *rootCACertificate = R("");

void initWiFi()
{
#if defined(ESP8266) || defined(ESP32)
    WiFi.mode(WIFI_STA);
#endif
    WiFi.begin(wifiSsid, wifiPass);
    Serial.print(F("Connecting to WiFi .."));
    uint32_t counter = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
        if (counter++ >= 100) {
            ESP.deepSleep(1);
        }
    }
    Serial.println(F("Connected. IP: "));
    Serial.println(WiFi.localIP());
}

void setup()
{
    Serial.begin(115200);
    IotexHelpers.setModuleLogLevel("USER", IotexLogLevel::TRACE);
    IotexHelpers.setModuleLogLevel("HTTP", IotexLogLevel::TRACE);
    IotexHelpers.setModuleLogLevel("GENERAL", IotexLogLevel::TRACE);
    IotexHelpers.setModuleLogLevel("CONTRACT", IotexLogLevel::TRACE);

    IotexHelpers.setGlobalLogLevel(IotexLogLevel::TRACE);

#if defined(__SAMD21G18A__)
    delay(5000); // Delay for 5000 seconds to allow a serial connection to be established
#endif

    // Connect to the wifi network
    initWiFi();
}

void loop()
{
    // Private key of the origin address
    const char pK[] = SECRET_PRIVATE_KEY;
    // Contract address
    const char contractAddress[] = "io1mql0rea4sxlvxaf4gq8gest487nppksfz9psc8";
    // Imei
    String name = "aaa";
    // Data
    // uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    // Signature
    // uint8_t signature[] = {0x05, 0x06, 0x07, 0x08};

    // Convert the privte key to a byte array
    uint8_t pk[IOTEX_PRIVATE_KEY_SIZE];
    signer.str2hex(pK, pk, IOTEX_PRIVATE_KEY_SIZE);

    // Create the account
    Account originAccount(pk);
    AccountMeta accMeta;
    char originIotexAddr[IOTEX_ADDRESS_C_STRING_SIZE] = "";

    // Get the account nonce
    originAccount.getIotexAddress(originIotexAddr);

    // Get the account nonce
    // originAccount.getEthereumAddress(originEthAddr);

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

    ResultCode result = connection.api.wallets.getAccount(originIotexAddr, accMeta);
    if (result != ResultCode::SUCCESS)
    {
        Serial.print("Error getting account meta: ");
        Serial.print(IotexHelpers.GetResultString(result));
    }
    int nonce = atoi(accMeta.pendingNonce.c_str());

    // Contruct the action
    // Create the parameters
    ParameterValue paramName = MakeParamString(name);
    // ParameterValue paramData = MakeParamBytes(data, sizeof(data), true);
    // ParameterValue paramSig = MakeParamBytes(signature, sizeof(signature), true);

    // Create parameter values dictionary
    ParameterValuesDictionary params;
    params.AddParameter("name", paramName);
    // params.AddParameter("data", paramData);
    // params.AddParameter("signature", paramSig);

    // Create the contract
    String abi = addDataAbiJson;
    Contract contract(abi);

    // Generate call data
    String callData = "";
    contract.generateCallData("setName", params, callData);
    Serial.print("Calling contract with data: 0x%");
    Serial.println(callData);

    uint8_t hash[IOTEX_HASH_SIZE] = {0};
    // 0xc47f0027000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000045961792100000000000000000000000000000000000000000000000000000000
    // 0xc47f00270000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000548656c6c6f000000000000000000000000000000000000000000000000000000
    result = originAccount.sendExecutionAction(connection, nonce, 1000000, "1000000000000", "0", contractAddress, callData, hash);

    responsetypes::ActionCore_Execution core;
    // core.version = 1;
    core.gasLimit = 1000000;
    core.nonce = 170;
    strcpy(core.gasPrice, "1000000000000");
    strcpy(core.execution.amount, "0");
    strcpy(core.execution.contract, contractAddress);
    core.execution.data = callData;

    Serial.print("Signed data: ");

    // rpc::Wallets::sendExecution(this->host_, execution, senderPubKey, signature);

    uint8_t signature[IOTEX_SIGNATURE_SIZE] = {0};
    // print signature hex
    Serial.println("===========");
    for (int i = 0; i < IOTEX_SIGNATURE_SIZE; i++)
    {
        Serial.printf("%02x", signature[i]);
    }
    Serial.println("===========");
    // originAccount.signMessage((const uint8_t*)"hello", 5, signature);
    Serial.println('callDarta');
    Serial.println(callData);

    originAccount.signMessage((const uint8_t *)callData.c_str(), callData.length(), signature);
    // originAccount.signExecutionAction(core, signature);
    Serial.println("===========");
    for (int i = 0; i < IOTEX_SIGNATURE_SIZE; i++)
    {
        Serial.printf("%02x", signature[i]);
    }
    Serial.println("===========");
    rpc::RpcCallData callDataX = rpc::Wallets::sendExecution(connection.host, core, (const uint8_t *)pubWtf, signature);
    Serial.println("KKKKKKKK");
    Serial.println("KKKKKKKK");
    Serial.println("KKKKKKKK");
    for (int i = 0; i < IOTEX_SIGNATURE_SIZE; i++)
    {
        Serial.printf("%02x", signature[i]);
    }
    Serial.println("KKKKKKKK");
    Serial.println("KKKKKKKK");
    Serial.println("KKKKKKKK");
    Serial.println("HELLLLLLLLLLLLLLL:");
    Serial.println(callDataX.body);
    Serial.print("Result: ");
    Serial.println(IotexHelpers.GetResultString(result));
    if (result == ResultCode::SUCCESS)
    {
        Serial.print("Hash: ");
        for (int i = 0; i < IOTEX_HASH_SIZE; i++)
        {
            char buf[3] = "";
            sprintf(buf, "%02x", hash[i]);
            Serial.print(buf);
        }
        Serial.println();
    }

    Serial.println("Program finished");
    while (true)
    {
        delay(10000);
    }
}