#pragma once
#include "Prerequisites.h"

class CMath
{
public:
	static float PointDir(const glm::vec2 &_pos1, const glm::vec2 &_pos2);
	static bool inRange(int _value, int _min, int _max);
	static glm::vec2 LengthDir(float _len, float _dir);
	// Lenght between two pos
	static float GetLen(const glm::vec2 &_pos1, const glm::vec2 &_pos2);
};

