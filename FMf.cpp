#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

class Item {
public:
    enum class Type { Folder, Document, Image, Audio, Video, Archive, Other };
    bool is_folder;
    std::string name;
    Type type;
    double size = 0;
};

Item::Type getFileType(const std::string& name) {
    std::size_t pos = name.find_last_of('.');
    if (pos != std::string::npos) {
        std::string extension = name.substr(pos + 1);

        if (extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "gif" || extension == "bmp") {
            return Item::Type::Image;
        } else if (extension == "mp3" || extension == "wav" || extension == "flac" || extension == "aac") {
            return Item::Type::Audio;
        } else if (extension == "mp4" || extension == "avi" || extension == "mkv" || extension == "mov") {
            return Item::Type::Video;
        } else if (extension == "zip" || extension == "rar" || extension == "tar" || extension == "gz") {
            return Item::Type::Archive;
        } else if (extension == "txt" || extension == "pdf" || extension == "doc" || extension == "docx") {
            return Item::Type::Document;
        }
    }
    return Item::Type::Other;
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
                    item.size = st.st_size / (1024.0 * 1024.0);
                    if (item.is_folder) {
                        item.type = Item::Type::Folder;
                    } else {
                        item.type = getFileType(name);
                    }
                    items.push_back(item);
                }
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Error opening directory" << std::endl;
    }
    return items;
}

bool compareItemsByName(const Item& a, const Item& b) {
    return a.name < b.name;
}

void start_menu();

void displayFiles(const std::vector<Item>& items, int currentItem) {
    std::cout << "\033c";
    for (size_t i = 0; i < items.size(); ++i) {
        if (i == currentItem) {
            std::string type_str;
            switch (items[i].type) {
                case Item::Type::Folder:
                    type_str = "Folder";
                    break;
                case Item::Type::Document:
                    type_str = "Document";
                    break;
                case Item::Type::Image:
                    type_str = "Image";
                    break;
                case Item::Type::Audio:
                    type_str = "Audio";
                    break;
                case Item::Type::Video:
                    type_str = "Video";
                    break;
                case Item::Type::Archive:
                    type_str = "Archive";
                    break;
                case Item::Type::Other:
                    type_str = "Other";
                    break;
            }
            std::cout << "-> " << items[i].name << "| " << type_str << " " << items[i].size << " MB\n";
        } else {
            std::string type_str;
            switch (items[i].type) {
                case Item::Type::Folder:
                    type_str = "Folder";
                    break;
                case Item::Type::Document:
                    type_str = "Document";
                    break;
                case Item::Type::Image:
                    type_str = "Image";
                    break;
                case Item::Type::Audio:
                    type_str = "Audio";
                    break;
                case Item::Type::Video:
                    type_str = "Video";
                    break;
                case Item::Type::Archive:
                    type_str = "Archive";
                    break;
                case Item::Type::Other:
                    type_str = "Other";
                    break;
            }
            std::cout << "   " << items[i].name << "| " << type_str << " " << items[i].size << " MB\n";
        }
    }
}

int main() {
    start_menu();

    std::string start_dir = "/home";
    std::vector<Item> items = set_list_of_current_dir(start_dir);

    int currentSelection = 0;

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    displayFiles(items, currentSelection);

    char input;
    while (read(STDIN_FILENO, &input, 1) == 1) {
        if (input == '\033') { // arrows
            read(STDIN_FILENO, &input, 1); 
            read(STDIN_FILENO, &input, 1);
            switch (input) {
                case 'A': 
                    currentSelection = (currentSelection == 0) ? items.size() - 1 : currentSelection - 1;
                    break;
                case 'B': 
                    currentSelection = (currentSelection + 1) % items.size();
                    break;
            }
            displayFiles(items, currentSelection);
        }
        if (input == 'q') { // quit
            break;
        }   
        if (input == '\n') { // Enter
            if (items[currentSelection].is_folder) { 
                start_dir += "/" + items[currentSelection].name;
                items = set_list_of_current_dir(start_dir); 
                currentSelection = 0; 
                displayFiles(items, currentSelection);
            }
        }
        if (input == 127) { // Backspace
            if(start_dir != "/" && start_dir != "/home") {
                size_t pos = start_dir.find_last_of('/');
                if (pos != std::string::npos) {
                    start_dir = start_dir.substr(0, pos);
                    items = set_list_of_current_dir(start_dir); 
                    currentSelection = 0; 
                    displayFiles(items, currentSelection);
                }
            } else {
                start_dir = "/";
                items = set_list_of_current_dir(start_dir); 
                currentSelection = 0; 
                displayFiles(items, currentSelection);
            }

        }
        if (input == 's') {
            std::sort(items.begin(), items.end(), compareItemsByName);
            displayFiles(items, currentSelection);
        }

    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}

void start_menu() {
    std::cout << "--------------------------------------\n";
    std::cout << "↓ ↑       |navigate through the items|\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "Enter     |enter dir                 |\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "Backspace |back to previous dir      |\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "q         |quit                      |\n";
    std::cout << "--------------------------------------\n\n";
    std::cout << "Press Enter for continue: ";
    std::cin.ignore();
}






