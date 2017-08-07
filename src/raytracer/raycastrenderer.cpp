
#include "raytracer\raycastrender.h"
#include "math/maths.h"
#include "math/matrixtransform.h"
#include <iostream>
#include "utils\systeminfo.h"
#include "image\imagecolors.h"
#include "image\ImageOperators.h"
#include <thread>
#include "image\Histrogram.hpp"



void RaycastRender::reset()
{
	_primaryRays = 0;

	if (output || output != nullptr)
		delete output;
	if (depth || depth != nullptr)
		delete depth;
	if (normal || normal != nullptr)
		delete normal;
	if (edges || edges != nullptr)
		delete edges;

	output = nullptr;
	depth = nullptr;
	normal = nullptr;
	edges = nullptr;
	distances.clear();
	float tmin = 0, tmax = 0;
}

void RaycastRender::end()
{
	_raytracer->countVariables();
}

Image * RaycastRender::draw() {


	/*Image * output = nullptr;
	Image * normal = nullptr;
	Image * depth = nullptr;*/

	if (_hasBeenInitialized)
	{

		output = new Image(_width, _height);
		normal = new Image(_width, _height);
		depth = new Image(_width, _height);
		

		Ray ray;		
		const mat4 & view = _view;
		ray.origin = MatrixTransform::multiplyVec(view, vec3(0));
		float z = -1; 

		float x = 0;
		float y = 0;


		Color color = Color::clear;

		for (int j = 0; j < _height; j++) {
			for (int i = 0; i < _width; i++) {

				
				//float x = ((float)i + 0.5f) / (float)_width;
				//float y = ((float)j + 0.5f) / (float)_height;
				//ray = _camera->generateRay(x, y);

				RaycastHit hit;

				// generate ray direction
				if (_options->antiAliasSamples < 2) {

					// generate ray direction
					x = (2.0f * (i + 0.5f) / (float)_width - 1.0f) * _fov_x;
					y = (1.0f - 2.0f * (j + 0.5f) / (float)_height) * _fov_y;

					// gererante primary ray
					// set ray direction based in the current pixel				
					ray.direction = vec3(x, y, z);
					ray.direction = MatrixTransform::multiplyPoint(view, ray.direction);
					ray.direction.normalize();

					// cast ray and set  color
					//output->setPixel(i, j, _raytracer->castRay(ray));
					
					_primaryRays++;
					color = _raytracer->castRay(ray, hit, 0);
				
				}
				else 
				{
					// clear color for next sampling
					color = Color::clear;

					// Anti-alising super sampling
					// Using Jittering method
					for (int k = 0; k < _options->antiAliasSamples; k++) {
						for (int l = 0; l < _options->antiAliasSamples; l++) {

							// Unfiform random number in the range 0 to 1
							float e = ((float)rand() / (float)RAND_MAX);

							
							x = (2.0f * (i + (((float)k + e) / (float)_options->antiAliasSamples)) / (float)_width - 1.0f) * _fov_x;
							y = (1.0f - 2.0f * (j + (((float)l + e) / (float)_options->antiAliasSamples)) / (float)_height) * _fov_y;

							ray.direction = vec3(x, y, z);
							ray.direction = MatrixTransform::multiplyPoint(view, ray.direction);
							ray.direction.normalize();

							// cast ray and set pixel color
							_primaryRays++;
							color += _raytracer->castRay(ray,hit, 0);
							
						}
					}

				

					// Divide the accumulated sample values by the number of samples
					float n2 = powf(_options->antiAliasSamples, 2.f); // # of samples
					color = color / n2;
					
					color.a = 1;
				}

				
					if (hit.object != nullptr)
					{
						//Color normal_color = vec4(vec3::abs(hit.normal * 0.5f + vec3(0.5f, 0.5f, 0.5f)), 1.0);
						Color normal_color = vec4(hit.normal * 0.5f + vec3(0.5f, 0.5f, 0.5f), 1.0f); //(abs(normal.x()), abs(normal.y()), abs(normal.z()));
						normal->setPixel(i, j, normal_color);

						/*t = hit.distance;
						t -= _minT;
						t *= 1.f / (_maxT - _minT);
						t = std::abs(t - 1.f);
						//Color depth_color(t,t,t);
						depth->setPixel(i, j, Color(t, t, t,1.0f));*/

						if (hit.distance < tmin)
							tmin = hit.distance;

						if (hit.distance > tmax)
							tmax = hit.distance;

						distances.push_back(std::make_pair(vec2i(i,j), hit.distance));
					}
				

				output->setPixel(i, j, color);

				//_raytracer->_depth = 0;

			}
		}

		computeSobelEdges();
		
			
	}
	
	return output;

}

Image * RaycastRender::drawMultiThread(const int & threads, bool screenSubDivision)
{
	

	if (_hasBeenInitialized)
	{

		output = new Image(_width, _height);
		normal = new Image(_width, _height);
		depth = new Image(_width, _height);

		int parts = threads; // 8
		std::vector<std::thread> threads;
		vec3 origin = MatrixTransform::multiplyVec(_view, vec3(0));

		if (screenSubDivision)
		{
			std::vector<int> bounds = threadBounds(parts, _width, _height);

			for (int i = 0; i < parts - 1; ++i) 
			{
				threads.push_back(std::thread(&RaycastRender::parallelRay, this, output, origin, bounds[i], bounds[i + 1]));
			}

			for (int i = parts - 1; i < parts; ++i) 
			{
				parallelRay(output, origin, bounds[i], bounds[i + 1]);
			}

			
		}
		else
		{
			
			int maxLinesPerThread = _height / parts;
			int tIndex = 0;

			for (; tIndex < parts - 1; tIndex++)
			{
				threads.push_back(std::thread(&RaycastRender::parallelRayEachLine, this, output, origin, parts, tIndex, _height));
			}

			//for (int i = parts - 1; i < parts; ++i)
			//{
			
				parallelRayEachLine(output, origin, parts, tIndex, _height);
			//}

		}

		for (auto &e : threads){
			e.join();
		}

		computeSobelEdges();
	}

	return output;
}

std::vector<int> RaycastRender::threadBounds(const int & nthreads, const int & width, const int & height)
{
	std::vector<int>bnd;
	int mem = height;//width * height;
	int delta = mem / nthreads;
	int reminder = mem % nthreads;
	int N1 = 0, N2 = 0;
	bnd.push_back(N1);
	for (int i = 0; i < nthreads; ++i) {
		N2 = N1 + delta;
		if (i == nthreads - 1)
			N2 += reminder;
		bnd.push_back(N2);
		N1 = N2;
	}
	return bnd;
}

void RaycastRender::parallelRay(Image * out, const vec3 & origin, int top, int bottom)
{
	float x = 0;
	float y = 0;

	
	//float z = -1;
	Ray ray;
	ray.origin = origin;
	Color color = Color::clear;

	for (int j = top; j < bottom; j++) {
		for (int i = 0; i < _width; i++) {

			RaycastHit hit;

			// generate ray direction
			if (_options->antiAliasSamples < 2) {

				x = (2.0f * (i + 0.5f) / (float)_width - 1.0f) * _fov_x;
				y = (1.0f - 2.0f * (j + 0.5f) / (float)_height) * _fov_y;

				// gererante primary ray
				// set ray direction based in the current pixel				
				ray.direction = vec3(x, y, -1);
				ray.direction = MatrixTransform::multiplyPoint(_view, ray.direction);
				ray.direction.normalize();

				// cast ray and set  color
				_primaryRays++;
				
				color = _raytracer->castRay(ray, hit, 0);

			}
			else 
			{

				// clear color for next sampling
				color = Color::clear;

				// Anti-alising super sampling
				// Using Jittering method
				for (int k = 0; k < _options->antiAliasSamples; k++) {
					for (int l = 0; l < _options->antiAliasSamples; l++) {

						// Unfiform random number in the range 0 to 1
						float e = ((float)rand() / (float)RAND_MAX);

						// Hybrid strategy that randomly perturbs a regular grid
						x = (2.0f * (i + (((float)k + e) / (float)_options->antiAliasSamples)) / (float)_width - 1.0f) * _fov_x;
						y = (1.0f - 2.0f * (j + (((float)l + e) / (float)_options->antiAliasSamples)) / (float)_height) * _fov_y;

						ray.direction = vec3(x, y, -1);
						ray.direction = MatrixTransform::multiplyPoint(_view, ray.direction);
						ray.direction.normalize();

						// cast ray and set pixel color
						_primaryRays++;
						color += _raytracer->castRay(ray, hit, 0);

					}
				}

				// Divide the accumulated sample values by the number of samples
				float n2 = powf(_options->antiAliasSamples, 2.f); // # of samples
				color = color / n2;

				color.a = 1;

			}

			if (hit.object != nullptr)
			{
				
				Color normal_color = vec4(hit.normal * 0.5f + vec3(0.5f, 0.5f, 0.5f), 1.0f); 
				normal->setPixel(i, j, normal_color);
				mtx.lock();
				if (hit.distance < tmin)
					tmin = hit.distance;

				if (hit.distance > tmax)
					tmax = hit.distance;

				distances.push_back(std::make_pair(vec2i(i, j), hit.distance));
				mtx.unlock();
			}

			out->setPixel(i, j, color);
		}
	}
}

void RaycastRender::parallelRayEachLine(Image * out, const vec3 & origin, int index, int start, int end)
{
	float x = 0;
	float y = 0;

	Ray ray;
	ray.origin = origin;
	Color color = Color::clear;
	
	//int skip = index + 1;
	int j = start;
	int i;

	for (j = start; j < end; j += index)
	{
		i = 0;

		for (; i < _width; i++) 
		{

			RaycastHit hit;

			// generate ray direction
			if (_options->antiAliasSamples < 2) 
			{

				x = (2.0f * (i + 0.5f) / (float)_width - 1.0f) * _fov_x;
				y = (1.0f - 2.0f * (j + 0.5f) / (float)_height) * _fov_y;

				// gererante primary ray
				// set ray direction based in the current pixel				
				ray.direction = vec3(x, y, -1);
				ray.direction = MatrixTransform::multiplyPoint(_view, ray.direction);
				ray.direction.normalize();

				// cast ray and set  color
				_primaryRays++;

				color = _raytracer->castRay(ray, hit, 0);

			}
			else 
			{

				// clear color for next sampling
				color = Color::clear;

				// Anti-alising super sampling
				// Using Jittering method
				for (int k = 0; k < _options->antiAliasSamples; k++) {
					for (int l = 0; l < _options->antiAliasSamples; l++) {

						// Unfiform random number in the range 0 to 1
						float e = ((float)rand() / (float)RAND_MAX);

						// Hybrid strategy that randomly perturbs a regular grid
						x = (2.0f * (i + (((float)k + e) / (float)_options->antiAliasSamples)) / (float)_width - 1.0f) * _fov_x;
						y = (1.0f - 2.0f * (j + (((float)l + e) / (float)_options->antiAliasSamples)) / (float)_height) * _fov_y;

						ray.direction = vec3(x, y, -1);
						ray.direction = MatrixTransform::multiplyPoint(_view, ray.direction);
						ray.direction.normalize();

						// cast ray and set pixel color
						_primaryRays++;
						color += _raytracer->castRay(ray, hit, 0);

					}
				}

				// Divide the accumulated sample values by the number of samples
				float n2 = powf(_options->antiAliasSamples, 2.f); // # of samples
				color = color / n2;

				color.a = 1;

			}

			if (hit.object != nullptr)
			{

				Color normal_color = vec4(hit.normal * 0.5f + vec3(0.5f, 0.5f, 0.5f), 1.0f);
				normal->setPixel(i, j, normal_color);
				mtx.lock();
				if (hit.distance < tmin)
					tmin = hit.distance;

				if (hit.distance > tmax)
					tmax = hit.distance;

				distances.push_back(std::make_pair(vec2i(i, j), hit.distance));
				mtx.unlock();
			}

			out->setPixel(i, j, color);
		}
	}
}

void  RaycastRender::computeSobelEdges()
{
	for (int i = 0; i < distances.size(); i++) {
		float t = distances[i].second;
		const vec2i & v = distances[i].first;
		/*t -= tmin;
		t *= 1.f / (tmax - tmin);
		t = std::abs(t - 1.f);*/
		t = 1.f - (t / (tmax - tmin));
		depth->setPixel(v.x, v.y, Color(t, t, t, 1.0f));
	}





	if (_options->outlineType > 0) {
		Image * normalSobel = normal->clone();
		Image * depthSobel = depth->clone();
		Image * fulldepth = depth->clone();


		//GaussianBlur * gaussian = new GaussianBlur;
		Sobel sobel(3);
		//Laplacian laplace(3);
		SmoothBlur smoth(3);

		//gaussian->apply(normal, normal);
		//gaussian->apply(depth, depth);


		ImageColors::threshold(fulldepth, 1);

		// extract external borders
		ImageOperator::bitwise_and(depthSobel, normalSobel);
		//ImageOperator::bitwise_xor(normalSobel, depthSobel); // internal borders
		ImageColors::contrastBrightnes(depthSobel, 10, 0);

		if (_options->outlineType == 1) { // full borders

			
			EdgeExtractor e(3);
			sobel.apply(fulldepth, fulldepth, 1, true);
			e.apply(normal, normalSobel, 3, true);
			
			ImageOperator::extractLuminance(normalSobel, normalSobel, 75, 255);
			ImageColors::grayscale(normalSobel, ImageColors::HSP);
			ImageColors::threshold(normalSobel, 5);
			ImageOperator::bitwise_or(normalSobel, fulldepth);

			smoth.apply(normalSobel, normalSobel);

			ImageOperator::bitwise_or_mask(output, normalSobel, Color::white);

			edges = normalSobel;

			// internal borders			
			/*
			Image * secdepth = depthSobel->clone();

			ImageColors::threshold(secdepth, 1);
			ImageColors::grayscale(depthSobel, ImageColors::HSP);
			
			EdgeExtractor e(3);
			e.apply(normalSobel, normalSobel, 3);
			ImageColors::contrastBrightnes(normalSobel, 127, 116);
			ImageColors::grayscale(normalSobel, ImageColors::Luma);
			ImageOperator::bitwise_or(normalSobel, secdepth);
			ImageOperator::extractLuminance(normalSobel, normalSobel, 50, 255);*/

			// final borders
			/*ImageColors::threshold(normalSobel, 1);
			ImageColors::threshold(depthSobel, 1);
			ImageOperator::add(normalSobel, depthSobel);
			ImageOperator::swapBinaryColor(normalSobel, Color(1, 1, 1));*/

			//SmoothBlur smoth(3);
			//smoth.divisor = 2;
			//smoth.apply(normalSobel, normalSobel);

			

			//ImageColors::blend(output, normalSobel);
			//ImageOperator::bitwise_left_shift(output, normalSobel);

			delete depthSobel;
			//delete secdepth;
			delete fulldepth;
		}
		else
		{
			
			/*ImageColors::grayscale(depthSobel, ImageColors::MaxDecomposition);

			float min = 0, max = 0;
			ImageOperator::minMaxLuminance(depthSobel, min, max, 25, 255);

			if (min < 25)
				min = 25;

			ImageOperator::extractLuminance(depthSobel, depthSobel, min, max);
			ImageColors::threshold(depthSobel, 1);*/
	
			sobel.apply(fulldepth, fulldepth, 1, true);

			
			smoth.divisor = 2;
			smoth.apply(fulldepth, fulldepth);
			//ImageOperator::swapBinaryColor(fulldepth, Color(1, 1, 1));

			//ImageColors::blend(output, fulldepth);
			//ImageOperator::bitwise_and(output, fulldepth);
			ImageOperator::bitwise_or_mask(output, fulldepth, Color::white);


			edges = fulldepth;
			delete depthSobel;
			delete normalSobel;
			//delete fulldepth;
		}


	}

	
}