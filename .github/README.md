# Proyecto personal

## Reloj - alarma con PIC16F887

Este es un proyecto personal en el que se utilizará un microcontrolador PIC16F887 para estar visualizando un RTC en un display LCD 16x2.

## Requisitos / SetUp
### Compilador
Es necesario descargar el compilador de Microchip [XC8]. Dependiendo del sistema operativo, es necesario descargar la versión correspondiente. 
#### Link de compiladores:

    https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers

### Repositorio de microcontroladores
También, dependiendo del microcontrolador que se usa, será necesario descargar el repositorio correcto. Para el caso de este proyecto se está usando el repositorio <Microchip PIC16Fxxx Series Device Support>. Pero será necesario descargar otro repositorio si en un futuro se quiere usar otro microcontrolador que no esté en ese repositorio.
#### Link de repositorios:

    https://packs.download.microchip.com/

## Compilación
Para poder compilar el proyecto, solamente basta con correr el archivo Makefile:

    make

Y eliminar los archivos/objetos hechos para la compilación que se encuentran en la carpeta .build/:

    make clean

## Programar el PIC
EL archivo .hex se ubica en la carpeta -build/ . 
(Quiero ver si desde el Makefile puedo hacer que suba el archivo)



## Retos:

* Utilizar un IDE sin nada de plugins de MPLAB.
* Solamente se utilizará el XC8 compiler [CLI] para poder compilarlo y cargar el programa al micro.
* Hacer que todo sea controlado desde el PIC sin ayuda de otro microcontrolador.
