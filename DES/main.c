#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "tables.h"

void HexToBit(int Dataout[],char Datain[],int Num);                   //十六进制到二进制
void BitToHex(int Dataout[],char Datain[]);                           //二进制到十六进制
int BitToDec(char Datain[],int Num);                                  //二进制到十进制
void DecToBit(char Dataout[],int Datain);                             //十进制到二进制

void SetKey(char Subkey[],char C[],char D[],int i);                   //得到子密钥
void SBox(char SO[],char T[]);                                        //S盒
void PlusMod2(char Dataout[],char Subkey[]);                          //模2相加
void TableChange(char Dataout[],char Datain[],const int Table[],int Num);  //置换
void LoopMove(char Datain[],int i);                                   //循环左移

void PlayEnc(int Mesout[],int Keyin[],int Mesin[]);                   //执行DES加密
void PlayDec(int Mesout[],int Keyin[],int Mesin[]);                   //执行DES解密


int main()
{
    int i=0;
    int Message[16];   //初始明文
    int Key[16];       //密钥
    int CipherText[16];//密文
    char H;

    printf("Machine: Hello!\nMachine: Do you want to Encrypt or Decode(E/D)?\n");
    printf("Human: ");

    H=getchar();//获取加密、解密信息
    getchar();

    if(H=='E'||H=='e')
    {
        printf("Machine: So,please input your Message(every digital separated by Space).\n");
        printf("Human: ");
        //输入明文
        for(i=0;i<16;i++)
        {
            scanf("%X",&Message[i]);
        }

        printf("Machine: Then,please input your Key(every digital separated by Space).\n");
        printf("Human: ");
        //输入密钥
        for(i=0;i<16;i++)
        {
            scanf("%X",&Key[i]);
        }
        putchar('\n');

        PlayEnc(CipherText,Key,Message);   // 执行DES加密

        printf("Machine: Your Message is Encrypted.\n");  // 信息已加密
        printf("Machine: ");
        for(i=0;i<16;i++)
        {
            printf("%X ",CipherText[i]);//输出密文
        }
        putchar('\n');
    }
    else
    {
        printf("Machine: So,please input your Ciphertext(every digital separated by Spaces).\n");
        printf("Human: ");

        for(i=0;i<16;i++)
        {
            scanf("%X",&CipherText[i]);//输入密文
        }

        printf("Machine: Then,please input your Key(every digital separated by Spaces).\n");
        printf("Human: ");

        for(i=0;i<16;i++)
        {
            scanf("%X",&Key[i]);//输入密钥
        }
        putchar('\n');

        PlayDec(Message,Key,CipherText);

        printf("Machine: Your Message is Decoded.\n");  // 信息已解密
        printf("Machine: ");
        for(i=0;i<16;i++)
        {
            printf("%X ",Message[i]);//输出明文
        }
        putchar('\n');
    }

    return 0;
}

//十六进制到二进制
void HexToBit(int Hex[],char Bit[],int Num)
{
    int i,j,n=0;
    char temp[4];

    for(i=0;i<Num;i++)
    {
        if(Hex>=0&&Hex[i]<=9)
        {
            DecToBit(temp,Hex[i]);
        }
        else
        {
            DecToBit(temp,Hex[i]-8);
            temp[0]='1';
        }
        for(j=0;j<4;j++)
        {
            Bit[j+n]=temp[j];
        }
        n=n+4;
    }
}

//二进制到十六进制
void BitToHex(int Dataout[],char Datain[])
{
    int i,j,n=0;
    char Temp[4];

    for(i=0;i<16;i++)
    {
        for(j=0;j<4;j++)
            Temp[j]=Datain[j+n];
        n=n+4;
        Dataout[i]=BitToDec(Temp,4);
    }
}

//二进制到十进制
int BitToDec(char Datain[],int Num)
{
    int i,Dataout=0;

    for(i=0;i<Num;i++)
    {
        if(Datain[i]=='1')
            Dataout=Dataout+pow(2,Num-i-1);
    }
    return Dataout;
}

//十进制到二进制
void DecToBit(char Dataout[],int Datain)
{
    int i,mod;

    for(i=0;i<4;i++)
    {
        mod=Datain%2;
        Datain=Datain/2;
        if(mod)
            Dataout[3-i]='1';
        else
            Dataout[3-i]='0';
    }
}

//模2相加
void PlusMod2(char Dataout[],char Subkey[])
{
    int i=0;

    for(i=0;i<48;i++)
    {
        if(Dataout[i]==Subkey[i])
            Dataout[i]='0';
        else
            Dataout[i]='1';
    }
}

//置换
void TableChange(char Dataout[],char Datain[],const int Table[],int Num)
{
    int i;

    for(i=0;i<Num;i++)
    {
        Dataout[i]=Datain[Table[i]-1];
    }
}

//循环左移
void LoopMove(char Datain[],int i)
{
    int j;
    char Num=Move_Table[i];
    char Temp[2];

    strncpy(Temp,Datain,Num);

    for(j=Num;j<28;j++)
    {
        Datain[j-Num]=Datain[j];
    }

    for(j=0;j<Num;j++)
    {
        Datain[j+28-Num]=Temp[j];
    }
}

//得到子密钥Ki
void SetKey(char Subkey[],char C[],char D[],int i)
{
    char Temp[56];
    int j;

    LoopMove(C,i);
    LoopMove(D,i);

    for(j=0;j<28;j++)
    {
        Temp[j]=C[j];
    }

    for(j=0;j<28;j++)
    {
        Temp[j+28]=D[j];
    }

    TableChange(Subkey,Temp,PC2_Table,48);
}

//S盒
void SBox(char SO[],char T[])
{
    int i,j,X,Y,n=0,m=0,TempS;
    char Temp[6],TempX[2],TempY[4],P[4];

    for(i=0;i<8;i++)
    {
        for(j=0;j<6;j++)
        {
            Temp[j]=T[j+n];
        }
        n=n+6;

        TempX[0]=Temp[0];
        TempX[1]=Temp[5];

        for(j=1;j<5;j++)
        {
            TempY[j-1]=Temp[j];
        }

        X=BitToDec(TempX,2);
        Y=BitToDec(TempY,4);
        TempS=S_Box[X+m][Y];
        DecToBit(P,TempS);

        for(j=0;j<4;j++)
        {
            SO[j+m]=P[j];
        }
        m=m+4;
    }
}

// 执行DES加密
void PlayEnc(int Mesout[],int Keyin[],int Mesin[])
{
    int i,j;
    char IP[64],Tem[64],MesoutBit[64],MesBit[64],KeyBit[64];
    char Change1[56];
    char Subkey[48],E[48];
    char L[32],R[32],S[32],P[32],temp[32];
    char C[28],D[28];

    HexToBit(Mesin,MesBit,16);
    HexToBit(Keyin,KeyBit,16);

    TableChange(IP,MesBit,IP_Table,64);//初始置换IP
    strncpy(L,IP,32);                  //L0
    for(i=0;i<32;i++)
    {
        R[i]=IP[i+32];                 //R0
    }

    TableChange(Change1,KeyBit,PC1_Table,56);
    strncpy(C,Change1,28);

    for(i=0;i<28;i++)
    {
        D[i]=Change1[i+28];
    }

    for(i=0;i<16;i++)
    {
        SetKey(Subkey,C,D,i);
        TableChange(E,R,E_Table,48);
        PlusMod2(E,Subkey);
        SBox(S,E);
        TableChange(P,S,P_Table,32);
        PlusMod2(L,P);

        for(j=0;j<32;j++)
        {
            temp[j]=L[j];
            L[j]=R[j];
            R[j]=temp[j];
        }
    }

    for(i=0;i<32;i++)
    {
        Tem[i]=R[i];
        Tem[i+32]=L[i];
    }

    TableChange(MesoutBit,Tem,IPR_Table,64);
    BitToHex(Mesout,MesoutBit);
}

void PlayDec(int Mesout[],int Keyin[],int Mesin[])
{
    int i,j;
    char IP[64],Tem[64],MesoutBit[64],MesinBit[64],KeyinBit[64];
    char Change1[56];
    char Subkey[48],E[48];
    char L[32],R[32],S[32],P[32],temp[32];
    char C0[28],D0[28],TemC[28],TemD[28],C[28],D[28];

    HexToBit(Mesin,MesinBit,16);
    HexToBit(Keyin,KeyinBit,16);

    TableChange(Tem,MesinBit,IP_Table,64);

    strncpy(L,Tem,32);
    for(i=0;i<32;i++)
    {
        R[i]=Tem[i+32];
    }

    TableChange(Change1,KeyinBit,PC1_Table,56);

    strncpy(C0,Change1,28);
    strncpy(TemC,Change1,28);
    for(i=0;i<28;i++)
    {
        D0[i]=Change1[i+28];
        TemD[i]=Change1[i+28];
    }

    for(i=0;i<16;i++)
    {
        for(j=0;j<16-i;j++)
        {
            SetKey(Subkey,C0,D0,j);
        }
        for(j=0;j<28;j++)
        {
            C0[j]=TemC[j];
            D0[j]=TemD[j];
        }
        TableChange(E,R,E_Table,48);
        PlusMod2(E,Subkey);
        SBox(S,E);
        TableChange(P,S,P_Table,32);
        PlusMod2(L,P);

        for(j=0;j<32;j++)
        {
            temp[j]=L[j];
            L[j]=R[j];
            R[j]=temp[j];
        }
    }

    for(i=0;i<32;i++)
    {
        Tem[i]=R[i];
        Tem[i+32]=L[i];
    }

    TableChange(MesoutBit,Tem,IPR_Table,64);
    BitToHex(Mesout,MesoutBit);
}
