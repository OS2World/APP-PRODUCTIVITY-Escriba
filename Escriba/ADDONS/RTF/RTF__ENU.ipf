#include "RTF.h"
:userdoc.
:title.RTF Import/Export Help
:docprof toc=123456.

:h1 res=IDD_PRODUCTINFO.Product Information
:p.This dialog displays the name, revision, date and copyright information
for this module.  Press ENTER or click on the OK button to dismiss the
dialog box.
:p.The RTF module provides Escriba with the ability to read and write Microsoft Rich Text Format (RTF) files
at revision 1.5.  Most of the specification's functions are ignored for the time being, though their
presence is tolerated without complaint, so long as the document has valid format according to 
Microsoft's specification.
:p.The following is a list of the currently supported functions, in alphabetical order&colon.
:table cols='12 60' rules=both frame=box.
:row.:c.\'           :c.Hexadecimal follows
:row.:c.\-           :c.Optional hyphen.
:row.:c.\*           :c.Ignore destination if not supported.
:row.:c.\\           :c.\
:row.:c.\_           :c.Nonbreaking hyphen.
:row.:c.\{           :c.{
:row.:c.\}           :c.}
:row.:c.\~           :c.Nonbreaking space.
:row.:c.\ansi        :c.ANSI (ISO) Character Set
:row.:c.\author      :c.Author
:row.:c.\b           :c.Bold
:row.:c.\bin         :c.Binary follows
:row.:c.\blue        :c.Blue
:row.:c.\bullet      :c.Standard bullet character
:row.:c.\caps        :c.Capitals
:row.:c.\cf          :c.Foreground Color
:row.:c.\colortbl    :c.Color Table
:row.:c.\cpg         :c.CodePage
:row.:c.\creatim     :c.Create Time
:row.:c.\da          :c.Day
:row.:c.\deff        :c.Default Font
:row.:c.\deftab      :c.Default tab spacing (in twips)
:row.:c.\emdash      :c.Em Dash
:row.:c.\endash      :c.En Dash
:row.:c.\f           :c.Select/Define Font
:row.:c.\fbias       :c.Font Bias - Default, Oriental
:row.:c.\fbidi       :c.Font Family - BIDI
:row.:c.\fcharset    :c.Font Character Set
:row.:c.\fdecor      :c.Font Family - Decorative
:row.:c.\fi          :c.First line indent              
:row.:c.\fmodern     :c.Font Family - Modern
:row.:c.\fnil        :c.Font Family - Nil
:row.:c.\fonttbl     :c.Font Table
:row.:c.\footer      :c.Page Footer definition
:row.:c.\fprq        :c.Font Pitch - Default, Fixed, Variable
:row.:c.\froman      :c.Font Family - Roman
:row.:c.\fs          :c.Font Size
:row.:c.\fscript     :c.Font Family - Script
:row.:c.\fswiss      :c.Font Family - Swiss
:row.:c.\ftech       :c.Font Family - Technical/Symbol
:row.:c.\ftnil       :c.Font Type - Nil
:row.:c.\fttruetype  :c.Font Type - TrueType
:row.:c.\green       :c.Green
:row.:c.\header      :c.Page Header definition
:row.:c.\hr          :c.Hour
:row.:c.\i           :c.Italic
:row.:c.\ldblquote   :c.Left doublequote
:row.:c.\lquote      :c.Left singlequote
:row.:c.\margb       :c.Margin (bottom)
:row.:c.\margl       :c.Margin (left)
:row.:c.\margr       :c.Margin (right)
:row.:c.\margt       :c.Margin (top)
:row.:c.\min         :c.Minute
:row.:c.\mo          :c.Month
:row.:c.\outl        :c.Outline
:row.:c.\page        :c.Required page break
:row.:c.\par         :c.End of paragraph
:row.:c.\pard        :c.Paragraph Default
:row.:c.\pc          :c.IBM PC Character Set
:row.:c.\pca         :c.IBM PC Multilingual Character Set
:row.:c.\plain       :c.Plain text (reset all font attributes)
:row.:c.\qc          :c.Center Justify
:row.:c.\qf          :c.Full Justify
:row.:c.\ql          :c.Left Justify
:row.:c.\qr          :c.Right Justify
:row.:c.\rdblquote   :c.Right doublequote
:row.:c.\red         :c.Red
:row.:c.\rquote      :c.Right singlequote
:row.:c.\rtf         :c.RTF Version
:row.:c.\sec         :c.Second
:row.:c.\sectd       :c.Section Default
:row.:c.\strike      :c.Strikeout
:row.:c.\tab         :c.Tab
:row.:c.\title       :c.Title
:row.:c.\ul          :c.Underline
:row.:c.\yr          :c.Year
:etable.

:euserdoc.
