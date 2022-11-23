
import matplotlib.pyplot as plt
import numpy as np


n_plots = 8
endtime = 4
size = 1000

time = np.linspace(0,endtime,size)
audio = 1023*np.random.random(size=(size))
audio = 512+(audio-512)*(0.2)*np.sin(np.linspace(0,endtime*2*3.14*2, size))+(audio-512)*0.3

#fig, axs = plt.subplots(n_plots, 1, sharex=True)
fig = plt.figure(figsize=(10,10))
gs = fig.add_gridspec(n_plots, hspace=0)
axs = gs.subplots(sharex=True)

axs[0].set_xlim((-0,4))


titlex = 0.5
titley = 0.5

axs[0].plot(time,audio)
axs[0].set_ylabel("Mikrofon\n-signal", x=titlex, y=titley)

audio_norm = audio-512
axs[1].plot(time,audio_norm)
axs[1].set_ylabel("Mikrofon\n-signal\nnormiert", x=titlex, y=titley)

spc = size/endtime



audio_norm = np.array(audio_norm)
audio_squared = np.square(audio_norm)

axs[2].plot(time, audio_squared)
axs[2].set_ylabel("Signal\n-energie", x=titlex, y=titley)

spc = int(size/endtime/40)

chunks = list()
chunktimes = list()
energy = 0
i = 0
for sample, timepoint in zip(audio_squared, time):
    energy += sample
    i += 1
    if i > spc:
        i = 0
        chunks.append(energy)
        chunktimes.append(timepoint)
        energy = 0

axs[3].plot(chunktimes, chunks)
axs[3].set_ylabel("Signale\n-nergie\nchunks", x=titlex, y=titley)

n_BP = 5
SAMPLING_FREQUENCY_BP = 40
import math


filter_outputs = list()

angles = list()
angles2 = list()

delayed = list()


for i in range(n_BP):
    filter_output = list()
    Q = 10
    frequency = 2.2 + i * 0.2
    w0 = 2. * 3.1415 * frequency / SAMPLING_FREQUENCY_BP
    a = math.sin(w0 / (2. * Q))
    b0 = a
    b1 = 0.
    b2 = -a
    a0 = 1. + a
    a1 = -2. * math.cos(w0)
    a2 = 1. - a

    x, xn1, xn2, yn1, yn2 = 0,0,0,0,0
    yn3, yn4, yn5 = 0,0,0
    angle2 = 0
    for chunk in chunks:
        y = b0 * chunk + b1 * xn1 + b2 * xn2 - yn1 * a1 - yn2 * a2

        xn2 = xn1
        xn1 = x

        yn5 = yn4
        yn4 = yn3
        yn3 = yn2
        yn2 = yn1
        yn1 = y

        if i==1:
            angle = math.atan2(yn5, y)
            angles.append(angle)
            delayed.append(yn5)


            PI = 3.141592
            if (PI < abs(angle - angle2) and abs(angle - angle2) < 3 * PI):
            
                angle2 = angle + 2 * PI
            
            else:
            
                angle2 = angle

            angles2.append(angle2)
            

        filter_output.append(y)

    axs[4].plot(chunktimes, filter_output, color="#1f77b4" if i == 1 else "grey")

    if i == 1:
        axs[5].plot(chunktimes, filter_output)

    filter_outputs.append(filter_output)

axs[4].set_ylabel("Band\n-pässe", x=titlex, y=titley)

axs[5].plot(chunktimes, delayed, color="grey")
axs[5].set_ylabel("Delay", x=titlex, y=titley)

axs[6].plot(chunktimes, angles)
axs[6].set_ylabel("Geschätzter\nPhasen\nwinkel", x=titlex, y=titley)

axs[7].plot(chunktimes, angles2)
axs[7].set_ylabel("Halbierte\nFrequenz", x=titlex, y=titley)

for i in range(n_plots):
    axs[i].set_yticks(())




plt.savefig("beaterkennung.svg")
plt.savefig("beaterkennung.png", dpi=500)
plt.show()