#include "PText.h"
:userdoc.
:title.Ayuda para importaci¢n y exportaci¢n de texto simple
:docprof toc=123456.

:h1 res=IDD_PRODUCTINFO.Informaci¢n del producto
Este di logo muestra el nombre, la revisi¢n, la fecha y la informaci¢n
de 'copyright' para este m¢dulo.  Para salir del di logo, hay que
oprimir la tecla 'ENTER' o hacer un click en bot¢n 'OK'.

:h1 res=IDD_CONFIGURE_IMPORT.Configuraci¢n para importar
Esta p gina de configuraci¢n permite al usuario fijar varias opciones
para la importaci¢n de textos simples.
Las opciones actualmente disponibles son&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_FONT.Fuente:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_SIZE.Tama¤o:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_METRIC.Usar cent¡metros:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_SOFTBREAKS.Ignorar de fines de l¡neas:elink.
:esl.:p.
Este di logo siempre se muestra como una p gina del cuaderno que forma
parte del di logo para configurar extensiones.
Las entradas que uno hace tomar n efecto despu‚s que uno haga un
click en el bot¢n de 'OK' de aquel di logo.
Si uno haga mas bien un click en el bot¢n de cancelar, entonces no
tomar n efecto.
De todos modos, las opciones se graban por la extension en si, en el
fichero PTEXT.INI, lo cual se encuentra en el directorio al cual
fue instalado PTEXT.

:h2 res=IDD_CONFIGURE_FONT.Configurar Fuente
Este control combinado le permite a uno especificar cu l fuente
ser  usado para textos simples importados, como un documento de
texto simple no contiene una especificaci¢n en si de cu l fuente
se debe de usar.
Solo se encuentran fuentes de ancho uniforme (monospaced) en esta
lista, como textos simples son uniformes as¡ de definici¢n.

:h2 res=IDD_CONFIGURE_SIZE.Configurar Tama¤o
Este control combinado le permite a uno especificar cu l tama¤o
ser  usado para textos simples importados, como un documento de
texto simple no contiene una especificaci¢n en si de cu l tama¤o
se debe de usar.
Para el tama¤o normal de maquinas de escribir, hay que usar
diez puntos.

:h2 res=IDD_CONFIGURE_METRIC.Usar cent¡metros
Cuando se marque este bot¢n el importador de documentos usar 
cent¡metros (en vez de pulgadas) en preparar el documento.

:h2 res=IDD_CONFIGURE_SOFTBREAKS.Ignorar de fines de l¡neas
Cuando se marque este bot¢n el importador de documentos no har 
caso a los fines individuales de l¡neas mientras cargue al
documento.  Fines multiples siguen proces ndose como normal.

:h1 res=IDD_CONFIGURE_EXPORT.Configuraci¢n para exportar
Esta p gina de configuraci¢n permite al usuario fijar varias opciones
para la exportaci¢n de textos simples.
Las opciones actualmente disponibles son&colon.
:sl compact.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_FORMAT.Formatear resulto:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_HEIGHT.Alto:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_WIDTH.Ancho:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_BOLD.Simular texto negrito:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_UNDERSCORE.Simulate texto subrayado:elink.
:li.&sqbul.:link reftype=hd res=IDD_CONFIGURE_STRIKEOUT.Simulate texto tachado:elink.
:esl.:p.
Este di logo siempre se muestra como una p gina del cuaderno que forma
parte del di logo para configurar extensiones.
Las entradas que uno hace tomar n efecto despu‚s que uno haga un
click en el bot¢n de 'OK' de aquel di logo.
Si uno haga mas bien un click en el bot¢n de cancelar, entonces no
tomar n efecto.
De todos modos, las opciones se graban por la extension en si, en el
fichero PTEXT.INI, lo cual se encuentra en el directorio al cual
fue instalado PTEXT.

:h2 res=IDD_CONFIGURE_FORMAT.Formatear Resulto
Cuando se marque este bot¢n el importador de documentos arreglar 
el texto resulto en p ginas y l¡neas, usando las medidas de
ancho y de alto especificadas en este p gina de di logo.
De otro modo, las l¡neas solo ser n divididas cuando se encuentren
directivos para l¡neas nuevas en el documento original.

:h2 res=IDD_CONFIGURE_HEIGHT.Configurar Alto
Este control 'spinbutton' le permite a uno especificar cu ntas
l¡neas ser n usadas por cada p gina del documento exportado.
Valores desde 40 l¡neas hasta 80 l¡neas se pueden usar.

:h2 res=IDD_CONFIGURE_WIDTH.Configurar Ancho
Este control 'spinbutton' le permite a uno especificar cu ntas
columnas ser n usados en el documento exportado.
Valores desde 40 columnas hasta 160 columnas se pueden usar.

:h2 res=IDD_CONFIGURE_BOLD.Simular Texto Negrito
Cuando se marque este bot¢n el exportador del documento simular 
textos negritos por imprimir la letra, hacer un retroceso, y 
entonces imprimir la letra otra vez.

:h2 res=IDD_CONFIGURE_UNDERSCORE.Simular Texto Subrayado
Cuando se marque este bot¢n el exportador del documento simular 
textos subrayados por imprimir la letra, hacer un retroceso, y
entonces imprimir una subraya.

:h2 res=IDD_CONFIGURE_STRIKEOUT.Simular Texto Tachado
Cuando se marque este bot¢n el exportador del documento simular 
textos tachados por imprimir la letra, hacer un retroceso, y
entonces imprimir una raya.

:euserdoc.
