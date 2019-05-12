
//<<>-<>>---------------------------------------------------------------------()
/*

	Editeur de niveaux pour Dungeon Keeper II

Notes :

	o Structure de la carte :

		+00.B - Identificateur de l'�l�ment (en offset dans les tables
			d'�l�ments pour acc�l�rer l'affichage).
		+01.B - Identificateur du joueur
				1 : H�ros
				2 : Neutre
				3 : Gardien 1
				4 : Gardien 2
				5 : Gardien 3
				6 : Gardien 4
				7 : Gardien 5
		+02.B - Param�tres de l'�l�ment
				1 : Valeur par d�faut
				2 : Sur la lave - Ponts uniquement -
		+03.B - Type de table utilis�e
				0 : Table des �l�ments du monde
				1 : Table des salles
				2 : Table des portails de h�ros
				3 : Table des murs

	o D'une mani�re g�n�rale, lorsqu'une structure est modifi�e, toujours
	  s'arranger pour que les nouvelles options apparaissent � la fin de
	  la structure (notamment pour MAPPROPERTIES). Cela �vite que le
	  chargement des versions pr�c�dentes ne cause des probl�mes (appel �
          CopyMemory() qui risque d'effectuer une copie d�cal�e des donn�es).
	  Voir Editor_ExtractMapInfo() pour un exemple.

	o L'ordre d'apparition dans les tables de variables de MAPOPTIONS
	  correspond � celui des structures ELEMENT. Si jamais une de ces
	  structures est modifi�e (particuli�rement celles des cr�atures
	  qui sont consid�r�es comme encha�n�es), penser � modifier les
	  variables en cons�quence.

	o Aucun espace ne doit se trouver avant la premi�re touche d'un
	  raccourci clavier (sinon la reconnaissance ne s'effectue pas).
	  Ceci serait par exemple faux : Key[]=" Shift + A";, l'espace avant
	  Shift fausse la comparaison.

	o Les coordonn�es contenues dans MAPRECT correspondent aux
          coordonn�es de la carte (et non de la zone affichable � l'�cran).

	o Certaines fonctions assument qu'il n'est pas possible de dessiner
	  sur les limites ! (Cas de Things_DrawDoors() dans Things\Display.c).

	o La fonction Compiler_SetLastErrorEx() ne peut pas stocker les
	  messages d'erreur de Windows.

	o Lorsqu'une requ�te peut r�pondre elle-m�me (Bo�te "Ne plus..."),
	  penser � placer la bonne valeur dans defaultgadget de la structure
	  MESSAGEBOXSTRUCT. Pour une requ�te avec un seul choix, il est plus
	  logique de mettre cette valeur � 1 (ce qui correspond au seul
	  choix disponible).
									      */
//()-------------------------------------------------------------------<<>-<>>//


// �������������������������������������������������������������������������� //
// ���									  ��� //
// ��� Donn�es invariables						  ��� //
// ���									  ��� //
// �������������������������������������������������������������������������� //

#include "..\Includes\Editor.h"


// �������������������������������������������������������������������������� //
// ���									  ��� //
// ��� Donn�es variables						  ��� //
// ���									  ��� //
// �������������������������������������������������������������������������� //

HINSTANCE		hInstance;
HINSTANCE		hDirectSound;
HANDLE			AppMutex;
HHOOK			KeyHook;
HWND			hWnd;
HWND			hStatusWnd;
HMENU			hMenu;
HBITMAP			hSquare32;
FONTINFO		Font;
FONTINFO		FixedFont;
UINT			Timer;
DWORD			TickCount;
ULONG			AutoSaveTimer;
ULONG			SpeechUniqueID;

HWND			hMapHScroll;
HWND			hMapVScroll;
HWND			hMapZoom;
HICON			hMapZoomIcon;
HICON			hMapCameras[5];
LONG			MapZoomCX = MAP_DEFAULTZOOMX;
LONG			MapZoomCY = MAP_DEFAULTZOOMY;
LONG			MapStartX = 0;
LONG			MapStartY = 0;
COLORREF		MapRectExColors[6] = {0x00FFFF00,0x00FFFF00,0x00AAAAAA,0x00AAAAAA,0x007F00FF,0x007F00FF};
MAPRECT			MapCursorRect = {0,0,1,1,FALSE};
MAPRECT			MapCursorSel = {0,0,0,0,TRUE,0x00FFFFFF,0x00000000,NULL};
MAPRECT			MapCursorHigh = {0,0,1,1,TRUE};
LONG			MapCursorMinWidth = 1;
LONG			MapCursorMinHeight = 1;
LONG			MapCursorMaxWidth = 0;
LONG			MapCursorMaxHeight = 0;
UINT			MapCursorMode = 0;
MAPRECT			MapCursorPen;
MAPRECT			MapCursorPenOld;
MAPCURSOR		MapCursor;
MAPCURSOR		MapCursorV;
MAPCURSOR		MapCursorH;
MAPCURSOR		MapCursorX;
LONG			MapChanges;
LONG			MapLocked;
UINT			MapFileType;

NODE			MapCreatures;
NODE			MapDoors;
NODE			MapTraps;
NODE			MapObjects;
NODE			MapMagicalObjects;
NODE			MapHerosBands;
NODE			MapAreas;
NODE			MapGates;
NODE			MapEffects;
NODE			MapActionPoints;
NODE			MapTriggers;
NODE			MapActions;
MAPTHING		MapImmCreature		= {{NULL,NULL,THING_CREATURE},	   0,0,0x00000001,0,100,0,{1},0,0x09,0x13,0,0,0};
MAPTHING		MapImmDoor 		= {{NULL,NULL,THING_DOOR},	   0,0,0x00000006,0,  0,0,{0},0,0x00,0x00,0,0,0};
MAPTHING		MapImmTrap		= {{NULL,NULL,THING_TRAP},	   0,0,0x00000004,0,  0,0,{4},0,0x00,0x00,0,0,0};
MAPTHING		MapImmObject		= {{NULL,NULL,THING_OBJECT},	   0,0,0x00000001,2,  0,0,{0},0,0x00,0x00,0,0,0};
MAPTHING		MapImmMagicalObject	= {{NULL,NULL,THING_MAGICALOBJECT},0,0,0x00000075,2,  0,0,{0},0,0x00,0x00,0,0,0};
MAPFIND			MapFind;
MAPEDITSTATS		MapStats;
DEBUGVERSIONS		MapVersions;
MAPSOUNDS		MapSounds;
IDirectSound*		MapDirectSound;
IDirectSoundBuffer*	MapSoundBuffer;

HWND			hToolbar;
HBRUSH			hToolbrush;

HWND			hMinimap;
HBITMAP			hMinimapBitmap;
BOOL			MinimapIsMoving;

HWND			hInfo;
HICON			hInfoRectIcon;
HICON			hInfoCopyIcon;
HICON			hInfoGoldIcon;
ULONG			InfoMemoryUsedByUndo;

HWND			hSplash;
HDC			hSplashDC;
HBITMAP			hSplashBitmap;
HBITMAP			hSplashOldBitmap;
FONTINFO		SplashFont;

DWORD			WorldItemID = 0x02020101;
DWORD			ToolsItemID = PAINTMODE_RECTFILL;
DWORD			ToolsPlyrID = 0;
DWORD			ToolsGateID = 0;
UINT			SelectLast  = 0;

MAPTHING		ManagerCreature		= {{NULL,NULL,THING_CREATURE},	   0,0,0x00000001,0,100,0,{1},0,0x09,0x13,0,0,0};
MAPTHING		ManagerDoor 		= {{NULL,NULL,THING_DOOR},	   0,0,0x00000006,0,  0,0,{0},0,0x00,0x00,0,0,0};
MAPTHING		ManagerTrap		= {{NULL,NULL,THING_TRAP},	   0,0,0x00000004,0,  0,0,{4},0,0x00,0x00,0,0,0};
MAPTHING		ManagerObject		= {{NULL,NULL,THING_OBJECT},	   0,0,0x00000001,2,  0,0,{0},0,0x00,0x00,0,0,0};
MAPTHING		ManagerMagicalObject	= {{NULL,NULL,THING_MAGICALOBJECT},0,0,0x00000075,2,  0,0,{0},0,0x00,0x00,0,0,0};
LONG			ManagerX		= 1;
LONG			ManagerY		= 1;

NODE			UndoList;

HIMAGELIST		hWorld;
HIMAGELIST		hRooms;
HIMAGELIST		hGates;
HIMAGELIST		hWalls;
HIMAGELIST		hDrawingTools;
HIMAGELIST		hMiscTools;

HIMAGELIST		MapWorld;
HIMAGELIST		MapWorldGates;
HIMAGELIST		MapRoomsWalls;
HIMAGELIST		MapRoomsHeros;
HIMAGELIST		MapRoomsNeutral;
HIMAGELIST		MapRoomsKeeper1;
HIMAGELIST		MapRoomsKeeper2;
HIMAGELIST		MapRoomsKeeper3;
HIMAGELIST		MapRoomsKeeper4;
HIMAGELIST		MapRoomsKeeper5;

MAP			Map;
MAPOPTIONS		MapOptions[MAP_MAXPLAYERS];
MAPCOPY			MapCopy;
NODE			MapCopyThings;
GLOBALVARS		MapGlobalVariables;
CONFIG			Config;

HWND			hFullWnd;
HBITMAP			hFullMap;
HWND			hFullRoom;
HBITMAP			hFullRoomIcon;
DWORD			hFullRoomID;
LONG			FullDoorsOffset   = 0;
LONG			FullTrapsOffset   = 0;
LONG			FullNeutralOffset = 0;
LONG			FullNeutralDelay  = 0;
LONG			FullRoomOffset    = 2;

HWND			hThumbWnd;
HWND			hThumbSizeWnd;
HWND			hThumbFlagsWnd;
HWND			hThumbRefreshWnd;
HWND			hThumbSaveWnd;
HBITMAP			hThumbBck;
HBITMAP			hThumbBmp;
LONG			ThumbSize = 144;

HWND			hDiskWnd;
HWND			hDiskStaticWnd;
HWND			hDiskProgressWnd;
UINT			DiskMethod;
char			Password[64];

COMPILERTASK*		CompilerTask;
LONG			CompilerGlobal;
DWORD			CompilerErrorCode;
char			CompilerError[256];

NODE			RecentFiles;
LONG			RecentItems;

LONG			LocaleGlobal;
char			Locale[LOCALE_MAXLEN*7+LOCALE_MAXLEN*12];

char			TriggersVariables[TRIGGERS_MAXVARIABLES][TRIGGERS_MAXVARLEN];
char			TriggersTimers[TRIGGERS_MAXTIMERS][TRIGGERS_MAXTIMLEN];

// �������������������������������������������������������������������������� //
// �������������������������������������������������������������������������� //
// END

