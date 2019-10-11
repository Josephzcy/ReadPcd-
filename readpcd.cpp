#include "pch.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <math.h>
using namespace std;

struct PointXYZRGBI
{
	float x, y, z ;     //4
	uint32_t  rgb;        //4
	uint8_t intensity,segmentation,ring,angle;   //4
};  
//读取pcd点云文件
void readPCDfile(const string finname,  const string foutname)
{
	std::ifstream fin(finname, ios::in| ios::binary);  //  二进制形式每次只能读取一个字符
	if (fin.bad()) {
		std::cout << "打开文件失败！" << std::endl;
		return;
	}
	cout << "成功打开文件！" << std::endl;
	

	char s[11][1024]; //存储Header
	int Points_Num; //点数量 26654
	std::string data_columns_type; //列数: X Y Z
	std::string data_type; //点坐标存储方式(ascii或者binary)

	//连续读取Header行数   
	std::cout << "start to read file header....." << std::endl;
	std::cout << "file header: " << std::endl;
	cout << "输入指针的当前位置 :" << fin.tellg() << std::endl;
	//输出点云信息  
	for (int i = 0; i < 11; ++i)
	{
		
		fin.getline(s[i],1024);
		cout << "第" << i + 1 << "行：" << s[i] << std::endl;
		
		//FIELDS x y z rgb
		if (i == 2) 
		{
			std::string s2 = s[2];
			size_t pos = s2.find("FIELDS");    //0
			size_t size = s2.size();          //50
			data_columns_type = s2.substr(pos + 7, size);
			std::cout << "data_columns_type:" << data_columns_type << std::endl;
		}
		//当前PCD文件中点的个数num
		if (i == 9) {
			std::string s9 = s[9], Points_Str;
			size_t pos = s9.find("POINTS");
			size_t size = s9.size();                     //0
			Points_Str = s9.substr(pos + 7, size);   
			Points_Num = atoi(Points_Str.c_str());  //把string转化成const char*，返回一个指向正规C字符串的指针常量 
			std::cout << "Points:" << Points_Num << endl;  //26654
		}
		//DATA binary
		if (i == 10) 
		{
			std::string s10 = s[10], DATA_SIZE;
			size_t pos = s10.find("DATA");     //0
			size_t size = s10.size();         
			data_type = s10.substr(pos + 5, size);
			std::cout << "data_type:" << data_type << std::endl;
		}
	}
	
	cout << "输入指针的当前位置 :" << fin.tellg() << std::endl;   //251 
	uint32_t PcdHeaderSize = fin.tellg();
	fin.seekg(0, ios::end);     //53331
	uint32_t PcdDataSize = uint32_t(fin.tellg()) - PcdHeaderSize;   //共53380个字节
	
	
	cout << "start to read point ....." << std::endl;
	std::vector<int8_t> PcdData;
	PcdData.resize(PcdDataSize);            //给vector分配内存
	fin.seekg(PcdHeaderSize, ios::beg);     //251 重新定位指针位置
	
	if ((data_columns_type == "x y z rgb intensity segmentation ring angle") && (data_type == "binary"))
	{
		fin.read(reinterpret_cast<char*>(PcdData.data()), PcdDataSize); //从vector的开头每次读一个字节（也即一个二进制数据到容器中）
		cout << "读取的点云大小：" << PcdData.size() << endl;
	} 
	else
		std::cout << "data_type = binary, read failed!" << std::endl;	

	//点坐标输出txt文本
	std::cout << "start to write point to txt....." << std::endl;
	std::ofstream fout(foutname);

	PointXYZRGBI * pclound = reinterpret_cast<PointXYZRGBI*>(PcdData.data());
	fout << std::setiosflags(std::ios::fixed) << std::setprecision(3)
		 <<"x" << " " << "y" << " " << "z" << " " << "d" << " "
		 << "intensity" << " "
		 << "type" << " "
		 << "ring" << " "
		 << "angle" << " " << endl;

	for (uint32_t i = 0; i < Points_Num; ++i)
	{
		float d = sqrt(pow((*pclound).x, 2) + pow((*pclound).y, 2) + pow((*pclound).y, 2));
		fout << std::setiosflags(std::ios::fixed) << std::setprecision(3)
			<< (*pclound).x << " "
			<< (*pclound).y << " "
			<< (*pclound).z << " "
			<< d << " "
			<< (int)((*pclound).intensity) << " "
			<< (int)((*pclound).segmentation) << " "
			<< (int)((*pclound).ring) << " "
			<< (int)((*pclound).angle) << " " << endl;
		pclound++;
	}
	std::cout << "write point to txt finished!" << std::endl;
}

int main()
{
	string finname = "H:\\bugtest\\19388.pcd";
	string foutame = "H:\\bugtest\\readpcd.txt";
	cout << sizeof(uint8_t) << endl;                    //unsigned char
	cout << "size_t 的类型" << sizeof(size_t) << endl;    //unsigned int 
	cout << "PointXYZRGBI结构体的大小" << sizeof(PointXYZRGBI) << endl;

	readPCDfile(finname, foutame);
	
	system("pause");
	return 0;
}






