#include "../gcc_dll.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char u8;
typedef struct {

	u8 *mem;
	int pos;
	int size;
} buffer;

buffer outBuffer;

//long int encodeSize;
//from tbl file
#define END '\xFF'

void pushOut(buffer * buf, u8 srcByte)
{

	buf->size++;
	buf->mem = realloc(buf->mem, buf->size);
	buf->mem[buf->pos] = srcByte;
	buf->pos++;
}

void destroyOut(buffer * buf)
{
	free(buf->mem);
	buf->mem = NULL;	//outBuffer will be used by next pointer GetData processing
	buf->pos = 0;
	buf->size = 0;
}

EXPORT void __stdcall Decode(tPluginData * data, long int offset)
{

	unsigned char hdr[3];	//3 header bytes
	int size = 9;		//2*3 inttohex+[]: 8chars+\0
	char *str = malloc(size);
	char *src = data->rom + offset;

	hdr[0] = *src++;
	hdr[1] = *src++;
	hdr[2] = *src++;
	snprintf(str, size, "[%2x%2x%2x]", hdr[0], hdr[1], hdr[2]);	//write header in brackets (which should be in table file in kpx)
	do {
		size++;		//+1 symbol
		str = (char *)realloc(str, size);	//#!could use fixed size buffers for perfomance
		str[size - 2] = *src++;
		str[size - 1] = '\0';
	}
	while (!((*(src - 1) == END) && (*(src - 2) == END)));	//Tbl END symbol, DEFINEd in the beginning

	data->tStringAdd(str, strlen(str));
	free(str);
}

EXPORT long int __stdcall GetEncodeSize(tPluginData * data)
{

	char *srcStr = NULL;
	int i, srcPos = 0;
	long int strSize = 0;
	unsigned int hdr[3];	//3 header bytes
	int lineLength = 0;	//for deciding even/odd out chunk length

	srcStr = data->tStringGet(0, &strSize);
	sscanf(srcStr, "[%02X%02X%02X]", &hdr[0], &hdr[1], &hdr[2]);
	srcPos = 8;
	pushOut(&outBuffer, (u8) hdr[0]);
	pushOut(&outBuffer, (u8) hdr[1]);
	pushOut(&outBuffer, (u8) hdr[2]);
	lineLength += 3;

	for (i = srcPos; i < strSize; i++) {
		if (srcStr[i] == '\xFE')	//allign not last line
		{
			if ((lineLength & 1) == 0) {
				pushOut(&outBuffer, 0x7F);	//2's allign
			}
			lineLength = 0;
		} else if ((srcStr[i] == '\xFF') && (srcStr[i + 1] == '\xFF'))	//align last line
		{
			if (((lineLength + 1) & 1) == 0) {
				pushOut(&outBuffer, 0x7F);	//2's allign
			}
			lineLength = 0;
		} else {

			lineLength++;
		}
		pushOut(&outBuffer, srcStr[i]);
	}
	return outBuffer.size;
}

EXPORT void __stdcall Encode(tPluginData * data, void *buffer)
{
//main buffer was already filled at GetEncodeSize. Just copy from global buffer to Kruptar's allocated buffer so Kruptar could free it.

	memcpy(buffer, outBuffer.mem, outBuffer.size);
	destroyOut(&outBuffer);
}
