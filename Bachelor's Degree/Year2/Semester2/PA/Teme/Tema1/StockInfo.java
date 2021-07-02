import java.util.List;

public class StockInfo {
	private int N;
	private int B;
	private int L;

	private List<Integer> currentValues;
	private List<Integer> maxLosses;
	private List<Integer> maxProfits;

	public int getN() {
		return N;
	}

	public void setN(int n) {
		N = n;
	}

	public int getB() {
		return B;
	}

	public void setB(int b) {
		B = b;
	}

	public int getL() {
		return L;
	}

	public void setL(int l) {
		L = l;
	}

	public List<Integer> getCurrentValues() {
		return currentValues;
	}

	public void setCurrentValues(List<Integer> currentValues) {
		this.currentValues = currentValues;
	}

	public List<Integer> getMaxLosses() {
		return maxLosses;
	}

	public void setMaxLosses(List<Integer> maxLosses) {
		this.maxLosses = maxLosses;
	}

	public List<Integer> getMaxProfits() {
		return maxProfits;
	}

	public void setMaxProfits(List<Integer> maxProfits) {
		this.maxProfits = maxProfits;
	}

	@Override
	public String toString() {
		return "StockInfo{"
				+ "N=" + N
				+ ", B=" + B
				+ ", L=" + L
				+ ", currentValues=" + currentValues
				+ ", maxLosses=" + maxLosses
				+ ", maxProfits=" + maxProfits
				+ '}';
	}
}
