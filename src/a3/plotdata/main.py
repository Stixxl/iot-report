import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('iot_g4_signal_strength.csv')
df = df[['id', 'gwaddr','datarate','snr','rssi','freq','fcount','name']]

fig, ax = plt.subplots(figsize=(10, 5), dpi=120)

df.boxplot(column=['rssi'], by='name', ax=ax)
#df.boxplot(column=['snr'], by='datarate', ax=ax)

fig.suptitle("Received Signal Strength Indication (RSSI) at different locations")
ax.set_title("")
ax.set_xlabel("Location")
ax.set_ylabel("RSSI (dBm)")

#fig.suptitle("Signal to noise ratio with different Spreading Factors")
#ax.set_title("")
#ax.set_xlabel("Spreading Factor")
#ax.set_ylabel("SNR (dB)")

plt.show()
fig.savefig('RSSI_Locations.png')
#fig.savefig('SNR_SF.png')


