EEG = readtable('C:/Users/Fedosov/Documents/projects/mks_return_ICA/raw_data/data1half_0808.txt');
raw = table2array(EEG(5000:15000,1)); %60000:150000
figure
plot(raw)
hold on
kalman = KalmanSimple(11.0,0.99,1000,0.0001,1);
kalman.apply(raw)