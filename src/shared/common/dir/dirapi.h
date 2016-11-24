#ifndef JXS_DIRAPI_H_
#define JXS_DIRAPI_H_

#include <string>

namespace dirapi
{
std::string ConvertDir(const std::string& current_dir, const std::string& relative_dir);
bool CaculateAbsoluteDir(const std::string& absolute_dir, const std::string& relative_dir, std::string& out_res);
bool CaculateRelativeDir(const std::string& absolute_dir_tar, const std::string& absolute_dir_src, std::string& out_res);
void ReplaceChar(char* in_str, int str_len, char old_char, char new_char);

void CutPathTail(std::string& path);//去掉路径的最后一个文件夹
int CutPathBegin(std::string& path);//去掉最前面的../  ./  / 
std::string GetFileName(const char* path);
}

#endif