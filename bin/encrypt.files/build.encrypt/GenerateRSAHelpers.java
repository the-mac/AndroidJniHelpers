import org.apache.commons.codec.binary.Base64;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.security.Key;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Properties;
import java.util.Scanner;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;

public class GenerateRSAHelpers {
	private static boolean DEBUGGING = false;
	private static boolean HAS_STATIC_MEMBERS = false;
	private static boolean HAS_NATIVE_MEMBERS = false;

	private static HashMap<String,String> mContent = new HashMap<String, String>();
	private static String VERSION_DELIMIT = new String(new char[] { 126, 126 });//"~~";
	CryptoHelper cryptoHelper;

	String className, packageName;


	public GenerateRSAHelpers(String className) {

//		int lastIndex = className.lastIndexOf('.');
//
//		if(lastIndex > -1) {
//			packageName = className.substring(0, lastIndex);
//			this.className = className.substring(lastIndex + 1);
//		}
//		else {
//			packageName = "";
//			this.className = className;
//		}
		try {
			cryptoHelper = new CryptoHelper();
		} catch (Exception e) {
			e.printStackTrace();
		}
//		methodMapping.put("constructor", "// This is a placeholder for the constructor");
//		methodMapping.put("boolean", "// This is a placeholder for a boolean function");

//		InputStream input = null;
//		try {
//			// load a properties file
//			input = new FileInputStream("encryptDataTypes.properties");
//			mDataTypes.load(input);
//			input.close();
//
//			// load a properties file
//			input = new FileInputStream("encryptReturnValues.properties");
//			mReturnValues.load(input);
//		} catch (IOException ex) {
//			ex.printStackTrace();
//		} finally {
//			if (input != null) {
//				try {
//					input.close();
//				} catch (IOException e) {
//					e.printStackTrace();
//				}
//			}
//		}
	}

	@Override
	public String toString() {
		StringBuilder output = new StringBuilder();

		String content = mContent.toString();
		content = content.replace("{","");
		content = content.replace("}","");
		content = content.replace(",","\n");
		output.append(content);
		
		return output.toString();
	}


	//	@Override public String toString() {
//		StringBuilder output = new StringBuilder();
//		StringBuilder examples = new StringBuilder();
//		StringBuilder header = new StringBuilder();
//		StringBuilder source = new StringBuilder();
//		StringBuilder signatures = new StringBuilder();
//		StringBuilder static_signatures = new StringBuilder();
//
//		boolean hasPackage = packageName.length() == 0;
//		String thePackage = hasPackage ? "" : packageName+".";
//
//		output.append(String.format("\n%s%s\n", thePackage, className).replace(' ', '*'));
//		String dividingLine = String.format("\n%80s\n\n", "").replace(' ', '*');
//		output.append(dividingLine);
//		output.append(String.format("%s%s.encrypt was generated as a helper for %s.java using the\nbin/encrypt.bash script. ", thePackage, className, className));
//		output.append(String.format("The following code segments are C++ header and source code containing:\n\n", thePackage, className));
//		output.append(String.format("- getCanonicalName: The method that enables the relationship between C++ and Java.\n"));
//
//		String allMethodNames = methodNames.toString();
//		output.append(String.format("- Java methods: %s\n\n", allMethodNames.substring(1, allMethodNames.length() - 1)));
//		output.append(String.format("The source code can be copied into the respective %s.h and\n%s.cpp files in a location of your choice.", className, className));
//		output.append(String.format(" Finally, the last segment\ncontains an example of what these method calls would look like in your code.\n"));
//
//		for (int index = 0; index < listOfClassSignatures.size(); index++) {
//
//			String signature = listOfClassSignatures.get(index);
//			String javaMethodSignature = methods.get(index);
//			String methodName = methodNames.get(index).replace(".", "/");
//			String returnType = returnTypes.get(index);
//
//			if("".equals(returnType)) continue;
//
//			boolean isStaticMethod = javaMethodSignature.contains("static");
//			boolean isNativeMethod = javaMethodSignature.contains("native");
//
//
//			if(isStaticMethod) {
//				String prefix = static_signatures.length() == 0 ? "    " : ",\n    ";
//				static_signatures.append(String.format("%s    { \"%s\", \"%s\" }", prefix, methodName, signature));
//				signatures.append(String.format("    cacheSignature(\"%s\", \"%s\");\n", methodName, signature));
//			} else if(isNativeMethod) {
//				signatures.append(String.format("    addNativeSignature(\"%s\", (void*)&%s::%s, \"%s\");\n", methodName, className, methodName, signature));
//			}
//			else {
//				signatures.append(String.format("    cacheSignature(\"%s\", \"%s\");\n", methodName, signature));
//			}
//		}
//
//		header.append("#include \"EncryptHelpers.h\"\n\n");
//
//		if(HAS_NATIVE_MEMBERS) {
//			header.append("class "+ className +" : public NativeObject {\n");
//		}
//		else {
//			header.append("class "+ className +" : public JavaClass {\n");
//		}
//	    if(HAS_STATIC_MEMBERS) {
//			header.append("    static std::map<std::string, std::string> static_signatures;\n");
//			//header.append("    static std::map<std::string, std::string> static_signatures = {\n");
//			//header.append(static_signatures + "\n");
//			//header.append("    };\n");
//	    }
//		header.append(java_instance + "\n");
//		header.append(public_head + "\n");
//
//		String classSignatures = signatures.toString();
//
//		String commentary = "    /**\n"+
//			"    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain \n"+
//			"    * the Encrypt Helper's relationship to the "+className+" class defined in Java.\n"+
//			"    */";
//		String getCanonicalName = String.format("%s\n%s", commentary, getMethod("const char *"));
//		getCanonicalName = getCanonicalName.replace("{CLASS_NAME}", className);
//
//		String canonicalPath = String.format("\"%s\"", packageName.replace(".", "/"));
//		getCanonicalName = getCanonicalName.replace("{PACKAGE_NAME}", canonicalPath);
//		header.append(getCanonicalName + "\n");
//
//	    if(HAS_STATIC_MEMBERS) {
//			commentary = "    /**\n"+
//					"    * This facsimile of the Java method java.lang.Object.getClass() is used to maintain \n"+
//					"    * the Encrypt Helper's static relationship to the "+className+" class defined in Java.\n"+
//					"    */";
//			String getClass = String.format("%s\n%s", commentary, getMethod("getClass"));
//			getClass = getClass.replace("{CLASS_NAME}", className);
//
//			String getClassPath = String.format("\"%s\"", packageName.replace(".", "/"));
//			getClass = getClass.replace("{PACKAGE_NAME}", getClassPath);
//			header.append(getClass + "\n");
//
//			commentary = "    /**\n"+
//					"    * The getStaticSignature method is used to get the Encrypt Helper's\n"+
//					"    * static signature for the "+className+" class defined in Java.\n"+
//					"    */";
//			String getStaticSignature = String.format("%s\n%s", commentary, getMethod("static_signatures"));
//			getStaticSignature = getStaticSignature.replace("{CLASS_NAME}", className);
//			header.append(getStaticSignature + "\n");
//
//			header.append(getMethod("addStaticSignature") + "\n\n");
//	    }
//
//		String defaultConstructorInHeader = String.format("%s", getHelperTemplate("defaultConstructor").replace(" : {NATIVE_CLASS}() {}", ""));
//		defaultConstructorInHeader = defaultConstructorInHeader.replace("{CLASS_NAME}", className);
//		defaultConstructorInHeader = defaultConstructorInHeader.replace("{CLASS_SIGNATURES}", classSignatures);
//		defaultConstructorInHeader = defaultConstructorInHeader.replace("{NATIVE_CLASS}", HAS_NATIVE_MEMBERS ? "NativeObject" : "JavaClass");
//		header.append(String.format("    %s;\n\n", defaultConstructorInHeader
//				.replace(className + "::", "").split("\n")[0]));
//
//		commentary = "";
//		String defaultConstructor = String.format("%s\n%s", commentary, getHelperTemplate("defaultConstructor"));
//		defaultConstructor = defaultConstructor.replace("{CLASS_NAME}", className);
//		defaultConstructor = defaultConstructor.replace("{CLASS_SIGNATURES}", classSignatures);
//		defaultConstructor = defaultConstructor.replace("{NATIVE_CLASS}", HAS_NATIVE_MEMBERS ? "NativeObject" : "JavaClass");
//
//		source.append(defaultConstructor + "\n\n");
//
//
//		String constructorInHeader = String.format("%s", getHelperTemplate("constructor").replace(" : {NATIVE_CLASS}(env)", ""));
//		constructorInHeader = constructorInHeader.replace("{CLASS_NAME}", className);
//		constructorInHeader = constructorInHeader.replace("{CLASS_SIGNATURES}", classSignatures);
//		constructorInHeader = constructorInHeader.replace("{NATIVE_CLASS}", HAS_NATIVE_MEMBERS ? "NativeObject" : "JavaClass");
//		header.append(String.format("    %s;\n\n", constructorInHeader
//				.replace(className + "::", "").split("\n")[0]));
//
//		commentary = "/**\n"+
//				"* Here you can construct the "+className+" object how ever you need to, \n"+
//				"* as well as load signatures for the Java instance method calls.\n"+
//				"*/";
//		String constructor = String.format("%s\n%s", commentary, getHelperTemplate("constructor"));
//		constructor = constructor.replace("{CLASS_NAME}", className);
//		constructor = constructor.replace("{CLASS_SIGNATURES}", classSignatures);
//		constructor = constructor.replace("{NATIVE_CLASS}", HAS_NATIVE_MEMBERS ? "NativeObject" : "JavaClass");
//
//		source.append(constructor + "\n\n");
//
//		String initialize = String.format("%s", getHelperTemplate("initialize"));
//		initialize = initialize.replace("{CLASS_NAME}", className);
//		initialize = initialize.replace("{CLASS_SIGNATURES}", classSignatures);
//		source.append(initialize + "\n\n");
//		header.append(String.format("    %s;\n\n", initialize
//				.replace(className + "::", "").split("\n")[0]));
//
//		String mapFields = String.format("%s", getHelperTemplate("mapFields"));
//		mapFields = mapFields.replace("{CLASS_NAME}", className);
//		mapFields = mapFields.replace("{CLASS_SIGNATURES}", classSignatures);
//		source.append(mapFields + "\n\n");
//		header.append(String.format("    %s;\n\n", mapFields
//				.replace(className + "::", "").split("\n")[0]));
//
//		for (String methodName : methodNames) {
//			int index = methodNames.indexOf(methodName);
//			String returnType = returnTypes.get(index);
//			String parameters = listOfParameters.get(index);
//			String parameterNames = listOfParameterNames.get(index);
//			String javaMethodSignature = methods.get(index);
//
//			if("".equals(returnType)) continue;
//
//
//			String method = getMethod(returnType);
//			if(method.length() == 0) method = String.format("// %s %s\n", returnType, methodName);
//
//			method = method.replace("{CLASS_NAME}", className);
//			method = method.replace("{METHOD_NAME}", methodName);
//			method = method.replace("{PARAMETERS}", parameters);
//			method = method.replace("{PARAMETER_NAMES}", parameterNames);
//			method = method.replace(", getMethod(", ", getJavaMethod(env, ");
//			method = method.replace("(getMethod(", "(getJavaMethod(env, ");
//
//			String prefix = "";
//			String variableName = "instance";
//			boolean isStaticMethod = javaMethodSignature.contains("static");
//			boolean isNativeMethod = javaMethodSignature.contains("native");
//
//			if(isStaticMethod) {
//				prefix = "static ";
//				variableName = className + "::";
//				method = method.replace("thisObj", "getClass(env)");
//				method = method.replace("->Call", "->CallStatic");
//				method = method.replace("getJavaMethod(env, ", "getStaticMethod(env, getClass(env), ");
//			} else if(isNativeMethod) {
//				prefix = "static ";
//
//				int firstBrace = method.indexOf("{");
//				int lastBrace = method.indexOf("}");
//
//				boolean hasNoReturnValue = "".equals(returnType) || "void".equals(returnType);
//				String returnPrefix = hasNoReturnValue ? "" : "return ";
//
//				String callNative = getHelperTemplate("callNative");
//				callNative = callNative.replace("{CLASS_NAME}", returnPrefix+className);
//				callNative = callNative.replace("{METHOD_NAME}", methodName);
//				callNative = callNative.replace("{PARAMETERS}", parameters);
//				callNative = callNative.replace("{PARAMETER_NAMES}", parameterNames);
//
////				method = method.replace(methodName+"(JNI", methodName+"Native");
//				method = String.format("%s %s", method.substring(0, firstBrace).trim(), callNative);
//				source.append(method.replace(methodName+"(JNI", methodName+"Native(JNI") + "\n\n");
//
//				String defaultReturnedValue = hasNoReturnValue ? "" : String.format("    return %s;\n", mReturnValues.getProperty(returnType));
//				String staticInitializer = "    {CLASS_NAME} *object = gClasses.getNativeInstance<{CLASS_NAME}>(env, java_this);";
//				staticInitializer = staticInitializer.replace("{CLASS_NAME}", className);
//
//
//				String embed = "\n    if (object != NULL)\n    {\n	// TODO: ADD YOUR NATIVE IMPLMENTATION HERE (i.e. object->callToSomeFunction())\n    }\n";// + method.substring(firstBrace, lastBrace + 1).replace("\n", "\n    ");
//
//
//				method = String.format("%s{\n%s\n    %s\n%s}\n", method.substring(0, firstBrace).replace("env, ", "env, jobject java_this, ").replace("env)", "env, jobject java_this)"), staticInitializer, embed, defaultReturnedValue);
//				method = method.replace("getMethod(", "object->getMethod(");
//				method = method.replace("thisObj", "java_this");
//
//				String prototype = String.format("    %s%s;\n\n", prefix, method.replace(className + "::", "").replace(" {\n", "\n").split("\n")[0]);
//				header.append(prototype.replace("static ", "").replace(", jobject java_this", "").replace(methodName+"(JNI", methodName+"Native(JNI"));
//			}
//
//			header.append(String.format("    %s%s;\n\n", prefix, method.replace(className + "::", "").replace(" {\n", "\n").split("\n")[0]));
//
//			source.append(method + "\n\n");
//
//			if("".equals(returnType)) {
//				examples.append(String.format("%s %s(env);\n\n", className, variableName));
//			} else if("void".equals(returnType)) {
//				String type = setUpParameterTypes(returnType);
//				examples.append(String.format( "%s%s(env%s);\n\n",
//					isStaticMethod ? variableName : variableName +".", methodName, parameterNames));
//			} else {
//				String type = setUpParameterTypes(returnType);
//				examples.append(String.format("%s %sReturnValue = %s%s(env%s);\n\n",
//					type, type, isStaticMethod ? variableName : variableName +".", methodName, parameterNames));
//			}
//		}
//		header.append(tail + "\n");
//
//
//
//		output.append(dividingLine);
//		output.append(header.toString());
//		output.append(dividingLine);
//		output.append(source.toString());
//		output.append(dividingLine);
//		output.append(examples.toString());
//		return output.toString();
//	}

//	private String setUpParameterTypes(String parameter) {
//
//		if(parameter.contains("java.lang.String")) {
//			parameter = parameter.replace("java.lang.String", mDataTypes.getProperty("java.lang.String"));
//		}
//		else if(parameter.contains(".")) {
//			parameter = mDataTypes.getProperty("*");
//		}
//		else {
//
//			parameter = parameter.replace("boolean", mDataTypes.getProperty("boolean"));
//			parameter = parameter.replace("byte", mDataTypes.getProperty("byte"));
//			parameter = parameter.replace("int", mDataTypes.getProperty("int"));
//			parameter = parameter.replace("float", mDataTypes.getProperty("float"));
//			parameter = parameter.replace("double", mDataTypes.getProperty("double"));
//			parameter = parameter.replace("char", mDataTypes.getProperty("char"));
//			parameter = parameter.replace("long", mDataTypes.getProperty("long"));
//			parameter = parameter.replace("short", mDataTypes.getProperty("short"));
//
//		}
//
//		parameter = parameter.replace("[]", "Array");
//
//		return parameter;
//
//	}
//
//	private String getMethod(String returnType) {
////		if("".equals(returnType)) returnType = "constructor";
//		if(returnType != null && !"".equals(returnType) && !methodMapping.containsKey(returnType)) {
//			methodMapping.put(returnType, getHelperTemplate(returnType));
//		}
//		return methodMapping.get(returnType);
//	}
//
//	private String getHelperTemplate(String returnType) {
//		return "";
////		String fileName = "";
////		if("initialize".equals(returnType)) {
////			fileName = "encryptMethods/initialize.encryptMethod";
////		} else if("defaultConstructor".equals(returnType)) {
////			fileName = "encryptMethods/defaultConstructor.encryptMethod";
////		} else if("constructor".equals(returnType)) {
////			fileName = "encryptMethods/constructor.encryptMethod";
////		} else if("mapFields".equals(returnType)) {
////			fileName = "encryptMethods/mapFields.encryptMethod";
////		} else if("callNative".equals(returnType)) {
////			fileName = "encryptMethods/callNative.encryptMethod";
////		} else if("static_signatures".equals(returnType)) {
////			fileName = "encryptMethods/getStaticSignature.encryptMethod";
////		} else if("addStaticSignature".equals(returnType)) {
////			fileName = "encryptMethods/addStaticSignature.encryptMethod";
////		} else if("getClass".equals(returnType)) {
////			fileName = "encryptMethods/getClass.encryptMethod";
////		} else if("const char *".equals(returnType)) {
////			fileName = "encryptMethods/getCanonicalName.encryptMethod";
////		} else if("java.lang.String".equals(returnType)) {
////			fileName = "encryptMethods/CallStringMethod.encryptMethod";
////		} else if(returnType.contains(".")) {
////			fileName = "encryptMethods/CallObjectMethod.encryptMethod";
////		} else if("int".equals(returnType)) {
////			fileName = "encryptMethods/CallIntMethod.encryptMethod";
////		} else if("byte[]".equals(returnType)) {
////			fileName = "encryptMethods/CallByteArrayMethod.encryptMethod";
////		} else if("void".equals(returnType)) {
////			fileName = "encryptMethods/CallVoidMethod.encryptMethod";
////		} else if("char".equals(returnType)) {
////			fileName = "encryptMethods/CallCharMethod.encryptMethod";
////		} else if("boolean".equals(returnType)) {
////			fileName = "encryptMethods/CallBooleanMethod.encryptMethod";
////		} else { return ""; }
////		try { return new Scanner(new File(fileName)).useDelimiter("\\A").next(); }
////		catch(Exception e) {
////			System.err.format("\nLoading Helper Template (%s) failed with ReturnType (%s)\n\n", fileName, returnType);
////			e.printStackTrace();
////			return null;
////		}
//	}

	public boolean isCorrectFileType(String fileName) {

		int beginIndex = fileName.lastIndexOf(".");

		boolean isAndroidJava = true;//VALID_FILE_TYPES.indexOf(".java") >= 0;
		boolean isAndroidXML = isAndroidJava && "AndroidManifest.xml".equals(fileName);
		boolean isAndroidCpp = isAndroidJava && "us_the_mac_reel_life_ReelApplication.cpp".equals(fileName);

		boolean isObjectiveC = false;//VALID_FILE_TYPES.indexOf(".m") >= 0;
		boolean isObjectiveCApp = isObjectiveC && "ReelApplication.mm".equals(fileName);

		return beginIndex != -1 && (
				(isAndroidXML || isAndroidCpp || isObjectiveCApp)
						|| true);//VALID_FILE_TYPES.indexOf(fileName.substring(beginIndex)) >= 0);

	}

	public boolean isCorrectStringType(String prefix, String extraction, String type, String line) {
		return isCorrectCategoryType(type) && isNotInComment(line, extraction) && isNotAnInvalidPrefix(prefix) && isNotIgnored(extraction);
	}

	public boolean isCorrectCategoryType(String type) {
		return "network".equals(type) || "database".equals(type);// || "app".equals(type);
	}

	public boolean isNotIgnored(String content) {
		return true;//IGNORED.indexOf(content) < 0 && !content.endsWith(".h");
	}

	public boolean isNotAnInvalidPrefix(String prefix) {
		return true;//INVALID_PREFIX_TYPES.indexOf(prefix) < 0;
	}

	public boolean isNotInComment(String line, String extraction) {
		/*[0]//DIRECTORS_ = "Directors"; */
		String comment = "//";

		int commentIndex = line.indexOf(comment); // 0
		boolean hasComment = commentIndex >= 0;

		int leftIndex = line.indexOf(extraction); // 21
		boolean textIsBeforeComment = commentIndex > leftIndex;

		return !hasComment || hasComment && textIsBeforeComment;
	}

	public void extractFromFile(File file) throws IOException {

		String fileName = file.getName();

		String input = new Scanner(file).useDelimiter("\\Z").next();
		String[] contents = input.split("\"");
		boolean hasStrings = contents.length > 1;

		if(hasStrings) {

			System.out.println("Extracting from file " + fileName);

			// Reading objects
			FileInputStream fis = new FileInputStream(file);
			DataInputStream in = new DataInputStream(fis);
			BufferedReader br = new BufferedReader(new InputStreamReader(in));

			int lineNumber = 0;
			String line;

			while ((line = br.readLine()) != null) {
				++lineNumber;
				for (int loopIndex = 1; loopIndex < contents.length; loopIndex += 2) {

					String extraction = contents[loopIndex];
					boolean hasString = line.contains(extraction);

					if(hasString) {
						try {

							int beginExtractionIndex = line.indexOf(extraction);
							String prefix = beginExtractionIndex > 0 ? line.substring(0,beginExtractionIndex - 1).replaceAll("^\\s+", "") : "";

							int beginIndex = fileName.lastIndexOf(".");
							String typeCheck = fileName.substring(0, beginIndex + 1);
							String type = "android";

							if(isCorrectStringType(prefix, extraction, type, line)) {

								boolean alreadyCached = mContent.containsKey(extraction);

								String platform = "android";
								String fileAndLine = fileName + "(" + lineNumber + ")";
								String contentPrefix = platform + "/" + fileAndLine;

								mContent.put(extraction, contentPrefix+VERSION_DELIMIT+type);
							}

						} catch(Exception e) { e.printStackTrace(); }
					}
				}
			}

			br.close();
			in.close();
			fis.close();

		}
	}

	public static void main (String[] args) throws Exception {

		if(args.length == 1 && args[0].equals("--key")) {
			if(DEBUGGING) System.out.printf("\nargs: %s\n\n", Arrays.toString(args));

			GenerateRSAHelpers helper = new GenerateRSAHelpers(args[0]);

			System.out.println (helper.generateKey());

		} else if(args.length == 2) {

			String inputFile = args.length == 0 ? "GenerateEncryptHelpers.encryptBlueprint" : args[0];

			File encryptFile = new File(inputFile);
			boolean isEncryptFileAvailable = encryptFile.exists();

			String contents = isEncryptFileAvailable ? new Scanner(encryptFile).useDelimiter("\\A").next() : "";
			if(DEBUGGING) System.out.printf("\ncontents: %s\n\n", contents);

			String[] array = contents.split("\"");

			GenerateRSAHelpers helper = new GenerateRSAHelpers(inputFile.replace(".encryptBlueprint", ""));

			for (int loopIndex = 1; loopIndex < array.length; loopIndex += 2) {

				String extraction = array[loopIndex];
				String encryptedText = helper.encrypt(extraction);
				mContent.put(extraction, encryptedText);

			}

			if(!DEBUGGING) System.out.println (helper.toString());
		} else { System.out.println ("Improper Input: " + Arrays.toString(args)); }

	}

	private String encrypt(String extraction) throws Exception {
		return cryptoHelper.encrypt(extraction);
	}

	private String generateKey() throws Exception {
		return new String( Base64.encodeBase64( cryptoHelper.generateIV() ));
	}

	/**
     * Created by christopher on 2/12/17.
     */

	public static class CryptoHelper {

		private Key key;
		private File accessFile = new File("../../../library/src/main/assets/native_key");

		public CryptoHelper( Key key ) {
			this.key = key;
		}

		public CryptoHelper() throws Exception {
			this( generateSymmetricKey() );
		}

		public String encrypt( String plaintext ) throws Exception {
			return encrypt( accessIV(), plaintext );
		}

		public String encrypt( byte [] iv, String plaintext ) throws Exception {

			byte [] decrypted = plaintext.getBytes();
			byte [] encrypted = encrypt( iv, decrypted );

//			StringBuilder ciphertext = new StringBuilder();
//
//			ciphertext.append( Base64.encodeBase64( iv ) );
//			ciphertext.append( ":" );
//			ciphertext.append( Base64.encodeBase64( encrypted ) );

			return new String( Base64.encodeBase64( encrypted ) );//ciphertext.toString(); //

		}

		public String decrypt( String ciphertext ) throws Exception {
			String [] parts = ciphertext.split( ":" );
			byte [] iv = Base64.decodeBase64( parts[0].getBytes() );
			byte [] encrypted = Base64.decodeBase64( parts[1].getBytes() );
			byte [] decrypted = decrypt( iv, encrypted );
			return new String( decrypted );
		}

		public Key getKey() {
			return key;
		}

		public void setKey( Key key ) {
			this.key = key;
		}

		public byte [] generateIV() {
			SecureRandom random = new SecureRandom();
			byte [] iv = new byte [16];
			random.nextBytes( iv );
			return iv;
		}

		public byte [] accessIV() {
			Scanner scanner = null;
			try {
				if(DEBUGGING) System.out.printf("\naccessFile: %s\n", accessFile);
				if(DEBUGGING) System.out.printf("\naccessFile Exists: %s\n\n", accessFile.exists());
				scanner = new Scanner(accessFile).useDelimiter("\\A");
			} catch (FileNotFoundException e) {
				e.printStackTrace();
				return null;
			}
			String contents = scanner.next();
			byte [] iv = new byte [16];
			if(DEBUGGING) System.out.printf("\naccessFile Contents: %s\n", contents);
			if(DEBUGGING) System.out.printf("\naccessFile Length: %s\n", iv.length);

			byte[] bytes = contents.getBytes();
			for (int i = 0; i < bytes.length; i++) {
				iv[i] = bytes[i];
			}
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
}