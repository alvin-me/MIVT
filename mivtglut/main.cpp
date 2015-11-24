#include "vrtest.h"
#include "opengltransformation.h"

#include <iostream>

int main(int argc, char* argv[])
{
  printf("Test case: \n" 
    "1. VR \n" 
    "2. OpenGL Transformation \n" 
    "\nPlease input an index \n");

  char c = (char)getchar();
  switch (c)
  {
  case '1':
    return VRTest::run(argc, argv);
  case '2':
    return OpenGLTransformation::run(argc, argv);
  default:
    return 0;
  }
}