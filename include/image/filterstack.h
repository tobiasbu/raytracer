
#ifndef _FILTERSTACK_H_
#define _FILTERSTACK_H_


#include <queue>
#include "filter.h"

class FilterStack {

public:

	struct ColorEffect {

		bool isFilter;

		Filter * filter;

		std::string type;
		float a;
		float b;
		ColorEffect(std::string t, float c, float d)
			:
			type(t),
			a(c),
			b(d)
		{
			isFilter = false;
		}

		ColorEffect(Filter * filter) {
			this->filter = filter;
				isFilter = true;
		}
	};

	std::queue<ColorEffect *> _filters;
	//std::queue<ColorEffect *> _colors;

	void addFilter(const std::string & type) {

		if (type == "sharpen") {

			//_filters.push(new ColorEffect(new Sharpen));

		}
		else if (type == "blur") {
			//_filters.push(new ColorEffect(new Blur));
		}
		else if (type == "unsharpmask") {
			//_filters.push(new ColorEffect(new UnsharpMask));
		}
		else if (type == "edge" || type == "edgedetection") {
			//_filters.push(new ColorEffect(new EdgeDetection));
		}
		else if (type == "emboss") {
			//_filters.push(new ColorEffect(new Emboss));
		}
		else if (type == "gaussianblur") {
			//_filters.push(new ColorEffect(new GaussianBlur));
		}
		else if (type == "sobel" || type == "sobelemboss") {
			//_filters.push(new ColorEffect(new SobelEmboss));
		}

	}

	void addColor(const std::string & type, float a, float b) {

		_filters.push(new ColorEffect(type, a, b));
	}

};

#endif