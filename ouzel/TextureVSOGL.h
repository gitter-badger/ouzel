// Copyright (C) 2015 Elviss Strazdins
// This file is part of the Ouzel engine.

const uint8_t TEXTURE_VERTEX_SHADER_OGL[] =
    "#version 400\n"
    "layout(location=0) in vec4 in_Position;\n"
    "layout(location=1) in vec4 in_Color;\n"
    "layout(location=2) in vec2 in_TexCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 proj;\n"
    "out vec4 ex_Color;\n"
    "out vec2 ex_TexCoord;\n"
    "void main(void)\n"
    "{\n"
    "    gl_Position = proj * view * model * in_Position;\n"
    "    ex_Color = in_Color;\n"
    "    ex_TexCoord = in_TexCoord;\n"
    "}";
