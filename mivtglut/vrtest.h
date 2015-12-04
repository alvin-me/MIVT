#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

#include <vector>

#include "application.h"

#pragma warning(disable:4505)

class VRTest
{
public:

  static void init();
  static void deinit();
  static void display();
  static void idle();
  static void resize(int w, int h);
  static void key(unsigned char k, int, int);
  static void mouse(int button, int, int x, int y);
  static void motion(int x, int y);
  static int  run(int argc, char* argv[]);

private:
  static int windowWidth;
  static int windowHeight;

  static mivt::Application *app;
  static unsigned char* buffer;
  static unsigned char* bufferRCP;
  static int bufferSize;

  typedef enum
  {
    MOUSE_ROTATE,
    MOUSE_ZOOM,
    MOUSE_MOVE,
    MOUSE_WINDOWING,
    MOUSE_SCULPT,
  } MouseMode;
  static MouseMode mouseMode;
  static int lastMouse[2];
  static bool interactionMode;
  static bool sculptMode;
  static std::vector<glm::vec2> polygon;
};

