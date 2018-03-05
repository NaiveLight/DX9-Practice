#pragma once

struct MYVERTEX
{
	float x, y, z, rhw;
	unsigned long color;
};

struct MYGEOVERTEX
{
	float x, y, z;
	unsigned long color;
};

struct MY3DVERTEX
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
};

