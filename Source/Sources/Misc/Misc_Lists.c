
//<<>-<>>---------------------------------------------------------------------()
/*
	Routine des gestions des listes en tout genre.
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


// �������������������������������������������������������������������������� //
// ���									  ��� //
// ��� Fonctions							  ��� //
// ���									  ��� //
// �������������������������������������������������������������������������� //

// ���� Calcul le nombre d'entr�e dans une liste �������������������������

long List_EntryCount(NODE *EntryList)
{
	NODE   *List;
	long	Result = 0;

	for (List = EntryList->next; List != 0; List = List->next, Result++);
	return(Result);
}


// ���� D�place toutes les entr�es d'une liste vers une autre liste ������

void List_Move(NODE *SrcList, NODE *DestList)
{
	if (!SrcList->next)
		{
		DestList->next = NULL;
		return;
		}

	DestList->next = SrcList->next;
	DestList->next->prev = DestList;
	SrcList->next = NULL;
	return;
}


// ���� Ajoute une entr�e ������������������������������������������������

void List_AddEntry(NODE *NewEntry, NODE *EntryList)
{
	NODE *List;

	for (List = EntryList; List->next != 0; List = List->next);

	List->next = NewEntry;
	NewEntry->prev = List;
	NewEntry->next = NULL;
	return;
}


// ���� Ins�re une entr�e ������������������������������������������������

void List_InsertEntry(NODE *NewEntry, NODE *InsEntry, BOOL InsertAfter)
{
	NODE	*Next;
	NODE	*Prev;

	Next = InsEntry->next;
	Prev = InsEntry->prev;

	if (InsertAfter)
		{
		NewEntry->next = Next;
		NewEntry->prev = InsEntry;
		InsEntry->next = NewEntry;
		if (Next) Next->prev = NewEntry;
		}
	else
		{
		NewEntry->next = InsEntry;
		NewEntry->prev = Prev;
		InsEntry->prev = NewEntry;
		if (Prev) Prev->next = NewEntry;
		}

	return;
}


// ���� Supprime une entr�e ����������������������������������������������

void List_RemEntry(NODE *Entry)
{
	NODE *Next;
	NODE *Prev;

	Next = Entry->next;
	Prev = Entry->prev;

	if (Next) Next->prev = Prev;
	if (Prev) Prev->next = Next;

	Entry->next = NULL;
	Entry->prev = NULL;
	return;
}


// ���� Supprime la m�moire utilis�e par une liste �����������������������

void List_ReleaseMemory(NODE *FreeList)
{
	NODE *List;
	NODE *Next;

	for (List = FreeList->next; List != 0;)
		{
		Next = List->next;
		GlobalFree(List);
		List = Next;
		}

	FreeList->next = NULL;
	FreeList->prev = NULL;
	return;
}


// ���� Compare deux entr�es ���������������������������������������������

long List_CompareEntries(NODE *FirstEntry, NODE *SecondEntry)
{
	NODE	*Entry;

	Entry = FirstEntry->next;
	while(Entry)
		{
		// First entry precedes the second entry ?
		if (Entry == SecondEntry) return(-1);
		Entry = Entry->next;
		}

	// First entry follows the second entry
	return(1);
}


// ���� Recherche l'entr�e s�lectionn�e dans une liste �������������������

int List_FindSelectedItem(HWND hList)
{
	LV_ITEM Item;

	Item.iItem = -1;
	Item.iItem = SendMessage(hList,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0));
	return(Item.iItem);
}


// ���� Range les entr�es dans l'ordre o� elles apparaissent �������������

#pragma argsused
int CALLBACK List_SortItems(LPARAM Item1, LPARAM Item2, LPARAM Unused)
{
	return(List_CompareEntries((NODE *)Item1,(NODE *)Item2));
}

// �������������������������������������������������������������������������� //
// �������������������������������������������������������������������������� //
// END
