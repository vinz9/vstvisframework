#ifndef SCENE_H_
#define SCENE_H_

#include "Tree.h"
#include "Box.h"
#include "fftBox.h"
#include "Flower.h"
#include "RandomLine.h"
#include "Alga.h"
#include "NurbsBox.h"
#include "FreqCubes.h"
#include "FreqSpheres.h"

#include "../VstPlugin.h"
#include "../AudioInput.h"

//#include "../glFonts.h"

#define NUMINPUTS 10
#define NUMOBJECTS 4

#define WIDTH 800
#define HEIGHT 600

#define SIZE 1024

typedef struct												// Create A Structure
{
	GLubyte	*imageData;										// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel.
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
} TextureImage;												// Structure Name

class Scene
{

public:

	Scene(AudioEffect* effect);
	~Scene();
	
	virtual void initScene() = 0;
	virtual void drawScene() = 0;
	virtual void updateScene() = 0;
	virtual void initInputsAndGl() = 0;

	GLuint emptyTexture(int size);
	void drawBlur(int times, float alpha, float d, float alphad);
	void kalei(int pattern);

	bool LoadTGA(TextureImage *texture, char *filename);
	void invertColors(int mode, int pattern);




	TextureImage textures[1];

	Object3d* objects3d[NUMOBJECTS];
	AudioEffect* effect;

	GLuint	BlurTexture;

private:

};

#endif