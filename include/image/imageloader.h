
#ifndef   _IMAGELOADER_H_
#define   _IMAGELOADER_H_

#include <string>

class ImageLoader 
{
public:
	static ImageLoader & getInstance();

	bool loadImage(const std::string & filename, unsigned char ** pixels, int & width, int & height);
	bool saveImageFile(const std::string &  filename, unsigned char * pixels, int & width, int & height);

private:

	ImageLoader();
	~ImageLoader();

};

#endif