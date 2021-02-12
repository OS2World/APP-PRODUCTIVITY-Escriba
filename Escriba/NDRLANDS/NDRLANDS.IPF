#include "..\Escriba.h"
#ifdef APIENTRY
#undef APIENTRY
#endif
:userdoc.
:title.Escriba-hulp voor tekstverwerker
:docprof toc=123456.

:h1 res=ID_MAIN.Inleiding
:i1.Inleiding
:artwork name='Escriba.bmp' align=center.
:p.
Dit programma is een zogenaamde WYSIWYG (&apos.What You See Is What You
Get&apos. m.a.w. het gedrukte eindresultaat zal gelijk zijn aan wat je op
het scherm ziet) tekstverwerker voor de OS/2 Presentation Manager.
Hierbij is het niet de bedoeling te concurreren met de grote, professionele
programma&apos.s voor OS/2-PM of Windows, maar wordt de gebruiker een meer bescheiden
aantal functies ter beschikking gesteld. Het programma beschikt echter wel over
alle gebruikelijke mogelijkheden voor tekstopmaak en laat het toe om grafische
afbeeldingen in documenten op te nemen. Daarenboven kunnen alle ge‹nstalleerde
lettertypes, in alle gangbare formaten en in een ruime selectie van groottes,
gebruikt worden. Het drukken van teksten gebeurt uitsluitend onder controle van
de OS/2 PM en ondersteund daarmee alle drukkers (en instellingen) die ook door
het besturingssysteem ondersteund worden. Dit omvat ook FAX-apparaten en
plotters.
Bij Oosterse versies van OS/2, wordt de dubbel-byte tekenset bij in- en uitvoer
ondersteund, zowel als True-Type lettertypen die van het Font Palette komen.
:p.
:sl compact.
:li.&sqbul.:link reftype=hd res=60001.Starten en be‰indigen:elink.
:li.&sqbul.:link reftype=hd res=60002.Laden en opslaan van documenten:elink.
:li.&sqbul.:link reftype=hd res=60003.Documenten bewerken:elink.
:li.&sqbul.:link reftype=hd res=60004.Documenten drukken:elink.
:li.&sqbul.:link reftype=hd res=60005.Vensterbeheer:elink.
:li.&sqbul.:link reftype=hd res=60006.Een paar waarschuwingen:elink.
:li.&sqbul.:link reftype=hd res=60007.Registratie:elink.
:esl.

:h2 res=60001.Starten &amp. be‰indigen
:hp4.Starten:ehp4.
:p.
Zodra Escriba ge‹nstalleerd is, zijn er drie mogelijkheden om het programma
te starten. Ten eerste kan er gewoon dubbel geklikt worden op het programma-
object dat gemaakt werd tijdens de installatie. Op deze manier wordt de
tekstverwerker geladen en wordt automatisch een leeg documentvenster getoond.
:p.
De tweede methode bestaat daarin, dat een documentbestand naar het programma-
object gesleept wordt. Het document wordt in een eigen venster geladen en
men kan men het meteen bewerken.
:p.
De derde manier is via de START-opdracht. Normaal werkt dit enkel vanaf de
directory waarop Escriba ge‹nstalleerd is. Is deze directory echter opgenomen in
de PATH, HELP en LIBPATH aanwijzingen van het CONFIG.SYS bestand, dan kan de
START-opdracht overal gebruikt worden.
:p.
Er is ook een indirecte manier om Escriba op te starten. Als Escriba een bestand
opslaat in zijn eigen formaat, merkt het dit als een Escriba bestand.  Dit
formaat wordt door het bestandsysteem herkend als behorende bij Escriba
en wanneer het bestand wordt geopend, wordt Escriba opgestart. Escriba kan ook
werken met platte tekst en Pluma-bestanden. Escriba verschijnt ook in het Open
Menu van dergelijke bestanden.
:p.
:hp4.Be‰indigen:ehp4.
:p.
Om Escriba te sluiten kan er eenvoudigweg dubbel geklikt worden op het
programma&apos.s systeemmenu (dit is het mini-ikoon in de linker bovenhoek
van het hoofdvenster) of kun je de F3 toets drukken. Zijn er op dat moment
documenten geladen waarvan de laatste veranderingen nog niet zijn opgeslagen,
dan vraagt het programma of je dit alsnog wilt doen. Het be‰indigen kan op dit
moment ook afgebroken worden.

:h2 res=60002.Laden &amp. opslaan van documenten
:hp4.Laden:ehp4.
:p.
Documenten kunnen in de tekstverwerker geladen worden tijdens het opstarten
of later.
Door een of meer documenten te laten vallen op het object van de
tekstverwerker, begint de tekstverwerker met een venster voor elk van de
documenten. Ook kun je de tekstverwerker direct vanaf de opdrachtlijn
in werking stellen waarbij je de namen van de te bewerken documenten
meegeeft. Wanneer de tekstverwerker op een van de beschreven wijzen
gestart wordt zal de tekstverwerker geladen worden en elk van de
aangegeven documenten in aparte vensters weergegeven worden.
Slechts ‚‚n kopie van de tekstverwerker zal op enig tijdstip in het
geheugen aanwezig zijn.
:p.
Als de tekstverwerker is begonnen, kan men bestanden laden door het
:link reftype=hd res=IDM_OPEN.Open:elink.  menu te kiezen.
Het standaard systeem keuze-dialoog wordt dan getoond. Het is zo dat het
documenten-typen zal presenteren op basis van bestandsnamen en
uitgangen. Maar je kunt &apos.Alle Bestanden&apos. kiezen waardoor
alle bestanden weergegeven worden.  Je kunt net zo veel bestanden
laden als nodig is, en als je de lijst geladen hebt, zullen ze alle
op de achtergrond gehouden worden.  Tijdens het laden van de documenten
zullen alle reeds open documenten tot ikonen verkleind worden.  Het eerste
document dat geladen wordt, wordt het huidige document. Alle andere zullen
door een ikoon weergegeven worden.
:p.
Als je de bestandlijst vrij geeft, zal de tekstverwerker bepalen van welk
type het document is. Als de inhoud van het document voor meer dan een type
geldig is, wordt je gevraagd om een keuze uit een lijstje te maken.
Als geen enkele bestandtype kan worden gevonden, dan wordt je daarover
ge‹nformeerd en stopt het laden.
:p.
:hp4.Opslaan:ehp4.
:p.
Je kunt een document op drie manieren opslaan&colon.
:link reftype=hd res=IDM_SAVE.Opslaan:elink.,
:link reftype=hd res=IDM_SAVEAS.Opslaan Als...:elink. and
:link reftype=hd res=IDM_SAVE_CLOSE.Opslaan en Sluiten:elink..
De Opslaan-opdracht zal het document met de huidige naam en type trachten op te
slaan.  Als dat niet kan zal je gevraagd worden een andere naam en/of type
te kiezen om het document op te kunnen slaan.
De Opslaan Als-opdracht neemt aan dat je het document onder een andere naam
of type op wilt slaan. Geef een nieuwe naam en/of type aan.
De Opslaan en Sluit-opdracht werkt op dezelfde manier als de Opslaan-opdracht,
echter wordt tevens het venster van het huidige document gesloten.
:p.
Probeer je het venster van een gewijzigd, maar niet opgeslagen, document te
sluiten, dan wordt je gevraagd of je dat alsnog wilt opslaan. Dit gebeurt
trouwens ook als je het programma wilt verlaten terwijl er nog gewijzigde
documenten geladen zijn.  De vraag wordt herhaald voor elk gewijzigd bestand dat
nog niet opgeslagen werd.

:h2 res=60003.Documenten bewerken
:hp4.Werking Toetsen:ehp4.
:p.
De volgende toetsen maken het mogelijk om binnen het documentvenster te
bewegen&colon.
:table cols='20 60' rules=both frame=box.
:row.:c.Pijl Links              :c.Ga naar het vorige (controle)teken.
:row.:c.Pijl rechts             :c.Ga naar het volgende (controle)teken.
:row.:c.Pijl naar boven         :c.Ga naar de vorige lijn.
:row.:c.Pijl naar beneden       :c.Ga naar de volgende lijn.
:row.:c.Page Up                 :c.Ga naar de vorige bladzijde.
:row.:c.Page Down               :c.Ga naar de volgende bladzijde.
:row.:c.Home                    :c.Ga naar het begin van de lijn.
:row.:c.End                     :c.Ga naar het einde van de lijn.
:row.:c.Ctrl + Pijl links       :c.Ga naar het vorige woord.
:row.:c.Ctrl + Pijl rechts      :c.Ga naar het volgende woord.
:row.:c.Ctrl + Page Up          :c.Ga naar de bovenkant van de pagina.
:row.:c.Ctrl + Page Down        :c.Ga naar de onderkant van de pagina.
:row.:c.Ctrl + Home             :c.Ga naar het begin van het document.
:row.:c.Ctrl + End              :c.Ga naar het einde van het document.
:etable.
Bovendien kan de muis gebruikt worden om de aanwijzer direct overal in het
zichtbare gedeelte van het document (venster) te plaatsen. Daarbij kan gebruik
gemaakt worden van de horizontale en verticale schuivers -als je die zichtbaar
gemaakt hebt- om het venster zelf over het document te bewegen.

:p.In de rechter benedenhoek van het documentvenster heeft Escriba vier speciale
knoppen, die enkel met de muis gebruikt kunnen worden. Deze knoppen geven
directe toegang tot de functies : Begin document, Vorige bladzijde, Volgende
bladzijde en Einde document en zijn daarmee de directe tegenhangers van de Ctrl-
Home, PageUp, PageDown en Ctrl-End toetsencombinaties.

:p.Terwijl je door door het document beweegt moet je er wel aan denken, dat de
codes die de vorm van het document bepalen (marges, lettertypes- en
groottes, enz.) als aparte tekens in het document aanwezig zijn. Dit wil zeggen
dat de aanwijzer, bij het bewegen, ook op of tussen dergelijke codes kan
geplaatst worden.

:note. Vergeleken met de industrie-standaarden is er een speciaal verschil
in de behandeling van Home en End toetsen.
Als je de Home-toets aanslaat terwijl die al aan het begin van de regel is,
zal de aanwijzer naar het begin van de vorige regel gaan.
Wanneer je de End-toets aanslaat en je staat reeds op het einde van de regel
dan zal je naar het einde van de volgende regel worden gebracht.

:p.:hp4.Tekst:ehp4.

:p.Nieuwe tekst kan eenvoudigweg door intypen ingebracht worden waarbij het
resultaat direct aan de actieve instellingen aangepast wordt. Wordt
een regel daarbij te lang voor de gekozen bladspiegel, dan wordt hij automatisch
gesplitst en komt het laatste woord op de volgende regel terecht.  Typ je
sneller dan het programma kan weergeven, dan gaat er toch niets verloren.  Alle
gegevens worden bijgehouden en het programma zal zo snel mogelijk
de achterstand proberen in te halen.

:p.Bevat een document reeds tekst dan kun je de aanwijzer erdoor verplaatsen en
veranderingen aanbrengen. Normaal bevindt de aanwijzer zich daarbij in de
invoegstand, die wordt voorgesteld door een vertikaal streepje.
Wordt in deze stand nieuwe tekst ingetypt, dan wordt deze tussen de bestaande
gevoegd. Het resultaat wordt direct aan de actieve instellingen aangepast.
Wil je daarentegen tekst vervangen, dan moet je de INSERT toets indrukken.
Hierdoor komt de aanwijzer in de overschrijfstand die voorgesteld wordt door een
zwart blokje.  Alles wat nu ingetypt wordt vervangt de tekens die zich op dat
moment op de positie van de aanwijzer bevinden.
Wees hiermee wel voorzichtig. Ook speciale codes (bijv. instelling lettertype)
kunnen zich onder de aanwijzer bevinden en daarmee, net zoals normale tekst,
overschreven worden.

:p.Bestaande tekst kan geselecteerd worden door de aanwijzer naar het begin van de
gewenste zone te bewegen, de SHIFT-toets in te drukken (en te houden) en daarna
de aanwijzer naar het einde van de zone te verplaatsen. Om hetzelfde te bereiken
kan men ook de muis gebruiken. Men brengt daarbij de aanwijzer naar de gewenste
startpositie, drukt de linker muisknop in (en houdt die ook ingedrukt) en
beweegt de muis tot de aanwijzer zich aan het andere uiteinde van de te
selecteren tekst bevindt.
De geselecteerde tekst kan nu verplaatst of gekopieerd worden, uitgeknipt of
naar het klembord gekopieerd worden of kan vervangen worden door de tekst, die
zich reeds in het klembord bevindt.
Begin je te typen terwijl er nog tekst geselecteerd is, dan wordt deze compleet
vervangen door wat je net intoetst hebt.

:p.Geselecteerde tekst kan eenvoudig met de muis verplaatst of gekopieerd worden.
Daarvoor moet je de rechter muisknop over het geselecteerde blok indrukken (en
ingedrukt houden) en dit dan letterlijk naar de nieuwe positie, zoals aangegeven
door de muisaanwijzer, verslepen. Zodra je de knop loslaat wordt de tekst naar de
nieuwe positie verplaatst. Wil je enkel kopi‰ren, dan moet je de controletoets
indrukken terwijl je de de muisknop loslaat.  Beweeg je de muis niet en laat je
de muisknop los, dan verschijnt er een menu met de beschikbare opdrachten voor
het geselecteerde tekstblok.
:p.
:hp4.Tekstkenmerken:ehp4.
:p.
Op elke positie van een document is een een bepaalde instelling van tekststijlen
en -formaten van toepassing. De marges hebben een bepaalde breedte, de
tabs een specifieke positie en is slechts een lettertype actief. Elk van
deze eigenschappen kan, ofwel voor de rest van het document ofwel voor het
geselecteerde gebied, veranderd worden. Verander je, bijvoorbeeld, het
lettertype terwijl er geen tekst geselecteerd is, dan wordt dit geldig vanaf de
huidige aanwijzerpositie tot aan de eerste code die een nieuwe typeverandering
aanduidt. Was er echter reeds tekst geselecteerd, dan wordt de verandering, in
dit geval enkel het lettertype voor dit hele gebied geldig. Voor de tekst die op
het geselecteerde gebied volgt, blijven de vroegere instellingen gewoon actief.
De voorafgaande beschrijving is niet alleen van toepassing op lettertypes maar
geldt voor alle teksteigenschappen: marges, tabs, tekstgroote, - kleur, etc.
:p.
:hp4.Grafische afbeeldingen:ehp4.
:p.
Grafische afbeeldingen kunnen vanaf het systeem-klembord in het document
geplaatst worden, vanuit elk deel van de zichtbare werkplek of vanuit grafische
bestanden.  Gebruik de :link reftype=hd res=IDM_PASTE_FROM.Plak van:elink. menu om dit
te doen.
:p.
Zodra de afbeelding deel uitmaakt van het document, kun je het op elk moment
verplaatsen, kopi‰ren, vergroten of verkleinen.  Om het te verplaatsen druk je
de rechter muisknop ergens binnenin de afbeelding in, waarna je het eenvoudigweg
naar een nieuwe positie kunt verslepen.  Om te kopi‰ren doe je precies
hetzelfde. Alleen moet je de Ctrl-toets ingedrukt houden wanneer je de
rechter muisknop loslaat.  Om de afmetingen van een afbeelding te veranderen
druk je de rechter muisknop in terwijl de aanwijzer zich net buiten het
(denkbeeldige) kader van de grafiek bevindt. Doe je dit op juiste manier dan
verschijnt er een kader rond de afbeelding en kun je met de muis de grootte
aanpassen.

:h2 res=60004.Documenten afdrukken
Er zijn twee mogelijkheden om documenten af te drukken.
:link reftype=hd res=IDM_PRINT_ALL.Druk alles:elink. en
:link reftype=hd res=IDM_PRINT_PAGE.Druk bladzijde:elink..
:p.
&apos.Druk alles&apos. zorgt ervoor dat het actieve document volledig afgedrukt wordt. Het
zelfde resultaat wordt bereikt door de Ctrl-toets en de &apos.P&apos. toets samen in te
drukken.
:p.&apos.Druk bladzijde&apos. drukt enkel de huidige pagina van het actieve document af.
Het zelfde resultaat wordt bereikt door de Alt-toets en de &apos.P&apos. toets samen in te
drukken.

:h2 res=60005.Vensterbeheer
Meerdere documentvensters kunnen tegelijkertijd geopend zijn. Wordt er een nieuw
document geopend (geladen) dan worden alle bestaande documentvensters in ikonen
omgezet en naast elkaar, onderin het hoofdvenster, gerangschikt.
Het nieuwe venster zal daarbij de ikonen niet verbergen.
:p.
Een dubbele klik op een ikoon volstaat om een documentvenster zijn vroegere
positie en afmetingen terug te geven.
:p.
Herstel je meerdere vensters dan zullen deze elkaar ongetwijfeld geheel of
gedeeltelijk verbergen. Er bestaan menuopdrachten om hieraan iets te doen:
:link reftype=hd res=IDM_TILE.Tegelen:elink. en
:link reftype=hd res=IDM_CASCADE.Stapelen:elink..
Het tegelen zorgt ervoor dat alle geopende documentvensters (d.w.z. die niet als
ikoon te zien zijn) naast en onder elkaar gerangschikt worden zodat er geen
enkel deel verborgen blijft.  Dit is gelijk aan het tegelen van vensters op het
scherm door het systeem zelf.  Bij het stapelen worden alle geopende
documentvensters op elkaar gelegd. Een kleine verschuiving tussen elk venster
zorgt ervoor dat de titels van alle documenten zichtbaar blijven. Het actieve
venster komt bovenop te liggen.
:p.
Er is nog een derde mogelijkheid om vensters te rangschikken:
:link reftype=hd res=IDM_ARRANGEICONS.Ikonen schikken:elink.
Deze menu-opdracht zorgt ervoor dat alle ikonen netjes onderin het hoofdvenster
geplaatst worden.
:p.
Is een documentvenster helemaal verborgen dan kan het 
:link reftype=hd res=IDM_WINDOW_MENU.Vensters:elink. menu, 
dat een lijst van alle geopende vensters bevat,
gebruikt worden om het gezochte venster actief te maken. De afmetingen van een
venster worden daarbij hersteld.

:h2 res=60006.Een paar waarschuwingen
:p.&sqbul.Het opnemen van een heleboel lettertypes in het overeenkomstige
Escriba-menu kan er heel leuk uitzien. OS/2 houdt alle geladen lettertypes echter
in het geheugen totdat het systeem opnieuw gestart wordt.  Dit betekent dat je
&apos.swap&apos.-bestand hierdoor gemakkelijk een aantal megabytes groter kan worden.
:p.&sqbul.Bepaalde ATM-lettertypes die je op BBSen kunt vinden zijn niet
voldoende gecontroleerd en kunnen ervoor zorgen dat je systeem gewoon stopt.
Wees dus voorzichtig als je met nieuwe lettertypes gaat experimenteren. Met de
versies die door IBM of Adobe zelf verspreid worden hoef je je geen zorgen te
maken.
:p.&sqbul.Afbeeldingen in documenten kunnen heel groot zijn. Het verkleinen van
de afbeelding nadat deze in het document opgenomen werd helpt hierbij niets. Het
blijft nog steeds dezelfde afbeelding.

:h2 res=60007.Registratie
Dit programma is shareware en kan als dusdanig (in ongewijzigde vorm !) vrij aan
alle ge‹nteresseerden doorgegeven worden. Vind je het nuttig en besluit je het
verder te gebruiken, dan moet je een bijdrage betalen.
De prijs bedraagt $30.00 (US) en er zijn verschillende manieren om deze
te betalen&colon.
:p.
:hp4.Cheque of overschrijving&colon.:ehp4.
Wil je met een persoonlijke cheque of overschrijving betalen dan moet je deze
samen met je naam, je postadres en het versienummer en datum van je Escriba
kopie zenden aan&colon.
:lm margin=6.:hp4.:sl compact.
:li.Escriba Registrations
:li.c/o Rick Papo
:li.38290 Avondale
:li.Westland, MI 48186-3830
:li.USA
:esl.:ehp4.:lm margin=1.
.br
:hp4.VISA/MasterCard/Discover&colon.:ehp4.
Wil je met een van deze kredietkaarten te betalen dan wordt dit door de firma
BMT Micro behandeld. Deze kan op het nummer (800) 414-4268 (enkel USA) of op
(910) 791-7052, FAX op (910) 350-2937, of, tenslotte, op zijn BBS op (910) 350-
8061 bereikt worden.  De gewone telefoonnummers zijn bereikbaar tussen 8 uur en 20 uur
(EST,-5 UTC).
De andere nummers staan 24 uur per dag, zeven dagen per week ter beschikking.
Het adres van de firma is&colon.
:lm margin=6.:hp4.
:sl compact.
:li.BMT Micro
:li.452 Horn Rd
:li.Wilmington, NC 28412-2411
:li.USA
:esl.:ehp4.:lm margin=1.
:note. Woon je in een van de vroegere Sovietrepublieken of Warschaupaktstaten
dan kun je registreren voor $5 (geen kredietkaarten a.u.b.).
:p.Na registratie wordt je een ontvangstbewijs en de nieuwste uitgave van
de tekstverwerker op diskette toegestuurd. Later ontvang je dan nog een extra
exemplaar bij het uitkomen van de eerstvolgende grote versie.
Gedrukte documentatie is nog niet beschikbaar, maar zodra dit wel het geval is
zullen alle geregistreerde gebruikers een gratis exemplaar ontvangen.
Wil je later nog verdere versies op diskette dan kost dit $5.

:h2 res=60008.Vragen en rapporteren van problemen
Als je enige problemen met dit programma ondervindt, kun je de schrijver Rick
Papo bereiken op een van de volgende manieren&colon.
:table cols='15 60' rules=both frame=box.
:row.:c.Internet        :c.:hp4.rpapo@msen.com:ehp4.
:row.:c.Mail            :c.:hp4.Rick Papo, 38290 Avondale, Westland, MI 48186-3830, USA:ehp4.
:row.:c.Telephone       :c.:hp4.USA (734) 326-8547:ehp4.
:etable.
De hoogste prioriteit wordt gegeven aan vragen en problemen van geregistreerde
gebruikers (zie :link reftype=hd res=60007.Registratie:elink.), maar er zal getracht
worden alle berichten die gedurende de dag binnenkomen te beantwoorden.
:p.
Vermeldt bij alle correspondentie het versienummer van Escriba die je gebruikt
en waar je het over hebt.

:h1 res=IDM_FILE_MENU.Bestandmenu
:i1.Bestandmenu
In het bestandmenu zijn de volgende opdrachten beschikbaar&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_NEW.Nieuw:elink.
:li.&sqbul.:link reftype=hd res=IDM_OPEN.Openen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_RECALL.Terughalen:elink.
:li.&sqbul.:link reftype=hd res=IDM_MERGE.Samenvoegen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVE.Opslaan:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVEAS.Opslaan als...:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVE_CLOSE.Opslaan en sluiten:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLOSE.Sluiten:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT.Afdrukken:elink.
:li.&sqbul.:link reftype=hd res=IDM_PREFERENCES.Algemene instellingen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_DEFAULTS.Documentinstellingen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_CONFIGURE_EXTENSIONS.Configuratie Aanvullingen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_LANGUAGE_MENU.Taal:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXIT.Be‰indigen:elink.
:esl.

:h2 res=IDM_NEW.Nieuwe (Menu-optie)
Deze optie opent een nieuw, leeg, documentvenster. Als beginwaarden voor de marges,
het lettertype en de andere instellingen worden die genomen die d.m.v. de laatste
documentinstellingen-opdracht ingegeven werden terwijl er geen documentvenster actief was.
:p.
Alle, reeds geopende, documentvensters worden tot ikonen gereduceerd en onderin
het hoofdvenster geplaatst. Het nieuwe venster neemt alle beschikbare plaats in.

:h2 res=IDM_OPEN.Openen... (Menu-optie)
Na het kiezen van deze optie wordt een standaard bestanddialoog geopend van
waaruit de gebruiker een of meerdere documenten kan kiezen. Het indrukken van de
Enter-toets, of het klikken op OK met de muis, start het laden van alle gekozen
bestanden.  De status wordt daarbij aangegeven door een rood veld in het
rechterdeel van de titel van het hoofdvenster. Voor elk document dat volledig
geladen is wordt een apart venster gemaakt.  Enkel het venster dat het document
bevat dat als eerste volledig geladen is, wordt echt geopend. Alle overige
worden als ikonen voorgesteld en onderin het hoofdvenster gerangschikt.
Ze kunnen op hun beurt d.m.v. het venstermenu of een dubbele klik met de muis
geopend worden.
:p.Deze opdracht kan ook d.m.v. de Ctrl+O toetsencombinatie gegeven worden.
:p.Zie ook :link reftype=hd res=256.Openen/Samenvoegen/Opslaan als... (Dialoog):elink.
voor meer informatie.

:h3 res=256.Openen/Samenvoegen/Opslaan als... (Dialoog)
Dit dialoogvenster wordt geopend door het kiezen van de Openen, Samenvoegen of
Opslaan als opdrachten en komt overeen met de standaard dialoog die ook door het
systeem zelf gebruikt wordt (zie systeemdocumentatie).
:artwork name='NdrLands\FileDlg.bmp' align=center.
:p.De volgende opmerkingen kunnen hierbij gemaakt worden&colon.
:p.Wanneer het dialoog-scherm getoond wordt voor Openen of Samenvoegen, wordt
het &apos.Bestandtype&apos.-veld ingesteld op &apos.Alle Bestandtypen&apos.
Nu worden alle bestanden getoond die voldoen aan Escriba&apos.s formaat
en van alle andere formaten (Platte tekst, BMP, GIF, JPG, PCX, TIF, etc.).
Je kunt de bestandtypen wijzigen in &apos.Alle Bestandtypen&apos. of enige
van de andere typen die in de lijst voorkomen.
:p.Als Opslaan of Opslaan Als wordt gekozen, wordt het huidige document-
type gekozen. Als je het bestandtype wilt wijzigen, maak dan een keuze
naar welk bestandtype je het wilt opslaan.
:p.Je kunt zoveel bestanden openen of selecteren als je zelf wilt.
:p.Bij het kiezen van een bestand om samen te voegen mag je telkens slechts een
naam selecteren.
:p.Bij het Opslaan als opdracht kun je geen bestaande naam kiezen uit de lijst.
Deze dient enkel als hulp. De naam moet je zelf intypen.
:p.De directory die in deze dialoog gekozen wordt, wordt automatisch de nieuwe
actieve directory en wordt getoond bij het opnieuw openen van de Openen,
Samenvoegen, Opslaan als dialogen.

:note.Bestanden worden als Escriba-documenten gemerkt op het moment dat ze op
de schijf opgeslagen worden. Dit merken gebeurt d.m.v. zogenaamde Extended
Attributes die jammer genoeg verloren gaan als het bestand naar diskette
gekopieerd wordt of gecomprimeerd wordt met bepaalde archiveringsprogramma&apos.s
(bijv. PKZIP). De nieuwste (OS/2) versies van deze programma&apos.s zorgen er wel
voor dat alle informatie behouden blijft.

:h4 res=IDD_FILETYPE.Kies Bestandtype (Dialoog)
Je komt in dit dialoog als je een bestand wilt laden en dit bestand
voldoet aan meerdere bestandtypen. Je krijgt de naam van het bestand
en de bestandtypen waaruit je een keus kunt maken.
Als je de keuze gemaakt hebt klik je op het gekozen type of drukt de
Enter-toets. Om het bestand te verlaten druk je op de Esc-toets of
klik op de Escape-knop.

:h2 res=IDM_RECALL.Terughalen Menu
Dit optiemenu geeft een submenu, waarin de tien recentste documenten zijn
weergegeven.
:p.Zie :link reftype=hd res=IDM_RECALL0.Terughalen Menu:elink.
voor meer informatie.

:h3 res=IDM_RECALL0.Haal terug menu lijst
Deze menulijst geeft de tien recentste opgeslagen documenten.
Om een van deze documenten te openen in een nieuw venster, maak een keuze met de
pijltoetsen en druk op Enter, of klik op een van hen met de muis.

:h2 res=IDM_MERGE.Samenvoegen... (Menu-optie)
Na het kiezen van deze optie wordt een standaard bestanddialoog geopend
waarmee je een (1) bestand kunt kiezen dat, op de huidige positie van de
aanwijzer, in het actieve document gevoegd wordt.
:p.Zie ook :link reftype=hd res=256.Openen/Samenvoegen/Opslaan als... (Dialoog):elink.
voor meer informatie.

:h2 res=IDM_SAVE.Opslaan (Menu-optie)
Deze optie zorgt ervoor dat het actieve document onmiddellijk onder zijn
huidige naam opgeslagen wordt. De optie is niet beschikbaar als er nog geen
veranderingen zijn t.o.v. de stand op schijf.
:p.Deze optie kan ook d.m.v. de F2-toets gestart worden.

:h2 res=IDM_SAVEAS.Opslaan als... (Menu-optie)
Na het kiezen van deze optie wordt een standaard bestanddialoog geopend.
Hier kun je de naam aangeven waaronder het actieve document zal worden
opgeslagen en de directory opgeven waarin dit zal gebeuren.
:p.Zie ook :link reftype=hd res=256.Openen/Samenvoegen/Opslaan als... (Dialoog):elink.
voor meer informatie.

:h2 res=IDM_SAVE_CLOSE.Opslaan &amp. sluiten (Menu-optie)
Hiermee is het mogelijk om met een enkele optie het actieve document op te
slaan en het overeenkomstige venster te sluiten.
:p.Deze optie kan ook d.m.v.  de F4-toets gestart worden.

:h2 res=IDM_CLOSE.Sluiten (Menu-optie)
Deze optie zorgt ervoor dat het actieve document gesloten wordt. Voor dit
gebeurt wordt er nagegaan of er verschillen bestaan met de stand op schijf. Is
dit het geval, dan wordt je gevraagd of je het bestand wilt opslaan. Druk je dan
de JA knop (J toets) dan wordt er opgeslagen. Druk je op de NEE knop (N toets)
dan gaan alle veranderingen verloren. De STOP knop (Escape-toets) onderbreekt
het sluiten.

:h2 res=IDM_PRINT.Afdrukken (Submenu)
Dit submenu geeft toegang tot de verschillende afdrukopdrachten. Op dit moment
zijn er daarvan twee beschikbaar&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL.Druk alles:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_PAGE.Druk bladzijde:elink.
:esl.

:h3 res=IDM_PRINT_ALL.Druk alles Menu
Dit submenu geeft toegang naar de mogelijke wijzen waarop je een geheel document
wilt afdrukken. De volgende opdrachten zijn beschikbaar&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL_NORMAL.Normaal:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL_REVERSE.Omgekeerd:elink.
:esl.

:h4 res=IDM_PRINT_ALL_NORMAL.Normaal (Menu-optie)
Het document, dat op het moment dat je dit menu selecteerde actief was, wordt
afgedrukt.  Een rood statusvenster wordt getoond wanneer het document naar de
systeem-spooler wordt gezonden, dit is alleen ter informatie. Je kunt direct
verder werken aan dit document nadat je de afdrukopdracht hebt gegeven. Het
document wordt dan afgedrukt in de staat zoals het was op het moment van het
geven van de afdrukopdracht.
:p.In plaats van het menu kun je ook Ctrl+P indrukken.

:h4 res=IDM_PRINT_ALL_REVERSE.Omgekeerd (Menu-optie)
Het huidige document wordt afgedrukt als je deze optie kiest, net zoals in
:link reftype=hd res=IDM_PRINT_ALL_NORMAL.Normaal:elink., behalve dat de pagina&apos.s dan
in omgekeerde volgorde worden afgedrukt.

:h3 res=IDM_PRINT_PAGE.Druk bladzijde (Menu-optie)
Deze optie zorgt ervoor dat de huidige pagina van het actieve document
gedrukt wordt. Ter informatie wordt er tijdens het afdrukken (zenden naar de
OS/2 spooler) een rode statusindicatie getoond. Zodra je de afdrukopdracht hebt
gegeven kun je verder werken. De staat waarin de bladzijde zich bevond bij het
geven van de opdracht, wordt afgedrukt.
:p.In plaats van het menu te gebruiken kun je ook ALT+P indrukken.

:h2 res=IDM_PREFERENCES.Algemene instellingen... (Menu-optie)
:p.Zie :link reftype=hd res=IDD_PREFERENCES.Algemene instellingen (Dialoog):elink.
voor meer informatie.

:h3 res=IDD_PREFERENCES.Algemene instellingen (Dialoog)
Deze dialoog maakt het mogelijk het voorkomen en het gedrag van Escriba te
wijzigen voor zover dit te maken heeft met het programma en niet op het geladen
document.
:artwork name='NdrLands\Prefer.bmp' align=center.
De volgende instellingen zijn beschikbaar&colon.
:p.:hp2.Animeer:ehp2.
Deze instelling zorgt ervoor dat Escriba, net zoals OS/2 zelf ook kan, het
openen en sluiten van zijn hoofdvenster animeert. Ook hier geldt dat deze
instelling enkel effectief is als animatie in de System Setup folder is
ingesteld.
:p.:hp2.Toon meetlatten:ehp2.
Na het inschakelen van deze keuze hebben alle documentvensters horizontale en
verticale meetlatten aan hun randen.
:p.:hp2.Toon schuivers:ehp2.
Deze instelling zorgt ervoor dat alle documentvensters horizontale en verticale
schuivers bezitten. Hierdoor kan er gemakkelijk met behulp van de muis door een
document gegaan worden.
:p.:hp2.Laat Statusregel zien:ehp2.
Als dit onderdeel is gemerkt zullen alle documenten van een statusregel
worden voorzien waar de positie van de aanwijzer en de interpretatie van
het symbool op de aanwijzer is aangegeven.
:p.:hp2.Toon volledige bestandnamen:ehp2.
Is deze instelling actief, dan wordt de volledige bestandnaam van een document
in het titelveld van het overeenkomstige venster getoond. In het andere geval
worden noch het station noch de directory vermeld.
:p.:hp2.Spool Pagina&apos.s:ehp2. Wanneer deze optie aanstaat, wordt bij het
afdrukken van een document met meerdere pagina&apos.s, elke pagina als een taak
weggezet. Daar afdruktaken niet worden begonnen voordat het complete document in
het spool-gebied is geplaatst, kan door het gebruik van deze optie het afdrukken
onmiddellijk beginnen nadat de eerste pagina overgedragen is. Echter als je
drukker in een netwerk wordt gebruikt kunnen je pagina&apos.s met die van andere
gebruikers door elkaar komen.
:p.:hp2.Druk Pagina&apos.s Omgekeerd af:ehp2.
Wanneer dit onderdeel is gemerkt, zal de afdrukvolgorde zo zijn dat de laatste
pagina eerst afgedrukt wordt. Deze optie is aangebracht ter ondersteuning
van die gebruikers die een Deskjet hebben, waarbij pagina&apos.s met de voorzijde
naar boven worden afgelegd.
:p.:hp2.Herlaadt Document bij opstarten:ehp2.
Als dit is gemerkt, dan zal het laatste document uit de oproeplijst automatische
geladen worden wanneer Escriba wordt opgestart.
:p.:hp2.Stand. Afbeeldingfactor:ehp2.
Met deze knoppen kun je de beginwaarde voor de afbeeldingfactor van een
documentvenster instellen. Deze waarde zal automatisch gebruikt worden bij het
openen van nieuwe vensters of bij het laden van documenten.  Er is keuze uit de
volgende waarden&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_100.Afbeelden 100&percent.:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLWIDTH.Afbeelden volle breedte:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLHEIGHT.Afbeelden volle hoogte:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_MARGINS.Afbeelden marges:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_ALL.Afbeelden alles:elink.
:esl.
:p.Veranderingen in de algemene instellingen kunnen door het drukken van de
Enter-toets of door het indrukken van de OK-knop opgeslagen worden. De nieuwe
waarden zijn vanaf dat moment voor alle nieuwe documenten geldig.
:p.Wil je de wijzigingen die je tijdens de dialoog gemaakt hebt ongedaan maken,
dan moet je op Esc drukken (toets of knop).

:h2 res=IDM_DEFAULTS.Documentinstellingen... (Menu-optie)
Deze optie begint een dialoog waarin het mogelijk is de begininstellingen voor
de drukker, de papiergrootte, het lettertype en -kenmerken en de tekstuitlijning
(horizontaal en verticaal) voor een document te wijzigen.
:p.Zie ook :link reftype=hd res=IDD_DEFAULTS.Documentinstellingen (Dialoog):elink.
voor meer informatie.

:h3 res=IDD_DEFAULTS.Documentinstellingen (Dialoog)
Dit dialoog maakt het mogelijk om de basiskenmerken van documenten te wijzigen.
Deze veranderingen slaan enkel op het actieve document tenzij er tijdens het
ingeven geen document geopend is. In dit geval worden de nieuwe waarden voor
alle nieuwe documenten gebruikt.
.br
De configuratie mogelijkheden zijn gegroepeerd bij categorie op de volgende
pagina&apos.s&colon.
:sl compact.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_AUTHOR.Auteur:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_PRINTER.Drukker:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_FONT.Lettertype:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_LAYOUT1.Opmaak (1):elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_LAYOUT2.Opmaak (2):elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_NATIONAL.Taal:elink.
:esl.
:p.Veranderingen in deze instellingen kunnen door het indreukken van de Enter-toets of de OK-knop opgeslagen worden. Zijn er op dit moment geen documenten geopend of wordt Algemeen Opslaan geselecteerd, dan zijn de wijzigingen voor alle nieuwe
document
en geldig. Is er een document geopend dan zijn ze ook hiervoor geldig. Geen enkel ander document wordt be‹nvloed.
:p.
Druk je op de DOEN-knop dan worden de wijzigingen onmiddelijk in het actieve document doorgevoerd en worden dit de nieuwe standaardwaarden.
:p.
Wil je de veranderingen die je tijden het dialoog gemaakt hebt ongedaan maken, dan moet je Esc. drukken (toets of knop). Het effect van de DOEN-Knop kan niet tenietgedaan worden.

:h4 res=IDD_DEFAULTS_PAGE_AUTHOR.Auteur Instellingen Pagina
:artwork name='NdrLands\Default0.bmp' align=center.
:p.:hp2.Auteur:ehp2.
Dit invoerveld geeft je de mogelijkheid om de naam van de auteur en de titel die met het document veilig gesteld zijn te bekijken/te veranderen. Je zult ook de aanmaakdatum kunnen zien.

:h4 res=IDD_DEFAULTS_PAGE_PRINTER.Instelling van de Drukker
:artwork name='NdrLands\Default1.bmp' align=center.
:p.:hp2.Drukker:ehp2.
Hiermee is het mogelijk om aan te geven op welke drukker (indien er meerdere
beschikbaar zijn) het document moet worden gedrukt. Elke drukker, die bij het
systeem bekend is, kan gekozen worden.
:p.:hp2.Drukkerinstellingen:ehp2.
Bij het drukken op deze knop wordt de standaard instellingendialoog voor de
geselecteerde drukker gestart. Aangezien deze dialoog afhankelijk is van het
drukkertype, kan er hier geen verdere uitleg gegeven worden. Na het verlaten van
deze dialoog worden het papiertype en -grootte (direct onder de knop) aangepast.

:h4 res=IDD_DEFAULTS_PAGE_FONT.Instellingen Lettertype
:artwork name='NdrLands\Default2.bmp' align=center.
:p.:hp2.Lettertype:ehp2.
Dit bepaalt het beginlettertype voor het document. Er kan gekozen worden uit
alle ge‹nstalleerde types.
:p.:hp2.Grootte:ehp2.
Dit is de begingrootte voor de letters van het document. Er kan gekozen worden
uit een uitgebreide lijst.
:p.:hp2.Kleur:ehp2.
Dit is de beginkleur voor de tekst van het document. Er kan gekozen worden uit
een uitgebreide lijst.
:p.:hp2.Vet:ehp2.
Wordt deze optie gekozen dan is de begininstelling voor nieuwe tekst vet.
Bestaat er hiervoor een apart lettertype dan wordt dit gebruikt. Anders maakt
het systeem zelf zijn eigen, afgeleide, type.
:p.:hp2.Schuin:ehp2.
Wordt deze optie gekozen dan is de begininstelling voor nieuwe tekst schuin.
Bestaat er hiervoor een apart lettertype dan wordt dit gebruikt. Anders maakt
het systeem zelf zijn eigen, afgeleide, type.
:p.:hp2.Onderlijnd:ehp2.
Wordt deze optie gekozen dan is de begininstelling voor nieuwe tekst onderlijnd.
:p.:hp2.Doorstreept:ehp2.
Wordt deze optie gekozen dan is de begininstelling voor nieuwe tekst
doorstreept.
:p.:hp2.Hol:ehp2.
Wordt deze optie gekozen dan is de begininstelling voor nieuwe tekst hol. M.a.w.
de letters worden niet opgevuld.
:p.:hp2.Hoofdletters:ehp2.
Wordt deze optie gekozen dan is de begininstelling voor nieuwe tekst
hoofdletters.

:h4 res=IDD_DEFAULTS_PAGE_LAYOUT1.Bladinstellingen Pagina 1
:artwork name='NdrLands\Default3.bmp' align=center.
:p.:hp2.Uitlijning:ehp2.
Dit bepaalt de standaard horizontale uitlijning die voor het document gebruikt
wordt. De beschikbare mogelijkheden zijn links, gecentreerd, rechts en volledig.
Linkse uitlijning zorgt ervoor dat alle tekst bij de linker kantlijn begint.
Centrering maakt dat de afzonderlijke lijnen telkens in het midden tussen de
beide kantlijnen staan. De afstand tussen de woorden wordt hiervoor niet
aangepast.  Rechtse uitlijning zorgt ervoor dat alle tekst bij de rechter
kantlijn eindigt.  Volledige uitlijning maakt dat alle volledige lijnen bij de
linker kantlijnen beginnen en mooi bij de rechter kantlijn eindigen. Om dit te
bereiken wordt de afstand tussen de afzonderlijke woorden aangepast.
:p.:hp2.Bladmidden:ehp2.
Wordt deze optie gekozen dan wordt de tekst verticaal op de bladzijde
gecentreerd.
:p.:hp2.Marges:ehp2.
Deze vier waarden, in centimeters of inches, bepalen de begininstellingen voor
de marges. De aangegeven waarden kunnen hierbij niet groter zijn dan de
actuele bladspiegel.

:h4 res=IDD_DEFAULTS_PAGE_LAYOUT2.Bladinstellingen Pagina 2
:artwork name='NdrLands\Default4.bmp' align=center.
:p.:hp2.Tabinstellingen:ehp2.
Dit veld bevat de begininstelling voor de onderlinge afstand tussen twee
opeenvolgende tabposities. Wordt hier 0.500 ingevuld dan betekent dit dat er een
afstand van een halve centimeter (of inch) is tussen twee posities beginnend
vanaf de linker kantlijn.
:p.:hp2.Inspringen:ehp2.
Dit veld geeft je de gelegenheid om de standaardwaarde in te stellen
waarmee elke paragraaf moet inspringen. Als je hier 0.500 in zou brengen
zou je zien dat de eerste regel van het document en elke eerste regel
die een einde-lijn-code volgt 0.5 centimeter of inch van de rechter
kantlijn zal beginnen.
:p.:hp2.Lijnafstand:ehp2.
Maakt het mogelijk te kiezen tussen drie verschillende lijnafstanden.
Mogelijkheid &apos.Enkel&apos. betekent een normale (=100&percent.) afstand.
Mogelijkheid 1 1/2 betekent 150&percent. en &apos.Dubbel&apos. betekent 200&percent. of dubbele afstand.

:h4 res=IDD_DEFAULTS_PAGE_NATIONAL.Nationale Instellingen Pagina
:artwork name='NdrLands\Default5.bmp' align=center.
:p.:hp2.Code Page:ehp2.
Door middel van de code is het mogelijk de code-page van je landtaal in te
stellen die met het document gebruikt moet worden.
See :link reftype=hd res=61002.Code Pages:elink. voor meer informatie.
:p.:hp2.Metrisch:ehp2.
Deze optie zorgt ervoor dat voor alle afmetingen metrische eenheden worden
gebruikt. De horizontale en verticale meetlatten worden in centimeters verdeeld.
:p.:hp2.Datumformaat:ehp2.
Dit veld toont het momenteel ingestelde datumformaat dat zal gebruikt worden in
de Huidige datum en Datum van vandaag functies.
Zie ook :link reftype=hd res=IDD_DATEFMT.Datumformaat:elink. voor meer informatie.

:h2 res=IDM_CONFIGURE_EXTENSIONS.Configuratie Aanvullingen... (Menu-optie)
Deze optie begint een dialoog waarmee het mogelijk is om aanvullende
instellingen te veranderen
:p.Zie :link reftype=hd res=IDD_CONFIGURE_EXTENSIONS.Configuratie Aanvullingen(Dialoog):elink.
voor meer informatie.

:h3 res=IDD_CONFIGURE_EXTENSIONS.Configuratie Aanvullingen (Dialoog)
Deze dialoog geeft je de mogelijkheid om de beschikbare aanvullingen in te
stellen.
:artwork name='NdrLands\ConfigEx.bmp' align=center.

:h3 res=IDM_LANGUAGE_MENU.Taal (Submenu)
Dit menu stelt je in staat om een taal in te stellen voor alle programma-
menu&apos.s en dialogen.
:p.Zie :link reftype=hd res=IDM_LANGUAGES.Taal Invoermenu Entry:elink.
voor meer informatie.

:h4 res=IDM_LANGUAGES.Taal Invoermenu (Menu Optie)
Elk van de regels in het Taal-submenu kan gebruikt worden om een andere
taal voor de programma-menu&apos.s en dialogen te kiezen. Als bijvoorbeeld
de namen in hun oorspronkelijke vorm zouden staan, wordt door een andere
keuze het hele programma aangepast om de andere taal te ondersteunen.
Sommige van de eigenschappen van de documenten zoals het formaat van
de datum-weergave zal mee veranderen.

:h2 res=IDM_EXIT.Be‰indigen (Menu Optie)
Deze optie be‰indigt het programma.

:h1 res=IDM_EDIT_MENU.Bewerken Menu
:i1.Bewerken Menu
In dit menu kun je kiezen uit de volgende mogelijkheden&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_UNDO.Tenietdoen:elink.
:li.&sqbul.:link reftype=hd res=IDM_REDO.Opnieuw:elink.
:li.&sqbul.:link reftype=hd res=IDM_CUT.Snijden:elink.
:li.&sqbul.:link reftype=hd res=IDM_COPY.Kopi‰ren:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE.Plakken:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEAR.Wissen:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEARCLIPBOARD.Klembord Wissen:elink.
:li.&sqbul.:link reftype=hd res=IDM_SELECT_ALL.Kies Alles:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM.Plak van Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_INSERT_FUNCTION.Invoeg Functie Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_SEARCH.Zoek...:elink.
:li.&sqbul.:link reftype=hd res=IDM_FINDNEXT.Zoek Volgende:elink.
:li.&sqbul.:link reftype=hd res=IDM_GOTOPAGE.Ga naar Pagina:elink.
:li.&sqbul.:link reftype=hd res=IDM_SPELL.Spellingcontrole...:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM.Afbeelden:elink.
:esl.

:h2 res=IDM_UNDO.Teniet doen (Menu-optie)
Deze functie zet de laatste wijzigingen in het document terug naar hun originele
waarde.  Als je verder gaat met deze functie wordt ook de voorlaatste wijziging
ongedaan gemaakt, en dat gaat zo verder tot de laatste wijziging in het document
ongedaan is gemaakt. Bij een nieuw document zal het uiteindelijke resultaat een
leeg document zijn.
:p.De snelle manier om dit te bewerkstelligen is om Alt+Backspace te gebruiken.

:h2 res=IDM_REDO.Opnieuw (Menu-optie)
Indien je het document sinds de laatste teniet-doen-actie niet veranderd hebt,
zal deze functie de verandering weer opnieuw instellen. Het is mogelijk om elke
wijziging in het document teniet te doen, en daarna weer opnieuw in originele
staat te brengen.
Als je echter, nadat je een aantal wijzigingen met een
:link reftype=hd res=IDM_UNDO.teniet doen:elink. actie ongedaan hebt gemaakt, zullen
alle verdere veranderingen niet vastgelegd worden en is het niet mogelijk om ze
weer opnieuw in te stellen.
:p.De snelle manier is Shift+Alt+Backspace.

:h2 res=IDM_CUT.Snijden (Menu-optie)
Deze optie zorgt ervoor dat de geselecteerde tekst uit het document wordt
verwijderd en in het klembord wordt geplaatst.
:p.Deze optie kan ook d.m.v. de Shift+Delete toetsencombinatie gestart
worden.

:h2 res=IDM_COPY.Kopi‰ren (Menu-optie)
Deze optie zorgt ervoor dat de geselecteerde tekst in het klembord wordt
gekopieerd.
:p.Deze optie kan ook d.m.v. de Ctrl+Insert toetsencombinatie gestart worden.

:h2 res=IDM_PASTE.Plakken (Menu-optie)
Deze optie zorgt ervoor dat de tekst in het klembord ofwel de geselecteerde
tekst in het actieve document vervangt (in Overschrijf-toestand) ofwel na de
aanwijzer ingevoegd wordt (in Invoeg-toestand).
:p.Deze optie kan ook d.m.v. de Shift+Insert toetsencombinatie gestart
worden.

:h2 res=IDM_CLEAR.Wissen (Menu-optie)
Deze optie wist de geselecteerde tekst uit het actieve document zonder
daarbij de inhoud van het klembord te wijzigen.  Deze optie komt volledig
overeen met het drukken van de Delete-toets.

:h2 res=IDM_CLEARCLIPBOARD.Klembord Wissen (Menu-optie)
Deze optie wist de volledige inhoud van het klembord.

:h2 res=IDM_SELECT_ALL.Kies Alles (Menu-optie)
Deze optie slecteert alle tekst van het huidige document van het begin
tot het einde.
:p.Deze optie kan ook gekozen worden door met toets 1 van de muis dubbel te klikken op de bovenste hoek tussen de horizontale en vertikale schuiflatten.

:h2 res=IDM_PASTE_FROM.Plak van Menu
Vanuit dit menuscherm is het mogelijk om algemene beelden in het document op te
nemen.  De volgende mogelijkheden zijn beschikbaar&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP.Plak vanuit het Systeem-klembord:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN.Plak vanuit Scherm:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_FILE.Plak vanuit Grafisch Bestand:elink.
:esl.

:h3 res=IDM_PASTE_FROM_CLIP.Plak vanuit Klembord Menu
:p.Vanuit dit menu is het mogelijk om een plak-optie te kiezen die gegevens
neemt van een grafisch beeld uit het klembord.  De volgende opties zijn mogelijk
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Plak Bitmap vanuit Klembord:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP_METAFILE.PLak Metafile vanuit Klembord:elink.
:esl.

:h4 res=IDM_PASTE_FROM_CLIP_BITMAP.Plak Bitmap van Klembord (Menu-optie)
:p.Als een beeld in bitmap in het systeem-klembord zit, staat dit menu je toe om
dit beeld in je document te plaatsen.  Nadat je deze optie gekozen hebt, moet je
wijzen naar een van de hoeken  van het gebied waar je het beeld wilt plaatsen.
Kies de hoek door de muis er naar te verplaatsen, dan de linker knop van de muis
indrukken. Terwijl je de muisknop ingedrukt houdt beweeg je de muis naar de
andere hoek  van het gebied waar je het beeld wilt plaatsen. Een rechthoek zal
ontstaan terwijl je de muis beweegt. Als je tevreden bent met de plaats en
grootte van de rechthoek, laat je de muisknop los. Het beeld zal dan binnen de
rechthoek geplaatst worden, en zal dan uitgerekt of samengepakt worden om te
passen binnen de rechthoek.

:h4 res=IDM_PASTE_FROM_CLIP_METAFILE.Plak Metabestand vanuit Klembord (Menu-optie)
:p.Indien een metabestand-beeld in het systeem-klembord aanwezig is, dan geeft
dit menu je de mogelijkheid om dit beeld in je document te plaatsen.  Nadat je
deze optie gekozen hebt, moet je dit beeld in je document plaatsen, net zoals je
dit gedaan hebt voor :link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Plak Bitmap vanuit Klembord:elink. menu optie.

:h3 res=IDM_PASTE_FROM_SCRN.Plak vanuit Scherm Menu
:p.Vanuit dit menu is het mogelijk om een Plak optie te kiezen die een grafisch
beeld pakt vanuit het zichtbare scherm. De volgende opties zijn
beschikbaar&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_ENTIRE.Plak de Gehele Werkplek vanuit het Scherm:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_FRAME.Plak een Raam vanuit het Scherm:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_WINDOW.Plak Venster vanuit het Scherm:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_RECT.Plak een Rechthoek vanuit het Scherm:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_HIDE.Verberg:elink.
:esl.

:h4 res=IDM_PASTE_FROM_SCRN_ENTIRE.Plak Gehele Werkplek vanuit het Scherm (Menu-optie)
:p.Wanneer je deze optie uit het menu kiest, een bitmap-kopie van de hele OS/2-
werkplek, zoals het nu is, wordt in het document geplaatst. Je moet nu het
grafische beeld in het document plaatsen op de zelfde manier als voor 
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Plak Bitmap vanuit Klembord:elink. menu optie.
:note. Pas op met dit grafische plakken, daar de grafische informatie erg groot
kan zijn met deze manier van kopi‰ren. Op wat voor manier je de grafische
gegevens ook inschaalt in het document, de ruimte die het beeld in het geheugen
inneemt blijft het zelfde. Het beeld van een gehele werkplek kan, afhankelijk
van het oplossend vermogen van de Video en de kleur, tussen de 100k en 800k
bytes in beslag nemen.

:h4 res=IDM_PASTE_FROM_SCRN_FRAME.Plak Kader vanuit het Scherm (Menu-optie)
Deze functie in nog niet beschikbaar.

:h4 res=IDM_PASTE_FROM_SCRN_WINDOW.Plak venster vanuit het Scherm (Menu-optie).
Deze functie is nog niet beschikbaar.

:h4 res=IDM_PASTE_FROM_SCRN_RECT.Plak Rechthoek vanuit het Scherm (Menu-optie).
Deze functie is nog niet beschikbaar.

:h4 res=IDM_PASTE_FROM_SCRN_HIDE.Verberg (Menu-optie)
Als deze menu-optie aangemerkt is zal het programma verdwijnen als een van de
:link reftype=hd res=IDM_PASTE_FROM_SCRN.Plak van Scherm menu:elink. gevraagd wordt.
Het programma verdwijnt, de Plak Van opdracht wordt uitgevoerd, en het
programma komt weer op zijn originele plaats op de werkplek terug.

:h3 res=IDM_PASTE_FROM_FILE.Plak Beeld vanuit Bestand (Menu-optie)
Wanneer je deze optie kiest wordt het normale systeembestand selectiedialoog
gepresenteerd, van waaruit je een grafische bestand kunt kiezen om in je
document te plakken. De bestandtypen die op dit moment worden ondersteund
zijn&colon.
:sl compact.
:li.&sqbul.IBM OS/2 Bitmap (.BMP)
:li.&sqbul.IBM OS/2 MetaFile (.MET)
:li.&sqbul.Microsoft Windows Bitmap (.BMP)
:li.&sqbul.Graphic Interchange Format (.GIF)
:li.&sqbul.Joint Photographic Experts Group File Interchange Format (.JPG,.JPEG,.JFIF)
:li.&sqbul.PC Paintbrush File Format (.PCX or .PCC)
:li.&sqbul.Tag Image File Format (.TIF o .TIFF)
:esl.
:p.
Zodra je een bestand geselecteerd hebt, wordt je gewezen om het bestand in je
huidige documentpagina te plaatsen op de zelfde manier als voor
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Plak Bitmap vanuit Klembord:elink.
menu optie.

:h2 res=IDM_INSERT_FUNCTION.Invoegfunctie Menu
Dit menu stelt je in staat om een aantal speciale functiecodes in het document
te plaatsen. De volgende opties zijn op dit moment beschikbaar&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_DATE.Datum:elink.
:li.&sqbul.:link reftype=hd res=IDM_CURRENTPAGE.Huidig paginanummer:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGECOUNT.Totaal Aantal Pagina&apos.s:elink.
:li.&sqbul.:link reftype=hd res=IDM_SEPARATOR.Scheiding:elink.
:li.&sqbul.:link reftype=hd res=IDM_BARCODE.Barcode...:elink.
:esl.
:note.Deze optie is niet beschikbaar in het opmaakmenu voor geselecteerde
tekst.

:h3 res=IDM_DATE.Datum invoegen   (Menu-optie)
Dit sub-menu geeft toegang tot de datuminvoegfunkties. Momenteel zijn er daarvan
drie beschikbaar&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_DATETODAY.Datum van vandaag:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATECURRENT.Huidige datum:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATECHANGED.Document Datum:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATEFMT.Datumformaat...:elink.
:esl.
:note.Deze optie is niet beschikbaar in het opmaakmenu voor geselecteerde
tekst.

:h4 res=IDM_DATETODAY.Datum Vandaag (Menu-optie)
Deze optie, indien geselecteerd, voegt een speciale code toe in het document aan
een object, die altijd de datum aangeeft waarop dit object aan het document is
toegevoegd. Deze datum verandert nooit, net als een object kan het niet
veranderd worden. Wanneer je er met de aanwijzer langs gaat, zal het zich als
een teken of letter gedragen. Het formaat van de datum kan veranderd worden via
het :link reftype=hd res=IDD_DATEFMT.Datumformaat...:elink. dialoog,
echter je moet het formaat van de datum aanpassen op een plaats voor de plek
waar de datum staat, daar het object het formaat gebruikt zoals dat geldt voor
de plaats in het document.
Deze optie is geen onderdeel van het Geselecteerde Tekst Menu.

:h4 res=IDM_DATECURRENT.Huidige datum (Menu-optie)
Deze optie voegt een speciale code toe op de aanwijzerpositie in de tekst.
Dit teken zorgt ervoor dat op deze plaats altijd de huidige datum verschijnt.
M.a.w. bij het tonen of drukken van het document wordt de datum van het tonen of
drukken gebruikt, niet de datum waarop het teken geplaatst werd.  De code
gedraagt zich als een teken of letter en kan niet gewijzigd worden.  Het formaat
dat voor de datum gebruikt wordt kan wel veranderd worden met de
:link reftype=hd res=IDD_DATEFMT.Datumformaat...:elink. dialoog. Daarbij moet je er
wel voor zorgen dat dit op een plaats gebeurt die voor het controleteken ligt.
:note.Deze optie is niet beschikbaar in het opmaakmenu voor geselecteerde
tekst.

:h4 res=IDM_DATECHANGED.Documentdatum (Menu-optie)
Deze functie voegt een speciaal object aan het document toe, dat altijd de datum
aangeeft waarop het document voor het laatst gewijzigd is. Deze datum wordt
gewijzigd wanneer het document wordt bijgewerkt.  Daar het een object is kan het
niet worden bijgewerkt. Wanneer je er met de aanwijzer langs gaat zal het zich
gedragen als een letter of teken. Het formaat waarin de datum wordt getoond zal
die zijn welke geldt voor dat deel van het document en kan gewijzigd worden
vanuit de :link reftype=hd res=IDD_DATEFMT.Datumformaat...:elink. dialoog, hoewel het
formaat geldig moet moet zijn voor de plaats waar het object in het document
geplaatst is.
Deze optie is geen onderdeel van het Geselecteerde Tekst menu.

:h4 res=IDM_DATEFMT.Datumformaat... (Menu-optie)
Deze optie start de :link reftype=hd res=IDD_DATEFMT.Datumformaat...:elink.
dialoog die het mogelijk maakt het formaat dat voor datum gebruikt wordt,te
wijzigen.

:h5 res=IDD_DATEFMT.Datumformaat... (Dialoog)
Met deze dialoog is het mogelijk het formaat van datum te wijzigen.
:artwork name='NdrLands\DateFmt.bmp' align=center.
:p.Daarbij kan elke tekst ingegeven worden.  Enkel de tekens die door een &apos.&percent.&apos.
voorafgegaan worden hebben en speciale betekenis.  De volgende speciale codes
bestaan&colon.
:sl compact.
:li.&percent.D - Deze code wordt vervangen door de dag van de week (zondag, maandag, enz.).
:li.&percent.d - Deze code wordt de afgekorte vorm voor de weekdag (zon, maan, etc.).
:li.&percent.g - Deze code wordt vervangen door de numerieke dag van de maand.
:li.&percent.n - Deze code wordt vervangen door de numerieke maand van het jaar.
:li.&percent.M - Deze code wordt vervangen door de maand van het jaar (januari, februari, enz.).
:li.&percent.m - Dit is de verkorte vorm van de maand (jan, feb, etc.).
:li.&percent.J - Het jaar voluit (4 cijfers).
:li.&percent.j - De verkorte vorm van het jaar (2 cijfers).
:esl.
:p.
Andere tekst in het formaat wordt onveranderd overgenomen.
Het formaat "&percent.D, &percent.g &percent.M, &percent.J" bijvoorbeeld, wordt "woensdag, 16 maart, 1994".
:p.Na het indrukken van de OK-knop wordt het nieuwe formaat opgeslagen. Het
actieve document wordt automatisch aangepast.
:p.Druk je op de DOEN-knop, dan worden de veranderingen onmiddellijk doorgevoerd
zonder de dialoog te verlaten.
:p.Om de dialoog zonder veranderingen te verlaten moet je de Esc-toets of -knop
drukken. Veranderingen t.g.v. de DOEN-knop blijven gelden.

:h3 res=IDM_CURRENTPAGE.Huidige Paginanummer (Menu Optie)
Deze optie voegt een speciaal object toe aan het document, dat altijd het
huidige paginanummer toont op de plaats van het object in het document.

:h3 res=IDM_PAGECOUNT.Total Paginatelling (Menu Optie)
Deze optie voegt een speciaal object toe aan het document, dat steeds het totale
aantal pagina&apos.s van het document toont op de plaats waar het object staat.

:h3 res=IDM_SEPARATOR.Scheiding (Menu-opdracht)
Deze menu-opdracht voegt een speciale horizantale scheidingfunctie
toe aan het document. De scheiding be‰indigt de huidige lijn en 
geeft een scheiding aan op de volgende lijn en gaat vervolgens 
naar de volgende lijn om meer tekst in te voeren.

:h3 res=IDM_BARCODE.Barcode (Menu-optie)
Deze menu-optie kan gebruikt worden om een barcode voor te bereiden die je in een document kunt voegen. Voor meer informatie zie
:link reftype=hd res=IDD_BARCODE.Voeg Barcode In (Dialog):elink.

:h4 res=IDD_BARCODE.Voeg Barcode In (Dialoog)
Deze dialoog geeft je de mogelijkheid om het type en de tekst van een barcode te benoemen die in het document gevoegd kan worden.
:artwork name='NdrLands\BarCode.bmp' align=center.
Je moet een barcode-type van de getoonde lijst kiezen en daarna de tekst invoeren die in barcode omgezet moet worden. Als je klaar bent druk dan op de ENTER-toets of klik op OK, waarna je gevraagd wordt om aan te geven waar de barcode in het document
moet worden geplaatst. Dit op de zelfde manier als in
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Voeg Bitmap in vanuit Klembord:elink.
menu-optie.

:h2 res=IDM_SEARCH.Zoek... (Menu-optie)
Dit menu zal een dialoog oproepen waarin je een reeks tekens kunt inbrengen
waarnaar gezocht dient te worden en een andere tekenreeks die de eerste reeks
kan vervangen als daar om gevraagd wordt.
:p.Zie :link reftype=hd res=IDD_SEARCH.Zoek... (Dialoog):elink. voor meer informatie.
:p.De snelle manier voor deze functie is Ctrl+S.

:h3 res=IDD_SEARCH.Zoek... (Dialoog)
Via dit dialoog is het mogelijk om tekst in te brengen waarnaar gezocht moet
worden met de :link reftype=hd res=IDM_SEARCH.Zoek:elink. en
:link reftype=hd res=IDM_FINDNEXT.Vind Volgende:elink. opdrachten.
Het ook mogelijk om een vervangende tekst in te brengen en het te laten
vervangen.
:artwork name='NdrLands\Search.bmp' align=center.
:p.Om een bepaalde tekst op te zoeken voer je de tekst in het eerste veld
en de Enter-toets in te drukken of op Zoek te klikken. Er wordt naar die tekst
gezocht die daarna oplicht. Het dialoogvenster blijft beschikbaar om een
eventuele nieuwe reeks te accepteren.
:p.Om tekst te vervangen vul je de te vervangen tekst in het eerste veld en het
de vervangende tekst in het tweede veld. Daarna druk je de enter-toets de te
vervangen tekst te vinden of je klikt op Verander om de tekst te vervangen zodra
de reeks gevonden is. Je kunt die Verander actie herhalen voor elk deel van de
tekst die met de gezochte overeen komt.
:p.Om een tekst door een andere te vervangen en dan naar de volgende vindplaats
te gaan, gebruik de Verander &amp. Vind-knop.
:p.Om alle voorkomens van de originele reeks te vervangen door een nieuwe tekst
druk op de Verander Alles knop.
:p.Als je het zoeken gevoelig wilt laten zijn voor grote of kleine letter, dan
kun je op de Gevoeligheidsknop drukken om je gevoeligheid aan te passen.
:p.Als je wilt dat het zoeken de regels van de Unix-expressie volgt, dan kun je de booleaanse expressie-toets merken.
:p.Om de dialoog te stoppen moet je op de Esc-toets drukken of klikken op de
escapeknop. Wat ook de te zoeken of te vervangen tekst ook mag zijn geweest,
ze worden bewaard  tot het volgende zoeken.

:h2 res=IDM_FINDNEXT.Vind Volgende (Menu-optie)
Als je de te zoeken tekst hebt ingegeven via 
:link reftype=hd res=IDD_SEARCH.Zoek... (Dialoog):elink. dialoogvenster, dan zal dit menu je
brengen naar het volgende woord die aan deze reeks voldoet.
:p.Zie :link reftype=hd res=IDD_SEARCH.Zoek...  (Dialoog):elink. voor meer
informatie.
:p.De snelle manier om deze functie te activeren is Ctrl+F.

:h2 res=IDM_GOTOPAGE.Ga naar Pagina (Menu Optie)
Deze menu-optie geeft een dialoog van waaruit je aan kunt geven
welke pagina je wilt zien voor de opmaak.
Zie de :link reftype=hd res=IDD_GOTOPAGE.Ga naar Pagina:elink. dialoog
voor hulp of meer informatie.

:h3 res=IDD_GOTOPAGE.Ga naar Pagina (Dialoog)
Dit dialoog geeft je de mogelijheid om het aan te geven welke
pagina van het document je wilt zien voor opmaak. Geef het
pagina-nummer op in het veld en druk op ENTER-toets of druk op ESC-
toets als je deze functie wilt verlaten

:h2 res=IDM_SPELL.Spellingcontrole (Menu-optie)
Indien de optionele spellingscontrole module in je systeem ge‹nstalleerd is, en
het kan een woordenboek benaderen, dan zal het mogelijk zijn om met menu verder
te werken.
Wanneer je klikt op deze menu-optie, zal de spellingcontrole je document testen
op niet juist gespelde woorden vanaf de plaats waar je nu bent.
Wanneer er zo&apos.n niet juist gespeld woord wordt gevonden, wordt het helder
weergegeven en de :link reftype=hd res=IDD_SPELL.Spellingcontrole:elink.  dialoog
wordt getoond. Het stelt je in staat om correctieve acties te ondernemen.
:p.De snelle manier voor deze optie is F5.

:h3 res=IDD_SPELL.Spellingcontrole (Dialoog)
Deze dialoog wordt getoond wanneer de spellingcontrole een woord heeft gevonden
dat niet goed gespeld is. Het woord wordt getoond samen met mogelijke
vervangers.
:artwork name='NdrLands\Spell.bmp' align=center.
:p.Als je het woord wilt overslaan druk je op Enter of klik je op de
:hp4.Slaover:ehp4. knop met de muis. Dit is de standaard actie.
:p.Als je wilt dat de spellingcontrole het woord voor de rest van het document
negeert, klik op de :hp4.Accepteer:ehp4. knop met de muis.
:p.Om het woord toe te voegen aan je persoonlijke woordenboek met de juiste
hoofd of kleine letter, klik op de :hp4.Voeg toe:ehp4. knop met de muis.
Als het woord alleen in hoofdletter is geschreven, wordt alleen vergeleken met
het woord in hoofdletter.
:p.Om het woord aan je persoonlijke woordenlijst toe te voegen in kleine letter,
klik dan op de :hp4.Voeg kleine letter in:ehp4. knop met de muis.
Zulke woorden zullen gevonden woorden in grote en kleine letter.
Wanneer een woord uit de getoonde lijst moet worden vervangen, druk dan op Enter
of dubbel klik op het woord. Het woord in de tekst zal vervangen worden door het
gekozen woord.
:p.Als je een nieuwe spelling wilt toevoegen aan de voorgestelde lijst van
vervangers, type dan het nieuwe woord in het invoerveld en klik op de
:hp4.Vervang:ehp4. knop. Het originele woord in de tekst zal worden vervangen
door het woord in de nieuwe spelling.
:p.Om de spellingcontrole te verlaten zonder verdere actie, druk op de Esc-
toets of druk op de:hp4.Escape:ehp4. knop met de muis.
Alle wijzigingen, die al gemaakt zijn worden behouden maar kunnen teniet gedaan
worden met de :link reftype=hd res=IDM_UNDO.Tenietdoen:elink. optie.

:h2 res=IDM_ZOOM.Afbeelden (Sub-menu)
Deze opdracht roept een sub-menu met alle beschikbare afbeeld-funkties op.
De volgende mogelijkheden worden ondersteund&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_100.Afbeelden 100&percent.:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLWIDTH.Afbeelden volle breedte:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLHEIGHT.Afbeelden volle hoogte:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_MARGINS.Afbeelden marges:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_ALL.Afbeelden alles:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_WINDOW.Afbeelden venster:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_SET.Afbeelden instellingen...:elink.
:esl.

:h3 res=IDM_ZOOM_100.Afbeelden 100&percent. (Menu-optie)
Deze optie zet de schaal op 100&percent.. Dit betekent dat wat je op
het scherm ziet ook met de zelfde afmetingen zal gedrukt worden.
Tenminste, als ook de schaal van het systeem (buiten de controle van Escriba)
correct ingesteld werd.

:h3 res=IDM_ZOOM_FULLWIDTH.Afbeelden volle breedte (Menu-optie)
Deze optie verandert de schaal zo, dat de breedte van een bladzijde het
volledige venster vult. D.w.z. dat, afhankelijk van de huidige grootte van het
venster, de schaal zowel vergroot als verkleind kan worden.

:h3 res=IDM_ZOOM_FULLHEIGHT.Afbeelden volle hoogte (Menu-optie)
Deze optie verandert de schaal zo, dat de hoogte (lengte) van een bladzijde
het volledige venster vult. Normaal wordt de schaal hiervoor verkleind m.a.w. op
het scherm ziet de tekst er kleiner uit dan op papier.

:h3 res=IDM_ZOOM_MARGINS.Afbeelden marges (Menu-optie)
Deze optie verandert de schaal zo, dat de afstand tussen de verticale
kantlijnen van een bladzijde het volledige venster vult.  Normaal wordt de
schaal hiervoor vergroot m.a.w. op het scherm ziet de tekst er groter uit dan op
papier.

:h3 res=IDM_ZOOM_ALL.Afbeelden alles (Menu-optie)
Deze optie zet de schaal op de kleinste van de Afbeelden volle breedte en
Afbeelden volle hoogte instellingen. Hierdoor wordt een bladzijde,
onafhankelijk van grootte en vorm, altijd volledig getoond.
:p.De verkorte manier voor deze functie is Ctrl-Z, en wanneer het gebruikt wordt
gaat het op en neer tussen Afbeelden Alles en welke andere afbeelding
standaard is ingesteld. Zie in :link reftype=hd res=IDD_PREFERENCES.Algemeen Voorkeur:elink. dialoog.

:h3 res=IDM_ZOOM_WINDOW.Afbeelden venster (Menu-optie)
Deze optie maakt het mogelijk een rechthoekig gedeelte van een document te
selecteren waarna de schaal aangepast wordt zodat dit gedeelte zo groot mogelijk
in het venster weergegeven wordt.

:h3 res=IDM_ZOOM_SET.Afbeelden Instelling... (Menu-optie)
Deze optie begint een dialoog, waarmee het mogelijk is om de huidige
afbeelding te aan te passen.
:p.Zie :link reftype=hd res=IDD_ZOOM_SET.Afbeelden Instelling... (Dialoog):elink. voor meer informatie.

:h4 res=IDD_ZOOM_SET.Afbeelden Instelling... (Dialoog)
Deze dialoog geeft de gebruiker de mogelijkheid om elke gewenste waarde in te
stellen. Gebruik de op en  neer pijlen om de gewenste waarde in te stellen, of
type de waarde direkt in. Druk daarna op de Enter-toets of klik op de OK knop om
de nieuwe waarde uit te voeren.
:artwork name='NdrLands\ZoomSet.bmp' align=center.
:p.Om de nieuwe schaal toe te passen zonder de dialoog te verlaten, moet je de
DOEN-knop indrukken. Veranderingen die op deze manier gemaakt worden blijven
geldig ook als dialoog met de Esc-toets afgesloten wordt.
:p.Om de dialoog zonder veranderingen te verlaten moet je de Esc-toets of knop
indrukken.

:h1 res=IDM_LAYOUT_MENU.Opmaakmenu
:i1.Opmaakmenu
In het opmaakmenu kun je uit de volgende opdrachten kiezen&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_MARGINS.Marges...:elink.
:li.&sqbul.:link reftype=hd res=IDM_TABS.Tabs...:elink.
:li.&sqbul.:link reftype=hd res=IDM_INDENT.Inspringen...:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINESPACING_MENU.Lijnafstand:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUSTIFICATION.Uitlijning:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGECENTER.Bladmidden:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGEBREAK.Bladwissel:elink.
:li.&sqbul.:link reftype=hd res=IDM_HEADER.Kop...:elink.
:li.&sqbul.:link reftype=hd res=IDM_FOOTER.Voet...:elink.
:esl.

:h2 res=IDM_MARGINS.Marges... (Menu-optie)
Deze optie start een nieuwe dialoog waarin het mogelijk is de vier
verschillende marges (boven, onder, links en rechts) in te stellen.
:p.Zie :link reftype=hd res=IDD_MARGINS.Marges (Dialoog):elink. voor meer
informatie.

:h3 res=IDD_MARGINS.Marges (Dialoog)
In deze dialoog kunnen de linker, rechter, bovenste en onderste marges van
het document gewijzigd worden.  Deze marges worden weergegeven in het
geldende meetsysteem (Engels of metrisch), en zijn de afstanden van de kanten
tot de letters.
:artwork name='NdrLands\Margins.bmp' align=center.
:p.Terwijl je nieuwe posities (afstanden) voor de marges ingeeft worden de
horizontale en verticale meetlatten van het actieve document automatisch
aangepast. Het document zelf wordt echter niet opnieuw geformatteerd.
:p.Verander de afstanden en druk op de Enter-toets of de OK-knop. Passen de
nieuwe posities niet binnen de huidige bladspiegel dan krijg je een foutmelding
en kun je opnieuw proberen.
:p.Druk je op de DOEN-knop dan worden de instellingen direct geldig voor het
actieve document.
:p.Om de dialoog zonder veranderingen te verlaten moet je de Esc-toets of knop
drukken. Wijzigingen t.g.v. het drukken van de DOEN-knop blijven geldig.
:note.De nieuwe linker en rechter marges zijn pas geldig vanaf de volgende
lijn, behalve wanneer de opdracht aan het begin van een lijn ingegeven wordt.
De nieuwe bovenste en onderste marges zijn pas geldig vanaf de volgende
bladzijde, behalve wanneer de opdracht helemaal bovenaan een bladzijde ingegeven
wordt.

:h2 res=IDM_TABS.Tabs... (Menu-optie)
Deze optie start een nieuwe dialoog waarin het mogelijk is de
tab-instellingen voor een document aan te passen. De nieuwe instellingen
gelden vanaf de huidige aanwijzerpositie.
:p.Zie :link reftype=hd res=IDD_TABS.Tabs (Dialoog):elink. voor meer
informatie.

:h3 res=IDD_TABS.Tabs (Dialoog)
Deze dialoog maakt het mogelijk de positie van een tab te wijzigen of
nieuwe tab toe te voegen resp. bestaande te wissen.
Veranderingen zijn onmiddellijk zichtbaar op de horizontale meetlat van het
actieve venster.
:artwork name='NdrLands\Tabs.bmp' align=center.
:p.Om een nieuwe tab te zetten moet je zijn positie in het betreffende
veld ingeven en de ZET-knop indrukken.
:p.Om een bestaande tab te wissen moet je zijn positie in het betreffende
veld ingeven en de WIS-knop indrukken.
:p.Als alle veranderingen ingegeven zijn, moet de Enter-toets of OK-knop
ingedrukt worden om ze effectief te maken.  De nieuwe instellingen zijn
vanaf de huidige aanwijzerpositie geldig.
:p.Druk je op de DOEN-knop, dan worden de tabs direct voor het actieve
document geldig en vormen ook de nieuwe standaardinstelling.
:p.Om de dialoog zonder veranderingen te verlaten moet je de Esc-toets of knop
indrukken.  Wijzigingen t.g.v. de DOEN-knop blijven geldig.

:h2 res=IDM_INDENT.Inspringen... (Menu-optie)
Deze optie start een dialoog waarin een nieuwe inspringafstand, die gebruikt
wordt aan het begin van elke nieuwe paragraaf, kan ingegeven worden.
:p.Zie :link reftype=hd res=IDD_INDENT.Paragraafinspringen (Dialoog):elink. voor meer
informatie.

:h3 res=IDD_INDENT.Paragraafinspringen (Dialoog)
In deze dialoog kan een nieuwe inspringafstand, die gebruikt wordt aan het begin
van elke nieuwe paragraaf, ingegeven worden. De nieuwe instelling is geldig
vanaf de huidige aanwijzerpositie in het document.
:artwork name='NdrLands\Indent.bmp' align=center.
:p.Geef een nieuwe afstand in en druk de OK-knop in. Het formaat van het
document wordt automatisch aangepast.
:p.Om de waarde te veranderen zonder de dialoog te verlaten moet je de DOEN-knop
drukken. Het document wordt onmiddellijk aangepast.
:p.Om de dialoog zonder veranderingen te verlaten moet je de Esc-toets of knop
indrukken.  Wijzigingen t.g.v. de DOEN-knop blijven geldig.

:h2 res=IDM_LINESPACING_MENU.Regelafstand Menu
Dit submenu geeft de verschillende vertikale regelafstanden die beschikbaar
zijn.
p.De regelafstand die op dit moment geldt is aangegeven.
p.Zie :link reftype=hd res=IDM_LINESPACINGS.Regelafstand Menulijst:elink. voor meer
informatie.

:h3 res=IDM_LINESPACINGS.Regelafstand Menulijst
Dit menu geeft de regelafstand-mogelijkheden die op dit moment beschikbaar zijn.
De regelafstand wordt weergegeven als percentage van de normale regelafstand
voor het geldige font en puntmaat. Als een een de regelafstand-mogelijkheden
wordt gekozen, dan geldt dit als een percentage van de normale regelafstand voor
het gedefinieerde blok tekst of als er geen tekst is aangegeven, vanaf de
positie van de aanwijzer.

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

:h2 res=IDM_JUSTIFICATION.Uitlijning (Menu-optie)
Deze optie roept een sub-menu op met de verschillende keuzemogelijkheden voor
tekstuitlijning.  Er kan gekozen worden tussen&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_JUST_LEFT.Links:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_CENTER.Gecentreerd:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_RIGHT.Rechts:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_FULL.Volledig:elink.
:esl.

:h3 res=IDM_JUST_LEFT.Links (Menu-optie)
Deze optie plaatst een controleteken in de tekst waardoor deze, beginnend
vanaf de volgende lijn, links uitgelijnd wordt. Wordt het teken aan het begin
van een lijn ingegeven dan wordt de instelling op de huidige lijn zelf reeds
actief.

:h3 res=IDM_JUST_CENTER.Gecentreerd (Menu-optie)
Deze optie plaatst een controleteken in de tekst waardoor deze, beginnend
vanaf de volgende lijn, tussen de kantlijnen gecentreerd wordt. Wordt het teken
aan het begin van een lijn ingegeven dan wordt de instelling op de huidige lijn
zelf reeds actief.

:h3 res=IDM_JUST_RIGHT.Rechts (Menu-optie)
Deze optie plaatst een controleteken in de tekst waardoor deze, beginnend
vanaf de volgende lijn, rechts uitgelijnd wordt. Wordt het teken aan het begin
van een lijn ingegeven dan wordt de instelling op de huidige lijn zelf reeds
actief.

:h3 res=IDM_JUST_FULL.Volledig (Menu-optie)
Deze optie plaatst een controleteken in de tekst waardoor deze, beginnende
vanaf de volgende lijn, volledig uitgelijnd wordt. Wordt het teken aan het begin
van een lijn ingegeven dan wordt de instelling op de huidige lijn zelf reeds
actief.  De afmetingen van de spaties in volledig uitgelijnde tekst worden
automatisch aangepast zodat beide randen recht zijn.  Lijnen die eindigen op een
&apos.wagenterugloop&apos. (Carriage Return) of bladwissel, worden links uitgelijnd.

:h2 res=IDM_PAGECENTER.Bladmidden (Menu-optie)
Deze optie plaatst een controleteken in de tekst waardoor deze, in vertikale
richting, in het midden van de bladzijde geplaatst wordt.
:note.Deze instelling kan niet voor bladzijden die afbeeldingen bevatten
gebruikt worden en is in dit geval ook niet beschikbaar.

:h2 res=IDM_PAGEBREAK.Bladwissel (Menu-optie)
Deze optie voegt een gedwongen bladwissel in op de huidige aanwijzerpositie.
De aanwijzer zelf wordt automatisch aan het begin van de volgende bladzijde
geplaatst.
:p.Deze opdracht kan ook d.m.v. de CTRL+ENTER toetsencombinatie gestart worden.
:note.Deze opdracht is niet beschikbaar in het opmaakmenu voor geselecteerde
tekst.

:h2 res=IDM_HEADER.Kop... (Menu-optie)
Dit nemu geeft een dialoog waarmee je de kop van een pagina kunt bepalen
vanaf de plaats waar de aanwijzer staat.

:h3 res=IDD_HEADER.Kop (Dialoog)
Dit dialoog stelt je in staat om een nieuwe kop van een pagina te bepalen.
Nu moet de kop gedefinieerd worden in het basis Escriba-tekst-formaat.
Later zal de definitie zichtbaar worden als WYSIWYG.
De meeste functie-codes (zoals PageNumber, PageCount, LeftJustify, etc)
zijn geldig voor het formaat van de kop, maar niet alle (zoals de
PrinterSettings).

:h2 res=IDM_FOOTER.Voet... (Menu-optie)
Dit menu geeft het dialoog waarmee je het uiterlijk van de voet van een
document kunt bepalen vanaf de plaats waar de aanwijzer staat.

:h3 res=IDD_FOOTER.Voet (Dialoog)
Dit dialoog stelt je in staat de voet van een nieuwe pagina te bepalen.
Nu moet de voet bepaald worden in het basis Escriba-tekst-formaat.
Later zal die zichtbaar worden als WYSIWYG.
De meeste functie-codes (zoals PageNumber, PageCount, LeftJustify, etc)
zijn geldig voor het formaat van de voet, maar niet alle (zoals de
PrinterSettings).

:h1 res=IDM_FONT_MENU.Lettertypemenu
:i1.Lettertypemenu
Dit menu geeft een lijst van alle lettertypes die beschikbaar zijn.
:p.Het lettertype die op dit moment geldt, is gemerkt.
:p.Zie :link reftype=hd res=IDM_FONTS.Lettertype Menulijst:elink. voor meer informatie.

:h2 res=IDM_FONTS.Lettertype Menulijst
Dit menu geeft een van de beschikbare typen weer zoals het zou zijn als het in
het document stond met een maat van 18 punten. Als je dit type wilt laten gelden
voor een vooraf gemerkte tekst of vanaf de huidige aanwijzerpositie, kies de
font met de pijltoetsen en druk op Enter of klik met de muis op je keuze.

:h2 res=IDM_FONTS_PGUP.Lettertype Menulijst Vooruit
Deze optie is in het menu beschikbaar als het gehele typenmenu niet in zijn
geheel kan worden getoond en er meer typen beschikbaar zijn dan er op dit moment
getoond kunnen worden. Om het typenmenu vooruit te doen gaan, maak een keuze met
de pijltoetsen en druk op Enter of klik er met de muis op.

:h2 res=IDM_FONTS_PGDN.Lettertype Menulijst Achteruit
Deze optie is in het menu beschikbaar als het gehele typemenu niet in zijn
geheel kan worden getoond en er meer typen beschikbaar zijn dan er op dit moment
getoond kunnen worden. Om het typemenu achteruit te doen gaan, maak een keuze
met de pijltoetsen en druk op Enter of klik er met de muis op.

:h2 res=IDM_FONTMGR.Lettertypen Menumanager
Deze functie is nog niet beschikbaar.

:h1 res=IDM_SIZE_MENU.Lettergroottemenu
:i1.Lettergroottemenu
Dit menu geeft een lijst van alle mogelijke lettergrootten.
:p.De lettergrootte die op dit moment geldt is gemerkt.
:p.Zie :link reftype=hd res=IDM_SIZES.Lettergrootte Menulijst :elink. voor meer
informatie.

:h2 res=IDM_SIZES.Lettergrootte Menulijst
Dit menu geeft een van de beschikbaar lettergrootten die beschikbaar zijn.
Als je de grootte aan wil passen voor vooraf geselecteerde tekst, of voor de
tekst die na de aanwijzer wordt ingebracht, maak een keus met de pijltoetsen en
druk op de Enter-toets of klik er op met de muis.

:h2 res=IDM_SIZES_PGUP.Lettergrootte Menulijst Vooruit
Deze optie is beschikbaar als niet het hele menu zichtbaar gemaakt kan worden,
en er meer grootten beschikbaar zijn dan er getoond kunnen worden. Om het menu
vooruit te bewegen, maak een keuze met de pijltoetsen of klik er op met de muis.

:h2 res=IDM_SIZES_PGDN.Lettergrootte Menu Omlaag
Deze optie is beschikbaar als niet het hele menu zichtbaar gemaakt kan worden,
en er meer grootten beschikbaar zijn dan er getoond kunnen worden. Om het menu
achteruit te bewegen, maak een keuze met de pijltoetsen of klik er op met de
muis.

:h2 res=IDM_SIZEMGR.Lettergrootte Menumanager
Deze functie is nog niet beschikbaar.

:h1 res=IDM_ATTR_MENU.Kenmerkenmenu
:i1.Kenmerkenmenu
In dit menu is het mogelijk letterkenmerken op het document toe te passen.
Alle, door de Presentation Manager ondersteunde, kenmerken zijn beschikbaar :
normaal, vet, schuin, onderlijnd, doorstreept en hol.  Het kiezen van een van
de mogelijkheden zorgt ervoor dat alle tekst vanaf de aanwijzerpositie of alle
geselecteerde tekst de nieuwe kenmerken krijgt.
De getoonde voorbeelden gebruiken het op de aanwijzerpositie geldige lettertype
in de correcte afmetingen. De kenmerken die op de huidige positie geldig zijn
worden in het menu aangegeven.
Elke combinatie van kenmerken, uitgezonderd normaal, kan samengesteld worden.
Normaal betekent gewoonweg dat geen van de andere kenmerken actief is.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_NORMAL.Normaal:elink.
:li.&sqbul.:link reftype=hd res=IDM_BOLD.Vet:elink.
:li.&sqbul.:link reftype=hd res=IDM_ITALIC.Schuin:elink.
:li.&sqbul.:link reftype=hd res=IDM_UNDERSCORE.Onderlijnd:elink.
:li.&sqbul.:link reftype=hd res=IDM_STRIKEOUT.Doorstreept:elink.
:li.&sqbul.:link reftype=hd res=IDM_OUTLINE.Hol:elink.
:li.&sqbul.:link reftype=hd res=IDM_CAPS.Hoofdletters:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXPAND_MENU.Uitbreidings Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINECOLOR_MENU.Lijnkleur Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEWIDTH_MENU.Lijnbreedte Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILLCOLOR_MENU.Vulkleur Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILLTYPE_MENU.Vultype Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEEND_MENU.Eindlijn Einde Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEJOIN_MENU.Voeglijn Voeg Menu:elink.
:esl.

:h2 res=IDM_NORMAL.Normaal (Menu-optie)
Deze optie heft alle actieve letterkenmerken (vet, schuin, ...) op.  Wordt
de optie gestart terwijl er tekst geselecteerd is, dan heeft het enkel
invloed op dit gedeelte. Anders zorgt het ervoor dat alle verdere tekst, tot aan
het volgende kenmerk-controleteken, veranderd wordt.
:p.Deze optie kan ook d.m.v. de Ctrl+N toetsencombinatie gestart worden..

:h2 res=IDM_BOLD.Vet (Menu-optie)
Deze optie zet het Vet kenmerk voor alle geselecteerde tekst. Is er geen
tekst geselecteerd dan zorgt het ervoor dat alle verdere tekst, tot aan het
volgende kenmerk-controleteken, veranderd wordt.
:p.Deze optie kan ook d.m.v. de Ctrl+V toetsencombinatie gestart worden..

:h2 res=IDM_ITALIC.Schuin (Menu-optie)
Deze optie zet het Schuin kenmerk voor alle geselecteerde tekst.  Is er geen tekst
geselecteerd dan zorgt het ervoor dat alle verdere tekst, tot aan het
volgende kenmerk-controleteken, veranderd wordt.
:p.Deze optie kan ook d.m.v. de Ctrl+C toetsencombinatie gestart worden..

:h2 res=IDM_UNDERSCORE.Onderlijnd (Menu-optie)
Deze optie zet het Onderlijnd kenmerk voor alle geselecteerde tekst.  Is er
geen tekst geselecteerd dan zorgt het ervoor dat alle verdere tekst, tot aan het
volgende kenmerk-controleteken, veranderd wordt.
:p.Deze optie kan ook d.m.v. de Ctrl+L toetsencombinatie gestart worden..

:h2 res=IDM_STRIKEOUT.Doorstreept (Menu-optie)
Deze optie zet het Doorstreept kenmerk voor alle geselecteerde tekst.  Is er
geen tekst geselecteerd dan zorgt het ervoor dat alle verdere tekst, tot aan het
volgende kenmerk-controleteken, veranderd wordt.

:h2 res=IDM_OUTLINE.Hol (Menu-optie)
Deze optie zet het Hol kenmerk voor alle geselecteerde tekst. Is er geen
tekst geselecteerd dan zorgt het ervoor dat alle verdere tekst, tot aan het
volgende kenmerk-controleteken, veranderd wordt.

:h2 res=IDM_CAPS.Hoofdletters (Menu-optie)
Deze optie verandert alle geselecteerde tekst in hoofdletters. Is er geen
tekst geselecteerd dan zorgt het ervoor dat alle verdere tekst, tot aan het
volgende kenmerk-controleteken, veranderd wordt.

:h2 res=IDM_EXPAND_MENU.Expandeer Menu
Dit menu geeft een overzicht van alle beschikbare horizontale tekst-expansie
factoren.
:p.De expansie-factor die op dit moment bij de aanwijzer geldt is aangegeven.
:p.Zie :link reftype=hd res=IDM_EXPANDS.Expandeer Menulijst:elink. voor meer
informatie.

:h3 res=IDM_EXPANDS.Expandeer Menulijst
Dit menu geeft een van de beschikbare expansiefactoren weer, terwijl het tekst
in het geldende lettertype weergeeft in de verschillende expansiefactoren.
Om een expansiefactor toe te kennen aan een geselecteerde tekst of voor de tekst
na de aanwijzer, kies de verlangde expansiefactor met de pijltoetsen en toets op
Enter of klik met de muis op de verlangde factor.

:h3 res=IDM_EXPANDS_PGUP.Expandeer Menu Vooruit
Deze menu-optie wordt getoond als niet het gehele expandeermenu kan worden
getoond, en er meer keuzes beschikbaar zijn. Om het menu vooruit te bewegen,
maak een keuze met de pijltoetsen en toets op Enter of klik er met de muis op.

:h3 res=IDM_EXPANDS_PGDN.Expandeer Menu Achteruit
Deze menu-optie wordt getoond als niet het gehele expandeermenu kan worden
getoond, en er meer keuzes beschikbaar zijn. Om het menu achteruit te bewegen,
maak een keuze met de pijltoetsen en toets op Enter of klik er met de muis op.

:h3 res=IDM_EXPANDMGR.Expandeer Menumanager
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

:h2 res=IDM_LINECOLOR_MENU.Lijn/Tekst-kleur Menu
Dit menu geeft een overzicht van alle beschikbare lijn/tekst- kleuren.
:p.De kleur die op dit moment geldt is gemerkt.
:p.Zie :link reftype=hd res=IDM_LINECOLORS.Lijn/Tekstkleur Menulijst :elink. voor meer
informatie.

:h3 res=IDM_LINECOLORS.Lijn/Tekst-kleur Menulijst
Dit menu geeft de beschikbare lijn/tekst-kleuren weer.
Om een lijn/tekst-kleur te kiezen voor een gemarkeerde tekst of voor de tekst na
de aanwijzer, maak een keuze met de pijltoetsen en druk op Enter of klik met de
muis op de te kiezen kleur.
:note.Deze kleuren gelden voor lijnen, tekst en voor de lijnen van open tekst.

:h3 res=IDM_LINECOLORS_PGUP.Lijn/Tekst-kleur Menu Vooruit
Deze optie is beschikbaar als niet het gehele lijn/tekst-kleur menu getoond kan
worden, en als er meer lijn/tekst-kleur mogelijkheden zijn dan er in het menu
kunnen. Om het menu vooruit te bewegen, maak een keuze in het menu met de
pijltoetsen en druk op de Enter-toets of klik er met de muis op.

:h3 res=IDM_LINECOLORS_PGDN.Lijn/Tekst-kleur Menu Achteruit
Deze optie is beschikbaar als niet het gehele lijn/tekst-kleur menu getoond kan
worden, en als er meer lijn/tekst-kleur mogelijkheden zijn dan er in het menu
kunnen. Om het menu achteruit te bewegen, maak een keuze in het menu met de
pijltoetsen en druk op de Enter-toets of klik er met de muis op.

:h3 res=IDM_LINECOLORMGR.Lijn/Tekst-kleur Menumanager
Dit menu roept de 
:link reftype=hd res=IDD_LINECOLORMGR.Lijn/Tekst-kleur Menumanager:elink.
dialoog op, dat de gebruiker de mogelijkheid geeft om de lijn/tekstkleurmenu aan te passen.

:h4 res=IDD_LINECOLORMGR.Lijn/Tekst-kleur Menumanager (Dialog)
Deze dialoog maakt het voor de gebruiker mogelijk om de Lijn/Tekst-kleurmenu aan te
passen. Een kleurwiel en een intensiteits-instelling geven een gemakkelijke manier 
om de kleuren in te stellen en voor een fijninstelling zijn er instellingsmogelijkheden voor rood, groen en blauw. De beschikbare kleuren zijn in een afrollijst geplaatst.:p.
Om een kleur toe te voegen kan een kleur vinden met het kleurwiel of door de rood,groen en blauwe kleurinstellingen te gebruiken en dan te klikken op de
&apos.>>&apos. Knop. Als de kleur nog niet bestaat wordt die toegeveogd aan het menu.:p.  
Om een kleur uit de lijst te verwijderen wordt dit in de lijst gemerkt en op de
&apos.<<&apos. knop.:p.
Om het menu in te stellen op zijn basiskleuren klik dan op de basis knop.:p.
Om terug te gaan zonder het menu aan te passen, druk dan op de escapetoets of druk
op de escape-knop.

:h2 res=IDM_LINEWIDTH_MENU.Lijndikte Menu
Dit menu geeft alle beschikbare lijndiktes weer.
p.De lijndikte die op dit moment geldt, is gemarkeerd.
:p.Zie :link reftype=hd res=IDM_LINEWIDTHS.Lijndikte Menulijst :elink. voor meer
informatie.

:h3 res=IDM_LINEWIDTHS.Line Lijndikte Menulijst
Dit menu geeft de beschikbare lijndiktes weer. Om een lijndikte in te stellen
voor gemarkeerde tekst of vanaf de huidige positie van de aanwijzer, maak een
keuze met de pijltoetsen en druk op de Enter-toets of klik er met de muis op.

:h3 res=IDM_LINEWIDTHS_PGUP.Lijndikte Menu Vooruit
Deze optie is beschikbaar als niet het gehele lijndikte-menu getoond kan worden,
en als er meer lijndikte-mogelijkheden zijn dan er in het menu kunnen.
Om het menu vooruit te bewegen, maak een keuze in het menu met de pijltoetsen en
druk op de Enter-toets of klik er met de muis op.

:h3 res=IDM_LINEWIDTHS_PGDN.Lijndikte Menu Achteruit
Deze optie is beschikbaar als niet het gehele lijndikte-menu getoond kan worden,
en er meer lijndikte-mogelijkheden zijn dan er in het menu kunnen.  Om het menu
achteruit te bewegen, maak een keuze in het menu met de pijltoetsen en druk op
de Enter-toets of klik er met de muis op.

:h3 res=IDM_LINEWIDTHMGR.Lijndikte Menumanager
Deze functie is nog niet beschikbaar.

:h2 res=IDM_FILLCOLOR_MENU.Vulkleur Menu
Dit menu geeft alle vulkleuren weer die gebruikt kunnen worden om open tekst te
vullen.
:p.De kleur die op dit moment geldt is gemerkt.
:p.Zie :link reftype=hd res=IDM_FILLCOLORS.Vulkleur Menulijst :elink. voor meer
informatie.

:h3 res=IDM_FILLCOLORS.Vulkleur Menulijst
Dit menu geeft de beschikbare vulkleuren weer voor gebruik in open tekst.
Om een vulkleur te kiezen voor vooraf gemerkte tekst of vanaf de aanwijzer, maak
een keuze met de pijltoetsen en druk op Enter of klik er op met de muis.

:h3 res=IDM_FILLCOLORS_PGUP.Vulkleur Menu Vooruit
Deze optie is beschikbaar als niet het gehele vulkleur-menu getoond kan worden,
en als er meer vulkleur-mogelijkheden zijn dan er in het menu kunnen.  Om het
menu vooruit te bewegen, maak een keuze in het menu met de pijltoetsen en druk
op de Enter-toets of klik er met de muis op.

:h3 res=IDM_FILLCOLORS_PGDN.Vulkleur Menu Achteruit.
Deze optie is beschikbaar als niet het gehele vulkleur-menu getoond kan worden,
en als er meer vulkleur-mogelijkheden zijn dan er in het menu kunnen.  Om het
menu achteruit te bewegen, maak een keuze in het menu met de pijltoetsen en druk
op de Enter-toets of klik er met de muis op.

:h3 res=IDM_FILLCOLORMGR.Vulkleur Menumanager
Dit menu roeps de 
:link reftype=hd res=IDD_FILLCOLORMGR.Vulkleur Menumanager:elink.
dialoog op, dit geeft de gebruiker de mogelijkheid om de Vulkleurmenu aan te passen.

:h4 res=IDD_FILLCOLORMGR.Vulkleur Menumanager (Dialog)
Deze dialoog maakt het voor de gebruiker mogelijk om de Vulkleurmenu aan te
passen. Een kleurwiel en een intensiteits-instelling geven een gemakkelijke manier 
om de kleuren vast te stellen en voor een fijninstelling zijn er instellingsmogelijkheden voor rood, groen en blauw. De beschikbare kleuren zijn in een afrollijst geplaatst.:p.
Om een kleur toe te voegen kan een kleur vinden met het kleurwiel of door de rood,groen en blauwe kleurinstellingen te gebruiken en dan de klikken op de
&apos.>>&apos. Knop. Als de kleur nog niet bestaat wordt die toegevoegd aan het menu.:p.  
Om een kleur uit de lijst te verwijderen wordt dit in de lijst gemerkt en op de
&apos.<<&apos. knop.:p.
Om het menu in te stellen op zijn basiskleuren klik dan op de basis-knop.:p.
Om terug te gaan zonder het menu aan te passen, druk dan op de escapetoets of druk
op de escape-knop.

:h2 res=IDM_FILLTYPE_MENU.Vultypen
Dit menu geeft alle vulkleuren weer die gebruikt kunnen worden om open tekst te
vullen.
:p.De kleur die op dit moment geldt is gemerkt.
:p.Zie :link reftype=hd res=IDM_FILLTYPES.Vultypen Menulijst :elink. voor meer
informatie.

:h3 res=IDM_FILLTYPES.Vultypen Menulijst
Dit menu geeft de beschikbare vultypen weer voor gebruik in open tekst.
Om een Vultypen te kiezen voor vooraf gemerkte tekst of vanaf de aanwijzer, maak
een keuze met de pijltoetsen en druk op Enter of klik er op met de muis.

:h3 res=IDM_FILLTYPES_PGUP.Vultypen Menu Vooruit
Deze optie is beschikbaar als niet het gehele Vultypen-menu getoond kan worden,
en als er meer vultypen-mogelijkheden zijn dan er in het menu kunnen.  Om het
menu vooruit te bewegen, maak een keuze in het menu met de pijltoetsen en druk
op de Enter-toets of klik er met de muis op.

:h3 res=IDM_FILLTYPES_PGDN.Vultypen Menu Achteruit.
Deze optie is beschikbaar als niet het gehele Vultypen-menu getoond kan worden,
en als er meer vultypen-mogelijkheden zijn dan er in het menu kunnen.  Om het
menu achteruit te bewegen, maak een keuze in het menu met de pijltoetsen en druk
op de Enter-toets of klik er met de muis op.

:h3 res=IDM_FILLTYPEMGR.Vultypen Menumanager
Deze functie is nog niet beschikbaar.

:h2 res=IDM_LINEEND_MENU.Lijneinde Menu
Dit menu geeft een overzicht van de  beschikbare lijneinde typen.
:p. Het type dat op dit moment geldig is bij de aanwijzer is gemerkt.
:p. Zie :link reftype=hd res=IDM_LINEENDS.Lijn Einde Menu Typen:elink.
voor meer informatie.

:h3 res=IDM_LINEENDS.Lijneinde Menu
Dit menu geeft de lijneinde typen die op dit moment beschikbaar zijn.
Om een lijneinde type aan te passen maak een selectie uit de getoonde
mogelijkheden door met de pijltoetsen op en neer te bewegen en met
op ENTER te drukken of er met de muis op te klikken.

:h3 res=IDM_LINEENDS_PGUP.Lijneinde Menu Vooruit
Deze optie is beschikbaar als niet alle Lijneinde-typen kunnen worden
afgebeeld. Rol het lijneinde menu op en neer om een keuze te kunnen maken.
Druk op ENTER of klik er met de muis op om de keuze te maken.

:h3 res=IDM_LINEENDS_PGDN.Lijneinde Menu Achteruit.
Deze optie is beschikbaar als niet alle Lijneinde-typen kunnen worden
afgebeeld. Rol het lijneinde-menu op en neer om een keuze te kunnen maken.
Druk op ENTER of klik er met de muis op om de keuze te maken.

:h3 res=IDM_LINEENDMGR.Lijneinde Menu Manager
Deze functie is nog niet beschikbaar.

:h2 res=IDM_LINEJOIN_MENU.Lijnvoeg Menu Keuzes
Dit menu toont alle methoden waarop lijnen aan elkaar gevoegd kunnen
worden.
:p.Het type dat op dit moment geldig is, is gemerkt.
:p.Zie :link reftype=hd res=IDM_LINEJOINS.Lijn Voeg Menu Keuzes:elink.
voor meer informatie..

:h3 res=IDM_LINEJOINS.Lijnvoeg Menu Mogelijkheden
Dit menu geeft de beschikbare lijnvoeg-types voor gebruik in holle tekst.
Om een lijnvoeg-type in te stellen beweeg de aanwijzer naar beneden of
pas de lijnvoeg-type toe op de geselecteerde tekst, kies een lijnvoeg-type met
de pijltoetsen en druk op de Enter-toets, of klik op uw keuze met de muis.

:h3 res=IDM_LINEJOINS_PGUP.Lijnvoeg Menu Vooruit
Deze functie is beschikbaar indien het aantal lijnvoegmogelijkheden groter
is dan in het venster kunnen worden afgebeeld. Rol het menu op en neer om je
keuze zichtbaar te maken. Klik er met de muis op of druk op de ENTER toets.

:h3 res=IDM_LINEJOINS_PGDN.Lijnvoeg Menu Achteruit
Deze functie is beschikbaar indien het aantal lijnvoegmogelijkheden groter
is dan in het venster kunnen worden afgebeeld. Rol het menu op en neer om je
keuze zichtbaar te maken. Klik er met de muis op of druk op de ENTER toets.

:h3 res=IDM_LINEJOINMGR.Lijnvoeg Menu Manager
Deze functie is nog niet beschikbaar.

:h1 res=IDM_WINDOW_MENU.Venstermenu
:i1.Venstermenu
Het venstermenu geeft je de keuze uit verschillende manipulaties die met
geopende vensters uitgevoerd kunnen worden.  De volgende mogelijkheden zijn
beschikbaar&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_TILE.Tegelen:elink.
:li.&sqbul.:link reftype=hd res=IDM_CASCADE.Stapelen:elink.
:li.&sqbul.:link reftype=hd res=IDM_ARRANGEICONS.Ikonen schikken:elink.
:li.&sqbul.:link reftype=hd res=IDM_WINDOWS.Actieve Vensters:elink.
:esl.

:h2 res=IDM_TILE.Tegelen (Menu-optie)
Deze optie herschikt alle vensters, die niet als ikonen voorgesteld worden,
zodat ze volledig zichtbaar zijn. Ook de ikonen aan de onderkant van het
hoofdvenster blijven zichtbaar. Bij het schikken worden zowel de positie als de
afmetingen van de vensters aangepast.

:h2 res=IDM_CASCADE.Stapelen (Menu-optie)
Deze optie herschikt alle vensters, die niet als ikonen voorgesteld worden,
zodat ze zo groot mogelijk zijn en alle titels zichtbaar blijven. Ook de ikonen
aan de onderkant van het hoofdvenster blijven zichtbaar. Bij het schikken worden
zowel de positie als de afmetingen van de vensters aangepast.

:h2 res=IDM_ARRANGEICONS.Ikonen schikken (Menu-optie)
Deze optie zorgt ervoor dat alle ikonen linksonder in het hoofdvenster
geschikt worden. Deze functie is ook een onderdeel van de Tegelen en Stapelen
opdrachten.

:h2 res=IDM_WINDOWS.Actieve Vensters
Dit deel van het venstermenu geeft een lijst van alle open document-vensters.
Om een document te kiezen, waardoor het vooraan de open documentvensters komt te
staan, maak een keuze met de pijltoetsen en druk op Enter of klik er met de muis
op.

:h1 res=IDM_HELP_MENU.Hulpmenu
:i1.Hulpmenu
Het hulpmenu houdt zich aan de geldende SAA-normen en omvat de volgende
keuzes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_HELP_INDEX.Hulpindex:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXTENDED_HELP.Algemene hulp:elink.
:li.&sqbul.:link reftype=hd res=IDM_HELP_FOR_HELP.Hulp gebruiken:elink.
:li.&sqbul.:link reftype=hd res=IDM_KEYS_HELP.Hulp voor toetsen:elink.
:li.&sqbul.:link reftype=hd res=IDM_ABOUT_MENU.Product-informatie Menu:elink.
:esl.

:h2 res=IDM_HELP_INDEX.Hulpindex (Menu-optie)
Het kiezen van deze optie toont de index van de Escriba hulpbibliotheek.

:h2 res=IDM_EXTENDED_HELP.Algemene hulp (Menu-optie)
Het kiezen van deze optie toont algemene informatie over de tekstverwerker.

:h2 res=IDM_HELP_FOR_HELP.Hulp gebruiken (Menu-optie)
Het kiezen van deze optie geeft een algemene uitleg over het gebruik van de
hulpmogelijkheden. Deze informatie is niet specifiek voor dit programma.

:h2 res=IDM_KEYS_HELP.Hulp voor toetsen (Menu-optie)
De volgende toetsencombinaties hebben in Escriba een speciale betekenis. Hierbij
werd zoveel mogelijk geprobeerd de SAA CUA normen, zoals ze door mij begrepen
worden, te volgen.
:table cols='20 60' rules=both frame=box.
:row.:c.F1              :c.Toon hulpinformatie
:row.:c.F2              :c.Sla het actieve document op
:row.:c.F3              :c.Be‰indig
:row.:c.F4              :c.Sla het actieve document op en sluit het
:row.:c.F5              :c.Spellingcontrole
:row.:c.F10             :c.Menu
:row.:c.Ctrl + Tab      :c.Volgende Document
:row.:c.Ctrl + O        :c.Open een document
:row.:c.Ctrl + P        :c.Druk een volledig document
:row.:c.Alt + P         :c.Druk de huidige bladzijde
:row.:c.Ctrl + N        :c.Zet normaal (verwijder tekstkenmerken)
:row.:c.Ctrl + B        :c.Zet vet
:row.:c.Ctrl + I        :c.Zet schuin
:row.:c.Ctrl + U        :c.Zet onderlijnd
:row.:c.Ctrl + S        :c.Zoek
:row.:c.Ctrl + F        :c.Zoek volgende
:row.:c.Ctrl + Z        :c.Vergroot Alles Schakel
:row.:c.Ctrl + Backspace:c.Verwijder tot het begin van het volgende woord.
:etable.
Bovendien zijn er een aantal toetsen gedefinieerd voor sturing binnen
het huidige document. Voor de definities zie het onderwerp
:link reftype=hd res=60003.Opmaken van Documenten (Sturing):elink..

:h2 res=IDM_ABOUT_MENU.Product Informatie Menu (Submenu)
Als je dit keuzemenu direct kiest zal de productinformatie en
copyrechten dialoog worden getoond.
:p.Zie :link reftype=hd res=IDD_ABOUT.Productinformatie (Dialoog):elink. voor meer
informatie.
:p.Als je op het voorwaardelijke menuknop drukt, zal een menulijst
van de ge‹nstalleerde producten zal worden getoond. Hier
kun je productinformatie verkrijgen van alle produkten die
ge‹nstalleerd zijn.

:h3 res=IDM_ABOUT.Productinformatie (Menu-optie)
Deze optie toont de gegevens over de huidige versie van Escriba.
:p.Zie :link reftype=hd res=IDD_ABOUT.Productinformatie (Dialoog):elink. voor meer
informatie.

:h4 res=IDD_ABOUT.Productinformatie (Dialoog)
Dit dialoog toont de naam, het ikoon en de copyright-informatie van het
programma. Druk op de Enter, spatie of de Escape-toets of de OK-knop om de
dialoog te be‰indigen.
:artwork name='NdrLands\About.bmp' align=center.

:h1 res=60009.Context Menu&apos.s
:i1.Context Menu&apos.s
Er zijn diverse Contextmenu&apos.s die opgeroepen kunnen worden met een enkele klik
van de rechter muisknop. Deze contextmenu&apos.s hebben nog meer functie dan reeds
beschikbaar met het hoofd-venster achtergrond-menu&apos.s.
:p.
Als je met de rechter muisknop klikt op de werkruimte van het hoofdprogramma
(dus niet in het venster van het document), zal het 
:link reftype=hd res=IDM_MAIN_CONTEXT.Hoofd Context Menu:elink. opgeroepen worden.
Als je met de rechter knop in het venster van het document klikt, maar niet
over gemarkeerde tekst of op grafische beelden, dan wordt het 
:link reftype=hd res=IDM_DEFAULTMENU.Vaste Optie Menu:elink. opgeroepen.
Klikken met de rechter knop in gemarkeerde tekst zal tot gevolg hebben dat het
:link reftype=hd res=IDM_TEXTMENU.Geselekteerde Tekstmenu:elink. komt.
Ook is er het :link reftype=hd res=IDM_BITMAPMENU.Grafisch Menu:elink. en het
:link reftype=hd res=IDM_BORDERMENU.Grafische Randmenu:elink..

:h2 res=IDM_MAIN_CONTEXT.Hoofd Contextmenu
Dit menu verschijnt als je met de rechter muisknop in de ruimte van het
hoofdprogramma klikt (niet in een documentvenster). Hierin komen alle menu-opties
voor die buiten het document geldig zijn. Dit zijn&colon.
:sl.
:li.&sqbul.:link reftype=hd res=IDM_REFRESH.Verversen:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILE_MENU.Bestand:elink.
:li.&sqbul.:link reftype=hd res=IDM_WINDOW_MENU.Venster:elink.
:li.&sqbul.:link reftype=hd res=IDM_HELP_MENU.Hulp:elink.
:esl.

:h3 res=IDM_REFRESH.Verversen (Menu-optie)
Deze optie zorgt ervoor dat het volledige document ververst wordt.

:h2 res=IDM_DEFAULTMENU.Standaardmenu
Dit menu verschijnt wanneer je met de rechter muisknop op een gedeelte van het
document klikt waar zich geen (1) geselecteerde tekst, (2) een afbeelding of (3)
een afbeeldingskader bevindt.  Het bevat opdrachten die voor het volledige
document van toepassing zijn. Dit omvat&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_REFRESH.Verversen:elink.
:esl.

:h2 res=IDM_TEXTMENU.Tekstmenu
Dit menu verschijnt wanneer je met de rechter muisknop op een geselecteerd
tekstblok klikt.  Het bevat alle menu-opdrachten die van toepassing zijn op een
dat blok.
De volgende mogelijkheden zijn beschikbaar&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_CUT.Snijden:elink.
:li.&sqbul.:link reftype=hd res=IDM_COPY.Kopi‰ren:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE.Plakken:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEAR.Wissen:elink.
:li.&sqbul.:link reftype=hd res=IDM_LAYOUT_MENU.Opmaak:elink.
:li.&sqbul.:link reftype=hd res=IDM_FONT_MENU.Lettertype:elink.
:li.&sqbul.:link reftype=hd res=IDM_SIZE_MENU.Lettergrootte:elink.
:li.&sqbul.:link reftype=hd res=IDM_ATTR_MENU.Kenmerken:elink.
:esl.
:note.Het sub-opmaakmenu bevat niet alle opdrachten van het normale opmaakmenu.
Enkel de mogelijkheden die gelden voor een geselecteerd tekstblok staan ter
beschikking.  Deze uitzonderingen worden aangeduid in de gedetailleerde
beschrijvingen van de verschillende opdrachten.

:h2 res=IDM_BITMAPMENU.Afbeeldingmenu
Dit menu verschijnt wanneer je met de rechter muisknop op een afbeelding klikt.
Het omvat alle opdrachten die op een dergelijk object van toepassing zijn.  De
mogelijkheden zijn&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_BACKGROUND.Achtergrond:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_NOTEXTASIDE.Geen nevenstaande tekst:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_HALFTONE.Halftoon:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_REVERSE.Reverse:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_OPTIMIZE.Optimaliseer:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION.Plaats:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE.Afmeting:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT.Verhouding:elink.
:esl.

:h3 res=IDM_BITMAP_BACKGROUND.Achtergrond (Menu-optie)
Als je deze optie kiest, wordt het grafische beeld beschouwd als behorende
tot de achtergrond van de tekst. Normale tekst kan dan over de afbeelding gaan.

:h3 res=IDM_BITMAP_NOTEXTASIDE.Geen nevenstaande tekst (Menu-optie)
Als je dit menu kiest, wordt het grafische beeld zo gemerkt dat het geen tekst
terzijde van de afbeelding toelaat.

:h3 res=IDM_BITMAP_HALFTONE.Halftoon (Menu-optie)
Als je deze optie kiest, wordt het grafische plaatje gemerkt, zodat het halftoon
of verzachte kleuren toelaat. Dit is vooral handig als je een beeld op de
achtergrond van de tekst wilt hebben.

:h3 res=IDM_BITMAP_REVERSE.Omgekeerd (Menu Optie)
Als je deze menu-optie kiest wordt dit grafische plaatje gemarkeerd om in omgekeerde
kleuren weergegeven te worden.
:note.Deze optie werkt alleen als er met bitmap-beelden gewerkt wordt(geen META-
bestanden.

:h3 res=IDM_BITMAP_OPTIMIZE.Optimaliseer (Menu Optie)
Als je deze keuze neemt, dan wordt het grafische plaatje gemerkt voor aanpassing
aan de drukker die in gebruik is. d.w.z. dat het plaatje zo aangepast wordt dat
de afmeting van het plaatje een veelvoud is van de originele afmeting van de
bitmap. Dit zorgt er voor dat de kwaliteit van het plaatje op de drukker zo goed
mogelijk is, aangezien de regel/kolom methode van tussenvoeging zal er voor zorgen
dat de bitmaps zo nauwkeurig mogelijk zijn. Deze optie werkt allem met
bitmap plaatjes (niet met META-bestanden).

:h3 res=IDM_BITMAP_POSITION.Grafische plaatsingmenu
Vanuit dit menu is het mogelijk om uit een aantal mogelijkheden te kiezen die
invloed hebben op de plaatsing van een plaatje in het document.
De volgende keuzes zijn mogelijk&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_NEXT_CHAR.Next Character:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_NEXT_LINE.Next Line:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_LEFT.Linkerkant:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_LEFT.Linkermarge:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_CENTERED_X.Gecentreerd (X):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_RIGHT.Rechtermarge:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_RIGHT.Rechterkant:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_TOP.Bovenkant:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_TOP.Bovenmarge:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_CENTERED_Y.Gecentreerd (Y):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_BOTTOM.Ondermarge:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_BOTTOM.Onderkant:elink.
:esl.
Als een van deze mogelijkheden op dit moment geldt, is deze gemerkt.

:h4 res=IDM_BITMAP_POSITION_NEXT_CHAR.Beeldpositie t.o.v. volgende teken (Menu Optie)
Als men deze menu-optie kiest, wordt het gekozen beeld behandeld als een groot teken
en zo in het document opgenomen. Waar de beeld-code ook in het document gevoegd wordt
het zal zich de document tekst aanpassen om zichzelf een plaats te geven.

:h4 res=IDM_BITMAP_POSITION_NEXT_LINE.Beeldpositie t.o.v. de Nieuwe Regel (Menu Optie)
Deze functie is nog niet ge‹mplementeerd.

:h4 res=IDM_BITMAP_POSITION_EDGE_LEFT.Beeldpostitie aan Linkerkant (Menu Optie)
Als je deze menu-optie kiest, zal het geselecteerde beeld verplaatst worden naar
de linkerkant van het document, het een en ander afhankelijk van de afmeting en
vorm. Als een andere vorm wordt gekozen zal de plaats aangepast worden.

:h4 res=IDM_BITMAP_POSITION_MARGIN_LEFT.Beeldpositie aan Linkermarge (Menu Optie)
Als je deze menu-optie kiest, zal het geselecteerde beeld verplaatst worden naar
de linkermarge van het document, het een en ander afhankelijk van de afmeting en
vorm. Als een andere vorm wordt gekozen zal de plaats aangepast worden.

:h4 res=IDM_BITMAP_POSITION_CENTERED_X.Beeldpositie Gecentreerd (X) (Menu Optie)
Als deze optie gekozen wordt, zal het gekozen beeld horizontaal gecentreerd worden.
Normaal zal deze centrering plaats vinden met inachtneming van de huidige marges.
Als de beeldmaat gezet is ingesteld op
:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Beeldmaat kant-tot-kant (X):elink.,
dan zal de centrering ingesteld worden op de ingestelde kliplimieten (kanten).

:h4 res=IDM_BITMAP_POSITION_MARGIN_RIGHT.Beeldpositie aan Rechtermarge (Menu Optie)
Als je deze menu-optie kiest, zal het geselecteerde beeld verplaatst worden naar
de rechtermarge van het document, het een en ander afhamkelijk van de afmeting en
vorm. Als een andere vorm wordt gekozen zal de plaats aangepast worden.

:h4 res=IDM_BITMAP_POSITION_EDGE_RIGHT.Beeldpositie aan Rechterkant (Menu Optie)
Als je deze menu-optie kiest, zal het geselecteerde beeld verplaatst worden naar
de rechterkant van het document, het een en ander afhankelijk van de afmeting en
vorm. Als een andere vorm wordt gekozen zal de plaats aangepast worden.

:h4 res=IDM_BITMAP_POSITION_EDGE_TOP.Beeldpositie aan Bovenkant (Menu Optie)
Als je deze menu-optie kiest, zal het geselecteerde beeld verplaatst worden naar
de rechterkant van het document, het een en ander afhamkelijk van de afmeting en
vorm. Als een andere vorm wordt gekozen zal de plaats aangepast worden.

:h4 res=IDM_BITMAP_POSITION_MARGIN_TOP.Beeldpositie aan Bovenmarge (Menu Optie)
Als je deze menu-optie kiest, zal het geselecteerde beeld verplaatst worden naar
de bovenmarge van het document, het een en ander afhankelijk van de afmeting en
vorm. Als een andere vorm wordt gekozen zal de plaats aangepast worden.

:h4 res=IDM_BITMAP_POSITION_CENTERED_Y.Beeldpositie Gecentreerd (Y) (Menu Optie)
Als deze optie gekozen wordt, zal het gekozen beeld vertikaal gecentreerd worden.
Normaal zal deze centrering plaats vinden met in achtneming van de huidige marges.
Als de beeldmaat gezet is ingesteld op
:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Beeldmaat kant-tot-kant (Y):elink.,
dan zal de centrering ingesteld worden op de ingestelde kliplimieten (kanten).

:h4 res=IDM_BITMAP_POSITION_MARGIN_BOTTOM.Beeldpositie aan Ondermarge (Menu Optie)
Als je deze menu-optie kiest, zal het geselecteerde beeld verplaatst worden naar
de ondermarge van het document, het een en ander afhankelijk van de afmeting en
vorm. Als een andere vorm wordt gekozen zal de plaats aangepast worden.

:h4 res=IDM_BITMAP_POSITION_EDGE_BOTTOM.Beeldpositie aan Onderkant (Menu Optie)
Als je deze menu-optie kiest, zal het geselecteerde beeld verplaatst worden naar
de onderkant van het document, het een en ander afhankelijk van de afmeting en
vorm. Als een andere vorm wordt gekozen zal de plaats aangepast worden.

:h3 res=IDM_BITMAP_SIZE.Beeldafmeting Menu
Met dit submenu is het mogelijk om een keuze te maken die van invloed zijn
op de afmeting van het beeld in het document.
De mogelijkheden zijn als volgt&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Kant-tot-Kant (X):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_MARGINTOMARGIN_X.Marge-tot-Marge (X):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Kant-tot-Kant (Y):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_MARGINTOMARGIN_Y.Marge-tot-Marge (Y):elink.
:esl.
Als een van deze opties geldig zijn voor het geselecteerde beeld, is dit gemerkt.

:h4 res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Beeldmaat Kant-tot-Kant (X) (Menu Optie)
Wanneer men deze keuze gemaakt heeft wordt het gekozen beeld zo aangepast dat
dat het tussen de beide kanten valt, tenzij het beeld ook gemerkt is om de verhoudingen
vast te houden. (zie :link reftype=hd res=IDM_BITMAP_ASPECT.Grafisch Aspect Menu:elink.).
In dat geval zal het beeld wat smaller worden.

:h4 res=IDM_BITMAP_SIZE_MARGINTOMARGIN_X.Beeldmaat Marge-tot-Marge (X) (Menu Optie)
Wanneer men deze keuze gemaakt heeft wordt het gekozen beeld zo aangepast dat
dat het tussen de beide marges valt, tenzij het beeld ook gemerkt is om de verhoudingen
vast te houden. (zie :link reftype=hd res=IDM_BITMAP_ASPECT.Grafisch Aspect Menu:elink.).
In dat geval zal het beeld wat smaller worden.

:h4 res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Beeldmaat Kant-tot-Kant (Y) (Menu Optie)
Wanneer men deze keuze gemaakt heeft wordt het gekozen beeld zo aangepast dat
dat het tussen de beide kanten valt, tenzij het beeld ook gemerkt is om de verhoudingen
vast te houden. (zie :link reftype=hd res=IDM_BITMAP_ASPECT.Grafisch Aspect Menu:elink.).
In dat geval zal het beeld wat korter worden.

:h4 res=IDM_BITMAP_SIZE_MARGINTOMARGIN_Y.Beeldmaat Marge-tot-Marge (Y) (Menu Optie)
Wanneer men deze keuze gemaakt heeft wordt het gekozen beeld zo aangepast dat
dat het tussen de beide marges valt, tenzij het beeld ook gemerkt is om de verhoudingen
vast te houden. (zie :link reftype=hd res=IDM_BITMAP_ASPECT.Grafisch Aspect Menu:elink.).
In dat geval zal het beeld wat korter worden.

:h3 res=IDM_BITMAP_ASPECT.Beeldverhoudings Menu
Vanuit dit menu is het mogelijk om te bepalen op welke manieren het beeld
vertikaal of horizontaal vastgehouden zal worden.
Er zijn de volgende mogelijkheden&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FLOAT.Niet Vast.:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FIX_X.Vaste X:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FIX_Y.Vaste Y:elink.
:esl.
Als een van deze opties op dit moment voor het beeld geldig is zal dat  gemerkt
zijn.
:h4 res=IDM_BITMAP_ASPECT_FLOAT.Beeldverhouding Niet Vast (Menu Optie)
Als deze mogelijkheid gekozen wordt, dan is het beeld niet gehouden aan een 1x1
verhouding. Normaal zal dit geen onmiddellijk zichtbaar effect hebben op het beeld,
tenzij voor het beeld ook een kant-tot-kant of marge-tot-marge instelling geldt.
In dat geval zal het beeld zichzelf aanpassen.

:h4 res=IDM_BITMAP_ASPECT_FIX_X.Beeldverhouding Vaste X (Menu Optie)
Als deze keuze gemaakt wordt heeft het beeld een beperking op het formaat
zodat het altijd afgebeeld wordt met een 1x1 verhouding.
Met deze keuze is de sturende afmeting de breedte. De hoogte zal aangepast
worden om de 1x1 verhouding te bewaren.

:h4 res=IDM_BITMAP_ASPECT_FIX_Y.Beeldverhouding Vaste Y (Menu Optie)
Als deze keuze gemaakt wordt heeft het beeld een beperking op het formaat
zodat het altijd afgebeeld wordt met een 1x1 verhouding.
Met deze keuze is de sturende afmeting de hoogte. De breedte zal aangepast
worden om de 1x1 verhouding te bewaren.

:h2 res=IDM_BORDERMENU.Afbeelding-kadermenu
Dit menu verschijnt wanneer je met de rechter muisknop op het (gebied van het)
kader van een afbeelding klikt.  Het omvat alle opdrachten die op dit gebied van
toepassing zijn. De mogelijkheden zijn&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_SIZE.Kaderbreedte:elink.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_COLOR.Kaderkleur:elink.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_CAPTION.kadertitel:elink.
:esl.

:h3 res=IDM_BORDER_SIZE.Kaderbreedte (Menu-optie)
Deze functie is nog niet beschikbaar.

:h3 res=IDM_BORDER_COLOR.Kaderkleur (Menu-optie)
Deze functie is nog niet beschikbaar.

:h3 res=IDM_BORDER_CAPTION.Kadertitel (Menu-optie)
Deze functie is nog niet beschikbaar.

:h1 res=60010.Andere Dialogen

:h2 res=IDD_PROFILE_PATH.Zet profielpad
Dit dialoog wordt gestart wanneer het programma het profielbestand (INI) niet
vindt. Je kunt ingeven waar het bestand gezocht of gemaakt moet worden.
:artwork name='NdrLands\Profile.bmp' align=center.
:p.Enkel een geldige directorynaam wordt aanvaard. Na het ingeven kun je
de Enter-toets of OK-knop drukken om het programma verder te laten gaan.
:p.Wil je het programma op dit moment be‰indigen dan moet je de Esc-toets of -knop
drukken.

:h2 res=IDD_PROMPT_SAVE.Gewijzigd document opslaan ?
.* :artwork name='NdrLands\SaveChng.bmp' align=center.
Je probeert het programma te verlaten of een venster te sluiten zonder alle
wijzigingen te hebben opgeslagen. Antwoordt &apos.J&apos.a, of druk de JA-knop in, als je
het document wil bewaren onder de huidige naam. Antwoordt &apos.N&apos.ee, of druk de NEE-
knop in, indien je dit niet wil doen (wijzigingen gaan verloren). Druk de Esc-toets of -
knop indien je toch niet wilt sluiten of verlaten.

:h2 res=IDD_PROMPT_SAVECONFIRM1.Opslaan bevestigen
Deze dialoog wordt gestart wanneer je probeert het actieve
document over een bestaand bestand op te slaan.
.* :artwork name='NdrLands\SaveConf.bmp' align=center.
:p.Is het werkelijk de bedoeling om het bestaande te vervangen,
dan kun je de de J-toets of JA-knop indrukken. Wil je het bestand op schijf niet
overschrijven, dan moet je de Enter of N-toets drukken. In dit
geval wordt het actieve document niet opgeslagen en moet je
opnieuw proberen met een andere naam.

:h2 res=IDD_PROMPT_SAVECONFIRM2.Bevestig Ander Type
Als deze dialoog wordt getoond, dan probeer je het document als aan ander
type op te slaan.
.* :artwork name='NdrLands\SaveCnf2.bmp' align=center.
:p.Als je het werkelijk als een ander type op wilt slaan, druk dan op de
J-toets of klik op de Ja-knop.
Als je niet wilt veranderen, druk dan op de N-toets, je document zal dan
niet opgeslagen worden en je zult dan weer moeten proberen onder een andere naam.

:h2 res=IDD_MESSAGE_SPELLDONE.Spellingcontrole gereed
Als dit dialoog wordt getoond, dan heb je net een spellingcontrole uitgevoerd.
.* :artwork name='NdrLands\SpelDone.bmp' align=center.
:p.Om verder te gaan druk op de Enter-toets of druk op de Enter-knop met de muis.

:h2 res=IDD_ERROR_HELPMANAGER.Fout in hulpbeheerder
Dit bericht wordt getoond wanneer er iets verkeerd is met het programmabestand.
Waarschuw a.u.b. de programmeur.

:h2 res=IDD_ERROR_EXCEPTION.Stop door &apos.Program Exception&apos.
.* :artwork name='NdrLands\Except.bmp' align=center.
Als dit dialoog wordt getoond heeft Escriba een ernstige fout ondergaan.
Informeer de auteur  zoals genoemd in de 
:link reftype=hd res=60008.Vragen en Fout-rapportering:elink. pagina en sluit een ESCRIBA.LOG-
bestand bij.  Deze log moet staan in de directory waar Escriba staat.

:h2 res=IDD_ERROR_WINREGISTERCLASS.Fout bij de aanmelding van de vensterklasse
Dit bericht waarschuwt je van het feit dat Escriba er niet in geslaagd is een
van zijn vensterklassen aan te melden.
Licht de auteur in op de manier aangegeven in het :link reftype=hd res=60008.Vragen en fout-rapportering:elink. pagina.

:h2 res=IDD_ERROR_CREATECLIENT.&apos.Client aanmaken Fout
Deze dialoog wordt getoond als Escriba om de een of andere reden niet in staat
was om een &apos.client window&apos.aan te maken.  Licht de auteur in op de manier aangegeven in het
:link reftype=hd res=60008.Vragen en fout-rapportering:elink. pagina.

:h2 res=IDD_ERROR_TOOMANYDOCUMENTS.Fout bij te veel documenten
Als dit dialoog wordt getoond, heb je geprobeerd om te veel documenten in
Escriba te laden.

:h2 res=IDD_ERROR_TOOMANYLOADED.Fout bij te veel geladen documenten
Als je deze fout krijgt gemeld heb je reeds te veel documenten geladen om nog
een venster te kunnen openen.

:h2 res=IDD_ERROR_DOCUMENT_CREATE.Fout bij het aanmaken van een document
Als dit dialoog wordt getoond, kon Escriba om de een of andere reden geen nieuw
document aanmaken.  Licht de auteur in op de manier aangegeven in het
:link reftype=hd res=60008.Vragen en fout-rapportering:elink. pagina.

:h2 res=IDD_ERROR_DOCUMENT_LOAD.Fout bij het laden van een document
Als dit dialoog wordt getoond, kon Escriba een document niet laden.
Ga al de benamingen na van het bestand en het pad naar het bestand.
Het is ook mogelijk dat het bestand op dit moment door een ander programma
gebruikt wordt en om die reden niet voor Escriba beschikbaar is.

:h2 res=IDD_ERROR_DOCUMENT_SAVE.Fout bij het opslaan van een document
Als dit dialoog wordt getoond, betekent dit dat Escriba niet kon opslaan in het
bestand dat je aangegeven hebt.
Ga na dat de naam juist is en dat ook het pad naar het bestand juist genoemd is.
Het is ook mogelijk dat het bestand op dit moment door een ander programma
gebruikt wordt en om die reden niet voor Escriba beschikbaar is.

:h2 res=IDD_ERROR_CREATEPRINTSTATUS.Drukkerstatusvenster Aanmaak Fout
Als dit dialoog wordt getoond, was het voor Escriba niet mogelijk om
Drukkerstatusvenster te openen. Normaal zal dit nooit gebeuren maar het
toegevoegd voor diagnostische doeleinden.
Laat het de auteur weten door middel van een van de kanalen die genoemd worden
in :link reftype=hd res=60008.Vragen en foutrapportering:elink. pagina.

:h2 res=IDD_ERROR_CREATELOADSTATUS.Drukkerlaadvenster Aanmaak Fout
Als deze dialoog wordt getoond, dan was Escriba niet in staat om zijn Drukker-
laadvenster aan te maken.  Normaal zal dit nooit gebeuren maar het toegevoegd
voor diagnostische doeleinden.  Laat het de auteur weten door middel van een van
de kanalen die genoemd worden in :link reftype=hd res=60008.Vragen en foutrapportering:elink. pagina.

:h2 res=IDD_ERROR_BLOCKMEMORY.Blokkopiegeheugen Fout
Als deze dialoog wordt getoond, dan was er niet geheugen beschikbaar om een deel
van de tekst naar het systeem-klembord te brengen.
Tracht kleinere delen naar het klembord over te brengen.

:h2 res=IDD_ERROR_BADGRAPHICFILE.Slecht Grafischbestand Fout
Als dit dialoog wordt getoond dan heb je getracht een grafisch bestand te laden
van een onbekend type of er zat een gegevensfout in. De ondersteunde
bestandstypen zijn BMP, GIF, JPG, PCX, PCC, MET en TIF. Als je een van deze bestandtypen
probeert te laden, en je kunt deze bestanden met een standaard programma
bekijken en je hebt toch een fout, laat het de auteur dan weten door middel van
een van de kanalen die genoemd worden in 
:link reftype=hd res=60008.Vragen en foutrapportering:elink. pagina.

:h2 res=IDD_ERROR_VERIFY1.Controlefout - Open
Als dit foutbericht wordt getoond, kan het bestand om de  een
of andere reden niet geladen worden. Ga na of bestandnaam
en pad kloppen en probeer nog eens.

:h2 res=IDD_ERROR_VERIFY2.Controlefout - Geheugen
Dit foutbericht wordt getoond als Escriba onvoldoende geheugen
kon beleggen.

:h2 res=IDD_ERROR_VERIFY3.Controlefout - Onjuiste Escape-codes
Als dit bericht wordt getoond, dan bevatte het oorspronkelijke
Escriba-bestand onjuiste escape-codes. Dit kan zijn veroorzaakt doordat
het bestand verminkt is.

:h2 res=IDD_ERROR_VERIFY4.Controlefout - Onjuiste functies
Als dit bericht getoond wordt bij het laden van oorspronkelijke
Escriba-bestanden, geeft dit aan dat het bestand onjuiste functiecodes
bevat. Het bestand kan aangemaakt zijn door een nieuwere versie van
Escriba en verminkt zijn geraakt.

:h2 res=IDD_ERROR_VERIFY5.Controlefout - Leeg
Als dit bericht wordt getoond, dan het bestand dat wordt geladen
bevatte geen andere tekens dan geldige functiecodes
Het bestand kan verminkt zijn.

:h2 res=IDD_ERROR_VERIFY6.Controlefout - Headers/Trailers niet overeen
Als dit bericht wordt getoond, dat bevatte een bestand in het oorspronkelijke
Escriba-formaat Headers en Trailers die niet overeen komen.
Het bestand kan verminkt zijn.

:h2 res=IDD_ERROR_VERIFY7.Controlefout - Geen Preface
Als dit bericht wordt getoond, dan betekent dat een oorspronkelijk
Escriba-bestand geen 'preface' bevat.
Het bestand kan verminkt zijn.

:h2 res=IDD_ERROR_VERIFY8.Controlefout - Geen Geldig Type
Als dit bericht wordt getoond, dan betekent dit dat het bestand
niet voldeed aan een van de geldige bestandtypen. Escriba kan een
dergelijk bestand niet laden.
:note.Verwerkers van nieuwe bestandtypen kunnen op elk moment
ontwikkeld worden. Als je een bestandtype hebt dat Escriba op dit
moment niet kan laden maar dat je graag met Escriba zou willen
bewerken, zendt dan een bericht naar de auteur op een van de adressen
weergegeven in de :link reftype=hd res=60008.Vragen en foutmelding:elink. pagina.

:h2 res=IDD_ERROR_IMPORT1.Importfout - Open
Als dit bericht getoond wordt, dan kon het bestand niet worden
geladen.
Ga de bestandnaam en het pad na en probeer nog eens.

:h2 res=IDD_ERROR_IMPORT2.Importfout - Geheugen
Wanneer dit bericht wordt getoond, kan Escriba niet genoeg geheugen beleggen
om het aangeduide bestand te laden-

:h2 res=IDD_ERROR_EXPORT1.Exportfout - Open/Maak aan
Als dit bericht getoond wordt, dan kon het aangegeven bestand om
de een of andere reden niet geopend worden. Ga bestandnaam en pad
na en probeer nogmaals.
Als het bestand reeds bestaat is het mogelijk dat het bestand gemerkt
is als 'read-only'.

:h2 res=IDD_ERROR_EXPORT2.Exportfout - Schijfruimte
Als dit bericht wordt getoond, betekent dit dat het aangegeven bestand
niet in zijn geheel naar de schijf geschreven kon worden.

:h2 res=IDD_METAFILE_VERIFY1.Meta-bestand Controlefout - Open
Wanneer deze boodschap wordt getoond, dan kon het Meta-bestand niet voor
controle en laden niet worden geopend. Normaal betekent dit dat het
bestand niet benaderd kan worden omdat het door een ander programma
in gebruik is.

:h2 res=IDD_METAFILE_VERIFY2.Metafile Controlefout - Kort Bestand.
Als deze boodschap wordt getoond betekent dit dat het Meta-bestand,
die voor controle of laden was geslecteerd, te kort is.

:h2 res=IDD_METAFILE_VERIFY3.Metafile Controlefout - Laden
Als deze boodschap wordt getoond, betekent dit dat een Meta-bestand
dat voor controle geselecteerd was niet in het geheugen geladen kon
worden. Als deze fout herhaaldelijk voorkomt dan wordt u vriendelijk
verzocht de auteur te informeren via een van de adresssen opgegeven
onder :link reftype=hd res=60008.Vragen en Foutmelding:elink. page.

:h2 res=IDD_METAFILE_VERIFY4.Metafile Controlefout - Ongeldig Gegeven
Als dit foutbericht wordt getoond, dan bevond zich in de kop van het META-bestand onjuiste informatie.
Ga na of het META-bestand bekeken kan worden of geladen kan worden met het OS/2-PICVIEW-programma. Als het zonder fouten geladen kan worden dan graag de auteur informeren via een van de opgegeven adressen in de 
:link reftype=hd res=60008.Vragen en foutrapportering:elink. pagina.

:h2 res=IDD_ERROR_LOADLANGUAGE.Taalkeuze-fout
Als deze fout wordt getoond dan kon de gekozen taal om de een of andere reden niet volledig geladen worden. Graag de auteur informeren via een van de opgegeven adressen in de :link reftype=hd res=60008.Vragen en foutrapportering:elink. pagina.

:h1 res=61000.Technische Referenties

:h2 res=61001.Document Bestandformaat
Een documentbestand kan voorgesteld worden als een reeks tekens en functiecodes.
Met uitzondering van de besturingtekens 15, 16 en 17 worden alle gegevens
zonder veranderingen opgeslagen. Codes 15, 16 en 17 worden overgeslagen, terwijl
code 15 de Escape-code is, d.w.z., dat als het originele teken letterlijk 16 is,
dan zal het uiteindelijke bestand een code 15 gevolgd door een code 16 bevatten
om de originele code 16 weer te kunnen geven. Als code 15 wordt aangetroffen
zonder dat dit gevolgd wordt door een 15, 16 of 17 dan wordt ook code 15
letterlijk genomen.
:p.
Een letterlijke code 17 &apos.&larrow.&apos. geeft een begin functiecode weer.
Een letterlijke code 16 &apos.&rarrow.&apos. geeft een einde functiecode weer.
Als een einde functiecode wordt aangetroffen zonder een bijbehorende begin
functiecode wordt deze letterlijk genomen.
:p.De volgende functies zijn geldig&colon.

:lm margin=3.:p.&larrow.Version&colon.:hp5.versienummer:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode geeft de versie weer van Escriba dat voor opmaak en
veiligstellen van het document op de schijf. Het versienummer heeft vier
cijfers, die worden beschouwd als of zij een decimale punt hadden tussen de
eerste twee en de laatste twee tekens. Versie 1.00 wordt weergegeven als
0100. Deze functiecode wordt gebruikt om documenten van oudere versie om te
kunnen zetten naar nieuwere formaten. Het was niet aanwezig in de bestands-
formaten voor de 0.92beta versie.
Deze code kan niet voorkomen buiten het variabelen-gebied van de &apos.Preface&apos. functiecode.

:lm margin=3.:p.&larrow.Preface&colon.:hp5.functiecodes:ehp5.&rarrow.
:lm margin=5.:p.
Dit is een speciale functiecode die wel aanwezig zal zijn in het bestand maar
niet in het document zoals de gebruiker het te zien krijgt.
Deze functie-veranderlijke gegevens  bevat andere functiecodes.
Het uiteindelijke resultaat zal zijn dat er een document basisdefinitie bestaat.
Alle functiecodes die deze basisdefinitie kunnen veranderen zijn hier geldig,
samen met de drukker, formulier, werkstatus, tabinstelling en metrische
functies, die elders niet geldig zijn. Functiecodes die tekstuitvoer genereren
zoals aantal pagina&apos.s, paginanummer, grafisch, datum, etc zijn hier niet geldig.

:lm margin=3.:p.&larrow.Printer&colon.:hp5.naam:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode geeft de naam van de drukker waarvoor dit document ingesteld
is. De code kan niet buiten het variabelen-gebied van de &apos.Preface&apos.
functie-code gebruikt worden.

:lm margin=3.:p.&larrow.Form&colon.:hp5.naam:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode werd gebruikt om de naam van het formulier, waarin de
specificaties  voor het document (A4, Brief, etc.) waren opgeslagen, veilig te stellen. 
Het wordt allen gebruikt wanneer een document voor een drukker is aangepast en omgezet
wordt voor een andere drukker.
Deze code kan niet voorkomen buiten het variabelen-gebied van de &apos.Preface&apos. functiecode.

:lm margin=3.:p.&larrow.CodePage&colon.:hp5.nummer:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode wordt gebruikt om het nummer van de codepage aan te geven bij
weergeven van het document.
Deze code kan niet voorkomen buiten het variabelen-gebied van de &apos.Voorwoord&apos. functiecode.
Zie :link reftype=hd res=61002.Code Pages:elink. voor meer informatie.

:lm margin=3.:p.&larrow.Font&colon.:hp5.naam:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode geeft de naam weer van het Lettertype dat gebruikt gaat worden vanaf
de positie van de aanwijzer. De code moet dat van een geldige schaalbare
lettertypes zijn in het Systeem Lettertype Pallette, in een Lettertype Dialoog van een
normaal systeem of in Escriba&apos.s Lettertypemenu.

:lm margin=3.:p.&larrow.Size&colon.:hp5.puntmaat:ehp5.&rarrow.
:lm margin=5.:p.
Deze code geeft de puntmaat van de letter in de drukker weer die vanaf dit punt
in het document gebruikt zal worden.  Een punt in de drukker is 1/72 duim
Engelse maat. Wanneer de instelling 12 punts is, zal dat letters geven 1/6 duim
hoog. Dit is een leiddraad voor de ontwikkelaars van lettertypes, het is geen regel.
In het algemeen zullen letters niet allemaal dezelfde afmeting hebben, zelfs
als ze met het zelfde aantal punten zijn opgebouwd.
Escriba accepteert puntinstellingen die leiden naar een resolutie van 1/10 punt.

:lm margin=3.:p.&larrow.Color&colon.:hp5.code:ehp5.&rarrow.
:lm margin=5.:p.
Deze code geeft de kleur (rood-groen-blauw) die voor de uitvoer van lijnen en
tekst gebruikt gaat worden vanaf deze plaats in het document. De code wordt
weergegeven als een 6-cijferige code, waarvan de eerste twee cijfers de rode
component aangeven, de tweede paar cijfers de groene component en de laatste
paar cijfers de blauwe component weergegeven.
Wit b.v. wordt &apos.FFFFFF&apos., zwart &apos.000000&apos., en geel &apos.FFFF00&apos..

:lm margin=3.:p.&larrow.LineWidth&colon.:hp5.dikte:ehp5.&rarrow.
:lm margin=5.:p.
Deze functie bepaald de lijndikte welke gebruikt gaat worden bij holle letters
als de holle variabele ingesteld is. De dikte wordt op een ander plaats
ingesteld.

:lm margin=3.:p.&larrow.LineEnd&colon.:hp5.type:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode zet het type lijneinde dat vanaf de huidige positie van de aanwijzer
moet worden gebruikt.
Geldige lijneinde-typen zijn al die LINEEND codes die in de OS/2 Programmer&apos.s
Reference gedefinieerd zijn.

:lm margin=3.:p.&larrow.LineJoin&colon.:hp5.type:ehp5.&rarrow.
:lm margin=5.:p.
Deze functie zet het type lijnvoeg instructie dat vanaf de huidige positie van de
aanwijzer moet worden gebruikt.
Geldige codes zijn al die codes die gespecificeerd zijn in de OS/2 Programmer&apos.s
Reference.

:lm margin=3.:p.&larrow.FillColor&colon.:hp5.code:ehp5.&rarrow.
:lm margin=5.:p.
Deze code geeft de kleur (rood-groen-blauw) die voor de uitvoer van holle
letters als vulkleur gebruikt gaat worden vanaf deze plaats in het documument.
De code is het zelfde als die bij lijn- tekstkleur gebruikt wordt.

:lm margin=3.:p.&larrow.Filltype&colon.:hp5.type:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode geeft aan met welk patroon de ruimte gevuld moet worden vanaf
de huidige positie van de aanwijzer.
De geldige codes zijn al die codes die gedefinieerd zijn in de PATSYM codes
in de OS/2 Programmer&apos.s Reference.

:lm margin=3.:p.&larrow.Bold&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode zet de VET-code aan voor de tekst op of na de huidige positie
van de aanwijzer.

:lm margin=3.:p.&larrow.Italic&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode zet de SCHUIN-code aan voor alle tekst op of na de huidige positie
van de aanwijzer.

:lm margin=3.:p.&larrow.Underscore&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode zet de ONDERSTREEP-code aan voor alle tekst op of na de positie
van de aanwijzer.

:lm margin=3.:p.&larrow.Strikeout&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode zet de DOORSTREEP-code aan voor alle tekst op en na de huidige
positie van de aanwijzer.

:lm margin=3.:p.&larrow.Outline&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Deze functie zet de HOLLE-code aan voor de tekst op of na de huidige positie
van de aanwijzer.

:lm margin=3.:p.&larrow.Caps&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode zet de GROTE LETTER-code aan voor alle tekst op of na de huidige
positie van de aanwijzer.

:lm margin=3.:p.&larrow.Expand&colon.:hp5.percent:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode zet de EXPANSIE-code aan voor alle tekst op en na de huidige
positie van de aanwijzer.
De factor wordt uitgedrukt in een percentage; 100 geeft aan dat de normale
horizontale gestrektheid zoals die voor het huidige document geldt, wordt
aangehouden

:lm margin=3.:p.&larrow.TopMargin&colon.:hp5.afstand:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode bepaalt de grote van de bovenmarge vanaf de huidige plaats
in het document. Deze waarde heeft een relatie met de ondermarge.
De instelling heeft effect op de volgende pagina na de plaats van de code.

:lm margin=3.:p.&larrow.BottomMargin&colon.:hp5.afstand:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode bepaalt de grote van de ondermarge vanaf de huidige positie
van de aanwijzer. Deze code treedt in werking op de volgende pagina tenzij er
nog niets staat op de huidige pagina.

:lm margin=3.:p.&larrow.LeftMargin&colon.:hp5.afstand:ehp5.&rarrow.
:lm margin=5.:p.
Deze functie stelt de linkermarge in vanaf de huidige positie van de aanwijzer.
De functie treedt in werking op de volgende lijn tenzij niets op de huidige
lijn staat.

:lm margin=3.:p.&larrow.RightMargin&colon.:hp5.afstand:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode stelt de rechtermarge in vanaf de huidige positie van de aanwijzer.
De marge is afhankelijk van de linker kant van de pagina.
De functie treedt in werking op de volgende regel tenzij er nog niets op de
huidige regel staat.

:lm margin=3.:p.&larrow.LeftJustify&rarrow.
:lm margin=5.:p.
Deze functiecode zet de huidige horizontale uitlijning van de tekst naar links,
waardoor de regels naar links uitgelijnd zullen zijn.
De functie treedt in werking op de volgende regel tenzij er nog niets op de
huidige regel staat.

:lm margin=3.:p.&larrow.CenterJustify&rarrow.
:lm margin=5.:p.
Deze functiecode centreert de tekst tussen de linker en rechter marge.
De functie treedt in werking op de volgende regel tenzij er nog niets op de
huidige regel staat.

:lm margin=3.:p.&larrow.RightJustify&rarrow.
:lm margin=5.:p.
Deze functiecode zet de huidige horizontale uitlijning van de tekst naar rechts,
waardoor de regels naar rechts uitgelijnd zullen zijn.
De functie treedt in werking op de volgende regel tenzij er nog niets op de
huidige regel staat.

:lm margin=3.:p.&larrow.FullJustify&rarrow.
:lm margin=5.:p.
Deze functiecode geeft een horizontale uitlijning van de tekst naar zowel rechts
als rechts.
De functie treedt in werking op de volgende regel tenzij er nog niets op de
huidige regel staat.

:lm margin=3.:p.&larrow.CenterPage&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode centreert de tekst vertikaal op de pagina.
Wanneer actief aan het begin van de pagina het centreert die pagina.

:lm margin=3.:p.&larrow.TabSet&colon.:hp5.afstand:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode voegt een tab toe op de opgegeven horizontale afstand
aan de tab-tabel.

:lm margin=3.:p.&larrow.TabClear&colon.:hp5.afstand:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode haalt een tab weg van de opgegeven horizontale afstand
uit de huidige tab-tabel.

:lm margin=3.:p.&larrow.TabSpacing&colon.:hp5.afstand:ehp5.&rarrow.
:lm margin=5.:p.
Deze code geeft weer welke afstand er gebruikt moet worden tussen de tabulator
vanaf het begin van het document. Vanaf de linker margin worden tabs vanzelf op
gelijke afstand ingesteld vanaf het begin van het document. Deze code kan niet
voorkomen buiten het variabelen-gebied van de &apos.Preface&apos. functiecode.

:lm margin=3.:p.&larrow.Indent&colon.:hp5.afstand:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode geeft weet hoeveel bij een alinea ingesprongen moet worden.
Het begin van een alinea is of (1) de eerste lijn van een alinea of (2)
de eerste lijn na een nieuwe lijn code die het einde van een alinea weergeeft.

:lm margin=3.:p.&larrow.Metric&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode geeft aan of het document geformateerd is volgens de metrische
(centimeters) of de Engelse (duimen) maten. Als dit op &apos.1&apos. gezet is zullen de
metrische maten worden gebruikt.
Deze code kan niet voorkomen buiten het variabelen-gebied van de &apos.Preface&apos. functiecode.

:lm margin=3.:p.&larrow.Graphic&colon.:hp5.x,y,cx,cy[,Background][,NoTextAside][,Halftone][,Reverse][,PrinterOptimize][[,KeepAspectFixX|KeepAspectFixY]][,BorderWidth=n][,BorderColor=rrggbb][,BorderCaption="text"]]&colon.type&bsl.0data:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode definieert een grafische cel in het document.
:p.De &apos.x&apos. and &apos.y&apos. co”rdinaten bepalen de positie van de cel tot de linksboven hoek van de pagina. Documenten die vastgelegd zijn voor revisie 0.97 bepalen de plaats van de beelden naar de linksonder hoek van de pagina.
De &apos.cx&apos. en &apos.cy&apos. variabelen geven de breedte en hoogte van de cel aan.
Alle vier de variabelen worden aangeduid in het meetsysteem dat op dat moment
geldt (1/1000 inch of centimeters).
:p.Zowel de numerieke waarden als de woorden 
&apos.Inline&apos.,
&apos.LeftMargin&apos.,
&apos.RightMargin&apos., 
&apos.LeftEdge&apos., 
&apos.RightEdge&apos. en
&apos.Centered&apos. 
worden voor de X-positie geaccepteerd, en 
&apos.TopMargin&apos., 
&apos.BottomMargin&apos., 
&apos.TopEdge&apos., 
&apos.BottomEdge&apos. en
&apos.Centered&apos. 
worden geaccepteerd voor de Y-positie.  De woorden
&apos.EdgeToEdge&apos. en
&apos.MarginToMargin&apos. 
worden geaccepteerd voor of de X-of de Y-positie.
Als &apos.Inline&apos. wordt opgegeven voor de X-positie dan wordt
de Y-positie genegeerd.
Eveneens als &apos.EdgeToEdge&apos. of &apos.MarginToMargin&apos. 
wordt opgegeven voor X of Y dan zal de daarbij behorende positie 
genegeerd worden.
:p.De &apos.Background&apos., &apos.NoTextAside&apos., &apos.Halftone&apos. en &apos.Reverse&apos. variabelen zijn
optioneel. Zij zijn ook teken-gevoelig.
:p.De &apos.Background&apos. variabele, indien genoemd, geeft aan dat het grafische beeld niet
mag botsen met de tekst in het document en moet in de tekst als een achtergrond aanwezig zijn.
:p.De &apos.NoTextAside&apos. variabele, indien vermeld, geeft aan dat geen tekst aan elk van
de zijden van de grafische beeld mag worden geplaatst. Het beeld wordt dan beschouwd als
breedte vullend.
:p.De &apos.Halftone&apos. variabele, indien genoemd, geeft aan dat het grafische beeld in
lichte kleuren moet worden afgebeeld.  Meestal wordt dit gedaan door slechts om de andere
pixel in te kleuren en wordt daarom niet ondersteund door de Metabestand beelden.
:p.De &apos.Reverse&apos. variabele, indien dit is aangegeven, geeft dit aan dat het beeld 
weergegeven moet worden in omgekeerde kleuren. Dit wordt niet ondersteund voor Metabestand beelden.
:p.De &apos.PrinterOptimize&apos. variabele geeft, als die aangegeven is,
aan dat de het grafische beeld geoptimaliseerd is om door de drukker afgedrukt te worden.
Dit wordt gedaan door het beeld op zo'n manier in te schalen dat de BITMAP inschaalfactor
steeds een geheel getal is. Dit wordt niet ondersteund door Meta-bestandbeelden.
:p.De &apos.KeepAspectFixX&apos. variabele geeft aan dat de breedte en de lengte van het beeld
voldoet aan de originele verhoudingen.
:p.De &apos.KeepAspectFixY&apos. variabele werkt op de zelfde manier, echter de breedte van het beeld
wordt bepaald door de hoogte van het originele beeld.
:p.The &apos.BorderWidth=n&apos. parameter declares that the graphic border will be a specific
size in thousandths of an inch or centimeter, depending on the document metric type.
:p.The &apos.BorderColor=rrggbb&apos. parameter declares the red-blue-green color of the graphic border.
:p.The &apos.BorderCaption="text"&apos. parameter declares the graphic caption text.  This text 
is literal, except that all embedded double-quote characters are doubled.
:p.De &apos.Type&apos. veld kan op dit moment gezet worden op&colon.
&apos.BM&apos. (for bitmap), naar
&apos.MF&apos. (for metafile), naar
&apos.UP&apos. (for UPC-Code), naar
&apos.39&apos. (for Code-39), naar
&apos.PN&apos. (for POSTNET). naar
&apos.E8&apos. (for EAN-8), of naar
&apos.EA&apos. (for EAN-13), of naar
&apos.27&apos. (for Codabar (2 of 7)), of naar
&apos.25&apos. (for Code 2 of 5) of naar
&apos.28&apos. (for Code-128).
Alle BMP, GIF, JPG, PCX, TIF en dergelijke beelden worden intern opgeslagen in OS/2 bitmap formaat.
Dit veld wordt afgesloten met nullen.
:p.Binaire informatie over de grafische gegevens worden achter de nul-afsluiting van
de kop-informatie. Deze binaire gegevens hebben de 15, 16 en 17 escape codes
als eerder besproken.
:p.Bij bitmaps heeft de binaire informatie eerst een 4-byte getal die de grootte van
de bitmap kopinformatie aangeeft. Hierop volgt een 4-byte getal die de grootte van
de bitmap zelf aangeeft. Hierna volgt de Bitmap kopinformatie en dan de bitmap gegevens.
:p.Bij Metabestanden, geeft de binaire informatie een 4-byte getal die de grootte van de
Metabestands-gegevens en dan de Metabestandsgegevens zelf.
:p.Bij UPC-Codes is de het gegeven aan 12-cijferige tekst die de UPC-code zelf bevat.
:p.Voor Code-39 barcodes, kunnen de gevens van elke lengte zijn, maar de tekenset is beperkt
tot de hoofdletters A-Z, de cijfers 0-9, spatie,
&apos.&dot.&apos.,
&apos.&plus.&apos.,
&apos.&dash.&apos.,
&apos.&asterisk.&apos.,
&apos.&slash.&apos.,
&apos.&dollar.&apos. and
&apos.&percent.&apos..
:p.Voor POSTNET-barcodes, is dit een 5- of 9-positie tekst met alleen cijfers.
:p.EAN-8 barcodes bestaan uit een 7-positie tekstveld met alleen cijfers.
:p.EAN-13 barcodes bestaan uit een 12-positie tekstveld met alleen cijfers.
:p.For Codabar codes, the data can be any number and combination of the characters 
0 to 9, &dash., &dollar., &colon., &slash., &dot. or &plus., and the first and last characters must be identical
and in the range A to D.

:lm margin=3.:p.&larrow.Date&colon.Fixed&colon.:hp5.tijd:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode genereert tekst, die een bepaalde datum, weergegeven in
seconden waarbij 00&colon.00, 1 januari 1970.  Het formaat van de tekst is
gedefinieerd in de &apos.DateFormat&apos. functie.

:lm margin=3.:p.&larrow.Date&colon.Current&rarrow.
:lm margin=5.:p.
Deze functiecode genereert tekst, die de huidige datum weergeeft volgens de
kalender van de computer.
Het formaat van de tekst is gedefinieerd in de &apos.DateFormat&apos. functie.

:lm margin=3.:p.&larrow.Date&colon.Changed&rarrow.
:lm margin=5.:p.
Deze functie genereert tekst, die de datum aangeeft waarop het document voor het
laatst gewijzigd is.
Het formaat van de tekst is gedefinieerd in de &apos.DateFormat&apos. functie.

:lm margin=3.:p.&larrow.DateFormat&colon.:hp5.formaatreeks:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode geeft een tekstreeks die wordt gebruikt om de plaats van de
code een sjabloon te gebruiken om volledige datumreeksen te ontwikkelen.
Het is gelijk aan een &apos.C&apos. &apos.printf&apos. formaatreeks, behalve dat
codes die de &apos.&percent.&apos. teken volgen als volgt zijn&colon.
:lm margin=10.:sl compact.
:li.m - Numerieke Maand (1,2,etc)
:li.M - Tekst Maand (januari, februari, etc)
:li.n - Tekst Maand (jan, feb, etc)
:li.d - Numerieke Dag
:li.y - Numeriek Jaar (met eeuwinformatie)
:li.z - Numeriek Jaar (2 cijfers, zonder eeuw)
:li.w - Tekst Weekdagen (zondag, maandag, etc)
:li.x - Tekst Dag (zon, maa, etc)
:esl.:lm margin=5.
:note.Dit interne formaat volgt niet de taal van het land.  De code die de
gebruiker ziet kan van taal tot taal verschillen.

:lm margin=3.:p.&larrow.LineSpacing&colon.:hp5.percent:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode stelt het percentage in van de normale regelafstand welke in
het document gebruikt moet worden vanaf de positie in het document waar de code
staat.

:lm margin=3.:p.&larrow.JobSettings&colon.:hp5.gegevens:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode bevat de variabelen die de gehele drukkertaak-instellingen
teruggeven van de drukkertaak-insteldialoog.  De gehele structuur van deze
gegevensreeks hangt af van de gegevens die een drukker-sturingsprogramma
terugstuurt en zou wel eens niet gedocumenteerd kunnen zijn. Toch hebben  de
gegevens een standaardstruktuur, die door de IBM gedocumenteerd is.
Deze gegevens worden geschreven met de 15, 16 en 16 escape, zal het vaak een
binaire struktuur hebben. Deze code kan niet voorkomen buiten het
variabelen-gebied van de &apos.Preface&apos. functiecode.

:lm margin=3.:p.&larrow.PageNumber&rarrow.
:lm margin=5.:p.
Deze functiecode genereert tekst die het huidige paginanummer in het document
weergeeft.

:lm margin=3.:p.&larrow.PageCount&rarrow.
:lm margin=5.:p.
Deze functiecode genereert tekst die het huidige aantal pagina&apos.s van het
document weergeeft

:lm margin=3.:p.&larrow.Glyph&colon.:hp5.nummer:ehp5.&rarrow.
:lm margin=5.:p.
De functiecode genereert een enkele tekst-teken waarvan zijn numerieke waarde
afhangt van het gegeven getal. Het weergegeven teken hangt van van de
code-page die in gebruik is.
De functiecode wordt gemaakt als de Alt-nnn techniek gebruikt wordt met behulp
van de toetsen op het numerieke toetsenbord. Dit elimineert de verwarring over
het al of niet toepassen van de terug- of regelopschuivings-code als voorbeeld.

:lm margin=3.:p.&larrow.Author&colon.:hp5.name:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode verschijnt alleen in de voorwoordsectie en dient om de naam van de auteur van het document vast te leggen.

:lm margin=3.:p.&larrow.CreateDate&colon.:hp5.time:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode komt alleen in de voorwoord-sectie van een document voor om de aanmaakdatum van het documtn vast te leggen. Het wordt op de zelfde manier vastgelegd als de andere datums, d.w.z. in seconden sinds 1 januari 1970.

:lm margin=3.:p.&larrow.Comment&colon.:hp5.commentary:ehp5.&rarrow.
:lm margin=5.:p.
Deze functiecode kan worden gebruikt om commentaar aan het document toe te voegen. Het commentaar wordt niet afgebeeld noch afgedrukt wanneer men met het document zelf werkt.

:lm margin=3.:p.&larrow.Separator&rarrow.
:lm margin=5.:p.
Deze functie-code veroorzaakt een lijnafbreking, gevolgd door een
scheidinglijn, gevolgd door een andere lijnafbreking. Het doet het zelfde
als een <hr> in de HTML-taal.

:lm margin=3.:p.&larrow.Title&colon.:hp5.Naam:ehp5.&rarrow.
:lm margin=5.:p.
Deze code verschijnt alleen in het Preface-deel een dient om
de naam van het document aan te geven.

:lm margin=3.:p.&larrow.Header&colon.:hp5.text:ehp5.&rarrow.
:lm margin=5.:p.
Deze functie-code bepaalt de tekst welke van nu af in de kop van de pagina
zal worden gebruikt bij de opmaak van de pagina&apos.s Als nog niets op de
huidige pagina staat zal het ook voor die pagina gebruikt worden.

:lm margin=3.:p.&larrow.Footer&colon.:hp5.text:ehp5.&rarrow.
:lm margin=5.:p.
Deze functie-code bepaalt de tekst welke van nu af in de voet van de pagina
zal worden gebruikt bij de opmaak van de pagina&apos.s. Als nog niets op de
huidige pagina staat zal het ook voor die pagina gebruikt worden.

:lm margin=1.

:note.Afstanden worden aangegeven in eenheden van 0,001 inches of centimeters,
afhankelijk of je Engelse of metrische maten in het document gebruikt.
Een afstand van een halve inch of centimeter wordt daarom uitgedrukt als 500.
In het algemeen worden alleen maar positieve afstanden gebruikt.

:note.Percentages worden uitgedrukt in gehele getallen met een aangenomen
decimale komma. b.v. 100&percent. wordt uitgedrukt met het getal 100, 125&percent. als 125,
en zo voort. In het algemeen worden allen positieve percentages gebruikt.

:h2 res=61002.Code Pages
De volgende code-pages zijn geldig&colon.

:table cols='4 60' rules=both frame=box.
:row.:c.037     :c.Brazili‰ / Portugees / U.S. Engels (EBCDIC)
:row.:c.273     :c.Oostenrijks / Duits (EBCDIC)
:row.:c.274     :c.Belgisch (old EBCDIC)
:row.:c.277     :c.Deens / Noors (EBCDIC)
:row.:c.278     :c.Fins / Zweeds (EBCDIC)
:row.:c.280     :c.Italiaans (EBCDIC)
:row.:c.282     :c.Portugees (old EBCDIC)
:row.:c.284     :c.Spaans (EBCDIC)
:row.:c.285     :c.U.K. Engels (EBCDIC)
:row.:c.297     :c.Frans (EBCDIC)
:row.:c.437     :c.Verenigede Staten (IBM PC)
:row.:c.500     :c.Belgisch / Internationaal (EBCDIC)
:row.:c.708     :c.Arabic (ASMO 708)
:row.:c.709     :c.Arabic (ASMO 449, BCON V4)
:row.:c.710     :c.Arabic (transparent Arabic)
:row.:c.711     :c.Arabic (Nafitha Enhanced)
:row.:c.720     :c.Arabic (transparent ASMO)
:row.:c.813     :c.Greek
:row.:c.819     :c.Veeltalig (ISO-8859/1)
:row.:c.850     :c.Veeltalig (Latin1)
:row.:c.852     :c.Slavisch (Latin2)
:row.:c.855     :c.Serbia / Montenegro / Macedonia / Bulgaria
:row.:c.857     :c.Turks
:row.:c.860     :c.Portugees
:row.:c.861     :c.IJslands
:row.:c.862     :c.Hebreeuws
:row.:c.863     :c.Frans-Canadees
:row.:c.864     :c.Arabisch
:row.:c.865     :c.Deens / Noors
:row.:c.866     :c.Russia
:row.:c.869     :c.Greek
:row.:c.870     :c.Tsjecho-Slowaaks/ Hongaars/Pools/Joegoslavisch (EBCDIC)
:row.:c.871     :c.IJslands (EBCDIC)
:row.:c.874     :c.Thailand
:row.:c.915     :c.Bulgaria / Macedonia
:row.:c.932     :c.Japans (Shift JIS X-0208)
:row.:c.934     :c.Koreaans
:row.:c.936     :c.Eenvoudig Chinees
:row.:c.938     :c.Traditioneel Chinees
:row.:c.942     :c.Japans (SAA)
:row.:c.944     :c.Koreaans (SAA)
:row.:c.946     :c.Eenvoudig Chinees (SAA)
:row.:c.948     :c.Traditioneel Chinees (SAA)
:row.:c.949     :c.Koreaans (Shift KSC 5601)
:row.:c.950     :c.Traditioneel Chinees (Big5)
:row.:c.1004    :c.Bureau Uitgifte (DP)
:row.:c.1026    :c.Turks (EBCDIC)
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

:note.Niet alle codepages zullen in alle systemen gebruikt kunnen worden.

:h2 res=61003.Toevoegings Interface
De toevoegings-interface zoekt in Escriba&apos.s installatie-directory wanneer
het programma begint naar alle DLL-bestanden. Elk van deze DLLs worden nagegaan
op de aanwezigheid van de functies &apos.Identify&apos., &apos.ProductName&apos.,
&apos.ProductVersion&apos., &apos.ProductInformation&apos. and &apos.ProductConfigure&apos..
Als al deze functies aanwezig zijn, wordt zo&apos.n module beschouwd als een soort
toevoeging-module.

:p.De prototypefunkties voor de &apos.C&apos. programmataal zijn&colon.

:font facename=Courier size=13x8.:lm margin=6.:sl compact.
:li.char* APIENTRY Identify ( void ) ;
:li.BOOL  APIENTRY SetLanguage ( char *Language ) ;
:li.char* APIENTRY ProductName ( void ) ;
:li.int   APIENTRY ProductVersion ( void ) ;
:li.void  APIENTRY ProductInformation ( HWND Owner ) ;
:li.int   APIENTRY ProductConfiguration ( int MaxPages, char Names[][64], char MajorTabs[][32], char MinorTabs[][32], PFNWP Processors[], HMODULE Libraries[], int DialogIDs[] ) ;
:esl.
:lm margin=1.:font facename=default.

:p.De identificeer-functie geeft een tekenreeks met als laatste een loos teken die
de uitbreidingtype identificeert. Op dit moment zijn die types
:link reftype=hd res=61004."ISPELLER":elink.
,
:link reftype=hd res=61005."IMPORT/EXPORT":elink.
en
:link reftype=hd res=61006."GRAPHICIMPORT":elink.
gedefinieerd.

:p.De TaalInstel-functie maakt het voor Escriba mogelijk de aanvullingen te beschrijven
in de eigen taal. De naam van de taal in die taal wordt gebruikt.
Bij voorbeeld het woord Frans is in het Frans, Fran&cc.ais.

:p.De ProduktNaam-functie geeft een tekenreeks met aan het eind een loos teken
die de type uitbreiding aangeeft in het ProduktInformatie-menu.
Deze naam mag geen versienummer bevatten.

:p.De ProduktVersie-functie geeft een geheel getal dat het versienummer
weergeeft vermenigvuldigd met 100. Dit wordt ook gedaan voor het Produkt-
Informatie-menu.

:p.De ProduktInformatie-functie laat een dialoog zien die de gebruiker
de volle naam geeft van het produkt, het versienummer, en het auteurschap en
de kopierechten-informatie die op het produkt betrekking heeft.  Deze functie
zal opgeroepen worden wanneer de gebruiker het produkt kiest uit de Produkt-
Informatie-menu.

:p.De ProduktConfiguratie-functie geeft informatie die gebruikt wordt bij het samenstellen
van de Configuratie-uitbreiding-dialoog. Deze functie hoeft niet aanwezig te zijn,
maar wanneer het aanwezig is, wordt verwacht dat het het nummer van de configuratie
notitieboek doorgeeft. Voor elk van die pagina&apos.s (tot het maximum
dat in de functie-oproep aangegeven is), een paginenaam, een hoofd-tab-tekst of een lagere-
tab-tekst (de een of de ander), een dialoog-processor-functie, een middelen-bibliotheek-
handle en een middelen-ID voor het dialoog-masker moeten alle terug gemeld worden.

:h3 res=61004.ISpeller Interface
De module "ISPELLER" is ontwikkeld uit het GNU ISPELL-programma, versie 3.1.18, en
daar het freeware is, de broncode inbegrepen. De copyrechten zijn eigendom van
Paul Willison en Geoff Keunning. ISPELLER zelf is een eenvoudige verpakking rond ISPELL,
aar aangepast aan de OS/2 Dynamic Link Library (DLL). ISPELLER kan geen gebruik maken van
enig ander versie van ISPELL 3.1.x op je systeem, maar -kan- gebruik maken van de
woordenboek-bestanden er van.
Om dit te doen, voeg daartoe de definitie&apos.SET ISPELL=pad&apos. in je CONFIG.SYS, waar
&apos.pad&apos. het geldige pad is waar de ISPELL directory-bestanden zich bevinden.
De modules ISPELLER.DLL en BUILDHAS.EXE moeten ook overgenomen worden in die directory.

:p.Bovendien worden de basis-funkties van een toevoeging door ISPELLER geleverd&colon.

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

:p.Degenen die ge‹nteresseerd zijn in deze functies, en hoe ze werken, worden verwezen
naar de bron-code.

:h3 res=61005.Document Import/Export Module Interface
Modules van het type "IMPORT/EXPORT" worden gebruikt om documenten te importeren die
niet van Escriba&apos.s oorspronkelijke formaat zijn, en om documenten te exporteren
die niet van Escriba&apos.s oorspronkelijke formaat zijn.

:p.Naast de basis-functies van de toevoeging worden de volgende functies
door Import/Export geleverd&colon.

:font facename=Courier size=13x8.:lm margin=6.:sl compact.
:li.char* APIENTRY QueryFileType ( void ) ;
:li.char* APIENTRY QueryWildcard ( void ) ;
:li.BOOL  APIENTRY Filter ( char *Name, char *Type ) ;
:li.BOOL  APIENTRY Verify ( HWND Owner, char *Name ) ;
:li.void* APIENTRY Import ( HWND Owner, char *Name, long *Size ) ;
:li.BOOL  APIENTRY Export ( HWND Owner, char *Name, void *Text, long Size ) ;
:esl.:lm margin=1.:font facename=default.

:p.De QueryFileType-functie geeft een uitgebreid bestandtype (zoals &apos.Escriba&apos.s
of &apos.Platte Tekst&apos.) die getoond zullen worden in het Bestand Open dialoog
bestandtype combobox, rechts onder de bestandnaam invoerveld.

:p.De QueryWildCard-functie geeft een wildcard bestanddefinitie (zoals &apos.*.TXT&apos.)
die automatisch in het Bestand Open Dialoog wordt gevoerd in het bestandinvoerveld.
als het bestandtype wordt gekozen uit de bestandtype combobox.

:p.De Filter-functie wordt opgeroepen als de Bestand Open Dialoog bestands
lijst wordt gevuld en wordt gebruikt om vast te stellen of er wel of niet een
bestandsnaam in de lijst staat. Dit is normaal een controle op de naam van het
bestand en zijn extensie, en geen controle op de inhoud van het bestand zelf.

:p.De Verify-functie wordt opgeroepen om een laatste controle uit te oefenen
op het type van het bestand vlak voordat het ingelezen wordt in de tekstverwerker.
Deze controle moet snel zijn, maar ook grondiger dan door de filter-functie.

:p.De Import-functie laadt een document in het geheugen, zet het om naar het
oorspronkelijke Escriba-formaat, en geeft de complete tekst terug.
De Import-functie wordt geacht het geheugen voor de tekst vast te leggen.
Escriba zal dit geheugen vrijgeven als het er mee klaar is.

:p.De Export-functie stelt het document veilig. Het document wordt naar de
exporteur gezonden in Escriba-formaat, en wordt omgezet en naar het opgegeven
bestand geschreven..

:h3 res=61006.Grafische Import Module Interface
Modules van het type "GRAPHICIMPORT" worden gebruikt om grafische beelden te
importeren, en om te zetten naar OS/2 system-bitmap in het geheugen.

:p.Behalve de basis-functies van de toevoeging worden de volgende functies
ook door de Grafische Import Modules geleverd&colon.

:font facename=Courier size=13x8.:lm margin=6.:sl compact.
:li.char*   APIENTRY QueryFileType ( void ) ;
:li.char*   APIENTRY QueryWildcard ( void ) ;
:li.BOOL    APIENTRY Filter ( char *Name, char *Type ) ;
:li.BOOL    APIENTRY Verify ( HWND Owner, char *Name ) ;
:li.HBITMAP APIENTRY GraphicImport ( HWND Owner, char *Name ) ;
:esl.:lm margin=1.:font facename=default.

:p.De QueryFileType-functie geeft een uitgebreide bestandtype (zoals &apos.s.Bitmap&apos.
of &apos.Metafile&apos.) die in het Grafisch Bestand Open dialoog getoond worden bij
bestandtype combobox, vlak onder de bestandnaam-invoerveld.

:p.De QueryWildCard-functie geeft een wildcard-bestandspecificatie (zoals &apos.*.BMP&apos.)
die automatisch geladen wordt in de Grafisch Bestand Open dialoog bestand-invoerveld
terwijl de bestandtype uit de Bestandtype combobox geselecteerd wordt.

:p.De Filter-functie wordt opgeroepen als de Grafisch Bestand Open dialoog gevuld
wordt en wordt gebruikt om te bepalen of de naam van het bestand al dan niet in de
lijst vermeld moet worden. Dit is allen maar een kleine controle op de bestandnaam
en extensie-type, het is geen controle op de inhoud van het bestand zelf.

:p.De Verify-functie wordt opgeroepen om een laatste controle op het bestandtype
uit te voeren voordat het ingevoerd wordt in de tekstverwerker. Deze controle moet
snel zijn maar grondiger als die in de Filter-functie.

:p.De GraphicImport-functie laadt het grafische bestand in het geheugen en zet het om
in een oorspronkelijke OS/2 bitmap-formaat, en geeft de handle over naar de bitmap.

:euserdoc.
