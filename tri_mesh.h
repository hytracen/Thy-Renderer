//
// Created by Yoyotao on 2022/7/27.
//

#ifndef RENDERER_TRI_MESH_H
#define RENDERER_TRI_MESH_H

#include <vector>

#include "Utils/matrix.h"
#include "Utils/OBJ_Loader.h"
#include "triangle.h"

class TriMesh {
public:
    TriMesh(const std::string &file_name);

    const std::vector<Triangle *> &GetTriList();

    void RotateBy(Vector3f axis, float angle);

    void MoveBy(Vector3f dir, float len);

private:
    std::vector<Triangle *> triangles_;
};


#endif //RENDERER_TRI_MESH_H
