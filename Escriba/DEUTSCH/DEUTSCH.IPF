#include "..\Escriba.h"
#ifdef APIENTRY
#undef APIENTRY
#endif
:userdoc.
:title.Escriba Textverarbeitung Hilfe
:docprof toc=123456.

:h1 res=ID_MAIN.EinfÅhrung
:i1.EinfÅhrung
:artwork name='Escriba.bmp' align=center.
:p.
Dieses Programm ist ein &apos.What You See Is What You Get&apos. (WYSIWYG)
Textverarbeitungs-Programm (Word Processor, WP) fÅr den OS/2 Presentation
Manager. Es versucht nicht mit den gro·en, professionellen WPs die fÅr
OS/2 PM bzw. fÅr MS Windows existieren, zu konkurrieren. Escriba stellt
aber die Basisfunktionen zur Text-Formatierung usw. zur VerfÅgung.
Au·erdem kann das Programm Grafik einbinden. Alle im System installierten
ATM Fonts werden in allen Formaten und verschiedenen Schriftgrî·en
unterstÅtzt. Die Druckeransteuerung ist Åber die OS/2 PM Schnittstelle
realisiert. Damit kînnen alle dem System bekannten Drucker, FAX-Programme
und Plotter Åber ihre Treiber angesteuert werden.
On Oriental versions of OS/2, double-byte character set input and output
is supported, as are True-Type fonts installed via the Font Palette.
:p.
:sl compact.
:li.&sqbul.:link reftype=hd res=60001.Programmstart und Programmende:elink.
:li.&sqbul.:link reftype=hd res=60002.Dokumente laden und speichern:elink.
:li.&sqbul.:link reftype=hd res=60003.Dokumente bearbeiten:elink.
:li.&sqbul.:link reftype=hd res=60004.Dokumente drucken:elink.
:li.&sqbul.:link reftype=hd res=60005.Fenster Funktionen:elink.
:li.&sqbul.:link reftype=hd res=60006.Einige Anmerkungen:elink.
:li.&sqbul.:link reftype=hd res=60007.Registration:elink.
:esl.

:h2 res=60001.Programmstart &amp. Programmende
:hp4.Programmstart:ehp4.
:p.
Ist Escriba erst einmal installiert, gibt es drei Mîglichkeiten das Programm zu
starten.
Erste Mîglichkeit: ein Doppel-Klick auf das bei der Installation erzeugte
Programmobjekt. In diesem Fall startet das Programm und
erzeugt ein neues Dokument zur direkten Verwendung.
:p.
Zweite Mîglichkeit: eine Dokumenten-Datei auf das Programm-Objekt ziehen
und sie dort loslassen. Das Dokument wird geîffnet, im Arbeitsbereich
angezeigt und steht zur weiteren Bearbeitung zur VerfÅgung.
:p.
Dritte Mîglichkeit: diese wird durch das START Kommando realisiert. Diese
Variante funktioniert nur, wenn das aktive Verzeichnis Escribas
Arbeitsverzeichnis ist. Wenn Sie das Escriba-Verszeichnis in Ihrer CONFIG.SYS
Datei in den HELP, PATH und LIBPATH Zeilen angegeben haben, kînnen Sie das
START Kommando von Åberall im System angeben.
:p.
Es gibt eine indirekte Mîglichkeit Escriba zu starten. Beim abspeichern einer
Datei im eigenen Format markiert das Programm diese als Type &apos.Escriba&apos.. 
Dieser Typ wird Åber das Dateisystem mit der Escriba Textverarbeitung assoziiert 
und wenn das Dateiobjekt gîffnet wird, ist die Standardaktion das Starten der
Textverarbeitung Escriba um die Datei anzuzeigen oder zu bearbeiten. Escriba
ist auch mit normalen Textdateien und Pluma Dateien assoziiert und erscheint
im ôffnen-MenÅ der Kontext-MenÅs dieser Dateien.

:p.
:hp4.Programmende:ehp4.
:p.
Um Escriba zu beenden, kînnen Sie das System-MenÅ (die Mini-Ikone oben links
in der Ecke) doppel-klicken oder Sie kînnen die F3-Taste drÅcken. Sollte ein
Dokument geîffnet und verÑndert sein, werden Sie gefragt, ob das Dokument
abgespeichert werden soll. Sie kînnen zu jedem Zeitpunkt den Vorgang abbrechen
und mit dem Programm weiterarbeiten.

:h2 res=60002.Dokumente laden &amp. speichern
:hp4.Laden:ehp4.
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
:hp4.Speichern:ehp4.
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
Wenn Sie versuchen ein Dokumenten-Fenster zu schlie·en, das verÑndert aber
nicht gespeichert ist, werden Sie befragt, ob Sie es sichern wollen.
Gleicherma·en, wenn Sie die Textverarbeitung beenden wollen und es gibt
noch geîffnete, verÑnderte Dokumente, werden Sie gefragt ob diese gespeichert
werden sollen. Diese Abfrage erfolgt fÅr jedes geîffnete Dokument separat.

:h2 res=60003.Dokumente bearbeiten
:hp4.Bewegen im Text:ehp4.
:p.
FÅr die Cursor-Bewegungen im Textfenster sind die folgenden Tasten
festgelegt&colon.
:table cols='20 60' rules=both frame=box.
:row.:c.Linke Pfieltaste        :c.Gehe zum vorigen Buchstaben / Symbol.
:row.:c.Rechte Pfieltaste       :c.Gehe zum nÑchsten Buchstaben / Symbol.
:row.:c.Hoch Pfieltaste         :c.Gehe zur vorigen Zeile.
:row.:c.Runter Pfieltaste       :c.Gehe zur nÑchsten Zeile.
:row.:c.Bild hoch               :c.Gehe zur vorigen Seite.
:row.:c.Bild Runter             :c.Gehe zur nÑchsten Seite.
:row.:c.Pos1                    :c.Gehe zum Zeilenanfang.
:row.:c.Ende                    :c.Gehe zum Zeilenende.
:row.:c.Strg + Linke Pfieltaste :c.Gehe zum vorigen Wort.
:row.:c.Strg + Rechte Pfieltaste:c.Gehe zum nÑchsten Wort.
:row.:c.Strg + Bild hoch        :c.Move to top of current page.
:row.:c.Strg + Bild Runter      :c.Move to bottom of current page.
:row.:c.Strg + Pos1             :c.Gehe zum Dokumenten Anfang.
:row.:c.Strg + Ende             :c.Gehe zum Dokumenten Ende.
:etable.

ZusÑtzlich kînnen Sie die Maus verwenden, um den Cursor direkt innerhalb des
sichbaren Textfensters zu positionieren. Sie kînnen au·erdem mit der Maus und
den Rollbalken den Bildausschnitt horizontal oder vertikal repositionieren,
vorausgesetzt Sie haben die Rollbalken aktiviert.

:p.Es gibt vier spezielle MausschaltfÑchen auf der unteren rechte Seite des
Dokumenten Fensters. Diese SchaltflÑchen aktivieren die &apos.Gehe zum Dokumenten
Anfang&apos., &apos.Gehe zur vorigen Seite&apos., &apos.Gehe zur nÑchsten Seite&apos. 
und &apos.Gehe zum Dokumenten Ende&apos. Funktionen. Sie entsprechen also den Bild hoch 
bis Strg+Ende Tastenfunktionen.

:p.Bedenken Sie bitte bei Cursorbewegungen im Dokument, da· die
Funktionscodes fÅr RÑnder, ZeichensÑtze, Gî·en usw. als unsichtbare Symbole im
Text abgelegt sind. Der Cursor wird an diesen Stellen anhalten, wenn Sie ihn
von Zeichen zu Zeichen bewegen.

:note.There is a special difference in how the Home and End keys
operate when compared to industry standards.  
If you press the Home key when you are already at the start of 
the current line, you will be moved to the start of the 
previous line.
Likewise, when you press the End key when you are already at the
end of the current line, you will be moved to the end of the
next line.

:p.:hp4.Text:ehp4.

:p.Text kann in ein Dokument durch einfaches eintippen eingegeben werden.
Dabei wird der Text gleichzeitig formatiert. Wenn ein Wort zu lang fÅr eine
Zeile ist, wird es automatisch in diese nÑchste Zeile geschoben. Wenn Sie
zu schnell tippen, werden Ihre TastendrÅcke gesammelt und in Gruppen zu
mehreren Zeichen auf den Bildschirm gebracht.

:p.Nachdem Sie Text eingegeben haben, kînnen Sie mit dem Cursor zurÅckgehen
und ihn verÑndern. Normalerweise ist der Cursor als EinfÅgecursor definiert
(erkennbar als vertikaler Strich). Wenn Sie mitten in einem schon vorhandenen
Text anfangen zu tippen, wird der neue Text entsprechend eingefÅgt und
formatiert. Sollten Sie den alten Text durch den neuen ersetzen wollen, drÅcken
Sie die EINF-Taste. Der Cursor Ñndert sein Aussehen in einen Block. Was immer
Sie jetzt eingeben, ersetzt den darunter liegenden Text. Verwenden Sie diesen
Modus mit Bedacht: wenn der Cursor Åber den unsichtbaren Symbolen, die
Zeichengrî·en, RÑnder usw. reprÑsentieren steht und Sie geben neuen Text ein,
werden auch die unsichtbaren Symbole Åberschrieben. Das kan z.B. dazu fÅhren,
da· bestimmte Formatierungsmerkmale verschwinden.

:p.Text kann markiert werden, indem man den Cursor an den Anfang des zu
selektierenden Blocks bewegt und ihn dann bei gleichzeitig gedrÅckter SHIFT Taste
bis zum Ende des Blocks bewegt. Eine weitere Mîglichkeit besteht darin, den
Cursor mit der Maus and den Anfang zu bewegen. Dann bewegt man den Cursor bis
zum Ende und hÑlt gleichzeitig die linke Maustaste gedrÅckt. Ist ein Textblock
markiert, kann er bewegt, kopiert, ausgeschnitten, in die Zwischenablage
kopiert oder durch den Inhalt der Zwischenablage ersetzt werden. Wenn Sie
Text markiert haben und beginnen neuen Text einzutippen, dann wird der markierte
Block gelîscht und durch den gerade eingegebenen Text ersetzt.

:p.Selektierter Text kann mit der Maus bewegt oder kopiert werden. Dazu mÅssen
Sie die rechte Maustaste im markierten Bereich drÅcken und dann die Maus bis
zur gewÅnschten Zielposition ziehen. Wenn Sie dort die Maus loslassen, wird
der markierte Text verschoben. Wenn Sie den Text nicht verschieben sondern
kopieren wollen, betÑtigen Sie die STRG Taste wÑhrend Sie die Maustaste
loslassen. Wenn Sie die Maus nicht bewegen, also nur die rechte Maustaste
drÅcken und wieder loslassen, erscheint ein KontextmenÅ, das all fÅr diesen
Textblock verfÅgbaren Kommandos anzeigt.

:p.:hp4.Text Attribute:ehp4.

:p.An jeder Stelle im Dokument sind spezielle Attribute wirksam, die das Aussehen
des Textes und seine Formatierung bestimmen. Die RÑnder sind auf bestimmte
Werte gesetzt, die Tabulatoren stehen an definierten Stellen und eine
bestimmte Schriftart ist gewÑhlt. Jeder dieser Werte kann ab der
augenblicklichen Position oder Åber einen markierten Bereich verÑndert werden.
Wenn Sie z.B. die Schriftart Times Roman gewÑhlt aber keinen Text markiert
haben, dann gilt diese Einstellung ab der augenblicklichen Cursorpostion
bis zur nÑchsten EinstellungsÑndereung im Text. War aber Text markiert,
so ersetzt die neue Einstellung alle eventuell im Text vorhandenen
Schriftart-Einstellungen. Schriftart-Einstellungen anderer Textbereich werden
durch die beschriebene énderung nicht beeinflu·t. Alle Texteinstellung
werden auf die beschriebene Art behandelt, seien es nun RÑnder, Tabulatoren,
EinrÅckungen, Schriftgrî·en, Farben usw.
:p.
:hp4.Grafik einfÅgen:ehp4.
:p.
Grafiken kînnen von der Zwischenablage, von jeder Stelle der ArbeitsoberflÑche
oder aus einer Datei in ein Dokument eingefÅgt werden. Verwenden Sie
das :link reftype=hd res=IDM_PASTE_FROM.EinfÅgen von:elink.
MenÅ um dies zu erreichen.
:p.
Ist eine Grafik in ein Dokument eingefÅgt, so kann sie jederzeit verschoben,
kopiert und in der Grî·e verÑndert werden. Um die Grafik zu verschieben, klicken
Sie mit der rechten Maustaste hinein und ziehen Sie sie dann an die neue Position.
Um die Grafik zu kopieren tun Sie das gleiche, mit der Ausnahme, da· Sie beim
loslassen der rechten Maustaste die STRG-Taste drÅcken. Um die Grî·e zu verÑndern,
klicken Sie mit der rechten Maustaste in den wei·en Bereich neben der Grafik. Es
erscheint eine Kante, die Ihnen bei der Grî·enverÑnderung Hilfestellung bietet.

:h2 res=60004.Dokumente drucken
 Zur Zeit gibt es zwei Mîglichkeiten Text zu drucken:
:link reftype=hd res=IDM_PRINT_ALL.Drucke alles:elink. und
:link reftype=hd res=IDM_PRINT_PAGE.Drucke Seite:elink..
:p.
Drucke alles bewirkt, da· das komplette Dokument ausgedruckt wird. Diese Funktion
kann auch durch Eingabe von Strg+P aktiviert werden.
:p.
Drucke Seite druckt nur die augenblicklich ausgewÑhlte Seite eines Dokuments.
Diese Funktion kann auch durch die Eingabe von ALT+P aktiviert werden.

:h2 res=60005.Fenster Funktionen
 Sie kînnen viele Dokumenten-Fenster gleichzeitig geîffnet haben.
Jedesmal wenn Sie ein neues Dokumenten-Fenster îffnen, werden alle zur
Zeit offenen Dokumenten-Fenster als Ikonen am unteren Rand des
Arbeitsbereichs angeordnet. Das neue Dokumenten-Fenster nimmt dann soviel
Platz ein wie mîglich. Die ikonisierten Dokumente bleiben aber sichtbar.
:p.
Um ein Dokumenten-Fenster wieder auf seine normale Grî·e zu bringen,
brauchen Sie es nur wie bei jeder anderen System-Ikone doppelt anzuklicken.
Das Fenster nimmt dann wieder die Grî·e und die Position ein, die es vor der
Ikonisierung hatte.
:p.
Wenn Sie mehrere Dokumente auf ihre normale Grî·e bringen, werden sie sich
teilweise oder ganz Åberdecken. Es gibt zwei MenÅ-Kommandos, die fÅr diesen
Fall vorgesehen sind:
:link reftype=hd res=IDM_TILE.Nebeneinander:elink. und
:link reftype=hd res=IDM_CASCADE.öberlappend:elink..
Der MenÅ-Punkt &apos.Nebeneinander&apos. ordnet die Fenster aller nichtikonisierten
Dokumente so an, da· keines ein anderes verdeckt. Das Verhalten ist mit dem
von OS/2 auf Systemebene identisch.
öberlappend ordnet die Dokumenten-Fenster in Form eines Stapels an. Der
Stapel ist so aufgebaut, da· die Titel-Balken alle sichtbar sind. Das aktive
Dokument liegt hierbei oben auf dem Stapel.
:p.
Es gibt noch eine weitere Ornungs-Mîglichkeit:
:link reftype=hd res=IDM_ARRANGEICONS.Iconen anordnen:elink.
Dieser MenÅ-Eintrag ordnet alle Iconen am unteren Rand des Arbeitsbereichs
an.
:p.
FÅr den Fall, da· ein Dokumenten-Fenster komplett verdeckt ist, bietet das
:link reftype=hd res=IDM_WINDOW_MENU.Fenster:elink. MenÅ eine Liste
aller im Augenblick geîffneten Dokumente an. Sie kînnen irgendein Dokument
auswÑhlen und machen es damit zum aktiven Fenster. Falls notwendig wird das
Fenster aus dem  minimierten Zustand wieder auf Originalgrî·e gebracht.

:h2 res=60006.Einige Anmerkungen
&sqbul.Viele Schriftarten im Schriftarten-MenÅ zu haben sieht sicher gut
aus. OS/2 gibt allerdings den durch die Fonts belegten Speicher nicht mehr vor einem
Systemabschlu· frei. Dies kann Ihre Auslagerungs-Datei (SWAPPER.DAT) auf
einige Megabytes vergrî·ern.
:p.
&sqbul.Bei einigen ATM Schriftarten aus Mailboxen sind nicht alle Zeichen
definiert. Die Verwendung kann zu einem Systemzusammenbruch fÅhren. Seien Sie
also vorsichtig mit neu installierten Fonts. Mit ZeichensÑtzen von IBM oder
Adobe gibt es keine Probleme.
:p.
&sqbul.In Dokumente integrierte Grafiken kînnen sehr gro· sein. Wenn Sie
solche Grafiken verwenden und verkleinern damit sie in eine Dokumenten-Seite
passen, hei·t dies nicht, da· sich auch der Speicherbedarf verringert.
Es ist nach wie vor die gleiche Grafik.

:h2 res=60007.Registration
Dieses Program ist Shareware. Sie kînnen jedem Interessierten eine
unverÑnderte Kopie zur VerfÅgung stellen. Wenn Sie mit dem Programm
zufrieden sind und es weiter benutzen wollen, mÅssen Sie es registrieren
lassen. Die Registration erfolgt, indem Sie $30 bezahlen auf einem der
folgenden Wege bezahlen:
:p.
:hp4.Scheck&colon.:ehp4.
Schicken Sie einen auf eine amerikanische Bank ausgestellten
Scheck an Rick Papo. Vermerken Sie bitte Ihren Namen, Ihre Anschrift,
die Revisionsnummer und das Datum Ihrer Escriba-Version und, falls mîglich,
Ihre E-Mail Adresse.

Meine Anschrift lautet&colon.
:lm margin=6.:hp4.:sl compact.
:li.Escriba Registrations
:li.c/o Rick Papo
:li.38290 Avondale
:li.Westland, MI 48186-3830
:li.USA
:esl.:ehp4.:lm margin=1.
.br
:hp4.VISA/MasterCard&colon.:ehp4.
Wenn Sie per Kreditkarte bezahlen mîchten, kînnen Sie dies Åber die
Firma BMT Micro tun. Die Firma hat die folgenden Nummern:
Telefon (800) 414-4268 (nur USA) und (910) 791-7052, FAX (910) 350-2937
Mailbox (910) 350-8061. Die GeschÑftszeiten sind von 08&colon.00 bis 20&colon.00 Uhr
(EST,-5 GMT).  Die FAX und Mailbox-Nummern sind 24 Stunden am Tag, 7 Tage
in der Woche erreichbar. Die Postanschrift lautet&colon.
:lm margin=6.:hp4.:sl compact.
:li.BMT Micro
:li.452 Horn Rd
:li.Wilmington, NC 28412-2411
:li.USA
:esl.:ehp4.:lm margin=1.
:p.
Wenn Sie in einer der ehemaligen Sowjetrepubliken leben, kînnen Sie
das Programm fÅr $5 (bitte keine Kreditkarte) registrieren lassen.
:p.
Mit Ihrer Registration erhalten Sie die neueste Version der
Textverarbeitung auf Diskette. Wenn Sie schon die aktuelle Version
verwenden, erhalten Sie eine BestÑtigung der Registrierung und die
nÑchste Version des Programms, sobald diese verfÅgbar ist. Eine gedruckte
Dokumentation ist noch nicht fertig. Sowie diese vorliegt, erhalten alle
registrierten Benutzer von Escriba eine Kopie. Wenn Sie spÑter eine neue
Version des Programms erwerben wollen, ist die GebÅhr dafÅr $5.

:h2 res=60008.Fragen und RÅckmeldung von Fehlern
Bei Problemen mit diesem Programm kînnen Sie den Autor, Rick Papo, wie folgt
erreichen&colon.
:table cols='15 60' rules=both frame=box.
:row.:c.Internet        :c.:hp4.rpapo@msen.com:ehp4.
:row.:c.Post            :c.:hp4.Rick Papo, 38290 Avondale, Westland, MI 48186-3830, USA:ehp4.
:row.:c.Telefon         :c.:hp4.USA (734) 326-8547:ehp4.
:etable.
Hîchste PrioritÑt bei Fragen und Problemen genie·en registrierte Anwender
(siehe :link reftype=hd res=60007.Registration:elink.), es wird aber versucht, auf
alle eingehenden Nachrichten zu antworten. Die Elektronische Post (E-Mail)
wird jeden Tag ÅberprÅft.
:p.
Geben Sie bei allen Anfragen die Revisionsnummer der betroffenen Escriba
Version an.

:h1 res=IDM_FILE_MENU.Datei MenÅ
:i1.Datei MenÅ
 Das Datei MenÅ bietet folgende Kommandos&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_NEW.Neu:elink.
:li.&sqbul.:link reftype=hd res=IDM_OPEN.ôffnen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_RECALL.ZurÅckrufen:elink.
:li.&sqbul.:link reftype=hd res=IDM_MERGE.Mischen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVE.Speichern:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVEAS.Speichern unter...:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVE_CLOSE.Speichern und Schlie·en:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLOSE.Schlie·en:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT.Drucken:elink.
:li.&sqbul.:link reftype=hd res=IDM_PREFERENCES.Allgemeine Voreinstellungen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_DEFAULTS.Dokumenten Voreinstellungen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_CONFIGURE_EXTENSIONS.Configure Extensions...:elink.
:li.&sqbul.:link reftype=hd res=IDM_LANGUAGE_MENU.Language:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXIT.Beenden:elink.
:esl.

:h2 res=IDM_NEW.Neu (MenÅ Option)
 Diese MenÅ Option erzeugt ein neues Dokument ohne Inhalt. Die
Dokumenten RÑnder, ZeichensÑtze und andere Einstellungen sind die, die
zuletzt unter Dokumenten Voreinstellungen eingestellt wurden, ohne das sich ein
aktives Dokument im Fenster befand.
:p.
Alle vorher geîffneten Fenster werden verkleinert am unteren Rand des
Arbeitsbereichs dargestellt, damit das neue Fenster soviel FlÑche wie mîglich
einnehmen kann.

:h2 res=IDM_OPEN.ôffnen... (MenÅ Option)
 Wenn diese MenÅ Option gewÑhlt ist, îffnet sich ein Standard Datei Dialog.
Sie kînnen ein oder mehrere Dokumente zum îffnen anwÑhlen. Wenn sie die
Eingabetaste oder die OK SchaltflÑche betÑtigen, werden alle gewÑhlten Dateien
geîffnet. Hierbei leuchtet eine rote Statusanzeige in der rechten HÑlfte der
Titelzeile des Programms auf. Jede geladene Datei erzeugt ein eigenes Dokumenten
Fenster. Die zuerst geladene Datei wird im aktiven Fenster dargestellt, alle
weiteren werden ikonisiert. Sie kînnen die einzelnen Dokumente vom Fenster-MenÅ
aus oder durch doppelklicken auf die Ikonen aktivieren.
:p.Die Schnellanwahl fÅr diese Funktion ist Strg+O.
:p.Siehe auch unter :link reftype=hd res=256.ôffnen/Mischen/Speichern unter... (Dialog):elink.

:h3 res=256.ôffnen/Mischen/Speichern unter... (Dialog)
 Bei Anwahl der ôffnen, Mischen oder Speichern unter MenÅ Optionen erscheint
ein Standard Datei Dialog. Dieser Dialog wird vom Betriebssystem bereitgestellt
und folgt damit den Richtlinien, die Sie in der Systemdokumentation nachlesen
kînnen.
:artwork name='Deutsch\FileDlg.bmp' align=center.
:p.An Besonderheiten ist zu beachten&colon.
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
&rarrow. Wenn Sie Dateien zum îffnen auswÑhlen, kînnen Sie soviele Dateien aus der
Dateiliste angeben wie Sie mîchten.
:p.
&rarrow. Wenn Sie eine Datei zum Mischen ausgewÑhlt haben, darf nur eine Datei
markiert werden.
:p.
&rarrow. Wenn Sie Speichern unter gewÑhlt haben, ist es nicht erlaubt, einen Namen
aus der Dateiliste auszuwÑhlen. Die Liste dient nur als Referenz. Sie mÅssen
den Namen fÅr die zu speichernde Datei im Eingabefeld angeben.
:p.
&rarrow. Das in diesem Dialog ausgewÑhlte Verzeichnis wird zum aktiven Verzeichnis.
Wenn Sie das nÑchste Mal ôffnen, Mischen oder Speichern unter aufrufen, wird
zuerst dieses Verzeicnis angezeigt.

:Note.Dateien werden beim Speichern auf die Festplatte als Escriba Dokumente
gekennzeichnet. Dies wird Åber die Erweiterten Attribute erreicht. Die
Kennzeichnung geht verloren, wenn die Dokumente auf Diskette
abgespeichert oder mit Programmen wie PKZIP archiviert werden, da beide Varianten
die sogenannte EAs nicht abspeichern. Es gibt allerdings neuere
Archivierungsprogramme, die mit EAs umgehen kînnen.

:h4 res=IDD_FILETYPE.Select File Type (Dialog)
This dialog is presented when you request the load of a file and that
file is found to be acceptable as more than one file type.
You are told the file&apos.s name and the acceptable types, and you
must select the correct type from the list.  Once you have selected
the correct file type, click on the OK button or press ENTER.
To abort the file load, click on the Escape button or press the Escape key.

:h2 res=IDM_RECALL.Wieder îffnen MenÅ
Diese MenÅ-Option aktiviert ein UntermenÅ, in dem die 10 zuletzt gespeicherten
Dateien aufgelistet sind.
:p.Siehe :link reftype=hd res=IDM_RECALL0.EintrÑge Wieder îffnen MenÅ:elink.
fÅr weitere Informationen.

:h3 res=IDM_RECALL0.Wieder ôffnen MenÅ EintrÑge
Dieser MenÅpunkt zeigt die Namen der letzten zehn abgespeicherten Dokumente an.
Um eines der Dokumente wieder zu îffnen wÑhlen Sie es mit den Cursor-Tasten
aus und betÑtigen dann die Eingabe taste oder Sie klicken es mit der Maus an.

:h2 res=IDM_MERGE.Mischen... (MenÅ Option)
 Wenn dieses MenÅ ausgewÑhlt ist, dann wird Ihnen ein Standard Dateidialog
angezeigt. Hier kînnen sie eine einzelne Datei auswÑhlen, die ab der
augenblicklichen Cursorposition in das aktive Dokument eingefÅgt werden soll.
:p.Siehe auch unter :link reftype=hd res=256.ôffnen/Mischen/Speichern unter... (Dialog):elink.

:h2 res=IDM_SAVE.Speichern (MenÅ Option)
 Dieses Kommando veranla·t das Speichern des aktiven Dokuments unter seinem
Dateinamen. Wenn keine énderungen am Dokument vorgenommen wurden, ist dieses
Kommando inaktiv.
:p.Die Schnellanwahl fÅr diese Funktion ist F2.

:h2 res=IDM_SAVEAS.Speichern unter... (MenÅ Option)
 Wenn dieses MenÅ ausgewÑhlt ist, dann wird Ihnen ein Standard Dateidialog
angezeigt. Hier kînnen sie einen Namen fÅr das zu speichernde aktive Dokument
angeben. Das Zielverzeichnis fÅr die Datei kann ebenfalls festgelegt werden.
:p.Siehe auch unter :link reftype=hd res=256.ôffnen/Mischen/Speichern unter... (Dialog):elink.

:h2 res=IDM_SAVE_CLOSE.Speichern &amp. Schlie·en (MenÅ Option)
 Diese MenÅoption ist eine schnelle Mîglichkeit das aktive Dokument zu speichern
und zu schlie·en.
:p.Die Schnellanwahl fÅr diese Funktion ist F4.

:h2 res=IDM_CLOSE.Schlie·en (MenÅ Option)
 Dieses Kommando schlie·t das aktive Dokument. Sollte ein Teil des Dokuments
nicht gespeichert sein, werden Sie gefragt ob Sie speichern mîchten. Wenn Sie
die JA SchaltflÑche betÑtigen oder J tippen wird das Dokument unter seinem
augenblicklichen Dateinamen gespeichert.
Wenn Sie die NEIN SchaltflÑche betÑtigen oder N tippen, wird das Dokument ohne
abspeichern von eventuellen énderungen geschlossen.
Wenn Sie die ABBRECHEN SchaltfÑche betÑtigen oder die ESCAPE Taste drÅcken,
wird das Kommando abgebrochen und das Dokumentenfenster bleibt aktiv.

:h2 res=IDM_PRINT.Drucken (UntermenÅ)
 öber dieses UntermenÅ haben Sie Zugang zu den vielfÑltigen Druckkommandos.
Zur Zeit sind zwei Kommandos verfÅgbar&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL.Drucke allse MenÅ:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_PAGE.Drucke Seite:elink.
:esl.

:h3 res=IDM_PRINT_ALL.Drucke Alles MenÅ
Dieses UntermenÅ verschafft Ihnen Zugang zu den angebotenen Mîglichkeiten
ein komplettes Dokument zu drucken. Folgende Kommandos stehen zur
VerfÅgung&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL_NORMAL.Drucke Alles Normal:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL_REVERSE.Drucke Alles RÅckwÑrts:elink.
:esl.

:h4 res=IDM_PRINT_ALL_NORMAL.Drucke Alles Normal (MenÅ Option)
Das aktive Dokument wird bei Auswahl dieser MenÅ Option gedruckt. WÑhrend
das Dokument zum System-Spooler geschickt wird, leuchtet zu Ihrer Information
ein rotes Statusfenster auf. Sie kînnen nach absetzen des Druck-Kommandos
sofort mit Ihrer Arbeit fortfahren. Das Dokument wird in dem Zustand gedruckt,
den es bei der Aktivierung des Druckkommandos hatte.
:p.Die Schnellanwahl fÅr diese Funktion ist Strg+P.

:h4 res=IDM_PRINT_ALL_REVERSE.Drucke Alles RÅckwÑrts (MenÅ Option)
Das aktive Dokument wird bei Auswahl dieser MenÅ Option gedruckt wie
unter :link reftype=hd res=IDM_PRINT_ALL_NORMAL.Drucke Alles Normal:elink.,
gedruckt mit der Ausnahme, da· die Seiten in umgekehrter Reihenfolge
ausgegeben werden.

:h3 res=IDM_PRINT_PAGE.Drucke Seite (MenÅ Option)
 Die aktuelle Seite des aktiven Dokuments wird gedruckt, wenn Sie diesen
MenÅpunkt aktivieren. Dabei leuchtet eine rote Statusmeldung auf, wÑhrend
das Dokument an den Systemspooler geschickt wird. Diese Anzeige dient nur
Ihrer Information. Nach Aktivierung des Druckkommandos kînnen Sie sofort
mit Ihrer Arbeit am Dokument fortfahren; gleichzeitig wird weitergedruckt.
:p.Die Schnellanwahl fÅr diese Funktion ist ALT+P.

:h2 res=IDM_PREFERENCES.Allgemeine Voreinstellungen... (MenÅ Option)
:p.Siehe auch unter :link reftype=hd res=IDD_PREFERENCES.Allgemeine Voreinstellungen (Dialog):elink.

:h3 res=IDD_PREFERENCES.Allgemeine Voreinstellungen (Dialog)
 Dieser Dialog erlaubt es Ihnen, das Aussehen und das Verhalten von Escriba zu
konfigurieren.
:artwork name='Deutsch\Prefer.bmp' align=center.
:p.Folgende Optionen stehen zur Zeit zur VerfÅgung&colon.
:p.:hp2.Animation:ehp2.
Wenn diese Option gesetzt ist, arbeitet die Textverarbeitung beim îffnen und
schlie·en des Hauptfensters, genau wie die Systemordner, mit explodierenden
bzw. implodierenden Fenstern. Genau wie bei den Systemordnern mu· dieses
Verhalten in den Einstellungen aktiviert sein.
:p.:hp2.Zeige Lineale:ehp2.
Wenn dieser Eintrag markiert ist, habem alle Dokumentenfenster horizontale
und vertikale Lineale an den RÑndern.
:p.:hp2.Zeige Rollbalken:ehp2.
Wenn dieser Eintrag markiert ist, haben alle Dokumentenfenster horizontale
und vertikale Rollbalken. Sie kînnen die Rollbalken verwenden, um sich mit der
Maus im Text zu bewegen.
:p.:hp2.Show Status Line:ehp2.
If this item is checked, then all document windows will include
a status line at the bottom, where the current cursor position and
an interpretation of the token at the cursor are given.
:p.:hp2.Zeige vollstÑndige Dateinamen:ehp2.
Wenn dieser Eintrag markiert ist, werden die vollstÑndigen Dateinamen jedes
Dokuments in der Kopfzeile des Fensters angezeigt. Andernfalls werden der
Laufwerksbuchstabe und der Name der Unterverzeichnissse nicht angezeigt.
:p.:hp2.Seiten einzeln spoolen:ehp2.
Wenn dieser Punkt markiert ist, werden die Seiten eines langen Dokuments als
einzelne DruckauftrÑge abgeschickt. DruckauftrÑge werden erst an den Drucker
geschickt, wenn sie sich vollstÑndig im Spooler befinden. Darum beschleunigt
diese Option das drucken. Andererseits kînnen ihre Seiten mit Seiten anderer
Benutzer gemischt werden oder nicht in der richtigen Reihenfolge ausgegeben
werden, wenn Sie einen Netzwerkdrucker verwenden.
:p.:hp2.Print Pages in Reverse:ehp2.
If this item is checked, then the normal method of printing multiple pages
will be to print the last page first.  This option is provided for the
convenience of users with printers such as the DeskJet, where pages are
stacked face up, and therefore in reverse order.
:p.:hp2.Reload Document at Start:ehp2.
If this item is checked, then the last document recorded in the Recall list
will be reloaded automatically at the time Escriba is started.
:p.:hp2.Anfangsvergrî·erung:ehp2.
Aus dieser Gruppe von Knîpfen wÑhlen Sie die Anfangsvergrî·erung fÅr das
Dokumentenfenster. Wenn Sie neue Dokumente îffnen oder Dokumente von der
Festplatte laden, wird diese Anfangseinstellung zur Darstellung im Fenster
verwendet. Sie kînnen eine der folgenden Vergrî·erungen wÑhlen&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_100.Vergrî·erung 100&percent.:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLWIDTH.Vergrî·erung volle Breite:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLHEIGHT.Vergrî·erung volle Hîhe:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_MARGINS.Vergrî·erung RÑnder:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_ALL.Vergrî·erung Alles:elink.
:esl.
:p.

Wenn alle Einstellungen vorgenommen sind, drÅcken Sie die EINGABE Taste oder
klicken Sie auf die OK SchaltflÑche. Die éndereungen beeinflussen alle
Dokumente die in Zukunft erstellt werden.
:p.
Wenn Sie den Dialog abbrechen und die Einstellungen unverÑndert lassen wollen,
kînnen Sie die ESC Taste drÅcken oder auf die ESCAPE SchaltflÑche klicken.

:h2 res=IDM_DEFAULTS.Dokumenten Voreinstellungen... (MenÅ Option)
Dieses Kommando startet einen Dialog mit dem man die Werte fÅr Drucker,
Papiergrî·e, Schriftart, Schriftgrî·e, Farbe, Zeichenattribute und
Textausrichtung (horizontal und vertikal) fÅr Dokumente festlegen kann.
:p.Siehe auch unter :link reftype=hd res=IDD_DEFAULTS.Dokumenten Voreinstellungen (Dialog):elink.

:h3 res=IDD_DEFAULTS.Dokumenten Voreinstellungen (Dialog)
Dieser Dialog ermîglicht es Ihnen, die Standard Attribute fÅr Dokumente
festzulegen. Diese Attribute gelten fÅr das aktive Dokument, oder, falls kein
Dokument geîffnet ist, werden die Attribute gespeichert und auf neue Dokumente
angewendet.
.br
The configuration options are grouped by category 
on the following pages&colon.
:sl compact.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_AUTHOR.Author:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_PRINTER.Drucker:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_FONT.Schriftart:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_LAYOUT1.Ansicht (1):elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_LAYOUT2.Ansicht (2):elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_NATIONAL.National:elink.
:esl.
:p.
Wenn alle Einstellungen wie gewÅnscht vorgenommen sind, drÅcken Sie die EINGABE
Taste oder klicken auf den OK Knopf um das Programm weiter auszufÅhren. Wenn
im Augenblick keine Dokumente geîffnet sind oder Sie die Alles sichern Einstellung markiert
haben, werden die neuen Einstellungen fÅr alle zu erstellenden Dokumente
verwendet. Wenn zur Zeit ein Dokument geîffnet ist, werden die neuen Einstellungen
auch fÅr dieses wirksam. Andere Dokumente werden nicht beeinflu·t.
:p.
Wenn Sie den Anwenden Knopf drÅcken, werden die aktuellen Einstellungen auf das
aktive Dokument angewendet und werden die neuen Standardeinstellungen.
:p.
Wenn Sie den Dialog abbrechen mîchten und die aktuellen Standardeinstellungen
unverÑndert bleiben sollen, kînnen Sie ESC tippen oder auf den ESC Knopf klicken.
énderungen die Sie mit Anwenden herbeigefÅhrt haben werden nicht verworfen.

:h4 res=IDD_DEFAULTS_PAGE_AUTHOR.Author Settings Page
:artwork name='Deutsch\Default0.bmp' align=center.
:p.:hp2.Author:ehp2.
This entryfield allows you to view and/or alter the author name and
title saved in the document header.  You are also shown the document
creation date.

:h4 res=IDD_DEFAULTS_PAGE_PRINTER.Printer Settings Page
:artwork name='Deutsch\Default1.bmp' align=center.
:p.Folgende Attribute kînnen mit diesem Dialog eingestellt werden&colon.
:p.:hp2.Drucker:ehp2.
Hier kînnen Sie einen Drucker auswÑhlen (wenn Sie mehrere installiert haben) auf
dem das Dokument ausgedruckt wird. Sie kînnen jeden beim System angemeldeten
Drucker auswÑhlen.
:p.:hp2.Drucker Einstellungen:ehp2.
Wenn Sie auf diese SchaltflÑche klicken, erscheint der Drucker Einstellungen
Dialog. Die Einstellungsmîglichkeiten variieren in AbhÑngigkeit vom Druckertyp
und kînnen hier nicht weiter beschrieben werden. Sie kînnen auch die Hilfe zum
Dialog aufrufen. Nach beenden des Dialogs werden die gewÑhlten Einstellungen
sofort Åbernommen.

:h4 res=IDD_DEFAULTS_PAGE_FONT.Font Settings Page
:artwork name='Deutsch\Default2.bmp' align=center.
:p.:hp2.Zeichensatz:ehp2.
Mit dem hier ausgewÑhlten Zeichensatz wird das aktuelle Dokument erstellt. Sie
kînnen jeden im System installierten Zeichensatz anwÑhlen.
:p.:hp2.Grî·e:ehp2.
Die ausgewÑhlte Grî·e wird auf den angewÑhlten Zeichensatz angewendet. Sie kînnen
jede der angegebenen Grî·en verwenden.
:p.:hp2.Farbe:ehp2.
Mit der angewÑhlten Farbe wird der Text des Dokuments dargestellt. Alle
angezeigten Farben kînnen verwendet werden.
:p.:hp2.Fett:ehp2.
Wenn diese Zeile markiert ist, wird der Text fett dargestellt. Wenn der
gewÑhlte Zeichensatz einen speziellen fetten Satz enthÑlt, wird dieser verwendet,
sonst erzeugt das System einen Satz fetter Zeichen.
:p.:hp2.Kursiv:ehp2.
Wenn diese Zeile markiert ist, wird der Text kursiv dargestellt.  Wenn der
gewÑhlte Zeichensatz einen speziellen kursiven Satz enthÑlt, wird dieser verwendet,
sonst erzeugt das System einen Satz kursiver Zeichen.
:p.:hp2.Unterstrichen:ehp2.
Wenn diese Zeile markiert ist, wird der Text unterstrichen dargestellt.
:p.:hp2.Durchgestrichen:ehp2.
Wenn diese Zeile markiert ist, wird der Text durchgestrichen dargestellt.
:p.:hp2.Umri·:ehp2.
Wenn diese Zeile markiert ist, wird der Text in Umrissen dargestellt aber
nicht ausgefÅllt.
:p.:hp2.Gro·schrift:ehp2.
Wenn diese Zeile markiert ist, wird der Text in Gro·schrift dargestellt.

:h4 res=IDD_DEFAULTS_PAGE_LAYOUT1.Layout Settings Page 1
:artwork name='Deutsch\Default3.bmp' align=center.
:p.:hp2.Ausrichtung:ehp2.
Hier wird die horizontale Ausrichtung ausgewÑhlt. Mîglich sind links,
zentriert, rechts und Blocksatz.
LinksbÅndig sorgt dafÅr, da· alle Textzeilen am linken Rand beginnen.
Zentriert stellt den Text symmetrisch zur Mitte dar, fÅgt aber keine zusÑtzlichen
Leerzeichen ein.
RechtsbÅndig sorgt dafÅr, da· alle Textzeilen am rechten Rand abschlie·en.
Blocksatz sorgt dafÅr, da· der Text links und rechts am Rand abschlie·t und fÅgt
bei Bedarf zusÑtzliche Leerzeichen ein.
:p.:hp2.Zentriert Seite:ehp2.
Wenn diese Zeile markiert ist, wird der Text vertikal zentriert auf allen
Seiten dargestellt.
:p.:hp2.RÑnder:ehp2.
Die vier Eingabefelder sind fÅr die Werte SeitenrÑnder in Zoll oder
Zentimeter vorgesehen. Die RÑnder kînnen nicht grî·er als die augenblicklich
eingestellte beschreibbare Seitengrî·e eingestellt werden.

:h4 res=IDD_DEFAULTS_PAGE_LAYOUT2.Layout Settings Page 2
:artwork name='Deutsch\Default4.bmp' align=center.
:p.:hp2.Tabulator Weite:ehp2.
In dieses Eingabefeld kann man die Standard Tabulatorweite eintragen.
Wenn Sie z.B. 0.5 wÑhlen, dann werden beginnend am linken Rand alle 0.5 Zoll
oder Zentimeter Tabulatoren gesetzt.
:p.:hp2.Indent:ehp2.
This entry field allows you to set the default paragraph indent to be
used from the start of the document.  If you were to enter 0.500 here,
then you will find the text of the first line of the document, and the
text following each end-of-line code in the document will be indented
one half-inch or centimeter from the left margin.
:p.:hp2.Zeilenabstand:ehp2.
Hier kann der Standard Zeilenabstand gewÑhlt werden. Sie kînnen einzeilig,
eineinhalbzeilig oder zweizeilig wÑhlen.

:h4 res=IDD_DEFAULTS_PAGE_NATIONAL.National Settings Page
:artwork name='Deutsch\Default5.bmp' align=center.
:p.:hp2.Code Page:ehp2.
With this control it is possible to set which code page, or national
language character mapping, is to be used for the document.
See :link reftype=hd res=61002.Code Pages:elink. for more information.
:p.:hp2.Metrisch:ehp2.
Wenn diese Zeile markiert ist, werden fÅr das Dokument metrische Einheiten
fÅr alle Ma·e verwendet. Die horizontalen und vertikalen Bildlaufleisten
zeigen die Skalen in Zentimetern an.
:p.:hp2.Datums Format:ehp2.
Dieses Feld zeigt das aktuelle Datums Format welches in den Funktionen
Aktuelles Datum und Festes Datum verwendet wird.
Siehe auch unter :link reftype=hd res=IDD_DATEFMT.Datums Format:elink.

:h2 res=IDM_CONFIGURE_EXTENSIONS.Configure Extensions... (MenÅ Option)
This command invokes a dialog from which it is possible to alter the
settings for any of the add-on extensions currently installed.
:p.See :link reftype=hd res=IDD_CONFIGURE_EXTENSIONS.Configure Extensions (Dialog):elink. 
for more information.

:h3 res=IDD_CONFIGURE_EXTENSIONS.Configure Extensions (Dialog)
This dialog allows you to change the settings for any of the currently
installed add-on extensions.
:artwork name='Deutsch\ConfigEx.bmp' align=center.

:h3 res=IDM_LANGUAGE_MENU.Language (UntermenÅ)
This submenu allows you to select a new current language for all the application
menus and dialogs.
:p.See :link reftype=hd res=IDM_LANGUAGES.Language Menu Entry:elink.
for more information.

:h4 res=IDM_LANGUAGES.Language Menu Entry (MenÅ Option)
Any of the entries in the Language submenu may be used to select a new
language for the application menus and dialogs.  
The language names are in their native form (Spanish = Espa§ol, for example).
After you select the new language, the entire application will be reformatted
to suit the new language.  Certain aspects of documents (like date formats)
are changed also.

:h2 res=IDM_EXIT.Beenden (MenÅ Option)
 Diese MenÅauswahl beendet das Programm.

:h1 res=IDM_EDIT_MENU.Bearbeiten MenÅ
:i1.Bearbeiten MenÅ
 Im Bearbeiten MenÅ stehen folgende Kommandos zur VerfÅgung&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_UNDO.Undo:elink.
:li.&sqbul.:link reftype=hd res=IDM_REDO.Redo:elink.
:li.&sqbul.:link reftype=hd res=IDM_CUT.Ausschneiden:elink.
:li.&sqbul.:link reftype=hd res=IDM_COPY.Kopieren:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE.EinfÅgen:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEAR.Lîschen:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEARCLIPBOARD.Zwischenablage Lîschen:elink.
:li.&sqbul.:link reftype=hd res=IDM_SELECT_ALL.Select All:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM.EinfÅgen von MenÅ:elink.
:li.&sqbul.:link reftype=hd res=IDM_INSERT_FUNCTION.Funktion einfÅgen MenÅ:elink.
:li.&sqbul.:link reftype=hd res=IDM_SEARCH.Suchen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_FINDNEXT.Finde nÑchstes:elink.
:li.&sqbul.:link reftype=hd res=IDM_GOTOPAGE.Go To Page:elink.
:li.&sqbul.:link reftype=hd res=IDM_SPELL.RechtschreibprÅfung...:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM.Ma·stab:elink.
:esl.

:h2 res=IDM_UNDO.Undo (MenÅ Option)
Diese Funktion macht die letzte am Dokument vorgenommene énderungen
rÅckgÑngig. Wenn Sie die Funktion mehrfach benutzen, wird immer die vorige
énderung rÅckgÑngig gemacht solange, bis alle énderungen am Dokument
zurÅckgenommen sind. Das Dokument befindet sich dann wieder in dem Zustand
wie nach dem laden von der Festplatte. Bei einem neuen Dokument ist der
Endzustand der ZurÅcknahme das leere Dokument.
:p.Die Schnellwahl fÅr diese Funktion ist Alt+Backspace.

:h2 res=IDM_REDO.Redo (MenÅ Option)
Wenn Sie das Dokument nach dem letzten Undu nicht verÑndert haben,
kînnen Sie mit dieser Funktion den Originalzustand wieder hesrstellen,
d.h. die RÅcknahme wieder zurÅcknehmen. Wenn Sie aber nach
einem :link reftype=hd res=IDM_UNDO.Undo:elink. eine VerÑnderung vorgenommen haben,
so ist es nicht mehr mîglich ein Redo vorzunehmen.
:p.Die Schnellwahl ist Shift+Alt+Backspace.

:h2 res=IDM_CUT.Ausschneiden (MenÅ Option)
 Dieser Befehl schneidet den markierten Text aus und plaziert ihn in der
Zwischenablage.
:p.Die Schnellauswahl ist Shift+Entfernen.

:h2 res=IDM_COPY.Kopieren (MenÅ Option)
 Dieser Befehl kopiert den angewÑhlten Text in die Zwischenablage.
:p.Die Schnellauswahl ist Strg+EinfÅgen.

:h2 res=IDM_PASTE.EinfÅgen (MenÅ Option)
Dieser Befehl sorgt dafÅr, da· der Text der Zwischenablage
entweder den aktuellen Text ersetzt (Ersetzungs Modus) oder ab der Cursor
Position eingefÅgt wird (EinfÅge Modus).
:p.Die Schnellauswahl ist Shift+EinfÅgen.

:h2 res=IDM_CLEAR.Lîschen (MenÅ Option)
Dieser Befehl lîscht den markierten Text ohne den Inhalt der
Zwischenablage zu verÑndern. Der Befehl entspricht der BetÑtigung der ENTFERNEN
Taste.

:h2 res=IDM_CLEARCLIPBOARD.Zwischenablage lîschen (MenÅ Option)
Dieser Befehl lîscht den gesamten Inhalt der Zwischenablage.

:h2 res=IDM_SELECT_ALL.Select All (MenÅ Option)
This command selects all the text of the current document, from start 
to finish.
:p.This command may also be invoked by double-clicking with mouse button one
on the upper-left corner area between the horizontal and vertical rulers.

:h2 res=IDM_PASTE_FROM.EinfÅgen von MenÅ
Von diesem MenÅ aus kînnen Sie die Quelle fÅr das EinfÅgen von Grafik-Daten
in ihr Dokument auswÑhlen.
Die folgenden Optionen stehen zur VerfÅgung&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP.EinfÅgen von der Zwischenablage:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN.EinfÅgen vom Bildschirm:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_FILE.EinfÅgen aus einer Grafik-Datei:elink.
:esl.

:h3 res=IDM_PASTE_FROM_CLIP.EinfÅgen aus der Zwischenablage MenÅ
Von diesem MenÅ aus kann man eine EinfÅgeoption wÑhlen, die Grafik-Daten aus der
Zwischenablage einfÅgt.
Folgende Optionen stehen zur VerfÅgung&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.EinfÅgen einer Grafik:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP_METAFILE.EinfÅgen einer Metadatei:elink.
:esl.

:h4 res=IDM_PASTE_FROM_CLIP_BITMAP.EinfÅgen einer Grafik (MenÅ Option)
Eine in der Zwischenablage plazierte Grafik kann Åber diese MenÅoption
in das Dokument eingefÅgt werden. Wenn Sie diese MenÅoption gewÑhlt haben,
mÅssen Sie mit der Maus eine der Ecken des Bereichs fÅr die Grafik anwÑhlen.
DrÅcken Sie dann die linke Maustaste, halten Sie den Mausknopf gedrÅckt und
bewegen Sie die Maus zur gegenÅberligenden Ecke. WÑhrend Sie die Maus bewegen
wird sich ein Rechteck îffnen. Wenn Sie mit der Grî·e und der Position zufrieden
sind, lassen Sie den Mausknopf los. Die Grafik wird jetzt in das Rechteck eingefÅgt
und dabei je nach Bedarf gestreckt oder gestaucht.

:h4 res=IDM_PASTE_FROM_CLIP_METAFILE.EinfÅgen einer Metadatei (MenÅ Option)
Wenn sich eine Grafik im Meta-Format in der Zwischenablage befindet, kann man
sie mit dieser MenÅoptions ins Dokument einsetzen. Wenn Sie die Option
ausgewÑhlt haben, werden Sie aufgefordert die Zielposition in der Dokumentenseite
anzugeben genauso wie bei der
bei :link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.EinfÅgen einer Grafik:elink. MenÅ
Option.

:h3 res=IDM_PASTE_FROM_SCRN.EinfÅgen vom Bildschirm MenÅ
Dieses MenÅ stellt Optionen bereit, die ein EinfÅgen beliebiger
Bildschirmbereiche ermîglichen.
Die folgenden Optionen stehen zur VerfÅgung&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_ENTIRE.EinfÅgen der gesamten ArbeitsoberflÑche:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_FRAME.EinfÅgen eines Fensterrahmens:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_WINDOW.EinfÅgen eines Fensters:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_RECT.EinfÅgen eines rechteckigen Bereiches:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_HIDE.Hide:elink.
:esl.

:h4 res=IDM_PASTE_FROM_SCRN_ENTIRE.EinfÅgen der gesamten ArbeitsoberflÑche (MenÅ Option)
Wenn Sie diese MenÅoption wÑhlen, wird eine Bitmap-Kopie der gesamten aktuellen
ArbeitsoberflÑche gemacht und Sie werden aufgefordert die Zielposition anzugeben, so wie
bei der :link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.EinfÅgen einer Grafik:elink. MenÅ Option.
:note.Seien Sie vorsichtig mit dieser Art der EinfÅgung, da die Grafik-Daten sehr
umfangreich sein kînnen. UnabhÑngig von der Skalierung des Bildes kînnen in
AbhÑngigkeit von der gewÑhlten Vide-Auflîsung und Farbtiefe 100k bis 800k Bytes
Daten zusammenkommen.

:h4 res=IDM_PASTE_FROM_SCRN_FRAME.EinfÅgen eines Fensterrahmens (MenÅ Option)
Diese Funktion ist noch nicht iplementiert.

:h4 res=IDM_PASTE_FROM_SCRN_WINDOW.EinfÅgen eines Fensters (MenÅ Option)
Diese Funktion ist noch nicht iplementiert.

:h4 res=IDM_PASTE_FROM_SCRN_RECT.EinfÅgen eines recheckigen Bereichs (MenÅ Option)
Diese Funktion ist noch nicht iplementiert.

:h4 res=IDM_PASTE_FROM_SCRN_HIDE.Hide (MenÅ Option)
This menu option, when checked, causes the application to be hidden
when any one of the
:link reftype=hd res=IDM_PASTE_FROM_SCRN.Paste From Screen Menu:elink.
options is requested.  The application is hidden, the Paste From command
is executed, and then the application is restored to its original state
on the system desktop.

:h3 res=IDM_PASTE_FROM_FILE.EinfÅgen einer Grafik aus einer Datei (MenÅ Option)
Bei der Anwahl dieser Option wird der normale Dateidialog angezeigt, von dem aus
Sie eine einzufÅgende Grafik-Datei auswÑhlen kînnen. Die zur Zeit unterstÅtzten
Dateiformate sind&colon.
:sl compact.
:li.&sqbul.IBM OS/2 Bitmap (.BMP)
:li.&sqbul.IBM OS/2 MetaFile (.MET)
:li.&sqbul.Microsoft Windows Bitmap (.BMP)
:li.&sqbul.Graphic Interchange Format (.GIF)
:li.&sqbul.Joint Photographic Experts Group File Interchange Format (.JPG,.JPEG,.JFIF)
:li.&sqbul.PC Paintbrush Datei Format (.PCX or .PCC)
:li.&sqbul.Tag Image File Format (.TIF or .TIFF)
:esl.
:p.
Nachdem Sie eine Datei ausgewÑhlt haben, werden Sie aufgefordert die Zielposition
anzugeben, so wie bei
der :link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.EinfÅgen einer Grafik:elink. MenÅ
Option.

:h2 res=IDM_INSERT_FUNCTION.Funktion einfÅgen MenÅ
Dieses UntermenÅ erlaubt es Ihnen, einige spezielle Funktionscodes in das
Dokument einzusetzen. Die folgenden Optionen stehen zur Zeit zur
VerfÅgung&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_DATE.Datum:elink.
:li.&sqbul.:link reftype=hd res=IDM_CURRENTPAGE.Aktuelle Seitennummer:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGECOUNT.Gesamtanzahl der Seiten:elink.
:li.&sqbul.:link reftype=hd res=IDM_SEPARATOR.Separator:elink.
:li.&sqbul.:link reftype=hd res=IDM_BARCODE.Barcode...:elink.
:esl.
:note.Dieses UntermenÅ ist nicht Teil des Markierter Text MenÅs.

:h3 res=IDM_DATE.Datum MenÅ
Dieses UntermenÅ ermîglicht Ihnen den Zugang zu den Datumsfunktionen.
Zur Zeit gibt es zwei Optionen&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_DATETODAY.Heutiges Datum:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATECURRENT.Aktuelles Datum:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATECHANGED.Dokumentn Datum:elink.
:esl.
:note.Dieses UntermenÅ ist nicht Teil des Markierter Text MenÅs.

:h4 res=IDM_DATETODAY.Heutiges Datum (MenÅ Option)
 Wenn dieser MenÅpunkt gewÑhlt wird, erfolgt die Einsetzung eines speziellen
Objektes im Dokument, welches fest das heutige Datum anzeigt. Dieses Datum
Ñndert sich nie und kann als Objekt nicht editiert werden. Wenn Sie mit dem
Cursor darÅber gehen verhÑlt es sich wie ein einzelnes Zeichen oder Symbol.
Das Format, indem es angezeigt wird, kann vom
:link reftype=hd res=IDD_DATEFMT.Datum Format...:elink. Dialog verÑndert werden.
Sie mÅssen die énderung im Text vor dem Datum vornehmen, da ein Format erst
ab der Position der énderung gÅltig ist.
:note.Diese Option ist nicht Teil des Markierter Text MenÅs.

:h4 res=IDM_DATECURRENT.Aktuelles Datum (MenÅ Option)
 Diese Funktion setzt ein spezielles Objekt in den Text ein, welches immer das
aktuelle Datum anzeigt. Es nimmt automatisch den Wert des aktuellen Datums an,
wenn das Dokument angezeigt oder gedruckt wird. Das Objekt kann nicht editiert
werden. Wenn Sie den Cursor Åber das Objekt bewegen, verhÑlt es sich wie ein
einzelnes Zeichen. Das dargestellte Format kann vom
:link reftype=hd res=IDD_DATEFMT.Datum Format...:elink. Dialog verÑndert werden.
Sie mÅssen das Format oberhalb der Objektposition im Dokument verÑndern,
um eine entsprechende Wirkung zu erzeilen.
:note.Diese Option ist nicht Teil des angewÑhlten Text MenÅs.

:h4 res=IDM_DATECHANGED.Dokumenten Datum (MenÅ Option)
 Diese Funktion setzt ein spezielles Objekt in den Text ein, welches immer das
Datum der letzten DokumentenÑnderung anzeigt. Das Datum Ñndert sich, wenn das
Dokument aktualisiert wird. Das Objekt kann nicht editiert
werden. Wenn Sie den Cursor Åber das Objekt bewegen, verhÑlt es sich wie ein
einzelnes Zeichen. Das dargestellte Format kann vom
:link reftype=hd res=IDD_DATEFMT.Datum Format...:elink. Dialog verÑndert werden.
Sie mÅssen das Format oberhalb der Objektposition im Dokument verÑndern,
um eine entsprechende Wirkung zu erzeilen.
:note.Diese Option ist nicht Teil des angewÑhlten Text MenÅs.

:h4 res=IDM_DATEFMT.Datum Format... (MenÅ Option)
 Wenn diese MenÅ Option gewÑhlt wird, erscheint der
:link reftype=hd res=IDD_DATEFMT.Datum Format...:elink. Dialog,
mit dem man das Format zur Erzeugung von Datumszeichenketten verÑndern kann.

:h5 res=IDD_DATEFMT.Datum Format... (Dialog)
 Von diesem MenÅ aus kann man das Format fÅr die Datumerzeugung einstellen.
:artwork name='Deutsch\DateFmt.bmp' align=center.
:p.Das Datumformat ist so, wie Sie es im Eingabefeld einstellen mit der EinschrÑnkung,
da· bestimmte Buchstaben mit einem vorangestellten &percent.-Zeichen eine besondere
Bedeutung haben. Die speziellen Codes sind die folgenden&colon.
:sl compact.
:li.&percent.w - Dieser Code wird zum Tag der Woche (Sonntag, Montag, etc.).
:li.&percent.x - This code becomes the abbreviated day of the week (Sun, Mon, etc.).
:li.&percent.t - Dieser Code wird in numerischer Form zum Tag des Monats.
:li.&percent.m - Dieser Code wird in numerischer Form zum Monat des Jahres.
:li.&percent.M - Dies ist der Monat des Jahres (Januar, Februar, etc.).
:li.&percent.n - This is the abbreviated month of the year (Jan, Feb, etc.).
:li.&percent.j - Dies ist das Jahr in numerischer Form (alle 4 Stellen).
:li.&percent.k - This is the abbreviated year (2 digits).
:esl.
:p.
Jeder andere Text der im Eingabefeld erscheint, wird direkt in das zu
erzeugende Datum kopiert. Zum Beispiel wird die Format-Zeichenkette
"&percent.w, &percent.t. &percent.M, &percent.j" zu "Mittwoch, 16. MÑrz, 1994".
:p.
Sie kînnen das neue Datumsformat eingeben und es durch anklicken des
OK Knopfes abspeichern. Das aktive Dokument wird sofort entsprechend
reformatiert.
:p.
Wenn Sie den Anwenden Knopf drÅcken, wird das neue Datumsformat auf das
aktive Dokument angewendet und werden die neuen Standardeinstellungen.
:p.
Wenn Sie den Dialog abbrechen mîchten ohne die énderungen zu speichern,
kînnen Sie ESC tippen oder auf den ESC Knopf klicken.
énderungen die Sie mit Anwenden herbeigefÅhrt haben werden nicht verworfen.

:h3 res=IDM_CURRENTPAGE.Aktuelle Seitennummer (MenÅ Option)
Diese MenÅoption setzt ein spezielles Objekt in den Text ein,
welches die aktuelle Seitennummer auf der sich das Objekt befindet anzeigt.

:h3 res=IDM_PAGECOUNT.Gesamtanzahl der Seiten (MenÅ Option)
Diese MenÅoption setzt ein spezielles Objekt in den Text ein,
welches die Gesamtanzahl der Seiten des Dokuments anzeigt.

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
:artwork name='Deutsch\BarCode.bmp' align=center.
You need to select the type of the barcode from the list
provided, and then enter the text to be barcoded in the 
entry field.  When you are ready, press ENTER or click on the
OK button, at which point you are prompted to position
the graphic on the current document page, just as was done for the 
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Paste Bitmap from Clipboard:elink. 
menu option.

:h2 res=IDM_SEARCH.Suchen... (MenÅ Option)
Dieses MenÅ aktiviert einen Dialog, von dem aus Sie eine zu suchende und eine
auf Wunsch zu ersetzende Zeichenkette eingeben kînnen.
:p.Siehe auch unter :link reftype=hd res=IDD_SEARCH.Suchen... (Dialog):elink. fÅr mehr
Informationen.
:p.Die Schnellwahl fÅr diese Funktion ist Strg+S.

:h3 res=IDD_SEARCH.Suchen... (Dialog)
Bei diesem Dialog geben Sie den zu suchenden Text fÅr die :link reftype=hd res=IDM_SEARCH.Suchen:elink.
und :link reftype=hd res=IDM_FINDNEXT.Finde nÑchstes:elink.
Kommandos ein. Es ist auch mîglich einen Ersatz Text einzugeben.
:artwork name='Deutsch\Search.bmp' align=center.
:p.Um einen bestimmten text zu finden, geben Sie erst den Text im vorgesehenen
Feld ein und drÅcken dann <EINGABE> oder klicken auf den Finde Knopf.
Der Text wird gesucht und wird markiert. Der Dialog bleibt aktiv, um Ihnen
die Mîglichkeit der énderung zu geben wenn Sie dies wÅnschen.
:p.Um einen Text durch einen anderen zu ersetzen, geben Sie den Originaltext im
ersten und den zu ersetzenden Text im zweiten Eingabefeld ein. Danach betÑtigen
Sie  die Eingabetaste um den zu ersetzenden Text zu finden oder Sie klicken auf
den Ersetze Knopf und ersetzen ihn sofort. Sie kînnen das Ersetzen fÅr jedes
Vorkommen des Textes wiederholen.
:p.To replace a text with another, and then proceed to find the next occurence of
the original text, use the Change &amp. Find button.
:p.To replace all occurences of the original text with the new text, use the
Change All button.
:p.Wenn die Textsuche Gro·- Kleinschreibung berÅcksichtigen soll kînnen Sie den
eintsprechenden Check-Knopf aktivieren oder deaktivieren.
:p.If you wish your searches to obey the rules of UNIX Regular Expressions, 
you may check the Regular Expression button.
:p.Um den Dialog zu schlie·en kînnen Sie die ESC Taste drÅcken oder auf den
Escape Knopf klicken. Die Inhalte der Eingabefelder werden in diesem Fall fÅr
eine erneute Aktivierung zwischengespeichert.

:h2 res=IDM_FINDNEXT.Finde nÑchstes (MenÅ Option)
Wenn sie einen Suchtext im
:link reftype=hd res=IDD_SEARCH.Suche... (Dialog):elink. Dialog eingegeben haben,
dann sucht diese MenÅfunktion im Text vorwÑrts bis zum nÑchsten auftreten
des angegebenen Textes und markiert ihn.
:p.Siehe auch :link reftype=hd res=IDD_SEARCH.Suche... (Dialog):elink. fÅr weitere
Informationen.
:p.Die Kurzwahl fÅr diese Funktion ist Strg+F.

:h2 res=IDM_GOTOPAGE.Go To Page (Menu Option)
This menu option summons a dialog from which you can specify the 
number of the page you wish to have displayed for editing.
See the :link reftype=hd res=IDD_GOTOPAGE.Go To Page:elink.
dialog help for more information.

:h3 res=IDD_GOTOPAGE.Go To Page (Dialog)
This dialog allows you to specify the number of the document page
that which you wish to display and edit.  Enter the page number
in the field provided and press the ENTER key, or press the ESC
key to abort this function.

:h2 res=IDM_SPELL.Spell Check (MenÅ Option)
Wenn das optionale RechtschreibprÅfungs Modul auf Ihrem System installiert ist
und es in der Lage war sein Wîrterbuch zu îffnen, dann ist diese MenÅ zugÑnglich.
Wenn Sie auf diese MenÅoption klicken, wird die RechtschreibprÅfung Ihr Dokument
von der augenblicklichen Position abwÑrts nach falsch geschriebenen Worten
absuchen. Wenn ein solches Wort gefunden wird, dann wird es markiert und der
:link reftype=hd res=IDD_SPELL.RechtschreibprÅfung:elink.
Dialog wird angezeigt. Sie kînnen dann ein korrigiertes Wort eingeben.
:p.Die Kurzwahl fÅr diese Funktion ist F5.

:h3 res=IDD_SPELL.RechtschreibprÅfung (Dialog)
Dieser Dialog wird angezeigt, wenn die RechtschreibprÅfung ein mîglicherweise
falsch geschriebenes Wort gefunden hat. Es wird au·erdem eine Liste von
mîglichen Ersatzworten aus dem Wîrterbuch angezeigt.
:artwork name='Deutsch\Spell.bmp' align=center.
:p.Wenn Sie das Wort Åberspringen wollen, kînnen Sie die Eingabe Taste betÑtigen
oder auf den :hp4.öberspringen:ehp4. Knopf klicken. Dies ist die Standardaktion.
:p.Wenn Sie mîchten, da· die RechtschreibprÅfung das Wort fÅr den Rest der
öberprÅfungen ignoriert, dann klicken Sie auf den :hp4.Akzeptieren:ehp4. Knopf.
:p.Um das Wort unter Beibehaltung von Gro·- Kleinschrift in Ihr persîhnliches
Wîrterbuch zu Åbernehmen, klicken Sie mit der Maus auf den :hp4.EinfÅgen:ehp4.
Knopf.
:p.Um das Wort in erzwungener Kleinschrift in Ihr persîhnliches Wîrterbuch zu
Åbernehmen, klicken Sie auf den :hp4.EinfÅgen in Kleinschrift:ehp4. Knopf.
Solche EintrÑge werden mit klein- und nicht kleingeschriebenen EintrÑgen
verglichen.
:p.Wenn Sie eines der angebotenen Ersatzworte einsetzen wollen, dann wÑhlen
Sie das Wort aus der Liste und betÑtigen Sie die EINGABE oder doppelklicken Sie
auf den Eintrag.
:p.Wenn Sie ein korrigiertes Wort, das noch nicht in der Liste ist, eingeben
wollen, geben Sie das neue Wort im Eingabefeld ein und klicken Sie dann auf
den :hp4.Ersetze:ehp4. Knopf.
Das Originalwort im Text wird durch das einegebene Wort ersetzt.
:p.Um die RechtschreibprÅfung ohne weitere Aktion zu verlassen, geben Sie ESC ein
oder klicken Sie mit der Maus auf den :hp4.Escape:ehp4. Knopf. Alle bisher vorgenommenen
énderungen bleiben erhalten. Sie kînnen aber mit
dem :link reftype=hd res=IDM_UNDO.Undo:elink. Kommando rÅckgÑngig gemacht werden.

:h2 res=IDM_ZOOM.Ma·stab (UntermenÅ)
 Dieser MenÅpunkt zeigt UntermenÅs fÅr verschieden Dokumentansichten und
Ma·stÑbe. Folgende Kommandos stehen zur VerfÅgung&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_100.Vergrî·erung 100&percent.:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLWIDTH.Vergrî·erung volle Breite:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLHEIGHT.Vergrî·erung volle Hîhe:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_MARGINS.Vergrî·erung RÑnder:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_ALL.Vergrî·erung Alles:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_WINDOW.Vergrî·erung Fenster:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_SET.Vergrî·erung Stufenlos...:elink.
:esl.

:h3 res=IDM_ZOOM_100.Vergrî·erung 100&percent. (MenÅ Option)
 Dieses Kommando setzt die Ansichtskalierung auf 100&percent.. Die Ansicht stimmt
jetzt mit dem zu erwartenden Druckbild Åberein. Vorausgesetzt die System
Skalierung (auf die ich keinen Einflu· habe) ist richtig eingestellt.

:h3 res=IDM_ZOOM_FULLWIDTH.Vergrî·erung Volle Breite (MenÅ Option)
 Dieses Kommando setzt die Ansichtskalierung so, da· die Seite die gesamte
Grî·e des Anzeigefensters einnimmt. Dies kann das Dokument vergrî·ern oder
verkleinern, je nachdem wie Sie die Fenstergrî·e gewÑhlt haben.

:h3 res=IDM_ZOOM_FULLHEIGHT.Vergrî·erung Volle Hîhe (MenÅ Option)
 Dieses Kommando setzt die Ansichtskalierung so, da· die Seite die gesamte
Hîhe des Anzeigefensters einnimmt. Normalerweise fÅhrt dies zu einer
Verkleinerung der Dokumentenskalierung mit der Folge, da· die Anzeige
kleiner als der spÑtere Ausdruck erscheint.

:h3 res=IDM_ZOOM_MARGINS.Vergrî·erung RÑnder (MenÅ Option)
 Dieses Kommando setzt die Ansichtskalierung so, da· der Seitenteil des
Dokuments zwischen den horizontalen RÑndern die gesamte
Breite des Anzeigefensters einnimmt. Normalerweise fÅhrt dies zu einer
Vergrî·erung der Dokumentenskalierung in AbhÑngigkeit der von Ihnen gewÑhlten
Fenstergrî·e.

:h3 res=IDM_ZOOM_ALL.Vergrî·erung Alles (MenÅ Option)
 Dieses Kommando setzt die Ansichtskalierung auf den kleineren der beiden Werte
Volle Hîhe und Volle Breite. Damit wird erreicht, da· die komplette Seite
im Anzeigefenster dargestellt wird, egal wie klein oder schlecht proportioniert
das Anzeigefenster ist.
:p.The short-cut key for this function is Strg+Z, and when used this way the
function toggles between Zoom All and whatever preferred Zoom is specified
in the :link reftype=hd res=IDD_PREFERENCES.General Preferences:elink. dialog.

:h3 res=IDM_ZOOM_WINDOW.Vergrî·erung Fenster (MenÅ Option)
 Mit diesem Kommodo kînnen Sie einen rechteckigen Bereich im aktiven Dokument
markieren und das Dokument auf diesen Bereich vergrî·ern. Wenn der Bereich definiert
ist, hat das Dokument einen neuen Vergrî·erungsfaktor der bei folgenden
Repositionierungen angewendet wird.

:h3 res=IDM_ZOOM_SET.Vergrî·erung Stufenlos... (MenÅ Option)
 Dieses Kommando aktiviert einen Dialog, von dem aus Sie den
Vergrî·erungsfaktor fÅr das Dokumentenfenster einstellen kînnen.
:p.Siehe auch unter :link reftype=hd res=IDD_ZOOM_SET.Vergrî·erung Stufenlos... (Dialog):elink.

:h4 res=IDD_ZOOM_SET.Vergrî·erung Stufenlos... (Dialog)
 Dieser Dialog erlaubt es Ihnen, den Vergrî·erungsfaktor auf einen Wert Ihrer
Wahl einzustellen. Sie kînnen die hoch- und runter Pfeile benutzen, um den
Wert zu verÑndern oder Sie kînnen den Wert direkt eingeben. Danach betÑtigen
Sie die Eingabe Taste oder klicken auf OK um den neuen Faktor wirksam zu
machen.
:artwork name='Deutsch\ZoomSet.bmp' align=center.
:p.Um den neuen Faktor anzuwenden ohne den Dialog zu beenden, kînnen Sie auf den
Anwenden Knopf klicken. So eingestellte Vergrî·erungsfaktoren bleiben auch wirksam,
nachdem der Dialog beendet ist oder ESCAPE gedrÅckt wurde.
:p.Um ohne weitere Wirkung den Dialog zu beenden, kînnen Sie die ESCAPE Taste
oder den ESCAPE Knopf drÅcken.
énderungen die mit Anwenden herbeigefÅhrt wurden, werden nicht zurÅckgenommen.

:h1 res=IDM_LAYOUT_MENU.Ansicht MenÅ
:i1.Ansicht MenÅ
Vom Ansicht MenÅ aus sind folgende Kommandos verfÅgbar&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_MARGINS.RÑnder...:elink.
:li.&sqbul.:link reftype=hd res=IDM_TABS.Tabulatoren...:elink.
:li.&sqbul.:link reftype=hd res=IDM_INDENT.EinrÅckungen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINESPACING_MENU.Zeilenabstand MenÅ:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUSTIFICATION.Ausrichtung MenÅ:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGECENTER.Seitenzentrierung:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGEBREAK.Seitenumbruch:elink.
:li.&sqbul.:link reftype=hd res=IDM_HEADER.Header...:elink.
:li.&sqbul.:link reftype=hd res=IDM_FOOTER.Footer...:elink.
:esl.

:h2 res=IDM_MARGINS.RÑnder... (MenÅ Option)
 Dieses Kommando aktiviert einen Dialog, von dem aus alle vier DruckrÑnder
(oben, unten, links, rechts) eingestellt werden kînnen.
:p.Siehe auch unter :link reftype=hd res=IDD_MARGINS.RÑnder (Dialog):elink.

:h3 res=IDD_MARGINS.RÑnder (Dialog)
 öber diesen Dialog kînnen sie den linken, rechten, oberen und unteren
Druckerrand einstellen. RÑnder werden im aktuell ausgewÑhlten Ma·system
(englisch, metrisch) eingestellt und die AbstÑnde zÑhlen von links bzw.
von unten.
:artwork name='Deutsch\Margins.bmp' align=center.
:p.Wenn Sie neue Werte fÅr die RÑnder eingeben werden die Lineale des aktiven
Dokuments entsprechend eingestellt; das Dokument selbst wird nicht neu
formatiert.
:p.Geben Sie die AbstÑnde ein und drÅcken Sie dann EINGABE oder klicken Sie
auf den OK Knopf. Wenn die AbstÑnde nicht zum ausgewÑhlten Formular passen,
werden Sie eine Fehlermeldung erhalten und Sie kînnen die Eingabe wiederholen.
:p.Wenn Sie den Anwenden Knopf betÑtigen werden alle Einstellungen auf das aktive
Dokument angewendet und sind die neuen Randeinstellungen.
:p.Wenn Sie den Dialog abbrechen und die Werte verwerfen wollen, drÅcken Sie die
ESC Taste oder klicken auf den ESCAPE Knopf.
énderungen die mit Anwenden herbeigefÅhrt wurden, werden nicht zurÅckgenommen.
:note.Die neuen linken und rechten RÑnder werden ab der nÑchsten Zeile im
Dokument wirksam, es sei denn, da· Kommando wurde am Anfag einer Zeile aktiviert.
Die neuen oberen und unteren RÑnder werden ab der nÑchsten Seite wirksam,
es sei denn, da· Kommando wurde am Anfang einer Seite eigegeben.

:h2 res=IDM_TABS.Tabulatoren... (MenÅ Option)
 Dieses Kommando îffnet einen Dialog, von dem aus man die Tabulatoren einstellen
kann. Die neuen Tabulatoren gelten ab der Cursorposition.
:p.Siehe auch unter :link reftype=hd res=IDD_TABS.Tabulatoren (Dialog):elink.

:h3 res=IDD_TABS.Tabulatoren (Dialog)
 Dieser Dialog erlaubt es, Tabulatorpositionen einzugeben und sie zu setzen
oder zu lîschen. Ihre Eingaben sind sofort Åber das horizontale Lineal
sichtbar.
:artwork name='Deutsch\Tabs.bmp' align=center.
:p.Um einen Tabulator zu setzen, geben Sie die Position im Eingabefeld ein und
drÅcken den &apos.Setzen&apos. Knopf.
:p.Um einen Tabulator zu lîschen, geben Sie die Position im Eingabefeld ein und
drÅcken den &apos.Lîschen&apos. Knopf.
:p.Wenn Sie alle gewÅnschten énderungen der Tabulatoren eingegeben haben, mÅssen
Sie EINGABE oder OK drÅcken. Die neuen Werte gelten ab der Cursorposition.
:p.Wenn Sie den Anwenden Knopf drÅcken, werden die neuen Tabulatorwerte auf das
aktive Dokument angewendet und sind die neuen Standardwerte.
:p.Wenn Sie den Dialog abbrechen und die Werte verwerfen wollen, drÅcken Sie die
ESC Taste oder klicken auf den ESCAPE Knopf.
énderungen die mit Anwenden herbeigefÅhrt wurden, werden nicht zurÅckgenommen.

:h2 res=IDM_INDENT.EinrÅckungen... (MenÅ Option)
 Von dieser MenÅoption wird ein Dialog gestartet, Åber den Sie die EinrÅckungen
fÅr jeden Absatz definieren kînnen.
:p.Siehe auch unter :link reftype=hd res=IDD_INDENT.Paragraph Indent (Dialog):elink.

:h3 res=IDD_INDENT.Absatz EinrÅckungen (Dialog)
Von diesem Dialog aus kann man die AbsatzeinrÅckungen einstellen, die ab
der augenblicklichen Position im Dokument gelten.
:artwork name='Deutsch\Indent.bmp' align=center.
:p.Sie geben die neuen EinrÅckungswerte an und klicken den OK Knopf um die Werte
zu speichern. Das aktive Dokumnet wird sofort reformatiert.
:p.Wenn Sie auf Anwenden klicken, wird die neue EinrÅckung auf das gesamte Dokument
angewendet und wird der neue Standardwert.
:p.Wenn Sie den Dialog abbrechen und die Werte verwerfen wollen, drÅcken Sie die
ESC Taste oder klicken auf den ESCAPE Knopf.
énderungen die mit Anwenden herbeigefÅhrt wurden, werden nicht zurÅckgenommen.

:h2 res=IDM_LINESPACING_MENU.Zeilenabstand MenÅ
Dieses UntermenÅ zeigt die verfÅgbaren unterschiedlichen ZeilenabstÑnde an.
:p.Der Eintrag des aktiven Zeilenabstandes ist markiert.
:p.Siehe auch :link reftype=hd res=IDM_LINESPACINGS.Zeilenabstand MenÅ Eintargungen:elink.
fÅr mehr Informationen.

:h3 res=IDM_LINESPACINGS.Zeilenabstand MenÅ EintrÑge
Dieses MenÅ zeigt eine der augenblicklich vorhandenen Zeilenabstands Optionen.
ZeilenabstÑnde sind als Prozentwerte bezogen auf den normalen Zeilenabstand
des augenblicklich aktiven Zeichensatzes und der aktiven Grî·e angegeben.
Wenn ein bestimmter Zeilenabstand gewÑhlt ist, gilt dieser fÅr den markierten
Textblock oder ab dem Cursor abwÑrts.

:h3 res=IDM_LINESPACINGMGR.Line Spacing Menu Manager
This menu option summons the :link reftype=hd res=IDD_LINESPACINGMGR.Line Spacing Menu Manager:elink. dialog,
from which it is possible to customize the contents of the Line Spacing Menu.

:h4 res=IDD_LINESPACINGMGR.Line Spacing Menu Manager Dialog
This dialog displays the current configuration of the Line Spacing Menu, and
allows it to be changed.:p.
The current menu is displayed in the list-box to the right.  If you click
on an entry, the setting will be displayed with the slider control on the left
and the appearance of the spaced text lines appears in the sample area in 
the middle area of the dialog.:p.
Use the slider control to alter the current line spacing.  To add the new
line spacing to the menu, click on the &apos.&gt.&gt.&apos. button.:p.
To remove a line spacing from the menu, click on the menu entry in the list-box
and then click on the &apos.&lt.&lt.&apos. button.:p.
To restore the line spacing menu to its default settings, click on the &apos.Default&apos.
button.

:h2 res=IDM_JUSTIFICATION.Ausrichtung MenÅ
 Dieses UntermenÅ zeigt die verschiedenen verfÅgbaren horizontalen
Textausrichtungen. Es sind die folgenden&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_JUST_LEFT.Links:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_CENTER.Zentriert:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_RIGHT.Rechts:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_FULL.Blocksatz:elink.
:esl.

:h3 res=IDM_JUST_LEFT.Links (MenÅ Option)
 Diese MenÅoption bewirkt, da· der Text ab der nÑchsten Zeile linksbÅndig
formatiert wird. Wenn die Option am Anfang einer Zeile aktiviert wird, gilt
sie sofort.

:h3 res=IDM_JUST_CENTER.Zentriert (MenÅ Option)
 Diese MenÅoption bewirkt, da· der Text ab der nÑchsten Zeile zentriert
formatiert wird. Wenn die Option am Anfang einer Zeile aktiviert wird, gilt
sie sofort.

:h3 res=IDM_JUST_RIGHT.Rechts (MenÅ Option)
 Diese MenÅoption bewirkt, da· der Text ab der nÑchsten Zeile rechtsbÅndig
formatiert wird. Wenn die Option am Anfang einer Zeile aktiviert wird, gilt
sie sofort.

:h3 res=IDM_JUST_FULL.Blocksatz (MenÅ Option)
 Diese MenÅoption bewirkt, da· der Text ab der nÑchsten Zeile im Blocksatz
formatiert wird. Wenn die Option am Anfang einer Zeile aktiviert wird, gilt
sie sofort.
Beim Blocksatz werden Leerzeichen in den Text eingefÅgt, so da· er links und
rechts abschlie·t. Zeilen die mit einem Zeilen- oder Seitenumbruch enden,
werden linksbÅndig justiert.

:h2 res=IDM_PAGECENTER.Seitenzentrierung (MenÅ Option)
 Diese MenÅoption bewirkt, da· das Dokument ab der nÑchsten Seite vertikal
zentriert formatiert wird. Wenn die Option am Anfang einer Zeile aktiviert
wird, gilt sie sofort.
:note.Diese Funktion kann nicht in Verbindung mit Grafik angewendet werden und
ist automatisch deaktiviert, wenn sich Grafik auf einer Seite befindet.

:h2 res=IDM_PAGEBREAK.Seitenumbruch (MenÅ Option)
 Bei Aktivierung dieses Kommandos wird ein Seitenumbruch an der Cursorposition
in den Text eingefÅgt. Der Cursor wird an den Anfang der nÑchsten Seite gestellt.
:p.Die Schnellauswahl fÅr diese Funktion ist STRG+EINGABE.
:note.Diese Option ist nicht Teil des angewÑhlten Text MenÅs.

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

:h1 res=IDM_FONT_MENU.Schriftart MenÅ
:i1.Schriftart MenÅ
Dieses MenÅ zeigt eine Liste aller vorhandenen Schriftarten.
:p.Die unter dem Cursor aktuell aktive Schriftart ist markiert.
:p.Siehe auch unter :link reftype=hd res=IDM_FONTS.Schriftart MenÅ Eintragungen:elink.
fÅr weitere Informationen.

:h2 res=IDM_FONTS.Schriftart MenÅ EintrÑge
Dieses MenÅ zeigt eine der vorhandenen Schriftarten mit 18 Punkten, so wie sie
im Dokument erscheinen wÅrde. Wenn Sie diese Schriftart wÑhlen wollen, gehen Sie
mit den Pfeiltasten darauf und betÑtigen Sie die EINGABE oder klicken Sie
mit der Maus darauf.

:h2 res=IDM_FONTS_PGUP.Schriftart MenÅ Seite hoch
Diese MenÅoption erscheint, falls nicht alle Schriftarten gleichzeitig
dargestellt werden kînnen und vor der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h2 res=IDM_FONTS_PGDN.Schriftart MenÅ Seite runter
Diese MenÅoption erscheint, falls nicht alle Schriftarten gleichzeitig
dargestellt werden kînnen und hinter der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h2 res=IDM_FONTMGR.Schriftart MenÅ Manager
Diese Funktion ist noch nicht implementiert.

:h1 res=IDM_SIZE_MENU.Grî·e MenÅ
:i1.Grî·e MenÅ
Dieses MenÅ zeigt eine Liste aller vorhandenen Schriftart Grî·en.
:p.Die augenblicklich an der Cursorposition aktive Grî·e ist markiert.
:p.Siehe auch unter :link reftype=hd res=IDM_SIZES.Grî·e MenÅ Eintragungen:elink.
fÅr weitere Informationen.

:h2 res=IDM_SIZES.Grî·e MenÅ EintrÑge
Dieser MenÅ Eintrag zeigt einen der vorhandenen SchriftarteintrÑge in allen
vorhandenen Darstellungsgrî·en. Wenn Sie eine Grî·e fÅr den augenblicklich
markierten Text oder ab der Cursorposition selektieren wollen, dann wÑhlen
Sie den Eintrag mit den Pfeiltasten und drÅcken Sie dann auf EINGABE oder
klicken Sie mit der Maus darauf.

:h2 res=IDM_SIZES_PGUP.Grî·e MenÅ Seite hoch
Diese MenÅoption erscheint, falls nicht alle Grî·en gleichzeitig
dargestellt werden kînnen und vor der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h2 res=IDM_SIZES_PGDN.Grî·e MenÅ Seite runter
Diese MenÅoption erscheint, falls nicht alle Grî·en gleichzeitig
dargestellt werden kînnen und hinter der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h2 res=IDM_SIZEMGR.Grî·e MenÅ Manager
Diese Funktion ist noch nicht implementiert.

:h1 res=IDM_ATTR_MENU.Stil MenÅ
:i1.Stil MenÅ
 Vom Stil MenÅ aus ist es mîglich, Schriftart Stilkommandos in das Dokument
einzufÅgen. Alle normalerweise von der BenutzeroberflÑche unterstÅtzten
Stile stehen zur VerfÅgung: Normal, Fett, Kursiv, Unterstrichen,
Durchgestrichen und Umri·. Um einen Stil auf den Text ab der Cursorposition
oder auf einen markierten Bereich anzuwenden, wÑhlen Sie einfach mit der
Tastatur oder der Maus eine Option aus. Die EintrÑge des Stil MenÅs werden in
der augenblicklich aktiven Schriftart und Grî·e darsgestellt und der aktive
Stil ist markiert. Zu jeder Zeit kînnen alle Stile, mit Ausnahme von Normal,
gemischt aktiviert werden. Normal ist die Einstellung, bei der kein anderer
Stil aktiviert ist.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_NORMAL.Normal:elink.
:li.&sqbul.:link reftype=hd res=IDM_BOLD.Fett:elink.
:li.&sqbul.:link reftype=hd res=IDM_ITALIC.Kursiv:elink.
:li.&sqbul.:link reftype=hd res=IDM_UNDERSCORE.Unterstrichen:elink.
:li.&sqbul.:link reftype=hd res=IDM_STRIKEOUT.Durchgestrichen:elink.
:li.&sqbul.:link reftype=hd res=IDM_OUTLINE.Umri·:elink.
:li.&sqbul.:link reftype=hd res=IDM_CAPS.Gro·:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXPAND_MENU.Vergrî·erungs MenÅ:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINECOLOR_MENU.Linien/Text Farbe MenÅ:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEWIDTH_MENU.Linienbreite MenÅ:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILLCOLOR_MENU.FÅllfarbe MenÅ:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILLTYPE_MENU.FÅlltyp MenÅ:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEEND_MENU.Line End Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEJOIN_MENU.Line Join Menu:elink.
:esl.

:h2 res=IDM_NORMAL.Normal (MenÅ Option)
 Wenn diese MenÅoption ausgewÑhlt ist, werden eventuell aktivierte Stile
(wie z.B. Fett, Kursiv, ...) deaktiviert. Wird die Option aktiviert, wÑhrend
ein Textbereich markiert ist, Ñndert sich nur der Stil des markierten Textes.
Wenn nichts markiert war, gilt die neue Einstellung ab der Cursorposition.
:p.Die Schnellauswahl fÅr diese Funktion ist STRG+N.

:h2 res=IDM_BOLD.Fett (MenÅ Option)
 Wenn diese MenÅoption ausgewÑhlt ist, wird der markierte Text Fett dargestellt.
War kein Text markiert, gilt die neue Einstellung ab der Cursorposition.
Andere im Text eingebettete Stil Kommandos werden nicht beeinflu·t.
:p.Die Schnellauswahl fÅr diese Funktion ist STRG+B.

:h2 res=IDM_ITALIC.Kursiv (MenÅ Option)
 Wenn diese MenÅoption ausgewÑhlt ist, wird der markierte Text Kursiv dargestellt.
War kein Text markiert, gilt die neue Einstellung ab der Cursorposition.
Andere im Text eingebettete Stil Kommandos werden nicht beeinflu·t.
:p.Die Schnellauswahl fÅr diese Funktion ist STRG+I.

:h2 res=IDM_UNDERSCORE.Unterstrichen (MenÅ Option)
 Wenn diese MenÅoption ausgewÑhlt ist, wird der markierte Text Unterstrichen dargestellt.
War kein Text markiert, gilt die neue Einstellung ab der Cursorposition.
Andere im Text eingebettete Stil Kommandos werden nicht beeinflu·t.
:p.Die Schnellauswahl fÅr diese Funktion ist STRG+U.

:h2 res=IDM_STRIKEOUT.Durchgestrichen (MenÅ Option)
 Wenn diese MenÅoption ausgewÑhlt ist, wird der markierte Text Durchgestrichen dargestellt.
War kein Text markiert, gilt die neue Einstellung ab der Cursorposition.
Andere im Text eingebettete Stil Kommandos werden nicht beeinflu·t.

:h2 res=IDM_OUTLINE.Umri· (MenÅ Option)
 Wenn diese MenÅoption ausgewÑhlt ist, wird der markierte Text mit Umri·-Zeichen
dargestellt.
War kein Text markiert, gilt die neue Einstellung ab der Cursorposition.
Andere im Text eingebettete Stil Kommandos werden nicht beeinflu·t.

:h2 res=IDM_CAPS.Gro· (MenÅ Option)
 Wenn diese MenÅoption ausgewÑhlt ist, wird der markierte Text in Gro·schrift dargestellt.
War kein Text markiert, gilt die neue Einstellung ab der Cursorposition.
Andere im Text eingebettete Stil Kommandos werden nicht beeinflu·t.

:h2 res=IDM_EXPAND_MENU.Vergrî·erungs MenÅ
Diese MenÅ listet alle augenblicklich vorhandenen horizontalen
Vergî·erungsfaktoren auf.
:p.Die bei der Cursorposition aktive Vergrî·erung ist markiert.
:p.Siehe auch unter :link reftype=hd res=IDM_EXPANDS.Vergrî·erungs MenÅ EintrÑge:elink.
fÅr weitere Informationen.

:h3 res=IDM_EXPANDS.Vergrî·erung MenÅ EintrÑge
Dieser MenÅeintrag zeigt vorhandene horizontalen Vergrî·erungsfaktoren. Der Text
wird im gewÑhlten Zeichensatz mit unterschiedlichen Vergrî·erungsfaktoren
dargestellt.Um einen Vergrî·erungsfaktor auf den Text ab der Cursorposition
oder auf einen markierten Bereich anzuwenden, wÑhlen Sie einfach mit der
Tastatur oder der Maus eine Option aus.

:h3 res=IDM_EXPANDS_PGUP.Vergrî·erungs MenÅ Seite hoch
Diese MenÅoption erscheint, falls nicht alle Vergrî·erungsfaktoren gleichzeitig
dargestellt werden kînnen und vor der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h3 res=IDM_EXPANDS_PGDN.Vergrî·erungs MenÅ Seite runter
Diese MenÅoption erscheint, falls nicht alle Vergrî·erungsfaktoren gleichzeitig
dargestellt werden kînnen und hinter der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h3 res=IDM_EXPANDMGR.Vergrî·erungs MenÅ Manager
This menu option summons the :link reftype=hd res=IDD_EXPANDMGR.Expand Menu Manager:elink. dialog,
from which it is possible to customize the contents of the Expand Menu.

:h4 res=IDD_EXPANDMGR.Expand Menu Manager Dialog
This dialog displays the current configuration of the Expand Menu, and
allows it to be changed.:p.
The current menu is displayed in the list-box to the right.  If you click
on an entry, the setting will be displayed with the slider control at the lower left
and the appearance of the expanded text appears in the sample area in 
the upper left area of the dialog.:p.
Use the slider control to alter the current expansion factor.  To add the new
expansion factor to the menu, click on the &apos.&gt.&gt.&apos. button.:p.
To remove an expansion factor from the menu, click on the menu entry in the list-box
and then click on the &apos.&lt.&lt.&apos. button.:p.
To restore the expand menu to its default settings, click on the &apos.Default&apos.
button.

:h2 res=IDM_LINECOLOR_MENU.Linien/Text Farbe MenÅ
Dieses MenÅ zeigt alle vorhandenen Linien/Text Farben.
:p.Die am Cursor aktive Farbe ist markiert.
:p.Siehe auch unter :link reftype=hd res=IDM_LINECOLORS.Linien/Text Farb MenÅ EintrÑge:elink.
fÅr weitere Informationen.

:h3 res=IDM_LINECOLORS.Linien/Text Farbe MenÅ EintrÑge
Dieser MenÅeintarg zeigt eine der vorhandenen Linien/Text Farben. Um eine
Farbe von der Cursorposition abwÑrts zu setzen oder eine Farbe auf einen
selektierten Textblock anzuwenden, wÑhlen Sie die Farbe mit den Pfeiltasten
aus und betÑtigen die EINGABE oder Sie klicken mit der Maus auf den MenÅeintrag.
:note.Diese Farben kînnen fÅr Linien, Text und den Umri· von Umri·text vergeben
werden.

:h3 res=IDM_LINECOLORS_PGUP.Linien/Text Farb MenÅ Seite hoch
Diese MenÅoption erscheint, falls nicht alle MenÅeintrÑge gleichzeitig
dargestellt werden kînnen und vor der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h3 res=IDM_LINECOLORS_PGDN.Linien/Text Farbe MenÅ Seite runter
Diese MenÅoption erscheint, falls nicht alle MenÅeintrÑge gleichzeitig
dargestellt werden kînnen und hinter der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h3 res=IDM_LINECOLORMGR.Linien/Text Farbe MenÅ Manager
This menu option summons the 
:link reftype=hd res=IDD_LINECOLORMGR.Line Color Menu Manager:elink.
dialog, which gives the user the ability to customize the Line/Text
Color Menu.

:h4 res=IDD_LINECOLORMGR.Linien/Text Farbe MenÅ Manager (Dialog)
This dialog provides the user with a way to customize the Line/Text
Color Menu.  A color wheel and intensity control provide an easy way
to specify colors, and for precise control there is a set of red, green
and blue value controls as well.  The current menu colors are displayed
in a scrollable list box.:p.
To add a color to the menu, specify the color through the color wheel
or through the red-green-blue value controls and then click on the 
&apos.>>&apos. button.  If the color does not already exist in the menu,
it will be added.:p.
To remove a color from the menu, select it from the list box and click
on the &apos.<<&apos. button.:p.
To restore the menu to its default colors, click on the Default button.:p.
To exit without changing the menu, press the Escape key or click on the 
Escape button.

:h2 res=IDM_LINEWIDTH_MENU.Linienbreite MenÅ
Dieses MenÅ zeigt alle vorhandenen LinienstÑrken an.
:p.Die Lininienbreite an der Corsorpositon ist markiert.
:p.Siehe auch bei :link reftype=hd res=IDM_LINEWIDTHS.Linienbreite MenÅ EintrÑge:elink.
fÅr weitere Informationen.

:h3 res=IDM_LINEWIDTHS.Linienbreite MenÅ EintrÑge
Dieser MenÅeintrag zeigt die vorhandenen Linienbreiten. Um eine Breite ab der
Cursorposition zu setzen oder sie auf einen markierten Textblock anzuwenden
wÑhlen Sie eine Breite mit den Pfeiltasten aus und betÑtigen EINGABE oder Sie
klicken mit der Maus auf den MenÅeintag.
:note.Diese Breiten gelten fÅr Linien und fÅr den Rand von Schattentext.

:h3 res=IDM_LINEWIDTHS_PGUP.Linienbreite MenÅ Seite hoch
Diese MenÅoption erscheint, falls nicht alle MenÅeintrÑge gleichzeitig
dargestellt werden kînnen und vor der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h3 res=IDM_LINEWIDTHS_PGDN.Linienbreite MenÅ Seite runter
Diese MenÅoption erscheint, falls nicht alle MenÅeintrÑge gleichzeitig
dargestellt werden kînnen und vor der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h3 res=IDM_LINEWIDTHMGR.Linienbreite MenÅ Manager
Diese Funktion ist noch nicht implementiert.

:h2 res=IDM_FILLCOLOR_MENU.FÅllfarbe MenÅ
Dieses MenÅ listet alle vorhandenen FÅllfarben.
:p.Die aktuelle FÅllfarbe unter dem Cursor ist markiert.
:p.Siehe auch unter :link reftype=hd res=IDM_FILLCOLORS.FÅllfarbe MenÅ EintrÑge:elink.
fÅr weitere Informationen.

:h3 res=IDM_FILLCOLORS.FÅllfarbe MenÅ EintrÑge
Dieser MenÅeintrag zeigt einen Eintrag des FÅllfarbe MenÅs. Um eine FÅllfarbe
auf den Text ab der Cursorposition oder auf einen markierten Bereich anzuwenden,
wÑhlen Sie einfach mit der Tastatur oder der Maus eine Option aus.

:h3 res=IDM_FILLCOLORS_PGUP.FÅllfarbe MenÅ Seite hoch
Diese MenÅoption erscheint, falls nicht alle MenÅeintrÑge gleichzeitig
dargestellt werden kînnen und vor der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h3 res=IDM_FILLCOLORS_PGDN.FÅllfarbe MenÅ Seite runter
Diese MenÅoption erscheint, falls nicht alle MenÅeintrÑge gleichzeitig
dargestellt werden kînnen und vor der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h3 res=IDM_FILLCOLORMGR.FÅllfarbe MenÅ Manager
This menu option summons the 
:link reftype=hd res=IDD_FILLCOLORMGR.Fill Color Menu Manager:elink.
dialog, which gives the user the ability to customize the Fill
Color Menu.

:h4 res=IDD_FILLCOLORMGR.FÅllfarbe MenÅ Manager (Dialog)
This dialog provides the user with a way to customize the Fill
Color Menu.  A color wheel and intensity control provide an easy way
to specify colors, and for precise control there is a set of red, green
and blue value controls as well.  The current menu colors are displayed
in a scrollable list box.:p.
To add a color to the menu, specify the color through the color wheel
or through the red-green-blue value controls and then click on the 
&apos.>>&apos. button.  If the color does not already exist in the menu,
it will be added.:p.
To remove a color from the menu, select it from the list box and click
on the &apos.<<&apos. button.:p.
To restore the menu to its default colors, click on the Default button.:p.
To exit without changing the menu, press the Escape key or click on the 
Escape button.

:h2 res=IDM_FILLTYPE_MENU.FÅlltyp MenÅ
Diese MenÅ zeigt alle vorhandenen FÅlltypen.
:p.Der am Cursor aktive FÅlltyp ist markiert.
:p.Siehe auch bei :link reftype=hd res=IDM_FILLTYPES.FÅlltyp MenÅ Eintragungen:elink.
fÅr weitere Informationen.

:h3 res=IDM_FILLTYPES.FÅlltyp MenÅ EintrÑge
Dieser MenÅeintrag zeigt einen der vorhandenen FÅlltypen. Um einen
FÅlltypen von der Cursorposition abwÑrts zu setzen oder einen FÅlltypen auf einen
selektierten Textblock anzuwenden, wÑhlen Sie die Farbe mit den Pfeiltasten
aus und betÑtigen die EINGABE oder Sie klicken mit der Maus auf den MenÅeintrag.

:h3 res=IDM_FILLTYPES_PGUP.FÅlltyp MenÅ Seite hoch
Diese MenÅoption erscheint, falls nicht alle MenÅeintrÑge gleichzeitig
dargestellt werden kînnen und vor der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h3 res=IDM_FILLTYPES_PGDN.FÅlltyp MenÅ Seite runter
Diese MenÅoption erscheint, falls nicht alle MenÅeintrÑge gleichzeitig
dargestellt werden kînnen und vor der augenblicklichen Position weitere
vorhanden sind. Um sich in den EintrÑgen zu bewegen, benutzen Sie die
Pfeiltasten und drÅcken Sie EINGABE oder klicken Sie mit der Maus.

:h3 res=IDM_FILLTYPEMGR.FÅlltyp MenÅ Manager
Diese Funktion ist noch nicht implementiert.

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
Diese Funktion ist noch nicht implementiert.

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
Diese Funktion ist noch nicht implementiert.

:h1 res=IDM_WINDOW_MENU.Fenster MenÅ
:i1.Fenster MenÅ
 Vom Fenster MenÅ aus sind verschiedene Mîglichkeiten aufrufbar, die die Darstellung
bzw. Anordnung von Fenstern beeinflussen. Au·erdem wird eine Liste aller zur
Zeit geîffneten Fenster angezeigt.
Die folgenden Kommandos stehen zur Auswahl&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_TILE.Nebeneinander:elink.
:li.&sqbul.:link reftype=hd res=IDM_CASCADE.öberlappend:elink.
:li.&sqbul.:link reftype=hd res=IDM_ARRANGEICONS.Ikonen anordnen:elink.
:li.&sqbul.:link reftype=hd res=IDM_WINDOWS.Active Windows:elink.
:esl.

:h2 res=IDM_TILE.Nebeneinander (MenÅ Option)
 Dieses Kommando bewirkt, da· alle nicht verkleinerten Dokumentenfenster so
angeordnet werden, da· sie komplett sichtbar sind. Die Ikonen der verkleinerten
Dokumentenfenster werden ebenfalls sichtbar am unteren Rand dargestellt. Dabei
werden sie, wenn notwendig, neu angeordnet.

:h2 res=IDM_CASCADE.öberlappend (MenÅ Option)
 Dieses Kommando bewirkt, da· alle nicht verkleinerten Dokumentenfenster so
angeordnet werden, da· sie so gro· wie mîglich sind. Dabei wird darauf geachtet,
da· alle Titelleisten sichtbar sind. Die Ikonen der verkleinerten
Dokumentenfenster werden ebenfalls sichtbar am unteren Rand dargestellt. Dabei
werden sie, wenn notwendig, neu angeordnet.

:h2 res=IDM_ARRANGEICONS.Ikonen anordnen (MenÅ Option)
 Dieses Kommando ordnet alle Ikonen der verkleinerten Dokumentenfenster unten
links auf der ArbeitflÑche an. Dieses Kommando wird auch bei Aktivierung der
Nebeneinander und öberlappend Optionen automatisch ausgefÅhrt.

:h2 res=IDM_WINDOWS.Aktive Fenster
Dieser Abschnitt des Fenter MenÅs zeigt eine Liste der augenblicklich geîffneten
Dokumentenfenster. Um ein Dokument zur Anzeige auszuwÑhlen, klicken Sie auf eine
der MenÅoptionen oder Sie wÑhlen mit den Pfeiltasten und betÑtigen dann die
EINGABE Taste.

:h1 res=IDM_HELP_MENU.Hilfe MenÅ
:i1.Hilfe MenÅ
 Das Hilfe MenÅ ist im SAA Format gestaltet und stellt die folgenden Punkte
bereit&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_HELP_INDEX.Hilfe Index:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXTENDED_HELP.Allgemeine Hilfe:elink.
:li.&sqbul.:link reftype=hd res=IDM_HELP_FOR_HELP.Hilfe fÅr Hilfe:elink.
:li.&sqbul.:link reftype=hd res=IDM_KEYS_HELP.Hilfe zu Tastaturkurzbefehlen:elink.
:li.&sqbul.:link reftype=hd res=IDM_ABOUT_MENU.Product Information Menu:elink.
:esl.

:h2 res=IDM_HELP_INDEX.Hilfe Index (MenÅ Option)
 Die Anwahl dieser MenÅoption zeigt den Hilfeindex an.

:h2 res=IDM_EXTENDED_HELP.Allgemeine Hilfe (MenÅ Option)
 Diese MenÅoption zeigt allgemeine Informationen zur Applikation an.

:h2 res=IDM_HELP_FOR_HELP.Hilfe fÅr Hilfe (MenÅ Option)
 Die Auswahl dieser MenÅoption zeigt Basisinformationen, wie die
Hilfefunktionen benutzt werden kînnen. Keine der gezeigten Informationen ist
spezifisch zu einer bestimmeten Applikation.

:h2 res=IDM_KEYS_HELP.Hilfe zu Tastaturkurzbefehlen (MenÅ Option)
 Die folgenden Tasten-Zuordnungen wurden im PM Textverarbeitungsprogramm
vorgenommen.
:table cols='20 60' rules=both frame=box.
:row.:c.F1              :c.Zeige Hilfeinformationen
:row.:c.F2              :c.Speichere das aktive Dokument
:row.:c.F3              :c.Beenden
:row.:c.F4              :c.Speichern und Schlie·en des aktuellen Dokuments
:row.:c.F5              :c.Spell Check
:row.:c.F10             :c.MenÅ
:row.:c.Strg + Tab      :c.Next Document
:row.:c.Strg + O        :c.ôffne Dokument
:row.:c.Strg + P        :c.Drucke ganzes Dokument
:row.:c.Alt + P         :c.Drucke aktuelle Seite
:row.:c.Strg + N        :c.Setze Normal (zurÅcksetzen der Text Attribute)
:row.:c.Strg + B        :c.Setze Fett
:row.:c.Strg + I        :c.Setze Kursiv
:row.:c.Strg + U        :c.Setze Unterstrichen
:row.:c.Strg + S        :c.Suchen
:row.:c.Strg + F        :c.Finde nÑchstes
:row.:c.Strg + Z        :c.Zoom All Toggle
:row.:c.Ctrl + Backspace:c.Delete to start of current word.
:etable.
In addition, there are a number of keys defined for navigation within
the current document.  For these definitions, see the topic 
:link reftype=hd res=60003.Editing Documents (Navigation):elink..

:h2 res=IDM_ABOUT_MENU.Product Information Menu (UntermenÅ)
If you select this menu option directly, the program&apos.s
Product Information and Copyright dialog to be displayed.
:p.Siehe auch unter :link reftype=hd res=IDD_ABOUT.Produkt Information (Dialog):elink.
:p.If you click on the conditional menu button instead,
a menu listing the installed products will be displayed, from
which you may select product information on any of those
products.

:h3 res=IDM_ABOUT.Product Information Menu Entry (MenÅ Option)
This is an entry in the Product Information Menu, representing 
this program or one of the extension products installed with it.
Click on the entry to see the product information itself
(see :link reftype=hd res=IDD_ABOUT.Product Information (Dialog):elink. for more information).

:h4 res=IDD_ABOUT.Produkt Information (Dialog)
 Diese Dialogbox zeigt den Programmnamen, die Ikone und die Urheberrechts
Informationen. Der Dialog kann mit EINGABE, Leertaste oder Escape Taste oder
durch klicken auf OK beendet werden.
:artwork name='Deutsch\About.bmp' align=center.

:h1 res=60009.Kontext MenÅs
:i1.Kontext MenÅs
Es gibt eine Menge Kontext MenÅs, die mit einem Klick der rechten Maustaste
aktiviert werden kînnen. Zusammen enthalten diese Kontext MenÅs mehr Funktionen
als die normalen MenÅs des Hauptfensters.
:p.
Wenn Sie mit der rechten Maustaste in das Hauptfenster klicken (nicht in den
Dokumentenbereichh), dann erscheint
das :link reftype=hd res=IDM_MAIN_CONTEXT.Haupt Kontext MenÅ:elink..
Wenn Sie mit der rechten Maustaste in den Dokumentenbereich aber nicht in den
Text klicken, dann erscheint das :link reftype=hd res=IDM_DEFAULTMENU.Standard MenÅ:elink..  Right clicking over a block of selected text will result
Au·erdem gibt es das :link reftype=hd res=IDM_TEXTMENU.Markierter Text MenÅ:elink. ,
das :link reftype=hd res=IDM_BITMAPMENU.Grafik MenÅ:elink. und
das :link reftype=hd res=IDM_BORDERMENU.Grafik Rand MenÅ:elink..

:h2 res=IDM_MAIN_CONTEXT.Haupt Kontext MenÅ
Dieses MenÅ erscheint, wenn Sie mit der rechten Maustaste in den Arbeitsbereich
klicken. Es enthÑlt alle MenÅoptionen, die au·erhalb eines Dokumentes wirksam werden
kînnen. Es sind unter anderen&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_REFRESH.Erneut darstellen:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILE_MENU.Datei:elink.
:li.&sqbul.:link reftype=hd res=IDM_WINDOW_MENU.Fenster:elink.
:li.&sqbul.:link reftype=hd res=IDM_HELP_MENU.Hilfe:elink.
:esl.
:note.Das Datei UntermenÅ bietet nicht alle Optionen des normalen DateimenÅs an.
Es zeigt nur EintrÑge die unabhÑngig von einem bestimmten Dokument Sinn machen.
Auf diese Ausnahmen wird innerhalb der ErklÑrungen zu den einzelnen MenÅoptionen
hingewiesen. Au·erdem ist diese MenÅoption nicht aufrufbar, wenn kein
Dokumentenfenster geîffnet ist.

:h3 res=IDM_REFRESH.Neu Zeichnen (MenÅ Option)
 Wenn Sie diese MenÅ Option aktivieren, wird das gesamte Dokument neu
dargestellt.

:h2 res=IDM_DEFAULTMENU.Standard MenÅ
Diese MenÅ erscheint, wenn Sie mit der rechten Maustaste in einen Teil eines
Dokumentenfensters klicken, der (1) kein ausgewÑhlter Text, keine (2) Grafik oder
(3) Grafik Rand ist. Es enthÑlt MenÅoptionen, die auf das Dokument als Ganzes
angewendet werden kînnen. Dies sind unter anderen&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_REFRESH.Neu darstellen:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILE_MENU.Datei MenÅ:elink.
:li.&sqbul.:link reftype=hd res=IDM_EDIT_MENU.Bearbeiten MenÅ:elink.
:esl.
:note.Die Datei und Bearbeiten MenÅs enthalten nicht alle Optionen der normalen
MenÅs, sondern nur die Optionen die auf eine geîffnete Datei im allgemeinen
angewendet werden kînnen. Diese Ausnahmen sind in den ausfÅhrlichen Beschreibungen
zu den einzelnen MenÅoptionen erwÑhnt.

:h2 res=IDM_TEXTMENU.Markierter Text MenÅ
 Dieses MenÅ erscheint, wenn Sie die rechte Maustaste in einem markierten
Textbereich anklicken. Es enthÑlt alle MenÅoptionen, die auf den markierten
Text angewendet werden kînnen. Es sind die folgenden&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_CUT.Ausschneiden:elink.
:li.&sqbul.:link reftype=hd res=IDM_COPY.Kopieren:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE.EinfÅgen:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEAR.Lîschen:elink.
:li.&sqbul.:link reftype=hd res=IDM_LAYOUT_MENU.Ansicht:elink.
:li.&sqbul.:link reftype=hd res=IDM_FONT_MENU.Schriftart:elink.
:li.&sqbul.:link reftype=hd res=IDM_SIZE_MENU.Grî·e:elink.
:li.&sqbul.:link reftype=hd res=IDM_ATTR_MENU.Stil:elink.
:esl.
:note.Das Ansicht UntermenÅ dieses MenÅs enthÑlt nicht alle Optionen des
normalen Ansicht MenÅs, sondern nur diejenigen Optionen, die auf den markierten
Text angewendet werden kînnen. Diese Ausnahmen werden in der ausfÅhrlichen
Beschreibung der einzelnen MenÅoptionen erwÑhnt.

:h2 res=IDM_BITMAPMENU.Bild MenÅ
 Dieses MenÅ erscheint, wenn Sie mit der rechten Maustaste in ein Bild oder
ein grafisches Objekt klicken. Es enthÑlt alle Optionen die auf ein solches
Objekt angewendet werden kînnen. Darunter sind die folgenden&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_BACKGROUND.Hintergrund:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_NOTEXTASIDE.Kein Text:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_HALFTONE.Halbton:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_REVERSE.Reverse:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_OPTIMIZE.Optimize:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION.Position:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE.Size:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT.Aspect:elink.
:esl.

:h3 res=IDM_BITMAP_BACKGROUND.Hintergrund (MenÅ Option)
 Wenn Sie diese MenÅoption auswÑhlen wird die Grafik als Hintergrund interpretiert,
so da· der normale text darauf plaziert werden kann.

:h3 res=IDM_BITMAP_NOTEXTASIDE.Kein Text (MenÅ Option)
 Wenn Sie diese MenÅoption aktiviert haben, wird die Grafik so markiert, da· der
Formatierer keinen Text seitlich plaziert.

:h3 res=IDM_BITMAP_HALFTONE.Halbton (MenÅ Option)
 Wenn Sie diese MenÅoption aktiviert haben, wird die Grafik in Halbton oder
Misch-Farben dargestellt. Dies ist sinnvoll, wenn die Grafik als Hintergrund
gedacht ist.

:h3 res=IDM_BITMAP_REVERSE.Reverse (MenÅ Option)
When you select this menu option, the graphic image is marked for painting
in inverted colors.  
:note.This option only works with bitmap graphics (not metafiles).

:h3 res=IDM_BITMAP_OPTIMIZE.Optimize (MenÅ Option)
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

:h4 res=IDM_BITMAP_POSITION_EDGE_LEFT.Graphic Position to Left Edge (MenÅ Option)
When one selects this menu option, the selected graphic is repositioned to the
left edge of the document page, subject to the current form&apos.s size and clipping
distances.  If a different form is selected, the graphic position will be adjusted
accordingly.

:h4 res=IDM_BITMAP_POSITION_MARGIN_LEFT.Graphic Position to Left Margin (MenÅ Option)
When one selects this menu option, the selected graphic is repositioned to the left
margin of the document page.  If the margins are adjusted, the graphic position
will be adjusted accordingly.

:h4 res=IDM_BITMAP_POSITION_CENTERED_X.Graphic Position Centered (X) (MenÅ Option)
When one selects this menu option, the selected graphic is centered horizontally.
Normally, this centering will take place with respect to the current margins.
If the graphic size option has been set to
:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Graphic Size Edge-to-Edge (X):elink.,
then the centering will take place with respect to the currently selected form
clipping limits (edges).

:h4 res=IDM_BITMAP_POSITION_MARGIN_RIGHT.Graphic Position to Right Margin (MenÅ Option)
When one selects this menu option, the selected graphic is repositioned to the right
margin of the document page.  If the margins are adjusted, the graphic position
will be adjusted accordingly.

:h4 res=IDM_BITMAP_POSITION_EDGE_RIGHT.Graphic Position to Right Edge (MenÅ Option)
When one selects this menu option, the selected graphic is repositioned to the
right edge of the document page, subject to the current form&apos.s size and clipping
distances.  If a different form is selected, the graphic position will be adjusted
accordingly.

:h4 res=IDM_BITMAP_POSITION_EDGE_TOP.Graphic Position to Top Edge (MenÅ Option)
When one selects this menu option, the selected graphic is repositioned to the
top edge of the document page, subject to the current form&apos.s size and clipping
distances.  If a different form is selected, the graphic position will be adjusted
accordingly.

:h4 res=IDM_BITMAP_POSITION_MARGIN_TOP.Graphic Position to Top Margin (MenÅ Option)
When one selects this menu option, the selected graphic is repositioned to the top
margin of the document page.  If the margins are adjusted, the graphic position
will be adjusted accordingly.

:h4 res=IDM_BITMAP_POSITION_CENTERED_Y.Graphic Position Centered (Y) (MenÅ Option)
When one selects this menu option, the selected graphic is centered vertically.
Normally, this centering will take place with respect to the current margins.
If the graphic size option has been set to
:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Graphic Size Edge-to-Edge (Y):elink.,
then the centering will take place with respect to the currently selected form
clipping limits (edges).

:h4 res=IDM_BITMAP_POSITION_MARGIN_BOTTOM.Graphic Position to Bottom Margin (MenÅ Option)
When one selects this menu option, the selected graphic is repositioned to the bottom
margin of the document page.  If the margins are adjusted, the graphic position
will be adjusted accordingly.

:h4 res=IDM_BITMAP_POSITION_EDGE_BOTTOM.Graphic Position to Bottom Edge (MenÅ Option)
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

:h4 res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Graphic Size Edge-to-Edge (X) (MenÅ Option)
When one selects this menu option, the selected graphic is resized so that it occupies
the full width of the page, from edge to edge, unless the graphic is also marked for
preservation of its aspect ratio (see :link reftype=hd res=IDM_BITMAP_ASPECT.Graphic Aspect Menu:elink.).
In that case, it is possible the graphic will be somewhat narrower.

:h4 res=IDM_BITMAP_SIZE_MARGINTOMARGIN_X.Graphic Size Margin-to-Margin (X) (MenÅ Option)
When one selects this menu option, the selected graphic is resized so that it occupies
the width of the page, from margin to margin, unless the graphic is also marked for
preservation of its aspect ratio (see :link reftype=hd res=IDM_BITMAP_ASPECT.Graphic Aspect Menu:elink.).
In that case, it is possible the graphic will be somewhat narrower.

:h4 res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Graphic Size Edge-to-Edge (Y) (MenÅ Option)
When one selects this menu option, the selected graphic is resized so that it occupies
the full height of the page, from edge to edge, unless the graphic is also marked for
preservation of its aspect ratio (see :link reftype=hd res=IDM_BITMAP_ASPECT.Graphic Aspect Menu:elink.).
In that case, it is possible the graphic will be somewhat shorter.

:h4 res=IDM_BITMAP_SIZE_MARGINTOMARGIN_Y.Graphic Size Margin-to-Margin (Y) (MenÅ Option)
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

:h4 res=IDM_BITMAP_ASPECT_FLOAT.Graphic Aspect Float (MenÅ Option)
When one selects this menu option, the selected graphic is freed from the requirement
of a fixed (1x1) aspect ratio.  Normally, this will have no immediate visible effect
on the graphic, unless it also has one of the edge-to-edge or margin-to-margin sizing
options in effect at the time, in which case the graphic may resize itself.

:h4 res=IDM_BITMAP_ASPECT_FIX_X.Graphic Aspect Fix X (MenÅ Option)
When one selects this menu option, the selected graphic has a restriction placed on its
sizing so that it will always be displayed with a one-to-one aspect ratio.
With this option, the controlling dimension is the width.  The height will
be adjusted to attain the one-to-one aspect.

:h4 res=IDM_BITMAP_ASPECT_FIX_Y.Graphic Aspect Fix Y (MenÅ Option)
When one selects this menu option, the selected graphic has a restriction placed on its
sizing so that it will always be displayed with a one-to-one aspect ratio.
With this option, the controlling dimension is the height.  The width will
be adjusted to attain the one-to-one aspect.

:h2 res=IDM_BORDERMENU.Bildrand MenÅ
 Dieses MenÅ erscheint, wenn Sie mit der rechten Maustaste in den Rand um das
grafische Objekt klicken. Es enthÑlt alle MenÅ Optionen, die auf solch einen
Bereich angewendet werden kînnen. Darunter sind die folgenden&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_SIZE.Rand Grî·e:elink.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_COLOR.Rand Farbe:elink.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_CAPTION.Rand Titel:elink.
:esl.

:h3 res=IDM_BORDER_SIZE.Rand Grî·e (MenÅ Option)
Diese Funktion ist noch nicht implementiert.

:h3 res=IDM_BORDER_COLOR.Rand Farbe (MenÅ Option)
Diese Funktion ist noch nicht implementiert.

:h3 res=IDM_BORDER_CAPTION.Rand Titel (MenÅ Option)
Diese Funktion ist noch nicht implementiert.

:h1 res=60010.Sonstige Dialoge

:h2 res=IDD_PROFILE_PATH.Definiere Profile Pfad
 Dieser Dialog wird ausgegeben, wenn das Programm seine Pfrofile (INI) Datei
nicht finden kann. Sie kînnen dann den Pfad eingeben oder entscheiden wo
die Datei erzeugt werden soll.
:artwork name='Deutsch\Profile.bmp' align=center.
:p.Es wird nur ein existierender Verzeichnisname akzeptiert. Schlie·en Sie den
Namen mit EINGABE ab oder klicken Sie auf OK um in der ProgrammausfÅhrung
fortzufahren.
:p.Wenn Sie die Initialisierung abbrechen wollen, drÅcken Sie ESC oder klicken
auf den Escape Knopf.

:h2 res=IDD_PROMPT_SAVE.GeÑndertes Dokument speichern?
.* :artwork name='Deutsch\SaveChng.bmp' align=center.
 Sie haben versucht das Programm zu beenden oder das Dokumenten
Fenster zu schlie·en, ohne die vorgenommenen énderungen zu speichern.
Antworten Sie mit &apos.J&apos.a oder klicken Sie auf den OK Knopf wenn Sie speichern
wollen.
Antworten Sie mit &apos.N&apos.ein oder drÅcken Sie auf den NO Knopf wenn Sie nicht
speichern wollen (Sie verwerfen die énderungen).
DrÅcken Sie ESC oder klicken Sie auf den ESC Knopf, wenn Sie das Programm
nicht beenden oder das Dokument nicht schlie·en wollen.

:h2 res=IDD_PROMPT_SAVECONFIRM1.Speichern bestÑtigen
 Wenn dieser Dialog erscheint, versuchen Sie Ihr Dokument unter einem Namen
abzuspeichern der schon existiert.
.* :artwork name='Deutsch\SaveConf.bmp' align=center.
:p.Wenn Sie die vorhandene Datei tatsÑchlich Åberschreiben wollen, kînnen
Sie entweder die &apos.J&apos. Taste drÅcken oder auf den Ja Knopf klicken. Wenn Sie
die Datei nicht Åberschreiben wollen, drÅcken Sie EINGABE oder den &apos.N&apos. Knopf.
In diesem Fall wird Ihr Dokument nicht gespeichert und Sie kînnen einen
anderen Namen eingeben.

:h2 res=IDD_PROMPT_SAVECONFIRM2.Type Change Confirm
If this dialog has been displayed, then you are attempting to save your document
with a changed file type.
.* :artwork name='Deutsch\SaveCnf2.bmp' align=center.
:p.If you really wish to change the file type, then either press the
&apos.Y&apos. key or click on the Yes button.
If you do not wish to change the file type, simply press Enter or the &apos.N&apos. key.
In this case, your document will not have been saved, and you will have to
try again, specifying a different name.

:h2 res=IDD_MESSAGE_SPELLDONE.RechtschreibprÅfung fertig
Wenn dieser Dialog angezeigt wird, ist ein Lauf der RechtschreibprÅfung
abgeschlossen.
.* :artwork name='Deutsch\SpelDone.bmp' align=center.
:p.Um fortzufahren betÑtigen Sie die EINGABE Taste oder Sie klicken mit der Maus
auf den Eingabe Knopf.

:h2 res=IDD_ERROR_HELPMANAGER.Hilfe Manager Fehler
 Diese Nachricht wird angezeigt, wenn irgend etwas mit den Hilfe Dateien nicht
in Ordnung ist. Bitte benachrichtigen Sie den Programmierer.

:h2 res=IDD_ERROR_EXCEPTION.Program Exception Abort
.* :artwork name='Deutsch\Except.bmp' align=center.
Wenn dieser Dialog angezeigt wird, dann ist bei Escriba ein schwerer Fehler
aufgetreten.
Bitte setzen Sie den Autor davon in Kenntnis und verwenden Sie das Formular
:link reftype=hd res=60008.Fragen und Fehlerreports:elink. und fÅgen Sie die Datei
ESCRIBA.LOG bei, die sich im Escriba Verzeichnis befindet.

:h2 res=IDD_ERROR_WINREGISTERCLASS.Klassen Registration Fehler
 Wenn diese Meldung angezeigt wird, konnte das Programm eine seiner Fenster
Klassen nicht registrieren lassen.
Bitte setzen Sie den Autor davon in Kenntnis und verwenden Sie das Formular
:link reftype=hd res=60008.Fragen und Fehlerreports:elink. und fÅgen Sie die Datei
ESCRIBA.LOG bei, die sich im Escriba Verzeichnis befindet.

:h2 res=IDD_ERROR_CREATECLIENT.Client Create Error
Wenn dieser Dialog angezeigt wird, dann war Escriba aus irgend einem Grund
nicht in der Lage ein Fenster zu îffnen.
Bitte setzen Sie den Autor davon in Kenntnis und verwenden Sie das Formular
:link reftype=hd res=60008.Fragen und Fehlerreports:elink. und fÅgen Sie die Datei
ESCRIBA.LOG bei, die sich im Escriba Verzeichnis befindet.

:h2 res=IDD_ERROR_TOOMANYDOCUMENTS.Zu viele Dokumente Fehler
Wenn dieser Dialog angezeigt wird, haben Sie versucht zu viele Dokumente
gleichzeitig zu îffnen.

:h2 res=IDD_ERROR_TOOMANYLOADED.Zu viele Dokumente geladen Fehler
Wenn dieser Dialog angezeigt wird, haben Sie zu vile Dukumente geladen,
um ein weiteres zu erzeugen.

:h2 res=IDD_ERROR_DOCUMENT_CREATE.Dokument Erzeugungs Fehler
Wenn dieser Dialog angezeigt wird, hat Escriba aus irgendeinem Grund
kein Dokument erzeugen kînnen.
Bitte setzen Sie den Autor davon in Kenntnis und verwenden Sie das Formular
:link reftype=hd res=60008.Fragen und Fehlerreports:elink. und fÅgen Sie die Datei
ESCRIBA.LOG bei, die sich im Escriba Verzeichnis befindet.

:h2 res=IDD_ERROR_DOCUMENT_LOAD.Dokument Lade Fehler
Wenn dieser Dialog angezeigt wird, dann hat Escriba aus irgendeinem Grund
das Dokument nicht laden kînnen. Bitte ÅberprÅfen Sie den Dateinamen und
den Pfad. Es ist auch mîglich, da· gerade ein anderes Programm die Datei
im Zugriff hat.

:h2 res=IDD_ERROR_DOCUMENT_SAVE.Dokument Speichern Fehler
Wenn dieser Dialog angezeigt wird, dann konnte Escriba die angegebene Datei
nicht îffnen um das Dokument zu speichern. Bitte ÅberprÅfen Sie den Dateinamen
und den Pfad auf GÅltigkeit. Es ist auch mîglich, da· gerade ein anderes
Programm die Datei im Zugriff hat.

:h2 res=IDD_ERROR_CREATEPRINTSTATUS.Druck Statusfenster Erzeugungs Fehler
Wenn dieser Dialog angezeigt wird, konnte Escriba sein Druck Statusfenster nicht
erzeugen. Diese sollte niemals passieren und ist nur aus GrÅnden der Diagnose
vorgesehen.
Bitte setzen Sie den Autor davon in Kenntnis und verwenden Sie das Formular
:link reftype=hd res=60008.Fragen und Fehlerreports:elink.

:h2 res=IDD_ERROR_CREATELOADSTATUS.Druck Ladefenster Erzeugungs Fehler
Wenn dieser Dialog angezeigt wird, konnte Escriba sein Druck Ladefenster nicht
erzeugen. Diese sollte niemals passieren und ist nur aus GrÅnden der Diagnose
vorgesehen.
Bitte setzen Sie den Autor davon in Kenntnis und verwenden Sie das Formular
:link reftype=hd res=60008.Fragen und Fehlerreports:elink.

:h2 res=IDD_ERROR_BLOCKMEMORY.Block kopieren Speicher Fehler
Wenn dieser Dialog angezeigt wird, war nicht mehr genÅgend Speicher vorhanden,
um ausgeschnittenen oder zu kopierenden Text in der Zwischenablage zu deponieren.
Versuchen Sie die Aktion mit kleineren Segmenten durchzufÅhren.

:h2 res=IDD_ERROR_BADGRAPHICFILE.UngÅltige Grafik datei Fehler
Wenn dieser Dialog angezeigt wird haben Sie versucht, eine Datei als Grafik zu
laden, die entweder von unbekanntem Typ ist oder ungÅltige  Daten enthÑlt. Die zur
Zeit unterstÅtzten Formate sind BMP, GIF, JPG, PCX, PCC, MET und TIF.  Wenn die Datei von einem
dieser Typen ist und Sie sie mit einem anderen Grafikprogramm anzeigen kînnen,
dann benachrichtigen Sie bitte den Autor und verwenden Sie das Formular
:link reftype=hd res=60008.Fragen und Fehlerreports:elink.

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

:h1 res=61000.Technische Daten

:h2 res=61001.Dokumenten Datei Format
Die Dokumentendatei kann als Zeichen- und Funktionscodes Strom verstanden werden.
Mit Ausnahme der Funktionscodes 15, 16 und 17 werden alle Daten ohne énderung
gespeichert. Die Codes 15, 16 und 17 werden mit vorangestelltem Code 15 versehen.
Wenn also der Orinal Datenstrom eine 16 enthÑlt, wird diese als 15, 16 abgespeichert.
Wenn Code 15 ohne folgenden Code 15, 16 oder 17 angetroffen wird, so wird er als
Code 15 interpretiert.
:p.
Ein Literal 17 &apos.&larrow.&apos. definiert den Start eines Funktionscodes.
Ein Literal 16 &apos.&rarrow.&apos. definiert das Ende eines Funktionscodes.
Wenn ein Funktionscode Ende ohne vorherigen Anfang entdeckt wird,
so wird er als normales Literal betrachtet.
:p.
Die folgenden Funktionen sind zur Zeit gÅltig&colon.

:lm margin=3.:p.&larrow.Version&colon.:hp5.versionnumber:ehp5.&rarrow.
:lm margin=5.:p.
This function code specifies the version of Escriba used to edit and save
the document to disk.  The version number has four digits, which are treated
as if they had a decimal point between the first two and the second two digits.
Version 1.00 would be represented as 0100.  This function code is used to
allow document format conversion from older to newer formats, and did not
exist in the file formats prior to beta revision 0.92.
Dieser Code kann nicht au·erhalb des Parameter Bereichs des &apos.Preface&apos. Functions Codes stehen.

:lm margin=3.:p.&larrow.Preface&colon.:hp5.Funktionscodes:ehp5.&rarrow.
:lm margin=5.:p.
Dies ist ein besonderer Funktionscode, der in einem abgespeicherten Dokument
auftritt, nicht aber in einem zur bearbeitung bereitstehenden Dokument.
Der Parameter dieses Funktionscodes enthÑlt andere Funktionscodes.
Der Grund fÅr all diese Funktionscodes ist es, ein Standard Dokument zu erstellen.
Alle Funktionscodes, die das Erscheinungsbild des Dokuments verÑndern sind hier
gÅltig, wie Drucker, Form, Tabulatoren und AbstÑnde. Funktionscodes, die Text
generieren wie Seitennummer, Datum, Grafik usw. sind hier nicht erlaubt.

:lm margin=3.:p.&larrow.Printer&colon.:hp5.Name:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Funktionscode gibt den Namen des Druckers an, fÅr den das Dokument
formatiert ist. Dieser Code kann nicht au·erhalb des Parameter Bereichs des
&apos.Preface&apos. Functions Codes stehen.

:lm margin=3.:p.&larrow.Form&colon.:hp5.Name:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Funktionscode wird benutzt, um den Name des Formats anzugeben, mit dem das Dokument formatiert wurde. 
It is only used when a document formatted for one printer is being reformatted for another.
Dieser Code kann nicht au·erhalb des Parameter Bereichs des &apos.Preface&apos. Functions Codes stehen.

:lm margin=3.:p.&larrow.CodePage&colon.:hp5.number:ehp5.&rarrow.
:lm margin=5.:p.
This function code is used to specify the number of the codepage to be used
when displaying the document.
Dieser Code kann nicht au·erhalb des Parameter Bereichs des &apos.Preface&apos. Functions Codes stehen.
See :link reftype=hd res=61002.Code Pages:elink. for more information.

:lm margin=3.:p.&larrow.Font&colon.:hp5.Name:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Funktionscode gibt den Namen der Schriftart an, die ab der Cursorposition
Verwendung findet. Der Name mu· eine gÅltige skalierbare Schriftart bezeichnen die
in der System Zeichensatzt Palett, im System Zeichensatz Dialog oder in Escribas
Zeichensatz MenÅ auftritt.

:lm margin=3.:p.&larrow.Size&colon.:hp5.Punktgrî·e:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Code legt die Punktgrî·e fÅr den Drucker fest. Ein Druckerpunkt ist
in Englischen Einheiten 1/72 Zoll. Wenn die Grî·e auf 12 Punkte festgelegt wird,
dann ist die normale Grî·e eines Zeichens 1/6 Zoll hoch. Dies ist eine Richtline
fÅr Zeichensatz Entwickler. Allgemein gilt, da· nicht alle ZeichensÑtze gleich
gro· dargestellt werden, auch wenn sie die gleiche Punktgrî·e haben. Escriba
akzeptiert Punktgrî·en bis zu einer Auflîsung von 1/10 Punkt.

:lm margin=3.:p.&larrow.Color&colon.:hp5.Farbe:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Code legt die Farbe (rot-blau-grÅn) fest, die ab der Cursorposition verwendet
werden soll. Der Code ist ein sechsstelliger hexadezimaler Code, bei dem die
ersten 2 Stellen die rote Komponente, die nÑchsten die grÅne Komponente und
die letzten zwei Stellen die blauer Komponente der Farbe festlegen.
Wei· ist dann &apos.FFFFFF&apos., schwarz &apos.000000&apos., und gelb ist &apos.FFFF00&apos..

:lm margin=3.:p.&larrow.LineWidth&colon.:hp5.Breite:ehp5.&rarrow.
:lm margin=5.:p.
Diese Funktion definiert die Linienbreite die fÅr die RÑnder von Zeichen verwendet
wird. Die Breite ist wie jede andere Strecke definiert.

:lm margin=3.:p.&larrow.LineEnd&colon.:hp5.Typ:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the line end type to be used beyond
the current position.  
The valid types include all the LINEEND codes defined in the
OS/2 Programmer&apos.s Reference.

:lm margin=3.:p.&larrow.LineJoin&colon.:hp5.Typ:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the line join type to be used beyond
the current position.  
The valid types include all the LINEJOIN codes defined in the
OS/2 Programmer&apos.s Reference.

:lm margin=3.:p.&larrow.FillColor&colon.:hp5.Farbe:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Code legt die Farbe (rot-blau-grÅn) fest, die fÅr ZeichenrÑnder
verwendet werden soll. Der Code ist wie bei Linient/Text Farbe aufgebaut.

:lm margin=3.:p.&larrow.FillType&colon.:hp5.typ:ehp5.&rarrow.
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

:lm margin=3.:p.&larrow.Expand&colon.:hp5.Prozent:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the horizontal text expansion factor to be
used from the current position in the document onwards.
The factor is expressed as a percentage, with 100 indicating that
the normal horizontal expansion defined for the current font is
to be used.

:lm margin=3.:p.&larrow.TopMargin&colon.:hp5.Abstand:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the current position of the top margin to the
specified value, which is relative to the bottom edge of the page.
This attribute takes effect on the next page, unless nothing displayable has
appeared on the current page yet.

:lm margin=3.:p.&larrow.BottomMargin&colon.:hp5.Abstand:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the current position of the bottom margin to the
specified value.
This attribute takes effect on the next page, unless nothing displayable has
appeared on the current page yet.

:lm margin=3.:p.&larrow.LeftMargin&colon.:hp5.Abstand:ehp5.&rarrow.
:lm margin=5.:p.
This function code sets the current position of the left margin to the
specified value.
This attribute takes effect on the next line, unless nothing displayable has
appeared on the current line yet.

:lm margin=3.:p.&larrow.RightMargin&colon.:hp5.Abstand:ehp5.&rarrow.
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

:lm margin=3.:p.&larrow.TabSet&colon.:hp5.Abstand:ehp5.&rarrow.
:lm margin=5.:p.
This function code causes a tab to be added at the specified horizontal
distance to the current tab table.

:lm margin=3.:p.&larrow.TabClear&colon.:hp5.Abstand:ehp5.&rarrow.
:lm margin=5.:p.
This function code causes any tab present at the specified horizontal distance
to be removed from the current tab table.

:lm margin=3.:p.&larrow.TabSpacing&colon.:hp5.Abstand:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Funktionscode definiert den Abstand zwischen Tabulatoren am Anfang des
Dokuments. Beginnend am linken Rand werden die Tabulatoren mit der hier
angegebenen Weite erzeugt. Dieser Code kann nicht au·erhalb des Parameter
Bereichs des &apos.Preface&apos. Functions Codes stehen.

:lm margin=3.:p.&larrow.Indent&colon.:hp5.Abstand:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Funktionscode legt den Abstand fÅr die EinrÅckung bei AbsÑtzen fest.
Erste Absatzzeilen sind entweder (1) die erste Zeile eines Dokuments oder
(2) die erste Zeile Text nach einem &apos.newline code&apos., welcher das Ende
eines Absatzes markiert.

:lm margin=3.:p.&larrow.Metric&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Funktionscode entscheidet, ob das Dokument metrisch oder in Zoll formatiert
wird. Eine &apos.1&apos. bedeutet, da· metrisch formatiert wird. 
Dieser Code kann nicht au·erhalb des Parameter Bereichs des &apos.Preface&apos. Functions Codes stehen.

:lm margin=3.:p.&larrow.Graphic&colon.:hp5.x,y,cx,cy[,Background][,NoTextAside][,Halftone][,Reverse][,PrinterOptimize][[,KeepAspectFixX|KeepAspectFixY]][,BorderWidth=n][,BorderColor=rrggbb][,BorderCaption="text"]&colon.type&bsl.0data:ehp5.&rarrow.
:lm margin=5.:p.
This function code defines a graphic cell within the document.
:p.The &apos.x&apos. and &apos.y&apos. coordinates define the cell&apos.s position on the page, relative
to the upper-left corner of the page.  Documents saved before revision 0.97 specify their graphics
relative to the lower-left corner of the page.
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
:p.The &apos.BorderWidth=n&apos. parameter declares that the graphic border will be a specific
size in thousandths of an inch or centimeter, depending on the document metric type.
:p.The &apos.BorderColor=rrggbb&apos. parameter declares the red-blue-green color of the graphic border.
:p.The &apos.BorderCaption="text"&apos. parameter declares the graphic caption text.  This text 
is literal, except that all embedded double-quote characters are doubled.
:p.The &apos.Type&apos. field can currently be set to 
&apos.BM&apos. (for bitmap),
&apos.MF&apos. (for metafile), 
&apos.UP&apos. (for UPC Code), 
&apos.39&apos. (for Code 39),
&apos.PN&apos. (for POSTNET),
&apos.E8&apos. (for EAN-8),
&apos.27&apos. (for Codabar (2 of 7)),
&apos.25&apos. (for Code 2 of 5) or
&apos.28&apos. (for Code-128).
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
:p.For UPC Codes, the data is a 12-digit text containing the UPC Code itself.
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
:p.For Codabar codes, the data can be any number and combination of the characters 
0 to 9, &dash., &dollar., &colon., &slash., &dot. or &plus., and the first and last characters must be identical
and in the range A to D.

:lm margin=3.:p.&larrow.Date&colon.Fixed&colon.:hp5.time:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Funktionscode generiert einen Text mit einem festen Datum in Sekunden seit
00&colon.00, 1 Januar 1970.
Das Format hierfÅr ist durch die &apos.DateFormat&apos. Funktion spezifiziert.

:lm margin=3.:p.&larrow.Date&colon.Current&rarrow.
:lm margin=5.:p.
Dieser Funktionscode generiert einen Text, der das Datum der internen Rechner Uhr
Åbernimmt.
Das Format hierfÅr ist durch die &apos.DateFormat&apos. Funktion spezifiziert.

:lm margin=3.:p.&larrow.Date&colon.Changed&rarrow.
:lm margin=5.:p.
Diese Funktion generiert Text, die das Datum der letzten DokumentenÑnderung enthÑlt.
Das Format hierfÅr ist durch die &apos.DateFormat&apos. Funktion spezifiziert.

:lm margin=3.:p.&larrow.DateFormat&colon.:hp5.DFormat Zeichenkette:ehp5.&rarrow.
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

:lm margin=3.:p.&larrow.LineSpacing&colon.:hp5.Prozent:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Funktionscode legt den Prozentwert des Zeilenabstandes fest, der ab der
Cursorposition verwendet werden soll.

:lm margin=3.:p.&larrow.JobSettings&colon.:hp5.driverdata:ehp5.&rarrow.
:lm margin=5.:p.
Dieser Funktionscode enthÑlt als Datenparameter die gesamte Datenstruktur
die der Druckerparameter Dialog zurÅckliefert. Die Struktur ist vom verwendeten
Druckertreiber abhÑngig und eventuell vom Autor des Treibers nicht dokumentiert.
Die Struktur hat aber einen Standardvorspann der vin IBM dokumentiert ist.
Diese Daten werden zwischen 15 und 16 eingeschlossen geschrieben, da angenommen
wird, da· sie binÑr sind. Dieser Code kann nicht au·erhalb des Parameter Bereichs
des &apos.Preface&apos. Functions Codes stehen.

:lm margin=3.:p.&larrow.PageNumber&rarrow.
:lm margin=5.:p.
Dieser Funktionscode erzeugt Text, der die augenblickliche Seitennummer an der
Position des Codes in das Dokument einsetzt.

:lm margin=3.:p.&larrow.PageCount&rarrow.
:lm margin=5.:p.
Dieser Funktionscode erzeugt Text, der die Anzahl der Seiten an der
Position des Codes in das Dokument einsetzt.

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

:note.AbstÑnde werden in Einheiten zu 0.001 Zoll oder Zentimeter ausgedrÅckt, je
nachdem ob Sie metrische oder Zoll Einheiten gewÑhlt haben. Ein Abstand von
1/2 Zentimeter oder Zoll wÅrde demnach als 500 dargestellt. Es werden generell nur
positive Werte dargestellt.

:note.Prozentwerte werden als Integerzahlen mit gedachtem Dezimalpunkt dargestellt.
Zum Beispiel wird 100&percent. als 100, 125&percent. als 125 usw dargestellt. Generell werden nur
positive Werte dargestellt.

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
:row.:c.708     :c.Arabic (ASMO 708)
:row.:c.709     :c.Arabic (ASMO 449, BCON V4)
:row.:c.710     :c.Arabic (transparent Arabic)
:row.:c.711     :c.Arabic (Nafitha Enhanced)
:row.:c.720     :c.Arabic (transparent ASMO)
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
:row.:c.1257    :c.Windows, Baltic
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

:p.The SetLanguage function allows Escriba to configure the extension&apos.s language.
The normal name for the language in the language itself is used.
For example, the word for French is Fran&cc.ais.

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
