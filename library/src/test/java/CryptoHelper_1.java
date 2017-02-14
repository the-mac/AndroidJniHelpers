import org.apache.commons.codec.binary.Base64;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.security.Key;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Scanner;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;

public class CryptoHelper_1 {

    private static String VERSION = getCurrent() + ".tsv";//"15.08.14.1.tsv";//"15.10.21.1.tsv";//"15.10.10.1.tsv";
    private static String VERSION_DELIMIT = new String(new char[] { 126, 126 });//"~~";
    private static String VALID_FILE_TYPES = ".m,.java";//"15.10.10.1.tsv";
    private static String INVALID_PREFIX_TYPES = "Log.d(getClass().getName(), Log.d(ApplicationService.class.getName(),,// ,A_FAMILY_HEADS_TO = @,ABOUT_A_1839_MUTINY = @,PETER_PAN_WILLIAMS = @,THE_LIVES_OF_TWO_MOB = @,if(debug) LOGI(,if(DEBUG) NSLog(@,#import,static char const * const,System.loadLibrary,static NSString *const,static NSString *const ";//"15.10.10.1.tsv";
    private static String IGNORED = "Service Reel Count response sent:, home, reelsCount(%d), uncachedCount(%d),reelServiceIds: %s,request: %s,jsonObject: %s,size=%d values=%s,<code>#<i>argb</i></code>,<code>#<i>aarrggbb</i></code>,<code>14.5sp</code>,<code>100</code>,Theme.AppCompat,<code>#<i>rgb</i></code>,<code>false</code>,<code>?[<i>package</i>:][<i>type</i>:]<i>name</i></code>,<code>@[+][<i>package</i>:]<i>type</i>:<i>name</i></code>,<code>true</code>,isRunningAlphaTests().isRunningUITests=,isRunningReelQueueTests,isRunningWatchNowScreenTests,isRunningUpdatesScreenTests,isRunningUITests,testQueueProcessing,dropdown,%@,%@,\\n\\n%@\\n\\n,162.209.99.206,_UIConstraintBasedLayoutLogUnsatisfiable,Z8VOcHJvcCByby5wcm1kdWNOLm9vZGVs,Z8VOcHJvcCByby5zZXJpYWxubw,\\n%s\\n,\\\\A,',,',pointer,owner,selector,search,sw_rear,sw_front,sw_right,SimpleDateFormat,SetJavaScriptEnabled,Url: ,deprecation,Recycle";

    private static String LOCAL_PATH = "/Users/christopher/git/reel-life-content/String-Constants/stripped/"+VERSION;
    private static String EXPORTS_PATH = "/Users/christopher/git/reel-life-content/reel_content_tests/export/.c/"+VERSION;
    private static String[] VERSIONS_DIRECTORIES = { LOCAL_PATH, EXPORTS_PATH };

    private static String ANDROID_DEV_ROOT_DIRECTORY = "/Users/christopher/git/reel-life-content/android-development/ReelContent.16.08.XX.X/src/us/the/mac/reel/life";
    private static String ANDROID_DEV_JNI_DIRECTORY = "/Users/christopher/git/reel-life-content/android-development/ReelContent.16.08.XX.X/jni";
    private static String IOS_DEV_APP_DIRECTORY = "/Users/christopher/git/reel-life-content/ios-development/ReelLife/ReelLife";
    //	private static String IOS_FRAMEWORK_DIRECTORY = "/Users/christopher/git/reel-content-alternate/ReelKit";
    private static String IOS_DEV_KIT_DIRECTORY = "/Users/christopher/git/reel-life-content/ios-development/ReelLife/ReelKit";
    private static String[] EXTRACTION_DEV_DIRECTORIES = { ANDROID_DEV_ROOT_DIRECTORY, ANDROID_DEV_JNI_DIRECTORY, IOS_DEV_APP_DIRECTORY, IOS_DEV_KIT_DIRECTORY } ;

    private static String ANDROID_LIBRARY_DIRECTORY = "/Users/christopher/git/reel-life-content/android-%s/ReelContentLibrary/src/com/operators/service/library";
    private static String ANDROID_ROOT_DIRECTORY = "/Users/christopher/git/reel-life-content/android-%s/ReelContent.16.08.XX.X/src/us/the/mac/reel/life";
    private static String ANDROID_JNI_DIRECTORY = "/Users/christopher/git/reel-life-content/android-%s/ReelContent.16.08.XX.X/jni";
    private static String IOS_APP_DIRECTORY = "/Users/christopher/git/reel-life-content/ios-%s/ReelLife/ReelLife";
    private static String IOS_KIT_DIRECTORY = "/Users/christopher/git/reel-life-content/ios-%s/ReelLife/ReelKit";
    private static String[] EXTRACTION_REPLACEMENT_DIRECTORIES = { ANDROID_LIBRARY_DIRECTORY, ANDROID_ROOT_DIRECTORY, ANDROID_JNI_DIRECTORY, IOS_APP_DIRECTORY, IOS_KIT_DIRECTORY } ;

    //	private static String APPTYPES = "ReelApplication.,ReelSettings.,";
//	private static String CONTENTTYPES = "ReelAPIProvider.,SwiftCoreDataHelper.";
    private static String NETWORKTYPES = "ApplicationService.,ServiceApplication.,ReelApplication.,Network.,SSLSocketFactory.,Alert.,Trailer.,FailedCertificateDelegate.";//,us_the_mac_reel_life_ReelApplication.
    private static String DATABASETYPES = "ReelAPIProvider.,Status.,Cursor.,ReelCursor.,ReelUpdates.,";
    private static String SCREENTYPES = "SidebarViewController.,SWRevealViewController.,ReelContentScreen.,SplashScreen.,TrailerFullScreen.,InitialStepsScreen.,UpdatesController.,SuggestsScreen.,SuggestsFragment.,ReelListFragment.,ReelFragment.,UpdatesFragment.";
    private static String VIEWTYPES = "UIScrollView+EmptyDataSet.,UIWebView+Id.,YouTubePresenter.,YouTubeView.,YouTubeButton.,UpdatesWatchNowCell.,ReelStatusSubGroup.,ReelStatusGroup.,ReelContentGroup.,ReelContentButton.,MenuAdapter.,MainMenu.,MenuItems.,";

    private static String ANDROID_HEAD_FILE = "/Users/christopher/git/reel-life-content/android-alpha/.git";
    private static String IOS_HEAD_FILE = "/Users/christopher/git/reel-life-content/ios-alpha/.git";

    private static String ANDROID_RELEASE_HEAD_FILE = "/Users/christopher/git/reel-life-content/android-release/.git";
    private static String IOS_RELEASE_HEAD_FILE = "/Users/christopher/git/reel-life-content/ios-release/.git";

    private static String ANDROID_PRERELEASE_FILE = "/Users/christopher/git/reel-life-content/android-release/ReelContent.16.08.XX.X/jni/prerelease";
    private static String IOS_PRERELEASE_FILE = "/Users/christopher/git/reel-life-content/ios-release/ReelLife/prerelease";

    /* A map of strings where:
     * key represents a string to be replaced
     * value is a string to replace key with
     */
    private static Map<String, Integer> replaceStrings;
    private static HashMap<String,String> mContent = new HashMap<String, String>();
    private static String mKey;
    private static boolean hasReleaseArg;
    private static boolean hasAndroidArg;
    private static boolean hasIOSArg;


    /**
     * Extract replace strings data.
     *
     * @param dataFile
     * @throws IOException
     */
    public static void extractReplaceData(String dataFile) throws IOException {
        System.out.println("\n========================Starting the Extraction Process!========================");

        for(String extractDirectory : EXTRACTION_DEV_DIRECTORIES) {
            if(extractDirectory.contains("/ios"))
                mKey = "ios"; else mKey = "android";

            File root = new File(extractDirectory);
            extractFromDirectory(root);
        }

        System.gc();
        try { Thread.sleep(1500); } catch(Exception e) {}


        System.out.println("========================Starting the Extraction Write!========================");

        StringBuilder sb = getContentString();

        System.gc();
        try { Thread.sleep(1500); } catch(Exception e) {}

        for(String writePath : VERSIONS_DIRECTORIES) {
            System.out.println("Extraction to: " + writePath);
//            FileWriter fr = new FileWriter(writePath, true);
//            fr.write(sb.toString());
//            fr.close();
        }

        System.out.println("========================Ending the Extraction Process!========================\n");
    }

    public static StringBuilder getContentString() {
        StringBuilder sb = new StringBuilder();
        Iterator<Entry<String, String>> it = mContent.entrySet().iterator();
        while (it.hasNext()) {
            Entry<String, String> pair = (Entry<String, String>)it.next();

            String key = pair.getKey();
            sb.append(pair.getValue() + VERSION_DELIMIT + key +(it.hasNext()?"\n":""));

            it.remove(); // avoids a ConcurrentModificationException
        }
        return sb;
    }

    public static String getCurrent() {
        File current = new File("/Users/christopher/git/reel-life-content/archives/.current");
        try { return new Scanner(current).useDelimiter("\\Z").next(); }
        catch(Exception e) { e.printStackTrace(); return ""; }
    }

    public static void extractFromDirectory(File directory) throws IOException {
        System.out.println("Evaluating directory " + directory.getName());

        try {
            for (File node: directory.listFiles()) {
                if (node.isFile())
                    extractFromFile(node);
                else if (node.isDirectory())
                    extractFromDirectory(node);
            }
        } catch(Exception e) {
            System.err.println("Failed Evaluating directory " + directory.getAbsolutePath() + "\n" + e);
        }
    }

    public static void extractFromFile(File file) throws IOException {

        String fileName = file.getName();

        // If a file has an appropriate extension then extract from it
        if (isCorrectFileType(fileName)) {

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
                                String type = getType(typeCheck);

                                if(isCorrectStringType(prefix, extraction, type, line)) {

                                    boolean alreadyCached = mContent.containsKey(extraction);
                                    boolean hasAndroid = alreadyCached && "android".equals(mContent.get(extraction));
                                    boolean hasIOS = alreadyCached && "ios".equals(mContent.get(extraction));
                                    boolean fromAndroid = "android".equals(mKey);

                                    boolean alreadyHasOtherPlatform = alreadyCached
                                            && ((hasAndroid && !fromAndroid) || (hasIOS && fromAndroid));
                                    String platform = alreadyHasOtherPlatform ? "both" : mKey;
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
    }

    public static boolean isCorrectFileType(String fileName) {

        int beginIndex = fileName.lastIndexOf(".");

        boolean isAndroidJava = VALID_FILE_TYPES.indexOf(".java") >= 0;
        boolean isAndroidXML = isAndroidJava && "AndroidManifest.xml".equals(fileName);
        boolean isAndroidCpp = isAndroidJava && "us_the_mac_reel_life_ReelApplication.cpp".equals(fileName);

        boolean isObjectiveC = VALID_FILE_TYPES.indexOf(".m") >= 0;
        boolean isObjectiveCApp = isObjectiveC && "ReelApplication.mm".equals(fileName);

        return beginIndex != -1 && (
                (isAndroidXML || isAndroidCpp || isObjectiveCApp)
                        || VALID_FILE_TYPES.indexOf(fileName.substring(beginIndex)) >= 0);

    }

    public static boolean isCorrectStringType(String prefix, String extraction, String type, String line) {
        return isCorrectCategoryType(type) && isNotInComment(line, extraction) && isNotAnInvalidPrefix(prefix) && isNotIgnored(extraction);
    }

    public static boolean isCorrectCategoryType(String type) {
        return "network".equals(type) || "database".equals(type);// || "app".equals(type);
    }

    public static boolean isNotIgnored(String content) {
        return IGNORED.indexOf(content) < 0 && !content.endsWith(".h");
    }

    public static boolean isNotAnInvalidPrefix(String prefix) {
        return INVALID_PREFIX_TYPES.indexOf(prefix) < 0;
    }

    public static boolean isNotInComment(String line, String extraction) {
		/*[0]//DIRECTORS_ = "Directors"; */
        String comment = "//";

        int commentIndex = line.indexOf(comment); // 0
        boolean hasComment = commentIndex >= 0;

        int leftIndex = line.indexOf(extraction); // 21
        boolean textIsBeforeComment = commentIndex > leftIndex;

        return !hasComment || hasComment && textIsBeforeComment;
    }

    public static String getType(String fileName) {
//		if(CONTENTTYPES.contains(fileName)) return "content";
//		else if(APPTYPES.contains(fileName)) return "app";
        if(DATABASETYPES.contains(fileName)) return "database";
        else if(SCREENTYPES.contains(fileName)) return "screen";
        else if(VIEWTYPES.contains(fileName)) return "view";
        else if(NETWORKTYPES.contains(fileName)) return "network";
        return "parse";
    }




    /**
     * Load replace strings data.
     *
     * @param dataFile
     * @throws IOException
     */
    public static void loadReplaceData(String dataFile) throws IOException {
        System.out.println("Loading replace data from file " + dataFile);

        replaceStrings = new HashMap<String, Integer>();

        FileInputStream fstream = new FileInputStream(dataFile);
        DataInputStream in = new DataInputStream(fstream);
        BufferedReader br = new BufferedReader(new InputStreamReader(in));

        //		replaceStrings.put("import us.the.mac.reel.life.development.R;", -1);
        //		replaceStrings.put("import us.the.mac.reel.life.development.BuildConfig;", -1);

        String line;
        while ((line = br.readLine()) != null) {
            String[] lineSplit = line.split(VERSION_DELIMIT);

            String reference = lineSplit[0];
            String type = lineSplit[1];
            String replacing = lineSplit[2];
            System.out.println("\nreference: " + reference + " type:" + type + " with:" + replacing);

            int position = replaceStrings.size();
            System.out.println("Replacing: " + replacing + " With: "+position+"\n");
            replaceStrings.put("\""+replacing+"\"", position);
        }

        br.close();
        in.close();
        fstream.close();

    }

    /**
     * Process directory
     * @param directory
     * @throws IOException
     */
    public static void processDirectory(File directory) throws IOException {
        System.out.println("Processing directory " + directory);
        System.out.println("Processing directory " + directory.getName());

        File[] contents = directory.listFiles();
        if(contents == null) return;

        System.out.println("Processing files " + Arrays.toString(contents));
        for (File node: contents) {
            if (node.isFile())
                processFile(node);
            else if (node.isDirectory())
                processDirectory(node);
        }

    }

    /**
     * Process file
     * @param file
     * @throws IOException
     */
    public static void processFile(File file) throws IOException {

        String fileName = file.getName();

        // If a file has an appropriate extension then process it
        if (isCorrectFileType(fileName)) {
            System.out.println("Processing file " + fileName);

            // Reading objects
            FileInputStream fis = new FileInputStream(file);
            DataInputStream in = new DataInputStream(fis);
            BufferedReader br = new BufferedReader(new InputStreamReader(in));

            // Writing objects
            File tmpFile = new File(file.getAbsolutePath() + "_tmp");
            FileWriter fw = new FileWriter(tmpFile);
            BufferedWriter out = new BufferedWriter(fw);

            String line;
            while ((line = br.readLine()) != null) {

                for (String replaceKey: replaceStrings.keySet()) {
                    if("AndroidManifest.xml".equals(fileName)) {
//						String fileNameReport = String.format("\n\n\n\n\nFile Name: %s \n\n\n\n\n", fileName);
//						System.out.println(fileNameReport);
                        break;
                    }

                    int beginIndex = line.indexOf(replaceKey);
                    int endIndex = beginIndex + replaceKey.length();

                    if (beginIndex != -1 && "\"".equals(line.substring(endIndex - 1, endIndex))) {

                        int beginFileIndex = fileName.lastIndexOf(".");
                        int beginReplacementIndex = line.indexOf(replaceKey);
                        String prefix = beginReplacementIndex > 0 ? line.substring(0,beginReplacementIndex - 1).replaceAll("^\\s+", "") : "";

                        String typeCheck = fileName.substring(0, beginFileIndex + 1);
                        String type = getType(typeCheck);
                        if(isCorrectStringType(prefix, replaceKey, type, line)) {

                            int position = replaceStrings.get(replaceKey);
                            if(fileName.contains("cpp")) {

                                String format = " u(" + position + ") ";
                                line = line.replace(replaceKey, format);

                            } else if(fileName.contains("java")) {
                                String format = null;

                                if(fileName.contains("Service") || fileName.contains("Provider")) {
                                    format = "ServiceApplication.s(" + position + ")";
                                } else {
                                    format = "i.s(" + position + ")";
                                }
                                line = line.replace(replaceKey, format);

                            } else {

                                String format = "[ReelApplication c:" + position + "]";
                                line = line.replace("@"+replaceKey, format);

                            }
                        }
                    }
                }

                if (line.contains(".development")) {
                    line = line.replace(".development", hasReleaseArg ? "" : ".beta");
                } else if(line.contains("ic_launcher_reel_life_dev_icon")) {
                    line = line.replace("_dev", hasReleaseArg ? "" : "_beta");
                }

                if(isAPrerelease()) {
                    if(!line.contains("android:debuggable=\"true\""))
                        out.write(line+"\n");
                } else {

                    if(!line.contains("android:debuggable=\"true\"")
                            && !line.contains("Log."))
                        out.write(line+"\n");
                }
            }

            br.close();
            in.close();
            fis.close();

            out.close();
            fw.close();

            // Tmp file is ready now
            // Delete original file
//            if (!file.delete())
//                throw new IOException("Can't delete " + file.getName());
//            if (!file.createNewFile())
//                throw new IOException("Can't create " + file.getName());
//
//            // Copy from _tmp to original file - we have to create it now
//            copyFile(tmpFile,file);
//
//            // Delete tmp file
//            if (!tmpFile.delete())
//                throw new IOException("Can't delete " + tmpFile.getName());

        }
    }



    /**
     * Copy from file to file.
     * @param src	- source file
     * @param dest	- destination file
     * @throws IOException
     */
    public static void copyFile(File src, File dest) throws IOException {
        InputStream in = new FileInputStream(src);
        OutputStream out = new FileOutputStream(dest);

        byte[] buf = new byte[1024];
        int len;
        while ((len = in.read(buf)) > 0){
            out.write(buf, 0, len);
        }

        in.close();
        out.close();
    }



    /**
     * Run application
     * @param args
     */
    public static void main(String[] args) {
        System.out.println("========================The Strip Script Loading!========================");
        System.out.println("Arguments: " + Arrays.toString(args));

        String placeHolder = "";
        String dataFilePath = LOCAL_PATH;
        List<String> contents = Arrays.asList(args);

        boolean hasArgs = args.length > 0;
        boolean hasNoArgs = !hasArgs;

        hasReleaseArg = contents.contains("release");
        hasAndroidArg = hasNoArgs || contents.contains("android");
        hasIOSArg = hasNoArgs || contents.contains("ios");

        VALID_FILE_TYPES = "";
        if(hasIOSArg) VALID_FILE_TYPES += ".m";
        if(hasAndroidArg) VALID_FILE_TYPES += ",.java";
        System.out.println("Build Type: " + (hasReleaseArg ? "release" : "beta" ) + " Version: " + VERSION + " File Types: " + VALID_FILE_TYPES);

        try {
            // Extract replace data
            placeHolder = String.format("extractReplaceData(dataFilePath: %s)", dataFilePath);
            extractReplaceData(dataFilePath);

            placeHolder = String.format(hasReleaseArg ? "!isOnReleaseBranch()" : "!isOnAlphaBranch()");
            if(hasReleaseArg && !isOnReleaseBranch()) return;
            else if(!isOnAlphaBranch()) return;

            System.out.println("========================The Strip Script Process is started!========================");

            // Load replace data
            placeHolder = String.format("loadReplaceData(dataFilePath: %s)", dataFilePath);
            loadReplaceData(dataFilePath);

            // Run directory processing
            for(String replaceDirectoryPath : EXTRACTION_REPLACEMENT_DIRECTORIES) {
                String directory = String.format(replaceDirectoryPath, hasReleaseArg ? "release" : "alpha");
                File startingDirectory = new File(directory);

                placeHolder = String.format("processDirectory(startingDirectory: %s)", startingDirectory);
                processDirectory(startingDirectory);
            }

            System.out.println("========================The Strip Script Process is complete!========================");
        } catch (Exception e) {

            System.err.println("\n\n\n========================Something went wrong with the Strip Script!========================\n");
            System.out.println("Place Holder: "+placeHolder);
            e.printStackTrace();

        }

        System.out.println("========================The Strip Script Generated the following Integers!========================");
        String searchValues[] = {
                "Uri: ",
                "SubGenre",
                "ALERT_STATE_TITLE",
                "us.the.mac.reel.life.ReelUITestCase",
                "User Confirmed!",
                "REEL_CACHE",
                "REEL_QUEUE",
                ".r",
                " NOT IN (",
                "NotSure",
                "WatchLater",
                "WatchedGood",
                "WatchedBad",
                "testQueueProcessing",
//				"\\\\A",
                "subGenre",
                "(WatchedGood IS NULL) AND (WatchLater IS NULL) AND (NotSure IS NULL) AND (WatchedBad IS NULL) AND (_id NOT IN ("
        };

        String mIntegers = "\t" + getInteger(searchValues[0]);
        for(int position = 1; position < searchValues.length; position++)
            mIntegers += ",\n\t" + getInteger(searchValues[position]);

        // /Users/christopher/git/reel-life-content/reel_content_tests/include/.iTest
//        try {
//            PrintWriter out = new PrintWriter("/Users/christopher/git/reel-life-content/reel_content_tests/include/.iTest");
//            out.print(mIntegers);
//            out.close();
//        }
//        catch(Exception e) { e.printStackTrace(); }

        System.out.println(mIntegers);
        try { replaceDevWithBeta(); }
        catch(Exception e) { e.printStackTrace(); }
    }

    private static void replaceDevWithBeta() throws IOException {
        File androidManifest = new File("/Users/christopher/git/reel-life-content/android-alpha/ReelContent.16.08.XX.X/AndroidManifest.xml");
        processFile(androidManifest);
    }
    // READ IN FileManger.h
    // STRIP EXPOSED STRINGS
    // PASTE INTO "gN.h"


    private static Integer getInteger(String key) {
        return replaceStrings.get("\""+key+"\"");
    }

    private static boolean isOnAlphaBranch() throws FileNotFoundException {
        File aBranch = new File(ANDROID_HEAD_FILE);
        File iBranch = new File(IOS_HEAD_FILE);

        String aContents = new Scanner(aBranch).useDelimiter("\\Z").next();
        String iContents = new Scanner(iBranch).useDelimiter("\\Z").next();

        if(aContents == null || iContents == null) return false;
        return aBranch.exists() && aContents.contains("alpha")
                && iBranch.exists() && iContents.contains("alpha");
    }

    private static boolean isOnReleaseBranch() throws FileNotFoundException {
        File aBranch = new File(ANDROID_RELEASE_HEAD_FILE);
        File iBranch = new File(IOS_RELEASE_HEAD_FILE);

        String aContents = new Scanner(aBranch).useDelimiter("\\Z").next();
        String iContents = new Scanner(iBranch).useDelimiter("\\Z").next();

        if(aContents == null || iContents == null) return false;
        return aBranch.exists() && aContents.contains("release")
                && iBranch.exists() && iContents.contains("release");
    }

    private static boolean isAPrerelease() throws FileNotFoundException {
        File aPrerelease = new File(ANDROID_PRERELEASE_FILE);
        File iPrerelease = new File(IOS_PRERELEASE_FILE);

        String aContents = new Scanner(aPrerelease).useDelimiter("\\Z").next();
        String iContents = "";//new Scanner(iPrerelease).useDelimiter("\\Z").next();

        if(aContents == null || iContents == null) return false;
        return aPrerelease.exists() && aContents.contains("prerelease exists");
//				&& iBranch.exists() && iContents.contains("release");
    }


    public static List<String> getReplaceStrings() {
        return Arrays.asList(replaceStrings.keySet().toArray(new String[]{}));
    }

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

    public CryptoHelper_1(Key key ) {
        this.key = key;
    }

    public CryptoHelper_1() throws Exception {
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