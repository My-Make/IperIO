#include <Wire.h>
#include <Arduino.h>
#include "IperIO.h"

int IperIO::myPins[] = {22, 23, 24, 25, 26, 27, 28, 29, 53, 52, 51, 50, 10, 11, 12, 13, 37, 36, 35, 34, 33, 32, 31, 30, 41, 40, 39,  4, 17, 16,  6,  7,  8,  9, 49, 48, 47, 46, 45, 44, 43, 42, 19, 18};
uint8_t* IperIO::pinsRegistry[] = {&PINA, &PINB, &PINC, &PIND, &PING, &PINH, &PINL};
uint8_t IperIO::myPinsMode[43] = {INPUT};

IperIO::IperIO() {
}

void IperIO::begin(void) {
	pinMode(LED_G, OUTPUT);
	pinMode(LED_R, OUTPUT);
	digitalWrite(LED_G, LOW);
	digitalWrite(LED_R, LOW);
	Wire.begin();  
    Wire.setClock(10000);
	setAllInputs();
	Serial.begin(115200);
	delay(1000);
}

void IperIO::statusLEDON(uint8_t led) {
	if (led == LED_G || led == LED_R) {
		digitalWrite(led, HIGH);
	}
	else if (led == LED_G + LED_R) {
		digitalWrite(LED_G, HIGH);
		digitalWrite(LED_R, HIGH);
	}
}

void IperIO::statusLEDOFF(uint8_t led) {
	if (led == 0) {
		digitalWrite(LED_G, LOW);
		digitalWrite(LED_R, LOW);
	}
	digitalWrite(led, LOW);
}

void IperIO::pinSetup(uint8_t pin, uint8_t mode) {
  uint8_t addr;
  uint8_t p;
  if (mode >= INPUT && mode <= INPUT_PULLUP) {
    if (pin > 41) {
      if (pin < 84) {
        addr = SLAVE1_ADRS;
        p = pin - 42;
      }
      else {
        addr = SLAVE2_ADRS;
        p = pin - 84;
      }
      Wire.beginTransmission(addr);
      Wire.write(CMD_PINSETUP);
      Wire.write(p);
      Wire.write(mode);
      Wire.endTransmission();       
    }
    else {
      pinMode(myPins[pin],mode);
    }
  }
}

int IperIO::pinRead(uint8_t pin) {
  uint8_t addr;
  uint8_t p;
  int value;
  if (pin > 41) {
    if (pin < 84) {
        addr = SLAVE1_ADRS;
        p = pin - 42;      
    }
    else {
        addr = SLAVE2_ADRS;
        p = pin - 84;
    }
    Wire.beginTransmission(addr);
    Wire.write(CMD_PINREAD);
    Wire.write(p);
    Wire.endTransmission();  
    Wire.requestFrom((int)addr, 2);
	unsigned long startTime = millis();
    while (Wire.available() < 2) {
		Wire.requestFrom((int)addr, 2);
		if (millis() - startTime > TIMEOUT_MS) {
			break;			
		}
	}
	if (Wire.available() >= 2) {
		int err = Wire.read();
		value = Wire.read();
		if (err != NO_ERROR) {
			value = ERROR;
		}
	}
	else  {
		value = ERROR_TIMEOUT;
	}
  }
  else {
    value = digitalRead(myPins[pin]);
  }
  return value;
}

void IperIO::pinWrite(uint8_t pin, uint8_t value) {
    uint8_t addr;
    uint8_t p;
    if (pin > 41) {
    if (pin < 84) {
        addr = SLAVE1_ADRS;
        p = pin - 42;      
    }
    else {
        addr = SLAVE2_ADRS;
        p = pin - 84;
    }
    Wire.beginTransmission(addr);
    Wire.write(CMD_PINWRITE);
    Wire.write(p);
    Wire.write(value);
    Wire.endTransmission();  
  }
  else {
    digitalWrite(myPins[pin], value);
  }
}

int IperIO::portRead(uint8_t port, uint8_t device) {
	Serial.println("portread");
	int value = 0;
	if (port >= 0 && port <= 6) {
		if (device == MASTER) {
			value = *pinsRegistry[port];
		}
		else if (device == SLAVE1 || device == SLAVE2) {
			Serial.print(" device");
			Serial.println(device);
			Serial.print(" port");
			Serial.println(port);

			Wire.beginTransmission(1);
			Wire.write(4);
			Wire.write(0);
			Wire.endTransmission();  
			
			Wire.requestFrom(1, 2);
			while (Wire.available() < 2) {}
			int err = Wire.read();
			value = Wire.read();
			if (err != NO_ERROR) {
			  value = ERROR;
			}
		}
	}
	else {
		value = ERROR;
	}
	return value;
}

void IperIO::setAllInputs() {
	DDRA = 0;
	PORTA = 0xFF;
    DDRB = 0;
	PORTB = 0xFF;
    DDRC = 0;
	PORTC = 0xFF;
	DDRD = 0;
	PORTD = 0x0C;
	DDRG = 0;
	PORTG = 0x27;
    DDRH = 0;
	PORTH = 0x7B;
    DDRL = 0;
	PORTL = 0xFF;
	DDRE = B00110000;
	memset(myPinsMode, INPUT_PULLUP, sizeof(myPinsMode));
    Wire.beginTransmission(SLAVE1_ADRS);
    Wire.write(CMD_PINALLIN);
	Wire.write(CMD_PINALLIN);
    Wire.endTransmission();       	
    Wire.beginTransmission(SLAVE2_ADRS);
    Wire.write(CMD_PINALLIN);
	Wire.write(CMD_PINALLIN);
    Wire.endTransmission();   	
}