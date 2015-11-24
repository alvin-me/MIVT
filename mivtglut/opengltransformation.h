#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

#pragma warning(disable:4505)

namespace tgt {
  class Camera;
  class Trackball;
}

class OpenGLTransformation
{
public:
  static void init(int argc, char* argv[]);
  static void deinit();
  static void display();
  static void idle();
  static void resize(int w, int h);
  static void key(unsigned char k, int, int);
  static void mouse(int button, int, int x, int y);
  static void motion(int x, int y);
  static int  run(int argc, char* argv[]);

private:
  static void drawSub1();
  static void drawSub2();

private:
  static int windowWidth;
  static int windowHeight;

  typedef enum
  {
    MOUSE_NONE,
    MOUSE_ROTATE,
    MOUSE_ZOOM,
    MOUSE_MOVE,
  } MouseMode;
  static MouseMode mouseMode;
  typedef enum
  {
    NONE_VIEW,
    LEFT_VIEW,
    RIGHT_VIEW,
  }ViewMode;
  static ViewMode viewMode;
  static int lastMouse[2];
  static bool interactionMode;

  static tgt::Camera *camera;
  static tgt::Trackball *trackball;
  static tgt::Camera *cameraRight;
  static tgt::Trackball *trackballRight;
};

