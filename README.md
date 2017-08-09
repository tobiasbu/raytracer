# raytracer
![Alt text](https://raw.githubusercontent.com/tobiasbu/raytracer/master/resources/raytracer_icon.png)
Simple Raytracer for Advanced Computer Graphics Techniques class 2017/1.

Visit my website for more screen-shots.

## Releases

Dowload the latest release:

| Windows 32-bit |
| -------------- |
| Link |

You can build the projet using CMake. The project was tested only on MSVC 2013 and 2017.
The project has the following dependencies: [freeglut](http://freeglut.sourceforge.net/) and [glew](http://glew.sourceforge.net/).

## Features

- Rendered scene preview and editing tools
    - Apply Kernel Convulation filters
    - Color tools
    - Scene Normal map, depth map and edges detection preview
- Scene file Loader
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
- Filters (Kernel Convolution)
    - Laplacian, Sharpen, Unsharp-Mask, Box-Blur, Motion-Blur, Smooth-Blur, Gaussian Blur and Emboss
- Silhouette Detection using Sobel Filter
    - Using scene normal map and depth map
    - Detects only external or full edges detection
- Image tools
    - Posterize, Brightness and Contrast, Grayscale, Threshold and Invert colors
- Multi-thread raytracing 
    - Screen space division or Each line one thread

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