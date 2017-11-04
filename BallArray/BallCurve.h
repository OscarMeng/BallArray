#pragma once

// CBallCurve ����Ŀ��
class CBall;
class CBallCurve : public CObject
{
public:
	CBallCurve();
	virtual ~CBallCurve();
public:
	static const int	m_nRadius = 100;		        //С��İ뾶
	static const int	m_nBallNum = 35 * 23;		//С�������
	static const int	m_nRowBallNum = 35;       //С�������
	static const int	m_nColBallNum = 23;        //С�������

	static const int	m_nBallsX = 6800;	           //С�������X��
	static const int	m_nBallsY = 4400;	           //С�������Y��
	static const int	m_nMargin = 1000;
	static const int	m_nCanetNum = 10;
	static const int	m_nTimerTime = 100;
	const double       m_dMaxZoom;
	const double       m_dMinZoom;

	CString     m_sPath;                  //���·��
	CFont		m_textFont;				 //��������
private:
	CPoint	m_ptStartPoint;			//��ʼ��
	CPoint	m_pt2DCenterPoint;	//2D���ĵ�
	double	m_dZoom;					//���ű�
	bool	m_bTextOn;					    //�Ƿ���ʾ����
	double  m_dXRotation;           //��X����ת����
	COLORREF m_colBackGround;        //���汳����ɫ
	COLORREF m_colCoordinate;		    //2D��������ɫ
	COLORREF m_colBoardNormal;		//��ͨ״̬�±߿����ɫ
	COLORREF m_colBoardSelected;  	//ѡ��ʱ�߿���ɫ
	double	 m_colUpH;				//��ߵ�HSV��ɫHֵ
	double	 m_colUpS;				//��ߵ�HSV��ɫSֵ
	double	 m_colUpV;				//��ߵ�HSV��ɫVֵ
	double	 m_colDownH;			//��͵�HSV��ɫHֵ
	double	 m_colDownS;			//��͵�HSV��ɫSֵ
	double	 m_colDownV;			//��͵�HSV��ɫVֵ

	int		 m_nPosLimit;	              //��е��λ��
	int		 m_nPosZero;	             //С�������
	int		 m_nPosCeil;	             //�컨��ƽ��
	int		 m_nPosStart;	             //С����ʼ��
	int		 m_nPosDown1;	        //С�������1
	int		 m_nPosDown2;			//С�������2
	int		 m_nPosDown3;			//С�������3
	int         m_nRelStartPos;        //С����������λ��
	bool	     m_bAutoRotation;	    //�Զ���ת
public:
	inline void		SetStartPoint(const CPoint& point)		    { m_ptStartPoint = point; }
	inline void		SetZoom(const double& zoom)				{ m_dZoom = zoom; }
	inline void		SetXRotation(const double& rotation)	{ m_dXRotation = rotation; }
	inline void		SetAutoRotation(const bool& rotation)	{ m_bAutoRotation = rotation; }
	inline CPoint	GetStartPoint()							            { return m_ptStartPoint; }
	inline double	GetZoom()								                { return m_dZoom; }
	inline double	GetXRotation()							            { return m_dXRotation; }
	inline bool		GetTextOn()								                { return m_bTextOn; }
	inline COLORREF GetColBoardNormal()					    	{ return m_colBoardNormal; }
	inline COLORREF GetColBoardSelected()				    	{ return m_colBoardSelected; }
	inline double   GetColUpH()								{ return m_colUpH; }
	inline double   GetColUpS()							    	{ return m_colUpS; }
	inline double   GetColUpV()								{ return m_colUpV; }
	inline double   GetColDownH()							{ return m_colDownH; }
	inline double   GetColDownS()							{ return m_colDownS; }
	inline double   GetColDownV()							{ return m_colDownV; }
	inline int		    GetPosLimit()							    { return m_nPosLimit; }                //С��������λ��
	inline int		    GetPosZero()							    { return m_nPosZero; }                //С�����Ļ�����
	inline int		    GetPosCeil()							    { return m_nPosCeil; }                 //С�������컨����
	inline int		    GetPosStart()								{ return m_nPosStart; }                //С��������ʼ��
	inline int		    GetPosDown1()							{ return m_nPosDown1; }            //С�����Ľ�����1
	inline int		    GetPosDown2()							{ return m_nPosDown2; }            //С�����Ľ�����2
	inline int		    GetPosDown3()							{ return m_nPosDown3; }            //С�����Ľ�����3
	inline bool		GetAutoRotation()						{ return m_bAutoRotation; }
public:
	CBall*		m_pBalls[m_nBallNum];	 //С��
 	void		    SetCurrentBall(int n,int nheight);//����С��λ��
private:
	void ReadCoordinate(CString filename,int coord[][2]);
	void ReadPosParam();
public:
	int		GetWidth();
	int		GetHeight();
	CPoint	Lp2Dp(const CPoint& point);
	CPoint	Dp2Lp(const CPoint& point);
	bool	FindCellIDFromLp(const CPoint& point, int& id);
	void	SelectNone();
	void	SelectOne(bool add, int id);
	void	SelectAll();

	void	SetColorContents(DWORD nStartTime);
	void	SetColorManually(DWORD nStartTime);
public:
	void	Draw(CDC* pDC, CRect rRect);                                                 //����άС��
	void	Draw3D();                                                                                 //����άС��
	COLORREF	CalGradientColor(double index, double range);          //����߶�����ɫ�Ĺ�ϵ
public:
	void	LoadMotorSetMode();			      //��ʼ�������ص��������λ��ģʽ
	void	ToLimitManually();			         //���е���е��λ��--�ֶ�����
	void	FromLimitToZeroManually();	//�ӻ�е��λ����λ��(��Ϊ�涨��λ��)--�ֶ�����
	void	BackToZeroManually();		    //�ص���Ϊ�涨����λ--�ֶ�����

	double		SetPosSpeedContents(int pos[], double time);               //��ģʽ���е��ض�λ��
	int		SetPosSpeedManually(int pos, int time);                       //���ֶ�����ֵ���е�ָ��λ��
	int		SetPosSpecialContents(int mode);                                 //��ģʽ���е��ض�λ��
	int		SetPosSpecialManually(int mode);
	int	    GetAbsolutePosition(int mode);					                   //С������ʱ�ľ���λ��

	void	RunContents();
	void	RunManually();
};


