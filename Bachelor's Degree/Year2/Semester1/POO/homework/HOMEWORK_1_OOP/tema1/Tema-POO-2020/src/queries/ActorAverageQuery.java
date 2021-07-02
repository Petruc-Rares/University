package queries;

import fileio.ActionInputData;
import fileio.ActorInputData;
import fileio.Writer;
import newfileio.Input;
import newfileio.MovieData;
import newfileio.SerialData;
import org.json.simple.JSONObject;
import solution.NameNumber;
import utils.PseudoGetters;
import utils.Utils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * The class contains method that solves a query
 * of type "Actors" with criteria "Average"
 */
public class ActorAverageQuery {

    private final Input input;
    private final Writer fileWriter;

    public ActorAverageQuery(final Input input, final Writer fileWriter) {
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current query
     *
     * @param query represents the current query
     * @return JSONObject in the desired format
     * @throws IOException in case of exceptions to reading / writing
     */
    public JSONObject solveQuery(final ActionInputData query) throws IOException {
        List<ActorInputData> actors = input.getActors();
        List<NameNumber> actorsData = new ArrayList<>();

        // for every actor we calculate the rating of the films he played in
        for (ActorInputData actor : actors) {
            ArrayList<String> actorFilms = actor.getFilmography();
            Double ratingActor = (double) 0;
            double noRatings = 0;
            for (String film : actorFilms) {
                MovieData movie = PseudoGetters.getMovie(film, input);
                // if movie found
                if (movie != null) {
                    if (movie.getNoRatings() == 0) {
                        continue;
                    }
                    ratingActor += movie.getRating();
                    noRatings++;
                    continue; // check next film
                }
                SerialData serial = PseudoGetters.getSerial(film, input);
                // if serial found
                if (serial != null) {
                    if (serial.getNoRatings() == 0) {
                        continue;
                    }
                    ratingActor += serial.getGrade();
                    noRatings++;
                }
                // if not found check next film
            }
            // if found some ratings
            if (noRatings != 0) {
                NameNumber actorData =
                        new NameNumber(actor.getName(), ratingActor / noRatings);
                actorsData.add(actorData);
            }
        }

        // now sort
        return fileWriter.writeFile(query.getActionId(),
                "", Utils.buildFilms(query, actorsData));
    }
}
