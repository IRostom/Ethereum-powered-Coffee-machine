#include <Arduino.h>
#include "Elkrem.h"
#include <qrcode.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
//#include <CoinMarketCapApi.h>
//#include <Adafruit_SSD1325.h>

#define Cup1_Input 13
#define Cup1_Output 26
#define Cup2_Input 12
#define Cup2_Output 27

// If using software SPI, define CLK and MOSI
#define OLED_CLK 18
#define OLED_MOSI 23

// These are neede for both hardware & softare SPI
#define OLED_CS 15
#define OLED_RESET 4
#define OLED_DC 21

#define MACHINE_ACCOUNT "0xade6696F50ceC29063bF5D03a27c6F9DD1eDaC33"

// this is software SPI, slower but any pins
Adafruit_SSD1325 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
QRcode qrcode(&display);

//unsigned long api_mtbs = 60000; //mean time between api requests
//unsigned long api_due_time = 0;

//CoinMarketCapApi api(client);

bool Cup1_flag = false;
bool Cup2_flag = false;

float Cup1_price = 0.2;
float Cup2_price = 0.5;

/* void checkTickerdata()
{
  CMCTickerResponse response = api.GetTickerInfo("ethereum");
  Serial.print("Price in USD: ");
  Serial.println(response.price_usd);
  Cup1_price = response.price_usd / 2.62;
  Cup2_price = response.price_usd / 5;
} */

void paymentCallback(EthereumAccountTransaction *transaction)
{
  float value = (float)transaction->getValue() / 1000000000ul;
  Serial9.println("Recieved ");
  Serial9.print(value);
  Serial9.print("Ether");
  if (value >= Cup2_price)
  {
    if (Cup2_flag)
    {
      digitalWrite(Cup2_Output, LOW);
      delay(150);
      digitalWrite(Cup2_Output, HIGH);
      Cup2_flag = false;
    }
  }
  else if (value >= Cup1_price)
  {
    if (Cup1_flag)
    {
      digitalWrite(Cup1_Output, LOW);
      delay(150);
      digitalWrite(Cup1_Output, HIGH);
      Cup1_flag = false;
    }
  }
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(Cup1_Input, INPUT_PULLUP);
  pinMode(Cup2_Input, INPUT_PULLUP);
  pinMode(Cup1_Output, OUTPUT);
  pinMode(Cup2_Output, OUTPUT);

  digitalWrite(Cup1_Output, HIGH);
  digitalWrite(Cup2_Output, HIGH);

  Elkrem.init();
  Ethereum.trackAccount(MACHINE_ACCOUNT);
  Ethereum.onAccountTransaction(&paymentCallback);

  //checkTickerdata();

  display.begin();
  display.clearDisplay();
  display.display();

  //Display Elkrem Logo

  //Display Instructions
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(BLACK, WHITE);
  display.setTextSize(1);
  display.println("Follow the Instructions on the left!");
  display.display();

  // Initialize QRcode display using library
  qrcode.init();
}

void loop()
{
  // put your main code here, to run repeatedly:
  Elkrem.run();
  if (!digitalRead(Cup1_Input))
  {
    Serial9.println("1 Cup");
    Cup1_flag = true;
    Cup2_flag = false;
    // create qrcode
    qrcode.create("0xade6696F50ceC29063bF5D03a27c6F9DD1eDaC33");
    display.setCursor(3, 10);
    display.setTextColor(BLACK, WHITE);
    display.setTextSize(1);
    display.println("Pay 0.2 Eth");
    display.setCursor(3, 26);
    display.println("to this");
    display.setCursor(3, 42);
    display.println("Address -->");
    display.display();
  }
  if (!digitalRead(Cup2_Input))
  {
    Serial9.println("2 Cups");
    Cup2_flag = true;
    Cup1_flag = false;
    // create qrcode
    qrcode.create("0xade6696F50ceC29063bF5D03a27c6F9DD1eDaC33");
    display.setCursor(3, 10);
    display.setTextColor(BLACK, WHITE);
    display.setTextSize(1);
    display.println("Pay 0.7 Eth");
    display.setCursor(3, 26);
    display.println("to this");
    display.setCursor(3, 42);
    display.println("Address -->");
    display.display();
  }

  /* unsigned long timeNow = millis();
  if ((timeNow > api_due_time))
  {
    //checkTickerdata();
    api_due_time = timeNow + api_mtbs;
  } */
}