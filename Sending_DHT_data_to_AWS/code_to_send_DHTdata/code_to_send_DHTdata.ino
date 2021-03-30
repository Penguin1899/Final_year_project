
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
extern "C" {
#include "libb64/cdecode.h"
}

#include"DHT.h"
#define DHTPIN 4 
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//const char* ssid = "AWSIOT";
//const char* password = "BMSCE2021!";

const char* ssid = "BRINDAVANA.";
const char* password = "jyothinaresh";

// Find this awsEndpoint in the AWS Console: Manage - Things, choose your thing
// choose Interact, its the HTTPS Rest endpoint 
const char* awsEndpoint = "a2zbvuc7hd3qk3-ats.iot.ap-south-1.amazonaws.com";

// For the two certificate strings below paste in the text of your AWS 
// device certificate and private key, comment out the BEGIN and END 
// lines, add a quote character at the start of each line and a quote 
// and backslash at the end of each line:

// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = \
//-----BEGIN CERTIFICATE-----
R"EOF(MIIDWTCCAkGgAwIBAgIUEYFuaQXcP7UEUdtCbZ6G1HepKAgwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMTIxMDE0NDU1
OVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAK+13X0n1LnYt6e/mTo4
7VZPk2bJyqvmkz0ZAs0sC3VKDB/8lQBLM748Bp0lk5vL4nbmTkTw/xdWrIGHtSYF
a8Szj6tw0TtWRgcqpJPq71lebpBIR7XpRA7dqpMrtYXV05GZvtA+NZw/4ZbwH5Vz
1QcKxri5SqSPb+wFw5Wx5vF31SfSDWWbQ4RxEBBq19FLwVWPJUKp9p3kIVVEbf4e
lZnjtfLRah6jX4Zz9hAdbFtcdWw7op+AsNUGRp+49FRmaSetH3zPFwTY12yykQcz
m6PRNex996YgOoUj4ud5AlbZaO03ibWAROFaXVC51flmiP2Kehciz8wj1AYejuKN
ShECAwEAAaNgMF4wHwYDVR0jBBgwFoAU2yGdqgF4ghIhuBETtFG0ENEbx6EwHQYD
VR0OBBYEFPNDfPGIKkXJhljX9VS9PP7/19c7MAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAQcLfgccLQBZ+Paq6tBXDBiwkh
24QKsRyhzSyE9YfUiMLQumbWMTzeQUfaPfGpwoGJXeXVJpzv7kJto9zLMQPgNGOH
tqw2h2q895UarTVGlwpHohQWahaWgGrJG14ePpvAkS1QhnxgiDFHWk1AVfs+mbe/
waIwDHe/gcsiN9XOz1+5czeM6hps0mxiUhGulRqp6cEG/JFKmaYbn0wr/igRSnRk
FQICZm8FoAuyNUyw6uNC0dO11oo7mRWX8qrh7Do5MXcTFYsFDZxZeP+uwxcycxF6
LxZ2dFxH0SPsN2GrZRO9WkxIxeWnnq7ji35Yw4/TAGf3GxiRxfwFpfQEqge/)EOF";
//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
R"EOF(MIIEpAIBAAKCAQEAr7XdfSfUudi3p7+ZOjjtVk+TZsnKq+aTPRkCzSwLdUoMH/yV
AEszvjwGnSWTm8viduZORPD/F1asgYe1JgVrxLOPq3DRO1ZGByqkk+rvWV5ukEhH
telEDt2qkyu1hdXTkZm+0D41nD/hlvAflXPVBwrGuLlKpI9v7AXDlbHm8XfVJ9IN
ZZtDhHEQEGrX0UvBVY8lQqn2neQhVURt/h6VmeO18tFqHqNfhnP2EB1sW1x1bDui
n4Cw1QZGn7j0VGZpJ60ffM8XBNjXbLKRBzObo9E17H33piA6hSPi53kCVtlo7TeJ
tYBE4VpdULnV+WaI/Yp6FyLPzCPUBh6O4o1KEQIDAQABAoIBAQCZFQA/ztjlhWsU
KbgMYpZb8P5BuiLpuvuiakznha0B5Z5Eq+EAnYQaA9/bP2angGzwG9wq8i+8HJY0
T5BB7M0kVTAhNCwF3zcpfTxfZnpayQvYhjB7yxuxO+wlRhpi3JTzsBfvA5PfakRX
JSLPKVh6Ryg0vbHLRvBCmA77N0A2NDaQ+JvRdSavoW0Hu2wl3putekxy8EYwZQZJ
JBSWtp6tBPTAa1EYoeltphpSw6ozX/J7U75kMVjjIlwZnAm8awlvwbcw544B9ifG
/DBSDmJQ29nFKkPigC1fUG+/4XQE9z03SoGGGh68q8pK6K7okXCk0bm4wTumHfZL
YleVWIbVAoGBAOiERVdr7csGxykROkO/R40XNXiLVySfJ+WIO+qrmqtKu+qXJ0Mq
vVcSEsM72ILSMpHLjNEPMhdqt9gBP3Cy+yoghipYgD5Z5x7R9XP3BnZm4pIBJ8pw
1i5zhx2L9MwY8Nm5j1E5KvUfm2QOU1cslYzxaSB8GZOlgyNIZWJ9idSrAoGBAMF0
3ZrYQcvh3fsYmcuoHpHVc79DgSPIJ842Sd8hIKWLvnQIqNToWCPsUETDoEnDOb5q
xEBgSM03cqP97zBsAJq+zwMVQfhIpSR2FLsKqtZEbrhugjSOzdM8q1ML9otI60Me
AvS/zdTV3tkE6G5JLSTbHk/sYdeuTHNhqvApz8QzAoGAFZ7nC7//dpeEwW7WqiIJ
NzdnQ3RBgChEf3VxjNq0ISVr3956SiaZQJVvpTPVaCS/iwsQZwgwsji8K5vCGGqs
LDimAR4FnsahVR5U4pmfwjbC7qyo1Iw1flBDyh2Pf8L58mTszLKyKeTsgBtM6Jg+
t8Ji8iBYCICldwxuUQA3Zs0CgYBYcmDa6aJzX4nIwj+Wb//B9xAbUvaY8GraidGO
VSwC4/goPslJxgx+KA2u0q3W643b9B5TvHGseXxNY8zWm5pPJYupwi9OH5B5Mg9a
UI84tULVyZh4BQ4vVJ6TsnV+ytDc97ysP4EQnNNs7ySBP62HGzMpoU4+eK71VaL8
eHFIJwKBgQCvha0Fay+ZMXIqXOQDytIbvNqwfXhT4UBClK6LoRDu9LzJ5CF0X6Xo
OixwNCT99dXoqWhGy4kmBm0szRpWe1Wg3M6JEwgpzlUWf+fm0cMASvzxRo8h6rPE
gqLL3jOvTNKttXkgxJUo9hg0/FSvHHFEkkVgyfD/ng5yCfLy4J0u4A==)EOF";
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

//The above command uses port83 - MQTT
//To use HTTPS, use port 443
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
  
  h = dht.readHumidity();  
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) 
  {
    h = 0;
    t = 0;
  }

  snprintf(fakedata,sizeof(fakedata),"{\"Channel\" : 2, \"temp\" :%f,\"humidity\" :%f}",t,h); 
  
  if (millis() - lastPublish > 360000) {
    pubSubClient.publish("DHTChannel",fakedata); //msg.c_str());
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
      pubSubClient.connect("ESPthing3");
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
