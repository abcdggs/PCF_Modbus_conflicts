#include "pinout.h"
#include "PCF8574.h"

PCF8574 I2C_1(0x20);
PCF8574 I2C_2(0x21);
PCF8574 I2C_3(0x22);
PCF8574 I2C_4(0x23);

pinout *pinout::_instance;

// [static]
pinout &pinout::instance() {
    if (!_instance) {
        _instance = new pinout();
    }
    return *_instance;
}

pinout::pinout() {
}

pinout::~pinout() {
}

void pinout::setup() {
	pinSetups();
	I2cSetup();
}

void pinout::loop() {
    if (millis() - wd_last_checked_time > wd_check_freq){
		servingWatchdog();
	}
}

void pinout::I2cSetup(){
	if (I2C_1.begin()){
		Serial.println("I2C 1 OK");
		I2C[0] = 1;
	}else{
		Serial.println("I2C 1 KO *********************");
	}
	if (I2C_2.begin()){
		Serial.println("I2C 2 OK");
		I2C[1] = 1;
	}else{
		Serial.println("I2C 2 KO *********************");
	}
	if (I2C_3.begin())
	{
		Serial.println("I2C 3 OK");
		I2C[2] = 1;
	}else{
		Serial.println("I2C 3 KO *********************");
	}
	if (I2C_4.begin()){
		Serial.println("I2C 4 OK");
		I2C[3] = 1;
	}else{ 
		Serial.println("I2C 4 KO *********************");
	}

	I2C_1.digitalWrite(P1, LOW);
	I2C_1.digitalWrite(P0, LOW);
	I2C_1.digitalWrite(P2, LOW);
	I2C_1.digitalWrite(P3, LOW);
	I2C_1.digitalWrite(P4, LOW);
	I2C_1.digitalWrite(P6, LOW);
	I2C_1.digitalWrite(P7, LOW);  


	I2C_2.digitalWrite(P3, LOW);// EN_12V_RFID - switches power on and off for only RFID

	I2C_3.digitalWrite(P1, HIGH); // SHT_nRESET
	I2C_3.digitalWrite(P3, HIGH); // COMMS POWER ENABLE
	I2C_3.digitalWrite(P4, LOW);  // SEL_B SEL_1
	I2C_3.digitalWrite(P5, HIGH); // SEL_A SEL_0
	I2C_3.digitalWrite(P6, LOW);  // TX_EN
	I2C_3.digitalWrite(P7, LOW);  // RX_EN
	

	I2C_4.digitalWrite(P4, HIGH); // 
	I2C_4.digitalWrite(P5, HIGH); // 
	I2C_4.digitalWrite(P6, HIGH); // 
	I2C_4.digitalWrite(P7, HIGH); // 


}

void pinout::rs485_toggle(bool status){
	I2C_3.digitalWrite(P6, status);
	I2C_3.digitalWrite(P7, status);
}

void pinout::i2cCheckFeedbacks(){
	solar_on = !I2C_2.digitalRead(P0); 
	charging = !I2C_2.digitalRead(P1); 
	fb_RS_PWR = I2C_3.digitalRead(P2); 
}

void pinout::pinSetups(){
	pinMode(WATCHDOG_PIN, OUTPUT);
	pinMode(EN_12V, OUTPUT);
	pinMode(I2C_POWER, OUTPUT);
	digitalWrite(I2C_POWER, HIGH);
	digitalWrite(EN_12V, LOW); 

	I2C_1.pinMode(P0, OUTPUT); 
	I2C_1.pinMode(P1, OUTPUT); 
	I2C_1.pinMode(P2, OUTPUT); 
	I2C_1.pinMode(P3, OUTPUT); 
	I2C_1.pinMode(P4, OUTPUT); 
	I2C_1.pinMode(P7, OUTPUT); 
	I2C_1.pinMode(P5, OUTPUT); 
	I2C_1.pinMode(P6, OUTPUT); 

	I2C_2.pinMode(P0, INPUT);  // PG - solar on - RED
	I2C_2.pinMode(P1, INPUT);  // ST1 - charging - GREEN
	I2C_2.pinMode(P2, INPUT);  // ST2 - charging complete - BLUE
	I2C_2.pinMode(P3, OUTPUT); // EN_12V_RFID - switches power on and off for only RFID
	I2C_2.pinMode(P4, INPUT);  
	I2C_2.pinMode(P5, INPUT);  
	I2C_2.pinMode(P6, INPUT);  
	I2C_2.pinMode(P7, INPUT);  // 12V_FEEDBACK - 

	I2C_3.pinMode(P0, INPUT);  // SHT_ALERT
	I2C_3.pinMode(P1, OUTPUT); // SHT_nRESET
	I2C_3.pinMode(P2, INPUT);  // RS485 POWER FEEDBACK - high when RFID has power - has fuse
	I2C_3.pinMode(P3, OUTPUT); // COMMS POWER ENABLE
	I2C_3.pinMode(P4, OUTPUT); // SEL_B
	I2C_3.pinMode(P5, OUTPUT); // SEL_A
	I2C_3.pinMode(P6, OUTPUT); // TX_EN
	I2C_3.pinMode(P7, OUTPUT); // RX_EN

	I2C_4.pinMode(P0, INPUT);  // board hardware identification register
	I2C_4.pinMode(P1, INPUT);  // board hardware identification register
	I2C_4.pinMode(P2, INPUT);  // board hardware identification register
	I2C_4.pinMode(P3, INPUT);  // board hardware identification register
	I2C_4.pinMode(P4, OUTPUT); // 
	I2C_4.pinMode(P5, OUTPUT); // 
	I2C_4.pinMode(P6, OUTPUT); // 
	I2C_4.pinMode(P7, OUTPUT); // 
}

void pinout::servingWatchdog(){
	wd_last_checked_time = millis();
	Log.info("SERVICING WD");
	digitalWrite(WATCHDOG_PIN, HIGH);
	delayMicroseconds(2);
	digitalWrite(WATCHDOG_PIN, LOW);
}
