package tasks;// Author: Matei Simtinică

import fileio.InputReader;
import network.MafiaNetworkBuilder;
import fileio.OutputWriter;
import usefuldatastructures.Graph;
import usefuldatastructures.VertexFamily;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * tasks.Task3
 * This being an optimization problem, the solve method's logic has to work differently.
 * You have to search for the minimum number of arrests by successively querying the oracle.
 * Hint: it might be easier to reduce the current task to a previously solved task
 */
public class Task3 extends Task {
    String task2InFilename;
    String task2OutFilename;
    /**
     * the complementary graph of @graphFamilies
     */
    private Graph complementaryGraphFamilies;
    /**
     * list containing all the nodes of mafia families
     */
    private List<VertexFamily> familiesNodes;
    /**
     * number of families that have to be arrested
     */
    private int noFamiliesArrested;
    /**
     * ids of the most influential/dangerous family
     */
    List<Integer> toArrestFamilies;

    public void buildComplementaryGraph() {
        complementaryGraphFamilies = new Graph();
        for (int familyId = 0; familyId < noFamilies; familyId++) {
            complementaryGraphFamilies.addVertex(familyId);
        }

        for (int familyId = 0; familyId < noFamilies; familyId++) {
            List<VertexFamily> friendFamilies = graphFamilies.
                    getAdjVertices(familyId);
            friendFamilies.add(new VertexFamily(familyId));

            List<VertexFamily> nonFriendFamilies = new ArrayList<>(familiesNodes);

            nonFriendFamilies.removeAll(friendFamilies);

            for (VertexFamily nonFriendFamily: nonFriendFamilies) {
                if (nonFriendFamily.getId() > familyId) {
                    complementaryGraphFamilies.addEdge(familyId,
                            nonFriendFamily.getId());
                }
            }
        }
    }

    @Override
    public void solve() throws IOException, InterruptedException {
        task2InFilename = inFilename + "_t2";
        task2OutFilename = outFilename + "_t2";
        Task2 task2Solver = new Task2();
        task2Solver.addFiles(task2InFilename, oracleInFilename,
                oracleOutFilename, task2OutFilename);
        readProblemData();

        task2Solver.setNoFamilies(noFamilies);
        task2Solver.setNoRelationships(noRelationships);
        task2Solver.setFamiliesNodes(familiesNodes);

        buildComplementaryGraph();

        task2Solver.setGraphFamilies(complementaryGraphFamilies);
        for (K = 1; K <= noFamilies; K++) {
            reduceToTask2(task2Solver, K);
        }

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

    public void reduceToTask2(Task2 task2, int k) throws IOException,
            InterruptedException {
        task2.setSizeFamily(k);
        task2.formulateOracleQuestion();
        task2.askOracle();
        task2.decipherOracleAnswer();
        if (task2.isSizeFamilyExists()) {
            // new max for k-clique found, so
            // new min for k-cover found
            extractAnswerFromTask2(task2, k);
        }
    }

    public int getFamilyNumber(int number) {
        int remainder = number % K;

        if (remainder == 0) {
            return (number / K);
        }

        return (number / K + 1);

    }

    public void extractAnswerFromTask2(Task2 task2, int k) {
        noFamiliesArrested = noFamilies - k;
        variablesValues = task2.getVariablesValues();

        toArrestFamilies = new ArrayList<>();
        for (int i = 0; i < noFamilies; i++) {
            toArrestFamilies.add(i);
        }

        List<Integer> notInfleuntFamilies = new ArrayList<>();
        for (int itSplit = 0; itSplit < variablesValues.length; itSplit += k) {
            for (int it = itSplit; it < itSplit + k; it++) {
                if (!(variablesValues[it].charAt(0) == 45)) {
                    int colorNumber = getFamilyNumber(Integer.
                            valueOf(variablesValues[it])) - 1;
                    notInfleuntFamilies.add(colorNumber);
                }
            }
        }

        toArrestFamilies.removeAll(notInfleuntFamilies);
    }

    @Override
    public void writeAnswer() throws IOException {
        // at the moment this function is called, toArrestFamilies and
        // noFamiliesArrested are initialised
        OutputWriter interpretationOutput = new OutputWriter(outFilename);

        StringBuilder arrestedFamilies = new StringBuilder();

        for (int it = 0; it < noFamiliesArrested; it++) {
            arrestedFamilies.append(toArrestFamilies.get(it) + 1).append(" ");
        }

        interpretationOutput.writeOutput(arrestedFamilies.toString());

    }
}
