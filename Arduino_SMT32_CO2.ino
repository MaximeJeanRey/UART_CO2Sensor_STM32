#include <SoftwareSerial.h>
//#include <pas-co2-ino.hpp>

static int data = 0;
String msgRecu = "0";
String val = "";
char stringChar[2]; 
char stringChar2[2]; 
HardwareSerial Serial_1(PB_7, PB_6);


void setup() {
    
    Serial_1.begin(9600);
    Serial.begin(115200);
    delay(1000);    
    // Idle mode
    //writeUartRegister("10", "A3");
    //delay(1500);
    writeUartRegister("04", "00");
    delay(800);


    //Set pressure : 
    writeUartRegister("0B", "03");
    writeUartRegister("0C", "F5");

    
    // Set measurement rate to 10 s
    writeUartRegister("02", "00");
    writeUartRegister("03", "0A");
  
    // Configure continous mode
   writeUartRegister("04", "02");
   Serial.flush();
    
}
 
void loop() {
 
  /*//String value1 = readUartRegister("01");
  //delay(150);
  //Serial.println(value1);
  writeUartRegister("04", "01");
  delay(800);
  
  String value2 = "";
  String value1 = readUartRegister("05");
  delay(50);
  //do {
    //int i = 0;
    value2 = readUartRegister("06");
    //i++;
   // Serial.println(i);
  //}while(value2 == value1);

  delay(50);

  //value1.toCharArray(stringChar, value1.length() + 1);
  //int value1int = hex2int(stringChar);
  
  //value2.toCharArray(stringChar, value1.length() + 1);
  //int value2int = hex2int(stringChar);  

  Serial.print("Value 1 et 2 = ");
  Serial.println(value1);
  Serial.println(value2);
  
  //int16_t result = value1 << 8 | value2;
  Serial.print("CO2: ");
  //Serial.print(result);
  Serial.println(" ppm");

  Serial.println("Fin");*/
  
  msgRecu = readUartRegister("07");
  delay(900);  
  
  Serial.print("Message recu = ");
  Serial.println(msgRecu);
 // Get PPM value
  if(msgRecu == "10"){
    Serial.println("10 recu");
    String value2 = "";
    String value1 = "";

    do {
        value1 = readUartRegister("05");
        Serial.print("value 1 = ");
        Serial.println(value1);
        delay(300);
    } while(value1 == "10");

    delay(150);
    
    do {
      value2 = readUartRegister("06");
      Serial.print("value 2 = ");
      Serial.println(value2);
      delay(300);
    } while(value2 == value1);
  
    delay(50);
    
    value1.toCharArray(stringChar, value1.length() + 1);
    int value1int = hex2int(stringChar);
    
    value2.toCharArray(stringChar, value1.length() + 1);
    int value2int = hex2int(stringChar);  
  
    Serial.print("Value 1 et 2 = ");
    Serial.println(value1int);
    Serial.println(value2int);
    
    int16_t result = value1int << 8 | value2int;
    Serial.print("CO2: ");
    Serial.print(result);
    Serial.println(" ppm");
   }
}


String readUartRegister(String register_){
  Serial_1.write("R");
  Serial_1.write(0x2C);
  Serial_1.write(register_[0]);// adress bit 0
  Serial_1.write(register_[1]);//adress 1
  Serial_1.write(0x0A);

  String result = "";
  while (Serial_1.available() > 0) {
    char inByte = Serial_1.read();
    if (inByte == 0x0A) {
      return result;
    }
    else{
      //Serial.print("OldByte = ");
      //Serial.println(inByte);
      result += inByte;
      //Serial.print("OldByte = ");
      //Serial.println(inByte);
    }
  }
}

void writeUartRegister(String register_, String dataToSend){
  Serial_1.write("W");
  Serial_1.write(0x2C);
  Serial_1.write(register_[0]);// adress bit 0
  Serial_1.write(register_[1]);//adress 1
  Serial_1.write(0x2C);
  Serial_1.write(dataToSend[0]);
  Serial_1.write(dataToSend[1]);
  Serial_1.write(0x0A);

  String result = "";
  /*while (Serial_1.available() > 0) {
    char inByte = Serial_1.read();
    if (inByte == 0x0A) {
      Serial.println("Return dans write");
      return;
    } else {
      if (inByte == 0x06){
        Serial.println("ACK");
      }
      else if (inByte == 0x15){
        Serial.println("NACK");
      }
      result += inByte;
    }
  }*/
}


/**
 * hex2int
 * take a hex string and convert it to a 32bit number (max 8 hex digits)
 */
uint32_t hex2int(char *hex) {
    uint32_t val = 0;
    while (*hex) {
        // get current character then increment
        uint8_t byte = *hex++; 
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
        // shift 4 to make space for new digit, and add the 4 bits of the new digit 
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}
