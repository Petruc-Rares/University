package tasks;// Author: Matei Simtinică

import fileio.InputReader;
import usefuldatastructures.Graph;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;

/**
 * This is the abstract base class for all tasks that have to be implemented.
 */
public abstract class Task {
    /**
     * the file who holds the initial data of the problem
     */
    String inFilename;
    /**
     * the file where the problem is translated, so that the oracle
     * can understand it
     */
    String oracleInFilename;
    /**
     * the file where the response to the problem
     * is given by the oracle
     */
    String oracleOutFilename;
    /**
     * the file where the final interpretation of the problem
     * is given
     */
    String outFilename;
    /**
     * number of mafia families
     */
    int noFamilies;
    /**
     * number of the relationships between the
     * families
     */
    int noRelationships;
    /**
     * common parameter in npc problems
     * might represent:
     * number of the spies who must infiltrate
     * between the families or
     * the size of the clique we are looking for in a graph
     */
    int K;
    /**
     * the graph who stores the families and the
     * relationships between them
     */
    Graph graphFamilies;
    /**
     * represents the first part of the
     * answer given by the oracle
     * to the current problem
     */
    boolean canBeDone;
    /**
     * represents the second part of the answer
     * given by the oracle to the current problem
     */
    int noVariables;
    /**
     * represents an array of strings representing
     * integer numbers (positive for the ones holding
     * true values, respectively negative for ones holding
     * false values)
     */
    String[] variablesValues;

    static final int MINUS_ASCII = 45;
    /**
     * used for checking if a number is negative
     */
    static final int SIGN_POSITION = 0;

    static final int NO_FAMILIES_POSITION = 0;
    static final int NO_RELATIONSHIPS_POSITION = 1;
    static final int K_POSITION = 2;

    public abstract void solve() throws IOException, InterruptedException;

    public abstract void readProblemData() throws IOException;

    public void formulateOracleQuestion() throws IOException {}

    public void decipherOracleAnswer() throws IOException {
        InputReader answerReader = new InputReader(oracleOutFilename);
        String oracleResponse = answerReader.readInput();

        Scanner scanner = new Scanner(oracleResponse);
        if (scanner.hasNextLine()) {
            canBeDone = Boolean.valueOf(scanner.nextLine());
        } else {
            // 1 - means nothing in file so can't solve anything
            System.exit(1);
        }

        if (!canBeDone) {
            return;
        }

        noVariables = Integer.valueOf(scanner.nextLine());

        variablesValues = scanner.nextLine().split(" ");

        scanner.close();
    }

    public abstract void writeAnswer() throws IOException;

    /**
     * Stores the files paths as class attributes.
     *
     * @param inFilename         the file containing the problem input
     * @param oracleInFilename   the file containing the oracle input
     * @param oracleOutFilename  the file containing the oracle output
     * @param outFilename        the file containing the problem output
     */
    public void addFiles(String inFilename, String oracleInFilename, String oracleOutFilename, String outFilename) {
        this.inFilename = inFilename;
        this.oracleInFilename = oracleInFilename;
        this.oracleOutFilename = oracleOutFilename;
        this.outFilename = outFilename;
    }

    /**
     * Asks the oracle for an answer to the formulated question.
     *
     * @throws IOException
     * @throws InterruptedException
     */
    public void askOracle() throws IOException, InterruptedException {
        ProcessBuilder builder = new ProcessBuilder();
        builder.redirectErrorStream(true);
        builder.command("python3", "sat_oracle.py", oracleInFilename, oracleOutFilename);
        Process process = builder.start();
        BufferedReader in = new BufferedReader(new InputStreamReader(process.getInputStream()));
        String buffer;
        StringBuilder output = new StringBuilder();

        while ((buffer = in.readLine()) != null) {
            output.append(buffer).append("\n");
        }

        int exitCode = process.waitFor();
        if (exitCode != 0) {
            System.err.println("Error encountered while running oracle");
            System.err.println(output.toString());
            System.exit(-1);
        }
    }
}
