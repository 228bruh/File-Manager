#include "/home/kirill/VS_Code_Files/Git/File-Manager/header/graphic_Functions.h"

void start_screen() {
    std::cout << "Navigation                            \033[32m            ________  ______  __        ________                                                 \033[0m\n";
    std::cout << "======================================\033[32m           |        \\|      \\|  \\      |        \\                                            \033[0m\n";
    std::cout << "\033[0;1mTab\033[0m       |show options              |\033[32m           | $$$$$$$$ \\$$$$$$| $$      | $$$$$$$$                                               \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m           | $$__      | $$  | $$      | $$__                                                    \033[0m\n";
    std::cout << "\033[0;1m↓ ↑\033[0m       |navigate through the items|\033[32m           | $$  \\     | $$  | $$      | $$  \\                                                 \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m           | $$$$$     | $$  | $$      | $$$$$                                                   \033[0m\n";
    std::cout << "\033[0;1mEnter\033[0m     |enter dir                 |\033[32m           | $$       _| $$_ | $$_____ | $$_____                                                 \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m           | $$      |   $$ \\| $$     \\| $$     \\                                             \033[0m\n";
    std::cout << "\033[0;1mBackspace\033[0m |back to previous dir      |\033[32m            \\$$       \\$$$$$$ \\$$$$$$$$ \\$$$$$$$$                                            \033[0m\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "\033[0;1mq\033[0m         |quit                      |\033[32m            __       __   ______   __    __   ______    ______   ________  _______               \033[0m\n";
    std::cout << "======================================\033[32m           |  \\     /  \\ /      \\ |  \\  |  \\ /      \\  /      \\ |        \\|       \\     \033[0m\n";
    std::cout << "                                      \033[32m           | $$\\   /  $$|  $$$$$$\\| $$\\ | $$|  $$$$$$\\|  $$$$$$\\| $$$$$$$$| $$$$$$$\\       \033[0m\n";
    std::cout << "Edit                                  \033[32m           | $$$\\ /  $$$| $$__| $$| $$$\\| $$| $$__| $$| $$ ____ | $$__    | $$__| $$           \033[0m\n";
    std::cout << "======================================\033[32m           | $$$$\\  $$$$| $$   \\$$| $$$$\\ $$| $$   \\$$| $$|    \\| $$  \\   | $$   \\$$      \033[0m\n";
    std::cout << "\033[0;1m+\033[0m         |create item in current dir|\033[32m           | $$\\$$ $$ $$| $$$$$$$$| $$\\$$ $$| $$$$$$$$| $$ \\$$$$| $$$$$   | $$$$$$$\\         \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m           | $$ \\$$$| $$| $$  | $$| $$ \\$$$$| $$  | $$| $$__| $$| $$_____ | $$  | $$           \033[0m\n";
    std::cout << "\033[0;1m-\033[0m         |delete selected item      |\033[32m           | $$  \\$ | $$| $$  | $$| $$  \\$$$| $$  | $$ \\$$    $$| $$     \\| $$  | $$         \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m            \\$$      \\$$ \\$$   \\$$ \\$$   \\$$ \\$$   \\$$  \\$$$$$$  \\$$$$$$$$ \\$$   \\$$ \033[0m\n";
    std::cout << "\033[0;1mr\033[0m         |rename selected item      |\n";
    std::cout << "======================================\n";
    std::cout << "\n";
    std::cout << "Manipulations\n";
    std::cout << "======================================           \033[33m┌─┐___\033[0m                  ┌───┐                 \033[33;2m┌───┐\033[0m\n";
    std::cout << "\033[0;1mc\033[0m         |copy selected item        |           \033[33m│  _  │\033[0m folder          │~~~│ document        \033[33;2m│ ↓ │\033[0m archive\n";
    std::cout << "----------|--------------------------|           \033[33m└─┴─┴─┘\033[0m                 └───┘                 \033[33;2m└───┘\n";
    std::cout << "\033[0;1mv\033[0m         |paste copied item         |\n";
    std::cout << "----------|--------------------------|           \033[34m┌───┐\033[31m                   ┌───┐                 \033[90m┌───┐\033[0m                 ┌───┐\n";
    std::cout << "\033[0;1mm\033[0m         |move selected item        |           \033[34m│◠˚✧│\033[0m image             \033[31m│ ▶ │ \033[0mvideo           \033[90m│ ♫ │\033[0m audio           │ ⚙ │ other\n";
    std::cout << "----------|--------------------------|           \033[34m└───┘\033[31m                   └───┘                 \033[90m└───┘\033[0m                 └───┘\n";
    std::cout << "\033[0;1ms\033[0m         |file search               |\n";
    std::cout << "======================================\n";

    std::cout << "\nPress \033[0;1mEnter\033[0m to continue: ";
    std::cin.ignore();
}

void disableCanonicInput(struct termios& oldt, struct termios& newt) {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void enableCanonicInput(struct termios& oldt) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void displayFiles(const std::vector<Item>& items, int currentItem, std::string copiedItemPath, std::string movingItemPath, std::string start_dir) {
    std::cout << "\033c";
    std::cout << " \033[0;1m" << start_dir << "  \033[0m|\n";
    for(int i = 0; i < start_dir.size() + 3; i++) {
        std::cout << "_";
    }
    std::cout << "|\n\n";

    if (items.empty()) {
        std::cout << "This folder is empty\n";
    } else {

    for (size_t i = 0; i < items.size(); ++i) {
        if (i == currentItem) {
            std::string type_str;
            switch (items[i].type) {
                case Item::Type::Folder:
                    type_str = "   \033[33m┌─┐___\n\033[32m->\033[33m │  _  │   \033[32m" + items[i].name + "\n\033[33m   └─┴─┴─┘";
                    break;
                case Item::Type::Document:
                    type_str = "   ┌───┐\n\033[32m->\033[0m │~~~│     \033[32m" + items[i].name + "\n\033[0m   └───┘";
                    break;
                case Item::Type::Image:
                    type_str = "   \033[34m┌───┐\n\033[32m->\033[34m │◠˚✧│     \033[32m" + items[i].name + "\n\033[34m   └───┘"; 
                    break;
                case Item::Type::Audio:
                    type_str = "   \033[90m┌───┐\n\033[32m->\033[90m │ ♫ │     \033[32m" + items[i].name + "\n\033[90m   └───┘";
                    break;
                case Item::Type::Video:
                    type_str = "   \033[31m┌───┐\n\033[32m->\033[31m │ ▶ │     \033[32m" + items[i].name + "\n\033[31m   └───┘";
                    break;
                case Item::Type::Archive:
                    type_str = "   \033[33;2m┌───┐\n\033[0m\033[32m->\033[33;2m │ ↓ │     \033[0m\033[32m" + items[i].name + "\n\033[33;2m   └───┘\033[0m";
                    break;
                case Item::Type::Other:
                    type_str = "   ┌───┐\n\033[32m->\033[0m │ ⚙ │     \033[32m" + items[i].name + "\n\033[0m   └───┘";
                    break;
            }

            std::cout << type_str;

            if (items[i].is_folder) {
                std::cout << "\033[0m\n";
            } else {
                std::cout << "     \033[32m" << items[i].size << " MB\033[0m\n";
            }    

        } else {
            std::string type_str;
            switch (items[i].type) {
                case Item::Type::Folder:
                    type_str = "\033[33m     ┌─┐___\n     │  _  │ \033[0m" + items[i].name +"\n\033[33m     └─┴─┴─┘";
                    break;
                case Item::Type::Document:
                    type_str = "     ┌───┐\n     │~~~│   " + items[i].name + "\n     └───┘"; 
                    break;
                case Item::Type::Image:
                    type_str = "\033[34m     ┌───┐\n     │◠˚✧│   \033[0m" + items[i].name +"\n\033[34m     └───┘";
                    break;
                case Item::Type::Audio:
                    type_str = "\033[90m     ┌───┐\n     │ ♫ │   \033[0m" + items[i].name +"\n\033[90m     └───┘";
                    break;
                case Item::Type::Video:
                    type_str = "\033[31m     ┌───┐\n     │ ▶ │   \033[0m" + items[i].name +"\n\033[31m     └───┘";
                    break;
                case Item::Type::Archive:
                    type_str = "\033[33;2m     ┌───┐\n     │ ↓ │   \033[0m" + items[i].name +"\n\033[33;2m     └───┘";
                    break;
                case Item::Type::Other:
                    type_str = "     ┌───┐\n     │ ⚙ │   " + items[i].name + "\n     └───┘";
                    break;
            }

            std::cout << type_str;

            if (items[i].is_folder) {
                std::cout << "\033[0m\n";
            } else {
                std::cout << "   \033[0m" << items[i].size << " MB\n";
            }  
        }
    }

    }

    if (copiedItemPath != "") {
        std::cout << "\n";
        for(int i = 0; i < copiedItemPath.size() + 13; i++) {
            std::cout << "_";
        }
        
        std::cout << "\nClipboard: \033[0;1m" << copiedItemPath << "\033[0m |\n";
    }

    if (movingItemPath != "") {
        std::cout << "\n";
        for(int i = 0; i < movingItemPath.size() + 10; i++) {
            std::cout << "_";
        }
        
        std::cout << "\nMoving: \033[0;1m" << movingItemPath << "\033[0m |\n";
    }
}
