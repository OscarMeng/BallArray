#pragma once

// CBallCurve 命令目标
class CBall;
class CBallCurve : public CObject
{
public:
	CBallCurve();
	virtual ~CBallCurve();
public:
	static const int	m_nRadius = 100;		        //小球的半径
	static const int	m_nBallNum = 35 * 23;		//小球的总数
	static const int	m_nRowBallNum = 35;       //小球的行数
	static const int	m_nColBallNum = 23;        //小球的列数

	static const int	m_nBallsX = 6800;	           //小球矩阵正X轴
	static const int	m_nBallsY = 4400;	           //小球矩阵正Y轴
	static const int	m_nMargin = 1000;
	static const int	m_nCanetNum = 10;
	static const int	m_nTimerTime = 100;
	const double       m_dMaxZoom;
	const double       m_dMinZoom;

	CString     m_sPath;                  //软件路径
	CFont		m_textFont;				 //文字字体
private:
	CPoint	m_ptStartPoint;			//开始点
	CPoint	m_pt2DCenterPoint;	//2D中心点
	double	m_dZoom;					//缩放比
	bool	m_bTextOn;					    //是否显示文字
	double  m_dXRotation;           //绕X轴旋转度数
	COLORREF m_colBackGround;        //界面背景颜色
	COLORREF m_colCoordinate;		    //2D坐标轴颜色
	COLORREF m_colBoardNormal;		//普通状态下边框的颜色
	COLORREF m_colBoardSelected;  	//选中时边框颜色
	double	 m_colUpH;				//最高点HSV颜色H值
	double	 m_colUpS;				//最高点HSV颜色S值
	double	 m_colUpV;				//最高点HSV颜色V值
	double	 m_colDownH;			//最低点HSV颜色H值
	double	 m_colDownS;			//最低点HSV颜色S值
	double	 m_colDownV;			//最低点HSV颜色V值

	int		 m_nPosLimit;	              //机械限位面
	int		 m_nPosZero;	             //小球回零面
	int		 m_nPosCeil;	             //天花板平面
	int		 m_nPosStart;	             //小球起始面
	int		 m_nPosDown1;	        //小球结束面1
	int		 m_nPosDown2;			//小球结束面2
	int		 m_nPosDown3;			//小球结束面3
	int         m_nRelStartPos;        //小球工作面的相对位置
	bool	     m_bAutoRotation;	    //自动旋转
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
	inline int		    GetPosLimit()							    { return m_nPosLimit; }                //小球工作的限位面
	inline int		    GetPosZero()							    { return m_nPosZero; }                //小球工作的回零面
	inline int		    GetPosCeil()							    { return m_nPosCeil; }                 //小球工作的天花板面
	inline int		    GetPosStart()								{ return m_nPosStart; }                //小球工作的起始面
	inline int		    GetPosDown1()							{ return m_nPosDown1; }            //小球工作的结束面1
	inline int		    GetPosDown2()							{ return m_nPosDown2; }            //小球工作的结束面2
	inline int		    GetPosDown3()							{ return m_nPosDown3; }            //小球工作的结束面3
	inline bool		GetAutoRotation()						{ return m_bAutoRotation; }
public:
	CBall*		m_pBalls[m_nBallNum];	 //小球
 	void		    SetCurrentBall(int n,int nheight);//调试小球位置
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
	void	Draw(CDC* pDC, CRect rRect);                                                 //画二维小球
	void	Draw3D();                                                                                 //画三维小球
	COLORREF	CalGradientColor(double index, double range);          //计算高度与颜色的关系
public:
	void	LoadMotorSetMode();			      //初始化，加载电机，设置位置模式
	void	ToLimitManually();			         //运行到机械限位处--手动操作
	void	FromLimitToZeroManually();	//从机械限位到零位，(人为规定的位置)--手动操作
	void	BackToZeroManually();		    //回到人为规定的零位--手动操作

	double		SetPosSpeedContents(int pos[], double time);               //以模式运行到特定位置
	int		SetPosSpeedManually(int pos, int time);                       //以手动输入值运行到指定位置
	int		SetPosSpecialContents(int mode);                                 //以模式运行到特定位置
	int		SetPosSpecialManually(int mode);
	int	    GetAbsolutePosition(int mode);					                   //小球运行时的绝对位置

	void	RunContents();
	void	RunManually();
};


