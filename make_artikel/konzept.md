
# Konzept

Der weiße Teil des Eis besteht aus drei Teilen, die jeweils 3D-gedruckt sind. Der mittlere dieser Teile ist für schnellen Druck im Vasenmodus ausgeführt - mehr dazu später. 

Im Inneren des mittleren weißen Teils befindet sich zur Stabilisierung ein geklebtes Skelett aus lasergeschnittenem Holz. Dieses ist formschlüssig mit dem mittleren weißen Teil verbunden und mit den äußeren weißen Teilen verschraubt. 

Im Inneren des Holzskeletts ist eine Holzplatte eingespannt, welche als Träger der Elektronik fungiert. 

Das Gesamtkonzept ist in der folgenden Explosionsansicht illustriert:

TODO explosionsansicht

Das Ei sitzt auf einem gedruckten Sockel, die Verbindung wird über Schrauben hergestellt.

Der Sockel enthält einen runden Schlitz, um ein handelsübliches Alurohr aus dem Baumarkt hereinzustecken. Die leichte Spielpassung in Verbindung mit den typischen Druckertoleranzen sorgt hier für einen Reibkraftschluss. 

Im Inneren befindet sich ein ESP32 als Mikrocontroller, ein Mikrofon, ein Beschleunigungssensor, ein 10Ah USB-Reiseakkupack und drei WS2812 LED-Bänder. 

Auf dem ESP32 läuft ein in C++ geschriebenes Programm, welches zwischen verschiedenen Animationen umschaltet. Die Umschaltung erfolgt durch den Beschleunigungssensor - ein doppeltes Aufstampfen des Stabes auf dem Boden wird hier als Umschaltsignal interpretiert. 

Auf dem Festival hatten wir zwei verschiedene Animationen implementiert, die wir hier "Beaterkennung" und "Fackel" nennen. Die Beaterkennung lässt auf jeden zweiten Beat eine Lichtwelle über das Ei laufen, die Fackel bildet ein Feuer nach, welches durch hohe Lautstärke angefacht wird. Nach den beiden Animationen geht das Ei in einen deep sleep, ein weiteres Doppelstampfen bringt es dann wieder auf die erste Animation.
