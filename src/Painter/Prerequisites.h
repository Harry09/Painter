#pragma once
#define _USE_MATH_DEFINES

#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <GLFW\glfw3.h>
#include <GLFW\glfw3native.h>

#include <FreeImage.h>

#include <Windows.h>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <Commctrl.h>

#include <gl\GLU.h>

#include "resource.h"

// struktura uchar
// Twórcy na to nie wpadli
typedef glm::highp_cvec3 cvec3;

class CClient;
class CRenderer;
class CView;
class CImageMgr;
class CImage;
class CCursor;
class CKeyboard;

class CSettings;
class CMenu;


bool FileExists(char* name);
const wchar_t *getExt(const wchar_t *fspec);