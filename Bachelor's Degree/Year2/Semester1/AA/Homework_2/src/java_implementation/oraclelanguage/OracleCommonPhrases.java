package oraclelanguage;

/**
 * This class is responsible for composing the classic
 * phrases the oracle wants to hear
 */
public class OracleCommonPhrases {
    int noClauses;
    int K;
    int noFamilies;

    public OracleCommonPhrases(int noClauses, int k, int noFamilies) {
        this.noClauses = noClauses;
        this.K = k;
        this.noFamilies = noFamilies;
    }

    /**
     * this method creates and returns the addressing form
     * to the oracle for the case of solving sat problems
     * @return a string containing the addressing
     */
    public String buildAddressingForm() {
        StringBuilder clauses = new StringBuilder();
        clauses.append("p cnf ");
        int noVariables = K * noFamilies;

        clauses.append(noVariables).append(" ").append(noClauses).append("\n");

        return clauses.toString();
    }

    /**
     * this method creates and returns the addressing form
     * to the oracle for the case of solving partial sat or
     * weighted partial sat problems
     * @return a string containing the addressing
     */
    public String buildSpecialAddressingForm(int top) {
        StringBuilder clauses = new StringBuilder();
        clauses.append("p wcnf ");
        int noVariables = noFamilies;

        clauses.append(noVariables).append(" ").append(noClauses).
                append(" ").append(top).append("\n");

        return clauses.toString();
    }
}
