# FFTW Basic Example

My input from a microphone will be as a 1D array of only real data. Thus I need to use the fftw_plan_dft_r2c_1d to apply a fft to it.

## Explanation

1. Generate sine wave
2. Create a fftw3 plan using fftw_plan_dft_r2c_1d
3. Execute plan
4. Find magnitude result.
5. Destroy plan