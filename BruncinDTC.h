/********************************************************************
*	Bruncin DTC Interface
*   Copyright (C) 2017  James Whitlock
*
*   This program is free software: you can redistribute it and/or 
*   modify it under the terms of the GNU Lesser General Public License 
*   as published by the Free Software Foundation, either version 3 of
*   the License, or (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Lesser General Public License for more details.
*
*   You should have received a copy of the GNU Lesser General Public 
*   License along with this program.  
*   If not, see <http://www.gnu.org/licenses/>.
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