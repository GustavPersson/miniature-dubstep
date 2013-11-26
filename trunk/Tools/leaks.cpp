//Stefan Petersson, 2008
//Demo, hitta minneläckor med ytterliggare information

#include <iostream>
using namespace std;

/*
	För att identifiera om minnesläckor räcker det att, först i main(),
	anropa funktionen _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF).
	Det är sällan det räcker att göra så, i större system vill vi även få reda på
	vilken fil och rad som har orsakat minnesläckan.

	Detta demo visar hur ytterliggare information (fil och rad) kan fås. Minnesläckorna presenteras
	endast(!) vid normal körning i DEBUG-läge. Release-läge och/eller Ctrl+F5 får inte användas.
	Informationen om de ev. läckorna kommer att synas i Visual Studios "Output"-fönster.

	Vill ni göra det enklare för studenterna räcker det att ni endast definierar
	myNew, myDelete samt myInitMemoryCheck(). Exempel:

	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>

	#ifdef _DEBUG // identifiera endast minnesläckor i debug
	  #define myNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
	  #define myDelete delete 

	  // ställ in så att minnesläckorna dumpas efter programavslut
	  #define myInitMemoryCheck() \
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)

	#else // vid release-build kör vi med standard new och delete
	  #define myNew new
	  #define myDelete delete
	  #define myInitMemoryCheck()
	#endif 
*/


//////////////////SEKTIONEN NEDAN KAN MED FÖRDEL PLACERAS I SEPARAT .H-FIL//////////////////////
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG // identifiera endast minnesläckor i debug
  #define myMalloc(s)       _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myCalloc(c, s)    _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myRealloc(p, s)   _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myExpand(p, s)    _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myFree(p)         _free_dbg(p, _NORMAL_BLOCK)
  #define myMemSize(p)      _msize_dbg(p, _NORMAL_BLOCK)
  #define myNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
  #define myDelete delete 
 
  // ställ in så att minnesläckorna dumpas efter programavslut
  #define myInitMemoryCheck() \
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)

#else // vid release-build kör vi med standard new och delete
  #define myMalloc malloc
  #define myCalloc calloc
  #define myRealloc realloc
  #define myExpand _expand
  #define myFree free
  #define myMemSize _msize
  #define myNew new
  #define myDelete delete
  #define myInitMemoryCheck()
#endif 
/////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	myInitMemoryCheck();
	int* i = myNew int; // minnesläcka med tillhörande fil och radnummer presenteras i output

	int* j = new int; // minnesläckan presenteras men ingen information om fil och radnummer, använd myNew istället
}