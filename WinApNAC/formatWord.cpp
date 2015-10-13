/*----------------------------------------------------------*/
/*			            フォーマット群
/*
/*
/* ---- フォーマットの拡張はここで行う
/*----------------------------------------------------------*/

//インクルード
#include <windows.h>

/*---------------------------------------------------------*/
/*		           プロトタイプ宣言			
/*----------------------------------------------------------*/

int readWord(TCHAR *,TCHAR *);
int randInt(int);


/*** 食べ物フォーマット ***/
void GetfoodFormat(TCHAR *tag,TCHAR *path){
	readWord(tag,path);
}

void SetfoodFormat(TCHAR *Word){
	int no;

	no = randInt(5);

	switch(no){
	
		case 0:
			lstrcat(Word,TEXT("おいしい～♪"));
		break;

		case 1:
			lstrcat(Word,TEXT("たべた～い♪"));
		break;

		case 2:
			lstrcat(Word,TEXT("たべたよ～♪"));
		break;

		case 3:
			lstrcat(Word,TEXT("をいっしょにたべたいね♪"));
		break;

		case 4:
			lstrcat(Word,TEXT("・・・まずいよ・・・・"));
		break;
	}
}

/*** 服フォーマット ***/
void GetClothesFormat(TCHAR *tag,TCHAR *path){
	readWord(tag,path);
}

void SetClothesFormat(TCHAR *Word){
	int no;
	TCHAR tmp[70];
	ZeroMemory(tmp,sizeof(TCHAR));

	no = randInt(5);

	switch(no){
	
		case 0:
			lstrcat(Word,TEXT("かわいい～♪"));
		break;

		case 1:
			lstrcat(Word,TEXT("ほしい～♪"));
		break;

		case 2:
			lstrcat(Word,TEXT("にあうかな～？"));
		break;

		case 3:
			lstrcat(Word,TEXT("・・・？"));
		break;

		case 4:
			lstrcat(tmp,TEXT("この"));
			lstrcat(tmp,Word);
			lstrcat(tmp,TEXT("・・・どうかな？"));
			lstrcpy(Word,tmp);
		break;
	}
}

/*** 機械フォーマット ***/
void GetMachineFormat(TCHAR *tag,TCHAR *path){
	readWord(tag,path);
}

void SetMachineFormat(TCHAR *Word){
	int no;
	TCHAR tmp[70];
	ZeroMemory(tmp,sizeof(TCHAR));

	no = randInt(5);

	//特定文字列時の仕様サンプル
	if(lstrcmp(Word,TEXT("パソコン"))==0){
		lstrcat(Word,TEXT("こわれちゃえー！"));
		no=-1;
	}

	switch(no){
	
		case 0:
			lstrcat(tmp,TEXT("この"));
			lstrcat(tmp,Word);
			lstrcat(tmp,TEXT("・・・買って？"));
			lstrcpy(Word,tmp);
		break;

		case 1:
			lstrcat(Word,TEXT("ほしい～♪"));
		break;

		case 2:
			lstrcat(Word,TEXT("こわしちゃうぞ～？"));
		break;

		case 3:
			lstrcat(Word,TEXT("・・・？"));
		break;

		case 4:
			lstrcat(Word,TEXT("ハイテクです！"));
		break;
	}
}

/*** 本フォーマット ***/
void GetBookFormat(TCHAR *tag,TCHAR *path){
	readWord(tag,path);
}

void SetBookFormat(TCHAR *Word){
	int no;
	TCHAR tmp[70];
	ZeroMemory(tmp,sizeof(TCHAR));

	no = randInt(5);

	switch(no){
	
		case 0:
			lstrcat(tmp,TEXT("この"));
			lstrcat(tmp,Word);
			lstrcat(tmp,TEXT("って、おもしろいね～♪"));
			lstrcpy(Word,tmp);
		break;

		case 1:
			lstrcat(Word,TEXT("つづきは～？"));
		break;

		case 2:
			lstrcat(Word,TEXT("もやしちゃうぞ～？"));
		break;

		case 3:
			lstrcat(Word,TEXT("・・・？"));
		break;

		case 4:
			lstrcat(Word,TEXT("・・・かんじおおい・・・"));
		break;
	}
}

/*** アクセサリーフォーマット ***/
void GetAccessoryFormat(TCHAR *tag,TCHAR *path){
	readWord(tag,path);
}

void SetAccessoryFormat(TCHAR *Word){
	int no;
	TCHAR tmp[70];
	ZeroMemory(tmp,sizeof(TCHAR));

	no = randInt(5);

	switch(no){
	
		case 0:
			lstrcat(Word,TEXT("かわいい～♪"));
		break;

		case 1:
			lstrcat(Word,TEXT("ほしい～♪"));
		break;

		case 2:
			lstrcat(Word,TEXT("にあうかな～？"));
		break;

		case 3:
			lstrcat(Word,TEXT("・・・？"));
		break;
			
		case 4:
			lstrcat(Word,TEXT("きらきら～"));
		break;
	}
}

/*** 飲み物フォーマット ***/
void GetDrinkFormat(TCHAR *tag,TCHAR *path){
	readWord(tag,path);
}

void SetDrinkFormat(TCHAR *Word){
	int no;

	no = randInt(5);

	switch(no){
	
		case 0:
			lstrcat(Word,TEXT("おいしい～♪"));
		break;

		case 1:
			lstrcat(Word,TEXT("のみた～い♪"));
		break;

		case 2:
			lstrcat(Word,TEXT("のんだよ～♪"));
		break;

		case 3:
			lstrcat(Word,TEXT("をいっしょにのみたいね♪"));
		break;

		case 4:
			lstrcat(Word,TEXT("・・・まずいよ・・・・"));
		break;
	}
}