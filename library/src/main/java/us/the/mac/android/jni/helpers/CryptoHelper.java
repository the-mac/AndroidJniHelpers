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
        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding" );
        cipher.init( Cipher.DECRYPT_MODE, generateKey(), new IvParameterSpec( iv ) );
        return cipher.doFinal( ciphertext );
    }
    public String decrypt( String ciphertext ) throws Exception {
        String [] parts = ciphertext.split( ":" );
        byte [] iv = Base64.decodeBase64( parts[0].getBytes() );
        byte [] encrypted = Base64.decodeBase64( parts[1].getBytes() );
        byte [] decrypted = decrypt( iv, encrypted );
        return new String( decrypted );
    }

    @Override
    public native void destroy();

    public static CryptoHelper testingDefault() { return new CryptoHelper(); }

}
