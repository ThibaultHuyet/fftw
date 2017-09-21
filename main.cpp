#include <iostream>
#include <cmath>
#include <fstream>
#include <fftw3.h>

#ifndef M_PI
#define M_PI (3.14159265359)
#endif

#define RESULT (1000)
#define TABLE_SIZE (2*(RESULT/(2+1)))

void do_something_with(fftw_complex* result)
{
	std::ofstream myfile;
	myfile.open("sine.csv");

    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        double mag = sqrt(result[i][0] * result[i][0] +
                          result[i][1] * result[i][1]);

        myfile << mag << "\n";
    }
}

int main()
{
	double sine[TABLE_SIZE];
	fftw_complex out[RESULT];

	// Generate a sine wave to apply an fft to

	fftw_plan plan = fftw_plan_dft_r2c_1d(TABLE_SIZE, sine, out, FFTW_ESTIMATE);

	for (int i=0; i < TABLE_SIZE; i++)
	{
		sine[i] = (float)sin(((double)i/(double)TABLE_SIZE)*M_PI *2.);
	}

	fftw_execute(plan);
	do_something_with(out);
	fftw_destroy_plan(plan);

	// Following block of code is just an easy way to that a sine wave was made
	// Writes a csv file so that I can graph sine wave easily
	/*ofstream myfile;
	myfile.open("sine.csv");
	for(int i = 0; i < table_size; i++)
	{
		myfile << i << "," << sine[i] << ",\n";
	}*/


	return 0;
}

