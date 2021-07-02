import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Stocks {

	public static StockInfo readStocksData() throws IOException {
		StockInfo stockInfo = new StockInfo();

		InputReader inputReader = new InputReader("stocks.in");
		String problemData = inputReader.readInput();
		Scanner scanner = new Scanner(problemData);

		if (scanner.hasNext()) {
			String initializerLine = scanner.nextLine();

			String[] numbers = initializerLine.split(" ");

			stockInfo.setN(Integer.valueOf(numbers[0]));
			stockInfo.setB(Integer.valueOf(numbers[1]));
			stockInfo.setL(Integer.valueOf(numbers[2]));
		}

		List<Integer> currentValues = new ArrayList<>();
		List<Integer> maxLosses = new ArrayList<>();
		List<Integer> maxProfits = new ArrayList<>();

		// read each line holding power and updateInfo
		while (scanner.hasNextLine()) {
			String initializerLine = scanner.nextLine();
			String[] numbers = initializerLine.split(" ");

			currentValues.add(Integer.valueOf(numbers[0]));
			maxLosses.add(Integer.valueOf(numbers[0]) - Integer.valueOf(numbers[1]));
			maxProfits.add(Integer.valueOf(numbers[2]) - Integer.valueOf(numbers[0]));
		}

		stockInfo.setCurrentValues(currentValues);
		stockInfo.setMaxLosses(maxLosses);
		stockInfo.setMaxProfits(maxProfits);

		return stockInfo;
	}

	static int getBiggestProfit(StockInfo stockInfo) {
		int N = stockInfo.getN();
		int B = stockInfo.getB();
		int L = stockInfo.getL();

		List<Integer> currentValues = stockInfo.getCurrentValues();
		List<Integer> maxLosses = stockInfo.getMaxLosses();
		List<Integer> maxProfits = stockInfo.getMaxProfits();

		int[][][] D = new int[N + 1][B + 1][L + 1];

		// obiectul 0 nu exista, e folosita
		// doar pe post de santinela linia corespunzatoare lui
		// iar intr-un rucsac cu 0kg nu intra nimic, deci avem valoarea 0
		for (int j = 0; j <= B; j++) {
			for (int k = 0; k <= L; k++) {
				D[0][j][k] = 0;
			}
		}

		for (int i = 1; i <= N; i++) {
			for (int j = 0; j <= B; j++) {
				for (int k = 0; k <= L; k++) {
					if ((j >= currentValues.get(i - 1))
						&& (k >= maxLosses.get(i - 1))) {
						// se ia maximul dintre obiectul i - 1 impreuna cu ce
						// mai ramane loc in ghiozdan si valoarea obtinuta la un pas anterior
						D[i][j][k] = Math.max(D[i - 1][j][k], maxProfits.get(i - 1)
						+ D[i - 1][j - currentValues.get(i - 1)][k - maxLosses.get(i - 1)]);
					} else {
						// daca nu e loc de noul obiect in ghiozdan
						// se ia valoarea obtinuta la un pas anterior
						D[i][j][k] = D[i - 1][j][k];
					}
				}
			}
		}

		return D[N][B][L];
	}


	public static void main(String[] args) {
		try {
			StockInfo stockInfo = readStocksData();
			int biggestProfit = getBiggestProfit(stockInfo);
			try {
				FileWriter myWriter = new FileWriter("stocks.out");
				myWriter.write(String.valueOf(biggestProfit));
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
