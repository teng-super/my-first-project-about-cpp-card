#include "command.hpp"
#include "repository.hpp"
#include "storage.hpp"
#include <iostream>

// ---------------------------------------------------------
// 助手函数声明（把麻烦的逻辑藏在下面，让主控室保持整洁）
// ---------------------------------------------------------
void executeAdd(repository& repo, const std::vector<std::string>& args);
void executeList(const repository& repo);
void executeFind(const repository& repo, const std::vector<std::string>& args);
void executeEdit(repository& repo, const std::vector<std::string>& args);
void executeRemove(repository& repo, const std::vector<std::string>& args);

// ---------------------------------------------------------
// 核心主程序（大堂经理）
// ---------------------------------------------------------
int main() {
    repository repo;
    Storage storage;
    CommandParser parser;

    // 1. 开机读档：召唤档案员把保存在硬盘的卡片唤醒到内存里
    try {
        storage.load(repo);  
        std::cout << "?? MiniNoteHub 启动成功！已成功加载存档。\n";
    } catch (const std::exception& e) {
        std::cout << "?? MiniNoteHub 启动！(提示: " << e.what() << ")\n";
    }

    // 2. 超级死循环：永远倾听用户的控制台输入
    while (true) {
        std::cout << "\nMiniNoteHub> ";
        std::string line;

        // 读取用户键盘敲的一整行
        if (!std::getline(std::cin, line) || line.empty()) {
            continue;
        }

        // 3. 把这句话交给接待员（解析器），换回一张规范的“任务单”
        ParsedCommand cmd = parser.parse(line);

        // 4. 根据任务单上的条形码（CommandType）派发任务
        if (cmd.type == CommandType::ADD) {
            executeAdd(repo, cmd.args);
        } 
        else if (cmd.type == CommandType::LIST) {
            executeList(repo);
        } 
        else if (cmd.type == CommandType::FIND) {
            executeFind(repo, cmd.args);
        } 
        else if (cmd.type == CommandType::EDIT) {
            executeEdit(repo, cmd.args);
        } 
        else if (cmd.type == CommandType::REMOVE) {
            executeRemove(repo, cmd.args);
        } 
        else if (cmd.type == CommandType::QUIT) {
            std::cout << "?? 正在将卡片打包放入保险箱...\n";
            storage.save(repo); // 永远记住：下班关门前一定要存档！
            std::cout << "?? 存档完成，下次再见！\n";
            break; // 只有这里能打破死循环，结束程序
        } 
        else {
            std::cout << "? 不认识这个指令！支持: add, list, find, edit, remove, quit\n";
        }
    }

    return 0;
}

// ---------------------------------------------------------
// 具体脏活累活的实现（眼不见心不烦区域）
// ---------------------------------------------------------

// ?? 逻辑1：添加卡片 (ADD)
void executeAdd(repository& repo, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "? 用法错误！正确格式: add <标题> <正文> [标签1,标签2...]\n";
        return;
    }

    std::string title = args[0];
    std::string content = args[1];
    std::vector<std::string> tags;

    // 如果用户填了标签（第3个参数），用逗号切开它
    if (args.size() >= 3) {
        std::stringstream ss(args[2]);
        std::string singleTag;
        while (std::getline(ss, singleTag, ',')) {
            tags.push_back(singleTag);
        }
    }

    // 组装新实体卡片并抛入仓库
    Note newNote(title, content, tags);
    repo.addNote(std::move(newNote));
    std::cout << "? 成功添加新卡片：[" << title << "]\n";
}

// ?? 逻辑2：查看所有卡片 (LIST)
void executeList(const repository& repo) {
    const auto& all_notes = repo.getNotes();
    if (all_notes.empty()) {
        std::cout << "?? 仓库空空如也，快去 add 点什么吧！\n";
        return;
    }

    std::cout << "\n=== ?? 你的所有卡片 ===\n";
    for (size_t i = 0; i < all_notes.size(); ++i) {
        std::cout << "[" << i << "] " << all_notes[i].getTitle() 
                  << " | " << all_notes[i].getContext() << " | ??? ";

        // 打印它所携带的标签
        const auto& tags = all_notes[i].getTags();
        for (size_t j = 0; j < tags.size(); ++j) {
            std::cout << tags[j] << (j == tags.size() - 1 ? "" : ",");
        }
        std::cout << "\n";
    }
    std::cout << "=======================\n";
}

// ?? 逻辑3：查找卡片 (FIND)
void executeFind(const repository& repo, const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "? 用法错误！正确格式: find <要找的标签>\n";
        return;
    }

    std::string searchTag = args[0];
    // 调用仓库的查找绝技，返回的是一批“只准看不准改”的卡片地址 (const Note*)
    std::vector<const Note*> results = repo.findByTag(searchTag);

    if (results.empty()) {
        std::cout << "?? 找遍了仓库，没发现带有 [" << searchTag << "] 标签的卡片。\n";
    } else {
        std::cout << "?? 找到了 " << results.size() << " 张相关卡片：\n";
        for (const Note* ptr : results) {
            // 通过指针箭头 -> 访问卡片内容
            std::cout << "  -> 标题: " << ptr->getTitle() 
                      << " | 正文: " << ptr->getContext() << "\n";
        }
    }
}

// ?? 逻辑4：编辑卡片 (EDIT)
void executeEdit(repository& repo, const std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cout << "? 用法错误！正确格式: edit <卡片编号> <新标题> <新正文> [新标签1,新标签2...]\n";
        return;
    }

    try {
        // 把用户输入的编号文字变成纯粹的整型数字 size_t
        size_t index = std::stoul(args[0]);

        // 拿到这堆卡片实体里的“真身”（注意这里用的是 & 引用！）
        Note& targetNote = repo.getNoteByIndex(index);

        // 无情篡改真身的属性
        targetNote.setTitle(args[1]);
        targetNote.setContext(args[2]);

        // 如果用户跟了新标签，重新切开覆盖
        if (args.size() >= 4) {
            std::vector<std::string> newTags;
            std::stringstream ss(args[3]);
            std::string tag;
            while (std::getline(ss, tag, ',')) {
                newTags.push_back(tag);
            }
            targetNote.setTags(newTags);
        }
        std::cout << "? 第 " << index << " 号卡片修改成功！\n";

    } catch (const std::out_of_range&) {
        // 抓住了仓库在 getNoteByIndex 时抛出来的越界炸弹
        std::cout << "? 没这张卡！你输入的编号太大了。\n";
    } catch (const std::invalid_argument&) {
        // 抓住了 stoul 把非数字转成数字时报的错
        std::cout << "? 别闹，卡片编号必须填纯数字！\n";
    }
}

// ?? 逻辑5：删除卡片 (REMOVE)
void executeRemove(repository& repo, const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "? 用法错误！正确格式: remove <卡片编号>\n";
        return;
    }

    try {
        size_t index = std::stoul(args[0]);

        // 调用仓库帮你写好的大杀器 deleteCard
        // 注意：因为你在 deleteCard 里越界是返回 false
        if (repo.deleteCard(index)) {
            std::cout << "??? 成功将第 " << index << " 号卡片放入废纸篓！\n";
        } else {
            std::cout << "? 删除失败：找不到这个编号的卡片！\n";
        }
    } catch (...) {
        std::cout << "? 别闹，卡片编号必须填纯数字！\n";
    }
}
