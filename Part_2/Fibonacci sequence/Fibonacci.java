
/**
* The Fibonacci program generates the Fibonacci series using 
* the Java thread library. 
*
* @author  Mamadou Bah
* 
*/

import java.util.Scanner;

public class Fibonacci {

	/**
	 * Get the number of Fibonacci numbers to generate, then create the parent
	 * thread.
	 * 
	 * @param no parameter
	 * 
	 * @return void
	 * 
	 */

	public static void main(String[] args) {

		// Get the number entered by the user
		System.out.println("Please enter the number of Fibonacci numbers to generate: ");
		Scanner myScanner = new Scanner(System.in);

		// Check if the input is an integer then starts the program
		try {

			int num = myScanner.nextInt();
			myScanner.close();

			long[] arrayData = new long[num];

			DisplayFibonacciSequence parentThread = new DisplayFibonacciSequence(arrayData);

			parentThread.start();
			Thread.yield();

		} catch (NumberFormatException e) {
			System.out.println("input is invalid. Please try again!");
		}

	}
}
