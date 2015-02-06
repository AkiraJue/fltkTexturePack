#include <stdlib.h>
#include <stdio.h>

#include "DzGlWindow.h"

CDzGlWindow::~CDzGlWindow()
{
	ReleaseTexture();
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

	//清空屏幕
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//开启2D纹理
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	int x, y;
	x = y = 0;

	for (auto it : m_TexList)
	{
		//居中显示矩形,设置矩形位置
		glViewport((int)(x), (int)(y), it->width, it->height);
		//绑定贴图
		glBindTexture(GL_TEXTURE_2D, it->tex);

		if (it->tex == 0)
			continue;

		//缩放矩形
		//glScalef(0.5, 0.5, 1.0);

		glBegin(GL_QUADS);
			//RGBA
			glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 1.0);
			glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 1.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 1.0);
			glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 1.0);

			////BGRA
			//glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, -1.0, 1.0);//左下角
			//glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, -1.0, 1.0);// 右下角
			//glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 1.0, 1.0);// 右上角
			//glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, 1.0, 1.0);// 左上角
		glEnd();
		
		x += it->width;
	}

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
	GLuint texName = 0;

	if (nullptr == img.array)
		return 0;

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
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.w(), img.h(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.array);

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
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.w(), img.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.array);

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

	//清除图片缓存
	img.uncache();

	return texName;
}

void CDzGlWindow::ReleaseTexture(void)
{
	for (auto it : m_TexList)
	{
		if (it != nullptr)
		{
			glDeleteTextures(1, &it->tex);
			delete it;			
		}
	}

	m_TexList.clear();
}
