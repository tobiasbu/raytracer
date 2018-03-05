
#include "manager\applistener.h"
#include "math\mat4.h"
#include "math\matrixtransform.h"
#include "others\log.h"
#include "math\maths.h"
#include "image\filter.h"
#include "utils\stringutils.h"
#include "image\imagecolors.h"

#include "ogl\glmanager.h"

#include <errno.h>
#include <fstream>

#define RAYTRACER_VERSION_MAJOR 1
#define RAYTRACER_VERSION_MINOR 2
#define RAYTRACER_VERSION_BUILD 1


std::string getAppVersion()
{
	std::stringstream ss;
	ss << RAYTRACER_VERSION_MAJOR << "." << RAYTRACER_VERSION_MINOR << "." << RAYTRACER_VERSION_BUILD;
	return ss.str();
}

AppListener::~AppListener()
{
	raytracerApplication.clear();
	_output = NULL;
	console.clear();


	if (threadApplyingFilter != nullptr)
	{
		delete threadApplyingFilter;
	}

	if (_previewBackup != NULL || _previewBackup != nullptr)
		delete _previewBackup;

	if (_preview != NULL || _preview != nullptr)
		delete _preview;

	_previewBackup = nullptr;
	_preview = nullptr;
}

void AppListener::start() 
{
	
	_configTable.push_back(new ConfigParam<bool>("overload config: ",true));
	_configTable.push_back(new ConfigParam<int>("width: ",800,64,8020));
	_configTable.push_back(new ConfigParam<int>("height: ",600,64,8020));

	_configTable.push_back(new ConfigParam<int>("max depth: ",2,0,16));
	_configTable.push_back(new ConfigParam<int>("anti-aliasing: ", 0, 0, 32));

	_configTable.push_back(new ConfigParam<const char *>("shading: ", "Phong\0Cel-Shading\0Gooch\0Gooch-Two-Tones", 3, 0));
	_configTable.push_back(new ConfigParam<bool>("cel/gooch outline: ", true));
	_configTable.push_back(new ConfigParam<int>("cel divisions: ", 6, 0, 32));
	_configTable.push_back(new ConfigParam<int>("gooch alpha: ", 25, 0, 100));
	_configTable.push_back(new ConfigParam<int>("gooch beta: ", 65, 0, 100));

	_configTable.push_back(new ConfigParam<const char *>("shadows: ", "No Shadows\0Hard Shadows\0Simple Soft Shadows\0Soft Shadows",3,1));
	_configTable.push_back(new ConfigParam<int>("shadows intensity: ", 50, 0, 100));
	_configTable.push_back(new ConfigParam<int>("shadows samples: ", 3, 1, 32));
	_configTable.push_back(new ConfigParam<int>("penumbra type: ", 0, 0, 2));	

	_configTable.push_back(new ConfigParam<const char *>("edges: ", "None\0Full\0Only Outline", 2, 0));

	_configTable.push_back(new ConfigParam<const char *>("trace method: ", "List\0AABB\0BVH\0SAH-BVH", 3, 2));
	_configTable.push_back(new ConfigParam<int>("threads: ", 0, 0, 8));
	_configTable.push_back(new ConfigParam<const char *>("thread func: ", "Screen Sub Division\0Per line Thread", 1));

	_configTableSize = _configTable.size();

	if (!loadConfig())
		saveConfig();

	//console.setLogCallback(log_string);
	Debug::_glConsole = &console;
	printHeaders();
	stream.str("");
	_keyPressingTime[0] = 0;
	_keyPressingTime[1] = 0;
	_fastChangingKeys = false;
	_keyPressingTimeAccumulator = 0;
	_keyPressingTimeTotal = 0;

	grayStr[0] = "[1] Average";
	grayStr[1] = "[2] Luminance";
	grayStr[2] = "[3] Desaturation";
	grayStr[3] = "[4] Luma";
	grayStr[4] = "[5] Max Decomposition";
	grayStr[5] = "[6] Min Decomposition";
	grayStr[6] = "[7] HSP";
}


void AppListener::update(const float & deltatime) {

	updateKeys(deltatime);

	if (!_flushed) {
		console.update(deltatime);

		if (_configGetInput)
		{
			_carretBlinkTime += deltatime / 0.65f;

			if (_carretBlinkTime >= 1) {
				_carret = !_carret;
				_carretBlinkTime = 0;
			}
		}

	}

	if (_state == State::Config)
	{
		manageCombosConfig();
	}
	else if (_state == State::PreProcessing && _rendering == 0)
	{
		if (static_cast<ConfigParam<bool>*>(_configTable[0])->value)
			raytracerApplication.passConfigTable(_configTable);
		
		raytracerApplication.preExecute();
		_rendering = 1;
	}
	else if (_rendering == 1)
	{
		setState(State::Proccessing);
		_rendering = 2;
	}
	else if (_state == State::ShowOutput)
	{
		if (_subState == 4)
		{
			updateApplyFilter(deltatime);
		}
		else if (_subState == 3)
		{
			updateColorFilter(deltatime);
		}
	}
	
	endUpdateKeys();

}

void AppListener::updateKeys(const float & dt)
{
	if (_click >= 1) {

		_doubleClickTime += dt;

		_doubleClick = true;

		if (_doubleClickTime >= 0.0014)
			_doubleClick = false;
		if (_click > 2)
			_doubleClick = false;

		//if (_doubleClickTime > 0.01)
		//{
		//_doubleClick = false;
		//}
		//else
		//	_doubleClick = true;
	}

	if (_keyLeft)
		_keyPressingTime[0] += dt;
	if (_keyRight)
		_keyPressingTime[1] += dt;
}

void AppListener::endUpdateKeys()
{
	if (_click > 0)
	{
		_click = 0;
		_doubleClick = false;
		_doubleClickTime = 0;
	}

	if (_keyPressedLeft && _keyLeftLock >= 1)
	{
		_keyPressedLeft = false;
	}

	if (_keyPressedRight && _keyRightLock >= 1)
	{
		_keyPressedRight = false;
	}
}

void AppListener::beginDraw() {

	//glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 1);

	if (_state == State::ShowOutput)
	{
		Image * img;

		


		if (_view == 0) {
			img = _output;
		}
		else if (_view == 1)
		{
			img = _depth;
			//glDrawPixels(_depth->getWidth(), _depth->getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, _depth->getPixels());
		}
		else if (_view == 2)
		{
			img = _normal;
			//glDrawPixels(_normal->getWidth(), _normal->getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, _normal->getPixels());
		}
		else if (_view == 3)
		{
			img = _edges;
			//glDrawPixels(_edges->getWidth(), _edges->getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, _edges->getPixels());
		} else if (_view == 4)
		{
			img = _previewBackup;
			//glDrawPixels(_edges->getWidth(), _edges->getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, _edges->getPixels());
		}

		int w = img->getWidth() * _scaleFactor.x;
		int h = img->getHeight() * _scaleFactor.y;
		float x;
		float y;


		if (_view == 4)
		{
			float x2;
			float y2;

			if ((_windowSize.y - 128) < h)
				y = 0;
			else {
				y = ((_windowSize.y - 128) - h) / 2;
			}

			if (_windowSize.x < w + w + 46) {
				x = 0;
				x2 = w;
			}
			else {
				x = (_windowSize.x / 2) - w - (_windowSize.x*0.025); /////// 2; //- (w * 2) + (_windowSize.x*0.05);// -w; // / 2;
				x2 = (_windowSize.x / 2) - w + w - (_windowSize.x*0.025) + (_windowSize.x*0.05); // / 2;// + (w * 2) + (_windowSize.x*0.05); // +w; // 2;
			}

			y2 = y;
			

			w = img->getWidth();
			h = img->getHeight();

			glRasterPos2f(x, y);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glPixelZoom(_scaleFactor.x, -_scaleFactor.y);

			glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, img->getPixelsPtr());

			//glFlush();

			glRasterPos2f(x2, y2);

			glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, _preview->getPixelsPtr());

			glFlush();

		}
		else
		{
			if ((_windowSize.y - 128) < h)
				y = 0;
			else
				y = ((_windowSize.y - 128) - h) / 2;

			if (_windowSize.x < w)
				x = 0;
			else
				x = (_windowSize.x - w) / 2;

			w = img->getWidth();
			h = img->getHeight();

			glDisable(GL_DEPTH_TEST);
			glRasterPos2f(x, y);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glPixelZoom(_scaleFactor.x, -_scaleFactor.y);

			glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, img->getPixelsPtr());
			//glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, img->getPixels());
			//glDrawPixels(x, y, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			//glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
			glFlush();
		}

		

		if (!_flushed) {

			/*s
			int ww = glutGet(GLUT_WINDOW_WIDTH);
			int wh = glutGet(GLUT_WINDOW_HEIGHT);

			

				int w = _output->getWidth() + 64;
				int h = _output->getHeight() + 128 + 64;

				if (w < 540)
					w = 540;
				
				if (h < 480)
					h = 480;
				

				if (wh > h)
					h = wh;
				else if (wh < 480)
					h = 480;

				if (ww > w)
					w = ww;
				else if (ww < 540)
					w = 540;

			
				glutReshapeWindow(w, h);

				ww = glutGet(GLUT_SCREEN_WIDTH); // returns Screen width
				wh = glutGet(GLUT_SCREEN_HEIGHT); // returns Screen height
				//int screen_pos_x = glutGet((GLenum)GLUT_WINDOW_X);
				//int screen_pos_y = glutGet((GLenum)GLUT_WINDOW_Y);
				glutPositionWindow((ww - w) / 2, (wh - h) / 2);
				*/
				

				_flushed = true;
		}
		

		//glReadPixels(0,0,)
		//glBitmap(img->getWidth(), img->getHeight(), 0, 0, 0, 0, img->getPixels());
	}


}

void AppListener::draw() {


	if (!_flushed) {
		console.draw();

		if (_state == Config)
			printConfig();
	}
	else {
		glColor4f(0.08, 0.09, 0.12, 0.25);
		glBegin(GL_QUADS);
		glVertex2f(0, _windowSize.y);
		glVertex2f(0, _windowSize.y - 128);
		glVertex2f(_windowSize.x, _windowSize.y - 128);
		glVertex2f(_windowSize.x, _windowSize.y);
		glEnd();


		glColor3f(1, 1, 1);

		switch (_subState) {
		case 0: printFastKeys(); break;
		case 1: printFilters(); break;
		case 2: printColors(); break;
		case 3: printApplyColors(); break;
		case 4: printApplyFilter(); break;
		}
	}



}

void AppListener::endDraw() {}

void AppListener::reshape(int width, int height, int previous_width, int previous_height)
{
	
	glViewport(0, 0, width, height);

	_windowSize.x = width;
	_windowSize.y = height;
	//mat4 mat = MatrixTransform::ortho(0, width, height, 0, 0, 0);

	/*
		//float aspectratio = Math::min((float)width / (float)_output->getWidth(), (float)height / (float)_output->getHeight());
		float aspectratio = (float)width / (float)height;
		float imageAspect = (float)_output->getHeight() / (float)_output->getWidth();


		//_scaleFactor.y =  aspectratio *  ((float)_output->getHeight() / (float)_output->getWidth());

		if (height > width) {
			_scaleFactor.x = (float)width / (float)_output->getWidth();
			_scaleFactor.y = 1;
		}
		else {
			//_scaleFactor.y = (float)height / (float)_output->getHeight();
			//_scaleFactor.x = _scaleFactor.y * ((float)_output->getWidth() / (float)_output->getHeight());
			_scaleFactor.y = (float)width / (float)_output->getWidth();
			_scaleFactor.x = _scaleFactor.y  * imageAspect;
		}

		/*if (aspectratio < imageAspect) {

			_scaleFactor.y = (float)_output->getWidth() / (float)_output->getHeight();
			_scaleFactor.x = _scaleFactor.y * ((float)width / (float)height);
		

		}
		else {

			_scaleFactor.x = (float)_output->getWidth() / (float)_output->getHeight();
			_scaleFactor.y = _scaleFactor.x * ((float)height / (float)width);
		}

		//_scaleFactor.x = (float)width / (float)_output->getWidth();
		//_scaleFactor.y = (float)height / (float)_output->getHeight();
	}*/
	
	// reset modelview matrix
	glLoadIdentity();                   
	// set to 2D orthogonal projection
	glMatrixMode(GL_PROJECTION);        // switch to projection matrix
	glPushMatrix();                     // save current projection matrix
	glLoadIdentity();                   // reset projection matrix
	//glLoadMatrixf(&mat.m[0]);
	gluOrtho2D(0.0, width, height, 0.0);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	

}



void  AppListener::printHeaders() {

	console.appendLine("** Simple Raytracer " + getAppVersion() + " **\n");
	//Debug::log("Starting Ray Programm\n");
	"* Submit 'config' to overload Raytracer Configuration\n" >> console;
	"* Type the scene file name.\n" >> console;
	"* Examples:\n" >> console;
	"* Submit 'test#' for render examples scene. (# is 0 to 4) \n" >> console;
	//"* Submit 'raytracer' or 'default' for render the Raytracer\n example scene.\n" >> console;
	//3 >> console;
}

void AppListener::printConfigHeaders()
{
	"** Raytracer Configuration **\n" >> console;
	"* Mouse to select a option.\n" >> console;
	"* Keyboard to change the selected option.\n" >> console;
	"***********************************\n" >> console;


}

void AppListener::printConfig()
{
	

	float y = _config_y;
	float x = 18;
	int w = 16;
	std::stringstream str;
	std::string v;

	for (int i = 0; i < _configTableSize; i++)
	{
		ConfigParamBase * param = _configTable[i];

		if (_configSelect == i && __configSelectEdit != i)
		{
			if (_configGetInput)
				glColor4f(0.75, 0.75, 0.25, 1);
			else
				glColor4f(1, 1, 0, 1);

		} else if (__configSelectEdit == i)
		{
			glColor4f(0, 1, 1, 1);
		}
		else 
			glColor4f(1, 1, 1, 1);

		x = 18;

		GlutConsole::drawText(_configTable[i]->name, x, y);

		x += 200;

		v = param->toString();

		/*if (param->type == 1) 
		{
			v = param->toString();
				//static_cast<ConfigParam<bool>*>(param)->toString();
			//	str << "ON";
			//else
			//	str << "OFF";
		}
		else 
		{
			
				//tatic_cast<ConfigParam<int>*>(param)->toString();
			//str << static_cast<ConfigParam<int>*>(param)->value;
		
		}*/
		
		if (param->type == 3) 
		{
			std::stringstream s;
			s << "< " << v << " >";

			GlutConsole::drawText(s.str(), x, y);
		
		}
		else {

			//if (param->type == 2) {
				if (_configGetInput && __configSelectEdit == i)
				{
					GlutConsole::drawText(stream.str(), x, y);
					w = stream.str().size() * 9;

					if (_carret) {
						glRasterPos2f(x + w, y /*+ (16 * countN)*/);
						glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '|');
					}
				} 
				else
				{
					GlutConsole::drawText(v, x, y);
				}
		}

		//str.str("");

		y += 16;
	}

	x = 16;

	if (_configSelect == _configTableSize)
	{
		glColor4f(1, 1, 0, 1);
	}
	else 
	{
		glColor4f(1, 1, 1, 1);
	}

	GlutConsole::drawText("DONE", x, y);

	glColor4f(1, 1, 1, 1);
	
}

void AppListener::printFastKeys() 
{
	float x = 18;
	float yinit = _windowSize.y - 128 + 16;
	float y = yinit;

	GlutConsole::drawText("Fast Keys:", x, y);
	y += 16;
	GlutConsole::drawText("[F] Apply convolution filter", x, y);
	y += 16;
	GlutConsole::drawText("[C] Apply color filter", x, y);
	y += 16;
	GlutConsole::drawText("[S] Save output copy", x, y);
	y += 16;
	GlutConsole::drawText("[A] Save all", x, y);
	y += 16;
	GlutConsole::drawText("[R] Restart Engine", x, y);

	x += 332;
	y = yinit + 16;

	GlutConsole::drawText("[D] View Depth Map", x, y);
	y += 16;
	GlutConsole::drawText("[N] View Normal Map", x, y);
	y += 16;

	if (_edges || _edges != nullptr || _edges != NULL) {
		
		GlutConsole::drawText("[E] View Edges", x, y);
		y += 16;

	}
	GlutConsole::drawText("[O] View Output", x, y);
}

void AppListener::printFilters() {

	float x = 18;
	float yinit = _windowSize.y - 128 + 16;
	float y = _windowSize.y - 128 + 16;

	GlutConsole::drawText("Filters:", 18, y);
	y += 16;
	GlutConsole::drawText("[B] Box Blur", 18, y);
	y += 16;
	GlutConsole::drawText("[O] Smooth Blur", 18, y);
	y += 16;
	GlutConsole::drawText("[M] Motion Blur", 18, y);
	y += 16;
	GlutConsole::drawText("[G] Gaussian Blur", 18, y);

	y += 32;
	GlutConsole::drawText("[C] Cancel", 18, y);

	x += 256;
	y = yinit + 16;
	GlutConsole::drawText("[S] Sharpen", x, y);
	y += 16;
	GlutConsole::drawText("[E] Emboss", x, y);
	y += 16;
	GlutConsole::drawText("[L] Laplacian", x, y);
	y += 16;
	GlutConsole::drawText("[U] Unsharp Mask", x, y);

	
}

/*void AppListener::printLeveler() {

	//std::stringstream str;

	//str << "Set level of " << levelerName;

	float y = _windowSize.y - 128 + 16;
	GlutConsole::drawText(levelerTitle, 18, y);
	y += 16;
	GlutConsole::drawText(stream.str(), 18, y);
	y += 48;
	GlutConsole::drawText("Press Enter to Confirm", 18, y);
}*/

void AppListener::printColors() {

	float y = _windowSize.y - 128 + 16;

	GlutConsole::drawText("Colors Tools:", 18, y);
	y += 16;
	GlutConsole::drawText("[B] Contrast & Brightness", 18, y);
	y += 16;
	GlutConsole::drawText("[P] Posterize", 18, y);
	y += 16;
	GlutConsole::drawText("[G] Grayscale", 18, y);
	y += 16;
	GlutConsole::drawText("[I] Invert", 18, y);


	//y += 48;
	y += 32;
	GlutConsole::drawText("[C] Back", 18, y);

	y = _windowSize.y - 128 + 32;
	GlutConsole::drawText("[T] Threshold", 18+332, y);

}

void AppListener::printApplyFilter()
{
	
	float y = _windowSize.y - 128 + 16;
	GlutConsole::drawText(filterTitle, 18, y);
	y += 20;

	//std::stringstream str;
	//str << "Level:" << filterLevel;
	//GlutConsole::drawText(str.str(), 18, y);
	//y += 28;
	
	levelerPos.y = y;
	drawLeveler(levelerPos, levelerSize, filterLevel, filterMaxLevel,0);

	
	y += 24 + 4;

	GlutConsole::drawText("Set level by Arrow keys and Hold to changes fast.", 18, y);

	y += 32;
	GlutConsole::drawText("[A] Apply filter", 18, y);
	y += 16;
	GlutConsole::drawText("[C] Cancel", 18, y);



	/*glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x, y - 16);
	glVertex2f(x + 300, y - 16);
	glVertex2f(x + 300, y);
	glEnd();

	glColor4f(1, 1, 1, 1);

	if (factor > 0) {

		

		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x, y - 16);
		glVertex2f(x + (300 * factor), y - 16);
		glVertex2f(x + (300 * factor), y);
		glEnd();

	}*/

	
}

void AppListener::printApplyColors()
{
	
	float y = _windowSize.y - 128 + 16;
	GlutConsole::drawText(filterTitle, 18, y);
	
	
	if (filterKind != 'g' && filterKind != 'i') 
	{
		y += 20;

		Color col = Color::white;

		if (_configSelect == 1)
			col = Color::yellow;

		levelerPos.y = y;
		drawLeveler(levelerPos, levelerSize, filterLevel, filterMaxLevel, filterMinLevel, levelerName[0], col);
		y += 20;


		if (showSecondLeveler) {

			if (_configSelect == 2)
				col = Color::yellow;
			else
				col = Color::white;

			levelerPos.y = y;
			drawLeveler(levelerPos, levelerSize, filterLevel2, filterMaxLevel2, filterMinLevel2, levelerName[1], col);
			y += 20;
		}
		else 
		{
			y += 8;
		}


		GlutConsole::drawText("Set level by Arrow keys and Hold to changes fast.", 18, y);
	}
	else if (filterKind == 'g')
	{
		y += 16;

		float x = 18;
		Color col = Color::white;

		for (int i = 0; i < 7; i++)
		{
			if ((i+1) == _configSelect)
				col = Color::yellow;
			else
				col = Color::white;

			glColor3fv(col.ptr());

			GlutConsole::drawText(grayStr[i], x, y);

			if ((i + 1) % 3 == 0)
			{
				x += 256;
				y = _windowSize.y - 128 + 32;
			}
			else {
				y += 16;
			}
		}

		glColor3fv(Color::white.ptr());
	}

	

	y = _windowSize.y - 128 + 96;

	GlutConsole::drawText("[A] Apply color tool", 18, y);
	y += 16;
	GlutConsole::drawText("[C] Cancel", 18, y);
}



void AppListener::drawLeveler(const vec2 & pos, const vec2 & size, const int & value, const int & maxValue, const int & minLevel, const std::string & levelerName, const Color & color)
{
	float x = pos.x;
	
	float hs = size.y / 2.f;
	float yl = pos.y-14.f; // (pos.y - hs) + ((15 - size.y) / 2);
	
	size_t l = levelerName.length();

	glColor3fv(color.ptr());

	if (l > 0) 
	{

		GlutConsole::drawText(levelerName, x, pos.y);
		x += 164;//(l * 10) + 16;
	}

	glColor4f(0, 0, 0, 0.25f);
	

	glBegin(GL_QUADS);
	glVertex2f(x, yl);
	glVertex2f(x, yl + size.y);
	glVertex2f(x + size.x, yl + size.y);
	glVertex2f(x + size.x, yl);
	glEnd();

	//glColor4f(1, 1, 1, 1);
	glColor3fv(color.ptr());

	if (value >= minLevel && value <= maxValue) {

		float factor = (float)(value - minLevel) / (float)(maxValue - minLevel);

		//std::cout << factor << std::endl;

		glBegin(GL_QUADS);
		glVertex2f(x, yl);
		glVertex2f(x, yl + size.y);
		glVertex2f(x + (size.x * factor), yl + size.y);
		glVertex2f(x + (size.x * factor), yl);
		glEnd();

	}

	GlutConsole::drawText(StringUtils::convert(value), x + size.x + 16, pos.y);

	glColor4f(1, 1, 1, 1);
}

int AppListener::updateLeveler(const float & deltaTime, int & level, const int & maxLevel, const int & minLevel)
{

	// this function returns the state of leveler update

	// check if user has inputed key
	int k = (_keyPressedRight && _keyRightLock == 1) - (_keyPressedLeft && _keyLeftLock == 1);
	bool fastChange = false;
	int changeX = 1;

	if (k == 0)
	{
		// check if user is holding a key
		k = (_keyPressingTime[0] > 0.5f && _keyLeft) ? -1 : (_keyPressingTime[1] > 0.5f && _keyRight) ? 1 : 0;

		fastChange = (k != 0) ? true : false;

		if (fastChange)
		{
			// accumulate time pressing any key
			_keyPressingTimeTotal += deltaTime;

			if (_keyPressingTimeTotal < 0.35f)
				_keyPressingTimeAccumulator += deltaTime / 0.05f;
			else if (_keyPressingTimeTotal >= 0.35f && _keyPressingTimeTotal < 0.75f)
				_keyPressingTimeAccumulator += deltaTime / 0.015f;
			else if (_keyPressingTimeTotal >= 0.75f && _keyPressingTimeTotal < 1.2f) {
				_keyPressingTimeAccumulator += 2;
				changeX = 2;
			}
			else if (_keyPressingTimeTotal >= 1.2f) {
				_keyPressingTimeAccumulator += 4;
				changeX = 4;
			}
			


			if (_keyPressingTimeAccumulator >= 1.f)
				_keyPressingTimeAccumulator = 0;
		}

	}

	if (k != 0)
	{
		int oldLevel = level ;

		// avoid super fast key input when the user is holding
		if (_keyPressingTimeAccumulator <= 0)
		{
			level += k * changeX;
			if (level > maxLevel)
				level = maxLevel;
			else if (level < minLevel)
				level = minLevel;
		}

		// change the filter level
		if (level != oldLevel)
		{
			if (!fastChange)
			{
				_keyPressingTimeAccumulator = 0;
				_keyPressingTimeTotal = 0;
				return 1; //applyFilterToPreview(); just one time pressed
			}
			else
			{
				_fastChangingKeys = true;

				return 2; // holding the key


				/*if (threadApplyingFilter != nullptr)
				{
				if (threadApplyingFilter->joinable())
				threadApplyingFilter->join();

				delete threadApplyingFilter;

				if (currentFilter != nullptr)
				delete currentFilter;

				currentFilter = nullptr;
				threadApplyingFilter = nullptr;

				}*/
			}
		}
	}
	else
	{
		if (_fastChangingKeys)
		{
			_fastChangingKeys = false;
			_keyPressingTimeAccumulator = 0;
			_keyPressingTimeTotal = 0;
			//applyFilterToPreview();

			return 3; // the key was holding but the user has released the key


		}
	}

	// has no change
	return 0;

	/*
	
	// check if user has inputed key
	int k = (_keyPressedRight && _keyRightLock == 1) - (_keyPressedLeft && _keyLeftLock == 1);
	bool fastChange = false;


	if (k == 0)
	{
	// check if user is holding a key
	k = (_keyPressingTime[0] > 0.5f && _keyLeft) ? -1 : (_keyPressingTime[1] > 0.5f && _keyRight) ? 1 : 0;

	fastChange = (k != 0) ? true : false;

	if (fastChange) {
	// accumulate time pressing any key
	_keyPressingTimeAccumulator += deltatime / 0.05f;

	if (_keyPressingTimeAccumulator >= 1.f)
	_keyPressingTimeAccumulator = 0;
	}

	}

	if (k != 0)
	{
	int oldLevel = filterLevel;

	// avoid super fast key input when the user is holding
	if (_keyPressingTimeAccumulator <= 0)
	{
	filterLevel += k;
	if (filterLevel > filterMaxLevel)
	filterLevel = filterMaxLevel;
	else if (filterLevel < 0)
	filterLevel = 0;
	}

	// change the filter level
	if (filterLevel != oldLevel)
	{
	if (!fastChange)
	{
	applyFilterToPreview();
	_keyPressingTimeAccumulator = 0;
	}
	else
	{
	_fastChangingKeys = true;


	if (threadApplyingFilter != nullptr)
	{
	if (threadApplyingFilter->joinable())
	threadApplyingFilter->join();

	delete threadApplyingFilter;

	if (currentFilter != nullptr)
	delete currentFilter;

	currentFilter = nullptr;
	threadApplyingFilter = nullptr;

	}
	}
	}
	}
	else
	{
	if (_fastChangingKeys)
	{
	applyFilterToPreview();
	_fastChangingKeys = false;
	_keyPressingTimeAccumulator = 0;
	}
	}
	
	*/
}


void AppListener::mouse(const int & button, int state, int x, int y)
{
	/*std::stringstream s;
	
	s.str() >> console;*/

	if (button == 0 && state == 0)
	{
		_click++;

		if (_state == Config)
		{
			int newEdit = -1;

			if (y >= (_config_y-16) && x >= 18 && x <= 400 && y < (_config_y-16) + (16 * (_configTableSize + 1)))
			{
				newEdit = (y - (_config_y-16)) / 16;

			}

				//if (newEdit != -1) {

				int old = __configSelectEdit;

				if (__configSelectEdit != -1)
				{
					configSetInputValue();
				}

				if (newEdit == old)
				{
					__configSelectEdit = -1;
				}
				else if (newEdit != -1 && newEdit != _configTableSize)
				{

					__configSelectEdit = newEdit;
					manageConfig();
				}
				else
				{
					if (newEdit == _configTableSize)
					{
						console.clear();
						console.setUserInput(true);
						_maxStream = 64;
						printHeaders();
						stream.clear();
						stream.str("");
						setState(SelectFile);
						saveConfig();
					}

					__configSelectEdit = -1;
				}
				//}
			

			
		}
	}
	



	//std::cout << _click << std::endl;

	//_doubleClickTime = 0;

	

}

void AppListener::mouseMove(int x, int y) {

	/*std::stringstream s;

	s << " " << " " << x << " " << y << "\n";

	s.str() >> console;*/
	//std::cout << x << " " << y << std::endl;

	if (_state == Config)
	{
		if (y >= (_config_y-16) && x >= 18 && x <= 400 && y < (_config_y-16) + (16 * (_configTableSize + 1)))
		{
			_configSelect = (y - (_config_y-16)) / 16;

		}
	}
}

void AppListener::keyboard(const unsigned char & key, const int & x, const int & y)
{
	bool update = true;

	if (_state == SelectFile) {

		if (getUserInput(key))
		{
			stream << "\n";
			stream.str("");
			if (console.getUserInput(_file, true))
				setState(LoadFile);
		} else
			console.userInput(stream.str());

	} else if (_state == Config) {

		if (__configSelectEdit != -1)
		{
			if (_configGetInput) 
			{

				if (getUserInput(key, 1))
				{
					configSetInputValue();
					//stream.str("");

				}
				else {
					//configSetInputValue(false);
					_carretBlinkTime = 0;
					_carret = true;
				}
			}
			else
			{
				if (key == 13)
					__configSelectEdit = -1;
			}

		}

		
	}
	else if (_state == ShowOutput)
	{
		switch (_subState) {
		default: break;

		case 0: { 
			manageFastKeys(key);
			break;
		}
		case 1: {
			manageFilterKeys(key); 
			break; 
		}
		case 2: { 
			manageColorsKeys(key);
			break; 
		}
		
		case 3: {
			manageApplyColorsKeys(key);
			break;
		}

		case 4: { 
			manageApplyFilterKeys(key); 
			break; 
		}
		}
			
		
	}

	if (key == 27)
		GLManager::close();
}

void AppListener::specialKeyboard(int key, int x, int y)
{
	/*if (_keyLeftLock == 1)
		_keyPressedLeft = false;
	
	if (_keyPressedRight == 1)
		_keyPressedRight = false;*/

	switch (key)
	{
	case GLUT_KEY_LEFT: { 
		_keyLeft = true; 
		
		_keyLeftLock++;
		if (_keyLeftLock > 2) 
			_keyLeftLock = 2;
		else
			_keyPressedLeft = true;
		break; 
	}
	case GLUT_KEY_RIGHT:{ 
		_keyRight = true; 
		
		_keyRightLock++; 
		if (_keyRightLock > 2)
			_keyRightLock = 2;
		else
			_keyPressedRight = true;
		break;
	}
	default:
		break;
	} 
}

void AppListener::specialKeyboardUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: { _keyLeft = false; _keyPressedLeft = false; _keyLeftLock = 0; _keyPressingTime[0] = 0; break; }
	case GLUT_KEY_RIGHT: { _keyRight = false;  _keyPressedRight = false; _keyRightLock = 0; _keyPressingTime[1] = 0; break; }
	default:
		break;
	}
}

bool AppListener::getUserInput(const unsigned char & key, const int & rules)
{
	if (key == 8) {

		std::string str = stream.str();
		if (str.size() > 0) {
			str.erase(str.begin() + str.size() - 1);


			if (str.size() > 0) {
				stream.str(str);
				stream.seekp(str.size());
			}
			else {
				stream.str("");
			}
		}
	}
	else if (key >= 32) {

		if (stream.str().size() <= _maxStream) {
			if (rules == 1) // only numbers
			{
				if (key >= 48 && key <= 57)
					stream << key;

			} else
				stream << key;
		}
		else
		{
			stream.seekp(_maxStream-1);
		}
			
	}

	if (key == 13) 
	{
		return true;
	}
	else
		return false;
}



void AppListener::manageConfig()
{
	if (_configSelect < _configTableSize)
	{
		ConfigParamBase * param = _configTable[_configSelect];

		if (param->type == 1)
		{
			if (_click) // _doubleClick = false;
			{
				bool value = static_cast<ConfigParam<bool>*>(param)->value;

				static_cast<ConfigParam<bool>*>(param)->value = !value;

				__configSelectEdit = -1;
			}


		}
	    else if (param->type == 2)
		{
			if (_click)
			{
				if (!_configGetInput)
				{
					stream.str("");
					//
					stream << static_cast<ConfigParam<int>*>(param)->value;
					stream.seekp(stream.str().size());
				}
				_configGetInput = true;
			}
		}
	}
}

void AppListener::manageCombosConfig()
{
	if (_configSelect < _configTableSize && __configSelectEdit != -1)
	{

		ConfigParamBase * param = _configTable[__configSelectEdit];

		if (param->type == 3)
		{
			int k = (_keyPressedRight && _keyRightLock == 1) - (_keyPressedLeft && _keyLeftLock == 1);

			if (k != 0)
			{
				int value = static_cast<ConfigParam<const char*>*>(param)->current;
				value += k;
				static_cast<ConfigParam<const char*>*>(param)->set(value);

			}
		}
		else if (param->type == 2)
		{
			int k = _keyRight - _keyLeft;
			if (k != 0)
			{
				
				int min = static_cast<ConfigParam<int>*>(param)->min;
				int max = static_cast<ConfigParam<int>*>(param)->max;
				float factor = ((float)max - (float)min);

				if (factor < 32)
					factor = 0.5;
				else if (factor >= 32 && factor <= 50)
					factor *= 0.05f;
				else if (factor > 50 && factor < 100)
					factor /= 5.f;
				else if (factor >= 100)
					factor /= 100.f;

				if (factor != 0) {

					int value = static_cast<ConfigParam<int>*>(param)->value;
					value += k * ceil(factor);

					value = static_cast<ConfigParam<int>*>(param)->set(value);

					stream.clear();
					stream.str("");
					stream << value;
					stream.seekp(stream.str().size());
				}
			}
		}

	}
}

void AppListener::configSetInputValue(bool close)
{
	ConfigParamBase * param = _configTable[__configSelectEdit];
	
	if (param->type == 2) 
	{

		int value = static_cast<ConfigParam<int>*>(param)->value;
		int newValue;
		stream >> newValue;

		if (newValue != value)
		{
			newValue = static_cast<ConfigParam<int>*>(param)->set(newValue);

			if (!close) {
				stream.clear();
				stream.str("");
				stream << newValue;
				stream.seekp(stream.str().size());
			}
		}

		
	}

	if (close) {
		stream.clear();
		//stream.str("");
		//stream.seekp(0);

		_configGetInput = false;
		__configSelectEdit = -1;
	}

}

void AppListener::manageFastKeys(const unsigned char & key) {

	if (key == 'F' || key == 'f') {
		_subState = 1;
		_view = 0;
	}
	else if (key == 'C' || key == 'c') {
		_subState = 2;
	} 
	else if (key == 'S' || key == 's') {
		time_t now = time(NULL);
		char date[64];
		//errno_t err;
		std::stringstream str;
		std::string ext = StringUtils::fileExtension(raytracerApplication.getAtr()->options.outputfileName);
		std::string fname = StringUtils::fileNameWithoutExtension(raytracerApplication.getAtr()->options.outputfileName);
		str << fname;

		struct tm * t = localtime(&now);

		//if ((err = ctime_s(date, 64, &now)) != 0)
		str << t->tm_hour << "-" << t->tm_min << "-" << t->tm_yday << "-" << t->tm_mon << "-" << t->tm_year << "." << ext;
		//else
		//	str << rand() % 256;


		_output->save(str.str());
	}
	else if (key == 'A' || key == 'a') {
		time_t now = time(NULL);
		char date[64];
		//errno_t err;
		std::stringstream str;
		std::stringstream o;
		std::string ext = StringUtils::fileExtension(raytracerApplication.getAtr()->options.outputfileName);
		std::string fname = StringUtils::fileNameWithoutExtension(raytracerApplication.getAtr()->options.outputfileName);
		str << fname;

		struct tm * t;
		errno_t e = localtime_s(t, &now);

		//if ((err = ctime_s(date, 64, &now)) != 0)
		str << t->tm_hour << "-" << t->tm_min << "-" << t->tm_yday << "-" << t->tm_mon << "-" << t->tm_year << "." << ext;
		o << "output/output_" << str.str();

		_output->save(o.str());

		o.clear();
		o.str("");
		o << "output/depth_" << str.str();
		_depth->save(o.str());

		o.clear();
		o.str("");
		o << "output/normal_" << str.str();
		_depth->save(o.str());

		if (_edges) {
			o.clear();
			o.str("");
			o << "output/normal_" << str.str();
			_edges->save(o.str());
		}
		//else
		//	str << rand() % 256;


		
	}
	else if (key == 'D' || key == 'd') {

		_view = 1;
	} 
	else if (key == 'N' || key == 'n') {

		_view = 2;
	} 
	else if (key == 'E' || key == 'e') {

		if (_edges || _edges != nullptr || _edges != NULL)
		_view = 3;
	} 
	else if (key == 'O' || key == 'o') {

		_view = 0;

	} else if (key == 'R' || key == 'r') {
		reset();
 }

}

void AppListener::manageColorsKeys(const unsigned char & key) {

	char k = key;
	StringUtils::toLower(&k);

	if (k == 'g' || k == 'p' || k == 'b' || k == 't' || k == 'i') {



		if (_preview != nullptr || _preview != NULL)
			delete _preview;

		_preview = _previewBackup->clone();

		filterKind = k;
		filterLevel = 0;
		_keyPressingTimeAccumulator = 0;
		_fastChangingKeys = false;
		showSecondLeveler = false;

		_configSelect = 0;

		levelerName[0] = "Level:";

		switch (k) {
		case 'g': {
			filterTitle = "Grayscale:   (Press keys shown to change the selected grayscale method.)";
			ImageColors::grayscale(_preview, ImageColors::Average);
			_configSelect = 1;
			break;
		}
		case 'p': {
			filterTitle = "Posterize: (reduce number of colors)";
			filterMinLevel = 2;
			filterLevel = 4;
			filterMaxLevel = 255;
			ImageColors::posterize(_preview, 4);
			break;
		}
		case 'b': {
			filterTitle = "Brightness & Contrast:    (Press keys shown to change the selected option.)";
			filterMinLevel = -255;
			filterLevel = 0;
			filterMaxLevel = 255;
			filterMinLevel2 = -255;
			filterLevel2 = 0;
			filterMaxLevel2 = 255;
			_configSelect = 1;
			showSecondLeveler = true;
			levelerName[0] = "[1] Brightness: ";
			levelerName[1] = "[2] Contrast: ";
			break;
		}
		case 'i': {
			filterTitle = "Invert: (invert colors)";
			ImageColors::invert(_preview);
			break;
		}
		case 't': {
			filterTitle = "Threshold: (binary color)";
			filterMinLevel = 0;
			filterLevel = 128;
			filterMaxLevel = 255;
			ImageColors::threshold(_preview, 128);
			break;
		}

				  // TODO colorize
		}

		_subState = 3;
		_view = 4;

	}
	else if (k == 'c')
	{
		filterKind = '_';
		_subState = 0;
	}
	
	

}

void AppListener::manageFilterKeys(const unsigned char & key)
{
	char k = key;
	StringUtils::toLower(&k);

	if (k == 'c')
	{
		filterKind = '_';
		_subState = 0;
	}
	else
	{
		if (k == 's' || k == 'u' || k == 'b' || k == 'g' || k == 'e' || k == 'o' || k == 'm' || k == 'l')
		{
			_subState = 4;
			_view = 4;

			filterKind = k;
			filterLevel = 1;
			filterMinLevel = 0;
			filterMaxLevel = 10;
			_keyPressingTimeAccumulator = 0;
			_fastChangingKeys = false;

			std::stringstream str;
			str << "Apply Filter " << getFilterName(k);
			filterTitle = str.str();

			if (k == 'g')
				filterMaxLevel = 2;
			else if (k == 'e' || k == 'l' || k == 'u')
				filterMaxLevel = 1;

			
			if (_preview != nullptr || _preview != NULL)
				delete _preview;

			
			_preview = _previewBackup->clone();
			
			
			applyFilterToPreview();
			
			
			//_scaleFactor.x = 0.4;
			//_scaleFactor.y = 0.4;
		}
	}

	/*if (key == 'S' || key == 's') {
		//Sharpen s;
		//_output = s.apply(_output);
	}
	else if (key == 'U' || key == 'u') {
		//UnsharpMask s;
		//_output = s.apply(_output);
	}
	else if (key == 'B' || key == 'b') {
		//Blur s;
		//_output = s.apply(_output);
	}
	else if (key == 'G' || key == 'g') {
		//GaussianBlur s;
		//_output = s.apply(_output);
	}
	else if (key == 'E' || key == 'e') {
		Emboss s;
		//_output = s.apply(_output);
	}
	else if (key == 'D' || key == 'd') {
		//EdgeDetection s;
		//_output = s.apply(_output);
	}
	else if (key == 'C' || key == 'c') {
		//_subState = 0;
	}*/
}

void AppListener::manageApplyFilterKeys(const unsigned char & key)
{
	if (key == 'a' || key == 'A' || key == 13)
	{
		if (threadApplyingFilter != nullptr)
		{
			if (threadApplyingFilter->joinable())
				threadApplyingFilter->join();
		}

		if (currentFilter != nullptr)
		{
			currentFilter->apply(_output, _output);
			delete _previewBackup;
			_previewBackup = _output->clone();
			_previewBackup->resize(previewSize[0], previewSize[1]);

			delete currentFilter;
			currentFilter = nullptr;
		}

		_view = 0;
		_subState = 1;

	}
	else  if (key == 'c' || key == 'C')
	{
		_scaleFactor.x = 1;
		_scaleFactor.y = 1;
		_view = 0;
		_subState = 1;
	}
}

void AppListener::manageApplyColorsKeys(const unsigned char & key)
{
	char k = key;
	StringUtils::toLower(&k);

	if (filterKind == 'b')
	{
		if (k == '1')
			_configSelect = 1;
		else if (k == '2')
			_configSelect = 2;
	}
	else if (filterKind == 'g')
	{
		if (k >= 49 && k <= 57) 
		{
			int n = (int)k;
			n -= 48;

			if (n >= 1 && n < 8) {
				int old = _configSelect;
				//n++;

				if (old != n) 
				{
					if (_preview != NULL || _preview != nullptr)
					{
						delete _preview;
						_preview = nullptr;
					}

					_preview = _previewBackup->clone();

					_configSelect = n;

					switch (_configSelect)
					{
					default:
						break;
					case 1: ImageColors::grayscale(_preview, ImageColors::Average); break;
					case 2: ImageColors::grayscale(_preview, ImageColors::Luminance); break;
					case 3: ImageColors::grayscale(_preview, ImageColors::Desaturation); break;
					case 4: ImageColors::grayscale(_preview, ImageColors::Luma); break;
					case 5: ImageColors::grayscale(_preview, ImageColors::MaxDecomposition); break;
					case 6: ImageColors::grayscale(_preview, ImageColors::MinDecomposition); break;
					case 7: ImageColors::grayscale(_preview, ImageColors::HSP); break;
					}
				}
			}
		}
	}

	if (k == 'c') {
		_scaleFactor.x = 1;
		_scaleFactor.y = 1;
		_view = 0;
		_subState = 2;
		filterKind = '_';
	}
	else if (k == 'a' || key == 13)
	{
		switch (filterKind)
		{
		default: break;
		case 'p': ImageColors::posterize(_output, filterLevel); break;
		case 'b': ImageColors::contrastBrightnes(_output, filterLevel2,filterLevel); break;
		case 't': ImageColors::threshold(_output, filterLevel); break;
		case 'i': ImageColors::invert(_output); break;
		case 'g': 
		{
			switch (_configSelect)
			{
			default:
				break;
			case 1: ImageColors::grayscale(_output, ImageColors::Average); break;
			case 2: ImageColors::grayscale(_output, ImageColors::Luminance); break;
			case 3: ImageColors::grayscale(_output, ImageColors::Desaturation); break;
			case 4: ImageColors::grayscale(_output, ImageColors::Luma); break;
			case 5: ImageColors::grayscale(_output, ImageColors::MaxDecomposition); break;
			case 6: ImageColors::grayscale(_output, ImageColors::MinDecomposition); break;
			case 7: ImageColors::grayscale(_output, ImageColors::HSP); break;
			}

			break;
		}
		}

		// TODO check if was really changed
		delete _previewBackup;
		_previewBackup = _output->clone();
		_previewBackup->resize(previewSize[0], previewSize[1]);

		filterKind = '_';
		_view = 0;
		_subState = 2;
	}
}



void AppListener::setState(State state) 
{
	_state = state;

	switch (_state)
	{
	case Config: {
		console.clear();
		printConfigHeaders();
		break;
	}

	case LoadFile: {
		console.clear();

		if (_file == "config")
		{
			console.setUserInput(false);
			_maxStream = 4;
			stream.str("");
			setState(Config);
		}
		else {

			console.clear();

			int r = raytracerApplication.loadScene(_file);

			if (r == 1) {
				setState(PreProcessing);
				_rendering = 0;
			} 
			else if (r <= 0)
			{

				printHeaders();

				if (r == -2)
					Debug::log("Invalid Command\n", _file.c_str());
				else
					Debug::log("File %s don't exists\n", _file.c_str());

				if (console.count() > 6)
					console.erase(5);

				setState(SelectFile);
			}
		}
		break;
	}
	case Proccessing: {

		

		raytracerApplication.execute();
		setState(ShowOutput);
		//setState(CheckStats);
		_output = raytracerApplication.getOutput();
		_depth = raytracerApplication.getDepthMap();
		_normal = raytracerApplication.getNormalMap();
		_edges = raytracerApplication.getEdges();
		_previewBackup = _output->clone();

		int ww = glutGet(GLUT_WINDOW_WIDTH);
		int wh = glutGet(GLUT_WINDOW_HEIGHT);



		int w = _output->getWidth() + 64;
		int h = _output->getHeight() + 128 + 64;

		if (w < 540)
			w = 540;

		if (h < 480)
			h = 480;

		if (wh > h)
			h = wh;
		else if (wh < 480)
			h = 480;

		if (ww > w)
			w = ww;
		else if (ww < 540)
			w = 540;

		_windowSize.x = w;
		_windowSize.y = h;
		glutReshapeWindow(w, h);

		ww = glutGet(GLUT_SCREEN_WIDTH); // returns Screen width
		wh = glutGet(GLUT_SCREEN_HEIGHT); // returns Screen height
		//int screen_pos_x = glutGet((GLenum)GLUT_WINDOW_X);
		//int screen_pos_y = glutGet((GLenum)GLUT_WINDOW_Y);
		glutPositionWindow((ww - w) / 2, (wh - h) / 2);

		unsigned int wi = _output->getWidth();
		unsigned int hi = _output->getHeight();

		// preview size
		unsigned int wp = (w / 2) - (16 * 3);//(_output->getWidth() / 2) - (18 * 3);
		unsigned int hp = (h / 2) - (16 * 3);//(_output->getHeight() / 2) - (18 * 3);

		if (wi < wp || hi < hp)
		{
			previewSize[0] = wi;
			previewSize[1] = hi;
		}
		else if (wp > hp)
		{
			previewSize[0] = wp;
			previewSize[1] = (hp * previewSize[0]) / wp;
		}
		else
		{
			previewSize[1] = hp;
			previewSize[0] = (wp * previewSize[1]) / hp;
		}

		/*if (w > 320) {
			previewSize[0] = 320;
			previewSize[1] = (h * previewSize[0]) / w;
		}
		else if (h > 240)
		{
			previewSize[1] = 240;
			previewSize[0] = (w * previewSize[1]) / h;
		}
		else
		{
			previewSize[0] = w;
			previewSize[1] = h;
		}*/

		_previewBackup->resize(previewSize[0], previewSize[1]);

		break;
	}
	}
}

void AppListener::reset() {

	raytracerApplication.clear();
	_flushed = false;
	_rendering = -1;
	_output = NULL;
	_subState = 0;
	console.clear();
	_file = "";
	stream.str("");
	_view = 0;
	_keyPressingTime[0] = 0;
	_keyPressingTime[1] = 0;
	_keyPressingTimeAccumulator = 0;
	_keyPressingTimeTotal = 0;
	_fastChangingKeys = false;

	if (threadApplyingFilter != nullptr)
	{
		if (threadApplyingFilter->joinable())
			threadApplyingFilter->join();

		delete threadApplyingFilter;
		threadApplyingFilter = nullptr;
	}

	if (currentFilter != nullptr)
		delete currentFilter;

	currentFilter = nullptr;

	if (_previewBackup != NULL ||_previewBackup != nullptr)
		delete _previewBackup;

	if (_preview != NULL || _preview != nullptr)
		delete _preview;

	_previewBackup = nullptr;
	_preview = nullptr;

	

	_state = SelectFile;

	printHeaders();


}

void AppListener::saveConfig()
{
	std::ofstream FILE;
	
	FILE.open("config.txt", std::ios::out);

	if (FILE.is_open())
	{ 
		FILE << "[CONFIG] Simple Raytracer" << std::endl;

		for each (ConfigParamBase * var in _configTable)
		{
			FILE << var->name;

			switch (var->type)
			{
			case 1: 
			{
				FILE << static_cast<ConfigParam<bool>*>(var)->value;
				break;
			}

			case 2:
			{
				FILE << static_cast<ConfigParam<int>*>(var)->value;
				break;
			}

			case 3:
			{
				FILE << static_cast<ConfigParam<const char*>*>(var)->current;
				break;
			}

			

			case 0:
			default:
				break;
			}

			FILE << std::endl;
		}

		FILE.close();
	}


		
}

bool AppListener::loadConfig()
{
	std::ifstream FILE("config.txt");

	if (!FILE.is_open())
		return false;
	else 
	{
		std::string str;

		std::getline(FILE, str);
		std::vector<std::string> split;
		std::string val;
		std::stringstream n;
		int index = 0;
		int type = 0;

		while (std::getline(FILE, str))
		{
			split.clear();
			split = StringUtils::split(str, ':', true);

			if (split.size() > 1)
			{
				val = split[1];

				val.erase(remove_if(val.begin(), val.end(), isspace), val.end());

				n.str("");
				n << split[0] << ": ";

				ConfigParamBase * param = findConfig(n.str());

				if (param != NULL)
				{
					setValueConfig(param, val);
					//setValueConfigTable(val, index, type);
				}
					
				
			}
			else
				continue;

			
		}

		FILE.close();
	}
	

}



void AppListener::setValueConfigTable(std::string value, int index, int type)
{
	switch (type)
	{
	case 1: static_cast<ConfigParam<bool>*>(_configTable[index])->value = StringUtils::parseBool(value); break;
	case 2: static_cast<ConfigParam<int>*>(_configTable[index])->value = StringUtils::parseInt(value); break;
	case 3: static_cast<ConfigParam<const char*>*>(_configTable[index])->set(StringUtils::parseInt(value)); break;

	case 0:
	default:
		break;
	
	}
}

void AppListener::setValueConfig(ConfigParamBase * param, std::string value)
{
	switch (param->type)
	{
	case 1: static_cast<ConfigParam<bool>*>(param)->value = StringUtils::parseBool(value); break;
	case 2: static_cast<ConfigParam<int>*>(param)->set(StringUtils::parseInt(value)); break;
	case 3: static_cast<ConfigParam<const char*>*>(param)->set(StringUtils::parseInt(value)); break;

	case 0:
	default:
		break;

	}
}

ConfigParamBase * AppListener::findConfig(std::string name)
{
	for each (ConfigParamBase * var in _configTable)
	{
		if (var->name == name)
			return var;
		else
			continue;
	}

	return NULL;
}


void AppListener::updateColorFilter(const float & dt)
{
	if (filterKind != 'g' && filterKind != 'i')
	{
		int & f = filterLevel;
		int & f2 = filterLevel2;
		const int & fmax = filterMaxLevel;
		const int & fmin = filterMinLevel;
		int r = 0;

		
		if (_configSelect == 2) // reuse this var
		{
			//f2 = filterLevel2;

			r = updateLeveler(dt, f2, filterMaxLevel2, filterMinLevel2);
		}
		else
		{
			r = updateLeveler(dt, f, fmax, fmin);
		}

		

		switch (r)
		{
		case 3:
		case 1:
		{
			if (_preview != NULL || _preview != nullptr)
			{
				delete _preview;
				_preview = nullptr;
			}

			_preview = _previewBackup->clone();
			
			switch (filterKind) {
			default:
				break;
			case 'p': ImageColors::posterize(_preview, f); break;
			case 'b': ImageColors::contrastBrightnes(_preview, f2, f); break;
			case 't': ImageColors::threshold(_preview, filterLevel); break;
			case 'i': ImageColors::invert(_preview); break;
			}
			break;
			
		}

		default:
			break;
		}
	}
}

void AppListener::updateApplyFilter(const float & dt)
{
	int r = updateLeveler(dt, filterLevel, filterMaxLevel, filterMinLevel);

	switch (r)
	{
	

	case 3:
	case 1: 
	{ 
		applyFilterToPreview();
		break;		
	}

	case 2:
	{
		if (threadApplyingFilter != nullptr)
		{
			if (threadApplyingFilter->joinable())
				threadApplyingFilter->join();

			delete threadApplyingFilter;

			if (currentFilter != nullptr)
				delete currentFilter;

			currentFilter = nullptr;
			threadApplyingFilter = nullptr;

		}
		break;
	}

	default:
		break;
	}
}

void AppListener::applyFilterToPreview()
{

	if (filterLevel > 0)
	{
		Filter * f = getFilterToApply(filterKind, filterLevel);
		

		if (threadApplyingFilter != nullptr)
		{
			if (threadApplyingFilter->joinable())
				threadApplyingFilter->join();
			
			if (currentFilter != nullptr)
				delete currentFilter;
			//delete threadApplyingFilter;
		}

		currentFilter = f;

		//if (mutex.try_lock())
	{
		mutex.lock();
		_preview = _previewBackup->clone();
		mutex.unlock();
	}

	threadApplyingFilter = new std::thread(&AppListener::filterThread, this, f);
	}
	else
	{
		if (mutex.try_lock())
		{
			//mutex.lock();
			if (_preview != NULL || _preview != nullptr)
			{
				delete _preview;
				_preview = nullptr;
			}
			mutex.unlock();
		}

		{
			mutex.lock();
			_preview = _previewBackup->clone();
			mutex.unlock();
		}

		if (threadApplyingFilter != nullptr)
		{
			if (threadApplyingFilter->joinable())
			threadApplyingFilter->join();

		}

		if (currentFilter != nullptr)
			delete currentFilter;

		currentFilter = nullptr;
	}
	//threadApplyingFilter->();


	//threadApplyingFilter->
	/*Filter * f;

	int level = 0;
	_preview = _previewBackup->clone();

	if (filterLevel > 0)
	{
	level = (filterLevel * 2) + 1;

	if (level % 2 == 0)
	level -= 1;

	switch (filterKind)
	{
	case 'b': f = new BoxBlur(level); break;

	default:
	break;
	}

	f->apply(_previewBackup, _preview);

	delete f;
	}*/




}

void AppListener::filterThread(Filter * f)
{

	mutex.lock();
	f->apply(_previewBackup, _preview);
	mutex.unlock();

}

Filter * AppListener::getFilterToApply(char  kind, int level)
{
	if (level > 0)
	{
		level = (level * 2) + 1;

		if (level % 2 == 0)
			level -= 1;

		switch (kind)
		{
		case 'b': return new BoxBlur(level); break;
		case 'o': return new SmoothBlur(level); break;
		case 'm': return new MotionBlur(level); break;
		case 'g': return new GaussianBlur(level); break;
		case 's' : return new Sharpen(level); break;
		case 'e': return new Emboss(level); break;
		case 'l': return new Laplacian(level); break;
		case 'u': return new UnsharpMask(level); break;

		default: return nullptr; break;
		}
	}
}

std::string AppListener::getFilterName(const char & kind)
{
	switch (filterKind)
	{
	case 'b': return "Box Blur:"; break;
	case 'o': return "Smooth Blur:"; break;
	case 'm': return "Motion Blur:"; break;
	case 'g': return "Gaussian Blur:"; break;
	case 'u': return "Unsharp Mask"; break;
	case 'l': return "Laplacian"; break;
	case 'e': return "Emboss"; break;
	case 's': return "Sharpen"; break;
	default: return "None:"; break;
		break;
	}
}

/*
glLoadIdentity();
glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
glPushMatrix();										// Store The Projection Matrix
glLoadIdentity();									// Reset The Projection Matrix
gluOrtho2D(0.0, right, bottom, 0.0);			// Set Up An Ortho Screen
glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
glPushMatrix();										// Store The Modelview Matrix
glLoadIdentity();									// Reset The Modelview Matrix

glBegin(GL_QUADS);
glVertex2f(x, pos.y - hs);
glVertex2f(x, pos.y + hs);
glVertex2f(x + size.x, pos.y + hs);
glVertex2f(x + size.x, pos.y - hs);
glEnd();

glColor4f(1, 1, 1, 1);

if (value > 0) {

float factor = (float)value / (float)maxValue;

glBegin(GL_QUADS);
glVertex2f(x, pos.y - hs);
glVertex2f(x, pos.y + hs);
glVertex2f(x + (size.x * factor), pos.y + hs);
glVertex2f(x + (size.x * factor), pos.y - hs);
glEnd();
*/