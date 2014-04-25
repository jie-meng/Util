/*
 * JniUtil.h
 *
 *  Created on: 2014-2-20
 *      Author: Administrator
 */

#ifndef UTIL_JNI_HPP
#define UTIL_JNI_HPP

#include <jni.h>
#include "base.hpp"

namespace util
{

const std::string kEncodeGBK = "GBK";
const std::string kEncodeUTF8 = "UTF-8";

class Jvm;

class Jni
{
public:
	explicit Jni(JNIEnv* je, const std::string& local_encode = kEncodeGBK);
	~Jni();
	inline JNIEnv* getJniEnv() const { return jni_env_; }
	//utf-8 string
	std::string jstring2utf8string(const jstring& js);
	jstring utf8string2jstring(const std::string& s);
	//local string
	std::string jstring2localstring(const jstring& js);
	jstring localstring2jstring(const std::string& s);
	//convert
	std::string fromJstring(const jstring& js, bool is_utf8_string = false);
	jstring toJstring(const std::string& s, bool is_utf8_string = false);
	//system_out
	void sysPrint(const std::string& str, bool is_utf8_string = false);
	void sysPrintln(const std::string& str, bool is_utf8_string = false);
	//thread
	bool attachCurrentThread(Jvm* jvm);
	bool detachCurrentThread(Jvm* jvm);
private:
	std::pair<size_t, std::string> encodeConv(const char* s, const std::string& to, const std::string& from, int len_multi);
private:
	JNIEnv* jni_env_;
	std::string local_encode_;
	class SystemOut;
	UtilAutoPtr<SystemOut> sysout_;
private:
	DISALLOW_COPY_AND_ASSIGN(Jni)
};

class Jvm
{
public:
	explicit Jvm(JNIEnv* je) : jvm_(0)
	{ if (je) init(je); }
	inline void init(JNIEnv* je) { je->GetJavaVM(&jvm_); }
	inline JavaVM* get() const { return jvm_; }
private:
	JavaVM* jvm_;
};

class AttachThreadJni
{
public:
	explicit AttachThreadJni(Jvm* jvm) : jni_(0), jvm_(jvm), is_ok_(false)
	{ is_ok_ = JNI_OK == jni_.attachCurrentThread(jvm_); }
	~AttachThreadJni()
	{ jni_.detachCurrentThread(jvm_); }
	Jni& getJni() { return jni_; }
	bool isOk() const { return is_ok_; }
private:
	Jni jni_;
	Jvm* jvm_;
	bool is_ok_;
private:
	DISALLOW_COPY_AND_ASSIGN(AttachThreadJni)
};

} // namespace util

#endif /* UTIL_JNI_HPP */
