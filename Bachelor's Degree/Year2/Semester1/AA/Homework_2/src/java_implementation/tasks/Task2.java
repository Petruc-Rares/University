package tasks;// Author: Matei Simtinică

import commonclauses.CommonClausesSolver;
import fileio.InputReader;
import network.MafiaNetworkBuilder;
import fileio.OutputWriter;
import oraclelanguage.OracleCommonPhrases;
import usefuldatastructures.Graph;
import usefuldatastructures.VertexFamily;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * tasks.Task2
 * You have to implement 4 methods:
 * readProblemData         - read the problem input and store it however you see fit
 * formulateOracleQuestion - transform the current problem instance into a SAT instance and write the oracle input
 * decipherOracleAnswer    - transform the SAT answer back to the current problem's answer
 * writeAnswer             - write the current problem's answer
 */
public class Task2 extends Task {
    /**
     * list containing all the nodes of mafia families
     */
    private List<VertexFamily> familiesNodes;

    public Task2() {

    }

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

            familiesNodes = new ArrayList<>();
            for (int i = 0; i < noFamilies; i++) {
                VertexFamily family = new VertexFamily(i);
                familiesNodes.add(family);
            }
        } else {
            // 1 - means nothing in file so can't solve
            // anything
            System.exit(1);
        }

        MafiaNetworkBuilder networkBuilder = new MafiaNetworkBuilder(noFamilies,
                scanner);
        graphFamilies = networkBuilder.buildNetwork();
    }

    public int calculateTotalNoClauses() {
        int clausesTypeA = K;
        int clausesTypeB = (noFamilies * (noFamilies - 1) / 2
                - noRelationships) * (K - 1) * K;
        int clausesTypeC = K * (K - 1) / 2 * noFamilies +
                noFamilies * (noFamilies - 1) / 2 * K;

        return clausesTypeA + clausesTypeB + clausesTypeC;
    }

    public String solveTypeAClauses() {
        StringBuilder clausesTypeA = new StringBuilder();
        for (int counter = 1; counter <= K; counter++) {
            for (int familyId = 0; familyId < noFamilies; familyId++) {
                int var = familyId * K + counter;
                clausesTypeA.append(var).append(" ");
            }
            clausesTypeA.append("0\n");
        }
        return clausesTypeA.toString();
    }

    public String solveTypeBClauses() {
        StringBuilder clausesTypeB = new StringBuilder();
        for (int famId = 0; famId < noFamilies; famId++) {
            List<VertexFamily> friendFamilies = graphFamilies.
                    getAdjVertices(famId);
            friendFamilies.add(new VertexFamily(famId));

            List<VertexFamily> nonFriendFamilies = new ArrayList<>(familiesNodes);

            nonFriendFamilies.removeAll(friendFamilies);

            for (VertexFamily nonFriendFamily: nonFriendFamilies) {
                if (nonFriendFamily.getId() > famId) {
                    for (int it1 = 1; it1 <= K; it1++) {
                        int it2 = 1;
                        while (it2 <= K) {
                            if (it2 != it1) {
                                int var1 = famId * K + it1;
                                int var2 = nonFriendFamily.getId() * K + it2;

                                clausesTypeB.append(-var1).append(" ").
                                        append(-var2).append(" 0\n");
                            }
                            it2++;
                        }
                    }
                }
            }
        }
        return clausesTypeB.toString();
    }

    public String solveTypeCbClauses() {
        StringBuilder clausesTypeCb = new StringBuilder();
        for (int var1 = 1; var1 <= (noFamilies - 1) * K; var1++) {
            for (int var2 = var1 + K; var2 <= noFamilies * K;
                 var2 += K) {
                clausesTypeCb.append(-var1).append(" ").append(-var2).append(" ").
                        append(" 0\n");
            }
        }
        return clausesTypeCb.toString();
    }

    @Override
    public void formulateOracleQuestion() throws IOException {
        // for each possible color of a node we establish a number
        int noClauses = calculateTotalNoClauses();

        OracleCommonPhrases oracleCommonPhrases =
                new OracleCommonPhrases(noClauses, K, noFamilies);

        String clauses = oracleCommonPhrases.buildAddressingForm();

        // type a clauses
        clauses += solveTypeAClauses();

        clauses += solveTypeBClauses();

        // type c clauses
        CommonClausesSolver commonClausesSolver = new CommonClausesSolver();
        commonClausesSolver.setK(K);
        commonClausesSolver.setNoFamilies(noFamilies);

        // type c first type of clauses
        // vertex in clique not occupying two different
        // positions in it
        clauses += commonClausesSolver.solveTypeUniquenessClauses();

        // type c second type of clauses
        // not occupying the same position in the clique
        clauses += solveTypeCbClauses();

        // write to the inputSat the reformulated problem
         OutputWriter outputWriter = new OutputWriter(oracleInFilename);
         outputWriter.writeOutput(clauses);
    }

    /**
     * in case a sizeFamily-clique is found
     * check which families take part of it
     * @param number representing the current variable (positive number)
     * @return the id of the family
     */
    public int getFamilyId(int number) {
        int remainder = number % K;

        if (remainder == 0) {
            return (number / K);
        }

        return (number / K + 1);

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
                if (!(variablesValues[it].charAt(SIGN_POSITION)
                        == MINUS_ASCII)) {
                    int colorNumber = getFamilyId(Integer.
                            valueOf(variablesValues[it]));
                    interpretation += colorNumber + " ";
                }
            }
        }
        interpretationOutput.writeOutput(interpretation);
    }

    public void setSizeFamily(int sizeFamily) {
        this.K = sizeFamily;
    }

    public boolean isSizeFamilyExists() {
        return canBeDone;
    }

    public void setNoFamilies(int noFamilies) {
        this.noFamilies = noFamilies;
    }

    public void setNoRelationships(int noRelationships) {
        this.noRelationships = noRelationships;
    }

    public void setGraphFamilies(Graph graphFamilies) {
        this.graphFamilies = graphFamilies;
    }

    public void setFamiliesNodes(List<VertexFamily> familiesNodes) {
        this.familiesNodes = familiesNodes;
    }

    public String[] getVariablesValues() {
        return variablesValues;
    }
}
