package queries;

import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import newfileio.MovieData;
import org.json.simple.JSONObject;
import solution.NameNumber;
import utils.Utils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * The class contains method that solves a query
 * of type "Movie" with criteria "longest"
 */
public class MovieLongestQuery {

    private final Input input;
    private final Writer fileWriter;

    public MovieLongestQuery(final Input input, final Writer fileWriter) {
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
        String desiredYear = query.getFilters().get(0).get(0);
        String desiredGenre = query.getFilters().get(1).get(0);

        List<MovieData> movies = input.getMovies();
        List<NameNumber> filmsData = new ArrayList<>();
        for (MovieData movie : movies) {
            Utils.filterFilmsDuration(movie, null, filmsData,
                    desiredYear, desiredGenre);
        }
        return fileWriter.writeFile(query.getActionId(),
                "", Utils.buildFilms(query, filmsData));
    }
}
