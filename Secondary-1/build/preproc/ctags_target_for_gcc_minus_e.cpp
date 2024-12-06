# 1 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Secondary-1\\Secondary-1.ino"
/*

  Secondary Sensors Sender



  The Skectch Made for Secondarys that collect Data from Sensors and Send them to the Priamry to be send to other locations.



  Pin List

    - D0 RX

    - D1 TX

    - D2 LED

    - D4 AM2302 Heat/DHT22

    - D5 SR-HC 50x Motion

    - D6 SW-1815P Vibration Sensor

    - D7 DS18B20 Heat

    - D14/A0 Water Leak Set 1 (Digital)

    - D15/A1 Water Leak Set 2 (Digital)

    - D18/A6 Water Leak Set 1 (Analog)

    - D19/A7 Water Leak Set 2 (Analog)



Coil Address Index

 - (0) Command to Read Sensors



Discrete Inputs Address Index 

 - ()



Input Register Address Index (InputRegister)[FloatRegister]

 - (0-5): Placeholder Datetime Data (Could be remove or add depending on speed of modbus)

 - (6-9)[4-5] DS18B20 Temperature and Humidity

 - (10-13)[6-7] Temperature and Humidity from DHT22







  Created on November 11, 2024

  By Zachary Schultz



*/
# 37 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Secondary-1\\Secondary-1.ino"
// Initializing libraries
# 39 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Secondary-1\\Secondary-1.ino" 2
//Needed for .h Files
# 41 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Secondary-1\\Secondary-1.ino" 2
# 42 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Secondary-1\\Secondary-1.ino" 2

//Importing .h files
# 45 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Secondary-1\\Secondary-1.ino" 2
# 46 "C:\\Users\\Zach_\\Documents\\Code\\HGV-Coop\\Rems006\\Secondary-1\\Secondary-1.ino" 2

//Modbus Arrays
bool Coils[1 /* Number of used Coil Address*/];
bool DiscreteInputs[5 /* Number of used Discrete Inputs Address*/];
uint16_t HoldingRegister[1 /* Number of used Holding Register Address*/];
uint16_t InputRegister[6 /* Number of used Input Register Address*/];
float *FloatRegister = (float*)InputRegister; // Turns an array of uint16 into floats by taking array in pairs
uint16_t LastHolding = HoldingRegister[0];

// Creating Modbus Connection
ModbusRTUSlave modbus(Serial /* Which Serial Is being Used*/); // No DERE Pins Used

void setup(){
  modbus.configureCoils(Coils,1 /* Number of used Coil Address*/);
  modbus.configureDiscreteInputs(DiscreteInputs,5 /* Number of used Discrete Inputs Address*/);
  modbus.configureHoldingRegisters(HoldingRegister,1 /* Number of used Holding Register Address*/);
  modbus.configureInputRegisters(InputRegister,6 /* Number of used Input Register Address*/);

  Serial.begin(9600);
  modbus.begin(2, 9600);

  Serial.println("Secondary Board Sketch");
  Serial.print("Board ID: ");
  Serial.println(2);
  delay(1000);
}

void loop() {
  modbus.poll(); // Checks for changes
  if (Coils[0] == 1) {
    FloatRegister[0] = DS18B20_Temp();
    Coils[0] = 0;
  }
}
