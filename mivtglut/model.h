#pragma once

const float DEG2RAD = 3.141593f / 180;

// vertices for camera
static GLfloat cameraVertices[] = {
  0.500000f, -0.350000f, 0.000000f, 0.500000f, -0.350000f, 0.000000f, 0.500000f, -0.350000f, 0.000000f,
  -0.500000f, -0.350000f, 0.000000f, -0.500000f, -0.350000f, 0.000000f,
  -0.500000f, -0.350000f, 0.000000f, -0.500000f, 0.350000f, 0.000000f,
  -0.500000f, 0.350000f, 0.000000f, -0.500000f, 0.350000f, 0.000000f,
  0.500000f, 0.350000f, 0.000000f, 0.500000f, 0.350000f, 0.000000f,
  0.500000f, 0.350000f, 0.000000f, -0.500000f, 0.350000f, 0.300000f,
  -0.500000f, 0.350000f, 0.300000f, -0.500000f, 0.350000f, 0.300000f,
  0.500000f, 0.350000f, 0.300000f, 0.500000f, 0.350000f, 0.300000f,
  0.500000f, 0.350000f, 0.300000f, -0.500000f, -0.350000f, 0.300000f,
  -0.500000f, -0.350000f, 0.300000f, -0.500000f, -0.350000f, 0.300000f,
  0.500000f, -0.350000f, 0.300000f, 0.500000f, -0.350000f, 0.300000f,
  0.500000f, -0.350000f, 0.300000f, -0.285317f, 0.0927050f, 0.000000f,
  -0.242705f, 0.176336f, 0.000000f, -0.242705f, 0.176336f, -0.300000f,
  -0.242705f, 0.176336f, -0.300000f, -0.285317f, 0.0927050f, -0.300000f,
  -0.285317f, 0.0927050f, -0.300000f, -0.176336f, 0.242705f, 0.000000f,
  -0.176336f, 0.242705f, -0.300000f, -0.176336f, 0.242705f, -0.300000f,
  -0.0927050f, 0.285317f, 0.000000f, -0.0927050f, 0.285317f, -0.300000f,
  -0.0927050f, 0.285317f, -0.300000f, 0.000000f, 0.300000f, 0.000000f,
  0.000000f, 0.300000f, -0.300000f, 0.000000f, 0.300000f, -0.300000f,
  0.0927050f, 0.285317f, 0.000000f, 0.0927050f, 0.285317f, -0.300000f,
  0.0927050f, 0.285317f, -0.300000f, 0.176336f, 0.242705f, 0.000000f,
  0.176336f, 0.242705f, -0.300000f, 0.176336f, 0.242705f, -0.300000f,
  0.242705f, 0.176336f, 0.000000f, 0.242705f, 0.176336f, -0.300000f,
  0.242705f, 0.176336f, -0.300000f, 0.285317f, 0.0927050f, 0.000000f,
  0.285317f, 0.0927050f, -0.300000f, 0.285317f, 0.0927050f, -0.300000f,
  0.300000f, 0.000000f, 0.000000f, 0.300000f, 0.000000f, -0.300000f,
  0.300000f, 0.000000f, -0.300000f, 0.285317f, -0.0927050f, 0.000000f,
  0.285317f, -0.0927050f, -0.300000f, 0.285317f, -0.0927050f, -0.300000f,
  0.242705f, -0.176336f, 0.000000f, 0.242705f, -0.176336f, -0.300000f,
  0.242705f, -0.176336f, -0.300000f, 0.176336f, -0.242705f, 0.000000f,
  0.176336f, -0.242705f, -0.300000f, 0.176336f, -0.242705f, -0.300000f,
  0.0927050f, -0.285317f, 0.000000f, 0.0927050f, -0.285317f, -0.300000f,
  0.0927050f, -0.285317f, -0.300000f, 0.000000f, -0.300000f, 0.000000f,
  0.000000f, -0.300000f, -0.300000f, 0.000000f, -0.300000f, -0.300000f,
  -0.0927050f, -0.285317f, 0.000000f, -0.0927050f, -0.285317f, -0.300000f,
  -0.0927050f, -0.285317f, -0.300000f, -0.176336f, -0.242705f, 0.000000f,
  -0.176336f, -0.242705f, -0.300000f, -0.176336f, -0.242705f, -0.300000f,
  -0.242705f, -0.176336f, 0.000000f, -0.242705f, -0.176336f, -0.300000f,
  -0.242705f, -0.176336f, -0.300000f, -0.285317f, -0.0927050f, 0.000000f,
  -0.285317f, -0.0927050f, -0.300000f, -0.285317f, -0.0927050f, -0.300000f,
  -0.300000f, 0.000000f, 0.000000f, -0.300000f, 0.000000f, -0.300000f,
  -0.300000f, 0.000000f, -0.300000f, -0.194164f, 0.141069f, -0.300000f,
  -0.194164f, 0.141069f, -0.300000f, -0.228254f, 0.0741640f, -0.300000f,
  -0.228254f, 0.0741640f, -0.300000f, -0.141069f, 0.194164f, -0.300000f,
  -0.141069f, 0.194164f, -0.300000f, -0.0741640f, 0.228254f, -0.300000f,
  -0.0741640f, 0.228254f, -0.300000f, 0.000000f, 0.240000f, -0.300000f,
  0.000000f, 0.240000f, -0.300000f, 0.0741640f, 0.228254f, -0.300000f,
  0.0741640f, 0.228254f, -0.300000f, 0.141069f, 0.194164f, -0.300000f,
  0.141069f, 0.194164f, -0.300000f, 0.194164f, 0.141069f, -0.300000f,
  0.194164f, 0.141069f, -0.300000f, 0.228254f, 0.0741640f, -0.300000f,
  0.228254f, 0.0741640f, -0.300000f, 0.240000f, 0.000000f, -0.300000f,
  0.240000f, 0.000000f, -0.300000f, 0.228254f, -0.0741640f, -0.300000f,
  0.228254f, -0.0741640f, -0.300000f, 0.194164f, -0.141069f, -0.300000f,
  0.194164f, -0.141069f, -0.300000f, 0.141069f, -0.194164f, -0.300000f,
  0.141069f, -0.194164f, -0.300000f, 0.0741640f, -0.228254f, -0.300000f,
  0.0741640f, -0.228254f, -0.300000f, 0.000000f, -0.240000f, -0.300000f,
  0.000000f, -0.240000f, -0.300000f, -0.0741640f, -0.228254f, -0.300000f,
  -0.0741640f, -0.228254f, -0.300000f, -0.141068f, -0.194164f, -0.300000f,
  -0.141068f, -0.194164f, -0.300000f, -0.194164f, -0.141068f, -0.300000f,
  -0.194164f, -0.141068f, -0.300000f, -0.228254f, -0.0741640f, -0.300000f,
  -0.228254f, -0.0741640f, -0.300000f, -0.240000f, 0.000000f, -0.300000f,
  -0.240000f, 0.000000f, -0.300000f, -0.228254f, 0.0741640f, 0.000000f,
  -0.194164f, 0.141069f, 0.000000f, -0.141069f, 0.194164f, 0.000000f,
  -0.0741640f, 0.228254f, 0.000000f, 0.000000f, 0.240000f, 0.000000f,
  0.0741640f, 0.228254f, 0.000000f, 0.141069f, 0.194164f, 0.000000f,
  0.194164f, 0.141069f, 0.000000f, 0.228254f, 0.0741640f, 0.000000f,
  0.240000f, 0.000000f, 0.000000f, 0.228254f, -0.0741640f, 0.000000f,
  0.194164f, -0.141069f, 0.000000f, 0.141069f, -0.194164f, 0.000000f,
  0.0741640f, -0.228254f, 0.000000f, 0.000000f, -0.240000f, 0.000000f,
  -0.0741640f, -0.228254f, 0.000000f, -0.141068f, -0.194164f, 0.000000f,
  -0.194164f, -0.141068f, 0.000000f, -0.228254f, -0.0741640f, 0.000000f,
  -0.240000f, 0.000000f, 0.000000f, 0.306365f, 0.350000f, 0.164697f,
  0.313467f, 0.350000f, 0.178636f, 0.313467f, 0.370000f, 0.178636f,
  0.313467f, 0.370000f, 0.178636f, 0.306365f, 0.370000f, 0.164697f,
  0.306365f, 0.370000f, 0.164697f, 0.324529f, 0.350000f, 0.189697f,
  0.324529f, 0.370000f, 0.189697f, 0.324529f, 0.370000f, 0.189697f,
  0.338467f, 0.350000f, 0.196799f, 0.338467f, 0.370000f, 0.196799f,
  0.338467f, 0.370000f, 0.196799f, 0.353918f, 0.350000f, 0.199246f,
  0.353918f, 0.370000f, 0.199246f, 0.353918f, 0.370000f, 0.199246f,
  0.369369f, 0.350000f, 0.196799f, 0.369369f, 0.370000f, 0.196799f,
  0.369369f, 0.370000f, 0.196799f, 0.383307f, 0.350000f, 0.189697f,
  0.383307f, 0.370000f, 0.189697f, 0.383307f, 0.370000f, 0.189697f,
  0.394369f, 0.350000f, 0.178636f, 0.394369f, 0.370000f, 0.178636f,
  0.394369f, 0.370000f, 0.178636f, 0.401471f, 0.350000f, 0.164697f,
  0.401471f, 0.370000f, 0.164697f, 0.401471f, 0.370000f, 0.164697f,
  0.403918f, 0.350000f, 0.149246f, 0.403918f, 0.370000f, 0.149246f,
  0.403918f, 0.370000f, 0.149246f, 0.401471f, 0.350000f, 0.133795f,
  0.401471f, 0.370000f, 0.133795f, 0.401471f, 0.370000f, 0.133795f,
  0.394369f, 0.350000f, 0.119857f, 0.394369f, 0.370000f, 0.119857f,
  0.394369f, 0.370000f, 0.119857f, 0.383307f, 0.350000f, 0.108795f,
  0.383307f, 0.370000f, 0.108795f, 0.383307f, 0.370000f, 0.108795f,
  0.369369f, 0.350000f, 0.101693f, 0.369369f, 0.370000f, 0.101693f,
  0.369369f, 0.370000f, 0.101693f, 0.353918f, 0.350000f, 0.0992460f,
  0.353918f, 0.370000f, 0.0992460f, 0.353918f, 0.370000f, 0.0992460f,
  0.338467f, 0.350000f, 0.101693f, 0.338467f, 0.370000f, 0.101693f,
  0.338467f, 0.370000f, 0.101693f, 0.324529f, 0.350000f, 0.108795f,
  0.324529f, 0.370000f, 0.108795f, 0.324529f, 0.370000f, 0.108795f,
  0.313467f, 0.350000f, 0.119857f, 0.313467f, 0.370000f, 0.119857f,
  0.313467f, 0.370000f, 0.119857f, 0.306365f, 0.350000f, 0.133795f,
  0.306365f, 0.370000f, 0.133795f, 0.306365f, 0.370000f, 0.133795f,
  0.303918f, 0.350000f, 0.149246f, 0.303918f, 0.370000f, 0.149246f,
  0.303918f, 0.370000f, 0.149246f, 0.353918f, 0.370000f, 0.149246f
};
static GLfloat cameraNormals[] = {
  1.00000f, 0.000000f, 0.000000f, 0.000000f, -1.00000f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -1.00000f, 0.000000f, 0.000000f, 0.000000f, -1.00000f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, -1.00000f, 0.000000f, 0.000000f,
  0.000000f, 1.00000f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  1.00000f, 0.000000f, 0.000000f, 0.000000f, 1.00000f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, -1.00000f, 0.000000f, 0.000000f,
  0.000000f, 0.000000f, 1.00000f, 0.000000f, 1.00000f, 0.000000f,
  1.00000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.00000f,
  0.000000f, 1.00000f, 0.000000f, -1.00000f, 0.000000f, 0.000000f,
  0.000000f, -1.00000f, 0.000000f, 0.000000f, 0.000000f, 1.00000f,
  1.00000f, 0.000000f, 0.000000f, 0.000000f, -1.00000f, 0.000000f,
  0.000000f, 0.000000f, 1.00000f, -0.951057f, 0.309016f, 0.000000f,
  -0.809017f, 0.587785f, 0.000000f, -0.809017f, 0.587785f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, -0.951057f, 0.309016f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, -0.587785f, 0.809017f, 0.000000f,
  -0.587785f, 0.809017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -0.309017f, 0.951057f, 0.000000f, -0.309017f, 0.951057f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, 0.000000f, 1.00000f, 0.000000f,
  0.000000f, 1.00000f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.309017f, 0.951056f, 0.000000f, 0.309017f, 0.951056f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, 0.587785f, 0.809017f, 0.000000f,
  0.587785f, 0.809017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.809017f, 0.587785f, 0.000000f, 0.809017f, 0.587785f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, 0.951057f, 0.309017f, 0.000000f,
  0.951057f, 0.309017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  1.00000f, 0.000000f, 0.000000f, 1.00000f, 0.000000f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, 0.951057f, -0.309017f, 0.000000f,
  0.951057f, -0.309017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.809017f, -0.587785f, 0.000000f, 0.809017f, -0.587785f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, 0.587785f, -0.809017f, 0.000000f,
  0.587785f, -0.809017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.309017f, -0.951057f, 0.000000f, 0.309017f, -0.951057f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, 0.000000f, -1.00000f, 0.000000f,
  0.000000f, -1.00000f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -0.309017f, -0.951056f, 0.000000f, -0.309017f, -0.951056f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, -0.587785f, -0.809017f, 0.000000f,
  -0.587785f, -0.809017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -0.809017f, -0.587785f, 0.000000f, -0.809017f, -0.587785f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, -0.951057f, -0.309017f, 0.000000f,
  -0.951057f, -0.309017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -1.00000f, -1.00000e-006f, 0.000000f, -1.00000f, -1.00000e-006f, 0.000000f,
  0.000000f, 0.000000f, -1.00000f, 0.000000f, 0.000000f, -1.00000f,
  0.809017f, -0.587785f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.951057f, -0.309016f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.587785f, -0.809017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.309017f, -0.951056f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.000000f, -1.00000f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -0.309017f, -0.951056f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -0.587785f, -0.809017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -0.809017f, -0.587785f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -0.951057f, -0.309017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -1.00000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -0.951057f, 0.309017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -0.809017f, 0.587785f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -0.587785f, 0.809017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  -0.309017f, 0.951057f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.000000f, 1.00000f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.309017f, 0.951056f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.587785f, 0.809017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.809017f, 0.587785f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.951057f, 0.309017f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  1.00000f, 1.00000e-006f, 0.000000f, 0.951057f, -0.309016f, 0.000000f,
  0.809017f, -0.587785f, 0.000000f, 0.587785f, -0.809017f, 0.000000f,
  0.309017f, -0.951056f, 0.000000f, 0.000000f, -1.00000f, 0.000000f,
  -0.309017f, -0.951056f, 0.000000f, -0.587785f, -0.809017f, 0.000000f,
  -0.809017f, -0.587785f, 0.000000f, -0.951057f, -0.309017f, 0.000000f,
  -1.00000f, 0.000000f, 0.000000f, -0.951057f, 0.309017f, 0.000000f,
  -0.809017f, 0.587785f, 0.000000f, -0.587785f, 0.809017f, 0.000000f,
  -0.309017f, 0.951057f, 0.000000f, 0.000000f, 1.00000f, 0.000000f,
  0.309017f, 0.951056f, 0.000000f, 0.587785f, 0.809017f, 0.000000f,
  0.809017f, 0.587785f, 0.000000f, 0.951057f, 0.309017f, 0.000000f,
  1.00000f, 1.00000e-006f, 0.000000f, -0.951057f, 0.000000f, 0.309017f,
  -0.809017f, 0.000000f, 0.587786f, -0.809017f, 0.000000f, 0.587786f,
  0.000000f, 1.00000f, -1.00000e-006f, -0.951057f, 0.000000f, 0.309017f,
  0.000000f, 1.00000f, -2.00000e-006f, -0.587785f, 0.000000f, 0.809017f,
  -0.587785f, 0.000000f, 0.809017f, 0.000000f, 1.00000f, 0.000000f,
  -0.309016f, 0.000000f, 0.951057f, -0.309016f, 0.000000f, 0.951057f,
  0.000000f, 1.00000f, 0.000000f, 1.00000e-006f, 0.000000f, 1.00000f,
  1.00000e-006f, 0.000000f, 1.00000f, 0.000000f, 1.00000f, 0.000000f,
  0.309018f, 0.000000f, 0.951056f, 0.309018f, 0.000000f, 0.951056f,
  0.000000f, 1.00000f, 0.000000f, 0.587785f, 0.000000f, 0.809017f,
  0.587785f, 0.000000f, 0.809017f, 0.000000f, 1.00000f, 0.000000f,
  0.809017f, 0.000000f, 0.587786f, 0.809017f, 0.000000f, 0.587786f,
  0.000000f, 1.00000f, 0.000000f, 0.951057f, 0.000000f, 0.309017f,
  0.951057f, 0.000000f, 0.309017f, 0.000000f, 1.00000f, 1.00000e-006f,
  1.00000f, 0.000000f, 0.000000f, 1.00000f, 0.000000f, 0.000000f,
  0.000000f, 1.00000f, 2.00000e-006f, 0.951057f, 0.000000f, -0.309017f,
  0.951057f, 0.000000f, -0.309017f, 0.000000f, 1.00000f, 2.00000e-006f,
  0.809017f, 0.000000f, -0.587786f, 0.809017f, 0.000000f, -0.587786f,
  0.000000f, 1.00000f, 1.00000e-006f, 0.587785f, 0.000000f, -0.809017f,
  0.587785f, 0.000000f, -0.809017f, 0.000000f, 1.00000f, 0.000000f,
  0.309017f, 0.000000f, -0.951056f, 0.309017f, 0.000000f, -0.951056f,
  0.000000f, 1.00000f, 0.000000f, 0.000000f, 0.000000f, -1.00000f,
  0.000000f, 0.000000f, -1.00000f, 0.000000f, 1.00000f, 0.000000f,
  -0.309017f, 0.000000f, -0.951056f, -0.309017f, 0.000000f, -0.951056f,
  0.000000f, 1.00000f, 0.000000f, -0.587786f, 0.000000f, -0.809017f,
  -0.587786f, 0.000000f, -0.809017f, 0.000000f, 1.00000f, 0.000000f,
  -0.809017f, 0.000000f, -0.587785f, -0.809017f, 0.000000f, -0.587785f,
  0.000000f, 1.00000f, 0.000000f, -0.951056f, 0.000000f, -0.309018f,
  -0.951056f, 0.000000f, -0.309018f, 0.000000f, 1.00000f, -1.00000e-006f,
  -1.00000f, 0.000000f, -1.00000e-006f, -1.00000f, 0.000000f, -1.00000e-006f,
  0.000000f, 1.00000f, -2.00000e-006f, 0.000000f, 1.00000f, 0.000000f
};
static GLint cameraIndices[] = {
  2, 5, 11, 5, 8, 10, 7, 17, 7, 14, 16, 13, 23, 13, 20, 22,
  19, 1, 19, 4, 3, 18, 6, 18, 12, 21, 0, 15, 0, 9, 203, 149,
  204, 147, 204, 152, 204, 155, 204, 158, 204, 161, 204, 164, 204, 167, 204, 170,
  204, 173, 204, 176, 204, 179, 204, 182, 204, 185, 204, 188, 204, 191, 204, 194,
  204, 197, 204, 200, 203, 144, 148, 144, 202, 201, 199, 198, 196, 195, 193, 192,
  190, 189, 187, 186, 184, 183, 181, 180, 178, 177, 175, 174, 172, 171, 169, 168,
  166, 165, 163, 162, 160, 159, 157, 156, 154, 153, 151, 150, 146, 145, 148, 145,
  144, 123, 87, 124, 87, 125, 85, 126, 89, 127, 91, 128, 93, 129, 95, 130,
  97, 131, 99, 132, 101, 133, 103, 134, 105, 135, 107, 136, 109, 137, 111, 138,
  113, 139, 115, 140, 117, 141, 119, 142, 121, 143, 123, 143, 124, 29, 86, 29,
  122, 83, 120, 80, 118, 77, 116, 74, 114, 71, 112, 68, 110, 65, 108, 62,
  106, 59, 104, 56, 102, 53, 100, 50, 98, 47, 96, 44, 94, 41, 92, 38,
  90, 35, 88, 32, 84, 27, 86, 27, 29, 24, 28, 24, 82, 81, 79, 78,
  76, 75, 73, 72, 70, 69, 67, 66, 64, 63, 61, 60, 58, 57, 55, 54,
  52, 51, 49, 48, 46, 45, 43, 42, 40, 39, 37, 36, 34, 33, 31, 30,
  26, 25, 28, 25, 24
};

///////////////////////////////////////////////////////////////////////////////
// draw camera
///////////////////////////////////////////////////////////////////////////////
void drawCamera()
{
  float shininess = 32.0f;
  float diffuseColor[3] = { 1.0f, 1.0f, 1.0f };
  float specularColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

  // set specular and shiniess using glMaterial
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

  // set ambient and diffuse color using glColorMaterial (gold-yellow)
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glColor3fv(diffuseColor);

  // start to render polygons
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  glNormalPointer(GL_FLOAT, 0, cameraNormals);
  glVertexPointer(3, GL_FLOAT, 0, cameraVertices);

  glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, &cameraIndices[0]);
  glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, &cameraIndices[5]);
  glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, &cameraIndices[10]);
  glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, &cameraIndices[15]);
  glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, &cameraIndices[20]);
  glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, &cameraIndices[25]);
  glDrawElements(GL_TRIANGLE_STRIP, 39, GL_UNSIGNED_INT, &cameraIndices[30]);
  glDrawElements(GL_TRIANGLE_STRIP, 44, GL_UNSIGNED_INT, &cameraIndices[69]);
  glDrawElements(GL_TRIANGLE_STRIP, 44, GL_UNSIGNED_INT, &cameraIndices[113]);
  glDrawElements(GL_TRIANGLE_STRIP, 44, GL_UNSIGNED_INT, &cameraIndices[157]);
  glDrawElements(GL_TRIANGLE_STRIP, 44, GL_UNSIGNED_INT, &cameraIndices[201]);

  glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
  glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays
}

///////////////////////////////////////////////////////////////////////////////
// draw frustum
///////////////////////////////////////////////////////////////////////////////
void drawFrustum(float fovY, float aspectRatio, float nearPlane, float farPlane)
{
  float tangent = tanf(fovY / 2 * DEG2RAD);
  float nearHeight = nearPlane * tangent;
  float nearWidth = nearHeight * aspectRatio;
  float farHeight = farPlane * tangent;
  float farWidth = farHeight * aspectRatio;

  // compute 8 vertices of the frustum
  float vertices[8][3];
  // near top right
  vertices[0][0] = nearWidth;     vertices[0][1] = nearHeight;    vertices[0][2] = -nearPlane;
  // near top left
  vertices[1][0] = -nearWidth;    vertices[1][1] = nearHeight;    vertices[1][2] = -nearPlane;
  // near bottom left
  vertices[2][0] = -nearWidth;    vertices[2][1] = -nearHeight;   vertices[2][2] = -nearPlane;
  // near bottom right
  vertices[3][0] = nearWidth;     vertices[3][1] = -nearHeight;   vertices[3][2] = -nearPlane;
  // far top right
  vertices[4][0] = farWidth;      vertices[4][1] = farHeight;     vertices[4][2] = -farPlane;
  // far top left
  vertices[5][0] = -farWidth;     vertices[5][1] = farHeight;     vertices[5][2] = -farPlane;
  // far bottom left
  vertices[6][0] = -farWidth;     vertices[6][1] = -farHeight;    vertices[6][2] = -farPlane;
  // far bottom right
  vertices[7][0] = farWidth;      vertices[7][1] = -farHeight;    vertices[7][2] = -farPlane;

  float colorLine1[4] = { 0.7f, 0.7f, 0.7f, 0.7f };
  float colorLine2[4] = { 0.2f, 0.2f, 0.2f, 0.7f };
  float colorPlane[4] = { 0.5f, 0.5f, 0.5f, 0.5f };

  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // draw the edges around frustum
  glBegin(GL_LINES);
  glColor4fv(colorLine2);
  glVertex3f(0, 0, 0);
  glColor4fv(colorLine1);
  glVertex3fv(vertices[4]);

  glColor4fv(colorLine2);
  glVertex3f(0, 0, 0);
  glColor4fv(colorLine1);
  glVertex3fv(vertices[5]);

  glColor4fv(colorLine2);
  glVertex3f(0, 0, 0);
  glColor4fv(colorLine1);
  glVertex3fv(vertices[6]);

  glColor4fv(colorLine2);
  glVertex3f(0, 0, 0);
  glColor4fv(colorLine1);
  glVertex3fv(vertices[7]);
  glEnd();

  glColor4fv(colorLine1);
  glBegin(GL_LINE_LOOP);
  glVertex3fv(vertices[4]);
  glVertex3fv(vertices[5]);
  glVertex3fv(vertices[6]);
  glVertex3fv(vertices[7]);
  glEnd();

  glColor4fv(colorLine1);
  glBegin(GL_LINE_LOOP);
  glVertex3fv(vertices[0]);
  glVertex3fv(vertices[1]);
  glVertex3fv(vertices[2]);
  glVertex3fv(vertices[3]);
  glEnd();

  // draw near and far plane
  glColor4fv(colorPlane);
  glBegin(GL_QUADS);
  glVertex3fv(vertices[0]);
  glVertex3fv(vertices[1]);
  glVertex3fv(vertices[2]);
  glVertex3fv(vertices[3]);
  glVertex3fv(vertices[4]);
  glVertex3fv(vertices[5]);
  glVertex3fv(vertices[6]);
  glVertex3fv(vertices[7]);
  glEnd();

  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////////////////
// draw a grid on the xz plane
///////////////////////////////////////////////////////////////////////////////
void drawGrid(float size, float step)
{
  // disable lighting
  glDisable(GL_LIGHTING);

  glBegin(GL_LINES);

  glColor3f(0.3f, 0.3f, 0.3f);
  for (float i = step; i <= size; i += step)
  {
    glVertex3f(-size, 0, i);   // lines parallel to X-axis
    glVertex3f(size, 0, i);
    glVertex3f(-size, 0, -i);   // lines parallel to X-axis
    glVertex3f(size, 0, -i);

    glVertex3f(i, 0, -size);   // lines parallel to Z-axis
    glVertex3f(i, 0, size);
    glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
    glVertex3f(-i, 0, size);
  }

  // x-axis
  glColor3f(0.5f, 0, 0);
  glVertex3f(-size, 0, 0);
  glVertex3f(size, 0, 0);

  // z-axis
  glColor3f(0, 0, 0.5f);
  glVertex3f(0, 0, -size);
  glVertex3f(0, 0, size);

  glEnd();

  // enable lighting back
  glEnable(GL_LIGHTING);
}



///////////////////////////////////////////////////////////////////////////////
// draw the local axis of an object
///////////////////////////////////////////////////////////////////////////////
void drawAxis(float size)
{
  glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
  glDisable(GL_LIGHTING);
  glPushMatrix();             //NOTE: There is a bug on Mac misbehaviours of
  //      the light position when you draw GL_LINES
  //      and GL_POINTS. remember the matrix.

  // draw axis
  glLineWidth(3);
  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(size, 0, 0);
  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, size, 0);
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, size);
  glEnd();
  glLineWidth(1);

  // draw arrows(actually big square dots)
  glPointSize(5);
  glBegin(GL_POINTS);
  glColor3f(1, 0, 0);
  glVertex3f(size, 0, 0);
  glColor3f(0, 1, 0);
  glVertex3f(0, size, 0);
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, size);
  glEnd();
  glPointSize(1);

  // restore default settings
  glPopMatrix();
  glEnable(GL_LIGHTING);
  glDepthFunc(GL_LEQUAL);
}

// indices for cube
GLfloat cubeVertices[] = {
  1, 1, 1, 1, 1, 0, 0, 1, 0,
  1, 1, 1, 0, 1, 0, 0, 1, 1,
  0, 0, 0, 0, 1, 0, 1, 1, 0,
  0, 0, 0, 1, 1, 0, 1, 0, 0,
  0, 0, 0, 0, 0, 1, 0, 1, 1,
  0, 0, 0, 0, 1, 1, 0, 1, 0,
  1, 1, 1, 0, 1, 1, 0, 0, 1,
  1, 1, 1, 0, 0, 1, 1, 0, 1,
  1, 1, 1, 1, 0, 1, 1, 0, 0,
  1, 1, 1, 1, 0, 0, 1, 1, 0,
  0, 0, 0, 1, 0, 0, 1, 0, 1,
  0, 0, 0, 1, 0, 1, 0, 0, 1,
};

// vertex normals for cube
GLfloat cubeNormals[] = {
  0, 1, 0, 0, 1, 0, 0, 1, 0,
  0, 1, 0, 0, 1, 0, 0, 1, 0,
  0, 0, -1, 0, 0, -1, 0, 0, -1,
  0, 0, -1, 0, 0, -1, 0, 0, -1,
  -1, 0, 0, -1, 0, 0, -1, 0, 0,
  -1, 0, 0, -1, 0, 0, -1, 0, 0,
  0, 0, 1, 0, 0, 1, 0, 0, 1,
  0, 0, 1, 0, 0, 1, 0, 0, 1,
  1, 0, 0, 1, 0, 0, 1, 0, 0,
  1, 0, 0, 1, 0, 0, 1, 0, 0,
  0, -1, 0, 0, -1, 0, 0, -1, 0,
  0, -1, 0, 0, -1, 0, 0, -1, 0,
};

///////////////////////////////////////////////////////////////////////////////
// draw cube
///////////////////////////////////////////////////////////////////////////////
void drawCube()
{
  //float shininess = 15.0f;
  //float diffuseColor[3] = { 0.929524f, 0.796542f, 0.178823f };
  //float specularColor[4] = { 1.00000f, 0.980392f, 0.549020f, 1.0f };

  //// set specular and shiniess using glMaterial (gold-yellow)
  //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
  //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

  //// set ambient and diffuse color using glColorMaterial (gold-yellow)
  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  //glColor3fv(diffuseColor);

  // start to render polygons
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glNormalPointer(GL_FLOAT, 0, cubeNormals);
  glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
  glColorPointer(3, GL_FLOAT, 0, cubeVertices);

  //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, &cubeIndices[0]);
  glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

  glDisableClientState(GL_COLOR_ARRAY);	  // disable color arrays
  glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
  glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays

}