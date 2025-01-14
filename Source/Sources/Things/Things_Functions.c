
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines diverses relatives aux portes, pi銶es, cr嶧tures, etc.
									      */
//()-------------------------------------------------------------------<<>-<>>//

// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// 中�									  中� //
// 中� Donn嶪s								  中� //
// 中�									  中� //
// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //

#include "..\Includes\Editor.h"
#include "..\Includes\Prototypes.h"
#include "..\Includes\Structures.h"
#include "..\Includes\Texts.h"

extern HWND		hWnd;
extern FONTINFO		Font;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern LONG		MapChanges;
extern MAPTHING		MapImmCreature;
extern MAPTHING		MapImmDoor;
extern MAPTHING		MapImmTrap;
extern MAPTHING		MapImmObject;
extern MAPTHING		MapImmMagicalObject;
extern GLOBALVARS	MapGlobalVariables;
extern DWORD		ToolsItemID;
extern CONFIG		Config;


// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// 中�									  中� //
// 中� Fonctions relatives aux 幨幦ents seuls				  中� //
// 中�									  中� //
// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //

// 姣遙 Obtention des dimensions du nombre de cr嶧tures 垂垂垂垂垂垂垂垂姣

long Things_GetCreaturesCountWidth(LONG MapX, LONG MapY)
{
	BOOL			 Expand;
	MAPTHING	*Creatures;
	long			 HerosCount = 0;
	long			 NeutralCount = 0;
	long			 Keeper1Count = 0;
	long			 Keeper2Count = 0;
	long			 Keeper3Count = 0;
	long			 Keeper4Count = 0;
	long			 Keeper5Count = 0;
	long			 Result;
	char			 Number[32];

	Expand = (Config.MapViewFlags&MVF_DETAILED);
//	if (Font.FontHeight*2 > MapZoomCY) Expand = FALSE;

	for (Creatures = (MAPTHING *)MapCreatures.next; Creatures != 0; Creatures = (MAPTHING *)Creatures->node.next)
		{
		if ((Creatures->x == MapX)&&(Creatures->y == MapY))
			{
			switch(Creatures->owner)
				{
				case 1:	HerosCount++;
					break;
				case 2:	NeutralCount++;
					break;
				case 3:	Keeper1Count++;
					break;
				case 4:	Keeper2Count++;
					break;
				case 5:	Keeper3Count++;
					break;
				case 6:	Keeper4Count++;
					break;
				case 7:	Keeper5Count++;
					break;
				}
			}
		}

	Result = HerosCount+NeutralCount+Keeper1Count+Keeper2Count+Keeper3Count+Keeper4Count+Keeper5Count;
	if (!Result) return(0);

	if (!Expand)
		{
		wsprintf(Number,szNumber,Result);
		Result = strlen(Number)*Font.FontAveWidth;
		}
	else
		{
		Result = 0;
		if (HerosCount)
			{
			wsprintf(Number,szNumber,HerosCount);
			Result += strlen(Number)*Font.FontAveWidth;
			}
		if (NeutralCount)
			{
			wsprintf(Number,szNumber,NeutralCount);
			Result += strlen(Number)*Font.FontAveWidth;
			}
		if (Keeper1Count)
			{
			wsprintf(Number,szNumber,Keeper1Count);
			Result += strlen(Number)*Font.FontAveWidth;
			}
		if (Keeper2Count)
			{
			wsprintf(Number,szNumber,Keeper2Count);
			Result += strlen(Number)*Font.FontAveWidth;
			}
		if (Keeper3Count)
			{
			wsprintf(Number,szNumber,Keeper3Count);
			Result += strlen(Number)*Font.FontAveWidth;
			}
		if (Keeper4Count)
			{
			wsprintf(Number,szNumber,Keeper4Count);
			Result += strlen(Number)*Font.FontAveWidth;
			}
		if (Keeper5Count)
			{
			wsprintf(Number,szNumber,Keeper5Count);
			Result += strlen(Number)*Font.FontAveWidth;
			}
		}

	return(Result);
}


// 姣遙 Obtention de l'objet se trouvant � l'emplacement sp嶰ifi� 垂垂垂姣

MAPTHING* Things_GetAtMapPos(NODE *ListPtr, LONG MapX, LONG MapY)
{
	MAPTHING	*Thing;

	for (Thing = (MAPTHING *)ListPtr->next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		if ((Thing->x == MapX)&&(Thing->y == MapY))
			return(Thing);

	return(NULL);
}


// 姣遙 Compte le nombre d'幨幦ent � un emplacement donn� 垂垂垂垂垂垂垂姣

long Things_GetCountAtMapPos(NODE *ListPtr, LONG MapX, LONG MapY)
{
	MAPTHING	*Thing;
	long		 Result;

	for (Result = 0, Thing = (MAPTHING *)ListPtr->next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		if ((Thing->x == MapX)&&(Thing->y == MapY))
			Result++;

	return(Result);
}


// 姣遙 Ajoute le nom d'un 幨幦ent � un texte 垂垂垂垂垂垂垂垂垂垂垂垂垂姣

void Things_StrCat(char *Header, char *Text, MAPTHING *Thing, ELEMENT *ThingTable)
{
	char	TempText[32];
	int	i;

	if (Thing)
		{
		for (i = 0; ThingTable[i].Id != 0; i++)
			if (ThingTable[i].Id == Thing->id)
				{
				if (Header) strcat(Text,Header);
				strcat(Text,ThingTable[i].Name);
				switch(Thing->node.type)
					{
					case THING_OBJECT:
						switch(Thing->id)
							{
							case 0x00000001:
							case 0x00000002:
							case 0x00000003:
								wsprintf(TempText,szThingGold,Thing->data.gold);
								strcat(Text,TempText);
								break;
							case 0x00000004:
								for (i = 0; Spells[i].Id != 0; i++) if (Spells[i].Id == Thing->data.spell) break;
								if (Spells[i].Id != 0)
									{
									wsprintf(TempText,szThingSpell,Spells[i].Name);
									strcat(Text,TempText);
									}
								break;
							}
					}
				break;
				}
		}

	return;
}


// 姣遙 Supprime l'objet se trouvant � l'emplacement sp嶰ifi� 垂垂垂垂垂姣

void Things_RemoveAtMapPos(NODE *ListPtr, LONG MapX, LONG MapY)
{
	MAPTHING	*Thing;
	UINT		 Type;

	if (ListPtr == &MapCreatures) Type = UNDO_SUBCREATURE;
	else if (ListPtr == &MapDoors) Type = UNDO_SUBDOOR;
	else if (ListPtr == &MapTraps) Type = UNDO_SUBTRAP;
	else if (ListPtr == &MapObjects) Type = UNDO_SUBOBJECT;
	else if (ListPtr == &MapMagicalObjects) Type = UNDO_SUBMAGIC;

	for (Thing = (MAPTHING *)ListPtr->next; Thing != 0;)
		{
		if ((Thing->x == MapX)&&(Thing->y == MapY))
			{
			Undo_CreateEntry((NODE *)Thing,Type);
			MapChanges++;
			Thing = (MAPTHING *)ListPtr->next;
			continue;
			}
		Thing = (MAPTHING *)Thing->node.next;
		}

	return;
}

//--- Suppression avec test dans les 憝鋝ements ---

void Things_RemoveAtMapPosSafe(NODE *ListPtr, LONG MapX, LONG MapY)
{
	MAPTHING	*Thing;
	UINT		 Type;
	BOOL		 KillAll;
	int		 Used;

	if (ListPtr == &MapCreatures) Type = UNDO_SUBCREATURE;
	else if (ListPtr == &MapDoors) Type = UNDO_SUBDOOR;
	else if (ListPtr == &MapTraps) Type = UNDO_SUBTRAP;
	else if (ListPtr == &MapObjects) Type = UNDO_SUBOBJECT;
	else if (ListPtr == &MapMagicalObjects) Type = UNDO_SUBMAGIC;

	for (Thing = (MAPTHING *)ListPtr->next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		if (!Thing->uniqueid) continue;
		if ((Thing->x == MapX)&&(Thing->y == MapY))
			{
			Used = Triggers_IsUsed(hWnd,Thing->uniqueid,&KillAll,SCOPE_CREATURE|SCOPE_KILL);
			if (!Used) return;
			if (Used == -1) continue;
			break;
			}
		}

	for (Thing = (MAPTHING *)ListPtr->next; Thing != 0;)
		{
		if ((Thing->x == MapX)&&(Thing->y == MapY))
			{
			if ((Thing->uniqueid)&&(!KillAll))
				{
				Thing = (MAPTHING *)Thing->node.next;
				continue;
				}
			Undo_CreateEntry((NODE *)Thing,Type);
			MapChanges++;
			Thing = (MAPTHING *)ListPtr->next;
			continue;
			}
		Thing = (MAPTHING *)Thing->node.next;
		}

	return;
}

//--- Suppression dans un contexte particulier ---

void Things_RemoveAtMapPosEx(NODE *ListPtr, LONG MapX, LONG MapY, UNDO **Parent)
{
	MAPTHING	*Thing;
	UNDO		*Child;
	UINT		 Type;

	if (ListPtr == &MapCreatures) Type = UNDO_SUBCREATURE;
	else if (ListPtr == &MapDoors) Type = UNDO_SUBDOOR;
	else if (ListPtr == &MapTraps) Type = UNDO_SUBTRAP;
	else if (ListPtr == &MapObjects) Type = UNDO_SUBOBJECT;
	else if (ListPtr == &MapMagicalObjects) Type = UNDO_SUBMAGIC;

	for (Thing = (MAPTHING *)ListPtr->next; Thing != 0;)
		{
		if ((Thing->x == MapX)&&(Thing->y == MapY))
			{
			Child = Undo_CreateNewChild(Parent,TRUE,0,NULL);
			Undo_StoreEntry(Child,(NODE *)Thing,Type);
			MapChanges++;
			Thing = (MAPTHING *)ListPtr->next;
			continue;
			}
		Thing = (MAPTHING *)Thing->node.next;
		}

	return;
}

// 姣遙 Duplique l'objet se trouvant � l'emplacement sp嶰ifi� 垂垂垂垂垂姣

void Things_CloneAtMapPos(NODE *ListPtr, LONG MapX, LONG MapY, NODE *DestListPtr)
{
	MAPTHING	*Thing;
	MAPTHING	*DestThing;

	for (Thing = (MAPTHING *)ListPtr->next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		if ((Thing->x == MapX)&&(Thing->y == MapY))
			{
			DestThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
			if (!DestThing) continue;
			CopyMemory(DestThing,Thing,sizeof(MAPTHING));
			List_AddEntry((NODE *)DestThing,DestListPtr);
			}
		}

	return;
}


// 姣遙 Test quel genre de cr嶧ture il s'agit 垂垂垂垂垂垂垂垂垂垂垂垂垂姣

int Things_IsHero(DWORD CreatureID)
{
	int	i;

	for (i = 0; Heros[i].Id != 0; i++)
		if (Heros[i].Id == CreatureID)
			return(1);

	return(0);
}

// 姣遙 Obtention de l'offset d'un objet en fonction de son ID 垂垂垂垂垂�

int Things_GetOffsetFromID(DWORD ID, UINT TypeOfThing)
{
	int	Result;
	int	i;

	switch(TypeOfThing)
		{
		case THING_CREATURE:
			for (Result = 0, i = 0; BadCreatures[i].Id != 0; i++, Result++)
				if (BadCreatures[i].Id == ID) return(Result);
			for (i = 0; BadElites[i].Id != 0; i++, Result++)
				if (BadElites[i].Id == ID) return(Result);
			for (i = 0; Heros[i].Id != 0; i++, Result++)
				if (Heros[i].Id == ID) return(Result);
			break;
		case THING_DOOR:
			for (Result = 0; Doors[Result].Id != 0; Result++)
				if (Doors[Result].Id == ID) return(Result);
			break;
		case THING_TRAP:
			for (Result = 0; Traps[Result].Id != 0; Result++)
				if (Traps[Result].Id == ID) return(Result);
			break;
		case THING_OBJECT:
			for (Result = 0; Objects[Result].Id != 0; Result++)
				if (Objects[Result].Id == ID) return(Result);
			break;
		case THING_MAGICALOBJECT:
			for (Result = 0; Magics[Result].Id != 0; Result++)
				if (Magics[Result].Id == ID) return(Result);
			break;
		}

	return(0);
}


// 姣遙 Obtention du nom d'un objet en fonction de son ID 垂垂垂垂垂垂垂姣

//--- Nom d'une cr嶧ture ---

char* Things_GetCreatureNameFromID(DWORD CreatureID)
{
	int	i;

	for (i = 0; BadCreatures[i].Id != 0; i++)
		if (BadCreatures[i].Id == CreatureID)
			return(BadCreatures[i].Name);

	for (i = 0; BadElites[i].Id != 0; i++)
		if (BadElites[i].Id == CreatureID)
			return(BadElites[i].Name);

	for (i = 0; Heros[i].Id != 0; i++)
		if (Heros[i].Id == CreatureID)
			return(Heros[i].Name);

	return(NULL);
}

//--- Nom d'un objet quelconque ---

char* Things_GetThingNameFromID(DWORD ThingID, ELEMENT *Table)
{
	int	i;

	for (i = 0; Table[i].Id != 0; i++)
		if (Table[i].Id == ThingID)
			return(Table[i].Name);

	return(NULL);
}

//--- Nom ASCII d'un objet quelconque ---

char* Things_GetThingASCIIFromID(DWORD ThingID, ELEMENT *Table)
{
	int	i;

	for (i = 0; Table[i].Id != 0; i++)
		if (Table[i].Id == ThingID)
			return(Table[i].ASCIIName);

	return(NULL);
}


// 姣遙 Retrouve la position d'un objet 垂垂垂垂垂垂垂垂垂垂垂垂垂垂垂垂姣

long Things_GetThingPos(NODE *List, DWORD ID, LONG Type)
{
	MAPTHING	*Thing;

	for (Thing = (MAPTHING *)List->next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		if (Thing->id == ID)
			{
			switch(Type)
				{
				case 0:	return(Thing->x);
				case 1:	return(Thing->y);
				}
			}
		}

	return(-1);
}


// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// 中�									  中� //
// 中� Fonctions relatives au terrain					  中� //
// 中�									  中� //
// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //

// 姣遙 Test si le terrain en question est solide 垂垂垂垂垂垂垂垂垂垂垂姣

int Things_IsSolid(BYTE *MapPtr)
{
	DWORD	MapID;

	MapID = (Map_GetRealIDFromMap(MapPtr))>>24;
	switch(MapID)
		{
		case 0x1E:			// Limite
		case 0x06:			// Or
		case 0x07:			// Gems
		case 0x01:			// Roche imp幯彋rable
		case 0x02:			// Roche
		case 0x09:			// Mur
		case 0xFE:			// Limite indestructible
			return(1);
		}

	return(0);
}


// 姣遙 Obtention du type d'utilisateur en fonction du terrain 垂垂垂垂垂�

BYTE Things_GetOwner(BYTE *MapPtr, UINT Owner)
{
	if (!Owner)
		return(*(MapPtr+1));
	else
		return((BYTE)Owner);
}


// 姣遙 Retourne le type de curseur en fonction du terrain cible 垂垂垂垂�

// 0 - La zone s幨ectionn嶪 est incompatible avec l'幨幦ent (Rouge)
// 1 - Pas de probl鋗e (Bleu)
// 2 - L'幨幦ent n'est pas sur son territoire (Vert)
// 3 - La cr嶧ture craint l'幨幦ent sur lequel elle se trouve (Bleu clair)

int Things_CanPaint(MAP *Map, LONG MapX, LONG MapY)
{
	DWORD	ItemID;
	UINT	Id;
	UINT	Owner;

	switch(ToolsItemID)
		{
		case PAINTMODE_CREATURE:
			ItemID = THING_CREATURE;
			Id = MapImmCreature.id;
			Owner = MapImmCreature.owner;
			break;
		case PAINTMODE_DOOR:
			ItemID = THING_DOOR;
			Id = MapImmDoor.id;
			Owner = MapImmDoor.owner;
			break;
		case PAINTMODE_TRAP:
			ItemID = THING_TRAP;
			Id = MapImmTrap.id;
			Owner = MapImmTrap.owner;
			break;
		case PAINTMODE_OBJECT:
			ItemID = THING_OBJECT;
			Id = MapImmObject.id;
			Owner = MapImmObject.owner;
			break;
		case PAINTMODE_MAGICALOBJECT:
			ItemID = THING_MAGICALOBJECT;
			Id = MapImmMagicalObject.id;
			Owner = MapImmMagicalObject.owner;
			break;
		}

	return(Things_CanPaintEx(Map,MapX,MapY,ItemID,Id,Owner,NULL));
}

int Things_CanPaintEx(MAP *Map, LONG MapX, LONG MapY, DWORD ItemID, UINT Id, UINT Owner, char *ErrorBuffer)
{
	BYTE		*MapPtr;
	BYTE	 MapID;
	int		 i;

	if ((MapX >= Map->Properties.Width)||(MapY >= Map->Properties.Height))
		{
		if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingOutErr);
		return(0);
		}

	MapPtr = Map->Map+MapX*MAP_CONTROLBYTES+MapY*Map->Properties.Width*MAP_CONTROLBYTES;
	MapID = (BYTE)((Map_GetRealIDFromMap(MapPtr))>>24);

	/* Quelque soit l'幨幦ent, il ne peut 皻re pos嶪 sur ce type de terrain */
	switch(MapID)
		{
		case 0xFE:					// Limite indestructible
			if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingInlErr);
			return(0);
		case 0x06:					// Or
		case 0x07:					// Gems
		case 0x01:					// Roche dure
		case 0x02:					// Roche
		case 0x1E:					// Limite
		case 0x09:					// Mur
			if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingIncErr);
			return(0);
		}

	switch(ItemID)
		{
		case THING_CREATURE:
			/* Les cr嶧tures suivantes SUPPORTENT la lave */
			if (MapID == 0x05)
				{
				for (i = 0; GCreatures[i].Id != 0; i++)
					if ((GCreatures[i].Id == Id)&&(MapGlobalVariables.Creatures[i].CanWalkLava))
						{
						if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingNpbErr);
						return(1);
						}
				if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingCrlErr);
				return(3);
				}

			/* Les cr嶧tures suivantes ne supportent PAS l'eau */
			if (MapID == 0x04)
				{
				for (i = 0; GCreatures[i].Id != 0; i++)
					if ((GCreatures[i].Id == Id)&&(!MapGlobalVariables.Creatures[i].CanWalkWater))
						{
						if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingCrwErr);
						return(3);
						}
				if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingNpbErr);
				return(1);
				}

			if (!Owner) break;

			/* Les cr嶧tures peuvent aller sur certains terrains */
			switch(MapID)
				{
				case 0x04:			// Eau
				case 0x05:			// Lave
				case 0x03:			// Terrain non r嶰lam�
				case 0x1F:			// Source de mana ferm嶪
					if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingNpbErr);
					return(1);
				}

			/* Les cr嶧tures peuvent aller dans une prison ennemie */
			if ((MapID == 0x14)&&(*(MapPtr+1) != Owner))
				{
				if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingNpbErr);
				return(1);
				}

			/* Attention! La cr嶧ture est sur le terrain de l'ennemi */
			if (*(MapPtr+1) != Owner)
				{
				if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingEnmErr);
				return(2);
				}

			break;

		case THING_DOOR:
		case THING_TRAP:
			/* Les portes et les pi銶es ne peuvent pas 皻re pos廥 ici */
			switch(MapID)
				{
				case 0x0B:			// Hantre
				case 0x0F:			// Biblioth鋂ue
				case 0x0A:			// Salle du tr廥or
				case 0x18:			// Casino
				case 0x15:			// Salle de torture
				case 0x17:			// Cimeti鋨e
				case 0x16:			// Temple
				case 0x0D:			// Couvoir
				case 0x10:			// Salle d'entra螽ement
				case 0x13:			// Atelier
				case 0x12:			// Salle de garde
				case 0x14:			// Prison
				case 0x19:			// Ar瘽e
				case 0x11:			// Pont de bois
				case 0x1A:			// Pont de pierre
				case 0x24:			// Pont de pierre des h廨os
					if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingInrErr);
					return(0);
				case 0x1F:			// Source de mana ferm嶪
				case 0x20:			// Source de mana ouverte
				case 0x04:			// Eau
				case 0x05:			// Lave
					if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingIncErr);
					return(0);
				}

			/* Les portes et les pi銶es ne peuvent 皻re pos廥 que sur leurs terrains */
			if ((Owner != 0)&&(*(MapPtr+1) != Owner))
				{
				if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingIneErr);
				return(0);
				}

		case THING_OBJECT:
		case THING_MAGICALOBJECT:
			/* Les objets, les portes et les pi銶es ne peuvent pas 皻re pos廥 sur ce type de terrain */
			switch(MapID)
				{
				case 0x0E:			// Coeur de donjon
				case 0x1C:			// Portail de h廨o 1
				case 0x22:			// Portail de h廨o 4
					/* Le cristal peut 皻re pos� sur ce type de terrain */
					if ((ItemID == THING_OBJECT)&&(Id == 0x1C)) break;
				case 0x28:			// Portail des mercenaires
				case 0x0C:			// Portail
				case 0x1D:			// Portail de h廨o 2
				case 0x21:			// Portail de h廨o 3
				case 0x25:			// Portail de h廨o 6
					if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingIn2Err);
					return(0);
				}

			if (!Owner) break;

			/* Les objets (magiques) neutres peuvent 皻re pos廥 sur tous les terrains */
			if ((ItemID == THING_OBJECT)&&(Owner == 2)) break;
			if ((ItemID == THING_MAGICALOBJECT)&&(Owner == 2)) break;

			/* Attention! Un objet n'a pas 彋� pos� sur son terrain */
			if (*(MapPtr+1) != Owner)
				{
				/* La bo褾e a h廨os peut 皻re pos嶪s sur un terrain ennemi */
				if ((ItemID != THING_OBJECT)||(Id != 0x82))
					{
					if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingEnmErr);
					return(2);
					}
				}

			break;
		}

	if (ErrorBuffer) strcpy(ErrorBuffer,szCompilerThingNpbErr);
	return(1);
}

// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// END
