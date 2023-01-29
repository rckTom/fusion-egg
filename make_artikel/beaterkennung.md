
# Beaterkennung

Die erste von zwei Animationen besteht aus einer Welle, welche von jedem zweiten beat bis zum jeweils nächten beat von oben nach unten läuft. 
Dafür muss zu jedem Zeitpunkt sowohl die Frequenz als auch die Phase des beats bekannt sein, und das auch über kürzere Störungen oder komplexere Rythmen hinweg.

Die zentrale Idee des Algorithmus ist es, Bandpassfilter mit schwacher Dämpfung als Phasenschätzer zu missbrauchen. 
Hierfür wird eine ganze Reihe an Bandpässen mit unterschiedlichen Durchlassfrequenzen parallel geschaltet. Diese Durchlassfrequenzen gehören zu unterschiedlichen beat-Geschwindigkeiten. In unserem Fall von 105 bis 150bpm, der typische Bereich elektronischer Tanzmusik. Auf jeden dieser Bandpässe wird nun die Signalenergie gegeben. Der Bandpass, der sich daraufhin am meisten aufschwingt, hat dann offensichtlich am besten die richtige Geschwindigkeit getroffen. 

Im Detail: Zuerst sammeln wir samples vom Mikrofon. Wir ziehen den gleitenden Durchschnitt ab, um ein Signal zu erzeugen, welches um Null herum schwingt. Über jeweils einen chunk - eine Vierzigstelsekunde - summieren wir quadrierte samples, um die Signalenergie in diesem chunk zu erhalten. Alles weitere passiert nun ebenfalls 40 mal die Sekunde. Wir geben die Signalenergie parallel als input auf alle Bandpässe - zufällig ebenfalls 40 an der Zahl. Eine der Bandpässe schwingt sich am meisten auf. Wir gehen davon aus, dass dessen Resonanzfrequenz am nähesten am tatsächlichen beat liegt. Kleine Abweichungen werden durch das ständige feedback fortlaufend korrigiert. Nun haben wir das Problem, dass der Bandpass einmal pro beat schwingt, unsere gewollte Animation aber zwei beats benötigt. Wir müssen also irgendwie die Frequenz halbieren. Dafür verzögern wir das Signal um Pi/4 und schätzen mit der atan2-Funktion die Phase. Mit der bekannten Phase und einem einfachen Zustandsautomat konstruieren wir uns die Phase eines Systems mit halber Frequenz. Diese Phase skalieren wir linear und erhalten so die Sollposition des Maximums der Lichtwelle. 

Wir wissen nun also zu jedem Zeitpunkt die Position des Scheitelpunktes der Lichtwelle. Um diese Position herum wird die Helligkeit langsam gesenkt und damit die Welle geformt. Die Maxima für den roten und blauen Kanal sind dabei gegenläufig verschoben, so dass ein räumlicher Farbverlauf entsteht. 

Das Ganze ist hier illustriert:

![Illustration Beaterkennung](grafiken/illustrationen/beaterkennung.PNG "Illustration Beaterkennung")


