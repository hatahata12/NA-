/*----------------------------------------------------------*/
/*			       �l�b�g���[�N�����Q
/*
/* WinSocket
/*----------------------------------------------------------*/

//�x���̗}��
#define _CRT_SECURE_NO_DEPRECATE

#include <winsock.h>
#include <windows.h>
#include <stdio.h>
#include "net.h"

#define MB 50000
char *hostname;
char *path;
int port;

char recvBuff[MB];
TCHAR rectData[MB];

int connectHttp(char *,char *,unsigned short);
int SetRssDatas(TCHAR *);
void SetTmpDatas(char *,char *);
int getRssIniData();
void endRssPosFunc();


/*---------------------------------------------------------*/
/*		               �R���g���[���[	
/*----------------------------------------------------------*/

int Controller(int mode,char *_hostname,char *_path,unsigned short _port){

	hostname = _hostname;
	path = _path;
	port = _port;

	switch(mode){

		//RSS�擾
		case 0:

			if(connectHttp(hostname,path,80)<0){

				if(SetRssDatas(rectData)){
					MessageBox(NULL,TEXT("���s"),NULL,MB_OK);
				}

				SetTmpDatas(hostname,path);
			}else{

				//��M���s����
				TCHAR *berror = TEXT("<?xml version=\"1.0\" encoding=\"utf-8\"?>")
									TEXT("<rss version=\"2.0\">")
											TEXT("<channel>")
												TEXT("<title>��M�G���[</title>")
												TEXT("<link>nothing</link>")
												TEXT("<item>")
														TEXT("<title>��M�G���[���ɂł܂��B�T�C�g�ɐڑ��ł��Ȃ��������A�������͓o�^URL�����m�F��������</title>")
														TEXT("<link>nothing</link>")
												TEXT("</item>")
											TEXT("/channel")
									TEXT("</rss>");

				lstrcpy(rectData,berror);

				SetRssDatas(rectData);
				SetTmpDatas(hostname,path);
			}

		break;

	}

return 0;

}


/*---------------------------------------------------------*/
/*		           �T�[�o�[�ւ̐ڑ�		
/*----------------------------------------------------------*/

int connectHttp(char *hostname,char *path,unsigned short port){


	WORD wVersionRequested;
	int  nErrorStatus;
	WSADATA wsaData;
	unsigned long serveraddr;				/* �T�[�o��IP�A�h���X */
	HOSTENT *serverhostent;					/* �T�[�o�̃z�X�g�����w���|�C���^ */
	int soc;								/* �\�P�b�g�iSoket Descriptor�j */
	sockaddr_in	 serversockaddr;
	int buf_len;
	char sendbuf[256];						/* ���M�o�b�t�@ */
	static int errorCounter=0;


	/* WinSock�̏��������s�� */
	/* �o�[�W���� 1.1 ��v������ */
	wVersionRequested = MAKEWORD(1,1);
	nErrorStatus = WSAStartup(wVersionRequested,&wsaData);
	/* �I������WinSock�̃��\�[�X���������悤�ɂ��Ă��� */
	if(atexit((void (*)(void))(WSACleanup))){	
		return 0;
	}
	if(nErrorStatus != 0){
		return 1;
	}

	/* soc�Ƀ\�P�b�g���쐬 */
	soc = socket(PF_INET, SOCK_STREAM, 0);
	if(soc == INVALID_SOCKET){
		return 2;
	}

	/** �T�[�o��IP�A�h���X�̎擾 **/
	serverhostent = gethostbyname(hostname);
	if (serverhostent == NULL) {
		return 3;
	}else{
		serveraddr = *((unsigned long *)((serverhostent->h_addr_list)[0]));
	}
	
	/*** �T�[�o�[�ɐڑ� ***/
	/* �T�[�o�̃A�h���X�̍\���̂ɃT�[�o��IP�A�h���X�ƃ|�[�g�ԍ���ݒ� */
	serversockaddr.sin_family	 = AF_INET;					/* �C���^�[�l�b�g�̏ꍇ */
	serversockaddr.sin_addr.s_addr  = serveraddr;			/* �T�[�o��IP�A�h���X */
	serversockaddr.sin_port	 = htons(port);					/* �|�[�g�ԍ� */

	memset(serversockaddr.sin_zero,(int)0,sizeof(serversockaddr.sin_zero));

	/* �T�[�o�֐ڑ� */
	if(connect(soc,(struct sockaddr *)&serversockaddr,sizeof(serversockaddr)) == SOCKET_ERROR){
		/* �\�P�b�g��j������ */
		closesocket(soc);
		return 4;
	}

	/* �T�[�o�� GET ���N�G�X�g�𑗂鏈�� */
	/* ���M����������̓T�[�o�ɓ͂��܂� */
	sprintf(sendbuf,"GET %s HTTP/1.0\r\nHost: %s:%d\r\nUser-Agent: httpc/0.0\r\n\r\n",path,hostname,port);	/* ���N�G�X�g�w�b�_���쐬���� */
	if(send(soc,sendbuf,strlen(sendbuf),0) == SOCKET_ERROR){
		/* ����M�𖳌��ɂ��� */
		shutdown(soc,2);
		/* �\�P�b�g��j������ */
		closesocket(soc);
		return 5;
	}


	/* �T�[�o����f�[�^����M���鏈�� */
	/* �T�[�o����f�[�^�����ׂĎ�M���I���܂ŌJ��Ԃ� */
					/* ��M�o�b�t�@ */
	while(1){
		/* �T�[�o����f�[�^����M���܂� */
		buf_len = recv(soc,recvBuff,MB - 1,0);	/* ��M���s�̏ꍇ */
		if(buf_len == SOCKET_ERROR){
			break;
		}else if(buf_len == 0){			/* ��M�I��� */
			break;
		}
		recvBuff[buf_len] = '\0';		/* ��M�o�b�t�@�̌���NULL��t������ */

	}

	//shutdown(soc,2);

	/* �\�P�b�g��j������ */
	closesocket(soc);

	/* ��M�`�F�b�N */
	if(recvBuff[0]!='H' || recvBuff[1]!='T' || recvBuff[2]!='T' || recvBuff[3]!='P'){

		if(errorCounter>100){

			//��M���s����
			TCHAR *berror = TEXT("<?xml version=\"1.0\" encoding=\"utf-8\"?>")
								TEXT("<rss version=\"2.0\">")
										TEXT("<channel>")
											TEXT("<title>��M�G���[</title>")
											TEXT("<link>nothing</link>")
											TEXT("<item>")
													TEXT("<title>��M�G���[���ɂł܂��B�T�C�g�ɐڑ��ł��Ȃ��������A�������͓o�^URL�����m�F��������</title>")
													TEXT("<link>nothing</link>")
											TEXT("</item>")
										TEXT("/channel")
								TEXT("</rss>");

			lstrcpy(rectData,berror);

			errorCounter=0;

			return -1;

		}
		
		errorCounter++;

		return connectHttp(hostname,path,port);

	}

	//�K�v�f�[�^�̒��o

	MultiByteToWideChar(CP_UTF8,0,recvBuff,sizeof(recvBuff),rectData,sizeof(rectData));

	int i=0,flag=0,j=0,n;
	TCHAR *tmp=TEXT("<");
	TCHAR *tmp2=TEXT("\0");

	n=lstrlen(rectData);
	while(i<10000){

		//�擾�����傫���Ȃ�΋�؂�
		if(i>=n){

			rectData[j] = *tmp2;

			break;

		}else{

			//�f�[�^���߂�
			if(rectData[i]==*tmp && !flag){
				flag=1;
			}

			if(flag){

				rectData[j] = rectData[i];
				j++;
				
			}
		}

		i++;

	}

return -1;

}