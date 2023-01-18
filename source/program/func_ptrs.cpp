#include "func_ptrs.hpp"

SETUP_FUNC_PTR(TextWriter_printf);
SETUP_FUNC_PTR(TextWriter_setupGraphics);
SETUP_FUNC_PTR(TextWriter_setCursorFromTopLeft);
SETUP_FUNC_PTR(TextWriter_beginDraw);
SETUP_FUNC_PTR(TextWriter_endDraw);
SETUP_FUNC_PTR(PrimitiveDrawer_Ctor);
SETUP_FUNC_PTR(PrimitiveDrawer_begin);
SETUP_FUNC_PTR(PrimitiveDrawer_end);
SETUP_FUNC_PTR(PrimitiveDrawer_drawLine);
SETUP_FUNC_PTR(PrimitiveDrawer_drawWireCube);
SETUP_FUNC_PTR(PrimitiveDrawer_drawSphere4x8);
SETUP_FUNC_PTR(PrimitiveDrawMgrNvn_drawCubeImpl);
SETUP_FUNC_PTR(DevTools_beginDrawImm);
SETUP_FUNC_PTR(DevTools_drawBoundBoxImm);
SETUP_FUNC_PTR(Lp_Utl_getGfxLayer3D);
SETUP_FUNC_PTR(agl_lyr_Layer_getRenderCamera);
SETUP_FUNC_PTR(agl_lyr_Layer_getRenderProjection);
SETUP_FUNC_PTR(agl_lyr_Layer_setVisible);
SETUP_FUNC_PTR(Cmn_GfxUtl_getLyrIdx_3D_Main);
SETUP_FUNC_PTR(Game_Player_makeShape_MapObjCollision);
SETUP_FUNC_PTR(Game_Player_isInSquid);
SETUP_FUNC_PTR(Game_CheckPointBaseOcta_getClassIterNodeStatic);
SETUP_FUNC_PTR(Lp_Sys_ActorClassIterNodeBase_derivedFrontActiveActor);
SETUP_FUNC_PTR(Lp_Sys_ActorClassIterNodeBase_derivedNextActiveActor);
SETUP_FUNC_PTR(agl_lyr_Renderer_calc);