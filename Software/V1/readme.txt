Opté por partir utilizando Arduíno para programar el ESP8266.

IDE preparado:

https://github.com/esp8266/Arduino/

	Install Arduino 1.6.8 from the Arduino website.
	Start Arduino and open Preferences window.
	Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
	Open Boards Manager from Tools > Board menu and install esp8266 platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

Referencias: 

https://github.com/esp8266/Arduino/blob/master/doc/reference.md

RTC wiki:

http://www.seeedstudio.com/wiki/index.php?title=Twig_-_RTC


https://git-scm.com/book/es/v1/Fundamentos-de-Git-Guardando-cambios-en-el-repositorio
Un firmware por cada componente para probarlos en el atmega.
Un firmware para el atmega que integra todos los componentes.
Un firmware para el esp8266 que incluye un servidor web local y envia la configuración por serial al atmega.