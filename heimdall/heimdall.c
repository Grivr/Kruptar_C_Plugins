#include "../gcc_dll.h"
#include "string.h"
#include <stdlib.h>

/*Heimdall Sega CD plugin*/

unsigned char *outBuffer = NULL;
long int encodeSize;
//from tbl file
#define END '\00'




EXPORT void __stdcall Decode (tPluginData *data, long int offset){

int size = 1;//with NULL terminator
char *str = malloc(size);
char *src = data->rom+offset;



	str[0] = '\0';//init as empty string
	do
	{
		size++;//+1 symbol
		str = (char *)realloc (str, size);//#!could use fixed size buffers for perfomance
		str[size-2] = *src++;
		str[size-1] = '\0';
	}		
	while (*src != END);


	data->tStringAdd(str, size);
	free (str);
}


EXPORT long int __stdcall GetEncodeSize (tPluginData *data){


char *srcStr = NULL;
int index = 0, i;
long int strSize = 0;
long int* pStrSize = &strSize;
	
	encodeSize = 0;
	while ((srcStr = data->tStringGet(index, pStrSize)) != NULL)//process all strings, added for one pointer by Decode
	{		
		strSize++;//each string has 1 terminator
		outBuffer = realloc(outBuffer, encodeSize+strSize);//with an END terminator
		for (i = 0; i< strSize-1; i++)
		{
			outBuffer[encodeSize + i] = srcStr[i];		
		}

		encodeSize += strSize;
		outBuffer[encodeSize-1] = END;
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
