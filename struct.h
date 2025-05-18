#ifndef STRUCT_H
#define STRUCT_H

#include <QMetaType>

#define MaxVertexNum 23

/* 边的权值信息 */
typedef struct arcell
{
    int adj;    // 权值(距离)
}arcell,adjmatrix[MaxVertexNum][MaxVertexNum];  // 图的邻接矩阵类型

/* 顶点信息 */
typedef struct vexsinfo
{
    int position;   // 景点的编号
    char name[32];  // 景点的名称
    bool accessible;
}vexsinfo;

/* 图结构信息 */
typedef struct mgraph
{
    vexsinfo vexs[MaxVertexNum];    // 顶点向量（数组）
    adjmatrix arcs;                 // 邻接矩阵
    int vexnum,arcnum;              // 顶点数和边数
}mgraph;

Q_DECLARE_METATYPE(arcell);
Q_DECLARE_METATYPE(vexsinfo);
Q_DECLARE_METATYPE(mgraph);

#endif // STRUCT_H
