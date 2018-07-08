#ifndef _CASIA_CG_PROJECT_ZHAOJING_
#define _CASIA_CG_PROJECT_ZHAOJING_



struct TextureTga            // ����һ���ṹ��
{
	GLubyte *imageData;           // ͼ������ (���32bit)
	GLuint bpp;            // ÿһ���ص�ͼ����ɫ���
	GLuint width;            // ͼ����
	GLuint height;            // ͼ��߶�
	GLuint texID;            // ����ID
};

// ����BMP,JPG,GIF���ļ�
bool BuildTexture(char *szPathName, GLuint &texid);

// ����TGA�ļ�
bool BuildTexture(char *filename, TextureTga *texture);



#endif

