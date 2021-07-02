package queries;

import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import newfileio.SerialData;
import newfileio.UserData;
import org.json.simple.JSONObject;
import utils.PseudoGetters;
import utils.Utils;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * The class contains method that solves a query
 * of type "Show" with criteria "most_viewed"
 */
public class ShowMstVwQuery {
    private final Input input;
    private final Writer fileWriter;

    public ShowMstVwQuery(final Input input, final Writer fileWriter) {
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

        Map<String, Integer> filmsViewed = new HashMap<>();

        for (UserData user : users) {
            Map<String, Integer> viewedMovies = user.getHistory();
            for (Map.Entry<String, Integer> entry : viewedMovies.entrySet()) {
                SerialData serial = PseudoGetters.getSerial(entry.getKey(), input);
                // if serial found
                if (!(serial == null)) {
                    // now check if it has the genre and year desired
                    // if desiredYear is specified
                    if (!(desiredYear == null)) {
                        // if year differs, check next serial
                        if (!(serial.getYear() == Integer.parseInt(desiredYear))) {
                            continue;
                        }
                    }
                    if (!(desiredGenre == null)) {
                        // if genres of serial do not contain genre of searched serial
                        // continue
                        if (!(serial.getGenres().contains(desiredGenre))) {
                            continue;
                        }
                    }

                    // increment number of times the film
                    // has been added to favourites
                    if (filmsViewed.containsKey(serial.getTitle())) {
                        filmsViewed.put(serial.getTitle(),
                                filmsViewed.get(serial.getTitle()) + entry.getValue());
                    } else {
                        filmsViewed.put(serial.getTitle(), entry.getValue());
                    }
                }
            }
        }
        String message =  Utils.sortFilms(query, filmsViewed);
        return fileWriter.writeFile(query.getActionId(), "", message);
    }
}
