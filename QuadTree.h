#pragma once
#include <cstdint> 
#include <memory>
#include <iostream>
#include <functional>
#include <vector>
#include <math.h> 

#include "SimplexNoise.h"
#include "Voxel.h"

template < class precision >
struct Point
{
    const precision x = 0;
    const precision y = 0;
    // Function to calculate distance
    double distanceToPoint(const Point p) const
    {
        return sqrt(pow(p.x - x, 2) +
            pow(p.y - y, 2) * 1.0);
    }
};
using Pointf = Point<float>;
using Pointd = Point<double>;

template < class precision>
struct Quad
{
    const Point<precision> topLeft;
    const Point<precision> bottomRight;
    Point<precision> getMidPoint() const
    {
        precision x = { topLeft.x + ((bottomRight.x - topLeft.x) / 2) };
        precision y = { topLeft.y + ((bottomRight.y - topLeft.y) / 2) };
        return Point<precision>{ x, y };
    }
};


template < class precision, class dataType>
struct QuadNode
{
    const QuadNode<precision, dataType>* parent;
    std::unique_ptr<QuadNode<precision, dataType>>   children[4];
    const Quad<precision>       area;
    const uint8_t               depth;
    bool                        active;
    std::unique_ptr<dataType>   data;
    QuadNode(QuadNode<precision, dataType>* _parent, Quad<precision> _quad, uint8_t _depth, bool _active = false)
        : area(_quad), depth(_depth), active(_active), data(nullptr), parent(_parent)
    {}
};

template < class precision, class dataType>
class QuadTree
{
public:
    QuadTree(size_t _size, size_t _maxDepth, Point<precision> _origin)
        : m_maxDepth(_maxDepth), m_gridSize(_size)
    {

        /*for (size_t i = 0; i < _size; ++i)
        {
            m_quadNodeVector.push_back(std::vector<QuadNode<precision, dataType>*>());
            for (size_t j = 0; j < _size; ++j)
                m_quadNodeVector[i].push_back(nullptr);
        }*/

        const Quad<precision> bounds = {
            Point<precision>{_origin.x, _origin.y},
            Point<precision>{_origin.x+(precision)_size, _origin.y+(precision)_size}
        };
        m_parentNode = std::make_unique<QuadNode<precision, dataType>>(nullptr, bounds, 0, true);
    }

    size_t size()
    {
        size_t count = 0;
        _quadTreeSize(m_parentNode.get(), count);
        return count;
        //size_t count = 0;
        /*for (size_t i = 0; i < m_gridSize; ++i)
            for (size_t j = 0; j < m_gridSize; ++j)
                if (m_quadNodeVector[i][j])
                    ++count;
        return count;*/
    }

    //void circle(const Point<precision>& p, double radius, bool addition, voxel::VOXEL_TYPE type)
    //{
    //    auto fun = [&](const Point<precision>) {return radius; };
    //    _circle(m_parentNode.get(), p, fun, addition, type);
    //}

    //void addNoise(
    //    size_t octaves = 4, 
    //    float frequency = 1.0f,
    //    float amplitude = 1.0f,
    //    float lacunarity = 2.0f,
    //    float persistence = 0.5f,
    //    bool addition = true)
    //{
    //    buildTree(m_parentNode.get(), true);
    //    SimplexNoise simplexNoise(frequency, amplitude, lacunarity, persistence);
    //    for (size_t i = 0; i < m_gridSize; ++i)
    //    {
    //        for (size_t j = 0; j < m_gridSize; ++j)
    //        {
    //            const auto point = m_quadNodeVector[i][j]->area.getMidPoint();
    //            m_quadNodeVector[i][j]->active = true;
    //            if (simplexNoise.fractal(octaves, point.x, point.y) < 0.1)
    //            {
    //                m_quadNodeVector[i][j]->data = std::make_unique<voxel::Voxel>();
    //                m_quadNodeVector[i][j]->data->type = voxel::VOXEL_TYPE::water;
    //            }
    //            if (simplexNoise.fractal(octaves, point.x, point.y) > 0.1)
    //            {
    //                m_quadNodeVector[i][j]->data = std::make_unique<voxel::Voxel>();
    //                m_quadNodeVector[i][j]->data->type = voxel::VOXEL_TYPE::sand;
    //            }
    //            if (simplexNoise.fractal(octaves, point.x, point.y) > 0.4)
    //            {
    //                m_quadNodeVector[i][j]->data = std::make_unique<voxel::Voxel>();
    //                m_quadNodeVector[i][j]->data->type = voxel::VOXEL_TYPE::dirt;
    //            }
    //        }
    //    }
    //    // m_quadNodeVector
    //}

    QuadNode<precision, dataType>* getParentNode() const 
    {
        return m_parentNode.get();
    }


    void buildTree(QuadNode<precision, dataType>* _node, bool _recursive)
    {
        if (_node->depth >= m_maxDepth)
        {
            //const size_t x = (size_t)_node->area.topLeft.x;
            //const size_t y = (size_t)_node->area.topLeft.y;
            //m_quadNodeVector[x][y] = _node;
            // printf("topLeft: x:%f, y:%f \n", _node->area.topLeft.x, _node->area.topLeft.y);
            return;
        }
        const Point<precision>& topleft = _node->area.topLeft;
        // const Point& topRight = _node->area.topRight;
        // const Point& botLeft = _node->area.bottomLeft;
        const Point<precision>& botRight = _node->area.bottomRight;

        // new points
        const Point<precision> TOP_LEFT = topleft;
        const Point<precision> TOP_MID = { topleft.x + ((botRight.x - topleft.x) / 2), topleft.y };
        //const Point<precision> TOP_RIGHT = { topleft.x, botRight.y };
        const Point<precision> MID_LEFT = { topleft.x, topleft.y + ((botRight.y - topleft.y) / 2) };
        const Point<precision> MID_MID = { topleft.x + ((botRight.x - topleft.x) / 2), topleft.y + ((botRight.y - topleft.y) / 2) };
        const Point<precision> MID_RIGHT = { botRight.x, topleft.y + ((botRight.y - topleft.y) / 2) };
        //const Point<precision> BOT_LEFT = { topleft.x, botRight.y };
        const Point<precision> BOT_MID = { topleft.x + ((botRight.x - topleft.x) / 2), botRight.y };
        const Point<precision> BOT_RIGHT = botRight;

        // Top left quad
        _node->children[0] = std::make_unique<QuadNode<precision, dataType>>(_node, Quad<precision>{ TOP_LEFT, MID_MID }, _node->depth + 1, false);
        // Top right quad
        _node->children[1] = std::make_unique<QuadNode<precision, dataType>>(_node, Quad<precision>{ TOP_MID, MID_RIGHT }, _node->depth + 1, false);
        // Bottom left quad
        _node->children[2] = std::make_unique<QuadNode<precision, dataType>>(_node, Quad<precision>{ MID_LEFT, BOT_MID }, _node->depth + 1, false);
        // Bottom right quad
        _node->children[3] = std::make_unique<QuadNode<precision, dataType>>(_node, Quad<precision>{ MID_MID, BOT_RIGHT }, _node->depth + 1, false);

        if (_recursive)
        {
            for (auto& node : _node->children)
            {
                buildTree(node.get(), true);
            }
        }
    }

    //std::vector<std::vector<QuadNode<precision, dataType>*>> m_quadNodeVector;


    bool isInsideQuad(const QuadNode<precision, dataType>* _node, const Point<precision>& p)
    {
        if (p.x < _node->area.topLeft.x || p.x > _node->area.bottomRight.x)
            return false;
        if (p.y < _node->area.topLeft.y || p.y > _node->area.bottomRight.y)
            return false;
        return true;
    }

    bool isInRange(const QuadNode<precision, dataType>* _node, const Point<precision>& p, const double& radius)
    {
        // test topLeft point
        if (p.distanceToPoint(_node->area.topLeft) <= radius)
            return true;
        // test topRight point
        if (p.distanceToPoint(Point<precision>{ _node->area.bottomRight.x, _node->area.topLeft.y }) <= radius)
            return true;
        // test bottomLeft point
        if (p.distanceToPoint(Point<precision>{ _node->area.topLeft.x, _node->area.bottomRight.y }) <= radius)
            return true;
        // test bottomRight point
        if (p.distanceToPoint(_node->area.bottomRight) <= radius)
            return true;
        // all points not in range so this node is not in range
        return false;
    }

private:

    std::unique_ptr<QuadNode<precision, dataType>> m_parentNode;
    size_t m_maxDepth;
    size_t m_gridSize;



    void _quadTreeSize(QuadNode<precision, dataType>* _node, size_t& _count)
    {
        ++_count;
        if (_node->depth == m_maxDepth)
            return;
        for (auto& node : _node->children)
            if (node)
                _quadTreeSize(node.get(), _count);
    }

    /*void _circle(QuadNode<precision, dataType>* _node, const Point<precision>& p, const std::function<double(Point<precision>)>& radiusFn, bool addition, voxel::VOXEL_TYPE type)
    {
        if (isInsideQuad(_node, p) || isInRange(_node, p, radiusFn(_node->area.getMidPoint())))
        {
            if (_node)
            {
                if (_node->depth >= m_maxDepth)
                {
                    _node->active = addition;
                    _node->data = std::make_unique<voxel::Voxel>();
                    _node->data->type = voxel::VOXEL_TYPE::dirt;
                    const size_t x = (size_t)_node->area.topLeft.x;
                    const size_t y = (size_t)_node->area.topLeft.y;
                    m_quadNodeVector[x][y] = addition ? _node : nullptr;
                    return;
                }
                if (!_node->children[0])
                    buildTree(_node, false);
                for (auto& node : _node->children)
                {
                    if (node)
                    {
                        _circle(node.get(), p, radiusFn, addition, type);
                    }
                }
            }
        }
    }*/
};

