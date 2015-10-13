#include "pl.h"


// PNG�t�@�C��������������ǂݍ���
HBITMAP LoadPngFromFile(HWND hWnd, FILE *fp, unsigned &uWidth, unsigned &uHeight)
{
    // �����`�F�b�N
    if (!fp) {
        return NULL; // �G���[���^�[��
    }

    // PNG�ǂݍ��݊J�n(�������m�ہE������)
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return NULL; // �G���[���^�[��
    }

    // png_info �\���̂̏������i�摜�f�[�^�̑O�ɂ���`�����N�p�j
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return NULL; // �G���[���^�[��
    }

    // png_info �\���̂̏������i�摜�f�[�^�̌�ɂ���`�����N�p�j[�{�v���O�����ł͕s�v�ȕ����ł�]
    png_info *end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL; // �G���[���^�[��
    }

    // �t�@�C���|�C���^��`����
    png_init_io(png_ptr, fp);

    // �摜�f�[�^�ǂݍ���
    png_uint_32 nWidth, nHeight;
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    // �摜�����擾
    nWidth = info_ptr->width;
    nHeight = info_ptr->height;

    // �r�b�g�}�b�v�n���h���̍쐬
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

    // �摜�f�[�^�������ǂݏo��
    for (x=0; x<nWidth; ++x) {
        for (y=0; y<nHeight; ++y) { // RGBA��ARGB �ϊ�
            DWORD r     = row_pointers[y][4*x  ];
            DWORD g     = row_pointers[y][4*x+1];
            DWORD b     = row_pointers[y][4*x+2];
            DWORD alpha = row_pointers[y][4*x+3];

            // [�������ď璷�ɍ���Ă���܂�]
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
    
    // �ǂݏo���p�\���̃��������
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

    // �ǂݏo�����ɃT�C�Y����ԋp
    uWidth = nWidth;
    uHeight = nHeight;

    return hbmp;
}
