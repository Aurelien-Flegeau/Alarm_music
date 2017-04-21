# Projet IOT - Alarm_music

Arduino movement detector which play songs

Presentation:
https://docs.google.com/presentation/d/137jWZd6tapu6696X0hdtBjZBa9_8uMd9ih9CgkLRI2c/edit?usp=sharing

state of the art:
https://docs.google.com/document/d/1WblYoKvYsVU3uSCl9s2EiusWHYPhPOMS6-9A7LZ6wk0/edit?usp=drive_web

## Needed hardware
- Weemos D1
- RED LED
- PIR motion sensor


## Installation notes

The PIR motion sensor code is based of this tutorial, you will need to follow their instrutcions:
- http://www.manuel-esteban.com/tuto-lire-un-capteur-infrarouge-avec-arduino/

Manual of PIR motion sensor:
- https://www.mpja.com/download/31227sc.pdf

How to make a server on an ESP8266:
- https://www.fais-le-toi-meme.fr/fr/electronique/tutoriel/programmes-arduino-executes-sur-esp8266-arduino-ide

## Distribution task

 - day 1
Andreï - displaying message in morse language using the internal LED
Aurélien, Andreï, Kévin, Sébastien - Choose the subject
Aurélien - buy a Wemos D1

 - day 2
Kévin, Sebastien, Andreï - State of the art of the similar projects
Aurelien, Andrei - Use and configure a PIR motion sensor
Sebastien, Kevin - POC websocket server

 - day 3 
Andreï - POC filesystem :transfering files from the computer in the ESP8266 using a plugin for Arduino IDE (https://github.com/esp8266/arduino-esp8266fs-plugin), writing and reading files in the ESP8266 file system
Aurelien - create a web interface which listens to the server and trigger a video/audio when the alarm goes off
Aurélien & Andreï - adding the file writing and reading to the main project in order to keep the configuration saved even after a reset
Kévin, Sébastien - POC reading audio from the webpage
Kévin, Sébastien - documentation start

 - day 4
Andreï - refactoring and creation of an admin page + server API to change the url link and type of the audio
Aurélien & Andreï -adding the admin page in the project
Andreï - POC of the mail sender using an D1 mini using the library GSender and following this tutorial : http://www.instructables.com/id/ESP8266-GMail-Sender/
Aurélien & Andreï - adding themail sending code in the project
Kévin, Sebastien, Aurelien - Readme and power point
Andreï - task list and distribution

## Steps followed
We made the project by following these steps :

- Step 1:
  - Using directly the sensor's output signal, light a led when sensor detect a move
  - Use a program which prints the value of the sensor when it detects a move and light an external and internal led
- Step 2:
  - Use a program to automatically reloads a client html page launches from a arduino server
  - Use the program which automatically reloads a client html page from the arduino server to only show a message when the sensor detects something
- Step 3:
  - Send a message from client to server, server responds with the message without reloading the client page
- Step 4:
  - listen an audio link
- Step 5:
  - have an ihm to store link
- Step 6:
  - With the program from the precedent step, play a song when the sensor detects something and don't reloads as the song don't end
  - Use Sockets to play the song on the web page, started from server without reloading the page
- Step 7:
  - Add an interface to load different songs of differents format 
- Step 8:
  - Add a mailing function to warn when the trap is triggered
  
  ## Breadboard wiring
  
  ![alt tag](https://image.noelshack.com/fichiers/2017/16/1492780973-plan.png)
