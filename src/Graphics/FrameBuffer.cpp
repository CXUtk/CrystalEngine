#include "FrameBuffer.h"

static constexpr int nChannel = 3;

FrameBuffer::FrameBuffer(int width, int height) :_width(width), _height(height) {
    _data = new unsigned char[width * height * nChannel];
}

void FrameBuffer::SetPixel(int x, int y, glm::vec3 color) {
    if (x < 0 || x >= _width || y < 0 || y >= _height) return;
    //y = _height - 1 - y;
    int start = nChannel * (y * _width + x);

    //color.r = sqrt(color.r);
    //color.g = sqrt(color.g);
    //color.b = sqrt(color.b);

    _data[start] = (unsigned char)floor(glm::clamp(color.r, 0.f, 0.999f) * 256);
    _data[start + 1] = (unsigned char)floor(glm::clamp(color.g, 0.f, 0.999f) * 256);
    _data[start + 2] = (unsigned char)floor(glm::clamp(color.b, 0.f, 0.999f) * 256);
}

void FrameBuffer::Clear() {
    memset(_data, 0, sizeof(unsigned char) * _width * _height * nChannel);
}

void FrameBuffer::Lock() {
    _mutexLock.lock();
}

void FrameBuffer::unlock() {
    _mutexLock.unlock();
}

const std::vector<unsigned char> FrameBuffer::GetDataVector() const {
    std::vector<unsigned char> res;
    res.reserve(_width * _height * (nChannel + 1));
    for (int i = _height - 1; i >= 0; i--) {
        for (int j = 0; j < _width; j++) {
            int id = i * _width + j;
            res.push_back(_data[id * 3]);
            res.push_back(_data[id * 3 + 1]);
            res.push_back(_data[id * 3 + 2]);
            res.push_back(255);
        }
    }
    return res;
}
