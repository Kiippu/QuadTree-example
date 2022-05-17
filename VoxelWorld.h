#pragma once

#include <memory>

#include "VoxelChonk.h"
#include "QuadTree.h"

namespace voxel
{
    class VoxelWorld
    {
    public:
        VoxelWorld(size_t _chonkSize, size_t _worldGridSize);
        ~VoxelWorld();
        // Assignment Operator
        VoxelWorld& operator=(VoxelWorld);

        void buildWorld();

        voxel::VoxelChonk* loadChonk(Point<size_t>);

        void printWorld();

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}


