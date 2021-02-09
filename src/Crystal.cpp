#include "Crystal.h"
#include <thread>
#include <lodepng/lodepng.h>

Crystal& Crystal::GetInstance() {
    static Crystal instance(800, 600);
    return instance;
}

Crystal::~Crystal() {
}


double oldTime;
void Crystal::Run() {
    std::thread thread([&]() {
        _rayTracer->Trace(_scene);
        std::vector<unsigned char> png;
        unsigned error = lodepng::encode(png, _rayTracer->GetFrameBuffer()->GetDataVector(), _width, _height);
        if (!error) lodepng::save_file(png, "result.png");
        if (error) {
            printf("Encoder error %d: %s\n", error, lodepng_error_text(error));
        }
        else {
            printf("Saved!\n");
        }
    });

    oldTime = glfwGetTime();
    while (!glfwWindowShouldClose(_window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update();
        draw();

        glfwSwapBuffers(_window);
        while (glfwGetTime() - oldTime < 1.0 / 60.0) {
            glfwPollEvents();
        }
        double curTime = glfwGetTime();
        oldTime = curTime;
    }
    thread.join();
}


static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

Crystal::Crystal(int width, int height) :_width(width), _height(height) {
    init();
}

void Crystal::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(_width, _height, "Crystal", nullptr, nullptr);
    if (!_window) {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to load glad!\n");
        glfwTerminate();
        return;
    }
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);


    _shaderManager = std::make_shared<ShaderManager>();
    _scene = std::make_shared<Scene>();
    _rayTracer = std::shared_ptr<Raytracer>(new Raytracer(_width, _height));
    _screenRenderer = std::make_shared<ScreenRenderer>();
}

void Crystal::update() {
    auto framebuffer = _rayTracer->GetFrameBuffer();
    framebuffer->Lock();
    _screenRenderer->copyTexture(framebuffer->GetData(), _width, _height);
    framebuffer->unlock();
}

void Crystal::draw() {
    _screenRenderer->draw();
}
