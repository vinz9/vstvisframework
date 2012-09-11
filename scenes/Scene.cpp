#include "Scene.h"

//--------------------------------------------------
Scene::Scene(AudioEffect* effect):effect(effect)
{
	for (int i = 0; i<NUMOBJECTS; i++)
	{
		objects3d[i] = NULL;
	}

	BlurTexture = emptyTexture(SIZE);
	

}

//--------------------------------------------------
Scene::~Scene()
{
	for (int i = 0; i<NUMOBJECTS; i++)
	{
		if (objects3d[i])
			delete objects3d[i];
	}
}

//----------------------------------------------------------------------------
GLuint Scene::emptyTexture(int size)										
{
	GLuint txtnumber;											// Texture ID
	unsigned int* data;											// Stored Data


	data = (unsigned int*)new GLuint[((size * size)* 4 * sizeof(unsigned int))];
	ZeroMemory(data,((size * size)* 4 * sizeof(unsigned int)));	// Clear Storage Memory

	glGenTextures(1, &txtnumber);								// Create 1 Texture
	glBindTexture(GL_TEXTURE_2D, txtnumber);					// Bind The Texture
	glTexImage2D(GL_TEXTURE_2D, 0, 4, size, size, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, data);						// Build Texture Using Information In data
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	delete [] data;												// Release data

	return txtnumber;											// Return The Texture ID
}

//----------------------------------------------------------------------------
void Scene::drawBlur(int times, float alpha, float d, float alphad)
{											

	d = d * (float)WIDTH/(float)SIZE;
	float tx = (float)WIDTH/(float)SIZE;
	float ty = (float)HEIGHT/(float)SIZE;

	// Disable AutoTexture Coordinates
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);



	glEnable(GL_TEXTURE_2D);									// Enable 2D Texture Mapping
	glDisable(GL_DEPTH_TEST);									// Disable Depth Testing
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);							// Set Blending Mode
	glEnable(GL_BLEND);											// Enable Blending
	glBindTexture(GL_TEXTURE_2D,BlurTexture);					// Bind To The Blur Texture

	glMatrixMode(GL_PROJECTION);								// Select Projection
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix
	glOrtho( 0.0, 1.0 ,  0.0, 1.0, -1, 1 );							// Select Ortho Mode (640x480)
	glMatrixMode(GL_MODELVIEW);									// Select Modelview Matrix
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix

	glBegin(GL_QUADS);											// Begin Drawing Quads

	for (int i = 1; i < times+1; i++)						// Number Of Times To Render Blur
	{
		glColor4f(1.0f, 1.0f, 1.0f, alpha - alphad*i);								

		glTexCoord2f(0.0+d*i,0.0+d*i); glVertex2f(0.0,0.0);									
		glTexCoord2f(0.0+d*i,ty-d*i);   glVertex2f(0.0,1.0);									
		glTexCoord2f(tx-d*i,ty-d*i); glVertex2f(1.0,1.0);							
		glTexCoord2f(tx-d*i,0.0+d*i); glVertex2f(1.0,0.0);
	}

	glEnd();												// Done Drawing Quads

	glMatrixMode( GL_PROJECTION );								// Select Projection
	glPopMatrix();												// Pop The Matrix
	glMatrixMode( GL_MODELVIEW );								// Select Modelview
	glPopMatrix();												// Pop The Matrix

	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);									// Disable 2D Texture Mapping
	glDisable(GL_BLEND);										// Disable Blending
	//glBindTexture(GL_TEXTURE_2D,0);								// Unbind The Blur Texture


}

//---------------------------------------------------------------------------------
bool Scene::LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
{    
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type=GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");						// Open The TGA File

	if(	file==NULL ||										// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
			return false;									// Return False
		else
		{
			fclose(file);									// If Anything Failed, Close The File
			return false;									// Return False
		}
	}

	texture->width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)
    
 	if(	texture->width	<=0	||								// Is The Width Less Than Or Equal To Zero
		texture->height	<=0	||								// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
	{
		fclose(file);										// If Anything Failed, Close The File
		return false;										// Return False
	}

	texture->bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= texture->bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData=(GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data

	if(	texture->imageData==NULL ||							// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData!=NULL)						// Was Image Data Loaded
			free(texture->imageData);						// If So, Release The Image Data

		fclose(file);										// Close The File
		return false;										// Return False
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
	{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);											// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);					// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);			// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	
	if (texture[0].bpp==24)									// Was The TGA 24 Bits
	{
		type=GL_RGB;										// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;											// Texture Building Went Ok, Return True
}

//----------------------------------------------------------------------------
void Scene::invertColors(int mode, int pattern)
{	

	float t,ds;
	t = ((VstPlugin*)effect)->temps->getElapsedTime();
	glEnable(GL_TEXTURE_2D);									// Enable 2D Texture Mapping
	glDisable(GL_DEPTH_TEST);									// Disable Depth Testing
	glDisable(GL_LIGHTING);

	if (mode == 0) glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ONE_MINUS_SRC_ALPHA); 							
	else glBlendFunc(GL_ZERO,GL_ONE_MINUS_DST_COLOR);

	glEnable(GL_BLEND);											// Enable Blending
	glBindTexture(GL_TEXTURE_2D,textures[0].texID);					// Bind To The Blur Texture

	glMatrixMode(GL_PROJECTION);								// Select Projection
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix
	glOrtho( 0.0, 1.0 ,  0.0, 1.0, -1, 1 );							// Select Ortho Mode (640x480)
	glMatrixMode(GL_MODELVIEW);									// Select Modelview Matrix
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix

	//float c = 0.5*(sin(t*2)+1);
	//glColor4f(c,c,c,c);

	if (pattern == 0)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0,0.0); glVertex2f(0.0,0.0);									
			glTexCoord2f(0.0,1.0);   glVertex2f(0.0,1.0);									
			glTexCoord2f(1.0,1.0); glVertex2f(1.0,1.0);							
			glTexCoord2f(1.0,0.0); glVertex2f(1.0,0.0);
		glEnd();
	}
	else if(pattern ==1)
	{
		glBegin(GL_QUADS);											// Begin Drawing Quads
			glTexCoord2f(0.0,0.0); glVertex2f(0.0,0.0);									
			glTexCoord2f(0.0,1.0);   glVertex2f(0.0,0.5);									
			glTexCoord2f(1.0,1.0); glVertex2f(0.5,0.5);							
			glTexCoord2f(1.0,0.0); glVertex2f(0.5,0.0);

			glTexCoord2f(0.0,0.0); glVertex2f(0.5,0.5);									
			glTexCoord2f(0.0,1.0);   glVertex2f(0.5,1.0);									
			glTexCoord2f(1.0,1.0); glVertex2f(1.0,1.0);							
			glTexCoord2f(1.0,0.0); glVertex2f(1.0,0.5);
		glEnd();
	}
	else
	{
		
		ds = (sin(2*t)+1)*0.375;
		glBegin(GL_QUADS);
			glTexCoord2f(0.0,0.0); glVertex2f(0.0,0.0+ds);									
			glTexCoord2f(0.0,1.0);   glVertex2f(0.0,0.25+ds);									
			glTexCoord2f(1.0,1.0); glVertex2f(1.0,0.25+ds);							
			glTexCoord2f(1.0,0.0); glVertex2f(1.0,0.0+ds);
		glEnd();
	}

	glMatrixMode( GL_PROJECTION );								// Select Projection
	glPopMatrix();												// Pop The Matrix
	glMatrixMode( GL_MODELVIEW );								// Select Modelview
	glPopMatrix();												// Pop The Matrix

	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);									// Disable 2D Texture Mapping
	glDisable(GL_BLEND);										// Disable Blending

}

//---------------------------------------------------
void Scene::kalei(int pattern)
{											

//	d = d * (float)WIDTH/(float)SIZE;
	float tx = (float)WIDTH/(float)SIZE;
	float ty = (float)HEIGHT/(float)SIZE;

	// Disable AutoTexture Coordinates
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);



	glEnable(GL_TEXTURE_2D);									// Enable 2D Texture Mapping
	glDisable(GL_DEPTH_TEST);									// Disable Depth Testing
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);							// Set Blending Mode
	//glEnable(GL_BLEND);											// Enable Blending
	glBindTexture(GL_TEXTURE_2D,BlurTexture);					// Bind To The Blur Texture

	glMatrixMode(GL_PROJECTION);								// Select Projection
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix
	glOrtho( 0.0, 1.0 ,  0.0, 1.0, -1, 1 );							// Select Ortho Mode (640x480)
	glMatrixMode(GL_MODELVIEW);									// Select Modelview Matrix
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();

	glColor4f(1.0,1.0,1.0,1.0);

	switch (pattern)
	{
	case 0 :

	glBegin(GL_QUADS);						

	glTexCoord2f(0.0,0.0); glVertex2f(0.0,0.0);									
		glTexCoord2f(0.0,ty*0.5);   glVertex2f(0.0,0.5);									
		glTexCoord2f(tx,ty*0.5); glVertex2f(1.0,0.5);							
		glTexCoord2f(tx,0.0); glVertex2f(1.0,0.0);

	glEnd();	
	
		glBegin(GL_QUADS);						

	glTexCoord2f(0.0,ty*0.5); glVertex2f(0.0,0.5);									
		glTexCoord2f(0.0,0);   glVertex2f(0.0,1.0);									
		glTexCoord2f(tx,0); glVertex2f(1.0,1.0);							
		glTexCoord2f(tx,ty*0.5); glVertex2f(1.0,0.5);

	glEnd();
	
	break;
	case 1 :

	glBegin(GL_QUADS);						

	glTexCoord2f(0.0,ty); glVertex2f(0.0,0.0);									
		glTexCoord2f(0.0,ty*0.5);   glVertex2f(0.0,0.5);									
		glTexCoord2f(tx,ty*0.5); glVertex2f(1.0,0.5);							
		glTexCoord2f(tx,ty); glVertex2f(1.0,0.0);

	glEnd();	
	
		glBegin(GL_QUADS);						

	glTexCoord2f(0.0,ty*0.5); glVertex2f(0.0,0.5);									
		glTexCoord2f(0.0,ty);   glVertex2f(0.0,1.0);									
		glTexCoord2f(tx,ty); glVertex2f(1.0,1.0);							
		glTexCoord2f(tx,ty*0.5); glVertex2f(1.0,0.5);

	glEnd();
	break;
	case 2 :

		glBegin(GL_QUADS);						

	glTexCoord2f(0.0,0.0); glVertex2f(0.0,0.0);									
		glTexCoord2f(0.0,ty);   glVertex2f(0.0,1.0);									
		glTexCoord2f(tx*0.5,ty); glVertex2f(0.5,1.0);							
		glTexCoord2f(tx*0.5,0.0); glVertex2f(0.5,0.0);

	glEnd();	
	
		glBegin(GL_QUADS);						

	glTexCoord2f(tx*0.5,0); glVertex2f(0.5,0.0);									
		glTexCoord2f(tx*0.5,ty);   glVertex2f(0.5,1.0);									
		glTexCoord2f(0.0,ty); glVertex2f(1.0,1.0);							
		glTexCoord2f(0.0,0); glVertex2f(1.0,0.0);

	glEnd();
	break;
	default:
		break;
	}

	glMatrixMode( GL_PROJECTION );								// Select Projection
	glPopMatrix();												// Pop The Matrix
	glMatrixMode( GL_MODELVIEW );								// Select Modelview
	glPopMatrix();												// Pop The Matrix

	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);									// Disable 2D Texture Mapping
	glDisable(GL_BLEND);										// Disable Blending
	//glBindTexture(GL_TEXTURE_2D,0);								// Unbind The Blur Texture


}