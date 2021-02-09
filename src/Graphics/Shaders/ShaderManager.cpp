#include "ShaderManager.h"

static std::string defaultVS = "Resources/Shaders/default.vs";
static std::string defaultFS = "Resources/Shaders/default.frag";

ShaderManager::ShaderManager() {
    auto default2Shader = std::make_shared<ShaderData>(ShaderData::loadShaderProgram(defaultVS, defaultFS));
    _shaders["default"] = default2Shader;
}

ShaderManager::~ShaderManager() {
}

void ShaderManager::update(float delta) {
}
