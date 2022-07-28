#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "Utils/numeric.h"
#include "Utils/matrix.h"
#include "triangle.h"
#include "scene.h"

const int kWidth = 600;
const int kHeight = 600;

void InitialScene(Scene &scene);

void Show(const std::vector<std::vector<Vector3f>> &frame_buffer);

int main() {
    Scene scene{kWidth, kHeight};
    InitialScene(scene);

    int pressed_key = 0;
    clock_t start, end;
    while (pressed_key != 27) {
        start = clock();

        scene.ProcessInput(pressed_key);
        scene.Render();
        Show(scene.GetFrameBuffer());
        pressed_key = cv::waitKey(10);
        scene.Clear();
        system("cls");

        end = clock();
        std::cout << "fps:" << 1.f / ((float)(end - start) / CLOCKS_PER_SEC) << std::endl;
    }
    return 0;
}

void InitialScene(Scene &scene) {
//    auto* t_1 = new Triangle();
//    t_1->SetVertexes({Vector4f {2.f,0.f,-2.f,1.f}, Vector4f {0.f,2.f,-2.f,1.f}, Vector4f {-2.f,0.f,-2.f,1.f}});
//    auto* t_2 = new Triangle();
//    t_2->SetVertexes({Vector4f {3.5f,-1.f,-5.f,1.f}, Vector4f {2.5f,1.5f,-5.f,1.f}, Vector4f {-1.f,0.5f,-5.f,1.f}});
    auto * tri_mesh = new TriMesh(R"(D:\Projects\ComputerGraphic\Learn\Renderer\models\cow.obj)");

    auto *camera = new Camera({0.f, 0.f, 5.f}, {0.f,0.f,-1.f}, {0.f,1.f,0.f});
    camera->SetFrustum((float)kWidth / (float)kHeight, 45.f, -50.f, -0.1f);

    scene.SetCamera(camera);
//    scene.AddTri(t_1);
//    scene.AddTri(t_2);
    scene.AddTriMesh(tri_mesh);
}

void Show(const std::vector<std::vector<Vector3f>> &frame_buffer) {
    std::vector<Vector3f> frame_buffer_1d(kWidth * kHeight);
    for (int i = 0; i < kWidth; ++i) {
        for (int j = 0; j < kHeight; ++j) {
            frame_buffer_1d.at(i * kWidth + j) = frame_buffer.at(kHeight - 1 - i).at(j) * 255.99f; // todo:二维数组改成一维数组
        }
    }
    cv::Mat image(kWidth, kHeight, CV_32FC3, frame_buffer_1d.data());
    image.convertTo(image, CV_8UC3, 1.0f);
    cv::imshow("image", image);
}