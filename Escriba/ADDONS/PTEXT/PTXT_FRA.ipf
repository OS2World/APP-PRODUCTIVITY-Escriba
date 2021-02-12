#include "PText.h"
:userdoc.
:title.Aide sur l'importation/exportation de texte brut
:docprof toc=123456.

:h1 res=IDD_PRODUCTINFO.Infos produit
Cette boŒte de dialogue affiche le nom, le num‚ro de version, la date et
le copyright associ‚s … ce module. Pressez :hp2.Entr‚e:ehp2. ou s‚lectez
:hp2.Ok:ehp2. pour fermer la boŒte de dialogue.

:h1 res=IDD_CONFIGURE_IMPORT.Import Configuration
Cette page de configuration permet de d‚finir les options d'importation
de texte brut. Les options actuellement disponibles sontÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_FONT.Police:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_SIZE.Taille:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_METRIC.Utiliser systŠme m‚trique:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_SOFTBREAKS.Ignorer sauts de ligne:elink.
:esl.:p.
Cette boŒte de dialogue est toujours affich‚e dans le bloc-notes de
configuration des extensions. Les choix effectu‚s seront appliqu‚s quand
vous aurez s‚lect‚ :hp2.Ok:ehp2.. Si vous s‚lectez :hp2.Annuler:ehp2.,
les modifications ne seront pas enregistr‚es. Les options sont
enregistr‚es par le module d'extension dans le fichier PTEXT.INI, qui se
trouve dans le r‚pertoire o— PTEXT est install‚.

:h2 res=IDD_CONFIGURE_FONT.Police
Vous pouvez ici choisir la police qui sera utilis‚e pour l'importation
de texte brut, les documents en texte brut n'ayant pas de police d‚finie
intrinsŠquement. Seules les polices … espacement fixe sont list‚es ici,
les textes bruts ‚tant … espacement fixe par d‚finition.

:h2 res=IDD_CONFIGURE_SIZE.Taille
Vous pouvez ici choisir la taille de police qui sera utilis‚e pour
l'importation de texte brut, les documents en texte brut n'ayant pas de
taille de police d‚finie intrinsŠquement. La taille standard d'une
machine … ‚crire est de 10 points.

:h2 res=IDD_CONFIGURE_METRIC.Utiliser systŠme m‚trique
Si cette case est coch‚e, le module d'importation utilisera le systŠme
m‚trique (centimŠtres) lors de l'importation du document.

:h2 res=IDD_CONFIGURE_SOFTBREAKS.Ignorer sauts de ligne
Si cette case est coch‚e, les sauts de ligne simples seront ignor‚s lors
de l'importation du document. Les sauts de ligne doubles seront trait‚s
normalement.

:h1 res=IDD_CONFIGURE_EXPORT.Configuration de l'exportation
Cette page de configuration permet de d‚finir les options d'exportation 
de texte brut. Les options actuellement disponibles sontÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_FORMAT.Formater le r‚sultat:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_HEIGHT.Lignes par page:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_WIDTH.Largeur:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_BOLD.Simuler texte gras:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_UNDERSCORE.Simuler texte soulign‚:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_STRIKEOUT.Simuler texte barr‚:elink.
:esl.:p.
Cette boŒte de dialogue est toujours affich‚e dans le bloc-notes de
configuration des extensions. Les choix effectu‚s seront appliqu‚s quand
vous aurez s‚lect‚ :hp2.Ok:ehp2.. Si vous s‚lectez :hp2.Annuler:ehp2.,
les modifications ne seront pas enregistr‚es. Les options sont
enregistr‚es par le module d'extension dans le fichier PTEXT.INI, qui se
trouve dans le r‚pertoire o— PTEXT est install‚.

:h2 res=IDD_CONFIGURE_FORMAT.Formater le r‚sultat
Si cette case est coch‚e, le fichier r‚sultat sera organis‚ en pages et
lignes, en utilisant les donn‚es hauteur et largeur sp‚cifi‚es sur cette
page. Dans le cas contraire, le fichier r‚sultat ne contiendra que les
sauts de ligne pr‚sents dans le document original.

:h2 res=IDD_CONFIGURE_HEIGHT.Lignes par page
Ce champ vous permet de d‚finir le nombre de lignes dans une page de
texte, lors de l'exportation. Les valeurs comprises entre 40 et 80 sont
autoris‚es.

:h2 res=IDD_CONFIGURE_WIDTH.Largeur
Ce bouton vous permet de d‚finir la largeur du document texte lors de
l'exportation. Les valeurs comprises entre 40 et 160 colones sont
autoris‚es.

:h2 res=IDD_CONFIGURE_BOLD.Simuler texte gras
Si cette case est coch‚e, le module d'exportation simulera le texte gras
en imprimant un caractŠre retour-arriŠre, suivi d'une r‚p‚tition du
caractŠre original.

:h2 res=IDD_CONFIGURE_UNDERSCORE.Simuler texte soulign‚
Si cette case est coch‚e, le module d'exportation simulera le texte
soulign‚ en imprimant le caractŠre … souligner, un retour-arriŠre puis
le caractŠre de soulignement.

:h2 res=IDD_CONFIGURE_STRIKEOUT.Simuler texte barr‚
Si cette case est coch‚e, le module d'exportation simulera le texte
barr‚ en imprimant le caractŠre … barrer, un retour-arriŠre puis
le caractŠre ®-¯.

:euserdoc.
