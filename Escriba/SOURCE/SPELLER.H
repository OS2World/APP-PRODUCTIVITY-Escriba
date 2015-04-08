/****************************************************************** SPELLER.H
 *                                                                          *
 *                         Spell Checker Functions                          *
 *                                                                          *
 ****************************************************************************/

#ifndef SPELLER_H
#define SPELLER_H

#include <stdlib.h>

#include "System.h"

#include "Addons.h"
#include "Event.h"
#include "ISpeller.h"
#include "Module.h"

class Dictionary : public Addon {

   private:
      int Handle ;
      Event DictionaryReady ;

      void  (APIENTRY *Speller_SetHome)           ( char *Path ) ;
      char* (APIENTRY *Speller_DefaultDictionary) ( void ) ;
      int   (APIENTRY *Speller_OpenDictionary)    ( char *Path ) ;
      void  (APIENTRY *Speller_CloseDictionary)   ( ) ;
      BOOL  (APIENTRY *Speller_Ready)             ( ) ;
      int   (APIENTRY *Speller_BuildWord)         ( char* (APIENTRY*GetPtr)(void *Object,long *Offset,long Delta), void *Object, char *Word, long *StartOffset, long *EndOffset ) ;
      int   (APIENTRY *Speller_FindWord)          ( char *Word ) ;
      int   (APIENTRY *Speller_FindReplacements)  ( char *Word, int ListSize, int EntrySize, void *List, int *EasyCount ) ;
      int   (APIENTRY *Speller_Accept)            ( char *Word ) ;
      int   (APIENTRY *Speller_Insert)            ( char *Word ) ;
      int   (APIENTRY *Speller_UnCaps)            ( char *Word ) ;

   public:

      Dictionary ( char *ModulePath, char *DefaultLanguage ) ;

      ~Dictionary ( ) ;

      BOOL OpenDictionary ( ) ;
      void CloseDictionary ( ) ;

      BOOL Available ( ) { return ( QueryLibrary() != 0 ) ; }
      BOOL Ready ( ) ;

      BOOL BuildWord ( char* (APIENTRY*GetPtr)(void *Object,long *Offset,long Delta), void *Object, char Word[], long &StartOffset, long &EndOffset ) ;
      BOOL FindWord ( char *Word ) ;
      int  FindReplacements ( char *Word, int ListSize, int EntrySize, void *List, int *EasyCount ) ;
      BOOL Accept ( char *Word ) ;
      BOOL Insert ( char *Word ) ;
      BOOL UnCaps ( char *Word ) ;

   friend void DictionaryOpenThread ( void *Parameter ) ;
} ;

#include "Document.h"

extern void SpellChecker ( HWND Owner, Documento *pDocument, Dictionary *pDictionary ) ;

#endif
