#include "computeshadertest.h"
#include "gpucapabilities.h"
#include "matrixstack.h"

mivt::Application * ComputeShaderTest::app = 0;
tgt::Shader * ComputeShaderTest::shaderProgram = 0;
tgt::Texture * ComputeShaderTest::texture = 0;

void ComputeShaderTest::init()
{
  glewInit();

  app = new mivt::Application();

  shaderProgram = ShdrMgr.loadCompute("hello.comp", false);

  texture = new tgt::Texture(glm::ivec3(512, 512, 1), GL_RED, GL_R8, GL_UNSIGNED_BYTE);

  // compute shader test
  {
    uint8_t *buffer = new uint8_t[512 * 512];
    memset(buffer, 0, 512 * 512 * sizeof(uint8_t));

    texture->setPixelData(buffer);
    texture->uploadTexture();
    texture->setPixelData(0);

    shaderProgram->activate();

    glBindImageTexture(0, texture->getId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);

    shaderProgram->setUniform("dim", glm::vec3(512, 512, 1));

    // run compute shader!
    glDispatchCompute(512 / 16, 512 / 16, 1);
    
    // cleanup
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    shaderProgram->deactivate();

    glFinish();

    texture->setPixelData(buffer);
    texture->downloadTexture();
    texture->setPixelData(0);

    delete[] buffer;
  }
}

void ComputeShaderTest::deinit()
{
  ShdrMgr.dispose(shaderProgram);
  shaderProgram = 0;

  delete texture;
  delete app;
}

void ComputeShaderTest::display()
{
  glutSwapBuffers();
}

void ComputeShaderTest::resize(int w, int h)
{
  w, h;
  glutPostRedisplay();
}

int  ComputeShaderTest::run(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitWindowPosition(500, 0);
  glutInitWindowSize(512, 512);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_ALPHA);
  glutCreateWindow("compute shader test");

  glutDisplayFunc(display);
  glutReshapeFunc(resize);

  init();

  glutMainLoop();
  deinit();
  return 0;
}