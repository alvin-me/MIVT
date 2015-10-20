#version 330

layout (location = 0) in vec4 vertexPosition;
layout (location = 1) in vec4 vertexColor;

uniform mat4 modelMatrix_;
uniform mat4 viewMatrix_;
uniform mat4 projectionMatrix_;

smooth out vec4 color;

void main()
{
  color = vertexColor;  // texture coordinate.

  gl_Position = (projectionMatrix_ * viewMatrix_ * modelMatrix_) * vertexPosition;
}