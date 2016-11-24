#include "dir.h"
#include <stdio.h>
#include "dirapi.h"
#include "../include/header_def.h"
#include "../log/logapi.h"

#ifdef _WIN32
#include <io.h>
#include<direct.h>
#else
#include <dirent.h>
#endif

Dir::Dir(Dir* parent, const std::string& path, bool sub_dir)
	:m_parent(parent)
	,m_path(path)
{
	Load(false, sub_dir);
}

Dir::Dir()
	:m_parent(NULL)
{

}

Dir::~Dir()
{
	Clear();
}

bool Dir::Load( bool reload, bool sub_dir)
{
	if (reload)
	{
		Clear();
	}

	std::string tmp_path = m_path;
	int count = 0;

#ifdef _WIN32
	if (tmp_path.size() > 0)
	{
		if (tmp_path[tmp_path.size() - 1] == '/')
		{
			tmp_path += "*.*";
		}
		else
		{
			tmp_path += "/*.*";
		}
	}
	_finddata_t filedata;
	//查找文件
	long file_handle = _findfirst(tmp_path.c_str(), &filedata);
	if(file_handle == -1)
	{
		printf("open dir [%s] error. \n", m_path);
		return false;
	}

	do 
	{
		DirItem item;
		if (_A_SUBDIR != filedata.attrib)
		{
			item.item_type = DirItem::DIT_FILE;
			std::string sub_path = tmp_path.substr(0, tmp_path.size() - 4);
			item.dir_name = sub_path + "/" + filedata.name;
			//item.dir = new Dir(this, filedata.name, sub_dir)
		}
		else
		{
			if (filedata.name[0] == '.') continue;
			item.item_type = DirItem::DIT_DIR;
			std::string sub_path = tmp_path.substr(0, tmp_path.size() - 4);
			item.dir_name = sub_path + "/" + filedata.name;
			item.dir = new Dir(this, item.dir_name, sub_dir);
		}
		m_items.push_back(item);

	} while (_findnext(file_handle, &filedata) == 0);
	//_close(file_handle);
#endif

#ifdef LINUX
	if (tmp_path.size() > 0)
	{
		if (tmp_path[tmp_path.size() - 1] != '/')
		{
			tmp_path += "/";
		}
	}
	struct dirent* file_info = NULL;
	DIR* dir = NULL;
	dir = opendir(tmp_path.c_str());
	if (NULL == dir)
	{
		printf("open dir [%s] error. \n", tmp_path.c_str());
		return false;
	}

	while ((file_info = readdir(dir)) != NULL )
	{	
		DirItem item;
		//if (strcmp(file_info->d_name,".") == 0) continue;
		//if (strcmp(file_info->d_name,"..") == 0) continue;
		//if (file_info->d_type != 4)
		//{
		//	strcpy(m_file_list[count], file_info->d_name);
		//}

		
		if (file_info->d_type == DT_DIR && sub_dir)
		{
			if (strcmp(file_info->d_name,".") == 0) continue;
			if (strcmp(file_info->d_name,"..") == 0) continue;

			item.item_type = DirItem::DIT_DIR;
			item.dir_name = tmp_path + file_info->d_name;
			item.dir = new Dir(this, item.dir_name, sub_dir);
		}
		else if (file_info->d_type == DT_REG || file_info->d_type == DT_UNKNOWN)
		{
			item.item_type = DirItem::DIT_FILE;
			item.dir_name = tmp_path + file_info->d_name;
		}
		else
		{
			DEBUG_LOG("unknow file type :%d", file_info->d_type);
			continue;
		}
		//DEBUG_LOG("put file[%s]", item.dir_name.c_str())
		m_items.push_back(item);
	}
	closedir(dir);
#endif
	return true;
}

void Dir::Clear()
{
	DirItemItr itr = m_items.begin();
	DirItemItr itr_end = m_items.end();
	for ( ; itr != itr_end; ++itr)
	{
		if (itr->dir != NULL) delete itr->dir;
	}
	m_items.clear();
	m_path.clear();
	m_parent = NULL;
}

void Dir::GetAllFile( NameList& file_names, bool sub_dir, const std::string& suffix )
{
	DirItemItr itr = m_items.begin();
	DirItemItr itr_end = m_items.end();
	unsigned int suffix_len = suffix.size();
	for ( ; itr != itr_end; ++itr)
	{
		if (itr->item_type == DirItem::DIT_FILE)
		{
			if (suffix_len > 0)
			{
				unsigned int name_len = itr->dir_name.size();
				if (name_len < suffix_len) continue;
				if (0 != memcmp(&suffix[0], &itr->dir_name[name_len - suffix_len], suffix_len - 1)) continue;
			}
			file_names.insert(itr->dir_name);
		}
		else if (itr->item_type == DirItem::DIT_DIR && itr->dir != NULL && sub_dir)
		{
			itr->dir->GetAllFile(file_names, sub_dir, suffix);
		}
	}
}

bool Dir::Open( Dir* parent, const std::string& path, bool sub_dir )
{
	m_parent = parent;
	m_path = path;
	return Load(true, sub_dir);
}
