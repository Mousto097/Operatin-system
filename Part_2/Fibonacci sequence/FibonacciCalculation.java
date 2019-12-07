/**
 * The FibonacciCalculation program create the child thread that will generate
 * the Fibonacci numbers, placing the sequence in an array that is shared by the
 * threads.
 *
 * @author Mamadou Bah
 * 
 */

public class FibonacciCalculation extends Thread implements Runnable {

	// the array for storing Fibonacci sequence
	private long[] sequenceData;

	/**
	 * class constructor for class FibonacciCalculation
	 * 
	 *
	 * @param sequenceData -- the array for storing Fibonacci sequence
	 * 
	 */
	public FibonacciCalculation(long[] sequenceData) {
		this.sequenceData = sequenceData;

	}

	/**
	 * Generate the Fibonacci numbers and place the sequence in in the array
	 * 
	 * @param no parameter
	 * 
	 * @return void
	 * 
	 */
	@Override
	public void run() {

		// Set initial values in the array
		sequenceData[0] = 0;
		sequenceData[1] = 1;

		for (int i = 2; i < sequenceData.length; i++) {
			sequenceData[i] = sequenceData[i - 2] + sequenceData[i - 1];
		}
	}
}
