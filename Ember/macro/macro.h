#pragma once
#define SMART_OBJECT
#define NAMESPACE chaos


#ifdef NAMESPACE
#define NAMESPACEBEGINE namespace NAMESPACE{
#define NAMESPACEEND }
#define USINGNAMESPACE using namespace NAMESPACE;
#else
#define NAMESPACEBEGINE
#define NAMESPACEEND
#define USINGNAMESPACE
#endif

/*
配置编译选项
*/

//是否编译智能指针模块
#define SMART_OBJECT
#define CLASS_INFO