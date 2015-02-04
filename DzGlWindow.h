#pragma once
//openGL
#include <Fl/gl.h>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/Fl_PNG_Image.H>

class CDzGlWindow : public Fl_Gl_Window 
{
	virtual void draw() override;
	virtual void draw_overlay() override;
public://enum
	struct colorRGBA
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};

	struct colorRGB
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

public:
	int sides;
	int overlay_sides;
	CDzGlWindow(int x, int y, int w, int h, const char *l = 0);
	~CDzGlWindow(void);

	//加载材质
	void LoadTexture(void);
private:
	GLuint texName;
	int m_nTextureHeight;
	int m_nTextureWidth;
};
