
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
R"EOF(MIIDWjCCAkKgAwIBAgIVAIp3OdSW8G7bqlNBKsrLjgvlN3ffMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDEyMTAxNDUw
NDNaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC8ii4XtPkmsF06JtQY
ubftYFKS4VUtMk+rHEvfZyqer30HCTPPp0gqe5HWkgTm1DCtHLT4RHj8l3dONJgB
Yhzjd77r1p94MnEPEcmeoIJytXprIv0a6etWiPV2IsylRhPMqWOvgq9knhr7iaMj
7WtAZhwUbOWUKBai5+VOF1cucR8VeBaHV11Sb/Rkp2Yf62A3+GLg+cv2Vw4vogst
6JhVgxfLjYBgxdHGcZ/F7HWNXZ78FSpnJf6fdDQb98x1VKxMsvKEdgxO1WOQdVjl
6dS2g9TM2tnt94h4fYzZ84uhctaTMsf6SkcUs3THUpUs1j3+8mm1VR0Tk5MZoOoF
AMwNAgMBAAGjYDBeMB8GA1UdIwQYMBaAFP6A2ANHylCYf5YuY+9MQTAmQQUkMB0G
A1UdDgQWBBSLgouhgH+ByQyD0kNi4cl2hJ5TfjAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAhDu8LPzbc/FzWP0bERq9hoZj
i1PBul0QCoGSxNSSoKphSNSRLBaw+ILnDUrs695Kvi9SLTpA6w2rHALexBV07Uea
6127Cd7c3EhR6QJBQSUkivl9qYB5S18sTOU6geOptdZ+ULGPGSBJfiUL179HShrC
bCnrto4ngfyAZS+sdEiDakzzJiyVvs6b1ahr2xh0sg2d5SmOGOLmE3JXc0h19ESz
ij6ObohMBNNm0swbbezwlSVzaCu0DA1YAkTD096Tic8IvzbzIrRpgauQRK7vaZpq
/1wR6yGIfp8/KowI01G4IyFnxqVhLq3Kyr4eUbbzezsCznZLN0Cv4Qde2HnkvQ==)EOF";
//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
R"EOF(MIIEpAIBAAKCAQEAvIouF7T5JrBdOibUGLm37WBSkuFVLTJPqxxL32cqnq99Bwkz
z6dIKnuR1pIE5tQwrRy0+ER4/Jd3TjSYAWIc43e+69afeDJxDxHJnqCCcrV6ayL9
GunrVoj1diLMpUYTzKljr4KvZJ4a+4mjI+1rQGYcFGzllCgWouflThdXLnEfFXgW
h1ddUm/0ZKdmH+tgN/hi4PnL9lcOL6ILLeiYVYMXy42AYMXRxnGfxex1jV2e/BUq
ZyX+n3Q0G/fMdVSsTLLyhHYMTtVjkHVY5enUtoPUzNrZ7feIeH2M2fOLoXLWkzLH
+kpHFLN0x1KVLNY9/vJptVUdE5OTGaDqBQDMDQIDAQABAoIBAQCLFXxvLKfn7I65
iU9JdifCyx3fEK9fChBPcC5gfaCpM1gOSrGX7BWGkWXCXjGTftDxw42U4B5IaAww
+wdvbctV2k5TeB5n8v4IWdzcUhgpOss2IbeJlSpwydQZ9A33ljyg7AyHuuZcWW4a
RSy+IjTOawUn5DVwGCqqvlsgQoOj07cj6qD05jIcb5UT73/TnOb8mwSrmVHt0PUB
15RvrYkSmlhoejV/s8f7iuIX5MpNl0Bqo2AO+LhAvkj36VZW8SyV1zPktwr0eZyO
K9sknCULLoine92aJ0KFPanNqeana6bUA/MqIOz8Pq1Yggi6CwUfBZIQD3G4vtZu
irDBdeZBAoGBAN+ZY0uIov5m97a3rG0o707R/teNMosLpdbkMdk4oAWyXTSv11+I
adha81JxBUFYBBvYqkx2U4EMx68+Ymmc8qyYKEbJnu+3s534iibOGXR7pZyHMWMO
e+eqqn/WiGBRcRYklLe1YiImx+Sy6/7ugPGPFS1Vz/9iV+NZqm62tyaFAoGBANfc
O2aRwXnr+spcNHKOmj3u6xTp3VDT+KdrvFPZ4u72ihD4eWHex+QzUVOuhj+aAKNg
OmxQs/O+H4gQE5xkQHjtZvuaP7bPLEWhs7dxp85iLnwaYxtnera74ihfKgl4LKFv
m9rBfa8zeq7Doa9aTbhnt3Gc1zEy3cwKN1yaiNnpAoGBALBA7PCBiFoZOHKjXhu+
BMWQ6t1mw8yvJc66mkCsThevfaEmaxM3KJgPOXZm+MmFY9e/dp5HN5OmS1FsT48I
1JIQbs895zb3C3Mek2f7Vj1nqig0uU1oAWugMD4NEfs6t3XI6CHXrL9W6oLwNu8B
ekrvirXtysN49eaUq7AR/1UxAoGARhp8IB+OGi6NUFMJpXqylRKWvkAFZE6zHoxa
qVDF5p2i0UghA6v6dx5Dd03iw50cP/LshmjfSHWif+nYtJ3KYnuXSncZ4iD+W87c
MuNygBBa0GsxbKD6tYXnOP1PBaxFslgw4aadTt0FJuTaCbzmKNFmPjuVl6DRb4aR
3D1D3KECgYAWHgh7odfcBHxHDe3EE51eEQB7oK2Jw1v0425rwZ2fx8JtVcizw9I1
V7TjPBBv3m7FxISHXck3d529Lh21OeteIb3TpDIFvu8v0RSnE9gcyuqxA0q+j8eN
c6ZkNQBala8UbMVZfEIOQXlUvymgdV2tSw89AZFRTUvpfg/eLisG1Q==)EOF";
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
  pinMode (16,INPUT);
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

//  Serial.print("Sensor Calibrating..wait for 2mins.");
//  int j=120;
//  while(j>0)
//  { 
//    Serial.print(".");
//    delay(1000);
//    j--;
//  }
//  Serial.print("\n");
}
//------------------------------------------------------------------------------------------------


unsigned long lastPublish;
int msgCount;
byte i;
byte motion_detected;

//------------------------------------------------------------------------------------------------
void loop() {

  pubSubCheckConnect();

  i = digitalRead(16);
  char fakedata[256];
//  int temp=random(20,35);
//  int humidity=random(50,83);
  if(i==1)
  {
    snprintf(fakedata,sizeof(fakedata),"{\"Channel\": 1, \"Motion\": 1}"); 
    
  }
  else
  {
    snprintf(fakedata,sizeof(fakedata),"{\"Channel\": 1, \"Motion\": 0}");
   
  }
  if (millis() - lastPublish > 2000) {
    //String msg = String("Hello from ESP8266: Penguin here ") + ++msgCount;
    pubSubClient.publish("motionChannel",fakedata); //msg.c_str());
    Serial.print("Published: "); Serial.println(fakedata);
//    if(i==1){delay(10000);}
//    else{delay(2000);}
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
    Serial.println("connected");
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
