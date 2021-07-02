package commands;

import common.Constants;
import fileio.ActionInputData;
import fileio.Writer;
import org.json.simple.JSONArray;
import newfileio.Input;

import java.io.IOException;

@SuppressWarnings("unchecked")
public class Command {

    private final JSONArray arrayResult;
    private final Input input;
    private final Writer fileWriter;

    public Command(final JSONArray arrayResult, final Input input, final Writer fileWriter) {
        this.arrayResult = arrayResult;
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current command
     * @param command represents the current command
     * @throws IOException in case of exceptions to reading / writing
     */
    public void solveCommand(final ActionInputData command) throws IOException {
        String type = command.getType();

        switch (type) {
            case Constants.FAVORITE -> {
                FavoriteCommand favCommand = new FavoriteCommand(input, fileWriter);
                arrayResult.add(favCommand.solveFavoriteCommand(command));
            }
            case Constants.VIEW -> {
                ViewCommand viewCommand  = new ViewCommand(input, fileWriter);
                arrayResult.add(viewCommand.solveViewCommand(command));
            }
            case Constants.RATING -> {
                RatingCommand ratingCommand = new RatingCommand(input, fileWriter);
                arrayResult.add(ratingCommand.solveRatingCommand(command));
            }
            default -> throw new IllegalStateException("Unexpected value: " + type);
        }
    }
}
