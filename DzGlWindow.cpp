#include <stdlib.h>
#include <stdio.h>

#include "DzGlWindow.h"

CDzGlWindow::~CDzGlWindow()
{
	Release();
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

	printf("screen W:%d H:%d\n", w(), h());

	//显示纹理
	//重置当前矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//居中显示矩形
	glViewport((w() / 2 - (m_nTextureWidth / 2)), (h() / 2) - (m_nTextureHeight / 2), m_nTextureWidth, m_nTextureHeight);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texName);
	if (texName == 0)
		return;

	//缩放矩形
	//glScalef(0.5, 0.5, 1.0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 1.0);
	
	glEnd();

	glFlush();
}

void CDzGlWindow::draw_overlay()
{
	// the valid() property may be used to avoid reinitializing your
	// GL transformation for each redraw:
}

GLuint CDzGlWindow::LoadPNGTexture(string szPath)
{
	//创建材质
	Fl_PNG_Image img(szPath.c_str());

	if (nullptr == img.array)
		return 0;

	m_nTextureHeight = img.h();
	m_nTextureWidth = img.w();

	colorRGBA *pPngA = nullptr;
	colorRGB *pPng = nullptr;
	STexInfo *pTex = nullptr;

	switch (img.d())
	{
	case 1:
		//pPng = (colorRGBA*)new uchar[img.w() * img.h() * img.d()];
		printf("Unknow Texture:%d\n", img.d());
		return 0;
		break;

	case 2:
		//pPng = (colorRGBA*)new uchar[img.w() * img.h() * img.d()];
		printf("Unknow Texture:%d\n", img.d());
		return 0;
		break;

	case 3://RGB
		pPng = (colorRGB*)new uchar[img.w() * img.h() * img.d()];
		
		memset(pPng, 0, img.w() * img.h() * img.d());
		memcpy(pPng, img.array, img.w() * img.h() * img.d());

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.w(), img.h(), 0, GL_RGB, GL_UNSIGNED_BYTE, pPng);

		if (0 != texName)
		{
			pTex = new STexInfo();
			pTex->height = img.h();
			pTex->tex = texName;
			pTex->type = eRGB;
			pTex->width = img.w();
		}

		break;

	case 4://RGBA
		pPngA = (colorRGBA*)new uchar[img.w() * img.h() * img.d()];
		memset(pPngA, 0, img.w() * img.h() * img.d());
		memcpy(pPngA, img.array, img.w() * img.h() * img.d());

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.w(), img.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pPngA);

		if (0 != texName)
		{
			pTex = new STexInfo();
			pTex->height = img.h();
			pTex->tex = texName;
			pTex->type = eRGBA;
			pTex->width = img.w();
		}

		break;

	default:
		printf("void CDzGlWindow::LoadTexture(void)----error\n");
		break;
	}

	if (nullptr != pTex)
	{
		//将贴图信息压入列表
		m_TexList.push_back(TEX2DINFOLIST::value_type(pTex));
	}

	if (pPng != nullptr)
		delete[]pPng;

	if (pPngA != nullptr)
		delete[]pPngA;

	img.uncache();

	return texName;
}

void CDzGlWindow::Release(void)
{
	for (auto it : m_TexList)
	{
		if (it != nullptr)
			delete it;
	}

	m_TexList.clear();
}
