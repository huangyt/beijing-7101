#if 0
#if 0
static U8 RGB1[256][3]=/*zxg for sinotype test*/
{
	{32,32,255},
	{87,87,255},
	{143,143,255},
	{124,124,255},
	{162,162,255},
	{199,199,255},
	{217,217,255},
	{217,217,255},
	{236,236,255},
	{236,236,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},/*12*/
	{32,32,32},
	{87,87,87},
	{143,143,143},
	{124,124,124},
	{162,162,162},
	{180,180,180},
	{199,199,199},
	{217,217,217},
	{217,217,217},
	{236,236,236},
	{236,236,236},
	{255,255,255},
	{255,255,255},/*25*/
	{255,255,255},
	{212,212,255},
	{191,191,255},
	{191,191,255},
	{169,169,255},
	{127,127,255},
	{106,106,255},
	{84,84,255},
	{84,84,255},
	{63,63,255},
	{21,21,255},
	{0,0,255},
	{0,0,255},/*38*/
	{255,255,255},
	{191,191,191},
	{127,127,127},
	{191,191,191},
	{169,169,169},
	{84,84,84},
	{63,63,63},
	{42,42,42},
	{84,84,84},
	{63,63,63},
	{42,42,42},
	{21,21,21},
	{0,0,0},/*51*/
	{85,127,0},
	{85,127,85},
	{85,127,170},
	{85,127,255},
	{85,159,0},
	{85,159,85},
	{85,159,170},
	{85,159,255},
	{85,191,0},
	{85,191,85},
	{85,191,170},
	{85,191,255},
	{85,223,0},
	{85,223,85},
	{85,223,170},
	{85,223,255},
	{85,255,0},
	{85,255,85},
	{85,255,170},
	{85,255,255},
	{127,0,0},
	{127,0,85},
	{127,0,170},
	{127,0,255},
	{127,31,0},
	{127,31,85},
	{127,31,170},
	{127,31,255},
	{127,63,0},
	{127,63,85},
	{127,63,170},
	{127,63,255},
	{127,95,0},
	{127,95,85},
	{127,95,170},
	{127,95,255},
	{127,127,0},
	{127,127,85},
	{127,127,170},
	{127,127,255},
	{127,159,0},
	{127,159,85},
	{127,159,170},
	{127,159,255},
	{127,191,0},
	{127,191,85},
	{127,191,170},
	{127,191,255},
	{127,223,0},
	{127,223,85},
	{127,223,170},
	{127,223,255},
	{127,255,0},
	{127,255,85},
	{127,255,170},
	{127,255,255},
	{170,0,0},
	{170,0,85},
	{170,0,170},
	{170,0,255},
	{170,31,0},
	{170,31,85},
	{170,31,170},
	{170,31,255},
	{170,63,0},
	{170,63,85},
	{170,63,170},
	{170,63,255},
	{170,95,0},
	{170,95,85},
	{170,95,170},
	{170,95,255},
	{170,127,0},
	{170,127,85},
	{170,127,170},
	{170,127,255},
	{170,159,0},
	{170,159,85},
	{170,159,170},
	{170,159,255},
	{170,191,0},
	{170,191,85},
	{170,191,170},
	{170,191,255},
	{170,223,0},
	{170,223,85},
	{170,223,170},
	{170,223,255},
	{170,255,0},
	{170,255,85},
	{170,255,170},
	{170,255,255},
	{212,0,0},
	{212,0,85},
	{212,0,170},
	{212,0,255},
	{212,31,0},
	{212,31,85},
	{212,31,170},
	{212,31,255},
	{212,63,0},
	{212,63,85},
	{212,63,170},
	{212,63,255},
	{212,95,0},
	{212,95,85},
	{212,95,170},
	{212,95,255},
	{212,127,0},
	{212,127,85},
	{212,127,170},
	{212,127,255},
	{212,159,0},
	{212,159,85},
	{212,159,170},
	{212,159,255},
	{212,191,0},
	{212,191,85},
	{212,191,170},
	{212,191,255},
	{212,223,0},
	{212,223,85},
	{212,223,170},
	{212,223,255},
	{212,255,0},
	{212,255,85},
	{212,255,170},
	{212,255,255},
	{255,0,85},
	{255,0,170},
	{255,31,0},
	{255,31,85},
	{255,31,170},
	{255,31,255},
	{255,63,0},
	{255,63,85},
	{255,63,170},
	{255,63,255},
	{255,95,0},
	{255,95,85},
	{255,95,170},
	{255,95,255},
	{255,127,0},
	{255,127,85},
	{255,127,170},
	{255,127,255},
	{255,159,0},
	{255,159,85},
	{255,159,170},
	{255,159,255},
	{255,191,0},
	{255,191,85},
	{255,191,170},
	{255,191,255},
	{255,223,0},
	{255,223,85},
	{255,223,170},
	{255,223,255},
	{255,255,85},
	{255,255,170},
	{204,204,255},
	{255,204,255},
	{51,255,255},
	{102,255,255},
	{153,255,255},
	{204,255,255},
	{0,127,0},
	{0,127,85},
	{0,127,170},
	{0,127,255},
	{0,159,0},
	{0,159,85},
	{0,159,170},
	{0,159,255},
	{0,191,0},
	{0,191,85},
	{0,191,170},
	{0,191,255},
	{0,223,0},
	{0,223,85},
	{0,223,170},
	{0,223,255},
	{0,255,85},
	{0,255,170},
	{42,0,0},
	{42,0,85},
	{42,0,170},
	{42,0,255},
	{42,31,0},
	{42,31,85},
	{42,31,170},
	{42,31,255},
	{42,63,0},
	{42,63,85},
	{255,251,240},
	{160,160,164},
	{128,128,128},
	{255,0,0},
	{0,255,0},
	{255,255,0},
	{0,0,255},
	{255,0,255},
	{0,255,255}
};
#else
/*static  yxl 2004-07-27 cancel static*/
static U8 RGB1[256][3]=
{
{0x00, 0x00, 0x00 },
{0x80, 0x00, 0x00 },
{0x00, 0x80, 0x00 },
{0x80, 0x80, 0x00 },
{0x00, 0x00, 0x80 },
{0x80, 0x00, 0x80 },
{0x00, 0x80, 0x80 },
{0x80, 0x80, 0x80 },
{0xc0, 0xdc, 0xc0 },
{0xa6, 0xca, 0xf0 },
{0x2a, 0x3f, 0xaa },
{0x2a, 0x3f, 0xff },
{0x2a, 0x5f, 0x00 },
{0x2a, 0x5f, 0x55 },
{0x2a, 0x5f, 0xaa },
{0x2a, 0x5f, 0xff },
{0x2a, 0x7f, 0x00 },
{0x2a, 0x7f, 0x55 },
{0x2a, 0x7f, 0xaa },
{0x2a, 0x7f, 0xff },
{0x2a, 0x9f, 0x00 },
{0x2a, 0x9f, 0x55 },
{0x2a, 0x9f, 0xaa },
{0x2a, 0x9f, 0xff },
{0x2a, 0xbf, 0x00 },
{0x2a, 0xbf, 0x55 },
{0x2a, 0xbf, 0xaa },
{0x2a, 0xbf, 0xff },
{0x2a, 0xdf, 0x00 },
{0x2a, 0xdf, 0x55 },
{0x2a, 0xdf, 0xaa },
{0x2a, 0xdf, 0xff },
{0x2a, 0xff, 0x00 },
{0x2a, 0xff, 0x55 },
{0x2a, 0xff, 0xaa },
{0x2a, 0xff, 0xff },
{0x55, 0x00, 0x00 },
{0x55, 0x00, 0x55 },
{0x55, 0x00, 0xaa },
{0x55, 0x00, 0xff },
{0x55, 0x1f, 0x00 },
{0x55, 0x1f, 0x55 },
{0x55, 0x1f, 0xaa },
{0x55, 0x1f, 0xff },
{0x55, 0x3f, 0x00 },
{0x55, 0x3f, 0x55 },
{0x55, 0x3f, 0xaa },
{0x55, 0x3f, 0xff },
{0x55, 0x5f, 0x00 },
{0x55, 0x5f, 0x55 },
{0x55, 0x5f, 0xaa },
{0x55, 0x5f, 0xff },
{0x55, 0x7f, 0x00 },
{0x55, 0x7f, 0x55 },
{0x55, 0x7f, 0xaa },
{0x55, 0x7f, 0xff },
{0x55, 0x9f, 0x00 },
{0x55, 0x9f, 0x55 },
{0x55, 0x9f, 0xaa },
{0x55, 0x9f, 0xff },
{0x55, 0xbf, 0x00 },
{0x55, 0xbf, 0x55 },
{0x55, 0xbf, 0xaa },
{0x55, 0xbf, 0xff },
{0x55, 0xdf, 0x00 },
{0x55, 0xdf, 0x55 },
{0x55, 0xdf, 0xaa },
{0x55, 0xdf, 0xff },
{0x55, 0xff, 0x00 },
{0x55, 0xff, 0x55 },
{0x55, 0xff, 0xaa },
{0x55, 0xff, 0xff },
{0x7f, 0x00, 0x00 },
{0x7f, 0x00, 0x55 },
{0x7f, 0x00, 0xaa },
{0x7f, 0x00, 0xff },
{0x7f, 0x1f, 0x00 },
{0x7f, 0x1f, 0x55 },
{0x7f, 0x1f, 0xaa },
{0x7f, 0x1f, 0xff },
{0x7f, 0x3f, 0x00 },
{0x7f, 0x3f, 0x55 },
{0x7f, 0x3f, 0xaa },
{0x7f, 0x3f, 0xff },
{0x7f, 0x5f, 0x00 },
{0x7f, 0x5f, 0x55 },
{0x7f, 0x5f, 0xaa },
{0x7f, 0x5f, 0xff },
{0x7f, 0x7f, 0x00 },
{0x7f, 0x7f, 0x55 },
{0x7f, 0x7f, 0xaa },
{0x7f, 0x7f, 0xff },
{0x7f, 0x9f, 0x00 },
{0x7f, 0x9f, 0x55 },
{0x7f, 0x9f, 0xaa },
{0x7f, 0x9f, 0xff },
{0x7f, 0xbf, 0x00 },
{0x7f, 0xbf, 0x55 },
{0x7f, 0xbf, 0xaa },
{0x7f, 0xbf, 0xff },
{0x7f, 0xdf, 0x00 },
{0x7f, 0xdf, 0x55 },
{0x7f, 0xdf, 0xaa },
{0x7f, 0xdf, 0xff },
{0x7f, 0xff, 0x00 },
{0x7f, 0xff, 0x55 },
{0x7f, 0xff, 0xaa },
{0x7f, 0xff, 0xff },
{0xaa, 0x00, 0x00 },
{0xaa, 0x00, 0x55 },
{0xaa, 0x00, 0xaa },
{0xaa, 0x00, 0xff },
{0xaa, 0x1f, 0x00 },
{0xaa, 0x1f, 0x55 },
{0xaa, 0x1f, 0xaa },
{0xaa, 0x1f, 0xff },
{0xaa, 0x3f, 0x00 },
{0xaa, 0x3f, 0x55 },
{0xaa, 0x3f, 0xaa },
{0xaa, 0x3f, 0xff },
{0xaa, 0x5f, 0x00 },
{0xaa, 0x5f, 0x55 },
{0xaa, 0x5f, 0xaa },
{0xaa, 0x5f, 0xff },
{0xaa, 0x7f, 0x00 },
{0xaa, 0x7f, 0x55 },
{0xaa, 0x7f, 0xaa },
{0xaa, 0x7f, 0xff },
{0xaa, 0x9f, 0x00 },
{0xaa, 0x9f, 0x55 },
{0xaa, 0x9f, 0xaa },
{0xaa, 0x9f, 0xff },
{0xaa, 0xbf, 0x00 },
{0xaa, 0xbf, 0x55 },
{0xaa, 0xbf, 0xaa },
{0xaa, 0xbf, 0xff },
{0xaa, 0xdf, 0x00 },
{0xaa, 0xdf, 0x55 },
{0xaa, 0xdf, 0xaa },
{0xaa, 0xdf, 0xff },
{0xaa, 0xff, 0x00 },
{0xaa, 0xff, 0x55 },
{0xaa, 0xff, 0xaa },
{0xaa, 0xff, 0xff },
{0xd4, 0x00, 0x00 },
{0xd4, 0x00, 0x55 },
{0xd4, 0x00, 0xaa },
{0xd4, 0x00, 0xff },
{0xd4, 0x1f, 0x00 },
{0xd4, 0x1f, 0x55 },
{0xd4, 0x1f, 0xaa },
{0xd4, 0x1f, 0xff },
{0xd4, 0x3f, 0x00 },
{0xd4, 0x3f, 0x55 },
{0xd4, 0x3f, 0xaa },
{0xd4, 0x3f, 0xff },
{0xd4, 0x5f, 0x00 },
{0xd4, 0x5f, 0x55 },
{0xd4, 0x5f, 0xaa },
{0xd4, 0x5f, 0xff },
{0xd4, 0x7f, 0x00 },
{0xd4, 0x7f, 0x55 },
{0xd4, 0x7f, 0xaa },
{0xd4, 0x7f, 0xff },
{0xd4, 0x9f, 0x00 },
{0xd4, 0x9f, 0x55 },
{0xd4, 0x9f, 0xaa },
{0xd4, 0x9f, 0xff },
{0xd4, 0xbf, 0x00 },
{0xd4, 0xbf, 0x55 },
{0xd4, 0xbf, 0xaa },
{0xd4, 0xbf, 0xff },
{0xd4, 0xdf, 0x00 },
{0xd4, 0xdf, 0x55 },
{0xd4, 0xdf, 0xaa },
{0xd4, 0xdf, 0xff },
{0xd4, 0xff, 0x00 },
{0xd4, 0xff, 0x55 },
{0xd4, 0xff, 0xaa },
{0xd4, 0xff, 0xff },
{0xff, 0x00, 0x55 },
{0xff, 0x00, 0xaa },
{0xff, 0x1f, 0x00 },
{0xff, 0x1f, 0x55 },
{0xff, 0x1f, 0xaa },
{0xff, 0x1f, 0xff },
{0xff, 0x3f, 0x00 },
{0xff, 0x3f, 0x55 },
{0xff, 0x3f, 0xaa },
{0xff, 0x3f, 0xff },
{0xff, 0x5f, 0x00 },
{0xff, 0x5f, 0x55 },
{0xff, 0x5f, 0xaa },
{0xff, 0x5f, 0xff },
{0xff, 0x7f, 0x00 },
{0xff, 0x7f, 0x55 },
{0xff, 0x7f, 0xaa },
{0xff, 0x7f, 0xff },
{0xff, 0x9f, 0x00 },
{0xff, 0x9f, 0x55 },
{0xff, 0x9f, 0xaa },
{0xff, 0x9f, 0xff },
{0xff, 0xbf, 0x00 },
{0xff, 0xbf, 0x55 },
{0xff, 0xbf, 0xaa },
{0xff, 0xbf, 0xff },
{0xff, 0xdf, 0x00 },
{0xff, 0xdf, 0x55 },
{0xff, 0xdf, 0xaa },
{0xff, 0xdf, 0xff },
{0xff, 0xff, 0x55 },
{0xff, 0xff, 0xaa },
{0xcc, 0xcc, 0xff },
{0xff, 0xcc, 0xff },
{0x33, 0xff, 0xff },
{0x66, 0xff, 0xff },
{0x99, 0xff, 0xff },
{0xcc, 0xff, 0xff },
{0x00, 0x7f, 0x00 },
{0x00, 0x7f, 0x55 },
{0x00, 0x7f, 0xaa },
{0x00, 0x7f, 0xff },
{0x00, 0x9f, 0x00 },
{0x00, 0x9f, 0x55 },
{0x00, 0x9f, 0xaa },
{0x00, 0x9f, 0xff },
{0x00, 0xbf, 0x00 },
{0x00, 0xbf, 0x55 },
{0x00, 0xbf, 0xaa },
{0x00, 0xbf, 0xff },
{0x00, 0xdf, 0x00 },
{0x00, 0xdf, 0x55 },
{0x00, 0xdf, 0xaa },
{0x00, 0xdf, 0xff },
{0x00, 0xff, 0x55 },
{0x00, 0xff, 0xaa },
{0x2a, 0x00, 0x00 },
{0x2a, 0x00, 0x55 },
{0x2a, 0x00, 0xaa },
{0x2a, 0x00, 0xff },
{0x2a, 0x1f, 0x00 },
{0x2a, 0x1f, 0x55 },
{0x2a, 0x1f, 0xaa },
{0x2a, 0x1f, 0xff },
{0x2a, 0x3f, 0x00 },
{0x2a, 0x3f, 0x55 },
{0xff, 0xfb, 0xf0 },
{0xa0, 0xa0, 0xa4 },
{0x80, 0x80, 0x80 },
{0xff, 0x00, 0x00 },
{0x00, 0xff, 0x00 },
{0xff, 0xff, 0x00 },
{0x00, 0x00, 0xff },
{0xff, 0x00, 0xff },
{0x00, 0xff, 0xff },
{0xff, 0xff, 0xff }
};
#endif

#if 1 /*yxl 2004-07-27  add this section for ipanel color pallette test*/
/*static*/
static U8 RGBIPANEL[256][3]=
{
0, 0, 0,
21, 21, 21,
21, 21, 64,
21, 21, 106,
21, 21, 149,
21, 21, 192,
21, 21, 234,
21, 57, 21,
21, 57, 64,
21, 57, 106,
21, 57, 149,
21, 57, 192,
21, 57, 234,
21, 94, 21,
21, 94, 64,
21, 94, 106,
21, 94, 149,
21, 94, 192,
21, 94, 234,
21, 130, 21,
21, 130, 64,
21, 130, 106,
21, 130, 149,
21, 130, 192,
21, 130, 234,
21, 167, 21,
21, 167, 64,
21, 167, 106,
21, 167, 149,
21, 167, 192,
21, 167, 234,
21, 203, 21,
21, 203, 64,
21, 203, 106,
21, 203, 149,
21, 203, 192,
21, 203, 234,
21, 240, 21,
21, 240, 64,
21, 240, 106,
21, 240, 149,
21, 240, 192,
21, 240, 234,
64, 21, 21,
64, 21, 64,
64, 21, 106,
64, 21, 149,
64, 21, 192,
64, 21, 234,
64, 57, 21,
64, 57, 64,
64, 57, 106,
64, 57, 149,
64, 57, 192,
64, 57, 234,
64, 94, 21,
64, 94, 64,
64, 94, 106,
64, 94, 149,
64, 94, 192,
64, 94, 234,
64, 130, 21,
64, 130, 64,
64, 130, 106,
64, 130, 149,
64, 130, 192,
64, 130, 234,
64, 167, 21,
64, 167, 64,
64, 167, 106,
64, 167, 149,
64, 167, 192,
64, 167, 234,
64, 203, 21,
64, 203, 64,
64, 203, 106,
64, 203, 149,
64, 203, 192,
64, 203, 234,
64, 240, 21,
64, 240, 64,
64, 240, 106,
64, 240, 149,
64, 240, 192,
64, 240, 234,
106, 21, 21,
106, 21, 64,
106, 21, 106,
106, 21, 149,
106, 21, 192,
106, 21, 234,
106, 57, 21,
106, 57, 64,
106, 57, 106,
106, 57, 149,
106, 57, 192,
106, 57, 234,
106, 94, 21,
106, 94, 64,
106, 94, 106,
106, 94, 149,
106, 94, 192,
106, 94, 234,
106, 130, 21,
106, 130, 64,
106, 130, 106,
106, 130, 149,
106, 130, 192,
106, 130, 234,
106, 167, 21,
106, 167, 64,
106, 167, 106,
106, 167, 149,
106, 167, 192,
106, 167, 234,
106, 203, 21,
106, 203, 64,
106, 203, 106,
106, 203, 149,
106, 203, 192,
106, 203, 234,
106, 240, 21,
106, 240, 64,
106, 240, 106,
106, 240, 149,
106, 240, 192,
106, 240, 234,
149, 21, 21,
149, 21, 64,
149, 21, 106,
149, 21, 149,
149, 21, 192,
149, 21, 234,
149, 57, 21,
149, 57, 64,
149, 57, 106,
149, 57, 149,
149, 57, 192,
149, 57, 234,
149, 94, 21,
149, 94, 64,
149, 94, 106,
149, 94, 149,
149, 94, 192,
149, 94, 234,
149, 130, 21,
149, 130, 64,
149, 130, 106,
149, 130, 149,
149, 130, 192,
149, 130, 234,
149, 167, 21,
149, 167, 64,
149, 167, 106,
149, 167, 149,
149, 167, 192,
149, 167, 234,
149, 203, 21,
149, 203, 64,
149, 203, 106,
149, 203, 149,
149, 203, 192,
149, 203, 234,
149, 240, 21,
149, 240, 64,
149, 240, 106,
149, 240, 149,
149, 240, 192,
149, 240, 234,
192, 21, 21,
192, 21, 64,
192, 21, 106,
192, 21, 149,
192, 21, 192,
192, 21, 234,
192, 57, 21,
192, 57, 64,
192, 57, 106,
192, 57, 149,
192, 57, 192,
192, 57, 234,
192, 94, 21,
192, 94, 64,
192, 94, 106,
192, 94, 149,
192, 94, 192,
192, 94, 234,
192, 130, 21,
192, 130, 64,
192, 130, 106,
192, 130, 149,
192, 130, 192,
192, 130, 234,
192, 167, 21,
192, 167, 64,
192, 167, 106,
192, 167, 149,
192, 167, 192,
192, 167, 234,
192, 203, 21,
192, 203, 64,
192, 203, 106,
192, 203, 149,
192, 203, 192,
192, 203, 234,
192, 240, 21,
192, 240, 64,
192, 240, 106,
192, 240, 149,
192, 240, 192,
192, 240, 234,
234, 21, 21,
234, 21, 64,
234, 21, 106,
234, 21, 149,
234, 21, 192,
234, 21, 234,
234, 57, 21,
234, 57, 64,
234, 57, 106,
234, 57, 149,
234, 57, 192,
234, 57, 234,
234, 94, 21,
234, 94, 64,
234, 94, 106,
234, 94, 149,
234, 94, 192,
234, 94, 234,
234, 130, 21,
234, 130, 64,
234, 130, 106,
234, 130, 149,
234, 130, 192,
234, 130, 234,
234, 167, 21,
234, 167, 64,
234, 167, 106,
234, 167, 149,
234, 167, 192,
234, 167, 234,
234, 203, 21,
234, 203, 64,
234, 203, 106,
234, 203, 149,
234, 203, 192,
234, 203, 234,
234, 240, 21,
234, 240, 64,
234, 240, 106,
234, 240, 149,
234, 240, 192,
234, 240, 234,
0, 0, 0,
0, 0, 0,
0, 0, 0,
};
#endif /*end yxl 2004-07-27  add this section for ipanel color pallette test*/


/* Exported Variables ------------------------------------------------------- */
/*  This array implements completely the palette specified on
    DTT MHEG_5specification, pages 31 - 35. It is specified in the YCrCb croma
    order of entries suitable for STGXOBJ 6888 AYCbCr  colortype
                     T    Y   Cb   Cr                */
static  U8 P8[256*4] =  {  0,  16, 128, 128,   /* index 0  */
                    63,  16, 128, 128,
                    63,  20, 150, 124,
                    63,  25, 172, 120,
                    63,  31, 195, 117,
                    63,  36, 218, 113,
                    63,  40, 239, 109,
                    63,  41, 113, 109,
                    63,  46, 135, 105,
                    63,  51, 157, 101,
                    63,  56, 180,  98,
                    63,  61, 203,  94,
                    63,  66, 225,  91,
                    63,  67,  98,  90,
                    63,  72, 120,  86,
                    63,  77, 143,  83,
                    63,  82, 165,  79,
                    63,  87, 188,  75,
                    63,  92, 210,  72,
                    63,  93,  83,  71,
                    63,  98, 105,  67,
                    63, 103, 127,  64,
                    63, 108, 150,  60,
                    63, 113, 173,  56,
                    63, 118, 195,  53,
                    63, 119,  68,  52,
                    63, 124,  90,  48,
                    63, 129, 113,  45,
                    63, 134, 135,  41,
                    63, 139, 158,  37,
                    63, 144, 180,  34,
                    63, 144,  53,  34,
                    63, 149,  76,  30,
                    63, 154,  98,  26, 
                    63, 159, 121,  23,
                    63, 164, 143,  19,
                    63, 169, 165,  16,
                    63,  29, 120, 150,
                    63,  34, 142, 146,
                    63,  39, 165, 143,
                    63,  44, 188, 139,
                    63,  49, 210, 135,
                    63,  54, 232, 132,
                    63,  54, 105, 131,
                    63,  59, 128, 128,
                    63,  64, 150, 124,
                    63,  69, 173, 120,
                    63,  74, 195, 117,
                    63,  79, 217, 113,
                    63,  80,  90, 112,
                    63,  85, 113, 109, /* 50 */
                    63,  90, 135, 105,
                    63,  95, 158, 101,
                    63, 100, 180,  98,
                    63, 105, 202,  94,
                    63, 106,  75,  93,
                    63, 111,  98,  90,
                    63, 116, 120,  86,
                    63, 121, 143,  82,
                    63, 126, 165,  79,
                    63, 131, 187,  75,
                    63, 132,  60,  75,
                    63, 137,  83,  71,
                    63, 142, 105,  67,
                    63, 147, 128,  64,
                    63, 152, 150,  60,
                    63, 157, 172,  56,
                    63, 157,  46,  56,
                    63, 162,  68,  52,
                    63, 167,  91,  49,
                    63, 172, 113,  45,
                    63, 177, 136,  41,
                    63, 182, 158,  38,
                    63,  42, 112, 172,
                    63,  47, 135, 169,
                    63,  52, 157, 165,
                    63,  57, 180, 161,
                    63,  62, 202, 158,
                    63,  67, 224, 154,
                    63,  67,  98, 154,
                    63,  72, 120, 150,
                    63,  77, 142, 146,
                    63,  82, 165, 143,
                    63,  87, 188, 139,
                    63,  92, 210, 135,
                    63,  93,  83, 135,
                    63,  98, 105, 131,
                    63, 103, 128, 128,
                    63, 108, 150, 124,
                    63, 113, 173, 120,
                    63, 118, 195, 117,
                    63, 119,  68, 116,
                    63, 124,  90, 112,
                    63, 129, 112, 108,
                    63, 134, 135, 105,
                    63, 139, 158, 101,
                    63, 144, 180,  97,
                    63, 145,  53,  97,
                    63, 150,  75,  93,
                    63, 155,  98,  90,
                    63, 160, 120,  86, /* 100 */
                    63, 165, 143,  82,
                    63, 170, 165,  79,
                    63, 170,  38,  79,
                    63, 175,  61,  75,
                    63, 180,  83,  71,
                    63, 185, 106,  68,
                    63, 190, 128,  64,
                    63, 195, 150,  60,
                    63,  55, 105, 195,
                    63,  60, 127, 191,
                    63,  65, 149, 188,
                    63,  70, 172, 184,
                    63,  75, 195, 180,
                    63,  80, 217, 177,
                    63,  81,  90, 176,
                    63,  86, 112, 173,
                    63,  91, 135, 169,
                    63,  96, 157, 165,
                    63, 101, 180, 162,
                    63, 106, 202, 158,
                    63, 106,  75, 158,
                    63, 111,  97, 154,
                    63, 116, 120, 150,
                    63, 122, 143, 147,
                    63, 127, 165, 143,
                    63, 131, 187, 139,
                    63, 133,  60, 138,
                    63, 138,  82, 135,
                    63, 143, 105, 131,
                    63, 148, 128, 128,
                    63, 153, 150, 124,
                    63, 158, 172, 120,
                    63, 158,  45, 120,
                    63, 163,  67, 116,
                    63, 168,  90, 112,
                    63, 173, 113, 109,
                    63, 178, 135, 105,
                    63, 183, 157, 102,
                    63, 184,  30, 101,
                    63, 189,  53,  98,
                    63, 194,  75,  94,
                    63, 199,  98,  90,
                    63, 204, 121,  87,
                    63, 209, 142,  83,
                    63,  68,  97, 218,
                    63,  73, 120, 214,
                    63,  78, 142, 210,
                    63,  83, 165, 207,
                    63,  88, 187, 203,
                    63,  93, 209, 199, /* 150 */
                    63,  94,  82, 199,
                    63,  99, 105, 195,
                    63, 104, 127, 191,
                    63, 109, 150, 188,
                    63, 114, 172, 184,
                    63, 119, 194, 181,
                    63, 120,  67, 180,
                    63, 125,  90, 176,
                    63, 130, 112, 173,
                    63, 135, 135, 169,
                    63, 140, 157, 165,
                    63, 145, 179, 162,
                    63, 146,  52, 161,
                    63, 151,  75, 157,
                    63, 156,  97, 154,
                    63, 161, 120, 150,
                    63, 166, 142, 146,
                    63, 171, 164, 143,
                    63, 171,  37, 142,
                    63, 176,  60, 138,
                    63, 181,  82, 135,
                    63, 187, 105, 131,
                    63, 192, 128, 128,
                    63, 196, 149, 124,
                    63, 197,  23, 124,
                    63, 202,  45, 120,
                    63, 207,  68, 116,
                    63, 212,  91, 113,
                    63, 217, 113, 109,
                    63, 222, 135, 106,
                    63,  81,  90, 239,
                    63,  86, 112, 236,
                    63,  91, 135, 232,
                    63,  96, 157, 229,
                    63, 101, 180, 225,
                    63, 106, 202, 221,
                    63, 107,  75, 221,
                    63, 112,  97, 217,
                    63, 117, 120, 213,
                    63, 122, 143, 210,
                    63, 127, 165, 206,
                    63, 132, 187, 203,
                    63, 132,  60, 202,
                    63, 137,  82, 198,
                    63, 142, 105, 195,
                    63, 147, 128, 191,
                    63, 152, 150, 187,
                    63, 157, 172, 184,
                    63, 159,  45, 183,
                    63, 164,  67, 179, /* 200 */
                    63, 169,  90, 176,
                    63, 174, 113, 172,
                    63, 179, 135, 168,
                    63, 184, 157, 165,
                    63, 184,  30, 164,
                    63, 189,  52, 160,
                    63, 194,  75, 157,
                    63, 199,  98, 153,
                    63, 204, 120, 149,
                    63, 209, 142, 146,
                    63, 210,  16, 146,
                    63, 215,  38, 142,
                    63, 220,  60, 138,
                    63, 225,  83, 135,
                    63, 230, 106, 131,
                    63, 235, 128, 128, /* 216 */
                    32,  16, 128, 128, /* 217 semi transparent entries */
                    32,  28, 184, 118,
                    32,  40, 239, 109,
                    32,  80,  90,  80,
                    32,  93, 146,  71,
                    32, 105, 202,  62,
                    32, 144,  53,  34,
                    32, 157, 110,  25,
                    32, 169, 165,  16,
                    32,  48, 109, 134,
                    32,  61, 165, 175,
                    32,  73, 221, 166,
                    32, 113,  71, 137,
                    32, 125, 128, 128,
                    32, 138, 183, 118,
                    32, 177,  34,  90,
                    32, 189,  91,  81,
                    32, 202, 146,  72,
                    32,  81,  90, 239,
                    32,  94, 146, 230,
                    32, 106, 202, 221,
                    32, 146,  52, 192,
                    32, 158, 109, 183,
                    32, 170, 164, 174,
                    32, 210,  16, 146,
                    32, 222,  72, 137,
                    32, 235, 128, 128,  /* 243 semi transparent last entry */
                     0,   0,   0,   0,  /* 244...255: reserved for */
                     0,   0,   0,   0,  /* manufacturer use        */
                     0,   0,   0,   0,
                     0,   0,   0,   0,
                     0,   0,   0,   0,
                     0,   0,   0,   0,
                     0,   0,   0,   0,
                     0,   0,   0,   0,
                     0,   0,   0,   0,
                     0,   0,   0,   0,
                     0,   0,   0,   0,
                     0,   0,   0,   0}; /* 255 */
#endif

