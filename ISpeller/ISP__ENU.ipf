#define LOCAL_DEFINES
#include "ISpeller.h"
:userdoc.
:title.ISpeller Spell-Checker Help
:docprof toc=123456.

:h1 res=IDD_PRODUCTINFO.Product Information
This dialog displays the name, revision, date and copyright information
for this module.  Press ENTER or click on the OK button to dismiss the
dialog box.

:h2 res=IDD_CONFIGURE.Spell Check Setup (Dialog)
From this dialog you may choose the dictionary file to be used by the
spell checker, build new dictionaries and remove old dictionaries.
:p.The currently available dictionary files are listed in the dialog.
To select one for use, click on the dictionary name and press ENTER,
or simply double-click on the name.  If for some reason it is not
possible to open the dictionary file, you will be told.  Otherwise,
this dialog exits and you are returned to the word processor.
:p.To build a new dictionary file, click on the Build pushbutton.
The :link reftype=hd res=IDD_BUILD.Spell Check Dictionary Build:elink.
dialog will be displayed, from which you may build a new dictionary.
:p.To remove a dictionary file from the system, click on the dictionary
file&apos.s name, and then on the Remove button.

:h3 res=IDD_BUILD.Spell Check Dictionary Build (Dialog)
From this dialog it is possible to build a new dictionary file for use 
with the spell checker.
:p.To build a spell checker dictionary file, you need to provide two input
files&colon. the word list and the language-dependent &apos.affix&apos. file.
:p.The word list file can be a simple list of words, one per line in text
format, or it can be in what is referred to as the ISpell &apos.munched&apos.
word list format, which marks words a special way to indicate ways they
might be altered with prefixes and suffixes.  There are no particular rules
for how this kind of file might be named, although normally the word&apos.s
language normally forms part of the file name.
:p.The affix file contains the rules by which words may be modified, and
is unique for each language.  The standard extension for affix files
is &apos..aff&apos..
:p.To build a new dictionary, you must specify the name of an existing
word list file, and of an existing &apos.affix&apos. file, and the name of the
dictionary you wish to create.  Once these names are entered, you may
click on the &apos.OK&apos. button, at which time the file names are verified and
the dictionary building process begins.  This may take a moment or two,
after which time you are returned to the 
:link reftype=hd res=IDD_CONFIGURE.Spell Check Setup:elink. dialog, from which
you may then select the new dictionary for use.
If there was a problem in building the dictionary, you will be told so and
allowed to take whatever corrective action is necessary (changing file names)
before attempting to build the dictionary again.
:p.To exit this dialog without building a dictionary, press the ESCAPE key
or click on the Escape button.

:euserdoc.
