package newfileio;

import fileio.SerialInputData;
import fileio.ShowInput;

import java.util.ArrayList;
import java.util.List;

/**
 * New information about a tv show, retrieved from parsing the input test files
 * <p>
 * DO NOT MODIFY
 */
public final class SerialData extends ShowInput {
    /**
     * Number of seasons
     */
    private final int numberOfSeasons;
    /**
     * Season list
     */
    private final ArrayList<SeasonData> seasons;
    /**
     * Grade of a serial
     */
    private Double grade;
    /**
     * Number of ratings for the serial (sum of number of ratings for the seasons)
     */
    private Integer noRatings;
    /**
     * Duration of serial (sum of durations for every season)
     */
    private final int duration;

    public SerialData(final String title, final ArrayList<String> cast,
                      final ArrayList<String> genres,
                      final int numberOfSeasons, final ArrayList<SeasonData> seasons,
                      final int year) {
        super(title, year, cast, genres);
        this.numberOfSeasons = numberOfSeasons;
        this.seasons = seasons;
        this.grade = (double) 0;
        this.noRatings = 0;
        this.duration = calculateDuration();
    }

    /**
     * Adds the rating field for the serials
     * @param serials represents a list with the serials in old format
     * @return a new list with the rating field in it
     */
    public static List<SerialData> addRating(final List<SerialInputData> serials) {
        List<SerialData> newSerials = new ArrayList<>();
        for (SerialInputData serial : serials) {
            ArrayList<SeasonData> newSeasons = SeasonData.addRating(serial);

            SerialData newserial =
                    new SerialData(serial.getTitle(), serial.getCast(), serial.getGenres(),
                            serial.getNumberSeason(), newSeasons, serial.getYear());
            newSerials.add(newserial);
        }
        return newSerials;
    }

    /**
     * Mofies the grade for a given serial considering
     * the ratings of the seasons composing it
     */
    public void modifyGrade() {
        Double sum = (double) 0;
        for (SeasonData season : this.getSeasons()) {
            sum += season.getRating();
        }
        this.grade = sum / this.getNumberOfSeasons();
        // also modify the number of ratings
        this.noRatings++;
    }


    /**
     * Calculate the duration of a serial
     * @return the duration of the serial
     */
    public int calculateDuration() {
        int durationSerial = 0;
        for (SeasonData season : this.getSeasons()) {
            durationSerial += season.getDuration();
        }
        return durationSerial;
    }

    public int getDuration() {
        return duration;
    }

    public int getNumberSeason() {
        return numberOfSeasons;
    }

    public ArrayList<SeasonData> getSeasons() {
        return seasons;
    }

    public int getNumberOfSeasons() {
        return numberOfSeasons;
    }

    public Double getGrade() {
        return grade;
    }

    public Integer getNoRatings() {
        return noRatings;
    }

    @Override
    public String toString() {
        return "SerialInputData{" + " title= "
                + super.getTitle() + " " + " year= "
                + super.getYear() + " cast {"
                + super.getCast() + " }\n" + " genres {"
                + super.getGenres() + " }\n "
                + " numberSeason= " + numberOfSeasons
                + ", seasons=" + seasons + "\n\n" + '}';
    }
}
