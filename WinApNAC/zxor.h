/*
zxor.h �������C�u�����E�w�b�_�t�@�C��
coded by isaku@pb4.so-net.ne.jp
*/

#ifndef __zxor_h
#define __zxor_h

/*
�A���S���Y����
http://www.jstatsoft.org/v08/i14/xorshift.pdf ���g�����B
�g�p���@�� zmtrand , zsfmt �Ƃقړ����B

��������������������������������������������������������������������
�ʏ�        �����n��      ����
��������������������������������������������������������������������
InitMt()    InitMt_m()    �����̎�r�ɂ�鏉����
InitMtEx()  InitMtEx_m()  �����m�̔z��j�ɂ�鏉����
NextMt()    NextMt_m()    32�r�b�g�����Ȃ������̗���
NextUnif()  NextUnif_m()  �O�ȏ�P�����̗���(53bit���x)
NextInt()   NextInt_m()   �O�ȏ�m�����̐�������
NextIntEx() NextIntEx_m() �ۂߌ덷�̂Ȃ��O�ȏ�m�����̐�������
��������������������������������������������������������������������

InitMt()�ANextUnif() �� _m �̕t���Ȃ��֐��͒ʏ�̎g����������B

�΂��� InitMt_m() NextUnif_m() ���̓}���`�X���b�h�ȃv���O�����╡��
�n��̗�����K�v�Ƃ����K�̓V�~�����[�V�����p�̃}�N���ł���B
�����͑������Ōn��ԍ����w�肷��B

�����̌n����g���ꍇ�� MSEQ �� 2 �ȏ�̕K�v�Ȑ��ɒ�`����B
�Ⴆ�Δԍ��� 0 �Ԃ��� 99 �Ԃ� 100 �̌n����g���Ȃ�

    bcc32 -DMSEQ=100 test.c

�̂悤�ɃR���p�C�����邩�Azxor ���C���N���[�h����O��

    #define MSEQ 100

�Ƃ���K�v������B

�قȂ�n����g���� �݂��̌Ăяo������Ɨ������n��𓾂���B
zxor �ł́A�����n����g���Ă��A�����Ɏg���p�����[�^������ŁA
��ԃe�[�u���̒l���قȂ邾������B
�]���ē�����ŏ���������΁A�����n�񂪓�����B

NextUnif() �� _m �̕t���Ȃ����̂́A
NextUnif_m(0) �̂悤�� 0 �Ԗڂ̌n����g���悤�ɒ�`����Ă���B

InitMt�`�����Ăяo������ Next�`�����Ăяo���ƁA
�f�t�H���g�̌n��ł͎���P�ɂ��ď���������B
�ق��̌n��ł͎�� -1 ���畉�̕����ɕω������ĂȂ��珉��������B
�������A���̋@�\�ɗ��炸�ɁANext�`�����g���O�ɁAInitMt�`����
�g���Ė����I�ɏ��������邱�Ƃ𐄏�����B
��������������������������������������������������������������������
���e���z�ɏ]������
�u�b����ɂ��ŐV�A���S���Y�����T�v1991 �Z�p�]�_�� ���Q�l�ɂ���
��������������������������������������������������������������������
�ʏ�}�N��      �����n��}�N��    ����
��������������������������������������������������������������������
NextChisq()     NextChisq_m()     ���R�x�˂̃J�C�Q�敪�z       p. 27
NextGamma()     NextGamma_m()     �p�����[�^���̃K���}���z     p. 31
NextGeometric() NextGeometric_m() �m���o�̊􉽕��z             p. 34
NextTriangle()  NextTriangle_m()  �O�p���z                     p. 89
NextExp()       NextExp_m()       ���ςP�̎w�����z             p.106
NextNormal()    NextNormal_m()    �W�����K���z(�ő�8.57��)     p.133
NextUnitVect()  NextUnitVect_m()  �m�����̃����_���P�ʃx�N�g�� p.185
NextBinomial()  NextBinomial_m()  �p�����[�^�m,�o�̂Q�����z    p.203
NextBinormal()  NextBinormal_m()  ���֌W���q�̂Q�ϗʐ��K���z   p.211
NextBeta()      NextBeta_m()      �p�����[�^�`,�a�̃x�[�^���z  p.257
NextPower()     NextPower_m()     �p�����[�^�m�̗ݏ敪�z       p.305
NextLogistic()  NextLogistic_m()  ���W�X�e�B�b�N���z           p.313
NextCauchy()    NextCauchy_m()    �R�[�V�[���z                 p.331
NextFDist()     NextFDist_m()     ���R�x�`,�a�̂e���z          p.344
NextPoisson()   NextPoisson_m()   ���σɂ̃|�A�\�����z         p.412
NextTDist()     NextTDist_m()     ���R�x�m�̂����z             p.430
NextWeibull()   NextWeibull_m()   �p�����[�^���̃��C�u�����z   p.431
��������������������������������������������������������������������
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
    unsigned long x[4];        /* �����̏�� */
    long          range;       /* NextIntEx �őO��͈̔� */
    unsigned long base;        /* NextIntEx �őO��̊�l */
    int           shift;       /* NextIntEx �őO��̃V�t�g�� */
    int           normal_sw;   /* NextNormal �Ŏc��������Ă��� */
    int           initialized;
    double        normal_save; /* NextNormal �̎c��̒l */
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

/*��������������������������������������������������
�@��32�r�b�g�����Ȃ������������A���̗����̊�{�֐���
�@��������������������������������������������������*/
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

/*��������������������������������������������������
�@���O�ȏ�P�����̕��������_��l����(32/53bit���x)��
�@��������������������������������������������������*/
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
