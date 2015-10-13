/*----------------------------------------------------------*/
/*			            �t�H�[�}�b�g�Q
/*
/*
/* ---- �t�H�[�}�b�g�̊g���͂����ōs��
/*----------------------------------------------------------*/

//�C���N���[�h
#include <windows.h>

/*---------------------------------------------------------*/
/*		           �v���g�^�C�v�錾			
/*----------------------------------------------------------*/

int readWord(TCHAR *,TCHAR *);
int randInt(int);


/*** �H�ו��t�H�[�}�b�g ***/
void GetfoodFormat(TCHAR *tag,TCHAR *path){
	readWord(tag,path);
}

void SetfoodFormat(TCHAR *Word){
	int no;

	no = randInt(5);

	switch(no){
	
		case 0:
			lstrcat(Word,TEXT("���������`��"));
		break;

		case 1:
			lstrcat(Word,TEXT("���ׂ��`����"));
		break;

		case 2:
			lstrcat(Word,TEXT("���ׂ���`��"));
		break;

		case 3:
			lstrcat(Word,TEXT("����������ɂ��ׂ����ˁ�"));
		break;

		case 4:
			lstrcat(Word,TEXT("�E�E�E�܂�����E�E�E�E"));
		break;
	}
}

/*** ���t�H�[�}�b�g ***/
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
			lstrcat(Word,TEXT("���킢���`��"));
		break;

		case 1:
			lstrcat(Word,TEXT("�ق����`��"));
		break;

		case 2:
			lstrcat(Word,TEXT("�ɂ������ȁ`�H"));
		break;

		case 3:
			lstrcat(Word,TEXT("�E�E�E�H"));
		break;

		case 4:
			lstrcat(tmp,TEXT("����"));
			lstrcat(tmp,Word);
			lstrcat(tmp,TEXT("�E�E�E�ǂ����ȁH"));
			lstrcpy(Word,tmp);
		break;
	}
}

/*** �@�B�t�H�[�}�b�g ***/
void GetMachineFormat(TCHAR *tag,TCHAR *path){
	readWord(tag,path);
}

void SetMachineFormat(TCHAR *Word){
	int no;
	TCHAR tmp[70];
	ZeroMemory(tmp,sizeof(TCHAR));

	no = randInt(5);

	//���蕶���񎞂̎d�l�T���v��
	if(lstrcmp(Word,TEXT("�p�\�R��"))==0){
		lstrcat(Word,TEXT("����ꂿ�Ⴆ�[�I"));
		no=-1;
	}

	switch(no){
	
		case 0:
			lstrcat(tmp,TEXT("����"));
			lstrcat(tmp,Word);
			lstrcat(tmp,TEXT("�E�E�E�����āH"));
			lstrcpy(Word,tmp);
		break;

		case 1:
			lstrcat(Word,TEXT("�ق����`��"));
		break;

		case 2:
			lstrcat(Word,TEXT("���킵���Ⴄ���`�H"));
		break;

		case 3:
			lstrcat(Word,TEXT("�E�E�E�H"));
		break;

		case 4:
			lstrcat(Word,TEXT("�n�C�e�N�ł��I"));
		break;
	}
}

/*** �{�t�H�[�}�b�g ***/
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
			lstrcat(tmp,TEXT("����"));
			lstrcat(tmp,Word);
			lstrcat(tmp,TEXT("���āA�������낢�ˁ`��"));
			lstrcpy(Word,tmp);
		break;

		case 1:
			lstrcat(Word,TEXT("�Â��́`�H"));
		break;

		case 2:
			lstrcat(Word,TEXT("���₵���Ⴄ���`�H"));
		break;

		case 3:
			lstrcat(Word,TEXT("�E�E�E�H"));
		break;

		case 4:
			lstrcat(Word,TEXT("�E�E�E���񂶂������E�E�E"));
		break;
	}
}

/*** �A�N�Z�T���[�t�H�[�}�b�g ***/
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
			lstrcat(Word,TEXT("���킢���`��"));
		break;

		case 1:
			lstrcat(Word,TEXT("�ق����`��"));
		break;

		case 2:
			lstrcat(Word,TEXT("�ɂ������ȁ`�H"));
		break;

		case 3:
			lstrcat(Word,TEXT("�E�E�E�H"));
		break;
			
		case 4:
			lstrcat(Word,TEXT("���炫��`"));
		break;
	}
}

/*** ���ݕ��t�H�[�}�b�g ***/
void GetDrinkFormat(TCHAR *tag,TCHAR *path){
	readWord(tag,path);
}

void SetDrinkFormat(TCHAR *Word){
	int no;

	no = randInt(5);

	switch(no){
	
		case 0:
			lstrcat(Word,TEXT("���������`��"));
		break;

		case 1:
			lstrcat(Word,TEXT("�݂̂��`����"));
		break;

		case 2:
			lstrcat(Word,TEXT("�̂񂾂�`��"));
		break;

		case 3:
			lstrcat(Word,TEXT("����������ɂ݂̂����ˁ�"));
		break;

		case 4:
			lstrcat(Word,TEXT("�E�E�E�܂�����E�E�E�E"));
		break;
	}
}