
#include "image/Histrogram.hpp"
#include "math/maths.h"
#include <iostream>


Histrogram::Histrogram(Image * img)
{
	if (!img || img != nullptr || img != NULL)
		image = img;
	else
		return;

	calculate();
}

void Histrogram::calculate()
{
	if (image != nullptr)
	{

		// initialize
		for (int i = 0; i < 256; i++)
		{
			value[i] = 0;
			r[i] = 0;
			g[i] = 0;
			b[i] = 0;
			a[i] = 0;
		}

		// find max values
		
		const unsigned char * ptr = image->getPixelsPtr();
		const unsigned char * end = &ptr[image->getBufferSize() - 1];

		maxr = ptr[0];
		maxg = ptr[1];
		maxb = ptr[2];
		maxa = ptr[3];

		float sumr = 0;
		freqsums[0] = 0;
		freqsums[1] = 0;
		freqsums[2] = 0;
		freqsums[3] = 0;
		freqsums[4] = 0;

		while (ptr < end)
		{
			if (ptr[0] > maxr)
				maxr = ptr[0];

			if (ptr[1] > maxg)
				maxg = ptr[1];

			if (ptr[2] > maxb)
				maxb = ptr[2];

			if (ptr[3] > maxa)
				maxa = ptr[3];

			freqsums[0] += ptr[0];
			freqsums[1] += ptr[1];
			freqsums[2] += ptr[2];
			freqsums[3] += ptr[3];
			freqsums[4] += 0.5f * Math::max(ptr[0], Math::max(ptr[1], ptr[2])) + 0.5f *  Math::min(ptr[0], Math::min(ptr[1], ptr[2]));

			r[ptr[0]] += ptr[0];
			g[ptr[1]] += ptr[1];
			b[ptr[2]] += ptr[2];
			a[ptr[3]] += ptr[3];
			
			

			/*r[ptr[0]]++; 
			g[ptr[1]]++; 
			b[ptr[2]]++; 
			a[ptr[3]]++;*/
			
			ptr += 4;

		}

		// find maxes
		/*float maxrr = r[0];

		for (int i = 0; i < 256; i++)
		{
			if (r[i] > maxrr)
				maxrr = r[i];

		}
		*/

		
		//float ta = 0, tb = 0, tc = 0;

		// compute frequencies (normalized)
		for (int i = 0; i < 256; i++)
		{
			value[i] = (0.5f * Math::max(r[i], Math::max(g[i], b[i])) + 0.5f *  Math::min(r[i], Math::min(g[i], b[i]))) / freqsums[4];

			r[i] /= freqsums[0];
			g[i] /= freqsums[1];
			b[i] /= freqsums[2];
			a[i] /= freqsums[3];
			

			std::cout << r[i] << " " << value[i] << std::endl;
			//ta += value[i];
			//tb += g[i];
			//tc += b[i];
		}
		//std::cout << ta << " " << tb << " " << tc << std::endl;
		
		
	}
}