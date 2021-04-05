#include "RLOctree.h"


enum class rlContainmentType
{
    Disjoint,
    Contains,
    Intersects,
};

 rlContainmentType BoundingBoxContains(BoundingBox& b1, BoundingBox &b2)
{
    //test if all corner is in the same side of a face by just checking min and max
    if (b2.max.x < b1.min.x
        || b2.min.x > b1.max.x
        || b2.max.y < b1.min.y
        || b2.min.y > b1.max.y
        || b2.max.z < b1.min.z
        || b2.min.z > b1.max.z)
        return rlContainmentType::Disjoint;

    if (b2.min.x >= b1.min.x
        && b2.max.x <= b1.max.x
        && b2.min.y >= b1.min.y
        && b2.max.y <= b1.max.y
        && b2.min.z >= b1.min.z
        && b2.max.z <= b1.max.z)
        return rlContainmentType::Contains;

    return rlContainmentType::Intersects;
}

RLOctreeLeaf::RLOctreeLeaf(BoundingBox& bounds)
{

}

void RLOctreeLeaf::FastRemove(RLOctreeObject::Ptr item)
{
    for (auto itr = ContainedObjects.begin(); itr != ContainedObjects.end();)
    {
        if ((*itr)->Object == item->Object)
        {
            ContainedObjects.erase(itr);
            for (std::vector<RLOctreeLeaf>::iterator leaf = Children.begin(); leaf != Children.end();)
            {
                leaf->FastRemove(item);
                if (leaf->ContainedObjects.empty())
                    leaf = Children.erase(leaf);
                else
                    leaf++;
            }
        }
    }
}

void RLOctreeLeaf::Distribute(int depth)
{
    if (ContainedObjects.size() > MaxObjects && depth <= MaxDepth )
    {
         Split();
         for (int i = int(ContainedObjects.size()) - 1; i >= 0; i--)// (OctreeObject item in containedObjects)
         {
             RLOctreeObject::Ptr item = ContainedObjects[i];
             for(auto& leaf : Children)
             {
                 BoundingBox &bounds = item->Bounds;
                 if (BoundingBoxContains(leaf.Bounds,bounds) == rlContainmentType::Contains)
                 {
                     leaf.ContainedObjects.push_back(item);
                     ContainedObjects.erase(ContainedObjects.begin() + i);
                     break;
                 }
             }
         }
 
         depth++;
         for(auto& leaf : Children)
             leaf.Distribute(depth);
         depth--;
    }
}

size_t RLOctreeLeaf::GetObjectsInFrustum(std::vector<RLOctreeObject::Ptr>& objects, const RLFrustum& frustum)
{

    // if the current box is totally contained in our leaf, then add me and all my kids
    if (DoFastOut && frustum.AABBoxIn(Bounds.min,Bounds.max))
        FastAddChildren(objects);
    else
    {
        // ok so we know that we are probably intersecting or outside
        for (auto item : ContainedObjects) // add our stragglers
            objects.emplace_back(item);


        for (auto& leaf : Children)
        {
            // if the child is totally in the volume then add it and it's kids
            if (DoFastOut && frustum.AABBoxIn(leaf.Bounds.min, leaf.Bounds.max))
                leaf.FastAddChildren(objects);
            else
            {
                if (frustum.AABBoxIn(leaf.Bounds.min,leaf.Bounds.max))
                    leaf.GetObjectsInFrustum(objects, frustum);
            }
        }
    }

    return objects.size();
}

size_t RLOctreeLeaf::GetObjectsInBoundingBox(std::vector<RLOctreeObject::Ptr>& objects, BoundingBox& box)
{
    // if the current box is totally contained in our leaf, then add me and all my kids
    if (BoundingBoxContains(box, Bounds) == rlContainmentType::Contains)
        FastAddChildren(objects);
    else
    {
        // ok so we know that we are probably intersecting or outside
        for (auto item : ContainedObjects) // add our stragglers
            objects.emplace_back(item);

        for (auto& leaf : Children)
        {
            // see if any of the sub boxes intersect our frustum
            if (BoundingBoxContains(leaf.Bounds,box) == rlContainmentType::Intersects)
                leaf.GetObjectsInBoundingBox(objects, box);
        }
    }

    return objects.max_size();
}

void RLOctreeLeaf::Split()
{
    Vector3 half = Vector3Scale(Vector3Subtract(Bounds.max, Bounds.min), 0.5f);
    Vector3 halfx = { half.x, 0, 0 };
    Vector3 halfy = { 0, half.y, 0 };
    Vector3 halfz = { 0, 0, half.z };

    Children.clear();

    Children.emplace_back(RLOctreeLeaf( Bounds.min, Vector3Add(Bounds.min, half)));
    Children.emplace_back(RLOctreeLeaf( Vector3Add(Bounds.min, halfx), Vector3Add(Vector3Subtract(Bounds.max, half), halfx)));
    Children.emplace_back(RLOctreeLeaf( Vector3Add(Bounds.min, halfz), Vector3Add(Vector3Add(Bounds.min,half),halfz)));
    Children.emplace_back(RLOctreeLeaf( Vector3Add(Vector3Add(Bounds.min, halfx), halfz), Vector3Subtract(Bounds.max,halfy)));
    Children.emplace_back(RLOctreeLeaf( Vector3Add(Bounds.min, halfy), Vector3Subtract(Vector3Subtract(Bounds.max,halfx), halfz)));
    Children.emplace_back(RLOctreeLeaf( Vector3Add(Vector3Add(Bounds.min, halfy),halfx), Vector3Subtract(Bounds.max, halfz)));
    Children.emplace_back(RLOctreeLeaf( Vector3Add(Vector3Add(Bounds.min, halfy ),halfz), Vector3Subtract(Bounds.max ,halfx)));
    Children.emplace_back(RLOctreeLeaf( Vector3Add(Bounds.min, half), Bounds.max));
}

void RLOctreeLeaf::FastAddChildren(std::vector<RLOctreeObject::Ptr>& objects)
{
    for(auto &item : ContainedObjects)
        objects.push_back(item);

    for(auto& leaf : Children)
        leaf.FastAddChildren(objects);
}

void MergeBox(BoundingBox &original, BoundingBox &additional)
{
    original.min.x = std::min(original.min.x, additional.min.x);
    original.min.y = std::min(original.min.y, additional.min.y);
    original.min.z = std::min(original.min.z, additional.min.z);
    original.max.x = std::min(original.max.x, additional.max.x);
    original.max.y = std::min(original.max.y, additional.max.y);
    original.max.z = std::min(original.max.z, additional.max.z);
}

RLOctree::RLOctree() : RLOctreeLeaf(BoundingBox{0,0,0,0})
{

}

void RLOctree::ComputeBounds()
{
    for (auto& item : ContainedObjects)
        MergeBox(Bounds, item->Bounds);
}

void RLOctree::Add(RLOctreeObject::Ptr item)
{
    ContainedObjects.push_back(item);
    ComputeBounds();
    Distribute(0);
}

RLOctreeObject::Ptr RLOctree::Add(BoundingBox& box, void* item)
{
    RLOctreeObject::Ptr object = std::make_shared<RLOctreeObject>();
    object->Bounds = box;
    object->Object = item;
    Add(object);
    return object;
}

