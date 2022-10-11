
#ifndef EDITOR_LASTERROR_H
#define EDITOR_LASTERROR_H

/*** Valeurs de pr�compilation ******************************************/

#define COMPILERR_MAPALLOC	103
#define COMPILERR_BADHEADER	104
#define COMPILERR_BADSIZE	105
#define COMPILERR_EOF		106
#define COMPILERR_BADVERSION	107
#define COMPILERR_UNKHEADER	108
#define COMPILERR_WINDOWS	200

/*** Structures *********************************************************/

typedef struct ERRORTABLE {
	DWORD		 ErrorCode;
	char		*ErrorText;
} ERRORTABLE;

#endif //EDITOR_LASTERROR_H
