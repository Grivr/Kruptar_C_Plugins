
typedef struct __attribute__((__packed__)){
    void *rom;
	long int romSize;
	long int param;
	long int charSize;
	
	int (__stdcall *tStringAdd)(char *str, long int size);
	void (__stdcall *tStringSet)(int index, char *str, long int size);
	char* (__stdcall *tStringGet)(int index, long int* size);
	long int (__stdcall *tStringsCount)();
	
	long int originalStringSize;
   
}tPluginData;



#define EXPORT __declspec(dllexport) 
#define IMPORT __declspec(dllimport) 


