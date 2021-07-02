package newfileio;

import fileio.SerialInputData;

import java.util.ArrayList;
import java.util.List;

/**
 * New Information about a season of a tv show
 * <p>
 * DO NOT MODIFY
 */
public final class SeasonData {
    /**
     * Number of current season
     */
    private final int currentSeason;
    /**
     * Duration in minutes of a season
     */
    private int duration;
    /**
     * List of ratings for each season
     */
    private List<Double> ratings;

    /**
     * List of ratings for each season
     */
    private Double rating;
    /**
     * important aux in calculating the new rating
     */
    private Double previousSum;

    public SeasonData(final int currentSeason, final int duration) {
        this.currentSeason = currentSeason;
        this.duration = duration;
        this.ratings = new ArrayList<>();
        this.rating = (double) 0;
        this.previousSum = (double) 0;
    }

    /**
     * Adds the rating field for the seasons in a serial
     * @param seasons represents a list with the seasons in old format
     * @return a new list with the rating field in it
     */
    public static ArrayList<SeasonData> addRating(final SerialInputData seasons) {
        ArrayList<SeasonData> newSeasons = new ArrayList<>();
        for (int i = 0; i < seasons.getNumberSeason(); i++) {
            SeasonData newseason = new SeasonData(i + 1,
                    seasons.getSeasons().get(i).getDuration());
            newSeasons.add(newseason);
        }
        return newSeasons;
    }

    /**
     * List of ratings for each season
     */
    public void modifiyRating(final Double userRating) {
        previousSum += userRating;

        // increase the number of ratings and add to the arraylist
        ratings.add(userRating);

        // establish the new Rating
        rating = previousSum / ratings.size();
    }

    public int getDuration() {
        return duration;
    }

    public void setDuration(final int duration) {
        this.duration = duration;
    }

    public List<Double> getRatings() {
        return ratings;
    }

    public void setRatings(final List<Double> ratings) {
        this.ratings = ratings;
    }

    public Double getRating() {
        return rating;
    }

    @Override
    public String toString() {
        return "Episode{"
                + "currentSeason="
                + currentSeason
                + ", duration="
                + duration
                + '}';
    }
}

