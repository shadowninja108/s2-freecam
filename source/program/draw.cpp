#include "draw.hpp"

#include <lib.hpp>
#include "func_ptrs.hpp"

#include <sead/container/seadSafeArray.h>
#include <sead/gfx/seadCamera.h>
#include <sead/gfx/seadProjection.h>

namespace shimmer::draw {

    namespace {
        std::array<DrawEntry_t, MaxDrawEntries> s_EntryBuffer;
        size_t s_EntryCount = 0;
    };

    uintptr_t GetLayer(int layerId) {
        auto mgrArray = exl::util::pointer_path::FollowSafe<sead::SafeArray<uintptr_t, 3>, 0x02D52F70, 0x30>();

        if(mgrArray == NULL)
            return 0;

        auto mgr = mgrArray->mBuffer[layerId];

        if(mgr == 0)
            return 0;

        return exl::util::pointer_path::Follow<uintptr_t, 0x348>(mgr);
    }

    void Subscribe(DrawEntry_t entry) {
        /* TODO: proper result. */
        if(s_EntryCount >= s_EntryBuffer.size())
            EXL_ABORT(0x69);

        /* Append to entries. */
        s_EntryBuffer[s_EntryCount] = entry;
        s_EntryCount++;
    }

    void DrawImpl(sead::TextWriter* textWriter, sead::DrawContext* sctx) {
        /* Setup primitive drawer. */
        PrimitiveDrawer drawer;
        PrimitiveDrawer_Ctor(&drawer, sctx);

        /* Try getting camera and projection. */
        auto layer = GetLayer(0);
        if(layer != 0) {
            auto camera = agl_lyr_Layer_getRenderCamera(layer);
            auto projection = agl_lyr_Layer_getRenderProjection(layer);

            drawer.mCamMatrix = &camera->mMatrix;
            drawer.mProjMatrix = &projection->mMatrix;
        }

        /* Build ctx. */
        DrawCtx ctx {
            .m_TextWriter = textWriter,
            .m_SeadDrawContext = sctx,
            .m_PrimDrawer = &drawer,
        };

        /* Setup textwriter. */
        textWriter->mColor = TextColor;
        TextWriter_setupGraphics(sctx);
        
        auto defaultModelMtx = drawer.mModelMatrix;

        /* Call all our subscribers. */
        for(size_t i = 0; i < s_EntryCount; i++) {
            s_EntryBuffer[i](ctx);

            /* Reset model matrix after calling, for safety/convenience. */
            ctx.m_PrimDrawer->mModelMatrix = defaultModelMtx;
        }
    }

    void Install() {
        namespace patch = exl::patch;
        namespace inst = exl::armv8::inst;
        namespace reg = exl::armv8::reg;

        /* Start of where invokeDrawTV_ is. */
        static constexpr uintptr_t DrawTvStart = 0x0185121C;
        /* Where NOP needs to be placed to enable drawing utility. */
        static constexpr uintptr_t EnableNop = DrawTvStart + 0x284;
        /* Base of the main asm to write. */
        static constexpr uintptr_t AsmBase = DrawTvStart + 0x390;
        /* Place to jump to at the end of asm. */
        static constexpr uintptr_t SkipTo = DrawTvStart + 0x654;

        patch::CodePatcher p(EnableNop);

        /* Enable debug drawing utility. */
        p.WriteInst(inst::Nop());

        /* Replace a sead::TextWriter call with a branch to our entrypoint. */
        p.Seek(AsmBase);
        p.WriteInst(inst::MovRegister(reg::X1, reg::X25)); /* Copy over draw context. */
        p.BranchLinkInst((void*) DrawImpl);
        /* Branch over other debug printing. */
        p.BranchInst(SkipTo);
    }   
}