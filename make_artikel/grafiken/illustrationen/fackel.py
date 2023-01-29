
import matplotlib.pyplot as plt
import numpy as np

n_plots = 3
titlex  = 0.5
titley  = 0.5
endtime = 4
size = 500

#fig, axs = plt.subplots(n_plots, 1, sharex=True)
fig = plt.figure(figsize=(10,4))
gs = fig.add_gridspec(n_plots, hspace=0)
axs = gs.subplots()


##############################################

time = np.linspace(0,endtime,size)
audio = 1023*np.random.random(size=(size))
audio = 512+(audio-512)*(0.2)*np.sin(np.linspace(0,endtime*2*3.14*2, size))+(audio-512)*0.3

for i in range(size):
    if i < size/2:
        audio[i] = 0.5*audio[i]+256

axs[0].set_xlim((-0,4))
axs[0].plot(time,audio)
axs[0].set_ylabel("Mikrofon\n-signal", x=titlex, y=titley)


audio_norm = audio-512


spc = size/endtime



audio_norm = np.array(audio_norm)
audio_squared = np.square(audio_norm)


audio_squared_filtered = list()
state = 0
for sample in audio_squared:
    state += (sample-state)*0.01
    audio_squared_filtered.append(state)


audio_squared_filtered2 = list()
state = 0
for sample in audio_squared:
    state += (sample-state)*0.1
    audio_squared_filtered2.append(state)


audio_squared = np.array(audio_squared)
audio_squared_filtered = np.array(audio_squared_filtered)
audio_squared_filtered2 = np.array(audio_squared_filtered2)

axs[1].plot(time, audio_squared)
axs[1].plot(time, audio_squared_filtered, "k--")
axs[1].plot(time, audio_squared_filtered2, "k")
axs[1].set_ylabel("Signal\n-energie\n(gefiltert)", x=titlex, y=titley)
axs[1].set_xlim((-0,4))


n_LED = 500

animation = np.zeros((size, n_LED))

for i in range(size):



    for j in range(n_LED):


        if i == 0:
            animation[i,j] = 0

        else:
            if j == 0:
                animation[i,j] = 10 if audio_squared_filtered2[i]*1.15 < audio_squared_filtered[i] else 255
            elif j == 1:
                animation[i,j] = 10 if audio_squared_filtered2[i]*1.15 < audio_squared_filtered[i] else 255
            elif j == 2:
                animation[i,j] = 10 if audio_squared_filtered2[i]*1.15 < audio_squared_filtered[i] else 255
            else:
                animation[i,j] = (animation[i-1,j-1]+animation[i-1,j-2]+animation[i-1,j-3])*0.33
            

print(repr(animation))

animation = animation.T

animation = np.flip(animation, axis=0)

axs[2].imshow(animation, cmap="Blues")
axs[2].set_aspect('auto')
axs[2].set_ylabel("LED\nLicht-\nverlauf", x=titlex, y=titley)



for i in range(n_plots):
    axs[i].set_yticks(())
    axs[i].set_xticks(())


#axs[2].set_xlabel("Zeit")


plt.savefig("fackel.svg")
plt.savefig("fackel.png", dpi=500)
plt.show()