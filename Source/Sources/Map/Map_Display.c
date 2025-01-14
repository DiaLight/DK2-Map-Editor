
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines d'affichage de la carte.
									      */
//()-------------------------------------------------------------------<<>-<>>//

// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� Donn�es								  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

#include "..\Includes\Editor.h"
#include "..\Includes\Prototypes.h"
#include "..\Includes\Structures.h"
#include "..\Includes\Texts.h"

extern HWND		hWnd;
extern FONTINFO		Font;
extern LONG		MapLocked;
extern MAP		Map;
extern LONG		MapZoomCX;
extern LONG		MapZoomCY;
extern LONG		MapStartX;
extern LONG		MapStartY;
extern LONG		MapCursorMinWidth;
extern LONG		MapCursorMaxWidth;
extern LONG		MapCursorMinHeight;
extern LONG		MapCursorMaxHeight;
extern LONG		MapChanges;
extern HIMAGELIST	MapWorld;
extern HIMAGELIST	MapWorldGates;
extern HIMAGELIST	MapRoomsWalls;
extern HIMAGELIST	MapRoomsHeros;
extern HIMAGELIST	MapRoomsNeutral;
extern HIMAGELIST	MapRoomsKeeper1;
extern HIMAGELIST	MapRoomsKeeper2;
extern HIMAGELIST	MapRoomsKeeper3;
extern HIMAGELIST	MapRoomsKeeper4;
extern HIMAGELIST	MapRoomsKeeper5;
extern MAPCURSOR	MapCursorX;
extern MAPCURSOR	MapCursorH;
extern MAPRECT		MapCursorRect;
extern MAPEDITSTATS	MapStats;
extern DWORD		ToolsItemID;
extern CONFIG		Config;


// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� Bloque et d�bloque l'affichage de la carte			  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

void Map_Lock(BOOL Lock, BOOL Update)
{
	if (Lock)
		{
		MapLocked++;
		if (MapLocked-1) return;
		}

	if (!Lock)
		{
		if (!MapLocked) return;
		MapLocked--;
		if (MapLocked) return;
		}

	if (Update) Map_Refresh();
	return;
}


// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� Affichage de la carte						  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

// �算� Mise � jour de la zone affichable ��������������������������������

void Map_Refresh()
{
	RECT	MapRect;

	Map_GetMapArea(hWnd,&MapRect);
	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	return;
}


// �算� Traduction des codes binaires en donn�es graphiques ��������������

void Map_Paint(HWND hWnd, HDC hDC)
{
	BYTE	*MapPtr;
	BYTE	*MapSavedPtr;
	HIMAGELIST	 MapImageList;
	RECT		 MapRect;
	RECT		 ClipRect;
	LONG		 DX,DY;
	LONG		 MX,MY;
	LONG		 X,Y;
	long		 i,j;

	Map_GetMapArea(hWnd,&MapRect);
	if ((!Map.Map)||(MapLocked)) goto Draw_BlackRect;
	if (!(Config.MapViewFlags&MVF_TERRAIN)) goto Draw_BlackRect;
	Map_GetMapSquares(NULL,&MapRect,&DX,&DY);

	if (DX+MapStartX > Map.Properties.Width) DX = Map.Properties.Width;
	if (DY+MapStartY > Map.Properties.Height) DY = Map.Properties.Height;
	X = MapRect.left;
	Y = MapRect.top;
	MX = 0;
	MY = 0;
	MapPtr = Map.Map+MapStartX*MAP_CONTROLBYTES+(MapStartY*Map.Properties.Width*MAP_CONTROLBYTES);

	for (i = 0; i != DY; i++)
		{
		MapSavedPtr = MapPtr;
		MX = 0;

		for (j = 0; j != DX; j++)
			{
			if (MapPtr >= MapSavedPtr+Map.Properties.Width*MAP_CONTROLBYTES-MapStartX*MAP_CONTROLBYTES) break;

			switch(*(MapPtr+3))
				{
				case 0:	MapImageList = MapWorld;
					break;
				case 1:	switch(*(MapPtr+1))
						{
						case 1:	MapImageList = MapRoomsHeros;
							break;
						case 2:	MapImageList = MapRoomsNeutral;
							break;
						case 3:	MapImageList = MapRoomsKeeper1;
							break;
						case 4:	MapImageList = MapRoomsKeeper2;
							break;
						case 5:	MapImageList = MapRoomsKeeper3;
							break;
						case 6:	MapImageList = MapRoomsKeeper4;
							break;
						case 7:	MapImageList = MapRoomsKeeper5;
							break;
						default:MapImageList = MapRoomsNeutral;
							break;
						}
					break;
				case 2:	MapImageList = MapWorldGates;
					break;
				case 3: MapImageList = MapRoomsWalls;
					break;
				}

			ImageList_DrawEx(MapImageList,*MapPtr,hDC,X,Y,MapZoomCX,MapZoomCY,CLR_DEFAULT,CLR_DEFAULT,ILD_NORMAL);
//			ImageList_Draw(MapImageList,*MapPtr,hDC,X,Y,ILD_NORMAL);

			X += MapZoomCX;
			MX += MapZoomCX;
			MapPtr += MAP_CONTROLBYTES;
			}

		MapPtr = MapSavedPtr+Map.Properties.Width*MAP_CONTROLBYTES;
		if (MapPtr >= Map.Map+Map.Properties.Width*MAP_CONTROLBYTES*Map.Properties.Height) break;
		Y += MapZoomCY;
		MY += MapZoomCY;
		X = MapRect.left;
		}

	if (MX < MapRect.right)
		{
		ClipRect.left = MX+MapRect.left;
		ClipRect.right = MapRect.left+MapRect.right;
		ClipRect.top = MapRect.top;
		ClipRect.bottom = MapRect.bottom;
		FillRect(hDC,&ClipRect,GetStockObject(BLACK_BRUSH));
		}

	if (MY < MapRect.bottom-MapRect.top)
		{
		ClipRect.left = MapRect.left;
		ClipRect.right = MX+MapRect.left;
		ClipRect.top = MY+MapZoomCY+MapRect.top;
		ClipRect.bottom = MapRect.bottom;
		FillRect(hDC,&ClipRect,GetStockObject(BLACK_BRUSH));
		}

	return;

//--- Affichage d'une carte vide ---

Draw_BlackRect:
	FillRect(hDC,&MapRect,GetStockObject(BLACK_BRUSH));
	return;
}


// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� Affichage d'un rectangle						  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

void Map_DrawRectangle(HWND hWnd, HDC hDC, MAPRECT *Rect)
{
	RECT			MapArea;
	HPEN			RectPen,OldPen;
	MAPRECT	MapRect;
	POINT		Limits[5];
	long			DX,DY;

	if (MapLocked) return;
	if (Rect->hidden) return;

	Map_GetMapArea(hWnd,&MapArea);
	Map_GetMapSquares(NULL,&MapArea,&DX,&DY);

	/* Invert coordinates in case of X1,Y1 is after X2,Y2 */

	if (Rect->x1 > Rect->x2)
		{
		MapRect.x1 = Rect->x2;
		MapRect.x2 = Rect->x1;
		}
	else
		{
		MapRect.x1 = Rect->x1;
		MapRect.x2 = Rect->x2;
		}

	if (Rect->y1 > Rect->y2)
		{
		MapRect.y1 = Rect->y2;
		MapRect.y2 = Rect->y1;
		}
	else
		{
		MapRect.y1 = Rect->y1;
		MapRect.y2 = Rect->y2;
		}

	/* Correct rectangle positions according to map position */

	MapRect.x1 -= MapStartX;
	MapRect.y1 -= MapStartY;
	MapRect.x2 -= MapStartX;
	MapRect.y2 -= MapStartY;

	/* Do not draw outbounds rectangles */

	if (MapRect.x2 < 0) return;
	if (MapRect.y2 < 0) return;
	if (MapRect.x1 > DX) return;
	if (MapRect.y1 > DY) return;

	/* Convert Map coordinates into Client coordinates */

	MapRect.x1 *= MapZoomCX;
	MapRect.y1 *= MapZoomCY;
	MapRect.x2 *= MapZoomCX;
	MapRect.y2 *= MapZoomCY;
	MapRect.x1 += MapArea.left;
	MapRect.y1 += MapArea.top;
	MapRect.x2 += MapArea.left+MapZoomCX-1;
	MapRect.y2 += MapArea.top+MapZoomCY-1;

	/* Draw rectangle name if possible */

	if (Rect->name)
		{
		COLORREF OldTextColor;
		UINT	 OldTextMode;
		HFONT	 OldTextFont;
		RECT	 TextRect;

		OldTextColor = SetTextColor(hDC,0x00000000);
		OldTextMode = SetBkMode(hDC,TRANSPARENT);
		OldTextFont = SelectObject(hDC,Font.hFont);
		TextRect.left = MapRect.x1;
		TextRect.right = MapRect.x2;
		TextRect.top = MapRect.y1;
		TextRect.bottom = MapRect.y2;
		if (TextRect.left < MapArea.left) TextRect.left = MapArea.left;
		if (TextRect.top < MapArea.top) TextRect.top = MapArea.top;
		if (TextRect.right > MapArea.right) TextRect.right = MapArea.right;
		if (TextRect.bottom > MapArea.bottom) TextRect.bottom = MapArea.bottom;
		TextRect.left += 3;
		TextRect.right -= 3;
		TextRect.top += 3;
		TextRect.bottom -= 3;
		DrawText(hDC,Rect->name,-1,&TextRect,DT_LEFT);
		--TextRect.left;
		TextRect.right++;
		--TextRect.top;
		TextRect.bottom++;
		SetTextColor(hDC,Rect->textcolor);
		DrawText(hDC,Rect->name,-1,&TextRect,DT_LEFT);
		SelectObject(hDC,OldTextFont);
		SetBkMode(hDC,OldTextMode);
		SetTextColor(hDC,OldTextColor);
		}

	/* Draw (partially) the rectangle */

	RectPen = CreatePen(PS_SOLID,0,Rect->color);
	if (RectPen)
		{
		OldPen = SelectObject(hDC,RectPen);
		if (OldPen)
			{
			Limits[0].x = MapRect.x1;
			Limits[0].y = MapRect.y1;
			Limits[1].x = MapRect.x2;
			Limits[1].y = MapRect.y1;
			Limits[2].x = MapRect.x2;
			Limits[2].y = MapRect.y2;
			Limits[3].x = MapRect.x1;
			Limits[3].y = MapRect.y2;
			Limits[4].x = MapRect.x1;
			Limits[4].y = MapRect.y1;
			Polyline(hDC,Limits,5);
			SelectObject(hDC,OldPen);
			}
		DeleteObject(RectPen);
		}

	return;
}


// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� Affichage du curseur						  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

void Map_PaintCursor(HWND hWnd, HDC hDC, MAPRECT *Rect, MAPCURSOR *Cursor, BOOL RealCursor)
{
	RECT			 MapRect;
	DWORD			 MapCursor;
	ELEMENT			*MapTable;
	MAPCURSOR		*MapTableCursor;
	MAPCLIENT		 MapClient;
	BYTE		*MapPtr;
	BYTE		 MapTableOffset;
	long		 SX,SY;
	long		 X,Y;
	long		 i,j;

	if (Rect->hidden) return;
	if (MapLocked) return;
	if (!Map_RectangleToClient(hWnd,Rect,&MapClient)) return;

	//--- Dessine le curseur dans la zone temporaire ---

	if (RealCursor)
		{
		if (((MapCursorMinWidth == MapCursorMaxWidth)&&(MapCursorMinHeight == MapCursorMaxHeight))||(MapCursorMinWidth != 1)||(MapCursorMinHeight != 1))
			MapCursor = PAINTMODE_RECTFILL;
		else
			MapCursor = ToolsItemID;
		}
	else
		MapCursor = PAINTMODE_RECTFILL;

	Map_PaintTemporaryElement(Rect,MapCursor);

	//--- Transpose les donn�es sur le carte ---

	MapPtr = Map_GetMapPtrFromClientPt(hWnd,MapClient.X,MapClient.Y);
	MapClient.Width /= MapZoomCX;
	MapClient.Height /= MapZoomCY;
	Map_GetMapArea(hWnd,&MapRect);
	MapTableCursor = Cursor;
	SX = (MapClient.X-MapRect.left)/MapZoomCX+MapStartX;
	SY = (MapClient.Y-MapRect.top)/MapZoomCY+MapStartY;

	for (Y = SY, i = 0; i != MapClient.Height; i++, Y++)
		for (X = SX, j = 0; j != MapClient.Width; j++, X++)
			{
			if (!GetPixel(Map.hDC,X,Y)) continue;

			if (RealCursor)
				{
				MapTableCursor = Cursor;
				MapTableOffset = *(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES);
				switch(*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+3))
					{
					case 0:	if (World[MapTableOffset].Id == 0xFE020101)
							{
							MapTableCursor = &MapCursorX;
							MapTable = NULL;
							break;
							}
						MapTable = World;
						break;
					case 1:	MapTable = Rooms;
						break;
					case 2:	MapTable = Gates;
						break;
					case 3:	MapTable = Walls;
						break;
					}
				if ((MapTable)&&(ToolsItemID < PAINTMODE_CREATURE)&&(Cursor != &MapCursorX))
					{
					if ((MapTable[MapTableOffset].MinWidth > 1)||(MapTable[MapTableOffset].MinHeight > 1)||(MapTable[MapTableOffset].MaxWidth)||(MapTable[MapTableOffset].MaxHeight))
						MapTableCursor = &MapCursorH;
					}
				}

			BitBlt(hDC,MapClient.X+j*MapZoomCX,MapClient.Y+i*MapZoomCY,MapZoomCX,MapZoomCY,MapTableCursor->hdc,0,0,SRCPAINT);
			}

	return;
}


// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� Affichage d'�l�ments dans la zone temporaire			  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

void Map_PaintTemporaryElement(MAPRECT *Rect, DWORD Mode)
{
	MAPCLIENT	MapClient;
	POINT		MapLines[5];

	MapClient.X = 0;
	MapClient.Y = 0;
	MapClient.Width = Map.Properties.Width;
	MapClient.Height = Map.Properties.Height;
	FillRect(Map.hDC,(RECT *)&MapClient,GetStockObject(BLACK_BRUSH));

	Map_CorrectRect(Rect,&MapClient);

	if ((MapClient.Width == 1)&&(MapClient.Height == 1))
		{
		SetPixelV(Map.hDC,MapClient.X,MapClient.Y,0x00FFFFFF);
		return;
		}

	switch(Mode)
		{
		case PAINTMODE_RECT:
		case PAINTMODE_RECTRND:
			MapClient.Width--;
			MapClient.Height--;
			if (!MapClient.Width) MapClient.Width++;
			if (!MapClient.Height) MapClient.Height++;
			MapLines[0].x = MapClient.X;
			MapLines[0].y = MapClient.Y;
			MapLines[1].x = MapClient.X+MapClient.Width;
			MapLines[1].y = MapClient.Y;
			MapLines[2].x = MapClient.X+MapClient.Width;
			MapLines[2].y = MapClient.Y+MapClient.Height;
			MapLines[3].x = MapClient.X;
			MapLines[3].y = MapClient.Y+MapClient.Height;
			MapLines[4].x = MapClient.X;
			MapLines[4].y = MapClient.Y;
			Polyline(Map.hDC,MapLines,5);
			break;

		case PAINTMODE_RECTFILL:
		case PAINTMODE_RECTFILLRND:
			MapClient.Width += MapClient.X;
			MapClient.Height += MapClient.Y;
			FillRect(Map.hDC,(RECT *)&MapClient,Map.hBrush);
			break;

		case PAINTMODE_CIRCLE:
		case PAINTMODE_CIRCLERND:
			if ((MapClient.Width == 1)&&(MapClient.Height != 1))
				{
				MoveToEx(Map.hDC,MapClient.X,MapClient.Y,NULL);
				LineTo(Map.hDC,MapClient.X,MapClient.Y+MapClient.Height);
				break;
				}
			if ((MapClient.Width != 1)&&(MapClient.Height == 1))
				{
				MoveToEx(Map.hDC,MapClient.X,MapClient.Y,NULL);
				LineTo(Map.hDC,MapClient.X+MapClient.Width,MapClient.Y);
				break;
				}
			SelectObject(Map.hDC,GetStockObject(BLACK_BRUSH));
			Ellipse(Map.hDC,MapClient.X,MapClient.Y,MapClient.X+MapClient.Width,MapClient.Y+MapClient.Height);
			break;

		case PAINTMODE_CIRCLEFILL:
		case PAINTMODE_CIRCLEFILLRND:
			if ((MapClient.Width == 1)&&(MapClient.Height != 1))
				{
				MoveToEx(Map.hDC,MapClient.X,MapClient.Y,NULL);
				LineTo(Map.hDC,MapClient.X,MapClient.Y+MapClient.Height);
				break;
				}
			if ((MapClient.Width != 1)&&(MapClient.Height == 1))
				{
				MoveToEx(Map.hDC,MapClient.X,MapClient.Y,NULL);
				LineTo(Map.hDC,MapClient.X+MapClient.Width,MapClient.Y);
				break;
				}
			SelectObject(Map.hDC,Map.hBrush);
			Ellipse(Map.hDC,MapClient.X,MapClient.Y,MapClient.X+MapClient.Width,MapClient.Y+MapClient.Height);
			break;

		case PAINTMODE_LINE:
		case PAINTMODE_LINERND:
			MoveToEx(Map.hDC,Rect->x1,Rect->y1,NULL);
			LineTo(Map.hDC,Rect->x2,Rect->y2);
			SetPixelV(Map.hDC,Rect->x2,Rect->y2,0x00FFFFFF);
			break;
		}

	return;
}


// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� Sous-Routines de dessin						  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

// �算� Remplissage d'une zone d�limit�e ���������������������������������

void Map_FillArea(HWND hWnd, LONG X, LONG Y)
{
	RECT		 MapRect;
	BOOL		 Restart;
	DWORD		 MapFullID;
	DWORD		 MapDestID;
	BYTE	 MapID;
	BYTE	*MapPtr,*MapLineBegin,*MapLineEnd;
	long	 i;

	//--- Initialisations ---

	Map_GetMapArea(hWnd,&MapRect);
	X -= MapRect.left;
	Y -= MapRect.top;
	X /= MapZoomCX;
	Y /= MapZoomCY;
	X += MapStartX;
	Y += MapStartY;
	if ((X >= Map.Properties.Width)||(Y >= Map.Properties.Height)) return;

	MapFullID = Map_TranslateWorldItem();
	if (MapFullID == 0xFFFFFFFF) return;

	MapPtr = Map.Map+X*MAP_CONTROLBYTES+Y*Map.Properties.Width*MAP_CONTROLBYTES;
	MapID = Map_TranslateID(0xFE,0x01,World);
	if ((*MapPtr == MapID)&&(*(MapPtr+3) == 0))
		{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
		}

	//--- Converti temporairement les zones cibles possibles ---

	MapPtr = Map.Map+X*MAP_CONTROLBYTES+Y*Map.Properties.Width*MAP_CONTROLBYTES;
	MapDestID = (DWORD)((*MapPtr)<<24);
	MapDestID |= (DWORD)((*(MapPtr+1))<<16);
	MapDestID |= (DWORD)((*(MapPtr+2))<<8);
	MapDestID |= (DWORD)(*(MapPtr+3));

	for (MapPtr = Map.Map, i = 0; i != Map.Properties.Width*Map.Properties.Height; i++)
		if ( (*(MapPtr+i*MAP_CONTROLBYTES) == (BYTE)(MapDestID>>24)) && (*(MapPtr+i*MAP_CONTROLBYTES+1) == (BYTE)(MapDestID>>16)) && (*(MapPtr+i*MAP_CONTROLBYTES+2) == (BYTE)(MapDestID>>8)) && (*(MapPtr+i*MAP_CONTROLBYTES+3) == (BYTE)(MapDestID)) )
			*(MapPtr+i*MAP_CONTROLBYTES) = 0xFD;

	//--- Routine de remplissage ---

	MapPtr = Map.Map+X*MAP_CONTROLBYTES+Y*Map.Properties.Width*MAP_CONTROLBYTES;
	MapLineBegin = Map.Map+Y*Map.Properties.Width*MAP_CONTROLBYTES;
	MapLineEnd = MapLineBegin+Map.Properties.Width*MAP_CONTROLBYTES;

	/* Step 1 : Fills first line in order to prepare flood fill */
	// Fills the current line to the right
	for (i = 0;; i++)
		{
		if (MapPtr+i*MAP_CONTROLBYTES >= Map.Map+Map.Properties.Width*MAP_CONTROLBYTES*Map.Properties.Height)
			break;
		if (MapPtr+i*MAP_CONTROLBYTES >= MapLineEnd)
			break;
		if (*(MapPtr+i*MAP_CONTROLBYTES) != 0xFD)
			break;
		*(MapPtr+i*MAP_CONTROLBYTES) = 0xFF;
		}
	// Fills the current line to the left
	for (i = 1;; i++)
		{
		if (MapPtr-i*MAP_CONTROLBYTES < MapLineBegin)
			break;
		if (MapPtr-i*MAP_CONTROLBYTES < Map.Map)
			break;
		if (*(MapPtr-i*MAP_CONTROLBYTES) != 0xFD)
			break;
		*(MapPtr-i*MAP_CONTROLBYTES) = 0xFF;
		}

	/* Step 2 : Process the entiere area starting at line+1 */
Fill_Loop:
	Restart = FALSE;
	for (MapPtr = MapLineBegin+Map.Properties.Width*MAP_CONTROLBYTES; MapPtr < Map.Map+Map.Properties.Width*MAP_CONTROLBYTES*Map.Properties.Height; MapPtr += Map.Properties.Width*MAP_CONTROLBYTES)
		{
		for (i = 0; i != Map.Properties.Width; i++)
			{
/* F:C is set */	if (*(MapPtr+i*MAP_CONTROLBYTES) == 0xFF) continue;
/* F:C is not bloc */	if (*(MapPtr+i*MAP_CONTROLBYTES) != 0xFD) continue;
/* T:C+1 is filled */	if ((MapPtr+i*MAP_CONTROLBYTES != MapPtr)&&(*(MapPtr+i*MAP_CONTROLBYTES-MAP_CONTROLBYTES) == 0xFF)) goto Set_Byte;
/* T:C-Lne is filled */	if (*(MapPtr+i*MAP_CONTROLBYTES-Map.Properties.Width*MAP_CONTROLBYTES) == 0xFF) goto Set_Byte;
/* T:C+1 is filled */	if ((MapPtr+i*MAP_CONTROLBYTES+MAP_CONTROLBYTES != MapPtr+Map.Properties.Width*MAP_CONTROLBYTES)&&(*(MapPtr+i*MAP_CONTROLBYTES+MAP_CONTROLBYTES) == 0xFF)) goto Set_Byte;
/* T:C+Lne is filled */	if ((MapPtr+i*MAP_CONTROLBYTES+Map.Properties.Width*MAP_CONTROLBYTES < Map.Map+Map.Properties.Width*MAP_CONTROLBYTES*Map.Properties.Height)&&(*(MapPtr+i*MAP_CONTROLBYTES+Map.Properties.Width*MAP_CONTROLBYTES) == 0xFF)) goto Set_Byte;
			continue;
		Set_Byte:
			*(MapPtr+i*MAP_CONTROLBYTES) = 0xFF;
			Restart = TRUE;
			}
		}
	if (Restart) goto Fill_Loop;

	/* Step 3 : Process the entiere area starting at line-1 */
Fill_Loop2:
	Restart = FALSE;
	for (MapPtr = MapLineBegin-Map.Properties.Width*MAP_CONTROLBYTES; MapPtr >= Map.Map; MapPtr -= Map.Properties.Width*MAP_CONTROLBYTES)
		{
		for (i = 0; i != Map.Properties.Width; i++)
			{
/* F:C is set */	if (*(MapPtr+i*MAP_CONTROLBYTES) == 0xFF) continue;
/* F:C is not bloc */	if (*(MapPtr+i*MAP_CONTROLBYTES) != 0xFD) continue;
/* T:C+1 is filled */	if ((MapPtr+i*MAP_CONTROLBYTES != MapPtr)&&(*(MapPtr+i*MAP_CONTROLBYTES-MAP_CONTROLBYTES) == 0xFF)) goto Set_Byte2;
/* T:C-Lne is filled */	if ((MapPtr+i*MAP_CONTROLBYTES-Map.Properties.Width*MAP_CONTROLBYTES >= Map.Map)&&(*(MapPtr+i*MAP_CONTROLBYTES-Map.Properties.Width*MAP_CONTROLBYTES) == 0xFF)) goto Set_Byte2;
/* T:C+1 is filled */	if ((MapPtr+i*MAP_CONTROLBYTES+MAP_CONTROLBYTES != MapPtr+Map.Properties.Width*MAP_CONTROLBYTES)&&(*(MapPtr+i*MAP_CONTROLBYTES+MAP_CONTROLBYTES) == 0xFF)) goto Set_Byte2;
/* T:C+Lne is filled */	if (*(MapPtr+i*MAP_CONTROLBYTES+Map.Properties.Width*MAP_CONTROLBYTES) == 0xFF) goto Set_Byte2;
			continue;
		Set_Byte2:
			*(MapPtr+i*MAP_CONTROLBYTES) = 0xFF;
			Restart = TRUE;
			}
		}
	if (Restart) goto Fill_Loop2;

	/* Step 4 : Reprocess the current line */
	Restart = FALSE;
	for (MapPtr = MapLineBegin, i = 0; i != Map.Properties.Width; i++)
		{
		if (*(MapPtr+i*MAP_CONTROLBYTES) == 0xFF) continue;
		if (*(MapPtr+i*MAP_CONTROLBYTES) != 0xFD) continue;
		if ((MapPtr+i*MAP_CONTROLBYTES != MapPtr)&&(*(MapPtr+i*MAP_CONTROLBYTES-MAP_CONTROLBYTES) == 0xFF)) goto Set_Byte3;
		if ((MapPtr+i*MAP_CONTROLBYTES-Map.Properties.Width*MAP_CONTROLBYTES >= Map.Map)&&(*(MapPtr+i*MAP_CONTROLBYTES-Map.Properties.Width*MAP_CONTROLBYTES) == 0xFF)) goto Set_Byte3;
		if ((MapPtr+i*MAP_CONTROLBYTES+MAP_CONTROLBYTES != MapPtr+Map.Properties.Width*MAP_CONTROLBYTES)&&(*(MapPtr+i*MAP_CONTROLBYTES+MAP_CONTROLBYTES) == 0xFF)) goto Set_Byte3;
		if ((MapPtr+i*MAP_CONTROLBYTES+Map.Properties.Width*MAP_CONTROLBYTES < Map.Map+Map.Properties.Width*MAP_CONTROLBYTES*Map.Properties.Height)&&(*(MapPtr+i*MAP_CONTROLBYTES+Map.Properties.Width*MAP_CONTROLBYTES) == 0xFF)) goto Set_Byte3;
			continue;
		Set_Byte3:
			*(MapPtr+i*MAP_CONTROLBYTES) = 0xFF;
			Restart = TRUE;
		}
	if (Restart) goto Fill_Loop;

	/* Step 5 : Set special bytes to fill byte */
	for (MapPtr = Map.Map; MapPtr != Map.Map+Map.Properties.Width*MAP_CONTROLBYTES*Map.Properties.Height; MapPtr += MAP_CONTROLBYTES)
		if (*MapPtr == 0xFF)
			{
			*(MapPtr+0) = (BYTE)(MapFullID>>24);
			*(MapPtr+1) = (BYTE)(MapFullID>>16);
			*(MapPtr+2) = (BYTE)(MapFullID>>8);
			*(MapPtr+3) = (BYTE)(MapFullID);
			}

	//--- Annule les conversions ---

	for (MapPtr = Map.Map; MapPtr != Map.Map+Map.Properties.Width*MAP_CONTROLBYTES*Map.Properties.Height; MapPtr += MAP_CONTROLBYTES)
		if (*MapPtr == 0xFD) *(MapPtr+0) = (BYTE)(MapDestID>>24);

	//--- Affichage des changements ---

	MapChanges++;
	MapStats.UsageOfPaintFill++;
	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	return;
}


// �算� Affichage d'un seul �l�ment ��������������������������������������

void Map_PaintElementSimple(HWND hWnd, LONG X, LONG Y)
{
	MAPCLIENT	 MapClient;
	DWORD	 MapFullID;
	BYTE	 MapID;
	BYTE	*MapPtr;

	MapPtr = Map_GetMapPtrFromClientPt(hWnd,X,Y);
	MapFullID = Map_TranslateWorldItem();
	MapID = Map_TranslateID(0xFE,0x01,World);

	if (!MapPtr) return;
	if (MapFullID == 0xFFFFFFFF) return;
	if ((*MapPtr == MapID)&&(!(*(MapPtr+3)))) return;
	if ((*(MapPtr+0) == (BYTE)(MapFullID>>24)) && (*(MapPtr+1) == (BYTE)(MapFullID>>16)) && (*(MapPtr+2) == (BYTE)(MapFullID>>8)) && (*(MapPtr+3) == (BYTE)(MapFullID))) return;

	*(MapPtr+0) = (BYTE)(MapFullID>>24);
	*(MapPtr+1) = (BYTE)(MapFullID>>16);
	*(MapPtr+2) = (BYTE)(MapFullID>>8);
	*(MapPtr+3) = (BYTE)(MapFullID);

	if (Map_RectangleToClient(hWnd,&MapCursorRect,&MapClient))
		{
		MapClient.Width += MapClient.X;
		MapClient.Height += MapClient.Y;
		InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
		UpdateWindow(hWnd);
		}

	MapChanges++;
	MapStats.UsageOfPaint++;
	return;
}


// �算� Remplissage (al�atoire) d'une zone � l'aide d'un �l�ment ���������

void Map_PaintElement(HWND hWnd, DWORD FillMode)
{
	MAPCLIENT	 MapClient;
	DWORD		 MapFullID;
	BYTE		*MapPtr;
	BYTE		 MapID;
	long		 i,j;
	long		 X,Y;
	int			 rnd;

	//--- Initialisations ---

	Map_CorrectRect(&MapCursorRect,&MapClient);
	MapFullID = Map_TranslateWorldItem();
	if (MapFullID == 0xFFFFFFFF) return;

	Map_PaintTemporaryElement(&MapCursorRect,FillMode);
	MapPtr = Map.Map+MapClient.X*MAP_CONTROLBYTES+MapClient.Y*Map.Properties.Width*MAP_CONTROLBYTES;
	MapID = Map_TranslateID(0xFE,0x01,World);

	switch(FillMode)
		{
		case PAINTMODE_RECT:
		case PAINTMODE_RECTFILL:
		case PAINTMODE_CIRCLE:
		case PAINTMODE_CIRCLEFILL:
		case PAINTMODE_LINE:
			for (Y = MapClient.Y, i = 0; i != MapClient.Height; i++, Y++)
				for (X = MapClient.X, j = 0; j != MapClient.Width; j++, X++)
					{
					if (!GetPixel(Map.hDC,X,Y)) continue;
					if ((*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES) == MapID)&&(!(*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+3)))) continue;
					*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+0) = (BYTE)(MapFullID>>24);
					*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+1) = (BYTE)(MapFullID>>16);
					*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+2) = (BYTE)(MapFullID>>8);
					*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+3) = (BYTE)(MapFullID);
					}
			MapChanges++;
			MapStats.UsageOfPaint++;
			break;

		case PAINTMODE_RECTRND:
		case PAINTMODE_RECTFILLRND:
		case PAINTMODE_CIRCLERND:
		case PAINTMODE_CIRCLEFILLRND:
		case PAINTMODE_LINERND:

			randomize();
			for (Y = MapClient.Y, i = 0; i != MapClient.Height; i++, Y++)
				for (X = MapClient.X, j = 0; j != MapClient.Width; j++, X++)
					{
					rnd = random(Config.RandomFreq);
					if (rnd) continue;
					if (!GetPixel(Map.hDC,X,Y)) continue;
					if ((*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES) == MapID)&&(!(*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+3)))) continue;
					*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+0) = (BYTE)(MapFullID>>24);
					*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+1) = (BYTE)(MapFullID>>16);
					*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+2) = (BYTE)(MapFullID>>8);
					*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+3) = (BYTE)(MapFullID);
					}
			MapChanges++;
			MapStats.UsageOfPaintRnd++;
			break;
		}

	//--- Mise � jour de la carte ---

	if (Map_RectangleToClient(hWnd,&MapCursorRect,&MapClient))
		{
		RECT	MapArea;

		MapClient.Width += MapClient.X;
		MapClient.Height += MapClient.Y;
		Map_GetMapArea(hWnd,&MapArea);
		MapClient.X -= MapZoomCX;			// Enlarge the update...
		MapClient.Y -= MapZoomCY;			// rectangle to be sure...
		MapClient.Width += MapZoomCX;			// that doors will be...
		MapClient.Height += MapZoomCY;			// correctly redrawn.
		if (MapClient.X < MapArea.left) MapClient.X = MapArea.left;
		if (MapClient.Y < MapArea.top) MapClient.Y = MapArea.top;
		if (MapClient.Width > MapArea.right) MapClient.Width = MapArea.right;
		if (MapClient.Height > MapArea.bottom) MapClient.Height = MapArea.bottom;
		InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
		UpdateWindow(hWnd);
		}

	return;
}

// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// END
