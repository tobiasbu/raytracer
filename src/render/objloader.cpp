
#include "render/objloader.h"


#include "utils/utils.h"
#include "utils/stringutils.h"
#include "math/maths.h"
#include <fstream>
#include <sstream>
#include <map>

ObjLoader::Model * ObjLoader::load(const std::string & filename)
{

	std::ifstream FILE(filename);

	if (!FILE.is_open())
	{
		//std::cerr << "ObjLoader: Failed to load file " << filename << "." << std::endl;
		return nullptr;
	}

	bool combineShapes = true;

	std::string directory = StringUtils::fileDirectory(filename);

	// temp variables
	std::string strbuf;
	// mesh name
	std::string name = "";
	// model name
	std::string modelName = "";
	bool newShape = false;
	std::vector<std::string> tokens;

	// vertexes position
	std::vector<vec3> positions; 
	// texture coords
	std::vector<vec2> texcoords; 
	// normals
	std::vector<vec3> normals; 

	// faces
	// every face has vertexes indexes
	std::vector<Face> faces;

	// materials
	std::vector<Material> materials;
	// materials map
	std::map<std::string, int> materialsMap;
	// current material id
	int materialID = -1;
	
	// model container = final result
	Model * model = new Model();

	model->path = directory;

	while (FILE.peek() != -1) {

		

		Utils::safeGetLine(FILE, strbuf);

		// Trim newline '\r\n' or '\n'
		if (strbuf.size() > 0) {
			if (strbuf[strbuf.size() - 1] == '\n')
				strbuf.erase(strbuf.size() - 1);
		}
		if (strbuf.size() > 0) {
			if (strbuf[strbuf.size() - 1] == '\r')
				strbuf.erase(strbuf.size() - 1);
		}

		if (strbuf.empty())
			continue;

		tokens.clear();



		StringUtils::tokenize<std::vector<std::string>>(strbuf, tokens, " \t",true);

		if (tokens.empty())
			continue;

		if (tokens[0] == "\0")
			continue;  // empty line

		if (tokens[0] == "#")
			continue;  // comment line

		if (tokens.size() > 1) {

			const std::string & key = tokens[0];


			if (key == "o" || key == "g" /*(str[0] == 'o' && str[1] == ' ') || (str[0] == 'g' && str[1] == ' ')*/) {
				//set new group or mesh name

				std::string mname(tokens[1]);

				if (mname.length() > 0)
					name = mname;
				else
					name = "unnamed";

				if (key == "o") 
					modelName = name;

				// generate new shape
				if (newShape)
				{
					if (faces.size() > 0) {

						exportFaces(model,faces, positions, normals, texcoords, materialID, name);

						/*if (m != nullptr) 
						{
							_meshes.push_back(m);
						}*/

						faces.clear();
					}
				}
				else
					newShape = true; // first mesh

			}
			else if (key == "v"/*str[0] == 'v' && str[1] == ' '*/) { //vertices
				//readVertices(str);
				
				positions.push_back(StringUtils::tokensParseVec3(tokens));
				continue;
			}
			else if (key == "vn" /*str[0] == 'v' && str[1] == 'n'*/) { //normals
				//readNormals(str);
				normals.push_back(StringUtils::tokensParseVec3(tokens));
				continue;
			}
			else if (key == "vt" /*str[0] == 'v' && str[1] == 't'*/) { //texture coords
				//readTextureCoord(str);
				texcoords.push_back(StringUtils::tokensParseVec2(tokens));
				continue;
			}
			else if (key == "f" /*str[0] == 'f' && str[1] == ' '*/) { //faces
				readFaces(tokens, faces);
				continue;
			}
			else if (key == "usemtl" /*str[0] == 'u' && str[1] == 's' && str[2] == 'e' && str[3] == 'm'*/) { // set material
				
				if (tokens.size() >= 2)
				{
					int newMaterialID = -1;

					// search if we have this material
					std::map<std::string, int>::iterator it = materialsMap.find(tokens[1]);

					if (it != materialsMap.end())
						newMaterialID = it->second;

					// if material not found or is new material
					if (newMaterialID != materialID)
					{
						// export mesh to model
						// every mesh has a material
						exportFaces(model, faces, positions, normals, texcoords, materialID,name);

						// set current material
						materialID = newMaterialID;

						// clear faces
						faces.clear();
					}

				}
				continue;
			}
			else if (key == "mtllib") // read material file
			{

					if (!tokens[1].empty()) 
					{
						readMaterial(directory, tokens[1], materials, materialsMap);
					}
					else 
					{
						// error
					}	
			}

		}

	}

	FILE.close();

	// export last mesh
	exportFaces(model, faces, positions, normals, texcoords,materialID,name);

	// give name to model
	model->name = name;

	model->materials.insert(model->materials.begin(), materials.begin(), materials.end());

	/*if (m != nullptr)
	{
		_meshes.push_back(m);
		//std::cout << m->_vertices.size() << std::endl;
	}*/

	faces.clear();
	positions.clear();
	normals.clear();
	texcoords.clear();


	return model;



}

int ObjLoader::parseFaceType(const std::string & str)
{
	if (str.c_str()[3] == ' ') { // only vertices
		return 1;
	}
	else if (str.c_str()[3] == '/' && str.c_str()[5] == '/') { //vertices and texture
		return 2;
	}
	else if (str.c_str()[3] == '/' && str.c_str()[4] == '/') { // vertices and normals
		return 3;
	}
	else if (str.c_str()[3] == '/' && str.c_str()[5] == ' ') { //vertices, texture and normals
		return 4;
	}
	else {
		return 0;
	}
	
}

void ObjLoader::readFaces(const std::vector<std::string> & tokens, std::vector<Face> & faces)
{
	// oldway
	//int vertexType = parseFaceType(str);


	//if (vertexType != 0) {

		int vertexType = 0;
		std::vector<std::string> vertexesIndices;

		// split face 
		//StringUtils::tokenize<std::vector<std::string>>(str, tokens, "f", true);

		size_t tsize = tokens.size();
		bool isQuad = false;
		Face f;

		// lets hack if we have a quad
		if (tsize > 4) // is quad
			isQuad = true;
		
		int prev = 0;

		// for each componente of face, verify the kind of vertex
		for (size_t i = 1; i < tsize; i++)
		{
			
			VertexIndex v;
			vertexType = 0;

			StringUtils::tokenize<std::vector<std::string>>(tokens[i], vertexesIndices, "/");

			// split will define the type of vertex type (dont trim the empty substrings)
			int size = vertexesIndices.size();

			// defining vertex type
			if (size == 1)
				vertexType = 1;
			else if (size == 2)
				vertexType = 2;
			else if (size == 3)
			{
				if (vertexesIndices[1] != "")
					vertexType = 4;
				else
					vertexType = 3;
			}

			if (size > 0) {

				int pos = StringUtils::parseInt(vertexesIndices[0]);

					switch (vertexType)
				{
					default:
						break;
						// only vertices position
					case 1:
					{
						v.vertex = pos;
						break;
					}
					// vertices and texture
					case 2:
					{
						v.vertex = pos;
						v.texcoord = StringUtils::parseInt(vertexesIndices[1]);
						//f.hasTexture = true;
						//f.indexes.push_back(StringUtils::parseInt(vertexesIndices[0]));
						//f.texIndexes.push_back(StringUtils::parseInt(vertexesIndices[1]));
						break;
					}
					// vertices and normal
					case 3:
					{
						v.vertex = pos;
						v.normal = StringUtils::parseInt(vertexesIndices[2]);
						/*f.hasNormals = true;
						f.indexes.push_back(StringUtils::parseInt(vertexesIndices[0]));
						f.normalIndexes.push_back(StringUtils::parseInt(vertexesIndices[2]));*/
						break;
					}
					// vertices, texture and normal
					case 4:
					{
						v.vertex = pos;
						v.texcoord = StringUtils::parseInt(vertexesIndices[1]);
						v.normal = StringUtils::parseInt(vertexesIndices[2]);
						/*f.hasNormals = true;
						f.hasTexture = true;
						f.indexes.push_back(StringUtils::parseInt(vertexesIndices[0]));
						f.texIndexes.push_back(StringUtils::parseInt(vertexesIndices[1]));
						f.normalIndexes.push_back(StringUtils::parseInt(vertexesIndices[2]));*/
						break;
					}
				}

					

					if (v.vertex != -1) {

						

						// is quad lets do some shitty hack
						if (i >= 4/*tsize - 1*/ && isQuad) {
							int vidx = f.vertexes.size() - 1;
							VertexIndex v0 = f.vertexes[vidx - 2], v1 = f.vertexes[vidx]; // 0, 2 
							f.vertexes.push_back(v0);
							f.vertexes.push_back(v1);
							f.vertexes.push_back(v);
						}
						else {
							f.vertexes.push_back(v);
						}

					}

				vertexesIndices.clear();
			}

			prev = i;

		}

		if (f.vertexes.size() > 0) 
		{
			faces.push_back(f);
		}
	
		

		/*
		/////////////// OLD WAY 
		int a, b, c, d;
		int t[] = { 0, 0, 0, 0 };
		int n[] = { 0, 0, 0, 0 };

		Face f;

		if (std::count(str.begin(), str.end(), ' ') == 4) {

			// is Quad
			f.isQuad = true;

			switch (vertexType)
			{
			default:
				// vertices
			case 1:  sscanf(str.c_str(), "f %d %d %d %d", &a, &b, &c, &d);  break;
				// vertices and texture
			case 2:{
				sscanf(str.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &a, &t[0], &b, &t[1], &c, &t[2], &d, &t[3]);
				f.hasTexture = true;
				break;
			}
				   // vertices and normals
			case 3: {
				sscanf(str.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &a, &n[0], &b, &n[1], &c, &n[2], &d, &n[3]);
				f.hasNormals = true;
				break;
			}
					//vertices / texture /  normal
			case 4: {
				sscanf(str.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &a, &t[0], &n[0], &b, &t[1], &n[1], &c, &t[2], &n[2], &d, &t[3], &n[3]);
				f.hasTexture = true;
				f.hasNormals = true;
				break;
			}
			}

		}
		else
		{
			// is Triangle
			f.isQuad = false;

			switch (vertexType)
			{
			default:
				// vertices
			case 1:  sscanf(str.c_str(), "f %d %d %d", &a, &b, &c);  break;
				// vertices and texture
			case 2:{
				sscanf(str.c_str(), "f %d/%d %d/%d %d/%d", &a, &t[0], &b, &t[1], &c, &t[2]);
				f.hasTexture = true;
				break;
			}
				   // vertices and normals
			case 3: {
				sscanf(str.c_str(), "f %d//%d %d//%d %d//%d", &a, &n[0], &b, &n[1], &c, &n[2]);
				f.hasNormals = true;
				break;
			}
					//vertices / texture /  normal
			case 4: {
				sscanf(str.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &a, &t[0], &n[0], &b, &t[1], &n[1], &c, &t[2], &n[2]);
				f.hasTexture = true;
				f.hasNormals = true;
				break;
			}
			}

		}

		f.set(a, b, c, d, t, n);

		faces.push_back(f);*/
	//}
}


/*void ObjLoader::readUsemtl(const std::vector<std::string> & tokens, std::map<std::string, int> & materialsMap, int & materialID)
{
	
}*/

bool ObjLoader::readMaterial(const std::string & baseDir, const std::string & materialFile, std::vector<Material> & materials, std::map<std::string, int> & materialMap)
{
	std::string fpath;

	if (baseDir.length() > 0)
		fpath = baseDir + materialFile;
	else
		fpath = materialFile;

	std::ifstream MATERIAL_FILE(fpath);

	if (!MATERIAL_FILE.is_open()) 
	{
		return false;
	}
	else 
	{
		loadMtl(MATERIAL_FILE, materialMap, materials);
	}

	MATERIAL_FILE.close();

	return true;
}

void ObjLoader::loadMtl(std::istream & stream, std::map<std::string, int> & materialsMap, std::vector<Material> & materials)
{
	std::string buf;
	std::vector<std::string> tokens;

	Material material;
	bool has_d_Readed = false;

	while (stream.peek() != -1) {

		Utils::safeGetLine(stream, buf);

		// Trim trailing whitespace.
		if (buf.size() > 0) {
			buf = buf.substr(0, buf.find_last_not_of(" \t") + 1);
		}

		// Trim newline '\r\n' or '\n'
		if (buf.size() > 0) {
			if (buf[buf.size() - 1] == '\n')
				buf.erase(buf.size() - 1);
		}
		if (buf.size() > 0) {
			if (buf[buf.size() - 1] == '\r')
				buf.erase(buf.size() - 1);
		}

		if (buf.empty())
			continue;

		tokens.clear();

		StringUtils::tokenize<std::vector<std::string>>(buf, tokens, " \t", true);

		if (tokens.empty())
			continue;

		if (tokens[0] == "\0")
			continue;  // empty line

		if (tokens[0] == "#")
			continue;  // comment line

		if (tokens.size() > 1)
		{
			const std::string & key = tokens[0];

			if (key == "newmtl") { // new material

				if (!material.name.empty()) {
					materialsMap.insert(std::pair<std::string, int>(material.name, static_cast<int>(materials.size())));
					materials.push_back(material);
				}

				has_d_Readed = false;
				material.reset();

				material.name = tokens[1];

				continue;

			}
			else if (key == "Ka") // ambient 
			{
				material.ka = StringUtils::tokensParseVec3(tokens);
				continue;
			}
			else if (key == "Kd") // diffuse
			{
				material.kd = StringUtils::tokensParseVec3(tokens);
				continue;
			}
			else if (key == "Ks") // specular
			{
				material.ks = StringUtils::tokensParseVec3(tokens);
				continue;
			}
			else if (key == "Kt" || key == "Tf") // transmittance
			{
				material.kt = StringUtils::tokensParseVec3(tokens);
				continue;
			}
			else if (key == "Ni") // index of refraction 
			{
				float val = StringUtils::parseFloat(tokens[1]);
				material.ni = val;
				continue;
			}
			else if (key == "Ke") // emission
			{
				material.ke = StringUtils::tokensParseVec3(tokens);
				continue;
			}
			else if (key == "Ns") // shininess
			{
				float val = StringUtils::parseFloat(tokens[1]);
				material.ns = val;
				continue;
			}
			else if (key == "illum") // illuminace model
			{
				int val = StringUtils::parseInt(tokens[1]);
				material.illum = val;
				continue;
			}
			else if (key == "d") // transparency - dissolve
			{
				float val = StringUtils::parseFloat(tokens[1]);
				material.d = val;
				has_d_Readed = true;
				continue;
			}
			else if (key == "Tr") { // transparency - dissolve

				if (!has_d_Readed) { // if d has been readed, just ignore it

					float val = 1.0f - StringUtils::parseFloat(tokens[1]);
					material.d = val;
				}
				continue;

			}
			else if (key == "map_Ka") {

				parseTexture(&(material.map_ka), tokens);
				continue;
			}
			else if (key == "map_Kd") {

				parseTexture(&(material.map_kd), tokens);
				continue;
			}
			else if (key == "map_Ks") {

				parseTexture(&(material.map_ks), tokens);
				continue;
			}
			else if (key == "map_d") {

				parseTexture(&(material.map_d), tokens);
				continue;
			}
			else if (key == "map_bump" || key == "bump") {

				parseTexture(&(material.map_bump), tokens);
				continue;
			}
		}
	}

	// add last amterial
	materialsMap.insert(std::pair<std::string, int>(material.name, static_cast<int>(materials.size())));
	materials.push_back(material);
}

bool ObjLoader::parseTexture(Texture * texture, std::vector<std::string> & tokens)
{
	bool hasTextureName = false;

	for (size_t i = 1; i < tokens.size(); i++)
	{
		if (tokens[i] == "-o") // offset
		{
			texture->offset = StringUtils::tokensParseVec3(tokens, i);
		}
		else if (tokens[i] == "-s") // scale
		{
			texture->offset = StringUtils::tokensParseVec3(tokens, i);
		}
		else 
		{
			texture->name = tokens[i];
			hasTextureName = true;
		}
	}

	return hasTextureName;

}

bool ObjLoader::exportFaces(Model * model, std::vector<Face> & faces, const std::vector<vec3> & positions, const std::vector<vec3> & normals, const std::vector<vec2> & texcoord, const int & materialID, const std::string & name)
{
	
	if (faces.size() <= 0)
		return false;

	std::vector<vec2> _texCoord;
	std::vector<vec3> _normals;
	std::vector<vec3> _positions;
	std::vector<unsigned int> _comboIndexes;
	std::vector<unsigned int> _triIndexes;
	
	//std::vector<unsigned int> tmpIndices;
	//std::vector<VertexIndex> tmpVertexes;

	std::map< std::pair<int, std::pair<int, int>>, unsigned int> _indexMap;
	std::vector<VertexIndex> _finalVertexes;
	unsigned int _nextCombinedIndex = 0;

	// first create vertexes from faces
	// for every face
	for (size_t i = 0; i < faces.size(); i++)
	{
		Face * f = &faces[i];

		// for every vertex index
		for (size_t j = 0; j < f->vertexes.size(); j++)
		{
			const VertexIndex & idx0 = f->vertexes[j];

			const int vertexIndex = idx0.vertex;
			const int texIndex = idx0.texcoord;
			const int normalIndex = idx0.normal;

			//if (texIndex != -1) { // has texture



			unsigned int combinedIndex = 0;
			std::map< std::pair<int, std::pair<int, int>>, unsigned int>::iterator it = _indexMap.find(std::make_pair(vertexIndex, std::make_pair(normalIndex, texIndex)));

			if (it != _indexMap.end())
			{
				combinedIndex = it->second;
			}
			else
			{
				combinedIndex = _nextCombinedIndex;
				_indexMap.insert(std::make_pair(std::make_pair(vertexIndex, std::make_pair(normalIndex, texIndex)), combinedIndex));
				_nextCombinedIndex++;
				_finalVertexes.push_back(VertexIndex(vertexIndex, normalIndex, texIndex));

			}

			_comboIndexes.push_back(combinedIndex);
			



		}

	}


	for (size_t i = 0; i < _finalVertexes.size(); i++)
		{
			int currentV = _finalVertexes[i].vertex - 1;
			int currentN = _finalVertexes[i].normal - 1;
			int currentSt = _finalVertexes[i].texcoord - 1;

			_positions.push_back(positions[currentV]);

			if (normals.size() > 0 && currentN >= 0)
			_normals.push_back(normals[currentN]);

			if (texcoord.size() > 0 && currentSt >= 0)
			_texCoord.push_back(texcoord[currentSt]);

		}

		// must be triangle
		/*for (size_t j = 0; j < f->vertexes.size(); j += 3)
		{

			const VertexIndex & idx0 = f->vertexes[j];
			const VertexIndex & idx1 = f->vertexes[j + 1];
			const VertexIndex & idx2 = f->vertexes[j + 2];

			_triangleIndexes.push_back(idx0.vertex - 1);
			_triangleIndexes.push_back(idx1.vertex - 1);
			_triangleIndexes.push_back(idx2.vertex - 1);

			// TODO arrange texture coord and normal to indexed model
			if (idx0.texcoord != -1) {
				_texCoord.push_back(texcoord[idx0.texcoord - 1]);
				_texCoord.push_back(texcoord[idx1.texcoord - 1]);
				_texCoord.push_back(texcoord[idx2.texcoord - 1]);
			}

			if (idx0.normal != -1)
			{
				_normals.push_back(normals[idx0.normal - 1]);
				_normals.push_back(normals[idx1.normal - 1]);
				_normals.push_back(normals[idx2.normal - 1]);
			}
			else // precompute normals
			{

			}
		}*/

		// triangulate if necessary
		// if is a valid face must be a triangle, then add to new indexes list
		/*vertexTriangulate(curr->vertexes, positions, tmpVertexes);

		size_t triSize = tmpVertexes.size();

		if (triSize > 3) {


		VertexIndex i0 = tmpVertexes[0];
		VertexIndex i1 = tmpVertexes[1];
		VertexIndex i2 = tmpVertexes[2];

		// for every 3 vertexes (is a triangle) - create a vertex atribute
		for (size_t i = 0; i < tmpVertexes.size(); i += 3)
		{


		const VertexIndex & idx0 = tmpVertexes[i];
		const VertexIndex & idx1 = tmpVertexes[i+1];
		const VertexIndex & idx2 = tmpVertexes[i+2];




		// replicate texcoord for every vertex (shity hack)

		if ((i-3) % 2 == 1) {
		texCoord.push_back(texcoord[i2.texcoord - 1]);
		texCoord.push_back(texcoord[i0.texcoord - 1]);
		texCoord.push_back(texcoord[i1.texcoord - 1]);
		}
		else {
		texCoord.push_back(texcoord[idx0.texcoord - 1]);
		texCoord.push_back(texcoord[idx1.texcoord - 1]);
		texCoord.push_back(texcoord[idx2.texcoord - 1]);
		}



		// append indices
		triangleIndexes.push_back(idx0.vertex - 1);
		triangleIndexes.push_back(idx1.vertex - 1);
		triangleIndexes.push_back(idx2.vertex - 1);

		i0 = idx0;
		i1 = idx1;
		i2 = idx2;

		//std::cout << curr->vertexes[idx0] << " " << curr->vertexes[idx1] << " " << curr->vertexes[idx2] << std::endl;
		std::cout << idx0 << " " << idx1 << " " << idx2 << std::endl;

		//v.position = positions[curr->indexes[idx0]-1];
		//v1.position = positions[curr->indexes[idx1]-1];
		//v2.position = positions[curr->indexes[idx1]-1];

		if (curr->hasTexture) {
		v.texCoord = texcoord[curr->texIndexes[idx0]];
		v1.texCoord = texcoord[curr->texIndexes[idx1]];
		v2.texCoord = texcoord[curr->texIndexes[idx2]];
		}

		if (curr->hasNormals) {
		v.normal = normals[curr->normalIndexes[idx0]-1];
		v1.normal = normals[curr->normalIndexes[idx1]-1];
		v2.normal = normals[curr->normalIndexes[idx2]-1];
		}
		else // compute normals
		{
		vec3 normal = Math::computeNormal(v.position,v1.position,v2.position);
		v.normal = normal;
		v1.normal = normal;
		v2.normal = normal;
		}

		vertexes.push_back(v);
		vertexes.push_back(v1);
		vertexes.push_back(v2);


		//}
		//tmpIndices.clear();*/
		//tmpVertexes.clear();


		//indices.push_back()
	

	Mesh * nmesh = nullptr;

	if (_comboIndexes.size() >= 3)
	{
		nmesh = new Mesh;
		nmesh->name = name;
		nmesh->indices = _comboIndexes;
		nmesh->vertices = _positions;
		nmesh->texcoord = _texCoord;
		nmesh->normals = _normals;
		nmesh->materialID = materialID;

		model->meshes.push_back(nmesh);


	}

	_texCoord.clear();
	_triIndexes.clear();
	_normals.clear();
	_positions.clear();

	if (nmesh != nullptr) {
		return true;
	}
	else 
	{
		return false;
	}

	

	
	
}

void ObjLoader::arrangeIndices(std::vector<int> & indices, int indice) {

	if (indices.size() == 0) {
		indices.push_back(indice);
		return;
	}

	//bool canAdd = true;
	int n = -1;

	for (size_t i = 0; i < indices.size(); i++)
	{
		if (indices[i] == indice)
			n = i;
	}

}

// triangulate list of faces
void ObjLoader::triangulate(std::vector<VertexIndex> & vertexes, const std::vector<vec3> & positions, std::vector<unsigned int> & outIndices)
{
	

	// is not a valid face
	if (vertexes.size() < 3)
		return;

	// if it is a triangle no need to calculate it
	if (vertexes.size() == 3)
	{
		outIndices.push_back(0);
		outIndices.push_back(1);
		outIndices.push_back(2);
		return;
	}


	// temp vertexes indices
	std::vector<VertexIndex> tIndices = vertexes;

	// simple triangulate
	while (true)
	{


		// for every index of face indexes
		for (int i = 0; i < (int)tIndices.size(); i++)
		{



			int current = tIndices[i].vertex;

			int next;
			int prev;

			// next index
			if (i == tIndices.size() - 1)
				next = tIndices[0].vertex;
			else
				next = tIndices[i + 1].vertex;

			// prev index
			if (i == 0)
				prev = tIndices[tIndices.size() - 1].vertex;
			else
				prev = tIndices[i - 1].vertex;


			// has only three indexes
			if (tIndices.size() == 3) // is triangle
			{
				// check face indexes
				for (size_t j = 0; j < tIndices.size(); j++)
				{
					if (vertexes[j].vertex == current)
						outIndices.push_back(j);
					if (vertexes[j].vertex == next)
						outIndices.push_back(j);
					if (vertexes[j].vertex == prev)
						outIndices.push_back(j);
				
				}

				tIndices.clear();
				break;
			}
			else if (tIndices.size() == 4) // is quad
			{
				// create a triangle
				for (size_t j = 0; j < vertexes.size(); j++)
				{
					if (vertexes[j].vertex == current)
						outIndices.push_back(j);
					if (vertexes[j].vertex == next)
						outIndices.push_back(j);
					if (vertexes[j].vertex == prev)
						outIndices.push_back(j);

				}

				int tempIndex;
				// double check again in the vertices indexes to not set the same as previous of quad
				for (size_t j = 0; j < tIndices.size(); j++)
				{
					if (tIndices[j].vertex != current
						&& tIndices[j].vertex != prev
						&& tIndices[j].vertex != next)
					{
						tempIndex = tIndices[j].vertex;

						break;
					}
				}

				// Create second triangle from quad
				for (size_t j = 0; j < vertexes.size(); j++)
				{
					if (vertexes[j].vertex == prev)
						outIndices.push_back(j);
					if (vertexes[j].vertex == next)
						outIndices.push_back(j);
					if (vertexes[j].vertex == tempIndex)
						outIndices.push_back(j);
				}

				tIndices.clear();
				break;
			}

			// checking vertexes
			// check angle between vertexes. is not inside vertex
			float angle = vec3::angle(positions[prev] - positions[current], positions[next] - positions[current]) * RAD_TO_DEG;//(180 / 3.14159265359);
			if (angle <= 0 && angle >= 180)
				continue;

			// If any vertices are within this triangle
			bool inTri = false;

			for (size_t j = 0; j < vertexes.size(); j++)
			{
				if (Math::inTriangle(
					positions[tIndices[prev].vertex],
					positions[tIndices[current].vertex],
					positions[tIndices[next].vertex],
					positions[vertexes[j].vertex])
					&& vertexes[j] != tIndices[prev]
					&& vertexes[j] != tIndices[current]
					&& vertexes[j] != tIndices[next])
				{
					inTri = true;
					break;
				}
			}

			if (inTri)
				continue;

			// Create triangle
			for (size_t j = 0; j < vertexes.size(); j++)
			{
				if (vertexes[j].vertex == current)
					outIndices.push_back(j);
				if (vertexes[j].vertex == prev)
					outIndices.push_back(j);
				if (vertexes[j].vertex == next)
					outIndices.push_back(j);
			}

			// delete indices
			// Delete pCur from the list
			for (size_t j = 0; j < tIndices.size(); j++)
			{
				if (tIndices[j].vertex == current)
				{
					tIndices.erase(tIndices.begin() + j);
					break;
				}
			}

			// reset i to the start
			// -1 since loop will add 1 to it
			i = -1;
		}

		// if no triangles were created
		if (outIndices.size() == 0)
			break;

		// if no more vertices
		if (tIndices.size() == 0)
			break;
	}

	
}

void ObjLoader::vertexTriangulate(std::vector<VertexIndex> & face, const std::vector<vec3> & positions, std::vector<VertexIndex> & outfaces) {

	// is not a valid face
	if (face.size() < 3)
		return;

	// if it is a triangle no need to calculate it
	if (face.size() == 3)
	{
		outfaces = face;
		return;
	}


	// temp vertexes indices
	std::vector<VertexIndex> tFace = face;

	// simple triangulate
	while (true)
	{


		// for every index of face indexes
		for (int i = 0; i < (int)tFace.size(); i++)
		{
			VertexIndex & current = tFace[i];
			VertexIndex next = tFace[i];
			VertexIndex prev = tFace[i];

			// next index vertex
			if (i == tFace.size() - 1)
				next = tFace[0];
			else
				next = tFace[i + 1];

			// prev index vertex
			if (i == 0)
				prev = tFace[tFace.size() - 1];
			else
				prev = tFace[i - 1];

			// has only three indexes
			if (tFace.size() == 3) // is triangle
			{
				// check face indexes
				for (size_t j = 0; j < tFace.size(); j++)
				{
					if (face[j] == current)
						outfaces.push_back(current);
					if (face[j] == next)
						outfaces.push_back(next);
					if (face[j] == prev)
						outfaces.push_back(prev);

				}

				tFace.clear();
				break;
			}
			else if (tFace.size() == 4) // is quad
			{
				// create a triangle
				for (size_t j = 0; j < face.size(); j++)
				{
					if (face[j] == current)
						outfaces.push_back(current);
					if (face[j] == next)
						outfaces.push_back(next);
					if (face[j] == prev)
						outfaces.push_back(prev);

				}

				VertexIndex & tempIndex = current;
				// double check again in the vertices indexes to not set the same as previous of quad
				for (size_t j = 0; j < tFace.size(); j++)
				{
					if (tFace[j] != current
						&& tFace[j] != prev
						&& tFace[j] != next)
					{
						tempIndex = tFace[j];

						break;
					}
				}

				// Create second triangle from quad
				for (size_t j = 0; j < face.size(); j++)
				{
					if (face[j] == prev)
						outfaces.push_back(prev);
					if (face[j] == next)
						outfaces.push_back(next);
					if (face[j] == tempIndex)
						outfaces.push_back(face[j]);
				}

				tFace.clear();
				break;
			}

			// If Vertex is not an interior vertex
			float angle = vec3::angle(positions[prev.vertex] - positions[current.vertex], positions[next.vertex] - positions[current.vertex]) * RAD_TO_DEG;
			if (angle <= 0 && angle >= 180)
				continue;

			bool inTri = false;

			for (int j = 0; j < int(face.size()); j++)
			{
				if (Math::inTriangle(
					positions[prev.vertex],
					positions[current.vertex],
					positions[next.vertex],
					positions[face[j].vertex]
					)
					&& face[j] != prev
					&& face[j] != current
					&& face[j] != next)
				{
					inTri = true;
					break;
				}
			}

			if (inTri)
				continue;

			// Create a triangle from pCur, pPrev, pNext
			for (int j = 0; j < int(face.size()); j++)
			{
				if (face[j] == current)
					outfaces.push_back(current);
				if (face[j] == prev)
					outfaces.push_back(prev);
				if (face[j] == next)
					outfaces.push_back(next);
			}

			// Delete pCur from the list
			for (int j = 0; j < int(tFace.size()); j++)
			{
				if (tFace[j] == current)
				{
					tFace.erase(tFace.begin() + j);
					break;
				}
			}

			// reset i to the start
			// -1 since loop will add 1 to it
			i = -1;

		}

		
		// if no triangles were created
		if (outfaces.size() == 0)
			break;

		// if no more vertices
		if (tFace.size() == 0)
			break;
		

	}

}