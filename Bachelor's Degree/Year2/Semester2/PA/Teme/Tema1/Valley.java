import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


public class Valley {

	public static List<Long> readHeightsData() throws IOException {
		List<Long> peaksHeights = new ArrayList<>();

		InputReader inputReader = new InputReader("valley.in");
		String problemData = inputReader.readInput();
		Scanner scanner = new Scanner(problemData);

		int N = 0;
		if (scanner.hasNext()) {
			String initializerLine = scanner.nextLine();
			N = Integer.valueOf(initializerLine);
		}

		// read each line holding power and updateInfo
		while (scanner.hasNextLine()) {
			String infoHeights = scanner.nextLine();

			String[] heights = infoHeights.split(" ");
			for (int i = 0; i < N; i++) {
				peaksHeights.add(Long.valueOf(heights[i]));
			}
		}

		return peaksHeights;
	}

	static int findMinPeak(List<Long> peakHeights) {
		int N = peakHeights.size();

		Long min = Long.MAX_VALUE;
		int minIdx = Integer.MIN_VALUE;
		for (int i = 1; i < (N - 1); i++) {
			if (peakHeights.get(i) < min) {
				min = peakHeights.get(i);
				minIdx = i;
			}
		}

		if (N >= 2) {
			return minIdx;
		}

		// else we have a maximum 2 element array
		if (N == 1) {
			return 0;
		}
		if (N == 2) {
			return ((peakHeights.get(1) > peakHeights.get(0)) ? 0 : 1);
		}
		// just for case of N = 0
		return 0;
	}

	static long timeBuildValley(List<Long> peakHeights) {
		int minIndex = findMinPeak(peakHeights);

		int N = peakHeights.size();

		long hoursNeeded = 0;
		// corner case, when min elements might be at the ends of the array
		if ((peakHeights.get(minIndex) > peakHeights.get(0))
			&& (peakHeights.get(minIndex) > peakHeights.get(N - 1))) {
			hoursNeeded = Utils.maxLongValue(peakHeights.get(minIndex) - peakHeights.get(0),
					peakHeights.get(minIndex) - peakHeights.get(N - 1));

			peakHeights.set(minIndex, peakHeights.get(minIndex) -  hoursNeeded);
		} else if (peakHeights.get(minIndex) > peakHeights.get(0)) {
			hoursNeeded = peakHeights.get(minIndex) - peakHeights.get(0);

			peakHeights.set(minIndex, peakHeights.get(minIndex) -  hoursNeeded);
		} else if (peakHeights.get(minIndex) > peakHeights.get(N - 1)) {
			hoursNeeded = peakHeights.get(minIndex) - peakHeights.get(N - 1);

			peakHeights.set(minIndex, peakHeights.get(minIndex) -  hoursNeeded);
		}

		for (int i = 1; i < minIndex; i++) {
			Long diff = peakHeights.get(i) - peakHeights.get(i - 1);
			if (diff > 0) {
				hoursNeeded += diff;
				peakHeights.set(i, peakHeights.get(i) - diff);
			}
		}

		for (int i = N - 2; i > minIndex; i--) {
			Long diff = peakHeights.get(i) - peakHeights.get(i + 1);
			if (diff > 0) {
				hoursNeeded += diff;
				peakHeights.set(i, peakHeights.get(i) - diff);
			}
		}

		return hoursNeeded;
	}

	public static void main(String[] args) {
		try {
			List<Long> peaksHeights = readHeightsData();
			long timeNeeded = timeBuildValley(peaksHeights);

			try {
				FileWriter myWriter = new FileWriter("valley.out");
				myWriter.write(String.valueOf(timeNeeded));
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
