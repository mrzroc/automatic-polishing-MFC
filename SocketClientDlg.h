// SocketClientDlg.h : header file
//

#if !defined(AFX_SOCKETCLIENTDLG_H__74837F69_BE13_47E6_BB40_213CC8994CDF__INCLUDED_)
#define AFX_SOCKETCLIENTDLG_H__74837F69_BE13_47E6_BB40_213CC8994CDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSocketClientDlg dialog

double Case_force = 10; // ����ʱ������
CString strCase_force ; 

CString Progress_View; // �׹������ʾ
CString Warning_view ; // �׹������ʾ

int Debug_switch = 0 ; // ���Դ򿪿���

int Release_point = 0 ;
CString strRelease_point;


// �ռ������Ĵ洢


double Check_Pos[3] = {0};  // ��������������
double Axis_workpiece_robot[3] = {0};// ��������ϵ�����������ϵ�����λ��
double Pos_Realtime[3] = {0}; // ʵʱ���յ����ݣ������˷��͹����ľ�������
double Pos_Temporary[3] = {0}; // ��ȷ���Ϸ�����ʱ��
double Pos_Correct[3] = {0}; // ��ͬ�׹����������ߵ��ľ�ȷ��
double True_Pos[3]= {0}; // �׹⹤���ľ������꣬ҲΪ������
double Send_XYZ[3]= {0}; // ������˷��͵ľ�������
double Send_Pos[3]= {0}; // �������ʵʱ���͵�����
double Send_DeltaPosition[3]= {0}; // �������ʵʱ���͵Ĳ�����

double TEXT_POS[3] = {0}; // SHIYAN

int Polish_TEXT_i = 0 ;



// ��������Ĵ��� error compensation
double ERR_Point[100][3] = {0}; //�����˶�����
double ERR_Judge[3] = {0}; // ��ʱ�洢����
int ERR_switch  = 2;


// ���ӻ���ӵĴ���
CString Polish_view; // �׹�������ʾ
CString Connect_view; // ͨ���Ƿ�������ʾ
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


// ��������任�ĳ���
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



// ͨ���йصĲ���
int SwitchPolishCase; // �����׹�ָ��
int Switch_Case; // �����׹�ָ���

// SaveData_Socket[0] �����׹�ָ��  SaveData_Socket[1~3] (��������)
// SaveData_Socket[4~6] (��������)  SaveData_Socket[7~9] (��������)
double SaveData_Socket[200000][11] = {0}; // ÿ��ͨ�ŷ������ݵĴ洢����
CString strSaveData ;
int Socket_Client_j = 0 ; // ����������ʱ�ͻ�һֱ��
int save_stop_i = 0 ; // ������ͣʱ������


// MOVESAFE�˶���ȫ��������
double MS_speed_min; // ��С�����ٶ�
double MS_speed_medium ; // �е������ٶ�
double MS_speed_max;  // ��������ٶ� 
CString MS_limit_x_min ; // X�᷽����С��λ 
CString MS_limit_x_max ; // X�᷽�������λ 
CString MS_limit_y_min ; // Y�᷽����С��λ 
CString MS_limit_y_max ; // Y�᷽�������λ 
CString MS_limit_z_min ; // Z�᷽����С��λ 
CString MS_limit_z_max ; // Z�᷽�������λ 

double MS_limit_Box ; // ��С�˶����ݺгߴ�

// �����˶���ȫ��Χ
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


// �Ƕ�����������ر���
double Polish_Correct_Length; // Ŀǰ�㵽Ŀ���ľ���
double Polish_Correct_xyz[3] = {0}; // Ŀǰ�㵽Ŀ���ľ���
int Polish_Correct_d; // Ŀǰ�㵽Ŀ���������ߵĲ���
int Polish_Correct_i;
double Polish_Correct_D[3] = {0}; // Ŀǰ�㵽Ŀ�����˶������еĲ���


// ���������Ե���ر���
double Polish_Text_Length; // Ŀǰ�㵽Ŀ���ľ���
double Polish_Text_xyz[3] = {0}; // Ŀǰ�㵽Ŀ���ľ���
int Polish_Text_d; // Ŀǰ�㵽Ŀ���������ߵĲ���
int Polish_Text_i;
double Polish_Text_D[3] = {0}; // Ŀǰ�㵽Ŀ�����˶������еĲ���
double Text_Force_a = 0 ; //����ʱ�õķ�ʸ��
double Text_Force_b = 1 ;
double Text_Force_c = 0 ;


// ��ͷ��ʼ�˶��ı���
double Polish_Start_Length; // Ŀǰ�㵽Ŀ���ľ���
double Polish_Start_xyz[3] = {0}; // Ŀǰ�㵽Ŀ���ľ���
int Polish_Start_d; // Ŀǰ�㵽Ŀ���������ߵĲ���
int Polish_Start_i;
double Polish_Start_D[3] = {0}; // Ŀǰ�㵽Ŀ�����˶������еĲ���


// �����׹��˶��ı���
double Polish_Continue_Length; // Ŀǰ�㵽Ŀ���ľ���
double Polish_Continue_xyz[3] = {0}; // Ŀǰ�㵽Ŀ���ľ���
int Polish_Continue_d; // Ŀǰ�㵽Ŀ���������ߵĲ���
int Polish_Continue_i;
double Polish_Continue_D[3] = {0};  // Ŀǰ�㵽Ŀ�����˶������еĲ���


// �����˶�����ر���
double Polish_UP_Length; // Ŀǰ�㵽Ŀ���ľ���
double Polish_UP_xyz[3] = {0}; // Ŀǰ�㵽Ŀ���ľ���
int Polish_UP_d; // Ŀǰ�㵽Ŀ���������ߵĲ���
int Polish_UP_i;
double Polish_UP_D[3] = {0};  // Ŀǰ�㵽Ŀ�����˶������еĲ���


// Ԥ���ӹ��ı���

int Polish_Advance_i = 0;

// ���ٻ����˶��ı���
double Polish_Fast_Length; // Ŀǰ�㵽Ŀ���ľ���
double Polish_Fast_xyz[3] = {0}; // Ŀǰ�㵽Ŀ���ľ���
int Polish_Fast_d; // Ŀǰ�㵽Ŀ���������ߵĲ���
int Polish_Fast_down_d; // Ŀǰ�㵽Ŀ���������ߵĲ���
int Polish_Fast_i;
int Polish_Fast_j;

double Polish_Fast_D[4] = {0}; // Ŀǰ�㵽Ŀ�����˶������еĲ��� 


// ��λ�������йر���
double Polish_Reset_Length; // Ŀǰ�㵽Ŀ���ľ���
double Polish_Reset_xyz[3] = {0}; // Ŀǰ�㵽Ŀ���ľ���
int Polish_Reset_d; // Ŀǰ�㵽Ŀ���������ߵĲ���
int Polish_Reset_i;
double Polish_Reset_D[3] = {0}; // Ŀǰ�㵽Ŀ�����˶������еĲ��� 


// �ᵶѭ������
int Polish_Hoist_i = 0;


// ����ʾ�̲����ı���
int Teach_i = 0 ;
double Teach_Pos[5] = {0};
double Teach_Length;  //ʾ�̲���ÿ�����ӵĳ���
double Teach_L;  //ʾ�̲�������ִ�г���
double Teach_d; //ʾ�̲���ѭ���Ĵ���
CString strTeach_Length;
CString strTeach_X;
CString strTeach_Y;
CString strTeach_Z;


// ���������Ʋ���
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

// �����׹�����
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
