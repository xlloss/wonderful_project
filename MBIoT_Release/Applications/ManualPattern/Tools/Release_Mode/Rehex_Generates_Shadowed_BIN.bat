echo off
copy .\Output_File\Release_Mode\ManualPattern_Release.H01 .\Tools\Release_Mode\UNTREATED.H00
copy .\Output_File\Release_Mode\ManualPattern_Release.H02 .\Tools\Release_Mode\UNTREATED.H01
copy .\Output_File\Release_Mode\ManualPattern_Release.H03 .\Tools\Release_Mode\UNTREATED.H02
copy .\Output_File\Release_Mode\ManualPattern_Release.H04 .\Tools\Release_Mode\UNTREATED.H03
copy .\Output_File\Release_Mode\ManualPattern_Release.H05 .\Tools\Release_Mode\UNTREATED.H04

del .\Output_File\Release_Mode\ManualPattern_Release.H0*
del .\Output_File\Release_Mode\ManualPattern_Release.B0*

cd .\Tools\Release_Mode
call .\rehex.exe rehex.cfg

cd..
copy .\Release_Mode\ManualPattern_Release.* ..\Output_File\Release_Mode\
del .\Release_Mode\ManualPattern_Release.*
del .\Release_Mode\UNTREATED.*
