
import matplotlib.pyplot as plt
import numpy as np


n_plots = 5
endtime = 4
size = 1000

time = np.linspace(0,endtime,size)
audio = 1023*np.random.random(size=(size))
audio = 512+(audio-512)*(0.2)*np.sin(np.linspace(0,endtime*2*3.14*2, size))+(audio-512)*0.3

fig, axs = plt.subplots(n_plots, 1)
axs[0].plot(time,audio)

audio_norm = audio-512
axs[1].plot(time,audio_norm)

spc = size/endtime



audio_norm = np.array(audio_norm)
audio_squared = np.square(audio_norm)

axs[2].plot(time, audio_squared)

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




plt.show()