#include "Math.h"


float CMath::PointDir(glm::vec2 pos1, glm::vec2 pos2)
{
	return float(atan2((pos1.y - pos2.y), (pos1.x - pos2.x)) * 180 / M_PI + 180);
}

bool CMath::inRange(int value, int min, int max)
{
	return value >= min && value <= max;
}

glm::vec2 CMath::LengthDir(float len, float dir)
{
	glm::vec2 _pos;

	_pos.x = (float)cos(dir * M_PI / 180) * len;
	_pos.y = (float)-sin(dir * M_PI / 180) * len;

	return _pos;
}

float CMath::GetLen(glm::vec2 pos1, glm::vec2 pos2)
{
	float len = 0.f;

	float cx = 0.f, cy = 0.f;

	cx = pos2.x - pos1.x;
	cy = pos2.y - pos1.y;

	len = sqrt(cx*cx + cy*cy);

	return len;
}

