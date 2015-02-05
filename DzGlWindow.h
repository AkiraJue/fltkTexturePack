#pragma once
//openGL
#include <Fl/gl.h>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/Fl_PNG_Image.H>
#include <vector>
#include <string>

using namespace std;

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
		int width;		//贴图宽
		int height;		//贴图高
		STexInfo():tex(0), type(eColorMax), width(0), height(0){}
	};

	typedef vector<STexInfo*> TEX2DINFOLIST;//贴图信息列表
public:
	int sides;
	int overlay_sides;
	CDzGlWindow(int x, int y, int w, int h, const char *l = 0);
	~CDzGlWindow(void);

	void Release(void);

	//加载材质
	//返回值:
	//	成功:贴图句柄
	//	失败:0
	GLuint LoadPNGTexture(string szPath);
private:
	GLuint texName;
	int m_nTextureHeight;
	int m_nTextureWidth;

	TEX2DINFOLIST m_TexList;//贴图信息列表
};
