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
* @note This is the same as memsetting to 0 and setting the base padding to 0x18000000, the default value.
*/
void BufInfo_Init(C3D_BufInfo* info);

/*
* @brief Adds a buffer to a BufInfo struct.
* @param[out] info Pointer to a BufInfo struct.
* @param[in] data Pointer to the buffer to be added.
* @param[in] stride Pointer difference between a single attribute. Usually, one can just input sizeof(my_attribute_struct).
* @param[in] attribCount How many vertex attributes are in this buffer.
* @param[in] permutation Determines the order of attributes in the buffer. Using this, one can rearrange the\
* positions of attributes in the struct by marking the attribute positions in reverse order.\
* For example, with three attributes in this buffer, setting this to 0x210 would configure the permutation to be the first attribute,\
* then the second, then the third. 0x120 would configure it for the second attribute first, then the third, then the first.
*/
int  BufInfo_Add(C3D_BufInfo* info, const void* data, ptrdiff_t stride, int attribCount, u64 permutation);

/*
* @brief Returns a pointer to the current BufInfo struct.
* @return A pointer to the current BufInfo struct.
*/
C3D_BufInfo* C3D_GetBufInfo(void);

/*
* @brief Sets the current BufInfo struct.
* @param[in] info Pointer to the BufInfo struct to set.
*/
void C3D_SetBufInfo(C3D_BufInfo* info);
