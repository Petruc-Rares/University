import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

/**
 * this class helps in reading some input from the specified inputFileName
 */
public class InputReader {
	private String inputFileName;

	public InputReader(String inputFileName) {
		this.inputFileName = inputFileName;
	}

	/**
	 * reads all the lines using
	 * @param reader of type BufferedReader for an easy parse of the file
	 * @return a string representing the content of the file
	 * @throws IOException if something wrong happens
	 */
	public String readAllLines(BufferedReader reader) throws IOException {
		StringBuilder content = new StringBuilder();
		String line;

		while ((line = reader.readLine()) != null) {
			content.append(line);
			content.append(System.lineSeparator());
		}

		return content.toString();
	}

	/**
	 * reads the input from the given inputFileName
	 * @return the input in a string format
	 * @throws IOException if something wrong happens
	 */
	public String readInput() throws IOException {
		BufferedReader reader =
				Files.newBufferedReader(Paths.get(inputFileName));

		return readAllLines(reader);
	}
}
