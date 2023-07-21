//客户端
#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <openssl/md5.h>
#include <cstdio>
#include <string>
#include <algorithm>
#include "Des_test_2.h"
#include "Des_filecode.h"

#define PORT 8080
#define BUFFER_SIZE 1024
using namespace std;


//获取文件的MD5值
void get_file_md5(const char *filename,unsigned char md5 [MD5_DIGEST_LENGTH])
{
    // unsigned char md5 [MD5_DIGEST_LENGTH];存储MD5结果的缓冲区
    int i;
    FILE *inFile = fopen (filename, "rb");
    //inFile是个文件指针(FILE*),filename是个文件名,类型是字符串(char*),rb表示是看成二进制打开并读该文件
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];
    if (inFile == NULL) {
        printf ("%s can't be opened.\n", filename);   
    }
    MD5_Init (&mdContext);// 初始化MD5上下文
    while ((bytes = fread (data, 1, 1024, inFile)) != 0)// 循环读取文件，并更新MD5上下文
        MD5_Update (&mdContext, data, bytes);
    MD5_Final (md5,&mdContext);// 计算MD5哈希值
    printf("文件MD5值为:");
    for(i = 0; i < MD5_DIGEST_LENGTH; i++) { // 输出MD5哈希值
       printf("%02x", md5[i]); 
    }
    printf("\n");
    fclose (inFile);// 关闭文件
}

//获取消息的MD5值
void get_md5(char input[],unsigned char digest[MD5_DIGEST_LENGTH]) {
    // char input[] = "4152";  // 要进行MD5加密的输入数据
    // unsigned char digest[MD5_DIGEST_LENGTH];  // 存储MD5结果的缓冲区
    MD5((const unsigned char*)input, strlen(input), digest);// 进行MD5计算
    printf("消息MD5值为:");
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) { // 将MD5结果以十六进制形式打印输出
        printf("%02x", digest[i]);
    }
    printf("\n");
}

//发送消息
void sendMessage(int socket) {
    char message[BUFFER_SIZE];
    std::cout << "请输入消息: ";
    std::cin.getline(message, BUFFER_SIZE);
    //读字符串,当读到回车键的时候，此次读取结束，会将指针光标移到回车键之后，从而下一次读取的是回车键之后的字符。
    unsigned char md5 [16];
    memset(md5,0,sizeof(md5));//对md5数组的的所有的字节都赋值成0
    get_md5(message,md5);//获取消息的md5值
    std::string key3 = "12345678";//密钥
    string output1 = DEncode(message, key3) ;//对消息进行加密
    std::cout<<"加密后的密文："<<output1<<endl;
    send(socket, output1.c_str(), strlen(output1.c_str()), 0);
    //第一个参数指定发送端套接字描述符；第二个参数指明一个存放应用程序要发送数据的缓冲区；
    //第三个参数指明实际要发送的数据的字节数；第四个参数一般置0。
    std::cout << "消息已发送。" << std::endl;
}

//发送文件
void sendFile(int socket) {
    std::string filename;
    std::cout << "请输入文件路径: ";
    std::cin >> filename;
    unsigned char file_md5 [16];
    memset(file_md5,0,sizeof(file_md5));
    get_file_md5(filename.c_str(),file_md5);//获取文件的MD5值
    std::string key3 = "12345678";
    string file1 = DEncode_file(filename,key3);//对文件加密
    std::ifstream file(file1, std::ios::binary);//以二进制模式打开文件
    if (!file) {
        std::cout << "文件未找到。" << std::endl;
        return;
    }

    // 获取文件大小
    file.seekg(0, std::ios::end);//用于在文件流中移动读取位置,相对于文件末尾的偏移量。
    int fileSize = file.tellg();//返回一个int型数值，它表示“内置指针”的当前位置。此函数仅当读取文件时有效
    file.seekg(0, std::ios::beg);//相对于文件开头的偏移量。

    // 发送文件大小
    send(socket, reinterpret_cast<const char*>(&fileSize), sizeof(fileSize), 0);
    //第一个参数指定发送端套接字描述符；第二个参数指明一个存放应用程序要发送数据的缓冲区；
    //第三个参数指明实际要发送的数据的字节数；第四个参数一般置0。

    // 发送文件数据
    char buffer[BUFFER_SIZE];
    while (!file.eof()) {//eof()函数返回文件指针是否已到末尾，是返回true
        file.read(buffer, BUFFER_SIZE);
        send(socket, buffer, file.gcount(), 0);
    }

    file.close();
    std::cout << "文件已发送。" << std::endl;
}

//登陆
void Login(int sock1)
{
    char username[10] = {0};
    char password[9] = {0};
    cout << "请输入用户名：";
    std::cin.getline(username, 10);
    send(sock1, username, strlen(username), 0);
    //cout<<username<<endl;
    cout << "请输入密码：";
    std::cin.getline(password, 9);
    //cout<<password<<endl;
    send(sock1, password, strlen(password), 0);

    return;
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);//创建TCP套接字
    // 成功：返回指向新创建的socket的文件描述符，失败：返回-1
    //af为地址族，也就是IP地址类型，常用的有AF_INET和AF_INET6。
    //type为数据传输方式/套接字类型，常用的有SOCK_STREAM和SOCK_DGRAM
    //protocol表示传输协议，常用的有IPPROTO_TCP和IPPTOTO_UDP，分别表示TCP和UDP传输协议
    if (sock == -1) {
        std::cout << "创建套接字失败。" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;//
    serverAddress.sin_port = htons(PORT);//端口网络字节序
    //将短整型(short)16位的主机字节序数据转化为网络字节序数据。

    //地址ip网络字节序
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0) {
        std::cout << "无效的地址。" << std::endl;
        return 1;
    }

    //connect()用来将参数sock的socket连至参数serverAddress指定的网络地址. 结构sockaddr请参考bind().第三个参数为sockaddr 的结构长度.
    //返回值：成功则返回0, 失败返回-1, 错误原因存于errno 中
    if (connect(sock, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        std::cout << "连接失败。" << std::endl;
        return 1;
    }

    //登陆验证
    int logo = 0;
    do
    {
        Login(sock);
        recv(sock, reinterpret_cast<char*>(&logo), sizeof(logo), 0 );
        //cout << logo << endl;
        if (logo != 1)
        {
            cout<< "登陆失败，请重试" <<endl;
        }
    }while(logo != 1);
    cout<< "登陆成功，请选择传输模式："<<endl;

    int choice;
    do {
        std::cout << "1. 发送消息" << std::endl;
        std::cout << "2. 发送文件" << std::endl;
        std::cout << "3. 退出" << std::endl;
        std::cout << "请输入选择: ";
        std::cin >> choice;
        std::cin.ignore(); // 忽略换行符
        
        //reinterpret_cast:强制类型转化符,int*转变成char*类型
        send(sock, reinterpret_cast<char*>(&choice), sizeof(choice), 0);

        switch (choice) {
            case 1:
                sendMessage(sock);
                break;
            case 2:
                sendFile(sock);
                break;
            case 3:
                std::cout << "正在退出..." << std::endl;
                break;
            default:
                std::cout << "无效的选择，请重试。" << std::endl;
                break;
        }
    } while (choice != 3);

    close(sock);
    return 0;
}
