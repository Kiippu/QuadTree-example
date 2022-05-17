#pragma once

#include <memory>
#include <map>
#include "QuadTree.h"
//

namespace voxel
{
    struct Voxel;
    struct FractalNoiseParams
    {
        size_t octaves = 4;
        float frequency = 1.0f;
        float amplitude = 1.0f;
        float lacunarity = 2.0f;
        float persistence = 0.5f;
        bool addition = true;
    };

    class VoxelQuerryParams
    {
    public:
        Point<float> p;
        double radius;
        bool addition;
        voxel::VOXEL_TYPE type;
        //std::function<double(Point<float>)> radiusFn;
        bool recursive;
    };


    class VoxelChonk
    {
    public:

        VoxelChonk(size_t _size, Point<float> _origin);
        ~VoxelChonk();

        void init();

        const std::map<size_t, std::map<size_t, QuadNode<float, Voxel>*>>& getNodeList2d()  const
        {
            return m_nodeList2d;
        }

        void circle(const VoxelQuerryParams _param);

        void addNoise(const FractalNoiseParams _params);

        void print();

        const std::map<size_t, std::map<size_t,QuadNode<float, Voxel>*>>& getNode2dList() { return m_nodeList2d; };

        const size_t getQuadTreeSize();

    private:

        void _addNoise(const FractalNoiseParams& _params, QuadNode<float, Voxel>*);

        bool _isInsideVoxel(const VoxelQuerryParams& _param, QuadNode<float, Voxel>* _node);

        bool _isInRange(const VoxelQuerryParams& _param, QuadNode<float, Voxel>* _node);

        void _circle(const VoxelQuerryParams& _param, QuadNode<float, Voxel>* _node);

        void _pruneTree(QuadNode<float, Voxel>* _node);

        void _getQuadTreeSize(size_t& _size, QuadNode<float, Voxel>* _node);

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
        std::map<size_t, std::map<size_t, QuadNode<float, Voxel>*>> m_nodeList2d;

    };
}

