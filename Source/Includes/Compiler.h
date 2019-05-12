
/*** Drapeaux d'information *********************************************/

#define COMPILER_QUERYALL	0x00000001
#define COMPILER_QUERYIMPS	0x00000002
#define COMPILER_QUERYMAPGOLD	0x00000004
#define COMPILER_QUERYMAPGEMS	0x00000008
#define COMPILER_QUERYOBJGOLD	0x00000010
#define COMPILER_QUERYMGCGOLD	0x00000020
#define COMPILER_QUERYWORLD	0x00000040
#define COMPILER_QUERYPLAYERS	0x00000080

/*** Identificateurs ****************************************************/

#define COMPILER_COMPILE	0
#define COMPILER_TEST		1
#define COMPILER_RUN		2

#define COMPILER_PBMBEGIN	0
#define COMPILER_PBMANLVER	1
#define COMPILER_PBMANLMAP	2
#define COMPILER_PBMANLPLAYERS	3
#define COMPILER_PBMANLTHINGS	4
#define COMPILER_PBMANLAREAS	5
#define COMPILER_PBMANLGATES	6
#define COMPILER_PBMANLTRIGGERS	7
#define COMPILER_PBMANLDONE	8
#define COMPILER_PBMWRITEINF	9
#define COMPILER_PBMWRITEPLY	10
#define COMPILER_PBMWRITETHS	11
#define COMPILER_PBMWRITEMAP	12
#define COMPILER_PBMWRITEVAR	13
#define COMPILER_PBMWRITETRS	14
#define COMPILER_PBMWRITEGLT	15
#define COMPILER_PBMWRITEGLR	16
#define COMPILER_PBMWRITEGLD	17
#define COMPILER_PBMWRITEGLA	18
#define COMPILER_PBMWRITEGLS	19
#define COMPILER_PBMWRITEGLC	20
#define COMPILER_PBMWRITEGLO	21

#define COMPILER_PBMCMPMAX	21
#define COMPILER_PBMTSTMAX	7
#define COMPILER_PBMRUNMAX	21

/*** Structures *********************************************************/

typedef struct COMPILERTASK {
	HWND		 hWnd;
	HWND		 hText;
	HWND		 hList;
	HWND		 hProgress;
	HWND		 hButton;
	HIMAGELIST	 hImageList;
	UINT		 Method;
	BOOL		 RepairTriggers;
	BOOL		 Generate;
	BOOL		 Failed;
	NODE		 Errors;
	LONG		 Warnings;
	BYTE		*MapCtrl;
	char		 MapPath[MAX_PATH];
	char		 MapKill[MAX_PATH];
	char		 Date[256];
	char		 Text[4096];
} COMPILERTASK;
