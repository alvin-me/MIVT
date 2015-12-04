#include "vrtest.h"


int                 VRTest::windowWidth = 512;
int                 VRTest::windowHeight = 512;
mivt::Application * VRTest::app = 0;
unsigned char*      VRTest::buffer = 0;
unsigned char*      VRTest::bufferRCP = 0;
int                 VRTest::bufferSize = 0;
VRTest::MouseMode   VRTest::mouseMode;
int                 VRTest::lastMouse[2];
bool                VRTest::interactionMode = false;

void VRTest::init()
{
  glewInit();
  glClearDepth(1.0);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  app = new mivt::Application();

  const std::string fileName = "E:/raw/CT-Head/CT-Head.img";
  const std::string format = "SHORT";
  const int dimension[3] = { 512, 512, 393 };
  const float spacing[3] = { 0.47f, 0.47f, 0.63f };
  float intercept = -1024;
  float slope = 1;
  float windowWidth = 300;
  float windowCenter = 30;
  app->LoadVolume(fileName, format, dimension, spacing, intercept, slope, windowWidth, windowCenter);

  app->SetTransfunc("Vascular_Leg_Runoff");
}

void VRTest::deinit()
{
  delete app;
  delete[] buffer;
  delete[] bufferRCP;
}

void VRTest::display()
{
  app->GetPixels(buffer, bufferSize, interactionMode);

  // cause of glut window origin point is at the left-bottom coroner,
  // we should flip the image from bottom to top before call glDrawPixels.
  size_t lineBytes = windowWidth * 4;
  for (int i = 0; i < windowHeight; ++i) {
    memcpy(bufferRCP + (windowHeight - 1 - i) * lineBytes, buffer + i * lineBytes, lineBytes);
  }

  glDisable(GL_DEPTH_TEST);
  glDrawPixels(windowWidth, windowHeight, GL_BGRA, GL_UNSIGNED_BYTE, bufferRCP);
  glEnable(GL_DEPTH_TEST);

  glutSwapBuffers();
}

void VRTest::idle()
{
  glutPostRedisplay();
}

void VRTest::resize(int w, int h)
{
  if (h == 0) {
    return;
  }

  windowWidth = w / 4 * 4;
  windowHeight = h;
  glViewport(0, 0, windowWidth, windowHeight);

  bufferSize = windowWidth * windowHeight * 4;
  if (buffer) {
    delete[] buffer;
    buffer = nullptr;
  }
  if (bufferRCP) {
    delete[] bufferRCP;
    bufferRCP = nullptr;
  }
  buffer = new unsigned char[bufferSize];
  bufferRCP = new unsigned char[bufferSize];

  app->Resize(windowWidth, windowHeight);

  glutPostRedisplay();
}

void VRTest::key(unsigned char k, int, int)
{
  switch (k)
  {
  case 27:
  case 'q':
    exit(0);
    break;
  default:
    break;
  }

  glutPostRedisplay();
}

void VRTest::mouse(int button, int state, int x, int y)
{
  int modifiers = glutGetModifiers();

  lastMouse[0] = x;
  lastMouse[1] = y;

  interactionMode = (state == GLUT_DOWN);

  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    mouseMode = MOUSE_ROTATE;
    break;
  case GLUT_MIDDLE_BUTTON:
    if (modifiers & GLUT_ACTIVE_CTRL) {
      mouseMode = MOUSE_MOVE;
    }
    else {
      mouseMode = MOUSE_WINDOWING;
    }
    break;
  case GLUT_RIGHT_BUTTON:
    mouseMode = MOUSE_ZOOM;
    break;
  default:
    break;
  }

  glutPostRedisplay();
}

void VRTest::motion(int x, int y)
{
  switch (mouseMode) {
  case MOUSE_ROTATE:
    app->Rotate(x, y, lastMouse[0], lastMouse[1]);
    break;
  case MOUSE_ZOOM:
    app->Zoom(x, y, lastMouse[0], lastMouse[1]);
    break;
  case MOUSE_MOVE:
    app->Pan(x, y, lastMouse[0], lastMouse[1]);
    break;
  case MOUSE_WINDOWING:
    break;
  default:
    break;
  }

  lastMouse[0] = x;
  lastMouse[1] = y;

  glutPostRedisplay();
}

int  VRTest::run(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitWindowPosition(500, 0);
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_ALPHA);
  glutCreateWindow("volume rendering test");

  glutDisplayFunc(display);
  glutMotionFunc(motion);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);
  glutKeyboardFunc(key);
  glutReshapeFunc(resize);

  init();

  glutMainLoop();
  deinit();
  return 0;
}
