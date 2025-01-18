#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aColor; 
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;

uniform mat4 transform;

out vec3 outPosition;
out vec2 TexCoord;

void main()
{    
    gl_Position = transform * vec4(aPos, 1.0f);

    ourColor = aColor;
    outPosition = aPos;
    TexCoord = aTexCoord;

}       
