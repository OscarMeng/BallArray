#define SUDUONE		    50
#define SUDUONELIMIT  30
#define MAX_DEVICES    100
#define MAXCANET	        24
#define CANETMOTOR   35
#define MAXMOTOR	    1000
#define MAXDATALEN	    4000
#define MAXPRJ		        100

#define LINENUM		2000

#define PI			        3.1415926
#define PAN_D		    0.12  	     //绕线盘的直径：m
#define MAX_SPEED	3000		//最大速度：转/分

#define ELERUNTIME	0.2
#define MAXDATA		100000
#define MOTOID0		5
#define MOTOID1		40
#define MAXTIME		1		     //最大时间，5分钟
#define STOPTIME	    20		//消震时间（20秒）
#define IMGTOBGDIS  0

#define CANETPORT	5001
#define CANETIP		    31

#define TOPDATA			    '0'
#define BOTTOMDATA		'9'
#define BOTTOMDATA2		'9'
#define TOPDATA1		        5555
#define BOTTOMDATA1		8888
#define BOTTOMDATA3		9999
#define XIELV			            500

#define JIA_ZAI				0
#define WEI_ZHI				1
#define JI_TING				2
#define QU_JITING			3
#define SET_JUEDUI		4
#define SET_PID				5

#define SEND_GENDATA		6
#define WULI_ZERO			7
#define WORK_DIST			15
#define LTOS_DIST			    18

#define QI_DONG				9
#define HUI_ZERO			    10
#define SEND_SUDU			11
#define SEND_POSITION		13
#define SAVE_SET			    14

#define ACCELERATION	30

#define TYPE_3D			    1
#define TYPE_IMAGE		2
#define TYPE_STRING		3
#define TYPE_PAUSE		4
#define TYPE_CURVE		5
#define TYPE_ZERO		6

#define PATH3D			    "C:\\Temp\\3Dimage\\"
#define PATH2D			    "C:\\Temp\\2Dimage\\"
#define PATHPARAM		"C:\\Temp\\参数.ini"

#define CONTENTS_PATH	 "./contents.txt"
#define PARAM_PATH		     "./param.ini"

#define POS_TIME                          0.01
#define MAX_FRAME                  	10000		//插入最大的帧数
#define PROJECTS                          "Projects"
#define CURVE_DATA                     "CurveData"
#define BIN_DATA                           "BinData"
#define CURVE_DYNAMIC              "dynamic"
#define CURVE_STATIC                   "static"
#define CONFIGURE_FILE	            "ConfigFile"
#define CONFIGURE_NAME           "PrjConfig.ini"
#define BALL_COORDINATE           "BallCoordinate.txt"
#define ROWCOL_COORDINATE    "RowColNum.txt"

#define ALL_MOTOR		    99
#define LIMIT_POS		        (-30)
#define LIMIT_SPEED		    30
#define UNIT_SPEED		    50               //共同运行的正常速度：转/分
#define BALL_MAX_POS		9999.0        //小球运行的最大相对位置

#define MODE_CONTENTS		  1
#define MODE_MANUALLY		  2
#define MODE_LIMIT			      3
#define MODE_ZERO               4
#define MODE_CEIL                 5
#define MODE_START			     6
#define MODE_DOWN1			 7
#define MODE_DOWN2			 8
#define MODE_DOWN3		     9

#define PRJ_TYPE	      _T("type = ")
#define PRJ_FRAME	  _T("frame = ")
#define PRJ_TIME	      _T("time = ")

#define ROW_NUM	         23                                //小球的行数
#define COL_NUM	         35                               //小球的列数
#define BALLS_NUM	     805                             //小球的个数
#define CURVE_NUM	     50                               //一个工程最大的文件曲线数
#define WRITE_NUM	     161                            //805个小球按照要求分成5份，每份161个
#define IDT_RUN		         WM_USER+100
#define PROJECT_NUM   6;                                //工程文件信息数量
typedef struct ProjectInfo
{
	char  sCurveType[10];
	char  sCurveName[20];
	int    nCurveTime;
	int    nPauseTime;
	char sCurveUndet2[10];
	char sCurveUndet3[10];
} ProjectDir;                                              //工程文件信息

typedef struct CurveData
{
	char  sTitle[15];
	char  sData[100];
} ProjectData;                                          //数据文件信息

#pragma once
class CMyConstMeth
{
public:
	CMyConstMeth();
	~CMyConstMeth();
	CString ReadModuleFileNm();
};

