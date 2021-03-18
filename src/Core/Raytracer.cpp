#include "Raytracer.h"
#include <Integrator/WhittedIntegrator.h>
#include <Integrator/PathTracingIntegrator.h>
#include <Integrator/EnvironmentIntegrator.h>
#include <Sampler/DefaultSampler.h>

// 传入线段起点坐标 (x1, y1), 终点坐标 (x2, y2)
void drawLine(int x1, int y1, int x2, int y2, const std::shared_ptr<FrameBuffer>& frameBuffer) {
    bool swp = false;
    int dx = x2 - x1;
    int dy = y2 - y1;
    // 一般假设是x轴的距离大于y轴的距离，如果并非如此就交换x,y
    // 坐标，并且绘制的时候也要交换坐标
    if (std::abs(dx) < std::abs(dy)) {
        std::swap(dx, dy);
        std::swap(x1, y1);
        std::swap(x2, y2);
        swp = true;
    }
    // 如果不是从左到右就交换起点和终点，强行转换到从左到右
    if (dx < 0) {
        std::swap(x1, x2);
        std::swap(y1, y2);
        dx *= -1;
        dy *= -1;
    }
    float slope = (float)dy / dx;
    float curY = y1;
    for (int i = x1; i <= x2; i++) {
        float down = std::abs(curY - std::trunc(curY));
        float up = 1 - down;
        if (swp) {
            frameBuffer->SetPixel((int)curY, i, glm::vec3(up));
            frameBuffer->SetPixel((int)curY + 1, i, glm::vec3(down));
        }
        else {
            frameBuffer->SetPixel(i, (int)curY, glm::vec3(up));
            frameBuffer->SetPixel(i, (int)curY + 1, glm::vec3(down));
        }
        curY += slope;
    }
}

Raytracer::Raytracer(int width, int height) : _width(width), _height(height) {
    _camera = std::shared_ptr<Camera>(new Camera(
        glm::vec3(0, 3, 5),
        glm::vec3(0, 3, -5),
        glm::vec3(0, 1, 0),
        glm::pi<float>() / 3,
        width / static_cast<float>(height),
        1,
        1000
    ));

    _frameBuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height));
    _frameBuffer->Clear();

    auto sampler = std::make_shared<DefaultSampler>(16);
    // _integrator = std::shared_ptr<Integrator>(new PathTracingIntegrator(_camera, sampler));
    _integrator = std::make_shared<WhittedIntegrator>(_camera, sampler, 5);
    fprintf(stdout, "Created\n");
}

void Raytracer::Trace(std::shared_ptr<Scene> scene) {
    clock_t startTime, endTime;
    startTime = clock();

    _integrator->Render(scene, _frameBuffer);
    //------------------------------------------------
    endTime = clock();  //计时结束
    printf("The running time is: %.4fs",
        (double)(endTime - startTime) / CLOCKS_PER_SEC);
}
