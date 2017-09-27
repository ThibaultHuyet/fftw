#include <iostream>
#include <cmath>
#include <fftw3.h>
#include <string>
#include <mosquitto.h>
#include <cstring>

#ifndef M_PI
#define M_PI (3.14159265359)
#endif

#define TABLE_SIZE (8192)
#define RESULT (TABLE_SIZE/2)

#define MQTT_TOPIC "sound"
#define MQTT_HOSTNAME "localhost"
#define MQTT_PORT 1883

std::string convert(float *arr, int size)
{
    // I am converting the float array to a
    // string so that I can send it via MQTT
    std::string ret;
    std::string temp;
    ret.append("[");
    for (int i = 0; i < size; i++)
    {
        temp = std::to_string(arr[i]);
        ret.append(temp + ", ");
    }
    ret.append("]");
    return ret;
}

void mag(fftwf_complex *out, float *data, int size)
{
    for (int i = 0; i < size/2; i++)
    {
        data[i] = sqrt(out[i][0] * out[i][0]
                        + out[i][1] * out[i][1]);
    }
}

int main()
{
	struct mosquitto *mosq = nullptr;
    mosquitto_lib_init();

    mosq = mosquitto_new(nullptr, true, nullptr);
    if (!mosq)
    {
        exit(-1);
    }

    int ret = mosquitto_connect(mosq, MQTT_HOSTNAME, MQTT_PORT, 0);
    if (ret)
    {
        exit(-1);
    }

	float sine[TABLE_SIZE];
	float data[RESULT];
	fftwf_complex out[RESULT];

	// Generate a sine wave to apply an fft to

	fftwf_plan plan = fftwf_plan_dft_r2c_1d(TABLE_SIZE, sine, out, FFTW_ESTIMATE);

	for (int i=0; i < TABLE_SIZE; i++)
	{
		sine[i] = (float)sin(((float)i/(float)TABLE_SIZE)*M_PI *2.);
	}

	fftwf_execute(plan);
	mag(out, data, RESULT);
	std::string message = convert(data, RESULT);

	const char *c = message.c_str();
	int msgLen = strlen(c);

	ret = mosquitto_publish(
		mosq,               // Initialized with mosquitto_lib_init
		nullptr,            // int *mid
		MQTT_TOPIC,         // Topic to publish to
		msgLen,             // int payload length
		c,                  // Message being sent
		0,                  // Quality of Service
		false               // Retain message
		);

	// If mqtt doesn manage a succesful publish
	// There is an error and program should end
	if (ret)
	{
	exit(-1);
	}

	fftwf_destroy_plan(plan);

	return 0;
}