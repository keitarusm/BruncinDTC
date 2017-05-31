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
*   2017-05-30  James Whitlock
*               Initial implementation
*
\*******************************************************************/

#include <BruncinDTC.h>

boolean BruncinDTC::begin (void)
{
	serialPort.begin(DTC_BAUD);
	delay(10);
	serialPort.print("\r\n");
	delay(10);
	serialPort.print("BT+TEST,26");
	
	unsigned long startRead = millis();
	while (millis()<startRead+10000) {
		char a = 0;
		char b = 0;
		
		if (serialPort.available()) {
			b = serialPort.read();
			if ( a == 'O' && b == 'K') {
				return true;
			} else {
				a = b;
			}
		}
	}

	return false;
}

void BruncinDTC::reset (void)
{
	serialPort.end();
	
	memset (readBuffer, 0, DTCBUFFER_LENGTH);
	lineidx = 0;
	recvdflag = false;
}

boolean BruncinDTC::read (void)
{
	serialPort.print("BT+TEMP,26,1;2");
	
	unsigned long startRead = millis();
	while (!recvdflag || millis()<startRead+10000) {
		if (serialPort.available()) {
			uint8_t c = 0;
			c = serialPort.read();
			push(c);
		}
	}
	
	if (recvdflag) {
		recvdflag = false;
		return true;
	} else {
		return false;
	}
}

boolean BruncinDTC::packBits (void) 
{
	int16_t highLow[DTC_LENGTH];
	for (int i=0; i<DTC_LENGTH; i++) {
		highLow[i] = ((int16_t)readBuffer[2*i] << 8) | readBuffer[2*i+1];
	}
	
	int16_t tenBit[DTC_LENGTH];
	for (int i=0; i<DTC_LENGTH; i++) {
		tenBit[i] = (highLow[i] >> 4);
		tenBit[i] = tenBit[i] & 0x03FF;
	}
	
	int j = 0;
	for (int i=0; i<OUTBUFFER_LENGTH; i=i+5) {
		outBuffer[i] = tenBit[j]      | (tenBit[j+1]<<10);
		outBuffer[i+1] = (tenBit[j+1]>>6) | (tenBit[j+2]<<4) | (tenBit[j+3]<<14);
		outBuffer[i+2] = (tenBit[j+3]>>2) | (tenBit[j+4]<<8);
		outBuffer[i+3] = (tenBit[j+4]>>8) | (tenBit[j+5]<<2) | (tenBit[j+6]<<12);	outBuffer[i+4] = (tenBit[j+6]>>4) | (tenBit[j+7]<<6);
		j = j+8;
	}
}

boolean BruncinDTC::push (uint8_t  c)
{
	if (lineidx >= DTCBUFFER_LENGTH-1) {
        readBuffer[lineidx] = c;
        lineidx = 0;
        recvdflag = true;
        return recvdflag;
    }

    readBuffer[lineidx] = c;
    lineidx++;

    return recvdflag;
}