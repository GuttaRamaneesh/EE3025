import numpy as np
import pandas as pd 
import matplotlib.pyplot as plt
from scipy import signal
import soundfile as sf

def  plot_fft(signal, name, sample_freq):
  plt.figure(figsize=(6,2))
  N = len(signal)
  T = 1/sample_freq
  fft  = np.abs(np.fft.fft(signal))
  fft = fft[0:N//2]
  freq = np.fft.fftfreq(signal.size, d=T)
  freq = freq[:N//2]
  plt.plot(freq, fft)
  plt.xlim([0, 6000])
  plt.savefig(name+'.eps')
  plt.show()
  return fft, freq

if __name__ == "__main__":

  inp_sng, sample_freq = sf.read('Sound_Noise.wav')
  Wn = 6168/sample_freq
  b, a = signal.butter(4, Wn, 'low')
  temp1 = signal.filtfilt(b, a, inp_sng)
  temp = inp_sng.copy()

  for i in range(20):
    temp = signal.filtfilt(b, a, temp)

  original_fft, freq = plot_fft(inp_sng, 'before', sample_freq)
  filtered_fft, freq = plot_fft(temp1, 'after-1', sample_freq)
  cascaded_fft, freq = plot_fft(temp, 'after-2', sample_freq)

  original_pre = 0
  filtered_pre = 0
  cascaded_pre = 0
  original_post = 0
  filtered_post = 0
  cascaded_post = 0

  for i in range(len(freq)):
      if freq[i]<2570:
          original_pre += original_fft[i]
          filtered_pre += filtered_fft[i]
          cascaded_pre += cascaded_fft[i]
      else:
          original_post += original_fft[i]
          filtered_post += filtered_fft[i]
          cascaded_post += cascaded_fft[i]
  print('Pre fraction: ', original_pre/original_fft.sum(), filtered_pre/filtered_fft.sum(), cascaded_pre/cascaded_fft.sum())
  print('Post fraction: ', original_post/original_fft.sum(), filtered_post/filtered_fft.sum(), cascaded_post/cascaded_fft.sum())
