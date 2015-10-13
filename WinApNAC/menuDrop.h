/************************************************************/
/*          ドロップメニューにおける構造体&定数             */
/*															*/
/************************************************************/

typedef struct{

	HICON dIcon;
	TCHAR dPath[MAX_PATH];
	TCHAR dname[MAX_PATH];

}dMenu;


#define DROPSIZEX 5				//ドロップできる横サイズ
#define DROPSIZEY 2				//ドロップできる縦サイズ