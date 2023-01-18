#pragma once

#include <lib.hpp>

#define _FUNC_PREFIX(name)  \
    shimmer::func_ptrs::name

#define _FUNC_TYPE(name)    \
    APPEND(_FUNC_PREFIX(name), _t)

#define _FUNC_OFFSET(name)  \
    APPEND(_FUNC_PREFIX(name), _Offset)

#define FUNC_PTR(offset, name, ...)                             \
    namespace shimmer::func_ptrs {                              \
        using APPEND(name, _t) = __VA_ARGS__;                   \
        static constexpr size_t APPEND(name, _Offset) = offset; \
    }                                                           \
    extern _FUNC_TYPE(name) name

#define SETUP_FUNC_PTR(name)     \
    _FUNC_TYPE(name) name = (_FUNC_TYPE(name)) exl::util::modules::GetTargetOffset(_FUNC_OFFSET(name))


#include "misc_types.hpp"
#include <sead/gfx/seadTextWriter.h>
#include <sead/gfx/seadDrawContext.h>

FUNC_PTR(0x0174A19C, TextWriter_printf,                                         void (*)(sead::TextWriter*, char const*, ...));
FUNC_PTR(0x0174A0EC, TextWriter_setupGraphics,                                  void (*)(sead::DrawContext*));
FUNC_PTR(0x0174A104, TextWriter_setCursorFromTopLeft,                           void (*)(sead::TextWriter*, sead::Vector2<float> const&));
FUNC_PTR(0x0174A148, TextWriter_beginDraw,                                      void (*)(sead::TextWriter*));
FUNC_PTR(0x0174A17C, TextWriter_endDraw,                                        void (*)(sead::TextWriter*));
FUNC_PTR(0x01748010, PrimitiveDrawer_Ctor,                                      void (*)(PrimitiveDrawer*, sead::DrawContext*));
FUNC_PTR(0x017480E8, PrimitiveDrawer_begin,                                     void (*)(PrimitiveDrawer*));
FUNC_PTR(0x0174810C, PrimitiveDrawer_end,                                       void (*)(PrimitiveDrawer*));
FUNC_PTR(0x01748738, PrimitiveDrawer_drawLine,                                  void (*)(PrimitiveDrawer*, sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Color4f const&));
FUNC_PTR(0x01748370, PrimitiveDrawer_drawWireCube,                              void (*)(PrimitiveDrawer*, PrimitiveDrawer_CubeArg const&));
FUNC_PTR(0x01748818, PrimitiveDrawer_drawSphere4x8,                             bool (*)(PrimitiveDrawer*, sead::Vector3<float> const&,float,sead::Color4f const&));
FUNC_PTR(0x0171986C, PrimitiveDrawMgrNvn_drawCubeImpl,                          void (*)(uintptr_t, sead::DrawContext *,sead::Matrix34<float> const&,sead::Color4f const&,sead::Color4f const&));
FUNC_PTR(0x017809C8, DevTools_beginDrawImm,                                     void (*)(sead::DrawContext *,sead::Matrix34<float> const&,sead::Matrix44<float> const&));
FUNC_PTR(0x017847C0, DevTools_drawBoundBoxImm,                                  void (*)(sead::DrawContext *,sead::BoundBox3<float> const&,sead::Color4f const&,float));
FUNC_PTR(0x019ED7B4, Lp_Utl_getGfxLayer3D,                                      uintptr_t (*)(int));
FUNC_PTR(0x017A0C74, agl_lyr_Layer_getRenderCamera,                             sead::Camera* (*)(uintptr_t));
FUNC_PTR(0x017A0CB4, agl_lyr_Layer_getRenderProjection,                         sead::Projection* (*)(uintptr_t));
FUNC_PTR(0x017A0D90, agl_lyr_Layer_setVisible,                                  void (*)(uintptr_t, bool));
FUNC_PTR(0x0010F5F0, Cmn_GfxUtl_getLyrIdx_3D_Main,                              int (*)());
FUNC_PTR(0x010022F0, Game_Player_makeShape_MapObjCollision,                     void (*)(uintptr_t, ShapeCapsule*));
FUNC_PTR(0x01001960, Game_Player_isInSquid,                                     bool (*)(uintptr_t));
FUNC_PTR(0x009FC93C, Game_CheckPointBaseOcta_getClassIterNodeStatic,            ActorClassIterNodeBase* (*)());
FUNC_PTR(0x0195612C, Lp_Sys_ActorClassIterNodeBase_derivedFrontActiveActor,     Actor* (*)(ActorClassIterNodeBase*));
FUNC_PTR(0x01956280, Lp_Sys_ActorClassIterNodeBase_derivedNextActiveActor,      Actor* (*)(ActorClassIterNodeBase*, Actor*));
FUNC_PTR(0x017A3338, agl_lyr_Renderer_calc,                                     void (*)(uintptr_t*, bool));

#undef FUNC_PTR