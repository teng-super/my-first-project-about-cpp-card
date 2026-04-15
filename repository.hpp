#pragma once
#include "Note.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <memory>
class repository {
	//实现卡片的增删查改
private:
	std::vector<Note> Notes;//存储卡片的容器
public:
	//增添卡片
	void addNote(Note&& note) {
		Notes.push_back(std::move(note));
	}
	//获取所有卡片
	const std::vector<Note>& getNotes() const {
		return Notes;
	}
	//查看卡片
	//1.通过索引查看，可更改卡片
	Note& getNoteByIndex(size_t index) {
		if (index < Notes.size()) {
			return Notes[index];
		}
		throw std::out_of_range("Index out of range");//异常抛出语法，throw表示抛出一个异常，out_of_range表示范围越界
	}
	//2.通过标题查看，返回一个指向卡片的指针，若未找到则返回nullptr
	Note* getNoteByTitle(const std::string Title) {
		for (auto& note : Notes) {
			if (note.getTitle() == Title) {
				return &note;
			}
		}
		return nullptr;
		/*for (const auto& note : Notes) {
			if(note->getTitle().find(Title)!=std::string::npos){
			return &note;
			}
			return nullptr;
		*/
	}
	//通过标签查看，返回一个包含所有匹配卡片的向量
	const std::vector<const Note*> findByTag(const std::string& Tag) const {//传入一个带有所有待查询标签的数组
		std::vector<const Note*>result; //存放结果的容器
		for (const auto& note : Notes) {
			const auto& tags = note.getTags();
			if (std::find(tags.begin(), tags.end(), Tag) != tags.end()) {
				result.push_back(&note);
			}
		}
		return result;
	}
	//删除卡片
	bool deleteCard(size_t index) {
		if (index > Notes.size()) {
			return false;
		}
		Notes.erase(Notes.begin() + index);
		return true;
	}
	//清空仓库
	void clear() {
		Notes.clear();
	}
	//获得数量
	size_t size() const {
		return Notes.size();
	}
};