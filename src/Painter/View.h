#pragma once

#include "Prerequisites.h"

class CView
{
private:
	glm::ivec2 m_iMoving;
	glm::ivec2 m_iOffset;

	float m_fScale; // @ zoom

	bool m_bMoving;

public:
	CView();
	~CView();

	void Pulse();

	glm::ivec2 GetOffset() { return m_iOffset; }

	float GetScale() { return m_fScale; }
	void SetScale(float y);

	bool isMoving() { return m_bMoving; }

	void ResetView();


	static CView *Get() { return s_pInst; }

private:
	static CView *s_pInst;
};