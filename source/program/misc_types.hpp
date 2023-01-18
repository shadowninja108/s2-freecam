#pragma once

#include <sead/prim/seadSafeString.hpp>
#include <sead/container/seadTreeNode.h>
#include <sead/math/seadMatrix.hpp>
#include <sead/math/seadBoundBox.hpp>
#include <sead/gfx/seadDrawContext.h>
#include <sead/gfx/seadColor.h>

/* TODO: clean up this... */

struct Actor;

struct PrimitiveDrawer {
    uintptr_t __vftable;
    sead::Matrix34<float>* mModelMatrix;
    sead::Matrix34<float>* mCamMatrix;
    sead::Matrix44<float>* mProjMatrix;
    sead::DrawContext* mDrawCtx;
};

struct ShapeCapsule {
    uintptr_t __vftable;
    sead::Vector3f mPos0;
    sead::Vector3f mPos1;
    float mRadius;
};

struct DIR {
    sead::Vector3f mRight;
    sead::Vector3f mUp;
    sead::Vector3f mFront;
};

struct ShapeBox {
    uintptr_t __vftable;
    sead::Vector3f mPos;
    DIR mDir;
    sead::Vector3f mScale;
};

struct ActorClassIterNodeBase {
    uintptr_t __vftable;
    sead::TreeNode mChildClasses;
    u8 mActorList[0x48 - 0x28];
    sead::SafeString mClassName;
};

struct PrimitiveDrawer_CubeArg
{
    sead::Vector3f mPos;
    sead::Vector3f mSize;
    sead::Color4f mColor1;
    sead::Color4f mColor2;

    inline PrimitiveDrawer_CubeArg(sead::Vector3f const& pos, sead::Vector3f const& size, sead::Color4f const& color) {
        mPos = pos;
        mSize = size;
        mColor1 = color;
        mColor2 = color;
    }

    inline PrimitiveDrawer_CubeArg(sead::BoundBox3f const& box, sead::Color4f const& color) {
        mPos = box.getCenter();
        mSize = { box.getSizeX(), box.getSizeY(), box.getSizeZ() };
        mColor1 = color;
        mColor2 = color;
    }
};