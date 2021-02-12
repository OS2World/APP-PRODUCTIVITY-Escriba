#include "PText.h"
:userdoc.
:title.Plain Text Import/Export Help
:docprof toc=123456.

:h1 res=IDD_PRODUCTINFO.Product Information
This dialog displays the name, revision, date and copyright information
for this module.  Press ENTER or click on the OK button to dismiss the
dialog box.

:h1 res=IDD_CONFIGURE_IMPORT.Import Configuration
This configuration page allows the user to set the different options
for Plain Text document importing.  The currently available
options are&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_FONT.Font:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_SIZE.Size:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_METRIC.Use metric units:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_SOFTBREAKS.Ignore line breaks:elink.
:esl.:p.
This dialog is always displayed as a page of the Configure Extensions
dialog notebook.  The entries you have made will take effect after you
click the OK button of that dialog.  If you click on the ESCAPE button
instead, your changes will not be saved.  In any case, the settings
are saved by the add-on extension itself, in the file PTEXT.INI, which
resides in the directory to which PTEXT was installed.

:h2 res=IDD_CONFIGURE_FONT.Configure Font
This combination entryfield and listbox allows you to specify which
installed font will be used for the imported plain text, since the
Plain Text document will not have a font specification code within
itself.  Only monospaced fonts are listed here, since Plain Text
files are monospaced by definition.

:h2 res=IDD_CONFIGURE_SIZE.Configure Size
This combination entryfield and listbox allows you to specify the
point size to be used for the imported plain text, since the Plain
Text document will not have a point size specification code within
itself.  For standard typewriter spacing, use 10 points.

:h2 res=IDD_CONFIGURE_METRIC.Use metric units
If this button is checked, then the document importer will use metric
units (centimeters) in preparing the document.

:h2 res=IDD_CONFIGURE_SOFTBREAKS.Ignore line breaks
If this button is checked, then the document importer will ignore
single line breaks while loading the document.  Double line breaks
continue to be processed.

:h1 res=IDD_CONFIGURE_EXPORT.Export Configuration
This configuration page allows the user to set the different options
for Plain Text document exporting.  The currently available
options are&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_FORMAT.Format output:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_HEIGHT.Height:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_WIDTH.Width:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_BOLD.Simulate Bold Text:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_UNDERSCORE.Simulate Underscored Text:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_STRIKEOUT.Simulate Struckout Text:elink.
:esl.:p.
This dialog is always displayed as a page of the Configure Extensions
dialog notebook.  The entries you have made will take effect after you
click the OK button of that dialog.  If you click on the ESCAPE button
instead, your changes will not be saved.  In any case, the settings
are saved by the add-on extension itself, in the file PTEXT.INI, which
resides in the directory to which PTEXT was installed.

:h2 res=IDD_CONFIGURE_FORMAT.Format Output
If this button is checked, then the document exporter will arrange.
the output text in pages and lines, using the Height and Width
items also specified in this dialog page.  Otherwise, lines will
be broken only when explicit newline characters are found in the
original document.

:h2 res=IDD_CONFIGURE_HEIGHT.Configure Height
This spinbutton control allows you to specify how many lines will
be used when formatting the document text page on export.  Values from
40 to 80 lines are allowed.  

:h2 res=IDD_CONFIGURE_WIDTH.Configure Width
This spinbutton control allows you to specify how many columns will
be used when formatting the document text on export.  Values from
40 to 160 columns are allowed.  

:h2 res=IDD_CONFIGURE_BOLD.Simulate Bold Text
If this button is checked, then the document exporter will simulate
bold text by printing a backspace, followed by the original character
repeated.  

:h2 res=IDD_CONFIGURE_UNDERSCORE.Simulate Underscored Text
If this button is checked, then the document exporter will simulate
underscored text by printing the character to be underscored, a backspace, 
and then the underscore character.

:h2 res=IDD_CONFIGURE_STRIKEOUT.Simulate Struckout Text
If this button is checked, then the document exporter will simulate
struckout text by printing the character to be struck out, a backspace, 
and then the strikeout character.

:euserdoc.
