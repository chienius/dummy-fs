//
//  CommandUtility.hpp
//  DummyFS
//
//  Created by Archie Lu on 2017/4/10.
//  Copyright © 2017年 Archibald Lu. All rights reserved.
//

#ifndef CommandUtility_hpp
#define CommandUtility_hpp

class CommandUtility {
public:
    static const int MAX_FILE_SIZE = (128 * 128 * 2 + 128 * 2 + 6)*512;
    static void load(char* src, char* dest);
    static void save(char* src, char* dest);
    static void ls(char* path);
    static void mkdir(char* path);
    static void unlink(char* path);
    static void rmdir(char* path);
};

#endif /* CommandUtility_hpp */
