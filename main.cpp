
#include <cstdint> 
#include <memory>
#include <iostream>
#include <functional>
#include <vector>
#include <math.h> 

#include "VoxelWorld.h"


int main()
{
    //const size_t GRID_SQUARED = 64; // 4 - 16 - 64 - 256 - 1024
    //const size_t DEPTH = (size_t)std::sqrt(GRID_SQUARED);
    //printf("DEPTH: %zu \n", DEPTH);
    //auto quadTree = std::make_unique<QuadTree<float, voxel::Voxel>>(GRID_SQUARED, DEPTH);
    //auto chonk = std::make_unique<voxel::VoxelChonk>(GRID_SQUARED);
   
    const size_t CHONK_GRID_SIZE = 16;
    auto voxelWorld = std::make_unique<voxel::VoxelWorld>(CHONK_GRID_SIZE, 3);
    voxelWorld->buildWorld();

    //const Point<float> origin_0 = { 32.f , 32.f };
    ////quadTree->circle(origin_0, 28., true, voxel::VOXEL_TYPE::dirt);

    //size_t octaves = 4;
    //float frequency = 0.04f;
    //float amplitude = 100.5f;
    //float lacunarity = 2.0f;
    //float persistence = 0.5f;

    //voxel::FractalNoiseParams params
    //{
    //    octaves,
    //    frequency,
    //    amplitude,
    //    lacunarity,
    //    persistence,
    //    true
    //};
    //chonk->addNoise(params);
    //chonk->print();


    //quadTree->addNoise(octaves, frequency, amplitude, lacunarity, persistence, false);

    // quadTree->circle( origin_0, 3, false ); 

    // big circle  // 64
    // const Point origin_0 = { 32 , 32};
    // quadTree->circle( origin_0, 12, true );
    // const Point origin_1 = { 26 , 26};
    // quadTree->circle( origin_1, 4, false ); 
    // const Point origin_2 = { 38 , 26};
    // quadTree->circle( origin_2, 4, false ); 
    // const Point origin_3 = { 38 , 38};
    // quadTree->circle( origin_3, 4, false ); 
    // const Point origin_4 = { 26 , 38};
    // quadTree->circle( origin_4, 4, false ); 


    //printf("number of nodes: %zu \n", quadTree->size());
    //printf("size of quadTree: %zu \n", sizeof(*quadTree.get()));
    //printf("size of QuadNode: %zu \n", sizeof(QuadNode<float, voxel::Voxel>));
    //const size_t bits = quadTree->size() * sizeof(QuadNode<float, voxel::Voxel>) + sizeof(*quadTree.get());
    //const size_t bytes = bits / 8;
    //const size_t kb = bytes / 1024;
    //const size_t mb = kb / 1024;

    //printf("size of nodes: %zub -> %zuB -> %zukb -> %zumb\n", bits, bytes, kb, mb);

    //const size_t length = (size_t)GRID_SQUARED;
    //for (size_t i = 0; i < length; ++i)
    //{
    //    for (size_t j = 0; j < length; ++j)
    //    {
    //        // add data type prints
    //        if (auto node = quadTree->m_quadNodeVector[j][i])
    //        {
    //            if (node->active && node->data)
    //            {
    //                switch (node->data->type)
    //                {
    //                case voxel::VOXEL_TYPE::water:
    //                    std::cout << "   ";
    //                    break;
    //                case voxel::VOXEL_TYPE::dirt:
    //                    std::cout << " 0 ";
    //                    break;
    //                case voxel::VOXEL_TYPE::sand:
    //                    std::cout << " . ";
    //                    break;
    //                default:
    //                    std::cout << " x ";
    //                }
    //            }
    //        }
    //        else
    //            std::cout << " x ";
    //    }
    //    std::cout << std::endl;
    //}

    return 0;
}