

/*----------------------------------------------------------*/
/*			    メニューファイルの読み込み			
/*----------------------------------------------------------*/

#include <windows.h>
#include <commctrl.h>
#include "menuDrop.h"


void readyMenuFile(dMenu *dMenuData,TCHAR *section,TCHAR *key1,TCHAR *key2,TCHAR *path,int dropsize){

	int n=0,size;
	TCHAR *buff;
	TCHAR strBuff[MAX_PATH];
	TCHAR nameBuff[MAX_PATH];
	HICON icon;
	SHFILEINFO shfi;
	HIMAGELIST list;

	size = lstrlen(section)*sizeof(TCHAR);
	buff = (TCHAR *)malloc(size+6);

	while(n<dropsize){

		wsprintf(buff,TEXT("%s%d"),section,n);
		GetPrivateProfileString(buff,key1,TEXT("Nothing"),strBuff,(DWORD)MAX_PATH,path);
		GetPrivateProfileString(buff,key2,TEXT("Nothing"),nameBuff,(DWORD)MAX_PATH,path);

		if(lstrcmp(strBuff,TEXT("Nothing"))!=0){

			//構造体にセット
			// アイコンの取得
			list = (HIMAGELIST)SHGetFileInfo(strBuff, FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof(SHFILEINFO),SHGFI_SYSICONINDEX  | SHGFI_LARGEICON);
			icon = ImageList_GetIcon(list,shfi.iIcon, ILD_NORMAL);

			dMenuData->dIcon = icon;
			//パスのコピー
			lstrcpy(dMenuData->dPath,strBuff);
			lstrcpy(dMenuData->dname,nameBuff);

		}
		dMenuData++;
		n++;

	}

	free(buff);

}
