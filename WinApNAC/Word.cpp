/*----------------------------------------------------------*/
/*			            ���[�h��������
/*
/*
/* ���C���Ƌ��L���Ă���words�ϐ�(TCHAR)�ɕ������R�s�[
/*
/* ---- �t�H�[�}�b�g�̊g���͂����ōs��
/*----------------------------------------------------------*/

#include <windows.h>
#include <shlwapi.h>
#include <stdlib.h>
#include <time.h>
#include <atlbase.h>  //CComPtr���g�p���邽��
#include <atlstr.h>   //CString���g�p���邽��
#include <xmllite.h>  //xmllite
#include "zxor.h"	  //��������
#include "Word.h"
#include "wordExtrn.h"
#include "resource.h"
#include "formatWord.h"

/*---------------------------------------------------------*/
/*		           �v���g�^�C�v�錾			
/*----------------------------------------------------------*/

void w4Format(TCHAR *);
void OrWordFormat(TCHAR *);
int readWord(TCHAR *,TCHAR *);
void startAppWord(TCHAR *);
int initWord(TCHAR *);
void freeList();
int newWord(TCHAR *);

/*----------------------------------------------------------*/
/*		            �O���[�o���ϐ�			
/*----------------------------------------------------------*/

TCHAR Word[50];
xmlStList *dataXList;  //�擾�^�[�Q�b�g�ɂȂ�\���̃|�C���^
int listMax=0;



/*----------------------------------------------------------*/
/*		         �����_���̐��l�𐶐�			
/*----------------------------------------------------------*/

int randInt(int x){

	int n;
	
	n = NextIntEx(x);

	return n; 

}


/*----------------------------------------------------------*/
/*		        �^�O���[�h�����X�g�ɍX�V		
/*----------------------------------------------------------*/

void createTagList(HWND hWnd){

	int i=0;

	while(i<KEYMAX){

		SendMessage(GetDlgItem(hWnd,IDC_LIST1),LB_ADDSTRING,0,(LPARAM )(LPCTSTR )keyword[i]);

		i++;
	}

}

/*----------------------------------------------------------*/
/*		        �f���[�g���[�h���X�g�ɍX�V		
/*----------------------------------------------------------*/

void DelcreateWordList(HWND hWnd){

	int i=0;

	while(i<listMax){

		SendMessage(GetDlgItem(hWnd,IDC_LIST1),LB_ADDSTRING,0,(LPARAM )(LPCTSTR )dataXList[i].xname);

		i++;
	}


}

/*----------------------------------------------------------*/
/*		     �f���[�g���[�h(ListBox)�̃^�O����\��		
/*----------------------------------------------------------*/

void DelSetTagName(HWND hWnd,int n){

	SetDlgItemText(hWnd,IDC_STATICS,dataXList[n].xtag); 

}

/*----------------------------------------------------------*/
/*		         ���[�h�������C������	
/*----------------------------------------------------------*/

int createOTMessage(TCHAR *mpath,int vp){


	int format=0;
	TCHAR path[MAX_PATH];

	lstrcpy(path,mpath);
	lstrcat(path,TEXT("Words.xml"));

	ZeroMemory(words,sizeof(TCHAR));
	ZeroMemory(Word,sizeof(TCHAR));


	//��������t�����[�h�Ȃ�
	switch(vp){

		//�N�����̃��[�h����
		case 1:
			startAppWord(path);
		break;

		//������
		case 2:
			if(initWord(path)){
				return 1;
			}
		break;
		
		//���
		case 3:

			freeList();

		break;

		case 0:

			//�t�H�[�}�b�g�̌���
			format = randInt(8);
	
			//�t�H�[�}�b�g���Ƃ̏���

	        //---------  �g���Ώ� -------------//

			switch(format){

				// 0 ---- ���A�ǂ��ŁA�N�ƁA�Ȃɂ�����
				case 0:
					w4Format(path);
				break;
					
				// 1 ---- �Z���t
				case 1:
					OrWordFormat(path);
				break;

				// 2 ---- �H�ו�
				case 2:
					GetfoodFormat(TEXT("�H�ו�"),path);

					if(lstrlen(Word)){
						SetfoodFormat(Word);
						lstrcpy(words,Word);
					}
				break;

				// 3 ---- ��
				case 3:
					GetClothesFormat(TEXT("��"),path);

					if(lstrlen(Word)){
						SetClothesFormat(Word);
						lstrcpy(words,Word);
					}
				break;

				// 4 ---- �@�B
				case 4:
					GetMachineFormat(TEXT("�@�B"),path);

					if(lstrlen(Word)){
						SetMachineFormat(Word);
						lstrcpy(words,Word);
					}
				break;

				// 5 ---- �{
				case 5:
					GetBookFormat(TEXT("�{"),path);

					if(lstrlen(Word)){
						SetBookFormat(Word);
						lstrcpy(words,Word);
					}
				break;

				// 6 ---- �A�N�Z�T���[
				case 6:
					GetAccessoryFormat(TEXT("�A�N�Z�T���["),path);

					if(lstrlen(Word)){
						SetAccessoryFormat(Word);
						lstrcpy(words,Word);
					}
				break;
				
				// 7 ---- ���ݕ�
				case 7:
					GetDrinkFormat(TEXT("���ݕ�"),path);

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
/*		           XML�ւ̏����ǂݍ���	
/*----------------------------------------------------------*/

int initWord(TCHAR *path){

    LPCWSTR pwszLocalName;
    LPCWSTR pwszValue;
    XmlNodeType nodeType;
	int i=1;

	InitMt((int)time(NULL));

	//IXmlReader�C���^�[�t�F�C�X�̎���
    CComPtr<IXmlReader> pReader=NULL;
    if(FAILED(
        CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&pReader), 0))){
        MessageBox(NULL, 
            _T("CreateXmlReader���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//---- ���[�_�[�����͂Ƃ��Ďg�p����X�g���[�W���w�肷��
	// �t�@�C���X�g���[���쐬
    CComPtr<IStream> pStream;
    if(FAILED(SHCreateStreamOnFile(path, STGM_READ, &pStream))){
        MessageBox(NULL, 
            _T("SHCreateStreamOnFile���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//�t�@�C���X�g���[�����֘A�t
	if(FAILED(pReader->SetInput(pStream))){
        MessageBox(NULL, 
            _T("SetInput���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }


	//�m�[�h��ǂݍ���ł���
    while(S_OK == pReader->Read(&nodeType)){

		switch(nodeType){

			//�v�f�̊J�n
			case XmlNodeType_Element:

				//�v�f���̎擾
				if(FAILED(pReader->GetLocalName(&pwszLocalName, NULL))){
					MessageBox(NULL, 
						_T("GetLocalName���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
					return 1;
				}

			break;

			//�e�L�X�g
			case XmlNodeType_Text:

				//�e�L�X�g���擾
				if(FAILED(pReader->GetValue(&pwszValue, NULL))){
					MessageBox(NULL, 
						_T("GetValue���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
					return 1;
				}

				//�\���̂̊m��
				dataXList = (xmlStList *)realloc(dataXList,sizeof(xmlStList) * i);


				//�^�O�Ƃ̔�r
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
/*		           XML�ւ̍X�V�ǂݍ���
/*----------------------------------------------------------*/
int newWord(TCHAR *path){

	LPCWSTR pwszLocalName;
    LPCWSTR pwszValue;
    XmlNodeType nodeType;
	int i=1;

	//IXmlReader�C���^�[�t�F�C�X�̎���
    CComPtr<IXmlReader> pReader=NULL;
    if(FAILED(
        CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&pReader), 0))){
        MessageBox(NULL, 
            _T("CreateXmlReader���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//---- ���[�_�[�����͂Ƃ��Ďg�p����X�g���[�W���w�肷��
	// �t�@�C���X�g���[���쐬
    CComPtr<IStream> pStream;
    if(FAILED(SHCreateStreamOnFile(path, STGM_READ, &pStream))){
        MessageBox(NULL, 
            _T("SHCreateStreamOnFile���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//�t�@�C���X�g���[�����֘A�t
	if(FAILED(pReader->SetInput(pStream))){
        MessageBox(NULL, 
            _T("SetInput���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }


	//�m�[�h��ǂݍ���ł���
    while(S_OK == pReader->Read(&nodeType)){

		switch(nodeType){

			//�v�f�̊J�n
			case XmlNodeType_Element:

				//�v�f���̎擾
				if(FAILED(pReader->GetLocalName(&pwszLocalName, NULL))){
					MessageBox(NULL, 
						_T("GetLocalName���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
					return 1;
				}

			break;

			//�e�L�X�g
			case XmlNodeType_Text:

				//�e�L�X�g���擾
				if(FAILED(pReader->GetValue(&pwszValue, NULL))){
					MessageBox(NULL, 
						_T("GetValue���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
					return 1;
				}

				//�^�O�Ƃ̔�r
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

//�������
void freeList(){

	free(dataXList);

}


/*----------------------------------------------------------*/
/*		            �t�H�[�}�b�g�����Q	
/*----------------------------------------------------------*/


//--- ���A�ǂ��ŁA�N�ƁA���������̌`�ɂ�鐶��
void w4Format(TCHAR *path){

	//���[�h��30�����܂�

	//���o�Ώ�
	TCHAR Now[][30] = {{TEXT("��")}};
	TCHAR Where[][30] = {{TEXT("�ꏊ")}};
	TCHAR Who[][30] = {
						{TEXT("�l")},
						{TEXT("����")},
						{TEXT("�A��")},
						{TEXT("�L�����N�^�[")}
					  };

	TCHAR Do[][30] = {{TEXT("�s��")}};

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

	
	//���o�Ώۂ�����XML���烏�[�h�����o��
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
						lstrcat(words,TEXT("��"));
					}

				}else if(i==1){

					if(lstrcmp(tmp[i+1],TEXT("NULL"))!=0 || lstrcmp(tmp[i+2],TEXT("NULL"))!=0){
						lstrcat(words,TEXT("��"));
					}

				}else if(i==2){
					if(lstrcmp(tmp[i+1],TEXT("NULL"))!=0){
						lstrcat(words,TEXT("��"));
					}
				}
		}
	}
	

}


//�Z���t�ɂ�鐶��

void OrWordFormat(TCHAR *path){

	readWord(TEXT("�Z���t"),path);

	if(lstrlen(Word)){

		lstrcat(words,Word);
	}

}


//����A�N�V�������̃Z���t����

//---�A�v���P�[�V�����̋N����
void startAppWord(TCHAR *path){

	int e;

	if(e=readWord(TEXT("�Z���t(�N����)"),path)){
		if(e==2){
			MessageBox(NULL,TEXT("COM ERROR"),NULL,MB_OK);
		}
	}

	if(lstrlen(Word)){

		lstrcpy(words,Word);
	}

}




/*----------------------------------------------------------*/
/*		     XML�f�[�^�i�[�\���̂��烏�[�h�̒��o	
/*----------------------------------------------------------*/

int readWord(TCHAR *keyword,TCHAR *path){

	int tn=0,n=0,i=0,s=0;

	TCHAR List[100][50];

	//�������̃N���A
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

	//���[�h������
	n=randInt(s);
	lstrcpy(Word,List[n]);


   return 0;

}



/*----------------------------------------------------------*/
/*		            XML�ւ̏�������		
/*----------------------------------------------------------*/

int addWord(TCHAR *name,TCHAR *keyword,TCHAR *mpath){

	int i;
	TCHAR path[MAX_PATH];

	lstrcpy(path,mpath);
	lstrcat(path,TEXT("Words.xml"));

	//IXmlWriter �C���^�[�t�F�C�X�̎���
    CComPtr<IXmlWriter> pWriter;
    if(FAILED(
        CreateXmlWriter(__uuidof(IXmlWriter), reinterpret_cast<void**>(&pWriter), 0))){
        MessageBox(NULL, 
            _T("CreateXmlWriter���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }


    // �t�@�C���X�g���[���쐬
    CComPtr<IStream> pStream;
    if(FAILED(SHCreateStreamOnFile(path, STGM_CREATE | STGM_WRITE, &pStream))){
        MessageBox(NULL, 
            _T("SHCreateStreamOnFile���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//�X�g���[���Ɗ֘A�t����
    if(FAILED(pWriter->SetOutput(pStream))){
        MessageBox(NULL, 
            _T("SetOutput���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

    //�C���f���g�L����
    if(FAILED(pWriter->SetProperty(XmlWriterProperty_Indent, TRUE))){
        MessageBox(NULL, 
            _T("SetProperty���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	// <?xml version="1.0" encoding="UTF-8"?>
    if(FAILED(pWriter->WriteStartDocument(XmlStandalone_Omit))){
        MessageBox(NULL, 
            _T("WriteStartDocument���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//�\�z

    if(FAILED(pWriter->WriteStartElement(NULL, TEXT("words"), NULL))){
        MessageBox(NULL, 
            _T("WriteStartElement���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	i=0;
	while(i<listMax){

		if(FAILED(pWriter->WriteStartElement(NULL, TEXT("word"), NULL))){
			MessageBox(NULL, 
				_T("WriteStartElement���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
			return 1;
		}
		if(FAILED(pWriter->WriteElementString(NULL, TEXT("name"), NULL,dataXList[i].xname))){
			MessageBox(NULL, 
				_T("WriteElementString���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
			return 1;
		}
		if(FAILED(pWriter->WriteElementString(NULL, TEXT("tag"), NULL,dataXList[i].xtag))){
			MessageBox(NULL, 
				_T("WriteElementString���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
			return 1;
		}
		if(FAILED(pWriter->WriteFullEndElement())){
			MessageBox(NULL, 
				_T("WriteFullEndElement���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
			return 1;
		}

		i++;

	}

	//�ǉ���������
	if(FAILED(pWriter->WriteStartElement(NULL, TEXT("word"), NULL))){
        MessageBox(NULL, 
            _T("WriteStartElement���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }
    if(FAILED(pWriter->WriteElementString(NULL, TEXT("name"), NULL,name))){
        MessageBox(NULL, 
            _T("WriteElementString���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }
    if(FAILED(pWriter->WriteElementString(NULL, TEXT("tag"), NULL,keyword))){
        MessageBox(NULL, 
            _T("WriteElementString���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }
	if(FAILED(pWriter->WriteFullEndElement())){
        MessageBox(NULL, 
            _T("WriteFullEndElement���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//�\���̂̍X�V
	dataXList = (xmlStList *)realloc(dataXList,sizeof(xmlStList) * (listMax+1));
	lstrcpy(dataXList[listMax].xname,name);
	lstrcpy(dataXList[listMax].xtag,keyword);
	listMax++;

	//����
	if(FAILED(pWriter->WriteFullEndElement())){
        MessageBox(NULL, 
            _T("WriteFullEndElement���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }


	//�I��
	if(FAILED(pWriter->Flush())){
        MessageBox(NULL, 
            _T("Flush���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

return 0;

}


/*----------------------------------------------------------*/
/*		            XML�ւ̍폜����	
/*----------------------------------------------------------*/

int delWord(int no,TCHAR *mpath){

	int i;
	TCHAR path[MAX_PATH];

	lstrcpy(path,mpath);
	lstrcat(path,TEXT("Words.xml"));

	//IXmlWriter �C���^�[�t�F�C�X�̎���
    CComPtr<IXmlWriter> pWriter;
    if(FAILED(
        CreateXmlWriter(__uuidof(IXmlWriter), reinterpret_cast<void**>(&pWriter), 0))){
        MessageBox(NULL, 
            _T("CreateXmlWriter���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }


    // �t�@�C���X�g���[���쐬
    CComPtr<IStream> pStream;
    if(FAILED(SHCreateStreamOnFile(path, STGM_CREATE | STGM_WRITE, &pStream))){
        MessageBox(NULL, 
            _T("SHCreateStreamOnFile���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//�X�g���[���Ɗ֘A�t����
    if(FAILED(pWriter->SetOutput(pStream))){
        MessageBox(NULL, 
            _T("SetOutput���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

    //�C���f���g�L����
    if(FAILED(pWriter->SetProperty(XmlWriterProperty_Indent, TRUE))){
        MessageBox(NULL, 
            _T("SetProperty���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	// <?xml version="1.0" encoding="UTF-8"?>
    if(FAILED(pWriter->WriteStartDocument(XmlStandalone_Omit))){
        MessageBox(NULL, 
            _T("WriteStartDocument���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//�\�z

    if(FAILED(pWriter->WriteStartElement(NULL, TEXT("words"), NULL))){
        MessageBox(NULL, 
            _T("WriteStartElement���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	i=0;
	while(i<listMax){

		if(no!=i){
			if(FAILED(pWriter->WriteStartElement(NULL, TEXT("word"), NULL))){
				MessageBox(NULL, 
					_T("WriteStartElement���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
				return 1;
			}
			if(FAILED(pWriter->WriteElementString(NULL, TEXT("name"), NULL,dataXList[i].xname))){
				MessageBox(NULL, 
					_T("WriteElementString���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
				return 1;
			}
			if(FAILED(pWriter->WriteElementString(NULL, TEXT("tag"), NULL,dataXList[i].xtag))){
				MessageBox(NULL, 
					_T("WriteElementString���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
				return 1;
			}
			if(FAILED(pWriter->WriteFullEndElement())){
				MessageBox(NULL, 
					_T("WriteFullEndElement���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
				return 1;
			}
		}

		i++;

	}

	//����
	if(FAILED(pWriter->WriteFullEndElement())){
        MessageBox(NULL, 
            _T("WriteFullEndElement���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }


	//�I��
	if(FAILED(pWriter->Flush())){
        MessageBox(NULL, 
            _T("Flush���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//�\���̂̍X�V
	if(newWord(path)){
		return 1;
	}

return 0;


}