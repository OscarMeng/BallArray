#pragma once

// CBall 命令目标

class CBallCurve;
class CBall : public CObject
{
public:
	//CBall();
	CBall(int nID, int nMotor, int nCenX, int nCenY, CBallCurve* pBallCurve);
	virtual ~CBall();
private:
	CBallCurve* m_pBallCurve;
	const int		m_nID;			     //小球编号
	const int		m_nMotor;		 //电机编号
	const int		m_nCenX;		//小球X坐标
	const int		m_nCenY;		//小球Y坐标

	double m_dRatio;                    //调整2D小球直接的间隔
	int		m_nLastPosition;	        //上一次位置
	int    	m_nCurrentPosition;	//当前位置(0~9999+BALL_MAX_POS)
	int		m_nNextPosition;	   //目标位置(0~9999+BALL_MAX_POS)
	int		m_nRelativePosition;//相对位置(从机械限位面到工作起始面)
	//int		m_nPos;				       //实际要运行到的位置
	double m_dSpeed;		          //小球移动速度:圈/min
	bool	    m_bSelected;		      //小球是否被选中
public:
	inline void SetNextPosition(int pos)		{ m_nLastPosition = m_nNextPosition; m_nNextPosition = pos; }//设置下一个位置
	inline void SetCurrtPosition(int pos)		{ m_nCurrentPosition=pos; }
	inline void SetSpeed(int speed)				{ m_dSpeed = speed; }
	inline void SetSelected(bool sel)			    { m_bSelected = sel; }
	inline int	GetID()							            { return m_nID; }
	inline int	GetMotor()					         	{ return m_nMotor; }
	inline int  GetCenX()					        	{ return m_nCenX; }
	inline int	GetCenY()					        	{ return m_nCenY; }
	inline int  GetLastPosition()			    	{ return m_nLastPosition; }
	inline int  GetNextPosition()			    	{ return m_nNextPosition; }
	//inline int  GetPos()						            { return m_nPos; }
	inline int	GetSpeed()						        { return m_dSpeed; }
	inline bool GetSelected()				    	{ return m_bSelected; }
public:
	void	SetCurrentPosition(DWORD nStartTime);
public:
	void Draw(CDC* pDC, CRect rRect);
	void Draw3D();

	double	CalSpeed(double time);  //根据运行时间、上次位置和下次位置计算速度，速度*已经运行时间转换为当前的位置
	double CalTime(double speed);//根据速度、上次位置和下次位置计算时间
};


