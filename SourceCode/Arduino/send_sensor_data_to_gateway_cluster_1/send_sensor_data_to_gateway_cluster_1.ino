#include <SPI.h>
#include <RF24.h>
#include "DHT.h"
#include <AES.h>

RF24 radio(9, 10);
AES aes ;

const byte *key = (unsigned char*)"0123456789010123";
const byte KEY_SIZE = 128; // size of key (bit)
const byte DATA_SIZE = 32; // size of payload and cipher  (byte)
const unsigned long long int my_iv = 36753562;
const int DHTPIN = A0;
const int DHTTYPE = DHT22;
const int CLUSTER_ID = 1;
const int CLUSTER_INDEX = 0;

const int TEMP_EXISTS_INDEX = 1;
const int TEMP_INT_INDEX = 2;
const int TEMP_FRACTION_INDEX = 3;

const int HUMI_EXISTS_INDEX = 4;
const int HUMI_INT_INDEX = 5;
const int HUMI_FRACTION_INDEX = 6;

const int CO2_EXISTS_INDEX = 7;
const int CO2_INT_INDEX = 8;
const int CO2_FRACTION_INDEX = 9;

const int ILLUMINANCE_EXISTS_INDEX = 10;
const int ILLUMINANCE_INT_INDEX = 11;
const int ILLUMINANCE_FRACTION_INDEX = 12;

const int FARM_INDEX = 13;
const int FARM_ID = 01;



DHT dht(DHTPIN, DHTTYPE);
const uint64_t pipe = 0xF0F0F0F0E2`LL;
float data[2];
byte msg[32];
byte cipher [32] ;
void setup(void) {
  Serial.begin(9600);
  printf_begin();
  radio.begin();
  sensor_setup();
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(0x76);
  radio.openWritingPipe(pipe);
 // radio.setDataRate(RF24_2MBPS);
  radio.enableDynamicPayloads();
  radio.setRetries(1, 15);
  radio.printDetails();

}

void sensor_setup(){
  msg[TEMP_EXISTS_INDEX] = 1;
  msg[HUMI_EXISTS_INDEX] = 1;
  msg[FARM_INDEX] = FARM_ID;
}


void loop(void) {
  float humi = dht.readHumidity();    
  float temp = dht.readTemperature(); 
  byte humi_int = read_integer_from_float(humi);
  byte humi_frac = read_fraction_from_float(humi);
  byte temp_int = read_integer_from_float(temp);
  byte temp_frac = read_fraction_from_float(temp);

  msg[TEMP_INT_INDEX] = temp_int;
  msg[TEMP_FRACTION_INDEX] = temp_frac;
  msg[HUMI_INT_INDEX] = humi_int;
  msg[HUMI_FRACTION_INDEX] = humi_frac;

  msg[CLUSTER_INDEX] = CLUSTER_ID;
 
  encryptData();
  bool result = radio.write(&cipher, sizeof(cipher));
 
  Serial.print("\n");
  Serial.print(temp_int);
  Serial.print("\n");
  Serial.print(temp_frac);
  Serial.print("\n");

  Serial.print("humi : ");
  Serial.print(msg[HUMI_INT_INDEX]);
  Serial.print("\n");
  Serial.print(msg[HUMI_FRACTION_INDEX]);

  Serial.print("  false \n");
  delay(10000);
}

void encryptData( ){
  aes.iv_inc();
  byte iv [N_BLOCK] ;
 // byte check [32] ;
 // unsigned long ms = micros ();
  aes.set_IV(my_iv);
  aes.get_IV(iv);
 
  aes.do_aes_encrypt(msg,32,cipher,key,KEY_SIZE,iv);
  Serial.print("Encryption took: ");
//  for(int i = 0; i < 32; i++)
//  {
//    Serial.println(cipher [i]);
//  }
  //ms = micros ();
 // aes.set_IV(my_iv);
 // aes.get_IV(iv);
 // aes.do_aes_decrypt(cipher,32,check,key,128,iv);
 // Serial.print("Decryption took: ");
 // Serial.println(micros() - ms);
 
//  for(int i = 0; i < 32; i++)
//  {
//    Serial.println(msg [i]);
//
//  }
  
 
  printf("\n============================================================\n");
}
byte read_integer_from_float(float value){
  return (byte) value;  
}

byte read_fraction_from_float(float value){
  return (byte(int(value * 100))%100);  
}


int serial_putc( char c, FILE * ) {
  Serial.write( c );
  return c;
}

void printf_begin(void) {
  fdevopen( &serial_putc, 0 );
}
