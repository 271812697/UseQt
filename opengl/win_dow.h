#ifndef WIN_DOW_H
#define WIN_DOW_H
class Window
{
public:
  static inline int width = 0, height = 0;
  static inline float aspect_ratio = 1.0f / 1.0f;
public:
  static void Resize(int w,int h);

};
#endif // WIN_DOW_H
