#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <glm/glm.hpp>

class FrameBuffer {
public:
    FrameBuffer(int width, int height);

    void SetPixel(int x, int y, glm::vec3 color);
    void Clear();
    void Lock();
    void Unlock();
    void ToneReproduction();
    const unsigned char* GetData() const { return _data; }
    const std::vector<unsigned char> GetDataVector() const;

    int GetWidth() const { return _width; }
    int GetHeight()const { return _height; }

private:
    int _width, _height;
    unsigned char* _data;
    std::mutex _mutexLock;
};
