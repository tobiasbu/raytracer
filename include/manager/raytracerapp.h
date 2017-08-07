


#include "raytracer\raycastrender.h"
#include "manager\input.h"
#include "manager\appattributes.h"
#include <chrono>

class RaytracerApp {

private:
	
	//std::chrono::system_clock::time_point  _appStartTime;
	clock_t begin;
	RaycastRender _render;
	Input _input;
	AppAttributes _attributes;
	
	void preProcessing();
	void postProcessing();
	bool valid = false;

public:
	RaytracerApp();
	~RaytracerApp();
	void execute();
	void passConfigTable(std::vector<ConfigParamBase *> & params);
	int loadScene(const std::string & filename);
	void preExecute();
	void postExecute();
	void clear();
	Image * getOutput() const;
	Image * getDepthMap() const;
	Image * getNormalMap() const;
	Image * getEdges() const;
	AppAttributes * getAtr() {
		return &_attributes;
	}
	
};