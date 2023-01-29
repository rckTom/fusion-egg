
# Software

Softwareseitig kam ein selbst geschriebenes Programm in C und C++ zum Einsatz. Dieses ist strukturiert in verschiedene "apps", welche jeweils unterschiedliche Animationen und Funktionen auf dem Ei ausführen. Eine übergeordnete Kontrollschleife schaltet zwischen den apps umher. 

Für das Ansprechen der LED-Streifen nutzten wir die FastLED-Bibliothek, für die Kommunikation mit dem Beschleunigungssensor die ADXL345-Bibliothek. 

Insgesamt entwickelten wir fünf apps, wovon jedoch nicht mehr alle produktiv genutzt werden: 
- Eine Fackelanimation, die bei lauten beats "Brennstoff ins Feuer schmeißt"
- Eine Animation mit Beaterkennung, die sich mittelfristig in Frequenz und Phase an die bpm anpasst 
- Ein vu-Meter, um die Aussteuerung des Mikrofons zu testen
- Eine fft-Anzeige, zu Testzwecken
- Deep sleep - das Ei hat keinen Ein/Ausschalter, diese app ermöglicht aber eine recht lange Zeit im standby

Für das Umschalten zwischen den Apps verwendeten wir interrupts seiten des Beschleunigungssensors. Die ADXL345-Bibliothek bietet hier bereits die Möglichkeit, um die nötigen Parameter eines double taps einfach einzustellen. Sobald das Ei mit dem richtigen zeitlichen Abstand und der richtigen Beschleunigung zwei mal bewegt wird, wird ein double tap ausgelöst und die app umgeschaltet. 

Weitere apps sind leicht ergänzbar. Es muss jeweils nur eine init, eine deinit, und eine loop-Methode definiert werden.