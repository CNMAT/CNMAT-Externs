These are Richard Dudas's "double-tilde" FFT/IFFT objects, now superceded by MSP's built-in
pfft world.

These objects pass FFT spectra to each other as pointers in the s_thing field of a special
symbol, thereby avoiding the latency of "trickling" the spectral data between objects by
representing it as a bogus signal.

-Matt
021210
