package commands;

import fileio.ActionInputData;
import fileio.Writer;
import org.json.simple.JSONObject;
import newfileio.Input;
import newfileio.UserData;
import utils.PseudoGetters;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Map;

/**
 * The class contains method that solves
 * the favorite command
 */
public class FavoriteCommand {

    private final Input input;
    private final Writer fileWriter;

    public FavoriteCommand(final Input input, final Writer fileWriter) {
        this.input = input;
        this.fileWriter = fileWriter;
    }


    /**
     * Solve the current favorite command and adds value to write to arrayResult
     *
     * @param command is the command received in the test
     * @return an JsonObject with the desired fields
     * @throws IOException in case of exceptions to reading / writing
     */
    public JSONObject solveFavoriteCommand(final ActionInputData command) throws IOException {
        String username = command.getUsername();
        String title = command.getTitle();

        int commandId = command.getActionId();
        String field = "";
        String message = "";

        UserData user = PseudoGetters.getUser(username, input);
        if (user != null) {
            Map<String, Integer> historyViews = user.getHistory();

            // check if the user saw the film so that he can not add
            // it to favorite if he has not seen it
            if (historyViews.containsKey(title)) {
                ArrayList<String> favMovies = user.getFavoriteMovies();

                if (favMovies.contains(title)) {
                    message = "error -> " + title + " is already in favourite list";
                } else {
                    favMovies.add(title);
                    message = "success -> " + title + " was added as favourite";
                }
            } else {
                message = "error -> " + title + " is not seen";
            }
        }
        return fileWriter.writeFile(commandId, field, message);
    }
}
