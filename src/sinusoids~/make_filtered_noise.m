% matlab script to build the noise table

n = 129088;
gaussnoise = randn(1,n);
fs = 44100;
fc = 500;
Wn = fc/(fs/2);
[B,A] = butter(4, Wn);  % 4th order Butterworth lowpass
goodnoise = filter(B,A,gaussnoise);

plot(db(abs(fft(gaussnoise))))
hold on
plot(db(abs(fft(goodnoise))), 'g')


fid = fopen('most_of_noisetable.c', 'wt');
for i=1:length(goodnoise)
  fprintf(fid, [num2str(goodnoise(i)) 'f, \n']);
end
fclose(fid)
