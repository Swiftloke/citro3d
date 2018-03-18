/*
* @file texenv.h
* @brief Functions to configure the PICA200's "combiner stages", AKA TexEnv.
* @note See the citro3D User Manual for more information.
*/

#pragma once
#include "types.h"

/*
* @brief Contains entire information about a single TexEnv stage.
* @note Usually one shouldn't modify this struct directly- instead the C3D_TexEnv* functions should be used.
*/
typedef struct
{
	u16 srcRgb, srcAlpha; ///<Source values for the color and alpha of this stage.
	union
	{
		u32 opAll; ///<Contains information on the operations to perform on this stage.
		struct 
		{ 
			u32 opRgb:12,  ///<Color operations.
				opAlpha:12;///<Alpha operations.
		};
	};
	u16 funcRgb, funcAlpha; ///<Functions to perform on the sources of this stage.
	u32 color; ///<Constant value for this stage.
	u16 scaleRgb, scaleAlpha; ///<Scale factor of the color and alpha of this stage.
} C3D_TexEnv;

///Color, alpha, or both.
typedef enum
{
	C3D_RGB = BIT(0), ///<Color.
	C3D_Alpha = BIT(1), ///<Alpha.
	C3D_Both = C3D_RGB | C3D_Alpha, ///<Both color and alpha.
} C3D_TexEnvMode;

/*
* @brief Gets the TexEnv for a given stage.
* @param[in] id Stage to return.
* @return TexEnv of the given stage.
*/
C3D_TexEnv* C3D_GetTexEnv(int id);

/*
* @brief Sets the TexEnv for a given stage.
* @param[in] id Stage to set.
* @param[in] env TexEnv to apply.
*/
void C3D_SetTexEnv(int id, C3D_TexEnv* env);

/*
* @brief Marks a TexEnv as needing to be updated.
* @note One must use this if they are continuing to use a TexEnv pointer they got from GetTexEnv after performing an action that flushes state.
* @param[in] env Pointer to a TexEnv struct.
*/
void C3D_DirtyTexEnv(C3D_TexEnv* env);

/*
* @brief Configures the stages where the GPU_PREVIOUS_BUFFER source value should be updated with the output of that stage.
* @note See the citro3D User Manual for more information.
* @param[in] mode TexEnv update modes (see \ref C3D_TexEnvMode)
* @param[in] mask Bitmask containing which stages update GPU_PREVIOUS_BUFFER (can be built using GPU_TEV_BUFFER_WRITE_CONFIG)
*/
void C3D_TexEnvBufUpdate(int mode, int mask);

/*
* @brief Configure the initial value of GPU_PREVIOUS_BUFFER. This value will be kept until it is updated; see \ref C3D_TexEnvBufUpdate.
* @param[in] RGBA color value to write.
*/
void C3D_TexEnvBufColor(u32 color);

/*
* @brief Sets/resets a TexEnv to its initial value.
* @param[out] env TexEnv to initialize.
* This is the same as performing the following:
* @code C3D_TexEnvSrc(env, C3D_Both, GPU_PREVIOUS, 0, 0);
* C3D_TexEnvOpRgb(env, GPU_TEVOP_RGB_SRC_COLOR, 0, 0);
* C3D_TexEnvOpA(env, GPU_TEVOP_A_SRC_ALPHA, 0, 0);
* C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
* C3D_TexEnvColor(env, 0xFFFFFFFF);
* C3D_TexEnvScale(env, C3D_Both, GPU_TEVSCALE_1);@endcode
*/
static inline void C3D_TexEnvInit(C3D_TexEnv* env)
{
	env->srcRgb     = GPU_TEVSOURCES(GPU_PREVIOUS, 0, 0);
	env->srcAlpha   = env->srcRgb;
	env->opAll      = 0;
	env->funcRgb    = GPU_REPLACE;
	env->funcAlpha  = env->funcRgb;
	env->color      = 0xFFFFFFFF;
	env->scaleRgb   = GPU_TEVSCALE_1;
	env->scaleAlpha = GPU_TEVSCALE_1;
}

#ifdef __cplusplus
#define _C3D_DEFAULT(x) = x
#else
#define _C3D_DEFAULT(x)
#endif

C3D_DEPRECATED static inline void TexEnv_Init(C3D_TexEnv* env)
{
	C3D_TexEnvInit(env);
}

/*
@brief Sets the input source of a TexEnv.
@param[out] env Pointer to TexEnv struct.
@param[in] mode TexEnv update modes (see \ref C3D_TexEnvMode)
@param[in] s1 First source.
@param[in] s2 Second source.
@param[in] s3 Third source.
*/
static inline void C3D_TexEnvSrc(C3D_TexEnv* env, C3D_TexEnvMode mode,
	GPU_TEVSRC s1,
	GPU_TEVSRC s2 _C3D_DEFAULT(GPU_PRIMARY_COLOR),
	GPU_TEVSRC s3 _C3D_DEFAULT(GPU_PRIMARY_COLOR))
{
	int param = GPU_TEVSOURCES((int)s1, (int)s2, (int)s3);
	if ((int)mode & C3D_RGB)
		env->srcRgb = param;
	if ((int)mode & C3D_Alpha)
		env->srcAlpha = param;
}

C3D_DEPRECATED static inline void C3D_TexEnvOp(C3D_TexEnv* env, C3D_TexEnvMode mode, int o1, int o2, int o3)
{
	int param = GPU_TEVOPERANDS(o1, o2, o3);
	if ((int)mode & C3D_RGB)
		env->opRgb = param;
	if ((int)mode & C3D_Alpha)
		env->opAlpha = param;
}

/*
* @brief Configures the operation to be applied to the input color of a TexEnv before the function is applied.
* @param[out] env Pointer to TexEnv struct.
* @param[in] o1 Operation to perform on the first source.
* @param[in] o2 Operation to perform on the second source.
* @param[in] o3 Operation to perform on the third source.
*/
static inline void C3D_TexEnvOpRgb(C3D_TexEnv* env,
	GPU_TEVOP_RGB o1,
	GPU_TEVOP_RGB o2 _C3D_DEFAULT(GPU_TEVOP_RGB_SRC_COLOR),
	GPU_TEVOP_RGB o3 _C3D_DEFAULT(GPU_TEVOP_RGB_SRC_COLOR))
{
	env->opRgb = GPU_TEVOPERANDS((int)o1, (int)o2, (int)o3);
}

/*
* @brief Configures the operation to be applied to the input alpha of a TexEnv before the function is applied.
* @param[out] env Pointer to TexEnv struct.
* @param[in] o1 Operation to perform on the first source.
* @param[in] o2 Operation to perform on the second source.
* @param[in] o3 Operation to perform on the third source.
*/
static inline void C3D_TexEnvOpAlpha(C3D_TexEnv* env,
	GPU_TEVOP_A o1,
	GPU_TEVOP_A o2 _C3D_DEFAULT(GPU_TEVOP_A_SRC_ALPHA),
	GPU_TEVOP_A o3 _C3D_DEFAULT(GPU_TEVOP_A_SRC_ALPHA))
{
	env->opAlpha = GPU_TEVOPERANDS((int)o1, (int)o2, (int)o3);
}

/*
* @brief Sets the combiner function to perform in this TexEnv.
* @param[out] env Pointer to TexEnv struct.
* @param[in] mode TexEnv update modes (see \ref C3D_TexEnvMode)
* @param[in] param Function to use.
*/
static inline void C3D_TexEnvFunc(C3D_TexEnv* env, C3D_TexEnvMode mode, GPU_COMBINEFUNC param)
{
	if ((int)mode & C3D_RGB)
		env->funcRgb = param;
	if ((int)mode & C3D_Alpha)
		env->funcAlpha = param;
}

/*
* @brief Sets the value of the GPU_CONSTANT source for a TexEnv stage.
* @param[out] env Pointer to TexEnv struct.
* @param[in] color RGBA color value to apply.
*/
static inline void C3D_TexEnvColor(C3D_TexEnv* env, u32 color)
{
	env->color = color;
}

/*
* @brief Configures the scaling to be applied to the output of a TexEnv.
* @param[out] env Pointer to TexEnv struct.
* @param[in] mode TexEnv update modes (see \ref C3D_TexEnvMode)
* @param[in] param Scale factor to apply.
*/
static inline void C3D_TexEnvScale(C3D_TexEnv* env, int mode, GPU_TEVSCALE param)
{
	if (mode & C3D_RGB)
		env->scaleRgb = param;
	if (mode & C3D_Alpha)
		env->scaleAlpha = param;
}

#undef _C3D_DEFAULT
