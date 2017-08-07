

#ifndef _APPLISTENER_H_
#define _APPLISTENER_H_

#include "manager\raytracerapp.h"
#include "ogl\Ilistener.h"
#include "ogl\glutconsole.h"
#include "manager\ConfigParam.h"

#include <thread>



class Job
{

};

class AppListener : public IListener 
{

private:

	enum State {
		SelectFile,
		LoadFile,
		PreProcessing,
		Proccessing,
		PostProcesing,
		CheckStats,
		Config,
		ShowOutput
	};

	// mains
	State _state = SelectFile;
	int _subState = 0;
	std::string _file;
	int _rendering = -1;

	// config vars
	int _configSelect = 0;
	int __configSelectEdit = -1;
	bool _configGetInput = false;
	bool _carret = false;
	float _carretBlinkTime = 0;
	std::vector<ConfigParamBase *> _configTable;
	int _configTableSize = 0;
	bool pushMatrix = false;

	// window and drawing vars
	vec2 _scaleFactor = vec2(1, 1);
	vec2 _windowSize = (640, 600);
	bool _flushed = false;
	int _config_y = 112;
	std::string filterTitle;

	// input stream
	std::stringstream stream;
	int _maxStream = 64;

	// images
	int _view = 0;
	Image * _output = NULL;
	Image * _depth = NULL;
	Image * _normal = NULL;
	Image * _edges = NULL;
	Image * _previewBackup = NULL;
	Image * _preview = NULL;

	// filter vars
	char filterKind = '_';
	int filterMaxLevel = 10, filterMaxLevel2 = 1;
	int filterLevel = 0, filterLevel2 = 0;
	int filterMinLevel = 0, filterMinLevel2 = 0;
	unsigned int previewSize[2];
	std::string grayStr[7];

	// input events
	float _doubleClickTime = 0;
	int _click = 0;
	bool _doubleClick = false;
	bool _keyLeft = false;
	bool _keyRight = false;
	bool _keyPressedLeft = false;
	bool _keyPressedRight = false;
	int _keyLeftLock = 0;
	int _keyRightLock = 0;
	float _keyPressingTime[2];
	float _keyPressingTimeAccumulator = 0;
	float _keyPressingTimeTotal = 0;
	bool _fastChangingKeys = false;


	// apps
	GlutConsole console;
	RaytracerApp raytracerApplication;

	// app listener functions
	void reset();
	void setState(State state);

	// config file
	void saveConfig();
	bool loadConfig();

	// config params
	void setValueConfigTable(std::string value, int index, int type);
	void setValueConfig(ConfigParamBase * param, std::string value);
	ConfigParamBase * findConfig(std::string name);

	// input and config managers
	void manageConfig();
	void manageCombosConfig();
	bool getUserInput(const unsigned char & key, const int & rules = 0);
	void configSetInputValue(bool close = true);

	// manager input keys
	void updateKeys(const float & dt);
	void endUpdateKeys();
	void manageFilterKeys(const unsigned char & key);
	void manageFastKeys(const unsigned char & key);
	void manageColorsKeys(const unsigned char & key);
	void manageApplyFilterKeys(const unsigned char & key);
	void manageApplyColorsKeys(const unsigned char & key);

	// draw stuff
	void printHeaders();
	void printConfigHeaders();
	void printFastKeys();
	void printFilters();
	void printColors();
	void printApplyColors();
	void printConfig();
	void printApplyFilter();

	// leveler stuff
	vec2 levelerPos = vec2(18, 20);
	vec2 levelerSize = vec2(300, 16);
	std::string levelerName[2];
	void drawLeveler(const vec2 & pos, const vec2 & size, const int & value, const int & maxValue, const int & minLevel, const std::string & levelerName = "Level:", const Color & color = Color::white);
	int updateLeveler(const float & deltaTime, int & level, const int & maxLevel, const int & minLevel);
	bool showSecondLeveler = false;

	// applying filter functions
	void updateApplyFilter(const float & dt);
	void applyFilterToPreview();
	Filter * currentFilter = nullptr;
	Filter * getFilterToApply(char kind, int level);
	std::string getFilterName(const char & kind);
	void filterThread(Filter * f);

	// applying color tools
	void updateColorFilter(const float & dt);

	// thread vars
	std::mutex mutex;
	std::thread * threadApplyingFilter = nullptr;
	
	

public:

	~AppListener();

	virtual void start();

	virtual void beginDraw();
	virtual void draw();
	virtual void endDraw();
	virtual void reshape(int width, int height, int previous_width, int previous_height);

	// event - keyboard
	virtual void keyboard(const unsigned char & key, const int & x, const int & y);
	virtual void specialKeyboard(int key, int x, int y);
	virtual void specialKeyboardUp(int key, int x, int y);

	// event - mouse
	virtual void mouse(const int & button, int state, int x, int y);
	virtual void mouseMove(int x, int y);

	// update
	virtual void update(const float & deltatime);

	
};

#endif