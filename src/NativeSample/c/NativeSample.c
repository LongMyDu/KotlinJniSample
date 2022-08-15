#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

JNIEXPORT void JNICALL
Java_NativeSample_intArgument(JNIEnv *env, jobject thisObject, jint n)
{
    printf("Hello From C World!");
    printf("\nInitialize %d elements array", n);

    int* a;
    a = (int*) malloc(n*sizeof(int));
    for (int i = 0; i< n; i++) {
        a[i] = i;
        printf("\na[%d]: %d", i, a[i]);
    }
    return;
}

JNIEXPORT void JNICALL
Java_NativeSample_helloName(JNIEnv *env, jobject thisObject, jstring name)
{
    jboolean isCopy = JNI_FALSE;

    const char* utfName = (*env)->GetStringUTFChars(env, name, &isCopy);
    printf("Hello %s From C World!", utfName);
    (*env)->ReleaseStringUTFChars(env, name, utfName);

    return;
}

JNIEXPORT jstring JNICALL
Java_NativeSample_getFullName(JNIEnv *env, jobject thisObject, jstring familyName, jstring firstName)
{
    jboolean isCopy = JNI_FALSE;

    //Get argument
    const char* utfFamilyName = (*env)->GetStringUTFChars(env, familyName, &isCopy);
    const char* utfFirstName = (*env)->GetStringUTFChars(env, firstName, &isCopy);

    //Make copy of familyName and firstName
    char *familyNameCopy = strdup(utfFamilyName), *firstNameCopy= strdup(utfFirstName);

    //Build fullName
    char* fullName;
    asprintf(&fullName, "%s %s", familyNameCopy, firstNameCopy);

    jstring result = (*env)->NewStringUTF(env, fullName);

    //Free memory
    (*env)->ReleaseStringUTFChars(env, familyName, utfFamilyName);
    (*env)->ReleaseStringUTFChars(env, firstName, utfFirstName);
    free(familyNameCopy);
    free(firstNameCopy);
    free(fullName);
    return result;
}

JNIEXPORT jlongArray JNICALL
Java_NativeSample_multiplyArrayElements(JNIEnv *env, jobject thisObject, jintArray arr, jlong factor)
{
    jboolean isCopy = JNI_TRUE;

    //Get argument
    jint* jArr = (*env)->GetIntArrayElements(env, arr, &isCopy);
    if (NULL == jArr) {
        return NULL;
    }
    jsize n = (*env)->GetArrayLength(env, arr);

    //Allocate result array
    jlong* cResultArray = malloc(n*sizeof(long long));

    //Calculate result array
    for (int i = 0; i < n; i++) {
        cResultArray[i] = jArr[i] * factor;
    }

    //Copy to jlongArray
    jlongArray jResultArray = (*env)->NewLongArray(env, n);
    if (NULL == jResultArray) {
        return NULL;
    }
    (*env)->SetLongArrayRegion(env, jResultArray, 0, n, cResultArray);
    
    //Free memory
    (*env)->ReleaseIntArrayElements(env, arr, jArr, 0);
    free(cResultArray);

    return jResultArray;
}


JNIEXPORT void JNICALL
Java_NativeSample_printMessage(JNIEnv *env, jobject thisObject, jintArray arr)
{
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);

    //Get static field message
    jfieldID messageId = (*env)->GetStaticFieldID(env, thisClass, "message", "Ljava/lang/String;");
    jstring jMessage = (jstring)(*env)->GetStaticObjectField(env, thisObject, messageId);

    //Convert to C type
    const char* cMessage = (*env)->GetStringUTFChars(env, jMessage, NULL);
    printf("Message from kotlin: %s", cMessage);

    //Set new message
    jstring newMessage = (*env)->NewStringUTF(env, "Hiiii");
    (*env)->SetStaticObjectField(env, thisClass, messageId, (jobject)newMessage);

    //Get method callback
    jmethodID callbackId = (*env)->GetMethodID(env, thisClass, "callback", "(Ljava/lang/String;)V");
    if (NULL == callbackId) return;

    // Callback the method
    (*env)->CallVoidMethod(env, thisObject, callbackId, newMessage);

    //Free memory
    (*env)->ReleaseStringUTFChars(env, jMessage, cMessage);
    return;
}

JNIEXPORT void JNICALL
Java_NativeSample_printPair(JNIEnv *env, jobject thisObject, jobject pair)
{
    // Get a class reference for Pair, first and second field id
    jclass classPair = (*env)->FindClass(env, "Pair");
    jfieldID firstId = (*env)->GetFieldID(env, classPair, "first", "I");
    jfieldID secondId = (*env)->GetFieldID(env, classPair, "second", "I");

    //Get value of first and second
    jint first = (*env)->GetIntField(env, pair, firstId);
    jint second = (*env)->GetIntField(env, pair, secondId);

    printf("\n%d %d", first, second);
    return;
}

JNIEXPORT void JNICALL
Java_NativeSample_printPairArray(JNIEnv *env, jobject thisObject, jobjectArray pairArray)
{
    // Get a class reference for Pair, first and second field id
    jclass classPair = (*env)->FindClass(env, "Pair");
    jfieldID firstId = (*env)->GetFieldID(env, classPair, "first", "I");
    jfieldID secondId = (*env)->GetFieldID(env, classPair, "second", "I");

    //Get length of array
    jsize n = (*env)->GetArrayLength(env, pairArray);

    for (int i = 0;i < n;i ++) {
        //Get each Pair object
        jobject curPair = (*env)->GetObjectArrayElement(env, pairArray, i);
        if (NULL == curPair) continue;

        //Get first and second of curPair
        jint first = (*env)->GetIntField(env, curPair, firstId);
        jint second = (*env)->GetIntField(env, curPair, secondId);

        printf("\n%d %d", first, second);
    }
    return;
}


JNIEXPORT jobject JNICALL
Java_NativeSample_getDateTime(JNIEnv *env, jobject thisObject)
{
    // Get a class reference for DateTime and all field ids
    jclass classDateTime = (*env)->FindClass(env, "DateTime");

    //Get constructor method id
    jmethodID constructorId = (*env)->GetMethodID(env, classDateTime, "<init>", "(IIIIIILjava/lang/String;)V");
    if (NULL == constructorId) {
        printf("\nNull constructor");
        return NULL;
    }
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char *cFullDatetime;
    asprintf(&cFullDatetime, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    jstring jFullDateTime = (*env)->NewStringUTF(env, cFullDatetime);
    jobject result = (*env)->NewObject(env, classDateTime, constructorId, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, jFullDateTime);

    //Free memory
    free(cFullDatetime);

    return result;
}


JNIEXPORT jobjectArray JNICALL
Java_NativeSample_getDateTimeArray(JNIEnv *env, jobject thisObject)
{
    // Get a class reference for class DateTime
    jclass classDateTime = (*env)->FindClass(env, "DateTime");

    //Get constructor method id
    jmethodID constructorId = (*env)->GetMethodID(env, classDateTime, "<init>", "(IIIIIILjava/lang/String;)V");
    if (NULL == constructorId) {
        printf("\nNull constructor");
        return NULL;
    }

    //Create an array of DateTime (just for sample, it's useless)
    char *cFullDateTime;
    jobjectArray result = (*env)->NewObjectArray(env, 5, classDateTime, NULL);
    jstring jFullDateTime;
    jobject kDateTime;
    time_t t;
    struct tm tm;
    for (int i = 0;i < 5; i++) {
        t = time(NULL);
        tm = *localtime(&t);

        asprintf(&cFullDateTime, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

        jFullDateTime = (*env)->NewStringUTF(env, cFullDateTime);
        kDateTime = (*env)->NewObject(env, classDateTime, constructorId, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, jFullDateTime);
        (*env)->SetObjectArrayElement(env, result, i, kDateTime);

        free(cFullDateTime);
        (*env)->DeleteLocalRef(env, kDateTime);
        (*env)->DeleteLocalRef(env, jFullDateTime);
    }

    return result;
}


JNIEXPORT void JNICALL
Java_NativeSample_doSthThenCallback(JNIEnv *env, jobject thisObject, jobject callback)
{
    // Get a class reference for callback
    jclass classCallback = (*env)->GetObjectClass(env, callback);
    if (NULL == classCallback) {
         printf("\nNull class");
        return;
    }


    //Get invoke method id
    jmethodID methodId = (*env)->GetMethodID(env, classCallback, "invoke", "()V");
    if (NULL == methodId) {
        printf("\nNull method");
        return;
    }

    (*env)->CallVoidMethod(env, thisObject, methodId, callback);
    return;
}

