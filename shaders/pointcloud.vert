#version 450 core

layout (location = 0) in vec3 aPos;
uniform mat4 projMatrix;
uniform mat4 camMatrix;
out vec3 vColor;

void main()
{
    gl_Position = projMatrix * camMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);

    gl_PointSize = 0.1f;

    vColor = vec3(1,1,1);
};

