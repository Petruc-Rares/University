package recommendation;

import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import newfileio.MovieData;
import newfileio.SerialData;
import org.json.simple.JSONObject;
import solution.NameNumber;
import solution.SortAscNo;
import utils.Utils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class SearchRecommendation {

    private final Input input;
    private final Writer fileWriter;

    public SearchRecommendation(final Input input, final Writer fileWriter) {
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current search recommendation
     *
     * @param recommendation is the current recommendation
     * @return an JsonObject with the desired fields
     * @throws IOException in case of exceptions to reading / writing
     */
    public JSONObject solveRecommendation(final ActionInputData recommendation) throws IOException {
        if (Utils.rightsStandard(recommendation.getUsername(), input)) {
            int actionId = recommendation.getActionId();
            String field = "";
            String message = "SearchRecommendation cannot be applied!";
            return fileWriter.writeFile(actionId, field, message);
        }
        List<MovieData> movies = input.getMovies();
        List<SerialData> serials = input.getSerials();
        List<NameNumber> genreFilms = new ArrayList<>();
        for (MovieData movie : movies) {
            if (movie.getGenres().contains(recommendation.getGenre())) {
                genreFilms.add(new NameNumber(movie.getTitle(), movie.getRating()));
            }
        }
        for (SerialData serial : serials) {
            if (serial.getGenres().contains(recommendation.getGenre())) {
                genreFilms.add(new NameNumber(serial.getTitle(), serial.getGrade()));
            }
        }
        List<NameNumber> finalFilms = new ArrayList<>();
        for (NameNumber film : genreFilms) {
            if (Utils.wasMovieSeen(film.getName(), recommendation.getUsername(), input)) {
                finalFilms.add(film);
            }
        }
        finalFilms.sort(new SortAscNo());
        String message;
        if (finalFilms.size() != 0) {
            message = "SearchRecommendation result: ";
            message += finalFilms;
        } else {
            message = "SearchRecommendation cannot be applied!";
        }
        return fileWriter.writeFile(recommendation.getActionId(), "", message);
    }
}
