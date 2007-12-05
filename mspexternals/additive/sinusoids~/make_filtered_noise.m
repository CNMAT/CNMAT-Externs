% matlab script to build the noise table

n = 96000;
gaussnoise = randn(1,n);
fs = 44100;
fc = 500;
Wn = fc/(fs/2);
[B,A] = butter(4, Wn);  % 4th order Butterworth lowpass
goodnoise = filter(B,A,gaussnoise);

plot(db(abs(fft(gaussnoise))))
hold on
plot(db(abs(fft(goodnoise))), 'g')


diary most_of_noisetable.c
for i=1:length(goodnoise)
  disp([num2str(goodnoise(i)) 'f, '])
end
diary off
