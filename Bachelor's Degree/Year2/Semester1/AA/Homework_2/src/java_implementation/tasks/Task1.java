package tasks;// Author: Matei Simtinică

import commonclauses.CommonClausesSolver;
import fileio.InputReader;
import fileio.OutputWriter;
import network.MafiaNetworkBuilder;
import oraclelanguage.OracleCommonPhrases;
import usefuldatastructures.VertexFamily;

import java.io.IOException;
import java.util.List;
import java.util.Scanner;

/**
 * tasks.Task1
 * You have to implement 4 methods:
 * readProblemData         - read the problem input and store it however you see fit
 * formulateOracleQuestion - transform the current problem instance into a SAT instance and write the oracle input
 * decipherOracleAnswer    - transform the SAT answer back to the current problem's answer
 * writeAnswer             - write the current problem's answer
 */
public class Task1 extends Task {

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
            K = Integer.valueOf(numbers[K_POSITION]);
        } else {
            // 1 - means nothing in file so can't solve
            // anything
            System.exit(1);
        }

        MafiaNetworkBuilder networkBuilder = new MafiaNetworkBuilder(noFamilies,
                scanner);
        graphFamilies = networkBuilder.buildNetwork();
    }


    /**
     * iterates through the edges of the graph
     * and build the type 1 clauses
     * @return a string representing the clauses
     * in the format desired by the oracle
     */
    public String solveType1Clauses() {
        StringBuilder clausesType1 = new StringBuilder();
        for (int familyId = 0; familyId < noFamilies; familyId++) {
            List<VertexFamily> friendFamilies = graphFamilies.
                    getAdjVertices(familyId);

            for (VertexFamily friendFamily: friendFamilies) {

                int friendFamilyId = friendFamily.getId();
                // check if edge has not been previously analysed
                if (friendFamilyId  > familyId) {

                    for (int counter = 1; counter <= K; counter++) {
                        int var1 = familyId * K + counter;
                        int var2 = friendFamilyId * K + counter;

                        clausesType1.append("-").append(var1).
                                append(" -").append(var2).append(" 0\n");
                    }
                }
            }
        }
        return clausesType1.toString();
    }

    /**
     * iterates trough the nodes of the graph and
     * build the type 2 clauses
     * @return a string representing the type 2 clauses
     * in the format desired by the oracle
     */
    public String solveType2Clauses() {
        StringBuilder clausesType2 = new StringBuilder();
        for (int familyId = 0; familyId < noFamilies; familyId++) {
            for (int counter = 1; counter <= K; counter++) {
                int var = familyId * K + counter;

                clausesType2.append(var).append(" ");
            }
            clausesType2.append("0\n");
        }
        return clausesType2.toString();
    }

    public int calculateTotalNoClauses() {
        int noClausesType1 = noRelationships * K;
        int noClausesType2 = noFamilies;
        int noClausesType3 = K * (K - 1) / 2 * noFamilies;

        return noClausesType1 + noClausesType2 + noClausesType3;
    }

    @Override
    public void formulateOracleQuestion() throws IOException {
        int noClauses = calculateTotalNoClauses();

        // for each possible color of a node we establish a number
        OracleCommonPhrases oracleCommonPhrases = new
                OracleCommonPhrases(noClauses, K, noFamilies);

        String clauses = oracleCommonPhrases.buildAddressingForm();
        noVariables = K * noFamilies;

        // type 1 clauses
        clauses += solveType1Clauses();

        // type 2 clauses
        clauses += solveType2Clauses();

        // type 3 clauses
        CommonClausesSolver commonClausesSolver = new CommonClausesSolver();
        commonClausesSolver.setK(K);
        commonClausesSolver.setNoFamilies(noFamilies);

        clauses += commonClausesSolver.solveTypeUniquenessClauses();

        // write to the inputSat the reformulated problem
        OutputWriter outputWriter = new OutputWriter(oracleInFilename);
        outputWriter.writeOutput(clauses);
    }

    /**
     * in case the assignment of the spies can be done
     * check which spy infiltrates in which family
     * @param number representing the current variable (positive number)
     * @return the id of the spy
     */
    public int getSpyNumber(int number) {
        int remainder = number % K;

        if (remainder == 0) {
            return K;
        }

        return remainder;
    }

    @Override
    public void writeAnswer() throws IOException {
        OutputWriter interpretationOutput = new OutputWriter(outFilename);
        // if the number of spies is insufficient
        if(!canBeDone) {
            interpretationOutput.writeOutput("False");
            return;
        }

        // true case
        String interpretation = "True" + "\n";

        // now check which spy is assigned to which family
        // acording to the answer given by the oracle
        for (int itSplit = 0; itSplit < variablesValues.length; itSplit += K) {
            for (int it = itSplit; it < itSplit + K; it++) {
                if (!(variablesValues[it].charAt(SIGN_POSITION) ==
                        MINUS_ASCII)) {
                    int colorNumber = getSpyNumber(Integer.
                            valueOf(variablesValues[it]));
                    interpretation += colorNumber + " ";
                }
            }
        }

        interpretationOutput.writeOutput(interpretation);
    }
}
