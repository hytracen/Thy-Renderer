//
// Created by Yoyotao on 2022/7/27.
//

#include "tri_mesh.h"

TriMesh::TriMesh(const std::string &file_name) {
    objl::Loader loader;
    loader.LoadFile(file_name);

    assert(loader.LoadedMeshes.size() == 1);
    auto mesh = loader.LoadedMeshes[0];

    for (int i = 0; i < mesh.Vertices.size(); i += 3) {
        std::array<Vector4f, 3> vertexes{};
        auto *triangle = new Triangle();
        for (int j = 0; j < 3; j++) {
            triangle->SetVertex(j, (Vector3f{mesh.Vertices[i + j].Position.X,
                                             mesh.Vertices[i + j].Position.Y,
                                             mesh.Vertices[i + j].Position.Z} * 1.f
                                    + Vector3f{0.f, 0.f, 0.f}).GetHomoCoordinate(1));
            triangle->SetNormal(j, {mesh.Vertices[i + j].Normal.X, mesh.Vertices[i + j].Normal.Y,
                                    mesh.Vertices[i + j].Normal.Z});
            triangle->SetTexCoord(j,
                                  {mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y});

        }
        triangles_.push_back(triangle);
    }
}

const std::vector<Triangle *> &TriMesh::GetTriList() {
    return triangles_;
}

void TriMesh::RotateBy(Vector3f axis, float angle) {
    for (auto &t: triangles_) {
        t->RotateBy(axis, angle);
    }
}

void TriMesh::MoveBy(Vector3f dir, float len) {
    for (auto &t: triangles_) {
        t->MoveBy(dir, len);
    }
}

void TriMesh::SetTexture(const std::shared_ptr<Texture> &texture) {
    texture_ = texture;
}

const std::shared_ptr<Texture> &TriMesh::GetTexture() const {
    return texture_;
}
