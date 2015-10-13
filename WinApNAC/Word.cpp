/*----------------------------------------------------------*/
/*			            ワード生成処理
/*
/*
/* メインと共有しているwords変数(TCHAR)に文字をコピー
/*
/* ---- フォーマットの拡張はここで行う
/*----------------------------------------------------------*/

#include <windows.h>
#include <shlwapi.h>
#include <stdlib.h>
#include <time.h>
#include <atlbase.h>  //CComPtrを使用するため
#include <atlstr.h>   //CStringを使用するため
#include <xmllite.h>  //xmllite
#include "zxor.h"	  //乱数生成
#include "Word.h"
#include "wordExtrn.h"
#include "resource.h"
#include "formatWord.h"

/*---------------------------------------------------------*/
/*		           プロトタイプ宣言			
/*----------------------------------------------------------*/

void w4Format(TCHAR *);
void OrWordFormat(TCHAR *);
int readWord(TCHAR *,TCHAR *);
void startAppWord(TCHAR *);
int initWord(TCHAR *);
void freeList();
int newWord(TCHAR *);

/*----------------------------------------------------------*/
/*		            グローバル変数			
/*----------------------------------------------------------*/

TCHAR Word[50];
xmlStList *dataXList;  //取得ターゲットになる構造体ポインタ
int listMax=0;



/*----------------------------------------------------------*/
/*		         ランダムの数値を生成			
/*----------------------------------------------------------*/

int randInt(int x){

	int n;
	
	n = NextIntEx(x);

	return n; 

}


/*----------------------------------------------------------*/
/*		        タグワードをリストに更新		
/*----------------------------------------------------------*/

void createTagList(HWND hWnd){

	int i=0;

	while(i<KEYMAX){

		SendMessage(GetDlgItem(hWnd,IDC_LIST1),LB_ADDSTRING,0,(LPARAM )(LPCTSTR )keyword[i]);

		i++;
	}

}

/*----------------------------------------------------------*/
/*		        デリートワードリストに更新		
/*----------------------------------------------------------*/

void DelcreateWordList(HWND hWnd){

	int i=0;

	while(i<listMax){

		SendMessage(GetDlgItem(hWnd,IDC_LIST1),LB_ADDSTRING,0,(LPARAM )(LPCTSTR )dataXList[i].xname);

		i++;
	}


}

/*----------------------------------------------------------*/
/*		     デリートワード(ListBox)のタグ名を表示		
/*----------------------------------------------------------*/

void DelSetTagName(HWND hWnd,int n){

	SetDlgItemText(hWnd,IDC_STATICS,dataXList[n].xtag); 

}

/*----------------------------------------------------------*/
/*		         ワード生成メイン処理	
/*----------------------------------------------------------*/

int createOTMessage(TCHAR *mpath,int vp){


	int format=0;
	TCHAR path[MAX_PATH];

	lstrcpy(path,mpath);
	lstrcat(path,TEXT("Words.xml"));

	ZeroMemory(words,sizeof(TCHAR));
	ZeroMemory(Word,sizeof(TCHAR));


	//特定条件付きワードなら
	switch(vp){

		//起動時のワード生成
		case 1:
			startAppWord(path);
		break;

		//初期化
		case 2:
			if(initWord(path)){
				return 1;
			}
		break;
		
		//解放
		case 3:

			freeList();

		break;

		case 0:

			//フォーマットの決定
			format = randInt(8);
	
			//フォーマットごとの処理

	        //---------  拡張対象 -------------//

			switch(format){

				// 0 ---- いつ、どこで、誰と、なにをした
				case 0:
					w4Format(path);
				break;
					
				// 1 ---- セリフ
				case 1:
					OrWordFormat(path);
				break;

				// 2 ---- 食べ物
				case 2:
					GetfoodFormat(TEXT("食べ物"),path);

					if(lstrlen(Word)){
						SetfoodFormat(Word);
						lstrcpy(words,Word);
					}
				break;

				// 3 ---- 服
				case 3:
					GetClothesFormat(TEXT("服"),path);

					if(lstrlen(Word)){
						SetClothesFormat(Word);
						lstrcpy(words,Word);
					}
				break;

				// 4 ---- 機械
				case 4:
					GetMachineFormat(TEXT("機械"),path);

					if(lstrlen(Word)){
						SetMachineFormat(Word);
						lstrcpy(words,Word);
					}
				break;

				// 5 ---- 本
				case 5:
					GetBookFormat(TEXT("本"),path);

					if(lstrlen(Word)){
						SetBookFormat(Word);
						lstrcpy(words,Word);
					}
				break;

				// 6 ---- アクセサリー
				case 6:
					GetAccessoryFormat(TEXT("アクセサリー"),path);

					if(lstrlen(Word)){
						SetAccessoryFormat(Word);
						lstrcpy(words,Word);
					}
				break;
				
				// 7 ---- 飲み物
				case 7:
					GetDrinkFormat(TEXT("飲み物"),path);

					if(lstrlen(Word)){
						SetDrinkFormat(Word);
						lstrcpy(words,Word);
					}
				break;

			}
		break;

	}

return 0;

}



/*----------------------------------------------------------*/
/*		           XMLへの初期読み込み	
/*----------------------------------------------------------*/

int initWord(TCHAR *path){

    LPCWSTR pwszLocalName;
    LPCWSTR pwszValue;
    XmlNodeType nodeType;
	int i=1;

	InitMt((int)time(NULL));

	//IXmlReaderインターフェイスの実装
    CComPtr<IXmlReader> pReader=NULL;
    if(FAILED(
        CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&pReader), 0))){
        MessageBox(NULL, 
            _T("CreateXmlReader失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//---- リーダーが入力として使用するストレージを指定する
	// ファイルストリーム作成
    CComPtr<IStream> pStream;
    if(FAILED(SHCreateStreamOnFile(path, STGM_READ, &pStream))){
        MessageBox(NULL, 
            _T("SHCreateStreamOnFile失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//ファイルストリームを関連付
	if(FAILED(pReader->SetInput(pStream))){
        MessageBox(NULL, 
            _T("SetInput失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }


	//ノードを読み込んでいく
    while(S_OK == pReader->Read(&nodeType)){

		switch(nodeType){

			//要素の開始
			case XmlNodeType_Element:

				//要素名の取得
				if(FAILED(pReader->GetLocalName(&pwszLocalName, NULL))){
					MessageBox(NULL, 
						_T("GetLocalName失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
					return 1;
				}

			break;

			//テキスト
			case XmlNodeType_Text:

				//テキストを取得
				if(FAILED(pReader->GetValue(&pwszValue, NULL))){
					MessageBox(NULL, 
						_T("GetValue失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
					return 1;
				}

				//構造体の確保
				dataXList = (xmlStList *)realloc(dataXList,sizeof(xmlStList) * i);


				//タグとの比較
				if(lstrcmp(pwszLocalName,_T("tag"))==0){

					lstrcpy(dataXList[i-1].xtag,pwszValue);

					i++;

				}else if(lstrcmp(pwszLocalName,_T("name"))==0){
					
					lstrcpy(dataXList[i-1].xname,pwszValue);

				}


			break;
        }

    }

	
	listMax = i-1;

   return 0;

}

/*----------------------------------------------------------*/
/*		           XMLへの更新読み込み
/*----------------------------------------------------------*/
int newWord(TCHAR *path){

	LPCWSTR pwszLocalName;
    LPCWSTR pwszValue;
    XmlNodeType nodeType;
	int i=1;

	//IXmlReaderインターフェイスの実装
    CComPtr<IXmlReader> pReader=NULL;
    if(FAILED(
        CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&pReader), 0))){
        MessageBox(NULL, 
            _T("CreateXmlReader失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//---- リーダーが入力として使用するストレージを指定する
	// ファイルストリーム作成
    CComPtr<IStream> pStream;
    if(FAILED(SHCreateStreamOnFile(path, STGM_READ, &pStream))){
        MessageBox(NULL, 
            _T("SHCreateStreamOnFile失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//ファイルストリームを関連付
	if(FAILED(pReader->SetInput(pStream))){
        MessageBox(NULL, 
            _T("SetInput失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }


	//ノードを読み込んでいく
    while(S_OK == pReader->Read(&nodeType)){

		switch(nodeType){

			//要素の開始
			case XmlNodeType_Element:

				//要素名の取得
				if(FAILED(pReader->GetLocalName(&pwszLocalName, NULL))){
					MessageBox(NULL, 
						_T("GetLocalName失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
					return 1;
				}

			break;

			//テキスト
			case XmlNodeType_Text:

				//テキストを取得
				if(FAILED(pReader->GetValue(&pwszValue, NULL))){
					MessageBox(NULL, 
						_T("GetValue失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
					return 1;
				}

				//タグとの比較
				if(lstrcmp(pwszLocalName,_T("tag"))==0){

					lstrcpy(dataXList[i-1].xtag,pwszValue);

					i++;

				}else if(lstrcmp(pwszLocalName,_T("name"))==0){
					
					lstrcpy(dataXList[i-1].xname,pwszValue);

				}


			break;
        }

    }

	
	listMax = i-1;

	return 0;
}

//解放処理
void freeList(){

	free(dataXList);

}


/*----------------------------------------------------------*/
/*		            フォーマット処理群	
/*----------------------------------------------------------*/


//--- いつ、どこで、誰と、何をしたの形による生成
void w4Format(TCHAR *path){

	//ワードは30文字まで

	//抽出対象
	TCHAR Now[][30] = {{TEXT("日")}};
	TCHAR Where[][30] = {{TEXT("場所")}};
	TCHAR Who[][30] = {
						{TEXT("人")},
						{TEXT("動物")},
						{TEXT("植物")},
						{TEXT("キャラクター")}
					  };

	TCHAR Do[][30] = {{TEXT("行動")}};

	TCHAR *List[] = {(TCHAR *)Now,(TCHAR *)Where,(TCHAR *)Who,(TCHAR *)Do};

	TCHAR keyList[4][30];
	TCHAR tmp[4][50];

	int i,n;

	for(i=0;i<4;i++){

		n = randInt(3);

		if(n){

			if(i==2){
				n = randInt(4);
				lstrcpy(keyList[i],Who[n]);
			}else{
				lstrcpy(keyList[i],List[i]);
			}
			
		}else{
			lstrcpy(keyList[i],TEXT("NULL"));
		}

	}

	
	//抽出対象を元にXMLからワードを取り出す
	for(i=0;i<4;i++){

		if(!readWord(keyList[i],path)){

			lstrcpy(tmp[i],Word);

		}else{
			lstrcpy(tmp[i],TEXT("NULL"));
		}


	}

   for(i=0;i<4;i++){

		if(lstrcmp(tmp[i],TEXT("NULL"))!=0){

			lstrcat(words,tmp[i]);

				if(i==0){

					if(lstrcmp(tmp[i+1],TEXT("NULL"))!=0 || lstrcmp(tmp[i+2],TEXT("NULL"))!=0 || lstrcmp(tmp[i+3],TEXT("NULL"))!=0){
						lstrcat(words,TEXT("は"));
					}

				}else if(i==1){

					if(lstrcmp(tmp[i+1],TEXT("NULL"))!=0 || lstrcmp(tmp[i+2],TEXT("NULL"))!=0){
						lstrcat(words,TEXT("で"));
					}

				}else if(i==2){
					if(lstrcmp(tmp[i+1],TEXT("NULL"))!=0){
						lstrcat(words,TEXT("と"));
					}
				}
		}
	}
	

}


//セリフによる生成

void OrWordFormat(TCHAR *path){

	readWord(TEXT("セリフ"),path);

	if(lstrlen(Word)){

		lstrcat(words,Word);
	}

}


//特定アクション時のセリフ生成

//---アプリケーションの起動時
void startAppWord(TCHAR *path){

	int e;

	if(e=readWord(TEXT("セリフ(起動時)"),path)){
		if(e==2){
			MessageBox(NULL,TEXT("COM ERROR"),NULL,MB_OK);
		}
	}

	if(lstrlen(Word)){

		lstrcpy(words,Word);
	}

}




/*----------------------------------------------------------*/
/*		     XMLデータ格納構造体からワードの抽出	
/*----------------------------------------------------------*/

int readWord(TCHAR *keyword,TCHAR *path){

	int tn=0,n=0,i=0,s=0;

	TCHAR List[100][50];

	//メモリのクリア
	ZeroMemory(List,sizeof(TCHAR));

	while(i<listMax){

		if(lstrcmp(dataXList[i].xtag,keyword)==0){
			lstrcpy(List[s++],dataXList[i].xname);
		}

		i++;

	}
	

	if(!lstrlen(List[0])){
		return 1;
	}

	//ワードを決定
	n=randInt(s);
	lstrcpy(Word,List[n]);


   return 0;

}



/*----------------------------------------------------------*/
/*		            XMLへの書き込み		
/*----------------------------------------------------------*/

int addWord(TCHAR *name,TCHAR *keyword,TCHAR *mpath){

	int i;
	TCHAR path[MAX_PATH];

	lstrcpy(path,mpath);
	lstrcat(path,TEXT("Words.xml"));

	//IXmlWriter インターフェイスの実装
    CComPtr<IXmlWriter> pWriter;
    if(FAILED(
        CreateXmlWriter(__uuidof(IXmlWriter), reinterpret_cast<void**>(&pWriter), 0))){
        MessageBox(NULL, 
            _T("CreateXmlWriter失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }


    // ファイルストリーム作成
    CComPtr<IStream> pStream;
    if(FAILED(SHCreateStreamOnFile(path, STGM_CREATE | STGM_WRITE, &pStream))){
        MessageBox(NULL, 
            _T("SHCreateStreamOnFile失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//ストリームと関連付ける
    if(FAILED(pWriter->SetOutput(pStream))){
        MessageBox(NULL, 
            _T("SetOutput失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

    //インデント有効化
    if(FAILED(pWriter->SetProperty(XmlWriterProperty_Indent, TRUE))){
        MessageBox(NULL, 
            _T("SetProperty失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	// <?xml version="1.0" encoding="UTF-8"?>
    if(FAILED(pWriter->WriteStartDocument(XmlStandalone_Omit))){
        MessageBox(NULL, 
            _T("WriteStartDocument失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//構築

    if(FAILED(pWriter->WriteStartElement(NULL, TEXT("words"), NULL))){
        MessageBox(NULL, 
            _T("WriteStartElement失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	i=0;
	while(i<listMax){

		if(FAILED(pWriter->WriteStartElement(NULL, TEXT("word"), NULL))){
			MessageBox(NULL, 
				_T("WriteStartElement失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
			return 1;
		}
		if(FAILED(pWriter->WriteElementString(NULL, TEXT("name"), NULL,dataXList[i].xname))){
			MessageBox(NULL, 
				_T("WriteElementString失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
			return 1;
		}
		if(FAILED(pWriter->WriteElementString(NULL, TEXT("tag"), NULL,dataXList[i].xtag))){
			MessageBox(NULL, 
				_T("WriteElementString失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
			return 1;
		}
		if(FAILED(pWriter->WriteFullEndElement())){
			MessageBox(NULL, 
				_T("WriteFullEndElement失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
			return 1;
		}

		i++;

	}

	//追加書き込み
	if(FAILED(pWriter->WriteStartElement(NULL, TEXT("word"), NULL))){
        MessageBox(NULL, 
            _T("WriteStartElement失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }
    if(FAILED(pWriter->WriteElementString(NULL, TEXT("name"), NULL,name))){
        MessageBox(NULL, 
            _T("WriteElementString失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }
    if(FAILED(pWriter->WriteElementString(NULL, TEXT("tag"), NULL,keyword))){
        MessageBox(NULL, 
            _T("WriteElementString失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }
	if(FAILED(pWriter->WriteFullEndElement())){
        MessageBox(NULL, 
            _T("WriteFullEndElement失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//構造体の更新
	dataXList = (xmlStList *)realloc(dataXList,sizeof(xmlStList) * (listMax+1));
	lstrcpy(dataXList[listMax].xname,name);
	lstrcpy(dataXList[listMax].xtag,keyword);
	listMax++;

	//閉じる
	if(FAILED(pWriter->WriteFullEndElement())){
        MessageBox(NULL, 
            _T("WriteFullEndElement失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }


	//終了
	if(FAILED(pWriter->Flush())){
        MessageBox(NULL, 
            _T("Flush失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

return 0;

}


/*----------------------------------------------------------*/
/*		            XMLへの削除処理	
/*----------------------------------------------------------*/

int delWord(int no,TCHAR *mpath){

	int i;
	TCHAR path[MAX_PATH];

	lstrcpy(path,mpath);
	lstrcat(path,TEXT("Words.xml"));

	//IXmlWriter インターフェイスの実装
    CComPtr<IXmlWriter> pWriter;
    if(FAILED(
        CreateXmlWriter(__uuidof(IXmlWriter), reinterpret_cast<void**>(&pWriter), 0))){
        MessageBox(NULL, 
            _T("CreateXmlWriter失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }


    // ファイルストリーム作成
    CComPtr<IStream> pStream;
    if(FAILED(SHCreateStreamOnFile(path, STGM_CREATE | STGM_WRITE, &pStream))){
        MessageBox(NULL, 
            _T("SHCreateStreamOnFile失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//ストリームと関連付ける
    if(FAILED(pWriter->SetOutput(pStream))){
        MessageBox(NULL, 
            _T("SetOutput失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

    //インデント有効化
    if(FAILED(pWriter->SetProperty(XmlWriterProperty_Indent, TRUE))){
        MessageBox(NULL, 
            _T("SetProperty失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	// <?xml version="1.0" encoding="UTF-8"?>
    if(FAILED(pWriter->WriteStartDocument(XmlStandalone_Omit))){
        MessageBox(NULL, 
            _T("WriteStartDocument失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//構築

    if(FAILED(pWriter->WriteStartElement(NULL, TEXT("words"), NULL))){
        MessageBox(NULL, 
            _T("WriteStartElement失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	i=0;
	while(i<listMax){

		if(no!=i){
			if(FAILED(pWriter->WriteStartElement(NULL, TEXT("word"), NULL))){
				MessageBox(NULL, 
					_T("WriteStartElement失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
				return 1;
			}
			if(FAILED(pWriter->WriteElementString(NULL, TEXT("name"), NULL,dataXList[i].xname))){
				MessageBox(NULL, 
					_T("WriteElementString失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
				return 1;
			}
			if(FAILED(pWriter->WriteElementString(NULL, TEXT("tag"), NULL,dataXList[i].xtag))){
				MessageBox(NULL, 
					_T("WriteElementString失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
				return 1;
			}
			if(FAILED(pWriter->WriteFullEndElement())){
				MessageBox(NULL, 
					_T("WriteFullEndElement失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
				return 1;
			}
		}

		i++;

	}

	//閉じる
	if(FAILED(pWriter->WriteFullEndElement())){
        MessageBox(NULL, 
            _T("WriteFullEndElement失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }


	//終了
	if(FAILED(pWriter->Flush())){
        MessageBox(NULL, 
            _T("Flush失敗"), _T("警告"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//構造体の更新
	if(newWord(path)){
		return 1;
	}

return 0;


}