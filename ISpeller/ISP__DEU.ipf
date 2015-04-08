#define LOCAL_DEFINES
#include "ISpeller.h"
:userdoc.
:title.ISpeller Spell-Checker Help
:docprof toc=123456.

:h1 res=IDD_PRODUCTINFO.Produkt Information (Dialog)
 Diese Dialogbox zeigt den Programmnamen, die Ikone und die Urheberrechts
Informationen. Der Dialog kann mit EINGABE, Leertaste oder Escape Taste oder
durch klicken auf OK beendet werden.

:h2 res=IDD_CONFIGURE.Rechtschreibung Voreinstellungen (Dialog)
Aus diesem Dialog heraus k”nnen Sie das W”rterbuch bestimmen, das die
Rechtschreibprfung verwendet.Auáerdem k”nnen Sie neue W”rterbcher erzeugen
und alte W”rterbcher l”schen.
:p.Die zur Zeit verfgbaren W”rterbcher werden in diesem Dialog angezeigt.
Um eines zum Gebrauch auszuw„hlen klicken Sie auf den Namen ung bet„tigen
die Eingabe Taste oder Sie doppelklicken auf den Namen. Sie werden benachrichtigt,
wenn es aus irgend einem Grund nicht m”glich ist ein W”rterbuch zu ”ffnen.
Andernfalls wird der Dialog verlassen und Sie befinden sich wieder in der
Textverarbeitung.
:p.Um ein neues W”rterbuch herzustellen klicken Sie auf den Erzeugen Knopf.
Der :link reftype=hd res=IDD_BUILD.W”rterbuch erzeugen:elink.
Dialog wird angezeigt und Sie k”nnen ein neues W”rterbuch erstellen.
:p.Um ein W”rterbuch zu l”schen klicken Sie auf den entsprechenden Dateinamen
und dann auf den Entfernen Knopf.

:h3 res=IDD_BUILD.W”rterbuch erzeugen (Dialog)
Von diesem Dialog aus kann man ein neues W”rterbuch fr die Rechtschreibprfung
erstellen.
:p.Um eine W”rterbuchdatei zu erstellen braucht man zwei
Komponenten in Dateiform&colon. die Liste der Worte und die sprachabh„ngige
Zuordnungs (&apos.affix&apos.) Datei.
:p.Die Datei der Worte kann eine zeilenorientierte Liste im normalen
Textformat sein oder sie kann im ISpell &apos.munched&apos. Wortlisten Format aufgebaut
sein. In diesem Format werden Worte in spezieller weise markiert, so daá sie
mit Prefixen oder Suffixen erweitert werden k”nnen. Es gibt keine besonderen
Regeln fr die Namen der Dateien, aber die Sprache sollte im Namen vorkommen.
:p.Die Zuordnungsdatei (&apos.affix&apos.) enth„lt die Regeln, nach denen die Worte
ver„ndert werden k”nnen und ist sprachspezifisch. Die Standardendung fr
diese Dateien ist &apos..aff&apos..
:p.Um ein neues W”rterbuch zu erzeugen, mssen Sie den Namen einer existierenden
Wort-Listen Datei, einer Zuordnungsdatei (affix) und den Namen des zu
erstellenden W”rterbuchs angeben. Wenn Sie diese Angaben gemacht haben, k”nne Sie
auf OK klicken. Die Dateinamen werden nun berprft und der Prozeá der
W”rterbucherzeugung beginnt. Dies kann einige Augenblicke dauern, danach befinden
Sie sich wieder im
:link reftype=hd res=IDD_CONFIGURE.Rechtschreibprfung Voreinstellunegen:elink. Dialog,
von dem aus Sie das neue W”rterbuch zur Benutzung anw„hlen k”nnen. Wenn es
Schwierigkeiten bei der Herstellung des W”rterbuches gegeben hat, werden Sie
darber informiert und Sie k”nne verschiedene Maánahmen (Dateinamen „ndern)
ergreifen, bevor noch einmal versucht wird den Vorgang zu wiederholen.
:p.Wenn Sie den Dialog verlassen wollen, ehe ein W”rterbuch erzeugt wird, k”nnen
Sie die ESCAPE Taste drcken oder auf den ESCAPE Knopf klicken.

:euserdoc.
