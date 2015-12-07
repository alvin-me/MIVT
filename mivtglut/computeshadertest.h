#pragma once
#include <GL/glew.h>
#include <GL/glut.h>

#include "application.h"
#include "shadermanager.h"
#include "texture.h"

#pragma warning(disable:4505)

class ComputeShaderTest
{
public:

  static void init();
  static void deinit();
  static void display();
  static void resize(int w, int h);
  static int  run(int argc, char* argv[]);

private:
  static mivt::Application *app;
  static tgt::Shader *shaderProgram;
  static tgt::Texture *texture;
};

