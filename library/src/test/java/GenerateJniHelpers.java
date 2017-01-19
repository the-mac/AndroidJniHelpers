import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Properties;
import java.util.Scanner;

public class GenerateJniHelpers {
	private static boolean DEBUGGING = false;
	private static boolean HAS_STATIC_MEMBERS = false;
	private static boolean HAS_NATIVE_MEMBERS = false;

	String className, packageName;
	ArrayList<String> methods = new ArrayList<String>();
	ArrayList<String> methodNames = new ArrayList<String>();
	ArrayList<String> returnTypes = new ArrayList<String>();
	ArrayList<String> listOfParameters = new ArrayList<String>();
	ArrayList<String> listOfParameterNames = new ArrayList<String>();
	ArrayList<String> listOfRegisteredMethods = new ArrayList<String>();
	ArrayList<String> listOfClassSignatures = new ArrayList<String>();
	HashMap<String,String> methodMapping = new HashMap<String,String>();
	Properties mDataTypes = new Properties();
	Properties mReturnValues = new Properties();

    
	String public_head  = "  public:";
	String java_instance  = "    jobject thisObj;";
	String tail  		= "};";
	public GenerateJniHelpers(String className) {

		int lastIndex = className.lastIndexOf('.');
		
		if(lastIndex > -1) {
			packageName = className.substring(0, lastIndex);
			this.className = className.substring(lastIndex + 1);	
		}
		else {
			packageName = "";
			this.className = className;
		}
		
//		methodMapping.put("constructor", "// This is a placeholder for the constructor");
//		methodMapping.put("boolean", "// This is a placeholder for a boolean function");

		InputStream input = null;
		try {
			// load a properties file
			input = new FileInputStream("jniDataTypes.properties");
			mDataTypes.load(input);
			input.close();

			// load a properties file
			input = new FileInputStream("jniReturnValues.properties");
			mReturnValues.load(input);
		} catch (IOException ex) {
			ex.printStackTrace();
		} finally {
			if (input != null) {
				try {
					input.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	@Override public boolean equals(Object obj) { return false; }
	@Override public String toString() {
		StringBuilder output = new StringBuilder();
		StringBuilder examples = new StringBuilder();
		StringBuilder header = new StringBuilder();
		StringBuilder source = new StringBuilder();
		StringBuilder signatures = new StringBuilder();
		StringBuilder static_signatures = new StringBuilder();

		boolean hasPackage = packageName.length() == 0;
		String thePackage = hasPackage ? "" : packageName+".";

		output.append(String.format("\n%s%s\n", thePackage, className).replace(' ', '*'));
		String dividingLine = String.format("\n%80s\n\n", "").replace(' ', '*');
		output.append(dividingLine);
		output.append(String.format("%s%s.jni was generated as a helper for %s.java using the\nbin/jni.bash script. ", thePackage, className, className));
		output.append(String.format("The following code segments are C++ header and source code containing:\n\n", thePackage, className));
		output.append(String.format("- getCanonicalName: The method that enables the relationship between C++ and Java.\n"));

		String allMethodNames = methodNames.toString();
		output.append(String.format("- Java methods: %s\n\n", allMethodNames.substring(1, allMethodNames.length() - 1)));
		output.append(String.format("The source code can be copied into the respective %s.h and\n%s.cpp files in a location of your choice.", className, className));
		output.append(String.format(" Finally, the last segment\ncontains an example of what these method calls would look like in your code.\n"));

		for (int index = 0; index < listOfClassSignatures.size(); index++) {

			String signature = listOfClassSignatures.get(index);
			String javaMethodSignature = methods.get(index);
			String methodName = methodNames.get(index).replace(".", "/");
			String returnType = returnTypes.get(index);

			if("".equals(returnType)) continue;

			boolean isStaticMethod = javaMethodSignature.contains("static");
			boolean isNativeMethod = javaMethodSignature.contains("native");


			if(isStaticMethod) {
				String prefix = static_signatures.length() == 0 ? "    " : ",\n    ";
				static_signatures.append(String.format("%s    { \"%s\", \"%s\" }", prefix, methodName, signature));
				signatures.append(String.format("    addStaticSignature(\"%s\", \"%s\");\n", methodName, signature));
			} else if(isNativeMethod) {
				signatures.append(String.format("    addNativeSignature(\"%s\", (void*)&%s::%s, \"%s\");\n", methodName, className, methodName, signature));
			}
			else {
				signatures.append(String.format("    addJavaSignature(\"%s\", \"%s\");\n", methodName, signature));
			}
		}
		if(HAS_NATIVE_MEMBERS) {
			header.append("class "+ className +" : public NativeObject {\n");
		}
		else {
			header.append("class "+ className +" : public JavaClass {\n");
		}
	    if(HAS_STATIC_MEMBERS) {
			header.append("    static std::map<std::string, std::string> static_signatures;\n");
			//header.append("    static std::map<std::string, std::string> static_signatures = {\n");
			//header.append(static_signatures + "\n");
			//header.append("    };\n");
	    }
		header.append(java_instance + "\n");
		header.append(public_head + "\n");

		String classSignatures = signatures.toString();

		String commentary = "    /**\n"+
			"    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain \n"+
			"    * the Jni Helper's relationship to the "+className+" class defined in Java.\n"+
			"    */";
		String getCanonicalName = String.format("%s\n%s", commentary, getMethod("const char *"));
		getCanonicalName = getCanonicalName.replace("{CLASS_NAME}", className);

		String canonicalPath = String.format("\"%s\"", packageName.replace(".", "/"));
		getCanonicalName = getCanonicalName.replace("{PACKAGE_NAME}", canonicalPath);
		header.append(getCanonicalName + "\n");

	    if(HAS_STATIC_MEMBERS) {
			commentary = "    /**\n"+
					"    * This facsimile of the Java method java.lang.Object.getClass() is used to maintain \n"+
					"    * the Jni Helper's static relationship to the "+className+" class defined in Java.\n"+
					"    */";
			String getClass = String.format("%s\n%s", commentary, getMethod("getClass"));
			getClass = getClass.replace("{CLASS_NAME}", className);

			String getClassPath = String.format("\"%s\"", packageName.replace(".", "/"));
			getClass = getClass.replace("{PACKAGE_NAME}", getClassPath);
			header.append(getClass + "\n");

			commentary = "    /**\n"+
					"    * The getStaticSignature method is used to get the Jni Helper's\n"+
					"    * static signature for the "+className+" class defined in Java.\n"+
					"    */";
			String getStaticSignature = String.format("%s\n%s", commentary, getMethod("static_signatures"));
			getStaticSignature = getStaticSignature.replace("{CLASS_NAME}", className);
			header.append(getStaticSignature + "\n");

			header.append(getMethod("addStaticSignature") + "\n\n");
	    }

		String defaultConstructorInHeader = String.format("%s", getHelperTemplate("defaultConstructor").replace(" : {NATIVE_CLASS}() {}", ""));
		defaultConstructorInHeader = defaultConstructorInHeader.replace("{CLASS_NAME}", className);
		defaultConstructorInHeader = defaultConstructorInHeader.replace("{CLASS_SIGNATURES}", classSignatures);
		defaultConstructorInHeader = defaultConstructorInHeader.replace("{NATIVE_CLASS}", HAS_NATIVE_MEMBERS ? "NativeObject" : "JavaClass");
		header.append(String.format("    %s;\n\n", defaultConstructorInHeader
				.replace(className + "::", "").split("\n")[0]));

		commentary = "";
		String defaultConstructor = String.format("%s\n%s", commentary, getHelperTemplate("defaultConstructor"));
		defaultConstructor = defaultConstructor.replace("{CLASS_NAME}", className);
		defaultConstructor = defaultConstructor.replace("{CLASS_SIGNATURES}", classSignatures);
		defaultConstructor = defaultConstructor.replace("{NATIVE_CLASS}", HAS_NATIVE_MEMBERS ? "NativeObject" : "JavaClass");

		source.append(defaultConstructor + "\n\n");


		String constructorInHeader = String.format("%s", getHelperTemplate("constructor").replace(" : {NATIVE_CLASS}(env)", ""));
		constructorInHeader = constructorInHeader.replace("{CLASS_NAME}", className);
		constructorInHeader = constructorInHeader.replace("{CLASS_SIGNATURES}", classSignatures);
		constructorInHeader = constructorInHeader.replace("{NATIVE_CLASS}", HAS_NATIVE_MEMBERS ? "NativeObject" : "JavaClass");
		header.append(String.format("    %s;\n\n", constructorInHeader
				.replace(className + "::", "").split("\n")[0]));

		commentary = "/**\n"+
				"* Here you can construct the "+className+" object how ever you need to, \n"+
				"* as well as load signatures for the Java instance method calls.\n"+
				"*/";
		String constructor = String.format("%s\n%s", commentary, getHelperTemplate("constructor"));
		constructor = constructor.replace("{CLASS_NAME}", className);
		constructor = constructor.replace("{CLASS_SIGNATURES}", classSignatures);
		constructor = constructor.replace("{NATIVE_CLASS}", HAS_NATIVE_MEMBERS ? "NativeObject" : "JavaClass");

		source.append(constructor + "\n\n");

		String initialize = String.format("%s", getHelperTemplate("initialize"));
		initialize = initialize.replace("{CLASS_NAME}", className);
		initialize = initialize.replace("{CLASS_SIGNATURES}", classSignatures);
		source.append(initialize + "\n\n");
		header.append(String.format("    %s;\n\n", initialize
				.replace(className + "::", "").split("\n")[0]));

		String mapFields = String.format("%s", getHelperTemplate("mapFields"));
		mapFields = mapFields.replace("{CLASS_NAME}", className);
		mapFields = mapFields.replace("{CLASS_SIGNATURES}", classSignatures);
		source.append(mapFields + "\n\n");
		header.append(String.format("    %s;\n\n", mapFields
				.replace(className + "::", "").split("\n")[0]));

		for (String methodName : methodNames) {
			int index = methodNames.indexOf(methodName);
			String returnType = returnTypes.get(index);
			String parameters = listOfParameters.get(index);
			String parameterNames = listOfParameterNames.get(index);
			String javaMethodSignature = methods.get(index);

			if("".equals(returnType)) continue;


			String method = getMethod(returnType);
			if(method.length() == 0) method = String.format("// %s %s\n", returnType, methodName);

			method = method.replace("{CLASS_NAME}", className);
			method = method.replace("{METHOD_NAME}", methodName);
			method = method.replace("{PARAMETERS}", parameters);
			method = method.replace("{PARAMETER_NAMES}", parameterNames);
			method = method.replace(", getMethod(", ", getJavaMethod(env, ");
			method = method.replace("(getMethod(", "(getJavaMethod(env, ");

			String prefix = "";
			String variableName = "instance";
			boolean isStaticMethod = javaMethodSignature.contains("static");
			boolean isNativeMethod = javaMethodSignature.contains("native");

			if(isStaticMethod) {
				prefix = "static ";
				variableName = className + "::";
				method = method.replace("thisObj", "getClass(env)");
				method = method.replace("->Call", "->CallStatic");
				method = method.replace("getJavaMethod(env, ", "getStaticMethod(env, getClass(env), ");
			} else if(isNativeMethod) {
				prefix = "static ";

				int firstBrace = method.indexOf("{");
				int lastBrace = method.indexOf("}");

				boolean hasNoReturnValue = "".equals(returnType) || "void".equals(returnType);
				String returnPrefix = hasNoReturnValue ? "" : "return ";

				String callNative = getHelperTemplate("callNative");
				callNative = callNative.replace("{CLASS_NAME}", returnPrefix+className);
				callNative = callNative.replace("{METHOD_NAME}", methodName);
				callNative = callNative.replace("{PARAMETERS}", parameters);
				callNative = callNative.replace("{PARAMETER_NAMES}", parameterNames);

//				method = method.replace(methodName+"(JNI", methodName+"Native");
				method = String.format("%s %s", method.substring(0, firstBrace).trim(), callNative);
				source.append(method.replace(methodName+"(JNI", methodName+"Native(JNI") + "\n\n");

				String defaultReturnedValue = hasNoReturnValue ? "" : String.format("    return %s;\n", mReturnValues.getProperty(returnType));
				String staticInitializer = "    {CLASS_NAME} *object = gClasses.getNativeInstance<{CLASS_NAME}>(env, java_this);";
				staticInitializer = staticInitializer.replace("{CLASS_NAME}", className);


				String embed = "\n    if (object != NULL)\n    {\n	// TODO: ADD YOUR NATIVE IMPLMENTATION HERE (i.e. object->callToSomeFunction())\n    }\n";// + method.substring(firstBrace, lastBrace + 1).replace("\n", "\n    ");


				method = String.format("%s{\n%s\n    %s\n%s}\n", method.substring(0, firstBrace).replace("env, ", "env, jobject java_this, ").replace("env)", "env, jobject java_this)"), staticInitializer, embed, defaultReturnedValue);
				method = method.replace("getMethod(", "object->getMethod(");
				method = method.replace("thisObj", "java_this");

				String prototype = String.format("    %s%s;\n\n", prefix, method.replace(className + "::", "").replace(" {\n", "\n").split("\n")[0]);
				header.append(prototype.replace("static ", "").replace(", jobject java_this", "").replace(methodName+"(JNI", methodName+"Native(JNI"));
			}

			header.append(String.format("    %s%s;\n\n", prefix, method.replace(className + "::", "").replace(" {\n", "\n").split("\n")[0]));

			source.append(method + "\n\n");

			if("".equals(returnType)) {
				examples.append(String.format("%s %s(env);\n\n", className, variableName));
			} else if("void".equals(returnType)) {
				String type = setUpParameterTypes(returnType);
				examples.append(String.format( "%s%s(env%s);\n\n",
					isStaticMethod ? variableName : variableName +".", methodName, parameterNames));
			} else {
				String type = setUpParameterTypes(returnType);
				examples.append(String.format("%s %sReturnValue = %s%s(env%s);\n\n",
					type, type, isStaticMethod ? variableName : variableName +".", methodName, parameterNames));
			}
		}
		header.append(tail + "\n");



		output.append(dividingLine);
		output.append(header.toString());
		output.append(dividingLine);
		output.append(source.toString());
		output.append(dividingLine);
		output.append(examples.toString());
		return output.toString();
	}

	/**
	 * @param parameter
	 * @param afterName
	 * @param parametersIndex
	 * @return
	 */
	private String setUpParameterTypes(String parameter) {

		if(parameter.contains("java.lang.String")) {
			parameter = parameter.replace("java.lang.String", mDataTypes.getProperty("java.lang.String"));			
		}
		else if(parameter.contains(".")) {
			parameter = mDataTypes.getProperty("*");
		}
		else {
			
			parameter = parameter.replace("boolean", mDataTypes.getProperty("boolean"));
			parameter = parameter.replace("byte", mDataTypes.getProperty("byte"));
			parameter = parameter.replace("int", mDataTypes.getProperty("int"));
			parameter = parameter.replace("float", mDataTypes.getProperty("float"));
			parameter = parameter.replace("double", mDataTypes.getProperty("double"));
			parameter = parameter.replace("char", mDataTypes.getProperty("char"));
			parameter = parameter.replace("long", mDataTypes.getProperty("long"));
			parameter = parameter.replace("short", mDataTypes.getProperty("short"));
			
		}
		
		parameter = parameter.replace("[]", "Array");		
		
		return parameter;
		
	}
	
	private String getMethod(String returnType) {
//		if("".equals(returnType)) returnType = "constructor";
		if(returnType != null && !"".equals(returnType) && !methodMapping.containsKey(returnType)) {
			methodMapping.put(returnType, getHelperTemplate(returnType));
		}
		return methodMapping.get(returnType);
	}

	private String getHelperTemplate(String returnType) {
		String fileName = "";
		if("initialize".equals(returnType)) {
			fileName = "jniMethods/initialize.jniMethod";
		} else if("defaultConstructor".equals(returnType)) {
			fileName = "jniMethods/defaultConstructor.jniMethod";
		} else if("constructor".equals(returnType)) {
			fileName = "jniMethods/constructor.jniMethod";
		} else if("mapFields".equals(returnType)) {
			fileName = "jniMethods/mapFields.jniMethod";
		} else if("callNative".equals(returnType)) {
			fileName = "jniMethods/callNative.jniMethod";
		} else if("static_signatures".equals(returnType)) {
			fileName = "jniMethods/getStaticSignature.jniMethod";
		} else if("addStaticSignature".equals(returnType)) {
			fileName = "jniMethods/addStaticSignature.jniMethod";
		} else if("getClass".equals(returnType)) {
			fileName = "jniMethods/getClass.jniMethod";
		} else if("const char *".equals(returnType)) {
			fileName = "jniMethods/getCanonicalName.jniMethod";
		} else if("java.lang.String".equals(returnType)) {
			fileName = "jniMethods/CallStringMethod.jniMethod";
		} else if(returnType.contains(".")) {
			fileName = "jniMethods/CallObjectMethod.jniMethod";
		} else if("int".equals(returnType)) {
			fileName = "jniMethods/CallIntMethod.jniMethod";
		} else if("byte[]".equals(returnType)) {
			fileName = "jniMethods/CallByteArrayMethod.jniMethod";
		} else if("void".equals(returnType)) {
			fileName = "jniMethods/CallVoidMethod.jniMethod";
		} else if("char".equals(returnType)) {
			fileName = "jniMethods/CallCharMethod.jniMethod";
		} else if("boolean".equals(returnType)) {
			fileName = "jniMethods/CallBooleanMethod.jniMethod";
		} else { return ""; }
		try { return new Scanner(new File(fileName)).useDelimiter("\\A").next(); }
		catch(Exception e) {
			System.err.format("\nLoading Helper Template (%s) failed with ReturnType (%s)\n\n", fileName, returnType);
			e.printStackTrace();
			return null;
		}
	}

	public static void main (String[] args) throws FileNotFoundException {
		
		String input = args.length == 0 ? "GenerateJniHelpers.jniBlueprint" : args[0];
		File jniFile = new File(input);
		boolean isJniFileAvailable = jniFile.exists();
		
		String contents = isJniFileAvailable ? new Scanner(jniFile).useDelimiter("\\A").next() : "";
		if(DEBUGGING) System.out.printf("\ncontents: %s\n\n", contents);

		String[] array = contents.split("--\n");
		
		GenerateJniHelpers helloWorld = new GenerateJniHelpers(input.replace(".jniBlueprint", ""));
		
		for (String string : array) {
			
			if(string.contains("private")) continue;
//			if(DEBUGGING) System.out.printf("\nstring: %s\n\n", string);
			
			if(string.contains(");")) {

				String[] components = string.split("\n");
				String method = components[0].trim();
				
				String methodName = components[0].trim();
				int afterName = methodName.indexOf('(');
				int parametersIndex = methodName.indexOf(')');
				
				String[] nameComponents =  methodName.substring(0, afterName).split(" ");
				String[] parameterComponents =  methodName.substring(afterName + 1, parametersIndex).split(", ");

				int parameterCount = 0;
				String parameters = "";
				String parameterNames = "";

				for (String parameter : parameterComponents) {
					if("".equals(parameter)) continue;
					String parameterType = helloWorld.setUpParameterTypes(parameter);
					String parameterName = parameterType + "Value"+ (++parameterCount);
					parameters +=  String.format(", %s %s", parameterType, parameterName);
					parameterNames +=  String.format(", %s", parameterName);
				}
				
				int index = nameComponents.length - 2;
				if(index < 0) index = nameComponents.length - 1;
				
				methodName = nameComponents[nameComponents.length - 1];
				String returnType = nameComponents[index].replace("public", "");
				
				String methodSignature = components[1].replace("descriptor: ", "").trim();
				
				helloWorld.methods.add(method);		
				helloWorld.methodNames.add(methodName);
				helloWorld.returnTypes.add(returnType);
				helloWorld.listOfParameters.add(parameters);
				helloWorld.listOfParameterNames.add(parameterNames);
				helloWorld.listOfClassSignatures.add(methodSignature);
				
			}
			String methods = helloWorld.methods.toString();
			HAS_STATIC_MEMBERS = methods.contains("static");
			HAS_NATIVE_MEMBERS = methods.contains("native");
		}

		if(!DEBUGGING) System.out.println (helloWorld.toString());		
	}

}