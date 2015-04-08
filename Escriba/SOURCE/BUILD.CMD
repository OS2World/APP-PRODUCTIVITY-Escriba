@Echo Off
Cd ..\RunTime
  depend2 -1 *.cpp > makefile.dep
  nmake /nologo %1 %2 %3 %4 %5 %6 %7 %8 %9
  If ErrorLevel 1 Goto Exit0
  Cd ..\Escriba
depend2 -1 -i ..\runtime -i ..\ispeller *.cpp *.rc > makefile.dep
nmake /nologo %1 %2 %3 %4 %5 %6 %7 %8 %9
If ErrorLevel 1 Goto Exit
Cd Addons
  Cd Bmp
    depend2 -1 -i ..\..\..\runtime -i ..\.. *.cpp *.rc *.ipf > makefile.dep
    nmake /nologo %1 %2 %3 %4 %5 %6 %7 %8 %9
    If ErrorLevel 1 Goto Exit2
    copy *.dll ..\..
    copy *.hlp ..\..
    Cd ..\gif
    depend2 -1 -i ..\..\..\runtime -i ..\.. *.cpp *.rc *.ipf > makefile.dep
    nmake /nologo %1 %2 %3 %4 %5 %6 %7 %8 %9
    If ErrorLevel 1 Goto Exit2
    copy *.dll ..\..
    copy *.hlp ..\..
    Cd ..\html
    depend2 -1 -i ..\..\..\runtime -i ..\.. *.cpp *.rc *.ipf > makefile.dep
    nmake /nologo %1 %2 %3 %4 %5 %6 %7 %8 %9
    If ErrorLevel 1 Goto Exit2
    copy *.dll ..\..
    copy *.hlp ..\..
    Cd ..\jpeg
    depend2 -1 -i ..\..\..\runtime -i ..\.. *.cpp *.rc *.ipf > makefile.dep
    nmake /nologo %1 %2 %3 %4 %5 %6 %7 %8 %9
    If ErrorLevel 1 Goto Exit2
    copy *.dll ..\..
    copy *.hlp ..\..
    Cd ..\pcx
    depend2 -1 -i ..\..\..\runtime -i ..\.. *.cpp *.rc *.ipf > makefile.dep
    nmake /nologo %1 %2 %3 %4 %5 %6 %7 %8 %9
    If ErrorLevel 1 Goto Exit2
    copy *.dll ..\..
    copy *.hlp ..\..
    Cd ..\ptext
    depend2 -1 -i ..\..\..\runtime -i ..\.. *.cpp *.rc *.ipf > makefile.dep
    nmake /nologo %1 %2 %3 %4 %5 %6 %7 %8 %9
    If ErrorLevel 1 Goto Exit2
    copy *.dll ..\..
    copy *.hlp ..\..
    Cd ..\tiff
    depend2 -1 -i ..\..\..\runtime -i ..\.. *.cpp *.rc *.ipf > makefile.dep
    nmake /nologo %1 %2 %3 %4 %5 %6 %7 %8 %9
    If ErrorLevel 1 Goto Exit2
    copy *.dll ..\..
    copy *.hlp ..\..
    Cd ..
  Cd ..
Cd ..\ISpeller
  depend2 -1 -i ..\runtime -i ..\escriba *.cpp *.rc *.ipf > makefile.dep
  nmake /nologo %1 %2 %3 %4 %5 %6 %7 %8 %9
  If ErrorLevel 1 Goto Exit1
  Cd ..\Escriba
Goto EXIT

:EXIT0
Cd ..\Escriba
Goto EXIT

:EXIT2
Cd ..
:EXIT1
Cd ..
:EXIT

