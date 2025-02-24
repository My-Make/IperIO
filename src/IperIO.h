#ifndef IperIO_h
#define IperIO_h

#define SLAVE1_ADRS 1
#define SLAVE2_ADRS 2

#define CMD_PINSETUP 1
#define CMD_PINWRITE 2
#define CMD_PINREAD 3
#define CMD_PORTREAD 4
#define CMD_PINALLIN 5
#define CMD_GETPINMODE 6

#define ERROR 0x100
#define ERROR_TIMEOUT 0x200
#define NO_ERROR 0

#define LED_G 15 
#define LED_R 14
#define LED_Y LED_G + LED_R 

#define MASTER 0
#define SLAVE1 SLAVE1_ADRS
#define SLAVE2 SLAVE2_ADRS

#define TIMEOUT_MS 5000

class IperIO 
{
private:
	static int myPins[];
	static uint8_t* pinsRegistry[];
	static uint8_t myPinsMode[43];

public:
	IperIO();
	void begin(void);
	void pinSetup(uint8_t, uint8_t);
	int pinRead(uint8_t);
	void pinWrite(uint8_t, uint8_t);
	void statusLEDON(uint8_t led = LED_G + LED_R);
	void statusLEDOFF(uint8_t led = 0);
	void setAllInputs(void);
	int portRead(uint8_t , uint8_t);
};

extern IperIO IperIO;

#endif	