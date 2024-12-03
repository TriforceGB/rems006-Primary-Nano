# 1 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Primary\\Primary.ino"
/*

  Primary Arduino Control



  This sketch is the Code That lets the Priamry Act as a RTC Master, TCP Server, and Control All Sensor and System Connected



  Pin List

    - D0 RX

    - D1 TX

    - D2 LED

    - D3 Smoke

    - D4 Heat On

    - D5 Water Off

    - D6 Power Off

    - D7 Cool On

    - D8 Eth-Int

    - D10 Eth-CSN

    - D11 Eth-MOSI

    - D12 Eth-MISO

    - D13 Eth-SCK





    Modbus Address

    Coils:

      Currently Just for Testing

    discreteInputs:

      - 0=Motion

      - 1=Water?

      - 2=...

    Holding Register:

      Currently Just for Testing

    Input Register:

    - 0-1=Temperature

    - 2-3=Humidity



  Created on November 11, 2024

  By Zachary Schultz



*/
# 40 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Primary\\Primary.ino"
// Initializing libraries
# 42 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Primary\\Primary.ino" 2
# 43 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Primary\\Primary.ino" 2
# 44 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Primary\\Primary.ino" 2
# 45 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Primary\\Primary.ino" 2

//Importing .h files
# 48 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Primary\\Primary.ino" 2
# 49 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Primary\\Primary.ino" 2

//EthernetClient client; //IDK What this Does or if it needed Probably Later

//Modbus Arrays
// bool Coils[NumSecondary][CoilAddress];
bool discreteInputs[4 /* Amount of Secondary Boards Being Used*/][5 /* Number of used Discrete Inputs Address*/];
// uint16_t HoldingRegisters[NumSecondary][HRAddress];
uint16_t InputRegister[4 /* Amount of Secondary Boards Being Used*/][6 /* Number of used Input Register Addresss*/];
float (*FloatRegisters)[6 /* Number of used Input Register Addresss*/] = (float(*)[6 /* Number of used Input Register Addresss*/])InputRegister; // Turns an array of uint16 into floats by taking array in pairs

// Creating Modbus Connection
ModbusRTUMaster modbus(Serial /* Which Serial Is being Used*/); // No DERE Pins Used


void setup() {
  Serial.begin(9600); // Could be remove after everything is wokring? It might only be needed for Debuging
  modbus.begin(9600);
  Serial.println("Primary Board Sketch");
  delay(1000);

  //check if We can get internet connection Just for testing will want a static IP Later
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    }
    else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
      // while(true) { // no point in carrying on, so do nothing forevermore:
        Serial.println("Board is most likely not the Priamry Board");
        delay(1000);
      // }    
  }
  else {
    Serial.println(Ethernet.localIP());
    Serial.println("This is a Priamry Board");
  }
}

void loop(){
  Serial.println();
  Serial.println("----------------------------------------------------------------");
  for (int i = 0; i < 4 /* Amount of Secondary Boards Being Used*/; i++) {
    errorCheck(modbus.readDiscreteInputs(i+1,0,discreteInputs[i],5 /* Number of used Discrete Inputs Address*/));
    delay(100);
  }
  Serial.println();
  Serial.println("-----Discrete Input-----");
  Serial.println(String(discreteInputs[0][0])+String(discreteInputs[0][1])+discreteInputs[0][2]+discreteInputs[0][3]+discreteInputs[0][4]);
  Serial.println(discreteInputs[1][0]+discreteInputs[1][1]+discreteInputs[1][2]+discreteInputs[1][3]+discreteInputs[1][4]);
  Serial.println(discreteInputs[2][0]+discreteInputs[2][1]+discreteInputs[2][2]+discreteInputs[2][3]+discreteInputs[2][4]);
  Serial.println(discreteInputs[3][0]+discreteInputs[3][1]+discreteInputs[3][2]+discreteInputs[3][3]+discreteInputs[3][4]);
  delay(5000);


  for (int i = 0; i < 4 /* Amount of Secondary Boards Being Used*/; i++) {
    errorCheck(modbus.readInputRegisters(i+1,0,InputRegister[i],6 /* Number of used Input Register Addresss*/));
    delay(100);
  }
  Serial.println();
  Serial.println("-----Input Register Raw-----");
  Serial.println(InputRegister[0][0]+InputRegister[0][1]+InputRegister[0][2]+InputRegister[0][3]+InputRegister[0][4]+InputRegister[0][5]);
  Serial.println(InputRegister[1][0]+InputRegister[1][1]+InputRegister[1][2]+InputRegister[1][3]+InputRegister[1][4]+InputRegister[1][5]);
  Serial.println(InputRegister[2][0]+InputRegister[2][1]+InputRegister[2][2]+InputRegister[2][3]+InputRegister[2][4]+InputRegister[2][5]);
  Serial.println(InputRegister[3][0]+InputRegister[3][1]+InputRegister[3][2]+InputRegister[3][3]+InputRegister[3][4]+InputRegister[3][5]);
  Serial.println();
  Serial.println("-----Input Register Float-----");
  Serial.println(FloatRegisters[0][0]+FloatRegisters[0][1]+FloatRegisters[0][2]);
  Serial.println(FloatRegisters[1][0]+FloatRegisters[1][1]+FloatRegisters[1][2]);
  Serial.println(FloatRegisters[2][0]+FloatRegisters[2][1]+FloatRegisters[2][2]);
  Serial.println(FloatRegisters[3][0]+FloatRegisters[3][1]+FloatRegisters[3][2]);
  delay(5000);
  Serial.println("Writing to Coils");
  errorCheck(modbus.writeSingleCoil(0,0,1));
  delay(100);
  Serial.println("Writing to Holding Register");
  errorCheck(modbus.writeSingleHoldingRegister(0,0,175));
  delay(3000);


}
