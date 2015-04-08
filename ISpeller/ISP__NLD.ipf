#define LOCAL_DEFINES
#include "ISpeller.h"
:userdoc.
:title.ISpeller Spellingcontrole Hulp
:docprof toc=123456.

:h1 res=IDD_PRODUCTINFO.Productinformatie (Dialoog)
Dit dialoog toont de naam, het ikoon en de kopierechten-informatie
van het programma.  Druk op de Enter, spatie of de Escape toets of de OK-toets
om de dialoog te be‰indigen.

:h2 res=IDD_CONFIGURE.Instelling spellingcontrole (Dialoog)
Door middel van dit dialoog kun je kiezen welk woordenboek je wilt gebruiken met
de spellingcontrole, nieuwe woordenboeken bouwen, of oude woordenboeken
verwijderen.
:p.De woordenboeken die beschikbaar zijn worden in het dialoogvenster getoond.
Om een te kiezen, moet je op de naam van het woordenboek klikken en dan op Enter
drukken. Als om de een of andere reden dit woordenboekbestand niet geopend kan
worden, wordt je dit medegedeeld. In het andere geval zal het dialoogvenster
sluiten en wordt je naar de tekstverwerker geleid.
:p.Om een nieuw woordenboek te bouwen, klik op de Bouw drukknop.
Het :link reftype=hd res=IDD_BUILD.Toevoegen woordenboek:elink.
dialoogvenster wordt getoond, zodat je een nieuw woordenboek kunt opbouwen.
:p.Om een woordenboek van het systeem te verwijderen, klik op de naam van het
bestand en daarna op de Verwijder drukknop.

:h3 res=IDD_BUILD.Toevoegen woordenboek (Dialoog)
Door middel van dit dialoog is het mogelijk een nieuw woordenboek te bouwen die
je met de spellingcontrole kunt gebruiken.
:p.Om een nieuw woordenboek op te bouwen, heb je twee bestanden nodig&colon. een
woordenlijst en een taalafhankelijk &apos.affix&apos. bestand.
:p.De woordenlijst kan een eenvoudige lijst van woorden zijn, een woord per lijn
in tekst formaat, of het kan een lijst waarnaar als ISpell gerefereerd wordt,
een bestaand bestand, waarin woorden op een speciale manier zijn gemerkt om
aan te geven hoe zij door voor- en achtervoegingen veranderd kunnen worden. Er
zijn geen regels hoe een dergelijk bestand moet worden genoemd, maar de naam van
het land is meestal een onderdeel van de bestandsnaam.
:p.Het &apos.affix&apos. bestand bevat de regels waarop woorden kunnen worden veranderd en
zijn uniek voor elke taal. De standaard uitgang voor affix bestanden is &apos..aff&apos.
:p.Om een nieuw woordenboek te bouwen moet je de naam van een bestaande
woordenlijst van een bestaand &apos.affix&apos. bestand, en van de naam van het
woordenboek dat je wilt aanmaken.  Wanneer al deze namen ingegeven zijn, kun je
op de &apos.OK&apos. toets drukken en als de namen juist zijn zal het bouwen beginnen. Dit
zal enkele tellen duren waarna je
:link reftype=hd res=IDD_CONFIGURE.Instellen spellingcontrole:elink. dialoog, van
waaruit je het nieuwe woordenboek kunt kiezen.  Als er zich bij het bouwen van
het nieuwe woordenboek een probleem heeft voorgedaan, wordt je daarover
ge‹nformeerd en kun je veranderingen aanbrengen voordat je weer een woordenboek
gaat aanmaken.
:p.Om uit het dialoog voor het bouwen van een woordenboek te komen, druk of klik
je op de Escape toets.

:euserdoc.