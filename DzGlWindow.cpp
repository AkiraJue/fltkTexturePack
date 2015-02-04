// fltkTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <Fl.H>
#include <Fl_Window.H>
#include <Fl_Button.H>
#include <fl_ask.H>
#include <Fl_PNG_Image.H>

//openGL
#include <gl.h>
#include <Fl_Gl_Window.H>
#include <GL\include\glut.h>

//#pragma comment(lib, "glew32.lib")
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#  ifndef M_PI
#    define M_PI            3.14159265358979323846
#    define M_PI_2          1.57079632679489661923
#    define M_PI_4          0.78539816339744830962
#    define M_1_PI          0.31830988618379067154
#    define M_2_PI          0.63661977236758134308
#  endif // !M_PI

#  ifndef M_SQRT2
#    define M_SQRT2         1.41421356237309504880
#    define M_SQRT1_2       0.70710678118654752440
#  endif // !M_SQRT2

class CDzGlWindow : public Fl_Gl_Window {
	void draw();
	void draw_overlay();
public:
	int sides;
	int overlay_sides;
	GLuint texName;
	Fl_PNG_Image *m_pImg;
	CDzGlWindow(int x, int y, int w, int h, const char *l = 0);
};

CDzGlWindow::CDzGlWindow(int x, int y, int w, int h, const char *l) :
Fl_Gl_Window(x, y, w, h, l) {
	sides = overlay_sides = 3;

	//生成一张贴图
	m_pImg = new Fl_PNG_Image("D:\\1.png");
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

	// draw an amazing but slow graphic:
	glClear(GL_COLOR_BUFFER_BIT);
	//  for (int j=1; j<=1000; j++) {
	glBegin(GL_POLYGON);

	for (int j = 0; j<sides; j++) 
	{
		double ang = j * 2 * M_PI / sides;
		glColor3f(float(j) / sides, float(j) / sides, float(j) / sides);
		glVertex3f(cos(ang), sin(ang), 0);		
	}

	glDrawPixels(m_pImg->w(), m_pImg->h(), GL_RGBA8, GL_UNSIGNED_BYTE, m_pImg->data());

	glEnd();
	// }
	//glutSwapBuffers();
}

void CDzGlWindow::draw_overlay() {
	// the valid() property may be used to avoid reinitializing your
	// GL transformation for each redraw:
	//if (!valid()) {
	//	valid(1);
	//	glLoadIdentity();
	//	glViewport(0, 0, w(), h());
	//}
	//// draw an amazing graphic:
	//gl_color(FL_RED);
	//glBegin(GL_LINE_LOOP);
	//for (int j = 0; j<overlay_sides; j++) {
	//	double ang = j * 2 * M_PI / overlay_sides;
	//	glVertex3f(cos(ang), sin(ang), 0);
	//}
	//glEnd();
}

int main(int argc, char ** argv)
{
	Fl_Window window(1024, 768);

	//openGL窗口
	CDzGlWindow xglWindow(10, 75, window.w() - 20, window.h() - 90);
	window.end();
	window.show(argc, argv);

	Fl_PNG_Image img("D:\\1.png");
	
	xglWindow.show();
	xglWindow.redraw();

	return Fl::run();
}

