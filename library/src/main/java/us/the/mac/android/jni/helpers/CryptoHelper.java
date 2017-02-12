package us.the.mac.android.jni.helpers;

import java.security.Key;
import java.security.SecureRandom;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;

/**
 * Created by christopher on 2/12/17.
 */

public class CryptoHelper {
    private Key key;

    public CryptoHelper( Key key ) {
        this.key = key;
    }

    public CryptoHelper() throws Exception {
        this( generateSymmetricKey() );
    }

    public static Key generateSymmetricKey() throws Exception {
        KeyGenerator generator = KeyGenerator.getInstance( "AES" );
        SecretKey key = generator.generateKey();
        return key;
    }

    public byte [] decrypt( byte [] iv, byte [] ciphertext ) throws Exception {
        Cipher cipher = Cipher.getInstance( key.getAlgorithm() + "/CBC/PKCS5Padding" );
        cipher.init( Cipher.DECRYPT_MODE, key, new IvParameterSpec( iv ) );
        return cipher.doFinal( ciphertext );
    }

}
