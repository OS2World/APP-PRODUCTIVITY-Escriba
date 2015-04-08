#include "..\Escriba.h"
#ifdef APIENTRY
#undef APIENTRY
#endif
:userdoc.
:title.Escriba Word Processor Help
:docprof toc=123456.

:h1 res=ID_MAIN.Introduction
:i1.Introduction
:artwork name='Escriba.bmp' align=center.
:p.
This program provides a What You See Is What You Get (WYSIWYG) word
processor for the OS/2 Presentation Manager interface.  It does not
attempt to go head-to-head with the large professional word processors
available for OS/2 PM or Windows, but rather tries to provide a more
modest set of functions.  Normal text formatting is provided for, plus
the program has the ability to handle embedded graphics.  All installed
ATM fonts are supported, in all the native formats and in a wide range
of sizes.  Printer support is strictly through the OS/2 PM interface,
providing output on any printer supported by that interface, including
FAX devices and plotters.
On Oriental versions of OS/2, double-byte character set input and output
is supported, as are True-Type fonts installed via the Font Palette.
:p.
:sl compact.
:li.&sqbul.:link reftype=hd res=60001.Startup and Shutdown:elink.
:li.&sqbul.:link reftype=hd res=60002.Loading and Saving Documents:elink.
:li.&sqbul.:link reftype=hd res=60003.Editing Documents:elink.
:li.&sqbul.:link reftype=hd res=60004.Printing Documents:elink.
:li.&sqbul.:link reftype=hd res=60005.Window Management:elink.
:li.&sqbul.:link reftype=hd res=60006.A Few Cautions:elink.
:li.&sqbul.:link reftype=hd res=60007.Registration:elink.
:esl.

:h2 res=60001.Startup and Shutdown
:hp4.Startup:ehp4.
:p.
Once Escriba has been installed, there are three ways to start it up.
The first method is to simply double-click on the program object that was
created during installation.  In this case, the word processor will
start up and create an empty document window for immediate use.
:p.
The second method is to drag a document file over to the program object
and drop it there.  The document file will be opened and displayed in
the work area, ready for editing.
:p.
The third method is with the START command.  Normally, this will only work
if your current directory is the directory to which you installed Escriba.
If you include that directory in the PATH, HELP and LIBPATH statements
of your CONFIG.SYS file, then you can use the START command from anywhere
on your system.
:p.
There is an indirect way to start Escriba.  When Escriba saves a file in
its native format, it marks the file as being of type &apos.Escriba&apos..  This type
is associated through the file system with the Escriba word processor, and
when the file&apos.s object is opened, the default action becomes to start
Escriba to display and edit the file.  The Escriba word processor is also
associated with Plain Text and Pluma files, and appears in the Open menu of
such file&apos.s context menus.

:p.
:hp4.Shutdown:ehp4.
:p.
To exit Escriba, you can double-click on the program&apos.s system menu (the 
mini-icon to the extreme upper-left of the program window), or you can
press the F3 key.  If you have any documents open that have been changed
but have not yet been saved, you will be asked if you wish to save them
first before exiting.  The exit process can be aborted at any of these
confirmation messages.

:h2 res=60002.Loading and Saving Documents
:hp4.Loading:ehp4.
:p.
Documents can be loaded into the word processor at startup or later.
By dropping one or more document file objects on the word processor object
you start a copy of the word processor which will hold all of the requested
documents in separate windows.  Also, you may execute the word processor
directly from the command line, giving the names of the document files
on the command-line.  If you attempt to do either of these operations
after a copy of the word processor has been loaded, that copy will be
directed to open and display the new documents as well.  Only one copy
of the word processor is kept in system memory at a time.
:p.
Once the word processor has been started, it is now possible to select files
for loading with the :link reftype=hd res=IDM_OPEN.Open:elink. menu option.
You are presented with the system&apos.s standard file selection dialog.
It is preset to display all recognizable document files, based on the file
types and extensions, but you may select &apos.All Files&apos. and have
all files displayed.  You may select as many files as you like for loading,
and after you finish the list, all will be loaded in the background.
At the start of the load, all the currently open document windows are
iconized, and the first document to complete loading becomes the current
document.  All the others will be automatically iconized as they complete
loading.
:p.
After you submit the file list for opening, the word processor will determine
each file&apos.s type.  If a file&apos.s contents appear valid for more than
one file type, you will be asked to select which type it is.  If the file contents
cannot be validated for any file type, you will be informed of this and the
rejected document&apos.s load will be aborted.
:p.
:hp4.Saving:ehp4.
:p.
There are three ways to save a document to disk&colon.
:link reftype=hd res=IDM_SAVE.Save:elink.,
:link reftype=hd res=IDM_SAVEAS.Save As...:elink. and
:link reftype=hd res=IDM_SAVE_CLOSE.Save and Close:elink..
The Save command attempts to save the file with its current name
and type.  If it cannot, you will be directed to change the name
and/or the file type in order to save the file.
The Save As command assumes you want to change the file name or
type and allows you to enter the new name and select the new type.
The Save and Close command works the same as the Save command,
except that the document window is closed after the save completes.
:p.
If you try to close a document window that has been changed and not
saved, you will be asked if you want to save the document first.
Likewise, if you try to exit the word processor while you still
have open, changed documents, you will be asked if you want to save
the documents before exiting.  You will be asked about each open
and changed document.

:h2 res=60003.Editing Documents
:hp4.Navigation:ehp4.
:p.
For movement within the document window, the following keys
have been defined&colon.
:table cols='20 60' rules=both frame=box.
:row.:c.Left Arrow              :c.Move to previous character/token.
:row.:c.Right Arrow             :c.Move to next character/token.
:row.:c.Up Arrow                :c.Move to previous line.
:row.:c.Down Arrow              :c.Move to next line.
:row.:c.Page Up                 :c.Move to previous page.
:row.:c.Page Down               :c.Move to next page.
:row.:c.Home                    :c.Move to start of line.
:row.:c.End                     :c.Move to end of line.
:row.:c.Ctrl + LeftArrow        :c.Move to previous word.
:row.:c.Ctrl + RightArrow       :c.Move to next word.
:row.:c.Ctrl + Page Up          :c.Move to top of current page.
:row.:c.Ctrl + Page Down        :c.Move to bottom of current page.
:row.:c.Ctrl + Home             :c.Move to start of document.
:row.:c.Ctrl + End              :c.Move to end of document.
:etable.

In addition, you may use the mouse to directly position
the cursor anywhere in the visible document.  You may use
the mouse with the scroll-bars, providing you have them
set visible, to reposition the page view horizontally
or vertically.

:p.Also, four special mouse buttons have been provided at the
lower-right of the document window.  These buttons provide
the Top, Page Up, Page Down and Bottom functions, the same
as those provided by the Ctrl+Home, PageUp, PageDown and
Ctrl+End keys.

:p.As you move about the document, remember that function codes
to set margins, fonts, sizes, etc. are considered as full
tokens in the data stream and the cursor will stop at each
of them as you use the left/right arrows to go back and forth
through the document.

:note.There is a special difference in how the Home and End keys
operate when compared to industry standards.  
If you press the Home key when you are already at the start of 
the current line, you will be moved to the start of the 
previous line.
Likewise, when you press the End key when you are already at the
end of the current line, you will be moved to the end of the
next line.

:p.:hp4.Text:ehp4.

:p.Text may be entered into the document by simply typing it in.
As you enter text, the text is formatted in place.  If a word
grows too long for the current line to hold it, it is moved
down to the next line.  If you type too fast, your keystrokes
are accumulated and inserted as groups to catch up with you.

:p.Once you have entered text into the document, you may move
the cursor back into the text to change it.  Normally, your
cursor is set for insertion, and has a vertical bar as the
cursor.  When you start typing into the middle of already
existing text, the new text is inserted into the old, and the
document is reformatted to suit.  If you wish to replace text
instead, then press the INSERT key.  Your cursor will change
to a block covering the current character/token, and whatever
you type will replace what is under the cursor.  Be careful
with this.  If the cursor is placed over a font or size change
code, or other type of special function code, whatever you type
will replace it, just as regular text would be replaced.

:p.Text can be selected by positioning the cursor to the start
of the block you wish to select, then pressing down the SHIFT
key and holding it down, and then repositioning the cursor
to the end of the block.  Another way is to position the cursor
with the mouse, then, holding the left mouse button down, moving
to the end of the block to be selected.  A single word or graphic
object may be selected by double-clicking the left mouse button
over it, and the entire document can be selected by double-clicking
on the blank upper-left corner of the document window, between the
horizontal and vertical text rulers.

:p.Once you have selected a block of text, that block can be moved or
copied, cut from the document, copied to the system clipboard, or
replaced with whatever is on the system clipboard.  Also, if you
have text selected, and you start typing, the selected block is
deleted and replaced with the new input.

:p.Selected text can be moved or copied with the mouse.  You must
click the right mouse button on the selected area, then drag the
mouse until the cursor is positioned at the text block&apos.s new
position.  When you release the mouse button, the move will take
place.  If you want to copy instead of move, hold down the Control
key as you release the mouse button.  If you do not move the
mouse, but simply release the mouse button, then a context menu
will appear containing all the available commands which can be
used on a selected block of text.

:p.:hp4.Text Attributes:ehp4.

:p.At any point in the document, there is a specific set of text
style and formatting attributes in effect.  The margins are set
to a particular position, the tabs are at a certain location, the
font is set a certain way.  Any of these attributes can be changed
from the current location onwards, or over a selected area.
If you set the font to Times Roman, and no text was selected at the
time, then the new font will take effect from the cursor location
onwards, until another font command is encountered in the text.
If text was selected, however, all other font commands will be
removed from the selected area and Times Roman will take effect
over the entire selected area.  The font that was in effect after
the selected area remains in effect there.  All text attributes
are handled in this way, be they margins, tabs, indentation,
font, font size, font style, colors . . . all of them.
:p.
:hp4.Graphic Images:ehp4.
:p.
Graphics images may be placed into a document from the system
clipboard, from any portion of the visible desktop, or from
graphics files.  Use the :link reftype=hd res=IDM_PASTE_FROM.Paste From:elink.
menu to do this.
:p.
Once the graphic is placed into the document, you can move it,
copy it or resize it at any time.  To move the graphic, click
the right mouse button on the body of the graphic and drag the
graphic to its new position.  To copy it, do the same, except
that while you release the right mouse button, hold down the
Control key.  To resize a graphic, click the right mouse button
on the white space immediately next to the graphic.  A sizing
border will appear and guide you as you adjust the graphic&apos.s size.

:h2 res=60004.Printing Documents
There are two document printing options currently available:
:link reftype=hd res=IDM_PRINT_ALL.Print All:elink. and
:link reftype=hd res=IDM_PRINT_PAGE.Print Page:elink..
:p.
Print All causes the currently active document to be printed entirely.
You can also do this by pressing the Control key and the &apos.P&apos. key.
:p.
Print Page prints the current page of the currently active document.
This can also be done by press the Alt key and the &apos.P&apos. key together.

:h2 res=60005.Window Management
You can have many document windows open at a time.
Each time you open a new document window, all the currently
open document windows are iconized and arranged across the
bottom of the work area.  The new document window will then
occupy as much space as it can, leaving enough space so that
the iconized documents are all visible.
:p.
To restore a document window to normal, simply double-click
on it as you would any system icon.  The window will return
to whatever size and location it had before it was iconized.
:p.
If you restore a number of documents to normal, they will
doubtlessly obscure parts of each other.  There are two
menu commands available to straighten things out:
:link reftype=hd res=IDM_TILE.Tile:elink. and
:link reftype=hd res=IDM_CASCADE.Cascade:elink..
Tiling the windows will arrange all the document windows
not currently iconized so that none obscures any other.
The effect is the same as when the system tiles windows
on the desktop.
Cascading the windows will arrange all the document windows
not currently iconized so that they form a stack, like a deck
of cards.  They are arranged so that their title-bars are
all visible.  The currently active document window gets
placed on top of the stack.
:p.
One additional window arrangement tool is provided:
:link reftype=hd res=IDM_ARRANGEICONS.Arrange Icons:elink.
This menu command will neatly arrange all the iconized
document windows across the bottom of the work area.
:p.
Just in case a document window is completely obscured, the
:link reftype=hd res=IDM_WINDOW_MENU.Windows:elink. menu
contains a list of all the currently open documents.
You may select any of the windows there and make it the
active window.  If necessary, the document window will be
restored from the minimized state.

:h2 res=60006.A Few Cautions
&sqbul.Having a lot of fonts on the Font menu looks neat, but OS/2 does not
release the fonts from memory until you shut down and reboot the system.
This can cause your swap file to grow by several megabytes.
:p.
&sqbul.Some ATM fonts available on electronic bulletin boards are
not completely verified and can cause your system to hang.
Be careful with newly installed third-party ATM fonts, though
you shouldn&apos.t have to worry about fonts purchased from IBM
or Adobe.
:p.
&sqbul.Graphics embedded into documents can be quite large.
When you imbed a large graphic into the document, but shrink
it on the document page, do not be fooled into thinking that
the graphic takes any less space.  It is still the same graphic
as before.

:h2 res=60007.Registration
This program is shareware.
Feel free to distribute unmodified copies of the program to anyone interested.
If you find the program useful, and decide to keep the program for use, then you must pay for it.
The price is US$30.00, and there are several ways to pay it&colon.
:p.
:hp4.Check or Money Order&colon.:ehp4.
If you wish to pay with a personal check
or money order, please send your payment, together with your name, your
mailing address, the revision number and date of your current copy of
Escriba, to&colon.
:lm margin=6.:hp4.:sl compact.
:li.Escriba Registrations
:li.c/o Rick Papo
:li.38290 Avondale
:li.Westland, MI 48186-3830
:li.USA
:esl.:ehp4.:lm margin=1.
.br
:hp4.VISA/MasterCard/Discover&colon.:ehp4.
If you wish to use a credit-card to
pay for your registration, the company BMT Micro is providing this
service for Escriba.  They may be reached by telephone at (800) 414-4268
(USA only) or at (910) 791-7052, by FAX at (910) 350-2937, or through
their BBS at (910) 350-8061.  The voice lines are open from 8am to 8pm
(EST,-5 GMT).  The other numbers function 24 hours per day, 7 days per
week.  In addition, their mailing address is&colon.
:lm margin=6.:hp4.:sl compact.
:li.BMT Micro
:li.452 Horn Rd
:li.Wilmington, NC 28412-2411
:li.USA
:esl.:ehp4.:lm margin=1.
:note.If you reside in any of the republics which were formerly of the
Soviet Union or of the Warsaw Pact, you may register for $5 (no credit
cards, please).
:p.Upon receiving your registration, you will be sent a receipt and the
latest revision of the word processor on diskette.  You will be sent one
more copy on diskette later, when the next major revision is released.
Printed documentation is not yet available, but once it is, all registered
users of the word processor will receive a copy.  If you would like
later upgrades on diskette, they will cost $5 U.S. dollars.

:h2 res=60008.Questions and Bug Reporting
Should you have problems with this program, the author Rick Papo can be
reached by one of the following means&colon.
:table cols='15 60' rules=both frame=box.
:row.:c.Internet        :c.:hp4.rpapo@msen.com:ehp4.
:row.:c.Mail            :c.:hp4.Rick Papo, 38290 Avondale, Westland, MI 48186-3830, USA:ehp4.
:row.:c.Telephone       :c.:hp4.USA (313) 326-8547:ehp4.
:etable.
:p.
First priority is given to queries and problems of registered users
(see :link reftype=hd res=60007.Registration:elink.), but an attempt is
made to deal with all messages that arrive.  Electronic mail
is checked every day.
:p.
In all correspondence, please provide the revision number of Escriba
that you are using and referring to.

:h1 res=IDM_FILE_MENU.File Menu
:i1.File Menu
From the File Menu the following commands are available&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_NEW.New:elink.
:li.&sqbul.:link reftype=hd res=IDM_OPEN.Open...:elink.
:li.&sqbul.:link reftype=hd res=IDM_RECALL.Recall:elink.
:li.&sqbul.:link reftype=hd res=IDM_MERGE.Merge...:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVE.Save:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVEAS.Save As...:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVE_CLOSE.Save and Close:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLOSE.Close:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT.Print Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_PREFERENCES.General Preferences...:elink.
:li.&sqbul.:link reftype=hd res=IDM_DEFAULTS.Document Defaults...:elink.
:li.&sqbul.:link reftype=hd res=IDM_CONFIGURE_EXTENSIONS.Configure Extensions...:elink.
:li.&sqbul.:link reftype=hd res=IDM_LANGUAGE_MENU.Language:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXIT.Exit:elink.
:esl.

:h2 res=IDM_NEW.New (Menu Option)
This menu option creates a new document window with nothing in it.
The document margins, font and other attributes are those that were
set the last time the Document Preferences dialog was used without
any active document windows present.
:p.
Any previously open windows are minimized to the bottom of the workspace
and the new window occupies as much of workspace as it can.

:h2 res=IDM_OPEN.Open... (Menu Option)
When this menu option is selected, the system standard File dialog is
presented.  You may select one or more documents to be opened.  After
you press ENTER, or click on the OK button, all the files you selected
will be opened.  A red loading status indicator will be shown in the
right portion of the main program title bar.  Each load, once complete,
will result in a separate document window being created.  The first
to complete will become the active document, and will be maximized within
the program&apos.s client area.  All further documents will be iconized.
They can be called forward from the Windows menu, or by double-clicking
on their icons with the mouse.
:p.The short-cut key for this function is Ctrl+O.
:p.See :link reftype=hd res=256.Open/Merge/SaveAs... (Dialog):elink. for more information.

:h3 res=256.Open/Merge/SaveAs... (Dialog)
You have selected the Open, Merge or SaveAs menu option, and as a result
you now have this dialog before you.  It is the standard File dialog
provided by the operating system, and follows the rules given in the
system documentation.
:artwork name='English\FileDlg.bmp' align=center.
:p.These special notes apply&colon.
:p.
&rarrow.When the dialog is initially displayed for Open or Merge, the 
&apos.Type of File&apos. field is set to &apos.All Document Files&apos..
All files that appear to be valid document files for Escriba&apos.s native
format or for any of the extended formats (Plain Text, BMP, GIF, JPG, PCX, TIF, etc)
are displayed in the list-box.  You can change this setting to &apos.<All Files>&apos. 
or to any of the specific file types listed.  
:p.
&rarrow.When the dialog is displayed for Save or Save As, the current 
type of the document being saved is selected.  If you wish to change the
file&apos.s type, select the type you wish to save it with.
:p.
&rarrow. When selecting files to open, you may select as many files from the file
list as you wish.
:p.
&rarrow. When selecting a file to merge, you may only select one file.
:p.
&rarrow. When doing a Save As, you are not allowed to select a file name from the
file list.  The list serves for reference only.  You must enter the name
of the file you wish to save into the entry field.
:p.
&rarrow. The directory selected from this dialog becomes the new current directory
for the program.  The next time you call up the Open, Merge or SaveAs
dialogs, they will start in this directory.

:note.Files are marked as Escriba documents as they are saved to the disk.
This marking is done via Extended Attributes, and as such the marking is lost
when you save the document to diskette or archive the document with a program
(like PKZIP) which does not handle EAs.  There are newer archiving programs
available which preserve this information.

:h4 res=IDD_FILETYPE.Select File Type (Dialog)
This dialog is presented when you request the load of a file and that
file is found to be acceptable as more than one file type.
You are told the file&apos.s name and the acceptable types, and you
must select the correct type from the list.  Once you have selected
the correct file type, click on the OK button or press ENTER.
To abort the file load, click on the Escape button or press the Escape key.

:h2 res=IDM_RECALL.Recall Menu
This menu option calls up a submenu wherein the ten most recently saved
documents are listed.
:p.See :link reftype=hd res=IDM_RECALL0.Recall Menu Entries:elink.
for more information.

:h3 res=IDM_RECALL0.Recall Menu Entries
This menu entry lists one of the ten most recently saved documents.
To reopen one of these documents as a new window, select it with the
arrow keys and press ENTER, or click on one of them with the mouse.

:h2 res=IDM_MERGE.Merge... (Menu Option)
When this menu option is selected, the system standard File dialog is
presented.  From this dialog you may select a single file to be merged
into the currently active document at the cursor position.
:p.See :link reftype=hd res=256.Open/Merge/SaveAs... (Dialog):elink. for more information.

:h2 res=IDM_SAVE.Save (Menu Option)
This command causes the currently active document to be saved to its
current file name.  If no changes have been made to the document,
this menu option is disabled.
:p.The short-cut key for this function is F2.

:h2 res=IDM_SAVEAS.Save As... (Menu Option)
When this menu option is selected, the system standard File dialog is
presented.  Using this dialog, you may enter the name the currently
active document will be saved under, and you may select the directory
to which it will be saved.
:p.See :link reftype=hd res=256.Open/Merge/SaveAs... (Dialog):elink. for more information.

:h2 res=IDM_SAVE_CLOSE.Save and Close (Menu Option)
This option provides a quick means to save and close
the current document.
:p.The short-cut key for this function is F4.

:h2 res=IDM_CLOSE.Close (Menu Option)
This command causes the currently active document to be closed.  If there are
unsaved changes in the document, you will be asked if you wish to save it.
If you click on the YES button, or press Y, the document will be
saved to its current file name.
If you click on the NO button, or press N, the document window will
close without having saved any changes.
If you click on the CANCEL button, or press the ESCAPE key, the close
command will be aborted and the document window will remain active.

:h2 res=IDM_PRINT.Print Menu
This submenu gives access to the various print commands.
The following commands are available&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL.Print All Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_PAGE.Print Page:elink.
:esl.

:h3 res=IDM_PRINT_ALL.Print All Menu
This submenu gives access to the possible ways to print an
entire document.  The following commands are available&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL_NORMAL.Print All Normal:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL_REVERSE.Print All Reverse:elink.
:esl.

:h4 res=IDM_PRINT_ALL_NORMAL.Print All Normal (Menu Option)
The currently active document is printed after you select this menu
option.  A red status window is displayed while the document is being
sent to the system spooler, but that is for your information only.  You
may return to working on the document immediately after submitting the
print command.  The document will be printed as it was the instant
you requested it printed.
:p.The short-cut key for this command is Ctrl+P.

:h4 res=IDM_PRINT_ALL_REVERSE.Print All Reverse (Menu Option)
The currently active document is printed after you select this menu
option, just as in :link reftype=hd res=IDM_PRINT_ALL_NORMAL.Print All Normal:elink.,
except that the pages are printed in reverse order.

:h3 res=IDM_PRINT_PAGE.Print Page (Menu Option)
The current page of the currently active document is printed after you select
this menu option.  A red status window is displayed while the document is
being sent to the system spooler, but that is for your information only.
You may return to working on the document immediately after submitting the
print command.  The document will be printed as it was the instant
you requested it printed.
:p.The short-cut key for this command is Alt+P.

:h2 res=IDM_PREFERENCES.General Preferences... (Menu Option)
:p.See :link reftype=hd res=IDD_PREFERENCES.General Preferences (Dialog):elink. for more information.

:h3 res=IDD_PREFERENCES.General Preferences (Dialog)
This dialog allows you to configure the appearance and behavior of Escriba in
ways which apply to the word processor itself, and not to the documents
it processes.
:artwork name='English\Prefer.bmp' align=center.
:p.The options currently available are&colon.
:p.:hp2.Animate:ehp2.
This option, if set, causes the word processor to animate its main window
open and close process, just as the system folders do.  As with the system
folders, however, animation must be enabled from the System object in the
System Setup folder.
:p.:hp2.Show Rulers:ehp2.
If this item is checked, then all document windows will have horizontal
and vertical rulers provided next to the document window.
:p.:hp2.Show Scroll Bars:ehp2.
If this item is checked, then all document windows will include
horizontal and vertical scroll bars for use in scrolling the document
text with the mouse.
:p.:hp2.Show Status Line:ehp2.
If this item is checked, then all document windows will include
a status line at the bottom, where the current cursor position and
an interpretation of the token at the cursor are given.
:p.:hp2.Show Complete Filenames:ehp2.
If this item is checked, then the complete file name of each document
will show in the document window&apos.s title-bar.  Otherwise, the drive
and directory portion of the document file name will not be shown.
:p.:hp2.Spool Pages Separately:ehp2.
If this item is checked, then upon printing a multi-page document the
pages will be sent to the system spooler as separate jobs.  Since print
jobs do not start printing until they are fully spooled, this option
allows multipage documents to start printing quicker.  On the other hand,
if your printer is shared over a network, the document pages could be
printed out of order or mixed in amongst other user&apos.s output.
:p.:hp2.Print Pages in Reverse:ehp2.
If this item is checked, then the normal method of printing multiple pages
will be to print the last page first.  This option is provided for the
convenience of users with printers such as the DeskJet, where pages are
stacked face up, and therefore in reverse order.
:p.:hp2.Reload Document at Start:ehp2.
If this item is checked, then the last document recorded in the Recall list
will be reloaded automatically at the time Escriba is started.
:p.:hp2.Initial Zoom:ehp2.
From this group of buttons you may select the initial document window
zoom.  When you open new document windows or load documents from disk,
the chosen zoom method will be used to display the window when it
is first displayed.  You may choose from any of the following
zoom types&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_100.Zoom 100&percent.:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLWIDTH.Zoom Full Width:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLHEIGHT.Zoom Full Height:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_MARGINS.Zoom Margins:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_ALL.Zoom All:elink.
:esl.
:p.
Once the settings are satisfactorily adjusted, press the ENTER key or
click on the OK button for the program to continue.  The changes will
take effect on all new documents created.
:p.
If you wish to abort the dialog and leave the current defaults as they
are, press the ESC key or click on the escape button.

:h2 res=IDM_DEFAULTS.Document Defaults... (Menu Option)
This command invokes a dialog from which it is possible to alter the
initial printer, form, font, point size, color, font attributes and
text justification (horizontal and vertical) for the document.
:p.See :link reftype=hd res=IDD_DEFAULTS.Document Defaults (Dialog):elink. for more information.

:h3 res=IDD_DEFAULTS.Document Defaults (Dialog)
This dialog allows you to change the initial attributes of documents.
The attributes apply to the currently active document, or, if you have
no documents open at the moment, the new attributes are saved for use
in creating new documents.
.br
The configuration options are grouped by category 
on the following pages&colon.
:sl compact.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_AUTHOR.Author:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_PRINTER.Printer:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_FONT.Font:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_LAYOUT1.Layout (1):elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_LAYOUT2.Layout (2):elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_NATIONAL.National:elink.
:esl.
:p.
Once the settings are satisfactorily adjusted,
click on the OK button for the program to continue.  If no documents
are currently open, or you have checked the Global Save box, then
the new settings will take effect for all new documents created from
this point on.  If there is a document window currently open, the
new defaults will take effect for that document as well.  No other
documents are affected.
:p.
If you click on the APPLY button, the new defaults settings are applied
to the currently active document at once, and become the new defaults.
:p.
If you wish to abort the dialog and leave the current defaults as they
are, click on the escape button.
Changes made by the APPLY button are not discarded.

:h4 res=IDD_DEFAULTS_PAGE_AUTHOR.Author Settings Page
:artwork name='English\Default0.bmp' align=center.
:p.:hp2.Author:ehp2.
This entryfield allows you to view and/or alter the author name and
title saved in the document header.  You are also shown the document
creation date.

:h4 res=IDD_DEFAULTS_PAGE_PRINTER.Printer Settings Page
:artwork name='English\Default1.bmp' align=center.
:p.:hp2.Printer:ehp2.
This item allows you to select which printer (if you have several) the
document will be printed to.  You may choose any printer currently
defined in the system.
:p.:hp2.Printer Settings:ehp2.
Clicking on this button will call up the current printer&apos.s job
settings dialog.  This dialog varies according to which type of
printer you have selected, and cannot be documented here.
Refer to the help information provided by the dialog.
After you return from the dialog, the currently selected form
and that form&apos.s size are updated in the area immediately below
the pushbutton.

:h4 res=IDD_DEFAULTS_PAGE_FONT.Font Settings Page
:artwork name='English\Default2.bmp' align=center.
:p.:hp2.Font:ehp2.
This will be the initial font for the document.  You may choose from
any outline font currently installed on your system.
:p.:hp2.Size:ehp2.
This will be the initial size used for the font.  You may choose any
of the sizes in the list provided.
:p.:hp2.Color:ehp2.
This will be the initial color used for the text in the document.
You may choose any of the colors in the list provided.
:p.:hp2.Bold:ehp2.
If this box is checked, the initial text will be made bold.
If the selected font has a special bold font available, it will be
used.  Otherwise, the system will construct a bold font for use.
:p.:hp2.Italic:ehp2.
If this box is checked, the initial text will be made italic.
If the selected font has a special italic font available, it will be
used.  Otherwise, the system will construct an italic font for use.
:p.:hp2.Underscore:ehp2.
If this box is checked, the initial text will be underscored.
:p.:hp2.Strikeout:ehp2.
If this box is checked, the initial text will be struck out.
:p.:hp2.Outline:ehp2.
If this box is checked, the initial text will be outlined, but not
filled in.
:p.:hp2.Caps:ehp2.
If this box is checked, the initial text will be capitalized.

:h4 res=IDD_DEFAULTS_PAGE_LAYOUT1.Layout Settings Page 1
:artwork name='English\Default3.bmp' align=center.
:p.:hp2.Justification:ehp2.
This defines the type of horizontal justification to be used.
The available types are Left, Center, Right and Full.
Left justification aligns all text to the left margin of the page.
Center justification centers all text between the left and right
margins, but does not attempt to alter the default spacing between
words.
Right justification aligns all text to the right margin of the page.
Full justification aligns text to both the left and right margins of
the page, altering the spacing between words appropriately.
:p.:hp2.Center Page:ehp2.
If this box is checked, the document will start out with vertical
centering of all pages.
:p.:hp2.Margins:ehp2.
These four entry fields are used to set the initial page margins,
inches or centimeters, according to your current setting.  The
margins cannot be set wider than the currently selected form&apos.s
clipped size.

:h4 res=IDD_DEFAULTS_PAGE_LAYOUT2.Layout Settings Page 2
:artwork name='English\Default4.bmp' align=center.
:p.:hp2.Tab Spacing:ehp2.
This entry field allows you to set the default tab spacing to be used
at the start of the document.  If you enter 0.500 here, then you will
initially have tabs placed every half inch or centimeter, starting at
the left margin and continuing to the right margin.
:p.:hp2.Indent:ehp2.
This entry field allows you to set the default paragraph indent to be
used from the start of the document.  If you were to enter 0.500 here,
then you will find the text of the first line of the document, and the
text following each end-of-line code in the document will be indented
one half-inch or centimeter from the left margin.
:p.:hp2.Line Spacing:ehp2.
This box contains the available default line spacing options.
Option Single sets normal 100&percent. vertical spacing.
Option 1 1/2 sets 150&percent. vertical spacing, and option Double sets 200&percent..

:h4 res=IDD_DEFAULTS_PAGE_NATIONAL.National Settings Page
:artwork name='English\Default5.bmp' align=center.
:p.:hp2.Code Page:ehp2.
With this control it is possible to set which code page, or national
language character mapping, is to be used for the document.
See :link reftype=hd res=61002.Code Pages:elink. for more information.
:p.:hp2.Metric:ehp2.
If this box is checked, the document will use metric units for all
measurements.  The horizontal and vertical rulers will use centimeters
as their units of measurement.
:p.:hp2.Date Format:ehp2.
This field shows the current default date format to be used with the
Current Date and Today&apos.s Date functions.
See :link reftype=hd res=IDD_DATEFMT.Date Format:elink. for more information.

:h2 res=IDM_CONFIGURE_EXTENSIONS.Configure Extensions... (Menu Option)
This command invokes a dialog from which it is possible to alter the
settings for any of the add-on extensions currently installed.
:p.See :link reftype=hd res=IDD_CONFIGURE_EXTENSIONS.Configure Extensions (Dialog):elink. 
for more information.

:h3 res=IDD_CONFIGURE_EXTENSIONS.Configure Extensions (Dialog)
This dialog allows you to change the settings for any of the currently
installed add-on extensions.
:artwork name='English\ConfigEx.bmp' align=center.

:h3 res=IDM_LANGUAGE_MENU.Language (Submenu)
This submenu allows you to select a new current language for all the application
menus and dialogs.
:p.See :link reftype=hd res=IDM_LANGUAGES.Language Menu Entry:elink.
for more information.

:h4 res=IDM_LANGUAGES.Language Menu Entry (Menu Option)
Any of the entries in the Language submenu may be used to select a new
language for the application menus and dialogs.  
The language names are in their native form (Spanish = Espa¤ol, for example).
After you select the new language, the entire application will be reformatted
to suit the new language.  Certain aspects of documents (like date formats)
are changed also.

:h2 res=IDM_EXIT.Exit (Menu Option)
Selecting this menu option will terminate the program.

:h1 res=IDM_EDIT_MENU.Edit Menu
:i1.Edit Menu
From the Edit Menu the following commands are available&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_UNDO.Undo:elink.
:li.&sqbul.:link reftype=hd res=IDM_REDO.Redo:elink.
:li.&sqbul.:link reftype=hd res=IDM_CUT.Cut:elink.
:li.&sqbul.:link reftype=hd res=IDM_COPY.Copy:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE.Paste:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEAR.Clear:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEARCLIPBOARD.Clear Clipboard:elink.
:li.&sqbul.:link reftype=hd res=IDM_SELECT_ALL.Select All:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM.Paste From Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_INSERT_FUNCTION.Insert Function Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_SEARCH.Search...:elink.
:li.&sqbul.:link reftype=hd res=IDM_FINDNEXT.Find Next:elink.
:li.&sqbul.:link reftype=hd res=IDM_GOTOPAGE.Go To Page:elink.
:li.&sqbul.:link reftype=hd res=IDM_SPELL.Spell Check...:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM.Zoom:elink.
:esl.

:h2 res=IDM_UNDO.Undo (Menu Option)
This function undoes the most recent change applied to the document.
If you continue to use this function, the next most recent change
gets undone as well, and so on until all changes made to the document
have been undone, and the document is as it was when it was loaded
from the disk.  On a new document, of course, the final state of all
this undoing will be the initial empty state.
:p.The short-cut key is defined to be Alt+Backspace.

:h2 res=IDM_REDO.Redo (Menu Option)
If you have not altered the document since the last time you undid
a change, this function will reinstate the change.  It is possible
to undo every change made to the document, the reinstate them all.
If, however, you alter the document in any way after an :link reftype=hd res=IDM_UNDO.Undo:elink., all
further change levels are discarded, and it is no longer possible
to redo them.
:p.The short-cut key is defined to be Shift+Alt+Backspace.

:h2 res=IDM_CUT.Cut (Menu Option)
This command causes the currently selected text to be removed
from the document and placed into the system clipboard.
:p.The short-cut key is defined to be Shift+Delete.

:h2 res=IDM_COPY.Copy (Menu Option)
This command causes the currently selected text to be copied
from the document and placed into the system clipboard.
:p.The short-cut key is defined to be Ctrl+Insert.

:h2 res=IDM_PASTE.Paste (Menu Option)
This command causes the contents of the system clipboard to either
replace the currently selected text (in replace mode) or to be inserted
following the cursor (in insert mode).

:h2 res=IDM_CLEAR.Clear (Menu Option)
This command causes the currently selected text to be cleared from the
document without altering the system clipboard.
This command is equivalent in every way to pressing the Delete key.

:h2 res=IDM_CLEARCLIPBOARD.Clear Clipboard (Menu Option)
This command clears the system clipboard, releasing all items stored there.

:h2 res=IDM_SELECT_ALL.Select All (Menu Option)
This command selects all the text of the current document, from start 
to finish.
:p.This command may also be invoked by double-clicking with mouse button one
on the upper-left corner area between the horizontal and vertical rulers.
:p.The short-cut key is defined to be Ctrl+A.

:h2 res=IDM_PASTE_FROM.Paste From Menu
From this menu it is possible to select the general source of image
data to be pasted into the document.
The following options are available&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP.Paste from System Clipboard:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN.Paste from Display Screen:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_FILE.Paste from Graphic File:elink.
:esl.

:h3 res=IDM_PASTE_FROM_CLIP.Paste From Clipboard Menu
From this menu it is possible to select a Paste option which takes
graphic image data from the system clipboard.
The following options are available&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Paste Bitmap from Clipboard:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP_METAFILE.Paste Metafile from Clipboard:elink.
:esl.

:h4 res=IDM_PASTE_FROM_CLIP_BITMAP.Paste Bitmap from Clipboard (Menu Option)
If a bitmapped image has been placed on the system clipboard, this menu
option enables you to paste that image into your document.
After you have selected this option, you must point to one of the corners
of the area where you want the image pasted.  Select that corner by moving
the mouse pointer there, then pressing the left mouse button.  Keeping the
mouse button pressed, move to the opposite corner of the area where
the image will be placed.  A rectangle will form as you move the mouse.
Once you are satisfied with the size and position of the rectangle,
release the mouse button.  The image will be pasted into the rectangle,
and will be stretched or compressed as necessary in both height and width.

:h4 res=IDM_PASTE_FROM_CLIP_METAFILE.Paste Metafile from Clipboard (Menu Option)
If a metafile graphic has been placed on the system clipboard, this menu
option enables you to paste that graphic into your document.
After you have selected this option, you are prompted to position the graphic
on the current document page, just as was done for the 
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Paste Bitmap from Clipboard:elink. menu option.

:h3 res=IDM_PASTE_FROM_SCRN.Paste From Screen Menu
From this menu it is possible to select a Paste option which takes
graphic image data from somewhere on the current screen display.
The following options are available&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_ENTIRE.Paste Entire Desktop from Screen:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_FRAME.Paste Frame Window from Screen:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_WINDOW.Paste Window from Screen:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_RECT.Paste Rectangle from Screen:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_HIDE.Hide:elink.
:esl.

:h4 res=IDM_PASTE_FROM_SCRN_ENTIRE.Paste Entire Desktop from Screen (Menu Option)
When you select this menu option, a bitmap copy of the entire OS/2 Desktop,
in its current state, is made and you are prompted to position the graphic
on the current document page, just as was done for the 
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Paste Bitmap from Clipboard:elink. menu option.
:note.Be careful with this kind of graphic paste, as the graphic data can be
quite large with this kind of copy.  No matter what size you scale the graphic
to on the document page, the graphic&apos.s size in memory remains the same, and a
an image of the entire desktop can range anywhere from 100K to 800K bytes,
depending on your selected video resolution and color depth.

:h4 res=IDM_PASTE_FROM_SCRN_FRAME.Paste Frame Window from Screen (Menu Option)
This function is not yet implemented.

:h4 res=IDM_PASTE_FROM_SCRN_WINDOW.Paste Window from Screen (Menu Option)
This function is not yet implemented.

:h4 res=IDM_PASTE_FROM_SCRN_RECT.Paste Rectangle from Screen (Menu Option)
This function is not yet implemented.

:h4 res=IDM_PASTE_FROM_SCRN_HIDE.Hide (Menu Option)
This menu option, when checked, causes the application to be hidden
when any one of the
:link reftype=hd res=IDM_PASTE_FROM_SCRN.Paste From Screen Menu:elink.
options is requested.  The application is hidden, the Paste From command
is executed, and then the application is restored to its original state
on the system desktop.

:h3 res=IDM_PASTE_FROM_FILE.Paste Graphic from File (Menu Option)
When you select this option, the normal system file selection dialog is
displayed, from which you need to select the graphic image file to be
pasted into your document.  The currently supported file types are&colon.
:sl compact.
:li.&sqbul.IBM OS/2 Bitmap (.BMP)
:li.&sqbul.IBM OS/2 MetaFile (.MET)
:li.&sqbul.Microsoft Windows Bitmap (.BMP)
:li.&sqbul.Graphic Interchange Format (.GIF)
:li.&sqbul.Joint Photographic Experts Group File Interchange Format (.JPG,.JPEG,.JFIF)
:li.&sqbul.PC Paintbrush File Format (.PCX or .PCC)
:li.&sqbul.Tag Image File Format (.TIF or .TIFF)
:esl.
:p.
Once you have selected a file, you are prompted to position the graphic
on the current document page, just as was done for the 
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Paste Bitmap from Clipboard:elink. menu option.

:h2 res=IDM_INSERT_FUNCTION.Insert Function Menu
This submenu enables you to insert a number of special function
codes into the document.  The following options are currently
available&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_DATE.Date:elink.
:li.&sqbul.:link reftype=hd res=IDM_CURRENTPAGE.Current Page Number:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGECOUNT.Total Page Count:elink.
:li.&sqbul.:link reftype=hd res=IDM_SEPARATOR.Separator:elink.
:li.&sqbul.:link reftype=hd res=IDM_BARCODE.Barcode...:elink.
:esl.
:note.This submenu is not a part of the Selected Text menu.

:h3 res=IDM_DATE.Date Menu
This sub-menu gives you access to the date insertion functions.
Currently, there are four options&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_DATETODAY.Today&apos.s Date:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATECURRENT.Current Date:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATECHANGED.Document Date:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATEFMT.Date Format...:elink.
:esl.
:note.This submenu is not a part of the Selected Text menu.

:h4 res=IDM_DATETODAY.Today&apos.s Date (Menu Option)
This menu option, when selected, inserts a special object into the document
which shows the date when the object was inserted, always.  This date
never changes.  As an object, it cannot be edited.
When you move past it with the cursor, it will behave as a single
character or token.  The format with which the date is shown
can be altered from
the :link reftype=hd res=IDD_DATEFMT.Date Format...:elink. dialog,
though you must alter the format upstream from the object, since
the object uses the format in effect at its location in the document.
:note.This option is not a part of the Selected Text menu.

:h4 res=IDM_DATECURRENT.Current Date (Menu Option)
This function inserts a special object into the document which shows
the current date, always.  It is always the current date when the
document is displayed or printed, not the date when the object was
inserted into the document.  As an object, it cannot be edited.
When you move past it with the cursor, it will behave as a single
character or token.  The format with which the date is shown
can be altered from
the :link reftype=hd res=IDD_DATEFMT.Date Format...:elink. dialog,
though you must alter the format upstream from the object, since
the object uses the format in effect at its location in the document.
:note.This option is not a part of the Selected Text menu.

:h4 res=IDM_DATECHANGED.Document Date (Menu Option)
This function inserts a special object into the document which shows
the date when the document was last changed, always.  This date will
change when the documented is updated.  As an object, it cannot be edited.
When you move past it with the cursor, it will behave as a single
character or token.  The format with which the date is shown
can be altered from
the :link reftype=hd res=IDD_DATEFMT.Date Format...:elink. dialog,
though you must alter the format upstream from the object, since
the object uses the format in effect at its location in the document.
:note.This option is not a part of the Selected Text menu.

:h4 res=IDM_DATEFMT.Date Format... (Menu Option)
This menu option, when selected, calls up
the :link reftype=hd res=IDD_DATEFMT.Date Format...:elink. dialog,
which is used to alter the format used for generation of dates.

:h5 res=IDD_DATEFMT.Date Format... (Dialog)
From this dialog it is possible to alter the format used in the
generation of dates.
:artwork name='English\DateFmt.bmp' align=center.
:p.The date format is whatever you type in the
entry field, except that certain characters preceded by the &apos.&percent.&apos.
character have special meaning.  These special codes are the following&colon.
:sl compact.
:li.&percent.w - This code becomes the day of the week (Sunday, Monday, etc.).
:li.&percent.x - This code becomes the abbreviated day of the week (Sun, Mon, etc.).
:li.&percent.d - This code becomes the day of the month, in numeric form.
:li.&percent.m - This becomes the month of the year, in numeric form.
:li.&percent.M - This is the month of the year (January, February, etc.).
:li.&percent.n - This is the abbreviated month of the year (Jan, Feb, etc.).
:li.&percent.y - This is the full year (4 digits).
:li.&percent.z - This is the abbreviated year (2 digits).
:esl.
:p.
Any other text entered in the field is copied directly to the generated
date.  For instance, the format string "&percent.w, &percent.M &percent.d, &percent.y" could become
"Wednesday, March 16, 1994".
:p.
You may enter the new date format, then click the OK button
to save the new value.  The currently active document will be
reformatted appropriately.
:p.
If you press the APPLY button, the new date format is applied
to the currently active document at once, and becomes the new value.
:p.
To escape the dialog without saving any more changes, then click the ESCAPE button.
Changes made with the APPLY button are not discarded.

:h3 res=IDM_CURRENTPAGE.Current Page Number (Menu Option)
This menu option inserts a special object into the document which displays the
current page number at the object&apos.s location within the document.

:h3 res=IDM_PAGECOUNT.Total Page Count (Menu Option)
This menu option inserts a special object into the document which displays the
document&apos.s total page count at the object&apos.s location within the document.

:h3 res=IDM_SEPARATOR.Separator (Menu Option)
This menu option inserts a special horizontal separator function
into the document.  The separator ends the current line, draws a
separator on the next line, and moves to the next line, ready for
more text.

:h3 res=IDM_BARCODE.Barcode (Menu Option)
This menu option summons a dialog from which you can prepare to insert
a barcode graphic into the document.  For more information, 
see :link reftype=hd res=IDD_BARCODE.Insert Barcode (Dialog):elink.

:h4 res=IDD_BARCODE.Insert Barcode (Dialog)
This dialog allows you to specify the type and text of a barcode
graphic to be inserted into the document.
:artwork name='English\BarCode.bmp' align=center.
You need to select the type of the barcode from the list
provided, and then enter the text to be barcoded in the 
entry field.  When you are ready, press ENTER or click on the
OK button, at which point you are prompted to position
the graphic on the current document page, just as was done for the 
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Paste Bitmap from Clipboard:elink. 
menu option.

:h2 res=IDM_SEARCH.Search... (Menu Option)
This menu option calls up a dialog from which you can set a text string
to be searched for and another text string with which the original can
be replaced if so requested.
:p.See :link reftype=hd res=IDD_SEARCH.Search... (Dialog):elink. for more information.
:p.The short-cut key for this function is Ctrl+S.

:h3 res=IDD_SEARCH.Search... (Dialog)
From this dialog it is possible to set the text to be searched for with the
:link reftype=hd res=IDM_SEARCH.Search:elink. and :link reftype=hd res=IDM_FINDNEXT.Find Next:elink.
commands.  It is also possible to set a replacement text and to cause that
replacement to occur.
:artwork name='English\Search.bmp' align=center.
:p.To search for a particular text, enter the text itself in the first entry field and
press <ENTER> or click on the Find button.  The text will be searched for and
highlighted.  The dialog remains present for you to change the search pattern
if you wish.
:p.To replace a text with another, enter the original text in the first entry field
and the replacement text in the second entry field, and then either press <ENTER>
to find the text to be replaced, or click on the Change button to search for the
text and replace it at once.  You may repeat the Change operation for each matching
text in the document if you wish, and the next match for the original text will
be replaced each time.
:p.To replace a text with another, and then proceed to find the next occurence of
the original text, use the Change &amp. Find button.
:p.To replace all occurences of the original text with the new text, use the
Change All button.
:p.If you wish your searches to be sensitive to letter case, or not sensitive, you
may click on the Case Sensitive check-button to change the current setting.
:p.If you wish your searches to obey the rules of UNIX Regular Expressions, 
you may check the Regular Expression button.
:p.To dismiss the dialog, press the <ESC> key or click on the Escape button.
Whatever the search and replacement texts were at the time are saved for the
next time you summon this dialog.

:h2 res=IDM_FINDNEXT.Find Next (Menu Option)
If you have defined a text string to be search for from the
:link reftype=hd res=IDD_SEARCH.Search... (Dialog):elink. dialog panel,
then this menu function will search forward to the next occurance
of the specified string and will select that string.
:p.See :link reftype=hd res=IDD_SEARCH.Search... (Dialog):elink. for more information.
:p.The short-cut key for this function is Ctrl+F.

:h2 res=IDM_GOTOPAGE.Go To Page (Menu Option)
This menu option summons a dialog from which you can specify the 
number of the page you wish to have displayed for editing.
See the :link reftype=hd res=IDD_GOTOPAGE.Go To Page:elink.
dialog help for more information.
:p.The short-cut key for this function is Ctrl+G.

:h3 res=IDD_GOTOPAGE.Go To Page (Dialog)
This dialog allows you to specify the number of the document page
that which you wish to display and edit.  Enter the page number
in the field provided and press the ENTER key, or press the ESC
key to abort this function.

:h2 res=IDM_SPELL.Spell Check (Menu Option)
If the optional spell checker module is installed on your system, and it was
able to open its dictionary file, then this menu option will be enabled.
When you click on this menu option, the spell checker will review the document
from the current position onwards, looking for possibly misspelled words.
When it finds one, the word is highlighted and the
:link reftype=hd res=IDD_SPELL.Spell Check:elink.
dialog will be displayed, allowing you to tell the checker what to do about
the word.
:p.The short-cut key for this function is F5.

:h3 res=IDD_SPELL.Spell Check (Dialog)
This dialog is displayed when the spell checker finds a possibly misspelled
word.  The word is displayed, along with a list of potential replacements
taken from the spell checker&apos.s dictionary.
:artwork name='English\Spell.bmp' align=center.
:p.If you wish to skip the word this one time, press ENTER or click on the
:hp4.Skip:ehp4. button with the mouse.  This is the default action.
:p.If you wish the spell checker to ignore the word for the rest of this session,
click on the :hp4.Accept:ehp4. button with the mouse.
:p.To add the word to your private dictionary, with its capitalization intact,
click on the :hp4.Insert:ehp4. button with the mouse.  If the word is capitalized, then
the dictionary entry will only match capitalized words.
:p.To add the word to your private dictionary, forced to lower case,
click on the :hp4.Insert Lowercase:ehp4. button with the mouse.  Such words can match
capitalized and non-capitalized versions of themselves.
:p.If you wish to use one of the suggested replacement words, select the
word from the list and press ENTER, or double-click on the word.  The original
word in the text will be replaced with the selected replacement.
:p.If you wish to enter a new spelling that is not in the list of suggested
replacements, type the new spelling in the entry field at the head of the list
of replacements, and click on the :hp4.Replace:ehp4. button.
The original word in the text will be replaced with the new spelling.
:p.To exit the spell checker without taking any further action, press the ESC
key, or click on the :hp4.Escape:ehp4. button with the mouse.  Any changes already done
are retained, though they may be removed with the :link reftype=hd res=IDM_UNDO.Undo:elink. command.

:h2 res=IDM_ZOOM.Zoom Menu
This menu option calls up a submenu of the different document viewing
zoom commands.  The following commands are available for use&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_100.Zoom 100&percent.:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLWIDTH.Zoom Full Width:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLHEIGHT.Zoom Full Height:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_MARGINS.Zoom Margins:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_ALL.Zoom All:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_WINDOW.Zoom Window:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_SET.Zoom Set...:elink.
:esl.

:h3 res=IDM_ZOOM_100.Zoom 100&percent. (Menu Option)
This command sets the current display scale to 100&percent..  What you see
displayed should print at exactly the same size, providing the system
scaling factors (out of my control) are set properly.

:h3 res=IDM_ZOOM_FULLWIDTH.Zoom Full Width (Menu Option)
This command sets the current display scale so that the page will occupy
the full width of its display window.  This may reduce or enlarge the
document scale, depending on how large you&apos.ve got the display window
set up to be.

:h3 res=IDM_ZOOM_FULLHEIGHT.Zoom Full Height (Menu Option)
This command sets the current display scale so that the page will occupy
the full height of its display window.  This will normally reduce the
document scale, causing it to display smaller than it will really print
out as.

:h3 res=IDM_ZOOM_MARGINS.Zoom Margins (Menu Option)
This command sets the current display scale so that the portion of the
document within the horizontal margins will occupy the full width of
its display window.  This will normally enlarge the document scale,
depending on how large you have set the display window to be.

:h3 res=IDM_ZOOM_ALL.Zoom All (Menu Option)
This command sets the current display scale to the smaller of the Zoom Full
Height and the Zoom Full Width scales.  This is done so the entire document
will appear in the window, no matter how small or misshapen the document
window is.
:p.The short-cut key for this function is Ctrl+Z, and when used this way the
function toggles between Zoom All and whatever preferred Zoom is specified
in the :link reftype=hd res=IDD_PREFERENCES.General Preferences:elink. dialog.

:h3 res=IDM_ZOOM_WINDOW.Zoom Window (Menu Option)
This command allows you to select a rectangular portion of the currently
active document and zoom in on it.  Once the portion has been selected,
the current document will be have a new zoom factor applied to it and
it will be repositioned so that the portion you have selected is visible
at the largest possible magnification.

:h3 res=IDM_ZOOM_SET.Zoom Set... (Menu Option)
This command invokes a dialog from which it is possible to change the
current window zoom factor.
:p.See :link reftype=hd res=IDD_ZOOM_SET.Zoom Set... (Dialog):elink. for more information.

:h4 res=IDD_ZOOM_SET.Zoom Set... (Dialog)
This dialog allows the user to set the viewing zoom factor to
any value desired.  Use the up and down arrows to change the zoom factor,
or enter the value directly, then press the ENTER key or click on the
OK button to make the new zoom factor take effect.
:artwork name='English\ZoomSet.bmp' align=center.
:p.To apply the zoom factor without dismissing the dialog, click on the
APPLY button.  Zooms applied in this manner remain in effect after
the dialog is dismissed, even if the ESCAPE key is used.
:p.To escape from the dialog without effecting any further change,
press the ESCAPE key or click on the ESCAPE button.
Changes made by the APPLY button are not discarded.

:h1 res=IDM_LAYOUT_MENU.Layout Menu
:i1.Layout Menu
From the Layout Menu the following commands are available&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_MARGINS.Margins...:elink.
:li.&sqbul.:link reftype=hd res=IDM_TABS.Tabs...:elink.
:li.&sqbul.:link reftype=hd res=IDM_INDENT.Indent...:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINESPACING_MENU.Line Spacing Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUSTIFICATION.Justification Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGECENTER.Page Center:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGEBREAK.Page Break:elink.
:li.&sqbul.:link reftype=hd res=IDM_HEADER.Header...:elink.
:li.&sqbul.:link reftype=hd res=IDM_FOOTER.Footer...:elink.
:esl.

:h2 res=IDM_MARGINS.Margins... (Menu Option)
This command invokes a dialog from which it is possible to set all
four printing margins (top, bottom, left, right).
:p.See :link reftype=hd res=IDD_MARGINS.Margins (Dialog):elink. for more information.

:h3 res=IDD_MARGINS.Margins (Dialog)
Through this dialog you can set the left, right, top and bottom
printing margins.
These margins are specified in the currently selected measurement system (english, metric),
and are the distances from the sides of the current form type.
:artwork name='English\Margins.bmp' align=center.
:p.
As you enter new values for the margins, the horizontal and vertical
rulers for the currently active document are adjusted accordingly,
though the document itself is not reformatted.
:p.
Enter the distances and press the ENTER key or click on the OK button.
If the margins will not work with the currently selected form, you
will get an error message and be allowed to try again.
:p.
If you press the APPLY button, the new margin settings are applied
to the currently active document at once, and become the new margins.
:p.
To abort the dialog and discard the changes entered, press the
ESC key or click on the &apos.Escape&apos. button.
Changes made by the APPLY button are not discarded.
:p.
:note.The new left and right margins will take effect with the next
line, unless the margin command was inserted into the document at
the very beginning of a new line.
The new top and bottom margins will take effect with the next page,
unless the margin command was inserted into the document at the very
beginning of a new page.

:h2 res=IDM_TABS.Tabs... (Menu Option)
This command invokes a dialog from which it is possible to alter the
document&apos.s tab settings.  The new settings will take effect from the cursor
location onwards.
:p.See :link reftype=hd res=IDD_TABS.Tabs (Dialog):elink. for more information.

:h3 res=IDD_TABS.Tabs (Dialog)
This dialog permits you to enter the position of a tab and either set
the tab, or clear it.  Your changes are immediately visible on the
active window&apos.s horizontal ruler.
:artwork name='English\Tabs.bmp' align=center.
:p.To set a tab, enter the tab position in the entry field, then push
the &apos.Set&apos. pushbutton.
:p.To clear a tab, enter the tab position in the entry field, then push
the &apos.Clear&apos. pushbutton.
:p.Once you have entered all the changes you wish to make to your document&apos.s
tab settings, you must select the OK button and press ENTER, or you can
click the OK button with the mouse.  The changes you have entered take
effect from the current cursor position onwards.
:p.If you press the APPLY button, the new tab settings are applied
to the currently active document at once, and become the new tabs.
:p.To abort the dialog and discard the changes entered, press the
ESC key or click on the &apos.Escape&apos. button.
Changes made by the APPLY button are not discarded.

:h2 res=IDM_INDENT.Indent... (Menu Option)
This menu option calls forth a dialog from which you can specify the
indent distance to be used in starting each new paragraph.
:p.See :link reftype=hd res=IDD_INDENT.Paragraph Indent (Dialog):elink. for more information.

:h3 res=IDD_INDENT.Paragraph Indent (Dialog)
From this dialog it is possible to set the paragraph indention distance
to be used from the document&apos.s current position onwards.
:artwork name='English\Indent.bmp' align=center.
:p.You may enter the new indention distance, then click the OK button
to save the new value.  The currently active document will be
reformatted appropriately.
:p.If you press the APPLY button, the new indention distance is applied
to the currently active document at once, and becomes the new indention.
:p.To escape the dialog without saving any more changes, then click the ESCAPE button.
Changes made by the APPLY button are not discarded.

:h2 res=IDM_LINESPACING_MENU.Line Spacing Menu
This submenu presents the different vertical line spacing
options that are available.
:p.The entry for the currently selected line spacing is check-marked.
:p.See :link reftype=hd res=IDM_LINESPACINGS.Line Spacing Menu Entries:elink.
for more information.

:h3 res=IDM_LINESPACINGS.Line Spacing Menu Entries
This menu entry represents one of the currently available line spacing options.
Line spacings are defined as percentages of the normal line
spacing for the current font and point size.  When one of these
line spacing options is selected, then the selected line spacing
percentage of normal is establish over the currently selected
text block, or from the cursor onwards if no text is selected.

:h3 res=IDM_LINESPACINGMGR.Line Spacing Menu Manager
This function is not yet implemented.

:h2 res=IDM_JUSTIFICATION.Justification (Menu Option)
This submenu presents the different horizontal text justification options
that are available.  They include the following&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_JUST_LEFT.Left Justify:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_CENTER.Center Justify:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_RIGHT.Right Justify:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_FULL.Full Justify:elink.
:esl.

:h3 res=IDM_JUST_LEFT.Left Justify (Menu Option)
This menu option inserts a command into the document to left justify the
document text, starting with the next line, unless the command was inserted
at the very beginning of a new line, in which case it will take effect
immediately.

:h3 res=IDM_JUST_CENTER.Center Justify (Menu Option)
This menu option inserts a command into the document to center justify the
document text, starting with the next line, unless the command was inserted
at the very beginning of a new line, in which case it will take effect
at once.

:h3 res=IDM_JUST_RIGHT.Right Justify (Menu Option)
This menu option inserts a command into the document to right justify the
document text, starting with the next line, unless the command was inserted
at the very beginning of a new line, in which case it takes effect
immediately.

:h3 res=IDM_JUST_FULL.Full Justify (Menu Option)
This menu option inserts a command into the document to full justify the
document text, starting with the next line, unless the command was inserted
at the very beginning of a new line, in which case it takes effect at once.
The sizes of spaces in fully justified text are adjusted to make the left and
right margins of the text turn out even.  Lines terminated with a carriage
return or page break are treated as left justified.

:h2 res=IDM_PAGECENTER.Page Center (Menu Option)
This menu option inserts a command into the document to center pages
vertically, starting with the next page, unless the command was inserted
at the very beginning of a new page, in which case it takes effect at once.
:note.This function cannot be used together with graphics, and is disabled
by the presence of graphics on the page.

:h2 res=IDM_PAGEBREAK.Page Break (Menu Option)
This command, when selected, inserts a page break into the document at
the cursor location.  The cursor will advance to the start of the new
page.
:p.The short-cut key for this function is CTRL+ENTER.
:note.This option is not a part of the Selected Text menu.

:h2 res=IDM_HEADER.Header... (Menu Option)
This menu option summons a dialog from which you may define the 
appearance of the page header from the location of the cursor onwards.

:h3 res=IDD_HEADER.Header (Dialog)      
This dialog allow the specification of the new page header.  
At this time, the header must be specified in native Escriba text format.
Later, the definition will become WYSIWYG.
Most function codes (like PageNumber, PageCount, LeftJustify, etc) are
valid for the header format, but not all (like PrinterSettings).

:h2 res=IDM_FOOTER.Footer... (Menu Option)
This menu option summons a dialog from which you may define the 
appearance of the page footer from the location of the cursor onwards.

:h3 res=IDD_FOOTER.Footer (Dialog)      
This dialog allow the specification of the new page footer.
At this time, the header must be specified in native Escriba text format.
Later, the definition will become WYSIWYG.
Most function codes (like PageNumber, PageCount, LeftJustify, etc) are
valid for the header format, but not all (like PrinterSettings).

:h1 res=IDM_FONT_MENU.Font Menu
:i1.Font Menu
This menu displays a list of all the currently available fonts.
:p.The font currently active at the cursor location is check-marked.
:p.See :link reftype=hd res=IDM_FONTS.Font Menu Entries:elink.
for more information.

:h2 res=IDM_FONTS.Font Menu Entries
This menu entry represents one of the currently available fonts, displayed
as it would appear in the document, at 18 points.  If you wish to set this
font from the cursor location onwards, or over the currently selected
block of text, select the font entry with the arrow keys and press ENTER,
or click on the font entry with the mouse.

:h2 res=IDM_FONTS_PGUP.Font Menu Page Up
This menu option is available if the entire font menu cannot be
displayed, and there are more font menu entries available before
the first one currently shown.  To scroll the font menu upwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h2 res=IDM_FONTS_PGDN.Font Menu Page Down
This menu option is available if the entire font menu cannot be
displayed, and there are more font menu entries available after
the last one currently shown.  To scroll the font menu downwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h2 res=IDM_FONTMGR.Font Menu Manager
This function is not yet implemented.

:h1 res=IDM_SIZE_MENU.Size Menu
:i1.Size Menu
This menu displays a list of all the currently available font sizes.
:p.The size currently active at the cursor location is check-marked.
:p.See :link reftype=hd res=IDM_SIZES.Size Menu Entries:elink.
for more information.

:h2 res=IDM_SIZES.Size Menu Entries
This menu entry represents one of the currently available font sizes,
showing the currently selected font in each of those sizes.  If you wish
to set a size from the cursor location onwards, or over the currently
selected block of text, select the size entry with the arrow keys and
press ENTER, or click on the size entry with the mouse.

:h2 res=IDM_SIZES_PGUP.Size Menu Page Up
This menu option is available if the entire size menu cannot be
displayed, and there are more size menu entries available before
the first one currently shown.  To scroll the size menu upwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h2 res=IDM_SIZES_PGDN.Size Menu Page Down
This menu option is available if the entire size menu cannot be
displayed, and there are more size menu entries available after
the last one currently shown.  To scroll the size menu downwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h2 res=IDM_SIZEMGR.Size Menu Manager
This function is not yet implemented.

:h1 res=IDM_ATTR_MENU.Attribute Menu
:i1.Attribute Menu
From the Attribute Menu it is possible to insert font attribute commands
into the document.  All the font attributes supported natively by the
Presentation Manager are here: Normal, Bold, Italic, Underscore, Strikeout
and Outline.  To cause all text from the cursor&apos.s current location onwards,
or all text currently highlighted, to have one of these attributes,
simply select the menu option with the mouse or with the keyboard.
The font attribute menu entries are shown in the currently selected
font and size, and the currently active attributes are checkmarked.
Any combination of attributes (except NORMAL) can be active at any given
time.  Normal is the setting when none of the others is active.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_NORMAL.Normal:elink.
:li.&sqbul.:link reftype=hd res=IDM_BOLD.Bold:elink.
:li.&sqbul.:link reftype=hd res=IDM_ITALIC.Italic:elink.
:li.&sqbul.:link reftype=hd res=IDM_UNDERSCORE.Underscore:elink.
:li.&sqbul.:link reftype=hd res=IDM_STRIKEOUT.Strikeout:elink.
:li.&sqbul.:link reftype=hd res=IDM_OUTLINE.Outline:elink.
:li.&sqbul.:link reftype=hd res=IDM_CAPS.Caps:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXPAND_MENU.Expand Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINECOLOR_MENU.Line Color Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEWIDTH_MENU.Line Width Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILLCOLOR_MENU.Fill Color Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILLTYPE_MENU.Fill Type Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEEND_MENU.Line End Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEJOIN_MENU.Line Join Menu:elink.
:esl.

:h2 res=IDM_NORMAL.Normal (Menu Option)
This menu option, if selected, will cancel any currently active font
attributes (like bold, italic, etc.).  If this option is selected while
text has been highlighted, then the effect will be applied only to the
highlighted text.  If nothing was selected, the effect is applied from
the cursor onwards.  Attribute commands embedded in the document further
along are not affected.
:p.The short-cut key for this function is Ctrl+N.

:h2 res=IDM_BOLD.Bold (Menu Option)
This menu option, when selected, will apply the Bold font attribute to the
currently selected text, or from the cursor location onwards.  Attribute
commands embedded further along in the document are not affected.
:p.The short-cut key for this function is Ctrl+B.

:h2 res=IDM_ITALIC.Italic (Menu Option)
This menu option, when selected, will apply the Italic font attribute to the
currently selected text, or from the cursor location onwards.  Attribute
commands embedded further along in the document are not affected.
:p.The short-cut key for this function is Ctrl+I.

:h2 res=IDM_UNDERSCORE.Underscore (Menu Option)
This menu option, when selected, will apply the Underscore font attribute
to the currently selected text, or from the cursor location onwards.
Attribute commands embedded further along in the document are not affected.
:p.The short-cut key for this function is Ctrl+U.

:h2 res=IDM_STRIKEOUT.Strikeout (Menu Option)
This menu option, when selected, will apply the Strikeout font attribute
to the currently selected text, or from the cursor location onwards.
Attribute commands embedded further along in the document are not affected.

:h2 res=IDM_OUTLINE.Outline (Menu Option)
This menu option, when selected, will apply the Outline font attribute
to the currently selected text, or from the cursor location onwards.
Attribute commands embedded further along in the document are not affected.

:h2 res=IDM_CAPS.Caps (Menu Option)
This menu option, when selected, will apply the Caps font attribute
to the currently selected text, or from the cursor location onwards.
Attribute commands embedded further along in the document are not affected.

:h2 res=IDM_EXPAND_MENU.Expand Menu
This menu lists all the currently available horizontal text expansion
factors.
:p.The expansion currently active at the cursor location is check-marked.
:p.See :link reftype=hd res=IDM_EXPANDS.Expand Menu Entries:elink.
for more information.

:h3 res=IDM_EXPANDS.Expand Menu Entries
This menu entry shows one of the currently available horizontal text expansion
factors, showing text in the current font expanded to the different factors.
To set an expansion factor from the cursor location onwards, or to apply
an expansion factor over the currently selected block of text, select the
desired expansion with the arrow keys and press ENTER, or click on the
menu entry with the mouse.

:h3 res=IDM_EXPANDS_PGUP.Expand Menu Page Up
This menu option is available if the entire expand menu cannot be
displayed, and there are more expand menu entries available before
the first one currently shown.  To scroll the expand menu upwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_EXPANDS_PGDN.Expand Menu Page Down
This menu option is available if the entire expand menu cannot be
displayed, and there are more expand menu entries available after
the last one currently shown.  To scroll the expand menu downwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_EXPANDMGR.Expand Menu Manager
This function is not yet implemented.

:h2 res=IDM_LINECOLOR_MENU.Line/Text Color Menu
This menu lists all the currently available line/text colors.
:p.The color currently active at the cursor location is check-marked.
:p.See :link reftype=hd res=IDM_LINECOLORS.Line/Text Color Menu Entries:elink.
for more information.

:h3 res=IDM_LINECOLORS.Line/Text Color Menu Entries
This menu entry shows one of the currently available line/text colors.
To set a line/text color from the cursor location onwards, or to apply
the color over the currently selected block of text, select the color
menu entry with the arrow keys and press ENTER, or click on the menu
entry with the mouse.
:note.These colors apply to lines, solid text, and to the outline
portion of outlined text.

:h3 res=IDM_LINECOLORS_PGUP.Line/Text Color Menu Page Up
This menu option is available if the entire line/text color menu cannot be
displayed, and there are more line/text color menu entries available before
the first one currently shown.  To scroll the line/text color menu upwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_LINECOLORS_PGDN.Line/Text Color Menu Page Down
This menu option is available if the entire line/text color menu cannot be
displayed, and there are more line/text color menu entries available after
the last one currently shown.  To scroll the line/text color menu downwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_LINECOLORMGR.Line/Text Color Menu Manager
This function is not yet implemented.

:h2 res=IDM_LINEWIDTH_MENU.Line Width Menu
This menu lists all the currently available line widths.
:p.The line width currently active at the cursor location is check-marked.
:p.See :link reftype=hd res=IDM_LINEWIDTHS.Line Width Menu Entries:elink.
for more information.

:h3 res=IDM_LINEWIDTHS.Line Width Menu Entries
This menu entry shows one of the currently available line widths for use
with outlined text.  To set a line width from the cursor location onwards,
or to apply the width over the currently selected block of text, select
the width menu entry with the arrow keys and press ENTER, or click on the
menu entry with the mouse.
:note.These widths apply to lines, and to the outline portion of outlined text.

:h3 res=IDM_LINEWIDTHS_PGUP.Line Width Menu Page Up
This menu option is available if the entire line width menu cannot be
displayed, and there are more line width menu entries available before
the first one currently shown.  To scroll the line width menu upwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_LINEWIDTHS_PGDN.Line Width Menu Page Down
This menu option is available if the entire line width menu cannot be
displayed, and there are more line width menu entries available after
the last one currently shown.  To scroll the line width menu downwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_LINEWIDTHMGR.Line Width Menu Manager
This function is not yet implemented.

:h2 res=IDM_FILLCOLOR_MENU.Fill Color Menu
This menu lists all the currently available outlined text fill colors.
:p.The fill color currently active at the cursor location is check-marked.
:p.See :link reftype=hd res=IDM_FILLCOLORS.Fill Color Menu Entries:elink.
for more information.

:h3 res=IDM_FILLCOLORS.Fill Color Menu Entries
This menu entry shows one of the currently available fill colors for use
with outlined text.  To set a fill color from the cursor location onwards,
or to apply the fill color over the currently selected block of text, select
the fill color menu entry with the arrow keys and press ENTER, or click on
the menu entry with the mouse.

:h3 res=IDM_FILLCOLORS_PGUP.Fill Color Menu Page Up
This menu option is available if the entire fill color menu cannot be
displayed, and there are more fill color menu entries available before
the first one currently shown.  To scroll the fill color menu upwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_FILLCOLORS_PGDN.Fill Color Menu Page Down
This menu option is available if the entire fill color menu cannot be
displayed, and there are more fill color menu entries available after
the last one currently shown.  To scroll the fill color menu downwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_FILLCOLORMGR.Fill Color Menu Manager
This function is not yet implemented.

:h2 res=IDM_FILLTYPE_MENU.Fill Type Menu
This menu lists all the currently available outlined text fill types.
:p.The fill type currently active at the cursor location is check-marked.
:p.See :link reftype=hd res=IDM_FILLTYPES.Fill Type Menu Entries:elink.
for more information.

:h3 res=IDM_FILLTYPES.Fill Type Menu Entries
This menu entry shows on of the currently available fill types for use
with outlined text.  To set a fill type from the cursor location onwards,
or to apply the fill type over the currently selected block of text, select
the fill type menu entry with the arrow keys and press ENTER, or click on
the menu entry with the mouse.

:h3 res=IDM_FILLTYPES_PGUP.Fill Type Menu Page Up
This menu option is available if the entire fill type menu cannot be
displayed, and there are more fill type menu entries available before
the first one currently shown.  To scroll the fill type menu upwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_FILLTYPES_PGDN.Fill Type Menu Page Down
This menu option is available if the entire fill type menu cannot be
displayed, and there are more fill type menu entries available after
the last one currently shown.  To scroll the fill type menu downwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_FILLTYPEMGR.Fill Type Menu Manager
This function is not yet implemented.

:h2 res=IDM_LINEEND_MENU.Line End Menu
This menu lists all the currently available line end types.
:p.The type currently active at the cursor location is check-marked.
:p.See :link reftype=hd res=IDM_LINEENDS.Line End Menu Entries:elink.
for more information.

:h3 res=IDM_LINEENDS.Line End Menu Entries
This menu entry shows on of the currently available line end types for use
with outlined text.  To set a line end type from the cursor location onwards,
or to apply the type over the currently selected block of text, select
the line end type menu entry with the arrow keys and press ENTER, or click on
the menu entry with the mouse.

:h3 res=IDM_LINEENDS_PGUP.Line End Menu Page Up
This menu option is available if the entire line end type menu cannot be
displayed, and there are more line end type menu entries available before
the first one currently shown.  To scroll the line end type menu upwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_LINEENDS_PGDN.Line End Menu Page Down
This menu option is available if the entire line end type menu cannot be
displayed, and there are more line end type menu entries available after
the last one currently shown.  To scroll the line end type menu downwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_LINEENDMGR.Line End Menu Manager
This function is not yet implemented.

:h2 res=IDM_LINEJOIN_MENU.Line Join Menu
This menu lists all the currently available line join types.
:p.The type currently active at the cursor location is check-marked.
:p.See :link reftype=hd res=IDM_LINEJOINS.Line Join Menu Entries:elink.
for more information.

:h3 res=IDM_LINEJOINS.Line Join Menu Entries
This menu entry shows on of the currently available line join types for use
with outlined text.  To set a line join type from the cursor location onwards,
or to apply the line join type over the currently selected block of text, select
the line join type menu entry with the arrow keys and press ENTER, or click on
the menu entry with the mouse.

:h3 res=IDM_LINEJOINS_PGUP.Line Join Menu Page Up
This menu option is available if the entire line join type menu cannot be
displayed, and there are more line join type menu entries available before
the first one currently shown.  To scroll the line join type menu upwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_LINEJOINS_PGDN.Line Join Menu Page Down
This menu option is available if the entire line join type menu cannot be
displayed, and there are more line join type menu entries available after
the last one currently shown.  To scroll the line join type menu downwards,
select this menu option with the arrow keys and press ENTER, or
click on it with the mouse.

:h3 res=IDM_LINEJOINMGR.Line Join Menu Manager
This function is not yet implemented.

:h1 res=IDM_WINDOW_MENU.Window Menu
:i1.Window Menu
From the Window Menu several document window manipulation functions are
available, along with a list of all the document windows currently open.
The following commands are available&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_TILE.Tile:elink.
:li.&sqbul.:link reftype=hd res=IDM_CASCADE.Cascade:elink.
:li.&sqbul.:link reftype=hd res=IDM_ARRANGEICONS.Arrange Icons:elink.
:li.&sqbul.:link reftype=hd res=IDM_WINDOWS.Active Windows:elink.
:esl.

:h2 res=IDM_TILE.Tile (Menu Option)
This command arranges the non-minimized document windows so that all are
completely visible, and so that all the minimized document icons are also
visible at the bottom of the workspace.  The minimized document icons are
rearranged as necessary.

:h2 res=IDM_CASCADE.Cascade (Menu Option)
This command arranges the non-minimized document windows so that they are
as large as they can be, yet still have all their titles showing and so
that all the minimized document icons are also visible.  The minimized
document icons are rearranged as necessary.

:h2 res=IDM_ARRANGEICONS.Arrange Icons (Menu Option)
This command will rearrange the minimized document window icons to the
bottom-left of the workspace.  This is done automatically when the Tile
and Cascade commands are executed.

:h2 res=IDM_WINDOWS.Active Windows
This section of the Window menu shows a list of the currently open document
windows.  To select a document, causing it to be displayed at the top
of the current stack of document windows, click on one of these menu
options, or select it with the arrow keys and press ENTER.

:h1 res=IDM_HELP_MENU.Help Menu
:i1.Help Menu
The Help Menu has been set up in the SAA standard format, providing the
following commands&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_HELP_INDEX.Help Index:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXTENDED_HELP.General Help:elink.
:li.&sqbul.:link reftype=hd res=IDM_HELP_FOR_HELP.Using Help:elink.
:li.&sqbul.:link reftype=hd res=IDM_KEYS_HELP.Keys Help:elink.
:li.&sqbul.:link reftype=hd res=IDM_ABOUT_MENU.Product Information Menu:elink.
:esl.

:h2 res=IDM_HELP_INDEX.Help Index (Menu Option)
Selecting this menu option will cause the help
library index to be displayed.

:h2 res=IDM_EXTENDED_HELP.General Help (Menu Option)
Selecting this menu option will display general information about the application.

:h2 res=IDM_HELP_FOR_HELP.Using Help (Menu Option)
Selecting this menu option will display the basic information on how to
use the Help Facility.
None of this information is specific to this application.

:h2 res=IDM_KEYS_HELP.Keys Help (Menu Option)
The following function key assignments have been made for use
within this program&colon.

:table cols='20 60' rules=both frame=box.
:row.:c.F1              :c.Display help information (SAA).
:row.:c.F2              :c.Save current document.
:row.:c.F3              :c.Exit program.
:row.:c.F4              :c.Save and close current document.
:row.:c.F5              :c.Spell Check
:row.:c.F10             :c.Menu (SAA).
:row.:c.Ctrl + Tab      :c.Next Document
:row.:c.Ctrl + O        :c.Open Document
:row.:c.Ctrl + P        :c.Print Entire Document
:row.:c.Alt + P         :c.Print Current Page
:row.:c.Ctrl + N        :c.Set Normal (Reset Text Attributes)
:row.:c.Ctrl + B        :c.Set Bold
:row.:c.Ctrl + I        :c.Set Italic
:row.:c.Ctrl + U        :c.Set Underscore
:row.:c.Ctrl + S        :c.Search
:row.:c.Ctrl + F        :c.Find Next
:row.:c.Ctrl + G        :c.Go To Page
:row.:c.Ctrl + Z        :c.Zoom All Toggle
:row.:c.Ctrl + L        :c.Left-justify Text
:row.:c.Ctrl + C        :c.Center-justify Text
:row.:c.Ctrl + R        :c.Right-justify Text
:row.:c.Ctrl + J        :c.Full-justify Text
:row.:c.Ctrl + A        :c.Select All Text
:row.:c.Ctrl + Backspace:c.Delete to start of current word.
:etable.
In addition, there are a number of keys defined for navigation within
the current document.  For these definitions, see the topic
:link reftype=hd res=60003.Editing Documents (Navigation):elink..

:h2 res=IDM_ABOUT_MENU.Product Information Menu (Submenu)
If you select this menu option directly, the program&apos.s
Product Information and Copyright dialog to be displayed.
:p.See :link reftype=hd res=IDD_ABOUT.Product Information (Dialog):elink. for more information.
:p.If you click on the conditional menu button instead,
a menu listing the installed products will be displayed, from
which you may select product information on any of those
products.

:h3 res=IDM_ABOUT.Product Information Menu Entry (Menu Option)
This is an entry in the Product Information Menu, representing 
this program or one of the extension products installed with it.
Click on the entry to see the product information itself
(see :link reftype=hd res=IDD_ABOUT.Product Information (Dialog):elink. for more information).

:h4 res=IDD_ABOUT.Product Information (Dialog)
This dialog box displays the program name, icon and copyright information.
To exit the dialog, press the ENTER key, the SPACE bar or the ESCAPE key,
or click on the OK button with the mouse.
:artwork name='English\About.bmp' align=center.

:h1 res=60009.Context Menus
:i1.Context Menus
There are several context menus that can be called up by a single click
on the right mouse button.  Together, these context menus contain even
more functions than those available from the main window pull-down menus.
:p.
When you right-click on the main program workspace (not over a document window),
the :link reftype=hd res=IDM_MAIN_CONTEXT.Main Context Menu:elink. is called up.
When you right-click over a document window, but not over selected text nor
over any graphic areas, then the :link reftype=hd res=IDM_DEFAULTMENU.Default Menu:elink.
is summoned.  Right clicking over a block of selected text will result
in the :link reftype=hd res=IDM_TEXTMENU.Selected Text Menu:elink. being brought up.
The are also the :link reftype=hd res=IDM_BITMAPMENU.Graphic Menu:elink. and
the :link reftype=hd res=IDM_BORDERMENU.Graphic Border Menu:elink..

:h2 res=IDM_MAIN_CONTEXT.Main Context Menu
This menu appears when you click on the right mouse button over the main
program workspace (not over a document window).  It includes all the menu
options that are valid outside of a document.  They include the following&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_REFRESH.Refresh:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILE_MENU.File:elink.
:li.&sqbul.:link reftype=hd res=IDM_WINDOW_MENU.Window:elink.
:li.&sqbul.:link reftype=hd res=IDM_HELP_MENU.Help:elink.
:esl.
:note.The File submenu of this menu does not contain all the options
that the normal File menu has, but rather only contain those options
that can be used without a specific document context.
These exceptions are noted within the detailed descriptions of each menu option.
Also, if no document window is open, the Window Menu option will
be disabled.

:h3 res=IDM_REFRESH.Refresh (Menu Option)
When this menu option is selected, the entire document window is repainted.

:h2 res=IDM_DEFAULTMENU.Default Menu
This menu appears when you click with the right mouse button over any
area of the document window which is not (1) selected text, (2) a graphic
or (3) a graphic&apos.s border.  It includes menu options that can be applied
to the document as a whole.  These include the following&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_REFRESH.Refresh:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILE_MENU.File Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_EDIT_MENU.Edit Menu:elink.
:esl.
:note.The File and Edit submenus of this menu do not contain all the options
that the normal File and Edit menus have, but rather only contain those
options that can be applied to an open file in general.
These exceptions are noted within the detailed descriptions of each menu option.

:h2 res=IDM_TEXTMENU.Selected Text Menu
This menu appears when you click with the right mouse button over an
area of selected text.  It includes all the menu options that can
be applied to an area of selected text.  They include the following&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_CUT.Cut:elink.
:li.&sqbul.:link reftype=hd res=IDM_COPY.Copy:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE.Paste:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEAR.Clear:elink.
:li.&sqbul.:link reftype=hd res=IDM_LAYOUT_MENU.Layout:elink.
:li.&sqbul.:link reftype=hd res=IDM_FONT_MENU.Font:elink.
:li.&sqbul.:link reftype=hd res=IDM_SIZE_MENU.Size:elink.
:li.&sqbul.:link reftype=hd res=IDM_ATTR_MENU.Attributes:elink.
:esl.
:note.The Layout submenu of this menu does not contain all the options
that the normal Layout menu has, but rather only contains those
options that can be applied to a block of selected text.  These
exceptions are noted within the detailed descriptions of each
menu option.

:h2 res=IDM_BITMAPMENU.Graphic Menu
This menu appears when you click with the right mouse button over a
graphic object.  It includes all the menu options that can be applied
to such an object.  These include the following&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_BACKGROUND.Background:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_NOTEXTASIDE.No Text Aside:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_HALFTONE.Halftone:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_REVERSE.Reverse:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_OPTIMIZE.Optimize:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION.Position:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE.Size:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT.Aspect:elink.
:esl.

:h3 res=IDM_BITMAP_BACKGROUND.Background (Menu Option)
When you select this menu option, the graphic image is marked as
belonging to the background of the page.  Normal text can flow on top of
the graphic.

:h3 res=IDM_BITMAP_NOTEXTASIDE.No Text Aside (Menu Option)
When you select this menu option, the graphic image is marked so that the
document formatter will not allow text to flow down the page to either side
of the graphic.

:h3 res=IDM_BITMAP_HALFTONE.Halftone (Menu Option)
When you select this menu option, the graphic image is marked for painting
in halftone, or muted, colors.  This is a useful attribute for graphics set
to the background.
:note.This option only works with bitmap graphics (not metafiles).

:h3 res=IDM_BITMAP_REVERSE.Reverse (Menu Option)
When you select this menu option, the graphic image is marked for painting
in inverted colors.  
:note.This option only works with bitmap graphics (not metafiles).

:h3 res=IDM_BITMAP_OPTIMIZE.Optimize (Menu Option)
When you select this menu option, the graphic image is marked for optimization
for the printer in use.  That is, the graphic scaling will be forced to a value
which will cause the size of the image on the printer to be an integer multiple
of the original bitmap image size.  This causes the printed appearance to be
about as good as it will get, since the line/column insertion method used to
scale bitmaps will be as accurate as it can be.
:note.This option only works with bitmap graphics (not metafiles).

:h3 res=IDM_BITMAP_POSITION.Graphic Position Menu
From this submenu it is possible to select a number of options which affect the
positioning of a graphic on the document page.
These options include the following&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_NEXT_CHAR.Next Character:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_NEXT_LINE.Next Line:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_LEFT.Left Edge:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_LEFT.Left Margin:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_CENTERED_X.Centered (X):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_RIGHT.Right Margin:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_RIGHT.Right Edge:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_TOP.Top Edge:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_TOP.Top Margin:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_CENTERED_Y.Centered (Y):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_BOTTOM.Bottom Margin:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_BOTTOM.Bottom Edge:elink.
:esl.
If any of these options is currently in effect for the selected graphic, they
are check-marked.

:h4 res=IDM_BITMAP_POSITION_NEXT_CHAR.Graphic Position to Next Character (Menu Option)
When one selects this menu option, the selected graphic is treated as a large character
inserted in the document text.  Wherever the graphic code was inserted into the
document, the graphic appears, expanding the document text vertically and horizontally
to accommodate itself.

:h4 res=IDM_BITMAP_POSITION_NEXT_LINE.Graphic Position to Next Line (Menu Option)
This function is not yet implemented.

:h4 res=IDM_BITMAP_POSITION_EDGE_LEFT.Graphic Position to Left Edge (Menu Option)
When one selects this menu option, the selected graphic is repositioned to the
left edge of the document page, subject to the current form&apos.s size and clipping
distances.  If a different form is selected, the graphic position will be adjusted
accordingly.

:h4 res=IDM_BITMAP_POSITION_MARGIN_LEFT.Graphic Position to Left Margin (Menu Option)
When one selects this menu option, the selected graphic is repositioned to the left
margin of the document page.  If the margins are adjusted, the graphic position
will be adjusted accordingly.

:h4 res=IDM_BITMAP_POSITION_CENTERED_X.Graphic Position Centered (X) (Menu Option)
When one selects this menu option, the selected graphic is centered horizontally.
Normally, this centering will take place with respect to the current margins.
If the graphic size option has been set to
:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Graphic Size Edge-to-Edge (X):elink.,
then the centering will take place with respect to the currently selected form
clipping limits (edges).

:h4 res=IDM_BITMAP_POSITION_MARGIN_RIGHT.Graphic Position to Right Margin (Menu Option)
When one selects this menu option, the selected graphic is repositioned to the right
margin of the document page.  If the margins are adjusted, the graphic position
will be adjusted accordingly.

:h4 res=IDM_BITMAP_POSITION_EDGE_RIGHT.Graphic Position to Right Edge (Menu Option)
When one selects this menu option, the selected graphic is repositioned to the
right edge of the document page, subject to the current form&apos.s size and clipping
distances.  If a different form is selected, the graphic position will be adjusted
accordingly.

:h4 res=IDM_BITMAP_POSITION_EDGE_TOP.Graphic Position to Top Edge (Menu Option)
When one selects this menu option, the selected graphic is repositioned to the
top edge of the document page, subject to the current form&apos.s size and clipping
distances.  If a different form is selected, the graphic position will be adjusted
accordingly.

:h4 res=IDM_BITMAP_POSITION_MARGIN_TOP.Graphic Position to Top Margin (Menu Option)
When one selects this menu option, the selected graphic is repositioned to the top
margin of the document page.  If the margins are adjusted, the graphic position
will be adjusted accordingly.

:h4 res=IDM_BITMAP_POSITION_CENTERED_Y.Graphic Position Centered (Y) (Menu Option)
When one selects this menu option, the selected graphic is centered vertically.
Normally, this centering will take place with respect to the current margins.
If the graphic size option has been set to
:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Graphic Size Edge-to-Edge (Y):elink.,
then the centering will take place with respect to the currently selected form
clipping limits (edges).

:h4 res=IDM_BITMAP_POSITION_MARGIN_BOTTOM.Graphic Position to Bottom Margin (Menu Option)
When one selects this menu option, the selected graphic is repositioned to the bottom
margin of the document page.  If the margins are adjusted, the graphic position
will be adjusted accordingly.

:h4 res=IDM_BITMAP_POSITION_EDGE_BOTTOM.Graphic Position to Bottom Edge (Menu Option)
When one selects this menu option, the selected graphic is repositioned to the
bottom edge of the document page, subject to the current form&apos.s size and clipping
distances.  If a different form is selected, the graphic position will be adjusted
accordingly.

:h3 res=IDM_BITMAP_SIZE.Graphic Size Menu
From this submenu it is possible to select a number of options which affect the
sizing of a graphic on the document page.
These options include the following&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Edge-to-Edge (X):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_MARGINTOMARGIN_X.Margin-to-Margin (X):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Edge-to-Edge (Y):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_MARGINTOMARGIN_Y.Margin-to-Margin (Y):elink.
:esl.
If any of these options is currently in effect for the selected graphic, they
are check-marked.

:h4 res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Graphic Size Edge-to-Edge (X) (Menu Option)
When one selects this menu option, the selected graphic is resized so that it occupies
the full width of the page, from edge to edge, unless the graphic is also marked for
preservation of its aspect ratio (see :link reftype=hd res=IDM_BITMAP_ASPECT.Graphic Aspect Menu:elink.).
In that case, it is possible the graphic will be somewhat narrower.

:h4 res=IDM_BITMAP_SIZE_MARGINTOMARGIN_X.Graphic Size Margin-to-Margin (X) (Menu Option)
When one selects this menu option, the selected graphic is resized so that it occupies
the width of the page, from margin to margin, unless the graphic is also marked for
preservation of its aspect ratio (see :link reftype=hd res=IDM_BITMAP_ASPECT.Graphic Aspect Menu:elink.).
In that case, it is possible the graphic will be somewhat narrower.

:h4 res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Graphic Size Edge-to-Edge (Y) (Menu Option)
When one selects this menu option, the selected graphic is resized so that it occupies
the full height of the page, from edge to edge, unless the graphic is also marked for
preservation of its aspect ratio (see :link reftype=hd res=IDM_BITMAP_ASPECT.Graphic Aspect Menu:elink.).
In that case, it is possible the graphic will be somewhat shorter.

:h4 res=IDM_BITMAP_SIZE_MARGINTOMARGIN_Y.Graphic Size Margin-to-Margin (Y) (Menu Option)
When one selects this menu option, the selected graphic is resized so that it occupies
the height of the page, from margin to margin, unless the graphic is also marked for
preservation of its aspect ratio (see :link reftype=hd res=IDM_BITMAP_ASPECT.Graphic Aspect Menu:elink.).
In that case, it is possible the graphic will be somewhat shorter.

:h3 res=IDM_BITMAP_ASPECT.Graphic Aspect Menu
From this submenu it is possible to select whether or not a graphic will be
retained at fixed proportions, and in which way.
These options include the following&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FLOAT.Unfixed:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FIX_X.Fixed X:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FIX_Y.Fixed Y:elink.
:esl.
If any of these options is currently in effect for the selected graphic, they
are check-marked.

:h4 res=IDM_BITMAP_ASPECT_FLOAT.Graphic Aspect Float (Menu Option)
When one selects this menu option, the selected graphic is freed from the requirement
of a fixed (1x1) aspect ratio.  Normally, this will have no immediate visible effect
on the graphic, unless it also has one of the edge-to-edge or margin-to-margin sizing
options in effect at the time, in which case the graphic may resize itself.

:h4 res=IDM_BITMAP_ASPECT_FIX_X.Graphic Aspect Fix X (Menu Option)
When one selects this menu option, the selected graphic has a restriction placed on its
sizing so that it will always be displayed with a one-to-one aspect ratio.
With this option, the controlling dimension is the width.  The height will
be adjusted to attain the one-to-one aspect.

:h4 res=IDM_BITMAP_ASPECT_FIX_Y.Graphic Aspect Fix Y (Menu Option)
When one selects this menu option, the selected graphic has a restriction placed on its
sizing so that it will always be displayed with a one-to-one aspect ratio.
With this option, the controlling dimension is the height.  The width will
be adjusted to attain the one-to-one aspect.

:h2 res=IDM_BORDERMENU.Graphic Border Menu
This menu appears when you click with the right mouse button over the
border area around a graphic object.  It includes all the menu options
that can be applied to such an area.  These include the following&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_SIZE.Border Size:elink.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_COLOR.Border Color:elink.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_CAPTION.Border Caption:elink.
:esl.

:h3 res=IDM_BORDER_SIZE.Border Size (Menu Option)
This function is not yet implemented.

:h3 res=IDM_BORDER_COLOR.Border Color (Menu Option)
This function is not yet implemented.

:h3 res=IDM_BORDER_CAPTION.Border Caption (Menu Option)
This function is not yet implemented.

:h1 res=60010.Miscellaneous Dialogs

:h2 res=IDD_PROFILE_PATH.Set Profile Path
This dialog is displayed when the program cannot find its profile (INI) file,
and asks you where the file is or where it is to be created.
:artwork name='English\Profile.bmp' align=center.
:p.Only a valid existing directory name will be accepted.
Once you have entered the name, press the ENTER key or click on the OK
button for the program to continue.
:p.If you wish to abort the program&apos.s initialization sequence, press
the ESC key or click on the escape button.

:h2 res=IDD_PROMPT_SAVE.Save Changed Document?
.* :artwork name='English\SaveChng.bmp' align=center.
You are trying to exit the program or close a document window,
but you have changes to the current document which have not yet
been saved.  Answer &apos.Y&apos.es or click on the YES button if you
wish to save the changes to the current file name.
Answer &apos.N&apos.o or click on the NO button if you do not wish to
do so (you don&apos.t care about the changes).
Press the ESC key or click on the Escape button if you wish
to abort the program termination or document close operation
in progress.

:h2 res=IDD_PROMPT_SAVECONFIRM1.Save Confirm
If this dialog has been displayed, then you are attempting to save your
document with a new name over a file that already exists.
.* :artwork name='English\SaveConf.bmp' align=center.
:p.If you really wish to replace the already existing file, then either
press the &apos.Y&apos. key or click on the Yes button.  If you do not wish to replace
the file on disk, simply press Enter or the &apos.N&apos. key.  In this case, your
document will not have been saved, and you will have to try again giving
a different name.

:h2 res=IDD_PROMPT_SAVECONFIRM2.Type Change Confirm
If this dialog has been displayed, then you are attempting to save your document
with a changed file type.
.* :artwork name='English\SaveCnf2.bmp' align=center.
:p.If you really wish to change the file type, then either press the
&apos.Y&apos. key or click on the Yes button.
If you do not wish to change the file type, simply press Enter or the &apos.N&apos. key.
In this case, your document will not have been saved, and you will have to
try again, specifying a different name.

:h2 res=IDD_MESSAGE_SPELLDONE.Speck-Check Complete
If this dialog has been displayed, then you have just completed a scan
of the current document with the spell checker.
.* :artwork name='English\SpelDone.bmp' align=center.
:p.To continue, press the ENTER key or click on the Enter pushbutton with
the mouse.

:h2 res=IDD_ERROR_HELPMANAGER.Help Manager Error
If this message has been displayed, then something&apos.s wrong with the
application help file.  Please inform the application programmer.

:h2 res=IDD_ERROR_EXCEPTION.Program Exception Abort
.* :artwork name='English\Except.bmp' align=center.
If this dialog has been displayed, then Escriba has had a fatal
error occur.
Please inform the author using one of the means listed in the
:link reftype=hd res=60008.Questions and Bug Reporting:elink. page
and include the file ESCRIBA.LOG, which should be present in
the directory to which you installed Escriba.

:h2 res=IDD_ERROR_WINREGISTERCLASS.Class Registration Error
If this message has been displayed, the application could
not register one of its window classes properly.
Please inform the author using one of the means listed in the
:link reftype=hd res=60008.Questions and Bug Reporting:elink. page.

:h2 res=IDD_ERROR_CREATECLIENT.Client Create Error
If this dialog has been displayed, then Escriba was not able
to create a client window for some reason.
Please inform the author using one of the means listed in the
:link reftype=hd res=60008.Questions and Bug Reporting:elink. page.

:h2 res=IDD_ERROR_TOOMANYDOCUMENTS.Too Many Documents Error
If this dialog has been displayed, you have tried to load
too many documents into Escriba, and the error was detected
after a document was loaded into memory.

:h2 res=IDD_ERROR_TOOMANYLOADED.Too Many Documents Loaded Error
If this dialog has been displayed, you already have too
many documents loaded to create yet another document window.

:h2 res=IDD_ERROR_DOCUMENT_CREATE.Document Create Error
If this dialog has been displayed, then Escriba was not able
to create a document object for some reason.
Please inform the author using one of the means listed in the
:link reftype=hd res=60008.Questions and Bug Reporting:elink. page.

:h2 res=IDD_ERROR_DOCUMENT_LOAD.Document Load Error
If this dialog has been displayed, then Escriba was not able
to load a document for some reason.  Please verify the file
name and all the directories in its path.  It is also possible
that the file is currently in use by another program and for
that reason cannot be opened for reading.

:h2 res=IDD_ERROR_DOCUMENT_SAVE.Document Save Error
If this dialog has been displayed, then Escriba was not able
to open the file to which you requested it save a document.
Please verify the file name to insure that it is valid, and
that all the directories in the path are valid also.  It is
also possible that the file is currently in use by another
program, and for that reason cannot be opened for writing.

:h2 res=IDD_ERROR_CREATEPRINTSTATUS.Print Status Window Create Error
If this dialog has been displayed, then Escriba was not able to
initialize its Print Status window.  This should never happen, but
has been provided for for diagnostic purposes.
Please inform the author using one of the means listed in the
:link reftype=hd res=60008.Questions and Bug Reporting:elink. page.

:h2 res=IDD_ERROR_CREATELOADSTATUS.Load Status Window Create Error
If this dialog has been displayed, then Escriba was not able to
initialize its Load Status window.  This should never happen, but
has been provided for for diagnostic purposes.
Please inform the author using one of the means listed in the
:link reftype=hd res=60008.Questions and Bug Reporting:elink. page.

:h2 res=IDD_ERROR_BLOCKMEMORY.Block Copy Memory Error
If this dialog has been displayed, then there was not enough memory
available to cut or copy the selected text to the system clipboard.
Try performing the copy in smaller segments.

:h2 res=IDD_ERROR_BADGRAPHICFILE.Bad Graphic File Error
If this dialog has been displayed, then you have attempted to load
as a graphic image a file which is either of an unrecognized type or
contains invalid data.  The currently supported graphic file types are
BMP, GIF, .JPG, PCX, PCC, MET and TIF.  If the file you tried to load is of one
of these types, and you can display the image with a regular image
display program, then
please inform the author using one of the means listed in the
:link reftype=hd res=60008.Questions and Bug Reporting:elink. page.

:h2 res=IDD_ERROR_VERIFY1.Verify Error - Open
If this error message has been displayed, then the file specified
for loading could not be opened for some reason.
Verify the file name and path and try again.

:h2 res=IDD_ERROR_VERIFY2.Verify Error - Memory
If this error message has been displayed, then Escriba
could obtain sufficient memory to load the specified file
into memory.

:h2 res=IDD_ERROR_VERIFY3.Verify Error - Invalid Escapes
If this error message has been displayed, then the file specified
for loading in the native Escriba format contained invalid
escape codes.
The file may have become corrupted.

:h2 res=IDD_ERROR_VERIFY4.Verify Error - Invalid Functions
If this error message has been displayed, then the file specified
for loading in the native Escriba format contained function codes
not recognized by this version of Escriba.
The file may have been created by a newer version of Escriba,
or it may have become corrupted.

:h2 res=IDD_ERROR_VERIFY5.Verify Error - Nulls
If this error message has bee displayed, then the file specified
for loading in the native Escriba format contained null characters
outside of normal function codes.
The file may have become corrupted.

:h2 res=IDD_ERROR_VERIFY6.Verify Error - Unmatched Headers/Trailers
If this error message has been displayed, then the file specified
for loading in the native Escriba format contained unmatched
function header and trailer bytes.
The file may have become corrupted.

:h2 res=IDD_ERROR_VERIFY7.Verify Error - No Preface
If this error message has been displayed, then the file specified
for loading in the native Escriba format did not start with a
Preface function.
The file may have become corrupted.

:h2 res=IDD_ERROR_VERIFY8.Verify Error - No Valid Type
If this error message has been displayed, then the file specified
for loading was not accepted by any of the currently valid file
type verifiers.  Escriba cannot load such a file.
:note.New file type processors may be developed at any time.  If
you have a file type you wish to load, but currently cannot,
send your request to the author at one of the addresses specified
on the :link reftype=hd res=60008.Questions and Bug Reporting:elink. page.

:h2 res=IDD_ERROR_IMPORT1.Import Error - Open
If this error message has been displayed, then the file specified
for loading could not be opened for some reason.
Verify the file name and path and try again.

:h2 res=IDD_ERROR_IMPORT2.Import Error - Memory
If this error message has been displayed, then Escriba
could obtain sufficient memory to load the specified file
into memory.

:h2 res=IDD_ERROR_EXPORT1.Export Error - Open/Create
If this error message has been displayed, then the file specified
for saving could not be opened for some reason.
Verify the file name and path and try again.
If the file already exists, it is possible that it has
been marked read-only.

:h2 res=IDD_ERROR_EXPORT2.Export Error - Disk Space
If this error message has been displayed, then the file specified
could not be fully written to disk for some reason.
Check that there is sufficient space on the destination disk drive.

:h2 res=IDD_METAFILE_VERIFY1.Metafile Verify Error - Open
If this error message has been displayed, then the metafile selected
for verification and loading could not be opened.  Normally this
means that the file is currently opened for writing by another
application.

:h2 res=IDD_METAFILE_VERIFY2.Metafile Verify Error - Short File
If this error message has been displayed, then the metafile
selected for verification and loading was not long enough.

:h2 res=IDD_METAFILE_VERIFY3.Metafile Verify Error - Load
If this error message has been displayed, then it was not possible
to load a metafile into memory for verification.  If this error
occurs repeatedly,
then please notify the author at one of the addresses specified
on the :link reftype=hd res=60008.Questions and Bug Reporting:elink. page.

:h2 res=IDD_METAFILE_VERIFY4.Metafile Verify Error - Invalid Data
If this error message has been displayed, then the metafile header
contained invalid information.
Please verify that the metafile can be viewed or loaded with the OS/2
PICVIEW program.  If it can be loaded by that program without errors, 
then please notify the author at one of the addresses specified
on the :link reftype=hd res=60008.Questions and Bug Reporting:elink. page.

:h2 res=IDD_ERROR_LOADLANGUAGE.Language Select Error
If this error message has been displayed, then the language selected
for loading could not be fully loaded for some reason.
Please notify the author at one of the addresses specified
on the :link reftype=hd res=60008.Questions and Bug Reporting:elink. page.

:h1 res=61000.Technical Reference

:h2 res=61001.Document File Format
The document file can be considered a stream of characters and
function codes.  With the exception of the control codes 15, 16 and 17,
all character data is stored without change.  Codes 15, 16 and 17
are escaped, with code 15 being the escape code.  That is, if the
original data stream contained a literal code 16, then the file
data would contain code 15 followed by code 16 to represent the
original code 16.  If code 15 is encountered without a code 15, 16 or 17
after it, it is also interpreted as a literal code 15.
:p.
A literal code 17 &apos.&larrow.&apos. defines a function code start.
A literal code 16 &apos.&rarrow.&apos. defines a function code terminator.
If a function code terminator is found without there having been a corresponding
function code start, it is passed through as a literal character.
:p.
The following functions are currently valid&colon.

:lm margin=3.:p.&larrow.Version&colon.:hp5.versionnumber:ehp5.&rarrow.
:lm margin=5.:p.
This function code specifies the version of Escriba used to edit and save
the document to disk.  The version number has four digits, which are treated
as if they had a decimal point between the first two and the second two digits.
Version 1.00 would be represented as 0100.  This function code is used to
allow document format conversion from older to newer formats, and did not
exist in the file formats prior to beta revision 0.92.
This code cannot appear outside of the parameter data area of the &apos.Preface&apos. function code.

:lm margin=3.:p.&larrow.Preface&colon.:hp5.functioncodes:ehp5.&rarrow.
:lm margin=5.:p.
This is a special function code which appears in the
saved document file but not in the document as viewed by the user for
editing.  This function&apos.s parameter data consists of other function
codes.  The combined effect of all the function codes is to build
the document&apos.s default state.  All function codes which alter the
document&apos.s state are valid here, along with the Printer, Form, JobSettings
TabSpacing and Metric functions, which are not valid elsewhere.  Function codes
which generate text output (PageCount, PageNumber, Graphic, Date) are
not valid here.

:lm margin=3.:p.&larrow.Printer&colon.:hp5.name:ehp5.&rarrow.
:lm margin=5.:p.
This function code specifies the name of the printer for which this
document was formatted.  This code cannot appear outside of the parameter
data area of the &apos.Preface&apos. function code.

:lm margin=3.:p.&larrow.Form&colon.:hp5.name:ehp5.&rarrow.
:lm margin=5.:p.
This function code is used to specify the name of the form for which the document was formatted.  
It is only used when a document formatted for one printer is being reformatted for another.
This code cannot appear outside of the parameter area of the &apos.Preface&apos. function code.

:lm margin=3.:p.&larrow.CodePage&colon.:hp5.number:ehp5.&rarrow.
:lm margin=5.:p.
This function code is used to specify the number of the codepage to be used
when displaying the document.
This code cannot appear outside of the parameter area of the &apos.Preface&apos. function code.
See :link reftype=hd res=61002.Code Pages:elink. for more information.

:lm margin=3.:p.&larrow.Font&colon.:hp5.name:ehp5.&rarrow.
:lm margin=5.:p.
This function code specifies the name of the font to be used from its location
in the document onwards.  This name must be of a valid scalable font
that appears in the system Font Palette object, in a normal system Font Dialog,
or in Escriba&apos.s Font menu.

:lm margin=3.:p.&larrow.Size&colon.:hp5.pointsize:ehp5.&rarrow.
:lm margin=5.:p.
This code specifies the printer&apos.s point size to be used from its location in
the document onwards.  A printer&apos.s point is 1/72 inch, in English units.
When the size setting is to 12 points, the normal size of the characters
generated will be about 1/6 inches high, though this is a guideline for
font designers, not a firm rule.  In general, fonts will not all appear
the same size even when all are generated at the same point size.  Escriba
will accept point sizes to a resolution of 1/10 point.

:lm margin=3.:p.&larrow.Color&colon.:hp5.color:ehp5.&rarrow.
:lm margin=5.:p.
This code specifies the color (red-blue-green) to be used for line and text
output from its location in the document onwards.  The code is specified as
a six-digit hexadecimal code, with the first two digits giving the red component,
the second pair giving the green and the last pair specifying the blue portion
of the color.  White, for instance, would be &apos.FFFFFF&apos., black is &apos.000000&apos., and
yellow would be &apos.FFFF00&apos..

:lm margin=3.:p.&larrow.LineWidth&colon.:hp5.width:ehp5.&rarrow.
:lm margin=5.:p.
This function sets the line (stroke) width to be used to be used in drawing
outlined characters, when the outline attribute is set.  The width is specified
as any other distance.

:lm margin=3.:p.&larrow.LineEnd&colon.:hp5.type:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the line end type to be used beyond
the current position.
The valid types include all the LINEEND codes defined in the
OS/2 Programmer&apos.s Reference.

:lm margin=3.:p.&larrow.LineJoin&colon.:hp5.type:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the line join type to be used beyond
the current position.
The valid types include all the LINEJOIN codes defined in the
OS/2 Programmer&apos.s Reference.

:lm margin=3.:p.&larrow.FillColor&colon.:hp5.color:ehp5.&rarrow.
:lm margin=5.:p.
This code specifies the color (red-blue-green) to be used to fill outlined
characters from its location in the document onwards.  The code is specified
in the same was as the line/text color (see above).

:lm margin=3.:p.&larrow.FillType&colon.:hp5.type:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the outlined text fill pattern type
for the text beyond the current position.
The valid types include all the PATSYM codes defined in the
OS/2 Programmer&apos.s Reference.

:lm margin=3.:p.&larrow.Bold&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the bold attribute on or off for
the text beyond the current position.

:lm margin=3.:p.&larrow.Italic&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the italic attribute on or off for
the text beyond the current position.

:lm margin=3.:p.&larrow.Underscore&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the underscore attribute on or off for
the text beyond the current position.

:lm margin=3.:p.&larrow.Strikeout&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the strikeout attribute on or off for
the text beyond the current position.

:lm margin=3.:p.&larrow.Outline&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the outline (hollow) attribute on or off
for the text beyond the current position.

:lm margin=3.:p.&larrow.Caps&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the uppercase attribute on or off for
the text beyond the current position.

:lm margin=3.:p.&larrow.Expand&colon.:hp5.percent:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the horizontal text expansion factor to be
used from the current position in the document onwards.
The factor is expressed as a percentage, with 100 indicating that
the normal horizontal expansion defined for the current font is
to be used.

:lm margin=3.:p.&larrow.TopMargin&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the current position of the top margin to the
specified value, which is relative to the bottom edge of the page.
This attribute takes effect on the next page, unless nothing displayable has
appeared on the current page yet.

:lm margin=3.:p.&larrow.BottomMargin&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the current position of the bottom margin to the
specified value.
This attribute takes effect on the next page, unless nothing displayable has
appeared on the current page yet.

:lm margin=3.:p.&larrow.LeftMargin&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the current position of the left margin to the
specified value.
This attribute takes effect on the next line, unless nothing displayable has
appeared on the current line yet.

:lm margin=3.:p.&larrow.RightMargin&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the current position of the right margin to the
specified value, which is relative to the left edge of the page.
This attribute takes effect on the next line, unless nothing displayable has
appeared on the current line yet.

:lm margin=3.:p.&larrow.LeftJustify&rarrow.
:lm margin=5.:p.
This function code sets the current horizontal text justification to &apos.Left&apos.,
causing text lines to be aligned against the left margin.
This attribute takes effect on the next line, unless nothing displayable has
appeared on the current line yet.

:lm margin=3.:p.&larrow.CenterJustify&rarrow.
:lm margin=5.:p.
This function code sets the current horizontal text justification to &apos.Center&apos.,
causing text lines to be centered between the left and right margins.
This attribute takes effect on the next line, unless nothing displayable has
appeared on the current line yet.

:lm margin=3.:p.&larrow.RightJustify&rarrow.
:lm margin=5.:p.
This function code sets the current horizontal text justification to &apos.Right&apos.,
causing text lines to be aligned against the right margin.
This attribute takes effect on the next line, unless nothing displayable has
appeared on the current line yet.

:lm margin=3.:p.&larrow.FullJustify&rarrow.
:lm margin=5.:p.
This function code sets the current horizontal text justification to &apos.Full&apos.,
causing full lines to be padded to have even left and right margins.
Partial lines are left justified without any additional padding.
This attribute takes effect on the next line, unless nothing displayable has
appeared on the current line yet.

:lm margin=3.:p.&larrow.CenterPage&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the current state of the &apos.CenterPage&apos. attribute.
When active at the effective start of a page, that page is centered vertically.

:lm margin=3.:p.&larrow.TabSet&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
This function code causes a tab to be added at the specified horizontal
distance to the current tab table.

:lm margin=3.:p.&larrow.TabClear&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
This function code causes any tab present at the specified horizontal distance
to be removed from the current tab table.

:lm margin=3.:p.&larrow.TabSpacing&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
This function code specifies the distance to be used between tabs at
the start of the document.  Starting at the left margin, tabs are created
automatically at the interval specified here.  This code cannot appear outside
of the parameter data area of the &apos.Preface&apos. function code.

:lm margin=3.:p.&larrow.Indent&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
This function code specifies the distance to be used for indenting paragraph
starting lines.  Paragraph starting lines are either (1) the first line of the
document, or (2) the first line of text after a newline code, marking the end
of a paragraph.

:lm margin=3.:p.&larrow.Metric&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
This function code indicates whether the document is to be formatted with
metric measurements (centimeters) or in english measurements (inches).  If
set to &apos.1&apos., metric measurements are used.
This code cannot appear outside of the parameter data area of the &apos.Preface&apos. function code.

:lm margin=3.:p.&larrow.Graphic&colon.:hp5.x,y,cx,cy[,Background][,NoTextAside][,Halftone][,Reverse][,PrinterOptimize][[,KeepAspectFixX|KeepAspectFixY]]&colon.type&bsl.0data:ehp5.&rarrow.
:lm margin=5.:p.
This function code defines a graphic cell within the document.
:p.The &apos.x&apos. and &apos.y&apos. coordinates define the cell&apos.s position on the page, relative
to the lower-left corner of the page (not the upper-left).
The &apos.cx&apos. and &apos.cy&apos. parameters specify the cell&apos.s width and height.
All four of these parameters are specified in the measurement system currently
in use (1/1000 inch or centimeter).
:p.As well as numeric coordinate values, the keywords 
&apos.Inline&apos.,
&apos.LeftMargin&apos., 
&apos.RightMargin&apos., 
&apos.LeftEdge&apos., 
&apos.RightEdge&apos. and 
&apos.Centered&apos. 
are accepted for the X position, and 
&apos.TopMargin&apos., 
&apos.BottomMargin&apos., 
&apos.TopEdge&apos., 
&apos.BottomEdge&apos. and 
&apos.Centered&apos. 
are accepted for the Y position.  The keywords 
&apos.EdgeToEdge&apos. and
&apos.MarginToMargin&apos. 
are accepted for either the X or Y widths.
If &apos.Inline&apos. is specified for the X position, then the Y position is ignored.
Likewise, if &apos.EdgeToEdge&apos. or &apos.MarginToMargin&apos. is specified for the X or Y size,
then the corresponding position is ignored.
:p.The &apos.Background&apos., &apos.NoTextAside&apos., &apos.Halftone&apos. and &apos.Reverse&apos. parameters are optional.
They are also case sensitive.
:p.The &apos.Background&apos. parameter, when specified, indicates that the graphic is not
to interfere with the positioning of text on the page, and is to be painted
behind the text, as a background.
:p.The &apos.NoTextAside&apos. parameter, when specified, indicates that no text is to
be formatted to either side of the graphic, treating the graphic as if it occupied
the entire width of the page.
:p.The &apos.Halftone&apos. parameter, when specified, indicates that the graphic is
to be painted in muted colors.  Normally, this is done by painting only every
alternate pixel, and for this reason is not supported for Metafile graphics.
:p.The &apos.Reverse&apos. parameter, when specified, indicates that the graphic is
to be painted in inverted colors.  This cannot be supported for Metafile graphics.
:p.The &apos.PrinterOptimize&apos. parameter, when specified, indicates that the graphic
is to be optimized for the printer resolution.  This is done by scaling the image so
that when printed, the bitmap scaling factor will be an integer.
This is not supported for Metafile graphics.
:p.The &apos.KeepAspectFixX&apos. parameter declares that the graphic height will be determined
by the graphic width and the original graphic size, maintaining the original graphic
aspect ratio.  The &apos.KeepAspectFixY&apos. parameter is similar, except that the graphic width
is determined by the height and the original graphic size.
:p.The &apos.Type&apos. field can currently be set to
&apos.BM&apos. (for bitmap), 
&apos.MF&apos. (for metafile),
&apos.UP&apos. (for UPC-A Code),
&apos.UE&apos. (for UPC-E Code),
&apos.39&apos. (for Code 39),
&apos.PN&apos. (for POSTNET),
&apos.E8&apos. (for EAN-8) or
&apos.EA&apos. (for EAN-13).
All BMP, GIF, JPG, PCX, TIF and such graphics are stored internally in OS/2 bitmap form.
This field is null terminated.
:p.Binary graphic data is placed after the null-terminated header information.
This binary data has the codes 15, 16, and 17 escaped as described earlier in this section.
:p.For bitmaps, the binary information first contains a four-byte number indicating the size
of the bitmap information header.  Following this is a four-byte number indicating the size
of the bitmap data itself.  After this is found the bitmap information header, and
after that the bitmap data.
:p.For metafiles, the binary information includes a four-byte number giving the size
of the metafile data, and then the metafile data itself.
:p.For UPC-A Codes, the data is a 11-digit text containing the UPC-A Code itself.
:p.For UPC-E Codes, the data is a 7-digit text containing the UPC-E Code itself.
:p.For Code-39 barcodes, the data can be a text of any length, but character data itself is
limited to the characters A-Z (upper-case), 0-9, space, 
&apos.&dot.&apos.,
&apos.&plus.&apos.,
&apos.&dash.&apos.,
&apos.&asterisk.&apos., 
&apos.&slash.&apos.,
&apos.&dollar.&apos. and
&apos.&percent.&apos..
:p.For POSTNET barcodes, the data is a 5 or 9 digit text with numeric digits only.
:p.For EAN-8 barcodes, the data is a 7 digit text with numeric digits only.
:p.For EAN-13 barcodes, the data is a 12 digit text with numeric digits only.

:lm margin=3.:p.&larrow.Date&colon.Fixed&colon.:hp5.time:ehp5.&rarrow.
:lm margin=5.:p.
This function code generates text showing a specific date, which date is specified
in seconds since 00&colon.00, 1 January 1970.
The text format is specified with the &apos.DateFormat&apos. function.

:lm margin=3.:p.&larrow.Date&colon.Current&rarrow.
:lm margin=5.:p.
This function code generates text showing the current date according to the
computer&apos.s calendar.
The text format is specified with the &apos.DateFormat&apos. function.

:lm margin=3.:p.&larrow.Date&colon.Changed&rarrow.
:lm margin=5.:p.
This function code generates text showing the date when the document
was last changed.
The text format is specified with the &apos.DateFormat&apos. function.

:lm margin=3.:p.&larrow.DateFormat&colon.:hp5.formatstring:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets a text string which will be used from the code&apos.s
location in the document text onwards as the template for generating full
date strings.  It is similar to a &apos.C&apos. &apos.printf&apos. format string, except that
the codes that may validly follow the &apos.&percent.&apos. character are as follows&colon.
:lm margin=10.:sl compact.
:li.m - Numeric Month (1,2,etc)
:li.M - Text Month (January, February, etc)
:li.n - Text Month (Jan, Feb, etc)
:li.d - Numeric Day
:li.y - Numeric Year (4 digits, including century)
:li.z - Numeric Year (2 digits, excluding century)
:li.w - Text Weekday (Sunday, Monday, etc)
:li.x - Text Weekday (Sun, Mon, etc)
:esl.:lm margin=5.
:note.This internal format does not vary with language.  The codes the
user sees may vary from language to language.

:lm margin=3.:p.&larrow.LineSpacing&colon.:hp5.percent:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the percentage of normal line spacing to be used from
the code&apos.s location in the document text onwards.

:lm margin=3.:p.&larrow.JobSettings&colon.:hp5.driverdata:ehp5.&rarrow.
:lm margin=5.:p.
This function code contains as parameter data the entire printer job settings
structure returned from the printer job settings dialog.  The complete structure
of this data depends on the printer driver, and may or may not be documented
by the printer driver&apos.s author (not by me), although the data has a standard
header structure documented by IBM.  This data is written with codes 15, 16 and
16 escaped, as it is assumed to be binary.  This code cannot appear outside
of the parameter data area of the &apos.Preface&apos. function code.

:lm margin=3.:p.&larrow.PageNumber&rarrow.
:lm margin=5.:p.
This function code generates text showing
the current page number at the function code&apos.s position within the document.

:lm margin=3.:p.&larrow.PageCount&rarrow.
:lm margin=5.:p.
This function code generates text showing
the current count of pages in the document.

:lm margin=3.:p.&larrow.Glyph&colon.:hp5.number:ehp5.&rarrow.
:lm margin=5.:p.
This function code generates a single text character whose numeric value
is the number given.  The precise character displayed or printed will
depend on the codepage and font in use.
This function code is created when the the Alt+nnn technique is used to
create arbitrary character codes from the numeric keypad, and eliminates
the confusion as to whether or not a carriage-return or linefeed code (for example)
should be interpreted as such, or as a displayable character.

:lm margin=3.:p.&larrow.Author&colon.:hp5.name:ehp5.&rarrow.
:lm margin=5.:p.
This function code only appears in the Preface section and serves to declare
the document author&apos.s name.

:lm margin=3.:p.&larrow.CreateDate&colon.:hp5.time:ehp5.&rarrow.
:lm margin=5.:p.
This function code only appears in the Preface section and is used to save
the original creation date of the document.  It is stored in the same way
as the other dates, that is in seconds since January 1st, 1970.

:lm margin=3.:p.&larrow.Comment&colon.:hp5.commentary:ehp5.&rarrow.
:lm margin=5.:p.
This function code can be used to embed a comment into the document body.
The comment will not be displayed nor printed in rendering the document itself.

:lm margin=3.:p.&larrow.Separator&rarrow.
:lm margin=5.:p.
This function code generates a line break, followed by a separator line,
followed by another line break.  It performs much the same function as the
<hr> function of the HTML language.

:lm margin=3.:p.&larrow.Title&colon.:hp5.name:ehp5.&rarrow.
:lm margin=5.:p.
This function code only appears in the Preface section and serves to declare
the document title.

:lm margin=3.:p.&larrow.Header&colon.:hp5.text:ehp5.&rarrow.
:lm margin=5.:p.
This function code specifies the page header text to be used from this point
onwards in formatting pages.  If nothing printable has yet appeared on the
page, it will be used for the current page as well.

:lm margin=3.:p.&larrow.Footer&colon.:hp5.text:ehp5.&rarrow.
:lm margin=5.:p.
This function code specifies the page footer text to be used from this point
onwards in formatting pages.  If nothing printable has yet appeared on the
page, it will be used for the current page as well.

:lm margin=1.

:note.Distances are expressed in units of 0.001 inches or centimeters, depending
on whether you are using english or metric units for measurements.
A distance of 1/2 centimeter or inch would therefore be expressed as 500.
Generally, only positive distances may be used.

:note.Percentages are expressed as integers with an assumed decimal point.
For example, 100&percent. is expressed as the number 100, 125&percent. as the number 125,
and so on.  Generally, only positive percentages may be used.

:h2 res=61002.Code Pages
The following code pages (national language character mappings)
are valid&colon.

:table cols='4 60' rules=both frame=box.
:row.:c.037     :c.Brazil / Portuguese / U.S. English (EBCDIC)
:row.:c.273     :c.Austrian / German (EBCDIC)
:row.:c.274     :c.Belgian (old EBCDIC)
:row.:c.277     :c.Danish / Norwegian (EBCDIC)
:row.:c.278     :c.Finnish / Swedish (EBCDIC)
:row.:c.280     :c.Italian (EBCDIC)
:row.:c.282     :c.Portuguese (old EBCDIC)
:row.:c.284     :c.Spanish (EBCDIC)
:row.:c.285     :c.U.K. English (EBCDIC)
:row.:c.297     :c.French (EBCDIC)
:row.:c.437     :c.United States (IBM PC)
:row.:c.500     :c.Belgian / International (EBCDIC)
:row.:c.813     :c.Greek
:row.:c.819     :c.Multilingual (ISO-8859/1)
:row.:c.850     :c.Multilingual (Latin1)
:row.:c.852     :c.Slavic (Latin2)
:row.:c.855     :c.Serbia / Montenegro / Macedonia / Bulgaria
:row.:c.857     :c.Turkey
:row.:c.860     :c.Portuguese
:row.:c.861     :c.Iceland
:row.:c.862     :c.Hebrew
:row.:c.863     :c.French-Canadian
:row.:c.864     :c.Arabic
:row.:c.865     :c.Danish / Norwegian
:row.:c.866     :c.Russia
:row.:c.869     :c.Greek
:row.:c.870     :c.Czechoslovakia / Hungary / Poland / Yugoslavia (EBCDIC)
:row.:c.871     :c.Iceland (EBCDIC)
:row.:c.874     :c.Thailand
:row.:c.915     :c.Bulgaria / Macedonia
:row.:c.932     :c.Japan (Shift JIS X-0208)
:row.:c.934     :c.Korea
:row.:c.936     :c.Simplified Chinese
:row.:c.938     :c.Traditional Chinese
:row.:c.942     :c.Japan (SAA)
:row.:c.944     :c.Korea (SAA)
:row.:c.946     :c.Simplified Chinese (SAA)
:row.:c.948     :c.Traditional Chinese (SAA)
:row.:c.949     :c.Korea (Shift KSC 5601)
:row.:c.950     :c.Traditional Chinese (Big5)
:row.:c.1004    :c.Desktop Publishing
:row.:c.1026    :c.Turkey (EBCDIC)
:row.:c.1200    :c.Unicode
:row.:c.1250    :c.Windows, Latin-2 (Eastern European)
:row.:c.1251    :c.Windows, Cyrillic
:row.:c.1252    :c.Windows, Multilingual (ANSI)
:row.:c.1253    :c.Windows, Greek
:row.:c.1254    :c.Windows, Turkish
:row.:c.1255    :c.Windows, Hebrew
:row.:c.1256    :c.Windows, Arabic
:etable.

:note.Not all codepages may be usable on all systems.

:h2 res=61003.Add-on Extension Interface
The add-on extension facility searches Escriba&apos.s installation directory
at program startup time for all DLL files.  Each of these DLLs are
checked for the presence of the functions &apos.Identify&apos., &apos.ProductName&apos.,
&apos.ProductVersion&apos. &apos.ProductInformation&apos. and &apos.ProductConfigure&apos..
If all of these functions are present, the module is considered to be an 
add-on extension of some sort.

:p.The function prototypes for the &apos.C&apos. programming language are&colon.

:font facename=Courier size=13x8.:lm margin=6.:sl compact.
:li.char* APIENTRY Identify ( void ) ;
:li.BOOL  APIENTRY SetLanguage ( char *Language ) ;
:li.char* APIENTRY ProductName ( void ) ;
:li.int   APIENTRY ProductVersion ( void ) ;
:li.void  APIENTRY ProductInformation ( HWND Owner ) ;
:li.int   APIENTRY ProductConfigure ( int MaxPages, char Names[][64], char MajorTabs[][32], char MinorTabs[][32], PFNWP Processors[], HMODULE Libraries[], int DialogIDs[] ) ;
:esl.:lm margin=1.:font facename=default.

:p.The Identify function returns a null-terminated character string which 
identifies the extension type.  Currently, the types 
:link reftype=hd res=61004."ISPELLER":elink.
, 
:link reftype=hd res=61005."IMPORT/EXPORT":elink.
and 
:link reftype=hd res=61006."GRAPHICIMPORT":elink.
are defined.

:p.The SetLanguage function allows Escriba to configure the extension&apos.s language.
The normal name for the language in the language itself is used.
For example, the word for French is Fran&cc.ais.

:p.The ProductName function returns a null-terminated character string 
which will be used to identify the extension in the Product Information 
menu.  This name should not include the product version number.

:p.The ProductVersion function returns an integer number which represents 
the product version number multiplied by 100.  This is also used for the 
Product Information menu.

:p.The ProductInformation function displays a dialog which gives the user 
the product&apos.s full name, version number and date, and any authorship 
and copyright information that pertains to the product.  This function 
will be called when the user selects the product from the Product 
Information menu.

:p.The ProductConfigure function returns information to be used in constructing
the Configure Extension dialog.  This function is not required to be present,
but when it is present it is expected to return the number of configuration
notebook pages it wishes to provide.  For each of these pages (up to the
maximum specified in the function call), a page name, major tab text or
minor tab text (one or the other), a dialog processor function, a resource
library module handle and a resource ID for the dialog template must all
be returned to the calling function.

:h3 res=61004.ISpeller Interface
The module "ISPELLER" was developed from the GNU ISPELL program, version 3.1.18, and
as such is freeware, source-code included.  The copyrights belong to Pace Willisson
and Geoff Kuenning.  ISPELLER itself is simply a wrapper around ISPELL, but redone
as an OS/2 dynamic-link library (DLL).  ISPELLER cannot make use of another version
of ISPELL 3.1.x resident on your system, but -can- use the dictionary files from such.
To do this, simply insert the statement &apos.SET ISPELL=path&apos. in your CONFIG.SYS, where
&apos.path&apos. is the fully qualified path to where your ISPELL dictionary files are kept.
The modules ISPELLER.DLL and BUILDHAS.EXE should also be copied to that directory.

:p.In addition to the basic functions of an add-on extension, the following
functions are provided by ISpeller&colon.

:font facename=Courier size=13x8.:lm margin=6.:sl compact.
:li.void  APIENTRY Speller_SetHome ( char *Path ) ;
:li.char* APIENTRY Speller_DefaultDictionary ( void ) ;
:li.int   APIENTRY Speller_OpenDictionary ( char *Path ) ;
:li.void  APIENTRY Speller_CloseDictionary ( ) ;
:li.BOOL  APIENTRY Speller_Ready ( ) ;
:li.int   APIENTRY Speller_BuildWord ( char* (*APIENTRY GetPtr)(void *Object,long *Offset,long Delta), void *Object, char *Word, long *Start, long *End, long *Next ) ;
:li.int   APIENTRY Speller_FindWord ( char *Word ) ;
:li.int   APIENTRY Speller_FindReplacements ( char *Word, int ListSize, int EntrySize, void *List, int *EasyCount ) ;
:li.int   APIENTRY Speller_Accept ( char *Word ) ;
:li.int   APIENTRY Speller_Insert ( char *Word ) ;
:li.int   APIENTRY Speller_UnCaps ( char *Word ) ;
:esl.:lm margin=1.:font facename=default.

:p.Those interested in what these functions do, and how they do it, are referred
to the source code.

:h3 res=61005.Document Import/Export Module Interface
Modules of type "IMPORT/EXPORT" are used to import documents not of Escriba&apos.s 
native format into the program, and to export documents to files not of
Escriba&apos.s native format.

:p.In addition to the basic functions of an add-on extension, the following
functions are provided by Import/Export modules&colon.

:font facename=Courier size=13x8.:lm margin=6.:sl compact.
:li.char* APIENTRY QueryFileType ( void ) ;           
:li.char* APIENTRY QueryWildcard ( void ) ;           
:li.BOOL  APIENTRY Filter ( char *Name, char *Type ) ;
:li.BOOL  APIENTRY Verify ( HWND Owner, char *Name ) ;            
:li.void* APIENTRY Import ( HWND Owner, char *Name, long *Size ) ;
:li.BOOL  APIENTRY Export ( HWND Owner, char *Name, void *Text, long Size ) ;
:esl.:lm margin=1.:font facename=default.

:p.The QueryFileType function returns an extended file type (like &apos.Escriba&apos. or 
&apos.Plain Text&apos.) which will be displayed in the File Open dialog&apos.s file type
combobox, right below the file name entry field.

:p.The QueryWildcard function returns a wildcard file specifier (like &apos.*.TXT&apos.)
which will be automatically loaded in the File Open dialog&apos.s file name
entry field when the file type is selected from the File Type combobox.

:p.The Filter function is called while filling the File Open dialog&apos.s file
listbox, and is used to determine whether or not to display a file&apos.s name
in the listbox.  This is normally only a quick check of the file&apos.s name
and extended type, and not a check of the file contents itself.

:p.The Verify function is called to do a final validation of a file&apos.s type
prior to importing it into the word processor.  This check should be quick,
but more thorough than the check performed by the Filter function.

:p.The Import function loads a document file into memory, converts it to the
native Escriba format, and returns the completed text.  The Import function
is expected to allocate the memory for the text dynamically.  Escriba will
free this memory when it finishes using it.

:p.The Export function saves a document file.  The document text is passed
to the exporter in Escriba format, and is converted and written to the
specified file by the exporter.

:h3 res=61006.Graphic Import Module Interface
Modules of type "GRAPHICIMPORT" are used to import graphic images into
a document, converting such images to an OS/2 system bitmap in memory.

:p.In addition to the basic functions of an add-on extension, the following
functions are provided by Graphic Import modules&colon.

:font facename=Courier size=13x8.:lm margin=6.:sl compact.
:li.char*   APIENTRY QueryFileType ( void ) ;
:li.char*   APIENTRY QueryWildcard ( void ) ;
:li.BOOL    APIENTRY Filter ( char *Name, char *Type ) ;
:li.BOOL    APIENTRY Verify ( HWND Owner, char *Name ) ;
:li.HBITMAP APIENTRY GraphicImport ( HWND Owner, char *Name ) ;
:esl.:lm margin=1.:font facename=default.

:p.The QueryFileType function returns an extended file type (like &apos.Bitmap&apos. or
&apos.Metafile&apos.) which will be displayed in the Graphic File Open dialog&apos.s file type
combobox, right below the file name entry field.

:p.The QueryWildcard function returns a wildcard file specifier (like &apos.*.BMP&apos.)
which will be automatically loaded in the Graphic File Open dialog&apos.s file name
entry field when the file type is selected from the File Type combobox.

:p.The Filter function is called while filling the Graphic File Open dialog&apos.s file
listbox, and is used to determine whether or not to display a file&apos.s name
in the listbox.  This is normally only a quick check of the file&apos.s name
and extended type, and not a check of the file contents itself.

:p.The Verify function is called to do a final validation of a file&apos.s type
prior to importing it into the word processor.  This check should be quick,
but more thorough than the check performed by the Filter function.

:p.The GraphicImport function loads a graphic file into memory and converts it to the
native OS/2 bitmap format, and returns a handle to the bitmap.

:euserdoc.
