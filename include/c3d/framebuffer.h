#pragma once
#include "texture.h"

/*
* @brief Contains information about a framebuffer.
* @note Usually one shouldn't modify this struct directly- the C3D_FrameBuf* functions should be used instead.
*/
typedef struct
{
	void* colorBuf; ///<Color buffer.
	void* depthBuf; ///<Depth (and optionally stencil) buffer.
	u16 width;  ///<Width of the framebuffer.
	u16 height; ///<Height of the framebuffer.
	GPU_COLORBUF colorFmt; ///<Format \ref GPU_COLORBUF of the framebuffer.
	GPU_DEPTHBUF depthFmt; ///<Format \ref GPU_DEPTHBUF of the depth/stencil buffer.
	bool block32; ///<?
	u8 colorMask : 4; ///<?
	u8 depthMask : 4; ///<?
} C3D_FrameBuf;

/*
* @brief Flags for C3D_FrameBufClear.
*/
typedef enum
{
	C3D_CLEAR_COLOR = BIT(0), ///<Clear the color buffer.
	C3D_CLEAR_DEPTH = BIT(1), ///<Clear the depth/stencil buffer.
	C3D_CLEAR_ALL   = C3D_CLEAR_COLOR | C3D_CLEAR_DEPTH, ///<Clear both buffers.
} C3D_ClearBits;

/*
* @brief Calculates the size of a color buffer.
* @param[in] width Width of the color buffer.
* @param[in] height Height of the color buffer.
* @param[in] fmt Format of the color buffer.
* @return Size of a color buffer with these properties.
*/
u32 C3D_CalcColorBufSize(u32 width, u32 height, GPU_COLORBUF fmt);

/*
* @brief Calculates the size of a depth buffer.
* @param[in] width Width of the depth buffer.
* @param[in] height Height of the depth buffer.
* @param[in] fmt Format of the depth buffer.
* @return Size of a depth buffer with these properties.
*/
u32 C3D_CalcDepthBufSize(u32 width, u32 height, GPU_DEPTHBUF fmt);

/*
* @brief Returns the currently bound framebuffer.
* @return Pointer to \ref C3D_FrameBuf struct.
*/
C3D_FrameBuf* C3D_GetFrameBuf(void);

/*
* @brief Binds a framebuffer to be drawn on.
* @param[in] fb Pointer to \ref C3D_FrameBuf struct.
*/
void C3D_SetFrameBuf(C3D_FrameBuf* fb);

/*
* @brief Binds a texture to a framebuffer. This texture will be used as the color buffer.
* @param[out] fb Pointer to \ref C3D_FrameBuf struct.
* @param[in] tex Pointer to \ref C3D_Tex struct.
* @param[in] face Face of the texture to use if it is a cubemap (ignored if it is a 2D texture)
* @param[in] level Mipmap level. 0 is the original image, 1 is the first mipmap, and so on.
* @remark This calls \ref C3D_FrameBufColor with the proper arguments for the buffer of the texture.
*/
void C3D_FrameBufTex(C3D_FrameBuf* fb, C3D_Tex* tex, GPU_TEXFACE face, int level);

/*
* @brief Sets the clear bits and color for a framebuffer.
* @param[in] fb Pointer to a \ref C3D_FrameBuf struct.
* @param[in] clearBits Which buffers to clear (see \ref C3D_ClearBits)
* @param[in] clearColor Color to clear the color buffer with.
* @param[in] clearDepth Color to clear the depth buffer with.
*/
void C3D_FrameBufClear(C3D_FrameBuf* fb, C3D_ClearBits clearBits, u32 clearColor, u32 clearDepth);

/*
* @brief Transfers a framebuffer to the LCD display.
* @param[in] fb Pointer to a \ref C3D_FrameBuf struct.
* @param[in] screen Screen to transfer the framebuffer to.
* @param[in] side Side of the screen to transfer the framebuffer to (unused for the bottom screen)
*/
void C3D_FrameBufTransfer(C3D_FrameBuf* fb, gfxScreen_t screen, gfx3dSide_t side, u32 transferFlags);

/*

*/
static inline void C3D_FrameBufAttrib(C3D_FrameBuf* fb, u16 width, u16 height, bool block32)
{
	fb->width   = width;
	fb->height  = height;
	fb->block32 = block32;
}

/*
* @brief Assigns a color buffer to a framebuffer.
* @param[in] fb Pointer to a \ref C3D_FrameBuf struct.
* @param[in] buf Pointer to the buffer to use.
* @param[in] fmt Format of the color buffer.
*/
static inline void C3D_FrameBufColor(C3D_FrameBuf* fb, void* buf, GPU_COLORBUF fmt)
{
	if (buf)
	{
		fb->colorBuf  = buf;
		fb->colorFmt  = fmt;
		fb->colorMask = 0xF;
	} else
	{
		fb->colorBuf  = NULL;
		fb->colorFmt  = GPU_RB_RGBA8;
		fb->colorMask = 0;
	}
}

/*
* @brief Assigns a depth buffer to a framebuffer.
* @param[in] fb Pointer to a \ref C3D_FrameBuf struct.
* @param[in] buf Pointer to the buffer to use.
* @param[in] fmt Format of the depth buffer. Based on the format, this may be used as a stencil buffer as well.
*/
static inline void C3D_FrameBufDepth(C3D_FrameBuf* fb, void* buf, GPU_DEPTHBUF fmt)
{
	if (buf)
	{
		fb->depthBuf  = buf;
		fb->depthFmt  = fmt;
		fb->depthMask = fmt == GPU_RB_DEPTH24_STENCIL8 ? 0x3 : 0x2;
	} else
	{
		fb->depthBuf  = NULL;
		fb->depthFmt  = GPU_RB_DEPTH24;
		fb->depthMask = 0;
	}
}
