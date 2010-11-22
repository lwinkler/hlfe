@echo off
echo Making %1.key
IF EXIST jpg\XXX.jpg del jpg\XXX.jpg
IF EXIST pgm\XXX.pgm del pgm\XXX.pgm
IF EXIST key\XXX.pgm del key\XXX.pgm

IF EXIST templates\%1.jpg rename templates\%1.jpg XXX.jpg
IF EXIST pgm\%1.pgm rename pgm\%1.pgm XXX.pgm
IF EXIST key\%1.key rename key\%1.key XXX.key

mingw32-make XXX

rename templates\XXX.jpg %1.jpg
rename pgm\XXX.pgm %1.pgm
rename key\XXX.key %1.key
