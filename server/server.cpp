//服务端
#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <openssl/md5.h>
#include<cstdio>
#include<string>
#include<algorithm>
#include "verify.cpp"
#include "Des_test_2.h"
#include "Des_filecode.h"

#define PORT 8080
#define BUFFER_SIZE 1024
using namespace std;
string key = "12345687";

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

void get_md5(const char input[],unsigned char digest[MD5_DIGEST_LENGTH]) {
    // char input[] = "4152";  // 要进行MD5加密的输入数据
    // unsigned char digest[MD5_DIGEST_LENGTH];  // 存储MD5结果的缓冲区
    MD5((const unsigned char*)input, strlen(input), digest);// 进行MD5计算
    printf("消息MD5值为:");
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) { // 将MD5结果以十六进制形式打印输出
        printf("%02x", digest[i]);
    }
    printf("\n");
}

void receiveMessage(int clientSocket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytesRead > 0) {
        std::string key3 = "12345678";
        string output2 = DDecode(buffer, key3);
        unsigned char md5 [16];
        memset(md5,0,sizeof(md5));
        std::cout << "收到消息: " << output2 << std::endl;
        get_md5(output2.c_str(),md5);
    } else {
        std::cout << "接收消息时发生错误。" << std::endl;
    }
}

void receiveFile(int clientSocket) {
    int fileSize;
    recv(clientSocket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

    char buffer[BUFFER_SIZE];
    std::ofstream file("recv", std::ios::binary);
    if (!file) {
        std::cout << "无法创建接收文件。" << std::endl;
        return;
    }

    int bytesReceived = 0;
    while (bytesReceived < fileSize) {
        int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesRead <= 0) {
            std::cout << "接收文件时发生错误。" << std::endl;
            break;
        }

        file.write(buffer, bytesRead);
        bytesReceived += bytesRead;
    }

    file.close();
    std::string key3 = "12345678";
    string file2 = DDecode_file("recv", key3);
    unsigned char file_md5 [16];
    memset(file_md5,0,sizeof(file_md5));
    get_file_md5(file2.c_str(),file_md5);
    std::cout << "文件接收完成。" << std::endl;
}

//接收登陆信息并验证
void Loginserver(int clinetSocket1)
{
    int logo = 0;
    while (logo == 0)
    {
        char username[10] = {0};
        char password[9] = {0};
        cout << "等待接收用户/密码..." << endl;
        recv(clinetSocket1, username, sizeof(username), 0 ) ;
        recv(clinetSocket1, password, sizeof(password), 0 ) ;
        cout << "用户/密码接收成功" << endl;
        cout << "用户/密码：" ;
        cout << username <<" : "<< password <<endl;
        //登陆验证函数
        logo = login(username, password);
        send(clinetSocket1, reinterpret_cast<char*>(&logo), sizeof(logo), 0);
    }
 
    return;
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress{}, clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cout << "创建套接字失败。" << std::endl;
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        std::cout << "绑定套接字失败。" << std::endl;
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        std::cout << "监听失败。" << std::endl;
        return 1;
    }

    std::cout << "等待客户端连接..." << std::endl;

    clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddressLength);
    if (clientSocket < 0) {
        std::cout << "接受客户端连接失败。" << std::endl;
        return 1;
    }

    std::cout << "客户端连接成功。" << std::endl;

    //登陆验证
    Loginserver(clientSocket);

    int choice;
    do {
        std::cout << "等待客户端操作..." << std::endl;
        recv(clientSocket, reinterpret_cast<char*>(&choice), sizeof(choice), 0);

        switch (choice) {
            case 1:
                receiveMessage(clientSocket);
                break;
            case 2:
                receiveFile(clientSocket);
                break;
            case 3:
                std::cout << "客户端请求退出。" << std::endl;
                break;
            default:
                std::cout << "无效的选择。" << std::endl;
                break;
        }
    } while (choice != 3);

    close(clientSocket);
    close(serverSocket);

    return 0;
}