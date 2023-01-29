
# Motivation

Ein Flugzeug mit schlagenden Flügeln. Ein handgeschriebenes Schild mit der Aufschrift "Bitte raven Sie hart!". Hängematten im Birkenwald, die von lautem Bass durchgeschüttelt werden.

Ungewöhnliche Eindrücke findet man auf dem Fusion Festival genug. Unter anderem ist die deutsche Variante des Burning Man auch bekannt für sogenannte Festivaltotems: 
Kunstwerke und Basteleien, die mit auf die Tanzfläche genommen werden, damit sich die eigene Crew auch findet. Mobilfunk sucht man nämlich meist vergeblich.

Heute erzählen wir die Geschichte unseres Festivaltotems. Sie beginnt ein paar Wochen vor dem großen Einsatz mit einer zentralen Idee -
es soll mit Licht auf Musik reagieren. Weiter geht es in unserer Erzählung mit einigen interessanten technischen Rätseln, die es zu lösen gilt. Am Ende steht ein 3D-gedrucktes Ei, gefüllt mit einem
lasergedruckten Holzskelett, LED-Ketten und Elektronik. Es tanzt zum beat, lässt sich über Gesten steuern und verbreitet wo es hinkommt gute Laune.

# Konzept

Die Hülle des Eis besteht aus drei vertikal gestapelten 3D-Druckteilen. Die mittlere Komponente ist für schnellen Druck im Vasenmodus ausgeführt - mehr dazu später. 

Im Inneren der Vase befindet sich zur Stabilisierung ein geklebtes Skelett aus lasergeschnittenem Holz. Dieses ist formschlüssig mit der Vase verbunden und mit dem Boden und Deckel verschraubt. 

Im Inneren des Holzskeletts ist eine Holzplatte eingespannt, welche als Träger der Elektronik fungiert. 

Das Gesamtkonzept ist in der folgenden Explosionsansicht illustriert:

![explosion](grafiken/cad/explosion.png "Explosionsansicht")

Das Ei sitzt verschraubt auf einem gedruckten Sockel. Dieser enthält einen kreisförmigen Schlitz, um ein handelsübliches Alurohr aus dem Baumarkt aufzunehmen. Die leichte Spielpassung in Verbindung mit den typischen Druckertoleranzen sorgt hier für einen stabilen Reibkraftschluss. 

Im Inneren befindet sich ein ESP32 Mikrocontroller, ein Mikrofon, ein Beschleunigungssensor, ein 10Ah Reiseakku und drei WS2812 LED-Bänder. 

(DSC07978.JPG)

Auf dem ESP32 läuft ein in C++ geschriebenes Programm, welches zwischen verschiedenen Animationen umschaltet. Die Umschaltung erfolgt durch den Beschleunigungssensor - ein doppeltes Aufstampfen des Stabes auf dem Boden wird hier als Umschaltsignal interpretiert. 

Auf dem Festival hatten wir zwei verschiedene Animationen implementiert, die wir hier "Beaterkennung" und "Fackel" nennen. Die Beaterkennung lässt auf jeden zweiten Beat eine Lichtwelle über das Ei laufen, die Fackel bildet ein Feuer nach, welches durch hohe Lautstärke angefacht wird. Nach den beiden Animationen geht das Ei in einen deep sleep, ein weiteres Doppelstampfen bringt es dann wieder auf die erste Animation.

# 3D-Druck - Vase mode mal anders

In einer idealen Welt würden wir das gesamte Ei einfach ganz normal drucken. Leider sagte der Slicer für unsere naive erste Konstruktion eine Druckzeit fast einer Woche voraus. 

Die zweite und entgültige Konstruktion nutzt zwei Tricks um das 32 Zentimeter hohe Ei in etwa einem Tag drucken zu können. 

Der erste Trick ist ein Umstieg auf eine 0.8mm breite Düse. 

Der zweite Trick ist eine kreative Verwendung des Vasenmodus. Auf den ersten Blick scheidet eine Vase aus, da zur Befestigung der LED-stripes ein gewisses Innenleben notwendig ist. 

Wir nutzen hier die Definition einer Vase aus. Der Slicer fährt auf jeder Lage sturr die äußere Kontur ab. "Außere Kontur" ist hier rein mathematisch definiert. Indem wir einen Volumenkörper von außen dünn einschlitzen, können wir also auch im Innenraum des Eis einen Tunnel für die LED-stripes erzeugen. Das ist in den Abbildungen (vase mode 1) und (vase mode 2) illustriert. 

![vase mode 1](grafiken/illustrationen/vase_cad.PNG "Geometrie laut CAD")

![vase mode 2](grafiken/illustrationen/vase_sliced.PNG "Tatsächlich gedruckte Geometrie")

An Boden und Deckel ist der Vasenmodus ungeeignet, da die Überlappung aufeinanderfolgender Lagen bei hier zunehmend klein wird. Daher wird das Ei in drei Teile geteilt, die äußeren beiden drucken wir normal. Trotz dieser Aufteilung ist das verbliebene Vasenteil recht groß, ein Drucker mit mindestens 250mm Bauraumhöhe ist also Voraussetzung. 

# Holzskelett

Um dem Ei trotz der dünnen Schale ausreichend Steifigkeit zu verleihen, wird es innen durch ein Holzskelett verstärkt. 
Dieses ist so konstruiert, dass es sich im Inneren des Vasenteils zusammenbauen und verkleben lässt und es so von innen aussteift.
Gleichzeitig sind alle Teile des Holzskelett prismatisch, lassen sich also im Lasercutter herstellen. Das ist hier dargestellt:

![holzskelett](grafiken/cad/zusammenbau4.png "Holzskelett")

In der Praxis überlebten die Eier dank dem Skelett auch die eine oder andere rauhe Behandlung und Katzenattacke. 


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

# Beaterkennung

Die erste von zwei Animationen besteht aus einer Welle, welche von jedem zweiten beat bis zum jeweils nächten beat von oben nach unten läuft. 
Dafür muss zu jedem Zeitpunkt sowohl die Frequenz als auch die Phase des beats bekannt sein, und das auch über kürzere Störungen oder komplexere Rythmen hinweg.

Die zentrale Idee des Algorithmus ist es, Bandpassfilter mit schwacher Dämpfung als Phasenschätzer zu missbrauchen. 
Hierfür wird eine ganze Reihe an Bandpässen mit unterschiedlichen Durchlassfrequenzen parallel geschaltet. Diese Durchlassfrequenzen gehören zu unterschiedlichen beat-Geschwindigkeiten. In unserem Fall von 105 bis 150bpm, der typische Bereich elektronischer Tanzmusik. Auf jeden dieser Bandpässe wird nun die Signalenergie gegeben. Der Bandpass, der sich daraufhin am meisten aufschwingt, hat dann offensichtlich am besten die richtige Geschwindigkeit getroffen. 

Im Detail: Zuerst sammeln wir samples vom Mikrofon. Wir ziehen den gleitenden Durchschnitt ab, um ein Signal zu erzeugen, welches um Null herum schwingt. Über jeweils einen chunk - eine Vierzigstelsekunde - summieren wir quadrierte samples, um die Signalenergie in diesem chunk zu erhalten. Alles weitere passiert nun ebenfalls 40 mal die Sekunde. Wir geben die Signalenergie parallel als input auf alle Bandpässe - zufällig ebenfalls 40 an der Zahl. Eine der Bandpässe schwingt sich am meisten auf. Wir gehen davon aus, dass dessen Resonanzfrequenz am nähesten am tatsächlichen beat liegt. Kleine Abweichungen werden durch das ständige feedback fortlaufend korrigiert. Nun haben wir das Problem, dass der Bandpass einmal pro beat schwingt, unsere gewollte Animation aber zwei beats benötigt. Wir müssen also irgendwie die Frequenz halbieren. Dafür verzögern wir das Signal um Pi/4 und schätzen mit der atan2-Funktion die Phase. Mit der bekannten Phase und einem einfachen Zustandsautomat konstruieren wir uns die Phase eines Systems mit halber Frequenz. Diese Phase skalieren wir linear und erhalten so die Sollposition des Maximums der Lichtwelle. 

Wir wissen nun also zu jedem Zeitpunkt die Position des Scheitelpunktes der Lichtwelle. Um diese Position herum wird die Helligkeit langsam gesenkt und damit die Welle geformt. Die Maxima für den roten und blauen Kanal sind dabei gegenläufig verschoben, so dass ein räumlicher Farbverlauf entsteht. 

Das Ganze ist hier illustriert:

![Illustration Beaterkennung](grafiken/illustrationen/beaterkennung.PNG "Illustration Beaterkennung")

# Fackel

Die zweite Animation ahmt eine Fackel nach, wobei der Brennstoff entsprechend dem beat nachgeführt wird. Entsprechend der Musik wabern dann Feuerschwaden langsam nach oben. Kurzfristige Änderungen in der Musik - z.B. durch bass drops - führen temporär zu größeren Feuerschwaden, was besonder spektakulär aussieht.

Wie bei der ersten Animation normieren wir zunächst das Mikrofonsignal auf einen Durchschnitt von Null und ermitteln daraus in chunks von 10 Millisekunden die Signalenergie. Die Signalenergie wird nun mit zwei unterschiedlichen Tiefpässen gefiltert. Ein Tiefpass hat eine kurze Zeitkonstante und dient nur zur Glättung des Signals. Der andere Tiefpass hat eine Zeitkonstante von mehreren Sekunden und gibt Auskunft über die durchschnittliche Lautstärke. 

Der folgende Teil wird alle 45 Millisekunden ausgeführt:
Wenn der schnelle Tiefpass mindestens 15% über dem langsamen Tiefpass liegt, dann gehen wir davon aus, dass aktuell ein beat vorliegt. Die untersten beiden LEDs werden nun mit mit der maximalen Helligkeit angesteuert. Alle anderen LEDs ermitteln ihre Helligkeit aus der durchschnittlichen Helligkeit der beiden LEDs darunter im vorhergehenden Zeitschritt, abzüglich einer kleinen Dämpfung. Das führt dazu, dass die Feuerschwaden nach oben hin dunkler, aber auch größer werden. Das Vorgehen ist im folgenden Bild illustriert:

![Illustration Beaterkennung](grafiken/illustrationen/fackel.PNG "Illustration Fackel")

# Anleitung

0) Organisiere dir an deinem lokalen makerspace folgende Fertigungsmöglichkeiten: Ein 3D-Drucker mit mindestens 25cm Bauraumhöhe. Ein Lasercutter oder eine andere Möglichkeit, um 3mm dickes Sperrholz zuzuschneiden. Ein Lötkolben.
1) Baue an deinem 3D-Drucker eine 0.8mm-Düse ein.
2) Drucke Boden und Deckel mit normalen Druckeinstellungen mit hellem PLA, support ist nicht nötig. Drucke die Kralle in einer anderen Farbe. Support ist ebenfalls nicht nötig, aber falls sich das Druckbett schnell bewegt, kann ein brim helfen. 
3) Drucke das mittlere Teil im Vasenmodus mit hellem PLA. Stelle die Anzahl der bottom layers auf Null. Auch hier kann ein brim helfen.
4) Schneide die Holzteile in 3mm Dicke zurecht. Teste, ob sich die Holzteile ineinander stecken lassen.
5) Presse die Muttern in die kleinen Ringe und klebe die kleinen Holzringe mit Muttern an die großen Holzringe. Beachte die Bilder, um die richtige Seite zu treffen. (TODO)
6) Senke mit einem Löteisen die Einsenkmuttern in das Bodenteil. 
7) Montiere die Holzteile innerhalb der Vase zusammen und verklebe die Holz-Holz-Kontakte mit Holzleim. Vermeide dabei Express-Holzleim. 
8) Stecke das LED-Band in einen Kanal, und kürze sie derart, dass es oben noch 10mm heraus schaut.  Ziehe es wieder heraus und schneide für die weiteren Kanäle LED-Streifen mit der selben Länge ab. 
9) Löte 3-polige Steckverbinder an das obere Ende der LED-Streifen. Orientiere es dabei so, dass die aufgedruckten Pfeile später nach unten (TODO) zeigen.
10) Schraube den Boden am Mittelteil fest. 
11) Ziehe die LED-Streifen in die Kanäle ein.
12) Spiele die Software auf den ESP, z.B. mit Visual Studio Code und der Erweiterung PlatformIO.
13) Verlöte Mikrofon, ESP, Beschleunigungssensor, Spannungsversorgung und die Anschlussleitungen für die LEDs gemäß der Darstellung (Todo).
14) Montiere die gelöteten Teile auf dem gelochten Holzbrett, z.B. mit Hilfe von dünnen Kabelbindern und Heißkleber. 
15) Lade den Akkupack, schließe und schalte ihn an.
16) Schraube die Kralle an den Boden.
17) Stecke das Holzbrett in die dafür vorgesehene Aussparung im Inneren des Bodens.
18) Verbinde die Steckverbinder der LEDs.
19) Schließe den Deckel und schraube ihn fest.
20) Stecke die Kralle auf den Stab.

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

Vor dem zweiten Abend stellten wir zur Sicherheit noch die Empfindlichkeit der Mikrofone über die kleine Einstellschraube auf den kleinstmöglichen Wert.
Bei der Gelegenheit verringerten wir außerdem die Helligkeit der Animationen.
Weitere Änderungen waren während des Festivals nicht mehr nötig.

Beim Auseinanderbauen stellten wir mit Freude fest, dass die Akkupacks nach einer langen Nacht nicht einmal
die Hälfte ihrer Ladung verloren hatten. Grob geschätzt dürfte die Einsatzdauer bei Benutzung bei etwa einem Tag liegen, 
etwa drei im deep sleep. 

Entsprechend war nun der Zeitpunkt gekommen, wo wir gedanklich vom Tüfelmodus vollständig auf den Festivalmodus umschwenken konnten.
Hier eine Impression von der Tanzfläche, um das Werk in einen Kontext zu setzen:

![fusion 6](grafiken/fusion_bilder/6.jpg "Impressionen von der Tanzfläche")

Wir freuen uns schon auf nächstes Jahr, dann mit Teleskopstab, sowie einer Ladelösung, die kein Aufschrauben mehr nötig macht.

Wenn ihr Lust bekommen habt, dann meldet euch bei uns! Wir sind vor allem interessiert an neuen Animationen, die wir auf das Ei laden können.