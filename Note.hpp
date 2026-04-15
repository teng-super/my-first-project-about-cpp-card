#pragma once
#include <ctime>
#include <vector>
#include <string>
#include<utility>
class Note {
private:
	std::string title_;
	std::string context_;
	std::vector<std::string> tags_;
	std::time_t date_;
public:
	Note(std::string title, std::string context, std::vector<std::string> tags) {
		title_ = std::move(title);
		context_ = std::move(context);
		tags_ = std::move(tags);
		date_ = std::time(nullptr);//获取当前时间
	}

	Note(Note&& note) = default;//移动构造
	Note& operator=(Note&& note) = default;//移动赋值

	Note(const Note& note) = delete;//拷贝构造不可用
	Note& operator=(const Note& note) = delete;//拷贝赋值不可用
	//获取每个成员的值的成员函数
	const std::string& getTitle() const { return title_; }
	const std::string& getContext() const { return context_; }
	const std::vector<std::string>& getTags() const { return tags_; }
	std::time_t getDate() const { return date_; }
	//编辑成员的值的成员函数
	void setTitle(const std::string& title) { title_ = title; };
	void setContext(const std::string& context) { context_ = context; };
	void setTags(const std::vector<std::string>& tags) { tags_ = tags; };
	void setDate(const std::time_t& date) { date_ = date; };
};