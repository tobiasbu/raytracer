# raytracer
![Alt text](https://raw.githubusercontent.com/tobiasbu/raytracer/master/resources/raytracer_icon128x.png)

Simple Raytracer for Advanced Computer Graphics Techniques class 2017/1. Written on C++11/14.

<img src="https://raw.githubusercontent.com/tobiasbu/raytracer/master/resources/raytracer0.png" width=33% style="display:inline-block; margin-left: 4px;"> <img src="https://raw.githubusercontent.com/tobiasbu/raytracer/master/resources/raytracer1.png" width=33% style="display:inline-block; margin-left: 4px;"> <img src="https://raw.githubusercontent.com/tobiasbu/raytracer/master/resources/raytracer2.png" width=33% style="display:inline-block;">



[Visit the website](https://tobiasbu.github.io/portfolio/raytracer/) for more screen-shots and details.

## Releases

Dowload the latest release:

| Windows - 25.75 MB |
| ------------------ |
| [tobiasbu-raytracer-1.0.0-win.zip](https://github.com/tobiasbu/raytracer/releases/download/v1.2.0/tobiasbu-raytracer-1.0.0-win.zip) |

## Features

- Rendered scene preview and editing tools
    - Apply Kernel Convulation filters
    - Color tools
    - Scene Normal map, Depth map and Edges Detection previews
- Scene file loader
- Basic Raytracer Architeture (Whitted-style) 
    - Fresnel: Reflection and Refraction
    - Point and Directional Light
    - Phong, Cel-Shading, Gooch and Two-Tones
    - Hard-shadows, Soft-Shadows (area light sampling) and fake soft-shadows
    - Anti-Aliasing (jittering method)
- Primitives
    - Plane
    - Sphere
    - Triangle
    - Mesh
    - Model Loader for Wavefront *.OBJ Loader
    - Supports Transforms
- Bounding volume hierarchy (BVH) - based on pbrt-v3
    - Surface Area Heuristics - SAH 
    - Can contain any primitives
- Texture Mapping
    - Availble Sphere, Triangle and Plane UVs
    - Normal mapping
    - Billinear and Point filter
    - Cube-map
- Kernel Convolution Filters
    - Laplacian, Sharpen, Unsharp-Mask, Box-Blur, Motion-Blur, Smooth-Blur, Gaussian Blur and Emboss
- Silhouette Detection using Sobel Filter
    - Using scene normal map and depth map
    - Detects only external or full edges detection
- Image tools
    - Posterize, Brightness and Contrast, Grayscale, Threshold and Invert colors
- Multi-thread raytracing 
    - Screen space division or Each line one thread

## Building

You can build the projet using CMake. The project was tested only on MSVC 2013 and 2017. 
Note: the code is a big mess! :S Any improvement is very welcomed.

The project has the following dependencies: [freeglut](http://freeglut.sourceforge.net/) and [glew](http://glew.sourceforge.net/).

## Code and References

Some Links and other user Codes that helped me during development:

- [Scrathpixel 2.0](http://www.scratchapixel.com/)
- [Computer Graphics Guide](https://computergraphicsguide.blogspot.com.br/)
- [Bounding Volume Hierarchy](http://fileadmin.cs.lth.se/cs/Education/EDAN30/lectures/S2-bvh.pdf)
- [Glavin001 / Raytracer](https://github.com/Glavin001/Raytracer)
- [flipcode](http://www.flipcode.com/)
- [pbrt-v3](https://github.com/mmp/pbrt-v3)
- [raycer](https://github.com/mikoro/raycer)
- [Rossetacode / Image convolution](https://rosettacode.org/wiki/Image_convolution)
- [songho.ca](http://www.songho.ca/index.html)
- [Sun & Black Cat](http://sunandblackcat.com/index.php?l=eng)
- [tinyobjloader](https://github.com/syoyo/tinyobjloader)
- [EuclideanSpace](http://www.euclideanspace.com/)

## Data Sources

Websites of data and resources sources: 

- [The Stanford 3D Scanning Repository](http://graphics.stanford.edu/data/3Dscanrep/)
- [OpenGameArt.org](https://opengameart.org/)
- [John Burkardt](http://people.sc.fsu.edu/~jburkardt/index.html)
- [Bump-map a sphere with a texture map](https://stackoverflow.com/questions/32550498/bump-map-a-sphere-with-a-texture-map)

## License

    The MIT License (MIT)

    Simple Raytracer
    Copyright (c) 2017 Tobias Ulrich

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.