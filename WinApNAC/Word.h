#define KEYMAX 17

TCHAR keyword[][30] = { {TEXT("人")},
						{TEXT("キャラクター")},
						{TEXT("動物")},
						{TEXT("植物")},
						{TEXT("アニメ")},
						{TEXT("日")},
						{TEXT("場所")},
						{TEXT("行動")},
						{TEXT("食べ物")},
						{TEXT("飲み物")},
						{TEXT("機械")},
						{TEXT("服")},
						{TEXT("アクセサリー")},
						{TEXT("本")},
						{TEXT("曲")},
						{TEXT("セリフ")},
						{TEXT("セリフ(起動時)")}
					  };


typedef struct{

	TCHAR xname[30];
	TCHAR xtag[30];

}xmlStList;