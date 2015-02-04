#include <stdlib.h>
#include <stdio.h>

#include "DzGlWindow.h"

CDzGlWindow::~CDzGlWindow()
{

}

CDzGlWindow::CDzGlWindow(int x, int y, int w, int h, const char *l) :
Fl_Gl_Window(x, y, w, h, l) 
{

}

//渲染2D纹理
void CDzGlWindow::draw() {
	// the valid() property may be used to avoid reinitializing your
	// GL transformation for each redraw:
	if (!valid()) {
		valid(1);
		glLoadIdentity();
		glViewport(0, 0, w(), h());
	}
	//创建材质
	Fl_PNG_Image img("D:\\1.png");

	m_nTextureHeight = img.h();
	m_nTextureWidth = img.w();

	colorRGBA *pPngA = nullptr;
	colorRGB *pPng = nullptr;
	switch (img.d())
	{
	case 1:
		//pPng = (colorRGBA*)new uchar[img.w() * img.h() * img.d()];
		break;

	case 2:
		//pPng = (colorRGBA*)new uchar[img.w() * img.h() * img.d()];
		break;

	case 3://RGB
		pPng = (colorRGB*)new uchar[img.w() * img.h() * img.d()];
		for (int y = 0; y < img.h(); y++)
		{
			for (int x = 0; x < img.w(); x++)
			{
				pPng[y*img.w() + x].r = img.array[y * img.w() + x * img.d() + 2];
				pPng[y*img.w() + x].g = img.array[y * img.w() + x * img.d() + 1];
				pPng[y*img.w() + x].b = img.array[y * img.w() + x * img.d() + 0];
			}
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.w(), img.h(), 0, GL_RGB, GL_UNSIGNED_BYTE, pPng);
		break;

	case 4://RGBA
		pPngA = (colorRGBA*)new uchar[img.w() * img.h() * img.d()];
		for (int y = 0; y < img.h(); y++)
		{
			for (int x = 0; x < img.w(); x++)
			{
				int n = 0;
				pPngA[y*img.w() + x].r = img.array[y*img.w() + x * 4 + 3];
				pPngA[y*img.w() + x].g = img.array[y*img.w() + x * 4 + 2];
				pPngA[y*img.w() + x].b = img.array[y*img.w() + x * 4 + 1];
				pPngA[y*img.w() + x].a = img.array[y*img.w() + x * 4 + 0];
			}
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.w(), img.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pPngA);
		break;

	default:
		printf("void CDzGlWindow::LoadTexture(void)----error\n");
		break;
	}

	if (pPng != nullptr)
		delete[]pPng;

	if (pPngA != nullptr)
		delete[]pPngA;


	//显示纹理
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glViewport(0, 0, img.w(), img.h());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texName);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 1.0f);
	glEnd();

	glFlush();

	//img.draw(0, 0);
	img.uncache();
}

void CDzGlWindow::draw_overlay() 
{
	// the valid() property may be used to avoid reinitializing your
	// GL transformation for each redraw:
}

void CDzGlWindow::LoadTexture(void)
{

}
