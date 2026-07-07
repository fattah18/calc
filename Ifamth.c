#include "Ifamth.h"
#include <stdio.h>

long double power(long double x, long n){
long double y =1;
    if(n==0 && x!=0){
        return 1;
    }
    if(n==0 && x==0){return 0.0L/0.0L;}
    else{
        if(n>0){
            for (int i = 1; i <= n; i++)
            {
                y*=x;
            }
            return y;
        }
        else{
            for (int j = 0; j > n ; j--)
            {
                y= y/x;
            }
            return y;
        }
    }
}
long double powerfloat(long double a,long double b){
    if (b>0){return expo(b*lnx(a));}if(b<0){return 1/expo(b*lnx(a));}
}
long double fact(long long int n){
    if(n == 0){return 1;}
    else{
        if(n<0){return 0.0L/0.0L;}
        else{
            long double fact = 1;
            for (int i = 1; i <= n; i++)
            {
                fact *=i;
            }
            return fact;
        }
    }
    
}
long double expo(long double x){
long double term = 1.0L;
long double exp = 1.0L;
    for ( int n = 1; n < 50000; n++)
    {
        term *= (x/n);
        exp += term;
    }
    return exp;
}
long double lnx(long double x){
    if (x <= 0){return 0.0L/0.0L;} 

    long double y = (x - 1)/(x + 1);
    long double term = y;
    long double lnx = y;

    for (int n = 3; n < 5000000; n+= 2)
    {
        term *= (y*y);
        lnx += (term / n);
    }
    return 2 * lnx;
}
long double cosin(long double x){
    long double term = 1.0L;
    long double cosin = 1.0L;
    if(fracpart(x/myp)==0.5){return 0;}
    else if(fracpart((x/myp)/2.0)==0){return 1;}
    else if(fracpart(x/myp)==0 && fracpart((x/myp)/2.0)!=0){return -1;}
    else{
    for(int n = 1; n < 50; n++){
        term *= ((-x * x)/((2.0*n - 1)*(2.0*n)));
        cosin += term;
    }
    return cosin;}
}
long double sinus(long double x){
    long double term = x;
    long double sinus = x;
    if(fracpart(x/myp)==0){return 0;}
    else{
    for (int n = 1; n < 50; n++){
        term *= ((-x*x)/((2.0*n)*(2.0*n + 1)));
        sinus += term;
    }return sinus;}
}
long double arcsin(long double x) {
    long double term = x;
    long double arcsin = x;
    long double num = 1.0L;
    long double den = 2.0L;
    long double xx = x * x;
    if(x<=1 && x>=-1){
        for (int n = 1; n < 30; n++) {
        num *= (2*n - 1);
        den *= (2*n);
        term *= xx * num / den;
        arcsin += term / (2*n + 1);
        return arcsin;
    }
    }else{
        return 0.0L/0.0L;
    }

}
long double arccosin(long double x){
    return (myp/2)-arcsin(x);
}
long double tng(long double x){
    return sinus(x)/cosin(x);
}
long double arctng(long double x){
    if (x > 1.0L)
        return myp/2.0L - arctng(1.0L/x);
    if (x < -1.0L)
        return -myp/2.0L - arctng(1.0L/x);

    long double arctng = x;
    long double term = x;
    long double xx = x * x;

    for (int n = 1; n < 50; n++) {
        term *= -xx;
        arctng += term / (2*n + 1);
    }
    return arctng;
}
long double racine(long double x) {
    if (x < 0.0L) {return 0.0L/0.0L;}
    if (x == 0) {return 0.0L;}
    long double y = x;
    long double prev = 0.0L;

    while (y != prev) {
        prev = y;
        y = (y + x / y) / 2.0L;
    }
    return y;
}
long double racinecube(long double x) {
    if (x == 0.0L) return 0.0L;

    long double y = x;
    long double prev = 0.0L;

    while (y != prev) {
        prev = y;
        y = (2.0L * y + x / (y * y)) / 3.0L;
    }
    return y;
}
long long bin_to_dec(long long bin) {
    long long dec = 0, base = 1;
    int isbin = 1;
    long long temp;
    temp = bin;
    while(temp>0){
        if (temp % 10 > 1){
            isbin=0;
            break;
        }
        temp = temp / 10;
    }
    if(isbin == 1){
    while (bin > 0) {
        int digit = bin % 10;
        dec += digit * base;
        base *= 2;
        bin /= 10;
    }
    return dec;}
    else{return 0/0;}
}
long long dec_to_bin(long long dec) {
    long long bin = 0, base = 1;
    while (dec > 0) {
        int bit = dec % 2;
        bin += bit * base;
        base *= 10;
        dec /= 2;
    }
    return bin;
}
long double logdec(long double x){
    return lnx(x)/lnx(10);
}
long long dec_to_oct(long long dec) {
    long long oct=0,base=1;
    while (dec>0)
    {
        oct += (dec % 8)*base;
        base *= 10;
        dec /= 8;
    }
    return oct;
}
long long oct_to_dec(long long oct) {
    long long dec = 0, base = 1;
    int isoct = 1;
    long long temp;
    temp = oct;
    while(temp>0){
        if (temp % 10 > 7){
            isoct=0;
            break;
        }
        temp = temp / 10;
    }
    if(isoct == 1){
    while (oct > 0) {
        int digit = oct % 10;
        dec += digit * base;
        base *= 8;
        oct /= 10;
    }
    return dec;}
    else{return 0/0;}
}
long double sinush(long double x) {
    return (expo(x) - expo(-x)) / 2.0;
}
long double cosinh(long double x) {
    return (expo(x) + expo(-x)) / 2.0;
}
long double tngh(long double x){
    double epx = expo(x);
    double emx = expo(-x);
    return (epx - emx) / (epx + emx);
}
long double vabs(long double x) {
    if(x>=0){ return x;}
    else{return -x;}
}
long double asinush(long double x) {
    return lnx(x + racine(x*x + 1));
}
long double acosinh(long double x) {
    return lnx(x + racine(x * x - 1.0));
}
long double atngh(long double x) {
    return 0.5 * logdec((1.0 + x) / (1.0 - x));
}
long long intpart(long double x){
    return (int)x;
}
long double fracpart(long double x){
    return x - (int)x;
}