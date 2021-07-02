package commonclauses;

import usefuldatastructures.Graph;

import java.util.List;

public class CommonClausesSolver {

    int noFamilies;
    Graph graphFamilies;
    int K;

    /**
     * iterates trough the ids of the nodes in the graph
     * and build clauses so to create uniqueness
     * for any variable xiv representing vertex v on position i / having
     * color i being true, xjv should not be true
     * @return a string representing the type 3 clauses
     * in the format desired by the oracle
     */
    public String solveTypeUniquenessClauses() {
        StringBuilder clausesType3 = new StringBuilder();

        for (int familyId = 0; familyId < noFamilies; familyId++) {
            for (int i = 1; i < K; i++) {
                for (int j = i + 1; j <= K; j++) {
                    int var1 = familyId * K + i;
                    int var2 = familyId * K + j;

                    clausesType3.append("-").append(var1).
                            append(" -").append(var2).append(" 0\n");
                }
            }
        }

        return clausesType3.toString();
    }

    /**
     * this method denies every id existent in the list of ids
     * that is in a graph and creates a clause for each one
     * @param weighted indicates whether a max-sat/partial max_sat
     *                 problem has to be solved
     * @param weights holds the weights
     * @return string representing all the clauses created
     */
    public String solveTypeDenyId(boolean weighted, Integer... weights) {
        StringBuilder clausesTypeDenyId = new StringBuilder();

        String prefix = "";
        // in our problem there is just one possible weight
        // so no other cases are checked; the code can be easily
        // extended anyway
        if (weighted) {
            prefix = weights[0] + " ";
        }

        for (int familyId = 0; familyId < noFamilies; familyId++) {
            // we add 1 to familyId, as the count in our graph regarding
            // the ids of the families started from 0.
            clausesTypeDenyId.append(prefix).append("-").append(familyId + 1).
                    append(" 0\n");
        }

        return clausesTypeDenyId.toString();
    }

    public void setNoFamilies(int noFamilies) {
        this.noFamilies = noFamilies;
    }

    public void setK(int k) {
        K = k;
    }
}
