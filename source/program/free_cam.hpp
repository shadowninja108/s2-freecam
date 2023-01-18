#pragma once

#include <lib.hpp>

namespace shimmer::free_cam {

    namespace impl {
        static constexpr uintptr_t LpLyr2D_OffVtable = 0x02CB2558;
        static constexpr uintptr_t LpLyr3D_OffVtable = 0x02CB3550;

        static constexpr uintptr_t Renderer_FuncCalc = 0x017A3338;
        static constexpr uintptr_t Renderer_OffDbgState = 0x558;

        static constexpr uintptr_t Layer_FuncCalc = 0x017A0E40;
        static constexpr uintptr_t Layer_OffController = 0x560;
        static constexpr uintptr_t Layer_OffDbgInfo = 0x1F0;

        static constexpr uintptr_t DevTools_OffsCameraOperationSpeed = 0x02C99B10;

        static constexpr uintptr_t DbgInfo_OffStrs = 0x3A8;

        static constexpr uintptr_t ControllerMgr_OffsInstance = 0x02E8FBD8;
        static constexpr uintptr_t ControllerMgr_OffControllerArray = 0x188;
    }

    void Install();

    static inline auto& GetCamOpSpeed() {
        return *exl::util::pointer_path::FollowSafe<float, impl::DevTools_OffsCameraOperationSpeed>();
    }
}