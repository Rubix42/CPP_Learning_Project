#pragma once

#include "texture.hpp"

#include <string>
#include <map>

namespace GL{
//TODO
class TexturePool{


private:
    std::map<std::string, GL::Texture2D> textures;
};


}