#ifndef TERMINALFILEEXPLORER_LIB_DIRECTORY_H_
#define TERMINALFILEEXPLORER_LIB_DIRECTORY_H_

#include <filesystem>
namespace fs = std::filesystem;

class Directory {
private:
    fs::path path_;

public:
    static inline std::string directory_icon_ =
        R"(@@@+
@   @@@@@@@@@
@           :@
@   @@@@@@@@@@@@.
@ .@           @
@.@           @
@            @
@  . ...    @@
*@@@@@@@@@@@% )";
};

#endif  // TERMINALFILEEXPLORER_LIB_DIRECTORY_H_
