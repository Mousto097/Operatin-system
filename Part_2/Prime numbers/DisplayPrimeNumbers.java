
/**
* The DisplayPrimeNumbers program will outputs 
* all the prime numbers less than or equal to the number entered by the user.
*
* @author  Mamadou Bah
* 
*/

import java.util.Scanner;

public class DisplayPrimeNumbers extends Thread {

	public static void main(String[] args) {

		// Get the number entered by the user
		System.out.println("Please enter the number of prime numbers: ");
		Scanner myScanner = new Scanner(System.in);

		// Check if the input is an integer then starts the program
		try {

			int num = myScanner.nextInt();
			myScanner.close();

			PrimeNumber prime1 = new PrimeNumber(num);

			System.out.println(">>>>>>>>>> List of all prime numbers less than or equal to {" + num + "} <<<<<<<<<<");
			prime1.start();
			Thread.yield();

		} catch (NumberFormatException e) {
			System.out.println("input is invalid. Please try again!");
		}

	}
}