num AntiAliasKernel {
	Box,
	Tent,
	Gaussian,
	MitchellNetravali,
	CatmullRom,
	Lanczos
};

class AntiAliasing {

	AntiAliasKernel kernel;
	float radius;
	float radiusX, radiusY;
	int samples;

};