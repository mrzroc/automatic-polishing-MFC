// SocketClientDlg.h : header file
//

#if !defined(AFX_SOCKETCLIENTDLG_H__74837F69_BE13_47E6_BB40_213CC8994CDF__INCLUDED_)
#define AFX_SOCKETCLIENTDLG_H__74837F69_BE13_47E6_BB40_213CC8994CDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSocketClientDlg dialog

double Case_force = 10; // 调试时测量力
CString strCase_force ; 

CString Progress_View; // 抛光进程显示
CString Warning_view ; // 抛光进程显示

int Debug_switch = 0 ; // 调试打开开关

int Release_point = 0 ;
CString strRelease_point;


// 空间坐标点的存储


double Check_Pos[3] = {0};  // 代码检查的坐标变量
double Axis_workpiece_robot[3] = {0};// 工件坐标系与机器人坐标系的相对位置
double Pos_Realtime[3] = {0}; // 实时接收的数据，机器人发送过来的绝对坐标
double Pos_Temporary[3] = {0}; // 精确点上方的临时点
double Pos_Correct[3] = {0}; // 不同抛光命令所需走到的精确点
double True_Pos[3]= {0}; // 抛光工件的绝对坐标，也为期望点
double Send_XYZ[3]= {0}; // 向机器人发送的绝对坐标
double Send_Pos[3]= {0}; // 向机器人实时发送的增量
double Send_DeltaPosition[3]= {0}; // 向机器人实时发送的补偿量

double TEXT_POS[3] = {0}; // SHIYAN

int Polish_TEXT_i = 0 ;



// 误差补偿定义的代码 error compensation
double ERR_Point[100][3] = {0}; //储存运动步长
double ERR_Judge[3] = {0}; // 临时存储数据
int ERR_switch  = 2;


// 可视化添加的代码
CString Polish_view; // 抛光命令显示
CString Connect_view; // 通信是否连接显示
CString Teach_X_view;
CString Teach_Y_view;
CString Teach_Z_view;
CString Send_X_view;
CString Send_Y_view;
CString Send_Z_view;
CString Send_X1_view;
CString Send_Y1_view;
CString Send_Z1_view;
CString Send_X2_view;
CString Send_Y2_view;
CString Send_Z2_view;


// 定义坐标变换的常量
double POS_Trans_X;
double POS_Trans_Y;
double POS_Trans_Z;
double POS_Trans_A;
double POS_Trans_H;
CString strPOS_Trans_X;
CString strPOS_Trans_Y;
CString strPOS_Trans_Z;
CString strPOS_Trans_A;
CString strPOS_Trans_H;



// 通信有关的参数
int SwitchPolishCase; // 定义抛光指令
int Switch_Case; // 定义抛光指令储存

// SaveData_Socket[0] 储存抛光指令  SaveData_Socket[1~3] (发送坐标)
// SaveData_Socket[4~6] (接收坐标)  SaveData_Socket[7~9] (力补偿量)
double SaveData_Socket[200000][11] = {0}; // 每次通信发送数据的存储数组
CString strSaveData ;
int Socket_Client_j = 0 ; // 服务器运行时就会一直加
int save_stop_i = 0 ; // 储存暂停时的数据


// MOVESAFE运动安全参数定义
double MS_speed_min; // 最小运行速度
double MS_speed_medium ; // 中等运行速度
double MS_speed_max;  // 最大运行速度 
CString MS_limit_x_min ; // X轴方向最小限位 
CString MS_limit_x_max ; // X轴方向最大限位 
CString MS_limit_y_min ; // Y轴方向最小限位 
CString MS_limit_y_max ; // Y轴方向最大限位 
CString MS_limit_z_min ; // Z轴方向最小限位 
CString MS_limit_z_max ; // Z轴方向最大限位 

double MS_limit_Box ; // 最小运动包容盒尺寸

// 定义运动安全范围
double LIN_x = 0;
double LIN_x1 = 0;
double LIN_x_min = 0;
double LIN_x_max = 0;
double LIN_y = 0;
double LIN_y1 = 0;
double LIN_y_min = 0;
double LIN_y_max = 0;
double LIN_z = 0;
double LIN_z1 = 0;
double LIN_z_min = 0;
double LIN_z_max = 0;


// 角度误差矫正的相关变量
double Polish_Correct_Length; // 目前点到目标点的距离
double Polish_Correct_xyz[3] = {0}; // 目前点到目标点的距离
int Polish_Correct_d; // 目前点到目标点的所需走的步数
int Polish_Correct_i;
double Polish_Correct_D[3] = {0}; // 目前点到目标点的运动过程中的步长


// 力补偿测试的相关变量
double Polish_Text_Length; // 目前点到目标点的距离
double Polish_Text_xyz[3] = {0}; // 目前点到目标点的距离
int Polish_Text_d; // 目前点到目标点的所需走的步数
int Polish_Text_i;
double Polish_Text_D[3] = {0}; // 目前点到目标点的运动过程中的步长
double Text_Force_a = 0 ; //测试时用的法矢量
double Text_Force_b = 1 ;
double Text_Force_c = 0 ;


// 重头开始运动的变量
double Polish_Start_Length; // 目前点到目标点的距离
double Polish_Start_xyz[3] = {0}; // 目前点到目标点的距离
int Polish_Start_d; // 目前点到目标点的所需走的步数
int Polish_Start_i;
double Polish_Start_D[3] = {0}; // 目前点到目标点的运动过程中的步长


// 继续抛光运动的变量
double Polish_Continue_Length; // 目前点到目标点的距离
double Polish_Continue_xyz[3] = {0}; // 目前点到目标点的距离
int Polish_Continue_d; // 目前点到目标点的所需走的步数
int Polish_Continue_i;
double Polish_Continue_D[3] = {0};  // 目前点到目标点的运动过程中的步长


// 向上运动的相关变量
double Polish_UP_Length; // 目前点到目标点的距离
double Polish_UP_xyz[3] = {0}; // 目前点到目标点的距离
int Polish_UP_d; // 目前点到目标点的所需走的步数
int Polish_UP_i;
double Polish_UP_D[3] = {0};  // 目前点到目标点的运动过程中的步长


// 预进加工的变量

int Polish_Advance_i = 0;

// 快速换刀运动的变量
double Polish_Fast_Length; // 目前点到目标点的距离
double Polish_Fast_xyz[3] = {0}; // 目前点到目标点的距离
int Polish_Fast_d; // 目前点到目标点的所需走的步数
int Polish_Fast_down_d; // 目前点到目标点的所需走的步数
int Polish_Fast_i;
int Polish_Fast_j;

double Polish_Fast_D[4] = {0}; // 目前点到目标点的运动过程中的步长 


// 复位操作的有关变量
double Polish_Reset_Length; // 目前点到目标点的距离
double Polish_Reset_xyz[3] = {0}; // 目前点到目标点的距离
int Polish_Reset_d; // 目前点到目标点的所需走的步数
int Polish_Reset_i;
double Polish_Reset_D[3] = {0}; // 目前点到目标点的运动过程中的步长 


// 提刀循环变量
int Polish_Hoist_i = 0;


// 定义示教操作的变量
int Teach_i = 0 ;
double Teach_Pos[5] = {0};
double Teach_Length;  //示教操作每次增加的长度
double Teach_L;  //示教操作单步执行长度
double Teach_d; //示教操作循环的次数
CString strTeach_Length;
CString strTeach_X;
CString strTeach_Y;
CString strTeach_Z;


// 定义力控制参数
struct PIParameter 
{
	double force_N_1 ;
	double force_N  ;
	double e_N  ;
	double de_N ;
	double u_N ;

	double k_n ;
	double k_n_1 ;
	double R_n  ;
	double P_n_1  ;
	double P_n  ;

	int count;

};

// 定义抛光点变量
struct PolishingPoint
{
	double d;
	double dd;
	double x;
	double y;
	double z;
	double a;
	double b;
	double c;	
	//int count1;	
};

class CSocketClientDlg : public CDialog
{
	// Construction
public:
	CSocketClientDlg(CWnd* pParent = NULL);	// standard constructor
	SOCKET m_SockClient;
	SOCKET m_SockForce;
    //SOCKET m_SockForce;
	FILE *m_pfMsgLog;

// Dialog Data
	//{{AFX_DATA(CSocketClientDlg)
	enum { IDD = IDD_SOCKETCLIENT_DIALOG };
	CProgressCtrl	m_progress;
	CEdit	m_path;
	CListBox	m_PolishingDate;
	CString	SaveData_Name;
	CString	Progress_View;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketClientDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSocketClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnExit();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnClean();
	afx_msg void OnSwitchPolish_Pause();
	afx_msg void OnSwitchPolish_Continue();
	afx_msg void OnSwitchPolish_Start();
	afx_msg void OnSwitchPolish_Hoist();
	afx_msg void OnSwitchPolish_Reset();
	afx_msg void OnSwitchPolish_Stop();
	afx_msg void OnSwitchPolish_Teach();
	afx_msg void OnTeach_X_Sub();
	afx_msg void OnTeach_Y_Add();
	afx_msg void OnTeach_Z_Add();
	afx_msg void OnTeach_X_Add();
	afx_msg void OnTeach_Y_Sub();
	afx_msg void OnTeach_Z_Sub();
	afx_msg void OnSwitchPolish_Fast();
	afx_msg void OnButton2();
	afx_msg void OnSaveData();
	afx_msg void OnTransPOS();
	afx_msg void OnTrans_ANGLE();
	afx_msg void OnBTNGraph();
	afx_msg void OnButton15();
	afx_msg void OnCeshi();
	afx_msg void OnDebugging();
	afx_msg void OnSwitchPolishCorrect();
	afx_msg void OnSwitchPolishText();
	afx_msg void OnBtnExit3();
	afx_msg void OnBtnExit10();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETCLIENTDLG_H__74837F69_BE13_47E6_BB40_213CC8994CDF__INCLUDED_)
