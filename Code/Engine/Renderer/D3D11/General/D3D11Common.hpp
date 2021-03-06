#pragma once

enum eD3D11BlendMode {
	BLEND_MODE_ZERO				= 1,
	BLEND_MODE_ONE				= 2,
	BLEND_MODE_SRC_COLOR		= 3,
	BLEND_MODE_INV_SRC_COLOR	= 4,
	BLEND_MODE_SRC_ALPHA		= 5,
	BLEND_MODE_INV_SRC_ALPHA	= 6,
	BLEND_MODE_DEST_ALPHA		= 7,
	BLEND_MODE_INV_DEST_ALPHA	= 8,
	BLEND_MODE_DEST_COLOR		= 9,
	BLEND_MODE_INV_DEST_COLOR	= 10,
	BLEND_MODE_SRC_ALPHA_SAT	= 11,
	BLEND_MODE_BLEND_FACTOR		= 14,
	BLEND_MODE_INV_BLEND_FACTOR = 15,
	BLEND_MODE_SRC1_COLOR		= 16,
	BLEND_MODE_INV_SRC1_COLOR	= 17,
	BLEND_MODE_SRC1_ALPHA		= 18,
	BLEND_MODE_INV_SRC1_ALPHA	= 19
};

enum eD3D11BlendOp {
	BLEND_OP_ADD				= 1,
	BLEND_OP_SUBTRACT			= 2,
	BLEND_OP_REV_SUBTRACT		= 3,
	BLEND_OP_MIN				= 4,
	BLEND_OP_MAX				= 5
};

enum eColorWrite {
	COLOR_WRITE_ENABLE_RED		= 1,
	COLOR_WRITE_ENABLE_GREEN	= 2,
	COLOR_WRITE_ENABLE_BLUE		= 4,
	COLOR_WRITE_ENABLE_ALPHA	= 8,
	COLOR_WRITE_ENABLE_ALL		= (((D3D11_COLOR_WRITE_ENABLE_RED | 
									D3D11_COLOR_WRITE_ENABLE_GREEN) | D3D11_COLOR_WRITE_ENABLE_BLUE) | D3D11_COLOR_WRITE_ENABLE_ALPHA)
};