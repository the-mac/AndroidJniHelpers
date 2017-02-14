import org.apache.commons.codec.binary.Base64;

import java.security.Key;
import java.security.SecureRandom;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;

public class CryptoHelper_0 {

    public String encrypt( String plaintext ) throws Exception {
        return encrypt( generateIV(), plaintext );
    }

    public String encrypt( byte [] iv, String plaintext ) throws Exception {

        byte [] decrypted = plaintext.getBytes();
        byte [] encrypted = encrypt( iv, decrypted );

        StringBuilder ciphertext = new StringBuilder();

        ciphertext.append( Base64.encodeBase64( iv ) );
        ciphertext.append( ":" );
        ciphertext.append( Base64.encodeBase64( encrypted ) );

        return ciphertext.toString();

    }

    public String decrypt( String ciphertext ) throws Exception {
        String [] parts = ciphertext.split( ":" );
        byte [] iv = Base64.decodeBase64( parts[0].getBytes() );
        byte [] encrypted = Base64.decodeBase64( parts[1].getBytes() );
        byte [] decrypted = decrypt( iv, encrypted );
        return new String( decrypted );
    }

    private Key key;

    public CryptoHelper_0(Key key ) {
        this.key = key;
    }

    public CryptoHelper_0() throws Exception {
        this( generateSymmetricKey() );
    }

    public Key getKey() {
        return key;
    }

    public void setKey( Key key ) {
        this.key = key;
    }

    public static byte [] generateIV() {
        SecureRandom random = new SecureRandom();
        byte [] iv = new byte [16];
        random.nextBytes( iv );
        return iv;
    }

    public static Key generateSymmetricKey() throws Exception {
        KeyGenerator generator = KeyGenerator.getInstance( "AES" );
        SecretKey key = generator.generateKey();
        return key;
    }

    public byte [] encrypt( byte [] iv, byte [] plaintext ) throws Exception {
        Cipher cipher = Cipher.getInstance( key.getAlgorithm() + "/CBC/PKCS5Padding" );
        cipher.init( Cipher.ENCRYPT_MODE, key, new IvParameterSpec( iv ) );
        return cipher.doFinal( plaintext );
    }

    public byte [] decrypt( byte [] iv, byte [] ciphertext ) throws Exception {
        Cipher cipher = Cipher.getInstance( key.getAlgorithm() + "/CBC/PKCS5Padding" );
        cipher.init( Cipher.DECRYPT_MODE, key, new IvParameterSpec( iv ) );
        return cipher.doFinal( ciphertext );
    }

}