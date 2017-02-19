package us.the.mac.android.jni.helpers;

import com.spotify.jni.NativeObject;

import org.apache.commons.codec.binary.Base64;

import java.security.Key;
import java.security.SecureRandom;
import java.util.Scanner;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

/**
 * Created by christopher on 2/12/17.
 */

public class CryptoHelper extends NativeObject {
    public native Key generateKey();

    public byte [] decrypt( byte [] iv, byte [] ciphertext ) throws Exception {
        Key key = generateKey();
        Cipher cipher = Cipher.getInstance(key.getAlgorithm() + "/CBC/PKCS5Padding" );
        cipher.init( Cipher.DECRYPT_MODE, key, new IvParameterSpec( iv ) );
        return cipher.doFinal( ciphertext );
    }

    @Override
    public native void destroy();
}
