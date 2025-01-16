#version 330 core
out vec4 finalColor;  
in vec3 ourColor;
in vec3 outPosition;
void main()
{
    finalColor = vec4(ourColor, 1.0);
}
