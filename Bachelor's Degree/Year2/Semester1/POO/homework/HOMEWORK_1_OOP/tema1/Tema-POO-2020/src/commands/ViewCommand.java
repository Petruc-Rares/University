package commands;

import fileio.ActionInputData;
import fileio.Writer;
import org.json.simple.JSONObject;
import newfileio.Input;
import newfileio.UserData;
import utils.PseudoGetters;

import java.io.IOException;
import java.util.Map;

/**
 * The class contains method that solves
 * the view command
 */
public class ViewCommand {
    private final Input input;
    private final Writer fileWriter;

    public ViewCommand(final Input input, final Writer fileWriter) {
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current view command and adds value to write to arrayResult
     * @param command represents the current command
     * @return an JsonObject with the desired fields
     * @throws IOException in case of exceptions to reading / writing
     */
    public JSONObject solveViewCommand(final ActionInputData command) throws IOException {
        String username = command.getUsername();
        String title = command.getTitle();

        int commandId = command.getActionId();
        String field = "";
        String message = "";

        UserData user = PseudoGetters.getUser(username, input);
        if (user != null) {
            Map<String, Integer> historyViews = user.getHistory();
            Integer filmViews = historyViews.get(title);

            // check if the user saw the film
            // if seen already, increment filmViews
            if (historyViews.containsKey(title)) {
                filmViews++;
            } else {
                // add the film to seen films and set view to 1
                filmViews = 1;
            }
            historyViews.put(title, filmViews);
            message = "success -> " + title + " was viewed with total views of " + filmViews;
        }
        return fileWriter.writeFile(commandId, field, message);
    }

}
