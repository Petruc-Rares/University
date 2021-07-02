import java.util.Arrays;

public class Utils {
	static Long maxLongValue(Long a, Long b) {
		return a > b ? a : b;
	}

	static Long minLongValue(Long a, Long b) {
		return a < b ? a : b;
	}

	static Long biggestInArray(long[] v) {
		return Arrays.stream(v).max().getAsLong();
	}

	static Long smallestInArray(long[] v) {
		return Arrays.stream(v).min().getAsLong();
	}
}
