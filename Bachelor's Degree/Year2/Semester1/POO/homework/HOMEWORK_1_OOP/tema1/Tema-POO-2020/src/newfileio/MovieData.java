package newfileio;

import fileio.MovieInputData;

import java.util.ArrayList;
import java.util.List;

/**
 * The class contains new information
 * about a movie, including rating
 */
public class MovieData {

    private final String title;
    private final ArrayList<String> cast;
    private final ArrayList<String> genres;
    private final int year;
    /**
     * Duration in minutes of a season
     */
    private final int duration;
    private Double rating = (double) 0;
    private Integer noRatings;

    public MovieData(final String title, final ArrayList<String> cast,
                     final ArrayList<String> genres, final int year,
                     final int duration) {
        this.title = title;
        this.cast = cast;
        this.genres = genres;
        this.year = year;
        this.duration = duration;
        this.noRatings = 0;
    }

    /**
     * Adds the rating field for the movies in the input from fileio
     * @param movies represents a list with movies in old format
     * @return a new list with the rating field in it
     */
    public static List<MovieData> addRatingField(final List<MovieInputData> movies) {
        List<MovieData> movies2 = new ArrayList<>();
        for (MovieInputData movie : movies) {
            MovieData movieAux = new MovieData(movie.getTitle(), movie.getCast(),
                    movie.getGenres(), movie.getYear(), movie.getDuration());
            movies2.add(movieAux);
        }
        return movies2;
    }

    /**
     * Sets the new rating and modifies the number of ratings
     * for the given movie
     * @param rating represents the rating given by the user
     */
    public final void setRating(final Double rating) {

        this.rating *= this.noRatings;

        this.rating += rating;
        // change the number of current ratings
        this.noRatings++;

        this.rating /= this.noRatings;
    }

    /**
     * Prints in a human readable format information about movie
     */
    @Override
    public String toString() {
        return "MovieInputData{" + "title= "
                + this.getTitle() + "year= "
                + this.getYear() + "duration= "
                + duration + "cast {"
                + this.getCast() + " }\n"
                + "genres {" + this.getGenres() + " }\n ";
    }

    public final String getTitle() {
        return title;
    }

    public final ArrayList<String> getCast() {
        return cast;
    }

    public final ArrayList<String> getGenres() {
        return genres;
    }

    public final int getYear() {
        return year;
    }

    public final Double getRating() {
        return rating;
    }

    public final Integer getNoRatings() {
        return noRatings;
    }

    public final int getDuration() {
        return duration;
    }

    public final void setNoRatings(Integer noRatings) {
        this.noRatings = noRatings;
    }
}
