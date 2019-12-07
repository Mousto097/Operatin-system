
/**
 * The PrimeNumber program will then create a separate thread that outputs all
 * the prime numbers less than or equal to the number entered by the user.
 *
 * @author Mamadou Bah
 * 
 */

public class PrimeNumber extends Thread {

    private int givenNumber;

    /**
     * class constructor for class PrimeNumber
     * 
     *
     * @param givenNumber -- the number entered by user on the command line
     * 
     */
    PrimeNumber(int givenNumber) {
        this.givenNumber = givenNumber;
    }

    /**
     * outputs all the prime numbers less than or equal to the number entered by the
     * user
     * 
     *
     * @param no parameter
     * 
     * @return void
     * 
     */
    public void run() {

        for (int i = 0; i <= givenNumber; i++) {
            if (CheckPrime(i) && i > 1) {
                System.out.println(i);
            }
        }
    }

    /**
     * Check if the number is prime.
     * 
     *
     * @param numberToCheck the number to check
     * 
     * @return returned is true if the number is prime and false otherwise
     */

    public static boolean CheckPrime(int numberToCheck) {
        for (int i = 2; i < numberToCheck; i++) {
            if (numberToCheck % i == 0)
                return false;
        }
        return true;
    }

}
