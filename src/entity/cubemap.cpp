

#include "entity/cubemap.h"

#include "utils/stringutils.h"
#include "others/log.h"
#include <sstream>


bool CubeMap::load(const std::string & filespath, const std::string & fileSuffix, std::string & extension)
{
	extension = StringUtils::toLower(extension);

	std::stringstream str;

	std::string suffix[6] = { "right", "left", "top", "bottom", "front", "back" };

	for (int i = 0; i < 6; i++)
	{
		str << filespath << fileSuffix << suffix[i] << "." << extension;
		if (!_textures[i]->load(str.str())) 
			Debug::log("CubeMap: Failed to load file: %s", str.str());
		

		str.str("");
	}

	return true;
}

void CubeMap::setTexture(const CubeMapFace & face, Texture * texture)
{
	_textures[face] = texture;
}

bool CubeMap::getTexel(const vec3 & direction, Color & outColor)
{

	float absX = fabsf(direction.x);
	float absY = fabsf(direction.y);
	float absZ = fabsf(direction.z);

	int isXPositive = direction.x > 0 ? true : false;
	int isYPositive = direction.y > 0 ? true : false;
	int isZPositive = direction.z > 0 ? true : false;

	float maxAxis, uc, vc;
	float u, v;
	//CubeMapFace face;
	int index = 0;

	// POSITIVE X - left
	if (isXPositive && absX >= absY && absX >= absZ) {
		// u (0 to 1) goes from +z to -z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = -direction.z;
		vc = direction.y;
		index = 0;
	}
	// NEGATIVE X - right
	if (!isXPositive && absX >= absY && absX >= absZ) {
		// u (0 to 1) goes from -z to +z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = direction.z;
		vc = direction.y;
		index = 1;
	}
	// POSITIVE Y - top
	if (isYPositive && absY >= absX && absY >= absZ) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from +z to -z
		maxAxis = absY;
		uc = direction.x;
		vc = -direction.z;
		index = 2;
	}
	// NEGATIVE Y - bottom
	if (!isYPositive && absY >= absX && absY >= absZ) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -z to +z
		maxAxis = absY;
		uc = direction.x;
		vc = direction.z;
		index = 3;
	}
	// POSITIVE Z - front
	if (isZPositive && absZ >= absX && absZ >= absY) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = direction.x;
		vc = direction.y;
		index = 4;
	}
	// NEGATIVE Z - back
	if (!isZPositive && absZ >= absX && absZ >= absY) {
		// u (0 to 1) goes from +x to -x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = -direction.x;
		vc = direction.y;
		index = 5;
	}

	// Convert range from -1 to 1 to 0 to 1
	u = 0.5f * (uc / maxAxis + 1.0f);
	v = 0.5f * (vc / maxAxis + 1.0f);

	if (_textures != nullptr && _textures[index] != nullptr) {

		if (_textures[index]->loaded())
			outColor = _textures[index]->getTexel(u, v);

		return true;
	}
	else
	{
		return false;
	}

	// right and left position
	/*if ((fabsf(direction.x) >= fabsf(direction.y)) && (fabsf(direction.x) >= fabsf(direction.y)))
	{
		if (dir[0] > 0.0f)
		{
			outColor = t[RIGHT]((dir[2] / dir[0] + 1.0f) * 0.5f,	(dir[1] / dir[0] + 1.0f) * 0.5f);
		}
		else if (dir[0] < 0.0f)
		{
			outColor = t[LEFT]((dir[2] / dir[0] + 1.0f) * 0.5f,
				1.0 - (dir[1] / dir[0] + 1.0f) * 0.5f);
		}
	}
	else if ((fabsf(dir[1]) >= fabsf(dir[0])) && (fabsf(dir[1]) >= fabsf(dir[2])))
	{
		if (dir[1] > 0.0f)
		{
			outColor = t[UP]((dir[0] / dir[1] + 1.0f) * 0.5f,
				(dir[2] / dir[1] + 1.0f) * 0.5f);
		}
		else if (dir[1] < 0.0f)
		{
			outColor = t[DOWN](1.0f - (dir[0] / dir[1] + 1.0f) * 0.5f,
				1.0f - (dir[2] / dir[1] + 1.0f) * 0.5f);
		}
	}
	else if ((fabsf(dir[2]) >= fabsf(dir[0]))
		&& (fabsf(dir[2]) >= fabsf(dir[1])))
	{
		if (dir[2] > 0.0f)
		{
			outColor = t[FRONT](1.0f - (dir[0] / dir[2] + 1.0f) * 0.5f,
				(dir[1] / dir[2] + 1.0f) * 0.5f);
		}
		else if (dir[2] < 0.0f)
		{
			outColor = t[BACK]((dir[0] / dir[2] + 1.0f) * 0.5f,
				1.0f - (dir[1] / dir[2] + 1.0f) * 0.5f);
		}
	}*/
}