#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "Des_test_2.h"
#pragma once
using namespace std;

//文件的des加密
string DEncode_file(string name, string key)//name 明文文件名，key 密钥， 返回密文文件名 namecode
{
    FILE *fp_en_before = NULL;
    FILE *fp_en_after = NULL;
    char MingWen[8040],Flag2[8040];
    string Flag1,MiWen;

    string namecode = name.substr(0, name.length() - 4) + "_code.txt";

    //cout << namecode ;

    fp_en_before = fopen(name.data(), "r"); 
    fp_en_after = fopen(namecode.data(),"w+"); 

    if (fp_en_before == NULL)
    {
        cout << "传输文件打开失败" << endl;
        return 0;
    }

    while( fgets(MingWen,1000,fp_en_before) != NULL )//
    {	
        Flag1 = MingWen;// char 转 string
        //cout <<Flag1;
        /*for(int i = 0;i < strlen(MingWen);i++)
        {
            cout <<MingWen[i];
        }*/
        
        //MiWen = Flag1;
        MiWen = DEncode (Flag1,key);
        
        MiWen = MiWen + '\n';
        for(int i = 0;i <sizeof(Flag2);i++)
        {
            Flag2[i]=0;
        }
        
        for(int i = 0;i < MiWen.length();i++) //清空Flag2，以防无法全部覆盖一行
        {
            Flag2[i] = MiWen[i];
        }
        
        
        fputs(Flag2,fp_en_after);
        //cout << Flag2<<endl;// 
    }
    fclose(fp_en_before);
    fclose(fp_en_after);
    
    return namecode ;
}

//文件解密函数
string DDecode_file(string namecode, string key) //namecode 密文文件名，key 密钥， 返回明文文件名 name
{
    FILE *fp_de_before = NULL;
    FILE *fp_de_after = NULL;
    char MiWen[8040],Flag2[8040];
    string Flag1,MingWen;
    string name = namecode.substr(0, namecode.length() - 9) + "_1.txt"  ;

    fp_de_before = fopen(namecode.data(),"r");//
    fp_de_after = fopen(name.data(),"w+");// 

    while( fgets(MiWen,1000,fp_de_before) != NULL )
	{	
		Flag1 = MiWen;// string 转 char
		Flag1.pop_back();
        //cout <<Flag1;
		/*for(int i = 0;i < strlen(MingWen);i++)
		{
		    cout <<MingWen[i];
		}*/
		
		//MingWen = Flag1;
		MingWen = DDecode (Flag1,key);
	    //MiWen = Flag;
	    
	    for(int i = 0;i <sizeof(Flag2);i++)
		{
			Flag2[i]=0;
		}
		
	    for(int i = 0;i < MingWen.length();i++)
		{
		    Flag2[i] = MingWen[i];
		}
		
		fputs(Flag2,fp_de_after);
		//cout << Flag2;
    }
    fclose(fp_de_before);
    fclose(fp_de_after);

    return name;
}
