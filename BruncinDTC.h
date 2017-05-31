/********************************************************************
*
*   Bruncin DTC Interface
*
*   The Bruncin Digital Temperature Chain (DTC) consists of a
*	number of digital thermometer modules with 16 MODEL# digital
*	thermometer ICs and 2 MUX chips. The DTC is interfaced over
*	TTL UART with a number of AT command set style commands.
*
*   2017-05-30  James Whitlock
*               Initial implementation
*
\*******************************************************************/

#ifndef BRUNCINDTC_H 
#define BRUNCINDTC_H 

#include <Arduino.h>

#define DTC_LENGTH			208
#define DTCBUFFER_LENGTH 	416
#define OUTBUFFER_LENGTH	130
#define DTC_BAUD			19200

class BruncinDTC {

    public:
    
		BruncinDTC (HardwareSerial& s):serialPort(s){}

        boolean begin(void); 
        void reset(void);
        boolean read(void);
		boolean packBits(void);
		
		static uint8_t readBuffer[DTCBUFFER_LENGTH];
		static uint16_t	outBuffer[OUTBUFFER_LENGTH];
		static uint16_t lineidx;
		static boolean recvdflag;

    private:

		HardwareSerial& serialPort;
		
		boolean push(uint8_t c);
};


#endif