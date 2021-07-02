package network;

import usefuldatastructures.Graph;

import java.util.Scanner;

/**
 * This class build the mafia network (graph)
 */
public class MafiaNetworkBuilder {

    int noFamilies;
    Scanner scanner;

    public MafiaNetworkBuilder(int noFamilies, Scanner scanner) {
        this.noFamilies = noFamilies;
        this.scanner = scanner;
    }

    public Graph buildNetwork() {
        Graph graphFamilies = new Graph();

        // add vertexes to the graph
        for (int i = 0; i < noFamilies; i++) {
            graphFamilies.addVertex(i);
        }

        // establish the relationships between the families
        while (scanner.hasNextLine()) {
            String familiesIds = scanner.nextLine();

            // familyId[0] id of the first family
            // familyId[1] id of the seconds family
            String[] familyId = familiesIds.split(" ");

            // as we have the ids of the family in the graph
            // numbered from 0, we have to decrease 1 from the
            // input received
            int familyId1 = Integer.valueOf(familyId[0]) - 1;
            int familyId2 = Integer.valueOf(familyId[1]) - 1;

            // now add them to graph
            graphFamilies.addEdge(familyId1, familyId2);

        }
        scanner.close();

        return graphFamilies;
    }

}
