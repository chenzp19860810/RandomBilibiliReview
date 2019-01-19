#define _CRT_SECURE_NO_WARNINGS
#define cls() system("cls");
#define pause() system("pause");

#define UNICODE() SetConsoleOutputCP(65001)
#define ANSI() SetConsoleOutputCP(936)

#include <afx.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <fstream>
#include <document.h>
#include <iostream>
#include <Urlmon.h>
#include <direct.h>
#include <time.h>
#include <math.h>
#include <sstream>
#include <atlstr.h>

#pragma comment(lib, "urlmon.lib")

using namespace std;
using namespace rapidjson;

int main()
{
	ANSI();

	string aid,temp,temp2;
	int feedback = 0, winning = 0, page = 0;
	bool valid = false;
	Document document;

	_mkdir("feedback");

	printf("-----------------------设置-----------------------\n");

	printf("视频aid:");
	getline(cin,aid);

	printf("评论页数:");
	scanf("%d", &page);

	temp = "https://api.bilibili.com/x/v2/reply?jsonp=jsonp&pn=1&type=1&oid="+ aid +"&sort=0";
	URLDownloadToFileA(NULL, temp.c_str(), "feedback/feedback.json", NULL, NULL);

	ifstream in("feedback/feedback.json");
	istreambuf_iterator<char> begin(in);
	istreambuf_iterator<char> end;
	string json(begin, end);

	document.Parse(json.c_str());

	feedback = document["data"]["page"]["count"].GetInt();
	cls();

	printf("视频评论数量:%d\n",feedback);
	page = (int)ceil(feedback / 20);

	while (valid == false)
	{
		srand((unsigned int)time(NULL));
		winning = (rand() % (feedback - 1 + 1)) + 1;
		printf("%d\n", winning);

		for (int i = 1; i <= page; i++)
		{
			temp = "https://api.bilibili.com/x/v2/reply?jsonp=jsonp&pn=" + to_string(i) + "&type=1&oid=" + aid + "&sort=0";
			temp2 = "feedback/feedback" + to_string(i) + ".json";
			URLDownloadToFileA(0, temp.c_str(), temp2.c_str(), 0, NULL);

			ifstream in(("feedback/feedback" + to_string(i) + ".json"));
			istreambuf_iterator<char> begin(in);
			istreambuf_iterator<char> end;
			string json(begin, end);
			document.Parse(json.c_str());

			const Value& comment = document["data"]["replies"];
			for (auto& v : comment.GetArray())
			{
				if (v["floor"].GetInt() == winning)
				{
					string commentMessage = v["content"]["message"].GetString();
					string commentName = v["member"]["uname"].GetString();

					printf("中奖楼层:%d\n", winning);
					printf("中奖用户:");
					UNICODE();
					printf("%s\n",commentName.c_str());
					ANSI();
					printf("中奖用户评论:");
					UNICODE();
					printf("%s\n", commentMessage.c_str());
					ANSI();
					
					valid = true;

					RemoveDirectory(L"feedback");
					pause();
					return 0;
				}
			}
		}
	}
	pause();
	return 0;
}