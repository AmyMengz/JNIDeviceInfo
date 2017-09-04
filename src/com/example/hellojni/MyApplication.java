package com.example.hellojni;

import android.app.Application;
import android.content.Context;

public class MyApplication extends Application {
	private static MyApplication myApplication;
	public static Context getContextObj(){
		return myApplication;
	}
	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		super.onCreate();
		myApplication = this;
	}

}
