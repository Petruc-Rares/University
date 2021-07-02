package commands;

import fileio.ActionInputData;
import fileio.Writer;
import org.json.simple.JSONObject;
import newfileio.Input;
import newfileio.MovieData;
import newfileio.SeasonData;
import newfileio.SerialData;
import newfileio.UserData;
import utils.PseudoGetters;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class RatingCommand {
    private final Input input;
    private final Writer fileWriter;

    public RatingCommand(final Input input, final Writer fileWriter) {
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current rating command and adds value to write to arrayResult
     * @param command represents the current command
     * @return an JsonObject with the desired fields
     * @throws IOException in case of exceptions to reading / writing
     */
    public JSONObject solveRatingCommand(final ActionInputData command) throws IOException {
        String username = command.getUsername();
        String title = command.getTitle();

        int commandId = command.getActionId();
        String field = "";
        String message = "";

        UserData user = PseudoGetters.getUser(username, input);
        if (user != null) {
            Map<String, Integer> historyViews = user.getHistory();

            String titleSeason = title + " " + command.getSeasonNumber();

            // check if the user saw the title
            // if seen, check if it is already rated
            if (historyViews.containsKey(title) || (historyViews.containsKey(titleSeason))) {
                Map<String, Double> historyRatings = user.getHistoryRatings();

                // if already rated (film or season)
                if ((historyRatings.containsKey(title))
                        || (historyRatings.containsKey(titleSeason))) {
                    message = "error -> " + title + " has been already rated";
                } else {
                    // case film
                    if (command.getSeasonNumber() == 0) {
                        // add to seen films
                        historyRatings.put(title, command.getGrade());
                        // no we have to add the rating of the film
                        List<MovieData> newMovies = input.getMovies();

                        for (MovieData movie : newMovies) {
                            if (movie.getTitle().equals(title)) {
                                movie.setRating(command.getGrade());
                            }
                        }
                    } else {
                        // case serial
                        historyRatings.put(titleSeason, command.getGrade());
                        List<SerialData> serials = input.getSerials();

                        for (SerialData serial : serials) {
                            if (serial.getTitle().equals(title)) {
                                ArrayList<SeasonData> seasons = serial.getSeasons();
                                // get the current season (for user count starts from 1)
                                // this is why we subtract 1 to get the desired season
                                SeasonData gradedSeason =
                                        seasons.get(command.getSeasonNumber() - 1);
                                gradedSeason.modifiyRating(command.getGrade());
                                serial.modifyGrade();
                            }
                        }
                    }
                    message = "success -> " + title + " was rated with " + command.getGrade()
                            + " by " + username;
                }
            } else {
                message = "error -> " + title + " is not seen";
            }
        }
        return fileWriter.writeFile(commandId, field, message);
    }
}
