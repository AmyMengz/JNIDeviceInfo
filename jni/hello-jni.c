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
#include <stdbool.h>
#include <sys/system_properties.h>

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<Android/log.h>
/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"TAG",__VA_ARGS__);
char * getfilemac();
char * getAndroid_MODEL();
char * getAndroid_BRAND();
char * getAndroidDeviceID_Serial();
char * getMac2();
char* getImei();
char * dns();
int get_mac_addr(char *mac_addr);
char cmd_res_line[256];
char total_cmd_res[25600];
char* exec_get_out(char* cmd) {

    FILE* pipe = popen(cmd, "r");

    if (!pipe)
    	return NULL;

    total_cmd_res[0] = 0;

    while(!feof(pipe)) {
        if(fgets(cmd_res_line, 256, pipe) != NULL)
        {
        	//TODO: add handling for long command reads...
        	strcat(total_cmd_res,cmd_res_line);
        }
    }
    pclose(pipe);
    return total_cmd_res;
}
jstring Java_com_example_hellojni_HelloJni_stringFromJNI(JNIEnv* env,
		jobject thiz) {
	const char * mac = getfilemac();
	const char * deviceID = getAndroidDeviceID_Serial();

//	getAndroidDeviceID_Serial(deviceID);
//	int len_buf = strlen(deviceID);
//	string result(deviceID, len_buf);
	return (*env)->NewStringUTF(env, deviceID);
//	return env->NewStringUTF(result.c_str());
}

jstring Java_com_example_hellojni_HelloJni_getMac(JNIEnv* env, jobject thiz) {
	const char * mac = getfilemac();
	return (*env)->NewStringUTF(env, mac);
}

jstring Java_com_example_hellojni_HelloJni_getserial(JNIEnv* env, jobject thiz) {
	const char * deviceID = getAndroidDeviceID_Serial();
	return (*env)->NewStringUTF(env, deviceID);
}
jstring Java_com_example_hellojni_HelloJni_getModel(JNIEnv* env, jobject thiz) {
	const char * model = getAndroid_MODEL();
	return (*env)->NewStringUTF(env, model);
}
Java_com_example_hellojni_HelloJni_getbrand(JNIEnv* env, jobject thiz) {
	const char * brand = getAndroid_BRAND();
	return (*env)->NewStringUTF(env, brand);
}
Java_com_example_hellojni_HelloJni_getparm(JNIEnv* env, jobject thiz) {
	const char * brand = dns();
	return (*env)->NewStringUTF(env, brand);
}

char * getAndroid_MODEL() //serial number
{
	static char buff[255];
	__system_property_get("ro.product.model", buff);
	return buff;
}
char * getAndroid_BRAND() //serial number
{
	static char buff[255];
	__system_property_get("ro.product.brand", buff);
	return buff;
}
char * getAndroidDeviceID_Serial() //serial number
{
	static char buff[255];
	__system_property_get("ro.serialno", buff);
	return buff;
}

//char * getAndroidDeviceID_Serial() //serial number
//{
//	static char buff[255];
//	__system_property_get("ro.serialno", buff);
//	return buff;
//}

char * dns() //serial number
{
	static char buff[255];
	__system_property_get("dhcp.wlan0.domain", buff);
	return buff;
}
//char * getAndroidDeviceID_Serial()//serial number
//{
//	static char buff[255];
//    __system_property_get("ro.serialno",buff);
//    return buff;
//}

char * getfilemac() {
	FILE *fp = NULL;
	static char buff[255];
	fp = fopen("/sys/class/net/wlan0/address", "r");
	if (fp != NULL) {
		fgets(buff, 255, (FILE*) fp);
		return buff;
	}
	return "null==" + (fp == NULL);
}

char * getMac2() {
	static char mac_addr[80] = { 0 };
	printf("MAC = %d, %s\n", get_mac_addr(mac_addr), mac_addr);
	return mac_addr;
}

int get_mac_addr(char *mac_addr) {
	int sockfd;
	struct ifreq ifr;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) >= 0) {
		//strncpy(ifr.ifr_name, "eth0", IFNAMESIZE);
		strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ);

		ifr.ifr_addr.sa_family = AF_INET;

		if (ioctl(sockfd, SIOCGIFHWADDR, (char*) &ifr) == 0) {
			sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
					(unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[0],
					(unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[1],
					(unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[2],
					(unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[3],
					(unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[4],
					(unsigned char) ifr.ifr_ifru.ifru_hwaddr.sa_data[5]);
			return 0;
		}
	}
	/* error */
	return -1;
}

char* getImei() {
	static char g_imei[15];
	static char imei_start[15];
	int ir = __system_property_get("persist.radio.imei", imei_start);
	__android_log_print(ANDROID_LOG_ERROR, "TAG", ir+"~~~get mac address failed~~"+ir);
	if (ir > 0) {
		imei_start[15] = 0; //strz end
		printf("method1 got imei %s len %d\r\n", imei_start,
				strlen(imei_start));
		strcpy(g_imei, imei_start);
		return g_imei;
	} else {
		__android_log_print(ANDROID_LOG_ERROR, "TAG", ir+"~~~get else~~"+ir);
		LOGD("els %d",ir);
		printf("method1 imei failed - trying method2\r\n");
		//old dumpsys imei getter
//		char* res = exec_get_out("dumpsys iphonesubinfo");
		char* res = exec_get_out("service call iphonesubinfo 3");

		LOGD("els %s",res);
		__android_log_print(ANDROID_LOG_ERROR, "TAG", res);
		const char* imei_start_match = "ID = ";
		int imei_start_match_len = strlen(imei_start_match);
		char* imei_start = strstr(res, imei_start_match);
		if (imei_start && strlen(imei_start) >= 15 + imei_start_match_len) {
			imei_start += imei_start_match_len;
			imei_start[15] = 0;
			printf("method2 IMEI [%s] len %d\r\n", imei_start,
					strlen(imei_start));
			strcpy(g_imei, imei_start);
			return g_imei;
		}
	}
	return g_imei;
}
/*bool GetMac(char *pBuffer)
 {
 struct ifreq *ifr;
 struct ifconf ifc;
 int s, i;
 int numif;

 // find number of interfaces.
 memset(&ifc, 0, sizeof(ifc));
 ifc.ifc_ifcu.ifcu_req = NULL;
 ifc.ifc_len = 0;

 if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
 {
 CCLog("SgProfile::GetMac 111111");
 return false;
 }

 if (ioctl(s, SIOCGIFCONF, &ifc) < 0)
 {
 CCLog("SgProfile::GetMac 222222");
 return false;
 }

 if ((ifr = (ifreq*) malloc(ifc.ifc_len)) == NULL)
 {
 CCLog("SgProfile::GetMac 333333");
 return false;
 }

 ifc.ifc_ifcu.ifcu_req = ifr;

 if (ioctl(s, SIOCGIFCONF, &ifc) < 0)
 {
 CCLog("SgProfile::GetMac 333333 ioctl SIOCGIFCONF error!");
 return false;
 }

 numif = ifc.ifc_len / sizeof(struct ifreq);

 for (i = 0; i < numif; i++)
 {
 struct ifreq *r = &ifr[i];
 struct sockaddr_in *sin = (struct sockaddr_in*)&r->ifr_addr;
 if (!strcmp(r->ifr_name, "lo"))
 continue; // skip loopback interface

 // get MAC address
 if(ioctl(s, SIOCGIFHWADDR, r) < 0)
 {
 CCLog("SgProfile::GetMac 333333 ioctl(SIOCGIFHWADDR) error!");
 continue;
 }

 memcpy(pBuffer,r->ifr_hwaddr.sa_data,6);
 close(s);
 free(ifr);

 return true;
 }

 return false;
 }*/

