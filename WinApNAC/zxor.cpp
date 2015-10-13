/*
zxor.c 乱数ライブラリ本体
coded by isaku@pb4.so-net.ne.jp
*/

#include <math.h>
#include "zxor.h"

/*┌────────────────┐
　│マクロで渡されるデフォルトの領域│
　└────────────────┘*/
struct _state_t DefaultMt[MSEQ];
volatile long InitialSeed=-1;

/*┌────────┐
　│初期化のお手伝い│
　└────────┘*/
static unsigned long InitMtSub(unsigned long s,unsigned long i)
{ return AND32(1812433253UL*(s^(s>>30))+i+1); }

/*┌──────┐
　│領域を初期化│
　└──────┘*/
void InitMt_r(state_p mt,unsigned long s)
{
    int i;

    for (i=0;i<4;i++) mt->x[i]=s=InitMtSub(s,i);
    mt->initialized=1; mt->range=0; mt->normal_sw=0;
}

/*┌─────────┐
　│配列を使って初期化│
　└─────────┘*/
void InitMtEx_r(state_p mt,unsigned long*key,int len)
{
    int i,k; unsigned long*x=mt->x,s;

    InitMt_r(mt,1); s=x[3];
    for (i=0;i<len;i++)   x[i&3]^=s=InitMtSub(s,key[i]+i);
    for (k=0;k<3;k++,i++) x[i&3]^=s=InitMtSub(s,k);
    if (x[0]==0&&x[1]==0&&x[2]==0&&x[3]==0) x[0]=1;
}

/*┌───────┐
　│コンストラクタ│
　└───────┘*/
#ifdef __cplusplus
_state_t::_state_t()
{
    static volatile long seed=-1;

    if (this==DefaultMt) InitMt_r(this,1);
    else InitMt_r(this,seed--);
}
#endif

/*┌───────────────────────────┐
　│丸め誤差のない０以上range未満の整数乱数　　　　　　　 │
　│　　　　　　　　　　　　　　　　　　　　　　　　　　　│
　│rangeが一定ならNextInt_r()よりも５倍～１０高速。　　　│
　│for (i=0;i<10000;i++)　　　　　　　　　　 　　　　　　│
　│{ x[i]=NextIntEx(100); y[i]=NextIntEx(200); } 　　　　│
　│とするよりも、　　　　　　　　　　　　　　　　　　　　│
　│for (i=0;i<10000;i++) x[i]=NextIntEx(100);　　　　　　│
　│for (i=0;i<10000;i++) y[i]=NextIntEx(200);　　　　　　│
　│としたほうが１０倍以上高速になる。また複数系列を使って│
　│for (i=0;i<10000;i++)　　　　　　　　　　 　　　　　　│
　│{ x[i]=NextIntEx_m(1,100); y[i]=NextIntEx_m(2,200); } │
　│としても１０倍以上高速になる　　　　　　　　　　　　　│
　│言い換えれば range が毎回変わるなら、　　　　　　　　 │
　│NextInt_r() に比べ２倍の時間がかかる　　　　　　　　　│
　└───────────────────────────┘*/
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

/*┌──────────────┐
　│自由度νのカイ２乗分布 p.27 │
　└──────────────┘*/
double NextChisq_r(state_p mt,double n)
{
    return 2*NextGamma_r(mt,0.5*n);
}

/*┌───────────────┐
　│パラメータａのガンマ分布 p.31 │
　└───────────────┘*/
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

/*┌───────────┐
　│確率Ｐの幾何分布 p.34 │
　└───────────┘*/
int NextGeometric_r(state_p mt,double p)
{ return(int)ceil(log(1.0-NextUnif_r(mt))/log(1-p)); }

/*┌───────┐
　│三角分布 p.89 │
　└───────┘*/
double NextTriangle_r(state_p mt)
{ double a=NextUnif_r(mt),b=NextUnif_r(mt); return a-b; }

/*┌───────────┐
　│平均１の指数分布 p.106│
　└───────────┘*/
double NextExp_r(state_p mt)
{ return-log(1-NextUnif_r(mt)); }

/*┌─────────────────┐
　│標準正規分布(最大6.660437σ) p.133│
　└─────────────────┘*/
double NextNormal_r(state_p mt)
{
    if (mt->normal_sw==0) {
        double t=sqrt(-2*log(1.0-NextUnif_r(mt)));
        double u=3.141592653589793*2*NextUnif_r(mt);
        mt->normal_save=t*sin(u); mt->normal_sw=1; return t*cos(u);
    }else{ mt->normal_sw=0; return mt->normal_save; }
}

/*┌─────────────────┐
　│Ｎ次元のランダム単位ベクトル p.185│
　└─────────────────┘*/
void NextUnitVect_r(state_p mt,double*v,int n)
{
    int i; double r=0;
    for (i=0;i<n;i++) { v[i]=NextNormal_r(mt); r+=v[i]*v[i]; }
    if (r==0.0) r=1.0;
    r=sqrt(r);
    for (i=0;i<n;i++) v[i]/=r;
}

/*┌────────────────┐
　│パラメータＮ,Ｐの２項分布 p.203 │
　└────────────────┘*/
int NextBinomial_r(state_p mt,int n,double p)
{
    int i,r=0;
    for (i=0;i<n;i++) if (NextUnif_r(mt)<p) r++;
    return r;
}

/*┌────────────────┐
　│相関係数Ｒの２変量正規分布 p.211│
　└────────────────┘*/
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

/*┌─────────────────┐
　│パラメータＡ,Ｂのベータ分布 p.257 │
　└─────────────────┘*/
double NextBeta_r(state_p mt,double a,double b)
{
    double temp=NextGamma_r(mt,a);
    return temp/(temp+NextGamma_r(mt,b));
}

/*┌──────────────┐
　│パラメータＮの累乗分布 p.305│
　└──────────────┘*/
double NextPower_r(state_p mt,double n)
{ return pow(NextUnif_r(mt),1.0/(n+1)); }

/*┌────────────┐
　│ロジスティック分布 p.313│
　└────────────┘*/
double NextLogistic_r(state_p mt)
{
    double r;
    do r=NextUnif_r(mt); while (r==0);
    return log(r/(1-r));
}

/*┌─────────┐
　│コーシー分布 p.331│
　└─────────┘*/
double NextCauchy_r(state_p mt)
{
    double x,y;
    do { x=1-NextUnif_r(mt); y=2*NextUnif_r(mt)-1; }
    while (x*x+y*y>1);
    return y/x;
}

/*┌─────────────┐
　│自由度Ａ,ＢのＦ分布 p.344 │
　└─────────────┘*/
double NextFDist_r(state_p mt,double n1,double n2)
{
    double nc1=NextChisq_r(mt,n1),nc2=NextChisq_r(mt,n2);
    return (nc1*n2)/(nc2*n1);
}

/*┌─────────────┐
　│平均λのポアソン分布 p.412│
　└─────────────┘*/
int NextPoisson_r(state_p mt,double lambda)
{
    int k; lambda=exp(lambda)*NextUnif_r(mt);
    for (k=0;lambda>1;k++) lambda*=NextUnif_r(mt);
    return k;
}

/*┌───────────┐
　│自由度Ｎのｔ分布 p.428│
　└───────────┘*/
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

/*┌────────────────┐
　│パラメータαのワイブル分布 p.431│
　└────────────────┘*/
double NextWeibull_r(state_p mt,double alpha)
{ return pow(-log(1-NextUnif_r(mt)),1/alpha); }
