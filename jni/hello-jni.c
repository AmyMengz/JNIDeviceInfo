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
#include <sys/socket.h>
#include <linux/sockios.h>
#include <linux/if.h>
#include <linux/in.h>
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
char * getMac2();
int get_mac_addr(char *mac_addr);
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
	const char * mac = getMac2();
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

char * getMac2(){
	static char mac_addr[80] = {0};
	printf( "MAC = %d, %s\n", get_mac_addr(mac_addr), mac_addr );
	return mac_addr;
}

int get_mac_addr(char *mac_addr)
{
  int sockfd;
  struct ifreq ifr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) >= 0) {
    //strncpy(ifr.ifr_name, "eth0", IFNAMESIZE);
    strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ);

    ifr.ifr_addr.sa_family = AF_INET;

    if (ioctl(sockfd, SIOCGIFHWADDR, (char*) &ifr) == 0) {
      sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
              (unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[0], (unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[1],
              (unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[2], (unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[3],
              (unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[4], (unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[5]);
      return 0;
    }
  }
  /* error */
  return -1;
}

