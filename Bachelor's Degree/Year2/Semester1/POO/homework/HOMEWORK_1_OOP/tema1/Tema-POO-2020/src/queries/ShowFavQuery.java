package queries;

import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import newfileio.SerialData;
import newfileio.UserData;
import org.json.simple.JSONObject;
import utils.PseudoGetters;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static utils.Utils.sortFilms;

/**
 * The class contains method that solves a query
 * of type "Show" with criteria "favorite"
 */
public class ShowFavQuery {

    private final Input input;
    private final Writer fileWriter;

    public ShowFavQuery(final Input input, final Writer fileWriter) {
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
        List<UserData> users = input.getUsers();

        Map<String, Integer> filmFavs = new HashMap<>();
        for (UserData user : users) {
            ArrayList<String> favMoviesUser = user.getFavoriteMovies();
            for (String favMovieUser : favMoviesUser) {
                SerialData movie = PseudoGetters.getSerial(favMovieUser, input);
                // if movie found
                if (!(movie == null)) {
                    // now check if it has the genre and year desired
                    // if desiredYear is specified
                    if (!(desiredYear == null)) {
                        // if year differs, check next movie
                        if (!(movie.getYear() == Integer.parseInt(desiredYear))) {
                            continue;
                        }
                    }
                    if (!(desiredGenre == null)) {
                        // if genres of movie do not contain genre of searched movie
                        // continue
                        if (!(movie.getGenres().contains(desiredGenre))) {
                            continue;
                        }
                    }

                    // increment number of times the film
                    // has been added to favourites
                    if (filmFavs.containsKey(favMovieUser)) {
                        filmFavs.put(favMovieUser, filmFavs.get(favMovieUser) + 1);
                    } else {
                        filmFavs.put(favMovieUser, 1);
                    }
                }
            }
        }
        String message = sortFilms(query, filmFavs);
        return fileWriter.writeFile(query.getActionId(), "", message);
    }
}
