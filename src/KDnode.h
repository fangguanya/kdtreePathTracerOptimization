#pragma once

#include <stdlib.h>
#include <vector>
#include <stack>


// bare versions are for compressed data to reduce overhead
// a change in data with significant impact:
//size of:
//      triangle: 116
//      tribare : 76
//      node : 136
//      nodebare : 64

namespace KDN
{
    // Used to prototype.  Not used for triangle KD tree.
    class Point
    {
    public:
        float x;
        float y;
        float z;

        Point()
        {
            x = 0;
            y = 0;
            z = 0;
        }

        Point(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Point(const Point& p)
        {
            x = p.x;
            y = p.y;
            z = p.z;
        }

        ~Point() {}
    };

    // these structs are compressed versions of the classes
    struct TriBare
    {
        float x1, x2, x3,
            y1, y2, y3,
            z1, z2, z3;

        float nx1, nx2, nx3,
            ny1, ny2, ny3,
            nz1, nz2, nz3;

        int mtlIdx;
    };

    struct NodeBare
    {
        int axis;
        float splitPos = 0.0;

        float mins[3];
        float maxs[3];

        int ID;
        int parentID;
        int leftID;
        int rightID;

        int triIdStart;
        int triIdSize;

        float tmin=0.0;
        float tmax=0.0;
    };

    // triangle storage for maintaining triangle information
    // should probably expand to include normals information
    class Triangle
    {
    public:
        int nodeID;
        float x1, x2, x3,
              y1, y2, y3,
              z1, z2, z3;

        float nx1, nx2, nx3,
            ny1, ny2, ny3,
            nz1, nz2, nz3;

        float center[3];
        float mins[3];
        float maxs[3];

        int mtlIdx;

        Triangle()
        {
            x1 = 0.0; y1 = 0.0; z1 = 0.0;
            x2 = 0.0; y2 = 0.0; z2 = 0.0;
            x3 = 0.0; y3 = 0.0; z3 = 0.0;

            nx1 = 0.0; ny1 = 0.0; nz1 = 0.0;
            nx2 = 0.0; ny2 = 0.0; nz2 = 0.0;
            nx3 = 0.0; ny3 = 0.0; nz3 = 0.0;

            center[0] = 0.0;
            center[1] = 0.0;
            center[2] = 0.0;
            computeBounds();
            nodeID = -1;
            mtlIdx = -1;
        }

        Triangle(float X1, float Y1, float Z1,
                 float X2, float Y2, float Z2,
                 float X3, float Y3, float Z3)
        {
            x1 = X1; y1 = Y1; z1 = Z1;
            x2 = X2; y2 = Y2; z2 = Z2;
            x3 = X3; y3 = Y3; z3 = Z3;

            nx1 = 0.0; ny1 = 0.0; nz1 = 0.0;
            nx2 = 0.0; ny2 = 0.0; nz2 = 0.0;
            nx3 = 0.0; ny3 = 0.0; nz3 = 0.0;

            computeCentroid();
            computeBounds();
            nodeID = -1;
            mtlIdx = -1;
        }

        Triangle(float X1, float Y1, float Z1,
                 float X2, float Y2, float Z2,
                 float X3, float Y3, float Z3,
                 float NX1, float NY1, float NZ1,
                 float NX2, float NY2, float NZ2,
                 float NX3, float NY3, float NZ3)
        {
            x1 = X1; y1 = Y1; z1 = Z1;
            x2 = X2; y2 = Y2; z2 = Z2;
            x3 = X3; y3 = Y3; z3 = Z3;

            nx1 = NX1; ny1 = NY1; nz1 = NZ1;
            nx2 = NX2; ny2 = NY2; nz2 = NZ2;
            nx3 = NX3; ny3 = NY3; nz3 = NZ3;

            computeCentroid();
            computeBounds();
            nodeID = -1;
            mtlIdx = -1;
        }

        ~Triangle() {}

        void setValues(float val)
        {
            x1 = val; y1 = val; z1 = val;
            x2 = val; y2 = val; z2 = val;
            x3 = val; y3 = val; z3 = val;

            nx1 = 0.0; ny1 = 0.0; nz1 = 0.0;
            nx2 = 0.0; ny2 = 0.0; nz2 = 0.0;
            nx3 = 0.0; ny3 = 0.0; nz3 = 0.0;

            center[0] = val; center[1] = val; center[2] = val;
            computeBounds();
        }

        void setValues(float X1, float Y1, float Z1,
                       float X2, float Y2, float Z2,
                       float X3, float Y3, float Z3)
        {
            x1 = X1; y1 = Y1; z1 = Z1;
            x2 = X2; y2 = Y2; z2 = Z2;
            x3 = X3; y3 = Y3; z3 = Z3;

            computeCentroid();
            computeBounds();
        }


        void setValues(float X1, float Y1, float Z1,
                       float X2, float Y2, float Z2,
                       float X3, float Y3, float Z3,
                       float NX1, float NY1, float NZ1,
                       float NX2, float NY2, float NZ2,
                       float NX3, float NY3, float NZ3)
        {
            x1 = X1; y1 = Y1; z1 = Z1;
            x2 = X2; y2 = Y2; z2 = Z2;
            x3 = X3; y3 = Y3; z3 = Z3;

            nx1 = NX1; ny1 = NY1; nz1 = NZ1;
            nx2 = NX2; ny2 = NY2; nz2 = NZ2;
            nx3 = NX3; ny3 = NY3; nz3 = NZ3;

            computeCentroid();
            computeBounds();
        }

        void computeCentroid()
        {
            center[0] = (x1 + x2 + x3) / 3.0;
            center[1] = (y1 + y2 + y3) / 3.0;
            center[2] = (z1 + z2 + z3) / 3.0;
        }

        void computeBounds()
        {
            mins[0] = x1 < x2 ? (x1 < x3 ? x1 : x3) : (x2 < x3 ? x2 : x3);
            mins[1] = y1 < y2 ? (y1 < y3 ? y1 : y3) : (y2 < y3 ? y2 : y3);
            mins[2] = z1 < z2 ? (z1 < z3 ? z1 : z3) : (z2 < z3 ? z2 : z3);

            maxs[0] = x1 > x2 ? (x1 > x3 ? x1 : x3) : (x2 > x3 ? x2 : x3);
            maxs[1] = y1 > y2 ? (y1 > y3 ? y1 : y3) : (y2 > y3 ? y2 : y3);
            maxs[2] = z1 > z2 ? (z1 > z3 ? z1 : z3) : (z2 > z3 ? z2 : z3);
        }
    };

    // bounding box class for creating bounds and setting node bounds
    class BoundingBox
    {
    public:
        float mins[3];
        float maxs[3];
        //float centerx, centery, centerz;
        float center[3];
        float size[3];

        BoundingBox()
        {
            setBounds(0.0);
        }

        BoundingBox(float xMin, float yMin, float zMin,
                    float xMax, float yMax, float zMax)
        {
            setBounds(mins[0], mins[1], mins[2],
                      maxs[0], maxs[1], maxs[2]);
            updateCentroid();
            updateSize();

        }

        BoundingBox(Triangle* t)
        {
            setBounds(t);
            updateCentroid();
            updateSize();
        }

        void setBounds(Triangle* t)
        {
            mins[0] = t->x1 < t->x2 ? (t->x1 < t->x3 ? t->x1 : t->x3) : (t->x2 < t->x3 ? t->x2 : t->x3);
            mins[1] = t->y1 < t->y2 ? (t->y1 < t->y3 ? t->y1 : t->y3) : (t->y2 < t->y3 ? t->y2 : t->y3);
            mins[2] = t->z1 < t->z2 ? (t->z1 < t->z3 ? t->z1 : t->z3) : (t->z2 < t->z3 ? t->z2 : t->z3);

            maxs[0] = t->x1 > t->x2 ? (t->x1 > t->x3 ? t->x1 : t->x3) : (t->x2 > t->x3 ? t->x2 : t->x3);
            maxs[1] = t->y1 > t->y2 ? (t->y1 > t->y3 ? t->y1 : t->y3) : (t->y2 > t->y3 ? t->y2 : t->y3);
            maxs[2] = t->z1 > t->z2 ? (t->z1 > t->z3 ? t->z1 : t->z3) : (t->z2 > t->z3 ? t->z2 : t->z3);
            updateCentroid();
            updateSize();
        }

        void updateCentroid()
        {
            center[0] = (mins[0] + maxs[0]) / 2.0;
            center[1] = (mins[1] + maxs[1]) / 2.0;
            center[2] = (mins[2] + maxs[2]) / 2.0;
            updateSize();
        }

        void setBounds(float xMin, float yMin, float zMin,
                       float xMax, float yMax, float zMax)
        {
            mins[0] = xMin; mins[1] = yMin; mins[2] = zMin;
            maxs[0] = xMax; maxs[1] = yMax; maxs[2] = zMax;
            updateCentroid();

        }

        void updateSize()
        {
            size[0] = mins[0] < maxs[0] ? maxs[0] - mins[0] : mins[0] - maxs[0];
            size[1] = mins[1] < maxs[1] ? maxs[1] - mins[1] : mins[1] - maxs[1];
            size[2] = mins[2] < maxs[2] ? maxs[2] - mins[2] : mins[2] - maxs[2];
        }

        void setBounds(float val)
        {
            mins[0] = val; mins[1] = val; mins[2] = val;
            maxs[0] = val; maxs[1] = val; maxs[2] = val;
            center[0] = val; center[1] = val; center[2] = val;
            size[0] = 0.0; size[1] = 0.0; size[2] = 0.0;
            // centery = val; centerz = val;
        }
    };

    // kn node class where the bounds and triangles are defined
    // this is where the logic happens
    class KDnode
    {
    public:
        int axis;
        float splitPos;
        bool visited;
        KDnode* parent;
        KDnode* left;
        KDnode* right;
        BoundingBox bbox;
        std::vector< Triangle* > triangles;
        
        int ID;
        int parentID;
        int leftID;
        int rightID;

        int triIdStart;
        int triIdSize;

        KDnode();
        KDnode(Triangle* t);
        KDnode(Triangle* t, int axis = 0);
        KDnode(Triangle** t, int size, int axis = 0);
        ~KDnode();

        KDnode* getRoot();
        void updateTriangleBbox(Triangle* t);
        void mergeBbox(BoundingBox b);
        BoundingBox updateBbox();
        void split(int maxdepth);
        void deleteTree(KDnode* root);
        void printTriangleCenters();
        void printTree(KDnode* root);
        int getDepth(KDnode* n, int depth = 0);
        int getLevel(KDnode* n);
        void add(Triangle* t);
        void updateBbox(KDnode* n);
        bool operator<(const KDnode& rhs);
    };


    class NodeStack
    {
    public:
        std::stack<KDnode> nodes;
        void push(KDnode);
        void pop();
        KDnode top();
    };

    class NodeStackBare
    {
    public:
        std::stack<NodeBare> nodes;
        void push(NodeBare);
        void pop();
        NodeBare top();
    };
}

