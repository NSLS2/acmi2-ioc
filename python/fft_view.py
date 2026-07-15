import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Button
import sys


fig, ax = plt.subplots(figsize=(10,5))
plt.subplots_adjust(bottom=0.2)

csv_file = "fft_data.csv"

if len(sys.argv) > 1:
    csv_file = sys.argv[1]

print(f"Using file: {csv_file}")

def load_and_plot():
    try: 
        data = np.loadtxt(csv_file, delimiter=",", skiprows=1)
    except Exception as e:
        print("Error loading file: ", e)
        return

    freqs = data[:, 0]
    amps = data[:, 1]

    ax.clear()
    ax.plot(freqs, amps, color='blue')
    ax.set_xlabel("Frequency (kHz)")
    ax.set_ylabel("Amplitude")
    ax.set_xlim(0, 20000)
    ax.set_title(f"Recreated FFT (file: {csv_file})")
    ax.grid(True)

    fig.canvas.draw()

# -------------------------
# Refresh button callback
# -------------------------
def refresh(event):
    print("Refreshing data...")
    load_and_plot()

# -------------------------
# Button
# -------------------------
ax_refresh = plt.axes([0.8, 0.05, 0.1, 0.075])
btn_refresh = Button(ax_refresh, 'Refresh')
btn_refresh.on_clicked(refresh)

# initial plot
load_and_plot()

plt.show()