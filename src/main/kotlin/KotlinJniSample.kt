data class Pair(val first: Int, val second: Int)

data class DateTime(val day: Int, val month: Int, val year: Int, val hour: Int, val minute: Int, val second: Int, val fullDateTime: String)



object NativeSample {
    external fun intArgument(n: Int)
    external fun helloName(name: String)
    external fun getFullName(familyName: String, firstName: String): String
    external fun multiplyArrayElements(arr: IntArray, factor: Long): LongArray

    
    external fun printMessage()
    val message = "Hello"
    fun callback(_message: String) {
        println("Message from C: $_message")
    }

    external fun printPair(pair: Pair)
    external fun printPairArray(pairArray: Array<Pair>)
    external fun getDateTime(): DateTime
    external fun getDateTimeArray(): Array<DateTime>

    external fun doSthThenCallback(callback: ()->Unit)

    init {
        System.loadLibrary("NativeSample")
    }
}