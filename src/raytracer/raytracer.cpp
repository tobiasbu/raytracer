
#include "raytracer\raytracer.h"
#include "render\texturemapping.h"
#include <algorithm> 
#include "math/maths.h"



void Raytracer::init(Scene * scene, Options * options)
{
	_scene = scene;
	_options = options;

	if (!bvh || bvh == NULL || bvh == nullptr) {


		bvh = new BVH; // (scene, split)
	}

	if (options->traceMethod >= 2)
	{
		

		BVH::SplitMethod split = BVH::SplitMethod::Middle;

		if (options->traceMethod == 3)
			split = BVH::SplitMethod::SAH;

		bvh->set(scene, split);
		bvh->build();
	}

	
}

Color Raytracer::castRay(const Ray & ray, RaycastHit & primaryHit, const uint32_t & depth) {

	if (depth > _options->maxDepth) {
		return _scene->getBackgroundColor(ray.direction);
	}

	RaycastHit hit;

	if (trace(ray, hit)) { 


		// get object infos
		hit.point = ray.origin + ray.direction * hit.distance;

		
		hit.object->computeSurfaceData(hit);

		/*if (_options->renderMethod == 1)
		{
			hit.normal = hit.point - hit.object->position;
			hit.normal.normalize();
			//hit.texCoord = TextureMapping::planar(hit.point, hit.normal);
		}
		else 
		{*/
			
		

		
		Material * material = hit.object->getMaterial();

		// get the default material
		if (material == nullptr)
			material = _scene->getDefaultMaterial();


		// ambient color
		vec4 color = _scene->getAmbientColor(); 
		

		// lights (diffuse, specular)
		std::vector<Light *> * __restrict lights = &_scene->getLights();
		
		
		//float bias = 0.0001f;//EPSILON;

		int imax = lights->size();

		//vec3 shadowRayOrig = hit.point + hit.normal * bias;

		if (imax > 0/*material != nullptr*/) 
		{

			color *= material->getVec3(MAT_AMBIENT);

			//if (!material->getUnlit()) {
			Ray lightRay;
			RaycastHit hitShadow;

			lightRay.origin = (vec3::dot(ray.direction, hit.normal) < 0) ?
				hit.point + hit.normal * _options->bias :
				hit.point - hit.normal *  _options->bias;
			//lightRay.origin = hit.point + hit.normal * _options->bias;
			vec3 lightAmt = 0;
			vec3 diff, spec;

			

				for (int i = 0; i < imax; i++) {

					vec3 lightDir;
					vec3 lightIntensity;
					float lightDistance = 0;
					float radius = 0;

					Light * light = (*lights)[i];

					light->illuminate(hit.point, lightDir, lightIntensity, lightDistance, radius);
					
					//vec3 lightDir2 = (*lights)[i]->position - hit.point;
					//lightDir.normalize();
					float lightDistance2 = vec3::dot(lightDir, lightDir);
					
					
					//float lightDistance2 = vec3::dot(lightDir, lightDir);

				
					//float LdotN = Math::max(0.f, vec3::dot(lightDir, hit.normal));
					
					hitShadow.object = nullptr;
					hitShadow.distance = Infinity;
					lightRay.direction = lightDir;

					if(_options->renderType == 0)
						color += material->shade2(ray, hit, lightIntensity, lightDir);
					else if (_options->renderType == 1)
						color += material->shadeCelShading(ray, hit, lightIntensity, lightDir, _options->cellevel, _options->celfactor);
					else if (_options->renderType == 2) 
					{
						color += material->shadeGooch(ray, hit, lightIntensity, lightDir,
							_options->warmColor, _options->coolColor, 
							_options->goshAlpha, _options->goshBeta, _options->celOutline);
					}
					else if (_options->renderType == 3) 
					{
						color += material->shadeGooch(ray, hit, lightIntensity, lightDir,
							_options->warmColor, _options->coolColor, _options->goshAlpha,
							_options->goshBeta, _options->celOutline, true);
					}

					if (light->type == 0) 
					{
						if (_options->shadowType > 0)
						{
							_shadowRays++;
							bool inShadows = trace(lightRay, hitShadow) && hitShadow.distance < lightDistance2 / 2;

							if (inShadows &&  hitShadow.object != hit.object)
								color *= _options->shadowIntensity;
						}
					}
					else
					{

						if (_options->shadowType == 1)// hard shadows
						{
							_shadowRays++;
							bool inShadows = trace(lightRay, hitShadow) && hitShadow.distance * hitShadow.distance < lightDistance2;

							if (inShadows &&  hitShadow.object != hit.object)
								color *= _options->shadowIntensity;

							//color += (1-inShadows) * material->shade2(ray, hit, lightIntensity, lightDir);

							/*if (_options->renderType == 0)
								color += material->shade2(ray, hit, lightIntensity, lightDir);
								else if (_options->renderType == 1)
								color += material->shadeCelShading(ray, hit, lightIntensity, lightDir, _options->cellevel, _options->celfactor);
								else if (_options->renderType == 2)
								color += material->shadeGooch(ray, hit, lightIntensity, lightDir, _options->warmColor, _options->coolColor);*/




							// lightAmt += (inShadows) * std::max(0.f, vec3::dot(hit.normal, lightDir)) / 2;
							//color = Color::black;
							//color -= lightIntensity * std::max(0.f, vec3::dot(hit.normal, -lightDir));// * 0.5f;
							// (1 - inShadows)
							//diff += material->getAlbedo(hit.texCoord) * lightIntensity * std::max(0.f, vec3::dot(hit.normal, -lightDir));

							//vec3 r = vec3::reflect(-lightDir2, hit.normal);
							//spec += std::powf(std::max(0.f, -vec3::dot(r, ray.direction)), material->_ns) * lightIntensity;
							// float LdotN

						}
						else if (_options->shadowType == 2) // soft shadows 1
						{
							_shadowRays++;
							bool inShadows = trace(lightRay, hitShadow) && hitShadow.distance < lightDistance / 2;

							//vec3::dot(, );
							//float v = vec3::dot(hit.normal, ray.direction);

							// esta na sombras, mas não é o proprio objeto?
							if (inShadows && hitShadow.object != hit.object) // logo estou na sombras
							{
								// atenuacao 
								float f = 1.f / (hitShadow.distance * hitShadow.distance);

								// normal em relação ao centro do objeto 
								// pode se pegar posição do objeto.
								// no caso, pego o centroid do delimitador (no final das contas da na mesma)
								vec3 c = hit.point - hitShadow.object->getBounds().centroid();
								c.normalize();

								// produto escalar do centro ao normal
								// isso aqui serve pra calcular a umbra
								float ct = vec3::dot(c, hit.normal);

							

								// calculos penumbra e umbra:

								float value = 0;

								/// PENUMBRA 1 - soft

								if (_options->penumbraType == 0) 
								{

									value = ct / f - 1;

									if (value >= 1)
										value /= ct / f - 1;

									color *= value * _options->shadowIntensity;;
								}
								else if (_options->penumbraType == 1) 
								{
									/// PENUMBRA 2 - soft 2

									
									value = ct / f - 1;
									//value /= (v + ct / f) - 1;
									value /= ct / f;

									color *= value * _options->shadowIntensity;
									
								}
								else if (_options->penumbraType == 2) 
								{
									
									/// PENUMBRA 3 - com deteccao de umbra
									// detecta umbra

									// cor da luz ( estilo difusa)
									// eu mesmo calculo usado para fazer a dfiusa
									float ln = vec3::dot(-lightDir, hit.normal);

									float t = ct + ln / f;
									value = t;
									value /= 1 - (t * f);

									if (value >= 1) // é penumbra
									{
										value = (ln + ct) - 1;

										if (value >= 1)
											value /= 1 - (ln + ct / f);

										color *= value *  _options->shadowIntensity;


									}
									else // é umbra
										color *= value * _options->shadowIntensity;

									//float penumbraSize = (radius * hitShadow.distance) / lightDistance;
								}
								

								/*/// PENUMBRA 2 - soft 2


								value = ct / f - 1;
								//value /= (v + ct / f) - 1;
								value /= ct / f;

								color *= value; */
							}
						}
						else if (_options->shadowType == 3) // soft jittered shadows 2
						{

							float shadow = 1.0 - calcShadowAmount(ray, *light, hit);
							color *= shadow * _options->shadowIntensity;

						}
					}

				}

		
				color.w = 1;

		}
		else // no lights, use face ratio instead
		{
			
			/*color = material->getAlbedo(hit.texCoord); 
			color *= std::max(0.f, vec3::dot(hit.normal, -ray.direction));
			color.w = 1;*/

			
			if (_options->renderMethod == 0) {
				const int scale = 5;
				float checker = (fmodf(hit.texCoord.x * scale, 1.0) > 0.5) ^ (fmodf(hit.texCoord.y * scale, 1.0) > 0.5);
				float c = Math::lerp(0.3f, 0.7f, checker);//0.3 * (1 - checker) + 0.7 * checker;
				color = c;
			}
			else 
			{
				const Bounds & world = _scene->getWorldBounds();

				if (_options->renderMethod == 2) 
				{
				// far distance from ray origin (camera)

					int axis = world.getMaxAxis();
					//int axismin = world.getMinAxis();
					//float max = world.getSize()[axis];
					

					float tFar = hit.distance / world.getSize().magnitude();
					color = vec3::lerp(vec3(1, 0, 0.531), vec3(0.037, 0.937, 0.231), 1-tFar);
				}
				else 
				{ // longest axis

					int axis = world.getMaxAxis();
					float v = world.getMax()[axis];
					float tFar = hit.point[axis] / v;
					color = vec3::lerp(vec3(0.037, 0.235, 0.531), vec3(0.937, 0.937, 0.331), 1 - tFar);
				}
				
			}

			// facing ratio
			float NdotView = std::max(0.25f, vec3::dot(hit.normal, -ray.direction));
			color *= NdotView;
			color.w = 1;

			// whitted checkboard colors
			/*float scale = 5;
			float pattern = (fmodf(hit.texCoord.x * scale, 1) > 0.5) ^ (fmodf(hit.texCoord.y * scale, 1) > 0.5);
			color = vec3::lerp(vec3(0.815, 0.235, 0.031), vec3(0.937, 0.937, 0.231), pattern);
			color.w = 1;*/

			
		}
	
		if (depth < _options->maxDepth) {

			float ior = material->getFloat(MAT_REFRACTION);
			float alpha = material->getFloat(MAT_TRANSPARENCY);

			if (ior > 0 && alpha > 0) { // 

				vec4 colRefraction(0);
				vec4 colReflection(0);

				// ray for reflection and refraction
				Ray rayrr;

				// compute fresnel
				float kr = 0;
				fresnel(ray.direction, hit.normal, ior, kr);

	
				// compute ray origin
				// apply bias acordding if intersect is inside or outside the object
				vec3 biasVec = hit.normal * _options->bias;

				// check if intersection ray is outside or inside object
				bool outside = vec3::dot(ray.direction, hit.normal) < 0;
	
				// refraction ---
				// compute refraction if it is not a case of total internal reflection
				if (kr < 1) 
				{
					rayrr.origin = outside ? hit.point - biasVec : hit.point + biasVec;
					rayrr.direction = vec3::refract(ray.direction, hit.normal, ior).normalized();
					_secondaryRays++;
					colRefraction = vec4(castRay(rayrr, primaryHit, depth + 1), 1) * alpha;
					colRefraction.w = 1;
					//colRefraction *= material->_d;
				}

				// reflection: ---

				// compute ray for reflection		
				rayrr.direction = vec3::reflect(ray.direction, hit.normal).normalized();
				rayrr.origin = outside ? hit.point + biasVec : hit.point - biasVec;
			
				// reflection color
				_secondaryRays++;
				colReflection = castRay(rayrr, primaryHit, depth + 1);
		
				vec4 kt = vec4(material->getVec3(MAT_TRANSMITTANCE),1);
				
				color += (colReflection * kt * kr) + (colRefraction * (1 - kr));
				color.w = 1;



			}
			else if (ior > 0) { // only reflection

				// compute fresnel
				float kr = 0;
				fresnel(ray.direction, hit.normal, ior, kr);

				// compute ray
				Ray rayRefract;

				//vec3 reflectionRayOrig = (vec3::dot(ray.direction, hit.normal) < 0) ?	hit.point + hit.normal * bias :	hit.point - hit.normal * bias;
				//vec3 reflectionRayOrig = hit.point + hit.normal * bias;
				rayRefract.origin = (vec3::dot(ray.direction, hit.normal) < 0) ? hit.point + hit.normal * _options->bias : hit.point - hit.normal * _options->bias;

				// vec3 reflectDir =
				rayRefract.direction = vec3::reflect(ray.direction, hit.normal).normalized();
				_secondaryRays++;
				vec4 c = vec4(castRay(rayRefract, primaryHit, depth + 1)) * kr;

				c.w = 1;
				color += c;
				
				

			}
		}

		/*float NdotView = std::max(0.f, vec3::dot(hit.normal,-ray.direction));
		const int M = 4;
		float checker = (fmod(hit.texCoord.x * M, 1.0) > 0.5) ^ (fmod(hit.texCoord.y * M, 1.0) < 0.5);
		float c = 0.3 * (1 - checker) + 0.7 * checker;

		color = vec4(vec3(c * NdotView),1);*/
		//color = material->getTexture()->getColor(hit.texCoord.x, hit.texCoord.y);
		
		/*float kd = 1;
		float ks = 0.1;
		color = diffuse * kd + specular * ks;*/

		// facing ratio
		// color *= std::max(0.f, vec3::dot(hit.normal, -ray.direction));

		primaryHit = hit;

		 return color.toColor();

	}
	else {
		return _scene->getBackgroundColor(ray.direction);
	}

}

// trace ray into objects
bool Raytracer::trace(const Ray & ray, RaycastHit & hit/*, float & tNear*/) {
	 

	if (_options->traceMethod == 0)
	{
		return traceList(ray,hit);
	}
	else if (_options->traceMethod == 1)
	{
		return traceAABB(ray, hit);
	}
	else if (_options->traceMethod >= 2)
	{
		return bvh->intersects(ray,hit);
	}

	

}

bool Raytracer::traceList(const Ray & ray, RaycastHit & hit)
{
	std::vector<Primitive *> * objs = &_scene->getPrimitives(); // (std::vector<Primitive *>*)
	hit.object = nullptr;
	float tNear = Infinity;

	Ray bboxray(ray);
	const bool sign[] = { (ray.direction.x < 0), (ray.direction.y < 0), (ray.direction.z < 0) };
	bboxray.direction = 1.0f / bboxray.direction;

	for (size_t i = 0; i < objs->size(); i++) {

		Primitive * __restrict obj = (*objs)[i];

		//float t = fInfinity;

				if (obj->getType() != Primitive::PrimitiveType::Mesh)
					_intersectionTests++;

				if (_options->renderMethod == 1) 
				{
					const Bounds & aabb = obj->getBounds();
					_aabbTests++;
					if (aabb.intersects(bboxray, sign, tNear) && tNear < hit.distance)
					{
						hit.object = (*objs)[i];
						hit.distance = tNear;
						hit.ray = &ray;
					}
				}
				else {

					if (obj->intersects(ray, hit, tNear) && tNear < hit.distance) { // has intersection to object // (*objs)[i]
						hit.object = (*objs)[i];
						hit.distance = tNear;
						hit.ray = &ray;
					}

				}


				_triangleTests += hit.tests;

		}

	return (hit.object != nullptr);
}

bool Raytracer::traceAABB(const Ray & ray, RaycastHit & hit)
{
	std::vector<Primitive *> * objs = &_scene->getPrimitives(); // (std::vector<Primitive *>*)

	hit.object = nullptr;



	//bboxray.origin = ray.origin;
	//bboxray.direction = ray.direction;
	Ray bboxray(ray);
	// bboxray.transform(obj->getInversedMatrix());
	const bool sign[] = { (ray.direction.x < 0), (ray.direction.y < 0), (ray.direction.z < 0) };
	bboxray.direction = 1.0f / bboxray.direction;

	float tNear = Infinity;

	for (size_t i = 0; i < objs->size(); i++) {

		Primitive * __restrict obj = (*objs)[i];

		float t = Infinity;

		if (_options->renderMethod == 0)
		{

			if (obj->getType() == Primitive::PrimitiveType::Plane) { // plane ignore bbox


				if (obj->intersects(ray, hit, tNear) && tNear < hit.distance) {
					hit.object = (*objs)[i];
					hit.distance = tNear;
					hit.ray = &ray;
				}

			}
			else {

				//bboxray.transform(obj->getInversedMatrix());

				const Bounds & aabb = obj->getBounds();
				_aabbTests++;
				if (aabb.intersects(bboxray, sign, t)) {

					if (obj->getType() != Primitive::PrimitiveType::Mesh)
						_intersectionTests++;

					if (obj->intersects(ray, hit, tNear) && tNear < hit.distance) { // has intersection to object // (*objs)[i]
						hit.object = (*objs)[i];
						hit.distance = tNear;
						hit.ray = &ray;
					}

					_triangleTests += hit.tests;
				}
				//else
				// continue;
			}
		}
		else 
		{
			const Bounds & aabb = obj->getBounds();
			_aabbTests++;
			if (aabb.intersects(bboxray, sign, tNear)) {

				if (obj->getType() != Primitive::PrimitiveType::Mesh)
					_intersectionTests++;

				
					hit.object = (*objs)[i];
					hit.distance = tNear;
					hit.ray = &ray;
				

				
			}
		}
	}



	return (hit.object != nullptr);
}

void Raytracer::countVariables()
{
	if (_options->traceMethod >= 2)
	{
		_aabbTests = bvh->getAABBTests();
		_intersectionTests = bvh->getIntersectionTests();
		_triangleTests = bvh->getTriangleTests();

		uint32_t t = 0, a = 0, p = 0;
		_scene->countVariables(t, a, p);
		_aabbTests += a;
		_triangleTests += t;
		_intersectionTests += p;
	}
}

void Raytracer::fresnel(const vec3 & i, const vec3 & n, const float & ior, float & kr)
{
	float cosi = Math::clamp(-1, 1, vec3::dot(i,n));
	float etai = 1, etat = ior;
	if (cosi > 0) 
	{ std::swap(etai, etat); }
	// Compute sini using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
}

float Raytracer::calcShadowAmount(const Ray& ray, const Light & light, const RaycastHit & hit)
{
	float shadowAmount = 0;

	// samples
	

	vec3 directionToLight = (light.position - hit.point).normalized();
	vec3 lightRight = vec3::cross(directionToLight, vec3(0.0001,1, 0.0001)).normalized(); // ALMOST UP
	vec3 lightUp = vec3::cross(lightRight,directionToLight).normalized();

	std::uniform_int_distribution<uint64_t> randomPermutation;
	uint64_t permutation = randomPermutation(generator);

	Ray lightRay;
	RaycastHit hitShadow;

	

	for (uint32_t y = 0; y < _options->shadowSamples; ++y)
	{
		for (uint32_t x = 0; x < _options->shadowSamples; ++x)
		{
			vec2 jitter = getDiscSample(x, y, _options->shadowSamples, _options->shadowSamples, permutation, generator) * 1;
			vec3 newLightPosition = light.position + jitter.x * lightRight + jitter.y * lightUp;
			vec3 newDirectionToLight = (newLightPosition - hit.point).normalized();

			lightRay.direction = newDirectionToLight;
			lightRay.origin = hit.point - newLightPosition * _options->bias;// < 0) 
				/*?
				vec3::dot(ray.direction, hit.normal)
				hit.point + newLightPosition  * _options->bias :
				;*/
			hitShadow.distance = Infinity;
			hitShadow.object = nullptr;
			if (trace(lightRay, hitShadow) && hitShadow.object != hit.object)  // && hitShadow.distance < lightDistance / 2;
			{
				shadowAmount += 1.0;
			}
		}

	}

	return shadowAmount / (double(_options->shadowSamples) * double(_options->shadowSamples));
}


uint64_t permute(uint64_t i, uint64_t l, uint64_t p)
	{
		uint64_t w = l - 1;

		w |= w >> 1;
		w |= w >> 2;
		w |= w >> 4;
		w |= w >> 8;
		w |= w >> 16;

		do
		{
			i ^= p;
			i *= 0xe170893d;
			i ^= p >> 16;
			i ^= (i & w) >> 4;
			i ^= p >> 8;
			i *= 0x0929eb3f;
			i ^= p >> 23;
			i ^= (i & w) >> 1;
			i *= 1 | p >> 27;
			i *= 0x6935fa69;
			i ^= (i & w) >> 11;
			i *= 0x74dcb303;
			i ^= (i & w) >> 2;
			i *= 0x9e501cc3;
			i ^= (i & w) >> 2;
			i *= 0xc860a3df;
			i &= w;
			i ^= i >> 5;
		} while (i >= l);

		return (i + p) % l;
	}


vec2  Raytracer::getCMJSampler(uint64_t x, uint64_t y, uint64_t nx, uint64_t ny, uint64_t permutation, std::mt19937 & generator)
{
	vec2 result;
	std::uniform_real_distribution<double> randomOffset(0.0, 1.0);

	uint64_t sx = permute(x, nx, permutation * 0x68bc21eb);
	uint64_t sy = permute(y, ny, permutation * 0x02e5be93);

	result.x = (double(x) + (double(sy) + randomOffset(generator)) / double(ny)) / double(nx);
	result.y = (double(y) + (double(sx) + randomOffset(generator)) / double(nx)) / double(ny);

	return result;
}

vec2 Raytracer::getDiscSample(uint64_t x, uint64_t y, uint64_t nx, uint64_t ny, uint64_t permutation, std::mt19937 & generator)
{
	vec2 point = getCMJSampler(x, y, nx, ny, permutation, generator);

	// disc sampler // mapToDisc(point)
	double theta = 2.0 * PI * point.x;
	double r = sqrt(point.y);

	point.x = r * cos(theta);
	point.y = r * sin(theta);

	return point;
}

void Raytracer::clear()
{
	_shadowRays = 0;
	_secondaryRays = 0;
	_aabbTests = 0;
	_intersectionTests = 0;
	_triangleTests = 0;

	if (bvh != nullptr)
	{
		bvh->clear();
		delete bvh;
	}
	bvh = nullptr;
}

/*
bool transmittedDirection(const Vector3f& normal, const Vector3f& incoming,
	float index_n, float index_nt,
	Vector3f& transmitted)
{
	// std::cout << "=== transmittedDirection ===" << endl;
	// std::cout << "index_n: " << index_n << ", index_nt: " << index_nt << endl;
	// std::cout << "normal: ";
	// normal.print();
	// std::cout << "incoming: ";
	// incoming.print();

	float n = (index_n / index_nt);

	float sinT2 = (
		pow(n, 2)
		*
		(1 - pow(Vector3f::dot(incoming, normal), 2))
		);

	// std::cout << "disc: " << disc << endl;

	if (sinT2 > 1.0) {
		// transmitted = mirrorDirection(normal, incoming);
		transmitted = Vector3f(0, 0, 0);
		return false;
	}

	float cotT = sqrtf(1 - sinT2);

	transmitted = (n
		* (incoming - normal * Vector3f::dot(incoming, normal))
		- (normal * cotT)).normalized();

	// std::cout << "transmitted dir: ";
	// transmitted.print();

	return true;

}*/
