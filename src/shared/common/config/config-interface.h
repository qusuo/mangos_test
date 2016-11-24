#ifndef JXS_CONFIGINTERFACE_H_
#define JXS_CONFIGINTERFACE_H_
//
namespace jxs
{
	class Config
	{
	public:
		Config() {}
		virtual ~Config() {}

		virtual bool Init(const char* file_path) = 0;
		virtual bool PreLoad() = 0;
		virtual bool ConformPreLoad() = 0;
		virtual bool RollBackPreLoad() = 0;
	private:


	};
}



#endif