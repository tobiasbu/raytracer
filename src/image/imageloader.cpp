
#include "image/imageloader.h"
#include "utils/stringutils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "extlibs\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "extlibs\stb_image_write.h"

ImageLoader & ImageLoader::getInstance()
{
	static ImageLoader instance;

	return instance;
}


ImageLoader::ImageLoader()
{

}

ImageLoader::~ImageLoader()
{

}

bool ImageLoader::loadImage(const std::string &  filename, unsigned char ** pixels, int & width, int & height)
{
	int channels = 0;
	//pixels = NULL;

	unsigned char * ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	if (ptr)
	{
		
		int buffer = width * height * 4;
		*pixels = new unsigned char[buffer];
		memcpy(&*pixels[0], ptr, buffer);

		// free
		stbi_image_free(ptr);
		return true;
	}
	else 
	{
		//err() << "Failed to load image \"" << filename << "\". Reason: " << stbi_failure_reason() << std::endl;
		return false;
	}
}

bool ImageLoader::saveImageFile(const std::string &  filename, unsigned char * pixels, int & width, int & height)
{
	if (pixels != nullptr && width > 0 && height > 0)
	{
		std::string extension = StringUtils::fileExtension(filename);

		if (extension != "") 
		{
			StringUtils::toLower(extension);

			if (extension == "bmp")
			{
				// BMP format
				if (stbi_write_bmp(filename.c_str(), width, height, 4, &pixels[0]))
					return true;
			}
			else if (extension == "tga")
			{
				// TGA format
				if (stbi_write_tga(filename.c_str(), width, height, 4, &pixels[0]))
					return true;
			}
			else if (extension == "png")
			{
				// PNG format
				if (stbi_write_png(filename.c_str(), width, height, 4, &pixels[0], 0))
					return true;
			}
			
			//else if (extension == "jpg" || extension == "jpeg")
			//{
		}
		
	}



	return false;
}
