/* ESP8266 AWS IoT
 *  
 * Simplest possible example (that I could come up with) of using an ESP8266 with AWS IoT.
 * No messing with openssl or spiffs just regular pubsub and certificates in string constants
 * 
 * This is working as at 3rd Aug 2019 with the current ESP8266 Arduino core release:
 * SDK:2.2.1(cfd48f3)/Core:2.5.2-56-g403001e3=20502056/lwIP:STABLE-2_1_2_RELEASE/glue:1.1-7-g82abda3/BearSSL:6b9587f
 * 
 * Author: Anthony Elder 
 * License: Apache License v2
 */

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
extern "C" {
#include "libb64/cdecode.h"
}

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
R"EOF(MIIDWTCCAkGgAwIBAgIUTQIYj8jhBeGdJVh4HIZrWFm3/1kwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMTAyOTEyNDYx
MFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMIPZSODkw+Tv5R23vwZ
xk472Tnn6jcHPYafNznB3IYMOCqtzpkCxeot5bgwPGsToZxYtXX93NzCiFNvpuAS
BU923qn/H1Sl6iuGUiTnOCPW0gil/NZxqNoyr4YeS/IWbt9mnFmzRLmqYJG4sHQ6
mfmYsaCTm69CHz9GYPZBMQSBl5g06XykCtXKNwVTutSMFLfiPJ/sodKiE8zquu3K
z9TgiCGGViOK2wp50NT8HSSDkIaVNgsNfS84fFQuOHEdcEwkKKbtNj1C/FMgUdp+
wrQVrB8Opw+rqUlcL5FMZa42xb4sXmwWYPJNH80hklszajbER2B5bXQQA7rZZO/T
bhcCAwEAAaNgMF4wHwYDVR0jBBgwFoAUbVFGPOp6BrY+wQwK4ZcYhDXBXd0wHQYD
VR0OBBYEFAnuGgKm9GHqGm54mVYT0/2zK27sMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQB3DbSySvFBeOSpCrdbvkBid6/h
LAhYfpstyQhvaxR7dR/01FYjtAjYV8WmdhWwiKHQ+wZefUboyMCGK6J/scMDnymB
rdwlREsMt90LeiWCbZiBrXkyu1hLoJH9SNItjm4QcL6TLPUiV2+Q3Z/e2wMcTnDN
c5SKnzjWgzUqYeQTDvak77jDhQ1z2Z1krD7kJ3ZZcgWAUlo919EBGlwBlXpq4KfN
f15t4n6LVZbHMmw9RpRo/eeE+WlPBZYbhHEIOCMkc25XvFXhB/NenoPe50SDb6Em
5tkzESQFjGgmJ/B7TIH2kaL3Nn5YYPyg4CnbQMMZDvRs5B34qZekImCbNTOx)EOF";
//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
R"EOF(MIIEogIBAAKCAQEAwg9lI4OTD5O/lHbe/BnGTjvZOefqNwc9hp83OcHchgw4Kq3O
mQLF6i3luDA8axOhnFi1df3c3MKIU2+m4BIFT3beqf8fVKXqK4ZSJOc4I9bSCKX8
1nGo2jKvhh5L8hZu32acWbNEuapgkbiwdDqZ+ZixoJObr0IfP0Zg9kExBIGXmDTp
fKQK1co3BVO61IwUt+I8n+yh0qITzOq67crP1OCIIYZWI4rbCnnQ1PwdJIOQhpU2
Cw19Lzh8VC44cR1wTCQopu02PUL8UyBR2n7CtBWsHw6nD6upSVwvkUxlrjbFvixe
bBZg8k0fzSGSWzNqNsRHYHltdBADutlk79NuFwIDAQABAoIBAGmoPCbD4nPeu7Y3
qlyUI/ZAe5sde9YOKR983HKj7E9Ho+UI0K1aZyuDZcmM6bha7ejUG6hsWa4QdG0J
I1BoP1/ulnj0ILjTxNWBwoO5KEXLFnwC89BF/esfIZXiNPNP53nWwlXMVyTHcOCm
L9x4c5ED4+JLu74GQEERLoWlJO0QNyIUSslJeYe8vMNTqoE0miWnafLe7sDp+wjb
wdDMtqWXoPrcilz++YEj+iTGb2PwAYKLFMIRLZw6WPDevm+LZdd490mLUW4xq5mg
plC4Y+Tq86YPHIdC9nOs0IY0OYSJELnO5Me7Aq5Rlv3fDinl5xF6NeeL3LzmWwRI
Q7ZXKWECgYEA4VIonELmVmPPM43xCvW/oLJMZZsN7MC4Nvd23pdqIgAO/hYe2J9T
u0k0kY4NVPGDrgv6Am1tbkSR43MoP/ioU7xm3MpbW+atKCoIsn7CgaIiypXS184/
ijqNts08/CVrFdERA+pKhPuo1IFCBoXSAWbjP3AqC/lTjc8k25SawykCgYEA3Hua
P/StdLoju3ojLf3T2Pq2QdD9+fXifNVJ//nsZiCX7Iv1y2uckVOE2NHtVONhpe/K
QcUZhk3nJRL1wuAD42ORSFeqS+nw+ZLFSdt6rMr1VKLiVuwiviPrTCIcwj+j8iPz
i5ciftCSb0I+pqUYgnyd+5JXII8pSKjUbzGeDz8CgYBlX/+hfn//slxRB7XBhpu3
A0FYHZEASh0XrfMvZtR7g6c7LlymfdjAVMlwC4d4BkmGjyDm4rDvga5ZtUAQuCGT
uxpUkD7WdEzH21GabWm5xWb92rbIHaV83adQ7bpYf8VURYHwTwNEHa3yHk3EfYTk
2JO5wQ9J39112hLtMEkJMQKBgDKDoxB6F2+B4gPm9L5pxCqXLkCgCEBHhG+DUmPy
N6YiW4xXeKX1brahAvPGDPEA83NYH+e3p6RsnsOQt3vBZUKoojWH2D1ySMJ1TPwh
69ZA3bpZYy/1A/S6OAOGYT5j5ETq1baqlr2lPZd9/uqhMp9au7tSiOGtyxCZlaow
HP1zAoGACBDmSZ1u6aQHPySl2ZKamo4cm3hzSq/NRAaVY4d9zEni5+fyJy8gt6dA
hiIp55qhMPQF6eZQwg1O+BHtVlDUSaG0N0G+pUpn4FZ9CG4+2Vvk0aMLaP5Jq6iJ
TNX6g0f6y7WqCK41CKFeYIOYoQMfcPr8hcjIv+ZvsLyNnVJkjJc=)EOF";
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

//object declarations and declaration of msgReceived func
WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 
//The above command uses port 8883 - MQTT
//To use HTTPS, use port 443
//PubSubClient pubSubClient(awsEndpoint, 443, msgReceived, wiFiClient);



void setup() {

  pinMode(LED_BUILTIN,OUTPUT);
   
  Serial.begin(115200); Serial.println();
  Serial.println("ESP8266 AWS IoT Example");

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

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

//unsigned long lastPublish;
int msgCount;
int LED_STATUS = 0;
DynamicJsonBuffer jsonBuffer(256);

void loop() {
  pubSubCheckConnect();
}



void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
    }
    else{
    Serial.println("Success in parsing Json..!");
    }

   int t = root["temp"];
   int h = root["humidity"];
   Serial.println(t);
   Serial.println(h);

  if(LED_STATUS==0){  digitalWrite(LED_BUILTIN,HIGH);  LED_STATUS=1;}
  else{  digitalWrite(LED_BUILTIN,LOW);  LED_STATUS=0;}
}


void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) 
  {
    Serial.print("PubSubClient connecting to end-point: "); Serial.print(awsEndpoint);
    
    while ( ! pubSubClient.connected()) 
    {
      Serial.print(".");
      pubSubClient.connect("ESPthing12");
    }
    
    Serial.println(" connected");
    pubSubClient.subscribe("outTopic");
  }
  
  pubSubClient.loop();
}


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
