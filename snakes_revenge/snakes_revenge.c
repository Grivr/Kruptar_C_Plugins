#include "../gcc_dll.h"
#include "string.h"
#include <stdlib.h>

/*Snake's revenge 6 bit encoding plugin*/

unsigned char *outBuffer = NULL;
long int encodeSize;
//from tbl file
#define END '\x3F'




EXPORT void __stdcall Decode (tPluginData *data, long int offset){

int size = 1;//with NULL terminator
char *str = malloc(size);
char *src = data->rom+offset;
char outByte, srcByte = 0;
int bitcount = 0, i;



	str[0] = '\0';//init as empty string
	do
	{
		outByte = 0;
		if (size == 1)//1st symbol is not encoded
		{
			outByte = *src;	
			src++;
			srcByte = *src;
			src++;
		}
		else//usual encoded symbol of text string
		{
			for (i = 0; i < 6; i++)
			{
				if (bitcount >= 8)
				{
					
					srcByte = *src;
					src++;
					bitcount = 0;
				}
				outByte <<= 1;
				outByte |= ((srcByte &0x80) >> 7);
				srcByte <<= 1;
				bitcount++;	
			}
		}
		size++;//+1 symbol
		str = (char *)realloc (str, size);//#!could use fixed size buffers for perfomance
		str[size-2] = outByte;
		str[size-1] = '\0';
	}		
	while (outByte != END);


	data->tStringAdd(str, size - 1); //copy all, except null terminator
	free (str);
}


void addOut(char outByte){
	
	encodeSize++;
	outBuffer = realloc(outBuffer, encodeSize);
	outBuffer[encodeSize-1] = outByte;


}

EXPORT long int __stdcall GetEncodeSize (tPluginData *data){


char *srcStr = NULL;
int index = 0, srcI = 0;
long int strSize = 0;
char srcByte = 0, dstByte = 0;
int bit = 0, srcBitCount = 0, dstBitCount = 0;
	
	encodeSize = 0;

	
	while ((srcStr = data->tStringGet(index, &strSize)) != NULL)
	{	
		addOut(srcStr[srcI]);
		srcI++;
		srcByte = srcStr[srcI];
		srcByte <<= 2;

		do
		{
			bit = (srcByte & 0x80) >> 7;
			srcByte <<= 1;
			srcBitCount++;
			if (srcBitCount >= 6)//input byte is shifted-out
			{                     
				srcI++;
				if (srcI >= strSize)//no more input bytes, just copy last bits and exit
				{
					dstByte |= bit;							
					addOut(dstByte << (7 - dstBitCount));									
					break;						
				}
				srcByte = srcStr[srcI];
				srcByte <<= 2;
				srcBitCount = 0;
			}
			dstByte |= bit;
			if (dstBitCount == 7)//no more room in output byte, copy it to output
			{			
				addOut (dstByte);
				dstBitCount = 0;
				dstByte = 0;
			}
			else
			{
				dstByte <<= 1;
				dstBitCount++;
			}
		}
		while(1);
		index++;
	}
	return encodeSize;	
}

EXPORT void __stdcall Encode (tPluginData *data, void *buffer){
//main buffer was already filled at GetEncodeSize. Just copy from global buffer to Kruptar's allocated buffer so Kruptar could free it.
	
	memcpy(buffer, outBuffer, encodeSize);
	free(outBuffer);
	outBuffer = NULL;//outBuffer will be used by next pointer GetData processing
}
