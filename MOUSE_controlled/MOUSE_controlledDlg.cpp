
// MOUSE_controlledDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MOUSE_controlled.h"
#include "MOUSE_controlledDlg.h"
#include "afxdialogex.h"
#include "math.h"
#include "TextureTga.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	MAX_PARTICLES  10000
#define PARTICLE_NUMBER  10000
int number = 0;
GLuint listname = 1;
float slowdown = 2.0f;   // ��������
float xspeed;    // X������ٶ�
float yspeed;
float zspeed;// Y������ٶ�
float zoom = -40.0f;   // ��Z������
/*
typedef struct //ѩ���Ľṹ��
{
	GLfloat x, y,z;
	GLfloat big;
	GLfloat life;
	GLfloat vy, vx;
	GLfloat ay, ax;
}particle;
*/

typedef struct //�𻨵Ľṹ��
{
	bool active;     // �Ƿ񼤻�
	float life, life1;     // ��������
	bool flag=true;
	float fade;     // ˥���ٶ�
	float r;     // ��ɫֵ
	float g;     // ��ɫֵ
	float b;     // ��ɫֵ
	float x;     // X λ��
	float y;     // Y λ��
	float z;     // Z λ��
	float xi;     // X ����
	float yi;     // Y ����
	float zi;     // Z ����
	float big;
	float xg;     // X �����������ٶ�
	float yg;     // Y �����������ٶ�
	float zg;     // Z �����������ٶ�
}Particle;

//ѩ��
Particle particles[PARTICLE_NUMBER];
//��
Particle particles_fire[PARTICLE_NUMBER];

//��������
Particle fire[PARTICLE_NUMBER];

//С��������
Particle star[PARTICLE_NUMBER];

GLuint texture[6];
static GLfloat colors[12][3] =    // �ʺ���ɫ
{
	{ 1.0f,0.5f,0.5f },{ 1.0f,0.75f,0.5f },{ 1.0f,1.0f,0.5f },{ 0.75f,1.0f,0.5f },
	{ 0.5f,1.0f,0.5f },{ 0.5f,1.0f,0.75f },{ 0.5f,1.0f,1.0f },{ 0.5f,0.75f,1.0f },
	{ 0.5f,0.5f,1.0f },{ 0.75f,0.5f,1.0f },{ 1.0f,0.5f,1.0f },{ 1.0f,0.5f,0.75f }
};
int color_new = 13;
float life_new = 1.0;

GLfloat angle=0;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Radio(pDX, IDC_XUEHUA, m_snow);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CMOUSE_controlledDlg �Ի���

double PI;
double tFovy;                   // ��gluPerspective��ʹ��(��OnMouseWheel��ʹ��)
CPoint prePt, nowPt;                                        // ��OnMouseMove��ʹ��
double tEyeX, tEyeY, tEyeZ;                                    // ��gluLookAt��ʹ�õ�3������
double tCenterX, tCenterY, tCenterZ;
double tUpX, tUpY, tUpZ;
double tVerticalAng, tHorizonAng, tRadius, tAngInc; // ˮƽ���򡢴�ֱ����Ľǡ��뾶


CMOUSE_controlledDlg::CMOUSE_controlledDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMOUSE_controlledDlg::IDD, pParent)	
	, m_num(0)
	, m_color(0)
	, m_life(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMOUSE_controlledDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_SNOW, m_snowflower);
	//  DDX_Control(pDX, IDC_SNOW, m_snow);
	DDX_Text(pDX, IDC_NUM, m_num);
	DDV_MinMaxInt(pDX, m_num, 0, 10100);
	DDX_Text(pDX, IDC_EDIT1, m_color);
	//  DDX_Text(pDX, IDC_EDIT2, m_life);
	//  DDV_MinMaxInt(pDX, m_life, 0, 10);
	DDX_Text(pDX, IDC_EDIT2, m_life);
}

BEGIN_MESSAGE_MAP(CMOUSE_controlledDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CMOUSE_controlledDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMOUSE_controlledDlg::OnBnClickedCancel)
	ON_STN_CLICKED(IDC_RENDER, &CMOUSE_controlledDlg::OnStnClickedRender)
	ON_WM_MOUSEWHEEL()
    ON_WM_MOUSEMOVE()
	ON_COMMAND(IDM_CLOSE, &CMOUSE_controlledDlg::OnClose)

//	ON_BN_CLICKED(IDC_XUEHUA, &CMOUSE_controlledDlg::OnBnClickedXuehua)
	ON_BN_CLICKED(IDC_SNOW, &CMOUSE_controlledDlg::OnBnClickedSnow)
	ON_BN_CLICKED(IDC_FIRE, &CMOUSE_controlledDlg::OnBnClickedFire)
//	ON_EN_CHANGE(IDC_NUM, &CMOUSE_controlledDlg::OnEnChangeNum)
	ON_BN_CLICKED(IDC_BUTTON1, &CMOUSE_controlledDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_HUOYAN, &CMOUSE_controlledDlg::OnBnClickedHuoyan)
	ON_BN_CLICKED(IDC_STAR, &CMOUSE_controlledDlg::OnBnClickedStar)
	ON_BN_CLICKED(IDC_BUTTON2, &CMOUSE_controlledDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMOUSE_controlledDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

int choose = 0;
// CMOUSE_controlledDlg ��Ϣ�������
//��ʼ���Ի���
BOOL CMOUSE_controlledDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	PI = 3.1415926535897;
    tAngInc = PI / 90;                      // ÿ�δ�������¼��任�ĽǶ�ֵ
    tFovy = 45.0;
    prePt.SetPoint(-1, -1);                 // ��ʼ��prePt & nowPt(��OnMouseMove��ʹ��)
    nowPt.SetPoint(-1, -1);
    tVerticalAng = 0.;
    tHorizonAng = PI / 2;
    tRadius = 300.0;
    tEyeX = tRadius * cos(tVerticalAng) * cos(tHorizonAng);
    tEyeY = tRadius * sin(tVerticalAng);
    tEyeZ = tRadius * cos(tVerticalAng) * sin(tHorizonAng);
    tCenterX = 0.;
    tCenterY = 0.;
    tCenterZ = 0.;
    tUpX = 0.;
    tUpY = 1.0;
    tUpZ = 0.;
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	CWnd* wnd = GetDlgItem(IDC_RENDER);                                        // ��ʼ��MFC�е�OpenGL����
    hrenderDC = ::GetDC(wnd->m_hWnd);
    if(SetWindowPixelFormat(hrenderDC) == FALSE) {                // ����hDC�����ظ�ʽ
        return 0;
    }
    if(CreateViewGLContext(hrenderDC) == FALSE) {                    // ��hDCת���õ�hRC
        return 0;
    }
    // openGL�ĳ�ʼ������
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);


    //glViewport(0, 0, 200, 200);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45, 1, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ���ü�ʱ��,10msˢ��һ��
    SetTimer(1, 10, 0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


//SetWindowPixelFormat()���������ظ�ʽ�ĺ�������
BOOL CMOUSE_controlledDlg::SetWindowPixelFormat(HDC hDC) { // CCOpenGLDlg��Ϊ���Լ�������MFC����
    PIXELFORMATDESCRIPTOR pixelDesc;
    pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixelDesc.nVersion = 1;
    pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
    pixelDesc.iPixelType = PFD_TYPE_RGBA;
    pixelDesc.cColorBits = 32;
    pixelDesc.cRedBits = 0;
    pixelDesc.cRedShift = 0;
    pixelDesc.cGreenBits = 0;
    pixelDesc.cGreenShift = 0;
    pixelDesc.cBlueBits = 0;
    pixelDesc.cBlueShift = 0;
    pixelDesc.cAlphaBits = 0;
    pixelDesc.cAlphaShift = 0;
    pixelDesc.cAccumBits = 0;
    pixelDesc.cAccumRedBits = 0;
    pixelDesc.cAccumGreenBits = 0;
    pixelDesc.cAccumBlueBits = 0;
    pixelDesc.cAccumAlphaBits = 0;
    pixelDesc.cDepthBits = 0;
    pixelDesc.cStencilBits = 1;
    pixelDesc.cAuxBuffers = 0;
    pixelDesc.iLayerType = PFD_MAIN_PLANE;
    pixelDesc.bReserved = 0;
    pixelDesc.dwLayerMask = 0;
    pixelDesc.dwVisibleMask = 0;
    pixelDesc.dwDamageMask = 0;

    PixelFormat = ChoosePixelFormat(hDC, &pixelDesc);
    if(PixelFormat == 0) {
        PixelFormat = 1;
        if(DescribePixelFormat(hDC, PixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0) {
            return FALSE;
        }
    }
    if(SetPixelFormat(hDC, PixelFormat, &pixelDesc) == FALSE) {
        return FALSE;
    }
    return TRUE;

}

//CreateViewGLContext()����HDCת���õ�HRC�ĺ�������

BOOL CMOUSE_controlledDlg::CreateViewGLContext(HDC hDC) {
    hrenderRC = wglCreateContext(hDC);
    if(hrenderRC == NULL) {
        return FALSE;
    }
    if(wglMakeCurrent(hDC, hrenderRC) == FALSE) {
        return FALSE;
    }
    return TRUE;
}


//��껬�֡����Ŵ���С
afx_msg BOOL CMOUSE_controlledDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
    int tWheelCount = zDelta / 120;
    if(tWheelCount > 0) {
        tFovy += 1.0;
    } else if(tWheelCount < 0) {
        tFovy -= 1.0;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                                            // �˴����䲻����glLoadIdentity()
    gluPerspective(tFovy, 1, 0.1, 1000.0);                        // ע��zNear,zFar��ȡֵ
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return TRUE;
}


// �ڽ�����ʾ��Ļ��ͨ���������任�����ӽ�
afx_msg void CMOUSE_controlledDlg::OnMouseMove(UINT nFlags, CPoint point) {
    if(nFlags & MK_LBUTTON == TRUE) {

        //MessageBox("mouse move function triggered!", "attentino", MB_OK);

        nowPt.x = point.x;
        nowPt.y = point.y;
        if(prePt.x!=-1 && prePt.y!=-1 && nowPt.x!=-1 && nowPt.y!=-1) {
            double tDx = nowPt.x - prePt.x;
            double tDy = nowPt.y - prePt.y;
            double tDis = sqrt(tDx*tDx+tDy*tDy);
            if(tDx > 0.) {
                tHorizonAng += tAngInc * tDx / tDis;
                if(tHorizonAng < 0.) { tHorizonAng += 2*PI; }
                if(tHorizonAng > 2*PI) { tHorizonAng -= 2*PI; }
            } else if(tDx < 0.) {
                tHorizonAng += tAngInc * tDx / tDis;
                if(tHorizonAng < 0.) { tHorizonAng += 2*PI; }
                if(tHorizonAng > 2*PI) { tHorizonAng -= 2*PI; }
            }
            if(tDy > 0.) {
                tVerticalAng = tVerticalAng + tAngInc * tDy / tDis;
                if(tVerticalAng > PI/2) { tVerticalAng = PI/2; }
            } else if(tDy < 0.) {
                tVerticalAng = tVerticalAng + tAngInc * tDy / tDis;
                if(tVerticalAng < -PI/2) { tVerticalAng = -PI/2; }
            }

            tEyeX = tRadius * cos(tVerticalAng) * cos(tHorizonAng);
            tEyeY = tRadius * sin(tVerticalAng);
            tEyeZ = tRadius * cos(tVerticalAng) * sin(tHorizonAng);
        }
        prePt.x = nowPt.x;
        prePt.y = nowPt.y;
    }
}

//ʵ��OpenGL��ͼ��RenderScene������
//RenderScene() �����Ĺ���
void CMOUSE_controlledDlg::RenderScene() {
	glMatrixMode(GL_PROJECTION);                            // ����modelview �� projection
	glLoadIdentity();                                       // �˴����䲻����glLoadIdentity()
	gluPerspective(tFovy, 1, 0.1, 1000.0);                  // ע��zNear,zFar��ȡֵ
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(tEyeX, tEyeY, tEyeZ, tCenterX, tCenterY, tCenterZ, tUpX, tUpY, tUpZ);

	glClear(GL_COLOR_BUFFER_BIT);
	SwapBuffers(hrenderDC);                                    // ʹ��glFlush()û����ʾ��
}

//RenderScene1() ����ѩ��
void CMOUSE_controlledDlg::RenderScene1() {

	glMatrixMode(GL_PROJECTION);                            // ����modelview �� projection
    glLoadIdentity();                                       // �˴����䲻����glLoadIdentity()
    gluPerspective(tFovy, 1, 0.1, 1000.0);                  // ע��zNear,zFar��ȡֵ
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(tEyeX, tEyeY, tEyeZ, tCenterX, tCenterY, tCenterZ, tUpX, tUpY, tUpZ);

    glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	for (int i = 0; i<number; i++)
	{
		if (particles[i].life>0)
		{
			glPushMatrix();
			glTranslatef(particles[i].x, particles[i].y, particles[i].z);
			glScalef(particles[i].big, particles[i].big, particles[i].big);// 1.0);
			//glRotated(particles[i].ang, 0.0f,  0.0,1.0f);
			glCallList(listname);
			glPopMatrix();
			particles[i].life -= float(rand() % 100) / 1000.0f + 0.0003f;
			particles[i].yi += particles[i].yg;
			particles[i].xi += particles[i].xg;
			particles[i].y += particles[i].yi;
			particles[i].x += particles[i].xi;
		
		}
		if (particles[i].life <= 0)
		{
			particles[i].life = rand() % 200;
			particles[i].big = rand() % 3 + 1;
			particles[i].x = rand() % 250 - 90.0;
			particles[i].y = rand() % 30 + 150;
			particles[i].z = rand() % 300-150 ;
			particles[i].yi = float(rand() % 10 - 4) / 40;
			particles[i].xi = float(rand() % 10 - 4) / 80;
			particles[i].yg = -4.9 / 40000;
			particles[i].xg = 0;// -1.0 / 10000;
			
		}
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glFlush();
	SwapBuffers(hrenderDC);                                    // ʹ��glFlush()û����ʾ��
															   // ʹ��glFlush()û����ʾ��
 
}

//���ƻ�
void CMOUSE_controlledDlg::RenderScene_fire() {
	glMatrixMode(GL_PROJECTION);                            // ����modelview �� projection
	glLoadIdentity();                                       // �˴����䲻����glLoadIdentity()
	gluPerspective(tFovy, 1, 0.1, 1000.0);                  // ע��zNear,zFar��ȡֵ
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(tEyeX, tEyeY-200, tEyeZ-150, tCenterX, tCenterY, tCenterZ, tUpX, tUpY, tUpZ);

	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	int count[PARTICLE_NUMBER] = { 0 };
	for (int i = 0; i < number; i++)
	{
		if(particles_fire[i].life>0)
		{ 
			if (particles_fire[i].flag)//particles_fire[i].life > 0 &&
			{
				glPushMatrix();
				glColor4f(particles_fire[i].r, particles_fire[i].g, particles_fire[i].b, particles_fire[i].life);
				glTranslatef(particles_fire[i].x, particles_fire[i].y, particles_fire[i].z);
				glScaled(particles_fire[i].big / 10.0, particles_fire[i].big / 10.0, particles_fire[i].big / 10.0);
				glCallList(listname);
				glPopMatrix();
				particles_fire[i].x += particles_fire[i].xi / (slowdown * 1000); // ����X�����λ��    //���Ե������ӵ��ٶ�
				particles_fire[i].y -= particles_fire[i].yi / (slowdown * 1000); // ����Y�����λ��
				particles_fire[i].z += particles_fire[i].zi / (slowdown * 1000); // ����Z�����λ��
				particles_fire[i].xi += particles_fire[i].xg;   // ����X�᷽���ٶȴ�С
				particles_fire[i].yi += particles_fire[i].yg;   // ����Y�᷽���ٶȴ�С
				particles_fire[i].zi += particles_fire[i].zg;   // ����Z�᷽���ٶȴ�С
				particles_fire[i].life -= particles_fire[i].fade / 5;  // �������ӵ�����ֵ

				if (particles_fire[i].life <= 0.35)
				{
					particles_fire[i].flag = false;
					count[i] = 1;

				}

				particles_fire[i].life1 = 1.0f;
			}
			if (!particles_fire[i].flag)
			{
				if (count[i] == 1)
				{
					particles_fire[i].xi = float((rand() % 500) - 26.0f)*sin(i * 15.0 / (2 * 3.14159));  // �������X�᷽���ٶ�
					particles_fire[i].yi = float((rand() % 500) - 26.0f)*sin(i * 15.0 / (2 * 3.14159)); // �������Y�᷽���ٶ�
					particles_fire[i].zi = float((rand() % 500) - 25.0f)*sin(i*15.0 / (2 * 3.14159));  // �������Z�᷽���ٶ�
																									   //particles_fire[i].xi = 500*sin(i*15.0 / (2 * 3.14159));  // �������X�᷽���ٶ�
																									   //particles_fire[i].yi = -7000;//float((rand() % 50) - 25.0f);  // �������Y�᷽���ٶ�
																									   //particles_fire[i].zi = 500*sin(i*15.0 / (2 * 3.14159));  // �������Z�᷽���ٶ�
					particles_fire[i].xg = float(rand() % 200) / 100.0f - 1.0f;      // ����X�᷽����ٶ�Ϊ0
					particles_fire[i].yg = -float(rand() % 3);      //  ����Y�᷽����ٶ�Ϊ-0.8
					particles_fire[i].zg = float(rand() % 200) / 100.0f - 1.0f;       //  ����Z�᷽����ٶ�Ϊ0
					particles_fire[i].life -= particles_fire[i].fade / 5;
				}
				count[i]++;
				glPushMatrix();
				glColor4f(particles_fire[i].r, particles_fire[i].g, particles_fire[i].b, particles_fire[i].life);
				glTranslatef(particles_fire[i].x, particles_fire[i].y, particles_fire[i].z);
				//	glScalef(particles_fire[i].r/5.0, particles_fire[i].r/5.0, 1.0);
				glCallList(listname);
				glPopMatrix();


				//	particles_fire[i].life1 -= particles_fire[i].fade / 5;    // ����һ���µ�����
				//	particles_fire[i].fade = float(rand() % 100) / 10000.0f + 0.003f; // �������˥������
				particles_fire[i].x += particles_fire[i].xi / (slowdown * 1000); // ����X�����λ��    //���Ե������ӵ��ٶ�
				particles_fire[i].y += particles_fire[i].yi / (slowdown * 1000); // ����Y�����λ��
				particles_fire[i].z += particles_fire[i].zi / (slowdown * 1000); // ����Z�����λ��
				particles_fire[i].xi += particles_fire[i].xg;   // ����X�᷽���ٶȴ�С
				particles_fire[i].yi += particles_fire[i].yg;   // ����Y�᷽���ٶȴ�С
				particles_fire[i].zi += particles_fire[i].zg;   // ����Z�᷽���ٶȴ�С
				particles_fire[i].r = colors[(int)(number*(i*1.0 / number))][0];   // ����������ɫ
				particles_fire[i].g = colors[(int)(number*(i*1.0 / number))][1];
				particles_fire[i].b = colors[(int)(number*(i*1.0 / number))][2];//֮ǰ��col
				particles_fire[i].life -= particles_fire[i].fade / 5;
			}

		}
		if (particles_fire[i].life <= 0)
		{
			for (int loop = 0; loop<PARTICLE_NUMBER; loop++)
			{
				particles_fire[loop].active = true;     // ʹ���е�����Ϊ����״̬
				particles_fire[loop].big = rand() % 10 + 1;
				particles_fire[loop].life = 1.0f;     // ���е���������ֵΪ���
				particles_fire[loop].fade = 0.01f; //float(rand() % 100) / 1000.0f + 0.003f;  // �������˥������
				particles_fire[loop].r = colors[loop*(12 / MAX_PARTICLES)][0];  // ���ӵĺ�ɫ��ɫ
				particles_fire[loop].g = colors[loop*(12 / MAX_PARTICLES)][1];  // ���ӵ���ɫ��ɫ
				particles_fire[loop].b = colors[loop*(12 / MAX_PARTICLES)][2];  // ���ӵ���ɫ��ɫ
				particles_fire[loop].xi = 0; //float((rand() % 50) - 26.0f);  // �������X�᷽���ٶ�
				particles_fire[loop].yi = -1200.0f;//float((rand() % 50) - 25.0f);  // �������Y�᷽���ٶ�
				particles_fire[loop].zi = 0;// float((rand() % 50) - 25.0f);  // �������Z�᷽���ٶ�
				particles_fire[loop].xg = 0.0f;      // ����X�᷽����ٶ�Ϊ0
				particles_fire[loop].yg = -1.5f;      //  ����Y�᷽����ٶ�Ϊ-0.8
				particles_fire[loop].zg = 0.0f;      //  ����Z�᷽����ٶ�Ϊ0
				particles_fire[loop].x = 0;
				particles_fire[loop].y = -50.0f;
				particles_fire[loop].z = 0;
				particles_fire[loop].flag = true;
			}
		}
			//particles_fire[i].flag = true;


	}//for

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		//glFlush();
		SwapBuffers(hrenderDC);                                    // ʹ��glFlush()û����ʾ��
	
}

void CMOUSE_controlledDlg::RenderScene_star()
{
	glMatrixMode(GL_PROJECTION);                            // ����modelview �� projection
	glLoadIdentity();                                       // �˴����䲻����glLoadIdentity()
	gluPerspective(tFovy, 1, 0.1, 1000.0);                  // ע��zNear,zFar��ȡֵ
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(tEyeX/5, tEyeY/5, tEyeZ/5, tCenterX, tCenterY, tCenterZ, tUpX, tUpY, tUpZ);

	glClear(GL_COLOR_BUFFER_BIT);
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texture[4]);

	for (int i = 0; i<number; i++)
	{
		if (star[i].life>0)
		{
			glPushMatrix();
			glColor4f(star[i].r, star[i].g, star[i].b, star[i].life);
			glTranslatef(star[i].x, star[i].y, star[i].z);
			glScalef(star[i].big/10.0, star[i].big / 10.0, star[i].big / 10.0);
			glCallList(listname);
			glPopMatrix();
			star[i].x += star[i].xi / (slowdown * 1000); // ����X�����λ��    //���Ե������ӵ��ٶ�
			star[i].y -= star[i].yi / (slowdown * 1000); // ����Y�����λ��
			star[i].z += star[i].zi / (slowdown * 1000); // ����Z�����λ��
			star[i].xi += star[i].xg;   // ����X�᷽���ٶȴ�С
			star[i].yi += star[i].yg;   // ����Y�᷽���ٶȴ�С
			star[i].zi += star[i].zg;   // ����Z�᷽���ٶȴ�С
			star[i].life -= star[i].fade;  // �������ӵ�����ֵ
		}
		if (star[i].life <= 0)
		{
			star[i].life = 1.0f;    // ����һ���µ�����
			star[i].fade = float(rand() % 100) / 1000.0f + 0.003f; // �������˥������
			star[i].x = float((rand() % 50) - 26.0f);     // �����ӳ�������Ļ������star[loop].xi = float((rand() % 50) - 26.0f);
			star[i].y = float((rand() % 50) - 26.0f);
			star[i].z = float((rand() % 50) - 26.0f);
			star[i].xi = xspeed + float((rand() % 60) - 32.0f); // ������������ٶ�
			star[i].yi = yspeed + float((rand() % 60) - 30.0f);
			star[i].zi = float((rand() % 60) - 30.0f);
			if (color_new == 13)
			{star[i].r = colors[i%12][0];   // ����������ɫ
			star[i].g = colors[i % 12][1];
			star[i].b = colors[i%12][2];//֮ǰ��col

			}
			else
			{
				star[i].r = colors[color_new][0];   // ����������ɫ
				star[i].g = colors[color_new][1];
				star[i].b = colors[color_new][2];//֮ǰ��col

			}
			
		}
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glFlush();
	{
		if (angle < 360)
		{
			glRotatef(angle, 0.0f, 30.0f, 0.0f);
			angle += 3;
		}
		else
		{
			angle = 0;
		}
		glTranslatef(50.0f, 0.0f, 0.0f);
		glutSolidSphere(100, 40, 32);
	}
	SwapBuffers(hrenderDC);
}

//���ƻ���
void CMOUSE_controlledDlg::RenderScene_huoyan()
{
	glMatrixMode(GL_PROJECTION);                            // ����modelview �� projection
	glLoadIdentity();                                       // �˴����䲻����glLoadIdentity()
	gluPerspective(tFovy, 1, 0.1, 1000.0);                  // ע��zNear,zFar��ȡֵ
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(tEyeX/6, tEyeY/6, tEyeZ/6, tCenterX, tCenterY, tCenterZ, tUpX, tUpY, tUpZ);

	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	/*		glBegin(GL_QUADS);

	*/


	for (int i = 0; i<number; i++)
	{
		if (fire[i].life>0)
		{
			glPushMatrix();
			glColor4f(fire[i].r, fire[i].g, fire[i].b, fire[i].life);
			glTranslatef(fire[i].x, fire[i].y, fire[i].z);
			glScalef(fire[i].big/15.0, fire[i].big / 15.0, fire[i].big / 15.0);
			glCallList(listname);
			glPopMatrix();
			fire[i].x += fire[i].xi / (slowdown * 1000); // ����X�����λ��    //���Ե������ӵ��ٶ�
			fire[i].y -= fire[i].yi / (slowdown * 1000); // ����Y�����λ��
			fire[i].z += fire[i].zi / (slowdown * 1000); // ����Z�����λ��
			fire[i].xi += fire[i].xg;   // ����X�᷽���ٶȴ�С
			fire[i].yi += fire[i].yg;   // ����Y�᷽���ٶȴ�С
			fire[i].zi += fire[i].zg;   // ����Z�᷽���ٶȴ�С
			
			fire[i].life -= fire[i].fade*1.5*life_new;  // �������ӵ�����ֵ
		}
		if (fire[i].life <= 0)
		{
			fire[i].life =life_new;    // ����һ���µ�����
			fire[i].fade = float(rand() % 100) /10000.0f  + 0.003f; //  �������˥������
			fire[i].x = 0.0f;     // �����ӳ�������Ļ������
			fire[i].y = -5.0f;
			fire[i].z = 0.0f;
			fire[i].xi = xspeed + float((rand() % 50) - 32.0f); // ������������ٶ�
			fire[i].yi = yspeed + float((rand() % 100) - 30.0f);
			fire[i].zi = zspeed + float((rand() % 50) - 30.0f);
			fire[i].r = colors[i % 3][0];   // ����������ɫ
			fire[i].g = colors[i % 3][1];
			fire[i].b = colors[i % 3][2];//֮ǰ��col
		}
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glFlush();
	SwapBuffers(hrenderDC);                                                    // ʹ��glFlush()û����ʾ��
}

void CMOUSE_controlledDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMOUSE_controlledDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMOUSE_controlledDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMOUSE_controlledDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void CMOUSE_controlledDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CMOUSE_controlledDlg::OnStnClickedRender()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}






void CMOUSE_controlledDlg::OnClose()
{
	// TODO: �ڴ���������������
	CDialogEx::OnCancel();
}




void CMOUSE_controlledDlg::init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 0.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	initparticles();
	BuildTexture("snowflower1.jpg", texture[1]);

	glNewList(listname, GL_COMPILE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	{
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-1, -1, 0.0);
		glTexCoord2f(1, 0); glVertex3f(1, -1, 0.0);
		glTexCoord2f(1, 1); glVertex3f(1, 1, 0.0);
		glTexCoord2f(0, 1); glVertex3f(-1, 1, 0.0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-1, -1, 0.1);
		glTexCoord2f(1, 0); glVertex3f(1, -1, 0.1);
		glTexCoord2f(1, 1); glVertex3f(1, 1, 0.1);
		glTexCoord2f(0, 1); glVertex3f(-1, 1, 0.1);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-1, -1, 0.2);
		glTexCoord2f(1, 0); glVertex3f(1, -1, 0.2);
		glTexCoord2f(1, 1); glVertex3f(1, 1, 0.20);
		glTexCoord2f(0, 1); glVertex3f(-1, 1, 0.2);
		glEnd();
	}
//	glRotated((rand() % 10 - 5), 0, 1.0, 1.0);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEndList();
}
void CMOUSE_controlledDlg::initparticles()
{
	for (int i = 0; i<PARTICLE_NUMBER; i++)
	{
		particles[i].life = rand() % 200;
		particles[i].big = rand() % 3 + 1;
		particles[i].x = rand() % 250 - 90.0;
		particles[i].y = rand() % 30 + 150;
		particles[i].z = rand() % 300 - 150;
		particles[i].yi = float(rand() % 10 - 4) / 40;
		particles[i].xi = float(rand() % 10 - 4) / 80;
		particles[i].yg = -4.9 / 40000;
		particles[i].xg = 0;// -1.0 / 10000;

	}
}





void CMOUSE_controlledDlg::init_fire(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 0.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	initparticles_fire();
	BuildTexture("fire.jpg", texture[2]);
	glNewList(listname, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	/*glTexCoord2f(0, 0); glVertex3f(-1, -1, 0.0);
	glTexCoord2f(1, 0); glVertex3f(1, -1, 0.0);
	glTexCoord2f(1, 1); glVertex3f(1, 1, 0.0);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 0.0);*/
	// Front Face 
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

	// Back Face 
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

	// Bottom Face 
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face 
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	// Left Face 
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	//glutSolidSphere(1, 40, 32);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEndList();
}

void CMOUSE_controlledDlg::initparticles_fire()
{
	for (int loop = 0; loop<PARTICLE_NUMBER; loop++)
	{
		particles_fire[loop].active = true;     // ʹ���е�����Ϊ����״̬
		particles_fire[loop].big = rand() % 10 + 1;
		particles_fire[loop].life = 1.0f;     // ���е���������ֵΪ���
		particles_fire[loop].fade = 0.01f; //float(rand() % 100) / 1000.0f + 0.003f;  // �������˥������
		particles_fire[loop].r = colors[loop*(12 / MAX_PARTICLES)][0];  // ���ӵĺ�ɫ��ɫ
		particles_fire[loop].g = colors[loop*(12 / MAX_PARTICLES)][1];  // ���ӵ���ɫ��ɫ
		particles_fire[loop].b = colors[loop*(12 / MAX_PARTICLES)][2];  // ���ӵ���ɫ��ɫ
		particles_fire[loop].xi = 0; //float((rand() % 50) - 26.0f);  // �������X�᷽���ٶ�
		particles_fire[loop].yi = -1200.0f;//float((rand() % 50) - 25.0f);  // �������Y�᷽���ٶ�
		particles_fire[loop].zi = 0;// float((rand() % 50) - 25.0f);  // �������Z�᷽���ٶ�
		particles_fire[loop].xg = 0.0f;      // ����X�᷽����ٶ�Ϊ0
		particles_fire[loop].yg = -1.5f;      //  ����Y�᷽����ٶ�Ϊ-0.8
		particles_fire[loop].zg = 0.0f;      //  ����Z�᷽����ٶ�Ϊ0
		particles_fire[loop].x = 0;
		particles_fire[loop].y = -50.0f;
		particles_fire[loop].z = 0;
		particles_fire[loop].flag = true;
	}
}

void CMOUSE_controlledDlg::init_huoyan(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 0.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	initparticles_huoyan();
	BuildTexture("fire1.jpg", texture[3]);
	glNewList(listname, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	
	// Front Face 
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

	// Back Face 
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

	// Bottom Face 
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face 
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	// Left Face 
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	//glutSolidSphere(1, 40, 32);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEndList();
}

void CMOUSE_controlledDlg::initparticles_huoyan()
{
	for (int loop = 0; loop<PARTICLE_NUMBER; loop++)
	{
		fire[loop].active = true;     // ʹ���е�����Ϊ����״̬
		fire[loop].life = 1.0f;     // ���е���������ֵΪ���
		fire[loop].big = rand() % 10 + 1;
		fire[loop].fade = float(rand() % 100) / 1000.0f + 0.003f;  // �������˥������
		fire[loop].r = colors[loop*(12 / MAX_PARTICLES)][0];  // ���ӵĺ�ɫ��ɫ
		fire[loop].g = colors[loop*(12 / MAX_PARTICLES)][1];  // ���ӵ���ɫ��ɫ
		fire[loop].b = colors[loop*(12 / MAX_PARTICLES)][2];  // ���ӵ���ɫ��ɫ
		fire[loop].xi = float((rand() % 50) - 26.0f);  // �������X�᷽���ٶ�
		fire[loop].yi = float((rand() % 50) - 25.0f);  // �������Y�᷽���ٶ�
		fire[loop].zi = float((rand() % 50) - 25.0f);  // �������Z�᷽���ٶ�
		fire[loop].xg = 0.0f;      // ����X�᷽����ٶ�Ϊ0
		fire[loop].yg = -1.5f;      //  ����Y�᷽����ٶ�Ϊ-0.8
		fire[loop].zg = 0.0f;      //  ����Z�᷽����ٶ�Ϊ0
		fire[loop].x = 0.0f;     // �����ӳ�������Ļ������
		fire[loop].y = -5.0f;
		fire[loop].z = 0.0f;
	}
}

void CMOUSE_controlledDlg::init_star()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 0.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	initparticles_star();
	//BuildTexture("galaxy.jpg",texture[0]);
	BuildTexture("star.jpg", texture[4]);//�򿪱���
	glNewList(listname, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBindTexture(GL_TEXTURE_2D,texture[1]);//ѩ������
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//��������
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-1, -1, 0.0);
	glTexCoord2f(1, 0); glVertex3f(1, -1, 0.0);
	glTexCoord2f(1, 1); glVertex3f(1, 1, 0.0);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEndList();
}

void CMOUSE_controlledDlg::initparticles_star()
{
	for (int loop = 0; loop<PARTICLE_NUMBER; loop++)
	{
		star[loop].active = true;     // ʹ���е�����Ϊ����״̬
		star[loop].life = 1.0f;     // ���е���������ֵΪ���
		star[loop].big = rand() % 10 + 1;
		star[loop].fade = float(rand() % 100) / 1000.0f + 0.003f;  // �������˥������
		star[loop].r = colors[loop*(12 / MAX_PARTICLES)][0];  // ���ӵĺ�ɫ��ɫ
		star[loop].g = colors[loop*(12 / MAX_PARTICLES)][1];  // ���ӵ���ɫ��ɫ
		star[loop].b = colors[loop*(12 / MAX_PARTICLES)][2];  // ���ӵ���ɫ��ɫ
															  //star[loop].xi = float((rand() % 50) - 26.0f);  // �������X�᷽���ٶ�
															  //star[loop].yi = float((rand() % 50) - 25.0f);  // �������Y�᷽���ٶ�
															  //star[loop].zi = float((rand() % 50) - 25.0f);  // �������Z�᷽���ٶ�
		star[loop].xg = 0.0f;      // ����X�᷽����ٶ�Ϊ0
								   //star[loop].yg = -1.0f;      //  ����Y�᷽����ٶ�Ϊ-0.8
		star[loop].zg = 0.0f;      //  ����Z�᷽����ٶ�Ϊ0
	}
}

void CMOUSE_controlledDlg::OnBnClickedSnow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CMOUSE_controlledDlg::OnInitDialog();
	//CDialogEx::OnInitDialog();
	if (BST_CHECKED == IsDlgButtonChecked(IDC_SNOW))
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_SMOOTH);

		init();

		glMatrixMode(GL_PROJECTION);
		gluPerspective(45, 1, 0.1, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	UpdateData(false);

}

void CMOUSE_controlledDlg::OnBnClickedFire()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CMOUSE_controlledDlg::OnInitDialog();
	if (BST_CHECKED == IsDlgButtonChecked(IDC_FIRE))
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_SMOOTH);
		
		init_fire();
		
		glMatrixMode(GL_PROJECTION);
		gluPerspective(45, 1, 0.1, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	UpdateData(false);
}

void CMOUSE_controlledDlg::OnBnClickedHuoyan()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CMOUSE_controlledDlg::OnInitDialog();
	if (BST_CHECKED == IsDlgButtonChecked(IDC_HUOYAN))
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_SMOOTH);

		init_huoyan();

		glMatrixMode(GL_PROJECTION);
		gluPerspective(45, 1, 0.1, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	UpdateData(false);
}


void CMOUSE_controlledDlg::OnBnClickedStar()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CMOUSE_controlledDlg::OnInitDialog();
	if (BST_CHECKED == IsDlgButtonChecked(IDC_STAR))
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_SMOOTH);

		init_star();

		glMatrixMode(GL_PROJECTION);
		gluPerspective(45, 1, 0.1, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	UpdateData(false);
}

void CMOUSE_controlledDlg::OnTimer(UINT nIDEvent)
{
	//UpdateData(true);
	if (BST_CHECKED == IsDlgButtonChecked(IDC_SNOW))
	{
		// ��ѡ
		RenderScene1();
	}
	if (BST_CHECKED == IsDlgButtonChecked(IDC_FIRE))
	{
		// ��ѡ
		RenderScene_fire();
	}
	if (BST_CHECKED == IsDlgButtonChecked(IDC_HUOYAN))
	{
		// ��ѡ
		RenderScene_huoyan();
	}
	if (BST_CHECKED == IsDlgButtonChecked(IDC_STAR))
	{
		// ��ѡ
		RenderScene_star();
	}
	if (BST_CHECKED != IsDlgButtonChecked(IDC_SNOW)&& 
		BST_CHECKED != IsDlgButtonChecked(IDC_FIRE) &&
		BST_CHECKED != IsDlgButtonChecked(IDC_HUOYAN) &&
		BST_CHECKED != IsDlgButtonChecked(IDC_STAR))
	{
		initparticles();
		initparticles_fire();
		initparticles_huoyan();
		initparticles_star();
		RenderScene();
	}
	

	CDialog::OnTimer(nIDEvent);
}

/*
void CMOUSE_controlledDlg::OnEnChangeNum()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}*/


void CMOUSE_controlledDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	number = m_num;
}






void CMOUSE_controlledDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	color_new = m_color;
}


void CMOUSE_controlledDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	life_new = m_life;
}
