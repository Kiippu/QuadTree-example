#include "VoxelChonk.h"
#include "Voxel.h"
#include <math.h> 
#include <set>

namespace voxel
{
    struct VoxelChonk::Impl
    {
        Impl(size_t _grid, Point<float> _origin)
            : gridSize(_grid), maxDepth(static_cast<size_t>(std::sqrt(_grid))),
            topLeft(_origin),
            quadTree(std::make_unique<QuadTree<float, Voxel>>(gridSize, maxDepth, topLeft))
        {}
        size_t maxDepth;
        size_t gridSize;
        Point<float> topLeft;
        std::unique_ptr<QuadTree<float, Voxel>> quadTree;
    };

    VoxelChonk::VoxelChonk(size_t _size, Point<float> _topLeft)
        : m_impl(std::make_unique<Impl>(_size, _topLeft))
    {
        init();
    }
    // Default Constructor
    VoxelChonk::~VoxelChonk() = default;

    void VoxelChonk::init()
    {
        /*for (size_t i = 0; i < m_impl->gridSize; ++i)
        {
            m_nodeList2d.insert( std::make_pair(i,std::map<size_t,QuadNode<float, Voxel>*>()) );
            for (size_t j = 0; j < m_impl->gridSize; ++j)
                m_nodeList2d[i][j] = nullptr;
        }*/
    }

    void VoxelChonk::circle(const VoxelQuerryParams _param)
    {
        _circle(_param, m_impl->quadTree->getParentNode());
    }

    void VoxelChonk::addNoise(const FractalNoiseParams _params)
    {
        _addNoise(_params, m_impl->quadTree->getParentNode());
        //_pruneTree(m_impl->quadTree->getParentNode());
    }

    void VoxelChonk::print()
    {
        printf("number of nodes: %zu \n", m_impl->quadTree->size());
        printf("size of quadTree: %zu \n", sizeof(*m_impl->quadTree.get()));
        printf("size of QuadNode: %zu \n", sizeof(QuadNode<float, voxel::Voxel>));
        const size_t bits = m_impl->quadTree->size() * sizeof(QuadNode<float, voxel::Voxel>) + sizeof(*m_impl->quadTree.get());
        const size_t bytes = bits / 8;
        const size_t kb = bytes / 1024;
        const size_t mb = kb / 1024;

        printf("size of nodes: %zub -> %zuB -> %zukb -> %zumb\n", bits, bytes, kb, mb);

        const size_t length = (size_t)m_impl->gridSize;
        for (size_t i = 0; i < length; ++i)
        {
            for (size_t j = 0; j < length; ++j)
            {
                // add data type prints
                auto node = m_nodeList2d[j].at(i);
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

    const size_t VoxelChonk::getQuadTreeSize()
    {
        size_t count{ 0 };
        _getQuadTreeSize(count, m_impl->quadTree->getParentNode());
        return count;
    }

    void VoxelChonk::_addNoise(const FractalNoiseParams& _params, QuadNode<float, Voxel>* _node)
    {
        SimplexNoise simplexNoise(_params.frequency, _params.amplitude, _params.lacunarity, _params.persistence);

        if (_node)
        {
            if (_node->depth >= m_impl->maxDepth)
            {
                const auto point = _node->area.getMidPoint();
                _node->active = true;
                if (simplexNoise.fractal(_params.octaves, point.x, point.y) < 0.1)
                {
                    _node->data = std::make_unique<voxel::Voxel>();
                    _node->data->type = voxel::VOXEL_TYPE::water;
                }
                if (simplexNoise.fractal(_params.octaves, point.x, point.y) > 0.1)
                {
                    _node->data = std::make_unique<voxel::Voxel>();
                    _node->data->type = voxel::VOXEL_TYPE::sand;
                }
                if (simplexNoise.fractal(_params.octaves, point.x, point.y) > 0.4)
                {
                    _node->data = std::make_unique<voxel::Voxel>();
                    _node->data->type = voxel::VOXEL_TYPE::dirt;
                }

                const size_t x = (size_t)(_node->area.topLeft.x);
                const size_t y = (size_t)(_node->area.topLeft.y);
                //printf("x: %d, y:%d\n",x,y);
                m_nodeList2d[x][y] = _params.addition ? _node : nullptr;
                return;
            }
            if (!_node->children[0])
                m_impl->quadTree->buildTree(_node, false);
            for (auto& node : _node->children)
            {
                if (node)
                {
                    _addNoise(_params, node.get());
                }
            }
        }
    }

    bool VoxelChonk::_isInsideVoxel(const VoxelQuerryParams& _param, QuadNode<float, Voxel>* _node)
    {
        if (_param.p.x < _node->area.topLeft.x || _param.p.x > _node->area.bottomRight.x)
            return false;
        if (_param.p.y < _node->area.topLeft.y || _param.p.y > _node->area.bottomRight.y)
            return false;
        return true;
    }

    bool VoxelChonk::_isInRange(const VoxelQuerryParams& _param, QuadNode<float, Voxel>* _node)
    {
        // test topLeft point
        if (_param.p.distanceToPoint(_node->area.topLeft) <= _param.radius)
            return true;
        // test topRight point
        if (_param.p.distanceToPoint(Point<float>{ _node->area.bottomRight.x, _node->area.topLeft.y }) <= _param.radius)
            return true;
        // test bottomLeft point
        if (_param.p.distanceToPoint(Point<float>{ _node->area.topLeft.x, _node->area.bottomRight.y }) <= _param.radius)
            return true;
        // test bottomRight point
        if (_param.p.distanceToPoint(_node->area.bottomRight) <= _param.radius)
            return true;
        // all points not in range so this node is not in range
        return false;
    }

    void VoxelChonk::_circle(const VoxelQuerryParams& _param, QuadNode<float, Voxel>* _node)
    {
        if (_isInsideVoxel(_param, _node) || _isInRange(_param, _node))
        {

            if (_node)
            {
                if (_node->depth >= m_impl->maxDepth)
                {
                    _node->active = _param.addition;
                    _node->data = std::make_unique<voxel::Voxel>();
                    _node->data->type = voxel::VOXEL_TYPE::dirt;
                    const size_t x = (size_t)_node->area.topLeft.x;
                    const size_t y = (size_t)_node->area.topLeft.y;
                    m_nodeList2d[y][x] = _param.addition ? _node : nullptr;
                    return;
                }
                if (!_node->children[0])
                    m_impl->quadTree->buildTree(_node, false);
                for (auto& node : _node->children)
                {
                    if (node)
                    {
                        _circle(_param, node.get());
                    }
                }

                //////////////////////////////////////////////////////////////////////////////
                // Prune alll node that have childrenn of all the same type
                // all nodes exist
                bool checker{ true };
                for (const auto& node : _node->children)
                    if (!node)
                        checker = false;;
                if (!checker)
                    return;

                // all types had data equal
                for (const auto& node : _node->children)
                    if (!node->data)
                        checker = false;;
                if (!checker)
                    return;

                // check all types
                std::set<VOXEL_TYPE> voxelCheck;
                for (const auto& node : _node->children)
                    voxelCheck.insert(node->data->type);

                if (voxelCheck.size() == 1 && checker)
                {
                    _node->data = std::make_unique<voxel::Voxel>();
                    _node->data->type = *(voxelCheck.begin());
                    for (size_t i = 0; i < 4; ++i)
                    {
                        const size_t x = (size_t)_node->children[i]->area.topLeft.x;
                        const size_t y = (size_t)_node->children[i]->area.topLeft.y;
                        m_nodeList2d[y][x] = nullptr;
                        _node->children[i].reset();
                    }
                }
                //////////////////////////////////////////////////////////////////////////////
            }
        }
    }

    void VoxelChonk::_pruneTree(QuadNode<float, Voxel>* _node)
    {
        if (_node->depth >= m_impl->maxDepth)
            return;
        else
        {
            for (auto& node : _node->children)
            {
                if (node.get())
                    _pruneTree(node.get());
            }
        }

        // reset parent node
        const bool allChildrenNullPtr = (!_node->children[0] && !_node->children[1] && !_node->children[2] && !_node->children[3]);
        if (allChildrenNullPtr)
        {
            _node = nullptr;
            return;
        }
        // reset parent data
        const bool allDataNullPtr = (!_node->children[0]->data && !_node->children[1]->data && !_node->children[2]->data && !_node->children[3]->data);
        if (allDataNullPtr)
        {
            _node = nullptr;
            return;
        }

        // make sure all data is valid before checking type
        bool allValidData = true;
        for (auto& node : _node->children)
            if (!node->data)
                allValidData = false;

        if (allValidData)
        {
            // check all types
            VOXEL_TYPE& type0 = _node->children[0]->data->type;
            VOXEL_TYPE& type1 = _node->children[1]->data->type;
            VOXEL_TYPE& type2 = _node->children[2]->data->type;
            VOXEL_TYPE& type3 = _node->children[3]->data->type;
            bool allSameType = (type0 == type1 && type0 == type2 && type0 == type3);
            if (allSameType)
            {
                _node->data = std::move(_node->children[0]->data);
                for (auto& node : _node->children)
                {
                    if (_node->depth == m_impl->maxDepth - 1)
                    {
                        const size_t x = (size_t)node->area.topLeft.x;
                        const size_t y = (size_t)node->area.topLeft.y;
                        m_nodeList2d[x][y] = nullptr;
                    }
                    node.reset();
                }
            }
        }
    }
    void VoxelChonk::_getQuadTreeSize(size_t& _size, QuadNode<float, Voxel>* _node)
    {
        ++_size;
        for (auto& node : _node->children)
            if (node)
                _getQuadTreeSize(_size, node.get());
        /*if (_node)
        {
            if (_node->data)
            {
                ++_size;
            }
            for (auto& node : _node->children)
            {
                _getQuadTreeSize(_size, node.get());
            }
        }*/
    }
}
