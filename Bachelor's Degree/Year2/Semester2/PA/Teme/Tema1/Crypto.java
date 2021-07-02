import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Objects;
import java.util.Scanner;

public class Crypto {
	static class InfoComputer {
		int power;
		int updatePrice;

		InfoComputer(int power, int updatePrice) {
			this.power = power;
			this.updatePrice = updatePrice;
		}

		public int getPower() {
			return power;
		}

		public void setPower(int power) {
			this.power = power;
		}

		public int getUpdatePrice() {
			return updatePrice;
		}

		@Override
		public boolean equals(Object o) {
			if (this == o) { 
				return true;
			}
			if (o == null || getClass() != o.getClass()) {
				return false;
			}
			InfoComputer that = (InfoComputer) o;
			return power == that.power && updatePrice == that.updatePrice;
		}

		@Override
		public int hashCode() {
			return Objects.hash(power, updatePrice);
		}

		@Override
		public String toString() {
			return "InfoComputer{"
					+ "power=" + power
					+ ", updatePrice=" + updatePrice
					+ '}';
		}
	}


	static class FieldsComparator implements Comparator<InfoComputer> {
		public int compare(InfoComputer calc1, InfoComputer calc2) {
			int power1 = calc1.getPower();
			int power2 = calc2.getPower();

			int diffPower = power1 - power2;

			if (diffPower != 0) {
				return diffPower;
			}

			int price1 = calc1.getUpdatePrice();
			int price2 = calc2.getUpdatePrice();

			if (price1 > price2) {
				return -1;
			} else if (price1 < price2) { 
				return 1;
			}
			return 0;
		}
	}

	static class InfoGigel {
		private int noComputers;
		private int budget;

		public int getNoComputers() {
			return noComputers;
		}

		public int getBudget() {
			return budget;
		}

		public void setNoComputers(int noComputers) {
			this.noComputers = noComputers;
		}

		public void setBudget(int budget) {
			this.budget = budget;
		}

		@Override
		public String toString() {
			return "InfoGigel{"
					+ "noComputers=" + noComputers
					+ ", budget=" + budget
					+ '}';
		}
	}

	public static List<InfoComputer> readComputersData() throws IOException {
		List<InfoComputer> infoComputers = new ArrayList<>();

		InputReader inputReader = new InputReader("crypto.in");

		String problemData = inputReader.readInput();

		Scanner scanner = new Scanner(problemData);

		// pass first line, as it holds gigel info
		scanner.nextLine();

		// read each line holding power and updateInfo
		while (scanner.hasNextLine()) {
			String initializerLine = scanner.nextLine();
			String[] numbers = initializerLine.split(" ");

			infoComputers.add(new InfoComputer(Integer.valueOf(numbers[0]),
												Integer.valueOf(numbers[1])));
		}

		return infoComputers;
	}


	public static InfoGigel readGigelData() throws IOException {
		InfoGigel infoGigel = new InfoGigel();

		InputReader inputReader = new InputReader("crypto.in");

		String problemData = inputReader.readInput();

		Scanner scanner = new Scanner(problemData);

		// read the first line holding the number of calculators
		// and the budget
		if (scanner.hasNextLine()) {
			String initializerLine = scanner.nextLine();
			String[] numbers = initializerLine.split(" ");

			infoGigel.setNoComputers(Integer.valueOf(numbers[0]));
			infoGigel.setBudget(Integer.valueOf(numbers[1]));
		} else {
			// 1 - means nothing in file so can't solve
			// anything
			System.exit(1);
		}

		scanner.close();
		return infoGigel;
	}


	public static void main(String[] args) {
		try {
			InfoGigel infoGigel = readGigelData();
			List<InfoComputer> infoComputers = readComputersData();

			//          System.out.println(infoComputers);

			infoComputers.sort(new FieldsComparator());

			//          System.out.println(infoComputers);

			boolean updateWorth = true;
			Integer crtIndex = 0;
			Integer currentSpent = 0;
			Integer noComputers = infoGigel.getNoComputers();
			Integer budget = infoGigel.getBudget();
			Integer needToUpdatePrice = 0;
			while (true) {
				InfoComputer needsUpdateComputer = infoComputers.get(crtIndex);
				int power = needsUpdateComputer.getPower();
				int price = needsUpdateComputer.getUpdatePrice() + needToUpdatePrice;

				if (price > (budget - currentSpent)) {
					break;
				}

				for (int i = crtIndex + 1; i < noComputers; i++) {
					InfoComputer nextComputer = infoComputers.get(i);

					if (nextComputer.getPower() != power) {
						break;
					}

					crtIndex++;

					price += nextComputer.getUpdatePrice();
					if (price > (budget - currentSpent)) {
						updateWorth = false;
						break;
					}
				}

				// if gigel doesn't have enough money to upgrade all computers
				// with the current lowest power, there's no point in going forward
				if (!updateWorth) {
					break;
				}

				currentSpent += price;
				needToUpdatePrice = price;

				infoComputers.get(crtIndex).setPower(power + 1);

				if (crtIndex < noComputers - 1) {
					if (infoComputers.get(crtIndex).getPower()
						== infoComputers.get(crtIndex + 1).getPower()) {
						crtIndex++;
					} else {
						Integer diffPower = infoComputers.get(crtIndex + 1).getPower()
											- infoComputers.get(crtIndex).getPower();
						// this means we will never reach the power
						// of the next computer
						if (diffPower >= ((budget - currentSpent) / needToUpdatePrice)) {
							infoComputers.get(crtIndex).setPower(
								infoComputers.get(crtIndex).getPower()
								+ (budget - currentSpent) / needToUpdatePrice);
							break;
						}
						needToUpdatePrice -= infoComputers.get(crtIndex).getUpdatePrice();
						// we need to update again in order to have more power
					}
				} else {
					// all computers have the same power now
					needToUpdatePrice -= infoComputers.get(crtIndex).getUpdatePrice();
				}
			}
			try {
				System.out.println(currentSpent);
				FileWriter myWriter = new FileWriter("crypto.out");
				myWriter.write(String.valueOf(infoComputers.get(crtIndex).getPower()));
				myWriter.close();
			} catch (IOException e) {
				System.out.println("An error occurred.");
				e.printStackTrace();
			}
		} catch (IOException e) {
			System.out.println("An error occurred.");
			e.printStackTrace();
		}
	}
}