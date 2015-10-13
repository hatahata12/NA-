/*----------------------------------------------------------*/
/*			       ネットワーク処理群
/*
/* WinSocket
/*----------------------------------------------------------*/

//警告の抑制
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
/*		               コントローラー	
/*----------------------------------------------------------*/

int Controller(int mode,char *_hostname,char *_path,unsigned short _port){

	hostname = _hostname;
	path = _path;
	port = _port;

	switch(mode){

		//RSS取得
		case 0:

			if(connectHttp(hostname,path,80)<0){

				if(SetRssDatas(rectData)){
					MessageBox(NULL,TEXT("失敗"),NULL,MB_OK);
				}

				SetTmpDatas(hostname,path);
			}else{

				//受信失敗処理
				TCHAR *berror = TEXT("<?xml version=\"1.0\" encoding=\"utf-8\"?>")
									TEXT("<rss version=\"2.0\">")
											TEXT("<channel>")
												TEXT("<title>受信エラー</title>")
												TEXT("<link>nothing</link>")
												TEXT("<item>")
														TEXT("<title>受信エラー時にでます。サイトに接続できなかったか、もしくは登録URLをご確認ください</title>")
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
/*		           サーバーへの接続		
/*----------------------------------------------------------*/

int connectHttp(char *hostname,char *path,unsigned short port){


	WORD wVersionRequested;
	int  nErrorStatus;
	WSADATA wsaData;
	unsigned long serveraddr;				/* サーバのIPアドレス */
	HOSTENT *serverhostent;					/* サーバのホスト情報を指すポインタ */
	int soc;								/* ソケット（Soket Descriptor） */
	sockaddr_in	 serversockaddr;
	int buf_len;
	char sendbuf[256];						/* 送信バッファ */
	static int errorCounter=0;


	/* WinSockの初期化を行う */
	/* バージョン 1.1 を要求する */
	wVersionRequested = MAKEWORD(1,1);
	nErrorStatus = WSAStartup(wVersionRequested,&wsaData);
	/* 終了時にWinSockのリソースを解放するようにしておく */
	if(atexit((void (*)(void))(WSACleanup))){	
		return 0;
	}
	if(nErrorStatus != 0){
		return 1;
	}

	/* socにソケットを作成 */
	soc = socket(PF_INET, SOCK_STREAM, 0);
	if(soc == INVALID_SOCKET){
		return 2;
	}

	/** サーバのIPアドレスの取得 **/
	serverhostent = gethostbyname(hostname);
	if (serverhostent == NULL) {
		return 3;
	}else{
		serveraddr = *((unsigned long *)((serverhostent->h_addr_list)[0]));
	}
	
	/*** サーバーに接続 ***/
	/* サーバのアドレスの構造体にサーバのIPアドレスとポート番号を設定 */
	serversockaddr.sin_family	 = AF_INET;					/* インターネットの場合 */
	serversockaddr.sin_addr.s_addr  = serveraddr;			/* サーバのIPアドレス */
	serversockaddr.sin_port	 = htons(port);					/* ポート番号 */

	memset(serversockaddr.sin_zero,(int)0,sizeof(serversockaddr.sin_zero));

	/* サーバへ接続 */
	if(connect(soc,(struct sockaddr *)&serversockaddr,sizeof(serversockaddr)) == SOCKET_ERROR){
		/* ソケットを破棄する */
		closesocket(soc);
		return 4;
	}

	/* サーバに GET リクエストを送る処理 */
	/* 送信した文字列はサーバに届きます */
	sprintf(sendbuf,"GET %s HTTP/1.0\r\nHost: %s:%d\r\nUser-Agent: httpc/0.0\r\n\r\n",path,hostname,port);	/* リクエストヘッダを作成する */
	if(send(soc,sendbuf,strlen(sendbuf),0) == SOCKET_ERROR){
		/* 送受信を無効にする */
		shutdown(soc,2);
		/* ソケットを破棄する */
		closesocket(soc);
		return 5;
	}


	/* サーバからデータを受信する処理 */
	/* サーバからデータをすべて受信し終わるまで繰り返す */
					/* 受信バッファ */
	while(1){
		/* サーバからデータを受信します */
		buf_len = recv(soc,recvBuff,MB - 1,0);	/* 受信失敗の場合 */
		if(buf_len == SOCKET_ERROR){
			break;
		}else if(buf_len == 0){			/* 受信終わり */
			break;
		}
		recvBuff[buf_len] = '\0';		/* 受信バッファの後ろにNULLを付加する */

	}

	//shutdown(soc,2);

	/* ソケットを破棄する */
	closesocket(soc);

	/* 受信チェック */
	if(recvBuff[0]!='H' || recvBuff[1]!='T' || recvBuff[2]!='T' || recvBuff[3]!='P'){

		if(errorCounter>100){

			//受信失敗処理
			TCHAR *berror = TEXT("<?xml version=\"1.0\" encoding=\"utf-8\"?>")
								TEXT("<rss version=\"2.0\">")
										TEXT("<channel>")
											TEXT("<title>受信エラー</title>")
											TEXT("<link>nothing</link>")
											TEXT("<item>")
													TEXT("<title>受信エラー時にでます。サイトに接続できなかったか、もしくは登録URLをご確認ください</title>")
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

	//必要データの抽出

	MultiByteToWideChar(CP_UTF8,0,recvBuff,sizeof(recvBuff),rectData,sizeof(rectData));

	int i=0,flag=0,j=0,n;
	TCHAR *tmp=TEXT("<");
	TCHAR *tmp2=TEXT("\0");

	n=lstrlen(rectData);
	while(i<10000){

		//取得数より大きいならば区切る
		if(i>=n){

			rectData[j] = *tmp2;

			break;

		}else{

			//データをつめる
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