
//<<>-<>>---------------------------------------------------------------------()
/*
	Routine de recherche
									      */
//()-------------------------------------------------------------------<<>-<>>//

// �������������������������������������������������������������������������� //
// ���									  ��� //
// ��� Donn�es								  ��� //
// ���									  ��� //
// �������������������������������������������������������������������������� //

#include "..\Includes\Editor.h"
#include "..\Includes\Prototypes.h"
#include "..\Includes\Structures.h"
#include "..\Includes\Texts.h"

extern MAPFIND		MapFind;


// �������������������������������������������������������������������������� //
// ���									  ��� //
// ��� Bo�te de dialogue						  ��� //
// ���									  ��� //
// �������������������������������������������������������������������������� //

// ���� S�lection des options de recherche �������������������������������

#pragma argsused
void Find_Select(HWND hWnd)
{
	MessageBox(hWnd,"Ok",NULL,MB_OK);
	return;
}


// �������������������������������������������������������������������������� //
// ���									  ��� //
// ��� Fonctions							  ��� //
// ���									  ��� //
// �������������������������������������������������������������������������� //

// ���� Supprime certaines entr�es dans la liste �������������������������

void Find_UpdateList(void *Address)
{
	MAPFINDENTRY	*Entry;

	for (Entry = (MAPFINDENTRY *)MapFind.Results.next; Entry != NULL;)
		{
		if (Address == Entry->address)
			{
			List_RemEntry((NODE *)Entry);
			GlobalFree(Entry);
			Entry = (MAPFINDENTRY *)MapFind.Results.next;
			continue;
			}
		Entry = (MAPFINDENTRY *)Entry->node.next;
		}

	return;
}

// �������������������������������������������������������������������������� //
// �������������������������������������������������������������������������� //
// END
