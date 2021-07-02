package recommendation;

import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import newfileio.MovieData;
import newfileio.SerialData;
import newfileio.UserData;
import org.json.simple.JSONObject;
import utils.PseudoGetters;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * The class contains method that solves a recommendation
 * with standard strategy
 */
public class StandardRecommendation {

    private final Input input;
    private final Writer fileWriter;

    public StandardRecommendation(final Input input, final Writer fileWriter) {
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current standard recommendation
     *
     * @param recommendation is the current recommendation
     * @return an JsonObject with the desired fields
     * @throws IOException in case of exceptions to reading / writing
     */
    public JSONObject solveRecommendation(final ActionInputData recommendation) throws IOException {
        String username = recommendation.getUsername();

        UserData user = PseudoGetters.getUser(username, input);
        if (user != null) {
            Map<String, Integer> userViews = user.getHistory();
            List<String> nameFilmsViewed = new ArrayList<>();
            for (Map.Entry<String, Integer> filmViews : userViews.entrySet()) {
                nameFilmsViewed.add(filmViews.getKey());
            }
            // now we have the films the user saw in nameFilmsViewed
            List<MovieData> moviesDatabase = input.getMovies();
            boolean movieSeen = true;
            String nameMovieUnseen = "";

            for (MovieData movie : moviesDatabase) {
                if (!nameFilmsViewed.contains(movie.getTitle())) {
                    movieSeen = false;
                    nameMovieUnseen = movie.getTitle();
                    break;
                }
            }
            // saw all the movies in the moviesDatabase
            // so check the serial
            if (movieSeen) {
                List<SerialData> serialDatabase = input.getSerials();
                for (SerialData serial : serialDatabase) {
                    if (!nameFilmsViewed.contains(serial.getTitle())) {
                        movieSeen = false;
                        nameMovieUnseen = serial.getTitle();
                        break;
                    }
                }
                // if still seen al the movies
                if (movieSeen) {
                    int actionId = recommendation.getActionId();
                    String field = "";
                    String message = "StandardRecommendation cannot be applied!";
                    return fileWriter.writeFile(actionId, field, message);
                }
            }
            int actionId = recommendation.getActionId();
            String field = "";
            String message = "";
            message += "StandardRecommendation result: ";
            message += nameMovieUnseen;
            return fileWriter.writeFile(actionId, field, message);
        }
        return null;
    }
}
