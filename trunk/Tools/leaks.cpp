//Stefan Petersson, 2008
//Demo, hitta minnel�ckor med ytterliggare information

#include <iostream>
using namespace std;

/*
	F�r att identifiera om minnesl�ckor r�cker det att, f�rst i main(),
	anropa funktionen _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF).
	Det �r s�llan det r�cker att g�ra s�, i st�rre system vill vi �ven f� reda p�
	vilken fil och rad som har orsakat minnesl�ckan.

	Detta demo visar hur ytterliggare information (fil och rad) kan f�s. Minnesl�ckorna presenteras
	endast(!) vid normal k�rning i DEBUG-l�ge. Release-l�ge och/eller Ctrl+F5 f�r inte anv�ndas.
	Informationen om de ev. l�ckorna kommer att synas i Visual Studios "Output"-f�nster.

	Vill ni g�ra det enklare f�r studenterna r�cker det att ni endast definierar
	myNew, myDelete samt myInitMemoryCheck(). Exempel:

	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>

	#ifdef _DEBUG // identifiera endast minnesl�ckor i debug
	  #define myNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
	  #define myDelete delete 

	  // st�ll in s� att minnesl�ckorna dumpas efter programavslut
	  #define myInitMemoryCheck() \
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)

	#else // vid release-build k�r vi med standard new och delete
	  #define myNew new
	  #define myDelete delete
	  #define myInitMemoryCheck()
	#endif 
*/


//////////////////SEKTIONEN NEDAN KAN MED F�RDEL PLACERAS I SEPARAT .H-FIL//////////////////////
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG // identifiera endast minnesl�ckor i debug
  #define myMalloc(s)       _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myCalloc(c, s)    _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myRealloc(p, s)   _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myExpand(p, s)    _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myFree(p)         _free_dbg(p, _NORMAL_BLOCK)
  #define myMemSize(p)      _msize_dbg(p, _NORMAL_BLOCK)
  #define myNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
  #define myDelete delete 
 
  // st�ll in s� att minnesl�ckorna dumpas efter programavslut
  #define myInitMemoryCheck() \
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)

#else // vid release-build k�r vi med standard new och delete
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
	int* i = myNew int; // minnesl�cka med tillh�rande fil och radnummer presenteras i output

	int* j = new int; // minnesl�ckan presenteras men ingen information om fil och radnummer, anv�nd myNew ist�llet
}