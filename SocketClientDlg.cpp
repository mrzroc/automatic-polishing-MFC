// SocketClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SocketClient.h"
#include "SocketClientDlg.h"
#include "string"
#include <stdio.h>
#include <math.h>
#include <dos.h> 

#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>

//#include <graphics.h>		// �������� EasyX ͼ�ο�
//#include <conio.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]= __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

#define	PI 3.14159
#define MAXNUM 60000
//ȫ�ֱ��������ź�
double Force_result = 0,Force_x,Force_y,Force_z;
double Pos[3];
extern double Force_result;//�׹���
double DesiredForce;//�����׹���
double DeltaPosition = 0;
double DeltaPosition_2 = 0;//��
double myDelta = 0;
int couter[4];//�����㷨��
double K_env=60;
double gKi=1/100;
double n_Delta_Postion=0;
int PointNumMax;
PIParameter Force_Parameter;

int alldtime=0;
int itime=0;//�������i
int dtime=0;//�������
int ipoint=0; //��ipoint���׹�㣻

char name[256] = {0};

double dx,dy,dz,da,db,dc=0;


int Forceproc_Flag=1;//�����㷨����
double EndToolCoordRelRobot[3];
double nvctRelRobot[3];
//int P[MAXNUM][6]={0};
struct PolishingPoint ppoint[MAXNUM]={0};

//PIParameter Force_Parameter;
double gEndToolCoordRelRobot[6]={0.0};//�յ㹤������ϵ���{B}λ��

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSocketClientDlg dialog

CSocketClientDlg::CSocketClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSocketClientDlg)
	SaveData_Name = _T("");
	Progress_View = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSocketClientDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_LISTBOX_POLISHINGDATE, m_PolishingDate);
	DDX_Text(pDX, IDC_SAVEDATA_NAME, SaveData_Name);
	DDX_Text(pDX, IDC_Progress_View, Progress_View);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSocketClientDlg, CDialog)
	//{{AFX_MSG_MAP(CSocketClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnExit)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnClean)
	ON_BN_CLICKED(IDC_BUTTON10, OnSwitchPolish_Pause)
	ON_BN_CLICKED(IDC_BUTTON11, OnSwitchPolish_Continue)
	ON_BN_CLICKED(IDC_BUTTON9, OnSwitchPolish_Start)
	ON_BN_CLICKED(IDC_BUTTON12, OnSwitchPolish_Hoist)
	ON_BN_CLICKED(IDC_BUTTON1, OnSwitchPolish_Reset)
	ON_BN_CLICKED(IDC_BTN_CONNECT2, OnSwitchPolish_Stop)
	ON_BN_CLICKED(IDC_BTN_EXIT2, OnSwitchPolish_Teach)
	ON_BN_CLICKED(IDC_BTN_EXIT8, OnTeach_X_Sub)
	ON_BN_CLICKED(IDC_BTN_EXIT7, OnTeach_Y_Add)
	ON_BN_CLICKED(IDC_BTN_EXIT5, OnTeach_Z_Add)
	ON_BN_CLICKED(IDC_BTN_EXIT9, OnTeach_X_Add)
	ON_BN_CLICKED(IDC_BTN_EXIT6, OnTeach_Y_Sub)
	ON_BN_CLICKED(IDC_BTN_EXIT4, OnTeach_Z_Sub)
	ON_BN_CLICKED(IDC_BUTTON13, OnSwitchPolish_Fast)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_SAVE_DATA, OnSaveData)
	ON_BN_CLICKED(IDC_Trans_POS, OnTransPOS)
	ON_BN_CLICKED(IDC_Trans_POS2, OnTrans_ANGLE)
	ON_BN_CLICKED(IDC_BTN_Graph, OnBTNGraph)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BTN, OnCeshi)
	ON_BN_CLICKED(IDC_Debugging, OnDebugging)
	ON_BN_CLICKED(IDC_SwitchPolish_Correct, OnSwitchPolishCorrect)
	ON_BN_CLICKED(IDC_SwitchPolish_Text, OnSwitchPolishText)
	ON_BN_CLICKED(IDC_BTN_EXIT3, OnBtnExit3)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BTN_EXIT10, OnBtnExit10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSocketClientDlg message handlers


//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                               ������ʼ��������Ҫ����                                               ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



BOOL CSocketClientDlg::OnInitDialog()//��ʼ��������Ҫ����
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
//	GetDlgItem(IDC_ED_IP)->SetWindowText("192.168.1.104");
	GetDlgItem(IDC_ED_PORT)->SetWindowText("59152");
	//��ȡ����IP����
	char* localIP;
	CString strIP;
	char hostname[MAX_PATH] = {0};
	gethostname(hostname,MAX_PATH);
	hostent* ptent = gethostbyname(hostname);
	if(ptent)
	{
		localIP = inet_ntoa(*(in_addr*)ptent->h_addr_list[0]);
	}
	GetDlgItem(IDC_ED_IP)->SetWindowText(localIP);//Ĭ��ֵ
	GetDlgItem(IDC_ED_IP)->EnableWindow(false);//���ܱ༭


	Force_Parameter.force_N = 0;
	Force_Parameter.force_N_1 = 0;
	Force_Parameter.e_N = 0;	
	Force_Parameter.de_N = 0;

	Force_Parameter.u_N = 0.001;

	Force_Parameter.k_n = 50;
	Force_Parameter.k_n_1 = 50;
	Force_Parameter.P_n_1 = 1/0.01;
	Force_Parameter.P_n = 1/0.01;
	Force_Parameter.R_n = 0.001;
	Force_Parameter.count = 0;

	GetDlgItem(IDC_MoveSafe_speed_min)->SetWindowText("0.2");
	CString strMS_speed_min;
	GetDlgItem(IDC_MoveSafe_speed_min)->GetWindowText(strMS_speed_min);
	MS_speed_min = atof(strMS_speed_min);//atof �ַ���תdouble

	GetDlgItem(IDC_Move_limit_Box)->SetWindowText("5");
	CString strMS_limit_Box;
	GetDlgItem(IDC_Move_limit_Box)->GetWindowText(strMS_limit_Box);
	MS_limit_Box = atof(strMS_limit_Box);//atof �ַ���תdouble

	GetDlgItem(IDC_MoveSafe_speed_max)->SetWindowText("0.8");
	CString strMS_speed_max;
	GetDlgItem(IDC_MoveSafe_speed_max)->GetWindowText(strMS_speed_max);
	MS_speed_max = atof(strMS_speed_max);//atof �ַ���תdouble

	GetDlgItem(IDC_MoveSafe_speed_medium)->SetWindowText("0.4");
	CString strMS_speed_medium;
	GetDlgItem(IDC_MoveSafe_speed_medium)->GetWindowText(strMS_speed_medium);
	MS_speed_medium = atof(strMS_speed_medium);//atof �ַ���תdouble


    SYSTEMTIME sys; 
    GetLocalTime( &sys ); 
    // printf( "%4d/%02d/%02d %02d:%02d:%02d.%03d ����%1d\n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek); 

    SaveData_Name.Format( "%4d-%02d-%02d-%02d-%02d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute ); 
	SetDlgItemText(IDC_SAVEDATA_NAME,SaveData_Name);	
//	GetDlgItem(IDC_SAVEDATA_NAME)->SetWindowText("2020-12-21-14-30");
	
// 	GetDlgItem(IDC_POS_Trans_X)->SetWindowText("-21.885");   //  -21.885
// 	GetDlgItem(IDC_POS_Trans_Y)->SetWindowText("-1284.796");       //  -1284.796
// 	GetDlgItem(IDC_POS_Trans_Z)->SetWindowText("540.122");     //  540.122
// 	GetDlgItem(IDC_POS_Trans_A)->SetWindowText("-85.561");        //   -85.5611627 ��ʱ����ת 4.4388373

	GetDlgItem(IDC_POS_Trans_X)->SetWindowText("0");   //  -21.885
	GetDlgItem(IDC_POS_Trans_Y)->SetWindowText("0");       //  -1284.796
	GetDlgItem(IDC_POS_Trans_Z)->SetWindowText("0");     //  540.122
	GetDlgItem(IDC_POS_Trans_A)->SetWindowText("0");        //   -85.5611627 ��ʱ����ת 4.4388373
	GetDlgItem(IDC_POS_Trans_H)->SetWindowText("0");     //  540.122
	GetDlgItem(IDC_Send_View)->SetWindowText("10");  
	
	GetDlgItem(IDC_ED_LabViewPORT)->SetWindowText("52233");   //labview�˿�
	GetDlgItem(IDC_ED_TEXT)->SetWindowText("4");   //labview�˿�


	
	GetDlgItem(IDC_POS_Trans_X)->GetWindowText(strPOS_Trans_X);
	POS_Trans_X = atof(strPOS_Trans_X);//atof �ַ���תdouble
	GetDlgItem(IDC_POS_Trans_Y)->GetWindowText(strPOS_Trans_Y);
	POS_Trans_Y = atof(strPOS_Trans_Y);//atof �ַ���תdouble
	GetDlgItem(IDC_POS_Trans_Z)->GetWindowText(strPOS_Trans_Z);
	POS_Trans_Z = atof(strPOS_Trans_Z);//atof �ַ���תdouble
	GetDlgItem(IDC_POS_Trans_A)->GetWindowText(strPOS_Trans_A);
	POS_Trans_A = atof(strPOS_Trans_A);//atof �ַ���תdouble

	Connect_view.Format("δ����");
	SetDlgItemText(IDC_Connect_view,Connect_view);


	m_PolishingDate.AddString( _T("KUKA�������׹�ϵͳʹ��˵��������ʹ���Ⱥ�˳��") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("���衾1���ڵ����׹��ǰ�����ø߶ȲH��Ȼ��������任��δ�궨����£�") );
	m_PolishingDate.AddString( _T("���衾2�����ð�ȫ�����ٶȣ���С�ٶȾ����׹��Ĳ���������ٶȱ�֤��ȫ") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("���衾3�����������׹�㣬�����������ļ�������������ݺ��ٴε���") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("���衾4�����KUKA�ļ�����ť��û�е������ʾδ��������������ʾ�Ѽ���") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("���衾5��������˳ɹ���������ʱ����ʾ�����ʾ�ȴ�ָ���") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("���衾6�������ʼ��ť�����ɿ�ʼ�׹⣬��ͣ��������ť�����������켣") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("���衾7���ᵶ�򻻵��󣬼����׹��������׹ⰴť����ͷ�׹���㿪ʼ��ť") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("���衾8��ʾ������ʾ�̰�ť�����������������ٳ������Ӽ���ť") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("���衾9����������ʱ�ȴ����ļ��У��޸����ڣ��������ݺ��ٴ��ļ�") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("��ʾ��1��SwitchPolishCase 1 �����׹� 2 �ȴ��ź� 3 �����׹�") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("��ʾ��2��SwitchPolishCase 4 ��ͷ�׹� 5 �����ᵶ 6 ��������") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("��ʾ��3��SwitchPolishCase 7 ͨ��ֹͣ 8 ʾ�̲��� 9 һ������") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("��ʾ��4��SwitchPolishCase 10 Ԥ�ӹ� 11 �ǶȽ��� 12 ������") );//��ʾ����Ļ��
	m_PolishingDate.AddString( _T("���衾1��") );
	m_PolishingDate.AddString( _T("���衾2��") );
	m_PolishingDate.AddString( _T("���衾3��") );
	m_PolishingDate.AddString( _T("���衾4��") );
	m_PolishingDate.AddString( _T("���衾5��") );
	m_PolishingDate.AddString( _T("���衾6��") );
	m_PolishingDate.AddString( _T("���衾7��") );
	m_PolishingDate.AddString( _T("���衾8��") );
	m_PolishingDate.AddString( _T("���衾9��") );											 
	m_PolishingDate.AddString( _T("��ʾ��1��") );
	m_PolishingDate.AddString( _T("��ʾ��2��") );
	m_PolishingDate.AddString( _T("��ʾ��3��") );	
	m_PolishingDate.AddString( _T("��ʾ��4��") );		
											 
	/*m_progress.SetRange(0, 1000);
	m_progress.SetPos(0);
	m_progress.SetPos(100);*/

	return TRUE;  
}

void CSocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSocketClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC  dcMem;   
                  dcMem.CreateCompatibleDC(&dc);   
                 CBitmap  bmpBackground;   
                  bmpBackground.LoadBitmap(IDB_BITMAP1);  //�˴���IDB_SHOUYE�����bitmap����Ҫ�����ͼƬ
    	         BITMAP  bitmap;   
                  bmpBackground.GetBitmap(&bitmap);  
		CBitmap  *pbmpOld=dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
		//CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSocketClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

struct sockaddr_in serveraddr;

//char  StringBuf[512*1024];


char *DeleteSpace(char *str)   //ȥ���ַ����ո���
{
    char *p,*q;
    for (p=q=str;*p!='\0';)
    {
        if (*p==' '|| *p=='\n' || *p=='\r')
            ++p;
        else
            *q++=*p++;
    }
    *q='\0';
    return str;
}

int myDelay(double timeDelay)//��ʱ����
{

	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq=(double)litmp.QuadPart;//��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1=litmp.QuadPart;//��ó�ʼֵ
	do 
	{
		QueryPerformanceCounter(&litmp);
		QPart2=litmp.QuadPart;//�����ֵֹ
		dfMinus=(double)(QPart2-QPart1);
		dfTim=dfMinus/dfFreq;//��ö�Ӧ��ʱ��ֵ����λΪ��
	} while (dfTim*1000<timeDelay);
	
	return 0;
}



//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                           �����㷨���β�Ϊ��������ʵ�ʽӴ���  ������ֵΪDeltaPosition                              ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

double Control_Method(const double DesiredForce, double force, double n_Delta_Postion)
{

	double Kp=0.02*1*0.5, Ki=0.02*0.5;
	double fd=10;
	int i=0;
	
	if (force > 30)//F>40Nʱ���ᵶ
	{
		//SwitchPolishCase = 5;
		DeltaPosition=-0.1;

	}
	else if (force < 1.5)//��ʱδ�Ӵ��������棬ÿ�ν���0.01mm�����Ӵ���������
	{
		DeltaPosition = 0.01;
//		SwitchPolishCase = 12;
	}	
	else
	{
		double limit_delpos=0.01;		
		Force_Parameter.force_N=force;
		Force_Parameter.e_N=DesiredForce-Force_Parameter.force_N;	
		Force_Parameter.de_N=Force_Parameter.force_N_1-Force_Parameter.force_N;
		
		Force_Parameter.u_N=Kp*Force_Parameter.de_N+Ki*Force_Parameter.e_N;
		
		if(Force_Parameter.u_N>limit_delpos )
		{
			Force_Parameter.u_N=limit_delpos;	
		}else if(Force_Parameter.u_N<-limit_delpos)
		{
			Force_Parameter.u_N=-limit_delpos;		
		}
		
		DeltaPosition = Force_Parameter.u_N;		
		Force_Parameter.force_N_1=Force_Parameter.force_N;
	}

	/*if(OnBackStepFlag == true)//������º��ˣ�������ĩ��Զ�빤������
	{
		OnBackStepFlag = false;
		DeltaPosition-=5;
	}*/

	return DeltaPosition;//����ֵDeltaPosition��Ϊȫ�ֱ�����������ʹ��

}


//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                        UDP�����������߳�                                                           ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DWORD WINAPI threadproServer(LPVOID pParam)//UDP�����������߳�
{
	
	CSocketClientDlg *pDlg=(CSocketClientDlg*)pParam;
	char cWelcomBuffer[]="Welcome to you\0";
	int iWhileListenCount=10;//���������
	CString strTmp;
	int num;   
	//��ʼ������Ϣ
	sockaddr_in addrFrom;//�������ӵ�ַ(KUKA�ͻ��˵�ַ��Ϣ)
	int len=sizeof(addrFrom);
	
	char eachBuf[50*1024];//����������ݻ�����
	memset(eachBuf,0,50*1024);//memset()������ʼ���ַ����飬��ʼ����ֵֻ��Ϊ0����-1

	CString sendstr,sendstr1,sendstr2,sendstr3,sendstr4,sendstr5,sendstr6,sendstr7,sendstr8,sendstr9,sendstr10 ;

//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     ��һ��       ����XML�ļ�                                                       ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	while(1)
	{
		num=recvfrom(pDlg->m_SockClient,eachBuf,50*1024,NULL,(sockaddr*)&addrFrom,&len);
		char receive[50*1024];	
		memset(receive,0,50*1024);
		if(num>=0)
		{  
			for(int j=0;j<num;j++)
			{
				receive[j]=eachBuf[j];//StringBuf1�ַ�������������н���
			}
	
			
//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     �ڶ���       ����XML�ļ�                                                       ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			CString cstr;
			DeleteSpace(receive);
			CString tempstr="";	
			CString str="",str1="",str2="";
			char *p=DeleteSpace(receive);	
			string s1;
			s1=p;
			int nfind=0,ans,i=0,nipoc1,nipoc2;
			int arry[100];
			while(i<60)
			{
				s1.find(('"'),nfind);
				ans=s1.find(('"'),nfind+1);
				arry[i]=ans;
				i++;
				nfind=ans;
			}	
			CString a,sx,sy,sz,sipoc,s;
			CString Cstr(p);//charת��ΪCString,��ֵ��CStr���洢��ȡ�Ĵ������ַ�����Ϣ��;
			
			sx=Cstr.Mid(arry[2]+1,arry[3]-arry[2]-1);//Mid������ȡһ���ַ�������һ��������ʾ��ȡ��㣬�ڶ���������ʾ��ȡ���ȣ�
			Pos[0]=atof(sx);                     //�������X;
			sy=Cstr.Mid(arry[4]+1,arry[5]-arry[4]-1);
			Pos[1]=atof(sy);                      //�������Y;
			sz=Cstr.Mid(arry[6]+1,arry[7]-arry[6]-1);//Mid()��һ��������1����Ȼ�������ǡ�;��2������-1����Ȼ����һλ;
			Pos[2]=atof(sz);  
			//�������Z;
			nipoc1=s1.find("IPOC",arry[20]);
			nipoc2=s1.find("IPOC",nipoc1+1);
			sipoc=Cstr.Mid(nipoc1+5,nipoc2-nipoc1-7);    //��ȡIPOC��ǩ��Ϣ;
			


//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     ������       ��ȫ���                                                          ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Pos_Realtime[0] = Pos[0] ; Pos_Realtime[1] = Pos[1] ; Pos_Realtime[2] = Pos[2] ;
			// �����飬��������˷��������곬�������İ�ȫ��Χ��������ֱ�ӹر�
			

			if( ( SwitchPolishCase != 5 && Debug_switch == 0 ) && (Pos_Realtime[0] > LIN_x_max || Pos_Realtime[0] < LIN_x_min || Pos_Realtime[1] > LIN_y_max || 
				Pos_Realtime[1] < LIN_y_min || Pos_Realtime[2] > LIN_z_max || Pos_Realtime[2] < LIN_z_min) ) 
			{

				Polish_Hoist_i = 3;	
				SwitchPolishCase = 5 ;
				Warning_view.Format(" �����˷��������곬��" );
				pDlg->SetDlgItemText(IDC_Warning_view,Warning_view);

			}
			if ( SwitchPolishCase != 2 && Force_result > 30 )
			{
				Polish_Hoist_i = 3;	
				SwitchPolishCase = 5 ;
				Warning_view.Format(" �������׹�������" );
				pDlg->SetDlgItemText(IDC_Warning_view,Warning_view);
			
			}

			
//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     ���Ĳ�       �������㷨                                                        ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
									
		    DeltaPosition_2=Control_Method(DesiredForce,Force_result,n_Delta_Postion);
			//DeltaPosition_2=0.000; //�ر��������㷨����Ҫ���Բ��������Ϸ�����

			// ������ mfc�������и����԰�ť����߱༭���ֵ��ΪCase_force
//			Force_result = Case_force;


			
//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     ���岽       �׹�ָ��ѡ��                                                      ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


			// �׹�ָ��ѡ��
			switch (SwitchPolishCase)
			{
			case 12: //���ı�λ��ipoint����������,ֻ�������ӹ�ʱ�Żᱻ����
			
				Polish_view.Format("PolishCase%d������" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);	
				
					Send_Pos[0] = -ppoint[ipoint].a*0.02;
					Send_Pos[1] = -ppoint[ipoint].b*0.02;
					Send_Pos[2] = -ppoint[ipoint].c*0.02;

					Send_DeltaPosition[0] = Send_Pos[0] ;
					Send_DeltaPosition[1] = Send_Pos[1] ;
					Send_DeltaPosition[2] = Send_Pos[2] ;
					
					if( Force_result > 1.5 )
					{
						//Switch_Case = SwitchPolishCase ;
						SwitchPolishCase = 1 ;
					}
			
				
				break ;
			
			case 11: // �ǶȽ���

				Polish_view.Format("PolishCase%d�ǶȽ�������" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				
				break;

			case 10: // �ӹ�Ԥ��
				
				Polish_view.Format("PolishCase%d�ӹ�Ԥ��" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				

				Polish_Advance_i ++ ; //��ʼֵΪ0						
				if (Polish_Advance_i <= 500) //�ӹ�Ԥ���ĸ߶�Ϊ10���� С��500
				{
					
					Send_Pos[0] = 0.00 ; Send_Pos[1] = 0.00 ; Send_Pos[2] = -0.2 ;
					
				}
				else if ( Polish_Advance_i > 500 && Polish_Advance_i <= 1000) //��1���״����������ƽ���������
				{
					
// 					Send_Pos[0] = -ppoint[itime].a/10 + ppoint[0].a*DeltaPosition_2; 
// 					Send_Pos[1] = -ppoint[itime].b/10 + ppoint[0].b*DeltaPosition_2; 
// 					Send_Pos[2] = -ppoint[itime].c/10 + ppoint[0].b*DeltaPosition_2;
					Send_Pos[0] = -ppoint[ipoint].a/100 ; 
					Send_Pos[1] = -ppoint[ipoint].b/100 ; 
					Send_Pos[2] = -ppoint[ipoint].c/100 ;

				}
				else
				{
					
					Send_Pos[0] = 0.00 ; 
					Send_Pos[1] = 0.00 ; 
					Send_Pos[2] = 0.00 ;
					
					SwitchPolishCase = 1 ;
				}
				
				
				
				if( Force_result > 1.5 )
				{
					//Switch_Case = SwitchPolishCase ;
					SwitchPolishCase = 1 ;
				}
				
				
				break;
			case 1: //�����׹�
				Polish_view.Format("PolishCase%d�����׹�" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				itime ++;//������whileѭ��һ�μ�һ    itime ��ʱΪ1
				if (itime<= alldtime )
				{	
					
					//dtime�ֶ��ܺ�			
					if (itime>dtime)   // dtime ��ʱΪ0������Ϊ�棬���� ipontֱ��ָ���һ����������
					{

						ipoint ++;
						dtime += ppoint[ipoint].dd;


						True_Pos[0] += ppoint[ipoint].x ; 
						True_Pos[1] += ppoint[ipoint].y ; 
						True_Pos[2] += ppoint[ipoint].z ;

						Send_X1_view.Format("%f" , True_Pos[0]);
						pDlg->SetDlgItemText(IDC_Send_X1,Send_X1_view);
						Send_Y1_view.Format("%f" , True_Pos[1]);
						pDlg->SetDlgItemText(IDC_Send_Y1,Send_Y1_view);
						Send_Z1_view.Format("%f" , True_Pos[2]);
						pDlg->SetDlgItemText(IDC_Send_Z1,Send_Z1_view);
					}
					
					dx=ppoint[ipoint].x/ppoint[ipoint].dd;
					dy=ppoint[ipoint].y/ppoint[ipoint].dd;
					dz=ppoint[ipoint].z/ppoint[ipoint].dd;
					
					EndToolCoordRelRobot[0]=dx-ppoint[ipoint].a*DeltaPosition_2;
					EndToolCoordRelRobot[1]=dy-ppoint[ipoint].b*DeltaPosition_2;
					EndToolCoordRelRobot[2]=dz-ppoint[ipoint].c*DeltaPosition_2;

					Check_Pos[0] += EndToolCoordRelRobot[0] ; Check_Pos[1] += EndToolCoordRelRobot[1]; Check_Pos[2] += EndToolCoordRelRobot[2];
				
					Send_Pos[0] = EndToolCoordRelRobot[0] ; Send_Pos[1] = EndToolCoordRelRobot[1] ; Send_Pos[2] = EndToolCoordRelRobot[2];

					Send_DeltaPosition[0] = -ppoint[ipoint].a*DeltaPosition_2 ;
					Send_DeltaPosition[1] = -ppoint[ipoint].b*DeltaPosition_2 ;
					Send_DeltaPosition[2] = -ppoint[ipoint].c*DeltaPosition_2 ;

					
					if ( Force_result < 1.5 )
					{
						Switch_Case = SwitchPolishCase ;
						SwitchPolishCase = 12 ;
					}
							
				} 
				else if (itime>alldtime && itime<=(alldtime+100) )
				{
					
					Send_Pos[0] = 0.00 ; 
					Send_Pos[1] = 0.00 ; 
					Send_Pos[2] = 0.05 ;
					
				} 
				else
					
				{
					Send_Pos[0] = 0.00 ; 
					Send_Pos[1] = 0.00 ; 
					Send_Pos[2] = 0.00 ;
					SwitchPolishCase = 5 ; //�׹���ɺ��ᵶ
				}
	

				break;

			case 2: // ��ͣ�׹� �����������ͣ������һֱ����λ���������꣨0��0��0��

				Polish_view.Format("PolishCase%d�ȴ�����ָ��" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);


				Send_Pos[0] = 0.00 ; Send_Pos[1] = 0.00 ; Send_Pos[2] = 0.00;
				
				break;
				
			case 3:  // �����׹� Continue
				
				Polish_view.Format("PolishCase%d�����׹�" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				Polish_Continue_i ++ ;
				if (Polish_Continue_i <= Polish_Continue_d ) 
				{
					
					Send_Pos[0] = Polish_Continue_D[0] ; 
					Send_Pos[1] = Polish_Continue_D[1] ; 
					Send_Pos[2] = Polish_Continue_D[2] ;

				}


				else
				{
					
					Send_Pos[0] = 0.00 ; 
					Send_Pos[1] = 0.00 ; 
					Send_Pos[2] = 0.00 ;
					SwitchPolishCase = 10;
					
				}

					
				break;
				
			case 4: // ��������ͷ��ʼ�׹�
				
				Polish_view.Format("PolishCase%d��ʼ/��ͷ�׹�" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				Polish_Start_i ++ ;
				if (Polish_Start_i <= Polish_Start_d ) 
				{
					
					Send_Pos[0] = Polish_Start_D[0] ; 
					Send_Pos[1] = Polish_Start_D[1] ; 
					Send_Pos[2] = Polish_Start_D[2] ;

				}
				else
				{
					
					
					Send_Pos[0] = 0.00 ; 
					Send_Pos[1] = 0.00 ; 
					Send_Pos[2] = 0.00 ;
					
					SwitchPolishCase = 10;
					ipoint = 0 ; itime = 0 ; dtime = 0 ;
					
				}

				break;

			case 5: // �������ᵶ

				Polish_view.Format("PolishCase%d�����ᵶ" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);

				Polish_Hoist_i ++ ; //��ʼֵΪ1

				if (Polish_Hoist_i < 4) 
				{
					
					Send_Pos[0] = 0.00 ; 
					Send_Pos[1] = 0.00 ; 
					Send_Pos[2] = 0.00 ;
					
				}
				else if ( Polish_Hoist_i >= 4 && Polish_Hoist_i <= 503)  // 503
				{
					
					Send_Pos[0] = 0.00 ; 
					Send_Pos[1] = 0.00 ; 
					Send_Pos[2] = 0.20 ;
				}
				else
				{

					Send_Pos[0] = 0.00 ; 
					Send_Pos[1] = 0.00 ; 
					Send_Pos[2] = 0.00 ;
					
					SwitchPolishCase = 2 ;
				}
				
				
				break;
				
			case 6: // ����������
				
				Polish_view.Format("PolishCase%d����������" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				

				if ( Polish_Text_i > 1000 )
				{
					Text_Force_a *= -1 ; 
					Text_Force_b *= -1 ; 
					Text_Force_c *= -1 ;
					Polish_Text_i = 0 ;
					
				}

				if ( Force_result > 30 )
				{
					Text_Force_a *= -1 ; 
					Text_Force_b *= -1 ; 
					Text_Force_c *= -1 ;
					Polish_Text_i = 0 ;
					
				}
				
				
				Send_Pos[0] = 0.1 * Text_Force_a ; 
				Send_Pos[1] = 0.1 * Text_Force_b ; 
				Send_Pos[2] = 0.1 * Text_Force_c ;
				
				
				Polish_Text_i ++  ;

				//closesocket(pDlg->m_SockClient); //���뻹δ���ƣ�����󰴴˰�ť��������ֱ�ӹر�
				break;
				
			case 7: // ͨ��ֹͣ

				Polish_view.Format("PolishCase%dͨ��ֹͣ" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				closesocket(pDlg->m_SockClient);
				break;
				
			case 8: // ʾ�̲���
				
				Polish_view.Format("PolishCase%dʾ�̲���" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				
				if ( Force_result > 60 )
				{

					SwitchPolishCase = 5 ;
					
				}


				if (Teach_i == 0 || Teach_i > Teach_d ) 
				{
					
					Send_Pos[0] = 0.00 ; 
					Send_Pos[1] = 0.00 ; 
					Send_Pos[2] = 0.00 ;
					
				}
				else
				{
					
					Send_Pos[0] = Teach_Pos[0] ; 
					Send_Pos[1] = Teach_Pos[1] ; 
					Send_Pos[2] = Teach_Pos[2];
					
					Teach_i ++ ;

				}
				break;

			case 9: // һ������


				Polish_view.Format("PolishCase%dһ������" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);


				if ( Polish_Fast_j == 0 && Polish_Fast_i < 500 ) 
				{
					
					Send_Pos[0] = 0.00 ; Send_Pos[1] = 0.00 ; Send_Pos[2] = Polish_Fast_D[2] ;
					Polish_Fast_i ++ ; 

					if (Polish_Fast_i == 500)
					{
						Polish_Fast_j = 1;
					}

					
				}
				else if ( Polish_Fast_j <= Polish_Fast_d )  // 1003
				{
					
					Send_Pos[0] = Polish_Fast_D[0] ; 
					Send_Pos[1] = Polish_Fast_D[1] ; 
					Send_Pos[2] = 0.0 ;

					Polish_Fast_j ++ ; 

				}
				else if (( Polish_Fast_j > Polish_Fast_d )&& (Polish_Fast_j <= (Polish_Fast_d + Polish_Fast_down_d )) )
				{

					Send_Pos[0] = 0.00 ; 
					Send_Pos[1] = 0.00 ; 
					Send_Pos[2] = Polish_Fast_D[3] ;
					Polish_Fast_j ++ ; 
				} 
				else

				{
					
					Send_Pos[0] = 0.00 ;
					Send_Pos[1] = 0.00 ;
					Send_Pos[2] = 0.00 ;
					
					SwitchPolishCase = 2 ; // ��ͣ�׹�
				}

				break;
				
			case 13: // ײ��ĥ������
				
				
				Polish_view.Format("PolishCase%dײ��ĥ������" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				
				
				Send_Pos[0] = 0 ; 
				Send_Pos[1] = -0.01 ; 
				Send_Pos[2] = 0 ;
	
				if ( Force_result > 30 )
				{
					
					SwitchPolishCase = 5 ;
					
				}
				
				break;


			case 14: // ĥ������
				
				
				Polish_view.Format("PolishCase%dĥ������" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				int pa;
				int pb;
				int pc ;

				
				dx = 0 ; 
				dy = -0.01 ; 
				dz = 0 ;
				pa = 0 ;
				pb = 1 ;
				pc = 0 ;
				
				
				EndToolCoordRelRobot[0]=dx-pa*DeltaPosition_2;
				EndToolCoordRelRobot[1]=dy-pb*DeltaPosition_2;
				EndToolCoordRelRobot[2]=dz-pc*DeltaPosition_2;
				
				
				Send_Pos[0] = EndToolCoordRelRobot[0] ; Send_Pos[1] = EndToolCoordRelRobot[1] ; Send_Pos[2] = EndToolCoordRelRobot[2];
				
				
				if ( Force_result > 30 )
				{
					
					SwitchPolishCase = 5 ;
					
				}
				
				break;
				
				
				
				
			}
			
			
//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     ������       �����ļ���鼰�洢                                                ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			
			
			//   �����飬�ٶȹ��죬ֱ�ӹرշ�������ͬ���ģ�Ҳ��Ϊ�˷�ֹ����������

			if ( pow (Send_Pos[0],2 ) + pow (Send_Pos[1],2 ) + pow (Send_Pos[2],2 ) > pow (MS_speed_max,2 ) )
			{
				closesocket(pDlg->m_SockClient);
				Warning_view.Format(" ���͵���ٶȹ���" );
				pDlg->SetDlgItemText(IDC_Warning_view,Warning_view);

			}
			
			
			// �����飬����׹�㳬�������İ�ȫ��Χ���ᵶ
			// Check_Pos[0] += EndToolCoordRelRobot[0] ; Check_Pos[1] += EndToolCoordRelRobot[1]; Check_Pos[2] += EndToolCoordRelRobot[2];
			// �Ѿ���CASE1�����½����˸�ֵ
			
			if( ( SwitchPolishCase != 5 && Debug_switch == 0 ) && ( Check_Pos[0] > LIN_x_max || Check_Pos[0] < LIN_x_min || Check_Pos[1] > LIN_y_max || 
				Check_Pos[1] < LIN_y_min || Check_Pos[2] > LIN_z_max || Check_Pos[2] < LIN_z_min ) )
			{
				Send_Pos[0] = 0.00 ; 
				Send_Pos[1] = 0.00 ; 
				Send_Pos[2] = 0.20 ;

				Polish_Hoist_i = 0;
				SwitchPolishCase=5; //�ᵶ
				Warning_view.Format(" �������곬����Χ" );
				pDlg->SetDlgItemText(IDC_Warning_view,Warning_view);
			}


//�жϷ�������λ��������λ������Ƿ񳬳���̬���ݺз�Χ


/*			if( Debug_switch == 0 && ( SwitchPolishCase == 1 || SwitchPolishCase == 12 ) )
			{	

				if( Pos_Realtime[0] > True_Pos[0] + MS_limit_Box || Pos_Realtime[0] < True_Pos[0] - MS_limit_Box ||
					Pos_Realtime[1] > True_Pos[1] + MS_limit_Box || Pos_Realtime[1] < True_Pos[1] - MS_limit_Box ||
					Pos_Realtime[2] > True_Pos[2] + MS_limit_Box || Pos_Realtime[2] < True_Pos[2] - MS_limit_Box )
				{ 
					SwitchPolishCase=5;
					Polish_Hoist_i=0;
				}
				
			}

*/

// ��������
			if ( SwitchPolishCase != 2 ) // ʵʱ�洢�������ݳ���ͣ������
			{
				
				SaveData_Socket[Socket_Client_j][0] = SwitchPolishCase ; 
				SaveData_Socket[Socket_Client_j][1] = Send_Pos[0] ; 
				SaveData_Socket[Socket_Client_j][2] = Send_Pos[1] ; 
				SaveData_Socket[Socket_Client_j][3] = Send_Pos[2] ; 
				SaveData_Socket[Socket_Client_j][4] = Pos[0] ; 
				SaveData_Socket[Socket_Client_j][5] = Pos[1] ; 
				SaveData_Socket[Socket_Client_j][6] = Pos[2] ; 
				SaveData_Socket[Socket_Client_j][7] = Send_DeltaPosition[0] ; 
				SaveData_Socket[Socket_Client_j][8] = Send_DeltaPosition[1] ; 
				SaveData_Socket[Socket_Client_j][9] = Send_DeltaPosition[2] ; 
				SaveData_Socket[Socket_Client_j][10] =Force_result;
				Socket_Client_j += 1;
			}

			if ( SwitchPolishCase == 2 && save_stop_i == 0  ) //��ͣʱֻ�洢һ������
			{
				
				SaveData_Socket[Socket_Client_j][0] = SwitchPolishCase ; 
				SaveData_Socket[Socket_Client_j][1] = Send_Pos[0] ; 
				SaveData_Socket[Socket_Client_j][2] = Send_Pos[1] ; 
				SaveData_Socket[Socket_Client_j][3] = Send_Pos[2] ; 
				SaveData_Socket[Socket_Client_j][4] = Pos[0] ; 
				SaveData_Socket[Socket_Client_j][5] = Pos[1] ; 
				SaveData_Socket[Socket_Client_j][6] = Pos[2] ; 
				SaveData_Socket[Socket_Client_j][7] = Send_DeltaPosition[0] ; 
				SaveData_Socket[Socket_Client_j][8] = Send_DeltaPosition[1] ; 
				SaveData_Socket[Socket_Client_j][9] = Send_DeltaPosition[2] ; 
				SaveData_Socket[Socket_Client_j][10] =Force_result;
				
				save_stop_i += 1;
			}


//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     ���߲�       �򴰿ڷ�������                                                    ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			// �������ʵʱ���͵�����
			Send_X_view.Format("%f" , Send_Pos[0]);
			pDlg->SetDlgItemText(IDC_Send_X,Send_X_view);
			Send_Y_view.Format("%f" , Send_Pos[1]);
			pDlg->SetDlgItemText(IDC_Send_Y,Send_Y_view);
			Send_Z_view.Format("%f" , Send_Pos[2]);
			pDlg->SetDlgItemText(IDC_Send_Z,Send_Z_view);

			// �׹⹤���ľ������꣬ҲΪ������
			Send_X1_view.Format("%f" , True_Pos[0]);
			pDlg->SetDlgItemText(IDC_Send_X1,Send_X1_view);
			Send_Y1_view.Format("%f" , True_Pos[1]);
			pDlg->SetDlgItemText(IDC_Send_Y1,Send_Y1_view);
			Send_Z1_view.Format("%f" , True_Pos[2]);
			pDlg->SetDlgItemText(IDC_Send_Z1,Send_Z1_view);

			
			// ������˷��͵ľ�������
			Send_XYZ[0] += Send_Pos[0] ;
			Send_XYZ[1] += Send_Pos[1] ;
			Send_XYZ[2] += Send_Pos[2] ;
			
// ����������ʱ��Ҫ��	Send_XYZ �ĳ� Pos_Realtime	

			if ( Debug_switch == 0 )
			{
				Teach_X_view.Format("%f" ,Pos_Realtime[0]);
				pDlg->SetDlgItemText(IDC_Teach_X,Teach_X_view);
				Teach_Y_view.Format("%f" ,Pos_Realtime[1]);
				pDlg->SetDlgItemText(IDC_Teach_Y,Teach_Y_view);
				Teach_Z_view.Format("%f" ,Pos_Realtime[2]);
				pDlg->SetDlgItemText(IDC_Teach_Z,Teach_Z_view);
			}
			if(	Debug_switch == 1 )
			{
				Teach_X_view.Format("%f" ,Send_XYZ[0]);
				pDlg->SetDlgItemText(IDC_Teach_X,Teach_X_view);
				Teach_Y_view.Format("%f" ,Send_XYZ[1]);
				pDlg->SetDlgItemText(IDC_Teach_Y,Teach_Y_view);
				Teach_Z_view.Format("%f" ,Send_XYZ[2]);
				pDlg->SetDlgItemText(IDC_Teach_Z,Teach_Z_view);
			}


			pDlg->m_progress.SetPos(ipoint); // ����������

			Progress_View.Format("%d/%d" ,itime,alldtime);
			pDlg->SetDlgItemText(IDC_Progress_View,Progress_View);


			

//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     �ڰ˲�       дXML�ļ�                                                         ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			
			sendstr1.Format("<Sen Type=\"ImFree\">\r\n");
			sendstr2.Format("<EStr>Message from RSI FLY</EStr>\r\n");
			sendstr3.Format("<RKorr ");
			sendstr4.Format("X=\"%.6lf\" ", Send_Pos[0] );
			sendstr5.Format("Y=\"%.6lf\" ", Send_Pos[1] );
			sendstr6.Format("Z=\"%.6lf\" ", Send_Pos[2] );
			sendstr7.Format("A=\"0.000\" B=\"0.000\" C=\"0.000\" />\r\n");
			sendstr8.Format("<IPOC>%s</IPOC>\r\n",sipoc);
			sendstr9.Format("</Sen>\r\n");
			sendstr=sendstr1+sendstr2+sendstr3+sendstr4+sendstr5+sendstr6+sendstr7+sendstr8+sendstr9;
			

//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     �ھŲ�       ����XML�ļ�                                                       ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		
			
			int sendlength=sendstr.GetLength();
			
			//1.�Ƚ�sendstr�ַ���д��SendBUFF[50*1024]�ַ�����
			char SendBUFF[50*1024];  //�������ݻ�������
			for(int k=0;k<sendstr.GetLength();k++)
			{
				SendBUFF[k]=sendstr.GetAt(k);
			}

			 //��Ҫ���͵�ÿ�λ������ַ��������ݶ�д��send.txt
			FILE *fp1;
			fp1=fopen("D:\\send.txt","at");
			fwrite(SendBUFF,k,1,fp1);
			fclose(fp1);

            //2.��SendBUFF���������д��eachBufSEND������
			int nretval,Send;
    		BYTE eachBufSEND[50*1024];
    		for(Send=0;Send<sendlength;Send++)
			{
    		    eachBufSEND[Send]=(BYTE)SendBUFF[Send];
			}
	    	nretval=sendto(pDlg->m_SockClient,(char*)eachBufSEND,sendlength,0,(sockaddr*)&addrFrom,sizeof(addrFrom));
			//sendto:�ɹ��Ļ����ط��͵��ֽ�����sendlength:eachBufSEND�����������ݵĳ���
			
		}//�н���          
	}//whileѭ������
	//closesocket(pDlg->m_SockClient);	
	return 0;	
}

//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                                ����UDP������                                                       ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	

void CSocketClientDlg::OnConnect() //����UDP������
{
	CString strIP,strPort,strDForce,strResult;
	GetDlgItem(IDC_ED_IP)->GetWindowText(strIP);
	GetDlgItem(IDC_ED_PORT)->GetWindowText(strPort);
	GetDlgItem(IDC_ED_TEXT)->GetWindowText(strDForce);//��ȡ��������
	//struct sockaddr_in serveraddr;
	int iPort;
	DWORD iThreadId=0;
	iPort=atoi(strPort);
	DesiredForce=atoi(strDForce);
	m_SockClient=socket(AF_INET,SOCK_DGRAM,0);
	CString strTmp;
	int iBindResult;
	
	Polish_view.Format("�󶨵Ķ˿���%d\r\n",iPort);
	SetDlgItemText(IDC_Polish_view,Polish_view);
	Connect_view.Format("�����ɹ�");
	SetDlgItemText(IDC_Connect_view,Connect_view);

	//���������ַ��Ϣ
	serveraddr.sin_family=AF_INET;					
	serveraddr.sin_port=htons(iPort);	//�˿�			
	serveraddr.sin_addr.S_un.S_addr=inet_addr(strIP.GetBuffer(0));//��ַ
	//�󶨵�ַ��Ϣ
    iBindResult=bind(m_SockClient,(struct sockaddr*)&serveraddr,sizeof(sockaddr));
	
	if(iBindResult!=0)
	{

		Polish_view.Format("KUKA����ʧ�ܣ�������");
		SetDlgItemText(IDC_Polish_view,Polish_view);
		Connect_view.Format("����ʧ��");
		SetDlgItemText(IDC_Connect_view,Connect_view);
		
		return;
	}
	HANDLE threadproServerThread=CreateThread(NULL,0,threadproServer,(LPVOID)this,0,&iThreadId);
    CloseHandle(threadproServerThread);
}


void CSocketClientDlg::OnExit() //�˳�
{
	OnCancel();	
}


BOOL CSocketClientDlg::DestroyWindow() 
{
	
	return CDialog::DestroyWindow();
}


//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                       UDPLabView�����������߳�                                                     ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	

DWORD WINAPI threadproLabView(LPVOID pParam)//UDPLabView�����������߳�
{
	CSocketClientDlg *pDlgF=(CSocketClientDlg*)pParam;
	int num,SendNum;
	//sockaddr_in addrFrom;//�������ӵ�ַ
	struct sockaddr_in addrFrom;
    int len = sizeof(struct sockaddr_in);  //
	//int len=sizeof(addrFrom);
	char  recvBuf[100]={0};
	int i=0;
	CString s1,Force_x_str,Force_y_str,Force_z_str, Force_result_str;
	int nfind = 0, ans;
	while(1)    
	{
			   
		num=recvfrom(pDlgF->m_SockForce,recvBuf,100,NULL,(sockaddr*)&addrFrom,&len);
		//len = recvfrom(sockfd, recvline, 512, 0, (struct sockaddr *) &ca,  (socklen_t *)&struct_len); 
		int h=WSAGetLastError();
		if (num>=0)
		{
			int arry[4] = {0};
			i=0;ans = 0; nfind = 0;
			s1.Format("%s",recvBuf);
			while(i<4)
			{
				ans=s1.Find('#', nfind+1);
				arry[i]=ans;
				i++;
				nfind=ans;
			}	
			Force_x_str=s1.Mid(0,arry[0]-1);
			Force_x=atof(Force_x_str);

			Force_y_str=s1.Mid(arry[0]+1,arry[1]-arry[0]-1);
			Force_y=atof(Force_y_str);

			Force_z_str=s1.Mid(arry[1]+1,arry[2]-arry[1]-1);
			Force_z=atof(Force_z_str);

			Force_result_str=s1.Mid(arry[2]+1,arry[3]-arry[2]-1);
			Force_result=atof(Force_result_str);
			
 /*			Force_result=2;
			GetDlgItem(IDC_EDIT1)->SetWindowText(recvBuf);
			((CEdit*)GetDlgItem(IDC_TOOL_RADIUS))->SetWindowText("5.00"); */
			(pDlgF->GetDlgItem(IDC_ED_FORCE))->SetWindowText(Force_result_str);
			(pDlgF->GetDlgItem(IDC_ED_FORCE))->UpdateWindow();
			
			char sendBuf[50];
			SendNum=40;
			//�������źź�xyz���굽LabVIEW
			sprintf(sendBuf, "%.3fx%.3fy%.3fz%.3fp%.5fk%.5fs",Pos[0],Pos[1],Pos[2],-DeltaPosition_2, gKi, n_Delta_Postion);
			int num=strlen(sendBuf); 
			char numBuf[10];

			sprintf(numBuf,"%d",num);
			sendto(pDlgF->m_SockForce,(char*)numBuf,strlen(numBuf),0,(sockaddr*)&addrFrom,sizeof(addrFrom));//�����ַ�������
			sendto(pDlgF->m_SockForce,(char*)sendBuf,strlen(sendBuf),0,(sockaddr*)&addrFrom,sizeof(addrFrom));//�����ַ���
		}				
	}
	closesocket(pDlgF->m_SockForce);
	//return 0;

}

void CSocketClientDlg::OnButton6() 
{
    CString strIP,strPort,strDForce;
	int iPort,iBindResult;
	DWORD iThreadId=0;

	GetDlgItem(IDC_ED_IP)->GetWindowText(strIP);//�õ�����IP�Ͷ˿�
	GetDlgItem(IDC_ED_LabViewPORT)->GetWindowText(strPort);
	iPort=atoi(strPort);
	sockaddr_in serveraddr;
	serveraddr.sin_family=AF_INET;	//���÷�������ַ����
	serveraddr.sin_port=htons(iPort);	//���÷������˿ں�
	serveraddr.sin_addr.S_un.S_addr=inet_addr(strIP.GetBuffer(0));
	//GetDlgItem(IDC_ED_TEXT)->GetWindowText(strDForce);
			
	m_SockForce=socket(AF_INET,SOCK_DGRAM,0);
	iBindResult=bind(m_SockForce,(struct sockaddr*)&serveraddr,sizeof(sockaddr));
	
	if(iBindResult!=0)
	{
		MessageBox("�󶨶˿�ʧ��!\r\n");
		Polish_view.Format("LeabVIEW����ʧ�ܣ�������");
		SetDlgItemText(IDC_Polish_view,Polish_view);
	    //return;
	}
	//CreateThread(NULL,0,threadproServer,(LPVOID)this,0,&iThreadId );

	HANDLE threadproLabViewThread=CreateThread(NULL,0,threadproLabView,(LPVOID)this,0,&iThreadId);
	CloseHandle(threadproLabViewThread);
	//sendto(m_SockClient,strDForce.GetBuffer(0),strText.GetLength(),0,(sockaddr*)&addrTo,sizeof(addrTo));	
}


//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                               �����׹��                                                           ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void CSocketClientDlg::OnButton7() //�����׹��
{
	SwitchPolishCase = 2; //���к�ֱ����ͣ���ȴ��ź�
//	m_PolishingDate.ResetContent(); //����б������

	CString strMS_speed_min;
	
	// ��ȡ�����ٶ�	
	GetDlgItem(IDC_MoveSafe_speed_min)->GetWindowText(strMS_speed_min);
	MS_speed_min = atof(strMS_speed_min);//atof �ַ���תdouble

	// ��ȡ�任����ϵ�Ĳ���
	GetDlgItem(IDC_POS_Trans_X)->GetWindowText(strPOS_Trans_X);
	POS_Trans_X = atof(strPOS_Trans_X);//atof �ַ���תdouble
	GetDlgItem(IDC_POS_Trans_Y)->GetWindowText(strPOS_Trans_Y);
	POS_Trans_Y = atof(strPOS_Trans_Y);//atof �ַ���תdouble
	GetDlgItem(IDC_POS_Trans_Z)->GetWindowText(strPOS_Trans_Z);
	POS_Trans_Z = atof(strPOS_Trans_Z);//atof �ַ���תdouble
	GetDlgItem(IDC_POS_Trans_A)->GetWindowText(strPOS_Trans_A);
	POS_Trans_A = atof(strPOS_Trans_A);//atof �ַ���תdouble

		   
    CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"All Files(*.TXT)|*.TXT||",AfxGetMainWnd());//���ļ�
	m_PolishingDate.DeleteString(0);
	int index=0,index1=1;
	if(IDOK!=dlg.DoModal())
	{
		AfxMessageBox("���ļ��Ի���ʧ��!");
	}
	CString str=dlg.GetPathName();
	CStdioFile file;
	if(!file.Open(str,CFile::modeRead))
	{
		AfxMessageBox("���ļ�ʧ��!");
	}
	
    ((CEdit*)GetDlgItem(IDC_EDIT_ADDRESS))->SetWindowText(str);
	CString tempstr,tempstr1,tempstrzh;
	
    
	double data[8]={0.0};	
	
	while(file.ReadString(tempstr))
	{
		tempstr.TrimLeft();
		tempstr.TrimRight();
		int str_len=tempstr.GetLength();
		if(-1!=tempstr.Find("��������"))
		{	
			
			tempstr1.Format("%05d  NOP",index);
			m_PolishingDate.AddString(tempstr1);
			m_PolishingDate.SetCurSel(index);
			index++;
			
		}
		else
		{  
			int s_index=tempstr.Find(':');//��굽:
			tempstr1="";int m=0; 
			//double data[8]={0.0};
			int data_index=0;
			for(int j=s_index+2;j<str_len-1;j++)
			{
				if(!(tempstr.GetAt(j)=='('||tempstr.GetAt(j)==')'))
				{
					if(tempstr.GetAt(j)!=',')//�ö�������������
					{
						tempstr1.Insert(m,tempstr.GetAt(j));//��ÿ�����ݷ���tempstr1����
						m++;
					}
					else
					{
						m=0;
                        data[data_index]=atof(tempstr1);//�����������򱣴�tempstr1������
						tempstr1="";
						data_index++;
					}
				}
				
			}
			data[data_index]=atof(tempstr1);//�õ�8������֮�е����һ������Ϊ������ݺ���û�ж���
		}
		/*EndToolCoordRelRobot[0]=data[2];
		EndToolCoordRelRobot[1]=data[3];
		EndToolCoordRelRobot[2]=data[4];
		
		nvctRelRobot[0]=data[5];
		nvctRelRobot[1]=data[6];
		nvctRelRobot[2]=data[7];*/
		//ppoint[index].d=data[0];
		ppoint[index].d=data[1];
		ppoint[index].dd=ceil(data[1]/MS_speed_min);
// 		ppoint[index].x=data[2];
// 		ppoint[index].y=data[3];
// 		ppoint[index].z=data[4];
// 		ppoint[index].a=data[5];
// 		ppoint[index].b=data[6];
// 		ppoint[index].c=data[7];
		alldtime+=ppoint[index].dd;

// POLISH.CHANGE1_XYZ(:,1) = POLISH.START.XYZ(:,2)*cosd(TRANSFORM_angle) - POLISH.START.XYZ(:,3)*sind(TRANSFORM_angle);
// POLISH.CHANGE1_XYZ(:,2) = POLISH.START.XYZ(:,2)*sind(TRANSFORM_angle) + POLISH.START.XYZ(:,3)*cosd(TRANSFORM_angle);

		ppoint[index].x = data[2] * cos(POS_Trans_A*PI/180) - data[3] *sin(POS_Trans_A*PI/180) ;
		ppoint[index].y = data[2] * sin(POS_Trans_A*PI/180) + data[3] *cos(POS_Trans_A*PI/180) ;
		ppoint[index].z = data[4] ;
		ppoint[index].a = data[5] * cos(POS_Trans_A*PI/180) - data[6] *sin(POS_Trans_A*PI/180) ;
		ppoint[index].b = data[5] * sin(POS_Trans_A*PI/180) + data[6] *cos(POS_Trans_A*PI/180) ;
		ppoint[index].c = data[7];

		if (index == 0)
		{
			ppoint[0].x = ppoint[index].x + POS_Trans_X ;
 			ppoint[0].y = ppoint[index].y + POS_Trans_Y ;
			ppoint[0].z = ppoint[index].z + POS_Trans_Z ;
		}

		tempstr1.Format("%04d (%.3f,%.f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f) ",index,ppoint[index].d,ppoint[index].dd,ppoint[index].x,ppoint[index].y,ppoint[index].z,ppoint[index].a,ppoint[index].b,ppoint[index].c);
		
		m_PolishingDate.AddString(tempstr1);//��ʾ����Ļ��
		m_PolishingDate.SetCurSel(index);

		// �����˶���ȫ��Χ�Ķ�̬���ݺ�
		LIN_x1 += ppoint[index].x;
		LIN_y1 += ppoint[index].y;
		LIN_z1 += ppoint[index].z;
		if (index>0)
		{
			
			LIN_x = LIN_x1 - ppoint[index].x;
			LIN_y = LIN_y1 - ppoint[index].y;
			LIN_z = LIN_z1 - ppoint[index].z;

			if (index == 1) 
			{
				if ( LIN_x1 >= LIN_x) { LIN_x_max = LIN_x1 ; LIN_x_min = LIN_x; }
				else {	LIN_x_max = LIN_x ; LIN_x_min = LIN_x1; }
				if ( LIN_y1 >= LIN_y) { LIN_y_max = LIN_y1 ; LIN_y_min = LIN_y; }
				else {	LIN_y_max = LIN_y ; LIN_y_min = LIN_y1; }
				if ( LIN_z1 >= LIN_z) { LIN_z_max = LIN_z1 ; LIN_z_min = LIN_z; }
				else {	LIN_z_max = LIN_z ; LIN_z_min = LIN_z1; }
			} 
			
			else
			{
				
				LIN_x_max = (((LIN_x > LIN_x1) ? LIN_x : LIN_x1) > LIN_x_max) ? ((LIN_x > LIN_x1) ? LIN_x : LIN_x1) : LIN_x_max;
				LIN_x_min = (((LIN_x < LIN_x1) ? LIN_x : LIN_x1) < LIN_x_min) ? ((LIN_x < LIN_x1) ? LIN_x : LIN_x1) : LIN_x_min;
				LIN_y_max = (((LIN_y > LIN_y1) ? LIN_y : LIN_y1) > LIN_y_max) ? ((LIN_y > LIN_y1) ? LIN_y : LIN_y1) : LIN_y_max;
				LIN_y_min = (((LIN_y < LIN_y1) ? LIN_y : LIN_y1) < LIN_y_min) ? ((LIN_y < LIN_y1) ? LIN_y : LIN_y1) : LIN_y_min;
				LIN_z_max = (((LIN_z > LIN_z1) ? LIN_z : LIN_z1) > LIN_z_max) ? ((LIN_z > LIN_z1) ? LIN_z : LIN_z1) : LIN_z_max;
				LIN_z_min = (((LIN_z < LIN_z1) ? LIN_z : LIN_z1) < LIN_z_min) ? ((LIN_z < LIN_z1) ? LIN_z : LIN_z1) : LIN_z_min;
			}
						
		}

		index++;

	}//whileѭ��
	//PointNumMax=index-1;
	Check_Pos[0] = ppoint[0].x ; Check_Pos[1] = ppoint[0].y ; Check_Pos[2] = ppoint[0].z ;  //�жϵ�ĳ�ʼֵ
	
	//���������ʼ��ֵ
	True_Pos[0] = ppoint[0].x ;
	True_Pos[1] = ppoint[0].y ; 
	True_Pos[2] = ppoint[0].z ; 

	//���������ʼֵ��ʾ
	Send_X1_view.Format("%f" , True_Pos[0]);
	Send_Y1_view.Format("%f" , True_Pos[1]);
	Send_Z1_view.Format("%f" , True_Pos[2]);
	
	// ������˷��͵ľ�������ĳ�ʼֵ
	Send_XYZ[0] = ppoint[0].x ;
	Send_XYZ[1] = ppoint[0].y ;
	Send_XYZ[2] = ppoint[0].z ;	

	SetDlgItemText(IDC_Send_X1,Send_X1_view);
	SetDlgItemText(IDC_Send_Y1,Send_Y1_view);
	SetDlgItemText(IDC_Send_Z1,Send_Z1_view);

	// ����ȫ��Χ����2cm
	LIN_x_max  += 200;
	LIN_x_min  -= 200;
	LIN_y_max  += 200;
 	LIN_y_min  -= 200;
	LIN_z_max  += 800; // �����������˶��ǰ�ȫ�ģ����Ը�ֵ���Դ�һЩ
	LIN_z_min  -= 100;

	// ��������Ҫ�ߵ�ԭ�㣬���Ա����������ԭ��
	if (LIN_x_max < POS_Trans_X )	{ LIN_x_max = POS_Trans_X + 10;	}
	if (LIN_x_min > POS_Trans_X )	{ LIN_x_min = POS_Trans_X - 10;	}
	if (LIN_y_max < POS_Trans_Y )	{ LIN_y_max = POS_Trans_Y + 10;	}
 	if (LIN_y_min > POS_Trans_Y )	{ LIN_y_min = POS_Trans_Y - 10;	}
	if (LIN_z_max < POS_Trans_Z )	{ LIN_z_max = POS_Trans_Z + 10;	}
	if (LIN_z_min > POS_Trans_Z )	{ LIN_z_min = POS_Trans_Z - 10;	}

	MS_limit_x_max.Format("%f" , LIN_x_max);
	MS_limit_x_min.Format("%f" , LIN_x_min);
	MS_limit_y_max.Format("%f" , LIN_y_max);
	MS_limit_y_min.Format("%f" , LIN_y_min);
	MS_limit_z_max.Format("%f" , LIN_z_max);
	MS_limit_z_min.Format("%f" , LIN_z_min);
/*	sprintf(MS_limit_x_max,"%.0f",LIN_x_max);
	sprintf(MS_limit_x_min,"%.0f",LIN_x_min);
	sprintf(MS_limit_y_max,"%.0f",LIN_y_max);
	sprintf(MS_limit_y_min,"%.0f",LIN_y_min);
	sprintf(MS_limit_z_max,"%.0f",LIN_z_max);
	sprintf(MS_limit_z_min,"%.0f",LIN_z_min);
*/

	SetDlgItemText(IDC_MoveSafe_limit_x_max,MS_limit_x_max);
	SetDlgItemText(IDC_MoveSafe_limit_x_min,MS_limit_x_min);
	SetDlgItemText(IDC_MoveSafe_limit_y_max,MS_limit_y_max);
	SetDlgItemText(IDC_MoveSafe_limit_y_min,MS_limit_y_min);
	SetDlgItemText(IDC_MoveSafe_limit_z_max,MS_limit_z_max);
	SetDlgItemText(IDC_MoveSafe_limit_z_min,MS_limit_z_min);

	Polish_view.Format("������ɣ���%d����" ,index-1);
	SetDlgItemText(IDC_Polish_view,Polish_view);

	// ��������ʼֵ��ֵ
	m_progress.SetRange( 0 , index-1 );
	
	Progress_View.Format("%d/%d" ,itime,alldtime);
	SetDlgItemText(IDC_Progress_View,Progress_View);
				
				
}

void CSocketClientDlg::OnClean()  // ����׹���йصı����������ٴε����׹��
{
	for(int index=0;index<MAXNUM;index++)
	{
		ppoint[index].d=0;
		ppoint[index].dd=0;
		ppoint[index].x=0;
		ppoint[index].y=0;
		ppoint[index].z=0;
		
		ppoint[index].a=0;
		ppoint[index].b=0;
		ppoint[index].c=0;
	}
	LIN_x = 0 ;
	LIN_y = 0 ;
	LIN_z = 0 ;
	LIN_x1 = 0 ;
	LIN_y1 = 0 ;
	LIN_z1 = 0 ;
	m_PolishingDate.ResetContent();
}

void CSocketClientDlg::OnSwitchPolish_Pause() // ��������ͣ�˶�
{
	// TODO: Add your control notification handler code here
	
	save_stop_i = 0 ; //�ȴ�ָ��ֻ����һ������
	SwitchPolishCase = 2;  // ��ͣ�˶�ͨ��
	
}

void CSocketClientDlg::OnSwitchPolish_Continue() // �����˼����˶�
{
	// TODO: Add your control notification handler code here


	save_stop_i = 0 ; //�ȴ�ָ��ֻ����һ������
		Polish_Hoist_i = 0;

	Polish_Advance_i = 0 ; // �����˶�������ӹ�Ԥ��

	Polish_Continue_i = 0; //�����˶���һ�ε�ǰ�㵽��ʼ��ľ��룬ͨ��ֱ�߲岹�ߵ���ʼ�㣬ÿ��ִ���궼��Ҫ���¼���
	

	// �����˶�ʱ���׹��ǰ��һ��ipoint
	
	dtime -= ppoint[ipoint].dd ;
	itime = dtime + 1 ;
	ipoint -= 1 ;
	
	Pos_Temporary[0] = True_Pos[0] + 10*ppoint[ipoint].a;
	Pos_Temporary[1] = True_Pos[1] + 10*ppoint[ipoint].b;
	Pos_Temporary[2] = True_Pos[2] + 10*ppoint[ipoint].c + 100;
	
	Send_X1_view.Format("%f" , True_Pos[0]);
	SetDlgItemText(IDC_Send_X1,Send_X1_view);
	Send_Y1_view.Format("%f" , True_Pos[1]);
	SetDlgItemText(IDC_Send_Y1,Send_Y1_view);
	Send_Z1_view.Format("%f" , True_Pos[2]);
	SetDlgItemText(IDC_Send_Z1,Send_Z1_view);

	if(	Debug_switch == 0 )
	{
	Polish_Continue_xyz[0] = -( Pos_Realtime[0] - Pos_Temporary[0] );
	Polish_Continue_xyz[1] = -( Pos_Realtime[1] - Pos_Temporary[1] );
	Polish_Continue_xyz[2] = -( Pos_Realtime[2] - Pos_Temporary[2] );
	}
	
	// �����ã��ڵ���ʱ��Pos_Realtime��ֵ��ΪSend_XYZ
	if(	Debug_switch == 1 )
	{
	Polish_Continue_xyz[0] = -( Send_XYZ[0] - Pos_Temporary[0] );
	Polish_Continue_xyz[1] = -( Send_XYZ[1] - Pos_Temporary[1] );
	Polish_Continue_xyz[2] = -( Send_XYZ[2] - Pos_Temporary[2] );
	}
	
	Polish_Continue_Length = sqrt( pow( Polish_Continue_xyz[0] ,2)
		+ pow( Polish_Continue_xyz[1] ,2) + pow( Polish_Continue_xyz[2] ,2) );
	Polish_Continue_d = ceil ( Polish_Continue_Length / 0.4 ); //�����ߵĵ�ĸ���
	
	Polish_Continue_D[0] = Polish_Continue_xyz[0] / Polish_Continue_d ;
	Polish_Continue_D[1] = Polish_Continue_xyz[1] / Polish_Continue_d ;
	Polish_Continue_D[2] = Polish_Continue_xyz[2] / Polish_Continue_d ;
	

	SwitchPolishCase = 3;  // �����˼����˶�ͨ��
	
}

void CSocketClientDlg::OnSwitchPolish_Start() // ��������ͷ��ʼ�˶�
{
	// TODO: Add your control notification handler code here

	save_stop_i = 0 ; //�ȴ�ָ��ֻ����һ������
	Polish_Hoist_i = 0;

	//�����˶���ܶ������Ҫ��ʼ��
	True_Pos[0] = ppoint[0].x ; 
	True_Pos[1] = ppoint[0].y ; 
	True_Pos[2] = ppoint[0].z ; 
	Send_XYZ[0] = ppoint[0].x ;
	Send_XYZ[1] = ppoint[0].y ;
	Send_XYZ[2] = ppoint[0].z ;	
	
	Send_X1_view.Format("%f" , True_Pos[0]);
	SetDlgItemText(IDC_Send_X1,Send_X1_view);
	Send_Y1_view.Format("%f" , True_Pos[1]);
	SetDlgItemText(IDC_Send_Y1,Send_Y1_view);
	Send_Z1_view.Format("%f" , True_Pos[2]);
	SetDlgItemText(IDC_Send_Z1,Send_Z1_view);

	ipoint = 0 ; itime = 0 ; dtime = 0 ;

	Polish_Advance_i = 0 ; // �����˶�������ӹ�Ԥ��
	
	// ��ʼ���жϵ��ֵ,CHECK��һ���ۼӱ���
	Check_Pos[0] = ppoint[0].x ; 
	Check_Pos[1] = ppoint[0].y ; 
	Check_Pos[2] = ppoint[0].z ; 

	Polish_Start_i = 0; //��ͷ�˶���һ�ε�ǰ�㵽��ʼ��ľ��룬ͨ��ֱ�߲岹�ߵ���ʼ�㣬ÿ��ִ���궼��Ҫ���¼���

	Pos_Temporary[0] = ppoint[0].x + 10*ppoint[0].a;
	Pos_Temporary[1] = ppoint[0].y + 10*ppoint[0].b;
	Pos_Temporary[2] = ppoint[0].z + 10*ppoint[0].c + 100;
	
	if(	Debug_switch == 0 )
	{
		Polish_Start_xyz[0] = -( Pos_Realtime[0] - Pos_Temporary[0] );
		Polish_Start_xyz[1] = -( Pos_Realtime[1] - Pos_Temporary[1] );
		Polish_Start_xyz[2] = -( Pos_Realtime[2] - Pos_Temporary[2] ); 
	}
		// �����ã��ڵ���ʱ��Pos_Realtime��ֵ��ΪSend_XYZ
	if(	Debug_switch == 1 )
	{
	Polish_Start_xyz[0] = -( Send_XYZ[0] - Pos_Temporary[0] );
	Polish_Start_xyz[1] = -( Send_XYZ[1] - Pos_Temporary[1] );
	Polish_Start_xyz[2] = -( Send_XYZ[2] - Pos_Temporary[2] );	
	}


	Polish_Start_Length = sqrt( pow( Polish_Start_xyz[0] ,2)
		+ pow( Polish_Start_xyz[1] ,2) + pow( Polish_Start_xyz[2] ,2) );
	Polish_Start_d = ceil ( Polish_Start_Length / 0.4 ); //�����ߵĵ�ĸ���

	Polish_Start_D[0] = Polish_Start_xyz[0] / Polish_Start_d ;
	Polish_Start_D[1] = Polish_Start_xyz[1] / Polish_Start_d ;
	Polish_Start_D[2] = Polish_Start_xyz[2] / Polish_Start_d ;
	
	SwitchPolishCase = 4;  // ��������ͷ��ʼ�˶�ͨ��

}

void CSocketClientDlg::OnSwitchPolish_Hoist() // �����˻�������
{
	// TODO: Add your control notification handler code here
	
	save_stop_i = 0 ; //�ȴ�ָ��ֻ����һ������

	Polish_Hoist_i = 0;	
	SwitchPolishCase = 5;
	
}

void CSocketClientDlg::OnSwitchPolish_Reset()  // �����˸�λ������û���������͹�������,Ϊ��֤�������У��������������ȫ�����룬�ݲ�ʹ��
{
	// TODO: Add your control notification handler code here

	Polish_Reset_i = 0; 

	Axis_workpiece_robot[0] = 800; Axis_workpiece_robot[1] = 0.0; Axis_workpiece_robot[2] = 1200;  //��������Ҫ�ֶ���ȡ	
	
	Polish_Reset_xyz[0] = -( Pos_Realtime[0] - Axis_workpiece_robot[0] );
	Polish_Reset_xyz[1] = -( Pos_Realtime[1] - Axis_workpiece_robot[1] );
	Polish_Reset_xyz[2] = -( Pos_Realtime[2] - Axis_workpiece_robot[2] );
	
	Polish_Reset_Length = sqrt( pow( Polish_Reset_xyz[0] ,2)
		+ pow( Polish_Reset_xyz[1] ,2) + pow( Polish_Reset_xyz[2] ,2) );
	Polish_Reset_d = ceil ( Polish_Reset_Length / 0.4 ); //�����ߵĵ�ĸ���
	
	Polish_Reset_D[0] = Polish_Reset_xyz[0] / Polish_Reset_d ;
	Polish_Reset_D[1] = Polish_Reset_xyz[1] / Polish_Reset_d ;
	Polish_Reset_D[2] = Polish_Reset_xyz[2] / Polish_Reset_d ;
	
	SwitchPolishCase = 6;
	
}

void CSocketClientDlg::OnSwitchPolish_Stop()  // ������ֹͣ�˶�
{
	// TODO: Add your control notification handler code here
	SwitchPolishCase = 7;



}


void CSocketClientDlg::OnSwitchPolish_Teach()   //ʾ��
{
	// TODO: Add your control notification handler code here
	
	save_stop_i = 0 ; //�ȴ�ָ��ֻ����һ������
	Polish_Hoist_i = 0;

	CString strMS_speed_medium; //��ȡʾ���ٶ�
	GetDlgItem(IDC_MoveSafe_speed_medium)->GetWindowText(strMS_speed_medium);
	MS_speed_medium = atof(strMS_speed_medium);//atof �ַ���תdouble
	
	SwitchPolishCase = 8;	
}


void CSocketClientDlg::OnSwitchPolish_Fast() //���ٻ���
{
	// TODO: Add your control notification handler code here

	save_stop_i = 0 ; //�ȴ�ָ��ֻ����һ������
		Polish_Hoist_i = 0;

	Polish_Fast_i = 0 ;
	Polish_Fast_j = 0 ;

	if(	Debug_switch == 0 )
	{
		Polish_Fast_xyz[0] = -( Pos_Realtime[0] - POS_Trans_X );
		Polish_Fast_xyz[1] = -( Pos_Realtime[1] - POS_Trans_Y );
		Polish_Fast_xyz[2] = -( Pos_Realtime[2] - POS_Trans_Z );
	}
	
	// �����ã�Pos_Realtime��ֵ��ΪSend_XYZ
	if(	Debug_switch == 1 )
	{
		Polish_Fast_xyz[0] = -( Send_XYZ[0] - POS_Trans_X );
		Polish_Fast_xyz[1] = -( Send_XYZ[1] - POS_Trans_Y );
		Polish_Fast_xyz[2] = -( Send_XYZ[2] - POS_Trans_Z );	
	}

	Polish_Fast_Length = sqrt( pow( Polish_Fast_xyz[0] ,2)
		+ pow( Polish_Fast_xyz[1] ,2) );
	Polish_Fast_d = ceil ( Polish_Fast_Length / 0.4 ); //�����ߵĵ�ĸ���
	Polish_Fast_down_d = ceil ( abs( -100 + Polish_Fast_xyz[2] ) / 0.2 ); // �½������ߵĵ�ĸ���
	
	Polish_Fast_D[0] = Polish_Fast_xyz[0] / Polish_Fast_d ;
	Polish_Fast_D[1] = Polish_Fast_xyz[1] / Polish_Fast_d ;
	Polish_Fast_D[2] = 0.2 ;
	Polish_Fast_D[3] = ( -100 + Polish_Fast_xyz[2] ) / Polish_Fast_down_d ;


	SwitchPolishCase = 9;	
}


void CSocketClientDlg::OnTeach_X_Add() 
{
	// TODO: Add your control notification handler code here

	Polish_Hoist_i = 0;


	GetDlgItem( IDC_Teach_d )->GetWindowText(strTeach_Length);
	Teach_Length = atof(strTeach_Length);//atof �ַ���תdouble

	Teach_i = 1;
	Teach_d = ceil( Teach_Length / MS_speed_medium );
	Teach_L = Teach_Length / Teach_d ;

	Teach_Pos[3] = Teach_L ; Teach_Pos[4] = 0 ; Teach_Pos[2] = 0 ;

	Teach_Pos[0] = Teach_Pos[3] * cos(POS_Trans_A*PI/180) - Teach_Pos[4] *sin(POS_Trans_A*PI/180) ;
	Teach_Pos[1] = Teach_Pos[3] * sin(POS_Trans_A*PI/180) + Teach_Pos[4] *cos(POS_Trans_A*PI/180) ;
	
}

void CSocketClientDlg::OnTeach_Y_Add() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Teach_d )->GetWindowText(strTeach_Length);
	Teach_Length = atof(strTeach_Length);//atof �ַ���תdouble

	Polish_Hoist_i = 0;

	Teach_i = 1;
	Teach_d = ceil( Teach_Length / MS_speed_medium );
	Teach_L = Teach_Length / Teach_d ;
	
	Teach_Pos[3] = 0 ; Teach_Pos[4] = Teach_L ; Teach_Pos[2] = 0 ;
	
	Teach_Pos[0] = Teach_Pos[3] * cos(POS_Trans_A*PI/180) - Teach_Pos[4] *sin(POS_Trans_A*PI/180) ;
	Teach_Pos[1] = Teach_Pos[3] * sin(POS_Trans_A*PI/180) + Teach_Pos[4] *cos(POS_Trans_A*PI/180) ;
	
}

void CSocketClientDlg::OnTeach_Z_Add() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Teach_d )->GetWindowText(strTeach_Length);
	Teach_Length = atof(strTeach_Length);//atof �ַ���תdouble

	Polish_Hoist_i = 0;

	Teach_i = 1;
	Teach_d = ceil( Teach_Length / MS_speed_medium );
	Teach_L = Teach_Length / Teach_d ;
	
	Teach_Pos[0] = 0 ; Teach_Pos[1] = 0 ; Teach_Pos[2] = Teach_L ;
	
}

void CSocketClientDlg::OnTeach_X_Sub() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Teach_d )->GetWindowText(strTeach_Length);
	Teach_Length = atof(strTeach_Length);//atof �ַ���תdouble

	Polish_Hoist_i = 0;

	Teach_i = 1;
	Teach_d = ceil( Teach_Length / MS_speed_medium );
	Teach_L = Teach_Length / Teach_d ;
	
	Teach_Pos[3] = -Teach_L ; Teach_Pos[4] = 0 ; Teach_Pos[2] = 0 ;
	
	Teach_Pos[0] = Teach_Pos[3] * cos(POS_Trans_A*PI/180) - Teach_Pos[4] *sin(POS_Trans_A*PI/180) ;
	Teach_Pos[1] = Teach_Pos[3] * sin(POS_Trans_A*PI/180) + Teach_Pos[4] *cos(POS_Trans_A*PI/180) ;


}

void CSocketClientDlg::OnTeach_Y_Sub() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Teach_d )->GetWindowText(strTeach_Length);
	Teach_Length = atof(strTeach_Length);//atof �ַ���תdouble

	Polish_Hoist_i = 0;

	Teach_i = 1;
	Teach_d = ceil( Teach_Length / MS_speed_medium );
	Teach_L = Teach_Length / Teach_d ;
	
	Teach_Pos[3] = 0 ; Teach_Pos[4] = -Teach_L ; Teach_Pos[2] = 0 ;
	
	Teach_Pos[0] = Teach_Pos[3] * cos(POS_Trans_A*PI/180) - Teach_Pos[4] *sin(POS_Trans_A*PI/180) ;
	Teach_Pos[1] = Teach_Pos[3] * sin(POS_Trans_A*PI/180) + Teach_Pos[4] *cos(POS_Trans_A*PI/180) ;


}

void CSocketClientDlg::OnTeach_Z_Sub() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Teach_d )->GetWindowText(strTeach_Length);
	Teach_Length = atof(strTeach_Length);//atof �ַ���תdouble

	Polish_Hoist_i = 0;

	Teach_i = 1;
	Teach_d = ceil( Teach_Length / MS_speed_medium );
	Teach_L = Teach_Length / Teach_d ;
	
	Teach_Pos[0] = 0 ; Teach_Pos[1] = 0 ; Teach_Pos[2] = -Teach_L ;

}


void CSocketClientDlg::OnButton2() 
{

	// TODO: Add your control notification handler code here

	ShellExecute(NULL,"explore", "F:\\KUKA�׹�ʵ������", NULL, NULL, SW_SHOW);	//��Windows��Դ������
}


void CSocketClientDlg::OnSaveData() 
{
	// TODO: Add your control notification handler code here
	
	/*   FILE *fpWrite=fopen("F:\\KUKA�׹�ʵ��������λ���շ�����.txt","w");  
	
	  for(int i=0;i<10;i++)  
	  fprintf(fpWrite,"%d ",i);  
    fclose(fpWrite);  */
	
	char filename[2000];
	sprintf(filename,"F:\\KUKA�׹�ʵ������\\%s-��λ���շ�����.txt",SaveData_Name);
	ofstream dataFile;
	dataFile.open(filename, ofstream::trunc);
	
	dataFile<<"SwitchPolishCase 1 �����׹� 2 �ȴ��ź� 3 �����׹� 4 ��ͷ�׹� 5 �����ᵶ 6 ��������\n"<<
		"SwitchPolishCase 7 ͨ��ֹͣ 8 ʾ�̲��� 9 һ������  10 Ԥ�ӹ� 11 �ǶȽ��� 12 ������ \n"<<
		"��i���� �׹�ָ�� (���͵���������)   (���յĻ����˷������ꡪ��ʵ��λ��)   (��ʸ��λ�Ʋ�����������������ֵ)<<ʵʱ���� "<< endl;
	
	for (int save_i = 1 ; save_i<=Socket_Client_j ;save_i++ )
	{

		dataFile<<setw(6)<<save_i<<" "<<"�׹�ָ��:"<<setw(2)<<SaveData_Socket[save_i][0]<<" ("
			<<setiosflags(ios::fixed)<<setprecision(6)
			<<setw(12)<<SaveData_Socket[save_i][1]<<","
			<<setw(12)<<SaveData_Socket[save_i][2]<<","
			<<setw(12)<<SaveData_Socket[save_i][3]<<")"<<" ("
			<<setw(12)<<SaveData_Socket[save_i][4]<<","
			<<setw(12)<<SaveData_Socket[save_i][5]<<","
			<<setw(12)<<SaveData_Socket[save_i][6]<<")"<<" ("
			<<setw(12)<<SaveData_Socket[save_i][7]<<","
			<<setw(12)<<SaveData_Socket[save_i][8]<<","
			<<setw(12)<<SaveData_Socket[save_i][9]<<")"<<"("
			<<setw(12)<<SaveData_Socket[save_i][10]<<")"
			<< endl;
		
	}
	
	dataFile.close();
	
}


void CSocketClientDlg::OnTransPOS() 
{
	// TODO: Add your control notification handler code here

//����궨����
double Calibration_Point[5][3] = {
	/*{ -128.79 , -1102.81 , 576 },
	{ 97.70  , -1102.22 , 576 },
	{ 168.08  , -1317.62 , 576 },
	{ -25.10 , -1451.11 , 576 },
	{ -198.21 , -1317.89 , 576 } };*/ 

	{ -122.18  , -1248.47 , 762.68 },
	{  -62.24 , -1246.43 , 762.68 },
	{  0 , 0 , 0 },
	{ 0 , 0 , 0 },
	{ 0 , 0 , 0 } }; 

	POS_Trans_X = -122.18;
	POS_Trans_Y = -1248.47;
	POS_Trans_Z = 762.68;

	POS_Trans_A = 1.96;


	/*POS_Trans_X = ( Calibration_Point[0][0] + Calibration_Point[1][0] + 
		Calibration_Point[2][0] + Calibration_Point[3][0] + Calibration_Point[4][0] ) / 5 ;
	POS_Trans_Y = ( Calibration_Point[0][1] + Calibration_Point[1][1] + 
		Calibration_Point[2][1] + Calibration_Point[3][1] + Calibration_Point[4][1] ) / 5 ;
	POS_Trans_Z = ( Calibration_Point[0][2] + Calibration_Point[1][2] + 
		Calibration_Point[2][2] + Calibration_Point[3][2] + Calibration_Point[4][2] ) / 5 ;
	POS_Trans_A = 90-( atan ( ( (Calibration_Point[0][0] + Calibration_Point[1][0] ) / 2 - Calibration_Point[3][0] ) / 
		( (Calibration_Point[0][1] + Calibration_Point[1][1] ) /2 - Calibration_Point[3][1] ) ) ) / PI * 180;*/
	// GetDlgItem(IDC_POS_Trans_X)->SetWindowText("-21.885");   //  -21.885
	// GetDlgItem(IDC_POS_Trans_Y)->SetWindowText("-1284.796");       //  -1284.796
	// GetDlgItem(IDC_POS_Trans_Z)->SetWindowText("540.122");     //  540.122
	// GetDlgItem(IDC_POS_Trans_A)->SetWindowText("85.561");        //   85.5611627 ��ʱ����ת 4.4388373
	
	GetDlgItem(IDC_POS_Trans_H)->GetWindowText(strPOS_Trans_H);
	POS_Trans_H = atof(strPOS_Trans_H);//atof �ַ���תdouble	
	
	POS_Trans_Z += POS_Trans_H ;

	strPOS_Trans_X.Format("%.4f" , POS_Trans_X);
	strPOS_Trans_Y.Format("%.4f" , POS_Trans_Y);
	strPOS_Trans_Z.Format("%.4f" , POS_Trans_Z);
	strPOS_Trans_A.Format("%.4f" , POS_Trans_A);

	GetDlgItem(IDC_POS_Trans_X)->SetWindowText(strPOS_Trans_X);
	GetDlgItem(IDC_POS_Trans_Y)->SetWindowText(strPOS_Trans_Y);
	GetDlgItem(IDC_POS_Trans_Z)->SetWindowText(strPOS_Trans_Z);
	GetDlgItem(IDC_POS_Trans_A)->SetWindowText(strPOS_Trans_A);
	
}

void CSocketClientDlg::OnTrans_ANGLE() 
{
	// TODO: Add your control notification handler code here
	//  �Ƕȱ任 
	GetDlgItem(IDC_POS_Trans_A)->GetWindowText(strPOS_Trans_A);
	POS_Trans_A = atof(strPOS_Trans_A);//atof �ַ���תdouble
	POS_Trans_A += -72 ; //�������ӵ����5�ȷֶԳ�ͼ�� 360/5
	char CHAR_POS_Trans_A[20];

	sprintf(CHAR_POS_Trans_A,"%.4f",POS_Trans_A);
	SetDlgItemText(IDC_POS_Trans_A,CHAR_POS_Trans_A);
//	GetDlgItem(IDC_POS_Trans_A)->SetWindowText(strPOS_Trans_A); 
	
}

void CSocketClientDlg::OnBTNGraph() 
{
	// TODO: Add your control notification handler code here
// 	initgraph(640, 480);	// ����� TC ��������
// 	circle(100, 100, 60);	// ��Բ��Բ��(100, 100)���뾶 60
// 	getch();				// �����������
// 	closegraph();			// �ر�ͼ�ν���
	
}

void CSocketClientDlg::OnButton15() 
{
	// TODO: Add your control notification handler code here
	
	save_stop_i = 0 ; //�ȴ�ָ��ֻ����һ������
	SwitchPolishCase = 1;  // �����˼����˶�ͨ��
	
}

void CSocketClientDlg::OnCeshi() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_Send_View)->GetWindowText(strCase_force);
	Case_force = atof(strCase_force);//atof �ַ���תdouble

	
}

void CSocketClientDlg::OnDebugging() 
{
	// TODO: Add your control notification handler code here
	// Debug_switch = 1;
	// GetDlgItem(IDC_ED_IP)->SetWindowText("192.168.1.108");
	GetDlgItem(IDC_ED_PORT)->SetWindowText("1234");
	
}

void CSocketClientDlg::OnSwitchPolishCorrect() 
{
	// TODO: Add your control notification handler code here
	SwitchPolishCase = 11 ;  // ���������˽Ƕ����
	
}

void CSocketClientDlg::OnSwitchPolishText() 
{
	// TODO: Add your control notification handler code here
	/*	Polish_Text_i = 0 ;
	SwitchPolishCase = 6 ;  // ����������*/

	// ��ȡ�ͷ��׹��ĸ���	
	GetDlgItem(IDC_Release_point)->GetWindowText( strRelease_point );
	Release_point = atof( strRelease_point );//atof �ַ���תdouble
	Release_point = Release_point - 20 ;

	while(1)
	{
		itime ++;//������whileѭ��һ�μ�һ    itime ��ʱΪ1
		if (itime<= alldtime )
		{	
			
			//dtime�ֶ��ܺ�			
			if (itime>dtime)   // dtime ��ʱΪ0������Ϊ�棬���� ipontֱ��ָ���һ����������
			{
				
				ipoint ++;
				dtime += ppoint[ipoint].dd;
				
				
				True_Pos[0] += ppoint[ipoint].x ; 
				True_Pos[1] += ppoint[ipoint].y ; 
				True_Pos[2] += ppoint[ipoint].z ;
				

			}
			
			
		}

		m_progress.SetPos(ipoint); // ����������
		
		Progress_View.Format("%d/%d" ,itime,alldtime);
		SetDlgItemText(IDC_Progress_View,Progress_View);

		if ( itime > Release_point )
		{
			True_Pos[2] -= 5 ;
			break ;
		}
		
		
		
	}
}

void CSocketClientDlg::OnBtnExit3() 
{
	// TODO: Add your control notification handler code here

		save_stop_i = 0 ; //�ȴ�ָ��ֻ����һ������
		Polish_Hoist_i = 0;


	SwitchPolishCase = 13;  // �����˼����˶�ͨ��

	Polish_TEXT_i = 0;


	TEXT_POS[0] = 0.01 ;
	TEXT_POS[1] = 0 ;
	TEXT_POS[2] = 0 ;

	
}

void CSocketClientDlg::OnBtnExit10() 
{
	// TODO: Add your control notification handler code here

	
		save_stop_i = 0 ; //�ȴ�ָ��ֻ����һ������
		Polish_Hoist_i = 0;


	SwitchPolishCase = 14;  // �����˼����˶�ͨ��

	Polish_TEXT_i = 0;


	TEXT_POS[0] = 0.01 ;
	TEXT_POS[1] = 0 ;
	TEXT_POS[2] = 0 ;
	
}
