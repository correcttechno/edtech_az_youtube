



#define LED_PIN 27
#define PHOTO_SENSOR 35
#define DHT11PIN 19
#define BUZZER_PIN 32
#define MOIS_SENSOR 34

#define BLYNK_TEMPLATE_ID           "TMPL6Gm8oT2hf"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "Sb-xj6-YXE_Fw5TLRBh-gdwwkvh_xIv5"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"



DHT dht(DHT11PIN, DHT11);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WiFi SSID";
char pass[] = "WiFi password";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  Serial.println("Oxunan Data");
  Serial.println(value);
  // Update state
  Blynk.virtualWrite(V1, value);
  digitalWrite(LED_PIN,value);

  
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{


  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  pinMode(LED_PIN,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(PHOTO_SENSOR,INPUT);
  pinMode(MOIS_SENSOR,INPUT);

  dht.begin();
  analogReadResolution(10);
  
}


const int warningFrequency = 1000;  // Uyarı frekansı (örneğin, 1000 Hz)
const int warningDuration = 1000; 

void playWarning() {
  unsigned long startTime = millis();  // Başlangıç zamanı kaydedin

  while (millis() - startTime < warningDuration) {
    digitalWrite(BUZZER_PIN, HIGH);    // Buzzerı aç
    delayMicroseconds(warningFrequency / 2);  // Frekans yarısını bekle
    digitalWrite(BUZZER_PIN, LOW);     // Buzzerı kapat
    delayMicroseconds(warningFrequency / 2);  // Frekans yarısını bekle
  }
}


void loop()
{
  
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  float photo_val=map(analogRead(PHOTO_SENSOR),0,1023,0,255);
  float mois=map(analogRead(MOIS_SENSOR),0,1023,0,255);
  
  Blynk.virtualWrite(V6,humi );
  Blynk.virtualWrite(V5,temp );
  Blynk.virtualWrite(V7,photo_val );
  Blynk.virtualWrite(V1,mois );

  Serial.println(mois);

  if(photo_val<10){
    digitalWrite(LED_PIN,1);
  }
  else{
    digitalWrite(LED_PIN,0);
  }


  if(mois>150){
    playWarning();
  }


  
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
