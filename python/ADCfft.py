from cothread.catools import caget, caput
import matplotlib.pyplot as plt
import numpy as np
from time import sleep

plt.ion()
f,ax = plt.subplots(figsize=(10,5))
while True:
    ADCpv = "lab{ACMI:A}ADC-Wfm"

    A = caget(ADCpv)

# Sampling interval
    dt_ms = 0.000005          # milliseconds
    dt = dt_ms * 1e-3         # convert ms to seconds

# Number of samples
    n = len(A)

# Sampling frequency
    fs = 1 / dt               # Hz

# Compute FFT (real FFT since input is real)
    fft_values = np.fft.rfft(A)

# Amplitude
    amplitude = np.abs(fft_values)

# Frequency axis in Hz
    freqs = np.fft.rfftfreq(n, d=dt)

# Convert to kHz
    freqs_khz = freqs / 1000

# Print results
    print("Frequencies (kHz):")
    print(freqs_khz)
#    caput("lab{ACMI:A}ADC:FFT:Freq-Wfm", freqs_khz[0:8000])

    amplitude[0] = 0
    amplitude[1] = 0
    print("Length",len(amplitude))
#    caput("lab{ACMI:A}ADC:FFT:Ampl-Wfm", amplitude[0:8000])
# Plot
    ax.clear()
    ax.plot(freqs_khz, amplitude)
    ax.set_xlim(0,20000)
    ax.set_xlabel("Frequency (kHz)")
    ax.set_ylabel("Amplitude")
    ax.set_title("FFT Amplitude Spectrum")
    ax.grid(True)
    plt.pause(0.01)
    sleep(1)
plt.show()

