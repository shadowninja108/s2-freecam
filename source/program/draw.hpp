#pragma once

#include <array>
#include <sead/gfx/seadTextWriter.h>
#include <sead/gfx/seadDrawContext.h>
#include "func_ptrs.hpp"
#include "misc_types.hpp"

namespace shimmer::draw {

    static constexpr size_t MaxDrawEntries = 5;
    static const auto TextColor = sead::Color4f(1, 1, 1, 1);

    class DrawerHandle {
        PrimitiveDrawer* m_PrimitiveDrawer;

        public:
        inline DrawerHandle(PrimitiveDrawer* drawer) : m_PrimitiveDrawer(drawer) {
            PrimitiveDrawer_begin(m_PrimitiveDrawer);
        }

        inline void DrawLine(sead::Vector3<float> const& p1,sead::Vector3<float> const& p2,sead::Color4f const& color) {
            PrimitiveDrawer_drawLine(m_PrimitiveDrawer, p1, p2, color);
        }

        inline void DrawWireCube(PrimitiveDrawer_CubeArg const& arg) {
            PrimitiveDrawer_drawWireCube(m_PrimitiveDrawer, arg);
        }

        inline void DrawAxis(sead::Vector3<float> const& pos, float length) {
            const auto red =    sead::Color4f(1.0, 0.0, 0.0, 1.0);
            const auto green =  sead::Color4f(0.0, 1.0, 0.0, 1.0);
            const auto blue =   sead::Color4f(0.0, 0.0, 1.0, 1.0);
            
            DrawLine(pos, pos + sead::Vector3f(length,  0.0,    0.0),       red);
            DrawLine(pos, pos + sead::Vector3f(0.0,     length, 0.0),       green);
            DrawLine(pos, pos + sead::Vector3f(0.0,     0.0,    length),    blue);
        }

        inline ~DrawerHandle() {
            PrimitiveDrawer_end(m_PrimitiveDrawer);
        }
    };

    struct DrawCtx {
        sead::TextWriter* m_TextWriter;
        sead::DrawContext* m_SeadDrawContext;
        PrimitiveDrawer* m_PrimDrawer;

        inline DrawerHandle OpenDrawer() const {
            return { m_PrimDrawer };
        }
    };

    using DrawEntry_t = void (*)(const DrawCtx&);

    void Subscribe(DrawEntry_t);
    void Install();
}