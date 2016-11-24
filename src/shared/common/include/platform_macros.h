#ifndef JXS_PLATFORMMACROS_H_
#define JXS_PLATFORMMACROS_H_
//author:hsj
//平台差异相关的配置宏定义

#if defined(_WIN32) || defined(LINUX)
#define JXS_FILE_LOG_INIT(log_file, file_dir, to_console)	log_file.Init(file_dir, to_console)
#define JXS_FILE_LOG(log_file, log_level, format, ...)      log_file.WriteLogFlush(log_level, format, ##__VA_ARGS__)
#else
#define JXS_FILE_LOG_INIT(log_file, file_dir, to_console) 0	
#define JXS_FILE_LOG(log_file, log_level, format, ...) 0      
#endif


#endif