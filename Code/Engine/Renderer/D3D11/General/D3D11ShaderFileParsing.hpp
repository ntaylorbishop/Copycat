#pragma once

#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"

extern bool		D3D11_BOOL_FALSE;
extern bool		D3D11_BOOL_TRUE;
extern float	D3D11_FLOAT_ZERO;
extern double	DOUBLE_ZERO;
extern int		INT_ZERO;
extern uint		UINT_ZERO;
extern short	SHORT_ZERO;

void ParseInAllShaderData();
void ParseInAllMaterialData();
void ParseInDefaultConstantBuffers();