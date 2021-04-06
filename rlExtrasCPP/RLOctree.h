#pragma once

#include "RLFrustum.h"

#include "raylib.h"
#include "raymath.h"

#include <memory>
#include <vector>

struct RLOctreeObject
{
    BoundingBox Bounds = { 0 };
    void* Object = nullptr;

    using Ptr = std::shared_ptr<RLOctreeObject>;
};

class RLOctreeLeaf
{
public:
    BoundingBox Bounds = { 0 };

    static const int MaxDepth = 40;
    static const bool DoFastOut = true;

    int MaxObjects = 8;

    std::vector<RLOctreeObject::Ptr> ContainedObjects;

    std::vector<RLOctreeLeaf> Children;

    RLOctreeLeaf(const BoundingBox& bounds);
    inline RLOctreeLeaf(Vector3& min, Vector3& max) : RLOctreeLeaf(BoundingBox{ min,max }) {}

    virtual ~RLOctreeLeaf() {}

    void FastRemove(RLOctreeObject::Ptr item);

    void Distribute(int depth);

    virtual size_t GetObjectsInFrustum(std::vector<RLOctreeObject::Ptr>& objets, const RLFrustum& frustum);
    virtual size_t GetObjectsInBoundingBox(std::vector<RLOctreeObject::Ptr>& objets, BoundingBox& box);

    void DrawDebug();

protected:
    void Split();
    void FastAddChildren(std::vector<RLOctreeObject::Ptr>& objets);
};

class RLOctree : public RLOctreeLeaf
{
public:
    RLOctree();
    void ComputeBounds();

    virtual void Add(RLOctreeObject::Ptr item);
    virtual RLOctreeObject::Ptr Add(BoundingBox& box, void* item);
    RLOctreeObject::Ptr Add(Vector3& min, Vector3& max, void* item) { return Add(BoundingBox{ min,max }, item); }
};
