package recommendation;

import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import newfileio.MovieData;
import newfileio.SerialData;
import org.json.simple.JSONObject;
import solution.Name2Number;
import solution.SortName2Number;
import utils.Utils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class FavoriteRecommendation {

    private final Input input;
    private final Writer fileWriter;

    public FavoriteRecommendation(final Input input, final Writer fileWriter) {
        this.input = input;
        this.fileWriter = fileWriter;
    }
    /**
     * Solve the current favorite recommendation
     *
     * @param recommendation is the current recommendation
     * @return an JsonObject with the desired fields
     * @throws IOException in case of exceptions to reading / writing
     */
    public JSONObject solveRecommendation(final ActionInputData recommendation) throws IOException {
        List<MovieData> moviesDatabase = input.getMovies();
        List<Name2Number> filmsFav = new ArrayList<>();
        int crtPos = 0;
        for (MovieData movie :moviesDatabase) {
            Integer noFavs = Utils.timesFav(movie.getTitle(), recommendation.getUsername(), input);
            // if no added to favorite check next movie
            if (noFavs == 0) {
                crtPos++;
                continue;
            } else {
                Name2Number filmFavorite =
                        new Name2Number(movie.getTitle(), (double) noFavs, (double) crtPos);
                filmsFav.add(filmFavorite);
            }
            crtPos++;
        }

        List<SerialData> serialsDatabase = input.getSerials();
        for (SerialData serial :serialsDatabase) {
            Integer noFavs = Utils.timesFav(serial.getTitle(), recommendation.getUsername(), input);
            // if no added to favorite check next movie
            if (noFavs == 0) {
                crtPos++;
                continue;
            } else {
                Name2Number filmFavorite =
                        new Name2Number(serial.getTitle(), (double) noFavs,
                                (double) crtPos);
                filmsFav.add(filmFavorite);
            }
            crtPos++;
        }

        // check if the user saw any of the films
        Utils.removeSeenFilms(filmsFav, recommendation.getUsername(), input);

        filmsFav.sort(new SortName2Number());

        int actionId = recommendation.getActionId();
        String field = "";
        String message;
        if (filmsFav.size() != 0) {
            message = "";
            message += "FavoriteRecommendation result: ";
            message += filmsFav.get(0).getName();

        } else {
            message = "FavoriteRecommendation cannot be applied!";
        }
        return fileWriter.writeFile(actionId, field, message);
    }
}
