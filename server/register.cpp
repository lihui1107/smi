#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#define SIZE 50
//为了安全的输入（防止暴力输入）
char get_choice();	
long get_tele();
char get_first();
char getch();
char * s_gets(char * st, int n);
void secret_pwd(char * pwd);
//具体功能实现函数
void create_user_file();
void register_user();
void login();
void find_pwd();
//加密解密函数
void encipher(char * original);
void decipher(char * cipher);
//菜单函数
void show_line(const char * line);
void user_menu();

typedef struct user		//用户信息主体
{
	char id[SIZE];		//账号
	char pwd[SIZE];		//密码
	long tele;			//电话号码
}user;

int main()
{
	user_menu();
	puts("请按任意键继续...");
	getch();	//模拟任意键继续
	puts("欢迎下次使用！");
	return 0;
}
//保证文件users的存在，该文件存在时也不清空其中的数据
void create_user_file()
{
	FILE * fp;

	if ((fp = fopen("users", "r")) == NULL)	
	{
		if ((fp = fopen("users", "w+")) == NULL)
		{
			fprintf(stderr, "无法打开用户信息!\n");
			//system("sleep 3s");
			return;
		}
	}
	fclose(fp);

	return;
}
//显示主界面并获取用户键入的信息
char get_choice(void)
{
	char ch;

	ch =get_first();
	while(ch != 'r' && ch != 'q')
	{
		puts("请输入正确选项");
		ch =get_first();
	}

	return ch;
}
//返回键入的第一个字母字符并清空缓存区
char get_first(void)
{
    char ch = 0;

    while (!isalpha(ch = getchar()))
        continue;
	while (getchar() != '\n')
		continue;

    return tolower(ch);	 //返回小写值
}
//注册函数
void register_user()
{
	FILE * fp;
	user new_, old;		//这里new后面多'_'的原因是c++有个关键字是new
	char temp[SIZE]; 

	show_line("请开始注册您的账号!");
	
	if ((fp = fopen("users", "r")) == NULL)
	{
		fprintf(stderr, "读取用户信息失败！\n");
		//system("sleep 3s");
		return;
	}
	puts("请输入您的账号：");
	s_gets(new_.id, SIZE);
	while (!feof(fp))
	{
		fread(&old, sizeof(user), 1, fp);
		decipher(old.pwd);
		if (!strcmp(new_.id, old.id))
		{
			fprintf(stderr, "账号重复！\n");
			//system("sleep 3s");
			return;
		}
	}
	puts("请输入您的密码：");
	secret_pwd(new_.pwd);
	puts("请再次输入您的密码：");
	secret_pwd(temp);
	while (1)
	{
		if (!strcmp(new_.pwd, temp))
			break;
		puts("两次密码不一致！");
		puts("请重新输入您的密码：");
		secret_pwd(new_.pwd);
		puts("请再次输入您的密码：");
		secret_pwd(temp);
	}
	puts("请输入您的电话号码以便找回密码：");
	new_.tele = get_tele();
	fp = fopen("users", "a");
	encipher(new_.pwd);
	fwrite(&new_, sizeof(user), 1, fp);
	printf("账号%s已注册成功！\n", new_.id);
	//system("sleep 3s");
	
	fclose(fp);
	return;
}
//安全地获取电话号码
//64位架构下long最大值是9223372036854775807，用来存储电话号码很合适
long get_tele()
{
	long input;
	char ch;

	while (scanf("%ld", &input) != 1) 
	{
		while ((ch =getchar()) != '\n')
			putchar(ch);
		printf(" 不是一个电话号码！\n请您重新输入：\n");
	}

	return input;
}
//登录函数
void login()
{
	FILE * fp;
	user old;
	char temp[SIZE];

	show_line("请开始登录您的账号！");

	if ((fp = fopen("users", "r")) == NULL)
	{
		fprintf(stderr, "读取用户信息失败！\n");
		system("sleep 3s");
		return;
	}
	puts("请输入您的账号：");
	s_gets(temp, SIZE);
	while (!feof(fp))	//判断是否到达文件结尾
	{
		fread(&old, sizeof(user), 1, fp);
		decipher(old.pwd);
		if (!strcmp(temp, old.id))
		{
			puts("请输入您的密码：");
			secret_pwd(temp);
			if (!strcmp(temp, old.pwd))
			{
				show_line("登录成功！");
				system("sleep 2s");	//休眠，用起来比较带感哈哈哈哈
				exit(0); //想要换页面就在这里做文章
			}
			else
			{
				puts("密码不匹配！");
				system("sleep 3s");
				return;
			}
		}
	}
	printf("账号%s不存在！\n", temp);
	system("sleep 3s");

	fclose(fp);
	return;
}
//找回密码函数
void find_pwd()
{
	FILE * fp;
	user old;
	char temp[SIZE];
	long key;

	show_line("请开始找回您的密码!");

	if ((fp = fopen("users", "r")) == NULL)
	{
		fprintf(stderr, "读取用户信息失败！\n");
		system("sleep 3s");
		return;
	}

	puts("请输入您的账号：");
	s_gets(temp, SIZE);
	while (!feof(fp))
	{
		fread(&old, sizeof(user), 1, fp);
		decipher(old.pwd);
		if (!strcmp(temp, old.id))
		{
			puts("请输入您的电话号码：");
			if ((key = get_tele()) == old.tele)
				printf("您的密码是：%s\n", old.pwd);
			else
				puts("电话号码不匹配！");
			system("sleep 3s");
			return;
		}
	}
	printf("账号%s不存在！\n", temp);
	system("sleep 3s");

	fclose(fp);
	return;
}

char * s_gets(char * st, int n)
{
	char * ret_val;
	int i = 0;

	ret_val = fgets(st, n, stdin);
	if (ret_val != NULL)
	{
		while (st[i] != '\0' && st[i] != '\n')
			i++;
		if (st[i] == '\n')
			st[i] = '\0';
		else
			while (getchar() != '\n')
				continue;
	}
	return ret_val;
}
//自定义的getch()函数
char getch()
{
	char ch;

    system("stty -echo");
    system("stty -icanon");
    ch = getchar();
    system("stty icanon");
    system("stty echo");

    return ch;
}

void secret_pwd(char * pwd)
{
	char ch;
	int i = 0;

	while ((ch = getch()) != '\n')
	{
		if (ch == '\x7F')
		{
			putchar('\b');
			putchar(' ');
			putchar('\b');
			if (i)
				i--;
			else
				putchar('\a');
		}
		else
		{
			putchar('*');
			pwd[i++] = ch;
		}
	}
	pwd[i] = '\0';
	putchar('\n');

	return;
}
//加密函数
void encipher(char * original)
{
    int key = 1;

    for (int i = 0; i < strlen(original); i++)
    {
		original[i] += key;
		key=-key;
    }

	return;
}
//解密函数
void decipher(char * cipher)
{
    int key = -1;
    for (int i = 0; i < strlen(cipher); i++)
    {
        cipher[i] += key;
        key = -key;
    }

	return;
}
//展示某句话
void show_line(const char * line)
{
	//system("clear");	//清屏函数
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
	printf("\n%s\n\n", line);
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');	

	return;
}

void user_menu()
{
	char choice;			//记录get_choice()返回值

	/*show_line("欢迎使用Linux版登录系统!");
//用字符串来输出是一个比较好用的技巧，来保证对齐
    printf("\n%-20s%-20s\n%-20s%-20s\n\n", "a. 注册账号", "b. 登录账号", "c. 找回密码", "q. 退出登录");
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
	create_user_file();
	while ((choice = get_choice()) != 'q')
	{
		switch(choice)
		{
			case 'a': register_user();
					  break;
			case 'b': login();
					  break;
			case 'c': find_pwd();
					  break;
		}
		show_line("欢迎使用Linux版登录系统!");
	    printf("\n%-20s%-20s\n%-20s%-20s\n\n", "a. 注册账号", "b. 登录账号", "c. 找回密码", "q. 退出登录");
		for (int i =0; i < 40;i++)
		putchar('*');
     	putchar('\n');
	}*/
    
    show_line("欢迎使用Linux版注册系统!");
//用字符串来输出是一个比较好用的技巧，来保证对齐
    printf("\n%-20s%-20s\n\n", "r. 注册账号", "q. 退出登录");
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
    create_user_file();
    while ((choice = get_choice()) != 'q')
	{
		if(choice = 'r')
		  register_user();
		show_line("欢迎使用Linux版注册系统!");
	    printf("\n%-20s%-20s\n\n", "r. 注册账号", "q. 退出登录");
		for (int i =0; i < 40;i++)
		putchar('*');
     	putchar('\n');
	}
    
	return;
}
