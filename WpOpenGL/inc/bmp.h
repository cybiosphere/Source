
#include <gl\gl.h>			// opengl32.lib 
/*
typedef struct _AUX_RGBImageRec {
	GLint sizeX, sizeY;
	unsigned char *data;
} AUX_RGBImageRec;*/

class AUX_RGBImageRec {
   void convertBGRtoRGB();
 public:
   byte *data;
   DWORD sizeX;
   DWORD sizeY;
   bool NoErrors;
   AUX_RGBImageRec(): NoErrors(false), data(NULL) {};
   AUX_RGBImageRec(const char *FileName);
   ~AUX_RGBImageRec();
   bool loadFile(const char *FileName);
   friend AUX_RGBImageRec *auxDIBImageLoad(const char *FileName);
};