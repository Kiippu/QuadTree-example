#include "VoxelWorld.h"
#include "VoxelChonk.h"

#include <memory>
#include <cstdint> 

namespace voxel
{
    struct VoxelWorld::Impl
    {
        Impl(size_t _chonkSize, size_t _worldGridSize)
            : chonkSize(_chonkSize), worldGridSize(_worldGridSize), 
            maxDepth(static_cast<size_t>(std::sqrt(_chonkSize)))
        {
            voxelChonk2d = std::vector < std::vector< std::unique_ptr< VoxelChonk > > >(_worldGridSize);
            for (auto &vec : voxelChonk2d)
            {
                vec = std::vector < std::unique_ptr< VoxelChonk >>(_worldGridSize);
                for (size_t i = 0; i < _worldGridSize; i++)
                    vec[i] = nullptr;
            }
        }
        size_t maxDepth;
        size_t chonkSize;
        size_t worldGridSize;
        std::vector < std::vector< std::unique_ptr< VoxelChonk > > > voxelChonk2d;
        voxel::FractalNoiseParams fractalNoiseParams{ 4, 0.04f, 100.5f, 2.0f, 0.5f };
    };

    VoxelWorld::VoxelWorld(size_t _chonkSize, size_t _worldGridSize)
        : m_impl(std::make_unique<Impl>(_chonkSize, _worldGridSize))
    {
    }
    // Default Constructor
    VoxelWorld::~VoxelWorld() = default;

    void VoxelWorld::buildWorld()
    {
        // load middle world chuck first
        //const size_t x = static_cast<size_t>(m_impl->worldGridSize / 2);
        //const size_t y = static_cast<size_t>(m_impl->worldGridSize / 2);
        
        voxel::VoxelQuerryParams voxelQuerryParams{ Point<float>{ 0.f,0.f }, 6., true, VOXEL_TYPE::dirt, true };
        auto chonk_0 = loadChonk(Point<size_t>{ 0, 0 });
        chonk_0->circle(voxelQuerryParams);

        auto chonk_1 = loadChonk(Point<size_t>{ 0, 1 });
        chonk_1->circle(voxelQuerryParams);
        auto chonk_3 = loadChonk(Point<size_t>{ 1, 1 });
        chonk_3->circle(voxelQuerryParams);
        auto chonk_4 = loadChonk(Point<size_t>{ 1, 0 });
        chonk_4->circle(voxelQuerryParams);

       /* auto chonk_0 = loadChonk(Point<size_t>{ 0, 0 });
        chonk_0->addNoise(m_impl->fractalNoiseParams);

        auto chonk_1 = loadChonk(Point<size_t>{ 0, 1 });
        chonk_1->addNoise(m_impl->fractalNoiseParams);

        auto chonk_2 = loadChonk(Point<size_t>{ 0, 2 });
        chonk_2->addNoise(m_impl->fractalNoiseParams);

        auto chonk_3 = loadChonk(Point<size_t>{ 1, 0 });
        chonk_3->addNoise(m_impl->fractalNoiseParams);

        auto chonk_4 = loadChonk(Point<size_t>{ 1, 1 });
        chonk_4->addNoise(m_impl->fractalNoiseParams);

        auto chonk_5 = loadChonk(Point<size_t>{ 1, 2 });
        chonk_5->addNoise(m_impl->fractalNoiseParams);

        auto chonk_6 = loadChonk(Point<size_t>{ 2, 0 });
        chonk_6->addNoise(m_impl->fractalNoiseParams);

        auto chonk_7 = loadChonk(Point<size_t>{ 2, 1 });
        chonk_7->addNoise(m_impl->fractalNoiseParams);

        auto chonk_8 = loadChonk(Point<size_t>{ 2, 2 });
        chonk_8->addNoise(m_impl->fractalNoiseParams);*/

        //auto chonk_3 = loadChonk(Point<float>{ 1, 0 });
        //chonk_3->addNoise(m_impl->fractalNoiseParams);

        printWorld();
    }

    voxel::VoxelChonk* VoxelWorld::loadChonk(Point<size_t> _point)
    {
        const auto& x = _point.x;
        const auto& y = _point.y;

        Point<float> origin{ static_cast<float>(_point.x*m_impl->chonkSize), static_cast<float>(_point.y*m_impl->chonkSize) };

        if(!m_impl->voxelChonk2d[x][y])
            m_impl->voxelChonk2d[x][y] = std::make_unique<voxel::VoxelChonk>(m_impl->chonkSize, origin);

        return m_impl->voxelChonk2d[x][y].get();
    }

    void VoxelWorld::printWorld()
    {

        auto chonk_1 = loadChonk(Point<size_t>{ 0, 0 });
        //auto chonk_3 = loadChonk(Point<size_t>{ 1, 1 });
        //auto chonk_4 = loadChonk(Point<size_t>{ 1, 0 });
        printf("chonk_1 nodes: %zu \n", chonk_1->getQuadTreeSize());
        //printf("chonk_4 nodes: %zu \n", chonk_4->getQuadTreeSize());
        //printf("chonk_3 nodes: %zu \n", chonk_3->getQuadTreeSize());
        //printf("size of quadTree: %zu \n", sizeof(*quadTree.get()));
        //printf("size of QuadNode: %zu \n", sizeof(QuadNode<float, voxel::Voxel>));
        //const size_t bits = quadTree->size() * sizeof(QuadNode<float, voxel::Voxel>) + sizeof(*quadTree.get());
        //const size_t bytes = bits / 8;
        //const size_t kb = bytes / 1024;
        //const size_t mb = kb / 1024;

        //printf("size of nodes: %zub -> %zuB -> %zukb -> %zumb\n", bits, bytes, kb, mb);

        std::map<size_t, std::map<size_t, QuadNode<float, Voxel>*>> nodesMap;
        const size_t length = m_impl->worldGridSize * m_impl->chonkSize;
        
        for (auto& chonkMap : m_impl->voxelChonk2d)
        {
            for (auto& chonk : chonkMap)
            {
                if (chonk)
                {
                    chonk;
                    auto subNodesMap = chonk->getNode2dList();
                    for (auto &node : subNodesMap)
                    {
                        nodesMap[node.first].merge(node.second);
                    }
                    //nodesMap.merge(subNodesMap);
                }
            }
        }

        for (size_t i = 0; i < length; ++i)
        {
            for (size_t j = 0; j < length; ++j)
            {
                auto itr = nodesMap[i].find(j);
                if(itr == nodesMap[i].cend())
                    nodesMap[i][j] = nullptr;
            }
        }

        for (size_t i = 0; i < length; ++i)
        {
            auto mapI = nodesMap.find(i);
            if (mapI == nodesMap.cend())
                continue;
            for (size_t j = 0; j < length; ++j)
            {
                auto itr = mapI->second.find(j);
                if (itr == mapI->second.cend())
                    continue;
                // add data type prints
                auto node = itr->second;
                if (node && node->data)
                {
                    if (node->active)
                    {
                        switch (node->data->type)
                        {
                        case voxel::VOXEL_TYPE::water:
                            std::cout << " - ";
                            break;
                        case voxel::VOXEL_TYPE::dirt:
                            std::cout << " 0 ";
                            break;
                        case voxel::VOXEL_TYPE::sand:
                            std::cout << " o ";
                            break;
                        default:
                            std::cout << " x ";
                        }
                    }
                }
                else
                    std::cout << " * ";
            }
            std::cout << std::endl;
        }
    }
}

