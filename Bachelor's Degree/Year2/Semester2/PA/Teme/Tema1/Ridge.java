import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Ridge {

	static class InfoDemolition {
		private int[] peaksHeights;
		private int[] demolitionPrice;

		public InfoDemolition(int[] peaksHeights, int[] demolitionPrice) {
			this.peaksHeights = peaksHeights;
			this.demolitionPrice = demolitionPrice;
		}

		public int[] getPeaksHeights() {
			return peaksHeights;
		}

		public int[] getDemolitionPrice() {
			return demolitionPrice;
		}
	}

	static int min(int a, int b) {
		if (a < b) {
			return a;
		}
		return b;
	}


	static long minimumLineIgnore(long[][] D, int[] peakHeights, int currPeak, int toIgnore) {
		long min = Long.MAX_VALUE;

		for (long i = 0; i <= peakHeights[currPeak]; i++) {
			if (i != toIgnore) {
				min = Utils.minLongValue(D[currPeak][(int) i], min);
			}
		}

		return min;
	}

	static void fillPosibleHeight(long[][] D, int crtIdx, int cost, int height) {
		if (height >= 2) {
			D[crtIdx][0] = 0;
			D[crtIdx][1] = cost;
			D[crtIdx][2] = cost * 2;
		} else if (height == 1) {
			D[crtIdx][0] = 0;
			D[crtIdx][1] = cost;
			// impossible to decrement twice a peak that already has height 1
			D[crtIdx][2] = Long.MAX_VALUE; 
		} else if (height == 0) {
			D[crtIdx][0] = 0;

			// impossible to decrement once a peak that already has height 0
			D[crtIdx][1] = Long.MAX_VALUE;
			// impossible to decrement twice a peak that already has height 0
			D[crtIdx][2] = Long.MAX_VALUE; 
		}
	}

	static void fillNonConflict(long[][] D, int crtIdx, int crtPeakHeight, int prevPeakHeight) {
		for (int noDecrements = 0; noDecrements < 3; noDecrements++) {
			if (D[crtIdx][noDecrements] == Long.MAX_VALUE) {
				; // do nothing
			} else if ((crtPeakHeight - noDecrements) == prevPeakHeight) {
				D[crtIdx][noDecrements] += Utils.minLongValue(D[crtIdx - 1][1], D[crtIdx - 1][2]);
			} else if ((crtPeakHeight - noDecrements) == prevPeakHeight - 1) {
				D[crtIdx][noDecrements] += Utils.minLongValue(D[crtIdx - 1][0], D[crtIdx - 1][2]);
			} else if ((crtPeakHeight - noDecrements) == prevPeakHeight - 2) {
				D[crtIdx][noDecrements] += Utils.minLongValue(D[crtIdx - 1][0], D[crtIdx - 1][1]);
			} else {
				// no conflict found
				D[crtIdx][noDecrements] += Utils.minLongValue(D[crtIdx - 1][0],
								Utils.minLongValue(D[crtIdx - 1][1], D[crtIdx - 1][2]));
			}
		}
	}

	static long findMinimum(int[] peaksHeights, int[] costs) {
		int N = costs.length;

		long[][] D = new long[N][3];

		fillPosibleHeight(D, 0, costs[0], peaksHeights[0]);

		for (int crtIdx = 1; crtIdx < N; crtIdx++) {
			fillPosibleHeight(D, crtIdx, costs[crtIdx], peaksHeights[crtIdx]);
			fillNonConflict(D, crtIdx, peaksHeights[crtIdx], peaksHeights[crtIdx - 1]);
		}

		return Utils.minLongValue(D[N - 1][0], Utils.minLongValue(D[N - 1][1], D[N - 1][2]));
	}

	public static InfoDemolition readHeightsData() throws IOException {
		InputReader inputReader = new InputReader("ridge.in");
		String problemData = inputReader.readInput();
		Scanner scanner = new Scanner(problemData);

		int N = 0;
		if (scanner.hasNext()) {
			String initializerLine = scanner.nextLine();
			N = Integer.valueOf(initializerLine);
		}

		int[] peaksHeights = new int[N];
		int[] demolitionPrice = new int[N];

		int i = 0;
		// read each line holding power and updateInfo
		while (scanner.hasNextLine()) {
			String infoHeights = scanner.nextLine();

			String[] info = infoHeights.split(" ");
			peaksHeights[i] = Integer.parseInt(info[0]);
			demolitionPrice[i] = Integer.parseInt(info[1]);

			i++;
		}

		InfoDemolition infoDemolition = new InfoDemolition(peaksHeights, demolitionPrice);
		return infoDemolition;
	}

	public static void main(String[] args) {
		try {
			InfoDemolition infoDemolition = readHeightsData();
			long minimumDemolition = findMinimum(infoDemolition.getPeaksHeights(),
											infoDemolition.getDemolitionPrice());

			try {
				FileWriter myWriter = new FileWriter("ridge.out");
				myWriter.write(String.valueOf(minimumDemolition));
				myWriter.close();
			} catch (IOException e) {
				System.out.println("An error occurred.");
				e.printStackTrace();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
