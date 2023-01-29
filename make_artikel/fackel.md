
# Fackel

Die zweite Animation ahmt eine Fackel nach, wobei der Brennstoff entsprechend dem beat nachgeführt wird. Entsprechend der Musik wabern dann Feuerschwaden langsam nach oben. Kurzfristige Änderungen in der Musik - z.B. durch bass drops - führen temporär zu größeren Feuerschwaden, was besonder spektakulär aussieht.

Wie bei der ersten Animation normieren wir zunächst das Mikrofonsignal auf einen Durchschnitt von Null und ermitteln daraus in chunks von 10 Millisekunden die Signalenergie. Die Signalenergie wird nun mit zwei unterschiedlichen Tiefpässen gefiltert. Ein Tiefpass hat eine kurze Zeitkonstante und dient nur zur Glättung des Signals. Der andere Tiefpass hat eine Zeitkonstante von mehreren Sekunden und gibt Auskunft über die durchschnittliche Lautstärke. 

Der folgende Teil wird alle 45 Millisekunden ausgeführt:
Wenn der schnelle Tiefpass mindestens 15% über dem langsamen Tiefpass liegt, dann gehen wir davon aus, dass aktuell ein beat vorliegt. Die untersten beiden LEDs werden nun mit mit der maximalen Helligkeit angesteuert. Alle anderen LEDs ermitteln ihre Helligkeit aus der durchschnittlichen Helligkeit der beiden LEDs darunter im vorhergehenden Zeitschritt, abzüglich einer kleinen Dämpfung. Das führt dazu, dass die Feuerschwaden nach oben hin dunkler, aber auch größer werden. Das Vorgehen ist im folgenden Bild illustriert:

![Illustration Beaterkennung](grafiken/illustrationen/fackel.PNG "Illustration Fackel")

