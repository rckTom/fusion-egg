
# Praxistest

Nach vielen Nachtschichten waren wir kurz vor dem Festival endlich soweit und hatten zwei funktionierende Eier.
Auch die mehrstündige, ungepolsterte Fahrt in Eurokisten überlebten alle Bauteile problemlos.
Nach einem kurzen Zusammenbau und Funktionscheck kam nun der erste große Test. 

Zu Beginn war das Laufen mit dem Totem noch sehr ungewohnt. Das lag zum einen am hohen Schwerpunkt, zum anderen an der Aufmerksamkeit,
die wir unweigerlich auf uns zogen. 

![fusion 1](grafiken/fusion_bilder/5.jpg "Totem auf dem Festivalgelände")

Eine Warnung an dieser Stelle: Das Totem verwandelt eine Festivalnacht zuverlässig in eine Nerdveranstaltung.
Viele interessierte Menschen kamen auf uns zu und stellten Fragen. 
*Ist das Ding selbst gemacht? Was für einen Mikrocontroller habt ihr dafür genommen? Wo im Netz gibt es die Anleitung dafür? Kannst du meinem Kumpel nochmal das mit dem double-tap zeigen?*
Besonderen Spaß gemacht hat uns der Austausch mit den makern anderer Totems. So verschieden die Basteleien aussahen, so ähnlich
waren doch oft die zugrundeliegenden Lösungen. Auch ein Sommertrend war klar erkennbar:

![sommertrend 2022](grafiken/memes/esp32_wled_hot.jpg "WLED auf einem ESP32 - so hot right now")

Am ersten Abend hatten wir zum debugging noch ein vu-Meter auf einem der Eier implementiert um die Aussteuerung zu messen. 
Daraus folgte die wenig überraschende Erkenntnis, dass die Mikrofone wenige Meter vor der Bühne völlig übersteuern. 
Zum Glück funktionierten die audioreaktiven Animationen trotzdem. 

Die Beaterkennung konnte vor allem bei Musik mit gleichmäßigem Rythmus überzeugen, während die Fackelanimation 
vor allem bei bass drops das Umfeld zum Staunen brachte. Der heimliche Star des Abends war jedoch die double-tap-Funktion. 

![fusion 2](grafiken/fusion_bilder/3.jfif "Totem im Einsatz")

Vor dem zweiten Abend stellten wir zur Sicherheit noch die Empfindlichkeit der Mikrofone auf den kleinstmöglichen Wert.
Bei der Gelegenheit verringerten wir außerdem die Helligkeit der Animationen.
Weitere Änderungen waren während des Festivals nicht mehr nötig.

Beim Auseinanderbauen stellten wir mit Freude fest, dass die Akkupacks nach einer langen Nacht nicht einmal
die Hälfte ihrer Ladung verloren hatten. Grob geschätzt dürfte die Einsatzdauer im Betrieb bei etwa 24h liegen, 
etwa dreimal so lang im deep sleep. 


Ausleitung

![fusion 6](grafiken/fusion_bilder/6.jpg "Impressionen von der Tanzfläche")




- Probleme
    - Stab nicht ausziehbar
    - Schwerpunkt
    - Aufschrauben zum Laden
    - Ein Tüftler ist nie fertig "Ein Projekt wird nie beendet, nur verlassen"