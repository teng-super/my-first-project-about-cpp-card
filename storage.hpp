#pragma once
#include"Note.hpp"
#include"repository.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include<cstring>
class Storage {
public:
	constexpr static const char* filename = "notes.txt";
	//保存卡片到文件
	//为什么保存时用ofstream？因为需要程序把数据吐到硬盘上来保存，而ofstream正好是用来输出数据到文件的流类，可以方便地将数据写入文件中。
	void save(const repository& repo) {
		std::ofstream out(filename);
		if (!out.is_open()) {
			throw std::runtime_error("我去，没打开？你再检查检查");
		}
		for (const auto& note : repo.getNotes()) {
			out<< note.getTitle() << "|" << note.getContext() << "|";
			//搞定标签数组
			const auto& tags = note.getTags();
			//将标签全部拿出
			for (size_t i = 0; i < tags.size(); ++i) {
				out << tags[i];
				if (i != tags.size() - 1) {
					out << ",";
				}
			}
			out << "|" << note.getDate() << "\n";
			//写完了，关闭文件
		}
	}
	void load(repository& repo) {
		std::ifstream in(filename);
		if (!in.is_open()) {
			throw std::runtime_error("打开了一片空气");
		}
		repo.clear();//加载前先清空仓库
		std::string line;//用来装每次读出的一行
		//只要还能读入就不要停
		while (std::getline(in,line)){
			std::stringstream ss(line);//为该字符串“套上一个文件流的接口”，从文件中逐词、逐行读取数据
			//准备四个分别装四个成员变量的函数
			std::string title, content, tags_str, data_str;
			//遇到|就分割
			std::getline(ss, title, '|');
			std::getline(ss, content, '|');
			std::getline(ss, tags_str, '|');
			std::getline(ss, data_str, '|');
			//处理标签数组
			std::vector<std::string> real_tags;
			std::stringstream tags_ss(tags_str);
			std::string tag;
			while (std::getline(tags_ss, tag, ',')) {
				real_tags.push_back(tag);//在tags_ss这个流里面，每次都把逗号前的标签读出来，放到tag里，再把tag放到real_tags这个数组里
			}
			//处理时间
			std::time_t date = std::stoll(data_str);//stoll是string to long long的缩写，可以把字符串转换成长整型
			//构造
			Note nemNote(title, content, real_tags);
			nemNote.setDate(date);//存入时间
			//移动语义之移动赋值
			repo.addNote(std::move(nemNote));
		}
		in.close();
	}
};