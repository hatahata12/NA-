/*----------------------------------------------------------*/
/*			          プロトタイプ宣言			
/*----------------------------------------------------------*/

void initRss(TCHAR *);
int rssDataAdd(TCHAR *,TCHAR *);
void rssPointDataFree();
void GetRssData();
int getRssIniData();
void readRssData();
TCHAR *GetRssIniPath();
void GetRssTitleData(TCHAR **,TCHAR **,TCHAR **);
void RssSetName(TCHAR **,int);
void RssSetLink(TCHAR **,int);
void updataRss();
void UpdataRssDialogData(TCHAR *,TCHAR *,int);
void DelRssDialogData(int no);
void BackRssData();