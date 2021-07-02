package tasks;// Author: Matei Simtinică

import commonclauses.CommonClausesSolver;
import fileio.InputReader;
import fileio.OutputWriter;
import network.MafiaNetworkBuilder;
import oraclelanguage.OracleCommonPhrases;
import usefuldatastructures.VertexFamily;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * Bonus tasks.Task
 * You have to implement 4 methods:
 * readProblemData         - read the problem input and store it however you see fit
 * formulateOracleQuestion - transform the current problem instance into a SAT instance and write the oracle input
 * decipherOracleAnswer    - transform the SAT answer back to the current problem's answer
 * writeAnswer             - write the current problem's answer
 */
public class BonusTask extends Task {
    int numberSoftClausesDissatisfied;

    @Override
    public void solve() throws IOException, InterruptedException {
        readProblemData();
        formulateOracleQuestion();
        askOracle();
        decipherOracleAnswer();
        writeAnswer();
    }

    @Override
    public void readProblemData() throws IOException {
        InputReader inputReader = new InputReader(inFilename);

        String problemData = inputReader.readInput();

        Scanner scanner = new Scanner(problemData);

        // read the first line holding the number of families,
        // relationships and spies one after another
        if (scanner.hasNextLine()) {
            String initializerLine = scanner.nextLine();
            String[] numbers = initializerLine.split(" ");

            noFamilies = Integer.valueOf(numbers[NO_FAMILIES_POSITION]);
            noRelationships = Integer.valueOf(numbers[NO_RELATIONSHIPS_POSITION]);
        } else {
            // 1 - means nothing in file so can't solve
            // anything
            System.exit(1);
        }

        MafiaNetworkBuilder networkBuilder = new MafiaNetworkBuilder(noFamilies,
                scanner);
        graphFamilies = networkBuilder.buildNetwork();
    }

    int calculateTotalNoClauses() {
        int noHardClauses = noRelationships;
        int noSoftClauses = noFamilies;

        return noHardClauses + noSoftClauses;
    }

    String solveHardClauses() {
        StringBuilder clausesHard = new StringBuilder();

        // create the prefix for the hard clauses
        String prefix = (noFamilies + 1) + " ";

        for (int familyId = 0; familyId < noFamilies; familyId++) {
            List<VertexFamily> friendFamilies = graphFamilies.
                    getAdjVertices(familyId);

            for (VertexFamily friendFamily: friendFamilies) {

                int friendFamilyId = friendFamily.getId();
                // check if edge has not been previously analysed
                if (friendFamilyId  > familyId) {
                    // we add 1 as the count in our graph started from
                    // 0 regarding the ids
                    int var1 = familyId + 1;
                    int var2 = friendFamilyId + 1;

                    clausesHard.append(prefix).append(var1).
                            append(" ").append(var2).append(" 0\n");
                }
            }
        }
        return clausesHard.toString();
    }

    @Override
    public void formulateOracleQuestion() throws IOException {
        int noClauses = calculateTotalNoClauses();

        // for each possible color of a node we establish a number
        OracleCommonPhrases oracleCommonPhrases = new
                OracleCommonPhrases(noClauses, K, noFamilies);

        String clauses = oracleCommonPhrases.
                buildSpecialAddressingForm(noFamilies + 1);
        noVariables = noFamilies;

        // hard clauses
        clauses += solveHardClauses();

        // soft clauses
        CommonClausesSolver commonClausesSolver = new CommonClausesSolver();
        commonClausesSolver.setNoFamilies(noFamilies);

        clauses += commonClausesSolver.solveTypeDenyId(true, 1);

        // write to the inputSat the reformulated problem
        OutputWriter outputWriter = new OutputWriter(oracleInFilename);
        outputWriter.writeOutput(clauses);
    }

    @Override
    public void decipherOracleAnswer() throws IOException {
        InputReader answerReader = new InputReader(oracleOutFilename);
        String oracleResponse = answerReader.readInput();

        String[] firstLineData = new String[0];

        Scanner scanner = new Scanner(oracleResponse);
        if (scanner.hasNextLine()) {
            firstLineData = scanner.nextLine().split(" ");
        } else {
            // 1 - means nothing in file so can't solve anything
            System.exit(1);
        }

        noVariables = Integer.valueOf(firstLineData[0]);
        numberSoftClausesDissatisfied = Integer.valueOf(firstLineData[1]);

        variablesValues = scanner.nextLine().split(" ");

        scanner.close();
    }

    @Override
    public void writeAnswer() throws IOException {
        OutputWriter interpretationOutput = new OutputWriter(outFilename);

        String interpretation = "";
        // check the minimum k-cover that can be established in the graph
        for (int it = 0; it < noFamilies; it++) {

            // if current node can take part of the k-cover
            // write it to the output
            if (!(variablesValues[it].charAt(SIGN_POSITION)
                    == MINUS_ASCII)) {
                interpretation += variablesValues[it] + " ";
            }
        }

        interpretationOutput.writeOutput(interpretation);
    }
}
