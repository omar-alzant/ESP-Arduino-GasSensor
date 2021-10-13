/////////////////////////////////
// Generated with a lot of love//
// with TUNIOT FOR ESP8266     //
// Website: Easycoding.tn      //
/////////////////////////////////

        /////////////// About espNodeMCU////////
////////////////////////////////////////////////////////////
https://cdn-shop.adafruit.com/product-files/2471/0A-ESP8266__Datasheet__EN_v4.3.pdf
////////////////////////////////////////////////////////////
/// https://www.espressif.com/en/products/socs/esp8266
////////////////////////////////////////////////////////////






#include <ESP8266WiFi.h>
//Include the library
#include <MQUnifiedsensor.h>

//Definitions
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A0 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  
#define calibration_button 13 //Pin to calibrate your sensor

//Declare Sensor
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

String  mystring;
String  mystring1;
String  mystring2;

WiFiClient client;

String MakerIFTTT_Key ;
;String MakerIFTTT_Event;
char *append_str(char *here, String s) {  int i=0; while (*here++ = s[i]){i++;};return here-1;}

char *append_ul(char *here, unsigned long u) { char buf[20]; return append_str(here, ultoa(u, buf, 10));}
char post_rqst[256];char *p;char *content_length_here;char *json_start;int compi;

void setup()
{
  
mystring = "";
mystring1 = "";
mystring2 = "";


Serial.begin(115200);
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
    MQ135.init(); 
 float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
   
  }
  MQ135.setR0(calcR0/10);

  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
   WiFi.begin("master","Master.123@oo");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.println("");

  }
  Serial.println("Connected");
  Serial.println("your IP is:");
  Serial.println((WiFi.localIP().toString()));

}


void loop()
{ float calcR0 = 0;
  calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
  MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin

  MQ135.setA(605.18); MQ135.setB(-3.937); // Configurate the ecuation values to get CO concentration
  float CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(110.47); MQ135.setB(-2.862); // Configurate the ecuation values to get CO2 concentration
  float CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    mystring1 = CO ;
    mystring2 = CO2 ;
    if (client.connect("maker.ifttt.com",80)) {
       if(isinf(calcR0)) mystring = "open circuit";

      MakerIFTTT_Key ="zbf1SqZU4Ly_JkfvMiQy5";
      MakerIFTTT_Event ="sheets";
      p = post_rqst;
      p = append_str(p, "POST /trigger/");
      p = append_str(p, MakerIFTTT_Event);
      p = append_str(p, "/with/key/");
      p = append_str(p, MakerIFTTT_Key);
      p = append_str(p, " HTTP/1.1\r\n");
      p = append_str(p, "Host: maker.ifttt.com\r\n");
      p = append_str(p, "Content-Type: application/json\r\n");
      p = append_str(p, "Content-Length: ");
      content_length_here = p;
      p = append_str(p, "NN\r\n");
      p = append_str(p, "\r\n");
      json_start = p;
      p = append_str(p, "{\"value1\":\"");
  p = append_str(p,mystring);
  //   p = CO;
      p = append_str(p, "\",\"value2\":\"");
  p = append_str(p, mystring1 );   //  stringTwo.concat(millis()); to add variable to string 

    //  p = CO2;
      p = append_str(p, "\",\"value3\":\"");
      p = append_str(p, mystring2);
      p = append_str(p, "\"}");

      compi= strlen(json_start);
      content_length_here[0] = '0' + (compi/10);
      content_length_here[1] = '0' + (compi%10);
      client.print(post_rqst);


      Serial.println("values sent to google sheet");
      
      
    }
    delay(5000);

}