#include <iostream>
#include <vector>
#include <string>
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

void displayFiles(const std::vector<Item>& items, int currentItem) {
    std::cout << "\033c"; // Clear screen
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
            std::cout << "-> " << items[i].name << "|\t" << type_str << " " << items[i].size << " MB\n";
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
            std::cout << "   " << items[i].name << "|\t" << type_str << " " << items[i].size << " MB\n";
        }
    }
}

int main() {
    std::string start_dir = "/"; 
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
        if (input == '\033') { // Первый символ последовательности ESC
            read(STDIN_FILENO, &input, 1); // Пропускаем '['
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
        if (input == 'q') { 
            break;
        }   
        if (input == '\n') { 
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
                if (pos != std::string::npos) { // Удаление последней части строки, если символ найден
                    start_dir = start_dir.substr(0, pos);
                    // Обновление списка файлов и папок для предыдущей директории
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

    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}












//    class Item
/*#include <iostream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>

class Item {
public:
    enum class Type { Folder, Document, Image, Audio, Video, Archive, Other };
    bool is_folder;
    std::string name;
    Type type;
    double size = 0;
};

Item::Type getFileType(const std::string& name) {
    // Получаем расширение файла
    std::size_t pos = name.find_last_of('.');
    if (pos != std::string::npos) {
        std::string extension = name.substr(pos + 1);

        // Определяем тип файла на основе расширения
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
    // Если расширение не соответствует известным типам, возвращаем "другое"
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
            if (name[0] != '.') { // not hidden file
                Item item;
                item.name = name;
                std::string full_path = start_dir + "/" + name;
                if (stat(full_path.c_str(), &st) == 0) {
                    item.is_folder = S_ISDIR(st.st_mode);
                    item.size = st.st_size / (1024.0 * 1024.0); // in MB
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

int main() {
    std::string start_dir;
    std::cout << "Enter starting directory: ";
    std::cin >> start_dir;

    std::vector<Item> items = set_list_of_current_dir(start_dir);

    // Выводим список файлов и папок
    for (const auto& item : items) {
        std::string type_str;
        switch (item.type) {
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
        std::cout << "Name: " << item.name << "\t\t\tType: " << type_str << "\t\t\tSize: " << item.size << " MB" << std::endl;
    }

    return 0;
}*/






//   ->Menu
/*#include <iostream>
#include <termios.h>
#include <unistd.h>

// Функция для включения и отключения буферизации ввода
void setBufferedInput(bool enable) {
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (!enable) {
        tty.c_lflag &= ~ICANON & ~ECHO;
    } else {
        tty.c_lflag |= ICANON | ECHO;
    }
    (void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

// Функция для отображения меню
void displayMenu(int currentSelection) {
    const char* menuItems[] = {"Create new folder", "Delete folder", "Move", "Properties"};
    for (int i = 0; i < 4; ++i) {
        if (i == currentSelection) {
            std::cout << "-> " << menuItems[i] << "\n";
        } else {
            std::cout << "   " << menuItems[i] << "\n";
        }
    }
}

int main() {
    int currentSelection = 0;
    setBufferedInput(false);

    system("clear");
    displayMenu(currentSelection);
    char input;
    while (read(STDIN_FILENO, &input, 1) == 1) {
        if (input == '\033') { // Первый символ последовательности ESC
            read(STDIN_FILENO, &input, 1); // Пропускаем '['
            read(STDIN_FILENO, &input, 1);
            switch (input) {
                case 'A': // Стрелка вверх
                    currentSelection = (currentSelection + 3) % 4;
                    break;
                case 'B': // Стрелка вниз
                    currentSelection = (currentSelection + 1) % 4;
                    break;
                default:
                    break;
            }
            std::cout << "\033c"; // Очистка экрана
            displayMenu(currentSelection);
        }
        if (input == 'q') { // Выход из программы
            break;
        }
    }

    setBufferedInput(true);
    return 0;
}*/






/*#include <iostream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>

bool isHidden(const char* name) {
    return name[0] == '.';
}

int main() {
    std::string home_dir; // = "/home/kirill";
    std::cout << "Enter starting directory: ";
    std::cin >> home_dir;
    std::vector<std::string> folders;

    DIR *dir;
    struct dirent *ent;
    struct stat st;
    if ((dir = opendir(home_dir.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            const char* name = ent->d_name;
            if (!isHidden(name)) {
                std::string full_path = home_dir + "/" + name;
                if (stat(full_path.c_str(), &st) == 0) {
                    if (S_ISDIR(st.st_mode)) {
                        folders.push_back(name);
                    } else {
                        // В случае, если нужно обрабатывать и обычные файлы, можно добавить соответствующий код здесь.
                    }
                }
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Error opening directory" << std::endl;
        return EXIT_FAILURE;
    }

    // Выводим список папок
    for (const auto& folder : folders) {
        std::cout << folder << std::endl;
    }

    return 0;
}*/