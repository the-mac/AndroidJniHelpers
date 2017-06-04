package us.the.mac.library.demo.androidjni;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.database.Cursor;
import android.net.Uri;
import android.util.Log;

import us.the.mac.android.jni.helpers.AndroidJniApp;

public class MainProvider extends ContentProvider {
	public static final String WELCOME_MESSAGE = AndroidJniApp.getS(0);

	@Override
	public boolean onCreate() {
		Log.d(getClass().getName(), "Called onCreate successfully");
		return true;
	}

	@Override public Cursor query(Uri uri, String[] strings, String s, String[] strings1, String s1) {
		return null;
	}

	@Override public String getType(Uri uri) {
		return null;
	}

	@Override public Uri insert(Uri uri, ContentValues contentValues) {
		return null;
	}

	@Override public int delete(Uri uri, String s, String[] strings) {
		return 0;
	}

	@Override public int update(Uri uri, ContentValues contentValues, String s, String[] strings) {
		return 0;
	}

	static {
		Log.d(MainProvider.class.getName(), "Called static {...} successfully");
		// SET AUTHORITY: us.the.mac.demo.library.androidjni.provider
	}
}