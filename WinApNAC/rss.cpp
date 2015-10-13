/*----------------------------------------------------------*/

/*			                RSS����

/*----------------------------------------------------------*/


#include <windows.h>

#include <atlbase.h>  //CComPtr���g�p���邽��
#include <atlstr.h>   //CString���g�p���邽��
#include <xmllite.h>  //xmllite


#define RSS_GET_MAX 31

/*----------------------------------------------------------*/
/*		               �v���g�^�C�v�錾		
/*----------------------------------------------------------*/

int getRssIniData();
int Controller(int mode,char *hostname,char *path,unsigned short port);
void urlDataEx(TCHAR *,char *,char *);
int rssDataAdd(TCHAR *,TCHAR *);
void SetListRssDataName(TCHAR *);
void okRssEx();
DWORD WINAPI ThreadFunc(LPVOID);
void StartTimer();

/*----------------------------------------------------------*/
/*		               �O���[�o���ϐ�	
/*----------------------------------------------------------*/

//----�\����

typedef struct rssList{

	TCHAR title[150];
	TCHAR link[MAX_PATH];

}rssMaDataList;


typedef struct list{

	TCHAR name[100];
	TCHAR url[MAX_PATH];
	char dm[50];
	char page[210];
	struct list *next;
	rssMaDataList DataList[RSS_GET_MAX];

}rssTarGetList;

typedef struct{

	TCHAR title[150];
	TCHAR link[MAX_PATH];

}tmpRssDataList;

TCHAR rpath[MAX_PATH];

tmpRssDataList tmpDataList[RSS_GET_MAX] = {0};

//�A�N�Z�X�|�C���^
rssTarGetList *head = NULL;
rssTarGetList *p;
rssTarGetList *stk=NULL;

int counter=1;


/*----------------------------------------------------------*/
/*		                  ������		
/*----------------------------------------------------------*/
void initRss(TCHAR *_path){

	int i=0;

	lstrcpy(rpath,_path);
	lstrcat(rpath,TEXT("rss.ini"));



}

TCHAR *GetRssIniPath(){
	return rpath;
}

/*----------------------------------------------------------*/
/*		            RSS�f�[�^�����ǂݍ���		
/*----------------------------------------------------------*/

void readRssData(){

	TCHAR nameBuff[MAX_PATH];
	TCHAR urlBuff[MAX_PATH];
	TCHAR buffs[30];

	int max,no,j,i;
	DWORD dwID;

	max = getRssIniData();

	if(max){
	

		for(i=0;i<=max;i++){

			for(j=0;j<=max;j++){

					wsprintf(buffs,TEXT("%s%d"),TEXT("Data"),j);
					no = GetPrivateProfileInt(buffs,TEXT("no"),-1,rpath);

					if(i==no){
					
						GetPrivateProfileString(buffs,TEXT("name"),TEXT("Nothing"),nameBuff,(DWORD)MAX_PATH,rpath);
						GetPrivateProfileString(buffs,TEXT("url"),TEXT("Nothing"),urlBuff,(DWORD)MAX_PATH,rpath);

						if(lstrcmp(nameBuff,TEXT("Nothing"))!=0){
							
							rssDataAdd(nameBuff,urlBuff);
							SetListRssDataName(nameBuff);

						}

					}

			}

		}

		
		CreateThread(NULL,0,ThreadFunc,0,0,&dwID);


	}

		


}

/*----------------------------------------------------------*/
/*		            �X�V		
/*----------------------------------------------------------*/

void updataRss(){

	DWORD dwID;
	int max;

	max = getRssIniData();

	if(max){
		CreateThread(NULL,0,ThreadFunc,0,0,&dwID);
	}

}

/*----------------------------------------------------------*/
/*		            �V�K�f�[�^��ǉ�����		
/*----------------------------------------------------------*/
int rssDataAdd(TCHAR *name,TCHAR *url){

	rssTarGetList *cpPoint;
	int i=0;

	p = (rssTarGetList *)malloc(sizeof(rssTarGetList));

	//�h���C���ƃy�[�W�𒊏o����
	urlDataEx(url,p->dm,p->page);

	lstrcpy(p->name,name);
	lstrcpy(p->url,url);

	p->next = NULL;

	while(i<RSS_GET_MAX){

		lstrcpy(p->DataList[i].title,TEXT(" "));
		lstrcpy(p->DataList[i].link,TEXT(" "));

		i++;
	}

	//�A�N�Z�X�|�C���^����T��
	if(head==NULL){
	
		head = p;

	}else{

		cpPoint = head;

		while(cpPoint->next!=NULL){
			cpPoint = cpPoint->next;
		}

		cpPoint->next = p;

	}

	return 0;

}


/*----------------------------------------------------------*/
/*		       �������t�@�C���̓o�^���𒲂ׂ�		
/*----------------------------------------------------------*/
int getRssIniData(){

	int no=0,counter=0;
	TCHAR *section = TEXT("Data");
	TCHAR buffs[30];
	TCHAR strBuff[100];

	while(1){
	
		wsprintf(buffs,TEXT("%s%d"),section,no);
		GetPrivateProfileString(buffs,TEXT("no"),TEXT("error"),strBuff,100,rpath);

		if(lstrcmp(strBuff,TEXT("-1"))!=0 && lstrcmp(strBuff,TEXT("error"))!=0){
			counter++;
		}else if(lstrcmp(strBuff,TEXT("error"))==0){
			break;
		}

		no++;

		
	}

	return counter;

}

/*----------------------------------------------------------*/
/*		      URL���X�g����f�[�^���擾�X���b�h
/*----------------------------------------------------------*/

DWORD WINAPI ThreadFunc(LPVOID vdParam){


	rssTarGetList *cpPoint;

	if(head!=NULL){

	cpPoint = head;

		do{

			Controller(0,cpPoint->dm,cpPoint->page,80);

			cpPoint = cpPoint->next;

		}while(cpPoint!=NULL);

	}

	StartTimer();

	return FALSE;
}


/*----------------------------------------------------------*/
/*		         �ꎞ�ۑ����烁�C���f�[�^��
/*----------------------------------------------------------*/

void SetTmpDatas(char *_hostname,char *_path){

	rssTarGetList *cpPoint;
	int i=0;

	if(head!=NULL){

	cpPoint = head;

		do{

			if(strcmp(_hostname,cpPoint->dm)==0 && strcmp(_path,cpPoint->page)==0){
				
				while(i<RSS_GET_MAX){
				
					lstrcpy(cpPoint->DataList[i].title,tmpDataList[i].title);
					lstrcpy(cpPoint->DataList[i].link,tmpDataList[i].link);
					
					i++;
				}

			}

			cpPoint = cpPoint->next;

		}while(cpPoint!=NULL);

		ZeroMemory(tmpDataList,sizeof(tmpRssDataList));

	}

}


/*----------------------------------------------------------*/
/*		      �^�C�g���̎擾
/*----------------------------------------------------------*/

void GetRssTitleData(TCHAR **title,TCHAR **topTitle,TCHAR **link){


	if(stk==NULL){
		stk = head;
		counter=1;
	}

	*title = stk->DataList[counter].title;
	*topTitle = stk->DataList[0].title;
	*link = stk->DataList[counter++].link;

	//�󔒃f�[�^�Ȃ�	
	if(lstrlen(*title)<3 || lstrlen(*topTitle)<3){

		stk = stk->next;
		counter=1;

		if(stk!=NULL){
				*title = stk->DataList[counter].title;
				*topTitle = stk->DataList[0].title;
				*link = stk->DataList[counter++].link;
		}else{

			stk = head;

			*title = stk->DataList[counter].title;
			*topTitle = stk->DataList[0].title;
			*link = stk->DataList[counter++].link;

		}


	}else{

		while(counter>=RSS_GET_MAX){
			stk = stk->next;
			counter=1;
		}
	}
	

}

void BackRssData(){

	stk = head;
	counter=1;

}


/*----------------------------------------------------------*/
/*		    �_�C�A���O�p�^�C�g���擾
/*----------------------------------------------------------*/

void RssSetName(TCHAR **setTitle,int no){

	rssTarGetList *cpPoint;
	int counter=0;

	if(head!=NULL){

	cpPoint = head;

		do{

			if(counter==no){

				*setTitle = cpPoint->name;

				break;

			}

			cpPoint = cpPoint->next;

			counter++;

		}while(cpPoint!=NULL);

	}

}

void UpdataRssDialogData(TCHAR *name,TCHAR *link,int no){

	rssTarGetList *cpPoint;
	int counter=0;

	if(head!=NULL){

	cpPoint = head;

		do{

			if(counter==no){

				lstrcpy(cpPoint->name,name);
				lstrcpy(cpPoint->url,link);

				break;

			}

			cpPoint = cpPoint->next;

			counter++;

		}while(cpPoint!=NULL);

	}

}

void DelRssDialogData(int no){

	rssTarGetList *cpPoint,*tmpPoint,*nextTmp;
	int counter=0;

	if(head!=NULL){

	cpPoint = head;
	nextTmp=NULL;

		do{

			if(counter==no){


				//��
				if(cpPoint==head && cpPoint->next==NULL){

					free(cpPoint);

					head = NULL;

				//����
				}else if(cpPoint==head && cpPoint->next!=NULL){

					tmpPoint = head;
					head = cpPoint->next;

					free(tmpPoint);


				//����
				}else if(nextTmp!=NULL && cpPoint->next==NULL){
				
					nextTmp->next = NULL;
					free(cpPoint);

				}else{

					nextTmp->next = cpPoint->next;

					free(cpPoint);


				}

				break;

			}

			nextTmp = cpPoint;
			cpPoint = cpPoint->next;

			counter++;

		}while(cpPoint!=NULL);

	}

}

/*----------------------------------------------------------*/
/*		      �_�C�A���O�p�����N�擾
/*----------------------------------------------------------*/

void RssSetLink(TCHAR **setLink,int no){

	rssTarGetList *cpPoint;
	int counter=0;

	if(head!=NULL){

	cpPoint = head;

		do{

			if(counter==no){

				*setLink = cpPoint->url;

				break;

			}

			cpPoint = cpPoint->next;

			counter++;

		}while(cpPoint!=NULL);

	}
}

/*----------------------------------------------------------*/
/*		              RSS�f�[�^�̐��`	
/*----------------------------------------------------------*/

int SetRssDatas(TCHAR *xml){

    LPCWSTR pwszLocalName;
    LPCWSTR pwszValue;
    XmlNodeType nodeType;

	int flag=0,i=1,fast=0;

	i=0;
	while(i<RSS_GET_MAX){

		lstrcpy(tmpDataList[i].title,TEXT(" "));
		lstrcpy(tmpDataList[i].link,TEXT(" "));

		i++;

	}
	i=1;

	//IXmlReader�C���^�[�t�F�C�X�̎���
    CComPtr<IXmlReader> pReader;
    if(FAILED(
        CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&pReader), 0))){
        MessageBox(NULL, 
            _T("CreateXmlReader���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }


	// XML�p���������蓖��
    HGLOBAL hXml = GlobalAlloc(GMEM_MOVEABLE, (lstrlen(xml) + 1) * sizeof(WCHAR));
    LPWSTR buf = reinterpret_cast<LPWSTR>(GlobalLock(hXml));
    lstrcpy(buf, xml);
    GlobalUnlock(hXml);

	CComPtr<IStream> pStream;
    if(FAILED(CreateStreamOnHGlobal(hXml, TRUE, &pStream))){
        MessageBox(NULL,
            _T("CreateStreamOnHGlobal���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	
    // �G���R�[�h�w��
    CComPtr<IXmlReaderInput> pReaderInput;
    if(FAILED(CreateXmlReaderInputWithEncodingName(pStream, NULL, 
        L"UTF-16", FALSE, NULL, &pReaderInput))){
        MessageBox(NULL, 
            _T("CreateXmlReaderInputWithEncodingName���s"), 
            _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }


    if(FAILED(pReader->SetInput(pReaderInput))){
        MessageBox(NULL, 
            _T("SetInput���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
        return 1;
    }

	//�K�v�ȃf�[�^�̒��o

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

				if(lstrcmp(pwszLocalName,_T("item"))==0){
					
					flag=1;

				}

			break;

			case XmlNodeType_Text:

				//�e�L�X�g���擾
				if(FAILED(pReader->GetValue(&pwszValue, NULL))){
					MessageBox(NULL, 
						_T("GetValue���s"), _T("�x��"), MB_OK | MB_ICONWARNING);
					return 1;
				}

				if(i>=RSS_GET_MAX){
					return 0;
				}

				//�^�O�Ƃ̔�r
				if(lstrcmp(pwszLocalName,_T("title"))==0){

					if(!fast){
						lstrcpy(tmpDataList[0].title,pwszValue);
	
					}

					if(flag){

						lstrcpy(tmpDataList[i].title,pwszValue);

					}

				}else if(lstrcmp(pwszLocalName,_T("link"))==0){

					if(!fast){

						lstrcpy(tmpDataList[0].link,pwszValue);
						fast=1;
					}

					if(flag){
						lstrcpy(tmpDataList[i].link,pwszValue);
						i++;
					}
				}

			break;

        }
	}

   return 0;

}

/*----------------------------------------------------------*/
/*		                 URL�̐��K��
/*----------------------------------------------------------*/
void urlDataEx(TCHAR *url,char *dm,char *page){

	int i=0,max=0,j=0;
	char tmpStr[1];

	max = lstrlen(url);


		while(i<max){

			//�h���C���ƃ����N�y�[�W�ł킯��
			if(url[i]=='/' && url[i+1]=='/'){
				i=i+2;
				while(url[i]!='/' && i<max){
					WideCharToMultiByte(CP_THREAD_ACP,0,&url[i],lstrlen(&url[i]),tmpStr,1,NULL,NULL);
					tmpStr[1]='\0';
					dm[j] = tmpStr[0]; 
					j++;
					i++;
				}

				dm[j]='\0';

				if(i<max){
					j=0;
					while(i<max){
						WideCharToMultiByte(CP_THREAD_ACP,0,&url[i],lstrlen(&url[i]),tmpStr,1,NULL,NULL);
						tmpStr[1]='\0';
						page[j] = tmpStr[0];
						j++;
						i++;
					}

					page[j]='\0';

				}else{
					page[0]='\0';
				}

				break;

			}
			
			i++;

		}



}

/*----------------------------------------------------------*/
/*		                     ���		
/*----------------------------------------------------------*/

void rssPointDataFree(){

rssTarGetList *tmp;

	if(head!=NULL){
		
		while(head->next!=NULL){
			tmp = head->next;
			free(head);
			head = tmp;
		}

		free(head);

	}

}