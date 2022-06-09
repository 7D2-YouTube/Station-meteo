#include <LiquidCrystal.h> //librairie pour l'afficheur LCD
#include <dht_nonblocking.h> //librairie pour le capteur d'humidité et de température
#include <Wire.h> //librairie pour le DS1307
#include <DS3231.h>//librairie pour le DS1307

#define DHT_SENSOR_TYPE DHT_TYPE_11 //définir le type de capteur

DS3231 clock;
RTCDateTime dt;

float temperature;
float humidite;

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //pins afficheur LCD
static const int DHT_SENSOR_PIN = 2; //pin capteur d'humidité et de température
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

static bool measure_environment( float *temperature, float *humidite )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( )/10000 - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidite ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2); //taille de l'afficheur

  // Initialize DS3231
  clock.begin();
}

void loop() {
  if( measure_environment (&temperature, &humidite) == true)
  {
    //afficher la temperature et l'humidité
    lcd.setCursor(3, 0);
    lcd.print( "T = " );
    lcd.print( temperature, 1);
    lcd.print( " C" );
    lcd.setCursor(3, 1);
    lcd.print( "H = " );
    lcd.print( humidite, 1);
    lcd.print( " %" );
    delay(5000);
  
    lcd.clear();
    delay(10);

    //afficher l'heure et la date
    dt = clock.getDateTime();

    lcd.setCursor(4, 0);
    lcd.print(dt.year);   lcd.print("-");
    lcd.print(dt.month);  lcd.print("-");
    lcd.print(dt.day);    lcd.print(" ");
    lcd.setCursor(5, 1);
    lcd.print(dt.hour);   lcd.print(":");
    lcd.print(dt.minute);

    delay(5000);

    lcd.clear();
    delay(10);
  }
  
}
