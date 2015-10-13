/*
zxor.h 乱数ライブラリ・ヘッダファイル
coded by isaku@pb4.so-net.ne.jp
*/

#ifndef __zxor_h
#define __zxor_h

/*
アルゴリズムは
http://www.jstatsoft.org/v08/i14/xorshift.pdf を使った。
使用方法は zmtrand , zsfmt とほぼ同じ。

──────────────────────────────────
通常        複数系列      説明
──────────────────────────────────
InitMt()    InitMt_m()    整数の種Ｓによる初期化
InitMtEx()  InitMtEx_m()  長さＮの配列Ｋによる初期化
NextMt()    NextMt_m()    32ビット符号なし整数の乱数
NextUnif()  NextUnif_m()  ０以上１未満の乱数(53bit精度)
NextInt()   NextInt_m()   ０以上Ｎ未満の整数乱数
NextIntEx() NextIntEx_m() 丸め誤差のない０以上Ｎ未満の整数乱数
──────────────────────────────────

InitMt()、NextUnif() 等 _m の付かない関数は通常の使い方をする。

対して InitMt_m() NextUnif_m() 等はマルチスレッドなプログラムや複数
系列の乱数を必要とする大規模シミュレーション用のマクロである。
これらは第一引数で系列番号を指定する。

複数の系列を使う場合は MSEQ を 2 以上の必要な数に定義する。
例えば番号が 0 番から 99 番の 100 個の系列を使うなら

    bcc32 -DMSEQ=100 test.c

のようにコンパイルするか、zxor をインクルードする前に

    #define MSEQ 100

とする必要がある。

異なる系列を使えば 互いの呼び出しから独立した系列を得られる。
zxor では、複数系列を使っても、生成に使うパラメータが同一で、
状態テーブルの値が異なるだけある。
従って同じ種で初期化すれば、同じ系列が得られる。

NextUnif() 等 _m の付かないものは、
NextUnif_m(0) のように 0 番目の系列を使うように定義されている。

InitMt～等を呼び出さずに Next～等を呼び出すと、
デフォルトの系列では種を１にして初期化する。
ほかの系列では種を -1 から負の方向に変化させてながら初期化する。
ただし、この機能に頼らずに、Next～等を使う前に、InitMt～等を
使って明示的に初期化することを推奨する。
──────────────────────────────────
●各分布に従う乱数
「Ｃ言語による最新アルゴリズム事典」1991 技術評論社 を参考にした
──────────────────────────────────
通常マクロ      複数系列マクロ    説明
──────────────────────────────────
NextChisq()     NextChisq_m()     自由度νのカイ２乗分布       p. 27
NextGamma()     NextGamma_m()     パラメータａのガンマ分布     p. 31
NextGeometric() NextGeometric_m() 確率Ｐの幾何分布             p. 34
NextTriangle()  NextTriangle_m()  三角分布                     p. 89
NextExp()       NextExp_m()       平均１の指数分布             p.106
NextNormal()    NextNormal_m()    標準正規分布(最大8.57σ)     p.133
NextUnitVect()  NextUnitVect_m()  Ｎ次元のランダム単位ベクトル p.185
NextBinomial()  NextBinomial_m()  パラメータＮ,Ｐの２項分布    p.203
NextBinormal()  NextBinormal_m()  相関係数Ｒの２変量正規分布   p.211
NextBeta()      NextBeta_m()      パラメータＡ,Ｂのベータ分布  p.257
NextPower()     NextPower_m()     パラメータＮの累乗分布       p.305
NextLogistic()  NextLogistic_m()  ロジスティック分布           p.313
NextCauchy()    NextCauchy_m()    コーシー分布                 p.331
NextFDist()     NextFDist_m()     自由度Ａ,ＢのＦ分布          p.344
NextPoisson()   NextPoisson_m()   平均λのポアソン分布         p.412
NextTDist()     NextTDist_m()     自由度Ｎのｔ分布             p.430
NextWeibull()   NextWeibull_m()   パラメータαのワイブル分布   p.431
──────────────────────────────────
*/
#if defined(_MSC_VER) && _MSC_VER <= 1200
#pragma warning(disable : 4710)
#pragma warning(disable : 4514)
#endif

#include <limits.h>

#ifndef MSEQ
#define MSEQ 1
#endif

#ifdef __GNUC__
#define __inline __inline__
#endif

#if LONG_MAX == 0x7FFFFFFFUL
#define AND32(X) (X)
#define MSK32(X)
#else
#define AND32(X) ((X)&0xFFFFFFFFUL)
#define MSK32(X) (X&=0xFFFFFFFFUL)
#endif

#if INT_MAX == 32767
#define __inline
#endif

typedef struct _state_t {
    unsigned long x[4];        /* 乱数の状態 */
    long          range;       /* NextIntEx で前回の範囲 */
    unsigned long base;        /* NextIntEx で前回の基準値 */
    int           shift;       /* NextIntEx で前回のシフト数 */
    int           normal_sw;   /* NextNormal で残りを持っている */
    int           initialized;
    double        normal_save; /* NextNormal の残りの値 */
#ifdef __cplusplus
    _state_t();
#endif
} state_t[1];

typedef struct _state_t*state_p;
extern struct _state_t DefaultMt[MSEQ];
extern volatile long InitialSeed;
extern void     InitMt_r       (state_p,unsigned long);
extern void     InitMtEx_r     (state_p,unsigned long*,int);
extern long     NextIntEx_r    (state_p,long);
extern long     NextIntEx      (long);
extern double   NextChisq_r    (state_p,double);
extern double   NextGamma_r    (state_p,double);
extern int      NextGeometric_r(state_p,double);
extern double   NextTriangle_r (state_p);
extern double   NextExp_r      (state_p);
extern double   NextNormal_r   (state_p);
extern void     NextUnitVect_r (state_p,double*,int);
extern int      NextBinomial_r (state_p,int,double);
extern void     NextBinormal_r (state_p,double,double*,double*);
extern double   NextBeta_r     (state_p,double,double);
extern double   NextPower_r    (state_p,double);
extern double   NextLogistic_r (state_p);
extern double   NextCauchy_r   (state_p);
extern double   NextFDist_r    (state_p,double,double);
extern int      NextPoisson_r  (state_p,double);
extern double   NextTDist_r    (state_p,double);
extern double   NextWeibull_r  (state_p,double);

#ifdef __cplusplus
#define DeclMt(N) state_t N
#else
#define DeclMt(N) state_t N={{0}}
#endif

#define NextInt_r(M,N) ((long)((N)*(1.0/4294967296.0)*NextMt_r(M)))
#define NextInt(N)     ((long)((N)*(1.0/4294967296.0)*NextMt()))
#define InitMt(S)      InitMt_r    (DefaultMt,S)
#define InitMtEx(K,N)  InitMtEx_r  (DefaultMt,K,N)

#define NextChisq(N)        NextChisq_r    (DefaultMt,N)
#define NextGamma(A)        NextGamma_r    (DefaultMt,A)
#define NextGeometric(P)    NextGeometric_r(DefaultMt,P)
#define NextTriangle()      NextTriangle_r (DefaultMt)
#define NextExp()           NextExp_r      (DefaultMt)
#define NextNormal()        NextNormal_r   (DefaultMt)
#define NextUnitVect(V,N)   NextUnitVect_r (DefaultMt,V,N)
#define NextBinomial(N,P)   NextBinomial_r (DefaultMt,N,P)
#define NextBinormal(R,X,Y) NextBinormal_r (DefaultMt,R,X,Y)
#define NextBeta(A,B)       NextBeta_r     (DefaultMt,A,B)
#define NextPower(N)        NextPower_r    (DefaultMt,N)
#define NextLogistic()      NextLogistic_r (DefaultMt)
#define NextCauchy()        NextCauchy_r   (DefaultMt)
#define NextFDist(A,B)      NextFDist_r    (DefaultMt,A,B)
#define NextPoisson(L)      NextPoisson_r  (DefaultMt,L)
#define NextTDist(N)        NextTDist_r    (DefaultMt,N)
#define NextWeibull(A)      NextWeibull_r  (DefaultMt,A)

#define InitMt_m(M,S)           InitMt_r       (DefaultMt+(M),S)
#define InitMtEx_m(M,K,N)       InitMtEx_r     (DefaultMt+(M),K,N)
#define NextMt_m(M)             NextMt_r       (DefaultMt+(M))
#define NextUnif_m(M)           NextUnif_r     (DefaultMt+(M))
#define NextInt_m(M,N)          NextInt_r      (DefaultMt+(M),N)
#define NextIntEx_m(M,N)        NextIntEx_r    (DefaultMt+(M),N)
#define NextChisq_m(M,N)        NextChisq_r    (DefaultMt+(M),N)
#define NextGamma_m(M,A)        NextGamma_r    (DefaultMt+(M),A)
#define NextGeometric_m(M,P)    NextGeometric_r(DefaultMt+(M),P)
#define NextTriangle_m(M)       NextTriangle_r (DefaultMt+(M))
#define NextExp_m(M)            NextExp_r      (DefaultMt+(M))
#define NextNormal_m(M)         NextNormal_r   (DefaultMt+(M))
#define NextUnitVect_m(M,V,N)   NextUnitVect_r (DefaultMt+(M),V,N)
#define NextBinomial_m(M,N,P)   NextBinomial_r (DefaultMt+(M),N,P)
#define NextBinormal_m(M,R,X,Y) NextBinormal_r (DefaultMt+(M),R,X,Y)
#define NextBeta_m(M,A,B)       NextBeta_r     (DefaultMt+(M),A,B)
#define NextPower_m(M,N)        NextPower_r    (DefaultMt+(M),N)
#define NextLogistic_m(M)       NextLogistic_r (DefaultMt+(M))
#define NextCauchy_m(M)         NextCauchy_r   (DefaultMt+(M))
#define NextFDist_m(M,A,B)      NextFDist_r    (DefaultMt+(M),A,B)
#define NextPoisson_m(M,L)      NextPoisson_r  (DefaultMt+(M),L)
#define NextTDist_m(M,N)        NextTDist_r    (DefaultMt+(M),N)
#define NextWeibull_m(M,A)      NextWeibull_r  (DefaultMt+(M),A)

/*┌───────────────────────┐
　│32ビット符号なし長整数乱数、他の乱数の基本関数│
　└───────────────────────┘*/
#if INT_MAX == 32767

static __inline unsigned long NextMt_r(state_p mt)
{
    unsigned short tlo,thi,wlo,whi;
#ifndef __cplusplus

    if (mt->initialized==0) {
        if (mt==DefaultMt) InitMt_r(mt,1);
        else InitMt_r(mt,InitialSeed--);
    }
#endif
    tlo=(unsigned short)mt->x[0];
    thi=(unsigned short)(mt->x[0]>>16);
    thi^=(thi<<11)|(tlo>>5); tlo^=tlo<<11;
    tlo^=(thi<<8)|(tlo>>8); thi^=thi>>8;
    mt->x[0]=mt->x[1]; mt->x[1]=mt->x[2]; mt->x[2]=mt->x[3];
    wlo=(unsigned short)mt->x[3];
    whi=(unsigned short)(mt->x[3]>>16);
    wlo^=whi>>3; wlo^=tlo; whi^=thi;
    return mt->x[3]=((unsigned long)whi<<16)|wlo;
}

static __inline unsigned long NextMt(void)
{
    state_p mt=DefaultMt; unsigned short tlo,thi,wlo,whi;

#ifndef __cplusplus
    if (mt->initialized==0) InitMt_r(mt,1);
#endif
    tlo=(unsigned short)mt->x[0];
    thi=(unsigned short)(mt->x[0]>>16);
    thi^=(thi<<11)|(tlo>>5); tlo^=tlo<<11;
    tlo^=(thi<<8)|(tlo>>8); thi^=thi>>8;
    mt->x[0]=mt->x[1]; mt->x[1]=mt->x[2]; mt->x[2]=mt->x[3];
    wlo=(unsigned short)mt->x[3];
    whi=(unsigned short)(mt->x[3]>>16);
    wlo^=whi>>3; wlo^=tlo; whi^=thi;
    return mt->x[3]=((unsigned long)whi<<16)|wlo;
}

#else

static __inline unsigned long NextMt_r(state_p mt)
{
    unsigned long t,r;
#ifndef __cplusplus

    if (mt->initialized==0) {
        if (mt==DefaultMt) InitMt_r(mt,1);
        else InitMt_r(mt,InitialSeed--);
    }
#endif
    t=mt->x[0]; r=t; t<<=11; MSK32(t); t^=r; r=t; r>>=8; t^=r; 
    r=mt->x[1]; mt->x[0]=r; r=mt->x[2]; mt->x[1]=r; r=mt->x[3];
    mt->x[2]=r; t^=r; r>>=19; r^=t; mt->x[3]=r; return r;
}

static __inline unsigned long NextMt(void)
{
    state_p mt=DefaultMt; unsigned long t,r;

#ifndef __cplusplus
    if (mt->initialized==0) InitMt_r(mt,1);
#endif
    t=mt->x[0]; r=t; t<<=11; MSK32(t); t^=r; r=t; r>>=8; t^=r; 
    r=mt->x[1]; mt->x[0]=r; r=mt->x[2]; mt->x[1]=r; r=mt->x[3];
    mt->x[2]=r; t^=r; r>>=19; r^=t; mt->x[3]=r; return r;
}

#endif

/*┌───────────────────────┐
　│０以上１未満の浮動小数点一様乱数(32/53bit精度)│
　└───────────────────────┘*/
static __inline double NextUnif_r(state_p mt)
{
#ifdef FAST_UNIF
    return NextMt_r(mt)*(1.0/4294967296.0);
#else
    unsigned long x=NextMt_r(mt)>>11,y=NextMt_r(mt);
    return(y*2097152.0+x)*(1.0/9007199254740992.0);
#endif
}

static __inline double NextUnif(void)
{
#ifdef FAST_UNIF
    return NextMt()*(1.0/4294967296.0);
#else
    unsigned long x=NextMt()>>11,y=NextMt();
    return(y*2097152.0+x)*(1.0/9007199254740992.0);
#endif
}

#endif
