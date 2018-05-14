/*
* @file bufinfo.h
* @brief Functions to configure vertex array buffers.
*/

#pragma once
#include "types.h"

typedef struct
{
	u32 offset;
	u32 flags[2];
} C3D_BufCfg;

typedef struct
{
	u32 base_paddr;
	int bufCount;
	C3D_BufCfg buffers[12];
} C3D_BufInfo;

/*
* @brief Initialize a buffer to its default state.
*/
void BufInfo_Init(C3D_BufInfo* info);

/*
* @brief Adds a buffer to a BufInfo struct.
* @param[out] info Pointer to a \ref C3D_BufInfo struct.
* @param[in] data Pointer to buffer.
* @param[in] stride Distance in bytes between vertex entries in the buffer (usually set to the size of the vertex structure).
* @param[in] attribCount Number of attributes to load from this buffer.
* @param[in] permutation Specifies the order of attributes in the buffer. Each attribute index is a nibble (4 bits), and they are ordered from least significant to most significant.\
* Padding can be specified with 0xC, 0xD, 0xE and 0xF. (See https://3dbrew.org/wiki/GPU/Internal_Registers#GPUREG_ATTRIBBUFFERi_CONFIG1 for more info)
* @remark Using this, one can rearrange the positions of attributes in the struct.\
* For example, with three attributes in this buffer, setting this to 0x210 would configure the permutation to be the first attribute,\
* then the second, then the third. 0x120 would set for the second attribute first, then the third, then the first.
*/
int  BufInfo_Add(C3D_BufInfo* info, const void* data, ptrdiff_t stride, int attribCount, u64 permutation);

/*
* @brief Returns a pointer to the \ref C3D_BufInfo citro3d is currently using.
* @return Pointer to the BufInfo struct in use.
*/
C3D_BufInfo* C3D_GetBufInfo(void);

/*
* @brief Sets the \ref C3D_BufInfo in use.
* @param[in] info Pointer to the BufInfo struct to use.
*/
void C3D_SetBufInfo(C3D_BufInfo* info);
