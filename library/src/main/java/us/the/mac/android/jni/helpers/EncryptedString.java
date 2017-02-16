package us.the.mac.android.jni.helpers;

import android.content.res.AssetManager;
import android.net.Uri;

import com.spotify.jni.NativeObject;

import java.io.File;
import java.io.InputStream;
import java.util.Scanner;

import static android.R.attr.path;

/**
 * Created by christopher on 1/14/17.
 */

public class EncryptedString extends NativeObject {
    private static final int BASE = 0;
    private static final int INCREMENT = 1;

    public static final int INLINE_STRINGS_ALGORITHM = BASE + INCREMENT;
    public static final int RESOURCE_STRINGS_ALGORITHM = INLINE_STRINGS_ALGORITHM + INCREMENT;

    public String encryptedString;

    @Override
    public boolean equals(Object obj) {
        if(encryptedString != null && obj instanceof EncryptedString) {
            EncryptedString str = (EncryptedString) obj;
            if(str.encryptedString != null) {
                return encryptedString.equals(str.encryptedString);
            }
        }
        return false;
    }

    @Override public String toString() { return encryptedString; }

    @Override
    public native void destroy();

    public native String decrypt(int algorithm);

    public InputStream getFileStream(String file) {
        if(AndroidJniApp.Instance() == null) return null;
//        return AndroidJniApp.Instance().getFilesDir().toString();
//        return "file:///android_asset";
        try {
            AssetManager assetManager = AndroidJniApp.Instance().getAssets();
            return assetManager.open(file);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    public static EncryptedString testingInstance() { return new EncryptedString(); }

}
