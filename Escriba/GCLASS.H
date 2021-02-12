// UNIX Regular Expression matching (as C++ object class)

#ifndef GCLASS_H
#define GCLASS_H

//  Limitations
#define MAX_STORE             1024
#define MAX_SUB_EXPRESSIONS     10

class cGREP {

 public:

   // query this element to learn if your regular expression
   // was compiled ok.
   int an_error_has_occured;

   // if the cGREP instance did not like something,
   // you can read all about it here.
   char error_string[255];

 public:

   // The single constructor requires a UNIX regular
   // expression.  It is ok to pass a constant.
   cGREP ( char *exp );

   // This method returns 1 if the regular expression
   // was matched.  Otherwise it returns 0.
   int is_reg_exp_present ( char *string );

   // This method returns the specified matched string.
   int matched_string ( char *buffer, int buffersize ) ;

   // Text String dereference.
   virtual char TextStringDereference ( char *textstring ) {
      return ( *textstring ) ;
   } /* endmethod */

   // Slightly different 'strchr'.
   virtual char *TextStringChar ( char *textstring, int character ) {
      return ( strchr ( textstring, character ) ) ;
   } /* endmethod */

   // Slightly different 'strncmp'.
   virtual int TextStringStringNCompare ( char *textstring, char *realstring, int maxlength ) {
      return ( strncmp ( textstring, realstring, maxlength ) ) ;
   } /* endmethod */

   //----------------------------------------------------------
   //------------  THE PRIVATE INTERFACE BEGINS HERE  ---------
   //----------------------------------------------------------
 protected:
   char *reg_exp_match_begins_here[MAX_SUB_EXPRESSIONS];
   char *reg_exp_match_ends_here[MAX_SUB_EXPRESSIONS];
 private:
   char first_char_of_regexp;
   char is_anchored;
   char *required_substring;
   int length_required_substring;
   char symbol_string[MAX_STORE];
   char *Input_scan_pointer;
   int parentheses_count;
   char reg_exp_shadow;
   char *code_emit_pointer;
   long symbol_string_size;
   char *input_string_pointer;
   char *beginning_of_input;
   char **pointer_to_rembh;
   char **pointer_to_remeh;
   char *Special_chars;
   //----------------------------------------------------------
   void record_error ( int n );
   char *parse_parens ( int paren, int *flagp );
   char *one_side_of_or_operator ( int *flagp );
   char *trailing_wild ( int *flagp );
   char *compress_ordinary_characters ( int *flagp );
   char *emit_node ( char op );
   void emit_next_byte ( char b );
   void insert_operator ( char op, char *opnd );
   void go_to_end_of_chain ( char *p, char *val );
   void go_to_end_of_chain_on_operand ( char *p, char *val );
   int lookup_engine ( char *prog );
   int wildcard_lookup ( char *p );
   char *get_next_pointer ( char *p );
   int evaluate_next ( char *string );
};

#include "Text.h"

class cGREP2 : public cGREP {

 private:
   Text *pText ;

 public:
   cGREP2 ( char *exp, Text *ptext ) : cGREP(exp), pText(ptext) {
   } /* endmethod */

   // Return starting and ending offsets of matched string.
   void matched_string ( long &Start, long &End ) {
      Start = (long) reg_exp_match_begins_here[0] ;
      End = (long) reg_exp_match_ends_here[0] ;
   } /* endmethod */

   // Text String dereference.
   virtual char TextStringDereference ( char *textstring ) {
      long Offset = (long) textstring ;
      PUCHAR pByte ;
      if ( !pText->QueryBytePtr ( Offset, pByte ) ) 
         return ( 0 ) ;
      return ( (char) *pByte ) ;
   } /* endmethod */

   // Slightly different 'strchr'.
   virtual char *TextStringChar ( char *textstring, int character ) {
      long Offset = (long) textstring ;
      while ( 1 ) {
         PUCHAR pByte ;
         if ( !pText->QueryBytePtr ( Offset, pByte ) ) 
            break ;
         if ( *pByte == character ) 
            return ( (char*) Offset ) ;
         Offset ++ ;
      } /* endwhile */
      return ( 0 ) ; 
   } /* endmethod */

   // Slightly different 'strncmp'.
   virtual int TextStringStringNCompare ( char *textstring, char *realstring, int maxlength ) {
      long Offset = (long) textstring ;
      for ( int i=0; i<maxlength; i++ ) {
         PUCHAR pByte ;
         if ( !pText->QueryBytePtr ( Offset+i, pByte ) ) 
            return ( -1 ) ;
         if ( *pByte - (UCHAR)realstring[i] ) 
            return ( *pByte - (UCHAR)realstring[i] ) ;
      } /* endfor */
      return ( 0 ) ;
   } /* endmethod */

} ;

#endif // GCLASS_H defined

