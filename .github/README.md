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

## Asignación de permisos para el USB.
Es muy probable que Linux no dé accesos a los puertos USB por default y no permita usar el Pickit 3 por primera vez. Para evitar esto, se tiene que correr los siguientes comandos (debe estar conectado a la computadora el Pickit):

    lsusb

Del comando anterior, se tiene que obtener el ID y VID del Pickit. Ejemplo:
    Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
    Bus 001 Device 002: ID 0bda:567e Realtek Semiconductor Corp. Integrated_Webcam_HD
    Bus 001 Device 007: ID 8087:0026 Intel Corp. AX201 Bluetooth
    Bus 001 Device 016: ID 04d8:900a Microchip Technology, Inc. PICkit3
    Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub  

Aquí se observa que el vendor ID del Pickit es "04d8" mientras que el ID es "900a".
Después es necesario guardar un archivo indicando los permisos que se le da a los demás usuarios para que puedan usar ese dispositivo en específico.

    sudo nano /etc/udev/rules.d/99-pickit.rules

Y dentro de ese archivo, guardar el siguiente comando de acuerdo a lo que haya salido en el comando "libusb". Para mi caso, es la siguiente linea la que se guarda en este archivo.

    SUBSYSTEM=="usb", ATTR{idVendor}=="04d8", ATTR{idProduct}=="900a", MODE="0666"

Guardar y correr los siguientes comandos.

    sudo udevadm control --reload-rules
    sudo udevadm trigger

## Compilación
Para poder compilar el proyecto, solamente basta con correr el archivo Makefile:

    make

Y eliminar los archivos/objetos hechos para la compilación que se encuentran en la carpeta .build/:

    make clean

## Programar el PIC
El archivo .hex se ubica en la carpeta -build/ . 
Para programar el PIc usando el archivo Makefile, simplemente se usa el siguiente comando:

    make flash

## Usar PICKIT como fuente
Desde el archivo Makefile podemos alimentar el Pickit para que pueda usarse como fuente de 5V. El comando es el siguiente:

    make on         // habilita los 5V
    make off        // apaga los 5V


## More Info
### Compilador
Para más información acerca de la compilación y de la programación con el compilador XC8, visitar el siguiente link:
    https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/ReferenceManuals/MPLAB-XC8-C-Compiler-Users-Guide-for-PIC-DS50002737.pdf

### PK2CMD
Para más información y poder descargar el release de este archivo que permite usar el Pickit desde Linux, seguir el siguiente repositorio de GitHub:

    https://github.com/jaka-fi/pk2cmd
    
En mi caso, el archivo Appimage lo guardé en la siguiente ruta:

    /opt/

## Retos:

* Utilizar un IDE sin nada de plugins de MPLAB.
* Solamente se utilizará el XC8 compiler [CLI] para poder compilarlo y cargar el programa al micro.
* Hacer que todo sea controlado desde el PIC sin ayuda de otro microcontrolador.
