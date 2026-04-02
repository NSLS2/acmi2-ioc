from cothread.catools import caget, caput
import numpy as np
from time import sleep

oldTrig = 0
while True:
    Trig = caget("lab{ACMI:A}TrigCnt-I")
    if(Trig != oldTrig):
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
        amplitude = (2.0/n)*np.abs(fft_values)
# Frequency axis in Hz
        freqs = np.fft.rfftfreq(n, d=dt)
# Convert to kHz
        freqs_khz = freqs / 1000
# Write Output PVs:
        caput("lab{ACMI:A}ADC:FFT:Freq-Wfm", freqs_khz[0:8000])
        amplitude[0] = 0
        amplitude[1] = 0
        caput("lab{ACMI:A}ADC:FFT:Ampl-Wfm", amplitude[0:8000])
    else:
        sleep(0.1)
