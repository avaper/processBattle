#!/bin/bash
#este archivo es un scrip que:
#1 compila los fuentes padre.c e hijo.c con gcc
#2 crea el fihero fifo "resultado"
#lanza un cat en segundo plano para leer "resultado"  
#lanza el proceso padre
#al acabar limpia todos los ficheros que ha creado

#1 compila los fuentes padre.c e hijo.c con gcc
gcc Trabajo2/padre.c -o Trabajo2/padre
gcc Trabajo2/hijo.c -o Trabajo2/hijo

#2 crea el fihero fifo "resultado"
mkfifo ./Trabajo2/resultado

#lanza un cat en segundo plano para leer "resultado"  
cat ./Trabajo2/resultado &

#lanza el proceso padre
./Trabajo2/padre 10

#al acabar limpia todos los ficheros que ha creado
rm ./Trabajo2/padre
rm ./Trabajo2/hijo
rm ./Trabajo2/resultado

