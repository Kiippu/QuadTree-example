#pragma once


namespace voxel
{
    enum class VOXEL_TYPE
    {
        dirt,
        sand,
        water
    };

    struct Voxel
    {
        VOXEL_TYPE type;
    };
}


