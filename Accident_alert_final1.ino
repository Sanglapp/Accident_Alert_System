
#include <TinyGPS++.h>
#include<SoftwareSerial.h>
SoftwareSerial Serial1(6,7); //make TX arduino line is pin 6,make RX arduino line is pin 7, .GSM
TinyGPSPlus gps; // create a TinyGps++ object
SoftwareSerial GPS(8,9);//GPS Rx=8,Tx=9
#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11,5, 4, 3, 2);//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

static const uint32_t GPSBaud = 9600;

#define x A1  //accelorometer x
#define y A2
#define z A3

int xsample=0;
int ysample=0;
int zsample=0;

#define samples 10

#define minVal -50
#define MaxVal 50

/*int i=0,k=0;
int  gps_status=0;
float latitude=0; 
float logitude=0;                       
String Speed="";
String gpsString="";
char *test="$GPRMC";*/

void initModule(String cmd, char *res, int t)
{
  while(1)
  {
    Serial.println(cmd);
    Serial1.println(cmd);
    delay(100);
    while(Serial1.available()>0)
    {
       if(Serial1.find(res))
       {
        Serial.println(res);
        delay(t);
        return;
       }

       else
       {
        Serial.println("Error");
       }
    }
    delay(t);
  }
}

//int GPSBaud=9600;


void setup() 
{
 
  Serial.begin(9600);
  GPS.begin(GPSBaud);
  Serial1.begin(9600);
  lcd.begin(16,2);  
  lcd.print("Accident Alert  ");
  lcd.setCursor(0,1);
  lcd.print("     System     ");
  delay(2000);
  lcd.clear();
  lcd.print("Initializing");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  delay(1000);
  
  Serial.println("Initializing....");
  initModule("AT","OK",1000);
  initModule("ATE1","OK",1000);
  initModule("AT+CPIN?","READY",1000);  
  initModule("AT+CMGF=1","OK",1000);     
  initModule("AT+CNMI=2,2,0,0,0","OK",1000);  
  Serial.println("Initialized Successfully");
  lcd.clear();
  lcd.print("Initialized");
  lcd.setCursor(0,1);
  lcd.print("Successfully");
  delay(2000);
  lcd.clear();
  lcd.print("Callibrating ");
  lcd.setCursor(0,1);
  lcd.print("Accelerometer");
  for(int i=0;i<samples;i++)
  {
    xsample+=analogRead(x);
    ysample+=analogRead(y);
    zsample+=analogRead(z);
  }

  xsample/=samples;
  ysample/=samples;
  zsample/=samples;

  Serial.println(xsample);
  Serial.println(ysample);
  Serial.println(zsample);
  delay(1000);
  
  lcd.clear();
  lcd.print("Waiting For GPS");
  lcd.setCursor(0,1);
  lcd.print("     Signal    ");
  delay(2000);
  GPS.begin(9600);
  
  //show_coordinate();
  //delay(2000);
  lcd.clear();
  lcd.print("GPS is Ready");
  delay(1000);
  lcd.clear();
  lcd.print("System Ready");
  Serial.println("System Ready..");
  
}



void loop() 
{
    get_gps();
    int value1=analogRead(x);
    int value2=analogRead(y);
    int value3=analogRead(z);

    int xValue=xsample-value1;
    int yValue=ysample-value2;
    int zValue=zsample-value3;
    
    Serial.print("x=");
    Serial.println(xValue);
    Serial.print("y=");
    Serial.println(yValue);
    Serial.print("z=");
    Serial.println(zValue);

    if(xValue < minVal || xValue > MaxVal  || yValue < minVal || yValue > MaxVal  || zValue < minVal || zValue > MaxVal)
    {
      get_gps();
      //show_coordinate();
      lcd.clear();
      lcd.print("Sending SMS ");
      Serial.println("Sending SMS");
      Send();
      Serial.println("SMS Sent");
      lcd.print("SMS Sent ");
      delay(2000);
      lcd.clear();
      lcd.print("System Ready");
    }       
}

/*void gpsEvent()
{
  gpsString="";
  while(1)
  {
   while (gps.available()>0)            //Serial incoming data from GPS
   {
    char inChar = (char)gps.read();
     gpsString+= inChar;                    //store incoming data from GPS to temparary string str[]
     i++;
    // Serial.print(inChar);
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //check for right string
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>60)
     {
       gps_status=1;
       break;
     }
     else
     {
       i=0;
     }
    }
  }
   if(gps_status)
    break;
  }
}
*/
void get_gps()
{
  lcd.clear();
  lcd.print("Getting GPS Data");
  lcd.setCursor(0,1);
  lcd.print("Please Wait.....");
   while (GPS.available() > 0){
    gps.encode(GPS.read());
    if (gps.location.isUpdated()){
      Serial.println("Latitude= "); 
      Serial.println(gps.location.lat(), 6);
      Serial.println(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
      lcd.clear();
      lcd.print("LAT:");
      lcd.print(gps.location.lat(),6);
      lcd.setCursor(0,1);
      lcd.print("LONG:");
      lcd.print(gps.location.lng(),6);
      
    }
  }
}



/*void show_coordinate()
{
    lcd.clear();
    lcd.print("Lat:");
    lcd.print(latitude);
    lcd.setCursor(0,1);
    lcd.print("Log:");
    lcd.print(logitude);
    Serial.print("Latitude:");
    Serial.println(latitude);
    Serial.print("Longitude:");
    Serial.println(logitude);
    Serial.print("Speed(in knots)=");
    Serial.println(Speed);
    delay(2000);
    lcd.clear();
    lcd.print("Speed(Knots):");
    lcd.setCursor(0,1);
    lcd.print(Speed);
}

void coordinate2dec()
{
  String lat_degree="";
    for(i=20;i<=21;i++)         
      lat_degree+=gpsString[i];
      
  String lat_minut="";
     for(i=22;i<=28;i++)         
      lat_minut+=gpsString[i];

  String log_degree="";
    for(i=32;i<=34;i++)
      log_degree+=gpsString[i];

  String log_minut="";
    for(i=35;i<=41;i++)
      log_minut+=gpsString[i];
    
    Speed="";
    for(i=45;i<48;i++)          //extract longitude from string
      Speed+=gpsString[i];
      
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     
     minut= log_minut.toFloat();
     minut=minut/60;
     degree=log_degree.toFloat();
     logitude=degree+minut;
}*/

void Send()
{ 
   Serial1.println("AT");
   delay(500);
   serialPrint();
   Serial1.println("AT+CMGF=1");
   delay(500);
   serialPrint();
   /*Serial1.print("AT+CMGS=");
   Serial1.print('"');
   Serial1.print("+8801760711055");    //mobile no. for SMS alert
   
   Serial1.println('"');*/
   Serial1.println("AT+CMGS=\"+01760711055\"\r");
   delay(500);
   serialPrint();
   Serial.print("Latitude:");
   Serial.println(gps.location.lat(), 6);
   Serial1.print("Accident has been occured.please send ambulance at:");
   Serial1.print("Latitude:");
   Serial1.println(gps.location.lat(), 6);
   
   //Serial1.println(gps.location.lat(), 6);
   
   serialPrint();
   Serial.print(" longitude:");
   Serial.println(gps.location.lng(), 6);
   Serial1.print(" longitude:");
   Serial1.println(gps.location.lng(), 6);
   lcd.clear();
   lcd.print("LAT:");
   lcd.print(gps.location.lat(),6);
   lcd.setCursor(0,1);
   lcd.print("LONG:");
   lcd.print(gps.location.lng(),6);
   delay(500);
   //serialPrint();
   //Serial1.print(" Speed:");
   //Serial1.print(Speed);
   //Serial1.println("Knots");
   //delay(500);
   serialPrint();
   
   Serial1.print("http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=");
   Serial1.print(gps.location.lat(), 6);
   Serial1.print("+");              //28.612953, 77.231545   //28.612953,77.2293563
   Serial1.print(gps.location.lng(),6);
   Serial1.write((char)26);
   delay(2000);
   serialPrint();
}

void serialPrint()
{
  while(Serial1.available()>0)
  {
    Serial.print(Serial1.read());
  }
}
