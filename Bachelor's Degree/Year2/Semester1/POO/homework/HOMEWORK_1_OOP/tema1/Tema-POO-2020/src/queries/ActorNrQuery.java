package queries;

import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import newfileio.UserData;
import org.json.simple.JSONObject;
import solution.NameNumber;
import utils.Utils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * The class contains method that solves a query
 * of type "Actor" with criteria "number of ratings"
 */
public class ActorNrQuery {
    private final Input input;
    private final Writer fileWriter;

    public ActorNrQuery(final Input input, final Writer fileWriter) {
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current query
     * @param query represents the current query
     * @return JSONObject in the desired format
     * @throws IOException in case of exceptions to reading / writing
     */
    public JSONObject solveQuery(final ActionInputData query) throws IOException {
        List<UserData> users = input.getUsers();
        List<NameNumber> userNum = new ArrayList<>();
        for (UserData user : users) {
            if (user.getHistoryRatings().size() != 0) {
                userNum.add(
                        new NameNumber(user.getUsername(),
                                (double) user.getHistoryRatings().size()));
            }
        }
        return fileWriter.writeFile(query.getActionId(),
                "", Utils.buildFilms(query, userNum));
    }
}
