package us.the.mac.android.jni.helpers;

import com.spotify.jni.NativeObject;

/**
 * Created by christopher on 1/14/17.
 */

public class EncodedString extends NativeObject {
    public String encodedString;

    @Override
    public boolean equals(Object obj) {
        if(encodedString != null && obj instanceof EncodedString) {
            EncodedString str = (EncodedString) obj;
            if(str.encodedString != null) {
                return encodedString.equals(str.encodedString);
            }
        }
        return false;
    }

    @Override public String toString() { return encodedString; }

    @Override
    public native void destroy();

    public native String decrypt(int algorithm);

    public static EncodedString testingInstance() { return new EncodedString(); }

}
