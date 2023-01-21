# -*- coding: utf-8 -*-
"""
Created on Thu Jan 19 02:12:48 2023

@author: Fedosov
"""



import numpy as np
import scipy.signal as sn

#data = np.random.randn(10000, 3)





class CFIRBandEnvelopeDetector:
    def ideal_H(self,n_fft, fs, band, delay=0):
            """
            Estimate ideal delayed analytic filter freq. response
            :param n_fft: length of freq. grid
            :param fs: sampling frequency
            :param band: freq. range to apply band-pass filtering
            :param delay: delay in samples
            :return: freq. response
            """
            w = np.arange(n_fft)
            H = 2*np.exp(-2j*np.pi*w/n_fft*delay)
            H[(w/n_fft*fs<band[0]) | (w/n_fft*fs>band[1])] = 0
            return H
    
    
    def __init__(self, band, fs, delay_ms=0, n_taps=500, n_fft=2000, reg_coeff=0):
        """
        Complex-valued FIR envelope detector based on analytic signal reconstruction
        :param band: freq. range to apply band-pass filtering, Hz
        :param fs: sampling frequency, Hz
        :param delay_ms: delay of ideal filter in ms
        :param n_taps: length of FIR
        :param n_fft: length of freq. grid to estimate ideal freq. response
        :param reg_coeff: least squares L2 regularisation coefficient
        """
        H = self.ideal_H(n_fft, fs, band, int(delay_ms*fs/1000))
        F = np.array([np.exp(-2j * np.pi / n_fft * k * np.arange(n_taps)) for k in np.arange(n_fft)])
        self.b = cLS(F, H, reg_coeff)
        self.a = np.array([1.])
        #self.zi = np.zeros(len(self.b)-1)
        #self.smoother = smoother

    def apply(self, chunk: np.ndarray):
        y= sn.lfilter(self.b, self.a, chunk,axis = -1)
        #y = self.smoother.apply(np.abs(y))
        return y
    
    
    
    
    
    
def cLS(X, Y, lam=0):
    """
    Complex valued Least Squares with L2 regularisation
    """
    reg = lam*np.eye(X.shape[1])
    b = np.linalg.inv(X.T@X.conj()+reg)@X.T.conj()@Y
    return b



def band_hilbert(x, fs, band, N=None, axis=-1):
    """
    Non-causal bandpass Hilbert transform to reconstruct analytic narrow-band signal
    :param x: input signal
    :param fs: sampling frequency
    :param band: band of interest
    :param N: fft n parameter
    :param axis: fft axis parameter
    :return: analytic narrow-band signal
    """
    #x = np.asarray(x)
    Xf = np.fft.fft(x, N, axis=axis)
    w = np.fft.fftfreq(N or x.shape[0], d=1. / fs)
    Xf[(w < band[0]) | (w > band[1])] = 0
    x = np.fft.ifft(Xf, axis=axis)[:x.shape[0]]
    return 2*x



#def temporal_fitting_envelope(x, ideal_env):
    
    
    # MSE FITTING of envelope 
    
   
    
def corr_latency(self, y):
    pass
    
    
    
    
class whiteKF:
    """
    Standard Kalman filter implementation
    Implementation follows eq. (2, 3) from [1]
    Parameters
    ----------
    Phi : np.ndarray of shape(n_states, n_states)
        State transfer matrix
    Q : np.ndarray of shape(n_states, n_states)
        Process noise covariance matrix
    H : np.ndarray of shape(n_meas, n_states)
        Matrix of the measurements model; maps state to measurements
    R : np.ndarray of shape(n_meas, n_meas)
        Driving noise covariance matrix for the noise AR model
    init_state : Gaussian, optional, default=None
        Initial state vector and its covariance
    References
    ----------
    .. [1] Wang, Kedong, Yong Li, and Chris Rizos. “Practical Approaches to
    Kalman Filtering with Time-Correlated Measurement Errors.” IEEE
    Transactions on Aerospace and Electronic Systems 48, no. 2 (2012): 1669–81.
    https://doi.org/10.1109/TAES.2012.6178086.
    """
    
    def generate_matsuda_params(fs: float, fc: float):
        arg = 2.0*np.pi*fc/fs
        Phi = np.array([[np.cos(arg),-np.sin(arg)],[np.sin(arg),np.cos(arg)]])
        
        
        return Phi, H
        

    def __init__(self, Phi, Q, H, R, init_x = None, init_P = None):
        self.Phi = Phi
        
        
        self.Phi = Phi
        self.Q = Q
        self.H = H
        self.R = R

        n_states = Phi.shape[0]
        if (init_x is None) or (init_P is None):
            
            self.x = np.random.randn(n_states, 1)
            self.P = np.eye(n_states) * 1e-3
        else:
            self.x = init_x
            self.P = init_P

    def predict(self, state):
        x_ = self.Phi @ state.x
        P_ = self.Phi @ state.P @ self.Phi.T + self.Q
        return x_, P_

    def update(self, y, x_, P_):
        Sigma = self.H @ P_ @ self.H.T + self.R
        Pxn = P_ @ self.H.T

        K = np.linalg.solve(Sigma, Pxn.T).T
        n = y - self.H @ x_
        self.x = x_ + K @ n
        self.P = P_ - K @ Sigma @ K.T
        
        return self.x, self.P

    def update_no_meas(self, state_):
        """Update step when the measurement is missing"""
        self.state = state_
        return state_



    def step(self, y):
         state_ = self.predict(self.state)
         return self.update(y, state_) if y is not None else self.update_no_meas(state_)

    
    
    
    
#LOAD REAL DATA


import mne
from matplotlib import pyplot as plt
def offline_test(): # make path as argument
    raw = mne.io.read_raw_bdf('C:/Users/Fedosov/Documents/NeoRec/Records/resting_state_prediciton/NeoRec_1.bdf')
    
    data = raw[:,:][0]
    
    fs = raw.info['sfreq']
    
    
    cfir = CFIRBandEnvelopeDetector(band = [8.0,12.0], fs = fs, delay_ms=0, n_taps=fs//5, n_fft=fs, reg_coeff=0)
    
    
    filtered = cfir.apply(data)
    
    plt.figure()
    plt.plot(np.abs(filtered[0]))
    plt.plot(data[0])
    
    
    
    
    
    
    
    kalman = (Phi, Q, H, R, init_x = None, init_P = None):



    
    
    
    
    
    

    






