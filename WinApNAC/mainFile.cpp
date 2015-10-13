/*********************************************************************************/
/*                    [  ですくとっぷのえぬえーちゃん 	]	　　　		       	 */
/*															   NAちゃん ver 0.85 */
/*																				 */
/*																				 */
/* 更新日：			    								                         */
/*********************************************************************************/

//警告の抑制
#define _CRT_SECURE_NO_DEPRECATE

/*----------------------------------------------------------*/
/*		      インクルードファイル＆ライブラリ		
/*----------------------------------------------------------*/

#include <windows.h>
//標準入出力
#include <stdio.h>
#include <time.h>
//プロセス系
#include <pdh.h>
//ドロップメニュー
#include "menuDrop.h"
//グローバル
#include "Global.h"
//リソース
#include "resource.h"
//RSS
#include "rss.h"
//pngファイル読み込み用
#include "png.h"
#include "zlib.h"
#pragma comment(lib, "libpng.lib")
#pragma comment(lib, "zlib.lib")
//シェル系
#include <Shlobj.h>
//MSIショートカット
#include <Msi.h>

/*----------------------------------------------------------*/
/*		                    定義			
/*----------------------------------------------------------*/

#define ID_TASK 200                                                   //タスク
#define MYWM_NOTIFYICON 100                                           //タスク
#define VER TEXT("Ver 1.00")                                       //バージョン
#define APP_NAME TEXT("ディスクトップアプリ")                         //ウィンドウ名                    
#define APP_NAME2 TEXT("ドロップアイコン用ウィンドウ")                //ウィンドウ名
#define APP_NAME3 TEXT("ドロップアイコンレイアウト用ウィンドウ")      //ドウィンドウ名
#define APP_NAME4 TEXT("メッセージレイアウト用ウィンドウ")            //ウィンドウ名
#define APP_NAME5 TEXT("メッセージ用ウィンドウ")                      //ウィンドウ名
#define IMG_NA1 TEXT("img/na-1.png")								  //表情「通常」
#define IMG_NA2 TEXT("img/na-2.png")								  //表情「むす」
#define IMG_NA3 TEXT("img/na-3.png")								  //表情「怒り」
#define IMG_NA4 TEXT("img/na-4.png")								  //表情「泣き」
#define IMG_NA5 TEXT("img/na-5.png")								  //表情「ぽかーん」
#define IMG_NA6 TEXT("img/na-6.png")								  //表情「うれしい」
#define ITEM1 TEXT("img/backDropImg.png")							  //ドロップ枠 png
#define ITEM2 TEXT("img/hukidasi.png")						          //ドロップ枠 png
#define FACETIME 300												  //表情変化の時間
#define MW 201                                                        //キャラからの相対配置
#define MH 428                                                        //キャラからの相対配置
#define DROPWIDTH 202												  //ドロップウィンドウの横幅
#define DROPHEIGHT 87												  //ドロップウィンドウの縦幅
#define MessageW 200												  //吹き出しの横幅
#define MessageH 85													  //吹き出しの縦幅
#define MessageTW 155										          //吹き出し（テキスト）の横幅
#define MessageTH 65										          //吹き出しの（テキスト）縦幅	
#define DROPWPOT 3											          //補正サイズ
#define DROPHPOT 210										          //補正サイズ
#define DHOSEI 6											          //補正サイズ
#define DWOSEI 5											          //補正サイズ
#define SHOSEI 11											          //補正サイズ
#define MTX 130												          //補正サイズ(テキスト)
#define MTY 110												          //補正サイズ(テキスト)
#define TXHOSEI 9											          //補正サイズ(テキスト)
#define TYHOSEI 10											          //補正サイズ(テキスト）
#define WM_DMESS WM_USER											  //特殊表情変化
#define WM_GTIME WM_USER+1											  //ドロップアニインメーション
#define WM_DCTIME WM_USER+2                                           //ドロップアウトアニメーション
#define WM_CHDROPM WM_USER+3                                          //ドロップメニューの領域切り替え
#define DROPCOLOR RGB(240,248,255)                                    //ドロップ領域の背景色
#define RSSIMG TEXT("img/rss.bmp")
#define RSSW 343
#define RSSH 108
#define RSSHOSEIL 350

/*----------------------------------------------------------*/
/*			          プロトタイプ宣言			
/*----------------------------------------------------------*/

LRESULT CALLBACK WindowProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK WindowProcDropIcon(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK WindowProcDropIconLy(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK WindowProcMessageLy(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK WindowProcMessage(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK rssWindow(HWND Dialog, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EdTitlePro(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK SystemEdit(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK WordProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EdWordProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DelWordProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK rssEditProc(HWND Dialog, UINT uMsg, WPARAM wParam, LPARAM lParam);
ATOM InitAppWindow(HINSTANCE);
BOOL CreateAppWindow(HINSTANCE);
HBITMAP LoadPngFromFile(HWND hWnd, FILE *fp, unsigned &uWidth, unsigned &uHeight);
void filePath(TCHAR *);
void exfilePath(TCHAR *);
void readyMenuFile(dMenu *,TCHAR *,TCHAR *,TCHAR *,TCHAR *,int);
void CreatePngLext(HWND,HBITMAP,int);
void Message(HWND,HWND,TCHAR *,int,int,int);
void CloseMessage(HWND,HWND);
void CloseWindows();
void ExStartUp();
void SystemInit();
void createTagList(HWND);
void timeData(TCHAR *);
void DelcreateWordList(HWND);
void DelSetTagName(HWND,int);
int  delWord(int,TCHAR *);
int addWord(TCHAR *,TCHAR *,TCHAR *);
int createOTMessage(TCHAR *,int);
int Controller(int,char *,char *,unsigned short);
int chekUrl(TCHAR *);
void SetListRssDataName(TCHAR *);
int GetRssSlspeed(TCHAR *path);

/*---------------------------------------------------------*/
/*			           グローバル変数			
/*----------------------------------------------------------*/

HWND hWnds,hWnd,DhWnd,LhWnd,MLhWnd,MhWnd,sDlg,dwhWnd;            //各ウィンドウハンドル
HWND whWnd,wRss,mRss;                                            //ウィンドウハンドル
HINSTANCE ghInstance;                                            //インスタンス変数
HBITMAP hBitmap1,hBitmap2,hBitmap3,hBitmap4,hBitmap5;            //ビットマップ用
HBITMAP hBitmap6,hWaku,Mes;                                      //ビットマップ用
unsigned uWidth = 0, uHeight = 0;                                //pngサイズ
unsigned dWidth = 0, dHeight = 0;                                //pngサイズ保存
int ImgPoint=0;                                                  //画像推移保存値
int wspeed=0;                                                    //ワード表示のカウント
int cpx=0,cpy=0;                                                 //ドロップ領域保存値
int X = ((GetSystemMetrics( SM_CXSCREEN ) - 300) / 2) ;          //起動配置X
int Y = ((GetSystemMetrics( SM_CYSCREEN ) - 400 ) / 2);          //起動配置Y
int wordTime=0;                                                  //ワード制限カウント
double cpum=0.0;												//CPU率
int rssSpeed=0;

/* フラグの詳細は別途「フラグ仕様」に記載してある */

BOOL handFlag=FALSE;                                             //カーソルの種類
BOOL pFlag=FALSE;                                                //固定の有無                                          
BOOL MessageCloseFlag=FALSE;                                     //メッセージクローズフラグ
BOOL errorMessage=FALSE;                                         //エラーメッセージフラグ
BOOL popFlag=FALSE;                                              //ドロップアイコンのタイトル変更フラグ
BOOL EdTitleFlag=FALSE;                                          //タイトルフラグ
BOOL stFlag=FALSE;                                               //スタートアップ
BOOL taskFlag=FALSE;                                             //タスク表示
BOOL animem=FALSE;                                               //ドロップアニメーションの有無
BOOL ClickLeft=FALSE;                                            //左クリックの有無
BOOL outWord=FALSE;                                              //ワード生成の制御
BOOL actWord=FALSE;                                              //ワード生成のチェック
BOOL popDMenu=FALSE;                                             //ドロップ上での制御
BOOL StTime=FALSE;                                               //日付ワード
BOOL rssFlag=FALSE;
BOOL rssSetFlag=FALSE;
BOOL rssStart = FALSE;
TCHAR *section =TEXT("menu");								     //セクション名（初期化ファイル）
TCHAR *section2 =TEXT("menu2");								     //セクション名（初期化ファイル）
TCHAR *tarGetSection = section;                                  //ターゲットセクション
TCHAR *key1 =TEXT("path");									     //キー名（初期化ファイル）
TCHAR *key2 =TEXT("name");									     //キー名（初期化ファイル）
TCHAR path[MAX_PATH];										     //初期化ファイルのパス
TCHAR tmppath[MAX_PATH];										 //初期化ファイルの保存パス
TCHAR keyBuff[72];                                               //入力ワード
dMenu dMenuData[DROPSIZEX*DROPSIZEY]={NULL,TEXT(""),TEXT("")};   //ドロップメニュー用構造体配列
dMenu dMenuDataN[DROPSIZEX*DROPSIZEY]={NULL,TEXT(""),TEXT("")};  //ドロップメニュー用構造体配列
dMenu *tarGetdMenu = dMenuData;	                                 //ドロップメニューターゲット構造体		



/*----------------------------------------------------------*/
/*			            ハンドル取得	
/*----------------------------------------------------------*/

BOOL CALLBACK  EnumWndProc( HWND hWnd, LPARAM lParam )
{

	TCHAR WndName[MAX_PATH];
    GetWindowText(hWnd, WndName,MAX_PATH);

	if(lstrcmp(WndName,TEXT("NA"))==0){

		SendMessage(hWnd,WM_DMESS,0,0);

		return FALSE;
	
	}

	return TRUE;

}

/*----------------------------------------------------------*/
/*			        エントリーポイント		
/*----------------------------------------------------------*/

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{

	MSG msg;
	int buffLeft;
	int buffTop;

	//多重起動のチェック
	if(FindWindow(APP_NAME,NULL)!=NULL){
			
		EnumWindows( EnumWndProc,0);

		return 0;
	}

	//記憶ファイルのフルパスを取得
    GetModuleFileName(NULL,path,MAX_PATH);
	filePath(path);
	lstrcat(path,TEXT("nasystem.ini"));
	lstrcat(path,TEXT("\0"));
	
	buffLeft = GetPrivateProfileInt(TEXT("window"),TEXT("left"),-1000,path);
	buffTop = GetPrivateProfileInt(TEXT("window"),TEXT("top"),-1000,path);


	//初期起動位置の設定
	if(buffLeft!=-1000 && buffTop!=-1000){

		X = buffLeft;
		Y = buffTop;

	}


	if(!InitAppWindow(hInstance)){
		MessageBox(NULL,TEXT("ウィンドウクラスの作成に失敗しました"),NULL,MB_OK);
		return 0;
	}else{
		
		if(!CreateAppWindow(hInstance)){
			MessageBox(NULL,TEXT("ウィンドウクラスの生成に失敗しました"),NULL,MB_OK);
			return 0;
		}
	}
	
	//メッセージループ
	while(GetMessage(&msg,NULL,0,0) > 0){

		if(!whWnd || !IsDialogMessage(whWnd,&msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}


	return msg.wParam;
}

/*----------------------------------------------------------*/
/*		　　	  ウィンドウプロシージャー			
/*----------------------------------------------------------*/
LRESULT CALLBACK WindowProc( HWND hWnd,UINT uMsg,WPARAM wParam, LPARAM lParam){

	static RECT nrct,brct,arct;
	static HMENU hMenu,hSubMenu;
	static MENUITEMINFO menuinfo,getmenuinfo;
	static NOTIFYICONDATA nid;
	static HQUERY   hQuery;
	static HCOUNTER hCounter;
	static PDH_FMT_COUNTERVALUE fntValue;
	static char cpuData[10];
	static BOOL ImgOK = FALSE,bC = FALSE;
	static int IMGTIME=0,IMGOVERTIME=0;
	static int imgal=255,wp=0;

	WINDOWPLACEMENT wndpl;
	POINT mouse,pos;
	RECT rc;
	BLENDFUNCTION blend;

	FILE *fp1;					
	FILE *fp2;					
	FILE *fp3;				
	FILE *fp4;
	FILE *fp5;
	FILE *fp6;

	switch(uMsg){

		case WM_CREATE:

		   GetModuleFileName(NULL,tmppath,MAX_PATH);
		   filePath(tmppath);

		    //イメージパスの取得
			lstrcpy(img_path1,tmppath);
			lstrcat(img_path1,IMG_NA1);
			lstrcpy(img_path2,tmppath);
			lstrcat(img_path2,IMG_NA2);
			lstrcpy(img_path3,tmppath);
			lstrcat(img_path3,IMG_NA3);
			lstrcpy(img_path4,tmppath);
			lstrcat(img_path4,IMG_NA4);
			lstrcpy(img_path5,tmppath);
			lstrcat(img_path5,IMG_NA5);
			lstrcpy(img_path6,tmppath);
			lstrcat(img_path6,IMG_NA6);

			lstrcpy(img_item1,tmppath);
			lstrcat(img_item1,ITEM1);
			lstrcpy(img_item2,tmppath);
			lstrcat(img_item2,ITEM2);


			//文字コードの変換
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path1,lstrlen(img_path1),img_path_n1,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path2,lstrlen(img_path2),img_path_n2,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path3,lstrlen(img_path3),img_path_n3,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path4,lstrlen(img_path4),img_path_n4,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path5,lstrlen(img_path5),img_path_n5,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path6,lstrlen(img_path6),img_path_n6,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_item1,lstrlen(img_item1),img_item_n1,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_item2,lstrlen(img_item2),img_item_n2,MAX_PATH ,NULL,NULL);


			//PNGの読み出し
			fp1 = fopen(img_path_n1,"rb");
			fp2 = fopen(img_path_n2, "rb");
			fp3 = fopen(img_path_n3, "rb");
			fp4 = fopen(img_path_n4, "rb");
			fp5 = fopen(img_path_n5, "rb");
			fp6 = fopen(img_path_n6, "rb");

			if (fp1 == NULL) {
                   MessageBox(hWnd, TEXT(".pngが開けません。"), TEXT("エラー"), MB_OK | MB_ICONSTOP);
                   DestroyWindow(hWnd);
                   break;
            }

		    //メモリに読み込んだpngファイルを、ビットマップハンドルにする
			hBitmap1 = LoadPngFromFile(hWnd, fp1, uWidth, uHeight);		    
			hBitmap2 = LoadPngFromFile(hWnd, fp2, uWidth, uHeight);
			hBitmap3 = LoadPngFromFile(hWnd, fp3, uWidth, uHeight);		    
			hBitmap4 = LoadPngFromFile(hWnd, fp4, uWidth, uHeight);
			hBitmap5 = LoadPngFromFile(hWnd, fp5, uWidth, uHeight);		    
			hBitmap6 = LoadPngFromFile(hWnd, fp6, uWidth, uHeight);

			fclose(fp1);
			fclose(fp2);
			fclose(fp3);
			fclose(fp4);
			fclose(fp5);
			fclose(fp6);

			//メイン画像(png)表示
			CreatePngLext(hWnd,hBitmap1,255);

			//サイズ情報コピー
			dWidth = uWidth;
			dHeight = uHeight;

			//CPU使用率の準備
			//パフォーマンス・データのハンドルを作成
			if ( PdhOpenQuery(NULL,0,&hQuery) == ERROR_SUCCESS ){
				 //ハンドル総数のカウンタパスを追加
				 PdhAddCounter( hQuery, TEXT("\\Processor(_Total)\\% Processor Time"), 0, &hCounter );
				 //ハンドル総数のカウンタ値を取得
				 PdhCollectQueryData( hQuery );
				 //タイマーの開始
				 SetTimer(hWnd,1,FACETIME,NULL);
				 //初期表情
				 IMGTIME=1;
			}

			//RECT構造体にセット(NAマークのとこ)
			SetRect(&nrct,10,10,70,70);
			//(リボンのとこ)
			SetRect(&brct,85,170,105,190);
			//(Aのとこ)
			SetRect(&arct,140,10,170,90);

			//設定ダイアログの生成
			sDlg = CreateDialog(
				ghInstance,
				MAKEINTRESOURCE(IDD_DIALOG2) , NULL ,(DLGPROC)SystemEdit
			);

			//RSS用初期化ファイルのパス設定
			initRss(tmppath);

			wRss = CreateDialog(
				ghInstance,
				MAKEINTRESOURCE(IDD_DIALOG4) ,NULL,(DLGPROC)rssEditProc
			);

			//RSS処理
			readRssData();

			//メニューのハンドル
			hMenu = LoadMenu((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_MENU1));


		return 0;

		//タイマーイベント
		case WM_TIMER:

			//ハンドル総数のカウンタ値を取得
			PdhCollectQueryData( hQuery );
			// double型の表示
			PdhGetFormattedCounterValue( hCounter, PDH_FMT_DOUBLE, NULL, &fntValue );
			cpum = fntValue.doubleValue;

			//サイズ情報コピー
			uWidth = dWidth;
			uHeight = dHeight;

			//CPUの使用率によって表情データをメモリデバイスに
			if(!EdTitleFlag){

				//特殊表情推移
				if(IMGTIME){

					switch(IMGTIME){

						case 1:
							CreatePngLext(hWnd,hBitmap1,imgal);
						break;
						case 4:
							CreatePngLext(hWnd,hBitmap4,imgal);
						break;

					}

					//特殊表情の終了時間
					if(IMGOVERTIME++>20){
						IMGTIME=0;
						IMGOVERTIME=0;
						MessageCloseFlag=FALSE;
						errorMessage=FALSE;
						CloseMessage(MhWnd,MLhWnd);
					}


				}else{
					if(cpum<=20 && ImgPoint!=1){
						CreatePngLext(hWnd,hBitmap1,imgal);
						ImgPoint=1;
						ImgOK = TRUE;
					}else if(cpum>20 && cpum<=40 && ImgPoint!=2){
						CreatePngLext(hWnd,hBitmap2,imgal);
						ImgPoint=2;
					}else if(cpum>40 && cpum<=60 && ImgPoint!=3){
						CreatePngLext(hWnd,hBitmap3,imgal);
						ImgPoint=3;
					}else if(cpum>60 && cpum<=80 && ImgPoint!=4){
						CreatePngLext(hWnd,hBitmap4,imgal);
						ImgPoint=4;
					}else if(cpum>=80 && ImgPoint!=5){
						CreatePngLext(hWnd,hBitmap5,imgal);
						ImgPoint=5;
					}
				}
			}


			if(actWord){
				if(!outWord && !IMGTIME && !errorMessage && !taskFlag){
						//ワードの制御
						if(wordTime>wspeed){

								//ワードの生成
								if(!wp){
									createOTMessage(tmppath,0);
									if(lstrlen(words)){
										MessageCloseFlag = TRUE;
										Message(MhWnd,MLhWnd,words,MessageTW,MessageTH,0);
										wp=1;
									}else{
										wordTime=0;
									}
								}

								//ワードの破棄
								if(wordTime>wspeed+25 && wp){
									CloseMessage(MhWnd,MLhWnd);
									MessageCloseFlag = FALSE;
									wordTime=0;
									wp=0;
								}

						}

						 wordTime++;
				//他イベント時ならワード生成しない	
				}else{
					wordTime=0;
				}
			}


		return 0;

		//マウスが動いたら
		case WM_NCMOUSEMOVE:

			//マウス座標の取得
			mouse.x = LOWORD(lParam);
			mouse.y = HIWORD(lParam);

			//座標変換
			ScreenToClient(hWnd,&mouse);

			//マウス座標は登録したRECT内かどうか
			if(PtInRect(&nrct,mouse)){
				handFlag = TRUE;
				bC = FALSE;
				if(ImgPoint!=6 && ImgOK && !IMGTIME && !EdTitleFlag){
					CreatePngLext(hWnd,hBitmap6,imgal);
					ImgPoint=6;
				}
			}else if(PtInRect(&brct,mouse)){
				handFlag = TRUE;
				bC = TRUE;
			}else if(PtInRect(&arct,mouse)){
				handFlag = TRUE;
			}else{
				handFlag = FALSE;
				bC = FALSE;
			}

		return 0;	


		//ウィンドウが動いたら
		case WM_MOVING:

			//ウィンドウの位置を固定する
			wndpl.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wndpl);
			rc = wndpl.rcNormalPosition;


			if(pFlag){

				//位置を固定する
				((RECT*)lParam)->left = rc.left;
				((RECT*)lParam)->top = rc.top;
				((RECT*)lParam)->right = rc.right;
				((RECT*)lParam)->bottom = rc.bottom;
			}

		break;

		//ウィンドウの移動直後
		case WM_MOVE:

			//ウィンドウの位置を固定する
			wndpl.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wndpl);
			rc = wndpl.rcNormalPosition;

			//ウィンドウの移動
			MoveWindow(MLhWnd,rc.left-MTX-TYHOSEI,rc.top+MTY-TYHOSEI,MessageW,MessageH,NULL);
			MoveWindow(LhWnd,rc.left-DROPWPOT,rc.top+MH-DROPHPOT,DROPWIDTH,DROPHEIGHT,NULL);
			SetWindowPos(MhWnd,NULL,rc.left-MTX,rc.top+MTY,0,0,SWP_NOSIZE);
			SetWindowPos(DhWnd,NULL,rc.left-DROPWPOT+DWOSEI,rc.top+MH-DROPHPOT+DHOSEI,0,0,SWP_NOSIZE);
			SetWindowPos(mRss,NULL,rc.left-RSSHOSEIL,rc.top+195,RSSW,RSSH,NULL);

			//クリック時のアルファ減
			if(ClickLeft){

				imgal=150;
				blend.BlendOp = AC_SRC_OVER;
				blend.BlendFlags = 0;
				blend.SourceConstantAlpha = imgal;
				blend.AlphaFormat = AC_SRC_ALPHA;
				UpdateLayeredWindow(hWnd, NULL, NULL, NULL, NULL, NULL,NULL, &blend, ULW_ALPHA);
	
			}

		break;

		case WM_EXITSIZEMOVE:

			ClickLeft=FALSE;

			//アルファ値を元に戻す
			imgal=255;
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			blend.SourceConstantAlpha = imgal;
			blend.AlphaFormat = AC_SRC_ALPHA;
			UpdateLayeredWindow(hWnd, NULL, NULL, NULL, NULL, NULL,NULL, &blend, ULW_ALPHA);

		break;

		//左クリック
		case WM_NCLBUTTONDOWN:

			//マウス座標の取得
			mouse.x = LOWORD(lParam);
			mouse.y = HIWORD(lParam);

			//座標変換
			ScreenToClient(hWnd,&mouse);

			//ドロップメニューのポップアップが表示されていたら
			if(popDMenu){
				popFlag=FALSE;
				popDMenu=FALSE;
			}

			if(handFlag && bC){

				SendMessage(DhWnd,WM_CHDROPM,0,0);

			}else if(handFlag && !animem && PtInRect(&nrct,mouse)){

				animem=TRUE;
				popFlag=FALSE;

				//ドロップメニューの表示
				ShowWindow(LhWnd,SW_SHOW);
				ShowWindow(DhWnd,SW_SHOW);
				UpdateWindow(DhWnd);

				//表示アニメーションのメッセージ送信
				SendMessage(LhWnd,WM_GTIME,0,0);

			}else if(handFlag && PtInRect(&arct,mouse) && !rssFlag){


				mRss = CreateDialog(
					ghInstance,
					MAKEINTRESOURCE(IDD_DIALOG5) ,NULL,(DLGPROC)rssWindow
				);

				ShowWindow(mRss,SW_SHOW);

				rssFlag=TRUE;

			}

			ClickLeft=TRUE;

		break;


		//右クリック
		case WM_NCRBUTTONDOWN:

			// サブメニューのハンドルを取得する
			hSubMenu = GetSubMenu( hMenu, 0 );

			//マウスカーソルの座標取得
			pos.x = LOWORD(lParam);
			pos.y = HIWORD(lParam);
		
			// ポップアップメニューを表示する
			TrackPopupMenu( hSubMenu,TPM_LEFTALIGN,pos.x,pos.y,0,hWnd, NULL );
		
		return 0;

		//左ダブルクリック
		case WM_NCLBUTTONDBLCLK:

			SendMessage(LhWnd,WM_DCTIME,0,0);
				
		return 0;

		//メニュー制御
		case WM_COMMAND:

			switch(LOWORD(wParam))
			{
				//終了
				case ID_DES:
					SendMessage(hWnd,WM_CLOSE,0,0);
				return 0;


				case ID_WORD:

					whWnd = CreateDialog(
						ghInstance,
						MAKEINTRESOURCE(IDD_WORDW) ,NULL,(DLGPROC)WordProc
					 );

					ShowWindow(whWnd,SW_SHOW);

					EdTitleFlag=TRUE;
					popFlag=TRUE;

				return 0;

				case ID_DWORD:

					dwhWnd = CreateDialog(
						ghInstance,
						MAKEINTRESOURCE(IDD_DIALOG3) ,NULL,(DLGPROC)DelWordProc
					 );

					ShowWindow(dwhWnd,SW_SHOW);

					EdTitleFlag=TRUE;
					popFlag=TRUE;

				return 0;

				case ID_RSS:

					ShowWindow(wRss,SW_SHOW);

					EdTitleFlag=TRUE;
					popFlag=TRUE;

				break;

				//タスクに格納する
				case ID_TK:

					taskFlag=TRUE;

					nid.cbSize = sizeof(NOTIFYICONDATA);
					nid.hWnd=hWnd;
					nid.uID=ID_TASK; //ID_TASKはこのアイコンに割り当てたID(自分で定義する)
					nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //メッセージとツールチップとアイコンを有効にする
					nid.uCallbackMessage = MYWM_NOTIFYICON; //ウィンドウに送られるメッセージのID(自分で定義する)
					nid.hIcon= LoadIcon((HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDI_ICON1));

					lstrcpy(nid.szTip,TEXT("NAちゃん"));
					Shell_NotifyIcon(NIM_ADD,&nid);

					CloseWindows();

				return 0;

				case ID_STOP:

					getmenuinfo.cbSize = sizeof (MENUITEMINFO);
					getmenuinfo.fMask = MIIM_SUBMENU;
					GetMenuItemInfo(hSubMenu,0,1,&getmenuinfo);

					menuinfo.cbSize = sizeof (MENUITEMINFO);
					menuinfo.fMask = MIIM_STATE;
					GetMenuItemInfo(getmenuinfo.hSubMenu,ID_STOP,FALSE, &menuinfo);

					if(menuinfo.fState==MFS_CHECKED){
						menuinfo.fState = MFS_UNCHECKED;
						pFlag = FALSE;
						SetMenuItemInfo(getmenuinfo.hSubMenu,ID_STOP,FALSE,&menuinfo);
					}else{
						menuinfo.fState = MFS_CHECKED;
						pFlag = TRUE;
						SetMenuItemInfo(getmenuinfo.hSubMenu,ID_STOP,FALSE,&menuinfo);
					}

					DrawMenuBar(hWnd);

				return 0;

				case ID_TOM:

					getmenuinfo.cbSize = sizeof (MENUITEMINFO);
					getmenuinfo.fMask = MIIM_SUBMENU;
					GetMenuItemInfo(hSubMenu,0,1,&getmenuinfo);

					menuinfo.cbSize = sizeof (MENUITEMINFO);
					menuinfo.fMask = MIIM_STATE;

					GetMenuItemInfo(getmenuinfo.hSubMenu,ID_TOM,FALSE, &menuinfo);

					if(menuinfo.fState==MFS_CHECKED){

						menuinfo.fState = MFS_UNCHECKED;
						SetMenuItemInfo(getmenuinfo.hSubMenu,ID_TOM,FALSE,&menuinfo);

						SetWindowPos(hWnd,HWND_NOTOPMOST,NULL,NULL,NULL,NULL, SWP_NOMOVE | SWP_NOSIZE);
	
					}else{

						menuinfo.fState = MFS_CHECKED;
						SetMenuItemInfo(getmenuinfo.hSubMenu,ID_TOM,FALSE,&menuinfo);

						SetWindowPos(hWnd,HWND_TOPMOST,NULL,NULL,NULL,NULL,SWP_NOMOVE | SWP_NOSIZE);

					}

					DrawMenuBar(hWnd);

				return 0;


				case ID_SED:

					EdTitleFlag=TRUE;
					popFlag=TRUE;
					ShowWindow(sDlg,SW_SHOW);

				return 0;

				
			}

		return 0;

		//タスクバー上
		case MYWM_NOTIFYICON: 
	     
			switch(lParam)
			{
				case WM_LBUTTONDBLCLK:

					taskFlag=FALSE;

					Shell_NotifyIcon(NIM_DELETE,&nid);
					
  					ShowWindow(hWnd,SW_SHOW);

				return 0;
		 
			}  

		return 0;

		//キーが押されたら
		case WM_KEYUP:
		
			if(wParam==VK_ESCAPE){
				SendMessage(hWnd,WM_CLOSE,0,0);
			}

		return 0;
		
		//ALT+F4対策
		case WM_SYSCOMMAND:

			if(wParam==SC_CLOSE){
				
				return 0;

			}

		break;

		//カーソル
		case WM_SETCURSOR:

			//フラグがあればカーソルの変更
			if(handFlag){

				SetCursor(LoadCursor(NULL,IDC_HAND));

			 }else{

				SetCursor(LoadCursor(NULL,IDC_ARROW));

			 }

	    return 0;

		//オリジナルメッセージ（特殊表情の変化）
		case WM_DMESS:
			if(!taskFlag){
				IMGTIME = 4;
				MessageCloseFlag = TRUE;
				errorMessage=TRUE;
				CreatePngLext(hWnd,hBitmap4,255);
				Message(MhWnd,MLhWnd,TEXT("浮気しちゃだめー！"),MessageTW,MessageTH,0);
			}
		return 0;

		//ヒットテスト
		case WM_NCHITTEST:
			return HTCAPTION;
	
		//破棄
		case WM_DESTROY:

			PdhCloseQuery(hQuery);
			//ビットマップの破棄
			DeleteObject(hBitmap1);
			DeleteObject(hBitmap2);
			DeleteObject(hBitmap3);
			DeleteObject(hBitmap4);
			DeleteObject(hBitmap5);
			DeleteObject(hBitmap6);

			//タスクトレイからアイコンを削除
			Shell_NotifyIcon(NIM_DELETE,&nid);
		
			createOTMessage(NULL,3);

			rssPointDataFree();

			PostQuitMessage(0);

		return 0;
			
	}
	
	return DefWindowProc(hWnd,uMsg,wParam,lParam);

}



/*----------------------------------------------------------*/
/*		  ドロップアイコン用ウィンドウプロシージャ			
/*----------------------------------------------------------*/

LRESULT CALLBACK WindowProcDropIcon(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	static HWND EdTitle;
	static HDC Dhdc,DhMemDC;
	static HMENU dhMenu,dhSubMenu;
	static HBITMAP Dhbitmap;
	static int DP[2][5]={0};
	static int DPN[2][5]={0};
	static int *tarDP;
	static int swi=0;
	static RECT drct[2][5];
	static RECT winrct;
	static POINT mp,dropP,poss;
	static BOOL dhandFlag=FALSE;
	static HDROP hDrop;
	static HICON icon;
	static HBRUSH ohbrush;
	static HBRUSH back_brush = CreateSolidBrush(DROPCOLOR);
	static TCHAR *buff;


	TCHAR strFilePath[MAX_PATH];
	PAINTSTRUCT ps;
	int x,y,di,dj,pi,pj,ci,cj,flag=0,tx=0,ty=0,dpoint,dn,fileopenext;
	
	UINT uiFileCount,size;
	SHFILEINFO shfi;

	char nameBuff[MAX_PATH];

	DWORD_PTR	dwRet;
	IShellLink   *pShellLink;     // IShellLinkへのポインタ
	IPersistFile *pPersistFile;   // IPersistFileへのポインタ

	WINDOWPLACEMENT wndpl;
	RECT rc;
	HIMAGELIST list;

	TCHAR szProductCode[39];
    TCHAR szFeatureId[MAX_FEATURE_CHARS + 1];
    TCHAR szComponentCode[39];
	DWORD dwBuffSize = MAX_PATH;


    switch (uMsg) {

        case WM_CREATE:

			 //初期ドロップ対象
			 tarDP = (int *)DP;

			 size = lstrlen(key1)*sizeof(TCHAR);
			 buff = (TCHAR *)malloc(size+6);

			 //ドロップの許可
			 DragAcceptFiles(hWnd,TRUE);

			 DhMemDC = CreateCompatibleDC(GetDC(DhWnd));
			 Dhbitmap = CreateCompatibleBitmap(GetDC(DhWnd),DROPWIDTH,DROPHEIGHT);

			 SelectObject(DhMemDC,Dhbitmap);
			
			 //背景を指定色
			 SelectObject(DhMemDC,GetStockObject(NULL_PEN));
			 SelectObject(DhMemDC,back_brush);
			 Rectangle(DhMemDC,0,0,DROPWIDTH,DROPHEIGHT);

			 ohbrush = CreateSolidBrush(RGB(204,204,204));
			 
			 //各ドロップフィールドの作成
			 x=5;y=4;
			 for(di=0;di<DROPSIZEY;di++){
				 for(dj=0;dj<DROPSIZEX;dj++){
					Rectangle(DhMemDC,x,y,x+32,y+32);
					SetRect(&drct[di][dj],x,y,x+32,y+32);
					x+=37;
				 }
				 x=5;
				 y+=35;
			 }

			 //各フィールド状況の更新
			 readyMenuFile(dMenuData,section,key1,key2,path,DROPSIZEX*DROPSIZEY);
			 readyMenuFile(dMenuDataN,section2,key1,key2,path,DROPSIZEX*DROPSIZEY);

			 dn=0;
			 while(dn<DROPSIZEX*DROPSIZEY){

				 if(dMenuData[dn].dIcon!=NULL){
					y=dn/DROPSIZEX;
					x=dn%DROPSIZEX;
					DrawIcon(DhMemDC,drct[y][x].left,drct[y][x].top,dMenuData[dn].dIcon);
					DP[y][x]=1;
				 }

				dn++;
			 }

			 //タイトル変更用ダイアログの生成
			 EdTitle = CreateDialog(
				ghInstance,
				MAKEINTRESOURCE(IDD_DIALOG1) , hWnds ,(DLGPROC)EdTitlePro
			 );

			 SetTimer(hWnd,2,100,NULL);

		return 0;

		case WM_DROPFILES:
				
			//パラメーターの取得
			hDrop = (HDROP)wParam;
			//ドロップ数
			uiFileCount = DragQueryFile(hDrop,0xFFFFFFFF,NULL,0);
	
			if(uiFileCount<=1){

				//ファイルパスの取得
				DragQueryFile(hDrop,0,strFilePath,MAX_PATH);

				DragQueryPoint(hDrop,&dropP);


				//ドロップ領域の特定
				flag=0;
				for(ci=0;ci<DROPSIZEY;ci++){
					for(cj=0;cj<DROPSIZEX;cj++){
			
						if(PtInRect(&drct[ci][cj],dropP)){
							*(tarDP+(ci*5)+cj)=1;
							ty = ci;
							tx = cj;
							flag=1;
							break;
						}
					}

				}

				//MSIショートカットチェック
				if(ERROR_SUCCESS == MsiGetShortcutTarget(strFilePath, szProductCode, szFeatureId, szComponentCode)){
					
					//MSIショートカットからのパスを得る
					MsiGetComponentPath(szProductCode, szComponentCode,strFilePath,&dwBuffSize);

				}else{

					//通常ショートカットかつショートカットかどうか
					//IShellLinkインターフェイスを使う
					dwRet = SHGetFileInfo(strFilePath,0,&shfi,sizeof(SHFILEINFO),SHGFI_ATTRIBUTES);
					if(dwRet){
						//ショートカットならば
						if(shfi.dwAttributes & SFGAO_LINK){

							//OLEの初期化
							CoInitialize(NULL);

							//空のインターフェースを作成
							CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(LPVOID *)&pShellLink);

							//IPersistFileへのポインタを取得
							pShellLink->QueryInterface(IID_IPersistFile,(LPVOID *)&pPersistFile);
					
							//ショートカットを読み込み。
							pPersistFile->Load(strFilePath, STGM_READ );

							//リンクを決定する
							pShellLink->Resolve( NULL, SLR_UPDATE );

							//パスを取得
							pShellLink->GetPath(strFilePath,MAX_PATH,NULL,0);

							if(!lstrlen(strFilePath)){

								DragQueryFile(hDrop,0,strFilePath,MAX_PATH);

							}

							//解放
							pPersistFile->Release();
							pShellLink->Release();
							CoUninitialize();

						}

					}
				
				}

				list = (HIMAGELIST)SHGetFileInfo(strFilePath, FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof(SHFILEINFO),SHGFI_SYSICONINDEX  | SHGFI_LARGEICON);
				icon = ImageList_GetIcon(list,shfi.iIcon, ILD_NORMAL);

				//構造体の更新
				if(flag){

					dpoint = tx+(ty*(DROPSIZEX));
					wsprintf(buff,TEXT("%s%d"),tarGetSection,dpoint);

					//パスがドライブのみだったら
					if(strFilePath[1]==':' && strFilePath[2]=='\\' && strFilePath[3]==NULL){

						lstrcpy((tarGetdMenu+dpoint)->dname,strFilePath);
						WritePrivateProfileString(buff,key2,strFilePath,path);

					}else{

						//ファイルの名前を取得
						GetFileTitle(strFilePath,(LPWSTR)nameBuff,sizeof(nameBuff));

						lstrcpy((tarGetdMenu+dpoint)->dname,(LPCWSTR)nameBuff);
						WritePrivateProfileString(buff,key2,(LPCWSTR)nameBuff,path);

					}

					(tarGetdMenu+dpoint)->dIcon = icon;

					lstrcpy((tarGetdMenu+dpoint)->dPath,strFilePath);

					WritePrivateProfileString(buff,key1,strFilePath,path);
					

					//領域を白に
					SelectObject(DhMemDC,back_brush);
					Rectangle(DhMemDC,drct[ty][tx].left,drct[ty][tx].top,drct[ty][tx].left+35,drct[ty][tx].top+35);
					InvalidateRect(hWnd,NULL,FALSE);

					//アイコンの描画
					DrawIcon(DhMemDC,drct[ty][tx].left,drct[ty][tx].top,icon);
					InvalidateRect(hWnd,NULL,FALSE);
				}


			}else{
				Message(MhWnd,MLhWnd,TEXT("ドロップできないよ"),MessageTW,MessageTH,0);
				errorMessage=TRUE;
			}


		return 0;

		case WM_PAINT:

			//クライアント領域のデバイスコンテキストの取得
			Dhdc = BeginPaint(hWnd,&ps);

			//仮想からデバイスへの転送
			BitBlt(
					Dhdc,0,0,DROPWIDTH,DROPHEIGHT,DhMemDC,0,0,SRCCOPY
			);

			EndPaint(hWnd,&ps);

		return 0;

		case WM_TIMER:

			//一定時間ごとにドロップ領域の監視
			GetCursorPos(&mp);
			ScreenToClient(hWnd,&mp);
			GetClientRect(hWnd,&winrct);

			if(!IsWindowVisible(DhWnd)){
				popFlag=TRUE;
			}

			if(!popFlag){

				//カーソルがドロップメニュー領域なら
				if(!PtInRect(&winrct,mp)){

				if(outWord){ outWord=FALSE; }

				//領域を白に
				for(pi=0;pi<DROPSIZEY;pi++){
					for(pj=0;pj<DROPSIZEX;pj++){
							if(!*(tarDP+(pi*5)+pj)){
								SelectObject(DhMemDC,back_brush);
								Rectangle(DhMemDC,drct[pi][pj].left,drct[pi][pj].top,drct[pi][pj].left+33,drct[pi][pj].top+33);
							}
						}
					}

				//メッセージの調整
				if(IsWindowVisible(MhWnd) && !MessageCloseFlag){
					CloseMessage(MhWnd,MLhWnd);
				}

				//エラーメッセージのフラグを戻す
				if(errorMessage){
					errorMessage=FALSE;
				}

				}else{

				if(!outWord){ outWord=TRUE; }

				if(MessageCloseFlag){
					MessageCloseFlag = FALSE;
				}


					for(pi=0;pi<DROPSIZEY;pi++){
						for(pj=0;pj<DROPSIZEX;pj++){
							if(PtInRect(&drct[pi][pj],mp)){
								dhandFlag = TRUE;
								cpy=pi;
								cpx=pj;
								if(!*(tarDP+(pi*5)+pj)){
									SelectObject(DhMemDC,ohbrush);
									//領域を灰色にする
									RoundRect(DhMemDC,drct[pi][pj].left,drct[pi][pj].top,drct[pi][pj].left+32,drct[pi][pj].top+32,10,10);
									if(!errorMessage){
										CloseMessage(MhWnd,MLhWnd);
									}
								}else{
									if(!errorMessage && IsWindowVisible(hWnd)){
										dpoint = cpx+(cpy*(DROPSIZEX));
										//吹き出しの表示
										Message(MhWnd,MLhWnd,(tarGetdMenu+dpoint)->dname,MessageTW,MessageTH,1);
										SetForegroundWindow(hWnds);
									}
								}
							}else{
								if(!*(tarDP+(pi*5)+pj)){
									SelectObject(DhMemDC,back_brush);
									Rectangle(DhMemDC,drct[pi][pj].left,drct[pi][pj].top,drct[pi][pj].left+32,drct[pi][pj].top+32);
								}
							}
						}
					}

			}

			InvalidateRect(hWnd,NULL,FALSE);

			}
		return 0;


		//切り替え
		case WM_CHDROPM:

			//領域を白に
			for(pi=0;pi<DROPSIZEY;pi++){
				for(pj=0;pj<DROPSIZEX;pj++){
					SelectObject(DhMemDC,back_brush);
					Rectangle(DhMemDC,drct[pi][pj].left,drct[pi][pj].top,drct[pi][pj].left+33,drct[pi][pj].top+33);
				}
			}

			if(!swi){

				//ドロップ格納情報の切り替え
				tarDP = (int *)DPN;

				//構造体の切り替え
				tarGetdMenu = (dMenu *)dMenuDataN;

				//セクションの切り替え
				tarGetSection = section2;

				swi=1;

			}else{

				//ドロップ格納情報の切り替え
				tarDP = (int *)DP;

				//構造体の切り替え
				tarGetdMenu = (dMenu *)dMenuData;

				//セクションの切り替え
				tarGetSection = section;

				swi=0;

			}

			//アイコンの描画
			dn=0;
			while(dn<DROPSIZEX*DROPSIZEY){

				 if((tarGetdMenu+dn)->dIcon!=NULL){
					y=dn/DROPSIZEX;
					x=dn%DROPSIZEX;
					DrawIcon(DhMemDC,drct[y][x].left,drct[y][x].top,(tarGetdMenu+dn)->dIcon);
					*(tarDP+(y*5)+x)=1;
				 }
				dn++;
			 }


		return 0;

		case WM_LBUTTONDOWN:

			if(dhandFlag && !popFlag){

				if(*(tarDP+(cpy*5)+cpx)){

					dpoint = cpx+(cpy*(DROPSIZEX));

					exfilePath((tarGetdMenu+dpoint)->dPath);

					fileopenext = (int)ShellExecute(hWnd,NULL,(tarGetdMenu+dpoint)->dPath,NULL,NULL,SW_SHOWNORMAL);
					if(fileopenext<=32){
						Message(MhWnd,MLhWnd,TEXT("ファイルが開けないよ"),MessageTW,MessageTH,0);
						errorMessage=TRUE;
						MessageCloseFlag = TRUE;
					}
				}
			}

			if(!EdTitleFlag){
				 popFlag=FALSE;
			}

		return 0;

		case WM_RBUTTONDOWN:

			if(dhandFlag && *(tarDP+(cpy*5)+cpx)){

				popFlag=TRUE;
				popDMenu=TRUE;

				//メニューのハンドル
				dhMenu = LoadMenu(ghInstance, MAKEINTRESOURCE(IDR_MENU2));
				// サブメニューのハンドルを取得する
				dhSubMenu = GetSubMenu(dhMenu, 0 );

				//マウスカーソルの座標取得
				poss.x = LOWORD(lParam);
				poss.y = HIWORD(lParam);

				ClientToScreen(hWnd,&poss);

				SetForegroundWindow(hWnds);
				// ポップアップメニューを表示する
				TrackPopupMenu( dhSubMenu,TPM_LEFTALIGN,poss.x,poss.y,0,hWnd, NULL );
				PostMessage(hWnds,WM_NULL,0L,0L);

			}

		return 0;

		case WM_COMMAND:
            switch(LOWORD(wParam)) {

				//登録メニューアイコンの削除
				case ID_D:

					popFlag=FALSE;

					dpoint = cpx+(cpy*(DROPSIZEX));
					wsprintf(buff,TEXT("%s%d"),tarGetSection,dpoint);
					WritePrivateProfileString(buff,key1,TEXT("Nothing"),path);
					WritePrivateProfileString(buff,key2,TEXT("Nothing"),path);

					*(tarDP+(cpy*5)+cpx)=0;
					//構造体の更新
					(tarGetdMenu+dpoint)->dIcon = NULL;
					lstrcpy((tarGetdMenu+dpoint)->dPath,TEXT(" "));


					SelectObject(DhMemDC,back_brush);
					Rectangle(DhMemDC,drct[cpy][cpx].left,drct[cpy][cpx].top,drct[cpy][cpx].left+35,drct[cpy][cpx].top+35);

					InvalidateRect(hWnd,NULL,FALSE);

               break;

			   //タイトルの変更
				case ED_TITLE:

					CloseMessage(MhWnd,MLhWnd);

					dpoint = cpx+(cpy*(DROPSIZEX));
	
					wndpl.length = sizeof(WINDOWPLACEMENT);
					GetWindowPlacement(hWnd, &wndpl);
					rc = wndpl.rcNormalPosition;


					EdTitleFlag=TRUE;

					//タイトル文字列をセット
					SetDlgItemText(EdTitle,IDC_EDIT1,dMenuData[dpoint].dname) ;

					SendMessage(GetDlgItem(EdTitle, IDC_EDIT1),EM_SETSEL,0,-1) ;

					SetWindowPos(EdTitle,NULL,rc.left-65,rc.top,0,0,SWP_NOSIZE | SWP_SHOWWINDOW);

				break;


            }
        return 0;


		case WM_SETCURSOR:
				
		   if(dhandFlag){

                SetCursor(LoadCursor(NULL,IDC_HAND));

		   }
		
		return 0;

        case WM_CLOSE: // 突然の終了禁止
            return 0;

		case WM_DESTROY:

			free(buff);
			DeleteObject(ohbrush);
			DeleteObject(back_brush);
			DeleteObject(Dhbitmap);
			DeleteDC(DhMemDC);
			DragFinish(hDrop);

		return 0;

            
    }

    return DefWindowProc( hWnd, uMsg , wParam, lParam );
}



/*----------------------------------------------------------*/
/* ドロップアイコンレイアウト用ウィンドウプロシージャ			
/*----------------------------------------------------------*/

LRESULT CALLBACK WindowProcDropIconLy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	FILE *fp1;								 //画像用
	static BLENDFUNCTION blend;
	static int gbAlpha=0;
	
	switch(uMsg){

		case WM_CREATE:

			//PNGの読み出し
			fp1 = fopen(img_item_n1, "rb");

			//読み出しのエラーチェック
			if (fp1 == NULL ) {
                   MessageBox(hWnd, TEXT(".pngが開けません。"), TEXT("エラー"), MB_OK | MB_ICONSTOP);
                   DestroyWindow(hWnd);
                   break;
            }

		    //メモリに読み込んだpngファイルを、ビットマップハンドルにする
			hWaku = LoadPngFromFile(hWnd, fp1, uWidth, uHeight);

			//解放
			fclose(fp1);

			//メイン画像(png)表示
			CreatePngLext(hWnd,hWaku,0);

		return 0;

		case WM_GTIME:

			SetTimer(hWnd,3,5,NULL);

		return 0;

		 case WM_TIMER:

            blend.BlendOp = AC_SRC_OVER;
            blend.BlendFlags = 0;
            blend.SourceConstantAlpha = gbAlpha;
            blend.AlphaFormat = AC_SRC_ALPHA;
 
			if(gbAlpha<=255){
				SetLayeredWindowAttributes(DhWnd, DROPCOLOR,gbAlpha, ULW_ALPHA | LWA_COLORKEY);
				UpdateLayeredWindow(hWnd, NULL, NULL, NULL, NULL, NULL,
						NULL, &blend, ULW_ALPHA);
			}

			if (gbAlpha >255){
				
				gbAlpha=0;

				SetLayeredWindowAttributes(DhWnd,NULL,255, ULW_ALPHA);

                KillTimer(hWnd,3);

			}else{

				gbAlpha+=25;

			}

		return 0;

		case WM_DCTIME:

			if(!gbAlpha){
				animem=FALSE;
	
				blend.BlendOp = AC_SRC_OVER;
				blend.BlendFlags = 0;
				blend.SourceConstantAlpha = 0;
				blend.AlphaFormat = AC_SRC_ALPHA;

				SetLayeredWindowAttributes(DhWnd,DROPCOLOR,0, ULW_ALPHA | LWA_COLORKEY);
				UpdateLayeredWindow(LhWnd, NULL, NULL, NULL, NULL, NULL,NULL, &blend, ULW_ALPHA);

				//レイアウトドロップメニュのダイアログ非表示
				ShowWindow(LhWnd,SW_HIDE);
				//ドロップメニュウィンドウ非表示
				ShowWindow(DhWnd,SW_HIDE);
			}

		return 0;



	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


/*----------------------------------------------------------*/
/*	    ドロップタイトル変更用ダイアログプロシージャー
/*----------------------------------------------------------*/

BOOL CALLBACK EdTitlePro(HWND Dialog,UINT uMsg,WPARAM wParam,LPARAM lParam){

	static TCHAR *buff;
	TCHAR titleBuff[32];
	int size,dpoint;

	switch(uMsg){

		case WM_INITDIALOG:
			
			 size = lstrlen(key1)*sizeof(TCHAR);
			 buff = (TCHAR *)malloc(size+6);

			 SetFocus(GetDlgItem(Dialog, IDC_EDIT1));

		break;

		
		case WM_COMMAND:

            switch(LOWORD(wParam)) {

				case IDOK:

					GetDlgItemText(Dialog,IDC_EDIT1,titleBuff,30);

					if(lstrlen(titleBuff)){

						dpoint = cpx+(cpy*(DROPSIZEX));

						wsprintf(buff,TEXT("%s%d"),tarGetSection,dpoint);

						WritePrivateProfileString(buff,key2,titleBuff,path);

						//構造体の更新
						readyMenuFile(dMenuData,tarGetSection,key1,key2,path,DROPSIZEX*DROPSIZEY);

						SendMessage(Dialog,WM_CLOSE,0,0);

					}

				break;

			}

		return 0;

		case WM_CLOSE:

			popFlag=FALSE;
			EdTitleFlag=FALSE;
			ShowWindow(Dialog,SW_HIDE);

		return 0;

		case WM_DESTROY:
			free(buff);
		return 0;

	}

	return FALSE;

}

/*----------------------------------------------------------*/
/*        吹き出し用ウィンドウプロシージャ			
/*----------------------------------------------------------*/

LRESULT CALLBACK WindowProcMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{



	static HDC Thdc;
	static HBITMAP Thbitmap;
	static HFONT hFont;
	PAINTSTRUCT ps;
	

	switch(uMsg){

		case WM_CREATE:
			
			 MesDC = CreateCompatibleDC(GetDC(hWnd));
			 Thbitmap = CreateCompatibleBitmap(GetDC(hWnd),MessageTW,MessageTH);
			 SelectObject(MesDC,Thbitmap);

			 SelectObject(MesDC,GetStockObject(NULL_PEN));
			 Rectangle(MesDC,0,0,MessageTW+1,MessageTH+1);

			 //フォントの設定
			 hFont = CreateFont(13,0,0,0,0,FALSE,FALSE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,NULL);
			 SelectObject(MesDC,hFont);


		return 0;

		case WM_LBUTTONDOWN:

			MessageCloseFlag = FALSE;
			errorMessage=FALSE;
			CloseMessage(MhWnd,MLhWnd);

		break;
		
		case WM_PAINT:

			//クライアント領域のデバイスコンテキストの取得
			Thdc = BeginPaint(hWnd,&ps);

			//仮想からデバイスへの転送
			BitBlt(
					Thdc,0,0,MessageTW,MessageTH,MesDC,0,0,SRCCOPY
			);

			EndPaint(hWnd,&ps);

		break;

		case WM_CLOSE:
			return 0;

		case WM_DESTROY:
			DeleteObject(Thbitmap);
			DeleteObject(hFont);
			DeleteDC(MesDC);
		return 0;

	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


/*----------------------------------------------------------*/
/*        吹き出しレイアウト用ウィンドウプロシージャ			
/*----------------------------------------------------------*/

LRESULT CALLBACK WindowProcMessageLy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	FILE *fp1;								 //画像用

	switch(uMsg){

		case WM_CREATE:

			//PNGの読み出し
			fp1 = fopen(img_item_n2, "rb");

			//読み出しのエラーチェック
			if (fp1 == NULL ) {
                   MessageBox(hWnd, TEXT(".pngが開けません。"), TEXT("エラー"), MB_OK | MB_ICONSTOP);
                   DestroyWindow(hWnd);
                   break;
            }

		    //メモリに読み込んだpngファイルを、ビットマップハンドルにする
			Mes = LoadPngFromFile(hWnd, fp1, uWidth, uHeight);

			//解放
			fclose(fp1);

			//メイン画像(png)表示
			CreatePngLext(hWnd,Mes,255);

		return 0;

		case WM_LBUTTONDOWN:

			MessageCloseFlag = FALSE;
			CloseMessage(MhWnd,MLhWnd);

		return 0;

		case WM_CLOSE:
			return 0;

	}


	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


/*----------------------------------------------------------*/
/*	              設定用ダイアログプロシージャー
/*----------------------------------------------------------*/

BOOL CALLBACK SystemEdit(HWND Dialog,UINT uMsg,WPARAM wParam,LPARAM lParam){

	TCHAR *stok = TEXT("スタートアップに登録しますか？");
	TCHAR *stno = TEXT("スタートアップを解除しますか？");
	TCHAR stbuff[20];
	TCHAR sliBuff[6];
	WINDOWPLACEMENT  wndpl;
	RECT rc;
	int sli;

	switch(uMsg){

		case WM_INITDIALOG:

		return 0;

		case WM_SHOWWINDOW:

			if(wParam==1){
				GetWindowPlacement(	hWnds, &wndpl );
				rc = wndpl.rcNormalPosition;

				SetWindowPos(Dialog,NULL,rc.left-95,rc.top+70,0,0,SWP_NOSIZE);
			}

		return 0;
		
		case WM_COMMAND:

            switch(LOWORD(wParam)) {


				//設定ボタン
				case IDC_BUTTON2:

					//一時的にワード生成はしない
					CloseMessage(MhWnd,MLhWnd);
					actWord=FALSE;

					//スライダー
					sli = SendDlgItemMessage(Dialog,IDC_SLIDER1,TBM_GETPOS,0,0);
					wsprintf(sliBuff,TEXT("%d"),sli);
					WritePrivateProfileString(TEXT("word"),TEXT("speed"),sliBuff,path);

					wspeed = sli;
					wordTime=0;

					actWord=TRUE;

					if(IsDlgButtonChecked(Dialog,IDC_CHECK3)){
						WritePrivateProfileString(TEXT("word"),TEXT("time"),TEXT("1"),path);
						StTime=TRUE;
					}else{
						WritePrivateProfileString(TEXT("word"),TEXT("time"),TEXT("0"),path);
						StTime=FALSE;
					}

					if(IsDlgButtonChecked(Dialog,IDC_CHECK1)){
						WritePrivateProfileString(TEXT("word"),TEXT("act"),TEXT("1"),path);
						actWord=TRUE;
					}else{
						WritePrivateProfileString(TEXT("word"),TEXT("act"),TEXT("0"),path);
						actWord=FALSE;
						wordTime=0;
					}

					SendMessage(Dialog,WM_CLOSE,0,0);

				break;

				case IDC_VER:

					MessageBox(Dialog,VER,TEXT("バージョン情報"),MB_OK);

				break;

				case IDC_STARTUP:

					if(stFlag){
						lstrcpy(stbuff,stno);
					}else{
						lstrcpy(stbuff,stok);
					}
					
				    if(IDCANCEL!=MessageBox(Dialog,stbuff,TEXT("確認"),MB_OKCANCEL)){
						ExStartUp();
					}

				break;

			}

		return 0;

		case WM_CLOSE:

			EdTitleFlag=FALSE;
			popFlag=FALSE;

			if(IsWindowVisible(dwhWnd)){
				SendMessage(dwhWnd,WM_CLOSE,0,0);
			}

			ShowWindow(Dialog,SW_HIDE);
		return 0;

		case WM_DESTROY:
		return 0;

	}

	return FALSE;

}

/*----------------------------------------------------------*/
/*        "言葉を教える" 用ダイアログプロシージャ			
/*----------------------------------------------------------*/

BOOL CALLBACK WordProc(HWND Dialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	//文字数制限は35文字、バイトは70バイト

	TCHAR nameBuff[72];
	HWND editWord;
	WINDOWPLACEMENT  wndpl;
	RECT rc;

	switch(uMsg){

		case WM_INITDIALOG:

			 SendDlgItemMessage(Dialog,IDC_EDIT1,EM_SETLIMITTEXT,35,0);

			 SetFocus(GetDlgItem(Dialog,IDC_EDIT1));

		return 0;

		case WM_SHOWWINDOW:

			if(wParam==1){
				GetWindowPlacement(	hWnds, &wndpl );
				rc = wndpl.rcNormalPosition;

				SetWindowPos(Dialog,NULL,rc.left-75,rc.top+200,0,0,SWP_NOSIZE);
			}
		return 0;

		//キーが押されたら
		case WM_KEYUP:
		
			if(wParam==VK_RETURN){

				SendMessage(Dialog,WM_COMMAND,1,0);
			}

		return 0;

		case WM_COMMAND:

			switch(LOWORD(wParam)){

				case IDOK:

					GetDlgItemText(Dialog,IDC_EDIT1,keyBuff,72);

					if(lstrlen(keyBuff)){

						editWord = CreateDialog(
							ghInstance,
							MAKEINTRESOURCE(IDD_WORDAD) , Dialog ,(DLGPROC)EdWordProc
						 );

						//タグリストを表示させる
						createTagList(editWord);

						//言葉を設定画面に表示させる
						lstrcpy(nameBuff,keyBuff);
						lstrcat(nameBuff,TEXT(" とは？"));
						SetDlgItemText(editWord,IDC_NAME,nameBuff); 

						ShowWindow(editWord,SW_SHOW);
						ShowWindow(Dialog,SW_HIDE);

					}

				break;

			}

		return 0;

		case WM_CLOSE:
			EdTitleFlag=FALSE;
			popFlag=FALSE;
			DestroyWindow(Dialog);
		return 0;

	}


	return FALSE;
}

/*----------------------------------------------------------*/
/*        "言葉を教える" 設定用ダイアログプロシージャ			
/*----------------------------------------------------------*/

BOOL CALLBACK EdWordProc(HWND Dialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	int no=0,e=0;
	TCHAR strName[30];

	switch(uMsg){

		case WM_COMMAND:

			switch(LOWORD(wParam)){

				case ID_WOK:

					//ターゲットリストの取得
					no = SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_GETCURSEL,0L,0L);
					SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_GETTEXT,(WPARAM)no,(LPARAM)strName);

					if (no!=LB_ERR){

						//XMLへの追加
						if(addWord(keyBuff,strName,tmppath)){
							errorMessage=TRUE;
							MessageCloseFlag = TRUE;
							Message(MhWnd,MLhWnd,TEXT("エラーだよ・・・（XML系）"),MessageTW,MessageTH,0);
							e =1;
						}

					}

					SendMessage(Dialog,WM_CLOSE,0,0);

					if(e){
						popFlag = TRUE;
					}

				break;

			}

		return 0;

		case WM_CLOSE:
			DestroyWindow(Dialog);
			SendMessage(whWnd,WM_CLOSE,0,0);
		return 0;

	}

	return FALSE;

}


/*----------------------------------------------------------*/
/*        言葉を忘れる用ダイアログプロシージャー		
/*----------------------------------------------------------*/


BOOL CALLBACK DelWordProc(HWND Dialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	static int no=-1;
	WINDOWPLACEMENT  wndpl;
	RECT rc;

	switch(uMsg){

		case WM_INITDIALOG:

			//ワードリストの取得
			DelcreateWordList(Dialog);

		return 0;

		case WM_SHOWWINDOW:

			if(wParam==1){
				GetWindowPlacement(	hWnds, &wndpl );
				rc = wndpl.rcNormalPosition;

				SetWindowPos(Dialog,NULL,rc.left-75,rc.top+70,0,0,SWP_NOSIZE);
			}
		return 0;

		case WM_COMMAND:

			 switch(LOWORD(wParam))
			 {  

				 case IDC_LIST1:

					 //フォーカス対象の番号を取得
					 if(HIWORD(wParam) == LBN_SELCHANGE){
						
						 no = SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_GETCURSEL,0L,0L);
						 DelSetTagName(Dialog,no);

					 }

				 break;

				 case IDC_BUTTON1:

					if(no>=0){

						//削除処理
						if(delWord(no,tmppath)){
							errorMessage=TRUE;
							MessageCloseFlag = TRUE;
							popFlag = TRUE;
							Message(MhWnd,MLhWnd,TEXT("エラーだよ・・・（XML系）"),MessageTW,MessageTH,0);
						}

						SendMessage(sDlg,WM_CLOSE,0,0);

					}

				break;

			 }

		return 0;

		case WM_CLOSE:
			no=-1;

			EdTitleFlag=FALSE;
			popFlag=FALSE;

			DestroyWindow(Dialog);
		return 0;

	}

	return FALSE;
}

/*----------------------------------------------------------*/
/*        RSS設定用ダイアログプロシージャー		
/*----------------------------------------------------------*/


BOOL CALLBACK rssEditProc(HWND Dialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	TCHAR nameBuff[50];
	TCHAR urlBuff[100];
	TCHAR strBuff[50];
	TCHAR buffs[30];
	TCHAR nuffs[4];
	TCHAR ints[4];
	TCHAR *setlink=NULL;
	TCHAR *setname=NULL;
	TCHAR sliBuff[6];
	static TCHAR *rpath;
	WINDOWPLACEMENT  wndpl;
	RECT rc;
	int no=0,sli,i=0,tn=0;


	switch(uMsg){

		case WM_INITDIALOG:

			rpath = GetRssIniPath();

			rssSpeed = GetRssSlspeed(rpath);

					
			//スライダーの設定
			SendDlgItemMessage(Dialog,IDC_SLIDER1, TBM_SETPOS, TRUE,(rssSpeed-1)/50);
	
		return 0;

		case WM_SHOWWINDOW:

			if(wParam==1){
				GetWindowPlacement(	hWnds, &wndpl );
				rc = wndpl.rcNormalPosition;

				SetWindowPos(Dialog,NULL,rc.left-75,rc.top,0,0,SWP_NOSIZE);
			}
		return 0;

		case WM_COMMAND:

			switch(LOWORD(wParam))
			{  

				case IDC_NADD:

					GetDlgItemText(Dialog,IDC_NAME,nameBuff,50);
					GetDlgItemText(Dialog,IDC_URL,urlBuff,100);

					if(lstrlen(nameBuff) && lstrlen(urlBuff)){
						
						if(chekUrl(urlBuff)){

							//新規データを追加する
							rssDataAdd(nameBuff,urlBuff);

							//初期化ファイルに書き込み
							no = getRssIniData();
							wsprintf(ints,TEXT("%d"),no);

							//投入位置を決める
							i=0;
							while(1){

								wsprintf(buffs,TEXT("%s%d"),TEXT("Data"),i);
								GetPrivateProfileString(buffs,TEXT("name"),TEXT("error"),strBuff,50,rpath);

								if(lstrcmp(strBuff,TEXT("Nothing"))==0){
									no = i;
									break;
								}else if(lstrcmp(strBuff,TEXT("error"))==0){
									
									break;
								}

								i++;
							}
							i=0;

							wsprintf(buffs,TEXT("%s%d"),TEXT("Data"),no);
						
							WritePrivateProfileString(buffs,TEXT("name"),nameBuff,rpath);
							WritePrivateProfileString(buffs,TEXT("url"),urlBuff,rpath);
							WritePrivateProfileString(buffs,TEXT("no"),ints,rpath);

							SetListRssDataName(nameBuff);

							//エディット内をクリアする
							SetWindowText(GetDlgItem(Dialog,IDC_NAME),TEXT(""));
							SetWindowText(GetDlgItem(Dialog,IDC_URL),TEXT(""));

							updataRss();

							if(mRss){
								KillTimer(mRss,6);
								ShowWindow(mRss,SW_HIDE);
								rssFlag=FALSE;
							}

						}

					}

				break;

				case IDC_BUTTON6:

					//スライダー
					sli = SendDlgItemMessage(Dialog,IDC_SLIDER1,TBM_GETPOS,0,0);
					wsprintf(sliBuff,TEXT("%d"),sli);
					WritePrivateProfileString(TEXT("rss"),TEXT("speed"),sliBuff,rpath);
					sli++;
					sli=sli*50;

					rssSpeed=sli;

					ShowWindow(Dialog,SW_HIDE);

				break;

				case IDC_LIST1:

					 //フォーカス対象の番号を取得
					 if(HIWORD(wParam) == LBN_SELCHANGE){
						
						no = SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_GETCURSEL,0L,0L);
						RssSetName(&setname,no);
						RssSetLink(&setlink,no);

						SetWindowText(GetDlgItem(Dialog,IDC_NAME),setname);
						SetWindowText(GetDlgItem(Dialog,IDC_URL),setlink);

					 }

				break;

				//編集
				case IDC_BUTTON5:

					no = SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_GETCURSEL,0L,0L);

					if(no>=0){
						wsprintf(buffs,TEXT("%s%d"),TEXT("Data"),no);

						GetDlgItemText(Dialog,IDC_NAME,nameBuff,50);
						GetDlgItemText(Dialog,IDC_URL,urlBuff,100);

						WritePrivateProfileString(buffs,TEXT("name"),nameBuff,rpath);
						WritePrivateProfileString(buffs,TEXT("url"),urlBuff,rpath);

						UpdataRssDialogData(nameBuff,urlBuff,no);

						SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_DELETESTRING,(WPARAM)no,0L);
						SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_INSERTSTRING,(WPARAM)no,(LPARAM)nameBuff);

					}

				break;

				//削除
				case IDC_BUTTON4:


					no = SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_GETCURSEL,0L,0L);

					if(no>=0){


						GetDlgItemText(Dialog,IDC_NAME,nameBuff,50);
						GetDlgItemText(Dialog,IDC_URL,urlBuff,100);

						while(1){
						
							wsprintf(buffs,TEXT("%s%d"),TEXT("Data"),i);

							tn = GetPrivateProfileInt(buffs,TEXT("no"),-2,rpath);

							if(no==tn){

								WritePrivateProfileString(buffs,TEXT("name"),TEXT("Nothing"),rpath);
								WritePrivateProfileString(buffs,TEXT("url"),TEXT("Nothing"),rpath);
								WritePrivateProfileString(buffs,TEXT("no"),TEXT("-1"),rpath);

							}else if(no<tn){

								tn = tn-1;
								wsprintf(buffs,TEXT("%s%d"),TEXT("Data"),i);
								wsprintf(nuffs,TEXT("%d"),tn);
								WritePrivateProfileString(buffs,TEXT("no"),nuffs,rpath);
							
							}else if(tn<-1){

								break;

							}

							i++;

							
						}

				

						DelRssDialogData(no);

						SetWindowText(GetDlgItem(Dialog,IDC_NAME),TEXT(""));
						SetWindowText(GetDlgItem(Dialog,IDC_URL),TEXT(""));

						SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_DELETESTRING,(WPARAM)no,0L);

						rssStart = FALSE;

						BackRssData();

						updataRss();

						if(mRss){
							KillTimer(mRss,6);
							ShowWindow(mRss,SW_HIDE);
							rssFlag=FALSE;
						}

					}

				break;

			}
		return 0;

		case WM_CLOSE:

			SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_SETCURSEL,(WPARAM)-1,0L);
			SetWindowText(GetDlgItem(Dialog,IDC_NAME),TEXT(""));
			SetWindowText(GetDlgItem(Dialog,IDC_URL),TEXT(""));

			EdTitleFlag=FALSE;
			popFlag=FALSE;

			ShowWindow(Dialog,SW_HIDE);

		return 0;

	}

	return FALSE;
}

/*----------------------------------------------------------*/
/*                  RSSリストにセット	
/*----------------------------------------------------------*/

void SetListRssDataName(TCHAR *name){

	SendMessage(GetDlgItem(wRss,IDC_LIST1),LB_ADDSTRING,0,(LPARAM )(LPCTSTR )name);

}
/*----------------------------------------------------------*/
/*                  RSS処理スピードの取得
/*----------------------------------------------------------*/

int GetRssSlspeed(TCHAR *path){

	return (GetPrivateProfileInt(TEXT("rss"),TEXT("speed"),0,path)+1)*50;

}

/*----------------------------------------------------------*/
/*                 RSS表示用ウィンドウ		
/*----------------------------------------------------------*/

BOOL CALLBACK rssWindow(HWND Dialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	WINDOWPLACEMENT  wndpl;
	static RECT rc,rct,rctt,rbatu,rst,rstop,rclink,rcl;
	static TCHAR *title=NULL,*topTitle=NULL,*link=NULL;
	HDC hdc;
	static HDC hMemDC;
	PAINTSTRUCT ps;
	static HBITMAP hbmp;
	HFONT hFont;
	POINT mouse;
	static BOOL hand = FALSE,mouseOver=FALSE;
	static HFONT font;
	static int rss_counter=0;

	switch(uMsg){

		case WM_INITDIALOG:

			hbmp= (HBITMAP)LoadImage( NULL,RSSIMG, IMAGE_BITMAP, 0, 0,
                                  LR_LOADFROMFILE|LR_CREATEDIBSECTION );
			if( hbmp==NULL ){
				MessageBox(hWnds,TEXT("画像ファイルがありません"),NULL, MB_OK);
				break;
			}

			//バツマーク
			SetRect(&rbatu,305,15,332,40);
			SetRect(&rst,100,15,127,40);
			SetRect(&rstop,146,15,175,40);

			//ウィンドウ位置調整
			GetWindowPlacement(	hWnds, &wndpl );
			rc = wndpl.rcNormalPosition;
			SetWindowPos(Dialog,NULL,rc.left-RSSHOSEIL,rc.top+195,RSSW,RSSH,NULL);

			//メモリデバイス
			hdc = GetDC(Dialog);
			hMemDC = CreateCompatibleDC(hdc);
			SelectObject(hMemDC,hbmp);

			//フォントの設定
			hFont = CreateFont(13,0,0,0,0,FALSE,FALSE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,NULL);
		    SelectObject(hMemDC,hFont);

			//--- タイトル表示の設定
			GetClientRect(Dialog,&rct);
			GetClientRect(Dialog,&rctt);

			//サイトタイトル
			rctt.top+=50;
			rctt.left+=10;
			rctt.right-=10;
			rctt.bottom-=10;
			//タイトル
			rct.top+=70;
			rct.left+=10;
			rct.right-=10;
			rct.bottom-=10;

			SetTimer(Dialog,6,rssSpeed,NULL);

			TextOut(hMemDC,rctt.left,rctt.top,TEXT("読み込み中だよ・・・"),lstrlen(TEXT("読み込み中だよ・・・")));
			InvalidateRect(Dialog,NULL,FALSE);


		return 0;

		case WM_TIMER:

			title=NULL;
			topTitle = NULL;
			link = NULL;

			if(rssStart){
	
					GetRssTitleData(&title,&topTitle,&link);

					if(lstrlen(title)>3 && lstrlen(topTitle)>3){


							SelectObject(hMemDC,GetStockObject(WHITE_BRUSH));
							SelectObject(hMemDC,GetStockObject(NULL_PEN));
							Rectangle(hMemDC,rct.left,rct.top,rct.right,rct.bottom);
							Rectangle(hMemDC,rctt.left,rctt.top,rctt.right,rctt.bottom);
				
							SetRect(&rclink,10,70,330,95);

							SetTextColor(hMemDC,RGB(0,0,0));
							hFont = CreateFont(13,0,0,0,0,FALSE,FALSE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,NULL);
							SelectObject(hMemDC,hFont);

							if(DrawText(hMemDC,topTitle,-1,&rctt,DT_WORDBREAK)){
								DrawText(hMemDC,title,-1,&rct,DT_WORDBREAK);
							}
				
							InvalidateRect(Dialog,NULL,FALSE);
				
					}

					rss_counter++;
					if(rss_counter>30){
						rss_counter=0;
					}

			}

	

		return 0;

		case WM_MOUSEMOVE:

			GetCursorPos(&mouse);
			ScreenToClient(Dialog, &mouse);


			//マウス座標は登録したRECT内かどうか
			if(PtInRect(&rbatu,mouse)){

				hand = TRUE;

			}else if(PtInRect(&rclink,mouse)){

				 //フォントの設定
				 hFont = CreateFont(13,0,0,0,0,FALSE,TRUE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,NULL);
				 SelectObject(hMemDC,hFont);

				 SetTextColor(hMemDC,RGB(0,0,255));
				 DrawText(hMemDC,title,-1,&rct,DT_WORDBREAK);

				 InvalidateRect(Dialog,NULL,FALSE);

				 hFont = CreateFont(13,0,0,0,0,FALSE,FALSE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,NULL);
				 SelectObject(hMemDC,hFont);

				 mouseOver=TRUE;

			}else{

				if(mouseOver){
		
					 hFont = CreateFont(13,0,0,0,0,FALSE,FALSE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,NULL);
					 SelectObject(hMemDC,hFont);

					 SetTextColor(hMemDC,RGB(0,0,0));

					 DrawText(hMemDC,title,-1,&rct,DT_WORDBREAK);

					 InvalidateRect(Dialog,NULL,FALSE);

				}

			}


		return 0;

		case WM_LBUTTONDOWN:

			GetCursorPos(&mouse);
			ScreenToClient(Dialog, &mouse);

			//マウス座標は登録したRECT内かどうか
			if(PtInRect(&rbatu,mouse)){

				rss_counter=0;
				KillTimer(Dialog,6);
				ShowWindow(Dialog,SW_HIDE);
				rssFlag=FALSE;

				updataRss();


			}else if(PtInRect(&rstop,mouse)){

				KillTimer(Dialog,6);

			}else if(PtInRect(&rst,mouse)){

				SetTimer(Dialog,6,rssSpeed,NULL);

			}else if(PtInRect(&rclink,mouse)){

				ShellExecute(NULL,TEXT("open"), link, NULL, NULL, SW_SHOWNORMAL);

			}

		return 0;

		//カーソル
		case WM_SETCURSOR:


	    return 0;

		case WM_PAINT:

			hdc = BeginPaint(Dialog,&ps);
			BitBlt(hdc,0,0,RSSW,RSSH,hMemDC,0,0,SRCCOPY);
			EndPaint(Dialog,&ps);

		return 0;

		case WM_CLOSE:

			DeleteObject(hbmp);
			ReleaseDC(hWnd, hMemDC);
			DestroyWindow(Dialog);

		return 0;

	}

	return FALSE;
}

void StartTimer(){
	
	rssStart=TRUE;

}

/*----------------------------------------------------------*/
/*		            システム系の初期化		
/*----------------------------------------------------------*/

void SystemInit(){

	int st,cw,ct;
	LONG setSli;

	//初期化ファイルからの読み込み
	cw = GetPrivateProfileInt(TEXT("word"),TEXT("act"),0,path);
	st = GetPrivateProfileInt(TEXT("system"),TEXT("startup"),0,path);
	ct = GetPrivateProfileInt(TEXT("word"),TEXT("time"),0,path);
	setSli = (LONG)GetPrivateProfileInt(TEXT("word"),TEXT("speed"),50,path);

	//おしゃべり機能の有無
	if(cw){
		CheckDlgButton(sDlg,IDC_CHECK1,1);
		actWord=TRUE;
	}else{
		actWord=FALSE;
	}

	//日付ワード生成の有無
	if(ct){
		CheckDlgButton(sDlg,IDC_CHECK3,1);
		StTime=TRUE;
	}


	//スライダーの設定
	SendDlgItemMessage(sDlg,IDC_SLIDER1, TBM_SETPOS, TRUE,setSli);

	//ワードスピードの設定
	wspeed = (int)setSli;

	//スタートアップに登録されているか
	if(st){
		SetDlgItemText(sDlg,IDC_STARTUP,TEXT("スタートアップの登録解除"));
		stFlag=TRUE;
	}



}



/*----------------------------------------------------------*/
/*		        ウィンドウ類の非表示			
/*----------------------------------------------------------*/

void CloseWindows(){

		ShowWindow(LhWnd,SW_HIDE);
		ShowWindow(DhWnd,SW_HIDE);
		ShowWindow(MLhWnd,SW_HIDE);
		ShowWindow(MhWnd,SW_HIDE);
		ShowWindow(hWnd,SW_HIDE);

}


/*----------------------------------------------------------*/
/*		       スタートアップに登録&解除			
/*----------------------------------------------------------*/

void ExStartUp(){

	TCHAR SCpath[MAX_PATH];
	TCHAR paths[MAX_PATH];
	TCHAR buffLeft[10];
	TCHAR buffTop[10];
	HRESULT hr;							 //戻り値
    IShellLink *pShellLink = NULL;
    IPersistFile *pPersistFile = NULL;
	WINDOWPLACEMENT wndpl;
	RECT rc;

	//スタートアップのパスを求める
	SHGetSpecialFolderPath (NULL, paths, CSIDL_STARTUP, 0);

	//作成パス
	lstrcat(paths,TEXT("\\NAちゃん.lnk"));

	//ターゲットパス
	GetModuleFileName(NULL,SCpath,MAX_PATH);

	if(stFlag){

		//ショートカットパスをスタートアップフォルダから削除
		if(DeleteFile(paths)==TRUE) {

			WritePrivateProfileString(TEXT("window"),TEXT("left"),TEXT("-1000"),path);
			WritePrivateProfileString(TEXT("window"),TEXT("top"),TEXT("-1000"),path);
			WritePrivateProfileString(TEXT("system"),TEXT("startup"),TEXT("0"),path);

			SetDlgItemText(sDlg,IDC_STARTUP,TEXT("スタートアップに登録"));
			
			stFlag=FALSE;

		}


	}else{

		try{

			//OLE初期化
			CoInitialize(NULL);

			//IShellLinkインターフェイスの作成
			hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
					IID_IShellLink, (void**)&pShellLink);
			if (hr != S_OK)
				throw(FALSE);

			//リンク先パスの設定
			pShellLink->SetPath(SCpath);

			//IPersistFileインターフェイスの取得
			if (pShellLink->QueryInterface(IID_IPersistFile, 
						(void**)&pPersistFile) != S_OK)
				throw(FALSE);

			//ファイルに保存
			if (pPersistFile->Save(paths, true) != S_OK)
				throw(FALSE);
			throw(TRUE);

		}
		catch(BOOL IsOK)
		{
			//ブジェクト解放
			if (pPersistFile != NULL)
				pPersistFile->Release();
			if (pShellLink != NULL)
				pShellLink->Release();

			// OLE終了処理
			CoUninitialize();
			if (!IsOK){
				MessageBox(NULL, TEXT("ショートカット作成失敗"), NULL, MB_OK);
			}else{

				//ウィンドウの位置
				wndpl.length = sizeof(WINDOWPLACEMENT);
				GetWindowPlacement(hWnds, &wndpl);
				rc = wndpl.rcNormalPosition;

				wsprintf(buffLeft,TEXT("%d"),rc.left);
				wsprintf(buffTop,TEXT("%d"),rc.top);

				WritePrivateProfileString(TEXT("system"),TEXT("startup"),TEXT("1"),path);
				WritePrivateProfileString(TEXT("window"),TEXT("left"),buffLeft,path);
				WritePrivateProfileString(TEXT("window"),TEXT("top"),buffTop,path);
				SetDlgItemText(sDlg,IDC_STARTUP,TEXT("スタートアップの登録解除"));
				stFlag=TRUE;
			}

		}
	}

}


/*----------------------------------------------------------*/
/*		        PNG表示とレイヤー設定			
/*----------------------------------------------------------*/

void CreatePngLext(HWND hWnd,HBITMAP hBitmap,int alpha){

			HGDIOBJ hOldObj;

			//ウィンドウの形状をビットマップに合わせて変更
            SetWindowPos(hWnd, 0, 0, 0, uWidth, uHeight, SWP_NOMOVE | SWP_NOZORDER);

            // 各種デバイスコンテキストの取得
            HDC hmemdc, hdc, hsdc;
            hsdc   = GetDC(0);                      // デスクトップのデバイスコンテキスト（色情報指定用）
            hdc    = GetDC(hWnd);                   // このウィンドウのデバイスコンテキスト
            hmemdc = CreateCompatibleDC(hdc);       // hdcの互換デバイスコンテキスト

            POINT wndPos;
            SIZE  wndSize;
            RECT  rc;

            // レイヤードウィンドウの画面位置とサイズ
            GetWindowRect(hWnd, &rc);
            wndPos.x = rc.left;
            wndPos.y = rc.top;
            wndSize.cx = uWidth;
            wndSize.cy = uHeight;

            // デバイスコンテキストにおけるレイヤの位置
            POINT po;
            po.x = po.y = 0;

            // レイヤードウィンドウの指定
            BLENDFUNCTION blend;
            blend.BlendOp = AC_SRC_OVER;
            blend.BlendFlags = 0;
            blend.SourceConstantAlpha = alpha; // 不透明度（レイヤードウィンドウ全体のアルファ値）
            blend.AlphaFormat = AC_SRC_ALPHA;

            // 画像を描画をする
            hOldObj = SelectObject(hmemdc, hBitmap);
            BitBlt(hdc, 0, 0, uWidth, uHeight, hmemdc, 0, 0, SRCCOPY|CAPTUREBLT); // レイヤードウィンドウではCAPTUREBLTが必要

            // レイヤードウィンドウの位置、サイズ、形、内容、透明度を更新
            if (0 == UpdateLayeredWindow(hWnd, hsdc, &wndPos, &wndSize, hmemdc, &po, 0, &blend, ULW_ALPHA)) {
                  TCHAR strErrMes[80];
                  DWORD err = GetLastError();

                  wsprintf(strErrMes, TEXT("UpdateLayeredWindow失敗：エラーコード＝%d"), err);
                  MessageBox(hWnd, strErrMes, TEXT("エラー"), MB_OK | MB_ICONSTOP);
                  DestroyWindow(hWnd);
              }

             SelectObject(hmemdc, hOldObj);
             DeleteDC(hmemdc);
             ReleaseDC(hWnd, hdc);
             ReleaseDC(0, hsdc);

}

/*----------------------------------------------------------*/
/*		                日付を取得		
/*----------------------------------------------------------*/

void timeData(TCHAR *t){

	struct tm *date;
	time_t now;
	int year, month,day,w;
	TCHAR wD[] = TEXT("日月火水木金土");

	/* 現在の日時を取得 */
	time(&now);
	date = localtime(&now);

	year = date->tm_year + 1900;
	month = date->tm_mon + 1;
	day = date->tm_mday;
	w = date->tm_wday;

	wsprintf(t,TEXT("%d年%d月%d日\n\nうんと・・・ぁ %c曜日だよ♪"),year,month,day,wD[w]);

}

/*----------------------------------------------------------*/
/*		              URLかどうか
/*----------------------------------------------------------*/

int chekUrl(TCHAR *url){

	if(url[0]=='h' && url[1]=='t' && url[2]=='t' && url[3]=='p'){
		return 1;
	}

	return 1;

}


/*----------------------------------------------------------*/
/*		         	ウィンドウの登録			
/*----------------------------------------------------------*/

ATOM InitAppWindow(HINSTANCE hInstance){

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);

    wc.style          = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc    = WindowProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 0;
    wc.hInstance      = hInstance;
    wc.hIcon          = NULL;
    wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName   = NULL;
    wc.lpszClassName  = APP_NAME;
    wc.hIconSm        = NULL;


	ATOM atom = RegisterClassEx(&wc);


	//ドロップアイコン描画用ウィンドウ
    wc.lpfnWndProc    = WindowProcDropIcon;
    wc.lpszClassName  = APP_NAME2;
	RegisterClassEx(&wc);

	//ドロップアイコンレイアウト用ウィンドウ
    wc.lpfnWndProc    = WindowProcDropIconLy;
    wc.lpszClassName  = APP_NAME3;

    RegisterClassEx(&wc);

	//ドロップアイコンレイアウト用ウィンドウ
    wc.lpfnWndProc    = WindowProcMessageLy;
    wc.lpszClassName  = APP_NAME4;

	RegisterClassEx(&wc);

	
	//ドロップアイコン用ウィンドウ
    wc.lpfnWndProc    = WindowProcMessage;
    wc.lpszClassName  = APP_NAME5;

	RegisterClassEx(&wc);

	return atom;
}


/*----------------------------------------------------------*/
/*		        	ウィンドウの生成			
/*----------------------------------------------------------*/

BOOL CreateAppWindow(HINSTANCE hInstance){

	hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,APP_NAME,TEXT("NA"),WS_POPUP,
		X,Y,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL);



    //ドロップアイコンレイアウト用ウィンドウ
	LhWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        APP_NAME3,TEXT("NA -dropwindowLy-"),WS_POPUP,
        X-DROPWPOT,Y+MH-DROPHPOT,DROPWIDTH,DROPHEIGHT,hWnd, NULL, hInstance, NULL);


	//ドロップアイコン用ウィンドウ
	DhWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        APP_NAME2,TEXT("NA -dropwindow-"),WS_POPUP,
        X-DROPWPOT+DWOSEI,Y+MH-DROPHPOT+DHOSEI,DROPWIDTH-SHOSEI,DROPHEIGHT-SHOSEI,LhWnd, NULL, hInstance, NULL);


	//吹き出し用レイアウトウィンドウ
	MLhWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        APP_NAME4,TEXT("NA -messageLy-"),WS_POPUP,
        X-MTX-TXHOSEI,Y+MTY-TYHOSEI,MessageW,MessageH,hWnd, NULL, hInstance, NULL);

	
	
	//吹き出し用ウィンドウ
	MhWnd = CreateWindowEx(WS_EX_TOOLWINDOW,
        APP_NAME5,TEXT("NA -message-"),WS_POPUP,
        X-MTX,Y+MTY,MessageTW,MessageTH,MLhWnd, NULL, hInstance, NULL);




	if(!hWnd || !DhWnd || !LhWnd || !MLhWnd || !MhWnd) {
        return FALSE;
    }

	
	//インスタンスのコピー
	ghInstance = hInstance;

	hWnds = hWnd;

	//システム系の初期化
	SystemInit();

	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);


	//ワードの初期化
	if(!createOTMessage(tmppath,2)){

		MessageCloseFlag = TRUE;
		errorMessage=TRUE;

		if(StTime){
			timeData(words);
		}else{
			//起動時の吹き出しを表示
			createOTMessage(tmppath,1);
		}

		if(lstrlen(words)){
			Message(MhWnd,MLhWnd,words,MessageTW,MessageTH,0);
		}

	}else{
		Message(MhWnd,MLhWnd,TEXT("XML読み込みエラーだよ"),MessageTW,MessageTH,0);
	}

	return TRUE;
}
