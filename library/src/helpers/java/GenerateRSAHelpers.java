import java.io.File;
import java.io.FileNotFoundException;
import java.security.Key;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Scanner;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import org.apache.commons.codec.binary.Base64;

/*
 * Arguments List:
 *
 *   --key --debug
 *
 *   --encrypt "The input string..." key_file --debug
 *
 *   file/path/to/blueprintFile file/path/to/key_file --debug
 *
 */
public class GenerateRSAHelpers {
	private static boolean DEBUGGING = false;

	private static File accessFile = null;//new File("key_file");
	private static HashMap<String,String> mContent = new HashMap<String, String>();
	private static HashMap<String,Integer> mNamesMap = new HashMap<String, Integer>();

	CryptoHelper cryptoHelper;
	String className, packageName;

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

	public GenerateRSAHelpers() {
		try {
			cryptoHelper = new CryptoHelper();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public GenerateRSAHelpers(String keyPath) throws Exception {
		try {

			accessFile = new File(keyPath);
			String contents = new Scanner(accessFile).useDelimiter("\\A").next();
			if(DEBUGGING) System.out.printf("File Contents: %s\n\n", contents);

			byte [] decodedKey = Base64.decodeBase64( contents.getBytes() );
			SecretKey key = new SecretKeySpec(decodedKey, 0, decodedKey.length, "AES");
			cryptoHelper = new CryptoHelper(key);

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public String toString() {
		StringBuilder output = new StringBuilder();
		output.append("\n");

		boolean hasPackage = packageName.length() == 0;
		String thePackage = hasPackage ? "" : packageName+".";
		output.append(String.format("%s%s\n", thePackage, className).replace(' ', '*'));

		String dividingLine = String.format("\n%80s\n\n", "").replace(' ', '*');
		output.append(dividingLine);
		output.append(String.format("%s%s.encrypt was generated as a helper for %s.java using the\nbin/encrypt.bash script. ", thePackage, className, className));
		output.append(String.format("The following segments are RSA helper components containing:\n\n"));
		output.append(String.format("- Encryption Mapping: Each input string is mapped to its android resource name and its encryption string.\n"));
		output.append(String.format("- Generated Resources: Each android resource name is used with its related encryption string.\n\n"));

		output.append(String.format("The generated resource code can be copied into the respective values/strings.xml file of your choice."));
		output.append(String.format(" Finally, the last segment\ncontains an example of what these resources would look like in your code.\n"));
		output.append(dividingLine);

		String content = "\t" + mContent.toString();
		content = content.replace("~=~","~~");
		content = content.replace("{","");
		content = content.replace("}","");
		content = content.replace(", ","\n\t");
		content = content.replace("~~"," ~~ ");
		output.append(content+"\n");
		output.append(dividingLine);

		StringBuilder resourceUsages = new StringBuilder();
		Iterator it = mContent.entrySet().iterator();
		while (it.hasNext()) {
			Map.Entry pair = (Map.Entry)it.next();

			// <string name="hello">Hello!</string>
			if(DEBUGGING) System.out.println(pair.getKey() + " = " + pair.getValue());
			String[] values = pair.getValue().toString().split("~~");
			String resourceName = values[0].substring(1);
			String resourceValue = values[1];
			output.append(String.format("\t<string name=\"%s\">%s</string>\n", resourceName, resourceValue));
			resourceUsages.append(String.format("\tString %s = decryptString(R.string.%s);\n\n", resourceName, resourceName));
		}

		output.append(dividingLine);
		output.append(resourceUsages);
		output.append(dividingLine);
		return output.toString();
	}

	private String encrypt(String encrypted) throws Exception {
		return cryptoHelper.encrypt(encrypted);
	}

	private String decrypt(String plaintext) throws Exception {
		return cryptoHelper.decrypt(plaintext);
	}

	private byte[] getEncoded() {
		return cryptoHelper.getKey().getEncoded();
	}

	public static void main(String[] args) throws Exception {

		String arguments = Arrays.toString(args);
		DEBUGGING = arguments.contains("--debug");

		if(DEBUGGING) System.out.printf("\nargs: %s\n\n", arguments);

		if(arguments.contains("--key")) {

			GenerateRSAHelpers helper = new GenerateRSAHelpers();

			String inputString = "This is an input test string";
			if(DEBUGGING) System.out.println( inputString );

			String ciphertext = helper.encrypt( inputString );
			if(DEBUGGING) System.out.println( ciphertext );

			String decryptedString = helper.decrypt( ciphertext );
			if(DEBUGGING) System.out.println( decryptedString );

			if(!inputString.equals(decryptedString))
				throw new IllegalArgumentException(String.format(
						"The input string (%s) has not been decrypted properly (%s).",
						inputString, decryptedString));

			System.out.println(new String(Base64.encodeBase64(helper.getEncoded())));

		} else if(arguments.contains("--encrypt")) {

			String inputString = args[1];
			String keyPath = args[2];

			if(DEBUGGING) System.out.println("Key Path: "+keyPath);

			GenerateRSAHelpers helper = new GenerateRSAHelpers(keyPath);

			String encryptedString = helper.encrypt(inputString);
			String decryptedString = helper.decrypt(encryptedString);

			if(!inputString.equals(decryptedString))
				throw new IllegalArgumentException(String.format(
						"The input string (%s) has not been decrypted properly (%s).",
						inputString, decryptedString));

			if(DEBUGGING) System.out.println("Input String: "+inputString);
			if(DEBUGGING) System.out.println("Encrypted String: "+encryptedString);
			if(DEBUGGING) System.out.println("Decrypted String: "+decryptedString);

			System.out.println(encryptedString);

		} else if(args.length >= 2) {

			String blueprintPath = args[0];
			String keyPath = args[1];
			if(DEBUGGING) System.out.println("File Path: "+blueprintPath);
			if(DEBUGGING) System.out.println("Key Path: "+keyPath);

			GenerateRSAHelpers helper = new GenerateRSAHelpers(keyPath);

			File blueprintTextFile = new File(blueprintPath);
			boolean isEncryptFileAvailable = blueprintTextFile.exists();

			int lastIndex = blueprintPath.lastIndexOf('.');
			lastIndex = blueprintPath.substring(0, lastIndex).lastIndexOf('.');
			blueprintPath = blueprintPath.replace(".encryptBlueprint", "");

			if(lastIndex > -1) {
				helper.packageName = blueprintPath.substring(0, lastIndex);
				helper.className = blueprintPath.substring(lastIndex + 1);
			}
			else {
				helper.packageName = "";
				helper.className = blueprintPath;
			}
			String contents = isEncryptFileAvailable ? new Scanner(blueprintTextFile).useDelimiter("\\A").next() : "";
			if(DEBUGGING) System.out.printf("File Contetnts: %s\n\n", contents);

			String[] array = contents.split("\"");

			for (int loopIndex = 1; loopIndex < array.length; loopIndex += 2) {

				String extraction = array[loopIndex];
				String encryptedString = helper.encrypt(extraction);
				String decryptedString = helper.decrypt(encryptedString);

				if(!extraction.equals(decryptedString))
					throw new IllegalArgumentException(String.format(
							"The input string (%s) has not been decrypted properly (%s).",
							extraction, decryptedString));

				String generatedName = array[loopIndex - 1].toLowerCase().trim();
				generatedName = generatedName.replace(");\n", "");
				generatedName = generatedName.replace("\\s+", " ");
				generatedName = generatedName.replaceAll("[^A-Za-z0-9 ]", "_");
				generatedName = generatedName.replace("__", "_");
				generatedName = generatedName.replace("__", "_");
				generatedName = generatedName.trim();

				Integer index = 0;

				if(mNamesMap.containsKey(generatedName)) {
					index = mNamesMap.get(generatedName);
					++index;
					mNamesMap.put(generatedName, index);
				}
				else {
					index = 0;
					mNamesMap.put(generatedName, index);
				}

				// Using Format: (Generated Name/Number~~Encrypted String)
				String content = generatedName+index+"~~"+encryptedString;
				mContent.put(extraction+"~", "~"+content);

			}

			System.out.println(helper.toString());
		} else { System.out.println ("Improper Input: " + Arrays.toString(args)); }
	}

	public static class CryptoHelper {

		private Key key;

		public CryptoHelper( Key key ) {
			this.key = key;
		}

		public CryptoHelper() throws Exception {
			this( generateSymmetricKey() );
		}

		public Key getKey() {
			return key;
		}

		public void setKey( Key key ) {
			this.key = key;
		}

		public static Key generateSymmetricKey() throws Exception {
			KeyGenerator generator = KeyGenerator.getInstance( "AES" );
			SecretKey key = generator.generateKey();
			return key;
		}

		public String encrypt( String plaintext ) throws Exception {
			return encrypt( generateIV(), plaintext );
		}

		public String encrypt( byte [] iv, String plaintext ) throws Exception {

			byte [] decrypted = plaintext.getBytes();
			byte [] encrypted = encrypt( iv, decrypted );

			StringBuilder ciphertext = new StringBuilder();

//			ciphertext.append( Base64.encodeBase64String( iv ).replace("==", Character.toString((char)8)+Character.toString((char)8)) );
			ciphertext.append( Base64.encodeBase64String( iv ) );
			ciphertext.append( ":" );
			ciphertext.append( Base64.encodeBase64String( encrypted ) );

			return ciphertext.toString();

		}

		public byte [] encrypt( byte [] iv, byte [] plaintext ) throws Exception {
			Cipher cipher = Cipher.getInstance( key.getAlgorithm() + "/CBC/PKCS5Padding" );
			cipher.init( Cipher.ENCRYPT_MODE, key, new IvParameterSpec( iv ) );
			return cipher.doFinal( plaintext );
		}

		public String decrypt( String ciphertext ) throws Exception {
//			String [] parts = ciphertext.replace(Character.toString((char)8)+Character.toString((char)8), "==:").split( ":" );
			String [] parts = ciphertext.split( ":" );
			byte [] iv = Base64.decodeBase64( parts[0] );
			byte [] encrypted = Base64.decodeBase64( parts[1] );
			byte [] decrypted = decrypt( iv, encrypted );
			return new String( decrypted );
		}

		public byte [] decrypt( byte [] iv, byte [] ciphertext ) throws Exception {
			Cipher cipher = Cipher.getInstance( key.getAlgorithm() + "/CBC/PKCS5Padding" );
			cipher.init( Cipher.DECRYPT_MODE, key, new IvParameterSpec( iv ) );
			return cipher.doFinal( ciphertext );
		}

	}
}