package solution;

import commands.Command;
import common.Constants;
import newfileio.Input;
import fileio.ActionInputData;
import fileio.Writer;
import org.json.simple.JSONArray;
import queries.Query;
import recommendation.Recommendation;

import java.io.IOException;
import java.util.List;

public final class Solver {
    /**
     * Input read
     */
    private final Input input;
    /**
     * Array returned to main
     */
    private final JSONArray arrayResult;
    /**
     * The class with which we right the array
     */
    private final Writer fileWriter;

    public Solver(final Input input, final JSONArray arrayResult, final Writer fileWriter) {
        this.input = input;
        this.arrayResult = arrayResult;
        this.fileWriter = fileWriter;
    }

    /**
     * Calculate the total number of views from a given list of films
     * @param films it's the given list
     */
    public static int getTotalNoViews(final List<NameNumber> films) {
        int noViews = 0;
        for (NameNumber film : films) {
            noViews += film.getNumber();
        }
        return noViews;
    }

    /**
     * Compares two </ListNumber> elements
     */
    public static int compare(final List<NameNumber> e1, final List<NameNumber> e2) {
        return -(getTotalNoViews(e1) - getTotalNoViews(e2));
    }

    /**
     * Solve the current action
     *
     * @param action comes from main
     * @throws IOException in case of exceptions to reading / writing
     */
    public void solveAction(final ActionInputData action) throws IOException {
        String actionType = action.getActionType();

        switch (actionType) {
            case Constants.COMMAND -> {
                Command command = new Command(arrayResult, input, fileWriter);
                command.solveCommand(action);
            }
            case Constants.QUERY -> {
                Query query = new Query(arrayResult, input, fileWriter);
                query.solveQuery(action);
            }
            case Constants.RECOMMENDATION -> {
                Recommendation recommendation = new Recommendation(arrayResult, input, fileWriter);
                recommendation.solveRecommendation(action);
            }
            default -> throw new IllegalStateException("Unexpected value: " + actionType);
        }

    }

    /**
     * Solves the actions in the current test
     * no @param
     *
     * @throws IOException in case of exceptions to reading / writing
     */
    public void solve() throws IOException {
        List<ActionInputData> actions = input.getCommands();
        for (ActionInputData action : actions) {
            solveAction(action);
        }
    }

    public Input getInput() {
        return input;
    }
}
