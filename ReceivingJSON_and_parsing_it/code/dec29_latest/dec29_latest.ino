
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
extern "C" {
#include "libb64/cdecode.h"
}

const char* ssid = "AWSIOT";
const char* password = "BMSCE2021!";

//const char* ssid = "BRINDAVANA.";
//const char* password = "jyothinaresh";

// Find this awsEndpoint in the AWS Console: Manage - Things, choose your thing
// choose Interact, its the HTTPS Rest endpoint 
//const char* awsEndpoint = "a31zfhb7x7v56l-ats.iot.ap-northeast-1.amazonaws.com";

const char* awsEndpoint = "a2zbvuc7hd3qk3-ats.iot.ap-south-1.amazonaws.com";

// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = \
//-----BEGIN CERTIFICATE-----
R"EOF(MIIDWTCCAkGgAwIBAgIURqRG0bEcqgT+8dpO/1rSdELEGwswDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMTIxMDE2NDYw
OFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKhUOg+AYPzBFe7SKPZz
KyLV4jKWv72BxtYdNSVHMxrH+b/jORRJjnIid0MeMStJ4REKUKYpLO+MpPS+JF6r
ltH/6hGhBzyHi3FU0Wm7s5RDD8velybMHWxVn+brE9vp9cPatYf8Q0AHmtm8iVuF
d4/UAsEibJNLFMZVxS+KvNLO77sfJKyqvdq/x7shpYXQbWEwhov4cOcDRbyzJ/q0
k/6iyevSCsL0wcSSuiOrmX2zl360pPS4eBZCxTu51Th17KBAgvDFGV+HNouqrg6o
nIHR8iFfZlHJoueXBKg4iesghzeZVMO5dcrMdDRznNtJk2pqVYQoBba3tru1GJlU
uEUCAwEAAaNgMF4wHwYDVR0jBBgwFoAUzaHTWPVtqIKxp66c3JEfJi9KO+owHQYD
VR0OBBYEFBGNUn6DwBd8oExPErX/1bZo5KXNMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBgcg0RothpxIssQuanok4C+wCw
Z3KXHVXpp640w3L6KWqUt9OAnQiu5QGZ1zuimSC5SjJ9OtS1SQAov77a+QAAr3r1
RatPFNF8RM33p/Le5zjXgjBvtkEi2riQ0Wrt0w7WhkasliQ+IcvVqFeW/ehVO9Nk
m0RsPo3yos/KQibNFL6CAN3kZL8aEGGedRafyaVLYEmQmUqGx/DUyv3GXAcuW6NZ
7GLAecd1xtGMUM5fMe0Yg4t97bXuCQXKMKyG4mGw8qkh+h7lAiquWT4/2DqrbZAQ
yWird+zWCT4+52++rdTcZQS3cXQt+1y5QqrrSi3YYCgfP0URjX6WUHwzkxx/)EOF";
//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
R"EOF(MIIEowIBAAKCAQEAqFQ6D4Bg/MEV7tIo9nMrItXiMpa/vYHG1h01JUczGsf5v+M5
FEmOciJ3Qx4xK0nhEQpQpiks74yk9L4kXquW0f/qEaEHPIeLcVTRabuzlEMPy96X
JswdbFWf5usT2+n1w9q1h/xDQAea2byJW4V3j9QCwSJsk0sUxlXFL4q80s7vux8k
rKq92r/HuyGlhdBtYTCGi/hw5wNFvLMn+rST/qLJ69IKwvTBxJK6I6uZfbOXfrSk
9Lh4FkLFO7nVOHXsoECC8MUZX4c2i6quDqicgdHyIV9mUcmi55cEqDiJ6yCHN5lU
w7l1ysx0NHOc20mTampVhCgFtre2u7UYmVS4RQIDAQABAoIBABSnKOyH+t5oLnG7
9WyvmsZOac99MY9l4eiZctDdGIcbrCgAEBvOHp0gX3Oru6qtCOFXW6fXe3z0y7R/
s+SttvaTKLv6/vFNTC2ek4cekZ0KUWDZMcB/4MhPBSyHAUvFO/NI3WcO641qoXQ/
rra8/Ht4LtJVj+7OkoD3OxdnNV3D9TMI6xNg6ugHmiOO2AMTi0aqmL7bnTEPaRkB
N30HazUJ+z49UXLeivs50DVkQ/OimRrRHHClNUcLWIytW+DZf3MzUfOUAnZsuUQP
CRYaCPtpWcdSC8Ho4eIQGdjPgAeZROjRdYGn845q6IGNXC0iRaxbuK8uWsaXWPXi
g4iZJU0CgYEA0RPzUL0qIq2IK5vSobcgwQOYB+/RxnUO/AtX+rR5JY+GlQ3Oyojv
cXQ26a6xOpb/LhpQ1kLbblZnBOBoHv0m6lKimXFAfenF2Npqx9HGQWjgEqti5M3H
DuLBU8XjTuyNWTa5yYhSsHgA8GKJvZsZJ5hU6y3/cPQKkoESVc9do3MCgYEAzhsm
KEsxUsgGCNcKIjbOizd9tOi9MFRTqn7SMvce3cC72/jFBRFdidGPHxZgLInZ3C68
6RhjpY0G5zqP3xqBNMYwtPh3pfvxEUJSCq/Hi+vB+/cfRO574hWlJISwk06B+LzT
f8yH8CHY0zz8L9iLxGcJIAAqfiBN9rHpzrE892cCgYEAlrMwoDOZGWHHUteAHPwo
abczcBUWOVvKrzVl9tj6sW4gbXOEaEwHetlvc0RtjnOj/xEMz0NlcsRogQVXR27l
C+UAOmV9PMSxzLqyWNEFe8QiLpLGN3FzV3FuM/ng+9RZlcJya6cNIxF8a8g6zCXt
HbMZEmMXSc09iZfrJVrQfa0CgYA8BfAQ4AUXRKVtQaLz39468Qgs/XLLdmN/TbvN
l+ZnUk/jegl2tl31WxBBT3Tmw6as1vexMqcieXW/NXDc2o0yX12j9pBxyHOmQRg/
gH2upBQ0Wv3CR2nQp9kWT4ZVABId7G/z0g7swg6xRhtzm67ondEsfp/hZ2sNIzci
OL+2lQKBgDfZVL8pkYVS4Q2EBoSIYaCeTmI0PLsxd/kDGkD1fXmRkP4xRA3bBgKF
bbdSbhc2Ub7KB2Ej773eU1RQTkXxJHK7JoMNweeELqO9sOmaJvZJUJBHpr3cQGrq
kVqKE42pVVZo0hWy9pUq40V+aXI3Za+t0EOl3R2QVFCUCsFqnHrt)EOF";
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

//The above command uses port8883 - MQTT
//To use HTTPS, use port 443
//PubSubClient pubSubClient(awsEndpoint, 443, msgReceived, wiFiClient);

//------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200); Serial.println();
  pinMode(5,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
 
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

  digitalWrite(5,HIGH);
}
//------------------------------------------------------------------------------------------------


unsigned long lastPublish;
unsigned long lastOn=0;
unsigned int motionFlag=0;
unsigned long totalOntime=0;
int msgCount;
//StaticJsonBuffer<200> jsonBuffer;

//DynamicJsonBuffer jsonBuffer(50);

//String dummy = "\"Channel\" : 1"; 
//JsonObject& root = jsonBuffer.parseObject(dummy);

//------------------------------------------------------------------------------------------------
void loop() {

  pubSubCheckConnect();

  

}

//------------------------------------------------------------------------------------------------


void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  DynamicJsonBuffer jsonBuffer(50);
  Serial.println();
  JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
    }
    else{
    Serial.println("Success in parsing Json..!");
    }

   int c = root["Channel"];
   if(c==1)
   {
    if(motionFlag==1)
     {
            if((millis()-lastOn >=60000))
            {
             int t = root["Motion"];
         
               if(t==1)
               {
                  motionFlag = 1;      
                  totalOntime  = totalOntime + 1;                
                  digitalWrite(LED_BUILTIN,LOW);
                  digitalWrite(5,LOW);
                  lastOn = millis();  
               }
               else
               {
                  motionFlag = 0;
                  digitalWrite(LED_BUILTIN,HIGH);
                  digitalWrite(5,HIGH);
               }
    
         Serial.println("total on time :");
         Serial.println(totalOntime);
          }
      }

      else
      {
            int t = root["Motion"];
         
               if(t==1)
               {
                  motionFlag = 1;      
                  totalOntime  = totalOntime + 1;                
                  digitalWrite(LED_BUILTIN,LOW);
                  digitalWrite(5,LOW);
                  lastOn = millis();  
               }
               else
               { 
                  motionFlag = 0;
                  digitalWrite(LED_BUILTIN,HIGH);
                  digitalWrite(5,HIGH);
               }
    
         Serial.println("total on time :");
         Serial.println(totalOntime);
        
      }
    }
    else if(c==2)
    {
      int te = root["temp"];
      int hm = root["humidity"];
      if((motionFlag==1)&&((te>28)||(hm>75)))
      {
        //do something like turn on a fan
        //digitalWrite(4,LOW);
      }
      
    }

//   free(jsonBuffer);
    jsonBuffer.clear();
    
}
//------------------------------------------------------------------------------------------------


void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthing2");
    }
    Serial.println("connected");
    pubSubClient.subscribe("motionChannel");
    pubSubClient.subscribe("DHTChannel");
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
