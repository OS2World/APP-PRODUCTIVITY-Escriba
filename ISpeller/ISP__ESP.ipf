#define LOCAL_DEFINES
#include "ISpeller.h"
:userdoc.
:title.Ayuda para 'ISpeller'
:docprof toc=123456.

:h1 res=IDD_PRODUCTINFO.Informaci¢n del producto (Di logo)
Esta caja de di logo visualiza el nombre del programa, su icono e
informaci¢n sobre el copyright. Para salir de este di logo,
pulsar la tecla ENTER, la barra de espacio o la tecla ESCAPE, o
hacer un click en el bot¢n "OK".

:h2 res=IDD_CONFIGURE.Configurar Diccionario (Di logo)
Desde este di logo se puede escoger el diccionario para usarse con
el chequeo de ortograf¡a, hacer diccionarios nuevos y remover
diccionarios viejos.
:p.Los diccionarios actualmente disponibles se ven en el di logo.
Para seleccionar uno para usarse, hay que hacer un click sobre el
nombre del diccionario, y pulse la tecla ENTER, o simplemente hacer
un doble-click sobre el nombre.  Si por alguna raz¢n no se puede
abrir el diccionario, habr  un aviso.  De otro modo, este di logo
se termina y se vuelve al trabajo normal de la procesadora de palabras.
:p.Para hacer un diccionario nuevo, hay que hacer un click en el bot¢n
de Hacer. El di logo :link reftype=hd res=IDD_BUILD.Hacer Diccionarios:elink.
se mostrar , del cual se podr  hacer un nuevo diccionario.
:p.Para remover un diccionario del sistema, hay que hacer un click sobre
el nombre del diccionario, y entonces sobre el bot¢n de Remover.

:h3 res=IDD_BUILD.Hacer Diccionarios (Di logo)
Desde este di logo se puede hacer un nuevo diccionario para usarse con
el chequeo de ortograf¡a.
:p.Para hacer un diccionario, uno necesita proveer dos ficheros de informaci¢n&colon.
un lista de palabras y un fichero &apos.affix&apos. que depiende del idioma en uso.
:p.La lista de palabras puede ser una lista simple, una palabra por cada l¡nea
en formato texto, o puede estar en el formato que se refiere como &apos.munched&apos.
para el programa &apos.ISpell&apos., el cual marque las palabras de un modo especial
para indicar las maneras en los cuales se pueden modificar las palabras.
No hay reglas particulares en cuanto al nombre de estos ficheros, aunque
normalmente el nombre del idioma forma parte del nombre del fichero.
:p.El fichero &apos.affix&apos. contiene las reglas por las cuales se pueden modificar
las palabras en el diccionario, y es distinto para cada idioma.
El tipo normal de tales ficheros es &apos..aff&apos..
:p.Para hacer un nuevo diccionario, uno tiene que especificar el nombre de
un fichero lista de palabras, de un fichero &apos.affix&apos., y el nombre del
diccionario que uno quiere crear.  Una vez que estos nombres est n entradas
al di logo, entonces se puede hacer un click en el bot¢n &apos.OK&apos., y entonces
se verificar n los nombres de los ficheros y el proceso de hacer el 
diccionario comenzar .  Este proceso puede demorar un momento o dos,
y despu‚s uno ser  devuelto al di logo
:link reftype=hd res=IDD_CONFIGURE.Configurar Diccionario:elink., del cual
se puede seleccionar el diccionario nuevo para usarse.
Si haya alg£n problema en hacer el diccionario, habr  un aviso y se
permitir  tomar medidas para corregir el problema que hab¡a (por cambiar
los nombres de los ficheros, por ejemplo) antes de atentar hacer el
diccionario de nuevo.
:p.Para salir de este di logo sin hacer un diccionario, hay que pulsar
la tecla ESC o hacer un click en el bot¢n Cancelar.

:euserdoc.
