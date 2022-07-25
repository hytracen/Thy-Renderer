#include <iostream>
#include <fstream>
#include <vector>

#include "Utils/numeric.h"
#include "Utils/matrix.h"
#include "triangle.h"
#include "scene.h"

int main() {
    std::vector<std::vector<Vector3f>> frame_buffer(kHeight, std::vector<Vector3f>(kWidth, Vector3f()));

    auto* t_1 = new Triangle();
    t_1->SetVertexes({Vector4f {2.f,0.f,-2.f,1.f}, Vector4f {0.f,2.f,-2.f,1.f}, Vector4f {-2.f,0.f,-2.f,1.f}});
//    auto* t_2 = new Triangle();
//    t_2->SetVertexes({Vector4f {0.f,300.f,-5.f,1.f}, Vector4f {0.f,0.f,-5.f,1.f}, Vector4f {300.f,300.f,-5.f,1.f}});

    Scene scene;
    scene.AddTri(t_1);
//    scene.AddTri(t_2);
    scene.Render(frame_buffer);

    std::string file_name = "Image.ppm";
    std::ofstream out_file(file_name);
    out_file << "P3\n" << kWidth << " " << kHeight << " \n" << "255\n";
    for (int i = kHeight - 1; i >= 0; --i) {
        for (int j = 0; j < kWidth; ++j) {
            Vector3f color = frame_buffer.at(i).at(j);
            // gamma矫正
            float r_f = powf(color.GetX(), 1.f / 2.2f);
            float g_f = powf(color.GetY(), 1.f / 2.2f);
            float b_f = powf(color.GetZ(), 1.f / 2.2f);
            int r_int = static_cast<int>(Numeric::Clamp(r_f, kEpsilon, 0.999999f) * 255.99f);
            int g_int = static_cast<int>(Numeric::Clamp(g_f, kEpsilon, 0.999999f) * 255.99f);
            int b_int = static_cast<int>(Numeric::Clamp(b_f, kEpsilon, 0.999999f) * 255.99f);
            out_file << r_int << " " << g_int << " " << b_int << "\n";
        }
    }
    out_file.close();

    return 0;
}