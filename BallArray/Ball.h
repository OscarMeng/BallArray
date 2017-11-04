#pragma once

// CBall ����Ŀ��

class CBallCurve;
class CBall : public CObject
{
public:
	//CBall();
	CBall(int nID, int nMotor, int nCenX, int nCenY, CBallCurve* pBallCurve);
	virtual ~CBall();
private:
	CBallCurve* m_pBallCurve;
	const int		m_nID;			     //С����
	const int		m_nMotor;		 //������
	const int		m_nCenX;		//С��X����
	const int		m_nCenY;		//С��Y����

	double m_dRatio;                    //����2DС��ֱ�ӵļ��
	int		m_nLastPosition;	        //��һ��λ��
	int    	m_nCurrentPosition;	//��ǰλ��(0~9999+BALL_MAX_POS)
	int		m_nNextPosition;	   //Ŀ��λ��(0~9999+BALL_MAX_POS)
	int		m_nRelativePosition;//���λ��(�ӻ�е��λ�浽������ʼ��)
	//int		m_nPos;				       //ʵ��Ҫ���е���λ��
	double m_dSpeed;		          //С���ƶ��ٶ�:Ȧ/min
	bool	    m_bSelected;		      //С���Ƿ�ѡ��
public:
	inline void SetNextPosition(int pos)		{ m_nLastPosition = m_nNextPosition; m_nNextPosition = pos; }//������һ��λ��
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

	double	CalSpeed(double time);  //��������ʱ�䡢�ϴ�λ�ú��´�λ�ü����ٶȣ��ٶ�*�Ѿ�����ʱ��ת��Ϊ��ǰ��λ��
	double CalTime(double speed);//�����ٶȡ��ϴ�λ�ú��´�λ�ü���ʱ��
};


