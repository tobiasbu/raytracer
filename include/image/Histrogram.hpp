
#include "image.h"

class Histrogram
{
private:

	Image * image = nullptr;

	unsigned char maxr;
	unsigned char maxg;
	unsigned char maxb;
	unsigned char maxa;

	float freqsums[5];

	// frequencies
	float value[256];
	float r[256];
	float g[256];
	float b[256];
	float a[256];

public:

	Histrogram(Image * img);
	void calculate();
};