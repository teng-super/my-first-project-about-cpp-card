#pragma once
#include<string>
#include<vector>
#include<sstream>
enum class CommandType{
    ADD,     // 添加卡片
    LIST,    // 列出所有卡片
    FIND,    // 查找卡片
    EDIT,    // 编辑卡片
    REMOVE,  // 删除卡片
    QUIT,    // 退出程序
    UNKNOWN  // 未知命令
};
struct ParsedCommand {
    CommandType type = CommandType::UNKNOWN;  // 要干什么事
    std::vector<std::string> args;            // 干这件事需要什么材料
};
class CommandParser {
public:
    ParsedCommand parse(const std::string& line) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;// 先把输入切成一个个单词
        std::string token;// 以空格为分隔符,放进来
        while (iss >> token) {
            tokens.push_back(token);//在istringstream里面会自动把空格当成分隔符,所以每次读取到一个单词就放进tokens里
        }
        if (tokens.empty()) {
            return { CommandType::UNKNOWN, {} };
        }
        ParsedCommand cmd;
        const std::string& cmdName = tokens[0];
        if (cmdName == "add") cmd.type = CommandType::ADD;
        else if (cmdName == "list") cmd.type = CommandType::LIST;
        else if (cmdName == "find") cmd.type = CommandType::FIND;
        else if (cmdName == "edit") cmd.type = CommandType::EDIT;
        else if (cmdName == "remove" || cmdName == "rm") cmd.type = CommandType::REMOVE;
        else if (cmdName == "quit" || cmdName == "exit") cmd.type = CommandType::QUIT;
        else cmd.type = CommandType::UNKNOWN;
        if (tokens.size() > 1) {
            cmd.args.assign(tokens.begin() + 1, tokens.end());
        }
        return cmd;
    }
};