/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <stdio.h>
#include <jni.h>
#include <sys/system_properties.h>


/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
char * getfilemac();
char * getAndroid_MODEL();
char * getAndroid_BRAND();
char * getAndroidDeviceID_Serial();
jstring
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
	const char * mac = getfilemac();
	const char * deviceID = getAndroidDeviceID_Serial();

//	getAndroidDeviceID_Serial(deviceID);
//	int len_buf = strlen(deviceID);
//	string result(deviceID, len_buf);
    return (*env)->NewStringUTF(env, deviceID);
//	return env->NewStringUTF(result.c_str());
}

jstring
Java_com_example_hellojni_HelloJni_getMac( JNIEnv* env,
                                                  jobject thiz )
{
	const char * mac = getfilemac();
	const char * deviceID = getAndroidDeviceID_Serial();
    return (*env)->NewStringUTF(env, mac);
}

jstring
Java_com_example_hellojni_HelloJni_getserial( JNIEnv* env,
                                                  jobject thiz )
{
	const char * deviceID = getAndroidDeviceID_Serial();
    return (*env)->NewStringUTF(env, deviceID);
}
jstring
Java_com_example_hellojni_HelloJni_getModel( JNIEnv* env,
                                                  jobject thiz )
{
	const char * model = getAndroid_MODEL();
    return (*env)->NewStringUTF(env, model);
}
Java_com_example_hellojni_HelloJni_getbrand( JNIEnv* env,
                                                  jobject thiz )
{
	const char * brand = getAndroid_BRAND();
    return (*env)->NewStringUTF(env, brand);
}

char * getAndroid_MODEL()//serial number
{
	static char buff[255];
    __system_property_get("ro.product.model",buff);
    return buff;
}
char * getAndroid_BRAND()//serial number
{
	static char buff[255];
    __system_property_get("ro.product.brand",buff);
    return buff;
}
char * getAndroidDeviceID_Serial()//serial number
{
	static char buff[255];
    __system_property_get("ro.serialno",buff);
    return buff;
}
//char * getAndroidDeviceID_Serial()//serial number
//{
//	static char buff[255];
//    __system_property_get("ro.serialno",buff);
//    return buff;
//}

char * getfilemac(){
	FILE *fp = NULL;
	static char buff[255];
	fp = fopen("/sys/class/net/wlan0/address","r");
	if(fp != NULL){
		fgets(buff, 255, (FILE*)fp);
		return buff;
	}
	return "null=="+(fp==NULL);
}
//JNIEXPORT jstring JNICALL Java_com_example_gnaix_ndk_NativeMethod_getString
//        (JNIEnv *env, jclass object, jstring str){
//
//    //1. 将unicode编码的java字符串转换成C风格字符串
//    const char *buf_name = env->GetStringUTFChars(str, 0);
//    if(buf_name == NULL){
//        return NULL;
//    }
//    int len = strlen(buf_name);
//    char n_name[len];
//    strcpy(n_name, buf_name);
//
//    //2. 释放内存
//    env->ReleaseStringUTFChars(str, buf_name);
//
//    //3. 处理 n_name="ro.serialno"
//    char buf[1024];
//    __system_property_get(n_name, buf);
//    LOGD("serialno : %s", buf);
//
//    //4. 去掉尾部"\0"
//    int len_buf = strlen(buf);
//    string result(buf, len_buf);
//
//    return env->NewStringUTF(result.c_str());
//}

