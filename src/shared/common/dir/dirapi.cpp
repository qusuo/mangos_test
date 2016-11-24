#include "dirapi.h"
#include "../include/header_def.h"

void dirapi::CutPathTail(std::string& path)
{
	unsigned int path_size = path.size();
	unsigned int src_pos = std::string::npos;
	if (path_size > 0)
	{
		src_pos = path.find_last_of('/');
		if (src_pos == path_size - 1)
		{
			//path.pop_back();
			path.erase(src_pos);
			src_pos = path.find_last_of('/');
		}
	}

	if (src_pos != std::string::npos)
	{
		path.replace(src_pos + 1, path_size - 1, "");
	}
	else
	{
		path.clear();
	}
}

int dirapi::CutPathBegin(std::string& path)
{
	unsigned int path_size = path.size();
	int ret = -1;
	if (path_size > 0 && path[0] == '/')
	{
		path.replace(0, 1, "");
		ret = 1;
	}
	else if (path_size >= 2 && path[0] == '.' && path[1] == '/')
	{
		path.replace(0, 2, "");
		ret = 2;
	}
	else if (path_size >= 3 && path[0] == '.' && path[1] == '.' && path[2] == '/')
	{
		path.replace(0, 3, "");
		ret = 3;
	}
	return ret;
}

std::string dirapi::ConvertDir(const std::string& current_dir, const std::string& relative_dir)
{
	std::string cur_dir_buff = current_dir;
	std::string convert_dir_buff = relative_dir;
	
	while (current_dir.size() > 0 && 3 == CutPathBegin(convert_dir_buff))
	{
		CutPathTail(cur_dir_buff);
	}
	
	return cur_dir_buff + convert_dir_buff;
}

bool dirapi::CaculateAbsoluteDir(const std::string& absolute_dir, const std::string& relative_dir, std::string& out_res)
{
	std::string tmp_abs_dir = absolute_dir;
	std::string tmp_rel_dir = relative_dir;

	unsigned src_pos = tmp_abs_dir.find_last_of('/');
	if (src_pos == tmp_abs_dir.size() - 1)
	{
		tmp_abs_dir.erase(src_pos);
	}

	if (relative_dir.length() >= 2 && relative_dir[0] == '.' && relative_dir[1] == '/')
	{
		tmp_rel_dir = relative_dir.substr(2, relative_dir.length());
	}

	while (true)
	{
		int abs_tail_pos = tmp_abs_dir.find_last_of('/');
		int rel_head_pos = tmp_rel_dir.find_first_of('/');
		if (rel_head_pos == 2 && tmp_rel_dir[0] == '.' && tmp_rel_dir[1] == '.')
		{
			if (abs_tail_pos < 0) return false;

			if (abs_tail_pos > 0)
			{
				tmp_abs_dir = tmp_abs_dir.substr(0, abs_tail_pos);
			}
			else
			{
				tmp_abs_dir.clear();
			}

			tmp_rel_dir = tmp_rel_dir.substr(rel_head_pos+1, tmp_rel_dir.length());
		}
		else
		{
			break;
		}
	}

	out_res = tmp_abs_dir + "/" + tmp_rel_dir;
	return true;
}

void dirapi::ReplaceChar(char* in_str, int str_len, char old_char, char new_char)
{
	for (int i = 0; i < str_len; ++i)
	{
		if (in_str[i] == old_char)
		{
			in_str[i] = new_char;
		}
	}
}

bool dirapi::CaculateRelativeDir( const std::string& absolute_dir_tar, const std::string& absolute_dir_src, std::string& out_res )
{
	std::string tmp_tar = absolute_dir_tar;
	std::string tmp_src = absolute_dir_src;

	int tmp_src_pos = tmp_tar.find_last_of('/');
	if (tmp_src_pos == tmp_tar.size() - 1)
	{
		tmp_tar.erase(tmp_src_pos);
	}


	int tar_pos = tmp_tar.find_first_of('/');
	int src_pos = tmp_src.find_first_of('/');
	if (tar_pos != src_pos) return false;

	while (true)
	{
		if (tar_pos < tmp_tar.length() - 1)
		{
			tmp_tar = tmp_tar.substr(tar_pos+1, tmp_tar.length());
		}
		else
		{
			tmp_tar.clear();
		}

		if (src_pos < tmp_src.length() - 1)
		{
			tmp_src = tmp_src.substr(src_pos+1, tmp_src.length());
		}
		else
		{
			tmp_src.clear();
		}

		tar_pos = tmp_tar.find_first_of('/');
		src_pos = tmp_src.find_first_of('/');
		if (tar_pos < 0 || tar_pos != src_pos || 0 != memcmp(tmp_tar.c_str(), tmp_src.c_str(), tar_pos+1))
		{
			break;
		}
	}
	
	if (tmp_tar.size() > 0)
	{
		int tmp_pos = tmp_tar.find_first_of('/');
		while (tmp_pos >= 0)
		{
			tmp_src = "../" + tmp_src;
			tmp_tar = tmp_tar.substr(tar_pos+1, tmp_tar.length());
			tmp_pos = tmp_tar.find_first_of('/');
		}

		tmp_src = "../" + tmp_src;
	}

	out_res = tmp_src;
	return true;
}

namespace dirapi
{
	std::string GetFileName(const char* path)
	{
		int begin_pos = 0;
		int end_pos = 0;
		while ('\0' != path[end_pos])
		{
			if ('/' == path[end_pos])
			{
				begin_pos = end_pos + 1;
			}
			++end_pos;
		}

		if (end_pos > begin_pos)
		{
			return &path[begin_pos];
		}

		return "";

	}
}