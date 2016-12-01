
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>


static const char* ssid = "myrouter";   // Your ROUTER SSID
static const char* pw = "password";     // WiFi PASSWORD
static const int port = 9265;


static WiFiServer server(port);


void setup()
{
  delay(500);
  
  Serial.begin(19200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pw);
  
  while (WiFi.status() != WL_CONNECTED)
    delay(1000);

  server.begin();
}

static inline void pump(Stream& input, Stream& output)
{
  const int BUFFER_SIZE = 1024;
  uint8_t buf[BUFFER_SIZE]; 
  int i = 0;
 
  while ((buf[i] = input.read()) != -1 && i < BUFFER_SIZE)
    ++i;

  if (i > 0)
    output.write(buf, i);
}

void loop()
{
  WiFiClient client = server.available();
  if (client && client.connected())
  {
    pump(client, Serial);
    pump(Serial, client);
  }
}

