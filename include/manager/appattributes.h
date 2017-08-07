
#ifndef   _APPATTRIBUTES_H_
#define   _APPATTRIBUTES_H_

#include "image\filterstack.h"
#include "entity\scene.h"
#include "image\image.h"
#include "entity\camera.h"
#include "manager\projection.h"
#include <string>
#include <atomic>
#include "manager\ConfigParam.h"

/*
struct Stastitics 
{
	std::atomic<int32_t> primaryRays;
	std::atomic<int32_t> shadowRays;
	std::atomic<int32_t> reflectionRays;
	std::atomic<int32_t> refractionRays;
	std::atomic<int32_t> aabbTests;
	std::atomic<int32_t> intersectionTests;
};*/

struct Options {
	int programType;

	int antiAliasSamples; //

	float bias;

	uint16_t shadowType;//
	int shadowSamples;
	float shadowIntensity;
	uint8_t penumbraType;

	uint8_t outlineType;
	Color outlineColor;

	uint8_t renderType;
	vec3 warmColor;
	vec3 coolColor;
	bool celOutline;
	float goshAlpha;
	float goshBeta;
	int cellevel;
	float celfactor;

	bool threadFunc;

	uint32_t maxDepth;
	uint8_t traceMethod;
	uint8_t threads;
	
	std::string outputfileName;
	
	uint8_t renderMethod;
	
	Options()
		:
		programType(0),
		antiAliasSamples(0),
		bias(1e-4f), // 1e-4f // 0.00001f
		outputfileName("output.png"),
		maxDepth(2),
		shadowType(2),
		shadowIntensity(0.5),
		traceMethod(2),
		renderMethod(0),
		threads(0),
		renderType(0), // phong, cel, goosh, gosh two tones
		outlineType(0), // outline for gosh or cel shading
		cellevel(5),
		warmColor(195.0f / 255, 255.0f / 255, 75.0f / 255),
		coolColor(35.0f / 255, 90.0f / 255, 240.0f / 255),
		threadFunc(false)
	{
		shadowSamples = 3;
		penumbraType = 0;

		outlineType = 0;
		outlineColor = Color::black;

		celfactor = 1.0 / (float)cellevel;
		goshAlpha = 0.25f;
		goshBeta = 0.65f;
		celOutline = true;

		
	}

	void reset() {
		maxDepth = 2;
		shadowType = 2;
		antiAliasSamples = 0;
		shadowIntensity = 1;
		outputfileName = "output.png";
		threadFunc = false;
	}

	std::string getTraceMethodName() const
	{
		switch (traceMethod)
		{
		case 0: return "List";
		case 1: return "AABB";
		case 2: return "BVH";
		case 3: return "SAH-BVH";

		default:
			return "unknown";
			break;
		}
	}

	std::string getRenderMethodName() const
	{
		switch (renderMethod)
		{
		case 0: return "Full";
		case 1: return "Only AABB (longest axis)";
		default:
			return "unknown";
			break;
		}
	}

	void passConfigTable(std::vector<ConfigParamBase *> & params)
	{
		for (int i = 0; i < params.size(); i++)
		{
			const ConfigParamBase * param = params[i];
			const std::string & name = param->name;

			if (param->type == 1)
			{
				bool value = static_cast<const ConfigParam<bool>*>(param)->value;

				if (name == "cel/gooch outline: ")
					celOutline = value;
				

			}
			else if (param->type == 2)
			{
				int value = static_cast<const ConfigParam<int>*>(param)->value;

				if (name == "max depth: ")
					maxDepth = value;
				else if (name == "anti-aliasing: ")
					antiAliasSamples = value;
				else if (name == "cel divisions: ")
					cellevel = value;
				else if (name == "gooch alpha: ")
					goshAlpha = (float)value / 100.f;
				else if (name == "gooch beta: ")
					goshBeta = (float)value / 100.f;
				else if (name == "shadows intensity: ")
					shadowIntensity = (float)value / 100.f;
				else if (name == "shadows samples: ")
					shadowSamples = value;
				else if (name == "penumbra type: ")
					penumbraType = value;
				else if (name == "threads: ")
					threads = value;
			}
			else if (param->type == 3)
			{
				int value = static_cast<const ConfigParam<const char*>*>(param)->current;

				if (name == "shading: ")
					renderType = value;
				else if (name == "shadows: ")
					shadowType = value;
				else if (name == "edges: ")
					outlineType = value;
				else if (name == "trace method: ")
					traceMethod = value;
				else if (name == "thread func: ") 
				{
					if (value == 1)
						threadFunc = true;
					else
						threadFunc = false;
				}
			}

		}

		if (cellevel != 0)
			celfactor = 1.0 / (float)cellevel;
		else
			celfactor = 0;
	}
};


struct AppAttributes {
	
	Options options;
	Projection projection;
	Scene scene;
	Image * image;
	Camera camera;
	FilterStack filters;
	
};

#endif