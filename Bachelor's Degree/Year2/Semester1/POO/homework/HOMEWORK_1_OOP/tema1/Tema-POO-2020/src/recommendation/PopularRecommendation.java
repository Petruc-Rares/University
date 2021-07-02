package recommendation;

import entertainment.Genre;
import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import newfileio.MovieData;
import newfileio.SerialData;
import org.json.simple.JSONObject;
import solution.NameNumber;
import solution.Solver;
import utils.Utils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class PopularRecommendation {

    private final Input input;
    private final Writer fileWriter;

    public PopularRecommendation(final Input input, final Writer fileWriter) {
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current Popular recommendation
     *
     * @param recommendation is the current recommendation
     * @return an JsonObject with the desired fields
     * @throws IOException in case of exceptions to reading / writing
     */
    public JSONObject solveRecommendation(final ActionInputData recommendation) throws IOException {

        if (Utils.rightsStandard(recommendation.getUsername(), input)) {
            int actionId = recommendation.getActionId();
            String field = "";
            String message = "PopularRecommendation cannot be applied!";
            return fileWriter.writeFile(actionId, field, message);
        }
        Map<Genre, List<NameNumber>> genreFilms = new HashMap<>();
        List<MovieData> moviesDatabase = input.getMovies();
        for (MovieData movie : moviesDatabase) {
            filterGenres(genreFilms, movie.getGenres(), movie.getTitle());
        }
        List<SerialData> serialsDatabase = input.getSerials();
        for (SerialData serial : serialsDatabase) {
            filterGenres(genreFilms, serial.getGenres(), serial.getTitle());
        }
        // map contains the genres from the films in the database

        Map<Genre, List<NameNumber>> sortedMap = genreFilms.entrySet()
                .stream()
                .sorted(Map.Entry.comparingByValue(Solver::compare))
                .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue,
                        (e1, e2) -> e1, LinkedHashMap::new));

        int noGenres = 0;
        String filmRecommendation = "";
        for (Map.Entry<Genre, List<NameNumber>> entry : sortedMap.entrySet()) {
            List<NameNumber> currentFilms = entry.getValue();
            int noMovies = 0;
            for (NameNumber currentFilm : currentFilms) {
                if (Utils.wasMovieSeen(currentFilm.getName(),
                        recommendation.getUsername(), input)) {
                    filmRecommendation = currentFilm.getName();
                    break;
                }
                noMovies++;
            }
            if (noMovies < currentFilms.size()) {
                break;
            }
            noGenres++;
        }
        if (noGenres == sortedMap.size()) {
            int actionId = recommendation.getActionId();
            String field = "";
            String message = "PopularRecommendation cannot be applied!";
            return fileWriter.writeFile(actionId, field, message);
        } else {
            int actionId = recommendation.getActionId();
            String field = "";
            String message = "PopularRecommendation result: ";
            message += filmRecommendation;
            return fileWriter.writeFile(actionId, field, message);
        }
    }

    private void filterGenres(final Map<Genre, List<NameNumber>> genreFilms,
                              final ArrayList<String> genres2,
                              final String title) {
        genres2.stream().map(Utils::stringToGenre).forEach(genre -> {
            if (genreFilms.containsKey(genre)) {
                List<NameNumber> filmsGenre = genreFilms.get(genre);
                NameNumber newFilmCategory =
                        new NameNumber(title,
                                (double) Utils.getNoViews(title,
                                input));
                filmsGenre.add(newFilmCategory);
            } else {
                NameNumber newFilmCategory =
                        new NameNumber(title,
                                (double) Utils.getNoViews(title,
                                input));
                List<NameNumber> filmsGenre = new ArrayList<>();
                filmsGenre.add(newFilmCategory);
                genreFilms.put(genre, filmsGenre);
            }
        });
    }
}
