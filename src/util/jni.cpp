/*
 * JniUtil.cpp
 *
 *  Created on: 2014-2-20
 *      Author: Administrator
 */

#include "jni.hpp"
#include "libiconv/include/iconv.h"

namespace util
{

class Jni::SystemOut
{
public:
	explicit SystemOut(Jni* pjni) :
		pjni_(pjni),
		cls_system_(0),
		fid_system_out_(0),
		obj_system_out_(0),
		cls_system_out_(0),
		mid_system_out_println_(0),
		mid_system_out_print_(0)
	{}

	void println(const std::string& str, bool is_utf8_string = false) {
		if (!testMethodPrintlnAndPrint())
			return;
		print_aux(str, is_utf8_string, mid_system_out_println_);
	}

	void print(const std::string& str, bool is_utf8_string = false) {
		if (!testMethodPrintlnAndPrint())
			return;
		print_aux(str, is_utf8_string, mid_system_out_print_);
	}
private:
	inline bool isPrintlnAndPrintMethodOk() const {
		return mid_system_out_println_ && mid_system_out_print_;
	}

	bool testMethodPrintlnAndPrint() {
		if (!isPrintlnAndPrintMethodOk()) {
			cls_system_ = pjni_->getJniEnv()->FindClass(kClass_System.c_str());
			if (!cls_system_) {
				return false;
			}
			fid_system_out_ = pjni_->getJniEnv()->GetStaticFieldID(cls_system_,
					kFieldName_out.c_str(), kFieldId_out.c_str());
			if (!fid_system_out_) {
				return false;
			}
			obj_system_out_ = pjni_->getJniEnv()->GetStaticObjectField(cls_system_,
					fid_system_out_);
			if (!obj_system_out_) {
				return false;
			}
			cls_system_out_ = pjni_->getJniEnv()->GetObjectClass(obj_system_out_);
			if (!cls_system_out_) {
				return false;
			}
			mid_system_out_println_ = pjni_->getJniEnv()->GetMethodID(
					cls_system_out_, kMethodName_println.c_str(),
					kMethodSignature_println.c_str());
			if (!mid_system_out_println_) {
				return false;
			}
			mid_system_out_print_ = pjni_->getJniEnv()->GetMethodID(
					cls_system_out_, kMethodName_print.c_str(),
					kMethodSignature_print.c_str());
			if (!mid_system_out_print_) {
				return false;
			}
		}

		return true;
	}

	void print_aux(const std::string& str, bool is_utf8_string, jmethodID mid) {
		if (is_utf8_string) {
			pjni_->getJniEnv()->CallVoidMethod(obj_system_out_, mid,
					pjni_->utf8string2jstring(str));
		} else {
			pjni_->getJniEnv()->CallVoidMethod(obj_system_out_, mid,
								pjni_->localstring2jstring(str));
		}
	}

private:
	static const std::string kClass_System;
	static const std::string kFieldName_out;
	static const std::string kFieldId_out;
	static const std::string kMethodName_println;
	static const std::string kMethodSignature_println;
	static const std::string kMethodName_print;
	static const std::string kMethodSignature_print;

	Jni* pjni_;
	jclass cls_system_;
	jfieldID fid_system_out_;
	jobject obj_system_out_;
	jclass cls_system_out_;
	jmethodID mid_system_out_println_;
	jmethodID mid_system_out_print_;
};

const std::string Jni::SystemOut::kClass_System = "java/lang/System";
const std::string Jni::SystemOut::kFieldName_out = "out";
const std::string Jni::SystemOut::kFieldId_out = "Ljava/io/PrintStream;";
const std::string Jni::SystemOut::kMethodName_println = "println";
const std::string Jni::SystemOut::kMethodSignature_println = "(Ljava/lang/String;)V";
const std::string Jni::SystemOut::kMethodName_print = "print";
const std::string Jni::SystemOut::kMethodSignature_print = "(Ljava/lang/String;)V";

Jni::Jni(JNIEnv* je, const std::string& local_encode) :
					jni_env_(je),
					local_encode_(local_encode),
					sysout_(new SystemOut(this))
{
	// TODO Auto-generated constructor stub
}

Jni::~Jni()
{
	// TODO Auto-generated destructor stub
}

std::string Jni::jstring2utf8string(const jstring& js)
{
	const char* p = jni_env_->GetStringUTFChars(js, NULL);
	std::string str(p);
	jni_env_->ReleaseStringUTFChars(js, p);
	return str;
}

jstring Jni::utf8string2jstring(const std::string& s)
{
	return jni_env_->NewStringUTF(s.c_str());
}

std::string Jni::jstring2localstring(const jstring& js)
{
	const char* p = jni_env_->GetStringUTFChars(js, NULL);
	std::string str = encodeConv(p, local_encode_, kEncodeUTF8, 4).second;
	jni_env_->ReleaseStringUTFChars(js, p);

	return str;
}

jstring Jni::localstring2jstring(const std::string& s)
{
	std::string str = encodeConv(s.c_str(), kEncodeUTF8, local_encode_, 4).second;
	return jni_env_->NewStringUTF(str.c_str());
}

std::pair<size_t, std::string> Jni::encodeConv(const char* s, const std::string& to, const std::string& from, int len_multi)
{
	size_t len = strlen(s);

	iconv_t cd = iconv_open(to.c_str(), from.c_str());

	Memory<char> in_mem(len + 1);
	Memory<char> out_mem(len_multi*len + 1);
	strcpy(in_mem.buf(), s);

	char* pin = in_mem.buf();
	char* pout = out_mem.buf();
	size_t in_len = in_mem.size()-1;
	size_t out_len = out_mem.size()-1;

	size_t ret = iconv(cd, &pin, &in_len, &pout, &out_len);
	std::string str(out_mem.buf());

	iconv_close(cd);

	return std::make_pair(ret, str);
}

std::string Jni::fromJstring(const jstring& js, bool is_utf8_string)
{
	if (is_utf8_string) {
		return jstring2utf8string(js);
	} else {
		return jstring2localstring(js);
	}
}

jstring Jni::toJstring(const std::string& s, bool is_utf8_string)
{
	if (is_utf8_string) {
		return utf8string2jstring(s);
	} else {
		return localstring2jstring(s);
	}
}

void Jni::sysPrintln(const std::string& str, bool is_utf8_string)
{
	sysout_->println(str, is_utf8_string);
}

void Jni::sysPrint(const std::string& str, bool is_utf8_string)
{
	sysout_->print(str, is_utf8_string);
}

bool Jni::attachCurrentThread(Jvm* jvm)
{
	return JNI_OK == jvm->get()->AttachCurrentThread(&jni_env_, NULL);
}

bool Jni::detachCurrentThread(Jvm* jvm)
{
	return JNI_OK == jvm->get()->DetachCurrentThread();
}

} // namespace util

