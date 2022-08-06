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

void Show(std::vector<Vector3f> frame_buffer);

Vector3f TextureFragShader(const FragShaderIn &in) {
    return in.texture_->GetColor(in.tex_coord_.GetX(), in.tex_coord_.GetY());
}

Vector3f PhoneFragShader(const FragShaderIn &in) {
    // 环境光(ambient)的系数，用来决定物体的底色
    Vector3f ka = {0.005f, 0.005f, 0.005f};
    // 漫反射(diffuse)的系数
    Vector3f kd = in.texture_->GetColor(in.tex_coord_.GetX(), in.tex_coord_.GetY()) * (1.f / 255.f);
    // 镜面(specular)反射的系数
    Vector3f ks = {0.7937f, 0.7937f, 0.7937f};

    Vector3f light_pos = {20.f, 20.f, 20.f};
    Vector3f light_intensity = {500.f, 500.f, 500.f};
    Vector3f amb_light_intensity = {10.f, 10.f, 10.f};
    Vector3f eye_pos = {0.f, 0.f, 10.f};

    // 高光面积系数，p值越大则高光面积越小
    float p = 150.f;

    Vector3f l_a = WiseProduct(ka, amb_light_intensity);

    float r = (light_pos - in.view_pos_).GetLen();
    Vector3f light_ray = (light_pos - in.view_pos_).GetNorm();
    Vector3f l_d = WiseProduct(kd, light_intensity * (1.f / (r * r)) * std::max(0.f, Dot(in.n_, light_ray)));

    Vector3f eye_ray = (eye_pos - in.view_pos_).GetNorm();
    Vector3f h = (light_ray + eye_ray).GetNorm();
    Vector3f l_s = WiseProduct(ks, light_intensity * (1.f / (r * r)) * powf(std::max(0.f, Dot(in.n_, h)), p));

    return (l_a + l_d + l_s) * 255.f;
}

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
        std::cout << "fps:" << 1.f / ((float) (end - start) / CLOCKS_PER_SEC) << std::endl;
    }
    return 0;
}

void InitialScene(Scene &scene) {
//    auto* t_1 = new Triangle();
//    t_1->SetVertexes({Vector4f {2.f,0.f,-2.f,1.f}, Vector4f {0.f,2.f,-2.f,1.f}, Vector4f {-2.f,0.f,-2.f,1.f}});
//    auto* t_2 = new Triangle();
//    t_2->SetVertexes({Vector4f {3.5f,-1.f,-5.f,1.f}, Vector4f {2.5f,1.5f,-5.f,1.f}, Vector4f {-1.f,0.5f,-5.f,1.f}});
    auto *tri_mesh = new TriMesh(R"(D:\Projects\ComputerGraphic\Learn\Renderer\models\cow.obj)");
    auto texture = std::make_shared<Texture>(R"(D:\Projects\ComputerGraphic\Learn\Renderer\models\cow_texture.png)");
    tri_mesh->SetTexture(texture);

    auto *camera = new Camera({0.f, 0.f, 5.f}, {0.f, 0.f, -1.f}, {0.f, 1.f, 0.f});
    camera->SetFrustum((float) kWidth / (float) kHeight, 45.f, -50.f, -0.1f);

    scene.SetCamera(camera);
//    scene.AddTri(t_1);
//    scene.AddTri(t_2);
    scene.AddTriMesh(tri_mesh);
    scene.SetFragShader(PhoneFragShader);
}

void Show(std::vector<Vector3f> frame_buffer) {
    cv::Mat image(kWidth, kHeight, CV_32FC3, frame_buffer.data());
    image.convertTo(image, CV_8UC3, 1.0f);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    cv::imshow("image", image);
}