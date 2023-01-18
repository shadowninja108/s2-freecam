#include "free_cam.hpp" 

#include "func_ptrs.hpp"
#include <sead/gfx/seadColor.h>
#include <sead/prim/seadSafeString.hpp>
#include <sead/container/seadPtrArray.h>
#include <sead/container/seadSafeArray.h>
#include <sead/controller/seadController.h>
#include <sead/gfx/seadCamera.h>
#include "draw.hpp"

namespace nn::hid {

    struct ControllerSupportResultInfo
    {
        int mConnectedControllers;
        int mSelectedId;
    };

    struct ControllerSupportArg
    {
        static constexpr size_t ControllerNameSize = 128 + 1;

        u8 mMinPlayerCount;
        u8 mMaxPlayerCount;
        bool mTakeOverConnection;
        bool mLeftJustify;
        bool mPermitJoyconDual;
        bool mSingleMode;
        bool mUseColors;
        sead::Color4u8 ControllerSupportArg_x7[4];
        bool mUsingControllerNames;
        char mControllerNames[4][ControllerNameSize];
    };

    Result ShowControllerSupport(ControllerSupportResultInfo *,ControllerSupportArg const&);
};

namespace shimmer::free_cam {

    /* Control bitfields. */
    static constexpr uint FreeCamEnableBits = 0x200;        /* Minus */
    static constexpr uint ShowDbgInfoEnableBits = 0x400;    /* Plus */
    static constexpr uint CamSpeedUpBits = 0x10000;         /* Dpad up */
    static constexpr uint CamSpeedDownBits = 0x20000;       /* Dpad down */

    using DbgInfoStrs = sead::SafeArray<sead::FixedSafeString<512>, 2>;

    #define PTR_AT(type, base, offset)    \
        reinterpret_cast<type>(base + offset)
    #define FUNC_PTR(offset, name, ...)     \
        namespace func_ptrs {               \
            using name##_t = __VA_ARGS__ ;  \
        }                                   \
        func_ptrs::name##_t name = (func_ptrs::name##_t) exl::util::modules::GetTargetOffset(offset)

    /* Quick getters/utilities. */
    static inline auto GetCtrls() {
        return exl::util::pointer_path::FollowSafe<sead::FixedPtrArray<sead::Controller, 8>, impl::ControllerMgr_OffsInstance, impl::ControllerMgr_OffControllerArray>();
    }
    static inline auto GetDbgInfo(uintptr_t layer) {
        return exl::util::pointer_path::Follow<uintptr_t, impl::Layer_OffDbgInfo>(layer);
    }
    static inline auto GetDbgInfoStrs(uintptr_t layer) {
        return exl::util::pointer_path::Follow<DbgInfoStrs*, impl::DbgInfo_OffStrs>(GetDbgInfo(layer));
    }

    /* Hacky type comparison by vtable. */
    static bool IsLayerLp3D(uintptr_t layer) {
        uintptr_t vtbl = exl::util::modules::GetTargetOffset(impl::LpLyr3D_OffVtable);
        return *(uintptr_t*)layer == vtbl;
    }
    static bool IsLayerLp2D(uintptr_t layer) {
        uintptr_t vtbl = exl::util::modules::GetTargetOffset(impl::LpLyr2D_OffVtable);
        return *(uintptr_t*)layer == vtbl;
    }
    
    /* Static state. */
    static bool s_FreeCamEnabled = false;
    static bool s_ShowDebugInfo = false;
    static DbgInfoStrs* s_DebugInfoStrs = NULL;
    static sead::LookAtCamera* s_DbgCamera;

    MAKE_HOOK_T(void, RendererCalc, (uintptr_t _this, bool a),
        /* Set the debug camera state. */
        int* dbgState = PTR_AT(int*, _this, impl::Renderer_OffDbgState);
        *dbgState = s_FreeCamEnabled;

        /* Try getting ctrl ptr array. */
        auto ctrls = GetCtrls();
        if(ctrls != NULL) {
            auto camCtrl = ctrls->at(1);
            sead::Controller** rCtrl = PTR_AT(sead::Controller**, _this, impl::Layer_OffController);
            *rCtrl = camCtrl;

            /* Add extra controls if we have a controller to use. */
            if(camCtrl != NULL) {
                auto trig = camCtrl->getButtonsTrigger();
                auto& camSpeed = GetCamOpSpeed();

                if(trig & CamSpeedUpBits) {
                    camSpeed += 0.5;
                } else if(trig & CamSpeedDownBits) {
                    camSpeed -= 0.5;
                }

                /* Check toggling the free cam. */
                if(trig & FreeCamEnableBits) {
                    s_FreeCamEnabled = !s_FreeCamEnabled;
                }

                /* Check for showing debug info. */
                if(trig & ShowDbgInfoEnableBits) {
                    s_ShowDebugInfo = !s_ShowDebugInfo;
                }
            }
        }

        impl(_this, a);
    )

    MAKE_HOOK_T(void, LayerCalc, (uintptr_t _this, uintptr_t controller, int camCtrlType, bool useTwist),
        static constexpr u16 Bf1EnableBits = 8 | 1;
        static constexpr u16 Bf1EnableMask = ~(0x100 | 0x10);
        static constexpr u16 Bf3EnableBits = 8;

        /* Get pointers to relevant bitfields. */
        auto bf1 = PTR_AT(u16*, _this, 0x82);
        auto bf3 = PTR_AT(u16*, _this, 0x80);

        /* Pass through if not a 3D layer.*/
        if(!IsLayerLp3D(_this)) {

            /* Check if this is a 2D layer. */
            if(IsLayerLp2D(_this)) {
                bool visible = *bf3 & 1;
                bool shouldShow = !s_FreeCamEnabled;

                /* Hide layers if free cam is enabled. */
                if(visible != shouldShow) {
                    agl_lyr_Layer_setVisible(_this, shouldShow);
                }
            }

            impl(_this, controller, camCtrlType, useTwist);
            return;
        }

        /* Write to bitfields. */
        if(s_FreeCamEnabled) {
            *bf1 |= Bf1EnableBits;
            *bf1 &= Bf1EnableMask;
            *bf3 |= Bf3EnableBits;
        } else {
            *bf1 &= ~Bf1EnableBits;
            *bf3 &= ~Bf3EnableBits;
        }

        s_DebugInfoStrs = GetDbgInfoStrs(_this);
        s_DbgCamera = PTR_AT(sead::LookAtCamera*, GetDbgInfo(_this), 0x68);
        
        /* Explicitly enable camera twist. TODO: what does dbg cam type 1 do? */
        impl(_this, controller, 0, true);
    )

    void* DoMalloc(size_t size, uintptr_t heap, int alignment) {
        return malloc(size);
    }

    MAKE_HOOK_T(Result, ShowControllerSupport, (nn::hid::ControllerSupportResultInfo* result, nn::hid::ControllerSupportArg const& arg),
        /* Copy arg and modify it. */
        nn::hid::ControllerSupportArg narg = arg;

        strncpy(narg.mControllerNames[0], "Player", nn::hid::ControllerSupportArg::ControllerNameSize);
        strncpy(narg.mControllerNames[1], "Camera", nn::hid::ControllerSupportArg::ControllerNameSize);

        narg.mUsingControllerNames = true;
        narg.mMinPlayerCount = 2;
        narg.mMaxPlayerCount = 2;
        narg.mSingleMode = false;

        return impl(result, narg);
    )

    void Draw(const draw::DrawCtx& ctx) {
        /* Only draw if we need to show info. */
        if(!s_ShowDebugInfo)
            return;

        TextWriter_printf(ctx.m_TextWriter, "Camera speed: %f\n", GetCamOpSpeed());

        /* We need this to draw any more info. */
        if(s_DebugInfoStrs == NULL) {
            return;
        }

        /* Print all the debug strings. */
        for(int i = 0; i < s_DebugInfoStrs->size(); i++) {
            auto& str = s_DebugInfoStrs->mBuffer[i];
            TextWriter_printf(ctx.m_TextWriter, "%s\n", str.cstr());
        }

        /* Need a debug camera for the rest. */
        if(s_DbgCamera == NULL) {
            return;
        }

        auto drawer = ctx.OpenDrawer();

        auto dist = (s_DbgCamera->getAt() - s_DbgCamera->getPos()).length();
        drawer.DrawAxis(s_DbgCamera->getAt(), dist / 10);
    }

    void Install() {
        namespace patch = exl::patch;
        namespace inst = exl::armv8::inst;
        namespace reg = exl::armv8::reg;
        
        INJECT_HOOK_T(impl::Renderer_FuncCalc, RendererCalc); 
        INJECT_HOOK_T(impl::Layer_FuncCalc, LayerCalc);
        INJECT_HOOK_T(nn::hid::ShowControllerSupport, ShowControllerSupport);

        /* All of these are in agl::lyr::Layer::initialize_ */
        /* null heap check before allocating/construction DebugInfo. */
        static constexpr uintptr_t DbgInfoHeapCheck = 0x017A02C4;
        /* First operator new call to allocate the DbgInfo. */
        static constexpr uintptr_t DbgInfoAlloc1 = 0x017A02D4;
        /* First operator new call to allocate the DbgInfo strings. */
        static constexpr uintptr_t DbgInfoAlloc2 = 0x017A02F8;

        patch::CodePatcher p(0x000D569C);
        /* Ignore second controller plugged in. */
        p.WriteInst(inst::Movz(reg::W0, 0));
        /* Supress disconnecting controllers in certain scenes. */
        p.Seek(0x000D52D0);
        p.WriteInst(inst::Movz(reg::W8, 2));
        p.Seek(0x000D52DC);
        p.BranchInst(0x000D5324);
        /* Allow trying to allocate agl's DebugInfo. */
        p.Seek(DbgInfoHeapCheck);
        p.WriteInst(inst::Nop());
        /* Overwrite alloc calls for DebugInfo to our func. */
        p.Seek(DbgInfoAlloc1);
        p.BranchLinkInst((void*) DoMalloc);
        p.Seek(DbgInfoAlloc2);
        p.BranchLinkInst((void*) DoMalloc);

        draw::Subscribe(Draw);
    }   
}