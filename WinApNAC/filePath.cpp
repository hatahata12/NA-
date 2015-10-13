
/*----------------------------------------------------------*/
/*			    ファイルパスの正規化			
/*----------------------------------------------------------*/

#include <windows.h>

void filePath(TCHAR *path){

	int len=0,i;
	TCHAR *tmp;

	tmp = path;
	lstrcat((LPWSTR)path,TEXT("\0"));
	while(*path!='\0'){
		if(*path=='\\'){
			*path = '/';
		}
		len++;
	path++;
	}

	path=tmp;
	path+=len-1;
	
	i=0;
	while(i<len-1){

		if(*path=='/'){
			path++;
			*path = '\0';
			break;
		}
		i++;
		path--;
	}

}


void exfilePath(TCHAR *path){

	int len=0;
	TCHAR *tmp;

	tmp = path;
	lstrcat((LPWSTR)path,TEXT("\0"));
	while(*path!='\0'){
		if(*path=='\\'){
			*path = '/';
		}
		len++;
	path++;
	}

	path=tmp;

}
