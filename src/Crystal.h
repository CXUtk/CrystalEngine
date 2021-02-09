#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include "Graphics/Shaders/ShaderManager.h"
#include "Graphics/ScreenRenderer.h"
#include "Core/Scene.h"
#include "Core/Raytracer.h"

class Crystal {
public:
    // Singleton
    static Crystal& GetInstance();
    ~Crystal();

    void Run();

    std::shared_ptr<ShaderManager> GetShaderManager() const { return _shaderManager; }


private:
    Crystal(int width, int height);

    void init();
    void update();
    void draw();


    int _width, _height;

    GLFWwindow* _window;

    std::shared_ptr<Raytracer> _rayTracer;
    std::shared_ptr<Scene> _scene;
    std::shared_ptr<ScreenRenderer> _screenRenderer;
    std::shared_ptr<ShaderManager> _shaderManager;
};
