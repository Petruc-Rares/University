import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class Lego {
	static class InfoPiese {
		private int K;
		private int N;
		private int T;

		public InfoPiese(int k, int n, int t) {
			K = k;
			N = n;
			T = t;
		}

		public int getK() {
			return K;
		}

		public int getN() {
			return N;
		}

		public int getT() {
			return T;
		}
	}

	public static InfoPiese lastCreate() {
		MyScanner scanner = new MyScanner("lego.in");

		Integer K = scanner.nextInt();
		Integer N = scanner.nextInt();
		Integer T = scanner.nextInt();

		InfoPiese infoPiese = new InfoPiese(K, N, T);

		return infoPiese;
	}

	private static ArrayList<Integer []> combinatii = new ArrayList<>();

	private static void generatePossibleCombinations(Integer N, Integer K, int crtVal,
			int crtPos, Integer[] array) {
		if (crtPos == N) {
			combinatii.add(array.clone());
			return;
		}

		for (int i = crtVal; i <= K; i++) {
			array[crtPos] = i;
			generatePossibleCombinations(N, K, i + 1, crtPos + 1, array);
		}
	}

	private static Integer[] finalResult;

	// check for current combination given if it gives a number
	// of consecutive numbers bigger than the best known until now
	private static int checkBestHelper(Integer[] combinatie) {
		int T = infoPiese.getT();
		int maxSum = combinatie[combinatie.length - 1] * T;

		boolean[][] D = new boolean[T + 1][maxSum + 1];
		D[0][0] = true;

		for (int i = 1; i <= T; i++) {
			for (int j = 0; j <= maxSum; j++) {
				if (D[i - 1][j]) {
					D[i][j] = true;
				} else {
					for (int K = 0; K < combinatie.length; K++) {
						if ((j - combinatie[K]) >= 0) {
							if (D[i - 1][j - combinatie[K]]) {
								D[i][j] = true;
								break;
							}
						}
					}
					// otherwise stay false
				}
			}
		}

		Integer maxConsecutive = Integer.MIN_VALUE;
		Integer crtCount = 0;
		for (int j = 1; j < maxSum; j++) {
			if ((D[T][j]) && (D[T][j + 1])) {
				crtCount++;
			} else if ((D[T][j]) && (!D[T][j + 1])) {
				crtCount++;
				if (crtCount > maxConsecutive) {
					maxConsecutive = crtCount;
				}
				crtCount = 0;
			}
		}
		if (D[T][maxSum]) {
			crtCount++;
		}

		// case not checked in the for loop above, as loop ends at idx maxSum-1
		// when the last element is also true
		if (crtCount > maxConsecutive) {
			maxConsecutive = crtCount;
		}

		return maxConsecutive;
	}

	public static void checkBest() {
		int bestFollowing = Integer.MIN_VALUE;
		int noCombinations = combinatii.size();
		for (int i = 0; i < noCombinations; i++) {
			int crtResult = checkBestHelper(combinatii.get(i));
			if (crtResult > bestFollowing) {
				bestFollowing = crtResult;
				finalResult = combinatii.get(i);
			}
		}

		try {
			FileWriter myWriter = new FileWriter("lego.out");
			myWriter.write(bestFollowing + "\n");

			for (int i = 0; i < finalResult.length; i++) {
				myWriter.write(finalResult[i] + " ");
			}
			myWriter.close();
		} catch (IOException e) {
			System.out.println("An error occurred.");
			e.printStackTrace();
		}
	}

	static InfoPiese infoPiese;

	public static void main(String[] args) {
		infoPiese = lastCreate();

		Integer[] crtArray = new Integer[infoPiese.getN()];
		crtArray[0] = 1;
		generatePossibleCombinations(infoPiese.getN(), infoPiese.getK(), 2, 1, crtArray);
		checkBest();
	}
}