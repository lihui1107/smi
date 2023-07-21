#include<iostream>
#include<cstdio>
#include<string>
#include<algorithm>
#pragma once
using namespace std;

//PC1选位表
int pc1Table[56]=
{
    57,49,41,33,25,17,9,1,
    58,50,42,34,26,18,10,2,
    59,51,43,35,27,19,11,3,
    60,52,44,36,63,55,47,39,
    31,23,15,7,62,54,46,38,
    30,22,14,6,61,53,45,37,
    29,21,13,5,28,20,12,4
};
//左移位数表
int loopTable[16]=
{
    1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
//PC2选位表
int pc2Table[48]=
{
    14,17,11,24,1,5,
    3,28,15,6,21,10,
    23,19,12,4,26,8,
    16,7,27,20,13,2,
    41,52,31,37,47,55,
    30,40,51,45,33,48,
    44,49,39,56,34,53,
    46,42,50,36,29,32
};
//置换IP表
int ipTable[64]=
{
    58,50,42,34,26,18,10,2,
    60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,
    64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,
    59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,
    63,55,47,39,31,23,15,7
};
//扩展置换表
int extendTable[48]=
{
    32,1,2,3,4,5,
    4,5,6,7,8,9,
    8,9,10,11,12,13,
    12,13,14,15,16,17,
    16,17,18,19,20,21,
    20,21,22,23,24,25,
    24,25,26,27,28,29,
    28,29,30,31,32,1
};
//S盒
int sBox[8][4][16]=
{
    //S1
    14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
    0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
    4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
    15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
    //S2
    15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
    3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
    0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
    13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
    //S3
    10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
    13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
    13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
    1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
    //S4
    7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
    13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
    10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
    3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
    //S5
    2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
    14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
    4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
    11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
    //S6
    12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
    10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
    9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
    4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
    //S7
    4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
    13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
    1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
    6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
    //S8
    13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
    1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
    7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
    2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
};
//P换位表
int pTable[32]=
{
    16,7,20,21,
    29,12,28,17,
    1,15,23,26,
    5,18,31,10,
    2,8,24,14,
    32,27,3,9,
    19,13,30,6,
    22,11,4,25
};
//逆置换IP^-1表
int ipReverseTable[64]={
    40,8,48,16,56,24,64,32,
    39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30,
    37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28,
    35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26,
    33,1,41,9,49,17,57,25
};
//字符转二进制
string charToBinary(char c)
{
    int i,b=c,k=0,flag=0;
    string result;
    if(b<0)
    {
        b=-b;
        flag=1;
    }
    while(k<8)
    {
        if(b)
        {
            result+=((b%2)+'0');
            b/=2;
        }
        else result+='0';
        k++;
    }
    //汉字字符处理
    if(flag)
    {
        for(i=0;i<result.length();i++)
        {
            if(result[i]=='0') result[i]='1';
            else result[i]='0';
        }
        for(i=0;result[i]!='0';i++)
        {
            result[i]='0';
        }
        result[i]='1';
    }
    reverse(result.begin(),result.end());
    return result;
}
//二进制转整型
int binaryToInt(string s)
{
    int i,result=0,p=1;
    for(i=s.length()-1;i>=0;i--)
    {
        result+=((s[i]-'0')*p);
        p*=2;
    }
    return result;
}
//整型转二进制
string intToBinary(int i)
{
    int k=0;
    string result;
    while(k<4)
    {
        if(i)
        {
            result+=((i%2)+'0');
            i/=2;
        }
        else result+='0';
        k++;
    }
    reverse(result.begin(),result.end());
    return result;
}
//异或运算
string xorAB(string a,string b)
{
    int i;
    string result;
    for(i=0;i<a.length();i++)
    {
        result+=(((a[i]-'0')^(b[i]-'0'))+'0');
    }
    return result;
}
//f函数
string f(string right,string k)
{
    int i,temp;
    string extendBinary,result,b0;
    string b[8],row,col;
    string b8,pb;
    for(i=0;i<48;i++)
    {
        extendBinary+=right[extendTable[i]-1];
    }
    b0=xorAB(extendBinary,k);
    for(i=0;i<8;i++)
    {
        b[i]=b0.substr(i*6,6);
    }
    for(i=0;i<8;i++)
    {
        row=b[i].substr(0,1)+b[i].substr(5,1);
        col=b[i].substr(1,4);
        temp=sBox[i][binaryToInt(row)][binaryToInt(col)];
        b[i]=intToBinary(temp);
        b8+=b[i];
    }
    for(i=0;i<32;i++)
    {
        pb+=b8[pTable[i]-1];
    }
    return pb;
}

//密钥+明文/密文处理
string wen(string wenBinary[], string miyao, int num, int flag)
{
    //密钥处理
    string k[16];
    int i,j;
    string miyaoBinary,pc1MiyaoBinary;
    string c[17],d[17],temp1,pc2Temp;
    
    for(i=0;i<miyao.size();i++)
    {
        miyaoBinary+=charToBinary(miyao[i]);
    }
    while(miyaoBinary.length()%64!=0)
    {
        miyaoBinary+='0';
    }
    for(i=0;i<56;i++)
    {
        pc1MiyaoBinary+=miyaoBinary[pc1Table[i]-1];
    }
    c[0]=pc1MiyaoBinary.substr(0,28);
    d[0]=pc1MiyaoBinary.substr(28,28);
    for(i=1;i<=16;i++)
    {
        c[i]=c[i-1].substr(loopTable[i-1],28-loopTable[i-1])+c[i-1].substr(0,loopTable[i-1]);
        d[i]=d[i-1].substr(loopTable[i-1],28-loopTable[i-1])+d[i-1].substr(0,loopTable[i-1]);
        temp1=c[i]+d[i];
        pc2Temp="";
        for(j=0;j<48;j++)
        {
            pc2Temp+=temp1[pc2Table[j]-1];
        }
        k[i-1]=pc2Temp;
    }

    //处理
    //int i,j;
    string ipWenBinary[100];
    string left[17],right[17],temp2,result;
    for(i=0;i<num;i++)
    {
        temp2="";
        for(j=0;j<64;j++)
        {
            temp2+=wenBinary[i][ipTable[j]-1];
        }
        ipWenBinary[i]=temp2;
    }
    for(i=0;i<num;i++)
    {
        left[0]=ipWenBinary[i].substr(0,32);
        right[0]=ipWenBinary[i].substr(32,32);
        for(j=0;j<16;j++)
        {
            left[j+1]=right[j];
            //加密和解密的区别
            if(flag==1) right[j+1]=xorAB(left[j],f(right[j],k[j]));
            else right[j+1]=xorAB(left[j],f(right[j],k[15-j]));
        }
        temp2=right[j]+left[j];
        for(j=0;j<64;j++)
        {
            result+=temp2[ipReverseTable[j]-1];
        }
    }
    //解密结果输出的是字符
    if(flag==2)
    {
        string ch;
        for(i=0;i<num*8;i++)
        {
            ch+=binaryToInt(result.substr(8*i,8));
        }
        result=ch;
    }
    return result;
}

//加密函数DEncode
string DEncode(string wenString, string key) //参数(明文， 密钥)
{
    int num1 = 0;
    int flag1 =1;
    string wenBinary1[1024],temp3;
    string out1;

    //密钥格式识别
    if ( key.length() > 8)
    {
        cout<<"密钥格式不能超过8位"<<endl;
        return 0;
    }
    //填补
    for(int i=0;i<wenString.length();i++)
    {
        temp3+=charToBinary(wenString[i]);
        //字符每满8bit为一组，最后一组可以不满8bit，后面会补0
        if( ((i+1)%8==0) || ( ((i+1)%8!=0)&&(i==wenString.length()-1)))
        {
            wenBinary1[num1++]=temp3;
            temp3="";
        }
    }
    //cout<<"temp："<<temp3<<endl;
    while(wenBinary1[num1-1].length()%64!=0)
    {
        wenBinary1[num1-1]+='0';
    }
    
    //cout<<"加密结果为（二进制）："<<wen(wenBinary1,key,num1, flag1)<<endl<<endl;
    out1 =wen(wenBinary1,key,num1,flag1);
    return out1;

}

//解密函数DDecode
string DDecode( string wenString, string key) //参数(密文， 密钥)
{
    int num2 = 0 ;
    int flag2 = 2;
    string wenBinary2[1024], temp4;
    string out2;

    //密钥格式识别
    if ( key.length() > 8)
    {
        cout<<"密钥格式不能超过8位"<<endl;
        return 0;
    }
    //解密
    for(int i=0;i*64<wenString.length();i++)
    {
        wenBinary2[num2++]=wenString.substr(i*64,64);
    }

    //cout<<"解密结果为（字符）："<<wen(wenBinary2, key,num2, flag2)<<endl<<endl;
    out2 = wen(wenBinary2, key, num2, flag2);
    return out2;    

}