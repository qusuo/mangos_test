#ifndef JXS_DIR_H_
#define JXS_DIR_H_

#include <string>
#include <vector>
#include <set>

class Dir;

struct DirItem
{
	enum DirItemType
	{
		DIT_INVALID,
		DIT_DIR,
		DIT_FILE,
	};
	DirItem()
		:item_type(DIT_INVALID)
		,dir(NULL)
	{

	}
	int item_type;
	std::string dir_name;
	Dir* dir;
};

typedef std::vector<DirItem> DirItemList;
typedef DirItemList::iterator DirItemItr;
typedef std::set<std::string> NameList;
typedef NameList::iterator NameListIter;

class Dir
{
public:
	Dir();
	Dir(Dir* parent, const std::string& path, bool sub_dir);
	~Dir();

	bool Open(Dir* parent, const std::string& path, bool sub_dir);
	void GetAllFile(NameList& file_names, bool sub_dir, const std::string& suffix);//获得目录下所有suffix后缀的文件名

	DirItemItr GetBeginItem() { return m_items.begin(); }
	DirItemItr GetEndItem() { return m_items.end(); }
	const std::string& GetPath() const { return m_path; }

private:
	bool Load(bool reload, bool sub_dir);
	void Clear();
	std::string m_path;	//绝对路径
	Dir*		m_parent;
	DirItemList m_items;
};


#endif