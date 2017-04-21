# Projet IOT - Alarm_music

Arduino movement detector which play songs

Presentation:


## Needed hardware
- Weemos D1
- RED LED
- PIR motion sensor


## Installation notes

The wifi code is based of this github repository so, you will need to follow their instructions:
- https://github.com/honnet/iot_intro/wiki/IOT-Intro

The PIR motion sensor code is based of this tutorial, you will need to follow their instrutcions:
- http://www.manuel-esteban.com/tuto-lire-un-capteur-infrarouge-avec-arduino/

How to make a server on an ESP8266 :
- https://www.fais-le-toi-meme.fr/fr/electronique/tutoriel/programmes-arduino-executes-sur-esp8266-arduino-ide

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
