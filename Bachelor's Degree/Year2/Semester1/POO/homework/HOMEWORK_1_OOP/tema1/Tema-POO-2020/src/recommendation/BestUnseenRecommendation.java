package recommendation;

import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import newfileio.MovieData;
import newfileio.SerialData;
import newfileio.UserData;
import org.json.simple.JSONObject;
import solution.Name2Number;
import solution.SortName2Number;
import utils.PseudoGetters;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class BestUnseenRecommendation {

    private final Input input;
    private final Writer fileWriter;

    public BestUnseenRecommendation(final Input input, final Writer fileWriter) {
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current bestUnseen recommendation
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
            List<Name2Number> moviesUnseen = new ArrayList<>();
            int crtPos = 0;

            for (MovieData movie : moviesDatabase) {
                if (!nameFilmsViewed.contains(movie.getTitle())) {
                    movieSeen = false;
                    moviesUnseen.add(new Name2Number(movie.getTitle(),
                            movie.getRating(), (double) crtPos));
                    crtPos++;
                }
            }
            List<SerialData> serialDatabase = input.getSerials();
            for (SerialData serial : serialDatabase) {
                if (!nameFilmsViewed.contains(serial.getTitle())) {
                    movieSeen = false;
                    moviesUnseen.add(new Name2Number(serial.getTitle(),
                            serial.getGrade(), (double) crtPos));
                }
            }
            if (movieSeen) {
                // user saw all movies if enter here
                int actionId = recommendation.getActionId();
                String field = "";
                String message = "BestRatedUnseenRecommendation cannot be applied!";
                return fileWriter.writeFile(actionId, field, message);
            }
            moviesUnseen.sort(new SortName2Number());
            int actionId = recommendation.getActionId();
            String field = "";
            String message = "BestRatedUnseenRecommendation result: ";
            message += moviesUnseen.get(0).getName();
            return fileWriter.writeFile(actionId, field, message);
        }
        return null;
    }
}
