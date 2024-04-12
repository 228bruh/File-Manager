#include <iostream>
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
}
























// #include <iostream>
// #include <vector>
// #include <dirent.h>
// #include <sys/stat.h>

// bool isHidden(const char* name) {
//     return name[0] == '.';
// }

// int main() {
//     std::string home_dir; // = "/home/kirill";
//     std::cout << "Enter starting directory: ";
//     std::cin >> home_dir;
//     std::vector<std::string> folders;

//     DIR *dir;
//     struct dirent *ent;
//     struct stat st;
//     if ((dir = opendir(home_dir.c_str())) != nullptr) {
//         while ((ent = readdir(dir)) != nullptr) {
//             const char* name = ent->d_name;
//             if (!isHidden(name)) {
//                 std::string full_path = home_dir + "/" + name;
//                 if (stat(full_path.c_str(), &st) == 0) {
//                     if (S_ISDIR(st.st_mode)) {
//                         folders.push_back(name);
//                     } else {
//                         // В случае, если нужно обрабатывать и обычные файлы, можно добавить соответствующий код здесь.
//                     }
//                 }
//             }
//         }
//         closedir(dir);
//     } else {
//         std::cerr << "Error opening directory" << std::endl;
//         return EXIT_FAILURE;
//     }

//     // Выводим список папок
//     for (const auto& folder : folders) {
//         std::cout << folder << std::endl;
//     }

//     return 0;
// }