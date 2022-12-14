#include "win_dow.h"



void Window::Resize(int w, int h)
{
width=w;
height=h;
aspect_ratio=float(w)/float(h);
}
