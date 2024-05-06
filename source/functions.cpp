#include "/home/kirill/VS_Code_Files/Git/File-Manager/header/functions.h"

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

Item::Item() : is_folder(false), name(""), type(Type::Other), size(0) {}
Item::Item(const std::string& name, Type type, bool is_folder, double size)
    : name(name), type(type), is_folder(is_folder), size(size) {}

Item::Type Item::getFileType(const std::string& name) {
    std::size_t pos = name.find_last_of('.');
    if (pos != std::string::npos) {
        std::string extension = name.substr(pos + 1);

        if (extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "gif" || extension == "bmp") {
            return Item::Type::Image;
        } else if (extension == "mp3" || extension == "wav" || extension == "flac" || extension == "aac") {
            return Item::Type::Audio;
        } else if (extension == "mp4" || extension == "avi" || extension == "mkv" || extension == "mov") {
            return Item::Type::Video;
        } else if (extension == "zip" || extension == "rar" || extension == "tar" || extension == "gz" || extension == "jar") {
            return Item::Type::Archive;
        } else if (extension == "txt" || extension == "pdf" || extension == "doc" || extension == "docx") {
            return Item::Type::Document;
        }
    }
    return Item::Type::Other;
}

bool compareItemsByName(const Item& a, const Item& b) {
    if (a.is_folder && !b.is_folder) {
        return true;
    } else if (!a.is_folder && b.is_folder) {
        return false;
    } else {
        return a.name < b.name;
    }
}

std::vector<Item> set_list_of_current_dir(std::string start_dir) {
    std::vector<Item> items;
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    if ((dir = opendir(start_dir.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            const char* name = ent->d_name;
            if (name[0] != '.') {
                Item item;
                item.name = name;
                std::string full_path = start_dir + "/" + name;
                if (stat(full_path.c_str(), &st) == 0) {
                    item.is_folder = S_ISDIR(st.st_mode);
                    if (item.is_folder) {
                        item.type = Item::Type::Folder;
                    } else {
                        item.size = st.st_size / (1000.0 * 1000.0);
                        item.size = std::round(item.size * 1000.0) / 1000.0;
                        Item file;
                        item.type = file.getFileType(name);
                    }
                    items.push_back(item);
                }
            }
        }
        closedir(dir);

        std::sort(items.begin(), items.end(), compareItemsByName);
    } else {
        std::cerr << "Error opening directory\n";
    }
    return items;
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

void createItem(std::string start_dir) {
    std::cout << "\033c";
    std::cout << " \033[0;1m" << start_dir << "  \033[0m|\n";
    for(int i = 0; i < start_dir.size() + 3; i++) {
        std::cout << "_";
    }
    std::cout << "|\n\n";

    std::cout << "Create item\n";
    std::cout << "Enter name: ";
    std::string itemName;

    std::cin >> itemName;
            
    std::string itemPath = start_dir + "/" + itemName;
        
    struct stat buffer;
    while (stat(itemPath.c_str(), &buffer) == 0) {
        std::cout << "\n\nItem with the same name '\033[0;1m" << itemName << "\033[0m' already exists in this directory\n";
        std::cout << "Enter name: ";
                
        std::cin >> itemName;

        itemPath = start_dir + "/" + itemName;
        if (stat(itemPath.c_str(), &buffer) != 0) {
            break;
        }
    }

    bool isFolder = (itemName.find('.') == std::string::npos); 

    if (isFolder) {
        mkdir(itemPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    } else {
        std::ofstream file(itemPath);
    }
}

bool removeItem(const std::string& path) {
    std::string rmCommand = "rm -r '" + path + "'";

    if (system(rmCommand.c_str()) != 0) {
        std::cerr << "Error removing directory: " << path << std::endl;
        return false;
    }

    return true;
}

void confirmAndDelete(const std::string& path) { 
    std::cout << "\n\nAre you sure you want to delete this item: \033[0;1m" << path << "\033[0m? (y/n)";
    char response;
    std::cin >> response;

    if (response == 'y' || response == 'Y') {
   
        if (removeItem(path)) {
            std::cout << "\nItem successfully deleted.\n";
        } else {
            std::cout << "\nFailed to delete item.\n";
        }

    } else {
        std::cout << "\nDeletion canceled.\n";
    }

    std::cout << "\nPress " << "\033[0;1m" << "any key" << "\033[0m" << " to continue: ";
    std::cin.ignore();
}

void renameItem(std::string start_dir, std::vector<Item>& items, int currentSelection) {
    std::cout << "\033c";
    std::cout << " \033[0;1m" << start_dir << "  \033[0m|\n";
    for(int i = 0; i < start_dir.size() + 3; i++) {
        std::cout << "_";            
    }
    std::cout << "|\n\n";

    std::cout << "Rename this item: \033[0;1m'" << items[currentSelection].name << "'\033[0m\n";
    std::cout << "Enter new name: ";
    std::string newName;
            
    std::cin >> newName;

    std::string newPath = start_dir + "/" + newName;
            
    struct stat buffer;
    while (stat(newPath.c_str(), &buffer) == 0) {
        std::cout << "\n\nItem with the name '\033[0;1m" << newName << "\033[0m' already exists in this directory\n";
        std::cout << "Enter new name: ";

        std::cin >> newName;
                
        newPath = start_dir + "/" + newName;
        if (stat(newPath.c_str(), &buffer) != 0) {
            break;
        }
    }

    Item& currentItem = items[currentSelection];
    std::string oldPath = start_dir + "/" + currentItem.name;
    rename(oldPath.c_str(), newPath.c_str());
}

void searchFile(const char* dir_name, const std::string& item_name, std::vector<std::string>& results) {
    DIR* dir = opendir(dir_name);
    if (dir == nullptr) {
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_name[0] != '.' && entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            std::string subdir_name = std::string(dir_name) + "/" + entry->d_name;
            searchFile(subdir_name.c_str(), item_name, results);
        } else if (entry->d_name[0] != '.' && entry->d_type == DT_REG && strcmp(entry->d_name, item_name.c_str()) == 0) {
            std::string file_path = std::string(dir_name) + "/" + entry->d_name;
            results.push_back(file_path);
        }
    }

    closedir(dir);
}

void searchMenu() {
    std::cout << "\033c";
            
    std::string item_name;
    std::cout << "Search files |\n";
    std::cout << "_____________|\n\n";
    std::cout << "Enter file name: ";

    std::cin >> item_name;

    std::vector<std::string> results;

    searchFile("/home", item_name, results); 

    if (!results.empty()) {
        std::cout << "\n\nFile(s) found:\n";
        for (const auto& file : results) {
            std::cout << "\033[0;1m" << file << "\033[0m\n"; 
        }
    } else {
        std::cout << "\n\nFile '\033[0;1m" << item_name << "\033[0m' not found :(\n";
    }

    std::cout << "\nPress " << "\033[0;1m" << "Backspace" << "\033[0m" << " to quit search: ";
}