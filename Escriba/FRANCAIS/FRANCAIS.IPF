#include "..\Escriba.h"
#ifdef APIENTRY
#undef APIENTRY
#endif
:userdoc.
:title.Aideÿ: Escriba, Traitement de textes
:docprof toc=123456.

:h1 res=ID_MAIN.Introduction
:i1.Introduction
:artwork name='Escriba.bmp' align=center.
:p.
Ce programme est un traitement de textes &odqf.tel ‚cran, tel
r‚sultat&cdqf. (What You See Is What You Get, WYSIWYG) pour le
Gestionnaire de Pr‚sentation d&apos.OS/2. Il ne se pose pas en
concurrent des traitements de textes professionnels pour OS/2 ou
Windows, mais a pour objectif de fournir un ensemble plus limit‚ de
fonctionnalit‚s. Il permet les op‚rations classiques de mise en forme de
texte, et offre la possibilit‚ d&apos.y ins‚rer des graphiques. Toutes
les polices ATM install‚es sont g‚r‚es, dans tous les formats natifs et
dans une large palette de tailles. L&apos.impression se fait au travers
du gestionnaire d&apos.impression d&apos.OS/2, ce qui permet
d&apos.imprimer sur tout support d&apos.impression g‚r‚ par cette
interface, y compris les p‚riph‚riques de t‚l‚copie et les traceurs.
Sur les versions ad-hoc d&apos.OS/2, la saisie et l&apos.affichage des caractŠres
cod‚s sur deux octets (DBCS) sont g‚r‚s, ainsi que les polices TrueType
install‚es dans la Palette de Polices.
:p.
:sl compact.
:li.&sqbul.:link reftype=hd res=60001.Lancement et Fermeture:elink.
:li.&sqbul.:link reftype=hd res=60002.Chargement et Sauvegarde de Documents:elink.
:li.&sqbul.:link reftype=hd res=60003.dition de Documents:elink.
:li.&sqbul.:link reftype=hd res=60004.Impression de Documents:elink.
:li.&sqbul.:link reftype=hd res=60005.Gestion des Fenˆtres:elink.
:li.&sqbul.:link reftype=hd res=60006.Quelques Informations:elink.
:li.&sqbul.:link reftype=hd res=60007.Enregistrement:elink.
:esl.

:h2 res=60001.Lancement &amp. Fermeture
:hp4.Lancement:ehp4.
:p.
Une fois Escriba install‚, il peut ˆtre lanc‚ de trois fa‡ons. La premiŠre
consiste simplement … cliquer deux fois sur l&apos.ic“ne cr‚‚e durant
l&apos.installation. Dans ce cas, le programme se chargera et cr‚era une
fenˆtre document vide utilisable imm‚diatement.
:p.
La seconde m‚thode consiste … faire glisser un fichier document sur
l&apos.ic“ne du programme. Le document sera ouvert et affich‚ dans la zone de
travail, prˆt … ˆtre modifi‚.
:p.
La troisiŠme m‚thode consiste … utiliser la commande START. Normalement,
ceci ne fonctionnera que si le r‚pertoire actif est le r‚pertoire o— est
install‚ Escriba. Si ce r‚pertoire est sp‚cifi‚ dans les paramŠtres
:hp2.PATH:ehp2., :hp2.HELP:ehp2. et :hp2.LIBPATH:ehp2. du fichier
:hp2.CONFIG.SYS:ehp2., vous pouvez utiliser la commande :hp2.START:ehp2.
… partir de n&apos.importe quel r‚pertoire.
:p.
Il existe aussi une m‚thode indirecte pour lancer Escriba. Quand Escriba
enregistre un fichier dans son propre format, il marque le fichier comme
‚tant de type &apos.Escriba&apos.. Le systŠme de fichiers associe ce
type de fichiers au traitement de textes Escriba, et quand l&apos.objet
fichier est ouvert, l&apos.action ex‚cut‚e par d‚faut consiste … lancer
Escriba, qui ouvre le fichier. Le traitement de textes Escriba est aussi
associ‚ aux types :hp1.Plain text:ehp1. et :hp1.Pluma:ehp1., et apparaŒt
dans le menu Ouvrir du menu systŠme des objets de ce type.

:p.
:hp4.Fermeture:ehp4.
:p.
Pour quitter Escriba, vous pouvez cliquer deux fois sur l&apos.ic“ne de
barre de titre du programme (la mini-ic“ne en haut et … gauche de la
fenˆtre du programme), ou bien vous pouvez utiliser la touche
:hp2.F3:ehp2.. Si certains documents ouverts n&apos.ont pas ‚t‚
sauvegard‚s aprŠs avoir ‚t‚ modifi‚s, Escriba vous proposera de les
enregistrer avant de sortir. La fermeture d&apos.Escriba peut ˆtre annul‚e
lors de ces messages de confirmation.

:h2 res=60002.Chargement &amp. Sauvegarde de Documents
:hp4.Chargement:ehp4.
:p.
Des documents peuvent ˆtre charg‚s dans le traitement de textes au
lancement ou plus tard. En lachant un ou plusieurs objets fichiers sur
l&apos.ic“ne d&apos.Escriba, celui-ci est ouvert et chargera les documents, dans
des fenˆtres s‚par‚es. Vous pouvez aussi lancer Escriba depuis la ligne
de commande, en sp‚cifiant ‚ventuellement le nom d&apos.un document … ouvrir.
Quelle que soit la m‚thode utilis‚e, le fait de lancer Escriba alors
qu&apos.il s&apos.ex‚cute d‚j… entraŒne la cr‚ation d&apos.une nouvelle fenˆtre et non
le chargement d&apos.une deuxiŠme copie d&apos.Escriba.
:p.
Une fois le traitement de textes lanc‚, il est possible d&apos.ouvrir des
documents avec l&apos.option de menu :link reftype=hd res=IDM_OPEN.Ouvrir:elink..
La boŒte de dialogue d&apos.ouverture de fichier standard du systŠme est
affich‚eÿ; par d‚faut, seuls les fichiers semblant ˆtre des documents
(d&apos.aprŠs le type du fichier ou l&apos.extension) sont affich‚s, mais vous
pouvez choisir :hp1.Tous les fichiers:ehp1.. Vous pouvez s‚lectionner
autant de fichiers … charger que vous le d‚sirezÿ; une fois votre choix
effectu‚, les fichiers seront charg‚s en arriŠre-plan. Au d‚but du
chargement, toutes les fenˆtres de documents actives sont r‚duites, et
le premier document … se charger devient le document actif. Les autres
seront r‚duits dŠs la fin de leur chargement.
:p.
Une fois la liste des documents … ouvrir valid‚e, Escriba d‚termine le
type de chaque fichier. Si le contenu d&apos.un fichier semble correct pour
plusieurs types de fichiers, vous devrez pr‚ciser de quel type est
r‚ellement le fichier. Si le contenu d&apos.un fichier ne semble correct pour
aucun des types de fichiers, vous en serez inform‚ et le document ne
sera pas charg‚.
:p.
:hp4.Sauvegarde:ehp4.
:p.
Il existe trois possibilit‚ pour sauvegarder un document sur disqueÿ&colon.
:link reftype=hd res=IDM_SAVE.Enregistrer:elink.,
:link reftype=hd res=IDM_SAVEAS.Enregistrer en...:elink. et
:link reftype=hd res=IDM_SAVE_CLOSE.Enregistrer et fermer:elink..
La commande :hp2.Enregistrer:ehp2. sauvegarde le fichier sous son nom et
avec son type actuel. Si ce n&apos.est pas possible, Escriba vous demandera
de modifier le nom du fichier ou son type.
La commande :hp2.Enregistrer en:ehp2. vous permet de modifier le nom du
fichier et/ou son type avant de l&apos.enregistrer.
Le commande :hp2.Enregistrer &amp. fermer:ehp2. fonctionne de fa‡on
identique … la commande :hp2.Enregistrer:ehp2., … ceci prŠs que la
fenˆtre du document est ferm‚e une fois celui-ci enregistr‚.
:p.
Si vous tentez de fermer la fenˆtre d&apos.un document ayant ‚t‚ modifi‚
mais pas sauvegard‚, Escriba vous demandera si vous voulez d&apos.abord
enregistrer le document. De mˆme, si vous quittez le traitement de
textes alors que des documents modifi‚s sont encore ouverts, Escriba
vous proposera de sauvegarder ces documents avant de sortir. Une
confirmation vous sera demand‚e pour chacun des documents ouverts
modifi‚s.

:h2 res=60003.dition de Documents
:hp4.D‚placement:ehp4.
:p.
Les touches suivantes permettent de se d‚placer dans la fenˆtre
du documentÿ&colon.
:table cols='20 60' rules=both frame=box.
:row.:c.Flˆche Gauche           :c.CaractŠre/code pr‚c‚dent.
:row.:c.Flˆche Droite           :c.CaractŠre/code suivant.
:row.:c.Flˆche Haut             :c.Ligne pr‚c‚dente.
:row.:c.Flˆche Bas              :c.Ligne suivante.
:row.:c.Page Haut               :c.Page pr‚c‚dente.
:row.:c.Page Bas                :c.Page suivante.
:row.:c.D‚but                   :c.D‚but de la ligne.
:row.:c.Fin                     :c.Fin de la ligne.
:row.:c.Ctrl + Flˆche Gauche    :c.Mot pr‚c‚dent.
:row.:c.Ctrl + Flˆche Droite    :c.Mot suivant.
:row.:c.Ctrl + Page Haut        :c.Haut de la page.
:row.:c.Ctrl + Page Bas         :c.Bas de la page.
:row.:c.Ctrl + D‚but            :c.D‚but du document.
:row.:c.Ctrl + Fin              :c.Fin du document.
:etable.

De plus, vous pouvez utiliser la souris pour positionner directement le
curseur … tout endroit visible du document. Vous pouvez utiliser la
souris sur les barres de d‚filement, si elles sont visibles, pour
d‚placer la page horizontalement ou verticalement dans la fenˆtre.

:p.Enfin, quatre plaquettes sp‚ciales sont dispos‚es en bas et … droite
de la fenˆtre du document. Ces plaquettes fournissent les
fonctionnalit‚s des touches :hp2.Ctrl+D‚but:ehp2., :hp2.Page Haut:ehp2.,
:hp2.Page Bas:ehp2. et :hp2.Ctrl+Fin:ehp2..

:p.Lors vos d‚placements … l&apos.int‚rieur du document, souvenez-vous que
les codes ins‚r‚s pour modifier les marges, les polices, etc... sont
consid‚r‚s comme des ‚l‚ments du texte, et le curseur s&apos.arrˆtera sur
chacun d&apos.eux quand vous utilisez les touches flˆch‚es vers la gauche ou
la droite pour vous d‚placer … travers le document.

:note.Les touches :hp1.D‚but:ehp1. et :hp1.Fin:ehp1. fonctionnent
de fa‡on l‚gŠrement diff‚rente du standard.
Si vous pressez la touche D‚but quand le curseur se trouve d‚j… en d‚but
de ligne, il se d‚place au d‚but de la ligne pr‚c‚dente. De mˆme, si
vous pressez la touche Fin quand le curseur se trouve d‚j… en fin de
ligne, il se d‚place … la fin de la ligne suivante.

:p.:hp4.Texte:ehp4.

:p.Le texte est ins‚r‚ dans le document en le tapant au clavier. ·
mesure que vous saisissez du texte, celui-ci est mis en forme. Si un mot
est trop long pour tenir sur une ligne, il est d‚plac‚ sur la ligne
suivante. Si vous tapez trop vite, les touches sont m‚moris‚es puis
ins‚r‚es par groupe pour vous rattraper.

:p.Une fois que vous avez saisi du texte dans le document, vous pouvez
d‚placer le curseur pour le modifier. Normalement, le curseur est en
mode insertion, repr‚sent‚ par une barre verticale. Quand vous tapez du
texte au milieu d&apos.un texte existant, le nouveau texte est ins‚r‚ au
milieu de l&apos.ancien, et le document est remis en forme en cons‚quence. Si
vous d‚sirez plut“t remplacer du texte, pressez la touche
:hp2.Inser:ehp2.. Le curseur apparaŒtra comme un bloc couvrant le
caractŠre sur lequel il se trouve, et ce que vous saisirez remplacera ce
qui se trouve sous le curseur. Attention, si le curseur se trouve sur un
code de changement de police ou une autre marque, ce code sera remplac‚
par ce que vous taperez, comme le serait un texte normal.
:p.
Pour s‚lectionner une portion de texte, positionnez le curseur au d‚but
de la zone … s‚lectionnerÿ; pressez ensuite la touche
:hp2.Majuscules:ehp2., et d‚placez le curseur jusqu&apos.… la fin de la zone
… s‚lectionner, tout en maintenant la touche :hp2.Majuscules:ehp2.
enfonc‚e. Une fois qu&apos.une portion de texte est s‚lectionn‚e, elle peut
ˆtre d‚plac‚e ou copi‚e, coup‚e du document, copi‚e vers le
presse-papiers, ou remplac‚e avec le contenu du presse-papiers. De plus,
si un bloc de texte est s‚lectionn‚, le fait de commencer … saisir …
nouveau du texte entrainera l&apos.effacement du texte s‚lectionn‚.
:p.
Le texte s‚lectionn‚ peut ˆtre d‚plac‚ ou copi‚ … l&apos.aide de la souris.
Pour cela, vous devez presser le bouton droit de la souris sur la zone
s‚lectionn‚e, puis d‚placer la souris jusqu&apos.… ce que le curseur soit sur
la nouvelle position du bloc de texte. Le texte sera d‚plac‚ quand vous
relacherez le bouton de la souris. Si vous voulez copier et non d‚placer
le texte, maintenez la touche :hp2.Ctrl:ehp2. enfonc‚e au moment ou vous
relachez le bouton de la souris. Si vous relachez le bouton de la souris
sans l&apos.avoir d‚plac‚e, un menu contextuel apparaŒtra, pr‚sentant toutes
les commandes qui peuvent ˆtre appliqu‚es … un bloc de texte.
:p.
:hp4.Attributs du Texte:ehp4.
:p.
En tout point du document, certains attributs de style et de formattage
sont effectifs. Les marges sont d‚finies … une certaine position, les
tabulations … des endroits pr‚cis, et la police des caractŠres possŠde des
caract‚ristiques pr‚cises. N&apos.importe lequel de ces attributs peut ˆtre
modifi‚ de la position du curseur jusqu&apos.… la fin du document, ou pour
une zone de texte s‚lectionn‚e. Si vous choisissez la police de
caractŠres :hp1.Times Roman:ehp1. sans qu&apos.une zone de texte ne soit
s‚lectionn‚e, la nouvelle police sera appliqu‚e … partir de la position
du curseur, jusqu&apos.… ce qu&apos.une autre marque de police soit rencontr‚e.
Dans le cas o— une zone de texte est s‚lectionn‚e, cependant, toutes les
marques de police seront supprim‚es de la zone s‚lectionn‚e, et la
police Times Roman sera appliqu‚e sur toute la zone s‚lectionn‚e. La
police en vigueur aprŠs la zone de texte s‚lectionn‚e n&apos.est pas
modifi‚e. Tous les attributs de texte sont g‚r‚s de la mˆme fa‡on, que
ce soient les marges, les tabulations, l&apos.indentation, la police, la
taille de la police, les couleurs... tous.
:p.
:hp4.Graphiques:ehp4.
:p.
Des graphiques peuvent ˆtre plac‚s dans un document … partir du presse-
papiers du systŠme, d&apos.une portion visible du bureau, ou d&apos.un fichier
graphique. Utilisez le menu :link reftype=hd res=IDM_PASTE_FROM.Coller depuis:elink. pour cela.
:p.
Le graphique ‚tant plac‚ dans le document, vous pouvez le d‚placer, le
copier ou le redimensionner … tout moment. Pour d‚placer le graphique,
cliquez le bouton droit de la souris sur le corps de l&apos.image et faites
la glisser jusqu&apos.… sa nouvelle position. Pour le copier, proc‚dez de la
mˆme fa‡on, mais maintenez la touche :hp2.Ctrl:ehp2. enfonc‚e au moment
o— vous relachez le bouton de la souris. Pour redimensionner une image,
cliquez le bouton droit de la souris sur l&apos.espace situ‚ imm‚diatement …
c“t‚ du graphiqueÿ; le bord du graphique apparaŒtra, et vous n&apos.aurez
qu&apos.… le faire glisser pour changer la taille de l&apos.image.

:h2 res=60004.Impression de Documents
:p.Il y a actuellement deux options d&apos.impression disponiblesÿ:
:link reftype=hd res=IDM_PRINT_ALL.Imprimer le Document:elink. et
:link reftype=hd res=IDM_PRINT_PAGE.Imprimer la Page:elink..
:p.
:hp4.Imprimer le Document:ehp4. entraŒne l&apos.impression du document actif
complet. Vous pouvez obtenir le mˆme r‚sultat en pressant les touches
:hp2.Ctrl:ehp2. et :hp2.I:ehp2..
:p.
:hp4.Imprimer la Page:ehp4. imprime la page affich‚e du document actif.
Ceci peut aussi ˆtre effectu‚ en pressant les touches :hp2.Alt:ehp2. et
:hp2.I:ehp2..

:h2 res=60005.Gestion des Fenˆtres
:p.Il est possible d&apos.avoir plusieurs fenˆtres de documents ouvertes en
mˆme temps. · chaque fois que vous ouvrez une nouvelle fenˆtre de
document, toutes les fenˆtres de documents ouvertes sont r‚duites et
rang‚es dans la partie inf‚rieure de la zone de travail. La nouvelle
fenˆtre de document occupera autant de surface que possible, sans
toutefois cacher les ic“nes des documents r‚duits.
:p.
Pour rendre … une fenˆtre de document sa taille normale, il suffit de
cliquer deux fois sur son ic“ne, comme pour n&apos.importe quelle ic“ne.
La fenˆtre retrouvera la position et la taille qu&apos.elle avait avant
d&apos.ˆtre r‚duite.
:p.
Si vous rendez … plusieurs fenˆtres de documents leur taille normale,
elles vont sans doute se chevaucher. Deux commandes permettent d&apos.y
mettre bon ordreÿ:
:link reftype=hd res=IDM_TILE.Mosa‹que:elink. et
:link reftype=hd res=IDM_CASCADE.Cascade:elink..
:hp2.Mosa‹que:ehp2. arrangera toutes les fenˆtres de document non
minimis‚es de fa‡on … ce qu&apos.aucune n&apos.en recouvre une autre.
L&apos.effet est le mˆme que celui constat‚ quand le systŠme arrange les
fenˆtres du bureau en mosa‹que.
La commande :hp2.Cascade:ehp2. arrangera toutes les fenˆtres de
documents non r‚duites de fa‡on … ce qu&apos.elles forment une pile, …
l&apos.instar d&apos.un jeu de cartes. Elles sont arrang‚es de fa‡on …
ce que toutes leurs barres de titre soient visibles. Le document actif
est plac‚ au sommet de la pile.
:p.
Il existe une autre commande de gestion de l&apos.espace de travailÿ:
:link reftype=hd res=IDM_ARRANGEICONS.Arranger les Ic“nes:elink.
Cette commande rangera proprement les ic“nes dans la partie inf‚rieure
de la zone de travail.
:p.
Pour le cas o— une fenˆtre de document est entiŠrement cach‚e, le menu
:link reftype=hd res=IDM_WINDOW_MENU.Fenˆtres:elink. contient une liste des
documents ouverts. Vous pouvez s‚lecter n&apos.importe laquelle des
fenˆtres dans ce menu pour en faire la fenˆtre active. Si la fenˆtre
‚tait r‚duite, elle retrouvera sa taille normale.

:h2 res=60006.Quelques Informations
&sqbul.Le fait d&apos.avoir beaucoup de polices de caractŠres dans le
menu :hp2.Polices:ehp2. est agr‚able, mais OS/2 ne libŠre pas la m‚moire
allou‚e aux polices avant que vous n&apos.arrˆtiez le systŠme. Ceci peut
provoquer une augmentation de la taille de votre fichier d&apos.‚change
de plusieurs m‚ga-octets.
:p.
&sqbul.Certaines polices ATM disponibles sur des serveurs t‚l‚matiques
ne sont pas complŠtement v‚rifi‚es et peuvent provoquer un arrˆt de
votre systŠme. Soyez prudents avec des polices venant de fournisseurs
tiers nouvellement install‚es, quoique vous ne devriez pas avoir de
problŠmes avec des polices fournies par IBM ou Adobe.
:p.
&sqbul.Les graphiques ins‚r‚s dans les documents peuvent ˆtre de grande
taille. Quand vous ins‚rez une grande image dans le document, mais la
r‚duisez une fois dans le document, ne croyez pas que le graphique
occupe moins de placeÿ; il s&apos.agit toujours du mˆme graphique
qu&apos.avant sa r‚duction.

:h2 res=60007.Enregistrement
:p.Ce programme est un partagiciel (shareware).
Vous pouvez librement en distribuer des copies non modifi‚es … toute
personne int‚ress‚e. Si vous trouvez le programme utile, et d‚cidez
de continuer … l&apos.utiliser, vous devez le payer. Le prix est de 30$
am‚ricains, et vous pouvez payer par les moyens suivantsÿ&colon.
:p.
:hp4.ChŠque ou mandat&colon.:ehp4.
Si vous d‚sirez payer par chŠque ou par mandat, veuillez envoyer votre
rŠglement, accompagn‚ de vos nom, adresse postale, de la date et du
num‚ro de version de votre copie de Escriba, …ÿ&colon.
:lm margin=6.:hp4.:sl compact.
:li.Escriba Registrations
:li.c/o Rick Papo
:li.38290 Avondale
:li.Westland, MI 48186-3830
:li.tats-Unis d&apos.Am‚rique
:esl.:ehp4.:lm margin=1.
.br
:hp4.VISA/MasterCard/Discover&colon.:ehp4.
Si vous d‚sirez utiliser une carte de cr‚dit pour r‚gler votre
enregistrement, la soci‚t‚ BMT Micro fournit ce service pour Escriba.
Elle peut ˆtre contact‚e au 1-800-414-4268 (tats-Unis uniquement) ou au
1-910-791-7052, par t‚l‚copie au 1-910-350-2937, ou sur son BBS au
1-910-350-8061. Les lignes vocales sont ouvertes de 8h … 20h (c“te est
des tats-Unis). Les autres num‚ros fonctionnent 24h/24. L&apos.adresse
postale de BMT Micro estÿ&colon.
:lm margin=6.:hp4.:sl compact.
:li.BMT Micro
:li.452 Horn Rd
:li.Wilmington, NC 28412-2411
:li.tats-Unis d&apos.Am‚rique
:esl.:ehp4.:lm margin=1.
:note. Si vous r‚sidez dans l&apos.une des r‚publique qui formaient
l&apos.Union Sovi‚tique ou appartenaient au pacte de Varsovie, vous
pouvez vous enregistrer pour 5$ am‚ricains (pas de cartes de cr‚dit,
s&apos.il vous plaŒt).
:p.Une fois votre enregistrement re‡u, il vous sera envoy‚ un re‡u et la
derniŠre version du traitement de texte sur disquette. Vous recevrez
plus tard une autre disquette, au prochain changement majeur de version.
La documentation imprim‚e n&apos.est pas encore disponible, mais quand
elle le sera, tous les utilisateurs enregistr‚s du traitement de texte
en recevront une copie. Si vous voulez obtenir les versions ult‚rieures
sur disquette, elles co–teront 5$ am‚ricains.

:h2 res=60008.Questions et rapport d&apos.anomalies
:p.Si vous recontrez des problŠmes dans l&apos.utilisation de ce programme,
l&apos.auteur, Rick Papo, peut ˆtre joint par l&apos.un des moyens
suivantsÿ&colon.
:table cols='15 60' rules=both frame=box.
:row.:c.Internet        :c.:hp4.rpapo@msen.com:ehp4.
:row.:c.Courrier postal :c.:hp4.Rick Papo, 38290 Avondale, Westland, MI 48186-3830, -U:ehp4.
:row.:c.T‚l‚phone       :c.:hp4.tats-Unis (734) 326-8547:ehp4.
:etable.
La priorit‚ est donn‚e aux requˆtes et aux problŠmes des utilisateurs
enregistr‚s (voir :link reftype=hd res=60007.Enregistrement:elink.), mais tous
les messages seront trait‚s. Le courrier ‚lectronique est ramass‚ tous
les jours.
:p.Dans toute correspondance, veuillez pr‚ciser le num‚ro de version
d&apos.Escriba que vous utilisez.

:h1 res=IDM_FILE_MENU.Menu Fichier
:i1.Menu Fichier
:p.Les commandes suivantes sont accessibles depuis le menu fichierÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_NEW.Nouveau:elink.
:li.&sqbul.:link reftype=hd res=IDM_OPEN.Ouvrir...:elink.
:li.&sqbul.:link reftype=hd res=IDM_RECALL.Rouvrir:elink.
:li.&sqbul.:link reftype=hd res=IDM_MERGE.Fusionner...:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVE.Enregistrer:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVEAS.Enregistrer en...:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVE_CLOSE.Enregistrer et Fermer:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLOSE.Fermer:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT.Imprimer:elink.
:li.&sqbul.:link reftype=hd res=IDM_PREFERENCES.Pr‚f‚rences...:elink.
:li.&sqbul.:link reftype=hd res=IDM_DEFAULTS.Caract‚ristiques du Document...:elink.
:li.&sqbul.:link reftype=hd res=IDM_CONFIGURE_EXTENSIONS.Configurer les extensions...:elink.
:li.&sqbul.:link reftype=hd res=IDM_LANGUAGE_MENU.Langue:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXIT.Quitter:elink.
:esl.

:h2 res=IDM_NEW.Nouveau (Option de menu)
:p.Cette option de menu cr‚e une nouvelle fenˆtre de document vide. Les
marges, police et autres attributs du document sont ceux qui ont ‚t‚
sp‚cifi‚s dans le dialogue :hp2.Caract‚ristiques du Document:ehp2.
s&apos.il a ‚t‚ appel‚ alors qu&apos.aucun document n&apos.‚tait actif.
:p.
Toutes les autres fenˆtres ouvertes sont r‚duites en bas de la zone de
travail et la nouvelle fenˆtre occupe une surface aussi importante que
possible.

:h2 res=IDM_OPEN.Ouvrir... (Option de menu)
:p.Quand cette option de menu est s‚lectionn‚e, le dialogue systŠme standard
d&apos.ouverture de fichier est propos‚. Vous pouvez choisir un ou
plusieurs documents … ouvrir. AprŠs avoir press‚ la touche
:hp2.Entr‚e:ehp2., ou s‚lectionn‚ la plaquette :hp2.Ok:ehp2., tous les
fichiers que vous avez choisis seront ouverts. Un indicateur de
chargement, rouge, apparaŒtra sur la partie droite de la barre de titre
du programme. Pour chaque document charg‚, une fenˆtre de document sera
cr‚‚e. Le premier document charg‚ deviendra le document actif, et sa
fenˆtre sera agrandie. Tous les autres documents charg‚s seront r‚duits.
Ils peuvent ˆtre affich‚s … partir du menu :hp2.Fenˆtres:ehp2., ou bien
en cliquant deux fois sur leur ic“ne.
:p.Le raccourci clavier pour cette fonction est :hp2.Ctrl+O:ehp2..
:p.Voirÿ:link reftype=hd res=256.Ouvrir/Fusionner/Enregistrer en... (Dialogue):elink. pour plus d&apos.informations.

:h3 res=256.Ouvrir/Fusionner/Enregistrer en... (Dialogue)
:p.Vous avez s‚lectionn‚ l&apos.option de menu :hp2.Ouvrir:ehp2.,
:hp2.Fusionner:ehp2. ou :hp2.Enregistrer en:ehp2., et vous ˆtes en
cons‚quence face … ce dialogue. C&apos.est la boŒte de dialogue fichier
standard fournie par le systŠme d&apos.exploitation, et elle suit les
rŠgles d‚finies dans la documentation du systŠme.
:artwork name='Francais\FileDlg.bmp' align=center.
:p.Quelques commentairesÿ:
:p.
&rarrow.· l&apos.ouverture de la boŒte de dialogue aprŠs une commande Ouvrir
ou Fusionner, le champ :hp1.Type de fichier:ehp1. contient :hp1.<Tous
les documents>:ehp1.. Tous les fichiers semblant ˆtre des documents au
format natif d&apos.Escriba ou dans l&apos.un des formats ‚tendus (Texte
brut, BMP, GIF, JPG, PCX, TIF, etc.) sont affich‚s dans la liste. Vous pouvez
choisir d&apos.afficher :hp1.<Tous les fichiers>:ehp1. ou bien l&apos.un des types
de fichiers enum‚r‚s.
:p.
&rarrow.Quand cette boŒte de dialogue est affich‚e aprŠs une commande
:hp2.Enregistrer:ehp2. ou :hp2.Enregistrer en:ehp2., le type actuel du
document est s‚lectionn‚. Si vous d‚sirez changer le type du document,
choisissez un autre type dans la liste.
:p.
&rarrow. Quand vous s‚lectionnez des fichiers … ouvrir, vous pouvez
choisir autant de fichiers que d‚sir‚ dans la liste des fichiers.
:p.
&rarrow. Quand vous s‚lectionnez un fichier … fusionner, un seul fichier
peut ˆtre choisi.
:p.
&rarrow. Quand vous avez s‚lectionn‚ l&apos.option :hp2.Enregistrer
en:ehp2., il est impossible de choisir un fichier dans la liste des
fichiers. La liste sert uniquement d&apos.aide-m‚moire. Vous devez
saisir au clavier le nom du fichier que vous d‚sirez sauvegarder dans le
champ de saisie.
:p.
&rarrow. Le r‚pertoire choisi dans ce dialogue devient le nouveau
r‚pertoire courant pour le programme. La prochaine fois que vous
utiliserez les commandes :hp2.Ouvrir:ehp2., :hp2.Fusionner:ehp2. ou
:hp2.Enregistrer en:ehp2., elles se positionneront dans ce r‚pertoire.

:note.Les fichiers enregistr‚s sur disque sont marqu‚s comme ‚tant des
documents de type :hp1.Escriba:ehp1.. Ce marquage est effectu‚ en
utilisant les attributs suppl‚mentaires, et il est donc perdu si vous
l&apos.archivez … l&apos.aide d&apos.un programme (tel PKZip) ne g‚rant
pas les attributs suppl‚mentaires.
Il existe des programmes d&apos.archivage plus r‚cents qui pr‚servent
ces informations.

:h4 res=IDD_FILETYPE.Choix du type de fichier (BoŒte de Dialogue)
:p.Cette boŒte de dialogue est pr‚sent‚e quand vous chargez un fichier dont
le format correspond … plusieurs types de fichiers. Le programme affiche
le nom du fichier et les types possibles, parmi lesquels vous devez
choisir le bon type. Pressez Entr‚e ou s‚lectionnez Ok aprŠs avoir choisi le
type. Pour annuler le chargement du fichier, s‚lectionnez Annuler ou pressez
la touche chap.

:h2 res=IDM_RECALL.Menu Rouvrir
:p.Cette option de menu ouvre un sous-menu dans lequel les dix documents
les plus r‚cemment sauvegard‚s sont propos‚s.
:p.Voir :link reftype=hd res=IDM_RECALL0.Lignes du menu Rouvrir:elink.
pour plus d&apos.informations.

:h3 res=IDM_RECALL0.Lignes du menu Rouvrir
:p.Cette ligne de menu repr‚sente l&apos.un des dix fichiers les plus
r‚cemment sauvegard‚s. Pour rouvrir l&apos.un de ces documents dans
une nouvelle fenˆtre, s‚lectionnez-le … l&apos.aide des touches du curseur
puis pressez :hp2.Entr‚e:ehp2., ou s‚lectionnez le fichier d‚sir‚ …
l&apos.aide de la souris.

:h2 res=IDM_MERGE.Fusionner... (Option de menu)
:p.Quand cette option de menu est s‚lectionn‚e, le dialogue systŠme standard de
choix de fichier apparaŒt. Ce dialogue permet de choisir un fichier,
dont le contenu sera ins‚r‚ dans le document actif … la position du
curseur.
:p.Voirÿ:link reftype=hd res=256.Ouvrir/Fusionner/Enregistrer en... (Dialogue):elink.
pour plus d&apos.informations.

:h2 res=IDM_SAVE.Enregistrer (Option de menu)
:p.Cette commande provoque la sauvegarde du document actif dans son
fichier. Si aucune modification n&apos.a ‚t‚ apport‚e au document, cette
option n&apos.est pas disponible.
:p.Le raccourci clavier pour cette fonction est :hp2.Ctrl+E:ehp2..

:h2 res=IDM_SAVEAS.Enregistrer en... (Option de menu)
:p.Quand cette option de menu est s‚lectionn‚e, le dialogue systŠme standard de
choix de fichier est pr‚sent‚. Il vous permet de saisir le nom de
fichier sous lequel sera sauvegard‚ le document actif, et il permet
aussi de choisir le r‚pertoire dans lequel il sera enregistr‚.
:p.Voirÿ:link reftype=hd res=256.Ouvrir/Fusionner/Enregistrer en... (Dialogue):elink. pour plus d&apos.informations.

:h2 res=IDM_SAVE_CLOSE.Enregistrer et Fermer (Option de menu)
:p.Cette option fournit un moyen rapide de sauvegarder et fermer le
document actif.
:p.Le raccourci clavier pour cette fonction est :hp2.F4:ehp2..

:h2 res=IDM_CLOSE.Fermer (Option de menu)
:p.Cette commande entraŒne la fermeture du document actif. Si des
modifications ont ‚t‚ apport‚es au document et n&apos.ont pas ‚t‚
sauvegard‚es, le programme vous demandera si vous voulez enregistrer le
document.
Si vous s‚lectionnez :hp2.Oui:ehp2., ou pressez la touche :hp2.O:ehp2., le
document sera enregistr‚ sous son nom de fichier actuel.
Si vous s‚lectionnez :hp2.Non:ehp2., ou pressez la touche :hp2.N:ehp2., le
document sera ferm‚ sans que les modifications ne soient sauvegard‚es.
Si vous s‚lectionnez :hp2.Annuler:ehp2., ou pressez la touche
:hp2.chap:ehp2., la commande :hp2.Fermer:ehp2. sera annul‚e et la
fenˆtre du document restera la fenˆtre active.

:h2 res=IDM_PRINT.Imprimer (Sous-menu)
:p.Ce sous-menu permet d&apos.acc‚der aux diverses commandes
d&apos.impression, qui sont actuellement au nombre de deuxÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL.Imprimer le Document:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_PAGE.Imprimer la Page:elink.
:esl.

:h3 res=IDM_PRINT_ALL.Menu Imprimer le Document
:p.Ce menu donne accŠs aux diff‚rentes options concernant l&apos.impression de
la totalit‚ du document. Les commandes suivantes sont
disponiblesÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL_NORMAL.Dans l&apos.ordre normal:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL_REVERSE.En ordre inverse:elink.
:esl.

:h4 res=IDM_PRINT_ALL_NORMAL.Imprimer le document dans l&apos.ordre normal (Option de menu)
:p.Cette option demande l&apos.impression du document actif. Une fenˆtre
d&apos.‚tat rouge est affich‚e pendant que le document est envoy‚ … la
file d&apos.impression du systŠme, pour information. Vous pouvez
continuer … travailler sur le document imm‚diatement aprŠs en avoir
demand‚ l&apos.impression. Le document sera imprim‚ tel qu&apos.il est
au moment pr‚cis de la demande d&apos.impression.
:p.Le raccourci clavier pour cette fonction est :hp2.Ctrl+P:ehp2..

:h4 res=IDM_PRINT_ALL_REVERSE.Imprimer le document en ordre inverse (Option de menu)
:p.Le document actif est imprim‚, comme par la commande 
:link reftype=hd res=IDM_PRINT_ALL_NORMAL.Imprimer le Document dans l&apos.ordre normal:elink.,
… ceci prŠs que les pages sont imprim‚es dans l&apos.ordre inverse.

:h3 res=IDM_PRINT_PAGE.Imprimer la Page (Option de menu)
:p.La page en cours du document actif est imprim‚e quand vous s‚lectionnez
cette option de menu. Une fenˆtre d&apos.‚tat rouge est affich‚e pendant
que le document est envoy‚ … la file d&apos.impression du systŠme, pour
votre information uniquement. Vous pouvez continuer … travailler sur le
document imm‚diatement aprŠs avoir command‚ l&apos.impression. Le
document sera imprim‚ tel qu&apos.il est au moment ou vous en demandez
l&apos.impression.
:p.Le raccourci clavier pour cette fonction est :hp2.Alt+I:ehp2..

:h2 res=IDM_PREFERENCES.Pr‚f‚rences... (Option de menu)
:p.Voirÿ:link reftype=hd res=IDD_PREFERENCES.Pr‚f‚rences (Dialogue):elink. pour plus d&apos.informations.

:h3 res=IDD_PREFERENCES.Pr‚f‚rences (Dialogue)
:p.Ce dialogue vous permet de configurer l&apos.apparence et le comportement de
Escriba en ce qui concerne le traitement de textes lui mˆme, et non pour
ce qui est des document en cours de mise en forme.
:artwork name='Francais\Prefer.bmp' align=center.
:p.Les options disponibles sontÿ&colon.
:p.:hp2.Animer:ehp2.
Quand cette option est s‚lectionn‚e, l&apos.ouverture et la fermeture de la
fenˆtre principale du traitement de textes sont anim‚es, de la mˆme
fa‡on que l&apos.ouverture et la fermeture des dossiers du systŠme. Pour que
cette animation fonctionne, elle doit ˆtre autoris‚e depuis l&apos.objet
:hp1.SystŠme:ehp1. dans le dossier :hp1.Configuration du SystŠme:ehp1..
:p.:hp2.Afficher les RŠgles:ehp2.
Si cette case est coch‚e, toutes les fenˆtres de documents contiendront
une rŠgle horizontale et une rŠgle verticale, dispos‚es sur les bords
des fenˆtres.
:p.:hp2.Afficher les Barres de D‚filement:ehp2.
Si cette case est coch‚e, toutes les fenˆtres de documents contiendront
des barres de d‚filement horizontale et verticale permettant de faire
d‚filer le document … l&apos.aide de la souris.
:p.:hp2.Afficher la ligne d&apos.‚tat:ehp2.
Si cette case est coch‚e, une ligne d&apos.‚tat est affich‚e en bas de chaque
fenˆtre de document. Cette ligne d&apos.‚tat affiche la position du curseur
et la nature du caractŠre ou du code de contr“le qui se trouve sous le
curseur.
:p.:hp2.Montrer noms de Fichiers complets:ehp2.
Si cette case est coch‚e, le nom de fichier complet de chaque document
sera affich‚ dans la barre de titre de la fenˆtre du document. Dans le
cas contraire, la partie du nom de fichier du document contenant le
sp‚cificateur de disque et le nom du r‚pertoire ne sera pas affich‚e.
:p.:hp2.S‚parer les pages dans la file d&apos.impression:ehp2.
Si cette case est coch‚e, les pages d&apos.un document seront envoy‚es
s‚par‚ment … la file d&apos.impression du systŠme. Comme les travaux
d&apos.impression ne commencent … ˆtre imprim‚s qu&apos.une fois qu&apos.ils sont
entiŠrement en file d&apos.attente, cette option permet un d‚marrage plus
rapide de l&apos.impression de documents comportant plusieurs pages.
Attention, si votre imprimante est partag‚e sur un r‚seau, les pages du
document peuvent ˆtre imprim‚es dans le d‚sordre ou m‚lang‚es avec les
documents d&apos.autres utilisateurs.
:p.:hp2.Imprimer en ordre inverse:ehp2.
Quand cette case est coch‚e, les pages du document sont imprim‚es en
ordre inverse. Cette option est utile si votre imprimante empile les
pages imprim‚es face en haut.
:p.:hp2.Rouvrir le document au lancement:ehp2.
Si cette case est coch‚e, le dernier document ferm‚ sera ouvert
automatiquement lors du lancement d'Escriba.
:p.:hp2.Zoom initial:ehp2.
Ce groupe de boutons radio vous permettent de s‚lectionner le type de
zoom qui sera appliqu‚ initialement aux fenˆtres de document. Quand
vous ouvrez une nouvelle fenˆtre de document ou que vous chargez un
document depuis le disque, la m‚thode de zoom sp‚cifi‚e sera appliqu‚e
… la fenˆtre lors de son ouverture. Vous pouvez choisir l&apos.un des types
de zoom suivantsÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_100.Zoom 100%:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLWIDTH.Zoom pleine largeur:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLHEIGHT.Zoom pleine hauteur:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_MARGINS.Zoom marges:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_ALL.Zoom tout:elink.
:esl.
:p.
Une fois les options d‚sir‚es coch‚es, pressez la touche
:hp2.Entr‚e:ehp2. ou s‚lectionnez la plaquette :hp2.Ok:ehp2. pour valider vos
choix. Ces changements seront appliqu‚s … tous les nouveaux documents
ult‚rieurement cr‚‚s.
:p.
Si vous voulez fermer la boŒte de dialogue en laissant les
caract‚ristiques par d‚faut inchang‚es, pressez la touche
:hp2.chap:ehp2. ou s‚lectionnez la plaquette :hp2.Annuler:ehp2..

:h2 res=IDM_DEFAULTS.Caract‚ristiques du Document... (Option de menu)
:p.Cette commande fait apparaŒtre une boŒte de dialogue … l&apos.aide de
laquelle il est possible de choisir l&apos.imprimante, le format de papier,
la police de caractŠres, sa taille et sa couleur, les attributs de la
police et la justification du texte (horizontale et verticale) pour le
document.
:p.Voirÿ:link reftype=hd res=IDD_DEFAULTS.Caract‚ristiques du Document (Dialogue):elink. pour plus d&apos.informations.

:h3 res=IDD_DEFAULTS.Caract‚ristiques du Document (Dialogue)
:p.Cette boŒte de dialogue vous permet de modifier les caract‚ristiques par
d‚faut du document. Ces caract‚ristiques s&apos.appliquent au document
actifÿ; si aucun document n&apos.est ouvert, les caract‚ristiques sont
m‚moris‚es et seront utilis‚es lors de la cr‚ation de nouveaux
documents.
.br
Les options de confirmation sont group‚es par cat‚gorie sur 
les pages suivantes.
:sl compact.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_AUTHOR.Auteur:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_PRINTER.Imprimante:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_FONT.Polices:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_LAYOUT1.Disposition (1):elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_LAYOUT2.Disposition (2):elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_NATIONAL.Langue:elink.
:esl.
:p.
Une fois les caract‚ristiques par d‚faut sp‚cifi‚es, pressez la touche
:hp2.Entr‚e:ehp2. ou s‚lectionnez la plaquette :hp2.Ok:ehp2. pour valider
vos choix. Si aucun document n&apos.est ouvert, ou si vous avez coch‚ la
case :hp2.Sauvegarde globale:ehp2., les caract‚ristiques
s&apos.appliqueront … tous les documents cr‚‚s ult‚rieurement. Si une
fenˆtre de document est ouverte, les caract‚ristiques seront aussi
appliqu‚es … ce document. Aucun autre document n&apos.est affect‚.
:p.
Si vous s‚lectionnez la plaquette :hp2.Appliquer:ehp2., les nouvelles
caract‚ristiques par d‚faut sont imm‚diatement appliqu‚es au document
actif, et en deviennent les nouvelles caract‚ristiques par d‚faut.
:p.
Si vous voulez annuler le dialogue et ne pas modifier les
caract‚ristiques par d‚faut, pressez la touche :hp2.chap:ehp2. ou
s‚lectionnez la plaquette :hp2.Annuler:ehp2.. Les modifications valid‚es par
la plaquette :hp2.Appliquer:ehp2. ne sont pas annul‚es.

:h4 res=IDD_DEFAULTS_PAGE_AUTHOR.Page d&apos.identification de l&apos.auteur
:artwork name='Francais\Default0.bmp' align=center.
:p.:hp2.Auteur:ehp2.
Ce champ vous permet de modifier le nom de l'auteur et le titre du
document sauvegard‚s dans l'en-tˆte. La date de cr‚ation du document est
aussi affich‚e.

:h4 res=IDD_DEFAULTS_PAGE_PRINTER.Configuration de l&apos.imprimante
:artwork name='Francais\Default1.bmp' align=center.
:p.:hp2.Imprimante:ehp2.
Ce champ vous permet de choisir quelle imprimante (si vous disposez de
plusieurs imprimantes) sera utilis‚e pour l&apos.impression du document.
Vous pouvez choisir n&apos.importe laquelle des imprimantes d‚finies
dans le systŠme.
:p.:hp2.Configuration de l&apos.imprimante:ehp2.
Le fait s‚lecter cette plaquette provoquera l&apos.ouverture de la boŒte
de dialogue permettant de d‚finir les caract‚ristiques d&apos.un travail
d&apos.impression. Le contenu de ce dialogue varie en fonction de
l&apos.imprimante s‚lectionn‚e, et ne peut ˆtre document‚ ici. Veuillez
vous reporter aux informations fournies par la boŒte de dialogue. Au
retour de la boŒte de dialogue, le format de papier sera mis … jour dans
la zone situ‚e au dessous de la plaquette.

:h4 res=IDD_DEFAULTS_PAGE_FONT.Configuration des polices
:artwork name='Francais\Default2.bmp' align=center.
:p.:hp2.Police:ehp2.
Permet de d‚signer la police initiale pour le document. Vous pouvez
choisir n&apos.importe quelle police install‚e sur votre systŠme.
:p.:hp2.Taille:ehp2.
D‚finit la taille initialement utilis‚e pour la police. Vous pouvez
choisir n&apos.importe quelle taille dans la liste propos‚e.
:p.:hp2.Couleur:ehp2.
D‚finit la couleur initiale utilis‚e pour le texte du document. Vous
pouvez choisir n&apos.importe quelle couleur dans la liste propos‚e.
:p.:hp2.Gras:ehp2.
Si cette case est coch‚e, le texte sera initialement en gras. Si la
police utilis‚e dispose d&apos.une police sp‚ciale pour l&apos.attribut gras, elle
sera utilis‚eÿ; dans le cas contraire, le systŠme construira une police
correspondant … l&apos.attribut gras.
:p.:hp2.Italique:ehp2.
Si cette case est coch‚e, le texte sera initialement en italiques. Si la
police utilis‚e dispose d&apos.une police sp‚ciale pour l&apos.attribut italique,
elle sera utilis‚eÿ; dans le cas contraire, le systŠme construira une
police correspondant … l&apos.attribut italique.
:p.:hp2.Soulign‚:ehp2.
Si cette case est coch‚e, le texte sera initialement soulign‚.
:p.:hp2.Barr‚:ehp2.
Si cette case est coch‚e, le texte sera initialement barr‚.
:p.:hp2.Contour:ehp2.
Si cette case est coch‚e, seul le contour des caractŠres sera dessin‚.
:p.:hp2.Majuscules:ehp2.
Si cette case est coch‚e, le texte sera initialement en lettres
majuscules.

:h4 res=IDD_DEFAULTS_PAGE_LAYOUT1.Disposition, page 1
:artwork name='Francais\Default3.bmp' align=center.
:p.:hp2.Justification:ehp2.
D‚finit le type de justification horizontale … utiliser. Les
justifications possibles sont :hp2.Gauche:ehp2., :hp2.Centr‚:ehp2.,
:hp2.Droite:ehp2., :hp2.Gauche et Droite:ehp2.. La justification …
gauche aligne le texte sur la marge gauche de la page. La justification
centr‚e centre le texte entre les marges gauche et droite, mais ne
modifie pas l&apos.espacement entre les mots. La justification … droite
aligne le texte sur la marge droite de la page. Enfin, la justification
… gauche et … droite aligne le texte … la fois sur les marges gauche et
droite de la page, au besoin en modifiant l&apos.espacement entre les
mots.
:p.:hp2.Centrer la Page:ehp2.
Si cette case est coch‚e, toutes les pages seront centr‚es
verticalement.
:p.:hp2.Marges:ehp2.
Ces quatre champs permettent de sp‚cifier les valeurs initiales des
marges, en pouces ou en centimŠtres selon l&apos.unit‚ choisie. Les
marges ne peuvent d‚passer les limites impos‚es par le support
s‚lectionn‚.

:h4 res=IDD_DEFAULTS_PAGE_LAYOUT2.Disposition, page 2
:artwork name='Francais\Default4.bmp' align=center.
:p.:hp2.Tabulations:ehp2.
Ce champ vous permet de sp‚cifier l&apos.espacement par d‚faut utilis‚
par les tabulations. Si vous saisissez 0,500 ici, les taquets de
tabulations seront initialement plac‚s tous les demi-pouce ou
centimŠtre, … partir de la marge gauche et jusqu&apos.… la marge droite.
:p.:hp2.Indentation:ehp2.
Ce champ de saisie vous permet de d‚finir l&apos.indentation de paragraphe
qui sera appliqu‚e, par d‚faut, au d‚but du document. Si vous entrez la
valeur 0,5, le texte de la premiŠre ligne du document (et de chaque
premiŠre ligne de paragraphe) sera d‚cal‚ d&apos.un demi centimŠtre (ou
pouce, le cas ‚ch‚ant) vers la droite.
:p.:hp2.Interligne:ehp2.
Cette zone contient les choix d&apos.interligne disponibles.
L&apos.option :hp2.Normal:ehp2. correspond … un espacement vertical de
100%, l&apos.option :hp2.1 1/2:ehp2. … un espacement de 150% et
l&apos.option :hp2.Double:ehp2. … 200%.

:h4 res=IDD_DEFAULTS_PAGE_NATIONAL.Langue
:artwork name='Francais\Default5.bmp' align=center.
:p.:hp2.Page de codes:ehp2.
Ce champ permet de d‚finir la page de codes, ou le jeu de caractŠres
nationaux, utilis‚ par le document.
Voir :link reftype=hd res=61002.Pages de codes:elink. pour plus d&apos.information.
:p.:hp2.CentimŠtres:ehp2.
Si cette case est coch‚e, les mesures seront exprim‚es dans le systŠme
m‚trique. Les rŠgles horizontale et verticale seront gradu‚es en
centimŠtres.
:p.:hp2.Format de la date:ehp2.
Ce champ permet de d‚finir le format de la date … utiliser pour les
commandes :hp2.Date d&apos.impression:ehp2. et
:hp2.Aujourd&apos.hui:ehp2.. Voirÿ
:link reftype=hd res=IDD_DATEFMT.Format de la date:elink. pour plus d&apos.informations.

:h2 res=IDM_CONFIGURE_EXTENSIONS.Configurer les extensions... (Option de menu)
:p.Cette commande appele une boŒte de dialogue … partir de laquelle
peuvent ˆtre configur‚es les extensions install‚es.
:p.Voir :link reftype=hd res=IDD_CONFIGURE_EXTENSIONS.Configurer les extensions (Dialogue):elink. pour plus d&apos.informations.

:h3 res=IDD_CONFIGURE_EXTENSIONS.Configurer les extensions (Dialogue)
:p.Cette boŒte de dialogue permet de configurer les extensions install‚es.
:artwork name='Francais\ConfigEx.bmp' align=center.

:h3 res=IDM_LANGUAGE_MENU.Langue (Sous-menu)
:p.Ce sous-menu permet de choisir la langue utilis‚e pour les menus et
boŒtes de dialogue de l&apos.application.
:p.Voir :link reftype=hd res=IDM_LANGUAGES.Ligne du menu Langue:elink.
pour plus d&apos.informations.

:h4 res=IDM_LANGUAGES.Ligne du menu Langue (Option de menu)
:p.Les lignes du sous-menu :hp2.Langue:ehp2. permettent de choisir la
langue dans laquelle seront affich‚s les menus et boŒtes de dialogue de
l&apos.application.
Les langues sont exprim‚es dans leur forme d&apos.origine (®Espa¤ol¯ pour
l&apos.espagnol, par exemple).
Une fois la nouvelle langue choisie, les fenˆtres de l&apos.application sont
r‚affich‚es. Certains aspects du document (tel le format des dates) sont
aussi chang‚s.

:h2 res=IDM_EXIT.Quitter (Option de menu)
:p.Cette commande permet de quitter le programme.

:h1 res=IDM_EDIT_MENU.Menu dition
:i1.Menu dition
:p.Les commandes suivantes sont accessibles depuis le menu
:hp2.dition:ehp2.ÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_UNDO.D‚faire:elink.
:li.&sqbul.:link reftype=hd res=IDM_REDO.Refaire:elink.
:li.&sqbul.:link reftype=hd res=IDM_CUT.Couper:elink.
:li.&sqbul.:link reftype=hd res=IDM_COPY.Copier:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE.Coller:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEAR.Effacer:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEARCLIPBOARD.Effacer le Presse-papiers:elink.
:li.&sqbul.:link reftype=hd res=IDM_SELECT_ALL.Tout s‚lectionner:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM.Menu Coller depuis:elink.
:li.&sqbul.:link reftype=hd res=IDM_INSERT_FUNCTION.Menu Ins‚rer fonction:elink.
:li.&sqbul.:link reftype=hd res=IDM_SEARCH.Chercher...:elink.
:li.&sqbul.:link reftype=hd res=IDM_FINDNEXT.Trouver suivant:elink.
:li.&sqbul.:link reftype=hd res=IDM_GOTOPAGE.Aller vers page:elink.
:li.&sqbul.:link reftype=hd res=IDM_SPELL.Correcteur orthographique...:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM.Zoom:elink.
:esl.

:h2 res=IDM_UNDO.D‚faire (Option de menu)
:p.Cette commande annule la modification la plus r‚cemment effectu‚e sur le
document. Si vous utilisez plusieurs fois de suite cette fonction, les
modifications de moins en moins r‚centes sont annul‚es, jusqu&apos.… ce que
toutes les modifications apport‚es au document aient ‚t‚ annul‚es (le
document se trouve alors dans l&apos.‚tat o— il se trouvait quand il a ‚t‚
charg‚). Sur un nouveau document, le document finirait par ˆtre dans son
‚tat initial, c&apos.est … dire vide.
:p.Le raccourci clavier pour cette fonction est :hp2.Alt+Retour:ehp2..

:h2 res=IDM_REDO.Refaire (Option de menu)
:p.Si vous n&apos.avez pas modifi‚ le document depuis la derniŠre fois que vous
avez d‚fait une modification, cette fonction vous permet de refaire la
modification qui a ‚t‚ d‚faite. Il est possible de d‚faire toutes les
modifications apport‚es au document, puis de les refaire toutes.
Toutefois, si vous modifiez le document aprŠs un
:link reftype=hd res=IDM_UNDO.D‚faire:elink., toutes les modifications
ult‚rieures sont perdues et il devient impossible de les refaire.
:p.Le raccourci clavier pour Refaire est :hp2.Maj+Alt+Retour:ehp2..

:h2 res=IDM_CUT.Couper (Option de menu)
:p.Cette commande efface le texte s‚lectionn‚ du document et le place dans
le presse-papiers.
:p.Le raccourci clavier pour cette fonction est :hp2.Maj+Suppr:ehp2..

:h2 res=IDM_COPY.Copier (Option de menu)
:p.Cette commande recopie le texte s‚lectionn‚ du document vers le
presse-papiers.
:p.Le raccourci clavier pour cette fonction est :hp2.Ctrl+Inser:ehp2..

:h2 res=IDM_PASTE.Coller (Option de menu)
:p.Cette commande copie le contenu du presse-papiers vers le document, en
rempla‡ant le texte s‚lectionn‚ (en mode remplacement) ou en l&apos.ins‚rant
aprŠs le curseur (en mode insertion).
:p.Le raccourci clavier pour cette fonction est :hp2.Maj+Inser:ehp2..

:h2 res=IDM_CLEAR.Effacer (Option de menu)
:p.Cette commande efface le texte s‚lectionn‚ du document, sans alt‚rer le
presse-papiers. Cette commande est exactement ‚quivalente … une pression
de la touche :hp2.Suppr:ehp2..

:h2 res=IDM_CLEARCLIPBOARD.Effacer le Presse-papiers (Option de menu)
:p.Cette commande efface le contenu du presse-papiers.

:h2 res=IDM_SELECT_ALL.Selectionner tout (Option de menu)
Cette commande s‚lectionne tout le texte du document, du d‚but … la fin.
:p.Cette commande peut aussi ˆtre invoqu‚e en cliquant deux fois le
bouton 1 de la souris sur la zone situ‚e en haut et … gauche, au
croisement des rŠgles verticale et horizontale.

:h2 res=IDM_PASTE_FROM.Menu Coller depuis
:p.Ce menu permet de choisir une source pour un graphique devant ˆtre ins‚r‚
dans le document.
Les options suivantes sont disponiblesÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP.Coller depuis le presse-papiers:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN.Coller depuis l&apos.‚cran:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_FILE.Coller depuis un fichier graphique:elink.
:esl.

:h3 res=IDM_PASTE_FROM_CLIP.Menu Coller depuis le presse-papiers
:p.Ce menu permet de choisir une option de collage … partir du presse-papiers
du systŠme.
Les options suivantes sont disponiblesÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Coller un bitmap depuis le presse-papiers:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP_METAFILE.Coller un m‚tafichier depuis le presse-papiers:elink.
:esl.

:h4 res=IDM_PASTE_FROM_CLIP_BITMAP.Coller un bitmap depuis le presse-papiers (Option de menu)
:p.Si une image en mode points (bitmap) a ‚t‚ plac‚e dans le presse-papiers
du systŠme, cette option de menu vous permet de la coller dans votre
document. AprŠs avoir s‚lectionn‚ cette option, vous devez indiquer
l&apos.un des angles de la zone o— vous voulez coller l&apos.image.
Indiquez cet angle en le d‚signant … l&apos.aide du pointeur de la
souris, puis en pressant le bouton gauche. Tout en conservant le bouton
press‚, d‚placez le curseur de la souris vers le coin oppos‚ de la zone
o— l&apos.image doit ˆtre plac‚e. Un rectangle va se former et reflˆter
les d‚placements de la souris. Une fois la dimension souhait‚e atteinte,
relƒchez le bouton de la souris. L&apos.image sera coll‚e dans le
rectangle, sa taille ‚tant automatiquement adapt‚e.

:h4 res=IDM_PASTE_FROM_CLIP_METAFILE.Coller un m‚tafichier depuis le presse-papiers (Option de menu)
:p.Si un m‚tafichier graphique a ‚t‚ plac‚ dans le presse-papiers du
systŠme, cette option de menu vous permet d&apos.ins‚rer ce graphique
dans votre document. AprŠs avoir s‚lectionn‚ cette option, vous devez
indiquer la position du graphique dans votre document, comme pour
l&apos.option de menu :link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Coller un bitmap depuis le presse-papiers:elink..

:h3 res=IDM_PASTE_FROM_SCRN.Menu Coller depuis l&apos.‚cran
:p.Ce menu permet de choisir une option de Collage utilisant une image
graphique captur‚e sur l&apos.affichage.
Les options suivantes sont disponiblesÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_ENTIRE.Coller le bureau:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_FRAME.Coller une zone de l&apos.‚cran:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_WINDOW.Coller une fenˆtre du bureau:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_RECT.Coller une zone rectangulaire de l&apos.affichage:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_HIDE.Hide:elink.
:esl.

:h4 res=IDM_PASTE_FROM_SCRN_ENTIRE.Coller le bureau (Option de menu)
:p.Quand vous s‚lectionnez cette option de menu, une image du bureau, dans son
‚tat actuel, est r‚alis‚e et vous devez ensuite indiquer l&apos.endroit
o— la positionner comme pour l&apos.option de menu 
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Coller un bitmap depuis le presse-papiers:elink..

:note.Attention, le volume du graphique cr‚‚ par cette option peut ˆtre
relativement important. Quelle que soit la taille de l&apos.image sur le
document, la quantit‚ de m‚moire occup‚e par le graphique reste la mˆme,
et une image du bureau complet peut repr‚senter de 100 … 800ko, en
fonction de la r‚solution de l&apos.‚cran et du nombre de couleurs.

:h4 res=IDM_PASTE_FROM_SCRN_FRAME.Coller une zone de l&apos.‚cran (Option de menu)
:p.Cette fonction n&apos.est pas encore disponible.

:h4 res=IDM_PASTE_FROM_SCRN_WINDOW.Coller une fenˆtre du bureau (Option de menu)
:p.Cette fonction n&apos.est pas encore disponible.

:h4 res=IDM_PASTE_FROM_SCRN_RECT.Coller une zone rectangulaire de l&apos.affichage (Option de menu)
:p.Cette fonction n&apos.est pas encore disponible.

:h4 res=IDM_PASTE_FROM_SCRN_HIDE.Cacher (Option de menu)
:p.Quand cette option est coch‚e, les fenˆtres de l&apos.application sont cach‚es
si vous appelez une des commandes du menu
:link reftype=hd res=IDM_PASTE_FROM_SCRN.Coller depuis l&apos.cran:elink..
Une fois la commande ex‚cut‚e, l&apos.application est … nouveau affich‚e.

:h3 res=IDM_PASTE_FROM_FILE.Coller depuis un fichier graphique (Option de menu)
:p.Quand vous s‚lectionnez cette option, la boŒte de dialogue de s‚lection de
fichier est affich‚e. Vous devez choisir le fichier graphique qui sera
ins‚r‚ dans votre document. Les types de fichier actuellement g‚r‚s
sontÿ&colon.
:sl compact.
:li.&sqbul.Image en mode points (bitmap) IBM OS/2 (.BMP)
:li.&sqbul.M‚tafichier (MetaFile) IBM OS/2 (.MET)
:li.&sqbul.Image en mode points (bitmap) MicroSoft Windows (.BMP)
:li.&sqbul.Graphic Interchange Format (.GIF)
:li.&sqbul.Joint Photographic Experts Group File Interchange Format (.JPG, .JPEG ou .JFIF)
:li.&sqbul.Fichiers PC Paintbrush (.PCX ou .PCC)
:li.&sqbul.Tag Image File Format (.TIF ou .TIFF)
:esl.
:p.AprŠs avoir s‚lectionn‚ un fichier, vous devrez indiquer
l&apos.endroit o— l&apos.ins‚rer dans le document, comme pour
l&apos.option de menu 
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Coller un bitmap depuis le presse-papiers:elink..

:h2 res=IDM_INSERT_FUNCTION.Menu Ins‚rer fonction
:p.Ce sous-menu vous permet d&apos.ins‚rer un certain nombre de codes fonctions
dans le document. Les options suivantes sont disponiblesÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_DATE.Date:elink.
:li.&sqbul.:link reftype=hd res=IDM_CURRENTPAGE.Num‚ro de la page:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGECOUNT.Nombre de pages:elink.
:li.&sqbul.:link reftype=hd res=IDM_SEPARATOR.Separator:elink.
:li.&sqbul.:link reftype=hd res=IDM_BARCODE.Code … barres...:elink.
:esl.
:note.Ce sous-menu ne fait pas partie du menu contextuel d&apos.un texte
s‚lectionn‚.

:h3 res=IDM_DATE.Ins‚rer la Date (Option de menu)
:p.Ce sous-menu vous donne accŠs aux fonctions d&apos.insertion de date.
Trois options sont propos‚esÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_DATETODAY.Date d&apos.aujourd&apos.hui:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATECURRENT.Date d&apos.impression:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATECHANGED.Date de mise … jour:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATEFMT.Format de la Date...:elink.
:esl.
:note.Ce sous-menu ne fait pas partie du menu contextuel d&apos.un texte
s‚lectionn‚.

:h4 res=IDM_DATETODAY.Date d&apos.aujourd&apos.hui (Option de menu)
:p.Cette option de menu insŠre dans le document un code qui affiche la date
… laquelle le code a ‚t‚ ins‚r‚. Cette date n&apos.‚volue pas, et ne peut pas
ˆtre ‚dit‚e.
Quand vous d‚placez le curseur dans le texte, cette date est consid‚r‚e
comme un unique caractŠre ou code. Le format d&apos.affichage de la date peut
ˆtre modifi‚ dans la boŒte de dialogue
:link reftype=hd res=IDD_DATEFMT.Format de la date...:elink. mais vous devez
modifier le format avant le code de la date, car ce code utilise le
format en vigueur … sa position dans le document.
:note.Cette option n&apos.est pas disponible dans le menu contextuel d&apos.un
texte s‚lectionn‚.

:h4 res=IDM_DATECURRENT.Date d&apos.impression (Option de menu)
:p.Cette commande insŠre dans le document un code qui affiche toujours la
date courante. La date affich‚e ou imprim‚e est toujours la date
courante, et non la date … laquelle la commande a ‚t‚ ex‚cut‚e. La date
‚tant pr‚sente sous la forme d&apos.un code, elle ne peut ˆtre modifi‚e.
Quand le curseur passe sur la date, elle est consid‚r‚e comme un seul
caractŠre. Le format sous lequel la date apparaŒt peut ˆtre modifi‚ via
la boŒte de dialogue :link reftype=hd res=IDD_DATEFMT.Format de la date...:elink., 
mais vous devez modifier le format avant le code de la
date, car ce code utilise le format en vigueur … sa position dans le
document.
:note.Cette option n&apos.est pas disponible dans le menu contextuel
d&apos.un texte s‚lectionn‚.

:h4 res=IDM_DATECHANGED.Date de mise … jour (Option de menu)
:p.Cette commande insŠre dans le document un code qui affiche la date de
derniŠre modification du document. La date sera modifi‚e quand le
document sera mis … jour. La date ‚tant pr‚sente sous la forme d&apos.un
code, elle ne peut ˆtre modifi‚e. Quand le curseur passe sur la date,
elle est consid‚r‚e comme un seul caractŠre. Le format sous lequel la
date apparaŒt peut ˆtre modifi‚ via la boŒte de dialogue 
:link reftype=hd res=IDD_DATEFMT.Format de la date...:elink., mais vous devez modifier le
format avant le code de la date, car ce code utilise le format en
vigueur … sa position dans le document.
:note.Cette option n&apos.est pas disponible dans le menu contextuel
d&apos.un texte s‚lectionn‚.

:h4 res=IDM_DATEFMT.Format de la Date... (Option de menu)
:p.La s‚lection de cette option de menu provoque l&apos.ouverture de la
boŒte de dialogueÿ:link reftype=hd res=IDD_DATEFMT.Format de la Date...:elink., 
qui permet de modifier le format utilis‚ pour la cr‚ation des dates.

:h5 res=IDD_DATEFMT.Format de la Date... (Dialogue)
:p.Cette boŒte de dialogue permet de modifier le format utilis‚ pour la
cr‚ation des dates.
:artwork name='Francais\DateFmt.bmp' align=center.
:p.La chaŒne de caractŠres cr‚‚e pour repr‚senter la
date est le contenu du champ de saisie, certains caractŠres pr‚c‚d‚s du
signe :hp1.%:ehp1. ayant une signification particuliŠre. Ces caractŠres
sont les suivantsÿ&colon.
:sl compact.
:li.&percent.s - Jour de la semaine (dimanche, lundi, etc.)
:li.&percent.t - Abr‚viation du jour de la semaine (dim, lun, etc.)
:li.&percent.j - Jour du mois, sous forme num‚rique.
:li.&percent.m - Mois de l&apos.ann‚e, sous forme num‚rique.
:li.&percent.M - Mois de l&apos.ann‚e (janvier, f‚vrier, etc...)
:li.&percent.n - Abr‚viation du mois (jan, fev, etc.)
:li.&percent.a - Ann‚e (4 chiffres).
:li.&percent.b - Abr‚viation de l&apos.ann‚e (2 chiffres).
:esl.
:p.
Tout autre texte entr‚ dans ce champ est copi‚ directement dans la date
cr‚‚e. Par exemple, la chaŒne de format &odqf.%s %j %M %a&cdqf. deviendrait
&odqf.:hp1.jeudi 13 juillet 1995:ehp1.&cdqf..
:p.
Pour valider le nouveau format de la date, s‚lectionnez la plaquette
:hp2.Ok:ehp2.. Le document actif sera remis en forme en cons‚quence.
:p.
Si vous s‚lectionnez la plaquette :hp2.Appliquer:ehp2., le nouveau format de
date est appliqu‚ au document actif.
:p.
Pour refermer la boŒte de dialogue sans sauvegarder les modifications,
s‚lectionnez la plaquette :hp2.Annuler:ehp2. ou pressez la touche
:hp2.chap:ehp2. Les modifications valid‚es par la plaquette
:hp2.Appliquer:ehp2. ne seront pas annul‚es.

:h3 res=IDM_CURRENTPAGE.Num‚ro de la page (Option de menu)
:p.Cette option de menu insŠre dans le document un code qui affiche le
num‚ro de la page o— se trouve ce code.

:h3 res=IDM_PAGECOUNT.Nombre de pages (Option de menu)
:p.Cette option de menu insŠre dans le document un code qui affiche le
nombre total de pages dans le document.

:h3 res=IDM_SEPARATOR.Separateur (Option de menu)
Cette option de menu insŠre une fonction sp‚ciale de s‚paration dans le
document. Le s‚parateur termine la ligne en cours, dessine un trait de
s‚paration sur la ligne suivante, et d‚place le curseur sur la ligne
suivante.

:h3 res=IDM_BARCODE.Code … barres (Option du menu)
Ce menu appelle un dialogue avec lequel vous pouvez pr‚parer l&apos.insertion
d&apos.un graphique de code … barres dans le document. Pour plus
d&apos.informations, voir 
:link reftype=hd res=IDD_BARCODE.Insertion de Code … barres (Dialogue):elink..

:h4 res=IDD_BARCODE.Insertion d&apos.un Code … Barres (Dialogue)
Ce dialogue vous permet de sp‚cifier le type et le texte
d&apos.un graphic de Code … Barres … ˆtre ins‚r‚ dans le document.
:artwork name='Francais\BarCode.bmp' align=center.
Vous devez
selectionnerle type de Code … Barres … partir de la liste fournie et ensuite 
entrer le texte qui devra ˆtre cod‚ dans l&apos.espace d&apos.insertion. Lorsque 
compl‚t‚, pressez la touche ENTREZ ou cliquez le bouton O.K., … ce moment 
l&apos.on vous demandera de positionner le graphique sur le document courant, 
comme dans le cas du 
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Coller un bitmap depuis le presse-papier (Option de menu):elink..

:h2 res=IDM_SEARCH.Chercher... (Option de menu)
:p.Cette option de menu ouvre une boŒte de dialogue dans laquelle vous
pouvez d‚finir une chaŒne de caractŠres … rechercher, et ‚ventuellement
une autre chaŒne de caractŠres par laquelle remplacer la premiŠre.
:p.Voir :link reftype=hd res=IDD_SEARCH.Chercher... (Dialogue):elink. pour
plus d&apos.informations.
:p.Le raccourci clavier pour cette fonction est :hp2.Ctrl+C:ehp2..

:h3 res=IDD_SEARCH.Chercher... (Dialogue)
:p.Cette boŒte de dialogue vous permet de d‚finir une chaŒne de caractŠres
… rechercher pour les commandes
:link reftype=hd res=IDM_SEARCH.Chercher:elink. et
:link reftype=hd res=IDM_FINDNEXT.Trouver suivant:elink., et ‚ventuellement
une autre chaŒne de caractŠres par laquelle remplacer la premiŠre.

:artwork name='Francais\Search.bmp' align=center.
:p.Pour rechercher une chaŒne de caractŠres, saisissez la dans la
premiŠre zone de saisie puis pressez :hp2.Entr‚e:ehp2. ou s‚lectionnez la
plaquette :hp2.Chercher:ehp2.. La chaŒne sera recherch‚e dans le document
et mise en surbrillance. La boŒte de dialogue reste pr‚sente … l&apos.‚cran
pour vous permettre de modifier la chaŒne … rechercher.
:p.Pour remplacer un texte par un autre, saisissez le texte original
dans la premiŠre zone de saisie et le nouveau texte dans la seconde zone
de saisie, puis pressez la touche :hp2.Entr‚e:ehp2. pour trouver le
texte … remplacer, ou s‚lectionnez la plaquette :hp2.Remplacer:ehp2. pour
rechercher et remplacer le texte. Vous pouvez r‚p‚ter l&apos.operation de
remplacement pour chaque occurence du texte recherch‚.
:p.Pour remplacer un texte par un autre, puis chercher l&apos.occurence
suivante du texte d&apos.origine, s‚lectionnez la plaquette :hp2.Remplacer
&amp. Chercher:ehp2..
:p.Pour remplacer toutes les occurences du texte original par le
nouveau, s‚lectionnez la plaquette :hp2.Remplacer tous:ehp2..
:p.Vous pouvez faire en sorte que les recherches soient influenc‚es ou
non par la casse des caractŠres en s‚lectant ou non la case … cocher
:hp2.Minuscules/majuscules:ehp2..
:p.Si vous d‚sirez que votre recherche ob‚isse aux rŠgles des
Expressions R‚guliŠre (®Regular Expressions¯) UNIX, cochez la
case :hp2.Expression R‚guliŠre:ehp2..
:p.Pour fermer la boŒte de dialogue, pressez la touche :hp2.chap:ehp2.
ou s‚lectionnez la plaquette :hp2.Annuler:ehp2.. Le contenu des champs de
saisie est conserv‚ pour la prochaine utilisation de cette fonction.

:h2 res=IDM_FINDNEXT.Trouver suivant (Option de menu)
:p.Si vous avez d‚fini une chaŒne de caractŠres … rechercher dans la
boŒte de dialogue
:link reftype=hd res=IDD_SEARCH.Chercher... (Dialogue):elink., cette option de
menu recherchera l&apos.occurence suivante de la chaŒne sp‚cifi‚e et la
s‚lectionnera.
:p.Voir :link reftype=hd res=IDD_SEARCH.Chercher... (Dialogue):elink. pour plus
d&apos.informations.
:p.Le raccourci clavier pour cette fonction est :hp2.Ctrl+F:ehp2.

:h2 res=IDM_GOTOPAGE.Aller vers page (Option de menu)
Cette option vous permet d'afficher une page donn‚e du document.
Voir l'aide de la boŒte de dialogue
:link reftype=hd res=IDD_GOTOPAGE.Aller vers page:elink.
pour plus d'informations.

:h3 res=IDD_GOTOPAGE.Aller vers page (Dialogue)
Cette boŒte de dialogue vous permet de sp‚cifier le num‚ro de la page du
document que vous d‚sirez afficher. Saisissez le num‚ro de la page puis
pressez :hp2.Entr‚e:ehp2., ou bien pressez :hp2.chap:ehp2. pour annuler
cette fonction.

:h2 res=IDM_SPELL.Correcteur orthographique (Option de menu)
:p.Cette option est disponible si le module optionnel de contr“le
orthographique est install‚ sur votre systŠme et si les fichiers
dictionnaires ont pu ˆtre ouverts. Quand vous s‚lectionnez cette option,
le correcteur orthographique inspecte le document actif … partir de la
position du curseur, … la recherche de fautes d&apos.orthographe. Quand un
mot mal orthographi‚ est trouv‚, il est mis en surbrillance et la boŒte
de dialogue :link reftype=hd res=IDD_SPELL.Correcteur orthographique:elink.
est affich‚e, vous permettant de pr‚ciser au correcteur l&apos.action …
entreprendre.
:p.Le raccourci clavier pour cette fonction est :hp2.F5:ehp2..

:h3 res=IDD_SPELL.Correcteur orthographique (BoŒte de dialogue)
:p.Cette boŒte de dialogue est affich‚e quand le correcteur orthographique
trouve un mot dont il ne reconnaŒt pas l&apos.orthographe. Le mot suspect est
affich‚, ainsi qu&apos.une liste de mots ressemblants issus du dictionnaire.

:artwork name='Francais\Spell.bmp' align=center.
:p.Si vous d‚sirez ignorer cette anomalie, pressez la touche
:hp2.Entr‚e:ehp2. ou s‚lectionnez la plaquette :hp2.Ignorer:ehp2..
:p.Si vous souhaitez que le correcteur orthographique ignore ce mot pour
le reste de la session, s‚lectionnez la plaquette :hp2.Confirmer:ehp2..
:p.Pour ajouter le mot … votre dictionnaire personnel, en conservant
la casse, s‚lectionnez la plaquette :hp2.Ajouter:ehp2..
:p.Pour ajouter le mot … votre dictionnaire, en caractŠres minuscules,
s‚lectionnez la plaquette :hp2.Ajouter minuscule:ehp2.. Un mot en minuscules
peut correspondre … ce mˆme mot quelle qu&apos.en soit la casse.
:p.Si vous d‚sirez appliquer l&apos.une des corrections propos‚es,
s‚lectionnez le mot dans la liste puis pressez :hp2.Entr‚e:ehp2., ou
cliquez deux fois sur le mot. Le mot original du texte sera remplac‚ par
le mot choisi.
:p.Si l&apos.orthographe correcte du mot ne figure pas dans la liste des
corrections propos‚es, vous pouvez saisir une orthographe diff‚rente
dans le champ de saisie au dessus de la liste des corrections, puis
s‚lecter la plaquette :hp2.Remplacer:ehp2..
:p.Pour quitter le correcteur orthographique sans appliquer de
correction, pressez la touche :hp2.chap:ehp2. ou s‚lectionnez la plaquette
:hp2.Annuler:ehp2.. Les corrections d‚j… appliqu‚es sont conserv‚es,
mais elles peuvent ˆtre annul‚es grƒce … la commande
:link reftype=hd res=IDM_UNDO.D‚faire:elink..

:h2 res=IDM_ZOOM.Zoom (Sous-menu)
:p.Cette option de menu ouvre un sous-menu des diff‚rentes commandes de
zoom sur le document disponibles. Les commandes suivantes sont
disponiblesÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_100.Zoom 100%:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLWIDTH.Zoom Pleine Largeur:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLHEIGHT.Zoom Pleine Hauteur:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_MARGINS.Zoom Marges:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_ALL.Zoom Tout:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_WINDOW.Zoom Fenˆtre:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_SET.Zoom Sp‚cial...:elink.
:esl.

:h3 res=IDM_ZOOM_100.Zoom 100% (Option de menu)
:p.Cette commande choisit une ‚chelle d&apos.affichage de 100%. Le
contenu de l&apos.‚cran devrait ˆtre imprim‚ avec exactement la mˆme
taille, … condition que les paramŠtres d&apos.‚chelle du systŠme (hors
de contr“le du programme) soient renseign‚s correctement.

:h3 res=IDM_ZOOM_FULLWIDTH.Zoom Pleine Largeur (Option de menu)
:p.Cette commande choisit une ‚chelle d&apos.affichage telle que la page
occupe toute la largeur de sa fenˆtre d&apos.affichage. Ceci peut
r‚duire ou augmenter l&apos.‚chelle d&apos.affichage, selon la largeur
de la fenˆtre d&apos.affichage.

:h3 res=IDM_ZOOM_FULLHEIGHT.Zoom Pleine Hauteur (Option de menu)
:p.Cette commande choisit une ‚chelle d&apos.affichage telle que la page
occupe toute la hauteur de sa fenˆtre d&apos.affichage. Ceci r‚duit
habituellement l&apos.‚chelle d&apos.affichage, r‚sultant en un
affichage plus petit que ce que donnera l&apos.impression.

:h3 res=IDM_ZOOM_MARGINS.Zoom Marges (Option de menu)
:p.Cette commande choisit une ‚chelle d&apos.affichage telle que la
portion de document situ‚e entre les marges horizontales occupe toute la
largeur de la fenˆtre d&apos.affichage. Habituellement, ceci augmente
l&apos.‚chelle d&apos.affichage, selon la largeur de la fenˆtre
d&apos.affichage.

:h3 res=IDM_ZOOM_ALL.Zoom Tout (Option de menu)
:p.Cette commande choisit l&apos.‚chelle d&apos.affichage la plus petite
parmis les ‚chelles correspondant … :hp1.Zoom Pleine Hauteur:ehp1. et
:hp1.Zoom Pleine Largeur:ehp1.. De cette fa‡on, la page entiŠre
apparaŒtra dans la fenˆtre, aussi petite ou difforme que soit la fenˆtre
du document.
:p.Le raccourci clavier pour cette fonction est :hp2.Ctrl+Z:ehp2.ÿ;
quand le raccourci clavier est utilis‚, cette fonction bascule entre les
modes :hp1.Zoom Tout:ehp1. et le type de zoom sp‚cifi‚ dans les :link
reftype=hd res=IDD_PREFERENCES.Pr‚f‚rences:elink..

:h3 res=IDM_ZOOM_WINDOW.Zoom Fenˆtre (Option de menu)
:p.Cette commande vous permet de d‚finir une portion rectangulaire du
document actif sur laquelle zoomer. Une fois la portion s‚lectionn‚e,
l&apos.‚chelle d&apos.affichage sera modifi‚e et le document
repositionn‚ de fa‡on … ce que la zone s‚lectionn‚e soit visible et
occupe la plus grande partie possible de la fenˆtre.

:h3 res=IDM_ZOOM_SET.Zoom Sp‚cial... (Option de menu)
:p.Cette commande entraŒne l&apos.ouverture d&apos.une boŒte de dialogue permettant
de modifier le facteur de zoom.
:p.Voir :link reftype=hd res=IDD_ZOOM_SET.Zoom Sp‚cial... (Dialogue):elink.
pour plus d&apos.informations.

:h4 res=IDD_ZOOM_SET.Zoom Sp‚cial... (Dialogue)
:p.Cette boŒte de dialogue permet … l&apos.utilisateur de sp‚cifier un facteur
de zoom quelconque. Utilisez les touches haut et bas pour modifier le
facteur de zoom, ou saisissez directement la valeur, puis pressez la
touche :hp2.Entr‚e:ehp2. ou s‚lectionnez la plaquette :hp2.Ok:ehp2. pour
appliquer le nouveau facteur de zoom.
:artwork name='Francais\ZoomSet.bmp' align=center.
:p.Pour appliquer le nouveau facteur de zoom sans refermer la boŒte de
dialogue, s‚lectionnez la plaquette :hp2.Appliquer:ehp2.. Un facteur de zoom
valid‚ ainsi restera actif aprŠs la fermeture de la boŒte de dialogue,
mˆme si vous utilisez la touche :hp2.chap:ehp2..
:p.Pour refermer la boŒte de dialogue sans appliquer les changements
apport‚s au facteur de zoom, pressez la touche :hp2.chap:ehp2. ou
s‚lectionnez la plaquette :hp2.Annuler:ehp2.. Les modifications valid‚es par
la plaquette :hp2.Appliquer:ehp2. ne seront pas annul‚es.

:h1 res=IDM_LAYOUT_MENU.Menu Disposition
:i1.Menu Disposition
:p.Les commandes suivantes sont accessibles depuis le menu
:hp4.Disposition:ehp4.ÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_MARGINS.Marges...:elink.
:li.&sqbul.:link reftype=hd res=IDM_TABS.Tabulations...:elink.
:li.&sqbul.:link reftype=hd res=IDM_INDENT.Indentation...:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINESPACING_MENU.Interligne:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUSTIFICATION.Justification:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGECENTER.Centrer la Page:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGEBREAK.Saut de Page:elink.
:li.&sqbul.:link reftype=hd res=IDM_HEADER.En-tˆte...:elink.
:li.&sqbul.:link reftype=hd res=IDM_FOOTER.Pied de page...:elink.
:esl.

:h2 res=IDM_MARGINS.Marges... (Option de menu)
:p.Cette commande entraŒne l&apos.apparition d&apos.une boŒte de dialogue … partir de
laquelle il est possible de d‚finir les quatre marges (haut, bas,
gauche, droite).
:p.Voirÿ:link reftype=hd res=IDD_MARGINS.Marges (Dialogue):elink. pour plus d&apos.informations.

:h3 res=IDD_MARGINS.Marges (Dialogue)
:p.Cette boŒte de dialogue vous permet de sp‚cifier les marges
d&apos.impression de gauche, droite, du haut et du bas de la page. Ces
marges sont exprim‚es dans l&apos.unit‚ de mesure s‚lectionn‚e (pouces ou
centimŠtres) ; elles repr‚sentent des distances … partir des bords du
support d&apos.impression actif.
:artwork name='Francais\Margins.bmp' align=center.
:p.Quand de nouvelles valeurs sont entr‚es pour les marges, les rŠgles
horizontale et verticale du document actif sont modifi‚es pour refl‚ter
les modifications, mais le document lui-mˆme n&apos.est pas remis en
forme.
:p.Pressez la touche :hp2.Entr‚e:ehp2. ou s‚lectionnez la plaquette
:hp2.Ok:ehp2. pour valider les valeurs saisies. Dans le cas o— les
marges ne sont pas applicables sur le support d&apos.impression en
cours, vous serez averti par un message d&apos.erreur et la boŒte de
dialogue restera ouverte.
:p.Si vous s‚lectionnez la plaquette :hp2.Appliquer:ehp2., les nouvelles marges
sont appliqu‚es au document actif, et deviennent les nouvelles marges.
:p.Pour fermer la boŒte de dialogue et annuler les modifications saisies,
pressez la touche :hp2.chap:ehp2. ou s‚lectionnez la plaquette
:hp2.Annuler:ehp2.. Les modifications valid‚es par la plaquette
:hp2.Appliquer:ehp2. ne sont pas annul‚es.
:note.Les nouvelles marges gauche et droite prendront effet … partir de
la ligne suivante, … moins que la commande de d‚finition des marges
n&apos.ait ‚t‚ ins‚r‚e dans le document au tout d‚but d&apos.une
nouvelle ligne. Les marges haut et bas seront appliqu‚es … partir de la
page suivante, … moins que la commande de d‚finition des marges
n&apos.ait ‚t‚ ins‚r‚e au tout d‚but d&apos.une nouvelle page.

:h2 res=IDM_TABS.Tabulations... (Option de menu)
:p.Cette commande invoque une boŒte de dialogue … l&apos.aide de laquelle
il est possible de modifier les positions des taquets de tabulation du
document. Les nouvelles valeurs seront appliqu‚es … partir de la
position du curseur.
:p.Voirÿ:link reftype=hd res=IDD_TABS.Tabulations (Dialogue):elink. pour plus d&apos.informations.

:h3 res=IDD_TABS.Tabulations (Dialogue)
:p.Cette boŒte de dialogue vous permet d&apos.ajouter et de supprimer des
taquets de tabulations. Vos modifications sont imm‚diatement visibles
sur la rŠgle horizontale de la fenˆtre active.
:artwork name='Francais\Tabs.bmp' align=center.
:p.Pour positionner un taquet de tabulation, entrez sa position dans le
champ de saisie, puis s‚lectionnez la plaquette :hp2.D‚finir:ehp2..
:p.Pour supprimer un taquet de tabulation, entrez sa position dans le
champ de saisie, puis s‚lectionnez la plaquette :hp2.Effacer:ehp2..
:p.Une fois que vous avez effectu‚ toutes les modifications que vous
d‚sirez apporter aux tabulations de votre document, pressez la touche
:hp2.Entr‚e:ehp2. ou s‚lectionnez la plaquette :hp2.Ok:ehp2.. Les modifications
appport‚es seront appliqu‚es au document … partir de la position du
curseur.
:p.Si vous s‚lectionnez la plaquette :hp2.Appliquer:ehp2., les nouvelles
d‚finitions des tabulations sont appliqu‚es imm‚diatement au document.
:p.Pour fermer la boŒte de dialogue et annuler les modifications
effectu‚es, pressez la touche :hp2.chap:ehp2. ou s‚lectionnez la plaquette
:hp2.Annuler:ehp2.. Les modification valid‚es … l&apos.aide de la plaquette
:hp2.Appliquer:ehp2. ne sont pas annul‚es.

:h2 res=IDM_INDENT.Indentation... (Option de menu)
:p.Cette commande provoque l&apos.ouverture d&apos.une boŒte de dialogue vous
permettant de d‚finir la taille de l&apos.indentation … utiliser au d‚but de
chaque nouveau paragraphe.
:p.Voirÿ:link reftype=hd res=IDD_INDENT.Indentation (Dialogue):elink. pour plus d&apos.informations.

:h3 res=IDD_INDENT.Indentation (Dialogue)
:p.Cette boŒte de dialogue vous permet de d‚finir l&apos.indentation
utilis‚e … partir de la position du curseur.
:artwork name='Francais\Indent.bmp' align=center.
:p.Vous pouvez saisir la nouvelle distance d&apos.indentation, puis
s‚lecter la plaquette :hp2.Ok:ehp2. pour prendre en compte la nouvelle
valeur. Le document actif sera reformat‚ en cons‚quence.
:p.Si vous s‚lectionnez la plaquette :hp2.Appliquer:ehp2., la nouvelle
distance d&apos.indentation est appliqu‚e imm‚diatement au document
actif.
:p.Pour fermer la boŒte de dialogue sans enregistrer les modifications,
s‚lectionnez la plaquette :hp2.Annuler:ehp2. ou pressez la touche
:hp2.chap:ehp2.. Les modifications valid‚es par la plaquette
:hp2.Appliquer:ehp2. ne seront pas annul‚es.

:h2 res=IDM_LINESPACING_MENU.Menu interligne
:p.Ce sous-menu propose les diff‚rents interlignes disponibles.
:p.L&apos.interligne actif est coch‚.
:p.Voir :link reftype=hd res=IDM_LINESPACINGS.Entr‚es du menu interligne:elink.
pour plus d&apos.informations.

:h3 res=IDM_LINESPACINGS.Entr‚es du menu interligne
:p.Ces lignes de menu repr‚sentent les options d&apos.interligne disponibles.
Les interlignes sont d‚finis comme des pourcentages d&apos.un interligne
normal pour la police et la taille en cours. Quand une de ces options
d&apos.interligne est choisie, l&apos.interligne correspondant est appliqu‚ … la
zone de texte s‚lectionn‚e, ou … partir de la position du curseur si
aucun texte n&apos.est s‚lectionn‚.

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

:h2 res=IDM_JUSTIFICATION.Justification (Option de menu)
:p.Ce sous-menu pr‚sente les diff‚rentes options de justification
horizontale du texte. Les options disponibles sontÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_JUST_LEFT.Justification … Gauche:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_CENTER.Justification au Centre:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_RIGHT.Justification … Droite:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_FULL.Justification … Gauche et … Droite:elink.
:esl.

:h3 res=IDM_JUST_LEFT.Justification … Gauche (Option de menu)
:p.Cette option de menu insŠre dans le document une commande entraŒnant la
justification … gauche du texte, … partir de la ligne suivant la
position du curseur, … moins que la commande ne soit ins‚r‚e au tout
d‚but d&apos.une ligne, auquel cas la commande prendra effet imm‚diatement.

:h3 res=IDM_JUST_CENTER.Justification au Centre (Option de menu)
:p.Cette option de menu insŠre dans le document une commande entraŒnant la
justification centr‚e du texte, … partir de la ligne suivant la position
du curseur, … moins que la commande ne soit ins‚r‚e au tout d‚but
d&apos.une ligne, auquel cas la commande prendra effet imm‚diatement.

:h3 res=IDM_JUST_RIGHT.Justification … Droite (Option de menu)
:p.Cette option de menu insŠre dans le document une commande entraŒnant la
justification … droite du texte, … partir de la ligne suivant la
position du curseur, … moins que la commande ne soit ins‚r‚e au tout
d‚but d&apos.une ligne, auquel cas la commande prendra effet imm‚diatement.

:h3 res=IDM_JUST_FULL.Justification Gauche et Droite (Option de menu)
:p.Cette option de menu insŠre dans le document une commande entraŒnant la
justification du texte … droite et … gauche, … partir de la ligne
suivant la position du curseur, … moins que la commande ne soit ins‚r‚e
au tout d‚but d&apos.une ligne, auquel cas la commande prendra effet
imm‚diatement. La taille des espaces … l&apos.int‚rieur du texte
justifi‚ … gauche et … droite est adapt‚e pour aligner le texte sur les
marges gauche et droite. Les lignes se terminant par un retour chariot
ou un saut de page sont justifi‚es … gauche.

:h2 res=IDM_PAGECENTER.Centrer la Page (Option de menu)
:p.Cette option de menu insŠre dans le document une commande entraŒnant le
centrage vertical de la page, … partir de la page suivanteÿ; si la
commande est entr‚e au tout d‚but d&apos.une nouvelle page, son effet
est imm‚diat.
:note.Cette fonction ne peut ˆtre utilis‚e si des graphiques sont
pr‚sents sur la pageÿ; le cas ‚ch‚ant, la fonction est inactive.

:h2 res=IDM_PAGEBREAK.Saut de Page (Option de menu)
:p.Cette commande provoque l&apos.insertion d&apos.un saut de page dans le document …
la position du curseur. Le curseur se d‚placera vers le d‚but de la
nouvelle page.
:p.Le raccourci clavier pour cette fonction est Ctrl+Entr‚e.
:note.Cette option n&apos.est pas disponible dans le menu contextuel
d&apos.un texte s‚lectionn‚.

:h2 res=IDM_HEADER.En-tˆte... (Option de menu)
Permet d'acc‚der … la boŒte de dialogue dans laquelle vous d‚finissez
l'apparence de l'en-tˆte.

:h3 res=IDD_HEADER.En-tˆte (Dialogue)
Cette boŒte de dialogue vous permet de d‚finir l'en-tˆte. Pour le
moment, l'en-tˆte doit ˆtre fourni dans le format natif d'Escriba. Plus
tard, la saisie se fera en WYSIWYG. La plupart des codes de fonction
(comme PageNumber, PageCount, LeftJustify, etc) sont valides pour le
format d'en-tˆte, mais pas tous (PrinterSettings).

:h2 res=IDM_FOOTER.Pied de page... (Option de menu)
Permet d'acc‚der … la boŒte de dialogue dans laquelle vous d‚finissez
l'apparence du pied de page.

:h3 res=IDD_FOOTER.Pied de page (Dialogue)
Cette boŒte de dialogue vous permet de d‚finir le pied de page. Pour le
moment, le pied de page doit ˆtre fourni dans le format natif d'Escriba.
Plus tard, la saisie se fera en WYSIWYG. La plupart des codes de
fonction (comme PageNumber, PageCount, LeftJustify, etc) sont valides
pour le format de pied de page, mais pas tous (PrinterSettings).

:h1 res=IDM_FONT_MENU.Menu Polices
:i1.Menu Police
:p.Ce menu propose la liste de toutes les polices de caractŠres disponibles.
:p.La police active est coch‚e.
:p.Voir :link reftype=hd res=IDM_FONTS.Entr‚es du menu Polices:elink.
pour plus d&apos.informations.

:h2 res=IDM_FONTS.Entr‚es du menu Polices
:p.Chaque ligne du menu repr‚sente une des polices de caractŠres
disponibles, affich‚e dans la police correspondante, en 18 points. La
s‚lection d&apos.une police dans le menu entraŒne son application … la
zone de texte s‚lectionn‚e, ou … partir de la position du curseur si
aucun texte n&apos.est s‚lectionn‚.

:h2 res=IDM_FONTS_PGUP.Suite 
:p.Cette option de menu est disponible si la liste complŠte des polices
disponibles ne peut ˆtre affich‚e dans le menu, et si d&apos.autres polices
sont disponibles avant la premiŠre affich‚e. Pour faire d‚filer le menu
vers le haut, s‚lectionnez cette option de menu.

:h2 res=IDM_FONTS_PGDN.Suite 
:p.Cette option de menu est disponible si la liste complŠte des polices
disponibles ne peut ˆtre affich‚e dans le menu, et si d&apos.autres
polices sont disponibles aprŠs la derniŠre affich‚e. Pour faire d‚filer
le menu vers le bas, s‚lectionnez cette option de menu.

:h2 res=IDM_FONTMGR.Gestionnaire du menu polices
:p.Cette fonction n&apos.est pas encore disponible.

:h1 res=IDM_SIZE_MENU.Menu Taille
:i1.Menu Taille
:p.Ce menu propose une liste des tailles de polices disponibles.
:p.La taille de la police … la position du curseur est coch‚e.
:p.Voir :link reftype=hd res=IDM_SIZES.Entr‚es du menu Taille:elink.
pour plus d&apos.informations.

:h2 res=IDM_SIZES.Entr‚es du menu Taille
:p.Chaque ligne du menu repr‚sente une des tailles de police disponibles,
dans la police de caractŠres active. La s‚lection d&apos.une taille
entraŒne son application … la zone de texte s‚lectionn‚e, ou … partir de
la position du curseur si aucun texte n&apos.est s‚lectionn‚.

:h2 res=IDM_SIZES_PGUP.Suite 
:p.Cette option de menu est disponible si la liste complŠte des tailles
disponibles ne peut ˆtre affich‚e dans le menu, et si d&apos.autres
tailles sont disponibles avant la premiŠre affich‚e. Pour faire d‚filer
le menu vers le haut, s‚lectionnez cette option de menu.

:h2 res=IDM_SIZES_PGDN.Suite 
:p.Cette option de menu est disponible si la liste complŠte des tailles
disponibles ne peut ˆtre affich‚e dans le menu, et si d&apos.autres
tailles sont disponibles aprŠs la derniŠre affich‚e. Pour faire d‚filer
le menu vers le bas, s‚lectionnez cette option de menu.

:h2 res=IDM_SIZEMGR.Gestionnaire du menu Taille
:p.Cette fonction n&apos.est pas encore disponible.

:h1 res=IDM_ATTR_MENU.Menu Attributs
:i1.Menu Attributs
:p.Ce menu permet d&apos.ins‚rer des commandes d&apos.attributs de polices
de caractŠres dans le document. Tous les attributs de police g‚r‚s
nativement par le Gestionnaire de Pr‚sentation sont pr‚sentsÿ&colon.
:hp2.Normal:ehp2., :hp2.Gras:ehp2., :hp2.Italique:ehp2.,
:hp2.Soulign‚:ehp2., :hp2.Barr‚:ehp2. et :hp2.Contour:ehp2.. Pour
affecter un attribut … tout le texte … partir de la position du curseur,
ou … la zone de texte s‚lectionn‚e, s‚lectionnez l&apos.option de menu
correspondante … l&apos.aide de la souris ou du clavier. Les lignes du menu
:hp2.Attributs:ehp2. sont pr‚sent‚es dans la police et la taille en
vigueur … la position du curseur, et les attributs actifs sont coch‚s.
N&apos.importe quelle combinaison d&apos.attributs (… l&apos.exclusion de l&apos.attribut
:hp2.Normal:ehp2.) peut ˆtre cr‚‚e. L&apos.attribut :hp2.Normal:ehp2. est
actif quand aucun des autres ne l&apos.est.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_NORMAL.Normal:elink.
:li.&sqbul.:link reftype=hd res=IDM_BOLD.Gras:elink.
:li.&sqbul.:link reftype=hd res=IDM_ITALIC.Italique:elink.
:li.&sqbul.:link reftype=hd res=IDM_UNDERSCORE.Soulign‚:elink.
:li.&sqbul.:link reftype=hd res=IDM_STRIKEOUT.Barr‚:elink.
:li.&sqbul.:link reftype=hd res=IDM_OUTLINE.Contour:elink.
:li.&sqbul.:link reftype=hd res=IDM_CAPS.Majuscules:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXPAND_MENU.Menu Largeur:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINECOLOR_MENU.Menu Couleur du trait:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEWIDTH_MENU.Menu paisseur du trait:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILLCOLOR_MENU.Menu Couleur de remplissage:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILLTYPE_MENU.Menu Motif de remplissage:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEEND_MENU.Line End Menu:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEJOIN_MENU.Line Join Menu:elink.
:esl.

:h2 res=IDM_NORMAL.Normal (Option de menu)
:p.La s‚lection de cette option de menu annule tous les attributs de police
actifs (tels gras, italique, etc...) Si cette option est s‚lectionn‚e alors
qu&apos.une portion de texte est s‚lectionn‚e, elle n&apos.a
d&apos.effet que sur le texte s‚lectionn‚. Si aucun texte n&apos.est
s‚lectionn‚, l&apos.effet est appliqu‚ … partir de la position du
curseur. Les commandes d&apos.attributs pr‚sentes plus loin dans le
document ne sont pas affect‚es.
:p.Le raccourci clavier pour cette fonction est :hp2.Ctrl+N:ehp2..

:h2 res=IDM_BOLD.Gras (Option de menu)
:p.La s‚lection de cette option de menu applique l&apos.attribut de police
:hp2.Gras:ehp2. au texte s‚lectionn‚, ou bien … partir de la position du
curseur. Les commandes d&apos.attributs pr‚sentes plus loin dans le
document ne sont pas affect‚es.
:p.Le raccourci clavier pour cette fonction est :hp2.Ctrl+G:ehp2..

:h2 res=IDM_ITALIC.Italique (Option de menu)
:p.La s‚lection de cette option de menu applique l&apos.attribut de police
:hp2.Italique:ehp2. au texte s‚lectionn‚, ou bien … partir de la
position du curseur. Les commandes d&apos.attributs pr‚sentes plus loin
dans le document ne sont pas affect‚es.
:p.Le raccourci clavier pour cette fonction est :hp2.Ctrl+T:ehp2..

:h2 res=IDM_UNDERSCORE.Soulign‚ (Option de menu)
:p.La s‚lection de cette option de menu applique l&apos.attribut de police
:hp2.Soulign‚:ehp2. au texte s‚lectionn‚, ou bien … partir de la
position du curseur. Les commandes d&apos.attributs pr‚sentes plus loin dans
le document ne sont pas affect‚es.
:p.Le raccourci clavier pour cette fonction est :hp2.Ctrl+S:ehp2..

:h2 res=IDM_STRIKEOUT.Barr‚ (Option de menu)
:p.La s‚lection de cette option de menu applique l&apos.attribut de police
:hp2.Barr‚:ehp2. au texte s‚lectionn‚, ou bien … partir de la position
du curseur. Les commandes d&apos.attributs pr‚sentes plus loin dans le
document ne sont pas affect‚es.

:h2 res=IDM_OUTLINE.Contour (Option de menu)
:p.La s‚lection de cette option de menu applique l&apos.attribut de police
:hp2.Contour:ehp2. au texte s‚lectionn‚, ou bien … partir de la position
du curseur. Les commandes d&apos.attributs pr‚sentes plus loin dans le
document ne sont pas affect‚es.

:h2 res=IDM_CAPS.Majuscules (Option de menu)
:p.La s‚lection de cette option de menu applique l&apos.attribut de police
:hp2.Majuscules:ehp2. au texte s‚lectionn‚, ou bien … partir de la
position du curseur. Les commandes d&apos.attributs pr‚sentes plus loin
dans le document ne sont pas affect‚es.

:h2 res=IDM_EXPAND_MENU.Menu Largeur
:p.Ce menu propose un choix de largeurs pour la police de caractŠres.
:p.La largeur active … la position du curseur est coch‚e.
:p.Voir :link reftype=hd res=IDM_EXPANDS.Entr‚es du menu Largeur:elink.
pour plus d&apos.informations.

:h3 res=IDM_EXPANDS.Entr‚es du menu Largeur
:p.Chaque ligne de ce menu repr‚sente une des largeurs disponibles, dans la
police actuelle. La s‚lection d&apos.une de ces largeurs entraŒne son
application … la zone de texte s‚lectionn‚e, ou … partir du curseur si
aucun texte n&apos.est s‚lectionn‚.

:h3 res=IDM_EXPANDS_PGUP.Suite 
:p.Cette option de menu est disponible si la liste complŠte des largeurs
disponibles ne peut ˆtre affich‚e dans le menu, et si largeurs sont
disponibles avant la premiŠre affich‚e. Pour faire d‚filer le menu vers
le haut, s‚lectionnez cette option de menu.

:h3 res=IDM_EXPANDS_PGDN.Suite 
:p.Cette option de menu est disponible si la liste complŠte des largeurs
disponibles ne peut ˆtre affich‚e dans le menu, et si d&apos.autres
largeurs sont disponibles aprŠs la derniŠre affich‚e. Pour faire d‚filer
le menu vers le bas, s‚lectionnez cette option de menu.

:h3 res=IDM_EXPANDMGR.Gestionnaire de Largeurs
:p.This menu option summons the :link reftype=hd res=IDD_EXPANDMGR.Expand Menu Manager:elink. dialog,
from which it is possible to customize the contents of the Expand Menu.

:h4 res=IDD_EXPANDMGR.Expand Menu Manager Dialog
:p.This dialog displays the current configuration of the Expand Menu, and
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

:h2 res=IDM_LINECOLOR_MENU.Menu Couleur du trait
:p.Ce menu propose les couleurs de trac‚ disponibles.
:p.La couleur active … la position du curseur est coch‚e.
:p.Voir :link reftype=hd res=IDM_LINECOLORS.Entr‚es du menu Couleur du trait:elink.
pour plus d&apos.informations.

:h3 res=IDM_LINECOLORS.Entr‚es du menu Couleur du trait
:p.Chaque ligne de ce menu repr‚sente une des couleurs de trait
disponibles. La s‚lection d&apos.une couleur entraŒne son application … la
zone de texte s‚lectionn‚e, ou … partir de la position du curseur si
aucun texte n&apos.est s‚lectionn‚.
:note.Ces couleurs s&apos.appliquent aux lignes, au texte plein et … la
partie contour des textes ayant l&apos.attribut contour.

:h3 res=IDM_LINECOLORS_PGUP.Suite 
:p.Cette option de menu est disponible si la liste complŠte des couleurs de
trait disponibles ne peut ˆtre affich‚e dans le menu, et si d&apos.autres
couleurs sont disponibles avant la premiŠre affich‚e. Pour faire d‚filer
le menu vers le haut, s‚lectionnez cette option de menu.

:h3 res=IDM_LINECOLORS_PGDN.Suite 
:p.Cette option de menu est disponible si la liste complŠte des couleurs de
trait disponibles ne peut ˆtre affich‚e dans le menu, et si d&apos.autres
couleurs sont disponibles aprŠs la derniŠre affich‚e. Pour faire d‚filer
le menu vers le bas, s‚lectionnez cette option de menu.

:h3 res=IDM_LINECOLORMGR.Gestionnaire du menu Couleur du trait
This menu option summons the 
:link reftype=hd res=IDD_LINECOLORMGR.Gestionnaire du menu Couleur du trait:elink.
dialog, which gives the user the ability to customize the Line/Text
Color Menu.

:h4 res=IDD_LINECOLORMGR.Gestionnaire du menu Couleur du trait (Dialog)
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

:h2 res=IDM_LINEWIDTH_MENU.Menu paisseur du trait
:p.Ce menu propose les ‚paisseurs de trait disponibles.
:p.L&apos.‚paisseur du trait active … la position du curseur est coch‚e.
:p.Voir :link reftype=hd res=IDM_LINEWIDTHS.Entr‚es du menu paisseur du trait:elink.
pour plus d&apos.informations.

:h3 res=IDM_LINEWIDTHS.Entr‚es du menu paisseur du trait
:p.Chaque ligne de ce menu repr‚sente une des ‚paisseurs de trait
disponibles. La s‚lection d&apos.une ‚paisseur entraŒne son application
… la zone de texte s‚lectionn‚e, ou … partir de la position du curseur
si aucun texte n&apos.est s‚lectionn‚.

:note.L&apos.‚paisseur du trait s&apos.applique aux lignes, et … la
partie contour des textes poss‚dant l&apos.attribut contour.

:h3 res=IDM_LINEWIDTHS_PGUP.Suite 
:p.Cette option de menu est disponible si la liste complŠte des ‚paisseurs
de trait disponibles ne peut ˆtre affich‚e dans le menu, et si
d&apos.autres ‚paisseurs sont disponibles avant la premiŠre affich‚e.
Pour faire d‚filer le menu vers le haut, s‚lectionnez cette option de menu.

:h3 res=IDM_LINEWIDTHS_PGDN.Suite 
:p.Cette option de menu est disponible si la liste complŠte des ‚paisseurs
de trait disponibles ne peut ˆtre affich‚e dans le menu, et si
d&apos.autres ‚paisseurs sont disponibles aprŠs la derniŠre affich‚e.
Pour faire d‚filer le menu vers le bas, s‚lectionnez cette option de menu.

:h3 res=IDM_LINEWIDTHMGR.Gestionnaire du menu paisseur du trait
:p.Cette fonction n&apos.est pas encore disponible.

:h2 res=IDM_FILLCOLOR_MENU.Menu Couleur de remplissage
:p.Ce menu propose toutes les couleurs de remplissage des textes poss‚dant
l&apos.attribut contour.
:p.La couleur de remplissage active … la position du curseur est coch‚e.
:p.Voir :link reftype=hd res=IDM_FILLCOLORS.Entr‚es du menu Couleur de remplissage:elink. 
pour plus d&apos.informations.

:h3 res=IDM_FILLCOLORS.Entr‚es du menu Couleur de remplissage
:p.Chaque ligne de ce menu repr‚sente une des couleurs de remplissage
disponibles. La s‚lection d&apos.une couleur entraŒne son application …
la zone de texte s‚lectionn‚e, ou … partir de la position du curseur si
aucun texte n&apos.est s‚lectionn‚.

:h3 res=IDM_FILLCOLORS_PGUP.Suite 
:p.Cette option de menu est disponible si la liste complŠte des couleurs de
remplissage disponibles ne peut ˆtre affich‚e dans le menu, et si
d&apos.autres couleurs sont disponibles avant la premiŠre affich‚e. Pour
faire d‚filer le menu vers le haut, s‚lectionnez cette option de menu.

:h3 res=IDM_FILLCOLORS_PGDN.Suite 
:p.Cette option de menu est disponible si la liste complŠte des couleurs de
remplissage disponibles ne peut ˆtre affich‚e dans le menu, et si
d&apos.autres couleurs sont disponibles aprŠs la derniŠre affich‚e. Pour
faire d‚filer le menu vers le bas, s‚lectionnez cette option de menu.

:h3 res=IDM_FILLCOLORMGR.Gestionnaire du menu Couleur de remplissage
This menu option summons the 
:link reftype=hd res=IDD_FILLCOLORMGR.Gestionnaire du menu Couleur de remplissage:elink.
dialog, which gives the user the ability to customize the Fill
Color Menu.

:h4 res=IDD_FILLCOLORMGR.Gestionnaire du menu Couleur de remplissage (Dialog)
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

:h2 res=IDM_FILLTYPE_MENU.Menu Motif de remplissage
:p.Ce menu pr‚sente la liste des motifs de remplissage des textes
poss‚dant l&apos.attribut contour.
:p.Le motif de remplissage actif … la position du curseur est coch‚.
:p.Voir :link reftype=hd res=IDM_FILLTYPES.Entr‚es du menu Motif de remplissage:elink. 
pour plus d&apos.informations.

:h3 res=IDM_FILLTYPES.Entr‚es du menu Motif de remplissage
:p.Chaque ligne de ce menu repr‚sente un des motifs de remplissage
disponibles. La s‚lection d&apos.un motif entraŒne son application … la
zone de texte s‚lectionn‚e, ou … partir de la position du curseur si
aucun texte n&apos.est s‚lectionn‚.

:h3 res=IDM_FILLTYPES_PGUP.Suite 
:p.Cette option de menu est disponible si la liste complŠte des motifs de
remplissage disponibles ne peut ˆtre affich‚e dans le menu, et si
d&apos.autres motifs sont disponibles avant le premier affich‚. Pour
faire d‚filer le menu vers le haut, s‚lectionnez cette option de menu.

:h3 res=IDM_FILLTYPES_PGDN.Suite 
:p.Cette option de menu est disponible si la liste complŠte des motifs de
remplissage disponibles ne peut ˆtre affich‚e dans le menu, et si
d&apos.autres motifs sont disponibles aprŠs le dernier affich‚. Pour
faire d‚filer le menu vers le bas, s‚lectionnez cette option de menu.

:h3 res=IDM_FILLTYPEMGR.Gestionnaire du menu Motif de remplissage
:p.Cette fonction n&apos.est pas encore disponible.

:h2 res=IDM_LINEEND_MENU.Menu Fin de ligne
:p.Ce menu affiche la liste des types de fin de lignes disponibles.
:p.Le type actif … la position du curseur est coch‚ dans la liste.
:p.Voir :link reftype=hd res=IDM_LINEENDS.Entr‚es du menu Fin de ligne:elink.
pour plus d&apos.information.

:h3 res=IDM_LINEENDS.Entr‚es du menu Fin de ligne
:p.Cette ligne du menu repr‚sente l&apos.un des types de fin de lignes
disponibles pour le texte poss‚dant l&apos.attribut :hp1.Contour:ehp1.. Pour
d‚finir un type de fin de ligne … appliquer … partir de la position du
curseur, ou pour appliquer un type … une zone de texte s‚lectionn‚e,
s‚lectionnez le type d‚sir‚ … l&apos.aide de la souris, ou bien utilisez les
flˆches du clavier puis pressez la touche :hp2.Entr‚e:ehp2..

:h3 res=IDM_LINEENDS_PGUP.Suite 
:p.Cette option de menu est disponible si tous les choix du menu :hp2.Fin
de ligne:ehp2. ne peuvent ˆtre affich‚s simultan‚ment, et que d&apos.autres
choix sont disponibles avant le premier affich‚. S‚lectez cette option
pour faire d‚filer le menu vers le haut.

:h3 res=IDM_LINEENDS_PGDN.Suite 
:p.Cette option de menu est disponible si tous les choix du menu :hp2.Fin
de ligne:ehp2. ne peuvent ˆtre affich‚s simultan‚ment, et que d&apos.autres
choix sont disponibles aprŠs le dernier affich‚. S‚lectez cette option
pour faire d‚filer le menu vers le bas.

:h3 res=IDM_LINEENDMGR.Gestionnaire du menu Fin de ligne
:p.Cette fonction n&apos.est pas encore disponible.

:h2 res=IDM_LINEJOIN_MENU.Menu Jointure
:p.Ce menu propose la liste des types de jointures disponibles.
:p.Le type de jointure actif … la position du curseur est coch‚ dans la
liste.
:p.Voir :link reftype=hd res=IDM_LINEJOINS.Entr‚es du menu Jointure:elink.
pour plus d&apos.information.

:h3 res=IDM_LINEJOINS.Entr‚es du menu Jointure
:p.Cette ligne du menu repr‚sente l&apos.un des types de jointures disponibles
pour le texte poss‚dant l&apos.attribut :hp1.Contour:ehp1.. Pour d‚finir un
type de jointure … appliquer … partir de la position du curseur, ou pour
appliquer un type … une zone de texte s‚lectionn‚e, s‚lectionnez le type
d‚sir‚ … l&apos.aide de la souris, ou bien utilisez les flˆches du clavier
puis pressez la touche :hp2.Entr‚e:ehp2..

:h3 res=IDM_LINEJOINS_PGUP.Suite 
:p.Cette option de menu est disponible si tous les choix du menu
:hp2.Jointure:ehp2. ne peuvent ˆtre affich‚s simultan‚ment, et que
d&apos.autres choix sont disponibles avant le premier affich‚. S‚lectez cette
option pour faire d‚filer le menu vers le haut.

:h3 res=IDM_LINEJOINS_PGDN.Suite 
:p.Cette option de menu est disponible si tous les choix du menu
:hp2.Jointure:ehp2. ne peuvent ˆtre affich‚s simultan‚ment, et que
d&apos.autres choix sont disponibles aprŠs le dernier affich‚. S‚lectez cette
option pour faire d‚filer le menu vers le bas.

:h3 res=IDM_LINEJOINMGR.Gestionnaire du menu Jointure
:p.Cette fonction n&apos.est pas encore disponible.

:h1 res=IDM_WINDOW_MENU.Menu Fenˆtres
:i1.Menu Fenˆtres
:p.Ce menu donne accŠs aux fonctions de manipulations de fenˆtres, ainsi
qu&apos.… la liste des fenˆtres de documents ouvertes. Les commandes
suivantes sont disponiblesÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_TILE.Mosa‹que:elink.
:li.&sqbul.:link reftype=hd res=IDM_CASCADE.Cascade:elink.
:li.&sqbul.:link reftype=hd res=IDM_ARRANGEICONS.Arranger les Ic“nes:elink.
:li.&sqbul.:link reftype=hd res=IDM_WINDOWS.Fenˆtres actives:elink.
:esl.

:h2 res=IDM_TILE.Mosa‹que (Option de menu)
:p.Cette commande arrange les fenˆtres de documents non-r‚duites de fa‡on
… ce que toutes soient entiŠrement visiblesÿ; les ic“nes repr‚sentant
les fenˆtres de documents r‚duites sont plac‚es en bas de l&apos.espace
de travail, et restent visibles.

:h2 res=IDM_CASCADE.Cascade (Option de menu)
:p.Cette commande arrange les fenˆtres de documents non-r‚duites de fa‡on …
ce qu&apos.elles soient aussi grandes que possible, mais en permettant
de voir les barres de titre de toutes les fenˆtres et les ic“nes des
fenˆtres r‚duites. Les ic“nes repr‚sentant les fenˆtres r‚duites sont
d‚plac‚es vers la partie inf‚rieure de l&apos.espace de travail si
n‚cessaire.

:h2 res=IDM_ARRANGEICONS.Arranger les Ic“nes (Option de menu)
:p.Cette commande arrange les ic“nes des fenˆtres de documents r‚duites
dans la partie inf‚rieure gauche de l&apos.espace de travail. Cette
op‚ration est r‚alis‚e automatiquement quand les commandes
:hp2.Mosa‹que:ehp2. et :hp2.Cascade:ehp2. sont ex‚cut‚es.

:h2 res=IDM_WINDOWS.Fenˆtres actives
:p.Cette partie du menu Fenˆtres affiche une liste des fenˆtres de
documents ouvertes. La s‚lection d&apos.un des document propos‚s
entraŒne le passage de la fenˆtre correspondante au premier plan.

:h1 res=IDM_HELP_MENU.Menu Aide
:i1.Menu Aide
:p.Le menu d&apos.aide est dispos‚ de fa‡on conforme au standard SAA, et fournit
les commandes suivantesÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_HELP_INDEX.Index:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXTENDED_HELP.Aide g‚n‚rale:elink.
:li.&sqbul.:link reftype=hd res=IDM_HELP_FOR_HELP.Utiliser l&apos.aide:elink.
:li.&sqbul.:link reftype=hd res=IDM_KEYS_HELP.Aide sur les touches:elink.
:li.&sqbul.:link reftype=hd res=IDM_ABOUT_MENU.Menu infos produit:elink.
:esl.

:h2 res=IDM_HELP_INDEX.Index (Option de menu)
:p.Cette option de menu donne accŠs … l&apos.index du systŠme d&apos.aide.

:h2 res=IDM_EXTENDED_HELP.Aide g‚n‚rale (Option de menu)
:p.Cette option donne accŠs … des informations d&apos.ordre g‚n‚ral sur
l&apos.application.

:h2 res=IDM_HELP_FOR_HELP.Utiliser l&apos.Aide (Option de menu)
:p.Cette option donne accŠs aux instructions de base sur la fa‡on
d&apos.utiliser le programme d&apos.aide en ligne. Ces informations ne
sont pas sp‚cifiques … cette application.

:h2 res=IDM_KEYS_HELP.Aide sur les touches (Option de menu)
:p.Les touches suivantes sont disponibles dans Escribaÿ&colon.
J&apos.ai fait de mon mieux pour rester conforme au standard SAA CUA, tel que
je le comprends. Le manque d&apos.information (dont je dispose) sur le sujet
est impressionnant.
:table cols='20 60' rules=both frame=box.
:row.:c.F1              :c.Aide en ligne
:row.:c.F3              :c.Sortie
:row.:c.F4              :c.Sortie aprŠs fermeture du document actif
:row.:c.F5              :c.Contr“le orthographique
:row.:c.F10             :c.Menu
:row.:c.Ctrl + Tab      :c.Document suivant
:row.:c.Ctrl + C        :c.Chercher
:row.:c.Ctrl + E        :c.Savegarde du document actif
:row.:c.Ctrl + G        :c.Gras
:row.:c.Ctrl + I        :c.Impression du document
:row.:c.Alt + I         :c.Impression de la page en cours
:row.:c.Ctrl + N        :c.Texte Normal (Annule les attributs de texte)
:row.:c.Ctrl + O        :c.Ouverture d&apos.un document
:row.:c.Ctrl + S        :c.Soulign‚
:row.:c.Ctrl + T        :c.Italique
:row.:c.Ctrl + Z        :c.Zoom Tout actif/inactif
:row.:c.Ctrl + Ret. Arr.:c.Efface jusqu&apos.au d‚but du mot
:etable.
De plus, certaines touches permettent de naviguer dans le document. Voir
:link reftype=hd res=60003.dition de documents (Navigation):elink. pour une
description de ces touches.

:h2 res=IDM_ABOUT_MENU.Menu infos produit (sous-menu)
:p.Cette option de menu donne accŠs … la boŒte de dialogue d&apos.informations
sur le programme et … la boŒte de dialogue de Copyright.
:p.Voirÿ:link reftype=hd res=IDD_ABOUT.Infos produit (Dialogue):elink. pour
plus d&apos.informations.
:p.Si vous s‚lectionnez la plaquette de menu conditionnel, une liste des
produits install‚s est propos‚e, vous donnant accŠs aux informations sur
chacun de ces produits.

:h3 res=IDM_ABOUT.Ligne du menu Infos produit (Option de menu)
:p.Cette ligne du menu Infos produit repr‚sente Escriba ou une des
extensions install‚es. S‚lectez une ligne du menu pour acc‚der aux
informations sur ce produit.
(Voir :link reftype=hd res=IDD_ABOUT.Infos produit (Dialogue):elink. pour plus
d&apos.informations).

:h4 res=IDD_ABOUT.Infos produit (Dialogue)
:p.Cette boŒte de dialogue affiche le nom du programme, son ic“ne et la
notice de Copyright. Pour quitter la boŒte de dialogue, pressez la
touche :hp2.Entr‚e:ehp2., la barre d&apos.espace ou la touche
:hp2.chap:ehp2., ou s‚lectionnez la plaquette :hp2.Ok:ehp2. … l&apos.aide
de la souris.
:artwork name='Francais\About.bmp' align=center.

:h1 res=60009.Menus contextuels
:i1.Menus contextuels
:p.Il existe plusieurs menus contextuels qui peuvent ˆtre appel‚s d&apos.un
simple clic du bouton droit de la souris. Dans leur ensemble, ces menus
contextuels proposent plus de fonctions que les menus d‚roulants de la
fenˆtre principale.
:p.
Quand vous cliquez le bouton droit de la souris sur l&apos.espace de
travail du programme (pas sur une fenˆtre de document), le 
:link reftype=hd res=IDM_MAIN_CONTEXT.Menu contextuel g‚n‚ral:elink. est affich‚. Quand
vous cliquez le bouton droit sur une fenˆtre de document, mais pas sur
une zone de texte s‚lectionn‚ ni sur un graphique, le 
:link reftype=hd res=IDM_DEFAULTMENU.Menu standard:elink. est affich‚. Un clic du bouton
droit sur une zone de texte s‚lectionn‚ appele le 
:link reftype=hd res=IDM_TEXTMENU.Menu Texte s‚lectionn‚:elink.. Il y a aussi un 
:link reftype=hd res=IDM_BITMAPMENU.Menu Graphique:elink. et un 
:link reftype=hd res=IDM_BORDERMENU.Menu Bordure de graphique:elink..

:h2 res=IDM_MAIN_CONTEXT.Menu contextuel g‚n‚ral
:p.Ce menu apparaŒt quand vous cliquez le bouton droit de la souris sur
l&apos.espace de travail du programme (hors d&apos.une fenˆtre de
document). Il propose les actions disponibles … l&apos.ext‚rieur
d&apos.un document, … savoirÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_REFRESH.R‚g‚n‚ration:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILE_MENU.Fichier:elink.
:li.&sqbul.:link reftype=hd res=IDM_WINDOW_MENU.Fenˆtres:elink.
:li.&sqbul.:link reftype=hd res=IDM_HELP_MENU.Aide:elink.
:esl.
:note.Le sous-menu Fichier de ce menu ne contient pas toutes les options
du menu Fichier normal, mais uniquement celles qui ne sont pas relatives
… un document. Ces exceptions sont not‚es dans les descriptions
d‚taill‚es de chaque option de menu. De plus, si aucune fenˆtre de
document n&apos.est ouverte, le sous-menu Fenˆtres sera d‚sactiv‚.

:h3 res=IDM_REFRESH.R‚-afficher (Option de menu)
:p.Cette option de menu provoque le r‚-affichage de la fenˆtre du
document.

:h2 res=IDM_DEFAULTMENU.Menu standard
:p.Ce menu apparaŒt quand vous cliquez le bouton droit de la souris sur
une zone du document qui n&apos.est ni (1) un texte s‚lectionn‚, (2) un
graphique, ou (3) le bord d&apos.un graphique. Il pr‚sente les commandes
qui peuvent ˆtre appliqu‚es au document dans son int‚gralit‚. Ces
commandes sont les suivantesÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_REFRESH.Refresh:elink.
:esl.

:h2 res=IDM_TEXTMENU.Menu texte
:p.Ce menu apparaŒt quand vous pressez le bouton droit de la souris sur
une zone de texte s‚lectionn‚e. Il pr‚sente les commandes qui peuvent
ˆtre appliqu‚es … une portion de texte s‚lectionn‚e, … savoirÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_CUT.Couper:elink.
:li.&sqbul.:link reftype=hd res=IDM_COPY.Copier:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE.Coller:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEAR.Effacer:elink.
:li.&sqbul.:link reftype=hd res=IDM_LAYOUT_MENU.Disposition:elink.
:li.&sqbul.:link reftype=hd res=IDM_FONT_MENU.Police:elink.
:li.&sqbul.:link reftype=hd res=IDM_SIZE_MENU.Taille:elink.
:li.&sqbul.:link reftype=hd res=IDM_ATTR_MENU.Attributs:elink.
:esl.
:note.Le sous-menu :hp2.Disposition:ehp2. de ce menu ne contient pas
toutes les options du menu :hp2.Disposition:ehp2. normal, mais
uniquement celles qui peuvent ˆtre appliqu‚es … un bloc de texte
s‚lectionn‚. Ces diff‚rences sont not‚es dans la description d‚taill‚e
de chaque option de menu.

:h2 res=IDM_BITMAPMENU.Menu graphique
:p.Ce menu apparaŒt quand vous pressez le bouton droit de la souris sur un
objet graphique. Il pr‚sente les commandes qui peuvent ˆtre appliqu‚es …
un tel objet. Les commandes sont les suivantesÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_BACKGROUND.ArriŠre-plan:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_NOTEXTASIDE.Pas d&apos.habillage:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_HALFTONE.Demi-teinte:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_REVERSE.Reverse:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_OPTIMIZE.Optimiser:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION.Position:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE.Taille:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT.Aspect:elink.
:esl.

:h3 res=IDM_BITMAP_BACKGROUND.ArriŠre-plan (Option de menu)
:p.Quand cette option de menu est s‚lectionn‚e, le graphique est marqu‚
comme ‚tant … l&apos.arriŠre-plan de la page. Le texte peut figurer
par dessus le graphique.

:h3 res=IDM_BITMAP_NOTEXTASIDE.Pas d&apos.habillage (Option de menu)
:p.Quand vous s‚lectionnez cette option de menu, le graphique est marqu‚ de
fa‡on qu&apos.aucun texte ne l&apos.habille … gauche ou … droite.

:h3 res=IDM_BITMAP_HALFTONE.Demi-teinte (Option de menu)
:p.Quand vous s‚lectionnez cette option de menu, le graphique est affich‚ en
demi-teintes, ce qui est particuliŠrement pratique pour les graphiques
utilis‚s en arriŠre-plan.

:h3 res=IDM_BITMAP_REVERSE.Inverse (Option de menu)
Quand cette option est coch‚e, l&apos.image est affich‚e en couleurs
invers‚es.
:note.Cette option ne fonctionne qu&apos.avec les graphiques en mode point
(pas avec les m‚tafichiers).

:h3 res=IDM_BITMAP_OPTIMIZE.Optimiser (Option de menu)
:p.Quand cette option de menu est s‚lectionn‚e, l&apos.image est optimis‚e pour
l&apos.imprimante utilis‚eÿ; c&apos.est … dire que la mise … l&apos.‚chelle sera faite
de telle sorte que la taille de l&apos.image sur l&apos.imprimante sera un
multiple entier de la taille du bitmap d&apos.origine. Cela permet
d&apos.obtenir la meilleure apparence possible sur l&apos.imprimante.
:note.Cette option ne fonctionne qu&apos.avec les graphiques bitmap
(pas les m‚tafichiers).

:h3 res=IDM_BITMAP_POSITION.Menu Position du graphique
:p.Ce menu donne accŠs aux options qui influent sur la position d&apos.un
graphique dans une page du document. Les options suivantes sont
disponiblesÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_NEXT_CHAR.Next Character:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_NEXT_LINE.Next Line:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_LEFT.Bord gauche:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_LEFT.Marge gauche:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_CENTERED_X.Centr‚ horizontalement:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_RIGHT.Marge droite:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_RIGHT.Bord droit:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_TOP.Bord sup‚rieur:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_TOP.Marge du haut:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_CENTERED_Y.Centr‚ verticalement:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_BOTTOM.Marge du bas:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_BOTTOM.Bord inf‚rieur:elink.
:esl.
Les options actives pour le graphique s‚lectionn‚ sont coch‚es.

:h4 res=IDM_BITMAP_POSITION_NEXT_CHAR.Positionner le graphique caractŠre suivant (Option de menu)
Quand cette option est s‚lect‚e, le graphique est consid‚r‚ comme un
gros caractŠre ins‚r‚ dans le texte du document. Le graphique sera
affich‚ … l&apos.endroit o— il a ‚t‚ ins‚r‚ dans le document, en adaptant le
texte autour de lui.

:h4 res=IDM_BITMAP_POSITION_NEXT_LINE.Positionner le graphique ligne suivante (Option de menu)
Fonction non impl‚ment‚e.

:h4 res=IDM_BITMAP_POSITION_EDGE_LEFT.Bord gauche (Option de menu)
:p.Quand cette option est coch‚e, le graphique est positionn‚ par rapport
au bord gauche de la page, en fonction de la taille et des limites du
support. Si un nouveau support est choisi, la position du graphique
sera adapt‚e au nouveau support.

:h4 res=IDM_BITMAP_POSITION_MARGIN_LEFT.Marge gauche (Option de menu)
:p.Quand cette option est coch‚e, le graphique est positionn‚ par rapport …
la marge gauche de la page. En cas de modification de la marge, la
position du graphique sera adapt‚e.

:h4 res=IDM_BITMAP_POSITION_CENTERED_X.Centr‚ horizontalement (Option de menu)
:p.Quand cette option est coch‚e, le graphique est centr‚ horizontalement,
normalement par rapport aux marges du document. Si l&apos.option de taille du
graphique s‚lectionn‚e est :link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Bord-…-bord horizontalement:elink., 
le centrage sera effectu‚ par rapport aux limites d&apos.impression du support.

:h4 res=IDM_BITMAP_POSITION_MARGIN_RIGHT.Marge droite (Option de menu)
:p.Quand cette option est coch‚e, le graphique est positionn‚ par rapport …
la marge droite de la page. En cas de modification de la marge, la
position du graphique sera adapt‚e.

:h4 res=IDM_BITMAP_POSITION_EDGE_RIGHT.Bord droit (Option de menu)
:p.Quand cette option est coch‚e, le graphique est positionn‚ par rapport
au bord droit de la page, en fonction de la taille et des limites du
support. Si un nouveau support est choisi, la position du graphique
sera adapt‚e au nouveau support.

:h4 res=IDM_BITMAP_POSITION_EDGE_TOP.Bord sup‚rieur (Option de menu)
:p.Quand cette option est coch‚e, le graphique est positionn‚ par rapport
au bord sup‚rieur de la page, en fonction de la taille et des limites du
support. Si un nouveau support est choisi, la position du graphique
sera adapt‚e au nouveau support.

:h4 res=IDM_BITMAP_POSITION_MARGIN_TOP.Marge du haut (Option de menu)
:p.Quand cette option est coch‚e, le graphique est positionn‚ par rapport …
la marge du haut de la page. En cas de modification de la marge, la
position du graphique sera adapt‚e.

:h4 res=IDM_BITMAP_POSITION_CENTERED_Y.Centr‚ verticalement (Option de menu)
:p.Quand cette option est coch‚e, le graphique est centr‚ verticalement,
normalement par rapport aux marges du document. Si l&apos.option de taille du
graphique s‚lectionn‚e est :link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Bord-…-bord verticalement:elink., 
le centrage sera effectu‚ par rapport aux limites d&apos.impression du support.

:h4 res=IDM_BITMAP_POSITION_MARGIN_BOTTOM.Marge du bas (Option de menu)
:p.Quand cette option est coch‚e, le graphique est positionn‚ par rapport …
la marge du base de la page. En cas de modification de la marge, la
position du graphique sera adapt‚e.

:h4 res=IDM_BITMAP_POSITION_EDGE_BOTTOM.Bord inf‚rieur (Option de menu)
:p.Quand cette option est coch‚e, le graphique est positionn‚ par rapport
au bord inf‚rieur de la page, en fonction de la taille et des limites du
support. Si un nouveau support est choisi, la position du graphique
sera adapt‚e au nouveau support.

:h3 res=IDM_BITMAP_SIZE.Menu Taille du graphique
:p.Ce sous-menu permet de d‚finir la taille du graphique sur la page du
document. Les options disponibles sontÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Bord-…-bord horizontalement:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_MARGINTOMARGIN_X.Marge-…-marge horizontalement:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Bord-…-bord verticalement:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_MARGINTOMARGIN_Y.Marge-…-marge verticalement:elink.
:esl.
La ou les options actives pour le graphique s‚lectionn‚ sont coch‚es.

:h4 res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Bord-…-bord horizontalement (Option de menu)
:p.Le graphique est dimentionn‚ de fa‡on … occuper toute la largeur de la
page, d&apos.un bord … l&apos.autre. Si l&apos.option de conservation des proportions
est active (voir le :link reftype=hd res=IDM_BITMAP_ASPECT.Menu Apparence du graphique:elink.), 
il est possible que le graphique soit moins large.

:h4 res=IDM_BITMAP_SIZE_MARGINTOMARGIN_X.Marge-…-marge horizontalement (Option de menu)
:p.Le graphique est dimentionn‚ de fa‡on … occuper toute la largeur de la
page, d&apos.une marge … l&apos.autre. Si l&apos.option de conservation des proportions
est active (voir le :link reftype=hd res=IDM_BITMAP_ASPECT.Menu Apparence du graphique:elink.), 
il est possible que le graphique soit moins large.

:h4 res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Bord-…-bord verticalement (Option de menu)
:p.Le graphique est dimentionn‚ de fa‡on … occuper toute la hauteur de la
page, d&apos.un bord … l&apos.autre. Si l&apos.option de conservation des proportions
est active (voir le :link reftype=hd res=IDM_BITMAP_ASPECT.Menu Apparence du graphique:elink.), 
il est possible que le graphique soit moins haut.

:h4 res=IDM_BITMAP_SIZE_MARGINTOMARGIN_Y.Marge-…-marge verticalement (Option de menu)
:p.Le graphique est dimentionn‚ de fa‡on … occuper toute la hauteur de la
page, d&apos.une marge … l&apos.autre. Si l&apos.option de conservation des proportions
est active (voir le :link reftype=hd res=IDM_BITMAP_ASPECT.Menu Apparence du graphique:elink.), 
il est possible que le graphique soit moins haut.

:h3 res=IDM_BITMAP_ASPECT.Menu Apparence du graphique
:p.Ce menu permet de d‚finir si les proportions d&apos.un graphique doivent ˆtre
conserv‚es, et le cas ‚ch‚ant selon quelle rŠgle. Les options
disponibles sontÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FLOAT.Libre:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FIX_X.Largeur fixe:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FIX_Y.Hauteur fixe:elink.
:esl.
La ou les options actives pour le graphique s‚lectionn‚ sont coch‚es.

:h4 res=IDM_BITMAP_ASPECT_FLOAT.Libre (Option de menu)
:p.Quand cette option est coch‚e, les dimensions du graphiques ne sont pas
tenues de respecter les proportions d&apos.origine. L&apos.effet n&apos.est pas
imm‚diatement visible, sauf si une des options de taille
:hp1.Bord-…-bord:ehp1. ou :hp1.Marge-…-marge:ehp1. est active, ce qui
peut entraŒner un changement de la taille du graphique.

:h4 res=IDM_BITMAP_ASPECT_FIX_X.Largeur fixe (Option de menu)
:p.Quand cette option est coch‚e, le graphique conserve toujours ses
proportions d&apos.origine. Si sa largeur est modifi‚e par l&apos.utilisateur, le
programme adaptera la hauteur de fa‡on … conserver le rapport
hauteur/largeur d&apos.origine.

:h4 res=IDM_BITMAP_ASPECT_FIX_Y.Hauteur fixe (Option de menu)
:p.Quand cette option est coch‚e, le graphique conserve toujours ses
proportions d&apos.origine. Si sa heuteur est modifi‚e par l&apos.utilisateur, le
programme adaptera la largeur de fa‡on … conserver le rapport
hauteur/largeur d&apos.origine.

:h2 res=IDM_BORDERMENU.Menu bordure de graphique
:p.Ce menu est appel‚ en pressant le bouton droit de la souris sur la
bordure d&apos.un objet graphique. Il pr‚sente les commandes qui peuvent
ˆtre appliqu‚es … cet objet, qui sontÿ&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_SIZE.Taille bordure:elink.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_COLOR.Couleur bordure:elink.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_CAPTION.Titre bordure:elink.
:esl.

:h3 res=IDM_BORDER_SIZE.Taille bordure (Option de menu)
:p.Cette fonction n&apos.est pas encore disponible.

:h3 res=IDM_BORDER_COLOR.Couleur bordure (Option de menu)
:p.Cette fonction n&apos.est pas encore disponible.

:h3 res=IDM_BORDER_CAPTION.Titre bordure (Option de menu)
:p.Cette fonction n&apos.est pas encore disponible.

:h1 res=60010.BoŒtes de dialogues diverses

:h2 res=IDD_PROFILE_PATH.R‚pertoire configuration
:p.Cette boŒte de dialogue apparaŒt quand le programme ne parvient pas …
retrouver son fichier de configuration (.INI), et vous demande le nom du
r‚pertoire dans lequel se trouve ou doit ˆtre cr‚‚ le fichier.
:artwork name='Francais\Profile.bmp' align=center.
:p.Le nom de r‚pertoire saisi doit ˆtre valide. Une fois le nom de
r‚pertoire saisi, pressez la touche :hp2.Entr‚e:ehp2. ou s‚lectionnez la
plaquette :hp2.Ok:ehp2. pour valider.
:p.Si vous voulez annuler la s‚quence d&apos.initialisation du
programme, pressez la touche :hp2.chap:ehp2. ou s‚lectionnez la plaquette
:hp2.Annuler:ehp2..

:h2 res=IDD_PROMPT_SAVE.Document modifi‚. Le sauvegarder ?
.* :artwork name='Francais\SaveChng.bmp' align=center.
:p.Vous tentez de quitter le programme ou de fermer une fenˆtre de
document, alors que les modifications apport‚es au document actif n&apos.ont
pas ‚t‚ sauvegard‚es. Pressez la touche :hp2.O:ehp2. ou s‚lectionnez la
plaquette :hp2.Oui:ehp2. si vous d‚sirez sauvegarder les modifications
dans le fichier. Pressez :hp2.N:ehp2. ou s‚lectionnez la plaquette
:hp2.Non:ehp2. si vous ne d‚sirez pas sauvegarder le document (les
modifications seront perdues). Pressez la touche :hp2.chap:ehp2. ou
s‚lectionnez la plaquette :hp2.Annuler:ehp2. si vous d‚sirez annuler la
fermeture du programme ou de la fenˆtre de document.

:h2 res=IDD_PROMPT_SAVECONFIRM1.Confirmation
:p.Si cette boŒte de dialogue apparaŒt, vous tentez d&apos.enregistrer votre
document sous un nouveau nom, correspondant … un fichier d‚j… existant.
.* :artwork name='Francais\SaveConf.bmp' align=center.
:p.Si vous d‚sirez effectivement remplacer le fichier existant, pressez la
touche :hp2.O:ehp2. ou s‚lectionnez la plaquette :hp2.Oui:ehp2.. Si vous ne
souhaitez pas ‚craser le fichier existant, s‚lectionnez la plaquette
:hp2.Non:ehp2. ou pressez la touche :hp2.Entr‚e:ehp2.ÿ; dans ce cas, le
document ne sera pas sauvegard‚, et vous devrez effectuer la sauvegarde
en fournissant un nom diff‚rent.

:h2 res=IDD_PROMPT_SAVECONFIRM2.Confirmation du changement de type
:p.Cette boŒte de dialogue est affich‚e quand vous avez demand‚ de
modifier le type d&apos.un fichier lors d&apos.une sauvegarde.
.* :artwork name='Francais\SaveCnf2.bmp' align=center.
:p.Pour changer le type du fichier, pressez la touche :hp2.O:ehp2. ou
s‚lectionnez la plaquette :hp2.Oui:ehp2..
Si vous ne d‚sirez pas changer le type du fichier, pressez la touche
:hp2.Entr‚e:ehp2. ou :hp2.N:ehp2.. Dans ce cas, le document ne sera
pas sauvegard‚, et vous devrez reprendre l&apos.op‚ration.

:h2 res=IDD_MESSAGE_SPELLDONE.Contr“le orthographique termin‚
:p.Cette boŒte de dialogue est affich‚e … la fin du contr“le
orthographique du document.
.* :artwork name='Francais\SpelDone.bmp' align=center.
:p.Pour continuer, pressez la touche :hp2.Entr‚e:ehp2. ou s‚lectionnez la
plaquette :hp2.Ok:ehp2..

:h2 res=IDD_ERROR_HELPMANAGER.Erreur du gestionnaire d&apos.aide
:p.Si ce message apparaŒt, il y a un problŠme dans le fichier d&apos.aide de
l&apos.application. Veuillez en informer le d‚veloppeur de l&apos.application.
(Voire le traducteur, Rapha‰l Vanneyÿ; Fido 2&colon.320/222.7 | e-mail
rvanney@ibm.net)

:h2 res=IDD_ERROR_EXCEPTION.Arrˆt du programme/exception
.* :artwork name='Francais\Except.bmp' align=center.
:p.Cette boŒte de dialogue est affich‚e quand Escriba rencontre une erreur
fatale … son fonctionnement. Veuillez informer l&apos.auteur en utilisant
l&apos.un des moyens pr‚sent‚s dans la section 
:link reftype=hd res=60008.Questions et rapport d&apos.anomalies:elink., en incluant le fichier
:hp1.ESCRIBA.LOG:ehp1., qui doit ˆtre pr‚sent dans le r‚pertoire o—
Escriba est install‚.

:h2 res=IDD_ERROR_WINREGISTERCLASS.Erreur d&apos.enregistrement de classe
:p.Si ce message d&apos.erreur a ‚t‚ affich‚, l&apos.application a ‚t‚
incapable d&apos.enregistrer une de ses classes de fenˆtres. Veuillez
informer l&apos.auteur en utilisant l&apos.un des moyens pr‚sent‚s dans
la section :link reftype=hd res=60008.Questions et rapport d&apos.anomalies:elink..

:h2 res=IDD_ERROR_CREATECLIENT.Erreur cr‚ation client
:p.Cette boŒte de dialogue est affich‚e quand Escrina est incapable de
cr‚er une fenˆtre cliente. Veuillez informer l&apos.auteur en utilisant
l&apos.un des moyens pr‚sent‚s dans la section 
:link reftype=hd res=60008.Questions et rapport d&apos.anomalies:elink..

:h2 res=IDD_ERROR_TOOMANYDOCUMENTS.Trop de documents ouverts
:p.Cette boŒte de dialogue est affich‚e si vous essayez d&apos.ouvrir trop
de documents.

:h2 res=IDD_ERROR_TOOMANYLOADED.Trop de documents charg‚s
:p.Cette boŒte de dialogue est affich‚e si vous avez charg‚ trop de
documents.

:h2 res=IDD_ERROR_DOCUMENT_CREATE.Erreur cr‚ation de document
:p.Cette boŒte de dialogue est affich‚e si Escriba n&apos.est pas capable
de cr‚er un objet document. Veuillez informer l&apos.auteur en utilisant
l&apos.un des moyens pr‚sent‚s dans la section 
:link reftype=hd res=60008.Questions et rapport d&apos.anomalies:elink..

:h2 res=IDD_ERROR_DOCUMENT_LOAD.Erreur de chargement de document
:p.Cette boŒte de dialogue est affich‚e si Escriba ne parvient pas …
charger un document. Veuillez v‚rifier le nom du fichier et les
r‚pertoires sp‚cifi‚s. Il est aussi possible que le fichier soit en
cours d&apos.utilisation par un autre programme, rendant impossible son
ouverture par Escriba.

:h2 res=IDD_ERROR_DOCUMENT_SAVE.Erreur sauvegarde document
:p.Cette boŒte de dialogue est affich‚e quand Escriba n&apos.a pas r‚ussi …
ouvrir le fichier dans lequel vous lui avez demand‚ d&apos.enregistrer un
document. Veuillez v‚rifier la validit‚ du nom de fichier, en
particulier l&apos.existance du r‚pertoire sp‚cifi‚. Il est aussi
possible que le fichier soit ouvert par un autre programme, ce qui
empˆche sa r‚-‚criture par Escriba.

:h2 res=IDD_ERROR_CREATEPRINTSTATUS.Erreur cr‚ation fenˆtre d&apos.‚tat impression
:p.Cette boŒte de dialogue est affich‚e si Escriba est incapable
d&apos.initialiser la fenˆtre d&apos.‚tat d&apos.impression. Veuillez
informer l&apos.auteur en utilisant l&apos.un des moyens pr‚sent‚s dans
la section :link reftype=hd res=60008.Questions et rapport d&apos.anomalies:elink..

:h2 res=IDD_ERROR_CREATELOADSTATUS.Erreur cr‚ation fenˆtre d&apos.‚tat chargement
:p.Cette boŒte de dialogue est affich‚e si Escriba est incapable
d&apos.initialiser la fenˆtre d&apos.‚tat de chargement. Veuillez
informer l&apos.auteur en utilisant l&apos.un des moyens pr‚sent‚s dans
la section :link reftype=hd res=60008.Questions et rapport d&apos.anomalies:elink..

:h2 res=IDD_ERROR_BLOCKMEMORY.M‚moire insuffisante pour la copie
:p.Cette boŒte de dialogue signale un manque de m‚moire pour couper ou
copier le texte s‚lectionn‚ dans le presse-papiers. Essayez
d&apos.effectuer la copie par petites portions.

:h2 res=IDD_ERROR_BADGRAPHICFILE.Fichier graphique non reconnu
:p.Cette boŒte de dialogue est affich‚e quand vous essayez de charger un
fichier graphique dont le format n&apos.est pas reconnu, ou qui contient
des donn‚es erron‚es. Les formats graphiques actuellement reconnus sont
les suivants : BMP, GIF, JPG, PCX, PCC, MET et TIF. Si le fichier que
vous essayez de charger est d&apos.un de ces types, et si vous pouvez le
visualiser … l&apos.aide d&apos.un programme de visualisation
d&apos.images, veuillez informer l&apos.auteur en utilisant l&apos.un
des moyens pr‚sent‚s dans la section 
:link reftype=hd res=60008.Questions et rapport d&apos.anomalies:elink..

:h2 res=IDD_ERROR_VERIFY1.Erreur v‚rification - Ouvrir
:p.Cette erreur signifie que le fichier que vous avez essay‚
d&apos.ouvrir n&apos.a pas pu l&apos.ˆtre pour une raison ind‚termin‚e.
V‚rifiez le nom et le chemin, puis r‚essayez.

:h2 res=IDD_ERROR_VERIFY2.Erreur v‚rification - M‚moire
:p.Cette erreur se produit quand Escriba n&apos.a pas pu obtenir
suffisemment de m‚moire pour charger le fichier sp‚cifi‚.

:h2 res=IDD_ERROR_VERIFY3.Erreur v‚rification - Code incorrect
:p.Cette erreur se produit si le fichier que vous tentez de charger, au
format Escriba, contient des codes d&apos.‚chappement incorrects.
Le fichier peut ˆtre corrompu.

:h2 res=IDD_ERROR_VERIFY4.Erreur v‚rification - Fonction incorrecte
:p.Cette erreur se produit si le fichier que vous tentez de charger, au
format Escriba, contient des codes de fonction incorrects.
Le fichier peut ˆtre corrompu, ou bien avoir ‚t‚ cr‚‚ avec une version
plus r‚cente d&apos.Escriba.

:h2 res=IDD_ERROR_VERIFY5.Erreur v‚rification - Nulls
:p.Cette erreur se produit si le fichier que vous tentez de charger, au
format Escriba, contient des caractŠres nuls.
Le fichier peut ˆtre corrompu.

:h2 res=IDD_ERROR_VERIFY6.Erreur v‚rification - En-tˆtes/fin non balanc‚s
:p.Cette erreur se produit si le fichier que vous tentez de charger, au
format Escriba, ne contient pas les octets de fin correspondant … chaque
en-tˆte de fonction.
Le fichier peut ˆtre corrompu.

:h2 res=IDD_ERROR_VERIFY7.Erreur v‚rification - Pas de pr‚face
:p.Cette erreur se produit si le fichier que vous tentez de charger, au
format Escriba, ne commence pas par une fonction Preface.
Le fichier peut ˆtre corrompu.

:h2 res=IDD_ERROR_VERIFY8.Erreur v‚rification - Aucun type valide
:p.Cette erreur se produit si le type du fichier que vous essayez de
charger n&apos.est pas reconnu. Escriba est incapable de charger un tel
fichier.
:note.De nouveaux modules de traitement des fichiers sont d‚velopp‚s en
permanence. Si vous n&apos.arrivez pas … charger un fichier dont vous avez
besoin, prenez contact avec l&apos.auteur … l&apos.une des adresses donn‚es dans
la rubrique :link reftype=hd res=60008.Questions et rapports d&apos.anomalies:elink..

:h2 res=IDD_ERROR_IMPORT1.Erreur d&apos.importation - Ouverture
:p.Cette erreur se produit si Escriba ne parvient pas … ouvrir le fichier
que vous d‚sirez charger. V‚rifiez le nom et le chemin, puis essayez …
nouveau.

:h2 res=IDD_ERROR_IMPORT2.Erreur d&apos.importation - M‚moire
:p.Cette erreur indique qu&apos.Escriba n&apos.a pas pu obtenir suffisemment de
m‚moire pour charger le fichier sp‚cifi‚ en m‚moire.

:h2 res=IDD_ERROR_EXPORT1.Erreur d&apos.exportation - Ouverture/cr‚ation
:p.Cette erreur indique qu&apos.Escriba n&apos.est pas parvenu … ouvrir le fichier …
sauvegarder. V‚rifiez … nouveau le chemin et le nom de fichier. Si le
fichier existe d‚j…, il est possible qu&apos.il ne soit accessible qu&apos.en
lecture.

:h2 res=IDD_ERROR_EXPORT2.Erreur d&apos.exportation - Espace disque
:p.Cette erreur est affich‚e quand Escriba ne parvient pas … ‚crire
un fichier sur disque.
V‚rifiez qu&apos.il reste suffisemment d&apos.espace libre sur le disque.

:h2 res=IDD_METAFILE_VERIFY1.Erreur, v‚rification de m‚tafichier - Ouverture
Ce message d&apos.erreur signifie que le m‚tafichier choisi n&apos.a pas pu ˆtre
ouvert. En g‚n‚ral, cette erreur r‚sulte du fait qu&apos.une application est
entrain d&apos.‚crire sur ce fichier.

:h2 res=IDD_METAFILE_VERIFY2.Erreur, v‚rification de m‚tafichier - Fichier trop petit
Le m‚tafichier choisi est trop petit.

:h2 res=IDD_METAFILE_VERIFY3.Erreur, v‚rification de m‚tafichier - Chargement
Cette erreur signifie qu&apos.il est impossible de charger le fichier en
m‚moire pour v‚rification. Si cette erreur se reproduit, veuillez
contacter l&apos.auteur … l&apos.une des adresses mentionn‚es sur la page
:link reftype=hd res=60008.Questions et rapports d&apos.anomalies:elink..

:h2 res=IDD_METAFILE_VERIFY4.Erreur, v‚rification de m‚tafichier - Donn‚es incorrectes
Cette erreur est affich‚e quand l&apos.en-tˆte du m‚tafichier contient des
donn‚es incorrectes. V‚rifiez que le m‚tafichier peut ˆtre affich‚ ou
charg‚ par le programme PICVIEW (:hp1.Affichage d&apos.images:ehp1., dans le
dossier :hp1.Production:ehp1.). Si c&apos.est le cas, veuillez contacter
l&apos.auteur … l&apos.une des adresses mentionn‚es sur la page 
:link reftype=hd res=60008.Questions et rapports d&apos.anomalies:elink..

:h2 res=IDD_ERROR_LOADLANGUAGE.Erreur de choix de langue
Cette erreur se produit quand les fichiers de la langue choisie n&apos.ont pu
ˆtre complŠtement charg‚s. Veuillez contacter l&apos.auteur … l&apos.une des
adresses mentionn‚es sur la page 
:link reftype=hd res=60008.Questions et rapports d&apos.anomalies:elink..

:h1 res=61000.Documentation technique

:h2 res=61001.Format des fichiers documents
:p.Un fichier document peut ˆtre consid‚r‚ comme un flux de caractŠres et
de codes de fonctions. · l&apos.exception des codes de contr“le 15, 16
et 17, les caractŠres du texte sont stock‚s sans modification. Les codes
15, 16 et 17 sont remplac‚s par une s‚quence d&apos.‚chappement, 15
‚tant le code d&apos.‚chappement : si le texte contient un code ASCII
16, le fichier de donn‚es contient un code ASCII 15 suivi d&apos.un code
ASCII 16 pour repr‚senter le code 16 d&apos.origine. Si un code 15 est
rencontr‚ sans ˆtre suivi d&apos.un code 15, 16 ou 17, il est interpr‚t‚
comme un code 15 litt‚ral.
:p.
Un code litt‚ral 17 &odqf.&larrow.&cdqf. d‚finit le d‚but d&apos.un code fonction.
Un code litt‚ral 16 &odqf.&rarrow.&cdqf. d‚finit la fin d&apos.un code fonction.
Si un terminateur de code fonction est trouv‚ sans ˆtre pr‚c‚d‚ d&apos.un
d‚but de code fonction, il est trait‚ comme un caractŠre litt‚ral.
:p.
Les codes fonctions suivants sont actuellement utilis‚sÿ&colon.

:lm margin=3.:p.&larrow.Version&colon.:hp5.versionnumber:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit la version de Escriba qui a ‚t‚ utilis‚e
pour ‚diter et enregistrer le document sur disque. Le num‚ro de version
est compos‚ de quatre chiffres, qui sont trait‚s comme s&apos.il y avait une
virgule entre les deux premiers et les deux derniers. La version 1.00
serait repr‚sent‚e par le nombre 0100. Ce code de fonction est destin‚ …
permettre la conversion de documents d&apos.un format ancien vers un format
r‚cent, et n&apos.existait pas dans les fichiers cr‚‚s par les version
ant‚rieures … la beta 0.92. Ce code de fonction ne peut ˆtre utilis‚
ailleurs que dans les paramŠtres du code fonction &odqf.Preface&cdqf..

:lm margin=3.:p.&larrow.Preface&colon.:hp5.codesfonctions:ehp5.&rarrow.
:lm margin=5.:p.
Ceci est un code de fonction sp‚cial qui apparaŒt dans le document
sauvegard‚ mais n&apos.est pas visible … l&apos.‚cran. Les paramŠtres de cette
fonction sont d&apos.autres codes fonctions. L&apos.effet combin‚ de tous les
codes de fonctions est de d‚finir l&apos.‚tat par d‚faut du document. Tous
les codes fonctions qui modifient l&apos.‚tat du document peuvent ˆtre
utilis‚s, en sus des fonctions Printer, Form, JobSettings, TabSpacing et
Metric, qui ne peuvent pas ˆtre utilis‚es ailleurs. Les codes fonctions
qui g‚nŠrent du texte (PageCount, PageNumber, Graphic, Date) ne peuvent
ˆtre utilis‚s ici.

:lm margin=3.:p.&larrow.Printer&colon.:hp5.nom:ehp5.&rarrow.
:lm margin=5.:p.
Ce code fonction d‚finit le nom de l&apos.imprimante pour laquelle le
document a ‚t‚ format‚. Ce code ne peut ˆtre utilis‚ ailleurs que
dans les paramŠtres du code fonction &odqf.Preface&cdqf..

:lm margin=3.:p.&larrow.Form&colon.:hp5.nom:ehp5.&rarrow.
:lm margin=5.:p.
Ce code fonction d‚finit le nom du support d&apos.impression pour lequel
le document a ‚t‚ format‚. Il n&apos.est utilis‚ que quand un document con‡u
pour une imprimante donn‚e est reformatt‚ pour une autre. Ce code ne
peut ˆtre utilis‚ ailleurs que dans les paramŠtres du code fonction
&odqf.Preface&cdqf..

:lm margin=3.:p.&larrow.CodePage&colon.:hp5.nombre:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction est utilis‚ pour indiquer le num‚ro de la page de
codes … utiliser pour afficher le document. Ce code ne peut ˆtre utilis‚
ailleurs que dans les paramŠtres du code fonction &odqf.Preface&cdqf..
Voir :link reftype=hd res=61002.Pages de codes:elink. pour plus d&apos.informations.

:lm margin=3.:p.&larrow.Font&colon.:hp5.nom:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit le nom de la police … utiliser pour la suite
du document. Ce nom doit ˆtre celui d&apos.une police vectorielle
disponible dans la Palette de Polices du systŠme, ou dans le menu
Polices d&apos.Escriba.

:lm margin=3.:p.&larrow.Size&colon.:hp5.corps:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit, en points, le corps de la police … utiliser
pour la suite du document. Un point repr‚sente 1/72 de pouce anglais. Un
corps (une hauteur) de 12 points correspond normalement … des caractŠres
hauts de 1/6 pouce ; cette correspondance est une rŠgle de conduite qui
n&apos.est pas n‚cessairement adopt‚e par les concepteurs de polices de
caractŠres. Diff‚rentes polices n&apos.ont pas forc‚ment la mˆme taille
quand elles sont imprim‚e dans le mˆme corps. Escriba accepte les
tailles de corps donn‚es avec une r‚solution de 1/10 de point.

:lm margin=3.:p.&larrow.Color&colon.:hp5.couleur:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit la couleur (rouge-vert-bleu) … utiliser pour
les traits et le texte dans la suite du document. La couleur est
exprim‚e sous la forme d&apos.un nombre hexad‚cimal de six chiffres, les
deux premiers repr‚sentant la composante rouge, la paire suivante le
vert et la derniŠre paire le bleu. Le blanc, par exemple, serait
&odqf.FFFFFF&cdqf., le noir &odqf.000000&cdqf., et le jaune &odqf.FFFF00&cdqf..

:lm margin=3.:p.&larrow.LineWidth&colon.:hp5.width:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit l&apos.‚paisseur du trait utilis‚ pour
dessiner le contour des caractŠres, quand l&apos.attribut contour est
d‚fini. Cette ‚paisseur est exprim‚e dans la mˆme unit‚ que toutes les
autres dimensions.

:lm margin=3.:p.&larrow.LineEnd&colon.:hp5.type:ehp5.&rarrow.
:lm margin=5.:p.
Ce code d‚termine le type de fin de ligne … utiliser au del… de la
position actuelle.
Les types valides incluent tous les codes :hp2.LINEEND:ehp2. d‚finis
dans l&apos.&odqf.OS/2 Programmer&apos.s Reference&cdqf..

:lm margin=3.:p.&larrow.LineJoin&colon.:hp5.type:ehp5.&rarrow.
:lm margin=5.:p.
Ce code d‚termine le type de jointure … utiliser au del… de la
position actuelle.
Les types valides incluent tous les codes :hp2.LINEJOIN:ehp2. d‚finis
dans l&apos.&odqf.OS/2 Programmer&apos.s Reference&cdqf..

:lm margin=3.:p.&larrow.FillColor&colon.:hp5.couleur:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit la couleur (rouge-vert-bleu) … utiliser pour
le remplissage des caractŠres poss‚dant l&apos.attribut contour dans la
suite du document. La couleur est exprim‚e de la mˆme fa‡on que pour le
code de fonction :hp2.Color:ehp2. (voir plus haut).

:lm margin=3.:p.&larrow.FillType&colon.:hp5.type:ehp5.&rarrow.
:lm margin=5.:p.
Ce code d‚termine le motif utilis‚ pour remplir le texte poss‚dant
l&apos.attribut :hp1.Contour:ehp1. … utiliser au del… de la position
actuelle.
Les types valides incluent tous les codes :hp2.PATSYM:ehp2. d‚finis
dans l&apos.&odqf.OS/2 Programmer&apos.s Reference&cdqf..

:lm margin=3.:p.&larrow.Bold&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction active ou d‚sactive l&apos.attribut :hp1.gras:ehp1. …
partir de la position actuelle.

:lm margin=3.:p.&larrow.Italic&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction active ou d‚sactive l&apos.attribut :hp1.italique:ehp1. …
partir de la position actuelle.

:lm margin=3.:p.&larrow.Underscore&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction active ou d‚sactive l&apos.attribut :hp1.soulign‚:ehp1. …
partir de la position actuelle.

:lm margin=3.:p.&larrow.Strikeout&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction active ou d‚sactive l&apos.attribut :hp1.biff‚:ehp1. …
partir de la position actuelle.

:lm margin=3.:p.&larrow.Outline&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction active ou d‚sactive l&apos.attribut :hp1.contour:ehp1. …
partir de la position actuelle.

:lm margin=3.:p.&larrow.Caps&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction active ou d‚sactive l&apos.attribut :hp1.majuscules:ehp1.
… partir de la position actuelle.

:lm margin=3.:p.&larrow.Expand&colon.:hp5.pourcentage:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit la largeur de texte … appliquer … partir de
la position actuelle. Ce facteur est exprim‚ sous forme de pourcentage,
100 repr‚sentant la taille normale des caractŠres.

:lm margin=3.:p.&larrow.TopMargin&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit la position courante de la marge du haut,
relativement au bas de la page. Cet attribut ne prend son effet qu&apos.…
la page suivante, sauf si la page en cours ne contient encore aucun
‚l‚ment imprimable.

:lm margin=3.:p.&larrow.BottomMargin&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit la position courante de la marge du bas,
relativement au bas de la page. Cet attribut ne prend son effet qu&apos.…
la page suivante, sauf si la page en cours ne contient encore aucun
‚l‚ment imprimable.

:lm margin=3.:p.&larrow.LeftMargin&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit la position courante de la marge gauche,
relativement au c“t‚ gauche de la page. Cet attribut ne prend son effet
qu&apos.… la ligne suivante, sauf si la ligne en cours ne contient encore
aucun ‚l‚ment imprimable.

:lm margin=3.:p.&larrow.RightMargin&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit la position courante de la marge de droite,
relativement au c“t‚ gauche de la page. Cet attribut ne prend son effet
qu&apos.… la ligne suivante, sauf si la ligne en cours ne contient encore
aucun ‚l‚ment imprimable.

:lm margin=3.:p.&larrow.LeftJustify&rarrow.
:lm margin=5.:p.
Ce code de fonction active la justification horizontale … gaucheÿ&colon.
le texte sera align‚ contre la marge gauche. Cet attribut ne prend son
effet qu&apos.… la ligne suivante, sauf si la ligne en cours ne contient
encore aucun ‚l‚ment imprimable.

:lm margin=3.:p.&larrow.CenterJustify&rarrow.
:lm margin=5.:p.
Ce code de fonction active la justification horizontale centr‚eÿ&colon.
le texte sera centr‚ entre les marges gauche et droite. Cet attribut ne
prend son effet qu&apos.… la ligne suivante, sauf si la ligne en cours ne
contient encore aucun ‚l‚ment imprimable.

:lm margin=3.:p.&larrow.RightJustify&rarrow.
:lm margin=5.:p.
Ce code de fonction active la justification horizontale … droiteÿ&colon.
le texte sera align‚ contre la marge droite. Cet attribut ne prend son
effet qu&apos.… la ligne suivante, sauf si la ligne en cours ne contient
encore aucun ‚l‚ment imprimable.

:lm margin=3.:p.&larrow.FullJustify&rarrow.
:lm margin=5.:p.
Ce code de fonction active la justification horizontale … droite et …
gaucheÿ&colon. le texte sera compl‚t‚ pour ˆtre align‚ … la fois sur les
marges droite et gauche. Les lignes de fin de paragraphe ou les lignes
isol‚es sont align‚es … gauche. Cet attribut ne prend son effet qu&apos.… la
ligne suivante, sauf si la ligne en cours ne contient encore aucun
‚l‚ment imprimable.

:lm margin=3.:p.&larrow.CenterPage&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit l&apos.‚tat de l&apos.attribut :hp1.centrage
vertical:ehp1. de la page. Quand il est actif au d‚but d&apos.une page, cette
page est centr‚e verticalement.

:lm margin=3.:p.&larrow.TabSet&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction ajoute un taquet de tabulation … la position
sp‚cifi‚e, dans la table de tabulation.

:lm margin=3.:p.&larrow.TabClear&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction supprime le taquet de tabulation pr‚sent … la
position sp‚cifi‚e.

:lm margin=3.:p.&larrow.TabSpacing&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit la distance … utiliser entre deux taquets de
tabulation au d‚but du document. · partir de la marge gauche, des
taquets de tabulation sont cr‚‚s automatiquement … l&apos.intervalle
d‚fini ici. Ce code de fonction ne peut ˆtre utilis‚ ailleurs que dans
les paramŠtres du code fonction &odqf.Preface&cdqf..

:lm margin=3.:p.&larrow.Indent&colon.:hp5.distance:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit la distance d&apos.indentation des lignes de
d‚but de paragraphe. Les lignes de d‚but de paragraphe sont (1) la
premiŠre ligne du document et (2) la premiŠre ligne de texte aprŠs un
caractŠre de fin de paragraphe.

:lm margin=3.:p.&larrow.Metric&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit l&apos.unit‚ de mesure utilis‚e dans le document.
Si le paramŠtre est &odqf.1&cdqf., l&apos.unit‚ de mesure est le centimŠtre ; dans le
cas contraire, l&apos.unit‚ est le pouce.
Ce code de fonction ne peut ˆtre utilis‚ ailleurs que dans les paramŠtres du code fonction &odqf.Preface&cdqf..

:lm margin=3.:p.&larrow.Graphic&colon.:hp5.x,y,cx,cy[,Background][,NoTextAside][,Halftone][,Reverse][,PrinterOptimize][[,KeepAspectFixX|KeepAspectFixY]][,BorderWidth=n][,BorderColor=rrggbb][,BorderCaption="text"]]&colon.type&bsl.0data:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit une zone de graphique dans le document. 
:p.The &apos.x&apos. and &apos.y&apos. coordinates define the cell&apos.s position on the page, relative
to the upper-left corner of the page.  Documents saved before revision 0.97 specify their graphics
relative to the lower-left corner of the page.
Les paramŠtres :hp2.cx:ehp2. et :hp2.cy:ehp2. d‚finissent la largeur et la hauteur de la zone. Les
quatres paramŠtres sont donn‚s dans l&apos.unit‚ active (milliŠme de pouce ou
de centimŠtre).
:p.Les coordonn‚es num‚riques peuvent ˆtre remplac‚es par les mots-cl‚s
:hp2.Inline:ehp2.,
:hp2.LeftMargin:ehp2.,
:hp2.RightMargin:ehp2.,
:hp2.LeftEdge:ehp2.,
:hp2.RightEdge:ehp2. ou
:hp2.Centered:ehp2. pour la position horizontale, et
:hp2.TopMargin:ehp2.,
:hp2.BottomMargin:ehp2.,
:hp2.TopEdge:ehp2.,
:hp2.BottomEdge:ehp2. ou
:hp2.Centered:ehp2. pour la position verticale. Les mots-cl‚s
:hp2.EdgeToEdge:ehp2. ou
:hp2.MarginToMargin:ehp2. 
sont accept‚s pour les deux dimensions.
Si &apos.Inline&apos. est sp‚cifi‚ pour la position X, la position Y est
ignor‚e. De mˆme, si &apos.EdgeToEdge&apos. ou
&apos.MarginToMargin&apos. est sp‚cifi‚ pour la taille X ou Y, la
position correspondante est ignor‚e.

:p.Les paramŠtres :hp2.Background:ehp2., :hp2.NoTextAside:ehp2.,
:hp2.Halftone:ehp2. et :hp2.Reverse:ehp2. sont optionnels. Ils doivent, le cas ‚ch‚ant,
figurer :hp2.exactement:ehp2. tels qu&apos.indiqu‚ ici.
:p.Le paramŠtre :hp2.Background:ehp2. indique que le graphique ne doit
pas influer sur la position du texte sur la page, et doit apparaŒtre
derriŠre le texte, en arriŠre-plan.
:p.Le paramŠtre :hp2.NoTextAside:ehp2. indique qu&apos.aucun texte ne doit
apparaŒtre sur les c“t‚s du graphiqueÿ; le graphique est trait‚ comme
s&apos.il occupait toute la largeur de la page.
:p.Le paramŠtre :hp2.Halftone:ehp2., quand il est pr‚sent, indique que
les couleurs du graphique doivent ˆtre at‚nu‚es. Cette op‚ration est
effectu‚e en ne dessinant qu&apos.un pixel sur deux, ce qui fait que le
paramŠtre n&apos.est pas valide dans le cas de m‚tafichiers graphiques.
:p.Le paramŠtre &apos.Reverse&apos. indique que le graphique doit ˆtre
affich‚ en couleurs invers‚es. Cette option ne peut pas ˆtre g‚r‚e pour
les m‚tafichiers graphiques.

:p.Le paramŠtre :hp2.PrinterOptimize:ehp2., quand il est pr‚sent,
indique que le graphique doit ˆtre optimis‚ pour la r‚solution de
l&apos.imprimante. Pour cela, l&apos.‚chelle choisie lors de l&apos.impression est un
nombre entier. Cette option ne s&apos.applique pas aux m‚tafichiers.

:p.Le paramŠtre :hp2.KeepAspectFixX:ehp2. indique que la hauteur du
graphique sera d‚termin‚e d&apos.aprŠs sa largeur, en conservant
l&apos.aspect du graphique original. Le paramŠtre :hp2.KeepAspectFixY:ehp2.
est similaire, sauf que c&apos.est la largeur qui sera d‚termin‚e d&apos.aprŠs la
hauteur.

:p.The &apos.BorderWidth=n&apos. parameter declares that the graphic border will be a specific
size in thousandths of an inch or centimeter, depending on the document metric type.

:p.The &apos.BorderColor=rrggbb&apos. parameter declares the red-blue-green color of the graphic border.

:p.The &apos.BorderCaption="text"&apos. parameter declares the graphic caption text.  This text 
is literal, except that all embedded double-quote characters are doubled.

:p.Le champ :hp2.Type:ehp2. d‚finit le type du graphique, parmi
:hp2.BM:ehp2. (pour bitmap),
:hp2.MF:ehp2. (pour m‚tafichier),
:hp2.UP:ehp2. (pour code … barre UPC), 
:hp2.39:ehp2. (pour code … barre 39),
:hp2.PN:ehp2. (pour code … barre POSTNET),
:hp2.E8:ehp2. (pour code … barre EAN-8),
:hp2.27:ehp2. (for Codabar (2 of 7)),
:hp2.25:ehp2. (for Code 2 of 5) ou
:hp2.28:ehp2. (for Code-128).
Tous les graphiques bitmaps (BMP, GIF, JPG, PCX, TIF...) sont stock‚s
dans le document sous forme de bitmap OS/2. Ce champ doit ˆtre termin‚
par un z‚ro (chaŒne ASCIIZ).
:p.Les donn‚es du graphique sont plac‚es aprŠs le z‚ro qui termine
l&apos.en-tˆte. Les codes 15, 16 et 17 pr‚sents dans les donn‚es binaires du
graphiques sont cod‚s comme d‚fini plus haut.
:p.Pour les bitmaps, les donn‚es contiennent tout d&apos.abord un nombre de
quatre octets indiquant la taille de l&apos.en-tˆteÿ; ensuite vient un nombre
de quatre octets indiquant la taille des donn‚es du bitmap. Viennent
enfin l&apos.en-tˆte puis les donn‚es du bitmap.
:p.Pour les m‚tafichiers, les donn‚es binaires incluent un nombre de
quatre octets donnant la taille des donn‚es du m‚tafichier, puis le
m‚tafichier lui-mˆme.
:p.Pour les codes UPC, les donn‚es contiennent un texte de douze
chiffres repr‚sentant le code UPC.
:p.Pour les codes Code-39, les donn‚es peuvent ˆtre de n&apos.importe quelle
longueur, les caractŠres valides ‚tantÿ&colon. les lettre de A … Z
(en majuscules), les chiffres de 0 … 9, l&apos.espace, et les signes
®&dot.¯,
®&plus.¯,
®&dash.¯,
®&asterisk.¯,
®&slash.¯,
®&dollar.¯ et
®&percent.¯.
:p.Pour les codes … barres POSTNET, les donn‚es sont compos‚es de cinq
ou neuf chiffres.
:p.Pour les codes … barres EAN-8, les donn‚es sont compos‚es de sept
caractŠres num‚riques.
:p.For Codabar codes, the data can be any number and combination of the characters 
0 to 9, &dash., &dollar., &colon., &slash., &dot. or &plus., and the first and last characters must be identical
and in the range A to D.

:lm margin=3.:p.&larrow.Date&colon.Fixed&colon.:hp5.time:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction g‚nŠre un texte repr‚sentant une date sp‚cifique,
date exprim‚e en secondes ‚coul‚es depuis le premier janvier 1970 …
minuit. Le format de la date est d‚fini par la fonction &odqf.DateFormat&cdqf..

:lm margin=3.:p.&larrow.Date&colon.Current&rarrow.
:lm margin=5.:p.
Ce code de fonction g‚nŠre un texte repr‚sentant la date du jour
d&apos.aprŠs l&apos.horloge du systŠme. Le format de la date est d‚fini
par la fonction &odqf.DateFormat&cdqf..

:lm margin=3.:p.&larrow.Date&colon.Changed&rarrow.
:lm margin=5.:p.
Ce code de fonction g‚nŠre un texte repr‚sentant la date de la derniŠre
modification apport‚e au document. Le format de la date est d‚fini par
la fonction &odqf.DateFormat&cdqf..

:lm margin=3.:p.&larrow.DateFormat&colon.:hp5.chaŒneformat:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit le format de date qui sera utilis‚ dans le
document … partir de la position courante. La syntaxe est similaire …
celle utilis‚es dans l&apos.instruction :hp2.printf:ehp2. du langage
:hp2.C:ehp2., … l&apos.exception des codes pouvant suivre le caractŠre
&odqf.&percent.&cdqf., qui sontÿ&colon.

:lm margin=10.:sl compact.
:li.m - mois, sous forme num‚rique (1, 2, etc)
:li.M - mois, en clair (janvier, f‚vrier, etc)
:li.n - mois abr‚g‚ (jan, f‚v, etc)
:li.d - jour du mois
:li.y - ann‚e (siŠcle inclus)
:li.z - ann‚e (2 chiffres, sans le siŠcle)
:li.w - jour de la semaine (dimanche, lundi, etc)
:li.x - jour de la semaine abr‚g‚ (dim, lun, etc)
:esl.:lm margin=5.
:note.Ce format interne n&apos.est pas d‚pendant du langage, contrairement
aux codes fournis par l&apos.utilisateur.

:lm margin=3.:p.&larrow.LineSpacing&colon.:hp5.pourcentage:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit l&apos.interligne, en pourcentage de
l&apos.interligne normal, … utiliser pour la suite du document.

:lm margin=3.:p.&larrow.JobSettings&colon.:hp5.donn‚espilote:ehp5.&rarrow.
:lm margin=5.:p.
Le paramŠtre de ce code de fonction est la structure de donn‚es
retourn‚e par la boŒte de dialogue de configuration de
l&apos.impression. La structure complŠte est fonction du pilote
d&apos.imprimante, et n&apos.est pas forc‚ment document‚e par
l&apos.auteur du pilote d&apos.imprimante ; une partie de ces donn‚es,
appel‚e en-tˆte, est d‚finie et document‚e par IBM. Les donn‚es ‚tant
sous forme binaire, les codes 15, 16 et 17 sont pr‚c‚d‚s d&apos.un code
d&apos.‚chappement. Ce code de fonction ne peut ˆtre utilis‚ ailleurs
que dans les paramŠtres du code fonction &odqf.Preface&cdqf..

:lm margin=3.:p.&larrow.PageNumber&rarrow.
:lm margin=5.:p.
Ce code de fonction g‚nŠre un texte repr‚sentant le num‚ro de la page
dans laquelle se trouve le code.

:lm margin=3.:p.&larrow.PageCount&rarrow.
:lm margin=5.:p.
Ce code de fonction g‚nŠre un texte repr‚sentant le nombre de pages
dans le document.

:lm margin=3.:p.&larrow.Glyph&colon.:hp5.nombre:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction g‚nŠre un texte d&apos.un caractŠre correspondant au
paramŠtre num‚rique. Le caractŠre affich‚ ou imprim‚ d‚pend de la page
de codes et de la police en cours.
Ce code de fonction est cr‚‚ quand l&apos.utilisateur utilise les touches
Alt+nnn pour entrer un caractŠre, et r‚soud le problŠme de savoir si un
code de contr“le doit ˆtre trait‚ comme tel ou affich‚.

:lm margin=3.:p.&larrow.Author&colon.:hp5.name:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fontion n&apos.apparaŒt que dans la section :hp2.Preface:ehp2. et
permet de d‚finir le nom de l&apos.auteur du document.

:lm margin=3.:p.&larrow.CreateDate&colon.:hp5.time:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fontion n&apos.apparaŒt que dans la section :hp2.Preface:ehp2. et
permet d&apos.enregistrer la date de cr‚ation du document. Cette date est
enregistr‚e sous la mˆme forme que les autres, c&apos.est … dire en nombre de
secondes ‚coul‚es depuis le 1er janvier 1970.

:lm margin=3.:p.&larrow.Comment&colon.:hp5.commentary:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction peut ˆtre utilis‚ pour inclure un commentaire dans
le corps du document. Le commentaire ne sera ni affich‚ ni imprim‚.

:lm margin=3.:p.&larrow.Separateur&rarrow.
:lm margin=5.:p.
Ce code de fonction genŠre une fin de ligne, suivie par une ligne de
s‚paration, puis une autre fin de ligne. Cette fonction est similaire …
la fonction :hp2.<hr>:ehp2. du langage HTML.

:lm margin=3.:p.&larrow.Title&colon.:hp5.nom:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction n'apparaŒt que dans la section Preface et d‚clare le
titre du document.

:lm margin=3.:p.&larrow.Header&colon.:hp5.texte:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit l'en-tˆte de page … utiliser … partir de ce
point. Si rien d'imprimable ne figure encore sur la page, l'en-tˆte est
aussi utilis‚ pour la page courante.

:lm margin=3.:p.&larrow.Footer&colon.:hp5.texte:ehp5.&rarrow.
:lm margin=5.:p.
Ce code de fonction d‚finit le pied de page … utiliser … partir de ce
point. Si rien d'imprimable ne figure encore sur la page, il est aussi
utilis‚ pour la page courante.

:lm margin=1.

:note.Les distances sont exprim‚es en unit‚s de 0,001 pouce ou
centimŠtre, en fonction de l&apos.unit‚ de mesure choisie. Une distance
de 0,5 centimŠtre ou pouce serait donc exprim‚e 500.

:note.Les pourcentages sont exprim‚s sous forme d&apos.entiers. Par
exemple, 100% est exprim‚ par le nombre 100, 125% par le nombre 125,
etc...

:h2 res=61002.Pages de codes
:p.Les pages de codes (codification nationale des caractŠres) suivantes
sont validesÿ&colon.

:table cols='4 60' rules=both frame=box.
:row.:c.037     :c.Br‚silien / Portugais / Anglais Am‚ricain (EBCDIC)
:row.:c.273     :c.Autrichien / Allemand (EBCDIC)
:row.:c.274     :c.Belge (EBCDIC ancien)
:row.:c.277     :c.Danois / Norv‚gien (EBCDIC)
:row.:c.278     :c.Finnois / Su‚dois (EBCDIC)
:row.:c.280     :c.Italien (EBCDIC)
:row.:c.282     :c.Portugais (EBCDIC ancien)
:row.:c.284     :c.Espagnol (EBCDIC)
:row.:c.285     :c.Anglais (EBCDIC)
:row.:c.297     :c.Fran‡ais (EBCDIC)
:row.:c.437     :c.tats-Unis (IBM PC)
:row.:c.500     :c.Belge / International (EBCDIC)
:row.:c.708     :c.Arabic (ASMO 708)
:row.:c.709     :c.Arabic (ASMO 449, BCON V4)
:row.:c.710     :c.Arabic (transparent Arabic)
:row.:c.711     :c.Arabic (Nafitha Enhanced)
:row.:c.720     :c.Arabic (transparent ASMO)
:row.:c.813     :c.Grec
:row.:c.819     :c.Multilingue (ISO-8859/1)
:row.:c.850     :c.Multilingue (Latin 1)
:row.:c.852     :c.Slave (Latin2)
:row.:c.855     :c.Serbie / Montenegro / Mac‚donie / Bulgarie
:row.:c.857     :c.Turc
:row.:c.860     :c.Portugais
:row.:c.861     :c.Islandais
:row.:c.862     :c.H‚breu
:row.:c.863     :c.Fran‡ais Canadien
:row.:c.864     :c.Arabe
:row.:c.865     :c.Danois / Norv‚gien
:row.:c.866     :c.Russie
:row.:c.869     :c.Grec
:row.:c.870     :c.Tch‚coslovaque / Hongrois / Polonais / Yougoslave (EBCDIC)
:row.:c.871     :c.Islandais (EBCDIC)
:row.:c.874     :c.Tha‹
:row.:c.915     :c.Bulgarie / Mac‚doine
:row.:c.932     :c.Japonais (Shift JIS X-0208)
:row.:c.934     :c.Cor‚en
:row.:c.936     :c.Chinois simplifi‚
:row.:c.938     :c.Chinois traditionnel
:row.:c.942     :c.Japonais (SAA)
:row.:c.944     :c.Cor‚en (SAA)
:row.:c.946     :c.Chinois simplifi‚ (SAA)
:row.:c.948     :c.Chinois traditionnel (SAA)
:row.:c.949     :c.Cor‚en (Shift KSC 5601)
:row.:c.950     :c.Chinois traditionnel (Big5)
:row.:c.1004    :c.Publication
:row.:c.1026    :c.Turc (EBCDIC)
:row.:c.1200    :c.Unicode
:row.:c.1250    :c.Windows, Latin-2 (Europe de l&apos.Est)
:row.:c.1251    :c.Windows, Cyrillique
:row.:c.1252    :c.Windows, Multilingual (ANSI)
:row.:c.1253    :c.Windows, Grec
:row.:c.1254    :c.Windows, Turc
:row.:c.1255    :c.Windows, Hebreu
:row.:c.1256    :c.Windows, Arabe
:row.:c.1257    :c.Windows, Baltic
:etable.

:note.Toutes les pages de codes ne sont pas disponibles sur tous les
systŠmes.

:h2 res=61003.Interface de modules d&apos.extension
:p.Le gestionnaire d&apos.extensions inventorie, au d‚marrage d&apos.Escriba,
les bibliothŠques li‚es dynamiquement (DLL) pr‚sentes dans le r‚pertoire
d&apos.installation. Les fonctions :hp2.Identify:ehp2.,
:hp2.ProductName:ehp2., :hp2.ProductVersion:ehp2.,
:hp2.ProductInformation:ehp2. et :hp2.ProductConfigure:ehp2. sont
recherch‚es dans chaque DLL. Si toutes ces fonctions sont pr‚sentes, la
bibliothŠque est suppos‚e ˆtre un module d&apos.extension pour Escriba.

:p.Les prototypes de fonctions pour le langage C sontÿ&colon.

:font facename=Courier size=13x8.:lm margin=6.:sl compact.
:li.char* APIENTRY Identify ( void ) ;
:li.BOOL  APIENTRY SetLanguage ( char *Language ) ;
:li.char* APIENTRY ProductName ( void ) ;
:li.int   APIENTRY ProductVersion ( void ) ;
:li.void  APIENTRY ProductInformation ( HWND Owner ) ;
:li.int   APIENTRY ProductConfigure ( int MaxPages, char Names[][64], char MajorTabs[][32], char MinorTabs[][32], PFNWP Processors[], HMODULE Libraries[], int DialogIDs[] ) ;
:esl.:lm margin=1.:font facename=default.

:p.La fonction :hp2.Identify:ehp2. retourne une chaŒne ASCIIZ
caract‚ristique du type d&apos.extension. Les types suivants sont
actuellement d‚finisÿ&colon.
:link reftype=hd res=61004."ISPELLER":elink.
,
:link reftype=hd res=61005."IMPORT/EXPORT":elink.
and
:link reftype=hd res=61006."GRAPHICIMPORT":elink..

:p.La fonction :hp2.SetLanguage:ehp2. permet … Escriba de configurer
la langue utilis‚e par le module. Le nom de la langue est pass‚ en
paramŠtre. Par exemple, le paramŠtre … utiliser pour le fran‡ais
est :hp1.Fran&cc.ais:ehp1..

:p.La fonction :hp2.ProductName:ehp2. retourne une chaŒne ASCIIZ
permettant d&apos.identifier l&apos.extension dans le menu :hp1.Infos
produit:ehp1.. Ce nom ne doit pas comporter de num‚ro de version.

:p.La fonction :hp2.ProductVersion:ehp2. retourne un nombre entier
qui repr‚sente le num‚ro de version multipli‚ par 100. Ce nombre
est aussi utilis‚ dans le menu :hp1.Infos produit:ehp1..

:p.La fonction :hp2.ProductInformation:ehp2. affiche une boŒte de dialogue
contenant le nom complet du module, le num‚ro de version et la date,
ainsi que les informations concernant l&apos.auteur et les droits. Cette
fonction est appel‚e quand l&apos.utilisateur choisit le module dans le
menu :hp1.Infos produit:ehp1..

:p.La fonction :hp2.ProductConfigure:ehp2. retourne les informations
n‚cessaires … la construction du dialogue :hp1.Configuration des
extensions:ehp1.. Cette fonction, si elle est pr‚sente, doit retourner
le nombre de pages du bloc-notes de configuration dont a besoin le
module. Pour chaque page demand‚e, la fonction doit retourner un num‚ro
de page, un texte d&apos.onglet principal ou secondaire (l&apos.un ou l&apos.autre),
une fonction de traitement du dialogue, une poign‚e
(®:hp1.handle:ehp1.¯) de bibliothŠque de ressources et un identificateur
de ressource pour le masque du dialogue.

:h3 res=61004.Interface ISpeller
:p.Le module :hp2.ISPELLER:ehp2. a ‚t‚ d‚velopp‚ … partir du programme GNU
ISPELL version 3.1.18, et est donc gratuit et disponible avec le code
source. Le copyright appartient … Pace Willisson et Geoff Kuenning.
ISPELLER n&apos.est qu&apos.un habillage d&apos.ISPELL, sous la forme d&apos.une
bibliothŠque OS/2 (DLL). ISPELLER ne peut pas utiliser une autre version
3.1.x de ISPELL ‚ventuellement pr‚sente sur votre systŠme, mais
:hp2.peut:ehp2. utiliser les fichiers dictionnaires. Pour cela, ajoutez
la commande :hp2.SET ISPELL=chamin:ehp2. dans votre fichier CONFIG.SYS,
:hp1.chemin:ehp1. ‚tant le nom complet du r‚pertoire o— se trouvent les
dictionnaires ISPELL. Les modules ISPELLER.DLL et BUILDHAS.EXE doivent
aussi ˆtre copi‚s dans ce r‚pertoire.

:p.En plus des fonctions de modules d&apos.extension d‚j… d‚crites, ISpeller
fournit les fonctions suivantesÿ&colon.

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

:p.Si vous d‚sirez savoir ce que font ces fonctions, et comment elles le
font, n&apos.h‚sitez pas … consulter le code source.

:h3 res=61005.Interface de module d&apos.import/export de document
:p.Les modules de type :hp2.IMPORT/EXPORT:ehp2. sont utilis‚s pour lire
et ‚crire des fichiers dans des formats diff‚rent de celui utilis‚
par Escriba.

:p.En plus des fonctions de base des modules d&apos.extension, les modules
d&apos.import/export fournissent les fonctions suivantesÿ&colon.
:font facename=Courier size=13x8.:lm margin=6.:sl compact.
:li.char* APIENTRY QueryFileType ( void ) ;
:li.char* APIENTRY QueryWildcard ( void ) ;
:li.BOOL  APIENTRY Filter ( char *Name, char *Type ) ;
:li.BOOL  APIENTRY Verify ( HWND Owner, char *Name ) ;
:li.void* APIENTRY Import ( HWND Owner, char *Name, long *Size ) ;
:li.BOOL  APIENTRY Export ( HWND Owner, char *Name, void *Text, long Size ) ;
:esl.:lm margin=1.:font facename=default.

:p.La fonction :hp2.QueryFileType:ehp2. retourne un type de fichier
(comme :hp1.Escriba:ehp1. ou :hp1.Texte brut:ehp1.) qui sera affich‚
dans la liste de types de fichiers du dialogue d&apos.ouverture de fichier.

:p.La fonction :hp2.QueryWildcard:ehp2. retourne une sp‚cification de
fichier (comme :hp1.*.Txt:ehp1.) qui sera utilis‚e dans le champ
:hp1.Nom de fichier:ehp1. du dialogue d&apos.ouverture de fichier quand le
type correspondant sera choisi.

:p.La fonction :hp2.Filter:ehp2. est appel‚e lors du remplissage de la
liste de fichiers du dialogue d&apos.ouverture de fichier, et d‚termine si un
nom de fichier doit ˆtre ou non affich‚ dans la liste. Il s&apos.agit d&apos.un
test rapide du nom et du type du fichier, pas d&apos.un test du contenu du
fichier.

:p.La fonction :hp2.Verify:ehp2. est utilis‚e pour v‚rifier la nature
du contenu du fichier. Le test doit ˆtre rapide, mais plus approfondi
que celui effectu‚ par la fonction :hp2.Filter:ehp2..

:p.La fonction :hp2.Import:ehp2. charge un document en m‚moire, le
convertit au format natif d&apos.Escriba, et retourne un pointeur sur le
texte. La fonction Import doit allouer dynamiquement la m‚moire pour le
texte. Escriba libŠrera cette m‚moire quand elle ne sera plus
n‚cessaire.

:p.La fonction :hp2.Export:ehp2. sauvegarde un fichier document. Un
pointeur sur le texte au format Escriba est pass‚ en paramŠtre, la
fonction Export se charge de le convertir et de l&apos.‚crire dans le
fichier sp‚cifi‚.

:h3 res=61006.Interface de module d&apos.importation graphique
:p.Les modules de type :hp2.GRAPHICIMPORT:ehp2. sont utilis‚s pour
importer des images graphiques dans un document, en convertissant ces
images au format bitmap d&apos.OS/2.

:p.En plus des fonctions de base des modules d&apos.extension, les
modules d&apos.importation graphique fournissent les fonctions
suivantesÿ&colon.

:font facename=Courier size=13x8.:lm margin=6.:sl compact.
:li.char*   APIENTRY QueryFileType ( void ) ;
:li.char*   APIENTRY QueryWildcard ( void ) ;
:li.BOOL    APIENTRY Filter ( char *Name, char *Type ) ;
:li.BOOL    APIENTRY Verify ( HWND Owner, char *Name ) ;
:li.HBITMAP APIENTRY GraphicImport ( HWND Owner, char *Name ) ;
:esl.:lm margin=1.:font facename=defau.

:p.La fonction :hp2.QueryFileType:ehp2. retourne un type de fichier
(comme :hp1.Bitmap:ehp1. ou :hp1.M‚tafichier:ehp1.) qui sera affich‚
dans la boŒte de dialogue d&apos.ouverture d&apos.un fichier graphique,
dans la liste des types.

:p.La fonction :hp2.QueryWildcard:ehp2. retourne une sp‚cification de
fichier (comme :hp1.*.Txt:ehp1.) qui sera utilis‚e dans le champ
:hp1.Nom de fichier:ehp1. du dialogue d&apos.ouverture de fichier
graphique quand le type correspondant sera choisi.

:p.La fonction :hp2.Filter:ehp2. est appel‚e lors du remplissage de la
liste de fichiers du dialogue d&apos.ouverture de fichier graphique, et
d‚termine si un nom de fichier doit ˆtre ou non affich‚ dans la liste.
Il s&apos.agit d&apos.un test rapide du nom et du type du fichier, pas
d&apos.un test du contenu du fichier.

:p.La fonction :hp2.Verify:ehp2. est utilis‚e pour v‚rifier la nature
du contenu du fichier. Le test doit ˆtre rapide, mais plus approfondi
que celui effectu‚ par la fonction :hp2.Filter:ehp2..

:p.La fonction :hp2.GraphicImport:ehp2. charge un graphique en m‚moire,
le convertit au format bitmap natif d&apos.OS/2, et retourne la poign‚e
(:hp1.handle:ehp1.) du bitmap.

:euserdoc.
