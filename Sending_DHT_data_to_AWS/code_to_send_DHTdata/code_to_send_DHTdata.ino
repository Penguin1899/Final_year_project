
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
extern "C" {
#include "libb64/cdecode.h"
}

#include"DHT.h"
#define DHTPIN 4 
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//const char* ssid = "ONEPLUS_co_apzxnl";
//const char* password = "zxnl7921";

const char* ssid = "BRINDAVANA.";
const char* password = "jyothinaresh";

// Find this awsEndpoint in the AWS Console: Manage - Things, choose your thing
// choose Interact, its the HTTPS Rest endpoint 
const char* awsEndpoint = "a31zfhb7x7v56l-ats.iot.ap-northeast-1.amazonaws.com";

// For the two certificate strings below paste in the text of your AWS 
// device certificate and private key, comment out the BEGIN and END 
// lines, add a quote character at the start of each line and a quote 
// and backslash at the end of each line:

// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = \
//-----BEGIN CERTIFICATE-----
R"EOF(MIIDWjCCAkKgAwIBAgIVALgI7rxGmpteVTkJmClKbQo4m0jJMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDEwMjkxMDQy
MDJaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC8FUaJuRYZGc1eB7+B
Pe45vB7SFf9A7fG+3i0mGJ3hdNlDVF+5Uj7elCEVe91c7Kv1kv0vHOmgkwGPhqKM
6ZdEDza4Cm3Ni6OSw/TzGyNcnRKEDJ3/W/AXeEyI63v/g9CR0Kz05MOcWaNivIgL
9F7Kkywp/4H97STLw7SHk5JHZlTlkUlcoJnMYo3lMEdlfClPT9PYeS3o+KW5s4lA
+j0QqQMpfUiqdiI442A26fJcKzIOzqtBY1z/fXDrEcM0fkddIBK4CLXeFMgFEdm9
W4AiLLT2dRorPK9YNSh8vvAejAc8I9Ldl7s1swkJ3BOU/UCZEO13jeTB7y2jPEVn
1mhhAgMBAAGjYDBeMB8GA1UdIwQYMBaAFBIPFp6N5j5IZBEugq0Hm2SKeXToMB0G
A1UdDgQWBBRFBSY9Mo0ITfbHyk6bkfNpdg5SaTAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAPDjH0tHV67yXsbB2KNHzCs8r
uBZAeSP+xR+j1t24vwZRy12WXDBnQ0r9uDgI2PC67bziOoh9P7Zb0xS2NshVnbK1
SfihPxo6hipvGnP/FbX89/7u2taW4MfzRtS5oyanBHFvdS5ZBJfJN6VEuEFnLj9p
Q6dwUocKCxzZmsX31eIZU1wSWesKxYECxUKgu7QIp82nqC/x2VnSMtG2kbGyE9yP
R5GNXksghZoli8XYoOkvTmMqwXbECI6N5MMfhneAYvtVOSmxHpaXbiXKOGC+fMLr
nxDsDG6g3NfdF5e5VydVN+orCt91QN8BS+TPQ5sGAqD1bryuqxNRa74THNnCtw==)EOF";
//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
R"EOF(MIIEowIBAAKCAQEAvBVGibkWGRnNXge/gT3uObwe0hX/QO3xvt4tJhid4XTZQ1Rf
uVI+3pQhFXvdXOyr9ZL9LxzpoJMBj4aijOmXRA82uAptzYujksP08xsjXJ0ShAyd
/1vwF3hMiOt7/4PQkdCs9OTDnFmjYryIC/ReypMsKf+B/e0ky8O0h5OSR2ZU5ZFJ
XKCZzGKN5TBHZXwpT0/T2Hkt6PilubOJQPo9EKkDKX1IqnYiOONgNunyXCsyDs6r
QWNc/31w6xHDNH5HXSASuAi13hTIBRHZvVuAIiy09nUaKzyvWDUofL7wHowHPCPS
3Ze7NbMJCdwTlP1AmRDtd43kwe8tozxFZ9ZoYQIDAQABAoIBACa0AhXpHmD/7EDD
O2cvBUygJ393w1TrIeny8jt6qVJxpxuxyxjpkcJfnQc8u+qZUTFxk+QUAQdn+NQ+
EkkI8HWy48Gvc7TuXTSgWlGEbhxqvhqqNgEMPDW4LZenTZbg2xSGanDielY8I+D3
hs9e8/qfmmDyER/dosrDdOzJMzq5u3tamN6RsC9UFDtU1tE6dxq+IO5OyDundOkE
ZkUWTNjJq0AvfuJ+4y1csdIIPx5AsjO/NPqh708inGGtFId1I8x9r7wxDh1CRIDV
xK0s4+gFKNn883D1k5OZNrjyQ/yBdlEEAF68Sqo3EQs/1HPLoS6dP3zGWCcS+6wf
27Rq7JkCgYEA7XupH2Ou5BtX8mB90ARPxaucNZxEJqlnuVQYuPKr30vOLURIV3Z3
N/GgpUhh6TnPUYpuXtDj3hXQrQcXXbv/D/uEwOY02B8BbR8ut+DNu/hN+i/v97ns
Ohy8kHGm3POuoBujLPClKxh/dYolfrKhDMDcpAYULy9bPlXPWbcMZSMCgYEAyr+O
GyTSTFGNv3uQrKTD5wOxRTx6dOJA2AnbEIPTjL/VZ9/qW6hwjS7GhfTVFVl876gm
k9rvdN6g5CfVEpUNma4xRzrJYciit2+77GNz5DBhPNxalhYtojLp88OzVdHieX47
mdav8fqexCoVunBjBIkAHuI8TUFFybvWZDmkXqsCgYAbxnfMTQtacHxDw9dTKf3u
f5jAiGtMHkocWeKeVvuVHTebAb54REnDSEZirmmNLo652bf47POJuQF0Cz1HYEJ/
qwomyYXqtQ2kpp3/MZK+ZlcwaMTnM16WmV/QgQpkEzgcT78w9WBL/wyBQcwK2neN
l0z2/iMENcS1MLIUqx0PAwKBgQCz8SC5XIu8qmDowk0bvLwVqwRJE60HUcsFMnIX
jThTYeqQX33LTYpe81XEjBAV+69TsGRUs2n0yqX2+Jc1/83rSFt6AWW8AGrR7TvX
SEkSDBIxBszrCdIEc7WJ4HWoxDeBZpSVK6JKXAlOxp80MgzfeIv/3QBx/G1o5uoX
V8VXMwKBgFhYAoJDgHYBfV/1CAxa0GX9gIluN0uOJSfRfGkquEf392+raVsNf8t9
rPTw+aaOOBbpQqnZ2WrKC/l5YxTUyBIdBYmXQPNiTpslU5CAl0ektcxZx9cggK+u
WlEdn3lhE+X0hrUvv5bK9/D2Z1A8W3BpE5YmknVRAMfuRcqumxh2)EOF";
//-----END RSA PRIVATE KEY-----

// This is the AWS IoT CA Certificate from: 
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// This one in here is the 'RSA 2048 bit key: Amazon Root CA 1' which is valid 
// until January 16, 2038 so unless it gets revoked you can leave this as is:
const String caPemCrt = \
//-----BEGIN CERTIFICATE-----
R"EOF(MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5)EOF";
//-----END CERTIFICATE-----

//------------------------------------------------------------------------------------------------

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

//The above command uses port83 - HTTPS
//To use MQTT, use port 443
//PubSubClient pubSubClient(awsEndpoint, 443, msgReceived, wiFiClient);

//------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("ESP8266 AWS IoT Example");

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  dht.begin();
  
  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();

  uint8_t binaryCert[certificatePemCrt.length() * 3 / 4];
  int len = b64decode(certificatePemCrt, binaryCert);
  wiFiClient.setCertificate(binaryCert, len);
  
  uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
  len = b64decode(privatePemKey, binaryPrivate);
  wiFiClient.setPrivateKey(binaryPrivate, len);

  uint8_t binaryCA[caPemCrt.length() * 3 / 4];
  len = b64decode(caPemCrt, binaryCA);
  wiFiClient.setCACert(binaryCA, len);
}
//------------------------------------------------------------------------------------------------


unsigned long lastPublish;
int msgCount;
float h;
float t;

//------------------------------------------------------------------------------------------------
void loop() {

  pubSubCheckConnect();

  char fakedata[256];
//  int temp=random(20,35);
//  int humidity=random(50,83);
  
  h = dht.readHumidity();
  
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) 
  {
    h = 0;
    t = 0;
  }

  snprintf(fakedata,sizeof(fakedata),"{\"temp\" :%f,\"humidity\" :%f}",t,h); 
  
  if (millis() - lastPublish > 10000) {
    //String msg = String("Hello from ESP8266: Penguin here ") + ++msgCount;
    pubSubClient.publish("outTopic",fakedata); //msg.c_str());
    Serial.print("Published: "); Serial.println(fakedata);
    lastPublish = millis();
  }
}
//------------------------------------------------------------------------------------------------


void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
//------------------------------------------------------------------------------------------------


void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthing");
    }
    Serial.println(" connected");
  }
  pubSubClient.loop();
}
//------------------------------------------------------------------------------------------------


int b64decode(String b64Text, uint8_t* output) {
  base64_decodestate s;
  base64_init_decodestate(&s);
  int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
  return cnt;
}

void setCurrentTime() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}
