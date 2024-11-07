#include "glloader.h"
#include <glad/glad.h>

bool CustomLoadGL(void* load)
{
	if (!gladLoadGLLoader((GLADloadproc)load)) {
		return false;
	}
	return true;
}


