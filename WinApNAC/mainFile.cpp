/*********************************************************************************/
/*                    [  �ł����Ƃ��Ղ̂��ʂ��[����� 	]	�@�@�@		       	 */
/*															   NA����� ver 0.85 */
/*																				 */
/*																				 */
/* �X�V���F			    								                         */
/*********************************************************************************/

//�x���̗}��
#define _CRT_SECURE_NO_DEPRECATE

/*----------------------------------------------------------*/
/*		      �C���N���[�h�t�@�C�������C�u����		
/*----------------------------------------------------------*/

#include <windows.h>
//�W�����o��
#include <stdio.h>
#include <time.h>
//�v���Z�X�n
#include <pdh.h>
//�h���b�v���j���[
#include "menuDrop.h"
//�O���[�o��
#include "Global.h"
//���\�[�X
#include "resource.h"
//RSS
#include "rss.h"
//png�t�@�C���ǂݍ��ݗp
#include "png.h"
#include "zlib.h"
#pragma comment(lib, "libpng.lib")
#pragma comment(lib, "zlib.lib")
//�V�F���n
#include <Shlobj.h>
//MSI�V���[�g�J�b�g
#include <Msi.h>

/*----------------------------------------------------------*/
/*		                    ��`			
/*----------------------------------------------------------*/

#define ID_TASK 200                                                   //�^�X�N
#define MYWM_NOTIFYICON 100                                           //�^�X�N
#define VER TEXT("Ver 1.00")                                       //�o�[�W����
#define APP_NAME TEXT("�f�B�X�N�g�b�v�A�v��")                         //�E�B���h�E��                    
#define APP_NAME2 TEXT("�h���b�v�A�C�R���p�E�B���h�E")                //�E�B���h�E��
#define APP_NAME3 TEXT("�h���b�v�A�C�R�����C�A�E�g�p�E�B���h�E")      //�h�E�B���h�E��
#define APP_NAME4 TEXT("���b�Z�[�W���C�A�E�g�p�E�B���h�E")            //�E�B���h�E��
#define APP_NAME5 TEXT("���b�Z�[�W�p�E�B���h�E")                      //�E�B���h�E��
#define IMG_NA1 TEXT("img/na-1.png")								  //�\��u�ʏ�v
#define IMG_NA2 TEXT("img/na-2.png")								  //�\��u�ނ��v
#define IMG_NA3 TEXT("img/na-3.png")								  //�\��u�{��v
#define IMG_NA4 TEXT("img/na-4.png")								  //�\��u�����v
#define IMG_NA5 TEXT("img/na-5.png")								  //�\��u�ۂ��[��v
#define IMG_NA6 TEXT("img/na-6.png")								  //�\��u���ꂵ���v
#define ITEM1 TEXT("img/backDropImg.png")							  //�h���b�v�g png
#define ITEM2 TEXT("img/hukidasi.png")						          //�h���b�v�g png
#define FACETIME 300												  //�\��ω��̎���
#define MW 201                                                        //�L��������̑��Δz�u
#define MH 428                                                        //�L��������̑��Δz�u
#define DROPWIDTH 202												  //�h���b�v�E�B���h�E�̉���
#define DROPHEIGHT 87												  //�h���b�v�E�B���h�E�̏c��
#define MessageW 200												  //�����o���̉���
#define MessageH 85													  //�����o���̏c��
#define MessageTW 155										          //�����o���i�e�L�X�g�j�̉���
#define MessageTH 65										          //�����o���́i�e�L�X�g�j�c��	
#define DROPWPOT 3											          //�␳�T�C�Y
#define DROPHPOT 210										          //�␳�T�C�Y
#define DHOSEI 6											          //�␳�T�C�Y
#define DWOSEI 5											          //�␳�T�C�Y
#define SHOSEI 11											          //�␳�T�C�Y
#define MTX 130												          //�␳�T�C�Y(�e�L�X�g)
#define MTY 110												          //�␳�T�C�Y(�e�L�X�g)
#define TXHOSEI 9											          //�␳�T�C�Y(�e�L�X�g)
#define TYHOSEI 10											          //�␳�T�C�Y(�e�L�X�g�j
#define WM_DMESS WM_USER											  //����\��ω�
#define WM_GTIME WM_USER+1											  //�h���b�v�A�j�C�����[�V����
#define WM_DCTIME WM_USER+2                                           //�h���b�v�A�E�g�A�j���[�V����
#define WM_CHDROPM WM_USER+3                                          //�h���b�v���j���[�̗̈�؂�ւ�
#define DROPCOLOR RGB(240,248,255)                                    //�h���b�v�̈�̔w�i�F
#define RSSIMG TEXT("img/rss.bmp")
#define RSSW 343
#define RSSH 108
#define RSSHOSEIL 350

/*----------------------------------------------------------*/
/*			          �v���g�^�C�v�錾			
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
/*			           �O���[�o���ϐ�			
/*----------------------------------------------------------*/

HWND hWnds,hWnd,DhWnd,LhWnd,MLhWnd,MhWnd,sDlg,dwhWnd;            //�e�E�B���h�E�n���h��
HWND whWnd,wRss,mRss;                                            //�E�B���h�E�n���h��
HINSTANCE ghInstance;                                            //�C���X�^���X�ϐ�
HBITMAP hBitmap1,hBitmap2,hBitmap3,hBitmap4,hBitmap5;            //�r�b�g�}�b�v�p
HBITMAP hBitmap6,hWaku,Mes;                                      //�r�b�g�}�b�v�p
unsigned uWidth = 0, uHeight = 0;                                //png�T�C�Y
unsigned dWidth = 0, dHeight = 0;                                //png�T�C�Y�ۑ�
int ImgPoint=0;                                                  //�摜���ڕۑ��l
int wspeed=0;                                                    //���[�h�\���̃J�E���g
int cpx=0,cpy=0;                                                 //�h���b�v�̈�ۑ��l
int X = ((GetSystemMetrics( SM_CXSCREEN ) - 300) / 2) ;          //�N���z�uX
int Y = ((GetSystemMetrics( SM_CYSCREEN ) - 400 ) / 2);          //�N���z�uY
int wordTime=0;                                                  //���[�h�����J�E���g
double cpum=0.0;												//CPU��
int rssSpeed=0;

/* �t���O�̏ڍׂ͕ʓr�u�t���O�d�l�v�ɋL�ڂ��Ă��� */

BOOL handFlag=FALSE;                                             //�J�[�\���̎��
BOOL pFlag=FALSE;                                                //�Œ�̗L��                                          
BOOL MessageCloseFlag=FALSE;                                     //���b�Z�[�W�N���[�Y�t���O
BOOL errorMessage=FALSE;                                         //�G���[���b�Z�[�W�t���O
BOOL popFlag=FALSE;                                              //�h���b�v�A�C�R���̃^�C�g���ύX�t���O
BOOL EdTitleFlag=FALSE;                                          //�^�C�g���t���O
BOOL stFlag=FALSE;                                               //�X�^�[�g�A�b�v
BOOL taskFlag=FALSE;                                             //�^�X�N�\��
BOOL animem=FALSE;                                               //�h���b�v�A�j���[�V�����̗L��
BOOL ClickLeft=FALSE;                                            //���N���b�N�̗L��
BOOL outWord=FALSE;                                              //���[�h�����̐���
BOOL actWord=FALSE;                                              //���[�h�����̃`�F�b�N
BOOL popDMenu=FALSE;                                             //�h���b�v��ł̐���
BOOL StTime=FALSE;                                               //���t���[�h
BOOL rssFlag=FALSE;
BOOL rssSetFlag=FALSE;
BOOL rssStart = FALSE;
TCHAR *section =TEXT("menu");								     //�Z�N�V�������i�������t�@�C���j
TCHAR *section2 =TEXT("menu2");								     //�Z�N�V�������i�������t�@�C���j
TCHAR *tarGetSection = section;                                  //�^�[�Q�b�g�Z�N�V����
TCHAR *key1 =TEXT("path");									     //�L�[���i�������t�@�C���j
TCHAR *key2 =TEXT("name");									     //�L�[���i�������t�@�C���j
TCHAR path[MAX_PATH];										     //�������t�@�C���̃p�X
TCHAR tmppath[MAX_PATH];										 //�������t�@�C���̕ۑ��p�X
TCHAR keyBuff[72];                                               //���̓��[�h
dMenu dMenuData[DROPSIZEX*DROPSIZEY]={NULL,TEXT(""),TEXT("")};   //�h���b�v���j���[�p�\���̔z��
dMenu dMenuDataN[DROPSIZEX*DROPSIZEY]={NULL,TEXT(""),TEXT("")};  //�h���b�v���j���[�p�\���̔z��
dMenu *tarGetdMenu = dMenuData;	                                 //�h���b�v���j���[�^�[�Q�b�g�\����		



/*----------------------------------------------------------*/
/*			            �n���h���擾	
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
/*			        �G���g���[�|�C���g		
/*----------------------------------------------------------*/

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{

	MSG msg;
	int buffLeft;
	int buffTop;

	//���d�N���̃`�F�b�N
	if(FindWindow(APP_NAME,NULL)!=NULL){
			
		EnumWindows( EnumWndProc,0);

		return 0;
	}

	//�L���t�@�C���̃t���p�X���擾
    GetModuleFileName(NULL,path,MAX_PATH);
	filePath(path);
	lstrcat(path,TEXT("nasystem.ini"));
	lstrcat(path,TEXT("\0"));
	
	buffLeft = GetPrivateProfileInt(TEXT("window"),TEXT("left"),-1000,path);
	buffTop = GetPrivateProfileInt(TEXT("window"),TEXT("top"),-1000,path);


	//�����N���ʒu�̐ݒ�
	if(buffLeft!=-1000 && buffTop!=-1000){

		X = buffLeft;
		Y = buffTop;

	}


	if(!InitAppWindow(hInstance)){
		MessageBox(NULL,TEXT("�E�B���h�E�N���X�̍쐬�Ɏ��s���܂���"),NULL,MB_OK);
		return 0;
	}else{
		
		if(!CreateAppWindow(hInstance)){
			MessageBox(NULL,TEXT("�E�B���h�E�N���X�̐����Ɏ��s���܂���"),NULL,MB_OK);
			return 0;
		}
	}
	
	//���b�Z�[�W���[�v
	while(GetMessage(&msg,NULL,0,0) > 0){

		if(!whWnd || !IsDialogMessage(whWnd,&msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}


	return msg.wParam;
}

/*----------------------------------------------------------*/
/*		�@�@	  �E�B���h�E�v���V�[�W���[			
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

		    //�C���[�W�p�X�̎擾
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


			//�����R�[�h�̕ϊ�
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path1,lstrlen(img_path1),img_path_n1,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path2,lstrlen(img_path2),img_path_n2,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path3,lstrlen(img_path3),img_path_n3,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path4,lstrlen(img_path4),img_path_n4,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path5,lstrlen(img_path5),img_path_n5,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_path6,lstrlen(img_path6),img_path_n6,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_item1,lstrlen(img_item1),img_item_n1,MAX_PATH ,NULL,NULL);
			WideCharToMultiByte(CP_THREAD_ACP,0,img_item2,lstrlen(img_item2),img_item_n2,MAX_PATH ,NULL,NULL);


			//PNG�̓ǂݏo��
			fp1 = fopen(img_path_n1,"rb");
			fp2 = fopen(img_path_n2, "rb");
			fp3 = fopen(img_path_n3, "rb");
			fp4 = fopen(img_path_n4, "rb");
			fp5 = fopen(img_path_n5, "rb");
			fp6 = fopen(img_path_n6, "rb");

			if (fp1 == NULL) {
                   MessageBox(hWnd, TEXT(".png���J���܂���B"), TEXT("�G���["), MB_OK | MB_ICONSTOP);
                   DestroyWindow(hWnd);
                   break;
            }

		    //�������ɓǂݍ���png�t�@�C�����A�r�b�g�}�b�v�n���h���ɂ���
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

			//���C���摜(png)�\��
			CreatePngLext(hWnd,hBitmap1,255);

			//�T�C�Y���R�s�[
			dWidth = uWidth;
			dHeight = uHeight;

			//CPU�g�p���̏���
			//�p�t�H�[�}���X�E�f�[�^�̃n���h�����쐬
			if ( PdhOpenQuery(NULL,0,&hQuery) == ERROR_SUCCESS ){
				 //�n���h�������̃J�E���^�p�X��ǉ�
				 PdhAddCounter( hQuery, TEXT("\\Processor(_Total)\\% Processor Time"), 0, &hCounter );
				 //�n���h�������̃J�E���^�l���擾
				 PdhCollectQueryData( hQuery );
				 //�^�C�}�[�̊J�n
				 SetTimer(hWnd,1,FACETIME,NULL);
				 //�����\��
				 IMGTIME=1;
			}

			//RECT�\���̂ɃZ�b�g(NA�}�[�N�̂Ƃ�)
			SetRect(&nrct,10,10,70,70);
			//(���{���̂Ƃ�)
			SetRect(&brct,85,170,105,190);
			//(A�̂Ƃ�)
			SetRect(&arct,140,10,170,90);

			//�ݒ�_�C�A���O�̐���
			sDlg = CreateDialog(
				ghInstance,
				MAKEINTRESOURCE(IDD_DIALOG2) , NULL ,(DLGPROC)SystemEdit
			);

			//RSS�p�������t�@�C���̃p�X�ݒ�
			initRss(tmppath);

			wRss = CreateDialog(
				ghInstance,
				MAKEINTRESOURCE(IDD_DIALOG4) ,NULL,(DLGPROC)rssEditProc
			);

			//RSS����
			readRssData();

			//���j���[�̃n���h��
			hMenu = LoadMenu((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_MENU1));


		return 0;

		//�^�C�}�[�C�x���g
		case WM_TIMER:

			//�n���h�������̃J�E���^�l���擾
			PdhCollectQueryData( hQuery );
			// double�^�̕\��
			PdhGetFormattedCounterValue( hCounter, PDH_FMT_DOUBLE, NULL, &fntValue );
			cpum = fntValue.doubleValue;

			//�T�C�Y���R�s�[
			uWidth = dWidth;
			uHeight = dHeight;

			//CPU�̎g�p���ɂ���ĕ\��f�[�^���������f�o�C�X��
			if(!EdTitleFlag){

				//����\���
				if(IMGTIME){

					switch(IMGTIME){

						case 1:
							CreatePngLext(hWnd,hBitmap1,imgal);
						break;
						case 4:
							CreatePngLext(hWnd,hBitmap4,imgal);
						break;

					}

					//����\��̏I������
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
						//���[�h�̐���
						if(wordTime>wspeed){

								//���[�h�̐���
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

								//���[�h�̔j��
								if(wordTime>wspeed+25 && wp){
									CloseMessage(MhWnd,MLhWnd);
									MessageCloseFlag = FALSE;
									wordTime=0;
									wp=0;
								}

						}

						 wordTime++;
				//���C�x���g���Ȃ烏�[�h�������Ȃ�	
				}else{
					wordTime=0;
				}
			}


		return 0;

		//�}�E�X����������
		case WM_NCMOUSEMOVE:

			//�}�E�X���W�̎擾
			mouse.x = LOWORD(lParam);
			mouse.y = HIWORD(lParam);

			//���W�ϊ�
			ScreenToClient(hWnd,&mouse);

			//�}�E�X���W�͓o�^����RECT�����ǂ���
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


		//�E�B���h�E����������
		case WM_MOVING:

			//�E�B���h�E�̈ʒu���Œ肷��
			wndpl.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wndpl);
			rc = wndpl.rcNormalPosition;


			if(pFlag){

				//�ʒu���Œ肷��
				((RECT*)lParam)->left = rc.left;
				((RECT*)lParam)->top = rc.top;
				((RECT*)lParam)->right = rc.right;
				((RECT*)lParam)->bottom = rc.bottom;
			}

		break;

		//�E�B���h�E�̈ړ�����
		case WM_MOVE:

			//�E�B���h�E�̈ʒu���Œ肷��
			wndpl.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wndpl);
			rc = wndpl.rcNormalPosition;

			//�E�B���h�E�̈ړ�
			MoveWindow(MLhWnd,rc.left-MTX-TYHOSEI,rc.top+MTY-TYHOSEI,MessageW,MessageH,NULL);
			MoveWindow(LhWnd,rc.left-DROPWPOT,rc.top+MH-DROPHPOT,DROPWIDTH,DROPHEIGHT,NULL);
			SetWindowPos(MhWnd,NULL,rc.left-MTX,rc.top+MTY,0,0,SWP_NOSIZE);
			SetWindowPos(DhWnd,NULL,rc.left-DROPWPOT+DWOSEI,rc.top+MH-DROPHPOT+DHOSEI,0,0,SWP_NOSIZE);
			SetWindowPos(mRss,NULL,rc.left-RSSHOSEIL,rc.top+195,RSSW,RSSH,NULL);

			//�N���b�N���̃A���t�@��
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

			//�A���t�@�l�����ɖ߂�
			imgal=255;
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			blend.SourceConstantAlpha = imgal;
			blend.AlphaFormat = AC_SRC_ALPHA;
			UpdateLayeredWindow(hWnd, NULL, NULL, NULL, NULL, NULL,NULL, &blend, ULW_ALPHA);

		break;

		//���N���b�N
		case WM_NCLBUTTONDOWN:

			//�}�E�X���W�̎擾
			mouse.x = LOWORD(lParam);
			mouse.y = HIWORD(lParam);

			//���W�ϊ�
			ScreenToClient(hWnd,&mouse);

			//�h���b�v���j���[�̃|�b�v�A�b�v���\������Ă�����
			if(popDMenu){
				popFlag=FALSE;
				popDMenu=FALSE;
			}

			if(handFlag && bC){

				SendMessage(DhWnd,WM_CHDROPM,0,0);

			}else if(handFlag && !animem && PtInRect(&nrct,mouse)){

				animem=TRUE;
				popFlag=FALSE;

				//�h���b�v���j���[�̕\��
				ShowWindow(LhWnd,SW_SHOW);
				ShowWindow(DhWnd,SW_SHOW);
				UpdateWindow(DhWnd);

				//�\���A�j���[�V�����̃��b�Z�[�W���M
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


		//�E�N���b�N
		case WM_NCRBUTTONDOWN:

			// �T�u���j���[�̃n���h�����擾����
			hSubMenu = GetSubMenu( hMenu, 0 );

			//�}�E�X�J�[�\���̍��W�擾
			pos.x = LOWORD(lParam);
			pos.y = HIWORD(lParam);
		
			// �|�b�v�A�b�v���j���[��\������
			TrackPopupMenu( hSubMenu,TPM_LEFTALIGN,pos.x,pos.y,0,hWnd, NULL );
		
		return 0;

		//���_�u���N���b�N
		case WM_NCLBUTTONDBLCLK:

			SendMessage(LhWnd,WM_DCTIME,0,0);
				
		return 0;

		//���j���[����
		case WM_COMMAND:

			switch(LOWORD(wParam))
			{
				//�I��
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

				//�^�X�N�Ɋi�[����
				case ID_TK:

					taskFlag=TRUE;

					nid.cbSize = sizeof(NOTIFYICONDATA);
					nid.hWnd=hWnd;
					nid.uID=ID_TASK; //ID_TASK�͂��̃A�C�R���Ɋ��蓖�Ă�ID(�����Œ�`����)
					nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //���b�Z�[�W�ƃc�[���`�b�v�ƃA�C�R����L���ɂ���
					nid.uCallbackMessage = MYWM_NOTIFYICON; //�E�B���h�E�ɑ����郁�b�Z�[�W��ID(�����Œ�`����)
					nid.hIcon= LoadIcon((HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDI_ICON1));

					lstrcpy(nid.szTip,TEXT("NA�����"));
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

		//�^�X�N�o�[��
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

		//�L�[�������ꂽ��
		case WM_KEYUP:
		
			if(wParam==VK_ESCAPE){
				SendMessage(hWnd,WM_CLOSE,0,0);
			}

		return 0;
		
		//ALT+F4�΍�
		case WM_SYSCOMMAND:

			if(wParam==SC_CLOSE){
				
				return 0;

			}

		break;

		//�J�[�\��
		case WM_SETCURSOR:

			//�t���O������΃J�[�\���̕ύX
			if(handFlag){

				SetCursor(LoadCursor(NULL,IDC_HAND));

			 }else{

				SetCursor(LoadCursor(NULL,IDC_ARROW));

			 }

	    return 0;

		//�I���W�i�����b�Z�[�W�i����\��̕ω��j
		case WM_DMESS:
			if(!taskFlag){
				IMGTIME = 4;
				MessageCloseFlag = TRUE;
				errorMessage=TRUE;
				CreatePngLext(hWnd,hBitmap4,255);
				Message(MhWnd,MLhWnd,TEXT("���C�����Ⴞ�߁[�I"),MessageTW,MessageTH,0);
			}
		return 0;

		//�q�b�g�e�X�g
		case WM_NCHITTEST:
			return HTCAPTION;
	
		//�j��
		case WM_DESTROY:

			PdhCloseQuery(hQuery);
			//�r�b�g�}�b�v�̔j��
			DeleteObject(hBitmap1);
			DeleteObject(hBitmap2);
			DeleteObject(hBitmap3);
			DeleteObject(hBitmap4);
			DeleteObject(hBitmap5);
			DeleteObject(hBitmap6);

			//�^�X�N�g���C����A�C�R�����폜
			Shell_NotifyIcon(NIM_DELETE,&nid);
		
			createOTMessage(NULL,3);

			rssPointDataFree();

			PostQuitMessage(0);

		return 0;
			
	}
	
	return DefWindowProc(hWnd,uMsg,wParam,lParam);

}



/*----------------------------------------------------------*/
/*		  �h���b�v�A�C�R���p�E�B���h�E�v���V�[�W��			
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
	IShellLink   *pShellLink;     // IShellLink�ւ̃|�C���^
	IPersistFile *pPersistFile;   // IPersistFile�ւ̃|�C���^

	WINDOWPLACEMENT wndpl;
	RECT rc;
	HIMAGELIST list;

	TCHAR szProductCode[39];
    TCHAR szFeatureId[MAX_FEATURE_CHARS + 1];
    TCHAR szComponentCode[39];
	DWORD dwBuffSize = MAX_PATH;


    switch (uMsg) {

        case WM_CREATE:

			 //�����h���b�v�Ώ�
			 tarDP = (int *)DP;

			 size = lstrlen(key1)*sizeof(TCHAR);
			 buff = (TCHAR *)malloc(size+6);

			 //�h���b�v�̋���
			 DragAcceptFiles(hWnd,TRUE);

			 DhMemDC = CreateCompatibleDC(GetDC(DhWnd));
			 Dhbitmap = CreateCompatibleBitmap(GetDC(DhWnd),DROPWIDTH,DROPHEIGHT);

			 SelectObject(DhMemDC,Dhbitmap);
			
			 //�w�i���w��F
			 SelectObject(DhMemDC,GetStockObject(NULL_PEN));
			 SelectObject(DhMemDC,back_brush);
			 Rectangle(DhMemDC,0,0,DROPWIDTH,DROPHEIGHT);

			 ohbrush = CreateSolidBrush(RGB(204,204,204));
			 
			 //�e�h���b�v�t�B�[���h�̍쐬
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

			 //�e�t�B�[���h�󋵂̍X�V
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

			 //�^�C�g���ύX�p�_�C�A���O�̐���
			 EdTitle = CreateDialog(
				ghInstance,
				MAKEINTRESOURCE(IDD_DIALOG1) , hWnds ,(DLGPROC)EdTitlePro
			 );

			 SetTimer(hWnd,2,100,NULL);

		return 0;

		case WM_DROPFILES:
				
			//�p�����[�^�[�̎擾
			hDrop = (HDROP)wParam;
			//�h���b�v��
			uiFileCount = DragQueryFile(hDrop,0xFFFFFFFF,NULL,0);
	
			if(uiFileCount<=1){

				//�t�@�C���p�X�̎擾
				DragQueryFile(hDrop,0,strFilePath,MAX_PATH);

				DragQueryPoint(hDrop,&dropP);


				//�h���b�v�̈�̓���
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

				//MSI�V���[�g�J�b�g�`�F�b�N
				if(ERROR_SUCCESS == MsiGetShortcutTarget(strFilePath, szProductCode, szFeatureId, szComponentCode)){
					
					//MSI�V���[�g�J�b�g����̃p�X�𓾂�
					MsiGetComponentPath(szProductCode, szComponentCode,strFilePath,&dwBuffSize);

				}else{

					//�ʏ�V���[�g�J�b�g���V���[�g�J�b�g���ǂ���
					//IShellLink�C���^�[�t�F�C�X���g��
					dwRet = SHGetFileInfo(strFilePath,0,&shfi,sizeof(SHFILEINFO),SHGFI_ATTRIBUTES);
					if(dwRet){
						//�V���[�g�J�b�g�Ȃ��
						if(shfi.dwAttributes & SFGAO_LINK){

							//OLE�̏�����
							CoInitialize(NULL);

							//��̃C���^�[�t�F�[�X���쐬
							CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(LPVOID *)&pShellLink);

							//IPersistFile�ւ̃|�C���^���擾
							pShellLink->QueryInterface(IID_IPersistFile,(LPVOID *)&pPersistFile);
					
							//�V���[�g�J�b�g��ǂݍ��݁B
							pPersistFile->Load(strFilePath, STGM_READ );

							//�����N�����肷��
							pShellLink->Resolve( NULL, SLR_UPDATE );

							//�p�X���擾
							pShellLink->GetPath(strFilePath,MAX_PATH,NULL,0);

							if(!lstrlen(strFilePath)){

								DragQueryFile(hDrop,0,strFilePath,MAX_PATH);

							}

							//���
							pPersistFile->Release();
							pShellLink->Release();
							CoUninitialize();

						}

					}
				
				}

				list = (HIMAGELIST)SHGetFileInfo(strFilePath, FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof(SHFILEINFO),SHGFI_SYSICONINDEX  | SHGFI_LARGEICON);
				icon = ImageList_GetIcon(list,shfi.iIcon, ILD_NORMAL);

				//�\���̂̍X�V
				if(flag){

					dpoint = tx+(ty*(DROPSIZEX));
					wsprintf(buff,TEXT("%s%d"),tarGetSection,dpoint);

					//�p�X���h���C�u�݂̂�������
					if(strFilePath[1]==':' && strFilePath[2]=='\\' && strFilePath[3]==NULL){

						lstrcpy((tarGetdMenu+dpoint)->dname,strFilePath);
						WritePrivateProfileString(buff,key2,strFilePath,path);

					}else{

						//�t�@�C���̖��O���擾
						GetFileTitle(strFilePath,(LPWSTR)nameBuff,sizeof(nameBuff));

						lstrcpy((tarGetdMenu+dpoint)->dname,(LPCWSTR)nameBuff);
						WritePrivateProfileString(buff,key2,(LPCWSTR)nameBuff,path);

					}

					(tarGetdMenu+dpoint)->dIcon = icon;

					lstrcpy((tarGetdMenu+dpoint)->dPath,strFilePath);

					WritePrivateProfileString(buff,key1,strFilePath,path);
					

					//�̈�𔒂�
					SelectObject(DhMemDC,back_brush);
					Rectangle(DhMemDC,drct[ty][tx].left,drct[ty][tx].top,drct[ty][tx].left+35,drct[ty][tx].top+35);
					InvalidateRect(hWnd,NULL,FALSE);

					//�A�C�R���̕`��
					DrawIcon(DhMemDC,drct[ty][tx].left,drct[ty][tx].top,icon);
					InvalidateRect(hWnd,NULL,FALSE);
				}


			}else{
				Message(MhWnd,MLhWnd,TEXT("�h���b�v�ł��Ȃ���"),MessageTW,MessageTH,0);
				errorMessage=TRUE;
			}


		return 0;

		case WM_PAINT:

			//�N���C�A���g�̈�̃f�o�C�X�R���e�L�X�g�̎擾
			Dhdc = BeginPaint(hWnd,&ps);

			//���z����f�o�C�X�ւ̓]��
			BitBlt(
					Dhdc,0,0,DROPWIDTH,DROPHEIGHT,DhMemDC,0,0,SRCCOPY
			);

			EndPaint(hWnd,&ps);

		return 0;

		case WM_TIMER:

			//��莞�Ԃ��ƂɃh���b�v�̈�̊Ď�
			GetCursorPos(&mp);
			ScreenToClient(hWnd,&mp);
			GetClientRect(hWnd,&winrct);

			if(!IsWindowVisible(DhWnd)){
				popFlag=TRUE;
			}

			if(!popFlag){

				//�J�[�\�����h���b�v���j���[�̈�Ȃ�
				if(!PtInRect(&winrct,mp)){

				if(outWord){ outWord=FALSE; }

				//�̈�𔒂�
				for(pi=0;pi<DROPSIZEY;pi++){
					for(pj=0;pj<DROPSIZEX;pj++){
							if(!*(tarDP+(pi*5)+pj)){
								SelectObject(DhMemDC,back_brush);
								Rectangle(DhMemDC,drct[pi][pj].left,drct[pi][pj].top,drct[pi][pj].left+33,drct[pi][pj].top+33);
							}
						}
					}

				//���b�Z�[�W�̒���
				if(IsWindowVisible(MhWnd) && !MessageCloseFlag){
					CloseMessage(MhWnd,MLhWnd);
				}

				//�G���[���b�Z�[�W�̃t���O��߂�
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
									//�̈���D�F�ɂ���
									RoundRect(DhMemDC,drct[pi][pj].left,drct[pi][pj].top,drct[pi][pj].left+32,drct[pi][pj].top+32,10,10);
									if(!errorMessage){
										CloseMessage(MhWnd,MLhWnd);
									}
								}else{
									if(!errorMessage && IsWindowVisible(hWnd)){
										dpoint = cpx+(cpy*(DROPSIZEX));
										//�����o���̕\��
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


		//�؂�ւ�
		case WM_CHDROPM:

			//�̈�𔒂�
			for(pi=0;pi<DROPSIZEY;pi++){
				for(pj=0;pj<DROPSIZEX;pj++){
					SelectObject(DhMemDC,back_brush);
					Rectangle(DhMemDC,drct[pi][pj].left,drct[pi][pj].top,drct[pi][pj].left+33,drct[pi][pj].top+33);
				}
			}

			if(!swi){

				//�h���b�v�i�[���̐؂�ւ�
				tarDP = (int *)DPN;

				//�\���̂̐؂�ւ�
				tarGetdMenu = (dMenu *)dMenuDataN;

				//�Z�N�V�����̐؂�ւ�
				tarGetSection = section2;

				swi=1;

			}else{

				//�h���b�v�i�[���̐؂�ւ�
				tarDP = (int *)DP;

				//�\���̂̐؂�ւ�
				tarGetdMenu = (dMenu *)dMenuData;

				//�Z�N�V�����̐؂�ւ�
				tarGetSection = section;

				swi=0;

			}

			//�A�C�R���̕`��
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
						Message(MhWnd,MLhWnd,TEXT("�t�@�C�����J���Ȃ���"),MessageTW,MessageTH,0);
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

				//���j���[�̃n���h��
				dhMenu = LoadMenu(ghInstance, MAKEINTRESOURCE(IDR_MENU2));
				// �T�u���j���[�̃n���h�����擾����
				dhSubMenu = GetSubMenu(dhMenu, 0 );

				//�}�E�X�J�[�\���̍��W�擾
				poss.x = LOWORD(lParam);
				poss.y = HIWORD(lParam);

				ClientToScreen(hWnd,&poss);

				SetForegroundWindow(hWnds);
				// �|�b�v�A�b�v���j���[��\������
				TrackPopupMenu( dhSubMenu,TPM_LEFTALIGN,poss.x,poss.y,0,hWnd, NULL );
				PostMessage(hWnds,WM_NULL,0L,0L);

			}

		return 0;

		case WM_COMMAND:
            switch(LOWORD(wParam)) {

				//�o�^���j���[�A�C�R���̍폜
				case ID_D:

					popFlag=FALSE;

					dpoint = cpx+(cpy*(DROPSIZEX));
					wsprintf(buff,TEXT("%s%d"),tarGetSection,dpoint);
					WritePrivateProfileString(buff,key1,TEXT("Nothing"),path);
					WritePrivateProfileString(buff,key2,TEXT("Nothing"),path);

					*(tarDP+(cpy*5)+cpx)=0;
					//�\���̂̍X�V
					(tarGetdMenu+dpoint)->dIcon = NULL;
					lstrcpy((tarGetdMenu+dpoint)->dPath,TEXT(" "));


					SelectObject(DhMemDC,back_brush);
					Rectangle(DhMemDC,drct[cpy][cpx].left,drct[cpy][cpx].top,drct[cpy][cpx].left+35,drct[cpy][cpx].top+35);

					InvalidateRect(hWnd,NULL,FALSE);

               break;

			   //�^�C�g���̕ύX
				case ED_TITLE:

					CloseMessage(MhWnd,MLhWnd);

					dpoint = cpx+(cpy*(DROPSIZEX));
	
					wndpl.length = sizeof(WINDOWPLACEMENT);
					GetWindowPlacement(hWnd, &wndpl);
					rc = wndpl.rcNormalPosition;


					EdTitleFlag=TRUE;

					//�^�C�g����������Z�b�g
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

        case WM_CLOSE: // �ˑR�̏I���֎~
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
/* �h���b�v�A�C�R�����C�A�E�g�p�E�B���h�E�v���V�[�W��			
/*----------------------------------------------------------*/

LRESULT CALLBACK WindowProcDropIconLy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	FILE *fp1;								 //�摜�p
	static BLENDFUNCTION blend;
	static int gbAlpha=0;
	
	switch(uMsg){

		case WM_CREATE:

			//PNG�̓ǂݏo��
			fp1 = fopen(img_item_n1, "rb");

			//�ǂݏo���̃G���[�`�F�b�N
			if (fp1 == NULL ) {
                   MessageBox(hWnd, TEXT(".png���J���܂���B"), TEXT("�G���["), MB_OK | MB_ICONSTOP);
                   DestroyWindow(hWnd);
                   break;
            }

		    //�������ɓǂݍ���png�t�@�C�����A�r�b�g�}�b�v�n���h���ɂ���
			hWaku = LoadPngFromFile(hWnd, fp1, uWidth, uHeight);

			//���
			fclose(fp1);

			//���C���摜(png)�\��
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

				//���C�A�E�g�h���b�v���j���̃_�C�A���O��\��
				ShowWindow(LhWnd,SW_HIDE);
				//�h���b�v���j���E�B���h�E��\��
				ShowWindow(DhWnd,SW_HIDE);
			}

		return 0;



	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


/*----------------------------------------------------------*/
/*	    �h���b�v�^�C�g���ύX�p�_�C�A���O�v���V�[�W���[
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

						//�\���̂̍X�V
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
/*        �����o���p�E�B���h�E�v���V�[�W��			
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

			 //�t�H���g�̐ݒ�
			 hFont = CreateFont(13,0,0,0,0,FALSE,FALSE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,NULL);
			 SelectObject(MesDC,hFont);


		return 0;

		case WM_LBUTTONDOWN:

			MessageCloseFlag = FALSE;
			errorMessage=FALSE;
			CloseMessage(MhWnd,MLhWnd);

		break;
		
		case WM_PAINT:

			//�N���C�A���g�̈�̃f�o�C�X�R���e�L�X�g�̎擾
			Thdc = BeginPaint(hWnd,&ps);

			//���z����f�o�C�X�ւ̓]��
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
/*        �����o�����C�A�E�g�p�E�B���h�E�v���V�[�W��			
/*----------------------------------------------------------*/

LRESULT CALLBACK WindowProcMessageLy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	FILE *fp1;								 //�摜�p

	switch(uMsg){

		case WM_CREATE:

			//PNG�̓ǂݏo��
			fp1 = fopen(img_item_n2, "rb");

			//�ǂݏo���̃G���[�`�F�b�N
			if (fp1 == NULL ) {
                   MessageBox(hWnd, TEXT(".png���J���܂���B"), TEXT("�G���["), MB_OK | MB_ICONSTOP);
                   DestroyWindow(hWnd);
                   break;
            }

		    //�������ɓǂݍ���png�t�@�C�����A�r�b�g�}�b�v�n���h���ɂ���
			Mes = LoadPngFromFile(hWnd, fp1, uWidth, uHeight);

			//���
			fclose(fp1);

			//���C���摜(png)�\��
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
/*	              �ݒ�p�_�C�A���O�v���V�[�W���[
/*----------------------------------------------------------*/

BOOL CALLBACK SystemEdit(HWND Dialog,UINT uMsg,WPARAM wParam,LPARAM lParam){

	TCHAR *stok = TEXT("�X�^�[�g�A�b�v�ɓo�^���܂����H");
	TCHAR *stno = TEXT("�X�^�[�g�A�b�v���������܂����H");
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


				//�ݒ�{�^��
				case IDC_BUTTON2:

					//�ꎞ�I�Ƀ��[�h�����͂��Ȃ�
					CloseMessage(MhWnd,MLhWnd);
					actWord=FALSE;

					//�X���C�_�[
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

					MessageBox(Dialog,VER,TEXT("�o�[�W�������"),MB_OK);

				break;

				case IDC_STARTUP:

					if(stFlag){
						lstrcpy(stbuff,stno);
					}else{
						lstrcpy(stbuff,stok);
					}
					
				    if(IDCANCEL!=MessageBox(Dialog,stbuff,TEXT("�m�F"),MB_OKCANCEL)){
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
/*        "���t��������" �p�_�C�A���O�v���V�[�W��			
/*----------------------------------------------------------*/

BOOL CALLBACK WordProc(HWND Dialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	//������������35�����A�o�C�g��70�o�C�g

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

		//�L�[�������ꂽ��
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

						//�^�O���X�g��\��������
						createTagList(editWord);

						//���t��ݒ��ʂɕ\��������
						lstrcpy(nameBuff,keyBuff);
						lstrcat(nameBuff,TEXT(" �Ƃ́H"));
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
/*        "���t��������" �ݒ�p�_�C�A���O�v���V�[�W��			
/*----------------------------------------------------------*/

BOOL CALLBACK EdWordProc(HWND Dialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	int no=0,e=0;
	TCHAR strName[30];

	switch(uMsg){

		case WM_COMMAND:

			switch(LOWORD(wParam)){

				case ID_WOK:

					//�^�[�Q�b�g���X�g�̎擾
					no = SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_GETCURSEL,0L,0L);
					SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_GETTEXT,(WPARAM)no,(LPARAM)strName);

					if (no!=LB_ERR){

						//XML�ւ̒ǉ�
						if(addWord(keyBuff,strName,tmppath)){
							errorMessage=TRUE;
							MessageCloseFlag = TRUE;
							Message(MhWnd,MLhWnd,TEXT("�G���[����E�E�E�iXML�n�j"),MessageTW,MessageTH,0);
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
/*        ���t��Y���p�_�C�A���O�v���V�[�W���[		
/*----------------------------------------------------------*/


BOOL CALLBACK DelWordProc(HWND Dialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	static int no=-1;
	WINDOWPLACEMENT  wndpl;
	RECT rc;

	switch(uMsg){

		case WM_INITDIALOG:

			//���[�h���X�g�̎擾
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

					 //�t�H�[�J�X�Ώۂ̔ԍ����擾
					 if(HIWORD(wParam) == LBN_SELCHANGE){
						
						 no = SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_GETCURSEL,0L,0L);
						 DelSetTagName(Dialog,no);

					 }

				 break;

				 case IDC_BUTTON1:

					if(no>=0){

						//�폜����
						if(delWord(no,tmppath)){
							errorMessage=TRUE;
							MessageCloseFlag = TRUE;
							popFlag = TRUE;
							Message(MhWnd,MLhWnd,TEXT("�G���[����E�E�E�iXML�n�j"),MessageTW,MessageTH,0);
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
/*        RSS�ݒ�p�_�C�A���O�v���V�[�W���[		
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

					
			//�X���C�_�[�̐ݒ�
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

							//�V�K�f�[�^��ǉ�����
							rssDataAdd(nameBuff,urlBuff);

							//�������t�@�C���ɏ�������
							no = getRssIniData();
							wsprintf(ints,TEXT("%d"),no);

							//�����ʒu�����߂�
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

							//�G�f�B�b�g�����N���A����
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

					//�X���C�_�[
					sli = SendDlgItemMessage(Dialog,IDC_SLIDER1,TBM_GETPOS,0,0);
					wsprintf(sliBuff,TEXT("%d"),sli);
					WritePrivateProfileString(TEXT("rss"),TEXT("speed"),sliBuff,rpath);
					sli++;
					sli=sli*50;

					rssSpeed=sli;

					ShowWindow(Dialog,SW_HIDE);

				break;

				case IDC_LIST1:

					 //�t�H�[�J�X�Ώۂ̔ԍ����擾
					 if(HIWORD(wParam) == LBN_SELCHANGE){
						
						no = SendMessage(GetDlgItem(Dialog,IDC_LIST1),LB_GETCURSEL,0L,0L);
						RssSetName(&setname,no);
						RssSetLink(&setlink,no);

						SetWindowText(GetDlgItem(Dialog,IDC_NAME),setname);
						SetWindowText(GetDlgItem(Dialog,IDC_URL),setlink);

					 }

				break;

				//�ҏW
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

				//�폜
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
/*                  RSS���X�g�ɃZ�b�g	
/*----------------------------------------------------------*/

void SetListRssDataName(TCHAR *name){

	SendMessage(GetDlgItem(wRss,IDC_LIST1),LB_ADDSTRING,0,(LPARAM )(LPCTSTR )name);

}
/*----------------------------------------------------------*/
/*                  RSS�����X�s�[�h�̎擾
/*----------------------------------------------------------*/

int GetRssSlspeed(TCHAR *path){

	return (GetPrivateProfileInt(TEXT("rss"),TEXT("speed"),0,path)+1)*50;

}

/*----------------------------------------------------------*/
/*                 RSS�\���p�E�B���h�E		
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
				MessageBox(hWnds,TEXT("�摜�t�@�C��������܂���"),NULL, MB_OK);
				break;
			}

			//�o�c�}�[�N
			SetRect(&rbatu,305,15,332,40);
			SetRect(&rst,100,15,127,40);
			SetRect(&rstop,146,15,175,40);

			//�E�B���h�E�ʒu����
			GetWindowPlacement(	hWnds, &wndpl );
			rc = wndpl.rcNormalPosition;
			SetWindowPos(Dialog,NULL,rc.left-RSSHOSEIL,rc.top+195,RSSW,RSSH,NULL);

			//�������f�o�C�X
			hdc = GetDC(Dialog);
			hMemDC = CreateCompatibleDC(hdc);
			SelectObject(hMemDC,hbmp);

			//�t�H���g�̐ݒ�
			hFont = CreateFont(13,0,0,0,0,FALSE,FALSE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,NULL);
		    SelectObject(hMemDC,hFont);

			//--- �^�C�g���\���̐ݒ�
			GetClientRect(Dialog,&rct);
			GetClientRect(Dialog,&rctt);

			//�T�C�g�^�C�g��
			rctt.top+=50;
			rctt.left+=10;
			rctt.right-=10;
			rctt.bottom-=10;
			//�^�C�g��
			rct.top+=70;
			rct.left+=10;
			rct.right-=10;
			rct.bottom-=10;

			SetTimer(Dialog,6,rssSpeed,NULL);

			TextOut(hMemDC,rctt.left,rctt.top,TEXT("�ǂݍ��ݒ�����E�E�E"),lstrlen(TEXT("�ǂݍ��ݒ�����E�E�E")));
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


			//�}�E�X���W�͓o�^����RECT�����ǂ���
			if(PtInRect(&rbatu,mouse)){

				hand = TRUE;

			}else if(PtInRect(&rclink,mouse)){

				 //�t�H���g�̐ݒ�
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

			//�}�E�X���W�͓o�^����RECT�����ǂ���
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

		//�J�[�\��
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
/*		            �V�X�e���n�̏�����		
/*----------------------------------------------------------*/

void SystemInit(){

	int st,cw,ct;
	LONG setSli;

	//�������t�@�C������̓ǂݍ���
	cw = GetPrivateProfileInt(TEXT("word"),TEXT("act"),0,path);
	st = GetPrivateProfileInt(TEXT("system"),TEXT("startup"),0,path);
	ct = GetPrivateProfileInt(TEXT("word"),TEXT("time"),0,path);
	setSli = (LONG)GetPrivateProfileInt(TEXT("word"),TEXT("speed"),50,path);

	//������ׂ�@�\�̗L��
	if(cw){
		CheckDlgButton(sDlg,IDC_CHECK1,1);
		actWord=TRUE;
	}else{
		actWord=FALSE;
	}

	//���t���[�h�����̗L��
	if(ct){
		CheckDlgButton(sDlg,IDC_CHECK3,1);
		StTime=TRUE;
	}


	//�X���C�_�[�̐ݒ�
	SendDlgItemMessage(sDlg,IDC_SLIDER1, TBM_SETPOS, TRUE,setSli);

	//���[�h�X�s�[�h�̐ݒ�
	wspeed = (int)setSli;

	//�X�^�[�g�A�b�v�ɓo�^����Ă��邩
	if(st){
		SetDlgItemText(sDlg,IDC_STARTUP,TEXT("�X�^�[�g�A�b�v�̓o�^����"));
		stFlag=TRUE;
	}



}



/*----------------------------------------------------------*/
/*		        �E�B���h�E�ނ̔�\��			
/*----------------------------------------------------------*/

void CloseWindows(){

		ShowWindow(LhWnd,SW_HIDE);
		ShowWindow(DhWnd,SW_HIDE);
		ShowWindow(MLhWnd,SW_HIDE);
		ShowWindow(MhWnd,SW_HIDE);
		ShowWindow(hWnd,SW_HIDE);

}


/*----------------------------------------------------------*/
/*		       �X�^�[�g�A�b�v�ɓo�^&����			
/*----------------------------------------------------------*/

void ExStartUp(){

	TCHAR SCpath[MAX_PATH];
	TCHAR paths[MAX_PATH];
	TCHAR buffLeft[10];
	TCHAR buffTop[10];
	HRESULT hr;							 //�߂�l
    IShellLink *pShellLink = NULL;
    IPersistFile *pPersistFile = NULL;
	WINDOWPLACEMENT wndpl;
	RECT rc;

	//�X�^�[�g�A�b�v�̃p�X�����߂�
	SHGetSpecialFolderPath (NULL, paths, CSIDL_STARTUP, 0);

	//�쐬�p�X
	lstrcat(paths,TEXT("\\NA�����.lnk"));

	//�^�[�Q�b�g�p�X
	GetModuleFileName(NULL,SCpath,MAX_PATH);

	if(stFlag){

		//�V���[�g�J�b�g�p�X���X�^�[�g�A�b�v�t�H���_����폜
		if(DeleteFile(paths)==TRUE) {

			WritePrivateProfileString(TEXT("window"),TEXT("left"),TEXT("-1000"),path);
			WritePrivateProfileString(TEXT("window"),TEXT("top"),TEXT("-1000"),path);
			WritePrivateProfileString(TEXT("system"),TEXT("startup"),TEXT("0"),path);

			SetDlgItemText(sDlg,IDC_STARTUP,TEXT("�X�^�[�g�A�b�v�ɓo�^"));
			
			stFlag=FALSE;

		}


	}else{

		try{

			//OLE������
			CoInitialize(NULL);

			//IShellLink�C���^�[�t�F�C�X�̍쐬
			hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
					IID_IShellLink, (void**)&pShellLink);
			if (hr != S_OK)
				throw(FALSE);

			//�����N��p�X�̐ݒ�
			pShellLink->SetPath(SCpath);

			//IPersistFile�C���^�[�t�F�C�X�̎擾
			if (pShellLink->QueryInterface(IID_IPersistFile, 
						(void**)&pPersistFile) != S_OK)
				throw(FALSE);

			//�t�@�C���ɕۑ�
			if (pPersistFile->Save(paths, true) != S_OK)
				throw(FALSE);
			throw(TRUE);

		}
		catch(BOOL IsOK)
		{
			//�u�W�F�N�g���
			if (pPersistFile != NULL)
				pPersistFile->Release();
			if (pShellLink != NULL)
				pShellLink->Release();

			// OLE�I������
			CoUninitialize();
			if (!IsOK){
				MessageBox(NULL, TEXT("�V���[�g�J�b�g�쐬���s"), NULL, MB_OK);
			}else{

				//�E�B���h�E�̈ʒu
				wndpl.length = sizeof(WINDOWPLACEMENT);
				GetWindowPlacement(hWnds, &wndpl);
				rc = wndpl.rcNormalPosition;

				wsprintf(buffLeft,TEXT("%d"),rc.left);
				wsprintf(buffTop,TEXT("%d"),rc.top);

				WritePrivateProfileString(TEXT("system"),TEXT("startup"),TEXT("1"),path);
				WritePrivateProfileString(TEXT("window"),TEXT("left"),buffLeft,path);
				WritePrivateProfileString(TEXT("window"),TEXT("top"),buffTop,path);
				SetDlgItemText(sDlg,IDC_STARTUP,TEXT("�X�^�[�g�A�b�v�̓o�^����"));
				stFlag=TRUE;
			}

		}
	}

}


/*----------------------------------------------------------*/
/*		        PNG�\���ƃ��C���[�ݒ�			
/*----------------------------------------------------------*/

void CreatePngLext(HWND hWnd,HBITMAP hBitmap,int alpha){

			HGDIOBJ hOldObj;

			//�E�B���h�E�̌`����r�b�g�}�b�v�ɍ��킹�ĕύX
            SetWindowPos(hWnd, 0, 0, 0, uWidth, uHeight, SWP_NOMOVE | SWP_NOZORDER);

            // �e��f�o�C�X�R���e�L�X�g�̎擾
            HDC hmemdc, hdc, hsdc;
            hsdc   = GetDC(0);                      // �f�X�N�g�b�v�̃f�o�C�X�R���e�L�X�g�i�F���w��p�j
            hdc    = GetDC(hWnd);                   // ���̃E�B���h�E�̃f�o�C�X�R���e�L�X�g
            hmemdc = CreateCompatibleDC(hdc);       // hdc�̌݊��f�o�C�X�R���e�L�X�g

            POINT wndPos;
            SIZE  wndSize;
            RECT  rc;

            // ���C���[�h�E�B���h�E�̉�ʈʒu�ƃT�C�Y
            GetWindowRect(hWnd, &rc);
            wndPos.x = rc.left;
            wndPos.y = rc.top;
            wndSize.cx = uWidth;
            wndSize.cy = uHeight;

            // �f�o�C�X�R���e�L�X�g�ɂ����郌�C���̈ʒu
            POINT po;
            po.x = po.y = 0;

            // ���C���[�h�E�B���h�E�̎w��
            BLENDFUNCTION blend;
            blend.BlendOp = AC_SRC_OVER;
            blend.BlendFlags = 0;
            blend.SourceConstantAlpha = alpha; // �s�����x�i���C���[�h�E�B���h�E�S�̂̃A���t�@�l�j
            blend.AlphaFormat = AC_SRC_ALPHA;

            // �摜��`�������
            hOldObj = SelectObject(hmemdc, hBitmap);
            BitBlt(hdc, 0, 0, uWidth, uHeight, hmemdc, 0, 0, SRCCOPY|CAPTUREBLT); // ���C���[�h�E�B���h�E�ł�CAPTUREBLT���K�v

            // ���C���[�h�E�B���h�E�̈ʒu�A�T�C�Y�A�`�A���e�A�����x���X�V
            if (0 == UpdateLayeredWindow(hWnd, hsdc, &wndPos, &wndSize, hmemdc, &po, 0, &blend, ULW_ALPHA)) {
                  TCHAR strErrMes[80];
                  DWORD err = GetLastError();

                  wsprintf(strErrMes, TEXT("UpdateLayeredWindow���s�F�G���[�R�[�h��%d"), err);
                  MessageBox(hWnd, strErrMes, TEXT("�G���["), MB_OK | MB_ICONSTOP);
                  DestroyWindow(hWnd);
              }

             SelectObject(hmemdc, hOldObj);
             DeleteDC(hmemdc);
             ReleaseDC(hWnd, hdc);
             ReleaseDC(0, hsdc);

}

/*----------------------------------------------------------*/
/*		                ���t���擾		
/*----------------------------------------------------------*/

void timeData(TCHAR *t){

	struct tm *date;
	time_t now;
	int year, month,day,w;
	TCHAR wD[] = TEXT("�����ΐ��؋��y");

	/* ���݂̓������擾 */
	time(&now);
	date = localtime(&now);

	year = date->tm_year + 1900;
	month = date->tm_mon + 1;
	day = date->tm_mday;
	w = date->tm_wday;

	wsprintf(t,TEXT("%d�N%d��%d��\n\n����ƁE�E�E�� %c�j�������"),year,month,day,wD[w]);

}

/*----------------------------------------------------------*/
/*		              URL���ǂ���
/*----------------------------------------------------------*/

int chekUrl(TCHAR *url){

	if(url[0]=='h' && url[1]=='t' && url[2]=='t' && url[3]=='p'){
		return 1;
	}

	return 1;

}


/*----------------------------------------------------------*/
/*		         	�E�B���h�E�̓o�^			
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


	//�h���b�v�A�C�R���`��p�E�B���h�E
    wc.lpfnWndProc    = WindowProcDropIcon;
    wc.lpszClassName  = APP_NAME2;
	RegisterClassEx(&wc);

	//�h���b�v�A�C�R�����C�A�E�g�p�E�B���h�E
    wc.lpfnWndProc    = WindowProcDropIconLy;
    wc.lpszClassName  = APP_NAME3;

    RegisterClassEx(&wc);

	//�h���b�v�A�C�R�����C�A�E�g�p�E�B���h�E
    wc.lpfnWndProc    = WindowProcMessageLy;
    wc.lpszClassName  = APP_NAME4;

	RegisterClassEx(&wc);

	
	//�h���b�v�A�C�R���p�E�B���h�E
    wc.lpfnWndProc    = WindowProcMessage;
    wc.lpszClassName  = APP_NAME5;

	RegisterClassEx(&wc);

	return atom;
}


/*----------------------------------------------------------*/
/*		        	�E�B���h�E�̐���			
/*----------------------------------------------------------*/

BOOL CreateAppWindow(HINSTANCE hInstance){

	hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,APP_NAME,TEXT("NA"),WS_POPUP,
		X,Y,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL);



    //�h���b�v�A�C�R�����C�A�E�g�p�E�B���h�E
	LhWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        APP_NAME3,TEXT("NA -dropwindowLy-"),WS_POPUP,
        X-DROPWPOT,Y+MH-DROPHPOT,DROPWIDTH,DROPHEIGHT,hWnd, NULL, hInstance, NULL);


	//�h���b�v�A�C�R���p�E�B���h�E
	DhWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        APP_NAME2,TEXT("NA -dropwindow-"),WS_POPUP,
        X-DROPWPOT+DWOSEI,Y+MH-DROPHPOT+DHOSEI,DROPWIDTH-SHOSEI,DROPHEIGHT-SHOSEI,LhWnd, NULL, hInstance, NULL);


	//�����o���p���C�A�E�g�E�B���h�E
	MLhWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        APP_NAME4,TEXT("NA -messageLy-"),WS_POPUP,
        X-MTX-TXHOSEI,Y+MTY-TYHOSEI,MessageW,MessageH,hWnd, NULL, hInstance, NULL);

	
	
	//�����o���p�E�B���h�E
	MhWnd = CreateWindowEx(WS_EX_TOOLWINDOW,
        APP_NAME5,TEXT("NA -message-"),WS_POPUP,
        X-MTX,Y+MTY,MessageTW,MessageTH,MLhWnd, NULL, hInstance, NULL);




	if(!hWnd || !DhWnd || !LhWnd || !MLhWnd || !MhWnd) {
        return FALSE;
    }

	
	//�C���X�^���X�̃R�s�[
	ghInstance = hInstance;

	hWnds = hWnd;

	//�V�X�e���n�̏�����
	SystemInit();

	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);


	//���[�h�̏�����
	if(!createOTMessage(tmppath,2)){

		MessageCloseFlag = TRUE;
		errorMessage=TRUE;

		if(StTime){
			timeData(words);
		}else{
			//�N�����̐����o����\��
			createOTMessage(tmppath,1);
		}

		if(lstrlen(words)){
			Message(MhWnd,MLhWnd,words,MessageTW,MessageTH,0);
		}

	}else{
		Message(MhWnd,MLhWnd,TEXT("XML�ǂݍ��݃G���[����"),MessageTW,MessageTH,0);
	}

	return TRUE;
}
