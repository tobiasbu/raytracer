
#include "manager\raytracerapp.h"

#include "others\log.h"
#include <chrono>


#include "image\imagecolors.h"

#include "image\filter.h"

#include <iostream>
#include "utils/stringutils.h"

RaytracerApp::RaytracerApp() {

	Debug::log_restart();

}

RaytracerApp::~RaytracerApp() {

	

}

int RaytracerApp::loadScene(const std::string & in) 
{
	auto timeStart = std::chrono::high_resolution_clock::now();

	int r = -1;

	if (in.length() > 0) {

		//_appStartTime = std::chrono::high_resolution_clock::now();

		begin = clock();

		if (StringUtils::fileExtension(in).length() == 0) {
			// in == "default" || in == "raycast" || in == "raytracer" || in == "ray" || in == "raytrace" || 
			// input example scene

			
			const char * filename = in.c_str();

			r = _input.loadExampleScene(filename, _attributes);
			
		}
		else {
			// input file scene
			r = _input.load(in.c_str(), _attributes);
		}

	}

	if (r == 1) 
	{
		valid = true;
		auto timeEnd = std::chrono::high_resolution_clock::now();
		auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
		Debug::log("--- Scene Parser\n");
		Debug::log("Parser time: %.4f (seconds)\n", passedTime / 1000);
		Debug::log("Total of objects: %d\n", _attributes.scene.getPrimitives().size());
		Debug::log("Primitives: %d | Meshes: %d | Triangles: %d\n", _attributes.scene.getPrimitivesCount(), _attributes.scene.getMeshesCount(),_attributes.scene.getTrianglesCount() );
		Debug::log("Lights: %d | Materials: %d | Textures: %d\n", _attributes.scene.getLights().size(), _attributes.scene.getMaterialsCount(), _attributes.scene.getTexturesCount());
		//Debug::log("CubeMap %d\n", _attributes.scene.getLights().size(), _attributes.scene.getTexturesCount(), _attributes.scene.getMaterialsCount());
	}

	return r;
}

void RaytracerApp::preExecute() 
{
	if (valid) {
		Debug::log("--- Starting Raytracer Render Engine\n");

		Debug::log("Image size: %d x %d (%d pixels)\n", _attributes.projection.getWidth(), _attributes.projection.getHeight(), _attributes.projection.getWidth() * _attributes.projection.getHeight());
		std::stringstream str;
		str << "Trace method: " << _attributes.options.getTraceMethodName() << "\n";
		Debug::log_string(str.str());
		Debug::log("Render method: %s\n", _attributes.options.getRenderMethodName().c_str());
		Debug::log("Max depth: %d | Samples: %d\n", _attributes.options.maxDepth, _attributes.options.antiAliasSamples);
		Debug::log("Threads: %d\n", _attributes.options.threads);
		

		//Debug::log("Program Type: %d \n", _attributes.options.programType);
		Debug::log("--- Pre-Processing: Setup transforms and Render engine\n");

		auto timeStart = std::chrono::high_resolution_clock::now();
		// pre-processing - set transform for objects, camera, projections
		preProcessing();

		if (_attributes.camera.targetObject != -1)
			_attributes.camera.setDirection(_attributes.scene.getPrimitives()[_attributes.camera.targetObject]->getBounds().centroid());

		auto timeEnd = std::chrono::high_resolution_clock::now();
		auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
		Debug::log("Setup done: %.4f (seconds)\n", passedTime / 1000);

		// set to render
		//if (_attributes.options.programType == 0) // the raycast render
		//_render = new RaycastRender();

		// preprocessing render scene
		// & _attributes.scene, & _attributes.camera, & _attributes.options, _attributes.projection
		_render.initialize(&_attributes);
		_render.reset();
		
		//Debug::log("Rendering... \n");
	}
}

void RaytracerApp::execute() {

	/*Image load;
	Image * out = NULL;
	if (load.load("test.png")) {
		Filter filter;
		filter.buildKernel(Filter::Kernel::Blur);
		filter.apply(&load, &out);
		//out->save("test.png");
	}*/
	

	//auto timeStart = std::chrono::high_resolution_clock::now();
	clock_t startRe = clock();

	if (_attributes.options.threads > 1)
		_attributes.image = _render.drawMultiThread(_attributes.options.threads, _attributes.options.threadFunc);
	else
		_attributes.image = _render.draw();
	
	clock_t end = clock();
	//auto timeEnd = std::chrono::high_resolution_clock::now();
	//auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();

	_render.end();
	
	double elapsed_secs = double(end - startRe) / CLOCKS_PER_SEC;

	Debug::log("--- Raytracer Stats:\n");
	Debug::log("Rendering Done: %.4f (seconds)\n", elapsed_secs);
	Debug::log("Total of primary rays: %d\n",_render.getPrimaryRays());
	Debug::log("Total of shadow rays: %d\n", _render.getRaytracer()->getShadowRays());
	Debug::log("Total of secondary rays: %d\n", _render.getRaytracer()->getSecondaryRays());
	Debug::log("Total of AABB tests: %d\n", _render.getRaytracer()->getAABBTests());
	Debug::log("Total of primitives intersection tests: %d\n", _render.getRaytracer()->getIntersectionTests());
	Debug::log("Total of triangle tests: %d\n", _render.getRaytracer()->getTriangleTests());
	
	postExecute();
	//Debug::log("Rendering Done\n");

	

}

void RaytracerApp::clear() {

	_attributes.scene.clear();
	_render.clear();
	
	_attributes.camera.reset();
	_attributes.options.reset();
	_attributes.projection.reset();
	
	//delete _attributes.image;
	//delete _render;
	//_attributes.image = nullptr;
	

}

void RaytracerApp::passConfigTable(std::vector<ConfigParamBase *> & params)
{
	int width = static_cast<const ConfigParam<int>*>(params[1])->value;
	int height = static_cast<const ConfigParam<int>*>(params[2])->value;

	_attributes.projection.setViewSize(width, height);

	_attributes.options.passConfigTable(params);
}

// setup projection, camera and scene transforms
void RaytracerApp::preProcessing() {

	
	_attributes.projection.updateProjection();
	_attributes.camera.setPerspective(_attributes.projection.getFOV());
	_attributes.camera.updateTransform();

	_attributes.scene.updateTransforms();

	// update transform by view matrix
	//_attributes.scene.updateTransforms(_attributes.camera.getMatrix());

	
}

void RaytracerApp::postExecute() {

	if (!_attributes.filters._filters.empty()) 
	{
		Debug::log("Post-Processing: Applying Filters and Colors Effects\n");

		std::queue<FilterStack::ColorEffect *> & filters = _attributes.filters._filters;

		while (!filters.empty()) {

			FilterStack::ColorEffect * effect = filters.front();

			if (effect->isFilter) {
				effect->filter->apply(_attributes.image, _attributes.image);
			}
			else {
				if (effect->type == "grayscale") {
					ImageColors::grayscale(_attributes.image, (ImageColors::GrayscaleType)(int)effect->a);
				}
				else if (effect->type == "posterize") {
					ImageColors::posterize(_attributes.image, (int)effect->a);
				}
				else if (effect->type == "cb") {
					ImageColors::contrastBrightnes(_attributes.image, effect->a, effect->b);
				}
			}

			delete effect;
			filters.pop();

		}


	}

	Debug::log("--- Saving output file:\n");

	auto timeStart = std::chrono::high_resolution_clock::now();
	_attributes.image->save(_attributes.options.outputfileName);
	auto timeEnd = std::chrono::high_resolution_clock::now();
	auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();

	Debug::log("Save Time: %.4f (seconds)\n", passedTime / 1000);

	Debug::log("------------------------------------------\n");

	//auto appTimeEnd = std::chrono::high_resolution_clock::now();
	//auto appPassedTime = std::chrono::duration<double, std::milli>(appTimeEnd - _appStartTime).count();

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	Debug::log("Done! Total Time: %.4f (seconds)\n", elapsed_secs);

	

	

	
}

Image * RaytracerApp::getOutput() const {

	return _attributes.image;

}

Image * RaytracerApp::getDepthMap() const
{
	return _render.getDepthImage();
}

Image * RaytracerApp::getNormalMap() const
{
	return _render.getNormalImage();

}

Image * RaytracerApp::getEdges() const
{
	return _render.getEdgesImage();
}