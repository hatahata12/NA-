/*
zxor.c �������C�u�����{��
coded by isaku@pb4.so-net.ne.jp
*/

#include <math.h>
#include "zxor.h"

/*������������������������������������
�@���}�N���œn�����f�t�H���g�̗̈愠
�@������������������������������������*/
struct _state_t DefaultMt[MSEQ];
volatile long InitialSeed=-1;

/*��������������������
�@���������̂���`����
�@��������������������*/
static unsigned long InitMtSub(unsigned long s,unsigned long i)
{ return AND32(1812433253UL*(s^(s>>30))+i+1); }

/*����������������
�@���̈����������
�@����������������*/
void InitMt_r(state_p mt,unsigned long s)
{
    int i;

    for (i=0;i<4;i++) mt->x[i]=s=InitMtSub(s,i);
    mt->initialized=1; mt->range=0; mt->normal_sw=0;
}

/*����������������������
�@���z����g���ď�������
�@����������������������*/
void InitMtEx_r(state_p mt,unsigned long*key,int len)
{
    int i,k; unsigned long*x=mt->x,s;

    InitMt_r(mt,1); s=x[3];
    for (i=0;i<len;i++)   x[i&3]^=s=InitMtSub(s,key[i]+i);
    for (k=0;k<3;k++,i++) x[i&3]^=s=InitMtSub(s,k);
    if (x[0]==0&&x[1]==0&&x[2]==0&&x[3]==0) x[0]=1;
}

/*������������������
�@���R���X�g���N�^��
�@������������������*/
#ifdef __cplusplus
_state_t::_state_t()
{
    static volatile long seed=-1;

    if (this==DefaultMt) InitMt_r(this,1);
    else InitMt_r(this,seed--);
}
#endif

/*����������������������������������������������������������
�@���ۂߌ덷�̂Ȃ��O�ȏ�range�����̐��������@�@�@�@�@�@�@ ��
�@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
�@��range�����Ȃ�NextInt_r()�����T�{�`�P�O�����B�@�@�@��
�@��for (i=0;i<10000;i++)�@�@�@�@�@�@�@�@�@�@ �@�@�@�@�@�@��
�@��{ x[i]=NextIntEx(100); y[i]=NextIntEx(200); } �@�@�@�@��
�@���Ƃ�������A�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
�@��for (i=0;i<10000;i++) x[i]=NextIntEx(100);�@�@�@�@�@�@��
�@��for (i=0;i<10000;i++) y[i]=NextIntEx(200);�@�@�@�@�@�@��
�@���Ƃ����ق����P�O�{�ȏ㍂���ɂȂ�B�܂������n����g���Ą�
�@��for (i=0;i<10000;i++)�@�@�@�@�@�@�@�@�@�@ �@�@�@�@�@�@��
�@��{ x[i]=NextIntEx_m(1,100); y[i]=NextIntEx_m(2,200); } ��
�@���Ƃ��Ă��P�O�{�ȏ㍂���ɂȂ�@�@�@�@�@�@�@�@�@�@�@�@�@��
�@������������� range ������ς��Ȃ�A�@�@�@�@�@�@�@�@ ��
�@��NextInt_r() �ɔ�ׂQ�{�̎��Ԃ�������@�@�@�@�@�@�@�@�@��
�@����������������������������������������������������������*/
long NextIntEx_r(state_p mt,long range)
{
    unsigned long y,base,remain; int shift;

    if (range<=0) return 0;
    if (range!=mt->range) {
        mt->range=base=range;
        for (shift=0;base<=(1UL<<30);shift++) base<<=1;
        mt->base=base; mt->shift=shift;
    }
    for (;;) {
        y=NextMt_r(mt)>>1;
        if (y<mt->base) return(long)(y>>mt->shift);
        base=mt->base; shift=mt->shift; y-=base;
        remain=(1UL<<31)-base;
        for (;remain>=(unsigned long)range;remain-=base) {
            for (;base>remain;base>>=1) shift--;
            if (y<base) return(long)(y>>shift); else y-=base;
        }
    }
}

long NextIntEx(long range)
{
    state_p mt=DefaultMt; unsigned long y,base,remain; int shift;

    if (range<=0) return 0;
    if (range!=mt->range) {
        mt->range=base=range;
        for (shift=0;base<=(1UL<<30);shift++) base<<=1;
        mt->base=base; mt->shift=shift;
    }
    for (;;) {
        y=NextMt()>>1;
        if (y<mt->base) return(long)(y>>mt->shift);
        base=mt->base; shift=mt->shift; y-=base;
        remain=(1UL<<31)-base;
        for (;remain>=(unsigned long)range;remain-=base) {
            for (;base>remain;base>>=1) shift--;
            if (y<base) return(long)(y>>shift); else y-=base;
        }
    }
}

/*��������������������������������
�@�����R�x�˂̃J�C�Q�敪�z p.27 ��
�@��������������������������������*/
double NextChisq_r(state_p mt,double n)
{
    return 2*NextGamma_r(mt,0.5*n);
}

/*����������������������������������
�@���p�����[�^���̃K���}���z p.31 ��
�@����������������������������������*/
double NextGamma_r(state_p mt,double a)
{
    double t,u,x,y;
    if (a>1) {
        t=sqrt(2*a-1);
        do {
            do {
                do {
                    x=1-NextUnif_r(mt);
                    y=2*NextUnif_r(mt)-1;
                } while (x*x+y*y>1);
                y/=x; x=t*y+a-1;
            } while (x<=0);
            u=(a-1)*log(x/(a-1))-t*y;
        } while (u<-50||NextUnif_r(mt)>(1+y*y)*exp(u));
    } else {
        t=2.718281828459045235/(a+2.718281828459045235);
        do {
            if (NextUnif_r(mt)<t) {
                x=pow(NextUnif_r(mt),1/a); y=exp(-x);
            } else {
                x=1-log(1-NextUnif_r(mt)); y=pow(x,a-1);
            }
        } while (NextUnif_r(mt)>=y);
    }
    return x;
}

/*��������������������������
�@���m���o�̊􉽕��z p.34 ��
�@��������������������������*/
int NextGeometric_r(state_p mt,double p)
{ return(int)ceil(log(1.0-NextUnif_r(mt))/log(1-p)); }

/*������������������
�@���O�p���z p.89 ��
�@������������������*/
double NextTriangle_r(state_p mt)
{ double a=NextUnif_r(mt),b=NextUnif_r(mt); return a-b; }

/*��������������������������
�@�����ςP�̎w�����z p.106��
�@��������������������������*/
double NextExp_r(state_p mt)
{ return-log(1-NextUnif_r(mt)); }

/*��������������������������������������
�@���W�����K���z(�ő�6.660437��) p.133��
�@��������������������������������������*/
double NextNormal_r(state_p mt)
{
    if (mt->normal_sw==0) {
        double t=sqrt(-2*log(1.0-NextUnif_r(mt)));
        double u=3.141592653589793*2*NextUnif_r(mt);
        mt->normal_save=t*sin(u); mt->normal_sw=1; return t*cos(u);
    }else{ mt->normal_sw=0; return mt->normal_save; }
}

/*��������������������������������������
�@���m�����̃����_���P�ʃx�N�g�� p.185��
�@��������������������������������������*/
void NextUnitVect_r(state_p mt,double*v,int n)
{
    int i; double r=0;
    for (i=0;i<n;i++) { v[i]=NextNormal_r(mt); r+=v[i]*v[i]; }
    if (r==0.0) r=1.0;
    r=sqrt(r);
    for (i=0;i<n;i++) v[i]/=r;
}

/*������������������������������������
�@���p�����[�^�m,�o�̂Q�����z p.203 ��
�@������������������������������������*/
int NextBinomial_r(state_p mt,int n,double p)
{
    int i,r=0;
    for (i=0;i<n;i++) if (NextUnif_r(mt)<p) r++;
    return r;
}

/*������������������������������������
�@�����֌W���q�̂Q�ϗʐ��K���z p.211��
�@������������������������������������*/
void NextBinormal_r(state_p mt,double r,double*x,double*y)
{
    double r1,r2,s;
    do {
        r1=2*NextUnif_r(mt)-1;
        r2=2*NextUnif_r(mt)-1;
        s=r1*r1+r2*r2;
    } while (s>1||s==0);
    s= -log(s)/s; r1=sqrt((1+r)*s)*r1;
    r2=sqrt((1-r)*s)*r2; *x=r1+r2; *y=r1-r2;
}

/*��������������������������������������
�@���p�����[�^�`,�a�̃x�[�^���z p.257 ��
�@��������������������������������������*/
double NextBeta_r(state_p mt,double a,double b)
{
    double temp=NextGamma_r(mt,a);
    return temp/(temp+NextGamma_r(mt,b));
}

/*��������������������������������
�@���p�����[�^�m�̗ݏ敪�z p.305��
�@��������������������������������*/
double NextPower_r(state_p mt,double n)
{ return pow(NextUnif_r(mt),1.0/(n+1)); }

/*����������������������������
�@�����W�X�e�B�b�N���z p.313��
�@����������������������������*/
double NextLogistic_r(state_p mt)
{
    double r;
    do r=NextUnif_r(mt); while (r==0);
    return log(r/(1-r));
}

/*����������������������
�@���R�[�V�[���z p.331��
�@����������������������*/
double NextCauchy_r(state_p mt)
{
    double x,y;
    do { x=1-NextUnif_r(mt); y=2*NextUnif_r(mt)-1; }
    while (x*x+y*y>1);
    return y/x;
}

/*������������������������������
�@�����R�x�`,�a�̂e���z p.344 ��
�@������������������������������*/
double NextFDist_r(state_p mt,double n1,double n2)
{
    double nc1=NextChisq_r(mt,n1),nc2=NextChisq_r(mt,n2);
    return (nc1*n2)/(nc2*n1);
}

/*������������������������������
�@�����σɂ̃|�A�\�����z p.412��
�@������������������������������*/
int NextPoisson_r(state_p mt,double lambda)
{
    int k; lambda=exp(lambda)*NextUnif_r(mt);
    for (k=0;lambda>1;k++) lambda*=NextUnif_r(mt);
    return k;
}

/*��������������������������
�@�����R�x�m�̂����z p.428��
�@��������������������������*/
double NextTDist_r(state_p mt,double n)
{
    double a,b,c;
    if (n<=2) {
        do a=NextChisq_r(mt,n); while (a==0);
        return NextNormal_r(mt)/sqrt(a/n);
    }
    do {
        a=NextNormal_r(mt); b=a*a/(n-2);
        c=log(1-NextUnif_r(mt))/(1-0.5*n);
    } while (exp(-b-c)>1-b);
    return a/sqrt((1-2.0/n)*(1-b));
}

/*������������������������������������
�@���p�����[�^���̃��C�u�����z p.431��
�@������������������������������������*/
double NextWeibull_r(state_p mt,double alpha)
{ return pow(-log(1-NextUnif_r(mt)),1/alpha); }
