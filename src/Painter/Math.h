#pragma once
#include "Prerequisites.h"

class CMath
{
public:
	static float PointDir(glm::vec2 pos1, glm::vec2 pos2);
	static bool inRange(int value, int min, int max);
	static glm::vec2 LengthDir(float len, float dir);
};

