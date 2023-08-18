
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



unsigned long rpmtime;
float rpmfloat;
unsigned int rpm;
bool tooslow = 1;
void setup() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12); //Prescaler 256
  TIMSK1 |= (1 << TOIE1); //enable timer overflow
  pinMode(2, INPUT);
  attachInterrupt(0, RPM, FALLING);

  Serial.begin(9600);

   if (display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS,true,false))
    {
    
    }

}

ISR(TIMER1_OVF_vect) {
  tooslow = 1;
}

void loop() {
  delay(1000);
  if (tooslow == 1) {
    drawData("SLOW");
  }
  else {
    rpmfloat = 120 / (rpmtime/ 31250.00);
    rpm = round(rpmfloat);
    drawData(String(rpm));
  }
}

void RPM () {
  rpmtime = TCNT1;
  TCNT1 = 0;
  tooslow = 0;
}


void drawData(String myString)
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.print("RPM:");
    display.print(myString);
    display.display();
}
