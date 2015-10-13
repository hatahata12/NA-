#include "pl.h"


// PNGファイルをメモリから読み込む
HBITMAP LoadPngFromFile(HWND hWnd, FILE *fp, unsigned &uWidth, unsigned &uHeight)
{
    // 引数チェック
    if (!fp) {
        return NULL; // エラーリターン
    }

    // PNG読み込み開始(メモリ確保・初期化)
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return NULL; // エラーリターン
    }

    // png_info 構造体の初期化（画像データの前にあるチャンク用）
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return NULL; // エラーリターン
    }

    // png_info 構造体の初期化（画像データの後にあるチャンク用）[本プログラムでは不要な部分です]
    png_info *end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL; // エラーリターン
    }

    // ファイルポインタを伝える
    png_init_io(png_ptr, fp);

    // 画像データ読み込み
    png_uint_32 nWidth, nHeight;
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    // 画像情報を取得
    nWidth = info_ptr->width;
    nHeight = info_ptr->height;

    // ビットマップハンドルの作成
    BITMAPV5HEADER bi;
    void *lpBits;

    ZeroMemory(&bi,sizeof(BITMAPV5HEADER));
    bi.bV5Size        = sizeof(BITMAPV5HEADER);
    bi.bV5Width       = nWidth;
    bi.bV5Height      = nHeight;
    bi.bV5Planes      = 1;
    bi.bV5BitCount    = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5RedMask     = 0x00FF0000;
    bi.bV5GreenMask   = 0x0000FF00;
    bi.bV5BlueMask    = 0x000000FF;
    bi.bV5AlphaMask   = 0xFF000000; 

    HDC hdc = GetDC(hWnd);
    HBITMAP hbmp = CreateDIBSection(hdc, (BITMAPINFO *)&bi, DIB_RGB_COLORS, 
        (void **)&lpBits, NULL, (DWORD)0);
    ReleaseDC(hWnd, hdc);
    DWORD *lpdwPixel = (DWORD *)lpBits;
    DWORD x, y;
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    // 画像データを順次読み出し
    for (x=0; x<nWidth; ++x) {
        for (y=0; y<nHeight; ++y) { // RGBA→ARGB 変換
            DWORD r     = row_pointers[y][4*x  ];
            DWORD g     = row_pointers[y][4*x+1];
            DWORD b     = row_pointers[y][4*x+2];
            DWORD alpha = row_pointers[y][4*x+3];

            // [↓あえて冗長に作ってあります]
            if (alpha == 255) {
                lpdwPixel[(nHeight-y-1)*nWidth+x] = (r << 16) | (g << 8) | (b << 0) | (alpha << 24);
            }
            else if (alpha == 0) {
                lpdwPixel[(nHeight-y-1)*nWidth+x] = 0;
            }
            else {
                r = r * alpha / 255;
                g = g * alpha / 255;
                b = b * alpha / 255;
                lpdwPixel[(nHeight-y-1)*nWidth+x] = (r << 16) | (g << 8) | (b << 0) | (alpha << 24);
            }
        }
    }
    
    // 読み出し用構造体メモリ解放
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

    // 読み出し元にサイズ情報を返却
    uWidth = nWidth;
    uHeight = nHeight;

    return hbmp;
}
