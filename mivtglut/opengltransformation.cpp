#include "opengltransformation.h"
#include "logmanager.h"
#include "filesystem.h"
#include "gpucapabilities.h"
#include "shadermanager.h"
#include "matrixstack.h"
#include "model.h"
#include "camera.h"
#include "trackball.h"

int  OpenGLTransformation::windowWidth = 1024;
int  OpenGLTransformation::windowHeight = 512;
int  OpenGLTransformation::lastMouse[2];
bool OpenGLTransformation::interactionMode = false;
OpenGLTransformation::MouseMode   OpenGLTransformation::mouseMode = MOUSE_NONE;
OpenGLTransformation::ViewMode    OpenGLTransformation::viewMode = NONE_VIEW;

tgt::Camera * OpenGLTransformation::camera = 0;
tgt::Trackball * OpenGLTransformation::trackball = 0;
tgt::Camera * OpenGLTransformation::cameraRight = 0;
tgt::Trackball * OpenGLTransformation::trackballRight = 0;

void OpenGLTransformation::init(int, char* argv[])
{
  // opengl init
  {
    glewInit();

    glShadeModel(GL_SMOOTH);                        // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);          // 4-byte pixel alignment

    // enable/disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_SCISSOR_TEST);

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0.f, 0.f, 0.f, 0.f);               // background color
    glClearStencil(0);                              // clear stencil buffer
    glClearDepth(1.0f);                             // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = { .0f, .0f, .0f, 1.0f };      // ambient light
    GLfloat lightKd[] = { .9f, .9f, .9f, 1.0f };      // diffuse light
    GLfloat lightKs[] = { 1, 1, 1, 1 };               // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light in eye space
    float lightPos[4] = { 0, 1, 1, 0 };               // directional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);
  }

  // basic init
  {
    tgt::LogManager::init();
    tgt::FileSystem::init();
    tgt::GpuCapabilities::init();
    tgt::ShaderManager::init();
    tgt::MatrixStack::init();

    std::string basePath = argv[0];
    basePath = tgt::FileSystem::findWithSubDir(basePath, "resource/transfuncs", 7);
    basePath = tgt::FileSystem::absolutePath(basePath);
    ShdrMgr.addPath(tgt::FileSystem::cleanupPath(basePath + "/mivt/glsl"));
    ShdrMgr.addPath(tgt::FileSystem::cleanupPath(basePath + "/mivt/glsl/base"));
    ShdrMgr.addPath(tgt::FileSystem::cleanupPath(basePath + "/mivt/glsl/modules"));
  }

  // scene init
  {
    /*camera = new tgt::Camera(glm::vec3(0, 0, 10), glm::vec3(0), glm::vec3(0, 1, 0), 60.f,
      (float)windowWidth * 0.5f / windowHeight, 1, 10);*/

    const float focusToOrigin = 10; // length from eye focus to the original point.
    glm::vec3 eye(0, 0, focusToOrigin);
    float nearPlane = 1.f;// focusToOrigin - 2;
    float farPlane = focusToOrigin + 2;
    float ratio = (float)windowWidth * 0.5f / windowHeight;
    float fovy = atan(1 / ratio / nearPlane) / DEG2RAD * 2;
    camera = new tgt::Camera(eye, glm::vec3(0), glm::vec3(0, 1, 0), fovy, ratio, nearPlane, farPlane);

    trackball = new tgt::Trackball(camera);
    trackball->setSize(0.7f); // sets trackball sensitivity

    cameraRight = new tgt::Camera(glm::vec3(0, 0, 25), glm::vec3(0), glm::vec3(0, 1, 0), 60.f,
      (float)windowWidth * 0.5f / windowHeight, 1, 100);
    trackballRight = new tgt::Trackball(cameraRight);
    trackballRight->setSize(0.7f);
    trackballRight->rotate(glm::vec3(0, 1, 0), 45.0f);
    trackballRight->rotate(glm::vec3(1, 0, 0), -45.0f);
  }
}

void OpenGLTransformation::deinit()
{
  delete camera;
  delete trackball;
  delete cameraRight;
  delete trackballRight;
}

void OpenGLTransformation::display()
{
  drawSub1();
  drawSub2();

  glutSwapBuffers();
}

void OpenGLTransformation::drawSub1()
{
  glViewport(0, 0, windowWidth / 2, windowHeight);
  glScissor(0, 0, windowWidth / 2, windowHeight);

  glClearColor(0.1f, 0.1f, 0.1f, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  MatStack.matrixMode(tgt::MatrixStack::PROJECTION);
  MatStack.pushMatrix();
  MatStack.loadMatrix(camera->getProjectionMatrix(glm::ivec2(windowWidth * 0.5f, windowHeight)));
  MatStack.matrixMode(tgt::MatrixStack::MODELVIEW);
  MatStack.pushMatrix();
  MatStack.loadMatrix(camera->getViewMatrix());

  // always draw the grid at the origin (before any modeling transform)
  drawGrid(10, 1);
  drawAxis(4);

  glm::mat4 cubeModel = glm::translate(glm::vec3(-0.5f));
  MatStack.pushMatrix();
  MatStack.loadMatrix(camera->getViewMatrix() * cubeModel);
  drawCube();
  MatStack.popMatrix();

  MatStack.matrixMode(tgt::MatrixStack::PROJECTION);
  MatStack.popMatrix();
  MatStack.matrixMode(tgt::MatrixStack::MODELVIEW);
  MatStack.popMatrix();
}

void OpenGLTransformation::drawSub2()
{
  glViewport(windowWidth / 2, 0, windowWidth / 2, windowHeight);
  glScissor(windowWidth / 2, 0, windowWidth / 2, windowHeight);

  glClearColor(0.f, 0.f, 0.f, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  MatStack.matrixMode(tgt::MatrixStack::PROJECTION);
  MatStack.pushMatrix();
  MatStack.loadMatrix(cameraRight->getProjectionMatrix(glm::ivec2(windowWidth * 0.5f, windowHeight)));
  MatStack.matrixMode(tgt::MatrixStack::MODELVIEW);
  MatStack.pushMatrix();
  MatStack.loadMatrix(cameraRight->getViewMatrix());

  // always draw the grid at the origin (before any modeling transform)
  drawGrid(10, 1);
  drawAxis(4);

  glm::mat4 cubeModel = glm::translate(glm::vec3(-0.5f));
  MatStack.pushMatrix();
  MatStack.loadMatrix(cameraRight->getViewMatrix() * cubeModel);
  drawCube();
  MatStack.popMatrix();

  // draw the camera
  glm::mat4 camModel = glm::translate(camera->getPosition()) * glm::inverse(camera->getRotateMatrix());
  MatStack.pushMatrix();
  MatStack.loadMatrix(cameraRight->getViewMatrix() * camModel);
  drawCamera();
  drawFrustum(camera->getFovy(), camera->getRatio(), camera->getNearDist(), camera->getFarDist());
  MatStack.popMatrix();

  MatStack.matrixMode(tgt::MatrixStack::PROJECTION);
  MatStack.popMatrix();
  MatStack.matrixMode(tgt::MatrixStack::MODELVIEW);
  MatStack.popMatrix();
}

void OpenGLTransformation::resize(int w, int h)
{
  if (h == 0) {
    return;
  }

  windowWidth = w / 4 * 4;
  windowHeight = h;

  glutPostRedisplay();
}

void OpenGLTransformation::key(unsigned char k, int, int)
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

void OpenGLTransformation::mouse(int button, int state, int x, int y)
{
  lastMouse[0] = x;
  lastMouse[1] = y;

  interactionMode = (state == GLUT_DOWN);

  viewMode = (x < windowWidth / 2 ? LEFT_VIEW : RIGHT_VIEW);

  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    mouseMode = MOUSE_ROTATE;
    break;
  case GLUT_MIDDLE_BUTTON:
    mouseMode = MOUSE_MOVE;
    break;
  case GLUT_RIGHT_BUTTON:
    mouseMode = MOUSE_ZOOM;
    break;
  default:
    break;
  }

  if (!interactionMode) {
    viewMode = NONE_VIEW;
    mouseMode = MOUSE_NONE;
  }

  glutPostRedisplay();
}

glm::vec2 scaleMouse(const glm::ivec2& coords, const glm::ivec2& viewport) {
  return glm::vec2(static_cast<float>(coords.x*2.f) / static_cast<float>(viewport.x) - 1.f,
    1.0f - static_cast<float>(coords.y*2.f) / static_cast<float>(viewport.y));
}

void OpenGLTransformation::motion(int x, int y)
{
  if (viewMode == LEFT_VIEW) {
    glm::vec2 newM = scaleMouse(glm::ivec2(x, y), glm::ivec2(windowWidth * 0.5f, windowHeight));
    glm::vec2 lastM = scaleMouse(glm::ivec2(lastMouse[0], lastMouse[1]), glm::ivec2(windowWidth * 0.5f, windowHeight));

    switch (mouseMode) {
    case MOUSE_ROTATE:
      trackball->rotate(newM, lastM);
      break;
    case MOUSE_ZOOM:
      trackball->zoom(newM, lastM, glm::vec2(0, 1));
      break;
    case MOUSE_MOVE:
      trackball->move(newM, lastM);
      break;
    default:
      break;
    }
  }
  else if (viewMode == RIGHT_VIEW) {
    glm::vec2 newM = scaleMouse(glm::ivec2(x - windowWidth * 0.5f, y), glm::ivec2(windowWidth * 0.5f, windowHeight));
    glm::vec2 lastM = scaleMouse(glm::ivec2(lastMouse[0] - windowWidth * 0.5f, lastMouse[1]), glm::ivec2(windowWidth * 0.5f, windowHeight));

    switch (mouseMode) {
    case MOUSE_ROTATE:
      trackballRight->rotate(newM, lastM);
      break;
    case MOUSE_ZOOM:
      trackballRight->zoom(newM, lastM, glm::vec2(0, 1));
      break;
    case MOUSE_MOVE:
      trackballRight->move(newM, lastM);
      break;
    default:
      break;
    }
  }

  lastMouse[0] = x;
  lastMouse[1] = y;

  glutPostRedisplay();
}

int  OpenGLTransformation::run(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitWindowPosition(500, 0);
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_ALPHA);
  glutCreateWindow("OpenGL Transformation");

  glutDisplayFunc(display);
  glutMotionFunc(motion);
  glutMouseFunc(mouse);
  glutKeyboardFunc(key);
  glutReshapeFunc(resize);

  init(argc, argv);

  glutMainLoop();
  deinit();
  return 0;
}

