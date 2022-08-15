fun main() {
    //Test passing primitive parameter
    // val n = 100
    // HelloWorld().sayHello(n)

    //Test get string result
    // println(NativeSample.getFullName("Long", "My Du"))

    //Test passing primitive array
    // val a = intArrayOf(1, 4, 8, 2, 10, 5)
    // val b = NativeSample.multiplyArrayElements(a, 500000000000)
    // for (x in b) {
    //     print("$x ")
    // }

    //Test access static field and static method
    // NativeSample.printMessage()

    //Test pass object 
    // NativeSample.printPair(Pair(7, 10))

    //Test pass array of objects
    // NativeSample.printPairArray(arrayOf(Pair(7, 10), Pair(2, 4), Pair(1, 9), Pair(4, 9)))

    //Test return an object
    // print(NativeSample.getDateTime())

    //Test return an object array
    // val dateTimeArray = NativeSample.getDateTimeArray()
    // for (dateTime in dateTimeArray) {
    //     println(dateTime)
    // }

    //Test callback
    
    NativeSample.doSthThenCallback {
        println("Hello from callback")
    }

}