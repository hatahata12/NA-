/*----------------------------------------------------------*/
/*			            êÅÇ´èoÇµèàóù		
/*----------------------------------------------------------*/


#include <windows.h>
#include "extern.h"

#define M1 TEXT(" ÇæÇÊ")
#define M2 TEXT(" Ç≈Ç∑Å`")

void Message(HWND MhWnd,HWND MLhWnd,TCHAR *str,int w,int h,int mode){

	TCHAR tmpBuff[100]={TEXT(" ")};
	RECT rct;

	GetClientRect(MhWnd,&rct);

	//îwåiÇîíÇ…
	SelectObject(MesDC,GetStockObject(NULL_PEN));
	Rectangle(MesDC,0,0,w+1,h+1);

	lstrcpy(tmpBuff,str);

	if(lstrlen(tmpBuff)){
		
		switch(mode){

			case 1:
				lstrcat(tmpBuff,M1);
			break;

			case 2:
				lstrcat(tmpBuff,M2);
			break;

		}

		DrawText(MesDC,tmpBuff,-1,&rct,DT_WORDBREAK);
		InvalidateRect(MhWnd,NULL,FALSE);
	}


	ShowWindow(MLhWnd,SW_SHOWNOACTIVATE);
	ShowWindow(MhWnd,SW_SHOWNOACTIVATE);


}


void CloseMessage(HWND MhWnd,HWND MLhWnd){

	ShowWindow(MhWnd,SW_HIDE);
	ShowWindow(MLhWnd,SW_HIDE);

}