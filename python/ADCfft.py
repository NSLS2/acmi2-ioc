from cothread.catools import caget
import matplotlib.pyplot as plt
from matplotlib.widgets import Button
import numpy as np
from time import sleep
import csv

plt.ion()
fig, ax = plt.subplots(figsize=(10,5))
plt.subplots_adjust(bottom=0.2)

latest_frame = None

# -------------------------
# Save button callback (CSV)
# -------------------------
def save_data(event):
    global latest_frame

    if latest_frame is None:
        print("No data yet.")
        return

    freqs, amps = latest_frame

    with open("fft_data.csv", "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["freq_khz", "amplitude"])  # header

        for f_khz, amp in zip(freqs, amps):
            writer.writerow([f_khz, amp])

    print("Saved latest FFT to fft_data.csv")

# -------------------------
# Button
# -------------------------
ax_save = plt.axes([0.8, 0.05, 0.1, 0.075])
btn_save = Button(ax_save, 'Save')
btn_save.on_clicked(save_data)

# -------------------------
# Main loop
# -------------------------
while True:
    A = caget("LN-BI{ACMI2:A}ADC-Wfm")

    dt = 0.000005 * 1e-3
    n = len(A)

    fft_values = np.fft.rfft(A)
    amplitude = np.abs(fft_values)

    freqs = np.fft.rfftfreq(n, d=dt) / 1000

    amplitude[0] = 0
    amplitude[1] = 0

    latest_frame = (freqs, amplitude)

    ax.clear()
    ax.plot(freqs, amplitude, color='blue')
    ax.set_xlim(0, 20000)
    ax.set_xlabel("Frequency (kHz)")
    ax.set_ylabel("Amplitude")
    ax.set_title("FFT Amplitude Spectrum")
    ax.grid(True)

    plt.pause(0.01)
    sleep(1)
