#include "PText.h"
:userdoc.
:title.Platte Tekst Import/Export Hulp
:docprof toc=123456.

:h1 res=IDD_PRODUCTINFO.Product-Informatie
Dit dialoog laat de naam, revisie, datum en de kopierecht-informatie
voor dit module zien. Druk op Enter of klik op de OK-toets om dit
dialoog af te breken.

:h1 res=IDD_CONFIGURE_IMPORT.Import-Configuratie
Deze configuratie-pagina laat de gebruiker de diverse opties instellen
om een platte-tekst document te importeren.
De huidige opties zijn&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_FONT.Lettertype:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_SIZE.Afmeting:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_METRIC.Gebruik metrische waarden:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_SOFTBREAKS.Let niet op lijafbrekingen:elink.
:esl.:p.
Dit dialoog wordt altijd getoond als een pagina van de Configureer
Uitbreidingen dialoog notitieboek. De ingevoerde informatie zal ingaan
zodra je de OK-toets van dit dialoog aanklikt.
Als je op de Escape-toets drukt zal je informatie geen effect hebben.
In ieder geval zullen de instellingen door de add-on uitbreiding zelf
worden opgeslagen in het bestand PTEXT.INI, die zich in de directory bevindt
waar PTEXT is ge�nstalleerd.

:h2 res=IDD_CONFIGURE_FONT.Instellen Lettertype
De combinatie invoerveld en lijst geeft je de mogelijkheid om te
bepalen welk van de ge�nstallleerde lettertypen gebruikt gaat worden
voor de ge�mporteerde platte tekst.
Platte tekst heeft geen lettertype-code. Alleen niet-proportionele
lettertypen zijn hier weergegeven, daar platte tekst per definitie geen
proportionele lettertype kent.

:h2 res=IDD_CONFIGURE_SIZE.Instellen Grootte
De combinatie invoerveld en lijst geeft je de mogelijkheid om te
bepalen uit hoeveel punts-letter gebruikt gaat worden voor de ge�mporteerde
platte tekst, daar platte tekst per definitie geen puntenwaarde bevat.
Voor standaard schrijfmachine-tekst gebruik 10 punten.

:h2 res=IDD_CONFIGURE_METRIC.Gebruik metrische waarden
Wanneer deze toets is gemerkt zal bij het importeren metrische waarden (centimeters)
worden gebruikt voor het opstellen van het document.

:h2 res=IDD_CONFIGURE_SOFTBREAKS.Let niet op Lijnafbrekingen
Als dit vak aangemerkt is, zal bij het importeren de enkele lijnafbrekingen
genegeerd worden.

:h1 res=IDD_CONFIGURE_EXPORT.Export Configuration
Deze configuratie-pagina laat de gebruiker de diverse opties instellen
om een platte-tekst document te exporteren.
De huidige opties zijn&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_FORMAT.Format output:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_HEIGHT.Hoogte:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_WIDTH.Breedte:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_BOLD.Simuleer zware tekst:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_UNDERSCORE.Simuleer understreepte tekst:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_STRIKEOUT.Simuleer doorstreepte tekst:elink.
:esl.:p.
Dit dialoog wordt altijd getoond als een pagina van de Configureer
Uitbreidingen dialoog notitieboek. De ingevoerde informatie zal ingaan
zodra je de OK-toets van dit dialoog aanklikt.
Als je op de Escape-toets drukt zal je informatie geen effect hebben.
In ieder geval zullen de instellingen door de add-on uitbreiding zelf
worden opgeslagen in het bestand PTEXT.INI, die zich in de directory bevindt
waar PTEXT is ge�nstalleerd.

:h2 res=IDD_CONFIGURE_FORMAT.Geef Output Vorm
Als dit vak aangemerkt is zal bij het bij het exporteren de tekst worden
opgeslagen op basis van de Hoogte en Breedte van de pagina zoals is
bepaald in de dialoog-pagina. In het andere geval zal de regel alleen
afgebroken worden als er een nieuwe-regel-tekens worden gevonden in het
originele document

:h2 res=IDD_CONFIGURE_HEIGHT.Instellen Hoogte
Deze draaiknop geeft je de mogelijkheid om aan te geven hoeveel lijnen gebruikt
zullen worden wanneer de tekst wordt gereedgemaakt voor exporteren.
Waarden van 40 tot 80 zijn toegestaan.

:h2 res=IDD_CONFIGURE_WIDTH.Instellen Breedte
Deze draaiknop geeft je de mogelijkheid om te bepalen uit hoeveel
kolommen de tekst zal beslaan bij export. Waarden van 40 tot 160 kunnen
gebruikt worden.

:h2 res=IDD_CONFIGURE_BOLD.Simuleer Zware Tekst
Als deze toets gemerkt is zal bij het exporteren van het document zware tekst
gesimuleerd worden door een terugspatie in te voeren gevolgd door een herhaling
van het teken.

:h2 res=IDD_CONFIGURE_UNDERSCORE.Simuleer Onderstreepte Tekst
Als deze toets gemerkt is zal bij het exporteren van het document de tekst understreept
worden door een terugspatie in te voeren gevolgd door een understreping.

:h2 res=IDD_CONFIGURE_STRIKEOUT.Simuleer Doorstreepte Tekst
Als deze toets gemerkt is zal bij het exporteren van het document de tekst doorstreept
worden door een terugspatie in te voeren gevolgd door een doorstreping.

:euserdoc.                                                                                                                                      