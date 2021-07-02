package utils;

import actor.ActorsAwards;
import common.Constants;
import entertainment.Genre;
import fileio.ActionInputData;
import newfileio.Input;
import newfileio.MovieData;
import newfileio.SerialData;
import newfileio.UserData;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import solution.Name2Number;
import solution.NameNumber;
import solution.SortAscNo;
import solution.SortDescNo;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.ListIterator;
import java.util.Map;

/**
 * The class contains static methods that helps with parsing.
 * <p>
 * We suggest you add your static methods here or in a similar class.
 */
public final class Utils {
    /**
     * for coding style
     */
    private Utils() {
    }

    /**
     * Transforms a string into an enum
     *
     * @param genre of video
     * @return an Genre Enum
     */
    public static Genre stringToGenre(final String genre) {
        return switch (genre.toLowerCase()) {
            case "action" -> Genre.ACTION;
            case "adventure" -> Genre.ADVENTURE;
            case "drama" -> Genre.DRAMA;
            case "comedy" -> Genre.COMEDY;
            case "crime" -> Genre.CRIME;
            case "romance" -> Genre.ROMANCE;
            case "war" -> Genre.WAR;
            case "history" -> Genre.HISTORY;
            case "thriller" -> Genre.THRILLER;
            case "mystery" -> Genre.MYSTERY;
            case "family" -> Genre.FAMILY;
            case "horror" -> Genre.HORROR;
            case "fantasy" -> Genre.FANTASY;
            case "science fiction" -> Genre.SCIENCE_FICTION;
            case "action & adventure" -> Genre.ACTION_ADVENTURE;
            case "sci-fi & fantasy" -> Genre.SCI_FI_FANTASY;
            case "animation" -> Genre.ANIMATION;
            case "kids" -> Genre.KIDS;
            case "western" -> Genre.WESTERN;
            case "tv movie" -> Genre.TV_MOVIE;
            default -> null;
        };
    }

    /**
     * Transforms a string into an enum
     *
     * @param award for actors
     * @return an ActorsAwards Enum
     */
    public static ActorsAwards stringToAwards(final String award) {
        return switch (award) {
            case "BEST_SCREENPLAY" -> ActorsAwards.BEST_SCREENPLAY;
            case "BEST_SUPPORTING_ACTOR" -> ActorsAwards.BEST_SUPPORTING_ACTOR;
            case "BEST_DIRECTOR" -> ActorsAwards.BEST_DIRECTOR;
            case "BEST_PERFORMANCE" -> ActorsAwards.BEST_PERFORMANCE;
            case "PEOPLE_CHOICE_AWARD" -> ActorsAwards.PEOPLE_CHOICE_AWARD;
            default -> null;
        };
    }

    /**
     * Transforms an array of JSON's into an array of strings
     *
     * @param array of JSONs
     * @return a list of strings
     */
    public static ArrayList<String> convertJSONArray(final JSONArray array) {
        if (array != null) {
            ArrayList<String> finalArray = new ArrayList<>();
            for (Object object : array) {
                finalArray.add((String) object);
            }
            return finalArray;
        } else {
            return null;
        }
    }

    /**
     * Transforms an array of JSON's into a map
     *
     * @param jsonActors array of JSONs
     * @return a map with Actors Awards as key and Integer as value
     */
    public static Map<ActorsAwards, Integer> convertAwards(final JSONArray jsonActors) {
        Map<ActorsAwards, Integer> awards = new LinkedHashMap<>();

        for (Object iterator : jsonActors) {
            awards.put(stringToAwards((String) ((JSONObject) iterator).get(Constants.AWARD_TYPE)),
                    Integer.parseInt(((JSONObject) iterator).get(Constants.NUMBER_OF_AWARDS)
                            .toString()));
        }

        return awards;
    }

    /**
     * Transforms an array of JSON's into a map
     *
     * @param movies array of JSONs
     * @return a map with String as key and Integer as value
     */
    public static Map<String, Integer> watchedMovie(final JSONArray movies) {
        Map<String, Integer> mapVideos = new LinkedHashMap<>();

        if (movies != null) {
            for (Object movie : movies) {
                mapVideos.put((String) ((JSONObject) movie).get(Constants.NAME),
                        Integer.parseInt(((JSONObject) movie).get(Constants.NUMBER_VIEWS)
                                .toString()));
            }
        } else {
            System.out.println("NO VIDEO SEEN");
        }

        return mapVideos;
    }

    /**
     * Gives the message containing the names of the final Films
     * sorted by the number field in the class
     *
     * @param query    represents the current query
     * @param filmFavs creates a correspondence from name to a number
     * @return a message giving the name of the films desired
     */
    public static String sortFilms(final ActionInputData query,
                                   final Map<String, Integer> filmFavs) {
        List<NameNumber> filmsData = new ArrayList<>();
        for (Map.Entry<String, Integer> filmFav : filmFavs.entrySet()) {
            filmsData.add(new NameNumber(filmFav.getKey(), (double) filmFav.getValue()));
        }
        return buildFilms(query, filmsData);
    }

    /**
     * sorts the query and build the final films' names
     *
     * @param query represents the current query
     * @param filmsData creates a correspondence from name to a number
     * @return a message giving the name of the films
     */
    public static String buildFilms(final ActionInputData query, final List<NameNumber> filmsData) {
        if (query.getSortType().equals("desc")) {
            filmsData.sort(new SortDescNo());
        } else {
            filmsData.sort(new SortAscNo());
        }
        int noFilms = query.getNumber();
        List<String> finalFilms = new ArrayList<>();
        int i = 0;
        while ((i < noFilms) && (i < filmsData.size())) {
            finalFilms.add(filmsData.get(i).getName());
            i++;
        }
        String message = "";
        message += "Query result: ";
        message += finalFilms.toString();
        return message;
    }

    /**
     * Checks if user has standard account
     *
     * @param username from any other class
     * @param input    from main
     * @return true if he has and false otherwise
     */
    public static boolean rightsStandard(final String username, final Input input) {
        UserData user = PseudoGetters.getUser(username, input);
        if (user != null) {
            return !user.getSubscriptionType().equals("PREMIUM");
        }
        // no user existent in database or standard account

        return true;
    }

    /**
     * Calculate the number of views for a specific film
     *
     * @param searchedFilmName represents the name of the searched film
     * @param input            from main
     * @return number of views for the film
     */
    public static Integer getNoViews(final String searchedFilmName, final Input input) {
        List<UserData> users = input.getUsers();

        Integer noViews = 0;
        for (UserData user : users) {
            Map<String, Integer> userHistory = user.getHistory();
            for (Map.Entry<String, Integer> entry : userHistory.entrySet()) {
                if (entry.getKey().equals(searchedFilmName)) {
                    noViews += entry.getValue();
                    break;
                }
            }
        }
        return noViews;
    }

    /**
     * Check if the film was seen or not by the user
     *
     * @param filmName represents the name of the film
     * @param input    from main
     * @return false if movie was seen and true otherwise
     */
    public static boolean wasMovieSeen(final String filmName, final String username,
                                       final Input input) {
        UserData user = PseudoGetters.getUser(username, input);
        if (user != null) {
            Map<String, Integer> historyViews = user.getHistory();
            for (Map.Entry<String, Integer> crtFilm : historyViews.entrySet()) {
                if (crtFilm.getKey().equals(filmName)) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Calculate times a movie was added to favorite by a user
     *
     * @param movieTitle represents the title of the film
     * @param input      from main
     * @return false if movie was seen and true otherwise
     */
    public static Integer timesFav(final String movieTitle, final String username,
                                   final Input input) {
        List<UserData> users = input.getUsers();
        Integer noFavs = 0;
        for (UserData user : users) {
            if ((user.getFavoriteMovies().contains(movieTitle))
                    && (!user.getUsername().equals(username))) {
                noFavs++;
            }
        }
        return noFavs;
    }

    /**
     * Remove seen films from List of favorite
     *
     * @param filmsFav represents the films added to favorite by the user
     * @param input    from main
     */
    public static void removeSeenFilms(final List<Name2Number> filmsFav,
                                       final String username, final Input input) {
        UserData user = PseudoGetters.getUser(username, input);
        if (user != null) {
            Map<String, Integer> userHistory = user.getHistory();
            ListIterator<Name2Number> iter = filmsFav.listIterator();
            while (iter.hasNext()) {
                Name2Number crtFavFilm = iter.next();
                for (Map.Entry<String, Integer> crtFilm : userHistory.entrySet()) {
                    // if movie found in the list of views of the user
                    // remove object from the filmFav
                    if (crtFavFilm.getName().equals(crtFilm.getKey())) {
                        iter.remove();
                    }
                }
            }
        }
    }

    /**
     * filterFilms after:
     * @param desiredYear, year selected by user
     * @param desiredGenre, genre selected by user
     *
     * @param movie can be either null, either an instance
     * @param serial can be either null either an instance
     *
     * and fill @param filmsData with name of the film and it's rating
     */
    public static void filterFilmsRating(final MovieData movie, final SerialData serial,
                                         final List<NameNumber> filmsData,
                                   final String desiredYear, final String desiredGenre) {
        if (serial != null) {
            // if no ratings were given check next serial
            if (checkDesired(desiredYear, desiredGenre, serial.getNoRatings(), serial.getYear(),
                    serial.getGenres(), true)) {
                return;
            }
            filmsData.add(new NameNumber(serial.getTitle(), serial.getGrade()));
        } else if (movie != null) {
            // if no ratings were given check next serial
            if (checkDesired(desiredYear, desiredGenre, movie.getNoRatings(), movie.getYear(),
                    movie.getGenres(), true)) {
                return;
            }
            filmsData.add(new NameNumber(movie.getTitle(), movie.getRating()));
        }
    }


    /**
     * filterFilms after:
     * @param desiredYear, year selected by user
     * @param desiredGenre, genre selected by user
     *
     * @param movie can be either null, either an instance
     * @param serial can be either null either an instance
     *
     * and fill @param filmsData with name of the film and it's duration
     */
    public static void filterFilmsDuration(final MovieData movie, final SerialData serial,
                                           final List<NameNumber> filmsData,
                                   final String desiredYear, final String desiredGenre) {
        if (serial != null) {
            // if no ratings were given check next serial
            if (checkDesired(desiredYear, desiredGenre, serial.getNoRatings(), serial.getYear(),
                    serial.getGenres(), false)) {
                return;
            }
            filmsData.add(new NameNumber(serial.getTitle(), (double) serial.getDuration()));
        } else if (movie != null) {
            // if no ratings were given check next serial
            if (checkDesired(desiredYear, desiredGenre, movie.getNoRatings(), movie.getYear(),
                    movie.getGenres(), false)) {
                return;
            }
            filmsData.add(new NameNumber(movie.getTitle(), (double) movie.getDuration()));
        }
    }

    /**
     * filterFilms after:
     * @param desiredYear, year selected by user
     * @param desiredGenre, genre selected by user
     * and checks if
     * @param interestedRating is true, as there are situations
     * when films must have been added to favorite
     * and fill @param filmsData with name of the film and it's duration
     */
    private static boolean checkDesired(final String desiredYear, final String desiredGenre,
                                        final Integer noRatings,
                                        final int year, final ArrayList<String> genres,
                                        final boolean interestedRating) {
        if (interestedRating) {
            if (noRatings == 0) {
                return true;
            }
        }
        if (!(desiredYear == null)) {
            // if year differs, check next serial
            if (!(year == Integer.parseInt(desiredYear))) {
                return true;
            }
        }
        if (!(desiredGenre == null)) {
            // if genres of serial do not contain genre of searched serial
            // check next serial
            return !(genres.contains(desiredGenre));
        }
        return false;
    }
}
