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

//#include <graphics.h>		// 这样引用 EasyX 图形库
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
//全局变量，力信号
double Force_result = 0,Force_x,Force_y,Force_z;
double Pos[3];
extern double Force_result;//抛光力
double DesiredForce;//期望抛光力
double DeltaPosition = 0;
double DeltaPosition_2 = 0;//△
double myDelta = 0;
int couter[4];//控制算法用
double K_env=60;
double gKi=1/100;
double n_Delta_Postion=0;
int PointNumMax;
PIParameter Force_Parameter;

int alldtime=0;
int itime=0;//计算次数i
int dtime=0;//计算次数
int ipoint=0; //第ipoint个抛光点；

char name[256] = {0};

double dx,dy,dz,da,db,dc=0;


int Forceproc_Flag=1;//控制算法开关
double EndToolCoordRelRobot[3];
double nvctRelRobot[3];
//int P[MAXNUM][6]={0};
struct PolishingPoint ppoint[MAXNUM]={0};

//PIParameter Force_Parameter;
double gEndToolCoordRelRobot[6]={0.0};//终点工具坐标系相对{B}位姿

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
//  ||                                               变量初始化，不需要监听                                               ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



BOOL CSocketClientDlg::OnInitDialog()//初始化，不需要监听
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
	//获取本机IP代码
	char* localIP;
	CString strIP;
	char hostname[MAX_PATH] = {0};
	gethostname(hostname,MAX_PATH);
	hostent* ptent = gethostbyname(hostname);
	if(ptent)
	{
		localIP = inet_ntoa(*(in_addr*)ptent->h_addr_list[0]);
	}
	GetDlgItem(IDC_ED_IP)->SetWindowText(localIP);//默认值
	GetDlgItem(IDC_ED_IP)->EnableWindow(false);//不能编辑


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
	MS_speed_min = atof(strMS_speed_min);//atof 字符串转double

	GetDlgItem(IDC_Move_limit_Box)->SetWindowText("5");
	CString strMS_limit_Box;
	GetDlgItem(IDC_Move_limit_Box)->GetWindowText(strMS_limit_Box);
	MS_limit_Box = atof(strMS_limit_Box);//atof 字符串转double

	GetDlgItem(IDC_MoveSafe_speed_max)->SetWindowText("0.8");
	CString strMS_speed_max;
	GetDlgItem(IDC_MoveSafe_speed_max)->GetWindowText(strMS_speed_max);
	MS_speed_max = atof(strMS_speed_max);//atof 字符串转double

	GetDlgItem(IDC_MoveSafe_speed_medium)->SetWindowText("0.4");
	CString strMS_speed_medium;
	GetDlgItem(IDC_MoveSafe_speed_medium)->GetWindowText(strMS_speed_medium);
	MS_speed_medium = atof(strMS_speed_medium);//atof 字符串转double


    SYSTEMTIME sys; 
    GetLocalTime( &sys ); 
    // printf( "%4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d\n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek); 

    SaveData_Name.Format( "%4d-%02d-%02d-%02d-%02d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute ); 
	SetDlgItemText(IDC_SAVEDATA_NAME,SaveData_Name);	
//	GetDlgItem(IDC_SAVEDATA_NAME)->SetWindowText("2020-12-21-14-30");
	
// 	GetDlgItem(IDC_POS_Trans_X)->SetWindowText("-21.885");   //  -21.885
// 	GetDlgItem(IDC_POS_Trans_Y)->SetWindowText("-1284.796");       //  -1284.796
// 	GetDlgItem(IDC_POS_Trans_Z)->SetWindowText("540.122");     //  540.122
// 	GetDlgItem(IDC_POS_Trans_A)->SetWindowText("-85.561");        //   -85.5611627 逆时针旋转 4.4388373

	GetDlgItem(IDC_POS_Trans_X)->SetWindowText("0");   //  -21.885
	GetDlgItem(IDC_POS_Trans_Y)->SetWindowText("0");       //  -1284.796
	GetDlgItem(IDC_POS_Trans_Z)->SetWindowText("0");     //  540.122
	GetDlgItem(IDC_POS_Trans_A)->SetWindowText("0");        //   -85.5611627 逆时针旋转 4.4388373
	GetDlgItem(IDC_POS_Trans_H)->SetWindowText("0");     //  540.122
	GetDlgItem(IDC_Send_View)->SetWindowText("10");  
	
	GetDlgItem(IDC_ED_LabViewPORT)->SetWindowText("52233");   //labview端口
	GetDlgItem(IDC_ED_TEXT)->SetWindowText("4");   //labview端口


	
	GetDlgItem(IDC_POS_Trans_X)->GetWindowText(strPOS_Trans_X);
	POS_Trans_X = atof(strPOS_Trans_X);//atof 字符串转double
	GetDlgItem(IDC_POS_Trans_Y)->GetWindowText(strPOS_Trans_Y);
	POS_Trans_Y = atof(strPOS_Trans_Y);//atof 字符串转double
	GetDlgItem(IDC_POS_Trans_Z)->GetWindowText(strPOS_Trans_Z);
	POS_Trans_Z = atof(strPOS_Trans_Z);//atof 字符串转double
	GetDlgItem(IDC_POS_Trans_A)->GetWindowText(strPOS_Trans_A);
	POS_Trans_A = atof(strPOS_Trans_A);//atof 字符串转double

	Connect_view.Format("未监听");
	SetDlgItemText(IDC_Connect_view,Connect_view);


	m_PolishingDate.AddString( _T("KUKA机器人抛光系统使用说明——按使用先后顺序") );//显示在屏幕上
	m_PolishingDate.AddString( _T("步骤【1】在导入抛光点前需设置高度差补H，然后再坐标变换（未标定情况下）") );
	m_PolishingDate.AddString( _T("步骤【2】设置安全运行速度，最小速度决定抛光点的步长，最大速度保证安全") );//显示在屏幕上
	m_PolishingDate.AddString( _T("步骤【3】导入增量抛光点，如果导入错误文件，可以清空数据后再次导入") );//显示在屏幕上
	m_PolishingDate.AddString( _T("步骤【4】点击KUKA的监听按钮，没有点击会显示未监听，点击后会显示已监听") );//显示在屏幕上
	m_PolishingDate.AddString( _T("步骤【5】与机器人成功交换数据时，显示框会显示等待指令发送") );//显示在屏幕上
	m_PolishingDate.AddString( _T("步骤【6】点击开始按钮，即可开始抛光，暂停、继续按钮不会打断期望轨迹") );//显示在屏幕上
	m_PolishingDate.AddString( _T("步骤【7】提刀或换刀后，继续抛光需点继续抛光按钮，重头抛光需点开始按钮") );//显示在屏幕上
	m_PolishingDate.AddString( _T("步骤【8】示教需点击示教按钮，输入运行增量，再出点击标加减按钮") );//显示在屏幕上
	m_PolishingDate.AddString( _T("步骤【9】保存数据时先创建文件夹，修改日期，保存数据后再打开文件") );//显示在屏幕上
	m_PolishingDate.AddString( _T("提示【1】SwitchPolishCase 1 正常抛光 2 等待信号 3 继续抛光") );//显示在屏幕上
	m_PolishingDate.AddString( _T("提示【2】SwitchPolishCase 4 重头抛光 5 机器提刀 6 补偿测试") );//显示在屏幕上
	m_PolishingDate.AddString( _T("提示【3】SwitchPolishCase 7 通信停止 8 示教操作 9 一键换刀") );//显示在屏幕上
	m_PolishingDate.AddString( _T("提示【4】SwitchPolishCase 10 预加工 11 角度矫正 12 力补偿") );//显示在屏幕上
	m_PolishingDate.AddString( _T("步骤【1】") );
	m_PolishingDate.AddString( _T("步骤【2】") );
	m_PolishingDate.AddString( _T("步骤【3】") );
	m_PolishingDate.AddString( _T("步骤【4】") );
	m_PolishingDate.AddString( _T("步骤【5】") );
	m_PolishingDate.AddString( _T("步骤【6】") );
	m_PolishingDate.AddString( _T("步骤【7】") );
	m_PolishingDate.AddString( _T("步骤【8】") );
	m_PolishingDate.AddString( _T("步骤【9】") );											 
	m_PolishingDate.AddString( _T("提示【1】") );
	m_PolishingDate.AddString( _T("提示【2】") );
	m_PolishingDate.AddString( _T("提示【3】") );	
	m_PolishingDate.AddString( _T("提示【4】") );		
											 
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
                  bmpBackground.LoadBitmap(IDB_BITMAP1);  //此处的IDB_SHOUYE是你的bitmap，即要插入的图片
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


char *DeleteSpace(char *str)   //去除字符串空格函数
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

int myDelay(double timeDelay)//延时函数
{

	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq=(double)litmp.QuadPart;//获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1=litmp.QuadPart;//获得初始值
	do 
	{
		QueryPerformanceCounter(&litmp);
		QPart2=litmp.QuadPart;//获得终止值
		dfMinus=(double)(QPart2-QPart1);
		dfTim=dfMinus/dfFreq;//获得对应的时间值，单位为秒
	} while (dfTim*1000<timeDelay);
	
	return 0;
}



//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                           控制算法，形参为期望力和实际接触力  ，返回值为DeltaPosition                              ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

double Control_Method(const double DesiredForce, double force, double n_Delta_Postion)
{

	double Kp=0.02*1*0.5, Ki=0.02*0.5;
	double fd=10;
	int i=0;
	
	if (force > 30)//F>40N时，提刀
	{
		//SwitchPolishCase = 5;
		DeltaPosition=-0.1;

	}
	else if (force < 1.5)//此时未接触工件表面，每次进给0.01mm慢慢接触工件表面
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

	/*if(OnBackStepFlag == true)//如果按下后退，机器人末端远离工件表面
	{
		OnBackStepFlag = false;
		DeltaPosition-=5;
	}*/

	return DeltaPosition;//返回值DeltaPosition作为全局变量被主程序使用

}


//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                        UDP服务器监听线程                                                           ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DWORD WINAPI threadproServer(LPVOID pParam)//UDP服务器监听线程
{
	
	CSocketClientDlg *pDlg=(CSocketClientDlg*)pParam;
	char cWelcomBuffer[]="Welcome to you\0";
	int iWhileListenCount=10;//最大连接数
	CString strTmp;
	int num;   
	//开始接收消息
	sockaddr_in addrFrom;//接收连接地址(KUKA客户端地址信息)
	int len=sizeof(addrFrom);
	
	char eachBuf[50*1024];//定义接收数据缓冲区
	memset(eachBuf,0,50*1024);//memset()用来初始化字符数组，初始化的值只能为0或者-1

	CString sendstr,sendstr1,sendstr2,sendstr3,sendstr4,sendstr5,sendstr6,sendstr7,sendstr8,sendstr9,sendstr10 ;

//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     第一步       接收XML文件                                                       ||
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
				receive[j]=eachBuf[j];//StringBuf1字符数组在下面进行解析
			}
	
			
//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     第二步       解析XML文件                                                       ||
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
			CString Cstr(p);//char转化为CString,赋值给CStr（存储读取的处理后的字符串信息）;
			
			sx=Cstr.Mid(arry[2]+1,arry[3]-arry[2]-1);//Mid函数截取一段字符串，第一个参数表示截取起点，第二个参数表示截取长度；
			Pos[0]=atof(sx);                     //求得坐标X;
			sy=Cstr.Mid(arry[4]+1,arry[5]-arry[4]-1);
			Pos[1]=atof(sy);                      //求得坐标Y;
			sz=Cstr.Mid(arry[6]+1,arry[7]-arry[6]-1);//Mid()第一个参数加1，不然读到的是“;第2个参数-1，不然会多读一位;
			Pos[2]=atof(sz);  
			//求得坐标Z;
			nipoc1=s1.find("IPOC",arry[20]);
			nipoc2=s1.find("IPOC",nipoc1+1);
			sipoc=Cstr.Mid(nipoc1+5,nipoc2-nipoc1-7);    //提取IPOC标签信息;
			


//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     第三步       安全检查                                                          ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Pos_Realtime[0] = Pos[0] ; Pos_Realtime[1] = Pos[1] ; Pos_Realtime[2] = Pos[2] ;
			// 代码检查，如果机器人反馈的坐标超出工件的安全范围，服务器直接关闭
			

			if( ( SwitchPolishCase != 5 && Debug_switch == 0 ) && (Pos_Realtime[0] > LIN_x_max || Pos_Realtime[0] < LIN_x_min || Pos_Realtime[1] > LIN_y_max || 
				Pos_Realtime[1] < LIN_y_min || Pos_Realtime[2] > LIN_z_max || Pos_Realtime[2] < LIN_z_min) ) 
			{

				Polish_Hoist_i = 3;	
				SwitchPolishCase = 5 ;
				Warning_view.Format(" 机器人反馈的坐标超限" );
				pDlg->SetDlgItemText(IDC_Warning_view,Warning_view);

			}
			if ( SwitchPolishCase != 2 && Force_result > 30 )
			{
				Polish_Hoist_i = 3;	
				SwitchPolishCase = 5 ;
				Warning_view.Format(" 机器人抛光力超限" );
				pDlg->SetDlgItemText(IDC_Warning_view,Warning_view);
			
			}

			
//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     第四步       力控制算法                                                        ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
									
		    DeltaPosition_2=Control_Method(DesiredForce,Force_result,n_Delta_Postion);
			//DeltaPosition_2=0.000; //关闭力控制算法后需要调试才需屏蔽上方代码

			// 测试用 mfc界面上有个测试按钮，左边编辑框的值即为Case_force
//			Force_result = Case_force;


			
//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     第五步       抛光指令选择                                                      ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


			// 抛光指令选择
			switch (SwitchPolishCase)
			{
			case 12: //不改变位置ipoint进行力补偿,只在正常加工时才会被调用
			
				Polish_view.Format("PolishCase%d力补偿" ,SwitchPolishCase);
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
			
			case 11: // 角度矫正

				Polish_view.Format("PolishCase%d角度矫正测试" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				
				break;

			case 10: // 加工预进
				
				Polish_view.Format("PolishCase%d加工预进" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				

				Polish_Advance_i ++ ; //初始值为0						
				if (Polish_Advance_i <= 500) //加工预进的高度为10厘米 小于500
				{
					
					Send_Pos[0] = 0.00 ; Send_Pos[1] = 0.00 ; Send_Pos[2] = -0.2 ;
					
				}
				else if ( Polish_Advance_i > 500 && Polish_Advance_i <= 1000) //留1厘米打开力补偿，逼近工件表面
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
			case 1: //正常抛光
				Polish_view.Format("PolishCase%d正常抛光" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				itime ++;//计数，while循环一次加一    itime 此时为1
				if (itime<= alldtime )
				{	
					
					//dtime分段总和			
					if (itime>dtime)   // dtime 此时为0，条件为真，所以 ipont直接指向第一个增量坐标
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
					SwitchPolishCase = 5 ; //抛光完成后提刀
				}
	

				break;

			case 2: // 暂停抛光 ，不是真的暂停，而是一直给上位机发送坐标（0，0，0）

				Polish_view.Format("PolishCase%d等待发送指令" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);


				Send_Pos[0] = 0.00 ; Send_Pos[1] = 0.00 ; Send_Pos[2] = 0.00;
				
				break;
				
			case 3:  // 继续抛光 Continue
				
				Polish_view.Format("PolishCase%d继续抛光" ,SwitchPolishCase);
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
				
			case 4: // 机器人重头开始抛光
				
				Polish_view.Format("PolishCase%d开始/重头抛光" ,SwitchPolishCase);
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

			case 5: // 机器人提刀

				Polish_view.Format("PolishCase%d向上提刀" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);

				Polish_Hoist_i ++ ; //初始值为1

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
				
			case 6: // 力补偿测试
				
				Polish_view.Format("PolishCase%d力补偿测试" ,SwitchPolishCase);
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

				//closesocket(pDlg->m_SockClient); //代码还未完善，如果误按此按钮，服务器直接关闭
				break;
				
			case 7: // 通信停止

				Polish_view.Format("PolishCase%d通信停止" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				closesocket(pDlg->m_SockClient);
				break;
				
			case 8: // 示教操作
				
				Polish_view.Format("PolishCase%d示教操作" ,SwitchPolishCase);
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

			case 9: // 一键换刀


				Polish_view.Format("PolishCase%d一键换刀" ,SwitchPolishCase);
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
					
					SwitchPolishCase = 2 ; // 暂停抛光
				}

				break;
				
			case 13: // 撞击磨损试验
				
				
				Polish_view.Format("PolishCase%d撞击磨损试验" ,SwitchPolishCase);
				pDlg->SetDlgItemText(IDC_Polish_view,Polish_view);
				
				
				
				Send_Pos[0] = 0 ; 
				Send_Pos[1] = -0.01 ; 
				Send_Pos[2] = 0 ;
	
				if ( Force_result > 30 )
				{
					
					SwitchPolishCase = 5 ;
					
				}
				
				break;


			case 14: // 磨损试验
				
				
				Polish_view.Format("PolishCase%d磨损试验" ,SwitchPolishCase);
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
//  ||                                     第六步       发送文件检查及存储                                                ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			
			
			//   代码检查，速度过快，直接关闭服务器，同样的，也是为了防止补偿量过大

			if ( pow (Send_Pos[0],2 ) + pow (Send_Pos[1],2 ) + pow (Send_Pos[2],2 ) > pow (MS_speed_max,2 ) )
			{
				closesocket(pDlg->m_SockClient);
				Warning_view.Format(" 发送点的速度过快" );
				pDlg->SetDlgItemText(IDC_Warning_view,Warning_view);

			}
			
			
			// 代码检查，如果抛光点超出工件的安全范围，提刀
			// Check_Pos[0] += EndToolCoordRelRobot[0] ; Check_Pos[1] += EndToolCoordRelRobot[1]; Check_Pos[2] += EndToolCoordRelRobot[2];
			// 已经在CASE1条件下进行了赋值
			
			if( ( SwitchPolishCase != 5 && Debug_switch == 0 ) && ( Check_Pos[0] > LIN_x_max || Check_Pos[0] < LIN_x_min || Check_Pos[1] > LIN_y_max || 
				Check_Pos[1] < LIN_y_min || Check_Pos[2] > LIN_z_max || Check_Pos[2] < LIN_z_min ) )
			{
				Send_Pos[0] = 0.00 ; 
				Send_Pos[1] = 0.00 ; 
				Send_Pos[2] = 0.20 ;

				Polish_Hoist_i = 0;
				SwitchPolishCase=5; //提刀
				Warning_view.Format(" 发送坐标超出范围" );
				pDlg->SetDlgItemText(IDC_Warning_view,Warning_view);
			}


//判断发送坐标位置与期望位置误差是否超出动态包容盒范围


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

// 储存数据
			if ( SwitchPolishCase != 2 ) // 实时存储所有数据除暂停命令外
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

			if ( SwitchPolishCase == 2 && save_stop_i == 0  ) //暂停时只存储一次数据
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
//  ||                                     第七步       向窗口发送数据                                                    ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			// 向机器人实时发送的增量
			Send_X_view.Format("%f" , Send_Pos[0]);
			pDlg->SetDlgItemText(IDC_Send_X,Send_X_view);
			Send_Y_view.Format("%f" , Send_Pos[1]);
			pDlg->SetDlgItemText(IDC_Send_Y,Send_Y_view);
			Send_Z_view.Format("%f" , Send_Pos[2]);
			pDlg->SetDlgItemText(IDC_Send_Z,Send_Z_view);

			// 抛光工件的绝对坐标，也为期望点
			Send_X1_view.Format("%f" , True_Pos[0]);
			pDlg->SetDlgItemText(IDC_Send_X1,Send_X1_view);
			Send_Y1_view.Format("%f" , True_Pos[1]);
			pDlg->SetDlgItemText(IDC_Send_Y1,Send_Y1_view);
			Send_Z1_view.Format("%f" , True_Pos[2]);
			pDlg->SetDlgItemText(IDC_Send_Z1,Send_Z1_view);

			
			// 向机器人发送的绝对坐标
			Send_XYZ[0] += Send_Pos[0] ;
			Send_XYZ[1] += Send_Pos[1] ;
			Send_XYZ[2] += Send_Pos[2] ;
			
// 在正常运行时需要将	Send_XYZ 改成 Pos_Realtime	

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


			pDlg->m_progress.SetPos(ipoint); // 进度条处理

			Progress_View.Format("%d/%d" ,itime,alldtime);
			pDlg->SetDlgItemText(IDC_Progress_View,Progress_View);


			

//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                     第八步       写XML文件                                                         ||
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
//  ||                                     第九步       发送XML文件                                                       ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		
			
			int sendlength=sendstr.GetLength();
			
			//1.先将sendstr字符串写入SendBUFF[50*1024]字符数组
			char SendBUFF[50*1024];  //发送数据缓冲区；
			for(int k=0;k<sendstr.GetLength();k++)
			{
				SendBUFF[k]=sendstr.GetAt(k);
			}

			 //将要发送的每次缓冲区字符数组数据都写入send.txt
			FILE *fp1;
			fp1=fopen("D:\\send.txt","at");
			fwrite(SendBUFF,k,1,fp1);
			fclose(fp1);

            //2.将SendBUFF数组的内容写入eachBufSEND缓冲区
			int nretval,Send;
    		BYTE eachBufSEND[50*1024];
    		for(Send=0;Send<sendlength;Send++)
			{
    		    eachBufSEND[Send]=(BYTE)SendBUFF[Send];
			}
	    	nretval=sendto(pDlg->m_SockClient,(char*)eachBufSEND,sendlength,0,(sockaddr*)&addrFrom,sizeof(addrFrom));
			//sendto:成功的话返回发送的字节数；sendlength:eachBufSEND缓冲区中数据的长度
			
		}//有接收          
	}//while循环结束
	//closesocket(pDlg->m_SockClient);	
	return 0;	
}

//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                                开启UDP服务器                                                       ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	

void CSocketClientDlg::OnConnect() //开启UDP服务器
{
	CString strIP,strPort,strDForce,strResult;
	GetDlgItem(IDC_ED_IP)->GetWindowText(strIP);
	GetDlgItem(IDC_ED_PORT)->GetWindowText(strPort);
	GetDlgItem(IDC_ED_TEXT)->GetWindowText(strDForce);//获取期望力；
	//struct sockaddr_in serveraddr;
	int iPort;
	DWORD iThreadId=0;
	iPort=atoi(strPort);
	DesiredForce=atoi(strDForce);
	m_SockClient=socket(AF_INET,SOCK_DGRAM,0);
	CString strTmp;
	int iBindResult;
	
	Polish_view.Format("绑定的端口是%d\r\n",iPort);
	SetDlgItemText(IDC_Polish_view,Polish_view);
	Connect_view.Format("监听成功");
	SetDlgItemText(IDC_Connect_view,Connect_view);

	//设置网络地址信息
	serveraddr.sin_family=AF_INET;					
	serveraddr.sin_port=htons(iPort);	//端口			
	serveraddr.sin_addr.S_un.S_addr=inet_addr(strIP.GetBuffer(0));//地址
	//绑定地址信息
    iBindResult=bind(m_SockClient,(struct sockaddr*)&serveraddr,sizeof(sockaddr));
	
	if(iBindResult!=0)
	{

		Polish_view.Format("KUKA监听失败，请重试");
		SetDlgItemText(IDC_Polish_view,Polish_view);
		Connect_view.Format("监听失败");
		SetDlgItemText(IDC_Connect_view,Connect_view);
		
		return;
	}
	HANDLE threadproServerThread=CreateThread(NULL,0,threadproServer,(LPVOID)this,0,&iThreadId);
    CloseHandle(threadproServerThread);
}


void CSocketClientDlg::OnExit() //退出
{
	OnCancel();	
}


BOOL CSocketClientDlg::DestroyWindow() 
{
	
	return CDialog::DestroyWindow();
}


//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                       UDPLabView服务器监听线程                                                     ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	

DWORD WINAPI threadproLabView(LPVOID pParam)//UDPLabView服务器监听线程
{
	CSocketClientDlg *pDlgF=(CSocketClientDlg*)pParam;
	int num,SendNum;
	//sockaddr_in addrFrom;//接收连接地址
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
			//发送力信号和xyz坐标到LabVIEW
			sprintf(sendBuf, "%.3fx%.3fy%.3fz%.3fp%.5fk%.5fs",Pos[0],Pos[1],Pos[2],-DeltaPosition_2, gKi, n_Delta_Postion);
			int num=strlen(sendBuf); 
			char numBuf[10];

			sprintf(numBuf,"%d",num);
			sendto(pDlgF->m_SockForce,(char*)numBuf,strlen(numBuf),0,(sockaddr*)&addrFrom,sizeof(addrFrom));//发送字符串长度
			sendto(pDlgF->m_SockForce,(char*)sendBuf,strlen(sendBuf),0,(sockaddr*)&addrFrom,sizeof(addrFrom));//发送字符串
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

	GetDlgItem(IDC_ED_IP)->GetWindowText(strIP);//得到本机IP和端口
	GetDlgItem(IDC_ED_LabViewPORT)->GetWindowText(strPort);
	iPort=atoi(strPort);
	sockaddr_in serveraddr;
	serveraddr.sin_family=AF_INET;	//设置服务器地址家族
	serveraddr.sin_port=htons(iPort);	//设置服务器端口号
	serveraddr.sin_addr.S_un.S_addr=inet_addr(strIP.GetBuffer(0));
	//GetDlgItem(IDC_ED_TEXT)->GetWindowText(strDForce);
			
	m_SockForce=socket(AF_INET,SOCK_DGRAM,0);
	iBindResult=bind(m_SockForce,(struct sockaddr*)&serveraddr,sizeof(sockaddr));
	
	if(iBindResult!=0)
	{
		MessageBox("绑定端口失败!\r\n");
		Polish_view.Format("LeabVIEW监听失败，请重试");
		SetDlgItemText(IDC_Polish_view,Polish_view);
	    //return;
	}
	//CreateThread(NULL,0,threadproServer,(LPVOID)this,0,&iThreadId );

	HANDLE threadproLabViewThread=CreateThread(NULL,0,threadproLabView,(LPVOID)this,0,&iThreadId);
	CloseHandle(threadproLabViewThread);
	//sendto(m_SockClient,strDForce.GetBuffer(0),strText.GetLength(),0,(sockaddr*)&addrTo,sizeof(addrTo));	
}


//  ||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//  ||                                               导入抛光点                                                           ||
//  ||                                                                                                                    ||
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void CSocketClientDlg::OnButton7() //导入抛光点
{
	SwitchPolishCase = 2; //运行后直接暂停，等待信号
//	m_PolishingDate.ResetContent(); //清空列表框数据

	CString strMS_speed_min;
	
	// 读取运行速度	
	GetDlgItem(IDC_MoveSafe_speed_min)->GetWindowText(strMS_speed_min);
	MS_speed_min = atof(strMS_speed_min);//atof 字符串转double

	// 读取变换坐标系的参数
	GetDlgItem(IDC_POS_Trans_X)->GetWindowText(strPOS_Trans_X);
	POS_Trans_X = atof(strPOS_Trans_X);//atof 字符串转double
	GetDlgItem(IDC_POS_Trans_Y)->GetWindowText(strPOS_Trans_Y);
	POS_Trans_Y = atof(strPOS_Trans_Y);//atof 字符串转double
	GetDlgItem(IDC_POS_Trans_Z)->GetWindowText(strPOS_Trans_Z);
	POS_Trans_Z = atof(strPOS_Trans_Z);//atof 字符串转double
	GetDlgItem(IDC_POS_Trans_A)->GetWindowText(strPOS_Trans_A);
	POS_Trans_A = atof(strPOS_Trans_A);//atof 字符串转double

		   
    CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"All Files(*.TXT)|*.TXT||",AfxGetMainWnd());//读文件
	m_PolishingDate.DeleteString(0);
	int index=0,index1=1;
	if(IDOK!=dlg.DoModal())
	{
		AfxMessageBox("打开文件对话框失败!");
	}
	CString str=dlg.GetPathName();
	CStdioFile file;
	if(!file.Open(str,CFile::modeRead))
	{
		AfxMessageBox("打开文件失败!");
	}
	
    ((CEdit*)GetDlgItem(IDC_EDIT_ADDRESS))->SetWindowText(str);
	CString tempstr,tempstr1,tempstrzh;
	
    
	double data[8]={0.0};	
	
	while(file.ReadString(tempstr))
	{
		tempstr.TrimLeft();
		tempstr.TrimRight();
		int str_len=tempstr.GetLength();
		if(-1!=tempstr.Find("网格数据"))
		{	
			
			tempstr1.Format("%05d  NOP",index);
			m_PolishingDate.AddString(tempstr1);
			m_PolishingDate.SetCurSel(index);
			index++;
			
		}
		else
		{  
			int s_index=tempstr.Find(':');//光标到:
			tempstr1="";int m=0; 
			//double data[8]={0.0};
			int data_index=0;
			for(int j=s_index+2;j<str_len-1;j++)
			{
				if(!(tempstr.GetAt(j)=='('||tempstr.GetAt(j)==')'))
				{
					if(tempstr.GetAt(j)!=',')//用逗号来区分数字
					{
						tempstr1.Insert(m,tempstr.GetAt(j));//将每个数据放在tempstr1里面
						m++;
					}
					else
					{
						m=0;
                        data[data_index]=atof(tempstr1);//遇到“，”则保存tempstr1到数组
						tempstr1="";
						data_index++;
					}
				}
				
			}
			data[data_index]=atof(tempstr1);//得到8个数据之中的最后一个，因为这个数据后面没有逗号
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
		
		m_PolishingDate.AddString(tempstr1);//显示在屏幕上
		m_PolishingDate.SetCurSel(index);

		// 创建运动安全范围的动态包容盒
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

	}//while循环
	//PointNumMax=index-1;
	Check_Pos[0] = ppoint[0].x ; Check_Pos[1] = ppoint[0].y ; Check_Pos[2] = ppoint[0].z ;  //判断点的初始值
	
	//绝对坐标初始赋值
	True_Pos[0] = ppoint[0].x ;
	True_Pos[1] = ppoint[0].y ; 
	True_Pos[2] = ppoint[0].z ; 

	//绝对坐标初始值显示
	Send_X1_view.Format("%f" , True_Pos[0]);
	Send_Y1_view.Format("%f" , True_Pos[1]);
	Send_Z1_view.Format("%f" , True_Pos[2]);
	
	// 向机器人发送的绝对坐标的初始值
	Send_XYZ[0] = ppoint[0].x ;
	Send_XYZ[1] = ppoint[0].y ;
	Send_XYZ[2] = ppoint[0].z ;	

	SetDlgItemText(IDC_Send_X1,Send_X1_view);
	SetDlgItemText(IDC_Send_Y1,Send_Y1_view);
	SetDlgItemText(IDC_Send_Z1,Send_Z1_view);

	// 将安全范围扩大2cm
	LIN_x_max  += 200;
	LIN_x_min  -= 200;
	LIN_y_max  += 200;
 	LIN_y_min  -= 200;
	LIN_z_max  += 800; // 机器人向上运动是安全的，所以该值可以大一些
	LIN_z_min  -= 100;

	// 机器人需要走到原点，所以必须包含坐标原点
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

	Polish_view.Format("导入完成，共%d个点" ,index-1);
	SetDlgItemText(IDC_Polish_view,Polish_view);

	// 进度条初始值赋值
	m_progress.SetRange( 0 , index-1 );
	
	Progress_View.Format("%d/%d" ,itime,alldtime);
	SetDlgItemText(IDC_Progress_View,Progress_View);
				
				
}

void CSocketClientDlg::OnClean()  // 清除抛光点有关的变量，可以再次导入抛光点
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

void CSocketClientDlg::OnSwitchPolish_Pause() // 机器人暂停运动
{
	// TODO: Add your control notification handler code here
	
	save_stop_i = 0 ; //等待指令只发送一条数据
	SwitchPolishCase = 2;  // 暂停运动通道
	
}

void CSocketClientDlg::OnSwitchPolish_Continue() // 机器人继续运动
{
	// TODO: Add your control notification handler code here


	save_stop_i = 0 ; //等待指令只发送一条数据
		Polish_Hoist_i = 0;

	Polish_Advance_i = 0 ; // 继续运动后仍需加工预进

	Polish_Continue_i = 0; //继续运动有一段当前点到开始点的距离，通过直线插补走到起始点，每次执行完都需要重新计数
	

	// 继续运动时将抛光点前移一个ipoint
	
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
	
	// 调试用，在调试时将Pos_Realtime的值改为Send_XYZ
	if(	Debug_switch == 1 )
	{
	Polish_Continue_xyz[0] = -( Send_XYZ[0] - Pos_Temporary[0] );
	Polish_Continue_xyz[1] = -( Send_XYZ[1] - Pos_Temporary[1] );
	Polish_Continue_xyz[2] = -( Send_XYZ[2] - Pos_Temporary[2] );
	}
	
	Polish_Continue_Length = sqrt( pow( Polish_Continue_xyz[0] ,2)
		+ pow( Polish_Continue_xyz[1] ,2) + pow( Polish_Continue_xyz[2] ,2) );
	Polish_Continue_d = ceil ( Polish_Continue_Length / 0.4 ); //所需走的点的个数
	
	Polish_Continue_D[0] = Polish_Continue_xyz[0] / Polish_Continue_d ;
	Polish_Continue_D[1] = Polish_Continue_xyz[1] / Polish_Continue_d ;
	Polish_Continue_D[2] = Polish_Continue_xyz[2] / Polish_Continue_d ;
	

	SwitchPolishCase = 3;  // 机器人继续运动通道
	
}

void CSocketClientDlg::OnSwitchPolish_Start() // 机器人重头开始运动
{
	// TODO: Add your control notification handler code here

	save_stop_i = 0 ; //等待指令只发送一条数据
	Polish_Hoist_i = 0;

	//重新运动后很多变量需要初始化
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

	Polish_Advance_i = 0 ; // 重新运动后仍需加工预进
	
	// 初始化判断点的值,CHECK是一个累加变量
	Check_Pos[0] = ppoint[0].x ; 
	Check_Pos[1] = ppoint[0].y ; 
	Check_Pos[2] = ppoint[0].z ; 

	Polish_Start_i = 0; //重头运动有一段当前点到开始点的距离，通过直线插补走到起始点，每次执行完都需要重新计数

	Pos_Temporary[0] = ppoint[0].x + 10*ppoint[0].a;
	Pos_Temporary[1] = ppoint[0].y + 10*ppoint[0].b;
	Pos_Temporary[2] = ppoint[0].z + 10*ppoint[0].c + 100;
	
	if(	Debug_switch == 0 )
	{
		Polish_Start_xyz[0] = -( Pos_Realtime[0] - Pos_Temporary[0] );
		Polish_Start_xyz[1] = -( Pos_Realtime[1] - Pos_Temporary[1] );
		Polish_Start_xyz[2] = -( Pos_Realtime[2] - Pos_Temporary[2] ); 
	}
		// 调试用，在调试时将Pos_Realtime的值改为Send_XYZ
	if(	Debug_switch == 1 )
	{
	Polish_Start_xyz[0] = -( Send_XYZ[0] - Pos_Temporary[0] );
	Polish_Start_xyz[1] = -( Send_XYZ[1] - Pos_Temporary[1] );
	Polish_Start_xyz[2] = -( Send_XYZ[2] - Pos_Temporary[2] );	
	}


	Polish_Start_Length = sqrt( pow( Polish_Start_xyz[0] ,2)
		+ pow( Polish_Start_xyz[1] ,2) + pow( Polish_Start_xyz[2] ,2) );
	Polish_Start_d = ceil ( Polish_Start_Length / 0.4 ); //所需走的点的个数

	Polish_Start_D[0] = Polish_Start_xyz[0] / Polish_Start_d ;
	Polish_Start_D[1] = Polish_Start_xyz[1] / Polish_Start_d ;
	Polish_Start_D[2] = Polish_Start_xyz[2] / Polish_Start_d ;
	
	SwitchPolishCase = 4;  // 机器人重头开始运动通道

}

void CSocketClientDlg::OnSwitchPolish_Hoist() // 机器人缓慢上升
{
	// TODO: Add your control notification handler code here
	
	save_stop_i = 0 ; //等待指令只发送一条数据

	Polish_Hoist_i = 0;	
	SwitchPolishCase = 5;
	
}

void CSocketClientDlg::OnSwitchPolish_Reset()  // 机器人复位，需调用机器人坐标和工件坐标,为保证正常运行，该命令会跳过安全检查代码，暂不使用
{
	// TODO: Add your control notification handler code here

	Polish_Reset_i = 0; 

	Axis_workpiece_robot[0] = 800; Axis_workpiece_robot[1] = 0.0; Axis_workpiece_robot[2] = 1200;  //该坐标需要手动获取	
	
	Polish_Reset_xyz[0] = -( Pos_Realtime[0] - Axis_workpiece_robot[0] );
	Polish_Reset_xyz[1] = -( Pos_Realtime[1] - Axis_workpiece_robot[1] );
	Polish_Reset_xyz[2] = -( Pos_Realtime[2] - Axis_workpiece_robot[2] );
	
	Polish_Reset_Length = sqrt( pow( Polish_Reset_xyz[0] ,2)
		+ pow( Polish_Reset_xyz[1] ,2) + pow( Polish_Reset_xyz[2] ,2) );
	Polish_Reset_d = ceil ( Polish_Reset_Length / 0.4 ); //所需走的点的个数
	
	Polish_Reset_D[0] = Polish_Reset_xyz[0] / Polish_Reset_d ;
	Polish_Reset_D[1] = Polish_Reset_xyz[1] / Polish_Reset_d ;
	Polish_Reset_D[2] = Polish_Reset_xyz[2] / Polish_Reset_d ;
	
	SwitchPolishCase = 6;
	
}

void CSocketClientDlg::OnSwitchPolish_Stop()  // 机器人停止运动
{
	// TODO: Add your control notification handler code here
	SwitchPolishCase = 7;



}


void CSocketClientDlg::OnSwitchPolish_Teach()   //示教
{
	// TODO: Add your control notification handler code here
	
	save_stop_i = 0 ; //等待指令只发送一条数据
	Polish_Hoist_i = 0;

	CString strMS_speed_medium; //获取示教速度
	GetDlgItem(IDC_MoveSafe_speed_medium)->GetWindowText(strMS_speed_medium);
	MS_speed_medium = atof(strMS_speed_medium);//atof 字符串转double
	
	SwitchPolishCase = 8;	
}


void CSocketClientDlg::OnSwitchPolish_Fast() //快速换刀
{
	// TODO: Add your control notification handler code here

	save_stop_i = 0 ; //等待指令只发送一条数据
		Polish_Hoist_i = 0;

	Polish_Fast_i = 0 ;
	Polish_Fast_j = 0 ;

	if(	Debug_switch == 0 )
	{
		Polish_Fast_xyz[0] = -( Pos_Realtime[0] - POS_Trans_X );
		Polish_Fast_xyz[1] = -( Pos_Realtime[1] - POS_Trans_Y );
		Polish_Fast_xyz[2] = -( Pos_Realtime[2] - POS_Trans_Z );
	}
	
	// 调试用，Pos_Realtime的值改为Send_XYZ
	if(	Debug_switch == 1 )
	{
		Polish_Fast_xyz[0] = -( Send_XYZ[0] - POS_Trans_X );
		Polish_Fast_xyz[1] = -( Send_XYZ[1] - POS_Trans_Y );
		Polish_Fast_xyz[2] = -( Send_XYZ[2] - POS_Trans_Z );	
	}

	Polish_Fast_Length = sqrt( pow( Polish_Fast_xyz[0] ,2)
		+ pow( Polish_Fast_xyz[1] ,2) );
	Polish_Fast_d = ceil ( Polish_Fast_Length / 0.4 ); //所需走的点的个数
	Polish_Fast_down_d = ceil ( abs( -100 + Polish_Fast_xyz[2] ) / 0.2 ); // 下降所需走的点的个数
	
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
	Teach_Length = atof(strTeach_Length);//atof 字符串转double

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
	Teach_Length = atof(strTeach_Length);//atof 字符串转double

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
	Teach_Length = atof(strTeach_Length);//atof 字符串转double

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
	Teach_Length = atof(strTeach_Length);//atof 字符串转double

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
	Teach_Length = atof(strTeach_Length);//atof 字符串转double

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
	Teach_Length = atof(strTeach_Length);//atof 字符串转double

	Polish_Hoist_i = 0;

	Teach_i = 1;
	Teach_d = ceil( Teach_Length / MS_speed_medium );
	Teach_L = Teach_Length / Teach_d ;
	
	Teach_Pos[0] = 0 ; Teach_Pos[1] = 0 ; Teach_Pos[2] = -Teach_L ;

}


void CSocketClientDlg::OnButton2() 
{

	// TODO: Add your control notification handler code here

	ShellExecute(NULL,"explore", "F:\\KUKA抛光实验数据", NULL, NULL, SW_SHOW);	//打开Windows资源管理器
}


void CSocketClientDlg::OnSaveData() 
{
	// TODO: Add your control notification handler code here
	
	/*   FILE *fpWrite=fopen("F:\\KUKA抛光实验数据上位机收发数据.txt","w");  
	
	  for(int i=0;i<10;i++)  
	  fprintf(fpWrite,"%d ",i);  
    fclose(fpWrite);  */
	
	char filename[2000];
	sprintf(filename,"F:\\KUKA抛光实验数据\\%s-上位机收发数据.txt",SaveData_Name);
	ofstream dataFile;
	dataFile.open(filename, ofstream::trunc);
	
	dataFile<<"SwitchPolishCase 1 正常抛光 2 等待信号 3 继续抛光 4 重头抛光 5 机器提刀 6 补偿测试\n"<<
		"SwitchPolishCase 7 通信停止 8 示教操作 9 一键换刀  10 预加工 11 角度矫正 12 力补偿 \n"<<
		"第i个点 抛光指令 (发送的增量坐标)   (接收的机器人反馈坐标——实际位置)   (法矢量位移补偿量——力补偿的值)<<实时合力 "<< endl;
	
	for (int save_i = 1 ; save_i<=Socket_Client_j ;save_i++ )
	{

		dataFile<<setw(6)<<save_i<<" "<<"抛光指令:"<<setw(2)<<SaveData_Socket[save_i][0]<<" ("
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

//储存标定坐标
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
	// GetDlgItem(IDC_POS_Trans_A)->SetWindowText("85.561");        //   85.5611627 逆时针旋转 4.4388373
	
	GetDlgItem(IDC_POS_Trans_H)->GetWindowText(strPOS_Trans_H);
	POS_Trans_H = atof(strPOS_Trans_H);//atof 字符串转double	
	
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
	//  角度变换 
	GetDlgItem(IDC_POS_Trans_A)->GetWindowText(strPOS_Trans_A);
	POS_Trans_A = atof(strPOS_Trans_A);//atof 字符串转double
	POS_Trans_A += -72 ; //汽车轮毂拥有是5等分对称图形 360/5
	char CHAR_POS_Trans_A[20];

	sprintf(CHAR_POS_Trans_A,"%.4f",POS_Trans_A);
	SetDlgItemText(IDC_POS_Trans_A,CHAR_POS_Trans_A);
//	GetDlgItem(IDC_POS_Trans_A)->SetWindowText(strPOS_Trans_A); 
	
}

void CSocketClientDlg::OnBTNGraph() 
{
	// TODO: Add your control notification handler code here
// 	initgraph(640, 480);	// 这里和 TC 略有区别
// 	circle(100, 100, 60);	// 画圆，圆心(100, 100)，半径 60
// 	getch();				// 按任意键继续
// 	closegraph();			// 关闭图形界面
	
}

void CSocketClientDlg::OnButton15() 
{
	// TODO: Add your control notification handler code here
	
	save_stop_i = 0 ; //等待指令只发送一条数据
	SwitchPolishCase = 1;  // 机器人继续运动通道
	
}

void CSocketClientDlg::OnCeshi() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_Send_View)->GetWindowText(strCase_force);
	Case_force = atof(strCase_force);//atof 字符串转double

	
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
	SwitchPolishCase = 11 ;  // 矫正机器人角度误差
	
}

void CSocketClientDlg::OnSwitchPolishText() 
{
	// TODO: Add your control notification handler code here
	/*	Polish_Text_i = 0 ;
	SwitchPolishCase = 6 ;  // 力补偿测试*/

	// 读取释放抛光点的个数	
	GetDlgItem(IDC_Release_point)->GetWindowText( strRelease_point );
	Release_point = atof( strRelease_point );//atof 字符串转double
	Release_point = Release_point - 20 ;

	while(1)
	{
		itime ++;//计数，while循环一次加一    itime 此时为1
		if (itime<= alldtime )
		{	
			
			//dtime分段总和			
			if (itime>dtime)   // dtime 此时为0，条件为真，所以 ipont直接指向第一个增量坐标
			{
				
				ipoint ++;
				dtime += ppoint[ipoint].dd;
				
				
				True_Pos[0] += ppoint[ipoint].x ; 
				True_Pos[1] += ppoint[ipoint].y ; 
				True_Pos[2] += ppoint[ipoint].z ;
				

			}
			
			
		}

		m_progress.SetPos(ipoint); // 进度条处理
		
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

		save_stop_i = 0 ; //等待指令只发送一条数据
		Polish_Hoist_i = 0;


	SwitchPolishCase = 13;  // 机器人继续运动通道

	Polish_TEXT_i = 0;


	TEXT_POS[0] = 0.01 ;
	TEXT_POS[1] = 0 ;
	TEXT_POS[2] = 0 ;

	
}

void CSocketClientDlg::OnBtnExit10() 
{
	// TODO: Add your control notification handler code here

	
		save_stop_i = 0 ; //等待指令只发送一条数据
		Polish_Hoist_i = 0;


	SwitchPolishCase = 14;  // 机器人继续运动通道

	Polish_TEXT_i = 0;


	TEXT_POS[0] = 0.01 ;
	TEXT_POS[1] = 0 ;
	TEXT_POS[2] = 0 ;
	
}
