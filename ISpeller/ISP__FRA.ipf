#define LOCAL_DEFINES
#include "ISpeller.h"
:userdoc.
:title.Aide sur le correcteur orthographique ISpell
:docprof toc=123456.

:h1 res=IDD_PRODUCTINFO.Infos produit (Dialogue)
Cette boŒte de dialogue affiche le nom du programme, son ic“ne et la
notice de Copyright. Pour quitter la boŒte de dialogue, pressez la
touche :hp2.Entr‚e:ehp2., la barre d&apos.espace ou la touche
:hp2.chap:ehp2., ou s‚lectez la plaquette :hp2.Ok:ehp2. … l&apos.aide 
de la souris.

:h2 res=IDD_CONFIGURE.Configuration du correcteur orthographique (Dialogue)
Cette boŒte de dialogue vous permet de choisir le fichier dictionnaire …
utiliser par le correcteur orthographique, de cr‚er de nouveaux
dictionnaires et de supprimer de vieux dictionnaires. :p.Les fichiers
dictionnaires disponibles sont list‚s dans la boŒte de dialogue. Pour
choisir d&apos.en utiliser un, s‚lectez son nom puis pressez
:hp2.Entr‚e:ehp2., ou cliquez deux fois sur son nom. Si, pour une raison
quelconque, il n&apos.est pas possible d&apos.ouvrir le fichier
dictionnaire, vous en serez inform‚. Dans le cas contraire, la boŒte de
dialogue est ferm‚e et vous revenez … la fenˆtre de travail.
:p.Pour cr‚er un nouveau fichier dictionnaire, s‚lectez la plaquette
:hp2.Cr‚er:ehp2.. La boŒte de dialogue
:link reftype=hd res=IDD_BUILD.Cr‚ation de dictionnaire:elink.
sera affich‚e, vous permettant de cr‚er un nouveau dictionnaire.
:p.Pour effacer un fichier dictionnaire, choisissez le nom du fichier
dictionnaire puis s‚lectez la plaquette :hp2.Effacer:ehp2..

:h3 res=IDD_BUILD.Cr‚ation de dictionnaire (BoŒte de dialogue)
Cette boŒte de dialogue vous permet de cr‚er un fichier dictionnaire que
pourra utiliser le correcteur orthographique.
:artwork name='Francais\SplBuild.bmp' align=center.
:p.Pour cr‚er un fichier dictionnaire utilisable par le correcteur
orthographique, vous devez fournir deux fichiersÿ&colon. la liste des
mots et le fichier ®affixe¯ d‚pendant de la langue.
:p.Le fichier de la liste des mots peut ˆtre une simple liste de mots,
un par ligne, au format texte, ou bien il peut s&apos.agir de ce que l&apos.on
appele le format de liste de mots &apos.munched&apos. de ISpellÿ; ce dernier
format permet d&apos.ajouter des informations sur les pr‚fixes et suffixes
qui peuvent alt‚rer un mot. Il n&apos.existe pas de convention de
d‚nomination de ces fichiers, mais le nom de la langue fait normalement
partie du nom de fichier.
:p.Le fichier affixe contient les rŠgles par lesquelles les mots peuvent
ˆtre modifi‚s, et est caract‚ristique de la langue. L&apos.extension standard
des fichiers affixe est ®.aff¯.
:p.Pour cr‚er un nouveau dictionnaire, vous devez donner le nom d&apos.un
fichier contenant la liste des mots, le nom d&apos.un fichier ®affixe¯
existant, et le nom du dictionnaire … cr‚er. Une fois ces noms fournis,
vous pouvez s‚lecter la plaquette :hp2.Ok:ehp2.. Les noms de fichiers
sont v‚rifi‚s puis la fabrication du nouveau dictionnaire commence.
Le traitement peut durer un certain temps, aprŠs quoi vous retournez
… la boŒte de dialogue 
:link reftype=hd res=IDD_CONFIGURE.Configuration du correcteur orthographique:elink., 
o— vous pouvez choisir
d&apos.utiliser le nouveau dictionnaire. S&apos.il s&apos.est produit un
problŠme lors de la cr‚ation du dictionnaire, vous en serez inform‚ afin
de pouvoir prendre les mesures ad‚quates (changement de noms de
fichiers) avant de r‚-essayer.
:p.Pour quitter cette boŒte de dialogue sans cr‚er de dictionnaire,
pressez la touche :hp2.chap:ehp2. ou s‚lectez la plaquette
:hp2.Annuler:ehp2..

:euserdoc.
