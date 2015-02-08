#pragma once
//openGL
#include "stdafx.h"
#include "PackTexture.h"

using namespace std;

class CDzGlWindow : public Fl_Gl_Window
{
	virtual void draw() override;
	virtual void draw_overlay() override;
public://enum

	typedef std::vector< const CPackTexture::SBigTextureInfo* const > BIG_TEXTURE_INFO_LIST;

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

	struct Vec2
	{
		int x;
		int y;

		Vec2& operator=(Vec2 &v)
		{
			this->x = v.x;
			this->y = v.y;
		}
	};

	enum colorType
	{
		eRGB,
		eRGBA,
		eColorMax
	};

	struct STexInfo
	{
		GLuint tex;		//贴图句柄
		colorType type;	//贴图类型
		Vec2 pos;		//渲染位置左下角
		int width;		//贴图宽
		int height;		//贴图高
		bool bShow;		//true:渲染,	false:不渲染

		STexInfo() :tex(0), type(eColorMax), width(0), height(0){}
	};
	
	typedef vector<STexInfo*> TEX2DINFOLIST;//贴图信息列表
public:
	int sides;
	int overlay_sides;
	CDzGlWindow(int x, int y, int w, int h, const char *l = 0);
	~CDzGlWindow(void);

	//释放所有纹理
	void ReleaseTexture(void);
		
	//加载材质
	//返回值:
	//	成功:贴图句柄
	//	失败:0
	GLuint LoadPNGTexture(string szPath, const unsigned char *pData = nullptr, int nDataLen = 0);

	GLuint LoadMemoryPNGTexture(const unsigned char *pData, int nHeight, int nWidth);

	GLuint LoadMemoryPNGTextures(const CPackTexture::SBigTextureInfo *TextureInfo);

	//Vec2 GetPngPos(void) const { return m_PngPos; }
	void SetPngPos(Vec2 pos);

	float GetPngScale(void) const { return m_fPngScale; }
	void SetPngScale(float fScale){ m_fPngScale = fScale; }
	virtual int handle(int e) override;

private:
	void OnlyFilename(char *pszStr);
private:
	GLuint texName;
	GLuint texName2;
	float m_fPngScale;//图片缩放因子
	bool m_bIsMouseDown;
	Vec2 m_MousePos;
	TEX2DINFOLIST m_TexList;//贴图信息列表
	BIG_TEXTURE_INFO_LIST m_BigTextureInfoList;//贴图信息列表,包含小图信息
};
