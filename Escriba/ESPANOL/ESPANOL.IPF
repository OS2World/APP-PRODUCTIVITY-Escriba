#include "..\Escriba.h"
#ifdef APIENTRY
#undef APIENTRY
#endif
:userdoc.
:title.Ayuda del procesador de textos Escriba
:docprof toc=123456.

:h1 res=ID_MAIN.Introducci¢n
:i1.Introducci¢n
:artwork name='Escriba.bmp' align=center.
:p.
Este es un procesador de textos "WYSIWYG" (lo que se ve en pantalla
es lo que se obtiene por la impresora) para el OS/2 Presentation
Manager. No pretende competir en prestaciones con los grandes
procesadores de textos disponibles para el OS/2 PM o Windows, ya que
ofrece un conjunto de funciones m s modesto. Dispone de las funciones
normales de formateo de textos m s la habilidad de insertar gr ficos.
Soporta todas las fuentes ATM instaladas, en todos sus formatos nativos
y en un  mplio rango de tama¤o. El soporte de impresora es a trav‚s del
interface de OS/2 PM, de forma que es posible la impresi¢n en cualquier
impresora soportada por este interface, incluyendo m quinas 'FAX' y que
usan lapiceros para dibujar (plotters).
En versiones orientales (chinos, japoneses, koreanas) entrada y imprenta
de textos de doble bytes (DBCS) se apoya, tanto como tambi‚n los fuentes
del tipo TrueType que est‚n instalados a trav‚s del palete de fuentes.
:p.
:sl compact.
:li.&sqbul.:link reftype=hd res=60001.Inicio y salida:elink.
:li.&sqbul.:link reftype=hd res=60002.Lectura y grabaci¢n de documentos:elink.
:li.&sqbul.:link reftype=hd res=60003.Editando documentos:elink.
:li.&sqbul.:link reftype=hd res=60004.Imprimiendo documentos:elink.
:li.&sqbul.:link reftype=hd res=60005.Gesti¢n de ventanas:elink.
:li.&sqbul.:link reftype=hd res=60006.Algunas precauciones:elink.
:li.&sqbul.:link reftype=hd res=60007.Registro:elink.
:esl.

:h2 res=60001.Inicio y salida
:hp4.Inicio:ehp4.
:p.
Una vez instalado Escriba, hay tres m‚todos para ejecutarlo. El primero es
haciendo un doble "click" sobre el icono del programa que ha sido creado
durante la instalaci¢n. De esta forma, se ejecutar  y crear  un
documento en blanco para uso inmediato.
:p.
El segundo m‚todo es arrastrar un documento sobre el objeto del programa
y soltarlo. De esta forma se abrir  el documento, mostr ndolo en el  rea
de trabajo, listo para su edici¢n.
:p.
El tercer m‚todo con la orden START. Normalmente esto s¢lo funcionar  si
el directorio actual es d¢nde est  instalado Escriba. Si has incluido este
directorio en las ¢rdenes PATH, HELP y LIBPATH de tu CONFIG.SYS entonces
si que puedes utilizar la orden START desde cualquier directorio del
ordenador para ejecutar Escriba.
:p.
Hay un modo indirecto de arrancar Escriba.  Cuando Escriba graba un documento
en su formato nativo, el documento est  marcado del tipo &apos.Escriba&apos..
Este tipo se asocia a trav‚s del sistema de ficheros con el program Escriba,
y cuando se abre un objeto que representa un documento del tipo Escriba,
la acci¢n por defecto es arrancar Escriba y cargar el documento para edici¢n.
Escriba tambi‚n se asocia con ficheros de los tipos &apos.Pluma&apos. y &apos.Plain Text&apos.
tambi‚n, y aparece en el men£ de Abrir del men£ de contexto de tales ficheros.

:p.
:hp4.Salida:ehp4.
:p.
Para finalizar la ejecuci¢n de Escriba, podemos hacer un doble "click" en
el men£ de sistema de programa (el mini-icono en el extremo superior
izquierda de la ventana del programa), o pulsar la tecla F3. Si tenemos
alg£n documento abierto que ha sido modificado pero no ha sido grabado,
nos preguntar  si deseamos grabarlo antes de salir. El proceso de salida
puede abortarse en cualquiera de estas preguntas de confirmaci¢n.

:h2 res=60002.Lectura y grabaci¢n de documetos
:hp4.Lectura:ehp4.
:p.

Se pueden cargar documentos al procesador al arrancarlo o mas tarde.
Por arrastrar y dejar caer objetos documentos por encima del objeto 
del procesador se arranca una copia del procesador lo cual contendr 
todos los documentos en ventanas separadas.
Tambi‚n, por ejecutar el procesador directamente de la l¡nea de mandatos.
Si uno atente hacer cualquier de las dos operaciones despu‚s que se haya
cargado una copia del procesador ya, entonces esa copia ser  dirigida
a abrir y mostrar los documentos nuevos tambi‚n.  Solo una copia del
procesador se guarda en la memoria del sistema a la vez.
:p.
Una vez que se haya arrancado el procesador, se puede seleccionar ficheros
para cargarse con la opci¢n de men£ &apos.:link reftype=hd res=IDM_OPEN.Abrir:elink.&apos..
Se presenta el di logo normal del sistema para seleccionar ficheros.
El di logo estar  fijado ya para mostrar todo los ficheros que parecen documentos,
seg£n los tipos y extensiones del los ficheros, pero se puede seleccionar
&apos.Todo Fichero (All Files)&apos. y entonces todos los ficheros se mostrar n.
Se puede seleccionar tantos ficheros como uno quiera para cargarse, y una vez
que uno oprime la tecla &apos.Entrar&apos. o haga un click sobre el bot¢n de
abrir, todos los ficheros indicados se cargar n en el fondo.
Al comienzo de cargarlos, todos las ventanas documentales actualmente abiertos
ser n iconizados, y el primer documento en completar de cargar llega a ser
el documento corriente.  Todos los otros ser n iconizados autom ticamente
al terminarse de cargar.
:p.
Despu‚s que uno remite una lista de ficheros para cargarse, el procesador determinar 
el tipo de cada fichero.  Si el contenido de un fichero parece v lido para mas que
un tipo de fichero, entonces se presentar  la lista de tipos aparentemente v lidos
para que el usuario seleccione el tipo correcto.  Si no se puede verificar el
contenido del fichero contra ninguno de los tipos definidos, entonces habr  una
notificaci¢n de esto y el documento no ser  cargado.
:p.
:hp4.Grabaci¢n:ehp4.
:p.
Hay tres maneras para grabar un documento al disco&colon.
:link reftype=hd res=IDM_SAVE.Grabar:elink.,
:link reftype=hd res=IDM_SAVEAS.Grabar como...:elink. y
:link reftype=hd res=IDM_SAVE_CLOSE.Grabar y cerrar:elink..
La opci¢n de Grabar atenta grabar el documento con su nombre y tipo actual.
Si no se puede, se dar  la posibilidad de cambiar el nombre o el
tipo del fichero para poderse grabar el documento.
La opci¢n de Grabar Como supone que uno quiere cambiar el nombre
o el tipo del fichero, y permite que uno cambie a un nombre nuevo
o seleccione un tipo nuevo.
La opci¢n de Grabar y Cerrar funciona igual como la opci¢n de Grabar,
salvo que se cerrar  la ventana del documento autom ticamente una vez
que se termine la grabaci¢n.
:p.
Si cerramos una ventana de documento y este ha sido cambiado y no
grabado, nos preguntar  antes de cerrarla si deseamos grabar el
docuemto. De la misma forma, si salimos del procesador de textos
mientras mantenemos cargados documentos que han sido modificados,
seremos interrogados sobre si queremos grabar estos documentos antes de
salir. La pregunta se realizar  por cada uno de los documentos cargados
y modificados.

:h2 res=60003.Editando documentos
:hp4.Navegando:ehp4.
:p.
Para movernos dentro de la ventana del documento, las siguientes teclas
han sido definidas&colon.
:table cols='20 60' rules=both frame=box.
:row.:c.Cursor izquierda        :c.Ir al car cter/se¤al previo.
:row.:c.Cursor derecha          :c.Ir al pr¢ximo car cter/se¤al.
:row.:c.Cursor arriba           :c.Ir a la l¡nea anterior.
:row.:c.Cursor abajo            :c.Ir a la pr¢xima l¡nea.
:row.:c.Re P g (PgUp)           :c.Ir a la p gina anterior.
:row.:c.Av P g (PgDn)           :c.Ir a la pr¢xima p gina.
:row.:c.Inicio (Home)           :c.Ir al comienzo de la l¡nea.
:row.:c.Fin (End)               :c.Ir al fin de la l¡nea.
:row.:c.Ctrl + Cursor izquierda :c.Ir a la palabra anterior.
:row.:c.Ctrl + Cursor derecha   :c.Ir a la pr¢xima palabra.
:row.:c.Ctrl + Re P g           :c.Ir al comienzo de la p gina actual.
:row.:c.Ctrl + Av P g           :c.Ir al fin de la p gina actual.
:row.:c.Ctrl + Inicio           :c.Ir al comienzo del documento.
:row.:c.Ctrl + Final            :c.Ir al fin del documento.
:etable.

Adicionalmente, podemos utilizar el mouse para posicionar el cursor
directamente en cualquier parte visible del documento. Tambi‚n podemos
utilizar el mouse con las barras de desplazamiento (en el supuesto de
que estas sean visibles) para reposicionar la p gina horizontal y
verticalmente.

:p.Adem s existen cuatro botones especiales en el extremo inferior derecha
de la ventana del documento. Estas botones realizan las funciones de ir
al inicio del documento, una p gina arriba, una p gina abajo y al final
del documento (las mismas funciones que realizan las teclas Ctrl+Inicio,
Re P g, Av P g y Ctrl+Final, respectivamente).

:p.A medida que te muevas por el documento, recuerda que los c¢digos de
funci¢n para establecer los m rgenes, fuentes, tama¤os, etc... son
considerados como se¤ales completas en el conjunto de datos y que el
cursor se detendr  en cada uno de ellos cuando se utilizen las teclas de
cursor derecha e izquierda para ir hacia delante o hacia atras en el
documento.

:note.Hay una diferencia especial en cuanto a las teclas de Inicio y
de Fin, en comparasi¢n a las normas de la industria.
Si uno aprete la tecla de Inicio cuando ya est  al inicio de la
l¡nea actual, entonces ser  movido al comienzo de la l¡nea anterior.
Si uno aprete la tecla de Fin cuando ya est  al fin de la l¡nea
actual, entonces ser  movido al fin de la l¡nea pr¢xima.

:p.:hp4.Texto:ehp4.

:p.Se puede introducir texto en el documento simplemente escribiendolo. A
medida que vayamos introduciendo texto, este ser  autom ticamente
formateado. Si una palabra es demasiado grande para poder contenerla en
la l¡nea actual, se traslada a la siguiente l¡nea. Si escribes muy
r pida, las letras introducidas son acumuladas e insertadas en grupos de
forma que se ponga al d¡a simult neamente a tu velocidad.

:p.Una vez entrado texto en el documento, podemos retroceder el cursor para
cambiarlo. Por defecto el cursor est  en modo de sobreescritura y
utiliza una barra vertical como se¤al. Cuando se empieza a escribir en
medio de un texto preexistente, el nuevo texto se inserta sobre el
antiguo y el documento es reformateado. Si lo que deseamos es sustituir
el texto, debemos pulsar antes la tecla INSERT. El cursor cambiar  a un
bloque que contendr  el car cter/se¤al actual. Hay que se prudentes en
este modo: si el cursor est  situado sobre una se¤al de fuente o de
tama¤o, o cualquier otra se¤al especial de c¢digo, cualquier cosa que se
escriba la sustituir , como si se tratara de texto normal.

:p.Se puede seleccionar texto posicionando el cursor al inicio del bloque
que se desea seleccionar y a continuaci¢n, manteniendo pulsada la tecla
SHIFT, posicionar el cursor al final del bloque. Otra forma es
posicionar el cursor con el mouse y manteniendo apretado el bot¢n
izquierdo del mismo ir hasta el final del bloque.  Una sola palabra o
gr fico se puede seleccionar por pulsar dos veces r pidamente al bot¢n
izquierdo del mouse por encima del objeto, y todo el documento se puede
seleccionar a la vez por pulsar dos veces r pidamente al bot¢n izquierdo
del mouse por encima de la ventanita vac¡o que se encuentra entre las reglas
horizontales y verticales, a la esquina por arriba y izquierda de la ventana
del documento.

:p.Una vez seleccionado el bloque de texto, puede moverse o copiar, borrarlo
del documento, copiar al clipboard (portapapeles o  rea com£n) del sistema,
o sustituirlo con lo que est‚ en el clipboard. Tambi‚n, si tenemos texto
selecci¢n y empezamos a escribir, el bloque seleccionado se borra y se
sustituye con lo que estemos escribiendo.

:p.El texto seleccionado puede moverse o copiarse con el mouse. Debemos
pulsar el bot¢n derecho del mouse en la  rea seleccionada y arrastrarlo
hasta la nueva posici¢n y soltar el bot¢n del mouse. En el momento que
liberemos el bot¢n del mouse, el texto se mover . Si lo que deseamos es
copiarlo en vez de moverlo, hemos de pulsar la tecla CONTROL en el
momento de soltar el bot¢n del mouse. Si lo movemos el mouse y s¢lo
liberemos el bot¢n del mouse, aparecer  un men£ con todas las funciones
que pueden realizarse en el bloque de texto seleccionado.
:p.
:hp4.Atributos del texto:ehp4.
:p.
En cualquier punto del documento est  en efecto un estilo de texto y
unos atributos de formateo determinados. Los m rgenes est n en una
posici¢n determinada, los tabuladores est n en un punto concreto y la
fuente es de un determinado tipo. Cualquiera de estos atributos puede
ser cambiado desde la posici¢n actual hacia adelante o sobre el texto
seleccionado. Si establecemos la fuente Times Roman y no hay ning£n
bloque seleccionado, se utilizar  la nueva fuente a partir de este punto
hasta que se indique el uso de otra fuente o hasta el final del
documento. Si existe un bloque seleccionado, en cambio, s¢lo afectar  al
citado bloque, cambiando todo ‚l a Times Roman en todo el bloque. Fuera
del bloque, permancer n activas todas las fuentes.  Todos los atributos
de texto son tratados de la misma forma: m rgenes, tabuladores,
indentaci¢n, fuente, tama¤o y estilo de la fuente, colores, etc...
:p.
:hp4.Im genes gr ficas:ehp4.
:p.
Im genes gr ficas se pueden colocar dentro de un documento a trav‚s
del &apos.clipboard&apos. del sistema, de cualquiera porci¢n de la pantalla
visible, o de ficheros gr ficos.  Se usa el men£
:link reftype=hd res=IDM_PASTE_FROM.Pegar de:elink. para hacer esto.
:p.
Una vez pegado el gr fico en el documento, podemos moverlo, copiarlo o
redimensionarlo en cualquier momento. Para mover el gr fico, hemos de
pulsar el bot¢n derecho del mouse en el cuerpo del gr fico y arrastrarlo
hasta la nueva posici¢n. Para copiarlo, es la misma operaci¢n excepto
que debemos pulsar la tecla CONTROL en el momento de soltar el bot¢n del
mouse. Para redimensionar el gr fico, hemos de hacer un click con el
mouse en el margen en blanco del gr fico, de forma que aparezca un
recuadro/gu¡a para el redimensionado y ajuste del gr fico.

:h2 res=60004.Imprimiendo documentos
Existen, por ahora, dos opciones de impresi¢n de los documentos:
:link reftype=hd res=IDM_PRINT_ALL.Imprimir todo:elink. y
:link reftype=hd res=IDM_PRINT_PAGE.Imprimir p gina:elink..
:p.
Imprimir todo imprime la totalidad del documento. Una forma de acceder a
esta funci¢n es pulsar la tecla CONTROL y la "I".
:p.
Imprimir p gina enviar  la p gina actual del documento activo a la
impresora. Esto tambi‚n puede realizarse pulsando la tecla ALT y la "I".

:h2 res=60005.Gesti¢n de ventanas
Es posible tener varias ventanas de documentos abiertas simult neamente.
Cada vez que se abre una nueva ventana el resto de las ventanas activas
son iconizadas y ordenadas en la parte inferior del  rea de trabajo. La
nueva ventana de documento ocupar  pues el m ximo de espacio que pueda,
dejando suficiente espacio para que todos los documentos iconizados sean
visibles.
:p.
Para restaurar una ventana a su tama¤o normal, hacer un doble-click
sobre ella tal como hacemos en cualquier icono del sistema. La ventana
volver  a la misma posici¢n y con el mismo tama¤o que tenia en el
momento de ser iconizada.
:p.
Si se restauran varios documentos a su posici¢n normal, es muy probable
que entre ellos se oculten diferentes partes. Existen dos opciones para
solucionar este problema:
:link reftype=hd res=IDM_TILE.Mosaico:elink. y
:link reftype=hd res=IDM_CASCADE.Cascada:elink..
La primera opci¢n ordenar  todas los documentos que no esten iconizados
de forma que no se oculten entre ellas.
La segunda opci¢n ordenar  los documentos no iconizados de forma que
formen una pila (de forma similar a una baraja de cartas), de forma que
todas las barras de t¡tulo permanezcan visibles quedando la ventana
activa en la parte superior de la pila.
:p.
Existe una funci¢n adicional de control de las ventanas:
:link reftype=hd res=IDM_ARRANGEICONS.Ordenar iconos:elink.
Esta opci¢n ordenar  todos los documentos iconizados, de forma que
queden en la parte inferior del  rea de trabajo distribuidos
proporcionalmente.
:p.
En el caso de que un documento est‚ complementamente escondidom el men£
:link reftype=hd res=IDM_WINDOW_MENU.Ventana:elink. contiene unalista de todos
los documentos abiertos. Podemos seleccionar cualquiera de los
documentos y convertirlo en la ventana activa.

:h2 res=60006.Algunas notas
&sqbul.Si se dispone de un gran n£mero de fuentes hay que tener
cuidado. Al acceder al men£ de Fuentes, se consume una memoria que el
sistema operativo no liberar  hasta el momento de realizar un shutdown.
Como consecuencia, el fichero de intercambio puede crear unos cuantos
megabytes.
:p.
&sqbul.Algunas fuentes ATM disponibles en BBS no est n completamente
verificadas y pueden llegar a causar un cuelge del sistema. Se han de
tomar precauciones con las fuentes recien instaladas (no hay que tener
ninguna preocupaci¢n con las fuentes adquiridas a IBM o Adobe).
:p.
&sqbul.Los gr ficos insertados en los documentos pueden ser de gran
tama¤o. Cuando se inserta un gr fico de gran tama¤o en un documento y
se reduce su tama¤o, no debemos pensar que el gr fico ocupar  menos
tama¤o: continuar  ocupando el mismo.

:h2 res=60007.Registro
Este programa es del tipo &apos.shareware&apos..  Sientese libre de distribuir copias
del programa a quien est‚ interesado.
Si Ud. encuentre al programa £til, y decida retenerlo en uso, entonces hay que
pagar por ello.
El precio es US$30.00, y hay varias maneras para pagarlo&colon.
:p.
:hp4.Cheque o Giro Bancario&colon.:ehp4.
Si Ud. desee pagar con un cheque personal o giro bancario (o postal), entonces
por favor envie su pago, junto con su nombre, su direcci¢n postal, el n£mero de
la revisi¢n del programa que est  usando y la fecha de su copia actual de Escriba,
a&colon.
:lm margin=6.:hp4.:sl compact.
:li.Registraci¢n "Escriba"
:li.c/o Rick Papo
:li.38290 Avondale
:li.Westland, MI 48186-3830
:li.USA / EEUU
:esl.:ehp4.:lm margin=1.
.br
:hp4.Tarjeta Bancario &apos.VISA&apos., &apos.MasterCard&apos. o &apos.Discover&apos.&colon.:ehp4.
Si Ud. desee usar un tarjeta de cr‚dito para pagar la registraci¢n, la
compa¤ia "BMT Micro" est  proviendo este servicio para Escriba.
Se puede contactarles por tel‚fono al n£mero (800) 414-4268 (dentro de los
EEUU), o al n£mero (910) 791-7052, o por FAX al n£mero (910) 350-2937,
o por medio de su BBS al n£mero (910) 350-8061.  Las l¡neas de voz trabajan
desde las ocho de la ma¤ana hasta las ocho de la noche (EST,-5 GMT).
Los otros n£meros funcionan veinticuatro horas al d¡a, siete d¡as a la semana.
Adem s, su direcci¢n postal es&colon.
:lm margin=6.:hp4.:sl compact.
:li.BMT Micro
:li.452 Horn Rd
:li.Wilmington, NC 28412-2411
:li.USA / EEUU
:esl.:ehp4.:lm margin=1.
:p.Al llegar su registraci¢n, envio un recibo y la revisi¢n m s £ltima del
programa en un diskette.  Tambi‚n se le enviar  otra copia en diskette mas
tarde, cuando la pr¢xima revisi¢n mayor se aliste.
Documentaci¢n imprimida todav¡a no existe, pero una vez que haya, todos
los usuarios registrados del programa recibir n una copia.
Si uno quiera revisiones de mas en diskette, costar n US$5.00.

:h2 res=60008.Preguntas y Reportar Problemas
Si Ud. tenga problemas con este programa, el autor (Rick Papo) se puede
contactar por una de las siguientes maneras&colon.
:table cols='15 60' rules=both frame=box.
:row.:c.Internet        :c.:hp4.rpapo@msen.com:ehp4.
:row.:c.Correo          :c.:hp4.Rick Papo, 38290 Avondale, Westland, MI 48186-3830, USA / EEUU:ehp4.
:row.:c.Tel‚fono        :c.:hp4.USA / EEUU (734) 326-8547:ehp4.
:etable.
Primera prioridad se da a preguntas y problemas de usuarios registrados
(v‚ase :link reftype=hd res=60007.Registro:elink.), pero un esfuerzo se
hace para tratar todos los mensajes que lleguen.  Correo electr¢nico
se chequea diariamente.
:p.
En toda correspondencia, por favor provea el n£mero de revisi¢n de Escriba
que Ud. esta usando y al cual esta refiri‚ndose.

:h1 res=IDM_FILE_MENU.Men£ Archivo
:i1.Men£ Archivo
En el men£ Archivo est n disponible las siguientes funciones&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_NEW.Nuevo:elink.
:li.&sqbul.:link reftype=hd res=IDM_OPEN.Abrir...:elink.
:li.&sqbul.:link reftype=hd res=IDM_RECALL.Recuperar:elink.
:li.&sqbul.:link reftype=hd res=IDM_MERGE.Unir...:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVE.Grabar:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVEAS.Grabar como...:elink.
:li.&sqbul.:link reftype=hd res=IDM_SAVE_CLOSE.Grabar y cerrar:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLOSE.Cerrar:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT.Imprimir:elink.
:li.&sqbul.:link reftype=hd res=IDM_PREFERENCES.Configuraci¢n general...:elink.
:li.&sqbul.:link reftype=hd res=IDM_DEFAULTS.Valores por defecto...:elink.
:li.&sqbul.:link reftype=hd res=IDM_CONFIGURE_EXTENSIONS.Configurar extensiones...:elink.
:li.&sqbul.:link reftype=hd res=IDM_LANGUAGE_MENU.Idioma:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXIT.Salir:elink.
:esl.

:h2 res=IDM_NEW.Nuevo (Opci¢n del men£)
Esta opci¢n crea un nuevo documento con nada en su interior. Los
m rgenes, fuente y otros atributos ser n los indicados en la caja de
di logo "Valores por defecto".
:p.
Cualquier ventana previamente abierta ser  minimizada en la parte
inferior del  rea de trabajo y la nueva ventana ocupar  tanto espacio de
trabajo como pueda.

:h2 res=IDM_OPEN.Abrir... (Opci¢n del men£)
Cuando se selecciona esta opci¢n del men£, aparecer  el di logo
est ndard del sistema de selecci¢n de ficheros. Se pueden seleccionar
uno o m s documentos a abrir. Una vez pulsada la tecla ENTER (o
pulsado el bot¢n OK), todos los ficheros seleccionados ser n
abiertos. En la parte derecha de la barra de t¡tulo del programa
se mostrar  un indicador rojo indicando que se est  realizando la
lectura. Cada carga, una vez finalizada, generar  una ventana de
documento separada. El primero en ser cargado pasar  a ser el
documento activo, y permanecer  maximizado en el  rea cliente del
programa. Los otros documentos ser n iconizados, y podremos
acceder a ellos a trav‚s del men£ Ventana o bien realizando un
doble-click con el mouse en sus respectivos iconos.
:p.La tecla aceleradora de esta funci¢n es Ctrl+A.
:p.Ver :link reftype=hd res=256.Abrir/Unir/Grabar como... (Di logos):elink. 
para m s informaci¢n

:h3 res=256.Abrir/Unir/Grabar como... (Di logos)
Si seleccionamos las opciones del men£ Abrir, Unir o Grabar como
aparecer  el di logo est ndard de ficheros del sistema operativo.
En el deberemos seguir las reglas indicadas en la documentaci¢n
del sistema. 
:artwork name='Espanol\FileDlg.bmp' align=center.
:p.Estas notas son de especial inter‚s&colon.
:p.
&rarrow.Al mostrarse el di logo para 'Abrir' o 'Unir', el control para
el tipo de fichero se fija para todos tipos de documentos.
Todos los ficheros que parecen documentos v lidos para Escriba, o sea
su tipo nativo o cualquier de los los tipos que se pueden importar (como
Texto Simple, BMP, GIF, JPG, PCX, TIF, etc), se muestran en la lista de
ficheros.  Se puede cambiar este control para mostrar todos los ficheros
no importa el tipo, o para mostrar solo los ficheros de un solo tipo.
:p.
&rarrow.Al mostrarse este dialogo para 'Grabar' o para 'Grabar Como',
el control para el tipo de fichero se fija al tipo actual del fichero que
est  para grabarse.  Si uno quiere cambiar el tipo del fichero, hay que
usar este control para seleccionar el tipo para usar el grabar el documento.
:p.
&rarrow.Cuando se seleccionan ficheros para abrir, es posible marcar
tantos ficheros, en la lista de ficheros, como se deseen.
:p.
&rarrow. Cuando se selecciona un fichero para unir, s¢lo es posible
marcar un fichero.
:p.
&rarrow. Cuando se utiliza la funci¢n "Grabar como", no es posible
marcar un fichero en la lista, sino que esta s¢lo se muestra como
referencia. El usuario debe entrar el nombre del fichero en el
que desea grabar el documento.
:p.
&rarrow. El directorio seleccionado a trav‚s de este di logo se
convierte en el directorio actual del programa. La pr¢xima vez
que se acceda a la funci¢n Abrir, Unir o Grabar como, la lista de
ficheros mostrar  el contenido de este directorio.

:note.Los ficheros son marcados como documentos de Escriba en el
momento en que son grabados en el disco. Esto se realiza a trav‚s
de los atributos extendidos, por lo que esta informaci¢n puede
perderse cuando se graba el documento en un archivo utilizando un
programa (como el PKZIP) que no reconozca los atributos
extendidos. Los programas m s nuevos de compresi¢n ya preservan
esta informaci¢n.

:h4 res=IDD_FILETYPE.Seleccionar tipo de fichero (Di logo)
Este di logo se presenta cuando uno trate de cargar un fichero
y el fichero se encuentra acepta para mas que un tipo de fichero.
Se presenta el nombre del fichero y sus tipos aceptables, y uno
tiene que elegir el tipo correcto de la lista.  Una vez que uno
haya seleccionado el tipo correcto de fichero, entonces hay que
hacer un click sobre el bot¢n de OK o oprimir la tecla ENTER.
Para escapar y no cargar el fichero, entonces hay que hacer un
click sobre el bot¢n de cancelar u oprimir la tecla ESC.

:h2 res=IDM_RECALL.Menu Recuperar
Esta opci¢n de men£ trae un submen£ en lo cual los diez documentos
grabados est n listados.
:p.V‚ase :link reftype=hd res=IDM_RECALL0.Entradas del Menu Recuperar:elink.
para mas informaci¢n.

:h3 res=IDM_RECALL0.Entradas del Menu Recuperar
Esta entrada de men£ muestra uno de los diez documentos grabados o
cerrados mas reci‚n por la procesadora.  Para abrir uno de estos
documentos de nuevo como una ventana nueva, selecci¢nelo con las
teclas flechadas y pulse ENTER, o haga un click a la entrada con 
el rat¢n.

:h2 res=IDM_MERGE.Unir... (Opci¢n del men£)
Cuando se selecciona esta opci¢n del men£, aparecer  el di logo
est ndard de selecci¢n de ficheros. En este di logo podemos
seleccionar un fichero que deseamos unir al documento activo en
la posici¢n del cursor.
:p.V‚ase :link reftype=hd res=256.Abrir/Unir/Grabar como... (Di logos):elink.
para m s informaci¢n.

:h2 res=IDM_SAVE.Grabar (Opci¢n del men£)
Esta orden graba el contenido del documento activo en el fichero
del disco. En el caso de que no se haya realizado ning£n cambio
en el documento, esta opci¢n est  deshabilitada.
:p.La tecla aceleradora de esta funci¢n es F2.

:h2 res=IDM_SAVEAS.Grabar como... (Opci¢n del men£)
Cuando se selecciona esta opci¢n del men£, aparecer  el di logo
est ndard de selecci¢n de ficheros. Utilizando este di logo,
podr s entrar el nombre del fichero en el que quieres grabar el
documento activo, as¡ como el directorio en el que se desea
grabar.
:p.Ver :link reftype=hd res=256.Abrir/Unir/Grabar como... (Di logos):elink. 
para m s informaci¢n.

:h2 res=IDM_SAVE_CLOSE.Grabar y cerrar (Opci¢n del men£)
Este es un m‚todo r pido de grabar y cerrar el documento activo,
con una s¢la selecci¢n del men£.
:p.La tecla aceleradora de esta funci¢n es F4.

:h2 res=IDM_CLOSE.Cerrar (Opci¢n del men£)
Esta orden cerrar  el documento activo. Si existieran cambios y
no se hubieran grabado, se preguntar  al usuario si desea
grabarlos. Si escoge la opci¢n SI, el documento se grabar  con el
nombre actual.
Si escoge la opci¢n NO, el documento se cerrar  sin grabar su
contenido.
Si escoge la opci¢n CANCELAR o pulsa la tecla ESCAPE, la funci¢n
de cerrar el documento no se realizar  y la ventana continuar 
activa.

:h2 res=IDM_PRINT.Imprimir (Submen£)
A trav‚s de este submen£ se acceden a las diferentes funciones de
impresi¢n. Por ahora hay disponible dos ¢rdenes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL.Imprimir todo:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_PAGE.Imprimir p gina:elink.
:esl.

:h3 res=IDM_PRINT_ALL.Menu de imprimir todo
Este submen£ da aceso a las diferentes maneras de imprimir un
documento entero.  Las siguientes opciones est n disponibles&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL_NORMAL.Imprimir todo normal:elink.
:li.&sqbul.:link reftype=hd res=IDM_PRINT_ALL_REVERSE.Imprimir todo a rev‚s:elink.
:esl.

:h4 res=IDM_PRINT_ALL_NORMAL.Imprimir todo normal (Opci¢n del men£)
Despu‚s que uno seleccione esta opci¢n, el documento activo se imprime.
Una ventanita roja se muestra mientras el documento est  siendo enviado
al &apos.spooler&apos. del sistema, pero esto es para informaci¢n nada mas.
Uno ya puede volver a trabajar sobre el documento de una vez despu‚s de
usar esta opci¢n.  El documento se imprimir  tal como estuvo al instante
que la impresi¢n fue pedido.
:p.La tecla aceleradora para esta funci¢n es Ctrl+I.

:h4 res=IDM_PRINT_ALL_REVERSE.Imprimir todo a rev‚s (Opci¢n del men£)
Despu‚s que uno seleccione esta opci¢n, el documento activo se imprime,
igual como en :link reftype=hd res=IDM_PRINT_ALL_NORMAL.imprimir todo normal:elink.,
salvo que las p ginas se imprimir n en orden a rev‚s, la £ltima p gina primera.
Esta opci¢n es para usar con impresoras como el DeskJet, los cuales
hacen amontonar las p ginas cara arriba al salir.

:h3 res=IDM_PRINT_PAGE.Imrpimir p gina (Opci¢n del men£)
Despu‚s de seleccionar esta opci¢n del men£ se imprimir 
£nicamente la p gina activa del documento actual. Un indicador
rojo aparecer  en la ventana mientras se est  enviando el
documento al spooler de la impresora, aunque este es un mensaje
informativo ya que puedes continuar trabajando en el documento
inmediatamente despu‚s de seleccionar esta orden. El documento
ser  imprimido tal como estaba en el momento de seleccionar la
orden de impresi¢n.
:p.La tecla aceleradora de esta orden es ALT+I.

:h2 res=IDM_PREFERENCES.Configuraci¢n general... (Opci¢n del men£)
:p.Ver :link reftype=hd res=IDD_PREFERENCES.Configuraci¢n general (Di logo):elink. 
para m s informaci¢n.

:h3 res=IDD_PREFERENCES.Configuraci¢n general (Di logo)
A trav‚s de este di logo es posible configurar el aspecto y
comportamiento de Escriba en lo que respecta al procesador de
textos como programa y no a como se deben procesar. 
:artwork name='Espanol\Prefer.bmp' align=center.
:p.Las funciones actualmente disponibles son&colon.
:p.:hp2.Animaci¢n:ehp2.
Si esta opci¢n esta marcada, se animar n los procesos de abrir y
cerrar la ventana principal del programa, tal como hacen los
folders (carpetas) del sistema. De la misma forma que con los
folders, si la animaci¢n ha de estar habilitada a nivel de
sistema (a trav‚s del objeto "System" de la carpeta "System Setup").
:p.:hp2.Mostrar reglas:ehp2.
Si esta opci¢n est  marcada, todas las ventanas de documentos
tendr n reglas horizontales y verticales al lado mismo de la
ventana del documento.
:p.:hp2.Mostrar barras de desplazamiento:ehp2.
Si esta opci¢n est  marcada, todas las ventanas de documentos
incluir n barras de desplazamiento horizontal y vertical, que
podr n utilizarse con el mouse para desplazar el texto del
documento.
:p.:hp2.Mostrar l¡nea de estatus:ehp2.
Si esta opci¢n est  marcada, todas las ventanas de documentos
incluir n una l¡nea de estatus por abajo, donde se mostrar 
la posici¢n actual del cursor y una interpretaci¢n del elemento
de texto en el documento donde est  el cursor.
:p.:hp2.Mostrar nombres completos:ehp2.
Si esta opci¢n esta marcada, en la barra de t¡tulo de las
ventanas de documentos se mostrar  el nombre completo del fichero
de cada documento. Si no lo est , no se mostrar  la unidad de
disco y el directorio.
:p.:hp2.Imprimir p ginas separadamente:ehp2.
Si esta opci¢n est  marcada, entonces al imprimirse un documento de
varias p ginas las p ginas se enviar n al &apos.spooler&apos. como tareas
separadas.
Como tareas del para imprimirse no comienzen a imprimirse hasta que est‚n
completamente listos, esta opci¢n permite que documentos de varias
p ginas puedan comenzar a imprimirse mas r pidamente.
A la otra mano, si su impresora est  compartida por una red,
las p ginas se pueden imprimir fuera de orden o mezcladas con las
tareas de otros usuarios.
:p.:hp2.Imprimir p ginas a rev‚s:ehp2.
Si esta opci¢n est  marcada, entonces la manera normal de imprimir
p ginas multiples ser  imprimir la £ltima p gina primera.  Esta opci¢n
se provea para la conveniencia de usuarios cuyos impresoras son tales
como el 'DeskJet', donde las p ginas est n acumuladas cara arriba, y
por eso a rev‚s.
:p.:hp2.Recargar documento al arrancar:ehp2.
Si esta opci¢n est  marcada, entonces el documento grabado mas £ltimamente
en la lista de Recordar ser  recargado autom ticamente cuando Escriba se
arranque.
:p.:hp2.Zoom Inicial:ehp2.
De este grupo de botones se puede seleccionar el zoom de la ventana
inicial de documentos.  Cuando se abren ventanas nuevas para documentos
o se abren documentos del fichero, el m‚todo eligido de zoom se
usar  para desplegar la ventana la primera vez.  Se puede eligir
de cualquier de estos tipos de zoom&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_100.Zoom 100&percent.:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLWIDTH.Zoom Anchura:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLHEIGHT.Zoom Altura:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_MARGINS.Zoom M rgenes:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_ALL.Zoom Completo:elink.
:esl.
:p.
Una vez que estos valores de configuraci¢n hayan sido
convenientemente ajustados, pulsar la tecla ENTER o realizar un
click en el bot¢n "OK" para continuar la ejecuci¢n del programa.
Los cambios afectar n a todos los documentos nuevos que se creen.
:p.
Si deseas abortar el di logo y dejar los valores tal como
estaban, pulsar la tecla ESC o bien hacer un click en el bot¢n de
anulaci¢n.

:h2 res=IDM_DEFAULTS.Valores por defecto... (Opci¢n del men£)
Esta orden invoca un di logo en el que es posible cambiar la
impresora, tama¤o de la hoja, fuente, tama¤o de punto, color,
atributos de fuentes y justificaci¢n del texto (horizontal y
vertical) del documento.
:p.Ver :link reftype=hd res=IDD_DEFAULTS.Valores por defecto (Di logo):elink. 
para m s informaci¢n.

:h3 res=IDD_DEFAULTS.Valores por defecto (Di logo)
A trav‚s de este di logo es posible cambiar los atributos
iniciales del programa. Estos atributos afectan al documento
activo o, en el caso de que no exista ning£n documento abierto, a
todos los documentos que se creen a continuaci¢n. 
.br
Las atributos est n agrupados seg£n su categor¡a
en las siguientes p ginas&colon.
:sl compact.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_AUTHOR.Autor:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_PRINTER.Impresora:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_FONT.Fuente:elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_LAYOUT1.Disposici¢n (1):elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_LAYOUT2.Disposici¢n (2):elink.
:li.:link reftype=hd res=IDD_DEFAULTS_PAGE_NATIONAL.Nacional:elink.
:esl.
:p.
Una vez que se hayan modificado los valores a nuestro gusto,
pulsando la tecla ENTER o realizando un click en el bot¢n OK el
programa continuar  su ejecuci¢n. Si no hay ning£n documento
abierto, o hemos marcado la opci¢n "Grabaci¢n global" estos
nuevos valores afectar n a todos los documentos que se creen a
partir de ahora. Si existe alg£n documento abierto, los valores
por defecto tendr n efecto s¢lo en este documento, sin afectar al
resto de los documentos.
:p.
Si se pulsa el bot¢n APLICAR, los nuevos valores por defecto
afectan al documento en seguida, conviertiendose en los nuevos
valores por defecto.
:p.
Si deseamos abortar el di logo y dejar los valores tal como
estaban, pulsar la tecla ESCAPE o bien el bot¢n Cancelar. Los
cambios realizados por el bot¢n APLICAR no son descartados.

:h4 res=IDD_DEFAULTS_PAGE_AUTHOR.Autor
:artwork name='Espanol\Default0.bmp' align=center.
:p.:hp2.Autor:ehp2.
Este campo le permite a uno ver y/o cambiar el nombre del
autor y el t¡tulo del documento que est n grabados en el prefacio del documento.
Tambi‚n muestra la fecha de creaci¢n del documento.

:h4 res=IDD_DEFAULTS_PAGE_PRINTER.Impresora
:artwork name='Espanol\Default1.bmp' align=center.
:p.:hp2.Impresora:ehp2.
Permite seleccionar la impresora (en el caso de disponer de
varias) en el que se imprimir  el documento. Se puede seleccionar
cualquier impresora definida en el sistema.
:p.:hp2.Configuraci¢n impresora:ehp2.
Seleccionando este bot¢n se activar  la caja de di logo de
configuraci¢n de la impresora actual. Este di logo cambia
dependiendo del tipo de impresora, por lo que no puede ser
documentado aqu¡. El usuario deber  guiarse a trav‚s de la
informaci¢n facilitada por el propio di logo. Al salir de este
di logo, el tama¤o de hoja y el tipo de hoja ser n
autom ticamente actualizados.

:h4 res=IDD_DEFAULTS_PAGE_FONT.Fuente
:artwork name='Espanol\Default2.bmp' align=center.
:p.:hp2.Fuente:ehp2.
Est  ser  la fuente (tipo de letra) inicial del documento. Es
posible seleccionar cualquier duente instalada en el sistema.
:p.:hp2.Tama¤o:ehp2.
Este ser  el tama¤o inicial de fuente del documento. Es posible
seleccionar cualquiera de los tama¤os que apareceran en la lista.
:p.:hp2.Color:ehp2.
Este ser  el color inicial que utilizar  el texto del documento.
Podemos seleccionar cualquiera de los colores que apareceran en
la lista.
:p.:hp2.Negrita:ehp2.
Si esta opci¢n est  marcada, el texto inicial utilizar  el
atributo de "negrita" (resaltado). Si la fuente dispone de un
tipo especial en negrita se utilizar  este. En caso contrario, el
sistema construir  la fuente negrita adecuada.
:p.:hp2.Cursiva:ehp2.
Si esta opci¢n est  marcada, el texto inicial utilizar  el
atributo de cursiva. Si la fuente seleccionada dispone de un tipo
especial en cursiva se utilizar  este. En caso contrario, el
sistema construir  la fuente cursiva adecuada.
:p.:hp2.Subrayado:ehp2.
Si esta opci¢n est  marcada, el texto inicial estar  subrayado.
:p.:hp2.Tachado:ehp2.
Si esta opci¢n est  marcada, el texto inicial estar  tachado.
:p.:hp2.Vacio:ehp2.
Si esta opci¢n est  marcada, el texto inicial estar  "vacio" (el
contorno de la letra sin nada en su interior).
:p.:hp2.May£sculas:ehp2.
Si esta opci¢n est  marcada, el texto inicial ser  capitalizado.

:h4 res=IDD_DEFAULTS_PAGE_LAYOUT1.Disposici¢n (1)
:artwork name='Espanol\Default3.bmp' align=center.
:p.:hp2.Justificaci¢n:ehp2.
Define el tipo de justificaci¢n horizontal que se utilizar :
izquierda, derecha, centrar o total.
La justificaci¢n izquierda alinea el texto al margen izquierdo de
la p gina.
La justificaci¢n centrada har  que el texto quede centrado entre
los m rgenes derecho e izquierdo, aunque no alterar  el espaciado
entre las palabras.
La justificaci¢n derecha alinea el texto al margen derecho de la
p gina.
La justificaci¢n total alinea el texto a ambos m rgenes de la
p gina, alterando el espaciado entre las palabras de forma
apropiada.
:p.:hp2.Centrar p gina:ehp2.
Si est  opci¢n est  marcada, el texto se centrar  autom ticamente
en la p gina.
:p.:hp2.M rgenes:ehp2.
Estos cuatro campos son utilizados para fijar los m rgenes
iniciales de la p gina, en pulgadas o cent¡metros de acuerdo con
la selecci¢n actual. Los m rgenes no podr n ser mayores que el
tama¤o de hoja seleccionado.

:h4 res=IDD_DEFAULTS_PAGE_LAYOUT2.Disposici¢n (2)
:artwork name='Espanol\Default4.bmp' align=center.
:p.:hp2.Tabuladores:ehp2.
Este campo permite definir la distancia por defecto que avanzar 
la tecla del tabulador. Si se introduce 0.500 significar  que
inicialmente se fijaran se¤ales de tabulaci¢n cada media pulgada
o medio cent¡metro (dependiendo de las unidades que se utilicen).
:p.:hp2.Indent:ehp2.
Este campo contiene la indentaci¢n por defecto para ser usado desde
el comienzo del documento.  Si uno fuera a entrar 0.500 ac ,
entonces encontrar¡a el texto de la primera l¡nea del documento, y de
todo texto que sigue un c¢digo del fin de l¡nea en el documento ser 
indentado una media pulgada o cent¡metro del lado izquierdo.
:p.:hp2.Espaciado:ehp2.
Este campo contiene el espaciado entre l¡neas que se utilizar 
por defecto. La opci¢n Single establece un espaciado vertical del
100&percent., la opci¢n 1 1/2 lo establece en el 150&percent. y la opci¢n Double lo
fija en el 200&percent.

:h4 res=IDD_DEFAULTS_PAGE_NATIONAL.Opciones Nacionales
:artwork name='Espanol\Default5.bmp' align=center.
:p.:hp2.P gina de c¢digos:ehp2.
Con este control se puede fijar la p gina de c¢digos (o tabla de
letras nacionales) para usarse con el documento.
V‚ase :link reftype=hd res=61002.P ginas de C¢digos:elink. para mas informaci¢n.
:p.:hp2.M‚trico:ehp2.
Si esta opci¢n est  marcada, todas las medidas se expresar n en
unidades m‚tricas. Las reglas horizontal y vertical utilizar n
centr¡metos como unidades de medida.
:p.:hp2.Formato de la fecha:ehp2.
Este campo permite cambiar el formato de la fecha que se
utilizar  con las funciones "D¡a de hoy" y "Fecha".
Ver :link reftype=hd res=IDD_DATEFMT.Formato de la fecha:elink. para
m s informaci¢n.

:h2 res=IDM_CONFIGURE_EXTENSIONS.Configurar Extensiones... (Opci¢n del men£)
Esta opci¢n del men£ muestra un di logo del cual se puede cambiar la
configuraci¢n de cualquier de las extensiones actualmente instalados
a la aplicaci¢n.
:p.V‚ase :link reftype=hd res=IDD_CONFIGURE_EXTENSIONS.Configure Extensions (Di logo):elink.
para mas informaci¢n.

:h3 res=IDD_CONFIGURE_EXTENSIONS.Configurar Extensiones (Di logo)
Este di logo le permite a uno que cambie la configuraci¢n de cualquier
de las extensiones actualmente instalados a la aplicaci¢n.
:artwork name='Espanol\ConfigEx.bmp' align=center.

:h3 res=IDM_LANGUAGE_MENU.Idioma (Submen£)
Este men£ permite que uno seleccione un idioma nuevo para todos los men£es y
di logos de la aplicaci¢n.
:p.V‚ase :link reftype=hd res=IDM_LANGUAGES.Entrada al men£ de idiomas:elink.
para mas informaci¢n.

:h4 res=IDM_LANGUAGES.Entrada al men£ de idiomas (Opci¢n del men£)
Cualquier de las entradas en el submen£ de idiomas se puede usar para
seleccionar un nuevo idioma para los men£es y di logos de la aplicaci¢n.
Los nombres de los idiomas se muestran en su forma nativo (ingl‚s es English,
por ejemplo).  Despu‚s de haberse elegido un idioma nuevo, la aplicaci¢n
entera se reformatear  para conformarse con el nuevo idioma.  Ciertos aspectos
de documentos (como los formatos de las fechas) se cambiar n tambi‚n.

:h2 res=IDM_EXIT.Salir (Opci¢n del men£)
Seleccionando esta opci¢n del men£ finalizar  el programa.

:h1 res=IDM_EDIT_MENU.Men£ Edici¢n
:i1.Men£ Edici¢n
Estas funciones est n disponibles en el men£ de edici¢n&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_UNDO.Deshacer:elink.
:li.&sqbul.:link reftype=hd res=IDM_REDO.Rehacer:elink.
:li.&sqbul.:link reftype=hd res=IDM_CUT.Cortar:elink.
:li.&sqbul.:link reftype=hd res=IDM_COPY.Copiar:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE.Pegar:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEAR.Borrar:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEARCLIPBOARD.Borrar clipboard:elink.
:li.&sqbul.:link reftype=hd res=IDM_SELECT_ALL.Seleccionar todo:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM.Menu &apos.Pegar de...&apos.:elink.
:li.&sqbul.:link reftype=hd res=IDM_INSERT_FUNCTION.Menu &apos.Insertar Funci¢n&apos.:elink.
:li.&sqbul.:link reftype=hd res=IDM_SEARCH.Buscar...:elink.
:li.&sqbul.:link reftype=hd res=IDM_FINDNEXT.Repetir busqueda:elink.
:li.&sqbul.:link reftype=hd res=IDM_GOTOPAGE.Go To Page:elink.
:li.&sqbul.:link reftype=hd res=IDM_SPELL.Chequeo de ortograf¡a...:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM.Zoom:elink.
:esl.

:h2 res=IDM_UNDO.Deshacer (Opci¢n del men£)
Esta funci¢n deshace el cambio mas £ltimo aplicado al documento.
Si uno sigue usando ‚sta funci¢n, el pr¢ximo cambio mas £ltimo
se deshar  tambi‚n, y as¡ sucesivamente hasta que todos los cambios
hechos al documento han sido deshechos, y el documento se queda tal
como estuvo al momento de ser cargado del fichero.  Con un documento
nuevo, porsupuesto, este estado final ser  el estado vac¡o inicial.
:p.La tecla aceleradora es Alt+Backspace.

:h2 res=IDM_REDO.Rehacer (Opci¢n del men£)
Si uno no haya cambiado al documento desde la £ltima vez que uno
haya deshecho un cambio, ‚sta funci¢n se har  el cambio de nuevo.
Es posible rehacer todos los cambios hechos al documento, restaurando
todos.  No obstante, si uno haga un cambio al documento, de cualquier
tipo, todos los cambios que fueron posibles restaurar antes, ya no
se pueden.
:p.La tecla aceleradora es Shift+Alt+Backspace.

:h2 res=IDM_CUT.Cortar (Opci¢n del men£)
Esta funci¢n borra del documento el texto seleccionado y lo copia
al clipboard del sistema.
:p.La tecla aceleradora es Shift+Delete.

:h2 res=IDM_COPY.Copiar (Opci¢n del men£)
Esta funci¢n copia el texto seleccionado al clipboard del
sistema.
:p.La tecla aceleradora es Ctrl+Insert.

:h2 res=IDM_PASTE.Pegar (Opci¢n del men£)
Con esta funci¢n podremos replazar (si estamos en modo de
sobreescritura) o insertar (si estamos en modo de inserci¢n) el
texto seleccionado por el contenido del clipboard.
:p.La tecla aceleradora es Shift+Insert.

:h2 res=IDM_CLEAR.Borrar (Opci¢n del men£)
Esta funci¢n borra el texto seleccionado en el documento, sin
alterar el contenido del clipboard.
Esta funci¢n es equivalente a pulsar la tecla Supr (Delete).

:h2 res=IDM_CLEARCLIPBOARD.Borrar clipboard (Opci¢n del men£)
Esta funci¢n borra el contenido del clipboard.

:h2 res=IDM_SELECT_ALL.Seleccionary todo (Opci¢n del men£)
Esta funci¢n selecciona todo el texto del documento actual, desde
el comienzo hasta el fin.
:p.Esta funci¢n tambi‚n se puede hacer por hacer un doble-click con
el bot¢n n£mero uno en el  rea de esquina por arriba a la izquierda,
entre las reglas horizontal y vertical.

:h2 res=IDM_PASTE_FROM.Menu &apos.Pegar de&apos.
De este men£ se puede seleccionar la fuente general de im genes
gr ficas para ser pegados en el documento.
Los siguientes opciones est n disponibles&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP.Pegar del clipboard:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN.Pegar de la pantalla:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_FILE.Pegar de un fichero gr fico:elink.
:esl.

:h3 res=IDM_PASTE_FROM_CLIP.Menu &apos.Pegar del clipboard&apos.
De este men£ se puede seleccionar las opciones que toman im genes
gr ficas del clipboard del sistema.
Los siguientes opciones est n disponibles&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Pegar bitmap del clipboard:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_CLIP_METAFILE.Pegar metafile del clipboard:elink.
:esl.

:h4 res=IDM_PASTE_FROM_CLIP_BITMAP.Pegar bitmap del clipboard (Opci¢n del men£)
Si un im gen del tipo bitmap est  puesto en el clipboard, esta opci¢n del men£
permite a uno que lo pegue en el documento.
Despu‚s que uno haya seleccionado esta opci¢n, entonces tiene que indicar
el lugar en la p gina donde quiere que una de las esquinas del im gen est‚.
Para indicarlo, uno tiene que mover el rat¢n hasta el lugar para la esquina,
entonces oprimir (no pulsar) el bot¢n izquierdo del rat¢n.
Con el bot¢n todav¡a oprimido, hay que mover el rat¢n hasta el lugar para
la esquina opuesta del imagen.
Se ver  un rect ngulo en la p gina mientras uno est‚ haciendo esto.
Una vez satisfecho del rect ngulo, hay que soltar el bot¢n.
El imagen ser  pegado al documento en el rect ngulo indicado, y ser 
apretado o estirado como sea necesario en altura y anchura.

:h4 res=IDM_PASTE_FROM_CLIP_METAFILE.Pegar Metafile from Clipboard (Opci¢n del men£)
Si un gr fico &apos.metafile&apos. ha sido puesto al clipboard, entonces esta opci¢n
del men£ le permite pegarlo en el documento.
Despu‚s que uno haya seleccionado esta opci¢n, uno ser  avisado a ubicar
el imagen en la p gina actual del documento, igual como se hizo para la opci¢n del men£
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Pegar bitmap del clipboard:elink..

:h3 res=IDM_PASTE_FROM_SCRN.Menu &apos.Pegar de pantalla&apos.
Desde este men£ se puede eligir entre las opciones de pegar que toman
su informaci¢n gr fica de la pantalla actual.
Los siguientes opciones est n disponibles&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_ENTIRE.Pegar de la pantalla entera:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_FRAME.Pegar de una ventana marco:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_WINDOW.Pegar de una ventana simple:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_RECT.Pegar de un rect ngulo:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE_FROM_SCRN_HIDE.Esconder:elink.
:esl.

:h4 res=IDM_PASTE_FROM_SCRN_ENTIRE.Pegar de la pantalla entera (Opci¢n del men£)
Cuando uno seleccione esta opci¢n del men£, se hace una copia &apos.bitmap&apos. de la 
pantalla entera en su estado actual y entonces uno ser  avisado a ubicar
el imagen en la p gina actual del documento, igual como se hizo para la opci¢n del men£
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Pegar bitmap del clipboard:elink..
:note.Hay que tener cuidado con este tipo de pegar gr fico, como la informaci¢n
gr fica puede ser bien voluminoso.  No importa a que tama¤o uno trate de pegar
el gr fico en el documento, la cantidad de memoria que consume queda igual, y un
imagen de la pantalla completa puede ser algo entre 100K y 800K bytes, seg£n
su tipo de adaptor de video, su resoluci¢n y cantidad de colores simultaneas.

:h4 res=IDM_PASTE_FROM_SCRN_FRAME.Pegar de una ventana marco (Opci¢n del men£)
Esta funci¢n no est  implementada todav¡a.

:h4 res=IDM_PASTE_FROM_SCRN_WINDOW.Pegar de una ventana simple (Opci¢n del men£)
Esta funci¢n no est  implementada todav¡a.

:h4 res=IDM_PASTE_FROM_SCRN_RECT.Pegar de un rect ngulo (Opci¢n del men£)
Esta funci¢n no est  implementada todav¡a.

:h4 res=IDM_PASTE_FROM_SCRN_HIDE.Esconder (Opci¢n del men£)
Esta opci¢n del men£, cuando est‚ chequeado, causa que la aplicaci¢n
se esconda cuando cualquier de las opciones del
:link reftype=hd res=IDM_PASTE_FROM_SCRN.Menu &apos.Pegar de pantalla&apos.:elink.
se pida.  La aplicaci¢n se esconde, la opci¢n del menu &apos.Pegar de&apos.
se hace, y entonces la aplicaci¢n se restaura a su estado original
en la pantalla.

:h3 res=IDM_PASTE_FROM_FILE.Pegar Graphic from File (Opci¢n del men£)
Cuando uno seleccione esta opci¢n del men£, el di logo normal del sistema para
seleccionar ficheros se mostrar , del cual uno necesita escoger el fichero
gr fico para ser pegado en el documento.  Los tipos de ficheros actualmente
aceptados son&colon.
:sl compact.
:li.&sqbul.Bitmap IBM OS/2 (.BMP)
:li.&sqbul.MetaFile IBM OS/2 (.MET)
:li.&sqbul.Bitmap Ventanas Microsoft (.BMP)
:li.&sqbul.Formato para Intercambio de Gr ficos (.GIF)
:li.&sqbul.Formato para Intercambio del Grupo Junto de Expertos Fotogr ficos (.JPG,.JPEG,.JFIF)
:li.&sqbul.Formato para &apos.PC Paintbrush&apos. (.PCX o .PCC)
:li.&sqbul.Tag Image File Format (.TIF o .TIFF)
:esl.
:p.Una vez que uno haya seleccionado un fichero, entonces uno ser  avisado a ubicar
el imagen en la p gina actual del documento, igual como se hizo para la opci¢n del men£
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Pegar bitmap del clipboard:elink..

:h2 res=IDM_INSERT_FUNCTION.Menu &apos.Insertar Funci¢n&apos.
Este submen£ le permite a uno usar varias opciones para insertar
c¢digos de funciones especiales en el documento.
Los siguientes opciones est n disponibles&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_DATE.Fecha:elink.
:li.&sqbul.:link reftype=hd res=IDM_CURRENTPAGE.N£mero actual de p gina:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGECOUNT.Cuenta total de p ginas:elink.
:li.&sqbul.:link reftype=hd res=IDM_SEPARATOR.Separador:elink.
:li.&sqbul.:link reftype=hd res=IDM_BARCODE.C¢digo de barras...:elink.
:esl.
:note.Este submen£ no forma parte del men£ para texto seleccionado.

:h3 res=IDM_DATE.Men£ de Fechas
Este submen£ permite acceder a las funciones de inseci¢n de
fecha. Actualmente son estas dos&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_DATETODAY.D¡a de hoy:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATECURRENT.Fecha:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATECHANGED.Fecha documento:elink.
:li.&sqbul.:link reftype=hd res=IDM_DATEFMT.Formato de la fecha...:elink.
:esl.
:note.Este submen£ no forma parte del men£ para texto seleccionado.

:h4 res=IDM_DATETODAY.Dia de hoy (Opci¢n del men£)
Esta funci¢n inserta un objeto especial en el documento que muestra
la fecha en la cual fue insertado el objeto.  No cambia la fecha.
Como objeto, no puede ser editado: cuando ponemos el cursor 
encima suyo se comportar  como un s¢lo caracter o se¤al.
El formato de esta fecha puede ser alterado con el di logo
:link reftype=hd res=IDD_DATEFMT.Formato de la fecha...:elink.,
siempre que se haga antes del objeto.

:h4 res=IDM_DATECURRENT.Fecha (Opci¢n del men£)
Esta funci¢n inserta un objeto especial en el documento que muestra 
la fecha actual.
Ser  siempre le fecha actual cuando se visualize o se imprima 
el documento, no la fecha de cuando se insert¢ el objeto en 
el documento.
Como objeto, no puede ser editado: cuando ponemos el cursor 
encima suyo se comportar  como un s¢lo caracter o se¤al.
El formato de esta fecha puede ser alterado con el di logo
:link reftype=hd res=IDD_DATEFMT.Formato de la fecha...:elink., 
siempre que se haga antes del objeto.

:h4 res=IDM_DATECHANGED.Fecha documento (Opci¢n del men£)
Esta funci¢n inserta un objeto especial en el documento que muestra 
la fecha en la cual fue hecho el £ltimo cambio al documento.
Esta fecha cambiar  cuando se haga un cambio al documento.
Como objeto, no se puede ser editado: cuando ponemos el cursor 
encima suyo se comportar  como un s¢lo caracter o se¤al.
El formato de esta fecha puede ser alterado con el di logo
:link reftype=hd res=IDD_DATEFMT.Formato de la fecha...:elink.,
siempre que se haga antes del objeto.

:h4 res=IDM_DATEFMT.Formato de la fecha... (Opci¢n del men£)
Esta opci¢n del men£ activa la caja de di logo
:link reftype=hd res=IDD_DATEFMT.Formato de la fecha...:elink. que se
utiliza para alterar el formato de las fechas generadas.

:h5 res=IDD_DATEFMT.Formato de la fecha... (Di logo)
En este di logo es posible alterar el formato utilizado en la
generaci¢n de las fechas. 
:artwork name='Espanol\DateFmt.bmp' align=center.
:p.El formato es cualquier cosa que se escribe en el campo, excepto 
determinados caracteres precedidos del s¡mbolo &apos.&percent.&apos. que tienen un 
significado especial. Estos c¢digos son&colon.
:sl compact.
:li.&percent.D - se sustituye por el d¡a de la semana (Lunes, martes, etc.).
:li.&percent.e - se sustituye por el d¡a de la semana (Lun, Mar, etc.).
:li.&percent.d - se sustituye por el n£mero del d¡a.
:li.&percent.m - se sustituye por el n£mero del mes.
:li.&percent.M - se sustituye por el nombre del mes (enero, febrero, etc.).
:li.&percent.n - se sustituye por el nombre del mes (ene, feb, etc.).
:li.&percent.a - se sustituye por el n£mero del a¤o (las cuatro cifras).
:li.&percent.b - se sustituye por el n£mero del a¤o (solo dos cifras).
:esl.
:p.
Cualquier otro texto introducido se copiar  directamente a la
fecha. Por ejemplo, la cadena "&percent.w, &percent.d de &percent.M de &percent.y" podr¡a generar
la fecha "Mi‚rcoles, 27 de abril de 1994".
:p.
Una vez introducido el nuevo formato de la fecha, pulsar el bot¢n OK
para grabarlo. El documento activo ser  reformateado
adecuadamente.
:p.
Si se pulsa el bot¢n APLICAR, el nuevo formato de la fecha ser 
aplicado al documento activo y pasar  a ser el valor por defecto.
:p.
Para salir del di logo sin grabar los cambios, pulsar el bot¢n
ESCAPE. Los cambios realizados con el bot¢n APLICAR no se
perder n.

:h3 res=IDM_CURRENTPAGE.N£mero actual de p gina (Opci¢n del Men£)
Esta opci¢n del men£ inserta un objeto especial en el documento que muestra
el n£mero de p gina actual donde se encuentra el objeto dentro del documento.

:h3 res=IDM_PAGECOUNT.Cuenta total de p ginas (Opci¢n del Men£)
Esta opci¢n del men£ inserta un objeto especial en el documento que muestra
la cuenta total (actualizado) de p ginas en el documento.

:h3 res=IDM_SEPARATOR.Separador (Opci¢n del Men£)
Esta opci¢n del men£ inserta una funci¢n especial de separador
horizontal al documento.  Este separador termina la l¡nea actual,
dibuja una l¡nea de separaci¢n horizontal, y mueve a la pr¢xima
l¡nea, listo para a¤adir mas texto.

:h3 res=IDM_BARCODE.C¢digo de barras... (Opci¢n del Men£)
Esta opci¢n del men£ muestra un di logo del cual se puede preparar
para a¤adir un gr fico c¢digo de barras al documento.  
Para mas informaci¢n, v‚ase 
:link reftype=hd res=IDD_BARCODE.A¤adir c¢digo de barras (Di logo):elink.

:h4 res=IDD_BARCODE.A¤adir c¢digo de barras (Di logo)
Este di logo le permite a uno especificar el tipo y el texto
para a¤adir un gr fico c¢digo de barras al documento.
:artwork name='Espanol\BarCode.bmp' align=center.
Se necesita seleccionar el tipo del c¢digo de barras de la lista,
y entonces entrar el texto para ser codificado en el campo de entrar.
Cuando todo est  listo, entonces hay que pulsar la tecla ENTER o 
hacer un click sobre el bot¢n de OK, y entonces saldr  un mensaje
instruy‚ndole a indicar d¢nde se pondr  el gr fico en la p gina,
igual como se hizo para la opci¢n del men£
:link reftype=hd res=IDM_PASTE_FROM_CLIP_BITMAP.Pegar bitmap del clipboard:elink..

:h2 res=IDM_SEARCH.Buscar... (Opci¢n del men£)
Esta opci¢n del men£ causa que se muestre un di logo del cual se puede
entrar un texto para ser buscada y otro texto con que se va a reemplazar
al primer texto, donde se encuentre y si se pida.
:p.V‚ase :link reftype=hd res=IDD_SEARCH.Buscar... (Di logo):elink.
para mas informaci¢n.

:h3 res=IDD_SEARCH.Buscar... (Di logo)
De este di logo se puede fijar un texto para buscarse con las opciones del men£
:link reftype=hd res=IDM_SEARCH.Buscar:elink. y :link reftype=hd res=IDM_FINDNEXT.Repetir busqueda:elink..
Tambi‚n se puede fijar un texto reemplazo y causar que ocurre el reemplazo del
texto original con el texto nuevo.
:artwork name='Espanol\Search.bmp' align=center.
:p.Para buscar un texto particular, hay que entrar el texto en el primer campo
de entrada, y entonces pulsar ENTER o hacer un clock en el bot¢n &apos.Buscar&apos..
Se har  la busqueda y el texto encontrado (si haya) ser  seleccionado y marcado.
El di logo queda presente para que se pueda cambiar el texto para buscarse si
uno quiera.
:p.Para reemplazar un texto con otro, hay que entrar el texto original en el
primer campo de entrada, su reemplazo en el segundo, y entonces pulsar el
bot¢n de &apos.Reemplazar&apos.,  Se puede repetir esta funci¢n por cada ocurrencia del
texto original si uno desee.
:p.Para reemplazar un texto con otro y entonces buscar la pr¢xima ocurrencia
del texto original, se puede usar el bot¢n de &apos.Cambiar y Buscar&apos..
:p.Para reemplazar todas las ocurrencias del texto original con el texto nuevo,
se puede usar el bot¢n de &apos.Cambiar Todos&apos..
:p.Si uno quiere que las busquedas sean sensibles a la diferencia entre letras 
may£sculas y men£sculas, entonces se puede hacer un click sobre el bot¢n
&apos.Sensible a may£scula/men£scula&apos. para cambiar la sensibilidad actual.
:p.If you wish your searches to obey the rules of UNIX Regular Expressions, 
you may check the Regular Expression button.
:p.Para cerrar al di logo, hay que pulsar la tecla ESC o hacer un click
sobre el bot¢n de cancelar.  Los textos para buscarse y de reemplazo
ser n guardados para la pr¢xima vez que se usa este di logo, o para
la funci¢n :link reftype=hd res=IDM_FINDNEXT.Repetir busqueda:elink..

:h2 res=IDM_FINDNEXT.Repetir busqueda (Opci¢n del men£)
Si ya se haya entrado un texto para buscarse con el di logo
:link reftype=hd res=IDD_SEARCH.Buscar... (Di logo):elink.,
entonces esta funci¢n buscar  mas adelante en el texto para la
pr¢xima ocurrencia del texto especificado y lo seleccionar .

:h2 res=IDM_GOTOPAGE.Saltar a p gina (Opci¢n del men£)
Esta opci¢n del men£ llama un di logo del cual se puede especificar
el n£mero de la p gina que uno quiere tener mostrado para
revisi¢n.  V‚ase la ayuda para el di logo 
:link reftype=hd res=IDD_GOTOPAGE.Saltar a p gina:elink.
para mas informaci¢n.

:h3 res=IDD_GOTOPAGE.Saltar a p gina (Di logo)
Este di logo le permite a uno especificar el n£mero de la p gina
del documento que uno quiere tener mostrado para revisi¢n.
Hay que entrar el n£mero de p gina en el campo provisto y
entonces pulsar la tecla ENTER, o pulsar la tecla ESC para
escapar esta funci¢n.

:h2 res=IDM_SPELL.Chequeo de ortograf¡a... (Opci¢n del Men£)
Si el chequeador de ortograf¡a opcional est‚ instalado en su sistema,
y pudo encontrar su diccionario, entonces esta opci¢n del men£ ser 
disponible.
Cuando uno seleccione esta opci¢n del men£, el chequeador repasar 
al documento desde su posici¢n actual hacia el fin del documento,
buscando para palabras que posiblemente contienen errores de ortograf¡a.
Cuando encuentre uno, se selecciona la palabra y el di logo
:link reftype=hd res=IDD_SPELL.Chequeo de ortograf¡a:elink.
ser  desplegada, permiti‚ndole a uno que vea la palabra y una lista
de reemplazos posibles, y d ndole la oportunidad de decirle al
chequeador qu‚ hacer con la palabra.
:p.La tecla aceleradora para esta funci¢n es F5.

:h3 res=IDD_SPELL.Chequeo de ortograf¡a (Di logo)
Este di logo se ve cuando el chequeo de ortograf¡a encuentre una palabra
que puede ser mal deletreado.  Muestra la palabra junta con una lista de
reemplazos potenciales tomados del diccionario.
:artwork name='Espanol\Spell.bmp' align=center.
:p.Si uno quiere dejar la palabra tal como est , solo hay que oprimir la
tecla ENTER o tocar al bot¢n de :hp4.Saltar:ehp4. con el rat¢n.
Esta es la acci¢n normal.
:p.Si uno quiere que el chequeo deje esta palabra por el resto de la sessi¢n,
entonces hay que pulsar al bot¢n de :hp4.Aceptar:ehp4. con el rat¢n.
:p.Para a¤adir la palabra al diccionario personal, con las letras exactamente
como est n, hay que pulsar al bot¢n de :hp4.Inyertar:ehp4. con el rat¢n.
Si la palabra tiene letras en mayusculas, entonces la entrada en el diccionario
ser  igual, y solo palabras id‚nticas se verificar n.
:p.Para a¤adir la palabra al diccionario personal, pero con las letras forzadas
a men£sculas, entonces hay que pulsar al bot¢n :hp4.Inyertar Men£scula:ehp4..
Tales palabras podr n verificar a palabras en may£scula o en men£scula.
:p.Si uno quiere usar una de las palabras potenciales, hay que seleccionar una
de la lista y pulsar ENTER, o pulsar la palabra dos veces r pidamente con el
rat¢n.  La palabra original ser  reemplazada con la palabra escogida.
:p.Si uno quiere entrar un reemplazo que no aparece en la lista, uno tiene
que entrarlo a la cabeza de la lista de reemplazos, y pulsar
al boton :hp4.Reemplazar:ehp4..  La palabra original ser  reemplazada
con la palabra nueva.
:p.Para salir del chequeo de ortograf¡a sin tomar mas acci¢n, hay que pulsar
la tecla ESC, o pulsar al bot¢n :hp4.Esc:ehp4. con el rat¢n.  Cambios ya hechos
ser n retenidas, aunque todav¡a se podr n remover con la funci¢n 
:link reftype=hd res=IDM_UNDO.Deshacer:elink..

:h2 res=IDM_ZOOM.Zoom (Submen£)
Esta opci¢n del men£ activa un submen£ con las diferentes
funciones de zoom de visualizaci¢n. Las funciones disponibles
son&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_100.Zoom 100&percent.:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLWIDTH.Zoom anchura:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_FULLHEIGHT.Zoom altura:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_MARGINS.Zoom m rgenes:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_ALL.Zoom completo:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_WINDOW.Zoom ventana:elink.
:li.&sqbul.:link reftype=hd res=IDM_ZOOM_SET.Indicar zoom...:elink.
:esl.

:h3 res=IDM_ZOOM_100.Zoom 100&percent. (Opci¢n del men£)
Esta orden fija la escala de visualizaci¢n en el 100&percent.. Lo que
vemos en pantalla se imprimir  con el mismo tama¤o, a condici¢n
de que los factores de escala del sistema (que estan fuera de mi
control) esten correctamente fijados.

:h3 res=IDM_ZOOM_FULLWIDTH.Zoom anchura (Opci¢n del men£)
Esta orden fija la escala de visualizaci¢n de forma que toda la
p gina ocupe la anchura total de la ventana. Esto puede reducir o
ampliar la escala del documento, dependiendo de lo grande que sea
la ventana.

:h3 res=IDM_ZOOM_FULLHEIGHT.Zoom altura (Opci¢n del men£)
Esta orden fija la escala de visualizaci¢n de forma que toda la
p gina ocupe la altura total de la ventana. Esto normalmente
reduce la escala del documento causando una visualizaci¢n m s
peque¤a de lo que realmente se imprimira.

:h3 res=IDM_ZOOM_MARGINS.Zoom m rgenes (Opci¢n del men£)
Esta orden fija la escala de visualizaci¢n de forma que la
porci¢n del documento dentro de los m rgenes horizontales ocupe
toda la ventana. Esto normalmente amplia la escala del documento,
dependiendo del tama¤o de la ventana.

:h3 res=IDM_ZOOM_ALL.Zoom completo (Opci¢n del men£)
Esta orden fija la escala de visualizaci¢n de forma que todo el
documento se visualize en la ventana, sin que importe lo peque¤a
o deformada que pueda ser.
:p.La tecla aceleradora para esta funci¢n es Ctrl+Z, y cuando se
usa de este modo la funci¢n alterna entre Zoom Completo y el zoom
normal que se especifica en el di logo
:link reftype=hd res=IDD_PREFERENCES.Configuraci¢n general:elink..

:h3 res=IDM_ZOOM_WINDOW.Zoom parcial (Opci¢n del men£)
Esta orden nos permite seleccionar una  rea rectangular de la
ventana activa y aumentar su tama¤o. Una vez seleccionado esta
 rea, el documento tendr  un nuevo factor de zoom que se aplicar 
al mismo y se reposicionar  a la porci¢n que haya sido
seleccionado sea visible al m ximo.

:h3 res=IDM_ZOOM_SET.Indicar Zoom... (Opci¢n del men£)
Esta opci¢n del men£ causa que se muestra un di logo del cual
se puede fijar el factor de zoom actual.
:p.V‚ase :link reftype=hd res=IDD_ZOOM_SET.Zoom Set... (Di logo):elink. 
para mas informaci¢n.

:h4 res=IDD_ZOOM_SET.Indicar Zoom... (Di logo)
Este di logo permite al usuario que fije el factor exacto de zoom,
como un porcentaje.
Se puede usar las flechas para arriba o para abajo para cambiar el
factor, o se puede entrar el factor directamente.
Despu‚s de cambiar el factor, hay que pulsar la tecla ENTER o hacer
un click en el bot¢n &apos.OK&apos. para hacer que el factor nuevo tome efecto.
:artwork name='Espanol\ZoomSet.bmp' align=center.
:p.Para aplicar el factor de zoom sin cerrar el di logo,
seleccionar el bot¢n APLICAR. El Zoom aplicado de esta forma
permanecer  en efecto despu‚s de cerrar el di logo, incluso si se
pulsa la tecla ESCAPE.
:p.Para salir del di logo sin realizar ning£n cambio, pulsa la tecla
ESCAPE o haz un click en el bot¢n Anular. Los cambios realizados
con el bot¢n APLICAR no desparecer n.

:h1 res=IDM_LAYOUT_MENU.Men£ Disposici¢n
:i1.Men£ Disposici¢n
En el men£ Disposici¢n estan disponibles las siguientes
¢rdenes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_MARGINS.M rgenes...:elink.
:li.&sqbul.:link reftype=hd res=IDM_TABS.Tabuladores...:elink.
:li.&sqbul.:link reftype=hd res=IDM_INDENT.Indentaci¢n...:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINESPACING_MENU.Espaciado:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUSTIFICATION.Justificaci¢n:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGECENTER.Centrar p gina:elink.
:li.&sqbul.:link reftype=hd res=IDM_PAGEBREAK.Salto de p gina:elink.
:li.&sqbul.:link reftype=hd res=IDM_HEADER.Cabecera de p gina...:elink.
:li.&sqbul.:link reftype=hd res=IDM_FOOTER.Pie de p gina...:elink.
:esl.

:h2 res=IDM_MARGINS.M rgenes... (Opci¢n del men£)
Esta orden invoca un di logo que permite establecer los cuatro
m rgenes de impresi¢n (superior, inferiror, derecha e izquierda).
:p.Ver :link reftype=hd res=IDD_MARGINS.M rgenes (Di logo):elink. para
m s informaci¢n.

:h3 res=IDD_MARGINS.M rgenes (Di logo)
A trav‚s de este di logo podemos establecer los m rgenes de
impresi¢n izquierdo, derecho superior e inferior.
Estos m rgenes se especifica bajo el sistema de medir actualmente
elegido, que sea inglesa o metrica, y son las distancias desde
los costados del formulario seleccionado actualmente.
:artwork name='Espanol\Margins.bmp' align=center.
:p.Cuando se entran nuevos valores, las reglas horizontal y vertical
son ajustadas conforme a los mismos, aunque el documento no es
reformateado.
:p.Entar la distancia y pulsar la tecla ENTER o haz un click en el
bot¢n OK. Si los m rgenes no son v lidos para la hoja activa,
aparecer  un mensaje de error y se deber n corregir los valores.
:p.Si se pulsa el bot¢n APLICAR, los nuevos m rgenes son aplicados
al instante al documento activo y se convierten en los nuevos
m rgenes.
:p.Para abortar el di logo y descartar los cambios, pulsar la tecla
ESC o hacer un click en el bot¢n "Cancelar".
Los cambios realizados con el bot¢n Aplicar no se perder n.
:note.Los nuevos m rgenes derecho e izquierdo ser n efectivos en
la siguiente l¡nea, excepto si la orden de margen se haya
insertado al inicio de la l¡nea.
Los nuevos m rgenes superior e inferior ser n efectivos en la
pr¢xima p gina, excepto si la orden de margen se haya insertado
al inicio de la p gina.

:h2 res=IDM_TABS.Tabuladores... (Opci¢n del men£)
Esta orden invoca una caja de di logo en la que es posible
alterar la posici¢n de los tabuladores. Los nuevos valores
tendr n efecto desde la posici¢n del cursor hacia adelante.
:p.Ver :link reftype=hd res=IDD_TABS.Tabuladores (Di logo):elink. para
m s informaci¢n.

:h3 res=IDD_TABS.Tabuladores (Di logo)
Este di logo permite fijar la posici¢n de los tabuladores. Los
cambios ser n inmediatamente visibles en la regla de la ventana
activa.
:artwork name='Espanol\Tabs.bmp' align=center.
:p.Para fijar un tabulador, indicar la posici¢n y a continuaci¢n
pulsar el bot¢n "Fijar".
:p.Para borrar un tabulador, indicar la posici¢n y a continuaci¢n
pulsar el bot¢n "Borrar".
:p.Una vez realizados todos los cambios, seleccionar el bot¢n OK y
pulsar ENTER. Los cambios realizados tendr n efecto desde la
posici¢n del cursor hacia adelante.
:p.Si pulsamos el bot¢n APLICAR, los nuevos valores del tabulador se
aplicar n al documento activa y pasar n a ser los nuevos valores
por defecto.
:p.Para salir sin grabar, pulsar la tecla ESCAPE o seleccionar el
bot¢n Cancelar. Los cambios realizados con el bot¢n APLICAR no
se perder n.

:h2 res=IDM_INDENT.Indentaci¢n... (Opci¢n del men£)
Esta opci¢n activa un di logo en el que se especifica la
distancia de indentaci¢n a utilizar en el inicio de cada nuevo
p rrafo.
:p.Ver :link reftype=hd res=IDD_INDENT.Indentaci¢n (Di logo):elink.
para m s informaci¢n

:h3 res=IDD_INDENT.Indentaci¢n (Di logo)
En este di logo es posible fijar la distancia de indentaci¢n de
los p rrafos, desde la posici¢n del cursor hacia adelante.
:artwork name='Espanol\Indent.bmp' align=center.
:p.Puedes entrar la nueva distancia y pulsar el bot¢n OK para grabar
el nuevo valor. El documento activo ser  reformateado.
:p.Si pulsas el bot¢n APLICAR la nueva distancia de indentaci¢n se
aplicar  al documento activo y pasar  a ser el valor por defecto.
:p.Para salir de este di logo sin realizar ningun cambio, pulsa el
bot¢n Cancelar. Los cambios realizados con el bot¢n APLICAR no se
perder n.

:h2 res=IDM_LINESPACING_MENU.Men£ de espaciados
Este submen£ presenta las diferentes opciones para espaciado
vertical de l¡neas que est n disponibles actualmente.
:p.La entrada que representa el espaciado en uso actualmente
est  chequeado.
:p.V‚ase :link reftype=hd res=IDM_LINESPACINGS.Entradas del men£ de espaciados:elink.
para mas informaci¢n.

:h3 res=IDM_LINESPACINGS.Entrada del men£ de espaciados
Esta entrada del men£ representa una de las opciones actualmente disponibles
para espaciado de l¡neas de texto.
Cuando una de estas opciones se seleccione, entonces espaciado elegido
se establecer  sobre el bloque de texto seleccionado actualmente, o desde
la posici¢n actual del cursor por adelante.

:h3 res=IDM_LINESPACINGMGR.Gestor de espaciado (Opci¢n del men£)
Esta opci¢n del men£ muestra el di logo :link reftype=hd res=IDD_LINESPACINGMGR.Gestor de espaciado:elink.,
del cual se puede cambiar el contenido del men£ de espaciados.

:h4 res=IDD_LINESPACINGMGR.Gestor de espaciado (Di logo)
Este di logo muestra la configuraci¢n actual del menu de espaciados, y 
permite que se le cambie.:p.
El menu actual se muestra en la lista a la derecha.  Si uno haga un click
sobre una entrada, entonces el valor ser  mostrado por el control a la 
izquierda y la apariencia de l¡neas de texto espaciados as¡ se ve en el 
area central del di logo.:p.
Se puede usar el control a la izquierda para cambiar el espaciado.  Para
a¤adir el nuevo espaciado al men£, solo hay que hacer un clock sobre el 
bot¢n &apos.&gt.&gt.&apos..:p.
Para removar una entrada del men£, hay que hacer un click sobre la entrada
en la lista, y entonces hacer un click al bot¢n &apos.&lt.&lt.&apos..
Para restaurar el men£ a su contenido por defecto, hay que hacer un click
sobre el bot¢n &apos.Defecto&apos..

:h2 res=IDM_JUSTIFICATION.Justificaci¢n (Opci¢n del men£)
Este submen£ muestra las opciones de justificaci¢n horizontal
existentes, que son&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_JUST_LEFT.Justificar a la izquierda:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_CENTER.Centrar:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_RIGHT.Justificar a la derecha:elink.
:li.&sqbul.:link reftype=hd res=IDM_JUST_FULL.Justificaci¢n completa:elink.
:esl.

:h3 res=IDM_JUST_LEFT.Justificaci¢n a la izquierda (Opci¢n del men£)
Esta opci¢n inserta una orden en el documento para justificar a
la izquierda el texto, a partir de la pr¢xima l¡nea a no ser que
la orden se inserte al inicio de la l¡nea.

:h3 res=IDM_JUST_CENTER.Centrar (Opci¢n del men£)
Esta opci¢n inserta una orden en el documento para centrar el
texto, a partir de la pr¢xima l¡nea a no ser que la orden se
inserte al inicio de la l¡nea.

:h3 res=IDM_JUST_RIGHT.Justificar a la derecha (Opci¢n del men£)
Esta opci¢n inserta una orden en el documento para justificar a
la derecha el texto, a partir de la pr¢xima l¡nea a no ser que
la orden se inserte al inicio de la l¡nea.

:h3 res=IDM_JUST_FULL.Justificaci¢n completa (Opci¢n del men£)
Esta opci¢n inserta una orden en el documento para justificar
completamente el texto, a partir de la pr¢xima l¡nea a no ser que
la orden se inserte al inicio de la l¡nea. El tama¤o de los
espacios entre palabras es ajustado, excepto si la l¡nea
finaliza con un salto de p gina o un retorno de carro, en cuyo
caso la l¡nea se justifica a la izquierda.

:h2 res=IDM_PAGECENTER.Centrar p gina (Opci¢n del men£)
Esta opci¢n inserta una orden en el documento para centrar las
p ginas verticalemente, a partir de la pr¢xima p gina a no ser
que la orden se inserte al inicio de la p gina.
:note.Esta funci¢n no puede utilizarse con gr ficos, y en el caso
de que la p gina contenga gr ficos estar  deshabilitada.

:h2 res=IDM_PAGEBREAK.Salto de p gina (Opci¢n del men£)
Esta orden insertar  un salto de p gina en la posici¢n del
cursor, de forma que se avanze a una nueva p gina.
:p.La tecla aceleradora es CTRL+ENTER.
:note.Esta opci¢n no forma parte del men£ de texto seleccionado.

:h2 res=IDM_HEADER.Cabecera de p gina... (Opci¢n del men£)
Esta opci¢n invoca un di logo del cual se puede fijar la
apariencia de la cabecera de la p gina desde este lugar para
adelante.

:h3 res=IDD_HEADER.Cabecera de p gina (Di logo)      
Este di logo permite la especificaci¢n de la nueva cabecera
de la p gina.
Por ahora, la cabecera tiene que especificarse en el formato 
nativo de Escriba. 
Mas tarde, la definici¢n se pondr  &apos.WYSIWYG&apos..
La mayor¡a de los c¢digos de funci¢n (como PageNumber, PageCount,
LeftJustify, etc) son v lidos para usarse en el formato de la
cabecera, pero no todos (como PrinterSettings).

:h2 res=IDM_FOOTER.Pie de p gina... (Opci¢n del men£)
Esta opci¢n invoca un di logo del cual se puede fijar la 
apariencia del pie de la p gina desde este lugar para adelante.

:h3 res=IDD_FOOTER.Pie de p gina (Dialog)      
Este di logo permite la especificaci¢n del nuevo pie de la p gina.
Por ahora, el pie de la p gina tiene que especificarse en el formato 
nativo de Escriba. 
Mas tarde, la definici¢n se pondr  &apos.WYSIWYG&apos..
La mayor¡a de los c¢digos de funci¢n (como PageNumber, PageCount,
LeftJustify, etc) son v lidos para usarse en el formato de la
cabecera, pero no todos (como PrinterSettings).

:h1 res=IDM_FONT_MENU.Men£ de fuentes
:i1.Men£ de fuentes
Este men£ muestra una lista de todos los fuentes de texto actualmente
disponibles.
:p.El fuente actualmente activo al lugar del cursor est  marcado.
:p.V‚ase :link reftype=hd res=IDM_FONTS.Entradas del men£ de fuentes:elink.
para mas informaci¢n.

:h2 res=IDM_FONTS.Entrada del men£ de fuentes
Esta entrada del men£ representa uno de los fuentes de texto actualmente
disponibles, mostrado tal como parecer¡a en el documento, a 18 puntos de
tama¤o.  Si uno seleccione esta opci¢n, entonces el fuente ser  aplicado
sobre el bloque de texto actualmente seleccionado, o desde la posici¢n 
actual del cursor por adelante,

:h2 res=IDM_FONTS_PGUP.Paginar arriba el men£ de fuentes
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de fuentes a la vez, y hay mas entradas disponibles antes
de la primera actualmente mostrada.
Para paginar el men£ hacia arriba, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h2 res=IDM_FONTS_PGDN.Paginar abajo el men£ de fuentes
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de fuentes a la vez, y hay mas entradas disponibles despu‚s
de la £ltima actualmente mostrada.
Para paginar el men£ hacia abajo, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h2 res=IDM_FONTMGR.Gestor del men£ de fuentes
Esta funci¢n no est  implementada todav¡a.

:h1 res=IDM_SIZE_MENU.Men£ de tama¤os
:i1.Men£ de tama¤os
Este men£ muestra todos tama¤os de fuentes actualmente disponibles
en el fuente actualmente escogido.
:p.El tama¤o actualmente activo al lugar del cursor est  marcado.
:p.V‚ase :link reftype=hd res=IDM_SIZES.Entrada del men£ de tama¤os:elink.
para mas informaci¢n.

:h2 res=IDM_SIZES.Entrada del men£ de tama¤os
Esta entrada del men£ representa uno de los tama¤os de texto actualmente
disponibles, mostrando el fuente actual tal como parecer¡a en el documento, 
a cada tama¤o.
Si uno seleccione esta opci¢n, entonces el tama¤o ser  aplicado
sobre el bloque de texto actualmente seleccionado, o desde la posici¢n 
actual del cursor por adelante,

:h2 res=IDM_SIZES_PGUP.Paginar arriba el men£ de tama¤os
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de tama¤os a la vez, y hay mas entradas disponibles antes
de la primera actualmente mostrada.
Para paginar el men£ hacia arriba, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h2 res=IDM_SIZES_PGDN.Paginar abajo el men£ de tama¤os
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de tama¤os a la vez, y hay mas entradas disponibles despu‚s
de la £ltima actualmente mostrada.
Para paginar el men£ hacia abajo, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h2 res=IDM_SIZEMGR.Gestor del men£ de tama¤os
Esta funci¢n no est  implementada todav¡a.

:h1 res=IDM_ATTR_MENU.Men£ Atributos
:i1.Men£ Atributos
Desde el men£ de atributos es posible insertar ¢rdenes de
atributos de texto en el documento. Es posible utilizar todos los
atributos de fuente soportados por el Presentation Manager:
Normal, Negrita, Cursiva, Subrayado, Tachado y Vacio. Para
activar un atributo en la posici¢n del cursor y hacia adelante (o
bien en el texto seleccionado), simplemente debemos seleccionarlo
con el teclado o el mouse en este men£. Las opciones del men£ se
visualizan utilizando el tipo de letra y el tama¤o en uso. Los
atributos en uso estan marcados. Se puede utilizar cualquier
combinaci¢n de atributos a la vez (excepto el normal, que es el
que est  activo cuando no hay ning£n otro atributo).
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_NORMAL.Normal:elink.
:li.&sqbul.:link reftype=hd res=IDM_BOLD.Negrita:elink.
:li.&sqbul.:link reftype=hd res=IDM_ITALIC.Cursiva:elink.
:li.&sqbul.:link reftype=hd res=IDM_UNDERSCORE.Subrayado:elink.
:li.&sqbul.:link reftype=hd res=IDM_STRIKEOUT.Tachado:elink.
:li.&sqbul.:link reftype=hd res=IDM_OUTLINE.Vacio:elink.
:li.&sqbul.:link reftype=hd res=IDM_CAPS.May£sculas:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXPAND_MENU.Expansiones:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINECOLOR_MENU.Color de texto:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEWIDTH_MENU.Grosor de l¡neas:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILLCOLOR_MENU.Color de  reas:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILLTYPE_MENU.Tipo de  reas:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEEND_MENU.Fin de l¡neas:elink.
:li.&sqbul.:link reftype=hd res=IDM_LINEJOIN_MENU.Uni¢n de l¡neas:elink.
:esl.

:h2 res=IDM_NORMAL.Normal (Opci¢n del men£)
Esta opci¢n del men£ cancela cualquier otro atributo de texto
(negrita, cursiva, etc...). Si esta opci¢n se selecciona cuando
hay texto marcado, s¢lo afecta a este bloqeu. En caso contrario
afectar  desde la posici¢n del cursor hacia adelante.

:h2 res=IDM_BOLD.Negrita(Opci¢n del men£)
Esta opci¢n del men£ aplica el atributo de negrita al texto
seleccionado o bien, en el caso de que no haya texto
seleccionado, desde la posici¢n del cursor hacia adelante.

:h2 res=IDM_ITALIC.Cursiva (Opci¢n del men£)
Esta opci¢n del men£ aplica el atributo de cursiva al texto
seleccionado o bien, en el caso de que no haya texto
seleccionado, desde la posici¢n del cursor hacia adelante.

:h2 res=IDM_UNDERSCORE.Subrayado (Opci¢n del men£)
Esta opci¢n del men£ aplica el atributo de subrayado al texto
seleccionado o bien, en el caso de que no haya texto
seleccionado, desde la posici¢n del cursor hacia adelante.

:h2 res=IDM_STRIKEOUT.Tachado (Opci¢n del men£)
Esta opci¢n del men£ aplica el atributo de tachado al texto
seleccionado o bien, en el caso de que no haya texto
seleccionado, desde la posici¢n del cursor hacia adelante.

:h2 res=IDM_OUTLINE.Vacio (Opci¢n del men£)
Esta opci¢n del men£ aplica el atributo de vacio (s¢lo se
utilizan los contornos de la letra, sin nada en su interior) al
texto seleccionado o bien, en el caso de que no haya texto
seleccionado, desde la posici¢n del cursor hacia adelante.

:h2 res=IDM_CAPS.May£sculas (Opci¢n del men£)
Esta opci¢n del men£ aplica el atributo de s¢lo may£sculas al
texto seleccionado o bien, en el caso de que no haya texto
seleccionado, desde la posici¢n del cursor hacia adelante.

:h2 res=IDM_EXPAND_MENU.Menu de expansiones
Este men£ muestra todas las diferentes expansiones horizontales que est n
disponibles para aplicar a textos actualmente.
:p.La expansi¢n activo a la posici¢n del cursor est  marcada.
:p.V‚ase :link reftype=hd res=IDM_EXPANDS.Entradas del men£ de expansiones:elink.
para mas informaci¢n.

:h3 res=IDM_EXPANDS.Entrada del men£ de expansiones
Esta entrada del men£ muestra una de las diferentes expansiones disponibles
actualmente, mostrando texto en el fuente actual, a 18 puntos de tama¤o,
en la expansi¢n que pertenece a la entrada del men£.
Si uno seleccione esta opci¢n, entonces la expansi¢n ser  aplicada
sobre el bloque de texto actualmente seleccionado, o desde la posici¢n 
actual del cursor por adelante,

:h2 res=IDM_EXPANDS_PGUP.Paginar arriba el men£ de expansiones
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de expansiones a la vez, y hay mas entradas disponibles antes
de la primera actualmente mostrada.
Para paginar el men£ hacia arriba, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_EXPANDS_PGDN.Paginar abajo el men£ de expansiones
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de expansiones a la vez, y hay mas entradas disponibles despu‚s
de la £ltima actualmente mostrada.
Para paginar el men£ hacia abajo, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_EXPANDMGR.Gestor del men£ de expansiones
Esta opci¢n del men£ causa que se muestre el di logo del 
:link reftype=hd res=IDD_EXPANDMGR.gestor del men£ de expansiones:elink.,
del cual se puede cambiar el contenido del men£ de expansiones.

:h4 res=IDD_EXPANDMGR.Expand Menu Manager Dialog
Este di logo muestra la configuraci¢n actual del men£ de expansiones,
y le permite hacer cambios al men£.:p.
El contenido del men£ actual se ve en la lista a la derecha.  Si se haga
un click sobre una entrada de la lista, entonces el valor se mostrar 
en el control a la izquierda por abajo y la apariencia del texto ampliado aparecer 
en el  rea de muestra para la izquierda por arriba del di logo.:p.
Se usa el control a la izquierda por abajo para cambiar el factor de expansi¢n actual.
Para a¤adir un factor nuevo al men£, hay que hacer un click sobre el bot¢n &apos.&gt.&gt.&apos..
Para remover un factor del men£, hay que seleccionar el factor de la lista y entonces
hacer un click sobre el bot¢n &apos.&lt.&lt.&apos..
Para restaurar el men£ a su estado por defecto, entonces hay que hacer un
click sobre el bot¢n &apos.Defecto&apos..

:h2 res=IDM_LINECOLOR_MENU.Men£ de color para l¡neas y textos
Este men£ muestra todos los diferentes colores disponibles actualmente
para aplicarse a l¡neas y textos.
:p.El color actualmente en uso a la posici¢n del cursor est  marcado.
:p.V‚ase :link reftype=hd res=IDM_LINECOLORS.Men£ de color para l¡neas y textos:elink.
para mas informaci¢n.

:h3 res=IDM_LINECOLORS.Men£ de colores para l¡neas y textos Entries
Esta entrada del men£ muestra uno de los colores actualmente disponibles
para aplicarse a l¡neas y textos.
Si uno seleccione esta opci¢n, entonces el color ser  aplicado
sobre el bloque de texto actualmente seleccionado, o desde la posici¢n 
actual del cursor por adelante,
:note.Estos colores aplican a l¡neas, texto s¢lido, y a las l¡neas de
afuera de texto vac¡o.

:h3 res=IDM_LINECOLORS_PGUP.Paginar arriba el men£ de colores para l¡neas y textos
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de colores para l¡neas y textos a la vez, y hay mas entradas disponibles antes
de la primera actualmente mostrada.
Para paginar el men£ hacia arriba, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_LINECOLORS_PGDN.Paginar abajo el men£ de colores para l¡neas y textos
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de colores para l¡neas y textos a la vez, y hay mas entradas disponibles despu‚s
de la £ltima actualmente mostrada.
Para paginar el men£ hacia abajo, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_LINECOLORMGR.Gestor para el men£ de colores para l¡neas y textos
Esta opci¢n del men£ causa que se muestre el 
:link reftype=hd res=IDD_LINECOLORMGR.Gestor para el men£ de colores para l¡neas y textos:elink.,
lo cual da al usuario la habilidad de cambiar el men£ de colores para l¡neas y textos.

:h4 res=IDD_LINECOLORMGR.Gestor para el men£ de colores para l¡neas y textos (di logo)
Este di logo provee al usuario una manera para cambiar el men£ de colores
para l¡neas y textos.  Una rueda de colores y un control de intensidad proveyen 
una manera facil para especificar colores, y para control mas fino hay tambi‚n
un juego de controles para los valores en rojo, verde y azul.
Los colores actuales del men£ se ven en una lista para revisar.:p.
Para a¤adir un color al men£, hay que especificar el color por medio de la
rueda o por medio de los controles para rojo, verde y azul, y entonces tocar
al bot¢n &apos.>>&apos..  Si el color no est‚ ya en la lista, se aumentar .:p.
Para remover un color del men£, hay que seleccionarlo de la lista y entonces
tocar al bot¢n &apos.<<&apos..:p.
Para restaurar el men£ a sus colores por defecto, hay que tocarlo al bot¢n
&apos.Defecto&apos..:p.
Para salir sin cambiar el men£, hay que tocarlo al bot¢n de cancelar, o oprimir
la tecla de escapar.

:h2 res=IDM_LINEWIDTH_MENU.Men£ grosor de l¡neas
Este men£ muestra todos los diferentes grosores actualmente disponibles
para aplicarse a l¡neas y textos vac¡os.
:p.El grosor actualmente en uso a la posici¢n del cursor est  marcado.
:p.V‚ase :link reftype=hd res=IDM_LINEWIDTHS.Entradas al menu grosor de l¡neas:elink.
para mas informaci¢n.

:h3 res=IDM_LINEWIDTHS.Entrada al men£ grosor de l¡neas
Esta entrada del men£ muestra uno de los grosores actualmente disponibles
para aplicarse a l¡neas y textos vac¡os.
Si uno seleccione esta opci¢n, entonces el grosor ser  aplicado
sobre el bloque de texto actualmente seleccionado, o desde la posici¢n 
actual del cursor por adelante,
:note.Estos grosores aplican a l¡neas y a las l¡neas de afuera de texto vac¡o.

:h3 res=IDM_LINEWIDTHS_PGUP.Paginar arriba el menu de grosor de l¡neas
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de grosores a la vez, y hay mas entradas disponibles antes
de la primera actualmente mostrada.
Para paginar el men£ hacia arriba, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_LINEWIDTHS_PGDN.Paginar abajo el menu de grosor de l¡neas
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de grosores a la vez, y hay mas entradas disponibles despu‚s
de la £ltima actualmente mostrada.
Para paginar el men£ hacia abajo, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_LINEWIDTHMGR.Gestor para el menu de grosor de l¡neas
Esta funci¢n no est  implementada todav¡a.

:h2 res=IDM_FILLCOLOR_MENU.Men£ color de  reas
Este men£ muestra todos los diferentes colores actualmente disponibles
para aplicarse al interior de textos vac¡os.
:p.El color actualmente en uso a la posici¢n del cursor est  marcado.
:p.V‚ase :link reftype=hd res=IDM_FILLCOLORS.Entradas al men£ color de  reas:elink.
para mas informaci¢n.

:h3 res=IDM_FILLCOLORS.Entrada al men£ color de  reas
Esta entrada del men£ muestra uno de los colores actualmente disponibles
para aplicarse al interior de textos vac¡os.
Si uno seleccione esta opci¢n, entonces el color ser  aplicado
sobre el bloque de texto actualmente seleccionado, o desde la posici¢n 
actual del cursor por adelante,

:h3 res=IDM_FILLCOLORS_PGUP.Paginar arriba el menu de color de  reas
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de colores para  reas a la vez, y hay mas entradas disponibles antes
de la primera actualmente mostrada.
Para paginar el men£ hacia arriba, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_FILLCOLORS_PGDN.Paginar abajo el menu de color de  reas
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de colores para  reas a la vez, y hay mas entradas disponibles despu‚s
de la £ltima actualmente mostrada.
Para paginar el men£ hacia abajo, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_FILLCOLORMGR.Gestor para el men£ de colores de  reas
Esta opci¢n del men£ causa que se muestre el 
:link reftype=hd res=IDD_FILLCOLORMGR.gestor para el men£ de colores de  reas:elink.,
lo cual da al usuario la habilidad de cambiar el men£ de colores de  reas.

:h4 res=IDD_FILLCOLORMGR.Gestor para el men£ de colores de  reas (di logo)
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

:h2 res=IDM_FILLTYPE_MENU.Men£ de tipos de  reas
Este men£ muestra todos los diferentes tipos de  reas actualmente disponibles
para aplicarse al interior de textos vac¡os.
:p.El tipo actualmente en uso a la posici¢n del cursor est  marcado.
:p.V‚ase :link reftype=hd res=IDM_FILLTYPES.Entradas del men£ de tipos de  reas:elink.
para mas informaci¢n.

:h3 res=IDM_FILLTYPES.Entrada del men£ de tipos de  reas
Esta entrada del men£ muestra uno de los tipos de  reas actualmente disponibles
para aplicarse al interior de textos vac¡os.
Si uno seleccione esta opci¢n, entonces el tipo ser  aplicado
sobre el bloque de texto actualmente seleccionado, o desde la posici¢n 
actual del cursor por adelante,

:h3 res=IDM_FILLTYPES_PGUP.Paginar arriba el menu de tipos de  reas
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de tipos para  reas a la vez, y hay mas entradas disponibles antes
de la primera actualmente mostrada.
Para paginar el men£ hacia arriba, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_FILLTYPES_PGDN.Paginar abajo el menu de tipos de  reas
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de tipos para  reas a la vez, y hay mas entradas disponibles despu‚s
de la £ltima actualmente mostrada.
Para paginar el men£ hacia abajo, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_FILLTYPEMGR.Gestor del menu de tipos de  reas
Esta funci¢n no est  implementada todav¡a.

:h2 res=IDM_LINEEND_MENU.Men£ de fines de l¡neas
Este men£ muestra todos los diferentes fines de l¡neas actualmente disponibles
para aplicarse a textos vac¡os y a l¡neas.
:p.El tipo actualmente en uso a la posici¢n del cursor est  marcado.
:p.V‚ase :link reftype=hd res=IDM_LINEENDS.Entradas del men£ de fines de l¡neas:elink.
para mas informaci¢n.

:h3 res=IDM_LINEENDS.Entrada del menu de fines de l¡neas
Esta entrada del men£ muestra uno de los fines de l¡neas actualmente disponibles
para aplicarse a textos vac¡os y a l¡neas.
Si uno seleccione esta opci¢n, entonces el tipo ser  aplicado
sobre el bloque de texto actualmente seleccionado, o desde la posici¢n 
actual del cursor por adelante,

:h3 res=IDM_LINEENDS_PGUP.Paginar arriba el menu de fines de l¡neas
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de fines de l¡neas a la vez, y hay mas entradas disponibles antes
de la primera actualmente mostrada.
Para paginar el men£ hacia arriba, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_LINEENDS_PGDN.Paginar abajo el menu de fines de l¡neas
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de fines de l¡neas a la vez, y hay mas entradas disponibles despu‚s
de la £ltima actualmente mostrada.
Para paginar el men£ hacia abajo, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_LINEENDMGR.Gestor del menu de fines de l¡neas
Esta funci¢n no est  implementada todav¡a.

:h2 res=IDM_LINEJOIN_MENU.Men£ de uniones de l¡neas
Este men£ muestra todos los diferentes uniones de l¡neas actualmente disponibles
para aplicarse a textos vac¡os y a l¡neas.
:p.El tipo actualmente en uso a la posici¢n del cursor est  marcado.
:p.V‚ase :link reftype=hd res=IDM_LINEJOINS.Entradas del men£ de uniones de l¡neas:elink.
para mas informaci¢n.

:h3 res=IDM_LINEJOINS.Entrada del menu de uniones de l¡neas
Esta entrada del men£ muestra uno de los uniones de l¡neas actualmente disponibles
para aplicarse a textos vac¡os y a l¡neas.
Si uno seleccione esta opci¢n, entonces el tipo ser  aplicado
sobre el bloque de texto actualmente seleccionado, o desde la posici¢n 
actual del cursor por adelante,

:h3 res=IDM_LINEJOINS_PGUP.Paginar arriba el menu de uniones de l¡neas
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de uniones de l¡neas a la vez, y hay mas entradas disponibles antes
de la primera actualmente mostrada.
Para paginar el men£ hacia arriba, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_LINEJOINS_PGDN.Paginar abajo el menu de uniones de l¡neas
Esta opci¢n del men£ est  disponible si no se puede mostrar todo
el men£ de uniones de l¡neas a la vez, y hay mas entradas disponibles despu‚s
de la £ltima actualmente mostrada.
Para paginar el men£ hacia abajo, hay que seleccionar esta opci¢n
del men£ con las flechas y pulsar ENTER, o hacer un click sobre
esta opci¢n.

:h3 res=IDM_LINEJOINMGR.Gestor del menu de uniones de l¡neas
Esta funci¢n no est  implementada todav¡a.

:h1 res=IDM_WINDOW_MENU.Men£ Ventana
:i1.Men£ Ventana
En este men£ hay varias funciones de manipulaci¢n de las
ventanas, as¡ como una lista de todos los documentos abiertos.
Las funciones disponibles son&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_TILE.Mosaico:elink.
:li.&sqbul.:link reftype=hd res=IDM_CASCADE.Cascada:elink.
:li.&sqbul.:link reftype=hd res=IDM_ARRANGEICONS.Ordenar iconos:elink.
:li.&sqbul.:link reftype=hd res=IDM_WINDOWS.Ventanas activas:elink.
:esl.

:h2 res=IDM_TILE.Mosaico (Opci¢n del men£)
Esta funci¢n ordena todos los documentos no minimizados de forma
que todos sean visibles y que todos los iconos de documentos sean
visibles en la parte inferior de la pantalla.

:h2 res=IDM_CASCADE.Cascada (Opci¢n del men£)
Esta funci¢n ordena todos los documentos no minimizados de forma
que todos sean tan grandes como sea posible y que su barra de
t¡tulo quede visible, as¡ como que todos los iconos de documentos
sean visibles en la parte inferior de la pantalla.

:h2 res=IDM_ARRANGEICONS.Ordenar iconos (Opci¢n del men£)
Esta funci¢n ordenar  todos los iconos de los documentos
minimizados en la parte inferior izquierda del  rea de trabajo.
Esta funci¢n se realiza implicitamente cuando se ejecutan las
funciones Cascada y Mosaico.

:h2 res=IDM_WINDOWS.Ventanas activas
Esta secci¢n del men£ de ventanas muestra una lista de los documentos
actualmente abiertos para revisi¢n.
Para seleccionar un documento, caus ndolo que se muestre delante de los
otros documentos abiertos, hay que hacer un click sobre una de las entradas,
o seleccionarla con las teclas flechas y pulsar ENTER.

:h1 res=IDM_HELP_MENU.Men£ Ayuda
:i1.Men£ Ayuda
Este men£ de ayuda cumple el formato est ndard SAA, con las
siguientes ¢rdenes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_HELP_INDEX.Indice:elink.
:li.&sqbul.:link reftype=hd res=IDM_EXTENDED_HELP.Ayuda general:elink.
:li.&sqbul.:link reftype=hd res=IDM_HELP_FOR_HELP.Utilizando la ayuda:elink.
:li.&sqbul.:link reftype=hd res=IDM_KEYS_HELP.Ayuda del teclado:elink.
:li.&sqbul.:link reftype=hd res=IDM_ABOUT_MENU.Men£ de informaci¢n de los productos:elink.
:esl.

:h2 res=IDM_HELP_INDEX.Indice (Opci¢n del men£)
Esta opci¢n visualizar  el ¡ndice del fichero de ayuda.

:h2 res=IDM_EXTENDED_HELP.Ayuda general (Opci¢n del men£)
Esta opci¢n visualizar  informaci¢n general sobre la aplicaci¢n.

:h2 res=IDM_HELP_FOR_HELP.Utilizando la ayuda (Opci¢n del men£)
Esta opci¢n visualizar  informaci¢n sobre el funcionamiento
b sico del Sistema de ayuda. Esta informaci¢n no es espec¡fica de
esta aplicaci¢n

:h2 res=IDM_KEYS_HELP.Ayuda del tectlado (Opci¢n del men£)
Las siguientes teclas han sido asignadas para su uso en el
procesador de textos, siempre dentro del marco del est ndard SAA CUA.
Existe una importante escasez de documentaci¢n sobre la materia.
:table cols='20 60' rules=both frame=box.
:row.:c.F1              :c.Mostrar informaci¢n
:row.:c.F2              :c.Grabar el documento
:row.:c.F3              :c.Salir
:row.:c.F4              :c.Grabar y cerrar el documento
:row.:c.F5              :c.Chequeo de ortograf¡a
:row.:c.F10             :c.Men£
:row.:c.Ctrl + Tab      :c.Pr¢ximo documento
:row.:c.Ctrl + A        :c.Abrir documento
:row.:c.Ctrl + I        :c.Imprimir el documento entero
:row.:c.Alt + I         :c.Imprimir la p gina actual
:row.:c.Ctrl + Z        :c.Zoom completo (o regresar al Zoom preferido)
:row.:c.Ctrl + Borrar   :c.Remover hasta el comienzo de la palabra actual.
:etable.
Adem s, hay una cantidad de teclas definidas para navegar dentro
del documento actual.  Para estas definiciones, v‚ase el t¢pico
:link reftype=hd res=60003.Editando documentos (Navegando):elink..

:h2 res=IDM_ABOUT_MENU.Men£ de informaci¢n de los productos (Submen£)
Si uno seleccione esta opci¢n de men£ directamente, la informaci¢n de
producto de este programa ser  mostrado.
:p.Ver :link reftype=hd res=IDD_ABOUT.Informaci¢n del producto (Di logo):elink.
para m s informaci¢n
:p.Si mas bien uno oprime al bot¢n del men£ condicional,
entonces un men£ mostrando los productos instalados ser 
mostrado, del cual se podr  seleccionar informaci¢n sobre
cualquier de los productos listados.

:h3 res=IDM_ABOUT.Entrada al men£ de informaci¢n de los productos (Opci¢n del men£)
Esta es una entrada al men£ de informaci¢n de los productos, representando
este program o uno los productos de extensi¢n instalados consigo.
Para ver la informaci¢n en s¡, hay que hacer un click sobre la opci¢n deseada.
(v‚ase :link reftype=hd res=IDD_ABOUT.Informaci¢n del producto (Di logo):elink. para mas informaci¢n).

:h4 res=IDD_ABOUT.Informaci¢n del producto (Di logo)
Esta caja de di logo visualiza el nombre del programa, su icono e
informaci¢n sobre el copyright. Para salir de este di logo,
pulsar la tecla ENTER, la barra de espacio o la tecla ESCAPE, o
hacer un click en el bot¢n "OK".
:artwork name='Espanol\About.bmp' align=center.

:h1 res=60009.Men£es de contexto
:i1.Men£es de contexto
Hay varios menues de contexto que se pueden usar por un solo &apos.click&apos. del
bot¢n derecho.  Juntos, estos menues contienen a£n mas funciones que hay
disponibles del los menues normales.
:p.Cuando se hace un click con el bot¢n derecho sobre el espacio borradora
principal del programa (no sobre una ventana de documento), entonces el
:link reftype=hd res=IDM_MAIN_CONTEXT.Men£ contexto principal:elink. se mostrar .
:p.Cuando sea el click en una ventana de documento, pero no en un  rea de
texto seleccionado, ni en una zona gr fica, entonces el
:link reftype=hd res=IDM_DEFAULTMENU.Men£ defecto:elink. se mostrar .
:p.Hacer un click en un bloque de texto seleccionado resultar  en el
:link reftype=hd res=IDM_TEXTMENU.Men£ de texto seleccionado:elink..
:p.Tambi‚n existe un 
:link reftype=hd res=IDM_BITMAPMENU.Men£ de gr ficos:elink.
y un 
:link reftype=hd res=IDM_BORDERMENU.Men£ de bordes:elink..

:h2 res=IDM_MAIN_CONTEXT.Men£ contexto principal
Este men£ se aparece cuando uno hace un click con el bot¢n derecho en un
 rea de la ventana principal del programa que no est‚ ocupado con una
ventana de documento.  El men£ contiene todas las opciones que son v lidas
fuera de un documento.  Estas opciones incluyen los siguientes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_REFRESH.Refrescar:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILE_MENU.Archivo:elink.
:li.&sqbul.:link reftype=hd res=IDM_WINDOW_MENU.Ventana:elink.
:li.&sqbul.:link reftype=hd res=IDM_HELP_MENU.Ayuda:elink.
:esl.

:h3 res=IDM_REFRESH.Refrescar (Opci¢n del men£)
Cuando se seleccione esta opci¢n del men£, toda la ventana se pinta de nuevo.

:h2 res=IDM_DEFAULTMENU.Men£ defecto
Este men£ aparece cuando uno hace un click con el bot¢n derecho en un  rea
de una ventana de documento que no es&colon.
(1) texto seleccionado,
(2) un gr fico,
(3) un borde de gr fico.
El men£ incluye opciones que se pueden aplicar al documento entero.
Estas opciones incluyen los siguientes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_REFRESH.Refrescar:elink.
:li.&sqbul.:link reftype=hd res=IDM_FILE_MENU.Archivo:elink.
:li.&sqbul.:link reftype=hd res=IDM_EDIT_MENU.Edici¢n:elink.
:esl.

:h2 res=IDM_TEXTMENU.Men£ de texto seleccionado
Este men£ aparece cuando uno hace un click con el bot¢n derecho en un  rea
de texto seleccionado.  El men£ incluye todas las opciones que se pueden
aplicar a texto seleccionado.  Estas opciones incluyen lo siguientes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_CUT.Cortar:elink.
:li.&sqbul.:link reftype=hd res=IDM_COPY.Copiar:elink.
:li.&sqbul.:link reftype=hd res=IDM_PASTE.Pegar:elink.
:li.&sqbul.:link reftype=hd res=IDM_CLEAR.Borrar:elink.
:li.&sqbul.:link reftype=hd res=IDM_LAYOUT_MENU.Disposici¢n:elink.
:li.&sqbul.:link reftype=hd res=IDM_FONT_MENU.Fuentes:elink.
:li.&sqbul.:link reftype=hd res=IDM_SIZE_MENU.Tama¤os:elink.
:li.&sqbul.:link reftype=hd res=IDM_ATTR_MENU.Atributos:elink.
:esl.
:note.El submenu de disposici¢n no contiene todas las opciones
que normalmente tiene, sino solo tales opciones que se pueden
aplicar a un bloque de texto seleccionado.  Estas excepciones
est n notadas dentro de las descripciones detalladas de cada
opci¢n del men£.

:h2 res=IDM_BITMAPMENU.Men£ de gr ficos
Este men£ aparece cuando uno hace un click con el bot¢n derecho en un  rea
gr fico.  Incluye todas las opciones del men£ que se pueden aplicar a un
gr fico.  Estas opciones incluyen los siguientes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_BACKGROUND.De fondo:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_NOTEXTASIDE.Sin texto al lado:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_HALFTONE.Medio tono:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_REVERSE.Color invertido:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_OPTIMIZE.Optimizado:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION.Posici¢n:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE.Tama¤o:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT.Aspecto:elink.
:esl.

:h3 res=IDM_BITMAP_BACKGROUND.De fondo (Opci¢n del men£)
Cuando se seleccione esta opci¢n del men£, el imagen gr fico es
marcado como parte del fondo de la p gina.  Texto normal puede pasar
por encima del gr fico.

:h3 res=IDM_BITMAP_NOTEXTASIDE.Sin texto al lado (Opci¢n del men£)
Cuando se seleccione esta opci¢n del men£, el imagen gr fico es marcado
de tal modo que el formateador de textos no permitir  que texto est‚
al lado del gr fico.  Puede estar por arriba, o por debajo en la p gina,
pero no a la izquierda ni a la derecha.

:h3 res=IDM_BITMAP_HALFTONE.Medio tono (Opci¢n del Men£)
Cuando se seleccione esta opci¢n del men£, el imagen gr fico es marcado
para ser pintado a medio tono, o sea, medio blanco.
Este es un atributo £til para gr ficos fijados para ser de fondo.
:note.Esta opci¢n no funciona para gr ficos del tipo &apos.Metafile&apos..

:h3 res=IDM_BITMAP_REVERSE.Color invertido (Opci¢n del Men£)
Cuando se seleccione esta opci¢n del men£, el imagen gr fico ser  marcado
para ser pintado en colores invertidos.
:note.Esta opci¢n no funciona para gr ficos del tipo &apos.Metafile&apos..

:h3 res=IDM_BITMAP_OPTIMIZE.Optimizado (Opci¢n del Men£)
Cuando se seleccione esta opci¢n del men£, el imagen gr fico se marca
para ser optimizado para la impresora en uso.  
Esto quiere decir que el tama¤o del imagen ser  calculado para la mejor
apariencia al imprimirse.
:note.Esta opci¢n no funciona para gr ficos del tipo &apos.Metafile&apos..

:h3 res=IDM_BITMAP_POSITION.Men£ de posici¢n gr fica
Desde este men£ se puede seleccionar un n£mero de opciones que afectan la
posici¢n de un gr fico en la p gina del documento.
Estas opciones incluyen los siguientes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_NEXT_CHAR.Pr¢xima car cter:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_NEXT_LINE.Pr¢xima l¡nea:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_LEFT.Costado izquierdo:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_LEFT.M rgen izquierda:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_CENTERED_X.Centrado (X):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_RIGHT.M gen derecha:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_RIGHT.Costado derecho:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_TOP.Costado superior:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_TOP.M rgen superior:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_CENTERED_Y.Centrado (Y):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_MARGIN_BOTTOM.M rgen inferior:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_POSITION_EDGE_BOTTOM.Costado inferior:elink.
:esl.
Si cualquiera de estas opciones est  en efecto actualmente para el gr fico 
seleccionado, ser  marcado.

:h4 res=IDM_BITMAP_POSITION_NEXT_CHAR.Poner gr fico a la pr¢xima car cter (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser  
tratado como un gran car cter metido en el texto del documento.  Dondequiera que est‚
puesto el c¢digo gr fico al documento, aparece el gr fico, ampliando el texto del documento
horizontal y verticalmente para acomodarse.

:h4 res=IDM_BITMAP_POSITION_NEXT_LINE.Poner gr fico a la pr¢xima l¡nea (Menu Option)
Esta funci¢n no est  implementada todav¡a.

:h4 res=IDM_BITMAP_POSITION_EDGE_LEFT.Poner gr fico al costado izquierdo (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser  
puesto al costado izquierdo de la p gina, sujeto a tama¤o actual de la p gina y las
distancias m¡nimas de los costados.  Si se seleccione otro formulario, entonces
la posici¢n del gr fico ser  ajustada seg£n el tama¤o nuevo de la p gina.

:h4 res=IDM_BITMAP_POSITION_MARGIN_LEFT.Poner gr fico a la m rgen izquierda (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser 
puesto a la m rgen izquierda de la p gina.  Si se cambien las m rgenes, entonces
la posici¢n del gr fico ser  ajustada seg£n las m rgenes nuevas.

:h4 res=IDM_BITMAP_POSITION_CENTERED_X.Poner gr fico centrado (X) (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser 
centrado horizontalmente.  Normalmente, ‚sta posici¢n se hace con respecto a las
m rgenes actuales.  Si la opci¢n de tama¤o para el gr fico ha sido fijado para
:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Costado-a-costado (X):elink.,
entonces el centrado se har  con respecto a los costados actuales de la p gina.

:h4 res=IDM_BITMAP_POSITION_MARGIN_RIGHT.Poner gr fico a la m rgen derecha (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser 
puesto a la m rgen derecha de la p gina.  Si se cambien las m rgenes, entonces
la posici¢n del gr fico ser  ajustada seg£n las m rgenes nuevas.

:h4 res=IDM_BITMAP_POSITION_EDGE_RIGHT.Poner gr fico al costado derecho (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser 
puesto al costado derecho de la p gina.  Si se cambie el formulario (el tipo de papel)
entonces la posici¢n del gr fico ser  adjustada seg£n el tama¤o nuevo de la p gina.

:h4 res=IDM_BITMAP_POSITION_EDGE_TOP.Poner gr fico al costado superior (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser 
puesto al costado superior de la p gina.  Si se cambie el formulario (el tipo de papel)
entonces la posici¢n del gr fico ser  adjustada seg£n el tama¤o nuevo de la p gina.

:h4 res=IDM_BITMAP_POSITION_MARGIN_TOP.Poner gr fico a la m rgen superior (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser 
puesto a la m rgen superior de la p gina.  Si se cambien las m rgenes, entonces
la posici¢n del gr fico ser  ajustada seg£n las m rgenes nuevas.

:h4 res=IDM_BITMAP_POSITION_CENTERED_Y.Poner gr fico centrado (Y) (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser 
centrado verticalmente.  Normalmente, ‚sta posici¢n se hace con respecto a las
m rgenes actuales.  Si la opci¢n de tama¤o para el gr fico ha sido fijado para
:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Costado-a-costado (Y):elink.,
entonces el centrado se har  con respecto a los costados actuales de la p gina.

:h4 res=IDM_BITMAP_POSITION_MARGIN_BOTTOM.Poner gr fico a la m rgen inferior (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser 
puesto a la m rgen inferior de la p gina.  Si se cambien las m rgenes, entonces
la posici¢n del gr fico ser  ajustada seg£n las m rgenes nuevas.

:h4 res=IDM_BITMAP_POSITION_EDGE_BOTTOM.POner gr fico al costado inferior (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser 
puesto al costado inferior de la p gina.  Si se cambie el formulario (el tipo de papel)
entonces la posici¢n del gr fico ser  adjustada seg£n el tama¤o nuevo de la p gina.

:h3 res=IDM_BITMAP_SIZE.Men£ de tama¤os gr ficos
Desde este men£ se puede seleccionar una variedad de opciones que afectan el tama¤o
de los gr ficos en la p gina.  Estas opciones incluyen los siguientes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Costado-a-costado (X):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_MARGINTOMARGIN_X.M rgen-a-M rgen (X):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Costado-a-costado (Y):elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_SIZE_MARGINTOMARGIN_Y.M rgen-a-M rgen (Y):elink.
:esl.
Si cualquier de estas opciones est n de fuerza para el gr fico seleccionado, entonces
ser n marcados.

:h4 res=IDM_BITMAP_SIZE_EDGETOEDGE_X.Estirar gr fico de costado a costado (X) (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser  cambiado
de tama¤o para que ocupe todo lo ancho de la p gina, de costado a costado, a menos que el
gr fico este tambi‚n marcado para preservar su aspecto (v‚ase 
:link reftype=hd res=IDM_BITMAP_ASPECT.Men£ de aspecto gr fico:elink.).  
En tal caso, es posible que el gr fico ser  algo mas angosto.

:h4 res=IDM_BITMAP_SIZE_MARGINTOMARGIN_X.Estirar gr fico de m rgen a m rgen (X) (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser  cambiado
de tama¤o para que ocupe la p gina horizontalmente, de m rgen a m rgen, a menos que el
gr fico este tambi‚n marcado para preservar su aspecto (v‚ase 
:link reftype=hd res=IDM_BITMAP_ASPECT.Men£ de aspecto gr fico:elink.).  
En tal caso, es posible que el gr fico ser  algo mas angosto.

:h4 res=IDM_BITMAP_SIZE_EDGETOEDGE_Y.Estirar gr fico de costado a costado (Y) (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser  cambiado
de tama¤o para que ocupe todo lo alto de la p gina, de costado a costado, a menos que el
gr fico este tambi‚n marcado para preservar su aspecto (v‚ase 
:link reftype=hd res=IDM_BITMAP_ASPECT.Men£ de aspecto gr fico:elink.).  
En tal caso, es posible que el gr fico ser  algo menos alto.

:h4 res=IDM_BITMAP_SIZE_MARGINTOMARGIN_Y.Estirar gr fico de m rgen a m rgen (Y) (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado ser  cambiado
de tama¤o para que ocupe la p gina verticalmente, de m rgen a m rgen, a menos que el
gr fico este tambi‚n marcado para preservar su aspecto (v‚ase 
:link reftype=hd res=IDM_BITMAP_ASPECT.Men£ de aspecto gr fico:elink.).  
En tal caso, es posible que el gr fico ser  algo menos alto.

:h3 res=IDM_BITMAP_ASPECT.Men£ de aspecto gr fico
Desde este men£ se puede seleccionar si es que un gr fico ser 
retenido a proporciones fijas, y de cual manera.
Las opciones incluyen los siguientes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FLOAT.No fijo:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FIX_X.Fijo X:elink.
:li.&sqbul.:link reftype=hd res=IDM_BITMAP_ASPECT_FIX_Y.Fijo Y:elink.
:esl.
Si cualquier de estas opciones est‚ en efecto actualmente para el
gr fico seleccionado, entonces ser n marcados.

:h4 res=IDM_BITMAP_ASPECT_FLOAT.Aspecto gr fico no fijo (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado
ser  librado del requisito de tener un aspecto fijo (de uno por uno). 
Normalmente, esto no tendr¡a efecto inmediato y visible sobre el gr fico,
a menos que tambi‚n tiene una de las opciones para tama¤o de costado-a-costado
o de m rgen-a-m rgen en efecto, en cual caso el gr fico puede cambiarse de
tama¤o.

:h4 res=IDM_BITMAP_ASPECT_FIX_X.Aspecto gr fico fijo en X (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado
ser  fijado de tal modo que su tama¤o vertical depender  de su medida horizontal,
con tal que se preserve su aspecto (proporcion) a uno por uno.

:h4 res=IDM_BITMAP_ASPECT_FIX_Y.Aspecto gr fico fijo en Y (Opci¢n de men£)
Cuando uno seleccione ‚sta opci¢n del men£, entonces el gr fico seleccionado
ser  fijado de tal modo que su tama¤o horizontal depender  de su medida vertical,
con tal que se preserve su aspecto (proporcion) a uno por uno.

:h2 res=IDM_BORDERMENU.Men£ de bordes
Este men£ aparece cuando se hace un click con el bot¢n derecho en el  rea
de border alrededor de un imagen gr fico.
El men£ incluye tales opciones que se pueden aplicar a tal  rea.
Estas opciones incluyen los siguientes&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_SIZE.Tama¤o del borde...:elink.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_COLOR.Color del borde...:elink.
:li.&sqbul.:link reftype=hd res=IDM_BORDER_CAPTION.Tipo...:elink.
:esl.

:h3 res=IDM_BORDER_SIZE.Tama¤o del borde... (Opci¢n del men£)
Esta funci¢n no est  implementada todav¡a.

:h3 res=IDM_BORDER_COLOR.Color del borde... (Opci¢n del men£)
Esta funci¢n no est  implementada todav¡a.

:h3 res=IDM_BORDER_CAPTION.Tipo... (Opci¢n del men£)
Esta funci¢n no est  implementada todav¡a.

:h1 res=60010.Di logos miscelaneos

:h2 res=IDD_PROFILE_PATH.Directorio del fichero .INI
Este di logo se visualiza cuando el programa no puede encontrar
su fichero .INI y pregunta al usuario d¢nde debe crearlo.
:artwork name='Espanol\Profile.bmp' align=center.
:p.S¢lo se accepta el nombre de un directorio existente en el disco.
Una vez introducido, pulsar la telca ENTER o hacer un click en "OK".
:p.Si se desea abortar la inicializaci¢n del programa, pulsar la
tecla ESC o hacer un click en el bot¢n de cancelar.

:h2 res=IDD_PROMPT_SAVE.¨Deseas grabar documento cambiado?
.* :artwork name='Espanol\SaveChng.bmp' align=center.
Un atento se hizo para salir del programa o cerrar un documento,
pero hab¡an cambios al documento que no fueron grabados todavia.
Hay que contestar &apos.S&apos.¡ o hacer un click en el bot¢n de &apos.S¡&apos. si quiere
grabar los cambios al fichero actual del documento.
Hay que contestar &apos.N&apos.o o hacer un click en el bot¢n de &apos.No&apos. si no quiere
hacer esto (o sea que no le importan los cambios).
Hay que pulsar la tecla ESC o hacer un click en el bot¢n de cancelar
si quiere parar la terminaci¢n del programa o la cerrada del documento.

:h2 res=IDD_PROMPT_SAVECONFIRM1.Confirmar grabaci¢n
Si aparece este mensaje significa que est s intentando grabar el
documento con un nuevo nombre, sobre un fichero ya existente. 
.* :artwork name='Espanol\SaveConf.bmp' align=center.
:p.Si deseamos reemplazar el fichero, escoger la opci¢n afirmativa. En
caso contrario, escoger la opci¢n negativa. En este £ltimo caso
el documento no se grabar  y deber s introducir un nuevo nombre.

:h2 res=IDD_PROMPT_SAVECONFIRM2.Confirmar cambio de tipo
Si aparece este mensaje significa que est s intentando grabar el
document con un tipo cambiado.
.* :artwork name='Espanol\SaveCnf2.bmp' align=center.
:p.Si deseamos cambiar el tipo del fichero, escoger la opci¢n afirmativa.
En caso contrario, escoger la opci¢n negativa.  En este £ltimo caso
el documento no se grabar  y deber s introducir un nuevo nombre.

:h2 res=IDD_MESSAGE_SPELLDONE.Chequeo de ortograf¡a completo
Si se ve este di logo, entonces acaba de terminar la revisi¢n del documento
por el chequeador de ortograf¡a.
.* :artwork name='Espanol\SpelDone.bmp' align=center.
:p.Para seguir adelante, hay que pulsar la tecla ENTER o hacer un click
en el bot¢n de ENTER.

:h2 res=IDD_ERROR_HELPMANAGER.Error de gestor de ayudas
Si se ve este mensaje, entonces algo ha pasado con el fichero de ayuda
del programa.  
Por favor notif¡quele al programador por uno de los modos listados en 
:link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..

:h2 res=IDD_ERROR_EXCEPTION.Excepci¢n del programa
.* :artwork name='Espanol\Except.bmp' align=center.
Si se ve este mensaje, entonces Escriba ha encontrado un error fatal.
Por favor notif¡quele al programador por uno de los modos listados en 
:link reftype=hd res=60008.Preguntas y Reportar Problemas:elink.
y incluye el fichero &apos.ESCRIBA.LOG&apos. que se encontrar  en el
directorio donde se instal¢ Escriba.

:h2 res=IDD_ERROR_WINREGISTERCLASS.Error en registrar clase
Si se ve este mensaje, entonces Escriba no pudo registrar una clase
de ventanas por alguna raz¢n.
Por favor notif¡quele al programador por uno de los modos listados en 
:link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..

:h2 res=IDD_ERROR_CREATECLIENT.Error en crear cliente
Si se ve este mensaje, entonces Escriba no pudo crear una ventana cliente
por alguna raz¢n.
Por favor notif¡quele al programador por uno de los modos listados en 
:link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..

:h2 res=IDD_ERROR_TOOMANYDOCUMENTS.Error de demasiados documentos
Si se ve este mensaje, entonces el usuario ha tratado de cargar
demasiados documentos al programa, y el error fue detectado
despu‚s que el documento fue cargado a la memoria, mientras estaba
tratando de crearle una ventana.

:h2 res=IDD_ERROR_TOOMANYLOADED.Error de demasiados documentos ya cargados
Si se ve este mensaje, entonces el usuario ha tratado de cargar
demasiados documentos al programa, y el error fue detectado antes
de tratar de cargar el documento a la memoria.

:h2 res=IDD_ERROR_DOCUMENT_CREATE.Error en crear documento
Si se ve este mensaje, entonces Escriba no pudo crear un objeto de documento
por alguna raz¢n.
Por favor notif¡quele al programador por uno de los modos listados en 
:link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..

:h2 res=IDD_ERROR_DOCUMENT_LOAD.Error en cargar documento
Si se ve este mensaje, entonces Escriba no pudo cargar un documento por
alguna raz¢n.  
Por favor verif¡que el nombre del fichero y los nombres de todos los 
directorios del camino al fichero.
Tambi‚n es posible que el fichero est  actualmente en uso por otro programa 
y por esa raz¢n no se pudo abrirlo para leer y cargar.

:h2 res=IDD_ERROR_DOCUMENT_SAVE.Error en grabar documento
Si se ve este mensaje, entonces Escriba no pudo abrir el fichero al cual
deb¡a de grabar un documento.  
Por favor verif¡que el nombre del fichero y los nombres de todos los
directorios en el camino al fichero.
Tambi‚n es posible que el fichero est  actualmente en uso por otro programa 
y por esa raz¢n no se pudo abrirlo para escribir.

:h2 res=IDD_ERROR_CREATEPRINTSTATUS.Error en crear ventana del estado de imprimir
Si se ve este mensaje, entonces Escriba no pudo crear su ventana para
mostrar el estado de imprimir.  Esto nunca debe pasar, pero este
mensaje existe para prop¢sitos de diagn¢sticas.
Por favor notif¡quele al programador por uno de los modos listados en 
:link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..

:h2 res=IDD_ERROR_CREATELOADSTATUS.Error en crear ventana del estado de cargar
Si se ve este mensaje, entonces Escriba no pudo crear su ventana para
mostrar el estado de cargar.  Esto nunca debe pasar, pero este
mensaje existe para prop¢sitos de diagn¢sticas.
Por favor notif¡quele al programador por uno de los modos listados en 
:link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..

:h2 res=IDD_ERROR_BLOCKMEMORY.Error en conseguir memoria para copiar
Si se ve este mensaje, entonces no hubo memoria disponible para cortar
o copiar el texto seleccionado al clipboard del sistema.
Por favor trate de hacer la edici¢n en porciones mas peque¤as.

:h2 res=IDD_ERROR_BADGRAPHICFILE.Fichero gr fico inv lido
Si se ve este mensaje, entonces hab¡a un atento de cargar un gr fico al
programa que conten¡a datos inv lidas o no reconocibles.
Los tipos de ficheros gr ficos que actualmente se apoyan son
BMP, GIF, JPG, PCX, PCC, MET y TIF.
Si el fichero que Ud. trat¢ de cargar tiene uno de estos tipos,
y se puede mostrar el imagen con un programa normal de mostrar tales
imagenes, entonces
por favor notif¡quele al programador por uno de los modos listados en
:link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..

:h2 res=IDD_ERROR_VERIFY1.Error de verificaci¢n - Abrir
Si se muestra este mensaje, entonces el fichero especificado
para cargar no se pudo abrir por alguna raz¢n.
Verifique el nombre y el directorio del archivo y trate de nuevo.

:h2 res=IDD_ERROR_VERIFY2.Error de verificaci¢n - Memoria
Si se muestra este mensaje, entonces el fichero especificado
no se pudo cargar a la memoria por falta de memoria.

:h2 res=IDD_ERROR_VERIFY3.Error de verificaci¢n - Escapes inv lidos
Si se muestra este mensaje, entonces el fichero especificado
para cargar en el formato nativo de Escriba contuvo c¢digos de escape
inv lidos.
Puede ser que el fichero era corrumpido.

:h2 res=IDD_ERROR_VERIFY4.Error de verificaci¢n - Funciones inv lidas
Si se muestra este mensaje, entonces el fichero especificado
para cargar en el formato nativo de Escriba contuvo c¢digos de funci¢n
no conocidos por esta revisi¢n de Escriba.
Puede ser que el fichero fue creado por una revisi¢n mas nuevo
de Escriba, o bien puede ser que el fichero era corrumpido.

:h2 res=IDD_ERROR_VERIFY5.Error de verificaci¢n - Car cteres nulos
Si se muestra este mensaje, entonces el fichero especificado
para cargar en el formato nativo de Escriba contuvo car cteres nulos
fuera de los c¢digos normales de funci¢n.
Puede ser que el fichero era corrumpido.

:h2 res=IDD_ERROR_VERIFY6.Error de verificaci¢n - Encabecimientos disparejas
Si se muestra este mensaje, entonces el fichero especificado
para cargar en el formato nativo de Escriba contuvo encabecimientos
de funciones disparejas.
Puede ser que el fichero era corrumpido.

:h2 res=IDD_ERROR_VERIFY7.Error de verificaci¢n - Falta de preface
Si se muestra este mensaje, entonces el fichero especificado
para cargar en el formato nativo de Escriba no comenz¢ con una
funci¢n de 'Preface'.
Puede ser que el fichero era corrumpido.

:h2 res=IDD_ERROR_VERIFY8.Error de verificaci¢n - Ning£n tipo v lido
Si se muestra este mensaje, entonces el fichero especificado
para cargar no fue aceptado por ning£no de los verificadores de
tipos de ficheros.  Escriba no puede cargar tales ficheros.
:note.Nuevos procesadores de tipos de fichero se puede desarollar
en cualquier momento.  Si Ud. tiene un tipo de fichero que quiere
cargar a Escriba, pero actualmente no puede, entonces sirvase enviar
su pedido al autor a trav‚s de unas de las direcciones especificas
en la p gina :link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..

:h2 res=IDD_ERROR_IMPORT1.Error de importaci¢n - Abrir
Si se muestra este mensaje, entonces el fichero especificado
para cargar no se pudo abrir por alguna raz¢n.
Verifique el nombre y el directorio del archivo y trate de nuevo.

:h2 res=IDD_ERROR_IMPORT2.Error de importaci¢n - Memoria
Si se muestra este mensaje, entonces el fichero especificado
no se pudo cargar a la memoria por falta de memoria.

:h2 res=IDD_ERROR_EXPORT1.Error de exportaci¢n - Abrir / Crear
Si se muestra este mensaje, entonces el fichero especificado
para grabar no se pudo abrir por alguna raz¢n.
Verifique el nombre y el directorio del archivo y trate de nuevo.
Si el fichero ya existe, entonces es posible que ha sido
marcado para leerse no mas.

:h2 res=IDD_ERROR_EXPORT2.Error de exportaci¢n - Espacio
Si se muestra este mensaje, entonces el fichero especificado
no se pudo grabar enteramente al disco por alguna raz¢n.
Verifique que haya espacio suficiente en el disco destinaci¢n.

:h2 res=IDD_METAFILE_VERIFY1.Error de verificar 'Metafile' - Abrir
Si se muestra este mensaje, entonces el metafile seleccionado para
verificarse o cargarse no se pudo abrir.  Normalmente este quiere
decir que el fichero est  actualmente abierto para escribir por
una otra aplicaci¢n.

:h2 res=IDD_METAFILE_VERIFY2.Error de verificar 'Metafile' - Fichero corto
Si se muestra este mensaje, entonces el metafile seleccionado para
verificarse y cargarse no conten¡a suficiente para verificar.

:h2 res=IDD_METAFILE_VERIFY3.Error de verificar 'Metafile' - No su pudo cargar
Si se muestra este mensaje, entonces no se pudo cargar el metafile
a la memoria para verificarlo.  Si este error ocurre repitidamente, 
entonces por favor notif¡quele al programador por uno de los modos listados en
:link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..
en la p gina :link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..

:h2 res=IDD_METAFILE_VERIFY4.Error de verificar 'Metafile' - Datos inv lidos
Si se muestra este mensaje, entonces el comienzo del metafile contuvo
informaci¢n inv lida.  Por favor verifique que el metafile puede verse
o cargarse con el program de OS/2 'PICVIEW'.  Si se puede cargar y ver
por ese programa sin errores,
entonces por favor notif¡quele al programador por uno de los modos listados en
:link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..
en la p gina :link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..

:h2 res=IDD_ERROR_LOADLANGUAGE.Error de seleccionar idioma
Si se muestra este mensaje, entonces el idioma seleccionado para cargarse
no se pudo cargarse por completo por alguna raz¢n.
Por favor notif¡quele al programador por uno de los modos listados en
:link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..
en la p gina :link reftype=hd res=60008.Preguntas y Reportar Problemas:elink..

:h1 res=61000.Referencia T‚cnica

:h2 res=61001.Formato del fichero de documento
El fichero de documento se puede considerar como una hilera de car cteres
y c¢digos de funciones.  Con la excepci¢n de los c¢digos 15, 16 y 17,
toda la informaci¢n se graba sin cambio.  Los c¢digos 15, 16 y 17 son
escapados, con el c¢digo 15 siendo el c¢digo de escape.  Eso es, si la
informaci¢n original conten¡a un c¢digo 16 literal, entonces el fichero
contendr¡a un c¢digo 15 seguido por un c¢digo 16 para representar el
c¢digo 16 original.  Si un c¢digo 15 se encuentre sin un c¢digo 15, 16 o 17
despu‚s, entonces ser  interpretado como un c¢digo 15 literal.
:p.
Un c¢digo 17 literal &apos.&larrow.&apos. representa el comienzo de un c¢digo de funci¢n.
Un c¢digo 16 literal &apos.&rarrow.&apos. representa el fin de un c¢digo de funci¢n.
Si el fin de un c¢digo de funci¢n se encuentra sin haber tenido un comienzo
correspondiente, entonces se tratar  como un car cter literal.
:p.
Los siguientes funciones son v lidos actualmente&colon.

:lm margin=3.:p.&larrow.Version&colon.:hp5.n£mero_de_revisi¢n:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica la revisi¢n de Escriba usada para revisar
y grabar el documento al fichero.  El n£mero de revisi¢n tiene cuatro d¡gitos,
los cuales se tratan como si tuvieron un punto decimal entre los primeros dos
y los £ltimos dos.  Revisi¢n 1.00 ser¡a representada como 0100.
Este c¢digo de funci¢n se usa para permitir conversi¢n de formatos de documentos
de formatos anteriores al actual, y no exist¡a en los formatos anteriores
a la revisi¢n &apos.beta&apos. 0.92.
Este c¢digo no puede aparecer afuera del  rea par metro del 
c¢digo de funci¢n &apos.Preface&apos..

:lm margin=3.:p.&larrow.Preface&colon.:hp5.c¢digos_de_funci¢n:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo especial de funci¢n aparece en un documento grabado, pero no
en el documento como se ve por el usuario para revisi¢n.
El  rea de par metros de esta funci¢n contiene otros c¢digos de funci¢n.
El efecto combinado de todos los c¢digos de funci¢n es para hacer el estado
inicial del documento.
Todos los c¢digos de funci¢n que cambian el estado de un documento son v lidos ac ,
juntos con las funciones Printer, Form, JobSettings, TabSpacing y Metric, que no
son v lidos en otras partes del documento.
C¢digos de funci¢n que generen texto (PageCount, PageNumber, Graphic, Date) no
son v lidos dentro del Preface.

:lm margin=3.:p.&larrow.Printer&colon.:hp5.nombre:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica el nombre de la impresora para el cual
fue formateado el documento.  
Este c¢digo no puede aparecer afuera del  rea par metro del 
c¢digo de funci¢n &apos.Preface&apos..

:lm margin=3.:p.&larrow.Form&colon.:hp5.nombre:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica el nombre del formulario (tama¤o de papel)
para el cual fue formateado el documento.  
Solo se usa este c¢digo cuando un documento formateado para una impresora
se reformatea para otra impresora.
Este c¢digo no puede aparecer afuera del  rea par metro del 
c¢digo de funci¢n &apos.Preface&apos..

:lm margin=3.:p.&larrow.CodePage&colon.:hp5.number:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica el n£mero de la p gina de c¢digos para
usarse en mostrar este documento.
Este c¢digo no puede aparecer afuera del  rea par metro del 
c¢digo de funci¢n &apos.Preface&apos..
V‚ase :link reftype=hd res=61002.P ginas de C¢digos:elink. para mas informaci¢n.

:lm margin=3.:p.&larrow.Font&colon.:hp5.nombre:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica el nombre de la fuente para usarse desde
su posici¢n en el documento para adelante.  Este nombre tiene que ser
de alg£n fuente que no sea de format &apos.bitmap&apos., y tiene que aparecer
en el objeto sistema de fuentes, en un di logo normal de fuentes, o
en el men£ de fuentes de Escriba.

:lm margin=3.:p.&larrow.Size&colon.:hp5.tama¤o_en_puntos:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica el tama¤o de car cteres para usarse desde
su posici¢n en el documento para adelante.
Se especifica en las unidades que se conocen como puntos tipogr ficos, los
cuales miden 1/72 de una pulgada cada uno. Cuando el tama¤o sea de 12 puntos,
por ejemplo, el tama¤o normal de las letras del fuente ser  1/6 de una pulgada,
aunque esta regla es una gu¡a para dese¤adores de fuentes, y no es una regla
fija.  Por lo general, fuentes no aparecer n todos de igual tama¤o aunque est‚n 
todos del mismo tama¤o de puntos.  Escriba acepta estos tama¤os a una resoluci¢n
de 1/10 punto.

:lm margin=3.:p.&larrow.Color&colon.:hp5.color:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica el color (rojo-verde-azul) para utilizarse para
textos y l¡neas desde su posici¢n en el documento para adelante.
El par metro de este c¢digo se especifica en seis d¡gitos de base 16, con los
primeros dos d¡gitos dando el componente rojo, los pr¢ximos dos lo verde,
y los £ltimos dos lo azul.  Blanco puro, por ejemplo, ser¡a &apos.FFFFFF&apos.,
negro ser¡a &apos.000000&apos., y amarillo ser¡a &apos.FFFF00&apos..

:lm margin=3.:p.&larrow.LineWidth&colon.:hp5.grosor:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica el grosor para utilizarse en dibujar letras
vac¡as y l¡neas.  El grosor se especifica como cualquier otra distancia.

:lm margin=3.:p.&larrow.LineEnd&colon.:hp5.tipo:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica fija el tipo de fin de l¡nea para usarse para
dibujar letras vac¡as y l¡neas desde su posici¢n en el documento para adelante.
Los tipos v lidos incluyen todos los valores &apos.LINEEND&apos. definidos
en la &apos.Referencia para Programadores de OS/2&apos..

:lm margin=3.:p.&larrow.LineJoin&colon.:hp5.tipo:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica fija el tipo de juntada de l¡neas para usarse para
dibujar letras vac¡as y l¡neas desde su posici¢n en el documento para adelante.
Los tipos v lidos incluyen todos los valores &apos.LINEJOIN&apos. definidos
en la &apos.Referencia para Programadores de OS/2&apos..

:lm margin=3.:p.&larrow.FillColor&colon.:hp5.color:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica el color (rojo-verde-azul) para utilizarse para
llenar textos vac¡os desde su posici¢n en el documento para adelante.
El c¢digo se especifica de igual manera como el color para l¡neas (v‚ase arriba).

:lm margin=3.:p.&larrow.FillType&colon.:hp5.tipo:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica el tipo de  rea para utilizarse para
llenar textos vac¡os desde su posici¢n en el documento para adelante.
Los tipos v lidos incluyen todos los c¢digos &apos.PATSYM&apos. definidos 
en la &apos.Referencia para Programadores de OS/2&apos..

:lm margin=3.:p.&larrow.Bold&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n prende o apague el atributo de negrito
para el texto desde su posici¢n en el documento para adelante.

:lm margin=3.:p.&larrow.Italic&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n prende o apague el atributo de cursivo
para el texto desde su posici¢n en el documento para adelante.

:lm margin=3.:p.&larrow.Underscore&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n prende o apague el atributo de subrayado
para el texto desde su posici¢n en el documento para adelante.

:lm margin=3.:p.&larrow.Strikeout&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n prende o apague el atributo de tachado
para el texto desde su posici¢n en el documento para adelante.

:lm margin=3.:p.&larrow.Outline&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n prende o apague el atributo de vac¡o
para el texto desde su posici¢n en el documento para adelante.

:lm margin=3.:p.&larrow.Caps&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n prende o apague el atributo de may£scula
para el texto desde su posici¢n en el documento para adelante.

:lm margin=3.:p.&larrow.Expand&colon.:hp5.porcentaje:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija la expansi¢n horizontal para ser aplicado
a textos desde su posici¢n el el documento para adelante.
Este factor se expresa como un porcentaje, con ci‚n indicando que
la expansi¢n normal definido para la fuente actual ser  usado.

:lm margin=3.:p.&larrow.TopMargin&colon.:hp5.distancia:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija el m rgen de arriba a la distancia especificada,
lo cual se mide desde el fondo de la p gina (desde abajo).
Este atributo toma efecto en la pr¢xima p gina, a menos que nada que se
podr¡a imprimir o desplegar ha aparecido en la p gina hasta ahora.

:lm margin=3.:p.&larrow.BottomMargin&colon.:hp5.distancia:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija el m rgen de abajo a la distancia especificada,
lo cual se mide desde el fondo de la p gina (desde abajo).
Este atributo toma efecto en la pr¢xima p gina, a menos que nada que se
podr¡a imprimir o desplegar ha aparecido en la p gina hasta ahora.

:lm margin=3.:p.&larrow.LeftMargin&colon.:hp5.distancia:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija el m rgen de la izquierda a la distancia especificada,
lo cual se mide desde el lado izquierdo de la p gina.
Este atributo toma efecto en la pr¢xima l¡nea, a menos que nada que se
podr¡a imprimir o desplegar ha aparecido en la l¡nea hasta ahora.

:lm margin=3.:p.&larrow.RightMargin&colon.:hp5.distancia:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija el m rgen de la derecha a la distancia especificada,
lo cual se mide desde el lado izquierdo de la p gina.
Este atributo toma efecto en la pr¢xima l¡nea, a menos que nada que se
podr¡a imprimir o desplegar ha aparecido en la l¡nea hasta ahora.

:lm margin=3.:p.&larrow.LeftJustify&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija la justificaci¢n horizontal a la izquierda,
causando que la primera palabra de cada l¡nea est‚ alineado a la m rgen
izquierda, a menos que haya una tabulaci¢n antes de la palabra.
Este atributo toma efecto en la pr¢xima l¡nea, a menos que nada que se
podr¡a imprimir o desplegar ha aparecido en la l¡nea hasta ahora.

:lm margin=3.:p.&larrow.CenterJustify&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija la justificaci¢n horizontal a centrado,
causando que todas la l¡neas de texto estar  colocados de con m rgenes 
horizontales iguales.
Este atributo toma efecto en la pr¢xima l¡nea, a menos que nada que se
podr¡a imprimir o desplegar ha aparecido en la l¡nea hasta ahora.

:lm margin=3.:p.&larrow.RightJustify&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija la justificaci¢n horizontal a la derecha,
causando que la £ltima palabra de cada l¡nea est‚ alineado a la m rgen
derecha.
Este atributo toma efecto en la pr¢xima l¡nea, a menos que nada que se
podr¡a imprimir o desplegar ha aparecido en la l¡nea hasta ahora.

:lm margin=3.:p.&larrow.FullJustify&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija la justificaci¢n horizontal a completo,
causando que l¡neas llenas de texto tendr n m rgenes iguales.
L¡neas que no son llenas son alineados a la izquierda.
Este atributo toma efecto en la pr¢xima l¡nea, a menos que nada que se
podr¡a imprimir o desplegar ha aparecido en la l¡nea hasta ahora.

:lm margin=3.:p.&larrow.CenterPage&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija el estado actual del atributo para centrar p ginas
verticalmente, que sea activo o no activo.
Este atributo toma efecto en la pr¢xima p gina, a menos que nada que se
podr¡a imprimir o desplegar ha aparecido en la p gina hasta ahora.

:lm margin=3.:p.&larrow.TabSet&colon.:hp5.distancia:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n causa que se fije un tabulador para una distancia
fija horizontal, respecto al lado izquierdo de la p gina.

:lm margin=3.:p.&larrow.TabClear&colon.:hp5.distancia:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n causa que se remueva el tabulador para una distancia
fija horizontal, respecto al lado izquierdo de la p gina.

:lm margin=3.:p.&larrow.TabSpacing&colon.:hp5.distancia:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija la distancia para usar para generar las tabulaciones
iniciales del documento, comenzando con la m rgen izquierda y repetiendo
en incrementos de esta distancia hasta la m rgen derecha.
Este c¢digo no puede aparecer afuera del  rea par metro del 
c¢digo de funci¢n &apos.Preface&apos..

:lm margin=3.:p.&larrow.Indent&colon.:hp5.distancia:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija la distancia para indentaci¢n.
Cuando una l¡nea sea la primera del documento, o la primera despu‚s
del fin de un p rafo, comenzar n esta distancia a la derecha de la
m rgen izquierda.  Si sea esta distancia negativa, entonces comenzar n
a la izquierda de la m rgen izquierda actual.  
Este tratamiento no afecta a otras l¡neas de texto.

:lm margin=3.:p.&larrow.Metric&colon.:hp5.1/0:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n indica si el documento ser  formateado con medidas
m‚tricas (cent¡metros), o inglesas (pulgadas).  Si el par metro sea &apos.1&apos.,
entonces se usar n cent¡metros.  De otro modo se usar n pulgadas.
Este c¢digo no puede aparecer afuera del  rea par metro del 
c¢digo de funci¢n &apos.Preface&apos..

:lm margin=3.:p.&larrow.Graphic&colon.:hp5.x,y,cx,cy[,Background][,NoTextAside][,Halftone][,Reverse][,PrinterOptimize][[,KeepAspectFixX|KeepAspectFixY]][,BorderWidth=n][,BorderColor=rrvvaa][,BorderCaption="texto"]]&colon.tipo&bsl.0datos:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n establece un  rea gr fico dentro del documento.
:p.Los par metros &apos.x&apos. y &apos.y&apos. determinan d¢nde estar 
la esquina izquierda y arriba del  rea, relativo a la esquina izquierda
y arriba de la p gina.  Documentos guardados antes de la revisi¢n 0.97
especifican sus gr ficos relativos a la esquina de la izquierda y por abajo.
Los par metros &apos.cx&apos. y &apos.cy&apos. especifican cu n ancho y alto ser .
Los cuatro par metros se especifican en el sistema de medida actualmente
en uso (1/1000 pulgada o cent¡metro).
:p.Adem s de los valores num‚ricos, las palabras claves 
&apos.Inline&apos.,
&apos.LeftMargin&apos., 
&apos.RightMargin&apos., 
&apos.LeftEdge&apos., 
&apos.RightEdge&apos. y
&apos.Centered&apos. 
se aceptan para la posici¢n horizontal, y 
&apos.TopMargin&apos.,
&apos.BottomMargin&apos., 
&apos.TopEdge&apos., 
&apos.BottomEdge&apos. y
&apos.Centered&apos. 
se aceptan para la posici¢n vertical.  Las palabras claves
&apos.EdgeToEdge&apos. y
&apos.MarginToMargin&apos. 
se aceptan para la medida de anchura horizontal o vertical.
Si la clave &apos.Inline&apos. se especifica para la posici¢n horizontal, 
entonces no se hace caso a la posici¢n vertical.
De igual manera, si se especifica o la clave &apos.EdgeToEdge&apos. o &apos.MarginToMargin&apos. 
para lo alto o lo ancho del gr fico, entonces no se hace caso a la posici¢n correspondiente.
:p.Los par metros &apos.Background&apos., &apos.NoTextAside&apos., &apos.Halftone&apos. y &apos.Reverse&apos.
son opcionales y sensibles a la diferencia entre may£scula y men£scula.
:p.El par metro opcional &apos.Background&apos., cuando se especifica, indica que
el gr fico no debe afectar al texto de la p gina, y que se debe pintar
detr s del texto, como un fondo.
:p.El par metro opcional &apos.NoTextAside&apos., cuando se especifica, indica que
el formateador no puede fijar texto ni a la izquierda del gr fico, ni a la derecha,
tratando al gr fico como si ocupara todo lo ancho de la p gina.
:p.El par metro opcional &apos.Halftone&apos., cuando se especifica, indica que
el gr fico ser  pintado medio blanco.  Normalmente, esto se hace pintando
cada otro punto, y por esta raz¢n no se hace con gr ficos &apos.metafile&apos..
:p.El par metro opcional &apos.Reverse&apos. cuando se especifica, indica que el
gr fico ser  pintado con los colores invertidos.  No se puede hacer esto con gr ficos &apos.metafile&apos..
:p.El par metro opcional &apos.PrinterOptimize&apos., cuando se especifica, indica que
el gr fico ser  optimizado para la resoluci¢n de la impresora.  Esto se hace por
fijar la tama¤o del im gen para que su magnificaci¢n en la impresora sea un n£mero
redondo.  Esto no se apoya para gr ficos del tipo &apos.Metafile&apos..
:p.El par metro &apos.KeepAspectFixX&apos. indica que la altura del im gen ser  determinado
por la anchura del gr fico y su tama¤o original, manteniendo las proporciones originales
del im gen.  El par metro &apos.KeepAspectFixY&apos. es semejante, salvo que la anchura 
se determine por la altura y el tama¤o original del gr fico.
:p.El par metro &apos.BorderWidth=n&apos. especifica lo ancho del borde alrededor del gr fico,
especificado en mil‚simas de una pulgada o de un cent¡metro, seg£n el tipo de medida del documento.
:p.El par metro &apos.BorderColor=rrvvaa&apos. especifica el color (rojo-verde-azul) del borde
alrededor del gr fico.
:p.El par metro &apos.BorderCaption="texto"&apos. especifica el texto comentario del gr fico.
Este texto es literal, salvo que todos los car cteres '"' son doblados.
:p.El par metro &apos.tipo&apos. actualmente puede ser &apos.BM&apos. (bitmap), 
&apos.MF&apos. (metafile), o &apos.UP&apos. (c¢digo &apos.UPC&apos.).
:p.El par metro &apos.tipo&apos. actualmente puede ser
&apos.BM&apos. (bitmap), 
&apos.MF&apos. (metafile), 
&apos.UP&apos. (c¢digo de barras UPC), 
&apos.39&apos. (c¢digo de barras 39),
&apos.PN&apos. (c¢digo de barras POSTNET),
&apos.E8&apos. (c¢digo de barras EAN-8),
&apos.27&apos. (c¢digo de barras Codabar (2 of 7)),
&apos.25&apos. (c¢digo de barras Code 2 of 5) o
&apos.28&apos. (c¢digo de barras Code-128).
Todo gr fico BMP, GIF, JPG, PCX o TIF se graban internalmente como bitmapes del
formato nativo de OS/2.
Este par metro se termina con un zero crudo (null).
:p.Informaci¢n gr fico crudo se pone despu‚s de la informaci¢n inicial,
lo cual se termina con un zero crudo (null).
Esta informaci¢n tiene los c¢digos 15, 16 y 17 escapados como se describi¢
mas antes en esta secci¢n.
:p.Para bitmapes, la informaci¢n primeramente incluye un n£mero crudo de cuatro bytes
que indica el tama¤o del cabecimiento del bitmap, entonces otro n£mero crudo de cuatro
:p.Para metafiles, la informaci¢n incluye un n£mero crudo de cuatro bytes dando el
tama¤o completo del metafile en s¡, y entonces el metafile.
:p.Para c¢digos &apos.UPC&apos., la informaci¢n de par metro contiene el c¢digo &apos.UPC&apos. mismo,
en forma textual.
:p.Para c¢digos de barras 'Code-39', la informaci¢n puede ser texto de cualquier largo,
pero la informaci¢n en si tiene que ser limitada a las car cteres A a Z (may£scula), 
0 a 9, el espacio,
&apos.&dot.&apos.,
&apos.&plus.&apos.,
&apos.&dash.&apos.,
&apos.&asterisk.&apos., 
&apos.&slash.&apos.,
&apos.&dollar.&apos. y
&apos.&percent.&apos..
:p.Para c¢digos de barras 'POSTNET', la informaci¢n tiene que ser un texto de 5 o 9
d¡gitos n£mericos.
:p.Para c¢digos de barras 'EAN-8', la informaci¢n tiene que ser un texto de 7 d¡gitos n£mericos.
:p.For Codabar codes, the data can be any number and combination of the characters 
0 to 9, &dash., &dollar., &colon., &slash., &dot. or &plus., and the first and last characters must be identical
and in the range A to D.

:lm margin=3.:p.&larrow.Date&colon.Fixed&colon.:hp5.tiempo:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n genera texto mostrando 
una fecha espec¡fica, cual fecha se especifica en segundos
desde el primer d¡a de enero, 1970.
El formato que se usa para presentar la fecha se especifica con
la funci¢n &apos.DateFormat&apos..

:lm margin=3.:p.&larrow.Date&colon.Current&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n genera texto mostrando 
la fecha actual, seg£n el calendario interno de la computadora.
El formato que se usa para presentar la fecha se especifica con
la funci¢n &apos.DateFormat&apos..

:lm margin=3.:p.&larrow.Date&colon.Changed&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n genera texto mostrando 
la fecha cuando el documento sostuvo su £ltimo cambio.
El formato que se usa para presentar la fecha se especifica con
la funci¢n &apos.DateFormat&apos..

:lm margin=3.:p.&larrow.DateFormat&colon.:hp5.texto_de_formato:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija el formato que se usa para presentar fechas.
Es semejante a como funciona la funci¢n &apos.C&apos. &apos.printf&apos., salvo
que los c¢digos que pueden seguir al &apos.&percent.&apos. son los
siguientes&colon.
:lm margin=10.:sl compact.
:li.m - Mes (en forma num‚rica)
:li.M - Mes (en forma texto)
:li.n - Mes (en forma texto abreviado)
:li.d - D¡a num‚rico del mes
:li.y - A¤o num‚rico (con el siglo)
:li.z - A¤o num‚rico (sin el siglo)
:li.w - D¡a de la semana (en forma texto)
:li.x - D¡a de la semana (en forma texto abreviado)
:esl.:lm margin=5.
:note.Este formato interno no var¡a con el idioma.  Los c¢digos que se ven
por el usuario pueden variar de idioma a idioma.

:lm margin=3.:p.&larrow.LineSpacing&colon.:hp5.porcentaje:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n fija el porcentaje del espaciado 
normal vertical entre l¡neas.  De normal, el porcentaje es ci‚n porciento.
Cuando el porcentaje es mas peque¤a, las l¡neas estar n mas pegadas
verticalmente.

:lm margin=3.:p.&larrow.JobSettings&colon.:hp5.datos_para_driver:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n contiene para par metro la informaci¢n de preferencias
devuelta por el di logo de preferencias de impresora.
La estructura completa de esta informaci¢n var¡a seg£n el tipo de impresora,
y puede ser (o no) documentado por el autor del driver para la impresora (no por mi),
aunque el comienzo de la informaci¢n tiene un formato documentado por IBM.
Esta informaci¢n se graba al fichero del documento con los c¢digos 15, 16 y 17
escapados, como se supone que la informaci¢n est  en forma cruda (binary).
Este c¢digo no puede aparecer afuera del  rea par metro del 
c¢digo de funci¢n &apos.Preface&apos..

:lm margin=3.:p.&larrow.PageNumber&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n genera texto mostrando 
el n£mero de la p gina en que se encuentra el c¢digo de funci¢n.

:lm margin=3.:p.&larrow.PageCount&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n genera texto mostrando 
la cuenta total de p ginas en el documento.

:lm margin=3.:p.&larrow.Glyph&colon.:hp5.number:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n genera un solo car cter cuyo valor num‚rico
es el n£mero especificado.  Cu l letra ser  desplegado o imprimido
depende de la p gina de c¢digos y el fuente en uso.
Este c¢digo de funci¢n se cree cuando se usa la t‚cnica de &apos.Alt+nnn&apos.
para crear c¢digos especiales de car cteres con el teclado num‚rico, y
elimina la confusi¢n de cu ndo tratar a un c¢digo de l¡nea nueva (por ejemplo)
como tal, o como un car cter para ser desplegado.

:lm margin=3.:p.&larrow.Author&colon.:hp5.nombre:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n solo aparece en la secci¢n de prefacio, y sirve para
guardar el nombre del creador original del documento.

:lm margin=3.:p.&larrow.CreateDate&colon.:hp5.tiempo:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n solo aparece en la secci¢n de prefacio, y sirve
para guardar la fecha y hora de la creaci¢n original del documento.
Se graba de la misma manera que las otras fechas, es decir en segundos
desde el primer de enero del a¤o 1970.

:lm margin=3.:p.&larrow.Comment&colon.:hp5.comentario:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n sirve para colocar un comentario adentro del 
documento.  El comentario no se muestra ni se imprime con el documento mismo.

:lm margin=3.:p.&larrow.Separator&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n causa una separaci¢n de l¡neas, seguido por una
l¡nea separadora, seguido por otra separaci¢n de l¡neas.  Tiene casi el
mismo efecto como la funci¢n <hr> del idioma &apos.HTML&apos..

:lm margin=3.:p.&larrow.Title&colon.:hp5.nombre:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n solo aparece en la secci¢n de prefacio, y sirve para
guardar el t¡tulo del documento.

:lm margin=3.:p.&larrow.Header&colon.:hp5.texto:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica el texto de formato para usarse para
la cabecera de la p gina desde el lugar actual para adelante.
Si nada visible se ha aparecido todavia en la p gina, entonces el formato
nuevo se usar  para la p gina actual tambi‚n.

:lm margin=3.:p.&larrow.Footer&colon.:hp5.texto:ehp5.&rarrow.
:lm margin=5.:p.
Este c¢digo de funci¢n especifica el texto de formato para usarse para
el pie de la p gina desde el lugar actual para adelante.
Si nada visible se ha aparecido todavia en la p gina, entonces el formato
nuevo se usar  para la p gina actual tambi‚n.

:lm margin=1.

:note.Distancias se representan en unidades de la mil‚sima parte de una pulgada
o de un cent¡metro, seg£n el tipo de medidas (ingl‚s o m‚trico) que se usa 
con el documento.  Una distancia de medio cent¡metro o pulgada entonces ser¡a
representado con el n£mero 500.  Generalmente solo se usan distancias positivas.

:note.Porcentajes se representan como n¢meros enteros con un punto decimal
presumido.  Por ejemplo, se representa ci‚n porciento como 100, cientoveinticinco 
porciento como 125, etc.  Generalmente solo se usan porcentajes positivos.

:h2 res=61002.P ginas de C¢digos
Los siguientes p ginas de c¢digos (tablas de car cteres nacionales)
son v lidos&colon.

:table cols='4 60' rules=both frame=box.
:row.:c.037     :c.Brasil / Portugu‚s / Ingl‚s americano (EBCDIC)
:row.:c.273     :c.Austria / Alem n (EBCDIC)
:row.:c.274     :c.Belgico (old EBCDIC)
:row.:c.277     :c.Dan‚s / Noruego (EBCDIC)
:row.:c.278     :c.Finland‚s / Sueco (EBCDIC)
:row.:c.280     :c.Italiano (EBCDIC)
:row.:c.282     :c.Portugu‚s (old EBCDIC)
:row.:c.284     :c.Espa¤ol (EBCDIC)
:row.:c.285     :c.Ingl‚s brit nico (EBCDIC)
:row.:c.297     :c.Francais (EBCDIC)
:row.:c.437     :c.Estado Unidos (IBM PC)
:row.:c.500     :c.Belgico / Internacional (EBCDIC)
:row.:c.708     :c.Arabe (ASMO 708)
:row.:c.709     :c.Arabe (ASMO 449, BCON V4)
:row.:c.710     :c.Arabe (Arabe transparente)
:row.:c.711     :c.Arabe (Nafitha Ampliado)
:row.:c.720     :c.Arabe (ASMO transparente)
:row.:c.813     :c.Griego
:row.:c.819     :c.Multilinguo (ISO-8859/1)
:row.:c.850     :c.Multilinguo (Latin1)
:row.:c.852     :c.Slavico (Latin2)
:row.:c.855     :c.Serbia / Montenegro / Macedonia / Bulgaria
:row.:c.857     :c.Turqu¡a
:row.:c.860     :c.Portugu‚s
:row.:c.861     :c.Icelandia
:row.:c.862     :c.Hebreo
:row.:c.863     :c.Francais canadiense
:row.:c.864     :c.Arabe
:row.:c.865     :c.Dan‚s / Noruego
:row.:c.866     :c.Rusia
:row.:c.869     :c.Griego
:row.:c.870     :c.Czechoslovaquia / Hungr¡a / Polonia / Yugoslavia (EBCDIC)
:row.:c.871     :c.Icelandia (EBCDIC)
:row.:c.874     :c.Tailandia
:row.:c.915     :c.Bulgaria / Macedonia
:row.:c.932     :c.Jap¢n (Shift JIS X-0208)
:row.:c.934     :c.Korea
:row.:c.936     :c.Chino simplificado
:row.:c.938     :c.Chino tradicional
:row.:c.942     :c.Jap¢n (SAA)
:row.:c.944     :c.Korea (SAA)
:row.:c.946     :c.Chino simplificado (SAA)
:row.:c.948     :c.Chino tradicional (SAA)
:row.:c.949     :c.Korea (Shift KSC 5601)
:row.:c.950     :c.Chino tradicional (Big5)
:row.:c.1004    :c.Publicaciones de escritorio
:row.:c.1026    :c.Turqu¡a (EBCDIC)
:row.:c.1200    :c.Unicode
:row.:c.1250    :c.Windows, Latin-2 (Europa Oriental)
:row.:c.1251    :c.Windows, Cyrillic
:row.:c.1252    :c.Windows, Multilingo (ANSI)
:row.:c.1253    :c.Windows, Griego
:row.:c.1254    :c.Windows, Turqueo
:row.:c.1255    :c.Windows, Hebreo
:row.:c.1256    :c.Windows, Arabe
:row.:c.1257    :c.Windows, Baltic
:etable.

:note.No todas las p ginas de c¢digos se pueden usar en todos los sistemas.

:h2 res=61003.Interface para extensiones
La facilidad para extensiones busca el directorio de instalaci¢n de Escriba
al arrancarlo al programa para todos los fichero &apos.DLL&apos..  Cada uno de estos
ficheros son chequeados para la presencia de las funciones &apos.Identify&apos.,
&apos.ProductName&apos., &apos.ProductVersion&apos., 
&apos.ProductInformation&apos. y &apos.ProductConfigure&apos..  
Si todas estas funciones est n presentes, entonces el m¢dulo se considera 
como una extensi¢n a Escriba, de alg£n tipo.

:p.Los prototipos para las funciones en el idioma de programaci¢n &apos.C&apos. son
los siguientes&colon.

:font facename=Courier size=13x8.:lm margin=6.:sl compact.
:li.char* APIENTRY Identify ( void ) ;
:li.BOOL  APIENTRY SetLanguage ( char *Language ) ;
:li.char* APIENTRY ProductName ( void ) ;
:li.int   APIENTRY ProductVersion ( void ) ;
:li.void  APIENTRY ProductInformation ( HWND Due¤o ) ;
:li.int   APIENTRY ProductConfigure ( int MaxPages, char Names[][64], char MajorTabs[][32], char MinorTabs[][32], PFNWP Processors[], HMODULE Libraries[], int DialogIDs[] ) ;
:esl.:lm margin=1.:font facename=default.

:p.La funci¢n &apos.Identify&apos. devuelve una hilera de car cteres terminada
con un car cter nulo que identifica el tipo de la extensi¢n.
Actualmente, los tipos definidos son
:link reftype=hd res=61004."ISPELLER":elink.
, 
:link reftype=hd res=61005."IMPORT/EXPORT":elink.
y
:link reftype=hd res=61006."GRAPHICIMPORT":elink..

:p.La funci¢n &apos.SetLanguage&apos. permite a Escriba configurar el idioma de la
extensi¢n.  Se usa el nombre normal para el idioma en el idioma mismo.
Por ejemplo, para francais, se usa la palabra &apos.Fran&cc.ais&apos..

:p.La funci¢n &apos.ProductName&apos. devuelve una sequencia de car cteres
terminada con un car cter nulo, la cual ser  utilizada para identificar
la extensi¢n en el men£ de informaci¢n de productos.
Esta nombre no debe de incluir el n£mero de revisi¢n del producto.

:p.La funci¢n &apos.ProductVersion&apos. devuelve un n£mero que representa
el n£mero de revisi¢n del producto multiplicado por cien.
Tambi‚n se usa este n£mero para el men£ de informaci¢n de productos.

:p.La funci¢n de &apos.ProductInformation&apos. muestra un di logo que da al usuario
el nombre completo del producto, el n£mero de versi¢n y la fecha, y cualquier
informaci¢n en cuanto al autor y sus derechos que pertenece al producto.
Esta funci¢n se llamar  cuando el usuario seleccione el producto del men£
de informaci¢n del producto.

:p.La funci¢n de &apos.ProductConfigure&apos. devuelve la informaci¢n que ser 
usado para construir el di logo de Configurar Extensiones.  Esta funci¢n no tiene
que estar presente, pero cuando existe se espera que devolver  el n£mero de 
p ginas de configuraci¢n que se proveer n.  Por cada una de estas p ginas
(hasta un l¡mite especificado en la llamada), un nombre de p gina, texto de
tabulador mayor o menor (el uno o el otro), una funci¢n procesadora de di logo,
una manija para el m¢dulo de textos y un n£mero de identificaci¢n para la 
descripci¢n del dialogo todos tienen que ser devueltos a la funci¢n que llama.

:h3 res=61004.Interface 'ISpeller'
El m¢dulo "ISPELLER" fue desarrollado del programa ISPELL (GNU), versi¢n 3.1.18,
y como tal tiene que ser gratis, y se puede conseguir el c¢digo fuente gratis tambi‚n.
Los derechos pertenecen a Pace Willisson y Geoff Kuenning.  
ISPELLER en si es simplemente una cubierta alrededor de ISPELL, pero rehecho como
un fichero DLL para OS/2.  
ISPELLER no puede hacer uso de otras versiones de ISPELL 3.1.x que residen en su
sistema, pero -si- puede usar los fichero diccionarios de tales.
Para hacer esto, solo hay que insertar el mandato
&apos.SET ISPELL=path&apos. en su fichero CONFIG.SYS, donde
&apos.path&apos. es el camino completo a donde se encuentren los ficheros
diccionarios ISPELL.
Los m¢dulos ISPELLER.DLL y BUILDHAS.EXE tambi‚n se deben copiar a ese directorio.

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
