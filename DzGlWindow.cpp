#include "DzGlWindow.h"

extern Fl_Tree *g_pTree;

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
	//显示纹理
	//重置当前矩阵
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//清空屏幕
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//开启2D纹理
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_ALPHA8);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//抗锯齿
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points  
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines 

	//开启混合模式
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//阿尔法混合

	for (auto it : m_TexList)
	{
		if (!it->bShow)
			continue;

		//缩放矩形
		//glScalef(m_fPngScale, m_fPngScale, 1.0);
		//居中显示矩形,设置矩形位置
		glViewport(it->pos.x, it->pos.y, (int)(it->width * m_fPngScale), (int)(it->height * m_fPngScale));
		//绑定贴图
		glBindTexture(GL_TEXTURE_2D, it->tex);

		if (it->tex == 0)
			continue;

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
	}

	glFlush();
}

void CDzGlWindow::draw_overlay()
{
	// the valid() property may be used to avoid reinitializing your
	// GL transformation for each redraw:
}

GLuint CDzGlWindow::LoadMemoryPNGTexture(const unsigned char *pData, int nHeight, int nWidth)
{
	GLuint texName = 0;
	STexInfo *pTex = nullptr;

	if (pData == nullptr)
		return 0;

	ReleaseTexture();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);

	if (0 != texName)
	{
		pTex = new STexInfo();
		pTex->height = nHeight;
		pTex->tex = texName;
		pTex->type = eRGBA;
		pTex->width = nWidth;
		pTex->pos.x = 0;
		pTex->pos.y = 0;
	}

	if (nullptr != pTex)
	{
		//将贴图信息压入列表
		m_fPngScale = 1.f;
		m_TexList.push_back(TEX2DINFOLIST::value_type(pTex));
	}

	return texName;
}

GLuint CDzGlWindow::LoadMemoryPNGTextures(CPackTexture::SBigTextureInfo const *TextureInfo)
{
	GLuint tex = LoadMemoryPNGTexture((const unsigned char *)TextureInfo->pData, TextureInfo->nHeight, TextureInfo->nWidth );
	
	if (tex > 0)
	{//将纹理信息写入树形列表
		char szTextureName[512] = { 0 };
		std::string szChileName;
		sprintf_s(szTextureName, "Texture_%d", m_BigTextureInfoList.size());
		g_pTree->add(szTextureName);

		for (auto it : TextureInfo->MinImgList)
		{
			szChileName.clear();

			szChileName = szTextureName;
			szChileName += '/';
			szChileName += it.szPath;
			g_pTree->add(szChileName.c_str());
		}

		m_BigTextureInfoList.push_back(BIG_TEXTURE_INFO_LIST::value_type(TextureInfo));
	}	

	return tex;
}

void CDzGlWindow::OnlyFilename(char *pszStr)
{

}

GLuint CDzGlWindow::LoadPNGTexture(string szPath, const unsigned char *pData, int nDataLen)
{	
	//创建材质
	Fl_PNG_Image img(szPath.c_str());
	GLuint texName = 0;

	if (nullptr == img.array)
		return 0;

	STexInfo *pTex = nullptr;

	m_fPngScale = 1.f;

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
			pTex->pos.x = 100;
			pTex->pos.y = 100;
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
			pTex->pos.x = 100;
			pTex->pos.y = 100;
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

	//纹理被释放的同时树形列表中的所有节点也要被释放
	g_pTree->clear();

	m_TexList.clear();
}

int CDzGlWindow::handle(int e)
{
	switch (e)
	{
	case FL_MOUSEWHEEL:
		if (Fl::event_dy() > 0)
		{//缩小图片
			m_fPngScale -= (fabsf(Fl::event_dy() * 0.01f));			
		}
		else
		{//放大图片
			m_fPngScale += (fabsf(Fl::event_dy() * 0.01f));
		}
		redraw();

		break;

	case FL_PUSH://鼠标按下
		m_bIsMouseDown = true;
		m_MousePos.x = Fl::event_x();
		m_MousePos.y = Fl::event_y();
		printf("push\n");
		break;

	case FL_RELEASE://鼠标弹起
		m_bIsMouseDown = false;
		printf("release\n");
		break;

	default:
		if (m_bIsMouseDown)
		{//鼠标处于按下则移动图片
			for (auto it : m_TexList)
			{
				it->pos.x -= (m_MousePos.x - Fl::event_x());
				it->pos.y += (m_MousePos.y - Fl::event_y());
				printf("X:%d     Y:%d\n", it->pos.x, it->pos.y);
			}			
			redraw();

			m_MousePos.x = Fl::event_x();
			m_MousePos.y = Fl::event_y();
		}		
		break;
	}

	return 1;
}

void CDzGlWindow::SetPngPos(Vec2 pos)
{

}
