
#ifndef _MATHSDEF_H_
#define _MATHSDEF_H_

#include "config.hpp"

#include <limits>

#define PI 3.14159265359f
#define TAU 6.28318530718f // 2.0 * PI
#define DEG_TO_RAD 0.01745329251f //PI / 180.0 // 0.0174533 - 0.01745329251
#define RAD_TO_DEG 57.2957795f //180.0 / PI //57.2957795 // 360.0 / (2.0 * M_PI)

#ifdef _MSC_VER
#define MaxFloat std::numeric_limits<float>::max()
#define Infinity std::numeric_limits<float>::infinity()
#else
static PBRT_CONSTEXPR Float MaxFloat = std::numeric_limits<Float>::max();
static PBRT_CONSTEXPR Float Infinity = std::numeric_limits<Float>::infinity();
#endif

#ifdef _MSC_VER
#define MachineEpsilon (std::numeric_limits<float>::epsilon() * 0.5)
#else
static PBRT_CONSTEXPR Float MachineEpsilon =
std::numeric_limits<Float>::epsilon() * 0.5;
#endif

static GGE_CONSTEXPR float Epsilon = 1e-8f;

#endif